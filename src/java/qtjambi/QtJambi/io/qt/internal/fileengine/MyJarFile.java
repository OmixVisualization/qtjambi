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

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.JarURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.zip.ZipEntry;

// Package private class and methods
class MyJarFile {
    private URLConnection urlConnection;  // do we need to keep this around ?
    private File fileToJarFile;    // we store this object type to differentiate between URLs and direct File IO.
    private URL urlToJarFile;  // we save this to allow for close/reopen based on just this handle
    private JarFile jarFile;
    private int refCount;

    MyJarFile(File fileToJarFile) throws IOException {
        this.fileToJarFile = fileToJarFile;
        openInternal();
    }

    MyJarFile(URL urlToJarFile) throws IOException {
        this.urlToJarFile = urlToJarFile;
        openInternal();
    }

    private void openInternal() throws IOException {
        if(fileToJarFile != null) {  // Direct File I/O Jar file
            jarFile = new JarFile(fileToJarFile);
        } else {
            urlConnection = urlToJarFile.openConnection();
            if((urlConnection instanceof JarURLConnection) == false) {
                IOException thr = new IOException("not a JarURLConnection: " + urlConnection.getClass().getName());
                urlConnection = null;  // we only keep handle when we have active Jar open
                throw thr;
            }
            JarURLConnection jarUrlConnection = (JarURLConnection) urlConnection;
            jarFile = jarUrlConnection.getJarFile();
        }
        refCount = 1;
    }

    // This method may never throw an exception
    void get() {
        synchronized(this) {
            refCount++;
        }
    }

    // This method must cause a double increment on the reopen() case
    // Returns the previous refCount, so 0 means we just reopened, non-zero means we did get()
    int getOrReopen() throws IOException {
        int oldRefCount;
        synchronized (this) {
            oldRefCount = refCount;
            if(refCount <= 0)
                reopen();
            get();
        }
        return oldRefCount;
    }

    // This method may never throw an exception
    void put() {
        JarFile closeJarFile = null;
        synchronized(this) {
            refCount--;
            if(refCount == 0) {
                closeJarFile = jarFile;
                jarFile = null;
            }
        }
        if(closeJarFile != null) {
            try {
                closeJarFile.close();
            } catch(IOException eat) {
            }
            if(urlConnection != null) {
                urlConnection = null;
            }
        }
    }

    void reopen() throws IOException {
        if(jarFile != null)
            throw new IOException("jarFile already open");
        openInternal();
    }

    String getName() {
        return jarFile.getName();
    }

    Enumeration<JarEntry> entries() {
        return jarFile.entries();
    }

    JarEntry getJarEntry(String name) {
        return jarFile.getJarEntry(name);
    }

    InputStream getInputStream(ZipEntry ze) throws IOException {
        return jarFile.getInputStream(ze);
    }
}
