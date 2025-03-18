/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.List;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.internal.TestUtility;
import io.qt.core.QByteArray;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.core.QLocale;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QRegularExpression;
import io.qt.core.QResource;
import io.qt.core.QString;
import io.qt.core.QStringList;
import io.qt.core.QVersionNumber;
import io.qt.core.Qt;
import io.qt.core.internal.QAbstractFileEngineHandler;
import io.qt.core.internal.QFSFileEngine;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPixmap;
import io.qt.widgets.QLabel;

public class TestFileEngine extends ApplicationInitializer {
	private static String search_path1 = null;
	private static java.io.File tmpFile1 = null;
	private static File tmpFile2 = null;
	private static String search_path2 = null;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		URL.setURLStreamHandlerFactory(protocol->{
			if("qjtest".equals(protocol)) {
				return new URLStreamHandler() {
					@Override
					protected URLConnection openConnection(URL u) throws IOException {
						return new URLConnection(u) {
							final byte[] file;
							{
								file = getURL().getPath().substring(getURL().getPath().indexOf('/')+1).getBytes();
								setDoInput(true);
							}
							@Override
							public void connect() throws IOException {
							}

							@Override
							public InputStream getInputStream() throws IOException {
								return new ByteArrayInputStream(file);
							}

							@Override
							public long getContentLengthLong() {
								return file.length;
							}
						};
					}
				};
			}
			return null;
		});
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@AfterClass
    public static void testDispose() throws Exception {
		if(search_path1!=null) {
			runGC();
			QResource.removeClassPath(search_path1);
			if(tmpFile1!=null && !tmpFile1.delete()) {
				tmpFile1.deleteOnExit();
			}
		}
		if(search_path2!=null) {
			runGC();
			QResource.removeClassPath(search_path2);
			if(tmpFile2!=null && !tmpFile2.delete()) {
				tmpFile2.deleteOnExit();
			}
		}
		ApplicationInitializer.testDispose();
	}
	
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
    
    @Test
    public void testCustomURL() {
    	QResource.addClassPath("qjtest:container1/");
    	try {
    		QFile file = new QFile(":file.txt");
    		Assert.assertTrue(file.exists());
    		Assert.assertTrue(file.open(QFile.OpenModeFlag.ReadOnly));
    		Assert.assertEquals("file.txt", file.readAll().toString());
    		file.close();
    	}finally {
    		QResource.removeClassPath("qjtest:container1/");
    	}
		QFile file = new QFile("classpath:qjtest:container2/#file2.txt");
		Assert.assertTrue(file.exists());
		Assert.assertTrue(file.open(QFile.OpenModeFlag.ReadOnly));
		Assert.assertEquals("file2.txt", file.readAll().toString());
		file.close();
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
            inputStream = TestFileEngine.class.getResourceAsStream("TestClassFunctionality.jar");
            if(inputStream != null)
                bf = true;

            // It should also be available via the file:/// protocol (i.e. a directly accessible file)
            URL url = TestFileEngine.class.getResource("TestClassFunctionality.jar");
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
        assertTrue("io/qt/autotests/TestClassFunctionality.jar exists on CLASSPATH", bf);
        // This checks it is not inside a JAR itself but directly accessible as a file
        assertTrue("io/qt/autotests/TestClassFunctionality.jar exists on CLASSPATH and is file:/// URL protocol", "file".equals(urlProtocol) || "jar".equals(urlProtocol));
    }

//    @SuppressWarnings("deprecation")
	@Test
    public void testClassPathFileEngine() {
    	{
	        QFileInfo info = new QFileInfo("classpath:io/qt/autotests/TestClassFunctionality.jar");
	        assertTrue(info.exists());
	        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "info.absoluteFilePath() = " + info.absoluteFilePath());
	
	        QFile af = new QFile(info.absoluteFilePath());
	        assertTrue(af.exists());
	        assertTrue(af.open(QIODevice.OpenModeFlag.ReadOnly));
	        af.close();
	    }
    	try{
    		URL url = TestFileEngine.class.getResource("TestClassFunctionality.jar");
        	Assume.assumeTrue(url!=null);
        	if(url.getProtocol().equals("file")) {
        		search_path1 = new java.io.File(url.toURI()).getAbsolutePath();
        	}else {
        		File userDir = new File(System.getProperty("user.dir", ""));
        		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
        			userDir = new File(QDir.tempPath());
        		}
	        	userDir.mkdirs();
	        	tmpFile1 = new File(userDir, "TestFileEngine_testClassPathFileEngine_pid" + TestUtility.processName() + ".jar");
        		try (InputStream is = url.openStream()){
            		Files.copy(is, tmpFile1.toPath(), StandardCopyOption.REPLACE_EXISTING);
            	}
        		search_path1 = tmpFile1.getAbsolutePath();
        	}
    	} catch (Exception e) {
    		e.printStackTrace();
    		Assume.assumeTrue("TestClassFunctionality.jar not found on classpath", false);
        }
        QResource.addClassPath(search_path1);
        
        {
	        QFileInfo ne_info = new QFileInfo("classpath:TestClassFunctionality_nosuchfile.txt");
	        assertFalse(ne_info.exists());
        }

        {
	        QFileInfo ne2_info = new QFileInfo("classpath:*#TestClassFunctionality_nosuchfile.txt");
	        assertFalse(ne2_info.exists());
        }

        {
	        QFileInfo pm_info = new QFileInfo("classpath:TestClassFunctionality_picture.jpg");
	        assertTrue(pm_info.exists());
	        assertEquals(pm_info.size(), 11769L);
        }

        {
	        QFileInfo pm2_info = new QFileInfo("classpath:*#TestClassFunctionality_picture.jpg");
	        assertTrue(pm2_info.exists());
	        assertEquals(pm2_info.size(), 11769L);
        }

        {
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
        }

        Runnable finalAction = null;
        {
	        QPixmap pm = new QPixmap("classpath:*#TestClassFunctionality_picture.jpg");
	        assertFalse(pm.isNull());  // if you fail here check your plugins are loading
	        assertEquals(pm.width(), 200);
	        assertEquals(pm.height(), 242);
	        if(QGuiApplication.primaryScreen()!=null) {
		        QLabel label = new QLabel();
		        label.setPixmap(pm);
		        label.show();
		        finalAction = label::hide;
	        }
        }
        try {
        	{
	        	QFileInfo info = new QFileInfo("classpath:TestClassFunctionality_test.txt");
		        assertTrue(info.exists());
		        assertEquals(info.size(), 8L);
		        assertEquals("classpath:", info.absolutePath());
		        assertEquals("classpath:TestClassFunctionality_test.txt", info.absoluteFilePath());
        	}
		        
	        assertTrue(QFile.exists("classpath:TestClassFunctionality_test.txt"));
	
	        {
		        QFile file = new QFile("classpath:TestClassFunctionality_test.txt");
		        assertTrue(file.exists());
		
		        assertTrue(file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly)));
		        assertTrue(file.isOpen());
		
		        QByteArray ba = file.readAll();
		        ba.append(new QByteArray(""));
		        String s = ba.toString();
		        assertTrue(s.startsWith("Qt rocks"));
		        assertEquals(file.bytesAvailable(), 0L);
		
		        file.reset();
		        file.seek(3);
		
		        assertEquals(file.bytesAvailable(), 5L);
		
		        ba = file.read(1000);
		        ba.append(new QByteArray(""));
		        s = ba.toString();
		        assertTrue(s.startsWith("rocks"));
		
		        file.reset();
		        assertEquals(file.bytesAvailable(), 8L);
		        file.seek(1);
		
		        assertEquals(file.bytesAvailable(), 7L);
		        ba = file.read(1);
		        s = ba.toString();
		        assertEquals("t", s);
		        ba.append(new QByteArray(""));
		        s = ba.toString();
		        assertEquals("t", s);
		
		        assertEquals(file.bytesAvailable(), 6L);
		        ba = file.read(1);
		        ba.append(new QByteArray(""));
		        s = ba.toString();
		        assertEquals(s, " ");
		        assertEquals(file.bytesAvailable(), 5L);
		
		        file.close();
		        assertTrue(!file.isOpen());
		
		        assertTrue(file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly, QIODevice.OpenModeFlag.Text)));
		        assertTrue(file.isOpen());
		        file.reset();
		
		        ba = file.readLine();
		        ba.append(new QByteArray(""));
		        s = ba.toString();
		        assertEquals(s, "Qt rocks");
		
		        file.close();
		        assertTrue(!file.isOpen());
	        }
	
	        {
		        QFileInfo info = new QFileInfo("classpath:*#TestClassFunctionality_dir");
		        assertTrue(info.exists());
		        assertTrue(info.isDir());
	        }
	
	        {
		        QDir dirtwo = new QDir("classpath:TestClassFunctionality_dirtwo/");
		        assertTrue(dirtwo.exists());
		        // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
		        QStringList direntList = dirtwo.entryList(); 
		        int direntCount = direntList.size();
		        if(direntCount == 2) {
		            assertEquals("..", direntList.get(0));
		            assertEquals("TestClassFunctionality_dir22", direntList.get(1));
		        } else {
		            assertEquals("number of files in directory classpath:TestClassFunctionality_dirtwo/", direntList.size(), 1);
		            assertEquals("TestClassFunctionality_dir22", direntList.get(0));
		        }

		        List<QFileInfo> dirTwoEntryInfoList = dirtwo.entryInfoList();
		        if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)
		        		&& QLibraryInfo.version().compareTo(new QVersionNumber(6,8,0))<0) {
		            // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
		            assertEquals(2, dirTwoEntryInfoList.size());
		        } else {
		            assertEquals(1, dirTwoEntryInfoList.size());
		        }
	        }
	
	        QDir dir = new QDir("classpath:TestClassFunctionality_dir/");
	        assertTrue(dir.exists());
	
	        if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)
	        		&& QLibraryInfo.version().compareTo(new QVersionNumber(6,8,0))<0) {
	            // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
	            // "..", "TestClassFunctionality_dir2", "TestClassFunctionality_file_in_dir.txt"
	            assertEquals("number of files in directory classpath:TestClassFunctionality_dir/", dir.entryList().size(), 3);
	        } else {
	            assertEquals("number of files in directory classpath:TestClassFunctionality_dir/", dir.entryList().size(), 2);
	        }
	        boolean found = false;
	        for(String oneName : dir.entryList()) {
	            if(oneName.equals("TestClassFunctionality_dir2"))
	                found = true;
	        }
	        assertTrue(found);
	
	        List<QFileInfo> entryInfoList = dir.entryInfoList();
	        if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)
	        		&& QLibraryInfo.version().compareTo(new QVersionNumber(6,8,0))<0) {
	            // CHECKME FIXME On Windows we see ".." entry, we should explain why this is different and if necessary fix something
	            // "..", "TestClassFunctionality_dir2", "TestClassFunctionality_file_in_dir.txt"
	            assertEquals(3, entryInfoList.size());
	        } else {
	            assertEquals(2, entryInfoList.size());
	        }
	
	        int i = 0;
	        QFileInfo info = null;
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
	        dir.dispose();
	
	        QFile file = new QFile("classpath:TestClassFunctionality_dir/TestClassFunctionality_dir2/TestClassFunctionality_indir.txt");
	        assertTrue(file.exists());
	        assertTrue(file.permissions().testFlag(QFile.Permission.ReadUser));
	        assertEquals(file.size(), 13L);
	
        }finally {
        	runGC();
        	if(finalAction!=null)
        		finalAction.run();
	        QResource.removeClassPath(search_path1);
	        if(tmpFile1!=null && tmpFile1.exists()) {
	        	if(!tmpFile1.delete()) {
	        		tmpFile1.deleteOnExit();
	        	}
	        }
        }
    }
	
	@Test
    public void testResourceFileEngine() throws Exception {
		QtUtilities.initializePackage(io.qt.autotests.generated.General.class);
		QFileInfo info = new QFileInfo(":io/qt/autotests/TestClassFunctionality.jar");
        assertTrue(info.exists());
        info.dispose();
        
        info = new QFileInfo(":io/qt/autotests/generated/");
		assertTrue(info.isDir());
		QDir dir = new QDir(":io/qt/autotests/generated/");
		QStringList content = dir.entryList(QDir.Filter.NoFilter.asFlags(), QDir.SortFlag.NoSort.asFlags());
		dir.dispose();
		if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
			assertTrue("directory :io/qt/autotests/generated/ contains class", content.contains("General.class"));
		assertTrue("directory :io/qt/autotests/generated/ contains image", content.contains("qjlogo.png"));
        info.dispose();
		
        info = new QFileInfo(":io/qt/autotests/generated/qjlogo.png");
		assertTrue(info.exists());
        info.dispose();
		
        info = new QFileInfo(":io/qt/autotests/generated/resource.txt");// this is the alias
		assertFalse(info.exists());
        info.dispose();
		
        info = new QFileInfo(":io/qt/autotests/generated/Text");
		assertTrue(info.exists());
		{
			QFile file = new QFile();
			try {
				file.setFileName(info.absoluteFilePath());
				file.open(QIODevice.OpenModeFlag.ReadOnly);
				assertEquals("Hello World!", file.readAll().toString());
			}finally {
				file.close();
				file.dispose();
			}
		}
		info.dispose();
		
		if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
	        info = new QFileInfo(":io/qt/autotests/generated/General.class");
			assertTrue(info.exists());
	        info.dispose();
		}
		
        URL url = TestFileEngine.class.getResource("resources.jar");
        try {
	        if(url.getProtocol().equals("file")) {
	        	search_path2 = new java.io.File(url.toURI()).getAbsolutePath();
	        }else {
	        	File userDir = new File(System.getProperty("user.dir", ""));
	        	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
        			userDir = new File(System.getProperty("java.io.tmpdir", ""));
        		}
	        	userDir.mkdirs();
	        	tmpFile2 = new File(userDir, "TestFileEngine_testResourceFileEngine_pid" + TestUtility.processName() + ".jar");
	        	try (InputStream is = url.openStream()){
	        		Files.copy(is, tmpFile2.toPath(), StandardCopyOption.REPLACE_EXISTING);
	        	}
	        	search_path2 = tmpFile2.getAbsolutePath();
	        }
	        QResource.addClassPath(search_path2);
	        
	        QLocale.setDefault(new QLocale(QLocale.Language.Spanish, QLocale.Country.Argentina));
	        info = new QFileInfo(":io/qt/qtjambi/resources/Text.txt");
			assertTrue(info.exists());
			{
				QFile file = new QFile();
				try {
					file.setFileName(info.absoluteFilePath());
					file.open(QIODevice.OpenModeFlag.ReadOnly);
					assertEquals("Hello World!", file.readAll().toString());
				}finally {
					file.close();
					file.dispose();
				}
			}
			info.dispose();
			
			QLocale.setDefault(new QLocale(QLocale.Language.German, QLocale.Country.Germany));
	        info = new QFileInfo(":io/qt/qtjambi/resources/Text.txt");
			assertTrue(info.exists());
			{
				QFile file = new QFile();
				try {
					file.setFileName(info.absoluteFilePath());
					file.open(QIODevice.OpenModeFlag.ReadOnly);
					assertEquals("Hallo Welt!", file.readAll().toString());
				}finally {
					file.close();
					file.dispose();
				}
			}
			info.dispose();
			
			QLocale.setDefault(new QLocale(QLocale.Language.German, QLocale.Country.Austria));
	        info = new QFileInfo(":io/qt/qtjambi/resources/Text.txt");
			assertTrue(info.exists());
			{
				QFile file = new QFile();
				try {
					file.setFileName(info.absoluteFilePath());
					file.open(QIODevice.OpenModeFlag.ReadOnly);
					assertEquals("Hallo Welt!", file.readAll().toString());
				}finally {
					file.close();
					file.dispose();
				}
			}
			info.dispose();
			
			QDir.addSearchPath("text", ":io/qt/qtjambi/resources/");
			
			QLocale.setDefault(new QLocale(QLocale.Language.German, QLocale.Country.Austria));
	        info = new QFileInfo("text:Text.txt");
			assertTrue(info.exists());
			{
				QFile file = new QFile();
				try {
					file.setFileName(info.absoluteFilePath());
					file.open(QIODevice.OpenModeFlag.ReadOnly);
					assertEquals("Hallo Welt!", file.readAll().toString());
				}finally {
					file.close();
					file.dispose();
				}
			}
			info.dispose();
        }finally {
        	runGC();
        	if(search_path2!=null) {
        		QResource.removeClassPath(search_path2);
    			if(tmpFile2!=null && tmpFile2.exists()) {
    				if(!tmpFile2.delete()) {
    					tmpFile2.deleteOnExit();
    				}
    			}
        	}
		}
	}
	
	@Test
    public void testDir(){
		QDir dir = new QDir(":/");
		for (String file : dir.entryList()) {
			System.out.println(file);
		}
	}
	
	@Test
    public void testStartsWithFileEngine() {
		String currentPath = QDir.current().absolutePath();
		QFileInfo fileInfo = new QFileInfo("FileEngineTest:"+currentPath);
		assertFalse(fileInfo.exists());
		assertFalse(fileInfo.isDir());
		boolean[] used = {false};
		QAbstractFileEngineHandler handler = QAbstractFileEngineHandler.fromFileNameTest(file->{
				used[0] = true;
				return new QFSFileEngine(file.substring(15));
			}, QString.Comparison.StartsWith, "FileEngineTest:");
		assertTrue("handler is null", handler!=null);
		try {
			fileInfo = new QFileInfo("FileEngineTest:"+currentPath);
			assertTrue(used[0]);
			assertTrue(fileInfo.exists());
			assertTrue(fileInfo.isDir());
		}finally {
			handler.dispose();
		}
	}
	
	@Test
    public void testEndsWithFileEngine() {
		String currentPath = QDir.current().absolutePath();
		QFileInfo fileInfo = new QFileInfo(currentPath+"::FileEngineTest");
		assertFalse(fileInfo.exists());
		assertFalse(fileInfo.isDir());
		boolean[] used = {false};
		QAbstractFileEngineHandler handler = QAbstractFileEngineHandler.fromFileNameTest(file->{
				used[0] = true;
				return new QFSFileEngine(file.substring(0, file.length() - 16));
			}, QString.Comparison.EndsWith, Qt.CaseSensitivity.CaseInsensitive, "::fileenginetest");
		assertTrue("handler is null", handler!=null);
		try {
			fileInfo = new QFileInfo(currentPath+"::FileEngineTest");
			assertTrue(used[0]);
			assertTrue(fileInfo.exists());
			assertTrue(fileInfo.isDir());
		}finally {
			handler.dispose();
		}
	}
	
	@Test
    public void testMatchFileEngine() {
		String currentPath = "FileEngineTest:"+QDir.current().absolutePath();
		QRegularExpression expr = new QRegularExpression("^FileEngineTest\\:.*$");
		QFileInfo fileInfo = new QFileInfo(currentPath);
		assertFalse(fileInfo.exists());
		assertFalse(fileInfo.isDir());
		boolean[] used = {false};
		QAbstractFileEngineHandler handler = QAbstractFileEngineHandler.fromFileNameTest(file->{
				used[0] = true;
				return new QFSFileEngine(file.substring(15));
			}, expr);
		assertTrue("handler is null", handler!=null);
		try {
			fileInfo = new QFileInfo(currentPath);
			assertTrue(used[0]);
			assertTrue(fileInfo.exists());
			assertTrue(fileInfo.isDir());
		}finally {
			handler.dispose();
		}
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestFileEngine.class.getName());
    }
}
