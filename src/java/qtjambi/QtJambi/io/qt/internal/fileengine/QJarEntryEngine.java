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

import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Collection;
import java.util.Date;
import java.util.Enumeration;
import java.util.GregorianCalendar;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.JarEntry;

import io.qt.QtUtilities;
import io.qt.core.QDate;
import io.qt.core.QDateTime;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QTime;
import io.qt.core.internal.QAbstractFileEngine;

// TODO: We this is re-worked we want to separate the resolution process for finding
//   locations to search, from the access/usage of a particular thing.
// We can then have a standard classpath resolver
// Have filter hierarchy/package/name/extension from a select group of JARs.
// Allow application writers to create their own custom resolvers.
// Have standard Java security ClassPath access, have this special kind, handle JAR, file, directory, HTTP

@Deprecated
class QJarEntryEngine extends QAbstractFileEngine implements QClassPathEntry
{
    // private String m_classPathEntryFileName;
    // private String m_jarFileName;
    private String m_entryFileName;

    private JarEntry m_entry;
    private MyJarFile m_myJarFile;

    private InputStream m_stream;
//    private BufferedReader m_reader;  // FIXME: Why do we even have this ?  m_stream will do right?

    private long m_pos;
    private int m_openMode;
    private boolean m_valid;
    private boolean m_directory;
    private String m_name;
    private boolean m_eof;
    private boolean m_closed;


    // FIXME: This JarFile needs to be a unique handle that does sharable accounting for JarCache
    // JAVADOC: Callers responsibiliy to transfer the reference on myJarFile to us during this ctor
    public QJarEntryEngine(MyJarFile myJarFile, String fileName, boolean isDirectory) {
        m_pos = -1;
        // we presume we already have a reference on myJarFile and the ownership
        //  of that reference is transfered to this instance.  So we don't increment here.
        m_myJarFile = myJarFile;
        m_directory = isDirectory;
        setFileName(fileName);
        WeakReference<QJarEntryEngine> thisRef = new WeakReference<>(this);
        QtUtilities.getSignalOnDispose(this).connect(()->{
        	QJarEntryEngine _this = thisRef.get();
        	if(_this!=null) {
        		_this.close();

        		_this.m_entry = null;

                if (!_this.m_closed) {
                	_this.m_myJarFile.put();
                    // FIXME: Do a put/deref/release for JarCache
                	_this.m_closed = true;	// FUTILE
                }
        	}
        });
    }

    // transfers ownership of reference count back to caller of this method
    //  this presume the caller has a handle to the MyJarFile already
    // FIXME: Maybe we should try to get rid of this method and object state to simplify API, i.e.
    //  once a MyJarFile has been handed to us, we always own it, so all the caller can do now is
    //  ask us to close/dispose/cleanup throw us away.
    void disown() {
        // Stops disposed() from trying to close handle
        m_myJarFile = null;
        m_closed = true;
    }

    @Override
    public void setFileName(String fileName) {
        m_entry = null;
        if (m_closed)
             return;

        if (fileName.length() == 0) {
            m_entryFileName = "";
            m_name = "";
            m_valid = true;
            m_directory = true;
            return;
        }

        m_entryFileName = fileName;
        m_entry = m_myJarFile.getJarEntry(m_entryFileName);

        if (m_entry == null && !m_directory) {
            m_valid = false;
        } else {
            if (m_entry == null)
                m_name = fileName;
            else
               m_name = m_entry.getName();
            m_valid = true;
        }
    }

    public String classPathEntryName() {
        if(m_closed)
            return null;
        return m_myJarFile.getName();
    }

    public boolean isValid() {
        return m_valid;
    }

    // FIXME: we need to use exception handling in here
    @Override
    public boolean copy(String newName) {
        final int BUFFER_SIZE = 64*1024;	// 64Kb will do us
        byte[] buffer = new byte[BUFFER_SIZE];

        QFile newFile = new QFile(newName);
        if (newFile.exists())
            return false;

        if (!open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly)))
            return false;

        if (!newFile.open(new QFile.OpenMode(QFile.OpenModeFlag.WriteOnly))) {
            closeInternal();
            return false;
        }

        long sz = size();
        long i = 0;
        int bytes_to_read = 0;

        if (sz > 0) {
            do {
                bytes_to_read = (int) Math.min(sz - i, BUFFER_SIZE);
                bytes_to_read = (int) read(buffer);

                byte bytes[] = Arrays.copyOf(buffer, bytes_to_read);
                if (bytes_to_read > 0 && newFile.write(bytes) != bytes_to_read)
                    return false;
            } while (i < sz && bytes_to_read > 0);
        }

        newFile.close();
        if (!closeInternal())
            return false;

        return (i == sz);
    }

    // Hmm why are these different virtual for AbstractFileEngine and IODevice ?
    //@Override
    public boolean reset() {
        if (m_stream == null)
            return false;  // not open
        if (m_pos == 0)
            return true;  // already open and at start
        return reopen();
    }

    private boolean reopen() {
        if (m_stream == null)
            return false;  // not open
        QIODevice.OpenMode om = new QIODevice.OpenMode(m_openMode);  // saved OpenMode
        if(closeInternal())
            return open(om);
        return false;
    }

    // Hmm why are these different virtual for AbstractFileEngine and IODevice ?
    // Want to make this public
    //@Override
    @SuppressWarnings("unused")
	private boolean atEnd_internal() {
        return m_eof;
    }

    @Override
    public boolean setPermissions(int perms) {
        return false;  // IODevice is immutable
    }

    @Override
    public boolean caseSensitive() {
        return true;
    }

    private boolean closeInternal() {
        boolean bf = false;

        // FIXME: close m_reader

        if(m_stream != null) {
            try {
                m_stream.close();
            } catch(IOException e) {
            } finally {
                m_stream = null;
                m_myJarFile.put();  // decrement reference
                bf = true;  // was open now closed
            }
        }

        return bf;
    }

    @Override
    public boolean close() {
        boolean bf = closeInternal();

        if(!m_closed) {
            // We really want to do this some how and not leave it to disposed()
            m_myJarFile.put();  // the reference this instance already had on construction
            // We do not null the reference here, since we could need to reopen() on handle
            // so we record the state separately with boolean m_closed flag.
            m_closed = true;
        }

        return bf;
    }

    @Override
    public List<String> entryList(QDir.Filters filters, Collection<String> filterNames) {
        if (!m_directory) {
             return new LinkedList<String>();
        }

        List<String> result = new LinkedList<String>();

        if (!filters.isSet(QDir.Filter.NoDotAndDotDot) && filters.isSet(QDir.Filter.Dirs)) {
            result.add(".");
            if (m_entryFileName.length() > 0)
                result.add("..");
        }


        // Default to readable
        if (!filters.isSet(QDir.Filter.Readable, QDir.Filter.Writable, QDir.Filter.Executable))
            filters.set(QDir.Filter.Readable);

        String mentryName = m_name;
        if (!mentryName.endsWith("/") && mentryName.length() > 0)
            mentryName = mentryName + "/";

        Enumeration<JarEntry> entries = m_myJarFile.entries();

        HashSet<String> used = new HashSet<String>();
        while (entries.hasMoreElements()) {
            JarEntry entry = entries.nextElement();

            String entryName = entry.getName();

            // Must be inside this directory
            if (entryName.length() <= mentryName.length() || !mentryName.equals(entryName.substring(0, mentryName.length())) || mentryName.equals(entryName))
                continue;

            // Only one level
            boolean isDir;
            int pos = entryName.indexOf("/", mentryName.length());
            if (pos > 0) {
                entryName = entryName.substring(0, pos);
                isDir = true;
            } else {
                isDir = entry.isDirectory();
                if (!isDir)
                    isDir = QClassPathEngine.checkIsDirectory(m_myJarFile, entry);
            }

            if (!filters.isSet(QDir.Filter.Readable))
                continue ;

            if (!filters.isSet(QDir.Filter.Dirs) && isDir)
                continue ;

            if (!filters.isSet(QDir.Filter.Files) && !isDir)
                continue ;

            if (filterNames.size() > 0) {
                if ((!isDir || !filters.isSet(QDir.Filter.AllDirs))
                    && (!QDir.match(filterNames, entryName.substring(mentryName.length())))) {
                    continue;
                }
            }

            if (entryName.endsWith("/") && entryName.length() > 1)
                entryName = entryName.substring(0, entryName.length() - 1);

            entryName = entryName.substring(mentryName.length());

            if (!used.contains(entryName)) {
                used.add(entryName);
                result.add(entryName);
            }
        }

        return result;
    }

    @Override
    public FileFlags fileFlags(FileFlags type) {
        try {
            int flags = 0;

            QFileInfo info = new QFileInfo(m_myJarFile.getName());
             if (info.exists()) {
//                 flags |= info.permissions().value()
//                          & (FileFlag.ReadOwnerPerm.value()
//                             | FileFlag.ReadGroupPerm.value()
//                             | FileFlag.ReadOtherPerm.value()
//                             | FileFlag.ReadUserPerm.value());
            	 if(info.isReadable()){
            		 flags |= FileFlag.ReadUserPerm.value();
            	 }
            	 if(info.isWritable()){
            		 flags |= FileFlag.WriteUserPerm.value();
            	 }
            	 if(info.isExecutable()){
            		 flags |= FileFlag.ExeUserPerm.value();
            	 }
             }

             if (m_directory)
                 flags |= FileFlag.DirectoryType.value();
             else
                 flags |= FileFlag.FileType.value();


             return new FileFlags((flags | FileFlag.ExistsFlag.value()) & type.value());
        } catch (Exception e) {
            java.util.logging.Logger.getLogger("io.qt.internal.fileengine").log(java.util.logging.Level.SEVERE, "", e);
            return null;
        }
    }

    @Override
    public String fileName(FileName file) {
        String entryFileName = m_entryFileName;

        String s;
        if (file == FileName.LinkName) {
            s = "";
        } else if (file == FileName.DefaultName
                || file == FileName.AbsoluteName
                || file == FileName.CanonicalName) {
            s = QClassPathResourceManager.FileNamePrefix1 + m_myJarFile.getName() + QClassPathResourceManager.FileNameDelim + entryFileName;
        } else if (file == FileName.BaseName) {
            int pos = m_entryFileName.lastIndexOf("/");
            s = pos >= 0 ? m_entryFileName.substring(pos + 1) : entryFileName;
        } else if (file == FileName.PathName) {
            int pos = m_entryFileName.lastIndexOf("/");
            s = pos > 0 ? m_entryFileName.substring(0, pos) : "";
        } else if (file == FileName.CanonicalPathName || file == FileName.AbsolutePathName) {
            s = QClassPathResourceManager.FileNamePrefix1 + m_myJarFile.getName() + QClassPathResourceManager.FileNameDelim + fileName(FileName.PathName);
        } else {
            throw new IllegalArgumentException("Unknown file name type: " + file);
        }
        return s;
    }

    @Override
    public QDateTime fileTime(FileTime time) {
        if (m_entry == null) {
            QFileInfo info = new QFileInfo(m_myJarFile.getName());

            if (info.exists())
                return info.lastModified();
            else
                return new QDateTime();  // the current time
        }

        long tm = m_entry.getTime();
        if (tm == -1)
            return new QDateTime();  // the current time

        Calendar calendar = new GregorianCalendar();
        calendar.setTime(new Date(tm));

        return new QDateTime(new QDate(calendar.get(Calendar.YEAR),
                                       calendar.get(Calendar.MONTH) + 1,
                                       calendar.get(Calendar.DAY_OF_MONTH)),
                             new QTime(calendar.get(Calendar.HOUR_OF_DAY),
                                       calendar.get(Calendar.MINUTE),
                                       calendar.get(Calendar.SECOND),
                                       calendar.get(Calendar.MILLISECOND)));
    }

    @Override
    public boolean link(String newName) {
        return false;  // IODevice is immutable
    }

    @Override
    public boolean mkdir(String dirName, boolean createParentDirectories) {
        return false;  // IODevice is immutable
    }

    @Override
    public boolean open(QIODevice.OpenMode openMode) {
        boolean bf = false;
        closeInternal();  // reset state to open again

        if (m_entry != null) {
            if (!openMode.isSet(QIODevice.OpenModeFlag.WriteOnly) && !openMode.isSet(QIODevice.OpenModeFlag.Append)) {
                int oldRefCount = -1;
                try {
                    // There is a usage case where the application may have called close() on the handle and is now calling open() again.
                    // This can also happen implicitly if we need to rewind() on the Jar stream
                    oldRefCount = m_myJarFile.getOrReopen();  // increment reference while we have stream open

                    m_stream = m_myJarFile.getInputStream(m_entry);
                    if (m_stream != null) {
                        //if (openMode.isSet(QIODevice.OpenModeFlag.Text))
                        //    m_reader = new BufferedReader(new InputStreamReader(m_stream));
                        m_pos = 0;
                        m_openMode = openMode.value();
                        m_closed = false;  // this maybe due to reopen() or first open()
                        bf = true;
                    }
                } catch (IOException eat) {  // cause a return false
                } finally {
                    if(!bf) {    // we failed but did we obtained reference?
                        // if oldRefCount==0 then we just did a reopen() and so we need to put() it as well as the get()
                        // if oldRefCount>0 then we only need to undo the get() operation
                        if(oldRefCount >= 0) {
                            if(oldRefCount == 0)
                                m_myJarFile.put(); // rollback extra reference
                            m_myJarFile.put(); // rollback reference
                        }
                    }
                }
            }
        }
        return bf;
    }

    @Override
    public long pos() {
        return m_pos;
    }

    @Override
    public int read(byte[] data) {
        if (m_stream == null)
            return -1;
        int maxlen = data.length;

        byte[] b = new byte[(int)maxlen];

        int bytes_read = 0;
        try {
            int read = -1;
            while (m_stream.available() > 0 && bytes_read < maxlen) {
                read = m_stream.read(b, 0, maxlen - bytes_read);
                if (read > 0) {
                    for (int i=0; i<read; ++i)
                        data[i + bytes_read] = b[i];
                    bytes_read += read;
                } else if (read  == -1) {
                    break;
                }
            }
            // Qt contract is that we return -1 on EoF/EoS
            if (read < 0 && bytes_read == 0) {
                m_eof = true;
                return -1;
            }
        } catch (IOException e) {
            return -1;
        }

        m_pos += bytes_read;
        return bytes_read;
    }

    @Override
    public int readLine(byte[] data) {
        if (m_stream == null /*|| m_reader == null*/)
            return -1;

        int maxlen = data.length;
        int bytes_read = 0;
        try {
            int read = -1;
            while (m_stream.available() > 0 && bytes_read < maxlen) {
                read = m_stream.read();
                if (read == -1)
                    break;

                data[bytes_read++] = (byte) read;
                // We follow how QIODevice::readLine behaviour here.
                // FIXME: We should have testcase to verify it, compare Qt API with our implementation.
                // CHECKME: This is how QIODevice::readLine almost works, except (if I understand docs
                //  correctly) that for <CR><NL> sequence on windows it might replace with <NL>.
                if (read == '\n')
                    break;
            }
            // Qt contract is that we return -1 on EoF/EoS
            if (read < 0 && bytes_read == 0) {
                m_eof = true;
                return -1;
            }
        } catch (IOException e) {
            return -1;
        }

        m_pos += bytes_read;
        return bytes_read;
    }

    @Override
    public boolean remove() {
        return false;  // IODevice is immutable
    }

    @Override
    public boolean rename(String newName) {
        return false;  // IODevice is immutable
    }

    @Override
    public boolean rmdir(String dirName, boolean recursive) {
        return false;  // IODevice is immutable
    }

    @Override
    public boolean seek(long offset) {
        if(offset < 0)
            return false;

        try {
            if (!open(new QIODevice.OpenMode(m_openMode)))  // open() will automatically force a close()
                return false;
            if (offset < m_pos) {
                if (!reset());  // auto-rewind
                    return false;
            }

            while (m_pos < offset) {
                long skipBytesRemaining = Math.min(offset - m_pos, Integer.MAX_VALUE);

                // InputStream#skip(long) may not skip all the requested bytes in a single invocation
                long skipBytesActual = m_stream.skip(skipBytesRemaining);
                if(skipBytesActual < 0)
                    throw new IOException("InputStream#skip() = " + skipBytesActual);
                m_pos += skipBytesActual;	// The actual number of bytes skipped
            }
            return true;
        } catch (IOException e) {
        }

        return false;
    }

    @Override
    public String owner(FileOwner owner) {
        return "";
    }

    @Override
    public int ownerId(FileOwner owner) {
        return -2;
    }

    @Override
    public boolean isRelativePath() {
        return false;
    }

    @Override
    public boolean isSequential() {
        return false;  // We allow size/seek/pos etc.. so we return false here.
    }

    @Override
    public boolean setSize(long sz) {
        return false;  // IODevice is read-only
    }

    @Override
    public long size() {
        // Check with the Qt source, should we return -1 ?
        long size = m_entry == null ? 0 : m_entry.getSize();
        return size;
    }

    @Override
    public int write(byte[] data) {
        return -1;  // IODevice is read-only
    }
}
