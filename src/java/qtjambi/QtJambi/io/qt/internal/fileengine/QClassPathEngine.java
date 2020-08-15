/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static io.qt.internal.fileengine.QClassPathResourceManager.FileNameDelim;
import static io.qt.internal.fileengine.QClassPathResourceManager.FileNamePrefix1;
import static io.qt.internal.fileengine.QClassPathResourceManager.FileNamePrefix2;
import static io.qt.internal.fileengine.QClassPathResourceManager.classPathDirs;
import static io.qt.internal.fileengine.QClassPathResourceManager.makeUrl;
import static io.qt.internal.fileengine.QClassPathResourceManager.pathToJarFiles;
import static io.qt.internal.fileengine.QClassPathResourceManager.resolveUrlToMyJarFile;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.JarURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.Collection;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.JarEntry;

import io.qt.core.QDateTime;
import io.qt.core.QDir;
import io.qt.core.QFileDevice;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.internal.QAbstractFileEngine;
import io.qt.core.internal.QAbstractFileEngineIterator;
import io.qt.internal.NativeAccess;

@Deprecated
class QClassPathEngine extends QAbstractFileEngine {
    private String m_fileName = "";
    private String m_baseName = "";
    private String m_selectedSource = "*";
    private LinkedList<QAbstractFileEngine> m_engines = new LinkedList<>();  // ConcurrentLinkedList ?

    @NativeAccess
    private QClassPathEngine(String fileName) {
        setFileName(fileName);
    }

    @Override
    public void setFileName(String fileName)
    {
        if (fileName.equals(fileName()))
            return;

        cleanUp();
        if(fileName.startsWith(FileNamePrefix2)) {
        	m_fileName = fileName.substring(FileNamePrefix2.length());
        }else {
	        if (!fileName.startsWith(FileNamePrefix1))
	            throw new IllegalArgumentException("Invalid format of path: '" + fileName + "'");
	        m_fileName = fileName.substring(FileNamePrefix1.length());
        }

        int idx = m_fileName.indexOf("#");
        m_selectedSource = "*";
        if (idx == -1) {
            m_baseName = m_fileName;
        } else {
            m_baseName = m_fileName.substring(idx+1);
            m_selectedSource = m_fileName.substring(0, idx);
        }

        int first = 0;
        int last = m_baseName.length();

        while (first < last && m_baseName.charAt(first) == '/')
            ++first;
        if (m_baseName.endsWith("/"))
            --last;

        if (last < first)
            m_baseName = "";
        else
            m_baseName = m_baseName.substring(first, last).replace('\\', '/');

        if ("*".equals(m_selectedSource)) {
        	Collection<String> pathToPotentialJars = pathToJarFiles(m_baseName);

            if (!pathToPotentialJars.isEmpty()) { // Its at least a directory which exists in jar files
                for (String pathToJar : pathToPotentialJars) {
                    addJarFileFromPath(pathToJar, m_baseName, true);
                }
            } else { // Its a file or directory, look for jar files which contains its a directory

                String parentSearch;

                int pos = m_baseName.lastIndexOf("/");
                if (pos < 0)
                    parentSearch = "";
                else
                    parentSearch = m_baseName.substring(0, pos);

                // This is all wrong... we need to maintain the ordered list of the mix then attempt
                //  to populate from each in turn (if we are exhaustive) otherwise
                Collection<String> pathToJars = pathToJarFiles(parentSearch);
                if (pathToJars != null) {
                    for (String pathToJar : pathToJars) {
                        addJarFileFromPath(pathToJar, m_baseName, false);
                    }
                }

                for (String path : classPathDirs()) {
                    try {
                        // FIXME: This maybe already URL or raw dir, I think we should just make this a
                        //  dir in the native String format
                        addFromPath(makeUrl(path), m_baseName);
                    } catch (Exception e) {
                        java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, "", e);
                    }
                }
            }
        } else
            try {
            	URL url = makeUrl(m_selectedSource);
                File fileOnlyIfDirectory = null;
                if("file".equals(url.getProtocol())) {
                    String pathString = url.getPath();
                    if(File.separatorChar == '\\')
                        pathString = pathString.replace('/', '\\');  // windows
                    fileOnlyIfDirectory = new File(pathString);
                    if(fileOnlyIfDirectory.isDirectory() == false)
                        fileOnlyIfDirectory = null;
                }
                if(fileOnlyIfDirectory != null || url.toString().endsWith("/"))
                    addFromPath(url, m_baseName);
                else if(url.toString().endsWith(".jar"))
                    addJarFileFromPath(new URL("jar:" + url + "!/"), m_baseName);
                else
                	addJarFileFromPath(url, m_baseName);
            } catch (Exception e) {
                java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, "", e);
            }
    }

    @Override
    public boolean copy(String newName) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.copy(newName);
        return false;
    }

    @Override
    public synchronized boolean setPermissions(int perms) {
        for(QAbstractFileEngine engine : m_engines) {
            if(engine.setPermissions(perms))
                return true;
        }
        return false;
    }

    @Override
    public boolean caseSensitive() {
        return true;
    }

    @Override
    public boolean close() {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.close();     // FIXME: How does the closed engine get removed from the list ?
        return false;
    }

    @Override
    public synchronized List<String> entryList(QDir.Filters filters, Collection<String> filterNames) {
        List<String> result = null;
        for (QAbstractFileEngine engine : m_engines) {
            if (result == null) {
                result = engine.entryList(filters, filterNames);
            } else {
                List<String> list = engine.entryList(filters, filterNames);
                // FIXME: Surely the higher precedence engines get asked first and the first found has priority over the last
                //   so why do we removeAll() here.  list.removeAll(result);  result.addAll(list); ?
                result.removeAll(list);
                result.addAll(list);
            }
        }
        return result;
    }

    @Override
    public synchronized FileFlags fileFlags(FileFlags type) {
        FileFlags flags = new FileFlags();

        for (QAbstractFileEngine engine : m_engines)
            flags.set(engine.fileFlags(type));

        if ("/".equals(fileName(FileName.PathName)))
            flags.set(QAbstractFileEngine.FileFlag.RootFlag);

        flags.clear(FileFlag.LocalDiskFlag);

        return flags;
    }

    @Override
    public synchronized String fileName(FileName file) {
        QAbstractFileEngine afe = null;
        int engineCount;
        engineCount = m_engines.size();
        if(engineCount > 0)
            afe = m_engines.getFirst();
        if (engineCount == 0) {
            return "";
        }

        String classPathEntry;
        if (engineCount == 1) {
            if (afe instanceof QClassPathEntry)
                classPathEntry = ((QClassPathEntry) afe).classPathEntryName();
            else
                throw new RuntimeException("Bogus engine in class path file engine");
        } else {
            classPathEntry = "*";
        }

        String result = "";
        if (file == FileName.DefaultName) {
            result = FileNamePrefix1 + m_fileName;
        } else if (file == FileName.CanonicalName || file == FileName.LinkName) {
            result = fileName(FileName.CanonicalPathName) + "/" + fileName(FileName.BaseName);
        } else if (file == FileName.AbsoluteName || file == FileName.LinkName) {
            result = FileNamePrefix1 + classPathEntry + FileNameDelim + m_baseName;
        } else if (file == FileName.BaseName) {
            int pos = m_baseName.lastIndexOf("/");
            result = pos > 0 ? m_baseName.substring(pos + 1) : m_baseName;
        } else if (file == FileName.PathName) {
            int pos = m_baseName.lastIndexOf("/");
            result = pos > 0 ? m_baseName.substring(0, pos) : "";
        } else if (file == FileName.AbsolutePathName) {
            result = FileNamePrefix1 + classPathEntry + FileNameDelim + fileName(FileName.PathName);
        } else if (file == FileName.CanonicalPathName) {
            // FIXME: can afe==null ?
            result = afe.fileName(file);
        } else {
            throw new IllegalArgumentException("Unknown file name type: " + file);
        }

        return result;
    }

    @Override
    public QDateTime fileTime(FileTime time) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.fileTime(time);
        return new QDateTime();
    }

    @Override
    public synchronized boolean link(String newName) {
        for (QAbstractFileEngine engine : m_engines) {
            if (engine.link(newName))
                return true;
        }
        return false;
    }

    @Override
    public synchronized boolean mkdir(String dirName, boolean createParentDirectories) {
        for (QAbstractFileEngine engine : m_engines) {
            if (engine.mkdir(dirName, createParentDirectories))
                return true;
        }
        return false;
    }

    @Override
    public boolean open(QIODevice.OpenMode openMode) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null){
            boolean result = afe.open(openMode);
            if(result){
                // without this, engine remains at UnspecifiedError which avoids QNetworkAccessManager to work right.
                this.setError(QFileDevice.FileError.NoError, "");
            }
            return result;
        }
        return false;
    }

    @Override
    public long pos() {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.pos();
        return -1;
    }

    @Override
    public int read(byte[] data) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.read(data);
        return -1;
    }

    @Override
    public int readLine(byte[] data) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.readLine(data);
        return -1;
    }

    @Override
    public synchronized boolean remove() {
        boolean ok = true;
        for (QAbstractFileEngine engine : m_engines)
            ok = ok && engine.remove();
        return ok;
    }

    @Override
    public synchronized boolean rename(String newName) {
        boolean ok = true;
        for (QAbstractFileEngine engine : m_engines)
            ok = ok && engine.rename(newName);
        return ok;
    }

    @Override
    public synchronized boolean rmdir(String dirName, boolean recursive) {
        boolean ok = true;
        for (QAbstractFileEngine engine : m_engines)
            ok = ok && engine.rmdir(dirName, recursive);
        return ok;
    }

    @Override
    public boolean seek(long offset) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.seek(offset);
        return false;
    }

    @Override
    public synchronized String owner(FileOwner owner) {
        for(QAbstractFileEngine afe : m_engines) {
            String result = afe.owner(owner);
            if(result != null && result.length() > 0)  // result.isEmpty() is Java 1.6+
                return result;
        }
        return ""; // FIXME: Why not null ?
    }

    @Override
    public synchronized int ownerId(FileOwner owner) {
        for(QAbstractFileEngine afe : m_engines) {
            int result = afe.ownerId(owner);
            if (result != -2)
                return result;
        }
        return -2;
    }

    @Override
    public boolean isRelativePath() {
        return false;
    }

    @Override
    public synchronized boolean isSequential() {
        for (QAbstractFileEngine engine : m_engines) {
            if (engine.isSequential())
                return true;
        }
        return false;
    }

    @Override
    public boolean setSize(long sz) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.setSize(sz);
        return false;
    }

    @Override
    public long size() {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return m_engines.get(0).size();
        return -1;
    }

    @Override
    public int write(byte[] data) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.write(data);
        return -1;
    }

    private synchronized void cleanUp() {
        if (m_engines != null)
            m_engines.clear();
    }

    // FIXME: private void addURLFromPath(URL url, String fileName) { }

    /**
     * 
     * @param url This maybe a file URL such as new URL("file:/C:/foobar/cp")
     * @param fileName
     * @return
     */
    private boolean addFromPath(URL url, String fileName) {
        // If it is a plain file on the disk, just read it from the disk
        if ("file".equals(url.getProtocol())) {
        	String path = url.getPath();  // All URL paths have "/" separators
            if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)) {
                if(path.length() > 2 && path.charAt(2) == ':' && path.startsWith("/"))
                    path = path.substring(1);	// Convert "/C:/foobar/cp" => "C:/foobar/cp"
            }
            QFileInfo file = new QFileInfo(path);
            if (file.isDir()
                    && file.exists()
                    && new QFileInfo(path + "/" + fileName).exists()) {
                // TODO check URL#toExternalForm() is correct here with s/ /%20/ conversion
                addEngine(new QFSEntryEngine(path + "/" + fileName, url.toExternalForm()));
                return true;
            }
        }
        if(url.toString().endsWith("/")){
        	QFileInfo file = new QFileInfo(url.toString() + fileName);
            if (file.exists()) {
                addEngine(new QFSEntryEngine(url.toString() + fileName, url.toExternalForm()));
                return true;
            }
        }
        try {
			URL fileUrl = new URL(url.toString() + fileName);
			URLConnection connection = fileUrl.openConnection();
			try(InputStream inputStream = connection.getInputStream()){
				inputStream.available();
			}
			addEngine(new QUrlEntryEngine(connection, url.toExternalForm()));
            return true;
		} catch (Exception e) {
		}
        return false;
    }

    // We are passed an open JarFile to take ownership of and use, if we throw exception caller is responsible for the still open JarFile
    private boolean addJarFileFromPath(MyJarFile myJarFile, String fileName, boolean directory) throws IOException {
        QJarEntryEngine engine = new QJarEntryEngine(myJarFile, fileName, directory);
        if(engine.isValid()) {
            addEngine(engine);
            return true;
        }
        engine.disown();  // transfers ownership of reference count back to us
        return false;
    }

    private boolean addJarFileFromPath(String pathToJarFile, String fileName, boolean directory) {
        boolean bf = false;
        MyJarFile myJarFile = null;
        try {
            File fileToJarFile = new File(pathToJarFile);
            // We use MyJarFile(File) constructor to indicate this is a direct I/O file (as opposed to a URL)
            myJarFile = new MyJarFile(fileToJarFile);
            // Each engine must have its own instance of JarFile as it can not be shared across threads
            bf = addJarFileFromPath(myJarFile, fileName, directory);
            if(bf)
                myJarFile = null;  // stops it being closed in finally
        } catch(IOException eat) {
        } finally {
            // We are responsible to close in all cases except bf==true (such as Exception or bf==false)
            if(myJarFile != null) {
                myJarFile.put();
                myJarFile = null;
            }
        }
        return bf;
    }

    /**
     * The JarEntry.isDirectory() method in Java returns false
     * even for directories, so we need this extra check
     * which tries to read a byte from the entry in order
     * to trigger an exception when the entry is a directory.
     */
    static boolean checkIsDirectory(MyJarFile myJarFile, JarEntry fileInJar) {
        InputStream inStream = null;
        try {
            // CHECKME is this hack/trick/kludge maybe somewhat problematic
            //  for connection handler based JarFile handles ?
            inStream = myJarFile.getInputStream(fileInJar);
            if(inStream == null)
                return true;	// avoid NPE
            inStream.read();
        } catch(IOException e) {
            return true;
        } finally {
            if(inStream != null) {
                try {
                    inStream.close();
                } catch(IOException eat) {
                }
                inStream = null;
            }
        }

        return false;
    }

    private void addJarFileFromPath(URL jarFileURL, String fileName) {
        URLConnection urlConnection = null;
        MyJarFile myJarFile = null;
        try {
            myJarFile = resolveUrlToMyJarFile(jarFileURL);
            if(myJarFile == null)
                return;  // anti NPE check (maybe classpath directory entries don't come through here)

            boolean isDirectory = false;
            JarEntry fileInJar = myJarFile.getJarEntry(fileName);

            // If the entry exists in the given file, look it up and
            // check if its a dir or not
            if (fileInJar != null) {
                isDirectory = fileInJar.isDirectory();
                if (!isDirectory) {
                    boolean tmpIsDirectory = checkIsDirectory(myJarFile, fileInJar);
                    isDirectory = tmpIsDirectory;
                } else {
                }
            }

            if (!isDirectory) {
                // Otherwise, look if the directory exists in the
                // cache...
            	Collection<String> pathToJarFiles = pathToJarFiles(fileName);
                String jarFileName = myJarFile.getName();
                if (pathToJarFiles != null) {
                    for (String thisPathToJar : pathToJarFiles) {
                        if (thisPathToJar.equals(jarFileName)) {
                            isDirectory = true;
                            break;
                        }
                    }
                }

                // Nasty fallback... Iterate through the .jar file and try to check if
                // fileName is the prefix (hence directory) of any of the entries...
                if (!isDirectory) {
                    String fileNameWithSlash = fileName + "/";
                    Enumeration<JarEntry> entries = myJarFile.entries();
                    while (entries.hasMoreElements()) {
                        JarEntry entry = entries.nextElement();
                        String entryName = entry.getName();
                        if (entryName.startsWith(fileNameWithSlash)) {
                            isDirectory = true;
                            break;
                        }
                    }
                }
            }

            if(addJarFileFromPath(myJarFile, fileName, isDirectory))  // handover jarFile
                myJarFile = null;	// inhibit jarFile.close() in finally block
        } catch(Exception e) {
            java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, "", e);
        } finally {
            if(myJarFile != null) {
                myJarFile.put();
                myJarFile = null;
            }
            if(urlConnection != null) {
                urlConnection = null;
            }
        }
    }

    @SuppressWarnings("unused")
	private static void urlConnectionCloser(URLConnection conn) {
        if(conn instanceof HttpURLConnection) {
            HttpURLConnection httpURLConnection = (HttpURLConnection) conn;
            httpURLConnection.disconnect();
            return;
        }
        if(conn instanceof JarURLConnection) {
            JarURLConnection jarURLConnection = (JarURLConnection) conn;
            return;
        }
    }

    private synchronized void addEngine(QAbstractFileEngine engine) {
        m_engines.add(engine);
    }

    @Override
    public QAbstractFileEngineIterator beginEntryList(QDir.Filters filters, java.util.Collection<String> nameFilters) {
        String path = "";
        if(m_baseName.startsWith(FileNamePrefix1))
            path = m_baseName;
        else
            path = FileNamePrefix1 + m_baseName;
        return new QClassPathFileEngineIterator(path, filters, nameFilters);
    }

    @Override
    public QAbstractFileEngineIterator endEntryList() {
        return null;
    }

    private synchronized QAbstractFileEngine getFirstEngine() {
        if(!m_engines.isEmpty())
            return m_engines.getFirst();
        return null;
    }
}
