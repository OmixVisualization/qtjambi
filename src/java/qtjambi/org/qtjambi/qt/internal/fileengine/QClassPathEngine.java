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
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.JarURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.zip.ZipException;

import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.Utilities;
import org.qtjambi.qt.core.QAbstractFileEngine;
import org.qtjambi.qt.core.QAbstractFileEngine.FileFlag;
import org.qtjambi.qt.core.QAbstractFileEngine.FileFlags;
import org.qtjambi.qt.core.QAbstractFileEngine.FileName;
import org.qtjambi.qt.core.QAbstractFileEngine.FileOwner;
import org.qtjambi.qt.core.QAbstractFileEngine.FileTime;
import org.qtjambi.qt.core.QAbstractFileEngineIterator;
import org.qtjambi.qt.core.QDateTime;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileDevice;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.osinfo.OSInfo;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

@Deprecated
public class QClassPathEngine extends QAbstractFileEngine {
    public final static String FileNameDelim = "#";
    public final static String FileNameIndicator = "classpath";
    public final static String FileNamePrefix = FileNameIndicator + ":";

    // JarCache should not be global but instated here
    private static HashSet<String> classpaths;

    private String m_fileName = "";
    private String m_baseName = "";
    private String m_selectedSource = "*";
    private LinkedList<QAbstractFileEngine> m_engines = new LinkedList<QAbstractFileEngine>();  // ConcurrentLinkedList ?

    private static String currentDirectory;

    private String namespaceSchemePrefix = "classpath";

    public QClassPathEngine(String fileName) {
        setFileName(fileName);
    }

    public QClassPathEngine(String fileName, String namespaceSchemePrefix) {
        this.namespaceSchemePrefix = namespaceSchemePrefix;
        setFileName(fileName);
    }

    private static String resolveCurrentDirectory() {
        String tmpCurrentDirectory = currentDirectory;
        if(tmpCurrentDirectory != null)
            return tmpCurrentDirectory;

        synchronized(QClassPathEngine.class) {
            tmpCurrentDirectory = currentDirectory;
            // retest
            if(tmpCurrentDirectory != null)
                return tmpCurrentDirectory;

            // FIXME: Maybe user.dir should be priority here ?

            File fileCurDir = new File(".");
            if(fileCurDir.isDirectory()) {
                // getParentFile() does not do the trick for us (it will/can be null in this circumstance)
                //  this method ensures it does not end with "/." or "\\." which is unwanted
                tmpCurrentDirectory = fileCurDir.getAbsolutePath();
                String removeDelimAndDot = File.separator + ".";  // "/." on unix, "\\." on windows
                if(tmpCurrentDirectory.endsWith(removeDelimAndDot))
                    tmpCurrentDirectory = tmpCurrentDirectory.substring(0, tmpCurrentDirectory.length() - removeDelimAndDot.length());
            } else {
                tmpCurrentDirectory = System.getProperty("user.dir");
            }

            currentDirectory = tmpCurrentDirectory;
        }
        return tmpCurrentDirectory;
    }

    static void setCurrentDirectory(String newCurrentDirectory) {
        synchronized(QClassPathEngine.class) {
            currentDirectory = newCurrentDirectory;
        }
    }

    // FIXME: This API needs improving, I think URL() can be heavy weight and I guess most
    //  user of this API probably want that.
    private static String makeUrl(String path) {
        String goodPath = null;

        // FIXME: Need special handling of "." to mean current directory.  But when
        //  conversion process should be controlled.  Cwd at app startup?

        if(path == null)
            return goodPath;
        final int pathLength = path.length();

        boolean skipTryAsis = false;	// attempt to not use exceptions for common situations
        if(pathLength > 0) {
            char firstChar = path.charAt(0);
            // Both a "/" and "\\" are illegal characters in the scheme/protocol.
            if(firstChar == File.separatorChar) {
                skipTryAsis = true;
            } else if(pathLength == 1) {
                if(firstChar == '.') {
                    // FIXME: ../../foo/bar
                    // Special case for current directory
                    String tmpPath = resolveCurrentDirectory();
                    if(tmpPath != null) {
                        path = tmpPath;
                        skipTryAsis = true;
                    }
                }
                // ELSE it is a relative path and will be picked up below
            } else if(pathLength > 1) {
                char secondChar = path.charAt(1);
                if(firstChar == '.' && secondChar == File.separatorChar) {
                    // ./foo/bar case
                    String tmpPath = resolveCurrentDirectory();
                    if(tmpPath != null) {
                        // FIXME: This is better resolved later via path canonicalization (to fix the ./././foo case)
                        path = tmpPath + File.separatorChar + path.substring(2);
                        skipTryAsis = true;
                    }
                } else if(pathLength > 2) {
                    // Windows "C:\\..." for which "\\" is incorrect for URLs
                    char thirdChar = path.charAt(2);
                    if((firstChar >= 'A' && firstChar <= 'Z') || (firstChar >= 'a' && firstChar <= 'z')) {
                        // We don't check for '/' since that might be a real URL "a://host:port/path?qs"
                        // and would be invalid for windows using java.io.File API anyway.
                        if(secondChar == ':' && thirdChar == '\\')
                            skipTryAsis = true;
                        if(secondChar == ':' && thirdChar == '/')  // "C:/dir1/dir2/file.dat" is seen when processing paths from QFileInfo
                            skipTryAsis = true;
                    } else if(pathLength > 3) {
                        // Eclipse "/C:/..."
                        char fourthChar = path.charAt(3);
                        if(firstChar == '/' && (secondChar >= 'A' && secondChar <= 'Z') || (secondChar >= 'a' && secondChar <= 'z')) {
                            if(thirdChar == ':' && fourthChar == '/')
                                skipTryAsis = true;  // we prefix it with file:// below
                        }
                    }
                }
            }
            if(skipTryAsis == false) {
                // If skipTryAsis==true then we found an absolute path (or converted
                //   what is there already to an absolute path)
                boolean prefix = true;
                // Eek... temporary hack, some users of this method makeUrl() seem to be
                //  already passing a valid URL as input.  This is one point to fix in a
                //  later review.  For now be blacklist known prefixes from being treated
                //  as relative paths.  FIXME
                if(path.startsWith("file:") || path.startsWith("jar:") || path.startsWith("http:") || path.startsWith("https:"))
                    prefix = false;
                if(prefix) {
                    String tmpPath = resolveCurrentDirectory();
                    if(tmpPath != null) {
                        path = tmpPath + File.separatorChar + path;
                        skipTryAsis = true;
                    }
                }
            }
        }

        Exception urlParseException = null;
        if(goodPath == null && skipTryAsis == false) {
            try {
                // See if the data passed is a well-formed URL
                // Relative paths end up here and throwing exceptions
                // Maybe we should look for "://" in path before using this method?
                // What is the resolution mechanism for using URL(path) with arbitrary string?
                // Does a windows path C:/foo/bar/file.dat end up as scheme=C or protocol=C ?
                URL url = new URL(path);
                if(url.getProtocol().length() > 0)
                    goodPath = path;
            } catch(Exception e) {
                urlParseException = e;
                //e.printStackTrace();
            }
        }

        Exception urlParseException2 = null;
        if(goodPath == null) {
            try {
                // Validate the URL we build is well-formed
                // FIXME: file://
                String tmpPath = "file:" + path;
				String xPath = path.replace('\\', '/');
				String xPrefix;
				if(path.length() > 0 && xPath.charAt(0) != '/')
				    xPrefix = "file:///";
				else
				    xPrefix = "file://";
                String newTmpPath = xPrefix;
                if(File.separatorChar == '\\')
                    newTmpPath += path.replace('\\', '/');  // windows
                else
                    newTmpPath += path;
                String newTmpPathY = Utilities.convertAbsolutePathStringToFileUrlString(path);
                URL url = new URL(newTmpPath);
                if(url.getProtocol().length() > 0) {
                     goodPath = newTmpPath;	// This must be converted to URL valid form like "file:///C:/foobar/cp"
                }
            } catch(Exception e) {
                urlParseException2 = e;
                //e.printStackTrace();
            }
        }

    URLConnection urlConn = null;
    InputStream inStream = null;
    try {
        URL openUrl = new URL(goodPath);
        URLAlias urlAlias = checkNeedWorkaround(openUrl);
        if(urlAlias.file != null) {  // Due to workaround
            if(urlAlias.file.isFile()) // skip dirs
                inStream = new FileInputStream(urlAlias.file);
        } else {
            urlConn = urlAlias.url.openConnection();
            inStream = urlConn.getInputStream();
        }
    } catch(Exception e) {
        e.printStackTrace();
    } finally {
        if(inStream != null) {
            try {
                inStream.close();
            } catch(IOException eat) {
            }
        }
    }
        return goodPath;
    }

    // Need API to add one/one-or-more, separate API to JarCache.reset()
    public static boolean addSearchPath(String path, boolean allowDuplicate) {
        boolean bf = false;
        String urlString = makeUrl(path);
        synchronized(QClassPathEngine.class){
            // FIXME: This should not be here, it should execute once by default
            // and once each time the user explicitly request such.  The user should
            // be allowed to add
            if(classpaths == null)
                findClassPaths();

            // Do not disurb the order of the existing classpaths, such
            // things are sensitive matters.
            if(urlString != null) {
                // FWIW this is a Set so we can't duplicate, but we probably should be a list
                // when we are a list, we should ignore the re-add if at a lower-priority position, but take not if higher-priority
                if(allowDuplicate || classpaths.contains(urlString) == false) {
                    classpaths.add(urlString);
                    bf = true;
                }
            }

            JarCache.reset(classpaths);
        }
        return bf;
    }

    public static boolean removeSearchPath(String path) {
        boolean bf = false;
        synchronized(QClassPathEngine.class){
            if(classpaths != null) {
                String urlString = makeUrl(path);
                if(urlString != null) {
                    bf = classpaths.remove(urlString);
                    JarCache.reset(classpaths);
                }
            }
        }
        return bf;
    }

    @Override
    public void setFileName(String fileName)
    {
        if (fileName.equals(fileName()))
            return;

        cleanUp();
        if (!fileName.startsWith(FileNamePrefix))
            throw new IllegalArgumentException("Invalid format of path: '" + fileName + "'");
        m_fileName = fileName.substring(FileNamePrefix.length());

        String searchPath[] = RetroTranslatorHelper.split(m_fileName, "#", 2);

        m_selectedSource = "*";
        if (searchPath.length == 1) {
            m_baseName = searchPath[0];
        } else {
            m_baseName = searchPath[1];
            m_selectedSource = searchPath[0];
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

        if (classpaths == null)
            findClassPaths();

        if ("*".equals(m_selectedSource)) {
            List<String> pathToPotentialJars = JarCache.pathToJarFiles(m_baseName);

            if (pathToPotentialJars != null) { // Its at least a directory which exists in jar files
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
                List<String> pathToJars = JarCache.pathToJarFiles(parentSearch);
                if (pathToJars != null) {
                    for (String pathToJar : pathToJars) {
                        addJarFileFromPath(pathToJar, m_baseName, false);
                    }
                }

                for (String path : JarCache.classPathDirs()) {
                    try {
                        // FIXME: This maybe already URL or raw dir, I think we should just make this a
                        //  dir in the native String format
                        addFromPath(new URL(makeUrl(path)), m_baseName);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        } else
            try {
                String urlString = makeUrl(m_selectedSource);

                // If it's a file (it should be), strip away the getProtocol() and check whether the
                // file is a directory. Otherwise it's assumed to be a .jar file
                URL url = new URL(urlString);
                File fileOnlyIfDirectory = null;
                if("file".equals(url.getProtocol())) {
                    String pathString = url.getPath();
                    if(File.separatorChar == '\\')
                        pathString = pathString.replace('/', '\\');  // windows
                    fileOnlyIfDirectory = new File(pathString);
                    if(fileOnlyIfDirectory.isDirectory() == false)
                        fileOnlyIfDirectory = null;
                }
                if(fileOnlyIfDirectory != null)
                    addFromPath(url, m_baseName);
                else
                    addJarFileFromPath(new URL("jar:" + urlString + "!/"), m_baseName);
            } catch (Exception e) {
                e.printStackTrace();
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
    public boolean setPermissions(int perms) {
        synchronized(QClassPathEngine.class) {
            for(QAbstractFileEngine engine : m_engines) {
                if(engine.setPermissions(perms))
                    return true;
            }
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
    public List<String> entryList(QDir.Filters filters, List<String> filterNames) {
        List<String> result = null;
        synchronized(QClassPathEngine.class) {
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
        }

        return result;
    }

    @Override
    public FileFlags fileFlags(FileFlags type) {
        FileFlags flags = new FileFlags();

        synchronized(QClassPathEngine.class) {
            for (QAbstractFileEngine engine : m_engines)
                flags.set(engine.fileFlags(type));
        }

        if ("/".equals(fileName(FileName.PathName)))
            flags.set(QAbstractFileEngine.FileFlag.RootFlag);

        flags.clear(FileFlag.LocalDiskFlag);

        return flags;
    }

    @Override
    public String fileName(FileName file) {
        QAbstractFileEngine afe = null;
        int engineCount;
        synchronized(QClassPathEngine.class) {
            engineCount = m_engines.size();
            if(engineCount > 0)
                afe = m_engines.getFirst();
        }
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
            result = QClassPathEngine.FileNamePrefix + m_fileName;
        } else if (file == FileName.CanonicalName || file == FileName.LinkName) {
            result = fileName(FileName.CanonicalPathName) + "/" + fileName(FileName.BaseName);
        } else if (file == FileName.AbsoluteName || file == FileName.LinkName) {
            result = QClassPathEngine.FileNamePrefix + classPathEntry + FileNameDelim + m_baseName;
        } else if (file == FileName.BaseName) {
            int pos = m_baseName.lastIndexOf("/");
            result = pos > 0 ? m_baseName.substring(pos + 1) : m_baseName;
        } else if (file == FileName.PathName) {
            int pos = m_baseName.lastIndexOf("/");
            result = pos > 0 ? m_baseName.substring(0, pos) : "";
        } else if (file == FileName.AbsolutePathName) {
            result = QClassPathEngine.FileNamePrefix + classPathEntry + FileNameDelim + fileName(FileName.PathName);
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
    public boolean link(String newName) {
        synchronized(QClassPathEngine.class) {
            for (QAbstractFileEngine engine : m_engines) {
                if (engine.link(newName))
                    return true;
            }
        }
        return false;
    }

    @Override
    public boolean mkdir(String dirName, boolean createParentDirectories) {
        synchronized(QClassPathEngine.class) {
            for (QAbstractFileEngine engine : m_engines) {
                if (engine.mkdir(dirName, createParentDirectories))
                    return true;
            }
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
    public long read(QNativePointer data, long maxlen) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.read(data, maxlen);
        return -1;
    }

    @Override
    public long readLine(QNativePointer data, long maxlen) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.readLine(data, maxlen);
        return -1;
    }

    @Override
    public boolean remove() {
        boolean ok = true;
        synchronized(QClassPathEngine.class) {
            for (QAbstractFileEngine engine : m_engines)
                ok = ok && engine.remove();
        }
        return ok;
    }

    @Override
    public boolean rename(String newName) {
        boolean ok = true;
        synchronized(QClassPathEngine.class) {
            for (QAbstractFileEngine engine : m_engines)
                ok = ok && engine.rename(newName);
        }
        return ok;
    }

    @Override
    public boolean rmdir(String dirName, boolean recursive) {
        boolean ok = true;
        synchronized(QClassPathEngine.class) {
            for (QAbstractFileEngine engine : m_engines)
                ok = ok && engine.rmdir(dirName, recursive);
        }
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
    public String owner(FileOwner owner) {
        synchronized(QClassPathEngine.class) {
            for(QAbstractFileEngine afe : m_engines) {
                String result = afe.owner(owner);
                if(result != null && result.length() > 0)  // result.isEmpty() is Java 1.6+
                    return result;
            }
        }
        return ""; // FIXME: Why not null ?
    }

    @Override
    public int ownerId(FileOwner owner) {
        synchronized(QClassPathEngine.class) {
            for(QAbstractFileEngine afe : m_engines) {
                int result = afe.ownerId(owner);
                if (result != -2)
                    return result;
            }
        }
        return -2;
    }

    @Override
    public boolean isRelativePath() {
        return false;
    }

    @Override
    public boolean isSequential() {
        synchronized(QClassPathEngine.class) {
            for (QAbstractFileEngine engine : m_engines) {
                if (engine.isSequential())
                    return true;
            }
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
    public long write(QNativePointer data, long len) {
        QAbstractFileEngine afe = getFirstEngine();
        if(afe != null)
            return afe.write(data, len);
        return -1;
    }

    private void cleanUp() {
        synchronized(QClassPathEngine.class) {
            if (m_engines != null)
                m_engines.clear();
        }
    }

    // FIXME: private void addURLFromPath(URL url, String fileName) { }

    /**
     * 
     * @param url This maybe a file URL such as new URL("file:/C:/foobar/cp")
     * @param fileName
     * @return
     */
    private boolean addFromPath(URL url, String fileName) {
        String path = url.getPath();  // All URL paths have "/" separators
        if(OSInfo.isWindows()) {
            if(path.length() > 2 && path.charAt(2) == ':' && path.startsWith("/"))
                path = path.substring(1);	// Convert "/C:/foobar/cp" => "C:/foobar/cp"
        }

        // If it is a plain file on the disk, just read it from the disk
        if ("file".equals(url.getProtocol())) {
            QFileInfo file = new QFileInfo(path);
            if (file.isDir()
                    && file.exists()
                    && new QFileInfo(path + "/" + fileName).exists()) {
                // TODO check URL#toExternalForm() is correct here with s/ /%20/ conversion
                addEngine(new QFSEntryEngine(path + "/" + fileName, url.toExternalForm()));
                return true;
            }
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

    static String stripSuffix(String s, String suffix, String defaultValue) {
        if(s.endsWith(suffix)) {
            int endIndex = s.length() - suffix.length();
            return s.substring(0, endIndex);
        }
        return defaultValue;
    }

    static String stripUrlFileColonPrefix(String urlString, String defaultValue) {
        final int len = urlString.length();
        final String PREFIX_FILE = "file:";
        if(urlString.startsWith(PREFIX_FILE)) {  // efficiency?  f**K that for now
            int skipCount = PREFIX_FILE.length();
            while(len > skipCount) {  // remove multiple "/" prefixes
                char c = urlString.charAt(skipCount);
                if(c != '/') {
                    if(skipCount > 0)
                        skipCount--;  // put at least one back
                    break;
                }
                skipCount++;
            }
            if(skipCount > 0)
                urlString = urlString.substring(skipCount);
            return urlString;
        }
        return defaultValue;
    }

    private static class URLAlias {
        URL url;
        File file;
    }

    static URLAlias checkNeedWorkaround(URL url) {
        final String SEPARATOR = "!/";
        File file = null;
        // Sun/Oracle bugid#7050028  "IllegalStateException: zip file closed" from JarURLConnection
        // Workarounds 1) Do not turn off URLConnection caches.
        //             2) Use file:/x.jar URLs rather than jar:file:x.jar!/ URLs for URLClassLoader
        if("jar".equals(url.getProtocol())) {
            // Replace "^jar:file:" with "file:" only if it also ends with "!/"
            final String PREFIX_JAR_FILE = "jar:file:";
            String externalForm = url.toExternalForm();
            if(externalForm.startsWith(PREFIX_JAR_FILE)) {
                // Don't use URL#toExternalForm() as this results in conversions in path to URL encoded
                //  such as s/ /%20/ this will break java.io.File API usage on the resulting path.
                String path = url.getPath();  // this has "file:///" prefix already and "!/" suffix
                int index = externalForm.indexOf(SEPARATOR);
                int externalFormLength = externalForm.length();
                // check the first "!/" is also the last and at the end of the string
                if(index == externalFormLength - 2) {
                    // Ok we implement the workaround (start at 4 as we need to keep the "file:"
                    //externalForm = externalForm.substring(4, externalFormLength - SEPARATOR.length());
                    // this approach caused %20 to appear which broke things later on.
                    String urlString = stripUrlFileColonPrefix(path, path);
                    urlString = stripSuffix(urlString, SEPARATOR, urlString);
                    file = new File(urlString);
                    try {
                        url = new URL(path);
                    } catch(MalformedURLException eat) {
                        url = null;
                    }
                }
            }
        } else if("file".equals(url.getProtocol())) {
            String path = url.getPath();
            String urlString = stripUrlFileColonPrefix(path, path);
            urlString = stripSuffix(urlString, SEPARATOR, urlString);
            file = new File(urlString);
        }
        URLAlias urlAlias = new URLAlias();
        urlAlias.url = url;    // never null
        urlAlias.file = file;  // maybe null (when not a "file:" based URL)
        return urlAlias;
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
                List<String> pathToJarFiles = JarCache.pathToJarFiles(fileName);
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
            e.printStackTrace();
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

    private void addEngine(QAbstractFileEngine engine) {
        synchronized(QClassPathEngine.class) {
            m_engines.add(engine);
        }
    }


    private static void findClassPaths() {
        synchronized(QClassPathEngine.class) {
            classpaths = new HashSet<String>();

            List<URL> cpUrls = new ArrayList<URL>();

            try {
                // FIXME: QtJambi should not mix and match the method of obtaining the current ClassLoader
                //  all use this class or all use Thread.
                ClassLoader loader = Thread.currentThread().getContextClassLoader();
                if (loader == null)
                    loader = QClassPathFileEngineHandler.class.getClassLoader();

                Enumeration<URL> urls = loader.getResources("META-INF/MANIFEST.MF");
                while (urls.hasMoreElements()) {
                    URL url = urls.nextElement();
                    if ("jar".equals(url.getProtocol())) {
                        try {
                            String f = url.getPath();
                            int bang = f.indexOf("!");
                            if (bang >= 0)
                                f = f.substring(0, bang);

                            if (f.trim().length() > 0) {
                                classpaths.add(f);
                                cpUrls.add(new URL(f));
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            String javaClassPath = System.getProperty("java.class.path");
            if(javaClassPath == null)
                javaClassPath = "";  // gets ignored below
            String paths[] = RetroTranslatorHelper.split(javaClassPath,
                                                         File.pathSeparator);

            // Only add the .jar files that are not already added...
            int k=0;
            for (String p : paths) {
                if (p.trim().length() > 0) {
                    k++; // count all paths, invalid and valid

                    String urlString = makeUrl(p);
                    boolean match = false;

                    try {
                        URL url = new URL(urlString);

                        if("file".equals(url.getProtocol())) {
                            File fileA = new File(url.getPath());
                            if(fileA.isDirectory()) {  // FIXME
                                classpaths.add(Utilities.convertAbsolutePathStringToFileUrlString(fileA));  // "file://" + "/C:/foo/bar"
                                continue;
                            }
                        }
                    } catch(Exception e) {
                        e.printStackTrace();
                        continue;
                    }

                    MyJarFile myJarFile2 = null;
                    MyJarFile myJarFile1 = null;
                    try {
                        myJarFile2 = resolveUrlToMyJarFile(urlString);
                        if(myJarFile2 != null) {
                            for (URL otherURL : cpUrls) {
                                myJarFile1 = resolveUrlToMyJarFile(otherURL);
                                if(myJarFile1 != null) {
                                    File file1 = new File(myJarFile1.getName());
                                    File file2 = new File(myJarFile2.getName());
                                    if (file1.getCanonicalPath().equals(file2.getCanonicalPath())) {
                                        match = true;
                                        break;
                                    }
                                }
                            }
                        }
                    } catch (Exception e) {   // This should probably just be IOException
                        e.printStackTrace();  // this has been so useful in finding many bugs/issues
                    } finally {
                        if(myJarFile2 != null) {
                            myJarFile2.put();
                            myJarFile2 = null;
                        }
                        if(myJarFile1 != null) {
                            myJarFile1.put();
                            myJarFile1 = null;
                        }
                    }

                    if (!match)
                        classpaths.add(urlString);
                }
            }

            // If there are no paths set in java.class.path, we do what Java does and
            // add the current directory; at least ask Java what the current directory
            // is and not Qt.
            if (k == 0) {
                // FIXME: Use JVM cwd notion
                classpaths.add("file:" + QDir.currentPath());	// CHECKME "file:///" ?
            }
        }
        JarCache.reset(classpaths);
    }

    private static MyJarFile resolveUrlToMyJarFile(URL url) throws IOException, ZipException {
        MyJarFile myJarFile = null;
        URLAlias urlAlias = checkNeedWorkaround(url);
        try {
            if(urlAlias.file != null) {  // Due to workaround
                if(urlAlias.file.isFile()) // skip dirs
                    myJarFile = new MyJarFile(urlAlias.file);
            } else {
                 myJarFile = new MyJarFile(urlAlias.url);
            }
        } catch(ZipException e) {
            // This often fails with "java.util.zip.ZipException: error in opening zip file" but never discloses the filename
            throw new ZipException(e.getMessage() + ": " + url);
        }
        return myJarFile;
    }

    private static MyJarFile resolveUrlToMyJarFile(String urlString) throws MalformedURLException, IOException, ZipException {
        URL url = new URL("jar:" + urlString.toString() + "!/");
        return resolveUrlToMyJarFile(url);
    }

    @Override
    public QAbstractFileEngineIterator beginEntryList(QDir.Filters filters, java.util.List<String> nameFilters) {
        String path = "";
        if(m_baseName.startsWith(namespaceSchemePrefix + ":"))
            path = m_baseName;
        else
            path = namespaceSchemePrefix + ":" + m_baseName;
        return new QClassPathFileEngineIterator(path, filters, nameFilters);
    }

    @Override
    public QAbstractFileEngineIterator endEntryList() {
        return null;
    }

    private QAbstractFileEngine getFirstEngine() {
        synchronized(QClassPathEngine.class) {
            if(m_engines.isEmpty() == false)
                return m_engines.getFirst();
        }
        return null;
    }
}
