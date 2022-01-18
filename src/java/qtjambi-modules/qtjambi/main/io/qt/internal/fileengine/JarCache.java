/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.internal.fileengine;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;
import java.util.jar.JarEntry;
import java.util.zip.ZipException;

class JarCache {
	
	private final Function<String, Set<String>> newSet = s -> new HashSet<>();

    synchronized Collection<String> pathToJarFiles(String entry) {
    	Collection<String> result = cache.get(entry);
        return result==null? Collections.emptyList() : new ArrayList<>(result);
    }

    synchronized Collection<String> classPathDirs() {
        return new ArrayList<>(classPathDirs);
    }

    private final Map<String, Set<String>> cache = new HashMap<>();
    private final Set<String> classPathDirs = new HashSet<String>();

	synchronized void addPath(URL url) {
		MyJarFile myJarFile = null;
        try {
            // 
            if(url.getProtocol().equals("file")) {
                File fileDir = new File(url.toURI());
                if(fileDir.isDirectory()) {
                    classPathDirs.add(fileDir.getAbsolutePath());
                    return;
                } else if(fileDir.isFile()) {
                    try {
                        myJarFile = new MyJarFile(fileDir);
                    } catch(IOException eat) {
                    }
                }
            }else if(url.toString().endsWith("/")) {
            	classPathDirs.add(url.toString());
                return;
            }
            if(myJarFile == null) {
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
                    cache.computeIfAbsent(dirName, newSet).add(jarFileNameX);
                    int slashPos = dirName.lastIndexOf("/");
                    if (slashPos > 0)
                        dirName = dirName.substring(0, slashPos);
                    else
                        dirName = null;
                }

                if (isToplevelFile) {
                    if (seenSet.contains("") == false) {
                        seenSet.add("");
                        cache.computeIfAbsent("", newSet).add(jarFileNameX);
                    }
                }
            }

            // Add root dir for all jar files (even empty ones)
            if (seenSet.contains("") == false) {
                seenSet.add("");
                // Add root dir for all jar files (even empty ones)
                cache.computeIfAbsent("", newSet).add(jarFileNameX);
            }
        } catch (Exception e) {
            // Expected as directories will fail when doing openConnection.getJarFile()
            // Note that ZipFile throws different types of run time exceptions on different
            // platforms (ZipException on Linux and FileNotFoundException on Windows)
        } finally {
            if (myJarFile != null) {
                myJarFile.put();
                myJarFile = null;
            }
        }
	}

	synchronized void removePath(URL url) {
		MyJarFile myJarFile = null;
        try {
            // 
            if(url.getProtocol().equals("file")) {
                File fileDir = new File(url.toURI());
                if(fileDir.isDirectory()) {
                	removeImpl(fileDir.getAbsolutePath());
                    return;
                } else if(fileDir.isFile()) {
                    try {
                        myJarFile = new MyJarFile(fileDir);
                    } catch(IOException eat) {
                    }
                }
            }else if(url.toString().endsWith("/")) {
            	removeImpl(url.toString());
                return;
            }
            if(myJarFile == null) {
                try {
                    myJarFile = new MyJarFile(url);
                } catch(ZipException e) {
                }
            }
            if(myJarFile != null) {
            	removeImpl(myJarFile.getName());  // "/foo/my.jar", "C:\foo\my.jar"
            }else {
            	removeImpl(url.toString());
            }
        } catch (Exception e) {
            // Expected as directories will fail when doing openConnection.getJarFile()
            // Note that ZipFile throws different types of run time exceptions on different
            // platforms (ZipException on Linux and FileNotFoundException on Windows)
        	removeImpl(url.toString());
        } finally {
            if (myJarFile != null) {
                myJarFile.put();
                myJarFile = null;
            }
        }
	}
	
	private void removeImpl(String jarFileName) {
		for(String key : new ArrayList<>(cache.keySet())) {
			Set<String> entries = cache.get(key);
			if(entries!=null) {
				entries.remove(jarFileName);
				if(entries.isEmpty()) {
					cache.remove(key);
				}
			}else {
				cache.remove(key);
			}
		}
	}
}
