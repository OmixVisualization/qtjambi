
package org.qtjambi.qt.internal.fileengine;

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
