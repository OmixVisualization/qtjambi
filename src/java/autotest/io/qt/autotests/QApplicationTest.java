/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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

import java.io.File;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.UUID;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;

import io.qt.QNoNativeResourcesException;
import io.qt.QtObjectInterface;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QProcess;
import io.qt.core.QProcessEnvironment;
import io.qt.core.QThread;
import io.qt.internal.QtJambiDebugTools;
import io.qt.internal.QtJambiInternal;
import io.qt.widgets.QApplication;

public abstract class QApplicationTest {    
	
	protected static final List<WeakReference<QtObjectInterface>> instances = Collections.synchronizedList(new ArrayList<>());
	
    @BeforeClass
	public static void testInitialize() throws Exception {
        try {
        	System.setProperty("io.qt.log-messages", "ALL");
			if(QCoreApplication.instance()==null) {
				Utils.println(2, "QApplicationTest.testInitialize(): begin");
			    io.qt.QtResources.addSearchPath(".");
			    QCoreApplication.setApplicationName("QtJambiUnitTest");
			    QApplication.initialize(new String[]{"arg1", "arg2", "arg3"});
			    Utils.println(2, "QApplicationTest.testInitialize(): done");
		        QThread.currentThread().setObjectName("main");
			}
		} catch (Throwable e) {
			e.printStackTrace();
			throw e;
		}
    }

    @AfterClass
    public static void testDispose() throws Exception {
    	try {
    		Object currentThread = QThread.currentThread();
    		while(!instances.isEmpty()) {
    			WeakReference<QtObjectInterface> weak = instances.remove(0);
    			QtObjectInterface o = weak.get();
    			if(o!=null && QtJambiInternal.ownership(o)==QtJambiInternal.Ownership.Cpp && !o.isDisposed()) {
    				if(o instanceof QObject && ((QObject) o).thread()!=currentThread) {
    					((QObject) o).disposeLater();
    				}else {
    					o.dispose();
    				}
    			}
    		}
	        Utils.println(2, "QApplicationTest.testDispose(): BEGIN");
	        System.gc();
	        System.runFinalization();
	
	        // We are attempting to reach a point here where memory should be reclaimed
	        // except for anything linked to QApplication.
	
	        QApplication app = QApplication.instance();
	        try {
	            Utils.println(3, "QApplicationTest.testDispose(): QApplication-only  app="+app);
	        } catch(QNoNativeResourcesException e) {
	            Utils.println(3, "QApplicationTest.testDispose(): QApplication-only  io.qt.QNoNativeResourcesException: app="+e.getMessage());
	        }
	
	        System.err.flush();
	        System.out.flush();
	
	        if(app != null) {
	            try {
	        		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 1 PRE");
	                QApplication.processEvents();	// NPE
	        		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 1 POST");
	            } catch(Throwable t) {
	                t.printStackTrace();
	            }
	    		Utils.println(3, "QApplicationTest.testDispose(): sendPostedEvents(DeferredDelete) PRE");
	            QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	    		Utils.println(3, "QApplicationTest.testDispose(): sendPostedEvents(DeferredDelete) POST");
	        }
    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.quit() PRE");
//	        QApplication.quit();
    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.quit() POST");
	        app = QApplication.instance();
	        if(app != null) {
				Utils.println(3, "QApplicationTest.testDispose(): garbage PRE");
		        for (int i = 0; i < 4; i++) {
		        	System.gc();
		            System.runFinalization();
		            Thread.sleep(50);
		            QApplication.processEvents();
		            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		        	System.gc();
		            System.runFinalization();
				}
		        while(QtJambiDebugTools.hasDeleteLaterEvents()) {
		            QApplication.processEvents();
		            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());	        	
		        }
				Utils.println(3, "QApplicationTest.testDispose(): garbage POST");
		        
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 2 PRE");
	            QApplication.processEvents();
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 2 POST");
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 3 PRE");
	            QApplication.processEvents(/*QEventLoop.ProcessEventsFlag.DeferredDeletion*/);
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 3 POST");
	    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.dispose() PRE");
	            app.dispose();
	    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.dispose() POST");
	            try {
	            	if(app.isDisposed())
	            		Utils.println(3, "QApplicationTest.testDispose(): done  app="+app.getClass()+"@"+app.hashCode());
	            	else
	            		Utils.println(3, "QApplicationTest.testDispose(): not done  app="+app);
	            } catch(QNoNativeResourcesException e) {
	                Utils.println(3, "QApplicationTest.testDispose(): done  io.qt.QNoNativeResourcesException: app="+e.getMessage());
	            }
	            app = null;		// kill hard-reference
	        }
			Utils.println(3, "QApplicationTest.testDispose(): garbage PRE");
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        Thread.sleep(50);
			Utils.println(3, "QApplicationTest.testDispose(): garbage POST");
	
	        // We are attempting to reach a point here where memory should be reclaimed
	        // including that which was linked to QApplication
	
	        Utils.println(3, "QApplicationTest.testDispose(): shutdown PRE");
	        QApplication.shutdown();
	        Utils.println(3, "QApplicationTest.testDispose(): shutdown POST");
	        
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        Thread.sleep(50);
	        
	        QtJambiDebugTools.flushOut();  // fflush(stdout)
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        int objectCount = QtJambiDebugTools.objectCount();  // QtJambiLink::QtJambiLink_dump()
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        Utils.println(3, "QApplicationTest.testDispose(): end objectCount="+objectCount);
	
	        if(objectCount == 0)
	            return;  // optimization due to class loading causing some references to be set
	
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        QApplication.processEvents();
	        System.gc();
	        System.runFinalization();
	        Thread.sleep(50);
	
	        QtJambiDebugTools.flushOut();  // fflush(stdout)
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        objectCount = QtJambiDebugTools.objectCount();  // QtJambiLink::QtJambiLink_dump()
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        Utils.println(3, "QApplicationTest.testDispose(): end objectCount="+objectCount);
    	}finally {
            Utils.println(2, "QApplicationTest.testDispose(): DONE");
    	}
    }
    

    
    static void testDeployerApp(String applicationName, String...modules) throws InterruptedException, IOException {
    	final QProcess process = new QProcess();
    	String version = QtJambiInternal.majorVersion()+"."+QtJambiInternal.minorVersion()+"."+QtJambiInternal.qtjambiPatchVersion();
    	boolean isDebug = "debug".equals(System.getProperty("io.qt.debug"));
    	String jvmVersion = System.getProperty("java.version");
    	boolean java8 = jvmVersion.startsWith("1.") || jvmVersion.startsWith("8.") || jvmVersion.startsWith("7.");
    	String jambidir = System.getProperty("user.dir");
    	Assume.assumeTrue(jambidir!=null);
    	File testsDir = new File(new File(jambidir, "build"), "tests");
    	Assume.assumeTrue(testsDir.getAbsolutePath()+" not a directory", testsDir.isDirectory());
    	File targetDir = new File(testsDir, "tmp");
    	targetDir.mkdirs();
    	Assert.assertTrue(targetDir.isDirectory());
    	String classPath;
    	String modulePath;
    	String executable = "";
    	String platform;
    	String osName = System.getProperty("os.name").toLowerCase();
    	if(osName.startsWith("windows")) {
    		if(isDebug)
    			executable += "d";
    		executable += ".exe";
    		platform = "windows";
    		switch(System.getProperty("os.arch").toLowerCase()) {
        	case "arm":
        	case "arm32":
        		platform += "-arm32"; break;
        	case "arm64":
        	case "aarch64":
        		platform += "-arm64"; break;
        	case "x86_64":
        	case "x64":
        	case "amd64":
        		platform += "-x64"; break;
    		default:
        		platform += "-x86"; break;
    		}
    	}else if(osName.startsWith("mac")) {
    		if(isDebug)
    			executable += "_debug";
    		executable += ".app";
    		platform = "macos";
    	}else if(osName.startsWith("android")) {
    		Assume.assumeFalse("Cannot run on android", true);
    		return;
    	}else {
    		if(isDebug)
    			executable += "_debug";
    		platform = "linux";
    		switch(System.getProperty("os.arch").toLowerCase()) {
        	case "arm":
        	case "arm32":
        		platform += "-arm32"; break;
        	case "arm64":
        	case "aarch64":
        		platform += "-arm64"; break;
        	case "x86_64":
        	case "x64":
        	case "amd64":
        		platform += "-x64"; break;
    		default:
        		platform += "-x86"; break;
    		}
    	}
    	String qtdir = System.getenv("QTDIR");
    	File deploymentDir = new File(new File(new File(jambidir, "deployment"), "native"), platform);
    	if(isDebug) {
    		platform += "-debug";
    		deploymentDir = new File(deploymentDir, "debug");
    	}else {
    		deploymentDir = new File(deploymentDir, "release");
    	}
    	deploymentDir = new File(deploymentDir, "utilities");
    	String macosPrefix = "";
    	if(osName.startsWith("mac")) {
    		macosPrefix = "../../../";
    	}
    	File jambiDeploymentDir = null;
    	try {
	    	if(java8) {
	    		classPath = macosPrefix+"../java8";
	    		classPath += File.pathSeparator+macosPrefix+"../../../deployment/qtjambi-native-"+platform+"-"+version+".jar";
	    		classPath += File.pathSeparator+macosPrefix+"../../../deployment/qtjambi-jre8-"+version+".jar";
	    		for(String mod : modules) {
		    		classPath += File.pathSeparator+macosPrefix+"../../../deployment/qtjambi-"+mod+"-native-"+platform+"-"+version+".jar";
		    		classPath += File.pathSeparator+macosPrefix+"../../../deployment/qtjambi-"+mod+"-jre8-"+version+".jar";
		    		if("sql".equals(mod) && !System.getProperty("qtjambi.deployer.test.jdbc", "").isEmpty()) {
		    			String jdbcPath = targetDir.toPath().toAbsolutePath().relativize(new File(System.getProperty("qtjambi.deployer.test.jdbc")).toPath().toAbsolutePath()).toString();
			    		classPath += File.pathSeparator+macosPrefix+jdbcPath;
		    		}
	    		}
	        	io.qt.qtjambi.deployer.Main.main(new String[]{
	        			"app",
	        			"--application="+applicationName,
	        			"--dir="+targetDir.getAbsolutePath(),
	        			"--class-path="+classPath,
	        			"--ico=C:\\",
	        			"--main-class=io.qt.autotests."+applicationName,
	        			"-Djava.library.path="+qtdir+File.separator+(osName.startsWith("windows") ? "bin" : "lib"),
	        			"--jvm-path="+System.getProperty("java.home"),
	        			"--executable="+new File(deploymentDir, "QtJambiLauncher"+executable).getAbsolutePath(),
	        			"-Dio.qt.debug="+(isDebug ? "debug" : "release"),
	        			"-Xrs",
	        			"-Dio.qt.debug.level="+System.getProperty("io.qt.debug.level", "1"),
	        			"-Dio.qt.verbose-loading="+System.getProperty("io.qt.verbose-loading", "false")
	            	});
	    	}else {
	    		classPath = macosPrefix+"../java";
	    		classPath += File.pathSeparator+macosPrefix+"../../../deployment/qtjambi-native-"+platform+"-"+version+".jar";
	    		modulePath = "../"+macosPrefix+"../../deployment/qtjambi-"+version+".jar";
	    		for(String mod : modules) {
		    		classPath += File.pathSeparator+macosPrefix+"../../../deployment/qtjambi-"+mod+"-native-"+platform+"-"+version+".jar";
		    		modulePath += File.pathSeparator+macosPrefix+"../../../deployment/qtjambi-"+mod+"-"+version+".jar";
		    		if("sql".equals(mod) && !System.getProperty("qtjambi.deployer.test.jdbc", "").isEmpty()) {
		    			String jdbcPath = targetDir.toPath().toAbsolutePath().relativize(new File(System.getProperty("qtjambi.deployer.test.jdbc")).toPath().toAbsolutePath()).toString();
			    		classPath += File.pathSeparator+macosPrefix+jdbcPath;
		    		}
	    		}
	        	io.qt.qtjambi.deployer.Main.main(new String[]{
	        			"app",
	        			"--application="+applicationName,
	        			"--dir="+targetDir.getAbsolutePath(),
	        			"--class-path="+classPath,
	        			"--module-path="+modulePath,
	        			"--ico=C:\\",
	        			"--main-class=io.qt.autotests."+applicationName,
	        			"-Djava.library.path="+qtdir+File.separator+(osName.startsWith("windows") ? "bin" : "lib"),
	        			"--jvm-path="+System.getProperty("java.home"),
	        			"--executable="+new File(deploymentDir, "QtJambiLauncher"+executable).getAbsolutePath(),
	        			"-Dio.qt.debug="+(isDebug ? "debug" : "release"),
	        			"-Xrs",
	        			"-Dio.qt.debug.level="+System.getProperty("io.qt.debug.level", "1"),
	        			"-Dio.qt.verbose-loading="+System.getProperty("io.qt.verbose-loading", "false")
	            	});
	    	}
	    	File testFile = new File(targetDir, "touch.test");
	    	testFile.delete();
	    	Assert.assertTrue(!testFile.exists());
	    	UUID uuid = UUID.randomUUID();
	    	process.setWorkingDirectory(targetDir.getAbsolutePath());
	    	process.setStandardOutputFile(new File(targetDir, applicationName+".out.log").getAbsolutePath());
	    	process.setStandardErrorFile(new File(targetDir, applicationName+".err.log").getAbsolutePath());
    		QProcessEnvironment environment = QProcessEnvironment.systemEnvironment();
	    	if(osName.startsWith("windows")) {
	    		File exeFile = new File(targetDir, applicationName+".exe");
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" does not exist", exeFile.exists());
	    		process.setProgram(exeFile.getAbsolutePath());
	    		process.setArguments(Arrays.asList(testFile.getAbsolutePath(), uuid.toString()));
	    		environment.insert("PATH", qtdir+"\\bin");
	    	}else if(osName.startsWith("mac")) {
	    		File exeFile = new File(targetDir, applicationName+".app/Contents/MacOS/"+applicationName);
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" does not exist", exeFile.exists());
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" cannot be executed", exeFile.canExecute());
	    		switch(System.getProperty("os.arch").toLowerCase()) {
	        	case "arm64":
	        	case "aarch64":
		    		process.setProgram("/usr/bin/arch");
		    		process.setArguments(Arrays.asList("-arm64", exeFile.getAbsolutePath(), testFile.getAbsolutePath(), uuid.toString()));
	    			break;
	        	case "x86_64":
	        	case "x64":
	        	case "amd64":
		    		process.setProgram("/usr/bin/arch");
		    		process.setArguments(Arrays.asList("-x86_64", exeFile.getAbsolutePath(), testFile.getAbsolutePath(), uuid.toString()));
	    			break;
	    		default:
		    		process.setProgram(exeFile.getAbsolutePath());
		    		process.setArguments(Arrays.asList(testFile.getAbsolutePath(), uuid.toString()));
	    		}
	    		if(!new File(targetDir, applicationName+".app/Contents/Frameworks").exists())
	    			Files.createSymbolicLink(new File(targetDir, applicationName+".app/Contents/Frameworks").toPath(), new File(qtdir+"/lib").toPath());
	    		environment.insert("DYLD_LIBRARY_PATH", qtdir+"/lib");
	    		environment.insert("DYLD_FRAMEWORK_PATH", qtdir+"/lib");
	    	}else {
	    		File exeFile = new File(targetDir, applicationName);
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" does not exist", exeFile.exists());
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" cannot be executed", exeFile.canExecute());
	    		process.setArguments(Arrays.asList(testFile.getAbsolutePath(), uuid.toString()));
	    		process.setProgram(exeFile.getAbsolutePath());
	    		environment.insert("LD_LIBRARY_PATH", qtdir+"/lib");
	    	}
    		process.setProcessEnvironment(environment);
	    	process.start();
			File tmpDir = new File(System.getProperty("java.io.tmpdir"));
			jambiDeploymentDir = new File(tmpDir, "QtJambi" + QtJambiInternal.majorVersion() + "." + QtJambiInternal.minorVersion() + "." + QtJambiInternal.qtjambiPatchVersion() + "_" + System.getProperty("user.name") + "_" + process.processId());
			while(!process.waitForFinished(500)) {
				if(Thread.interrupted())
					throw new InterruptedException();
			}
			if(process.state()==QProcess.ProcessState.Running) {
				try{
					process.kill();
				}catch(Throwable t) {}
			}
	    	Assert.assertTrue("Test file does not exist, i.e. standalone program did not run. Process returned: "+process.exitCode(), testFile.exists());
	    	Assert.assertEquals(uuid.toString(), new String(Files.readAllBytes(testFile.toPath())));
    	}catch(InterruptedException e) {
    	}finally {
    		if(process!=null && !process.isDisposed() && process.state()==QProcess.ProcessState.Running) {
        		process.write(new QByteArray("quit"));
        		process.closeWriteChannel();
        		process.waitForFinished(500);
    			process.terminate();
    			process.waitForFinished(500);
    			if(process.state()==QProcess.ProcessState.Running) {
    				process.kill();
    				process.waitForFinished();
    			}
    		}
			try {
				String content = new String(Files.readAllBytes(new File(targetDir, applicationName+".out.log").toPath()));
				if(!content.trim().isEmpty())
					System.out.println(content);
			} catch (Throwable e) {}
			try {
				String content = new String(Files.readAllBytes(new File(targetDir, applicationName+".err.log").toPath()));
				if(!content.trim().isEmpty())
					System.err.println(content);
			} catch (Throwable e) {}
    		if(jambiDeploymentDir!=null && jambiDeploymentDir.isDirectory()) {
    			clearAndDelete(jambiDeploymentDir);
    		}
    		if(!Boolean.getBoolean("qtjambi.deployer.skip.deletion")) {
		    	try {
					Files.walkFileTree(targetDir.toPath(), new SimpleFileVisitor<Path>() {
					    @Override
					    public FileVisitResult visitFile(Path file, BasicFileAttributes attrs)
					        throws IOException
					    {
					        Files.delete(file);
					        return FileVisitResult.CONTINUE;
					    }
					    @Override
					    public FileVisitResult postVisitDirectory(Path dir, IOException e)
					        throws IOException
					    {
					        if (e == null) {
					            Files.delete(dir);
					            return FileVisitResult.CONTINUE;
					        } else {
					            // directory iteration failed
					            throw e;
					        }
					    }
					});
				} catch (IOException e) {
					e.printStackTrace();
				}
    		}
    	}
    }
    
    private static void clearAndDelete(File directory) {
    	for(File file : directory.listFiles()) {
    		if(file.getName().equals(".") || file.getName().equals(".."))
    			continue;
    		if(file.isDirectory()) {
    			clearAndDelete(file);
    		}else {
    			file.delete();
    		}
    	}
    	directory.delete();
    }
}
