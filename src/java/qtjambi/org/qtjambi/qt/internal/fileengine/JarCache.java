/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.internal.fileengine;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.jar.JarEntry;
import java.util.zip.ZipException;

class JarCache {
    // Must only be called with lock.writeLock() held.
    private static void add(HashMap<String, List<String>> lookupCache, String dirName, String pathToJarFile) {
        List<String> files = lookupCache.get(dirName);
        if (files == null) {
            files = new ArrayList<String>();
            files.add(pathToJarFile);
            lookupCache.put(dirName, files);
        } else {
            // CHECKME: Why are we checking for the instance ?  not the the file/path ?  TRY A FIX NOW
            if (!files.contains(pathToJarFile))
                files.add(pathToJarFile);
        }
    }

    public static void reset(Set<String> jarFileList) {
        Lock thisLock = lock.writeLock();
        thisLock.lock();
        try {
            // FIXME: Argh... we can't invalidate the previous list since there are an unknown
            //   number of random users out there with references to the JarFile's we've provided
            //   and they don't expect us to close their file handle from underneth them use.
            // We need a reference counter and all users must obtain/release their handle.
            // This means we need to perform a kind of merge operation to add new entries found here
            //   but keep that reference counting intact for old entries.
            //invalidateLocked();

            HashMap<String, List<String>> tmpCache = new HashMap<String, List<String>>();
            classPathDirs = new ArrayList<String>();

            for (String jarFileName : jarFileList) {
                MyJarFile myJarFile = null;
                try {
                    // 
                    if(jarFileName.startsWith("file:")) {
                        URL urlDir = new URL(jarFileName);
                        String urlFileString = urlDir.getFile();
                        File fileDir = new File(urlFileString);
                        if(fileDir.isDirectory()) {
                            // Need to add in top level so that
                            // add(tmpCache, "", new DirectoryResolver(fileDir));
                            classPathDirs.add(jarFileName);
                            continue;   // FIXME: Use a directory resolver
                        } else if(fileDir.isFile()) {
                            try {
                                myJarFile = new MyJarFile(fileDir);
                            } catch(IOException eat) {
                                // We handle myJarfile==null case below
                            }
                        }
                    }
                    if(myJarFile == null) {  // without "file:" prefix
                        File fileDir = new File(jarFileName);
                        if(fileDir.isDirectory()) {
                            // Need to add in top level so that
                            // add(tmpCache, "", new DirectoryResolver(fileDir));
                            classPathDirs.add(jarFileName);
                            continue;   // FIXME: Use a directory resolver
                        } else if(fileDir.isFile()) {
                            try {
                                myJarFile = new MyJarFile(fileDir);
                            } catch(IOException eat) {
                                // We handle myJarFile==null case below
                            }
                        }
                    }
                    if(myJarFile == null) {
                        URL url = new URL("jar:" + jarFileName + "!/");
                        try {
                            myJarFile = new MyJarFile(url);
                        } catch(ZipException e) {
                            // This often fails with "java.util.zip.ZipException: error in opening zip file" but never discloses the filename
                            throw new ZipException(e.getMessage() + ": " + url);
                        }
                    }

                    String jarFileNameX = myJarFile.getName();  // "/foo/my.jar", "C:\foo\my.jar"

                    Set<String> seenSet = new HashSet<String>();

                    Enumeration<JarEntry> entries = myJarFile.entries();
                    // FIXME: Special case for empty directory
                    while (entries.hasMoreElements()) {
                        JarEntry entry = entries.nextElement();

                        String dirName = null;
                        boolean isToplevelFile = false;

                        String entryName = entry.getName();

                        // Remove potentially initial '/'
                        while (entryName.startsWith("/"))
                            entryName = entryName.substring(1);

                        if (entry.isDirectory()) {
                            if (entryName.endsWith("/"))
                                dirName = entryName.substring(0, entryName.length() - 1);  // canonicalize
                            else
                                dirName = entryName;
                        } else {
                            int slashPos = entryName.lastIndexOf("/");
                            if (slashPos > 0)
                                dirName = entryName.substring(0, slashPos);  // isolate directory part
                            else
                                isToplevelFile = true;  // dirName will be null; there is no directory part
                        }


                        // Add all parent directories "foo/bar/dir1/dir2", "foo/bar/dir1", "foo/bar", "foo"
                        while (dirName != null) {
                            // optimization: if we saw the long nested path (then we already processed its parents as well)
                            if (seenSet.contains(dirName))
                                break;
                            seenSet.add(dirName);
                            add(tmpCache, dirName, jarFileNameX);
                            int slashPos = dirName.lastIndexOf("/");
                            if (slashPos > 0)
                                dirName = dirName.substring(0, slashPos);
                            else
                                dirName = null;
                        }

                        if (isToplevelFile) {
                            if (seenSet.contains("") == false) {
                                seenSet.add("");
                                add(tmpCache, "", jarFileNameX);
                            }
                        }
                    }

                    // Add root dir for all jar files (even empty ones)
                    if (seenSet.contains("") == false) {
                        seenSet.add("");
                        // Add root dir for all jar files (even empty ones)
                        add(tmpCache, "", jarFileNameX);
                    }
                } catch (Exception e) {
                    // Expected as directories will fail when doing openConnection.getJarFile()
                    // Note that ZipFile throws different types of run time exceptions on different
                    // platforms (ZipException on Linux and FileNotFoundException on Windows)
                    classPathDirs.add(jarFileName);
                } finally {
                    if (myJarFile != null) {
                        myJarFile.put();
                        myJarFile = null;
                    }
                }
            }

            cache = tmpCache;
        } finally {
            thisLock.unlock();
        }
    }

    private static void dumpCache(PrintStream out, Map<String, List<String>> thisCache) {
        Lock thisLock = null;
        if(cache == null) {
            thisLock = lock.writeLock();
            thisLock.lock();
            thisCache = cache;
        }
        try {
            for(String s : thisCache.keySet()) {
                out.println(s);
                for(String pathToJarFile : thisCache.get(s)) {
                    out.println(" - '" + pathToJarFile + "'");
                }
            }
        } finally {
            if(thisLock != null)
                thisLock.unlock();
        }
    }

    public static List<String> pathToJarFiles(String entry) {
        List<String> files = null;
        Lock thisLock = lock.readLock();
        thisLock.lock();
        try {
            if(cache != null)
                files = cache.get(entry);
        } finally {
            thisLock.unlock();
        }
        return files;
    }

    private static void invalidateLocked() {
        if(cache == null)
            return;
//        for(Map.Entry<String, List<MyJarFile>> entry : cache.entrySet()) {
//            String key = entry.getKey();
//            List<MyJarFile> value = entry.getValue();
//            for(MyJarFile myJarFile : value) {
//                myJarFile.put();
//            }
//        }
        cache.clear();
        cache = null;
    }

    public static void invalidate() {
        Lock thisLock = lock.writeLock();
        thisLock.lock();
        try {
            invalidateLocked();
        } finally {
            thisLock.unlock();
        }
    }

    public static List<String> classPathDirs() {
        return classPathDirs;
    }

    // Need a way to invalidate the cache (keeping the settings) full/partial
    // Need a way to ensure re-evaluation (removing repeatible-read) full/partial
    // Need a way to add/remove/move one/one-or-more items
    // Need a way to configure each source http/jar/dir (allow adding a source)
    // Need a way to enumerate what we have setup, the source, its policy/config

    // We must maintain an ordered list of the operational ClassPath we a using for resolution
    private static ReadWriteLock lock = new ReentrantReadWriteLock();
    private static HashMap<String, List<String>> cache;
    private static List<String> classPathDirs;
}
