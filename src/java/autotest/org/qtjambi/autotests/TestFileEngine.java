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

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.List;

import org.junit.Test;

import org.qtjambi.qt.core.QAbstractFileEngine;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.osinfo.OSInfo;

public class TestFileEngine extends QApplicationTest {
    private int byteArrayCompare(byte[] a, int aOff, byte[] b, int bOff, int len) {
        int count = 0;
        while(len > 0) {
            byte aByte = a[aOff + count];
            byte bByte = b[bOff + count];
            if(aByte != bByte)
                break;
            count++;
            len--;
        }
        return count;
    }

    // The purpose of this test is to check the JUnit testcase CLASSPATH is setup as
    // the test case needs to perform correctly.
    @Test
    public void testCheckClassPath() {
        String urlProtocol = null; 
        boolean bf = false;
        InputStream inputStream = null;
        try {
            // This should be available on the regular class path
            inputStream = TestFileEngine.class.getClassLoader().getResourceAsStream("org/qtjambi/autotests/TestClassFunctionality.jar");
            if(inputStream != null)
                bf = true;

            // It should also be available via the file:/// protocol (i.e. a directly accessible file)
            URL url = TestFileEngine.class.getClassLoader().getResource("org/qtjambi/autotests/TestClassFunctionality.jar");
            if(url != null)
                urlProtocol = url.getProtocol();
        } finally {
            if(inputStream != null) {
                try {
                    inputStream.close();
                } catch (IOException eat) {
                }
                inputStream = null;
            }
        }
        assertTrue("org/qtjambi/autotests/TestClassFunctionality.jar exists on CLASSAPTH", bf);
        // This checks it is not inside a JAR itself but directly accessible as a file
        assertEquals("org/qtjambi/autotests/TestClassFunctionality.jar exists on CLASSPATH and is file:/// URL protocol", "file", urlProtocol);
    }

    @Test
    public void run_classPathFileEngine() {
        QAbstractFileEngine.addSearchPathForResourceEngine(".");  // Hmm not sure on the merit of this cwd will be project top-level dir
        QFileInfo info = new QFileInfo("classpath:org/qtjambi/autotests/TestClassFunctionality.jar");
        assertTrue(info.exists());
        Utils.println(3, "info.absoluteFilePath() = " + info.absoluteFilePath());

        QFile af = new QFile(info.absoluteFilePath());
        assertTrue(af.exists());
        assertTrue(af.open(QIODevice.OpenModeFlag.ReadOnly));
        af.close();

        String search_path = info.canonicalFilePath();  // on windows this is in the format "C:/dir1/dir2/TestClassFunctionality.jar"
        QAbstractFileEngine.addSearchPathForResourceEngine(search_path);

        QFileInfo ne_info = new QFileInfo("classpath:TestClassFunctionality_nosuchfile.txt");
        assertFalse(ne_info.exists());

        QFileInfo ne2_info = new QFileInfo("classpath:*#TestClassFunctionality_nosuchfile.txt");
        assertFalse(ne2_info.exists());

        QFileInfo pm_info = new QFileInfo("classpath:TestClassFunctionality_picture.jpg");
        assertTrue(pm_info.exists());
        assertEquals(pm_info.size(), 11769L);

        QFileInfo pm2_info = new QFileInfo("classpath:*#TestClassFunctionality_picture.jpg");
        assertTrue(pm2_info.exists());
        assertEquals(pm2_info.size(), 11769L);

        QFile pm2_file = new QFile("classpath:*#TestClassFunctionality_picture.jpg");
        assertTrue(pm2_file.exists());
        assertTrue(pm2_file.open(QIODevice.OpenModeFlag.ReadOnly));

        final byte[] FIRST16 = {
            (byte) 0xff, (byte) 0xd8, (byte) 0xff, (byte) 0xe0,
            (byte) 0x00, (byte) 0x10, (byte) 0x4a, (byte) 0x46,
            (byte) 0x49, (byte) 0x46, (byte) 0x00, (byte) 0x01,
            (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x64,
        };
        final byte[] LAST16 = {
            (byte) 0x50, (byte) 0x47, (byte) 0x55, (byte) 0x0f,
            (byte) 0xe6, (byte) 0x1f, (byte) 0xc2, (byte) 0xeb,
            (byte) 0xde, (byte) 0xb2, (byte) 0xb2, (byte) 0xb2,
            (byte) 0x8a, (byte) 0x8b, (byte) 0xff, (byte) 0xd9
        };
        byte[] bA = new byte[16];
        // protected readData ?  why ?
        long n = pm2_file.read(bA);
        assertEquals("read() return", 16, n);
        // Read file, compare first 16 bytes to known constant
        assertEquals("compare FIRST16", byteArrayCompare(bA, 0, FIRST16, 0, FIRST16.length), FIRST16.length);
        final long size = pm2_file.size();
        assertEquals("size", size, 11769L);
        assertTrue("seek", pm2_file.seek(size - 16));
        assertEquals("seek", pm2_file.pos(), size - 16);
        n = pm2_file.read(bA);
        assertEquals("read() return 16", 16, n);
        // Read file, compare last 16 bytes to known constant
        assertEquals("compare LAST16", byteArrayCompare(bA, 0, LAST16, 0, LAST16.length), LAST16.length);
        n = pm2_file.read(bA);	// over read
        assertEquals("readData() return -1", -1, n);

        assertTrue("seek", pm2_file.seek(size - 14));  // also checks auto-rewind on non-seekable sources
        assertEquals("seek", pm2_file.pos(), size - 14);
        n = pm2_file.read(bA);  // short read
        assertEquals("read() return 14", 14, n);
        // Read file, compare last 14 bytes to known constant
        assertEquals("compare LAST14", byteArrayCompare(bA, 0, LAST16, 2, LAST16.length - 2), LAST16.length - 2);
        n = pm2_file.read(bA);	// over read
        assertEquals("read() return -1", -1, n);
        pm2_file.close();

        QPixmap pm = new QPixmap("classpath:*#TestClassFunctionality_picture.jpg");
        assertFalse(pm.isNull());  // if you fail here check your plugins are loading
        assertEquals(pm.width(), 200);
        assertEquals(pm.height(), 242);

        QLabel label = new QLabel();
        label.setPixmap(pm);
        label.show();

        info = new QFileInfo("classpath:TestClassFunctionality_test.txt");
        assertTrue(info.exists());
        assertEquals(info.size(), 8L);
        assertTrue(info.absolutePath().endsWith("#") && info.absolutePath().startsWith("classpath:"));
        assertTrue(info.absoluteFilePath().endsWith("#TestClassFunctionality_test.txt") && info.absoluteFilePath().startsWith("classpath:"));
        assertNotNull(info.absoluteDir().absolutePath().endsWith("TestClassFunctionality.jar#."));

        assertTrue(QFile.exists("classpath:TestClassFunctionality_test.txt"));

        QFile file = new QFile("classpath:TestClassFunctionality_test.txt");
        assertTrue(file.exists());

        assertTrue(file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly)));
        assertTrue(file.isOpen());

        QByteArray ba = file.readAll();
        ba.append("");
        String s = ba.toString();
        assertTrue(s.startsWith("Qt rocks"));
        assertEquals(file.bytesAvailable(), 0L);

        file.reset();
        file.seek(3);

        assertEquals(file.bytesAvailable(), 5L);

        ba = file.read(1000);
        ba.append("");
        s = ba.toString();
        assertTrue(s.startsWith("rocks"));

        file.reset();
        assertEquals(file.bytesAvailable(), 8L);
        file.seek(1);

        assertEquals(file.bytesAvailable(), 7L);
        ba = file.read(1);
        ba.append("");
        s = ba.toString();
        assertEquals(s, "t");

        assertEquals(file.bytesAvailable(), 6L);
        ba = file.read(1);
        ba.append("");
        s = ba.toString();
        assertEquals(s, " ");
        assertEquals(file.bytesAvailable(), 5L);

        file.close();
        assertTrue(!file.isOpen());

        assertTrue(file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly, QIODevice.OpenModeFlag.Text)));
        assertTrue(file.isOpen());
        file.reset();

        ba = file.readLine();
        ba.append("");
        s = ba.toString();
        assertEquals(s, "Qt rocks");

        file.close();
        assertTrue(!file.isOpen());

        info = new QFileInfo("classpath:*#TestClassFunctionality_dir");
        assertTrue(info.exists());
        assertTrue(info.isDir());

        QDir dirtwo = new QDir("classpath:TestClassFunctionality_dirtwo/");
        assertTrue(dirtwo.exists());
        // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
        List<String> direntList = dirtwo.entryList(); 
        int direntCount = direntList.size();
        if(direntCount == 2) {
            assertEquals(direntList.size(), 2);
            assertTrue(direntList.get(0).equals(".."));
            assertTrue(direntList.get(1).equals("TestClassFunctionality_dir22"));
        } else {
            assertEquals(direntList.size(), 1);
            assertTrue(direntList.get(0).equals("TestClassFunctionality_dir22"));
        }

        List<QFileInfo> dirTwoEntryInfoList = dirtwo.entryInfoList();
        if(OSInfo.isWindows()) {
            // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
            assertEquals(dirTwoEntryInfoList.size(), 2);
        } else {
            assertEquals(dirTwoEntryInfoList.size(), 1);
        }

        QDir dir = new QDir("classpath:TestClassFunctionality_dir/");
        assertTrue(dir.exists());

        if(OSInfo.isWindows()) {
            // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
            // "..", "TestClassFunctionality_dir2", "TestClassFunctionality_file_in_dir.txt"
            assertEquals(dir.entryList().size(), 3);
        } else {
            assertEquals(dir.entryList().size(), 2);
        }
        boolean found = false;
        for(String oneName : dir.entryList()) {
            if(oneName.equals("TestClassFunctionality_dir2"))
                found = true;
        }
        assertTrue(found);

        List<QFileInfo> entryInfoList = dir.entryInfoList();
        if(OSInfo.isWindows()) {
            // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
            // "..", "TestClassFunctionality_dir2", "TestClassFunctionality_file_in_dir.txt"
            assertEquals(entryInfoList.size(), 3);
        } else {
            assertEquals(entryInfoList.size(), 2);
        }

        int i = 0;
        info = null;
        while(true) {
            if(i >= entryInfoList.size())
                break;
            QFileInfo tmpInfo = entryInfoList.get(i++);
            String fileName = tmpInfo.fileName();
            if(fileName.equals(".") || fileName.equals(".."))
                continue;
            info = tmpInfo;
            break;
        }
        assertTrue(info.exists());
        assertTrue(info.isDir());
        assertEquals(info.fileName(), "TestClassFunctionality_dir2");
        String abs = info.absoluteFilePath();
        assertTrue(abs.startsWith("classpath:") && abs.endsWith("TestClassFunctionality_dir/TestClassFunctionality_dir2"));

        info = new QFileInfo("classpath:TestClassFunctionality_dir/TestClassFunctionality_dir2");
        assertTrue(info.exists());
        assertTrue(info.isDir());

        dir = new QDir("classpath:TestClassFunctionality_dir/TestClassFunctionality_dir2");
        assertTrue(dir.exists());
        assertTrue(dir.isReadable());
        assertTrue(!dir.isRoot());
        assertEquals(dir.entryList().size(), 1);

        file = new QFile("classpath:TestClassFunctionality_dir/TestClassFunctionality_dir2/TestClassFunctionality_indir.txt");
        assertTrue(file.exists());
        assertTrue(file.permissions().isSet(QFile.Permission.ReadUser));
        assertEquals(file.size(), 13L);

        QAbstractFileEngine.removeSearchPathForResourceEngine(search_path);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestFileEngine.class.getName());
    }
}
