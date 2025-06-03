/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.UUID;
import java.util.function.Consumer;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.rules.TestRule;

import io.qt.Nullable;
import io.qt.QNoNativeResourcesException;
import io.qt.QtObjectInterface;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QProcess;
import io.qt.core.QProcessEnvironment;
import io.qt.core.QResource;
import io.qt.core.QStringList;
import io.qt.core.QThread;
import io.qt.core.QVersionNumber;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QIcon;
import io.qt.gui.QShowEvent;
import io.qt.internal.TestUtility;
import io.qt.widgets.QApplication;
import io.qt.widgets.QDialog;

public abstract class ApplicationInitializer extends UnitTestInitializer{
	
	public static class MyTestRule implements TestRule{
		private String className;
		private String methodName;
		@Override
		public org.junit.runners.model.Statement apply(org.junit.runners.model.Statement base, org.junit.runner.Description description) {
			className = description.getTestClass().getSimpleName();
			methodName = description.getMethodName();
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.INFO, "Start test "+className+"."+methodName);
			return base;
		}
		public String getClassName() {
			return className;
		}
		public String getMethodName() {
			return methodName;
		}
	}
	
	@org.junit.Rule public final MyTestRule testRule = new MyTestRule();
	
	private static String testClassName = "";
	
	protected static final List<WeakReference<QtObjectInterface>> instances = Collections.synchronizedList(new ArrayList<>());
	
    @BeforeClass
	public static void testInitialize() throws Exception {
    	testInitialize(0);
    }
    
    public static void testInitializeWithGui() throws Exception {
    	testInitialize(1);
    }
    
    public static void testInitializeWithWidgets() throws Exception {
    	testInitialize(2);
    }
    
    private static void testInitialize(int mode) throws Exception {
        try {
			if(QCoreApplication.instance()==null) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.INFO, "testInitialize({0}): BEGIN", testClassName);
				QResource.addClassPath(".");
				QCoreApplication.setApplicationName("QtJambiUnitTest");
				switch(mode){
				case 0:
					QCoreApplication.initialize(new String[0]); 
					break;
				case 1:
					QGuiApplication.initialize(new String[0]);
					QGuiApplication.setWindowIcon(new QIcon(":io/qt/autotests/icon.png"));
					break;
					default:
					QApplication.initialize(new String[0]);
					QGuiApplication.setWindowIcon(new QIcon(":io/qt/autotests/icon.png"));
					break;
				}
		        QThread.currentThread().setObjectName("main");
			    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.INFO, "testInitialize({0}): DONE", testClassName);
			    if(mode>=2 
			    		&& QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS)
			    		&& QLibraryInfo.version().equals(new QVersionNumber(6,5,3))
			    		) {
			    	QDialog dialog = new QDialog(){
						@Override
						protected void showEvent(@Nullable QShowEvent arg__1) {
							super.showEvent(arg__1);
					    	QMetaObject.invokeMethod(this::reject, Qt.ConnectionType.QueuedConnection);
						}
			    	};
			    	dialog.exec();
			    	dialog.dispose();
			    }
			}
		} catch (Throwable e) {
			e.printStackTrace();
			throw e;
		}
    }

    @AfterClass
    public static void testDispose() throws Exception {
    	try {
	        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.INFO, "testDispose({0}): BEGIN", testClassName);
    		Object currentThread = QThread.currentThread();
    		while(!instances.isEmpty()) {
    			WeakReference<QtObjectInterface> weak = instances.remove(0);
    			QtObjectInterface o = weak.get();
    			if(o!=null && General.internalAccess.isCppOwnership(o) && !o.isDisposed()) {
    				try {
	    				if(o instanceof QObject && ((QObject) o).thread()!=currentThread) {
	    					((QObject) o).disposeLater();
	    				}else {
	    					o.dispose();
	    				}
    				}catch(QNoNativeResourcesException e) {}
    			}
    		}
	        runGC();
	        boolean noShutdown = Boolean.getBoolean("io.qt.autotests.no-app-shutdown");
	
	        // We are attempting to reach a point here where memory should be reclaimed
	        // except for anything linked to QApplication.
	
	        QCoreApplication app = QCoreApplication.instance();
	        try {
	            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): QCoreApplication-only  app={1}", new Object[]{testClassName, app});
	        } catch(QNoNativeResourcesException e) {
	            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): QCoreApplication-only  io.qt.QNoNativeResourcesException: {1}", new Object[]{testClassName, e.getMessage()});
	        }
	
	        System.err.flush();
	        System.out.flush();
	
	        if(app != null) {
	        	QCoreApplication.quit();
	            try {
	        		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): processEvents() 1 PRE", testClassName);
	                QCoreApplication.processEvents();	// NPE
	        		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): processEvents() 1 POST", testClassName);
	            } catch(Throwable t) {
	                t.printStackTrace();
	            }
	    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): sendPostedEvents(DeferredDelete) PRE", testClassName);
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): sendPostedEvents(DeferredDelete) POST", testClassName);
	        }
	        app = QCoreApplication.instance();
	        if(app != null) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): garbage PRE", testClassName);
		        for (int i = 0; i < 4; i++) {
		        	runGC();
		            Thread.sleep(50);
		            QCoreApplication.processEvents();
		            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		        	runGC();
				}
		        while(TestUtility.hasDeleteLaterEvents()) {
		            QCoreApplication.processEvents();
		            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());	        	
		        }
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): garbage POST", testClassName);
		        
	    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): processEvents() 2 PRE", testClassName);
	            QCoreApplication.processEvents();
	    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): processEvents() 2 POST", testClassName);
	    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): processEvents() 3 PRE", testClassName);
	            QCoreApplication.processEvents(/*QEventLoop.ProcessEventsFlag.DeferredDeletion*/);
	    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): processEvents() 3 POST", testClassName);
	    		if(!noShutdown) {
		    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): QCoreApplication.dispose() PRE", testClassName);
		            app.dispose();
		    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): QCoreApplication.dispose() POST", testClassName);
		            try {
		            	if(app.isDisposed())
		            		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): done  app={1}@{2}", new Object[]{testClassName, app.getClass(), app.hashCode()});
		            	else
		            		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): not done  app={1}", new Object[]{testClassName, app});
		            } catch(QNoNativeResourcesException e) {
		                java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): done  io.qt.QNoNativeResourcesException: app={1}", new Object[]{testClassName, e.getMessage()});
		            }
		            app = null;		// kill hard-reference
		            if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
			            Thread timeoutThread = new Thread(()->{
							try {
								synchronized(ApplicationInitializer.class) {
									ApplicationInitializer.class.wait(15000);
								}
								System.err.println("Test process does not terminate.");
								Runtime.getRuntime().halt(-1);
							} catch (InterruptedException e) {
							}
						});
						timeoutThread.setDaemon(true);
						timeoutThread.start();
		            }
	    		}
	        }
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): garbage PRE", testClassName);
	        runGC();
	        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        runGC();
	        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        Thread.sleep(50);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): garbage POST", testClassName);
	
	        // We are attempting to reach a point here where memory should be reclaimed
	        // including that which was linked to QCoreApplication
	
			if(!noShutdown) {
		        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): shutdown PRE", testClassName);
		        QCoreApplication.shutdown();
		        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): shutdown POST", testClassName);
			}
	        
	        runGC();
	        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        Thread.sleep(50);
	        
	        TestUtility.flushOut();  // fflush(stdout)
	        TestUtility.flushErr();  // fflush(stderr)
	        int objectCount = TestUtility.objectCount();  // QtJambiLink::QtJambiLink_dump()
	        TestUtility.flushErr();  // fflush(stderr)
	        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): end objectCount={1}", new Object[]{testClassName, objectCount});
	
	        if(objectCount == 0)
	            return;  // optimization due to class loading causing some references to be set
	
	        runGC();
	        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        QCoreApplication.processEvents();
	        runGC();
	        Thread.sleep(50);
	
	        TestUtility.flushOut();  // fflush(stdout)
	        TestUtility.flushErr();  // fflush(stderr)
	        objectCount = TestUtility.objectCount();  // QtJambiLink::QtJambiLink_dump()
	        TestUtility.flushErr();  // fflush(stderr)
	        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testDispose({0}): end objectCount={1}", new Object[]{testClassName, objectCount});
    	}finally {
            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.INFO, "testDispose({0}): DONE", testClassName);
    	}
    }
    
	static void testDeployerApp(String applicationName, String...modules) throws InterruptedException, IOException {
    	String version = QtUtilities.qtjambiVersion().toString();
    	boolean isDebug = TestUtility.isDebugBuild();
    	String jvmVersion = System.getProperty("java.version");
    	boolean java8 = jvmVersion.startsWith("1.") || jvmVersion.startsWith("8.") || jvmVersion.startsWith("7.");
    	String jambidir = System.getProperty("user.dir");
    	Assume.assumeTrue(jambidir!=null);
    	final File testsDir = new File(jambidir).getParentFile();
    	Assume.assumeTrue(new File(jambidir).getParentFile()+" not a directory", testsDir!=null && testsDir.isDirectory());
    	final File targetDir = new File(testsDir, "tmp_"+TestUtility.processName());
    	targetDir.mkdirs();
    	Assert.assertTrue(targetDir.isDirectory());
    	String classPath;
    	String modulePath;
    	String executable = "";
    	String osName = System.getProperty("os.name").toLowerCase();
    	if(osName.startsWith("windows")) {
    		if(isDebug)
    			executable += "d";
    		executable += ".exe";
    	}else if(osName.startsWith("mac")) {
    		if(isDebug)
    			executable += "_debug";
    		executable += ".app";
    	}else if(osName.startsWith("android")) {
    		Assume.assumeFalse("Cannot run on android", true);
    		return;
    	}else {
    		if(isDebug)
    			executable += "_debug";
    	}
    	String qtBinariesPath = "";
    	if(isDebug) {
    		qtBinariesPath = System.getProperty("io.qt.library-path-override");
    	}else {
        	File qtLibraryPath = TestUtility.qtLibraryPath();    		
        	if(qtLibraryPath!=null) {
        		qtBinariesPath = qtLibraryPath.getAbsolutePath();
        	}
    	}
    	String macosPrefix = "";
    	File utilitiesDir = new File(macosPrefix + "../../../../deployment/platforms/" + TestUtility.osArchName() + (isDebug ? "/debug/utilities" : "/release/utilities"));
		final File tmpDir = new File(System.getProperty("java.io.tmpdir"));
		String processName = "";
    	File jambiDeploymentDir = null;
    	final QProcess process = new QProcess();
    	try {
	    	if(java8) {
	    		classPath = macosPrefix+"../qtjambi-autotests-"+version+".jar";
	    		classPath += File.pathSeparator+macosPrefix+"../../../../deployment/qtjambi-"+version+".jar";
	    		for(String mod : modules) {
		    		classPath += File.pathSeparator+macosPrefix+"../../../../deployment/qtjambi-"+mod+"-"+version+".jar";
		    		if("sql".equals(mod) && !System.getProperty("qtjambi.deployer.test.jdbc", "").isEmpty()) {
		    			String jdbcPath = targetDir.toPath().toAbsolutePath().relativize(new File(System.getProperty("qtjambi.deployer.test.jdbc")).toPath().toAbsolutePath()).toString();
			    		classPath += File.pathSeparator+macosPrefix+jdbcPath;
		    		}
	    		}
	        	io.qt.qtjambi.deployer.Main.main(new String[]{
	        			"application",
	        			"--application-name="+applicationName,
	        			"--dir="+targetDir.getAbsolutePath(),
	        			"--class-path="+classPath,
	        			"--main-class=io.qt.autotests."+applicationName,
	        			"--jvm-path="+System.getProperty("java.home"),
	        			"--executable="+new File(utilitiesDir, "QtJambiLauncher"+executable).getAbsolutePath(),
	        			"--",
	        			"-Djava.library.path="+qtBinariesPath,
	        			"-Dio.qt.debug="+(isDebug ? "debug" : "release"),
	        			"-Djava.util.logging.config.file="+System.getProperty("java.util.logging.config.file", ""),
	        			"-Xrs",
	        			"-Dio.qt.log-messages="+System.getProperty("io.qt.log-messages", "ALL")
	            	});
	    	}else {
	    		classPath = macosPrefix+"../qtjambi-autotests-"+version+".jar";
	    		modulePath = "../"+macosPrefix+"../../../deployment/qtjambi-"+version+".jar";
	    		for(String mod : modules) {
		    		modulePath += File.pathSeparator+macosPrefix+"../../../../deployment/qtjambi-"+mod+"-"+version+".jar";
		    		if("sql".equals(mod) && !System.getProperty("qtjambi.deployer.test.jdbc", "").isEmpty()) {
		    			String jdbcPath = targetDir.toPath().toAbsolutePath().relativize(new File(System.getProperty("qtjambi.deployer.test.jdbc")).toPath().toAbsolutePath()).toString();
			    		classPath += File.pathSeparator+macosPrefix+jdbcPath;
		    		}
	    		}
	        	io.qt.qtjambi.deployer.Main.main(new String[]{
	        			"application",
	        			"--application-name="+applicationName,
	        			"--dir="+targetDir.getAbsolutePath(),
	        			"--class-path="+classPath,
	        			"--module-path="+modulePath,
	        			"--main-class=io.qt.autotests."+applicationName,
	        			"--jvm-path="+System.getProperty("java.home"),
	        			"--executable="+new File(utilitiesDir, "QtJambiLauncher"+executable).getAbsolutePath(),
	        			"--",
	        			"-Djava.library.path="+qtBinariesPath,
	        			"-Dio.qt.debug="+(isDebug ? "debug" : "release"),
	        			"-Djava.util.logging.config.file="+System.getProperty("java.util.logging.config.file", ""),
	        			"-Xrs",
	        			"-Dio.qt.log-messages="+System.getProperty("io.qt.log-messages", "ALL")
	            	});
	    	}
	    	File testFile = new File(targetDir, applicationName+".touch.test");
	    	testFile.delete();
	    	Assert.assertTrue(!testFile.exists());
	    	UUID uuid = UUID.randomUUID();
	    	QProcessEnvironment processEnvironment = new QProcessEnvironment(QProcessEnvironment.systemEnvironment());
	    	process.setWorkingDirectory(targetDir.getAbsolutePath());
	    	process.setStandardErrorFile(new File(targetDir, applicationName+".err.log").getAbsolutePath());
	    	process.setStandardOutputFile(new File(targetDir, applicationName+".out.log").getAbsolutePath());
	    	if(osName.startsWith("windows")) {
	    		File exeFile = new File(targetDir, applicationName+".exe");
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" does not exist", exeFile.exists());
	    		process.setProgram(exeFile.getAbsolutePath());
	    		process.setArguments(new QStringList(testFile.getAbsolutePath(), uuid.toString()));
	    		processEnvironment.insert("PATH", qtBinariesPath);
	    	}else if(osName.startsWith("mac")) {
	    		File exeFile = new File(targetDir, applicationName+".app/Contents/MacOS/"+applicationName);
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" does not exist", exeFile.exists());
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" cannot be executed", exeFile.canExecute());
	    		switch(System.getProperty("os.arch").toLowerCase()) {
	        	case "arm64":
	        	case "aarch64":
	        		process.setProgram("/usr/bin/arch");
		    		process.setArguments(new QStringList("-arm64", "-e", "DYLD_FRAMEWORK_PATH=" + qtBinariesPath, exeFile.getAbsolutePath(), testFile.getAbsolutePath(), uuid.toString()));
	    			break;
	        	case "x86_64":
	        	case "x64":
	        	case "amd64":
	        		process.setProgram("/usr/bin/arch");
		    		process.setArguments(new QStringList("-x86_64", "-e", "DYLD_FRAMEWORK_PATH=" + qtBinariesPath, exeFile.getAbsolutePath(), testFile.getAbsolutePath(), uuid.toString()));
	    			break;
	    		default:
	    			process.setProgram(exeFile.getAbsolutePath());
		    		process.setArguments(new QStringList(testFile.getAbsolutePath(), uuid.toString()));
		    		processEnvironment.insert("DYLD_FRAMEWORK_PATH", qtBinariesPath);
	    		}
	    		//if(!new File(targetDir, applicationName+".app/Contents/Frameworks").exists())
	    		//	Files.createSymbolicLink(new File(targetDir, applicationName+".app/Contents/Frameworks").toPath(), new File(qtBinariesPath).toPath());
	    	}else {
	    		File exeFile = new File(targetDir, applicationName);
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" does not exist", exeFile.exists());
	    		Assert.assertTrue("Executable "+exeFile.getAbsolutePath()+" cannot be executed", exeFile.canExecute());
	    		process.setProgram(exeFile.getAbsolutePath());
	    		process.setArguments(new QStringList(testFile.getAbsolutePath(), uuid.toString()));
	    		processEnvironment.insert("LD_LIBRARY_PATH", qtBinariesPath);
	    	}
	    	process.setProcessEnvironment(processEnvironment);
	    	processEnvironment = null;
	    	process.start(QIODevice.OpenModeFlag.ReadWrite);
	    	processName = ""+process.processId();
			jambiDeploymentDir = new File(tmpDir, "QtJambi" + version + "_" + System.getProperty("user.name") + "_" + processName);
			while(!process.waitForFinished(500)) {
				if(Thread.interrupted())
					throw new InterruptedException();
			}
			if(process.state()==QProcess.ProcessState.Running) {
				try{
	    			process.write("quit".getBytes());
	    			process.closeWriteChannel();
	    			if(!process.waitForFinished(500) || process.state()==QProcess.ProcessState.Running)
	    				process.terminate();
				}catch(Throwable t) {}
			}
	    	Assert.assertTrue("Test file does not exist, i.e. standalone program did not run. Process returned: "+process.exitCode(), testFile.exists());
	    	Assert.assertEquals(uuid.toString(), new String(Files.readAllBytes(testFile.toPath())));
    	}catch(InterruptedException e) {
    	}finally {
    		if(process.state()==QProcess.ProcessState.Running) {
    			process.write("quit".getBytes());
    			process.closeWriteChannel();
    			if(!process.waitForFinished(500) || process.state()==QProcess.ProcessState.Running) {
    				process.kill();
    			}
    		}
    		if((processName==null || processName.isEmpty()) && new File(targetDir, "pid").isFile()) {
    			processName = new String(Files.readAllBytes(new File(targetDir, "pid").toPath())).trim();
				jambiDeploymentDir = new File(tmpDir, "QtJambi" + version + "_" + System.getProperty("user.name") + "_" + processName);
    		}
    		process.waitForFinished();
    		process.dispose();
    		runGC();
    		synchronized(ApplicationInitializer.class) {
    			Thread.sleep(1000);
    		}
    		runGC();
			try {
				String content = new String(Files.readAllBytes(new File(targetDir, applicationName+".out.log").toPath()));
				if(!content.trim().isEmpty()) {
					System.out.println("##### "+applicationName+".out.log begin #####");
					System.out.println(content);
					System.out.println("###### "+applicationName+".out.log end ######");
					System.out.flush();
				}
			} catch (Throwable e) {}
			try {
				String content = new String(Files.readAllBytes(new File(targetDir, applicationName+".err.log").toPath()));
				if(!content.trim().isEmpty()) {
					System.err.println("##### "+applicationName+".err.log begin #####");
					System.err.println(content);
					System.err.println("###### "+applicationName+".err.log end ######");
					System.err.flush();
				}
			} catch (Throwable e) {}
			Preferences preferences = Preferences.userNodeForPackage(io.qt.internal.TestUtility.class);
			String dirs = preferences.get("qtjambi.previous.deployment.dir", null);
    		if(dirs!=null && !dirs.isEmpty()) {
    	        preferences.remove("qtjambi.previous.deployment.dir");
    	        for(String dir : dirs.split("\\"+File.pathSeparator)) {
    	        	final File jambiTempDir = new File(dir);
        			if(jambiTempDir.exists() && jambiTempDir.isDirectory()) {
        				Logger.getLogger("io.qt.autotests").log(Level.FINEST, ()->"Cleaning previous deployment directory "+jambiTempDir);
        				clearAndDelete(jambiTempDir);
        			}
    	        }
    		}
    		if(jambiDeploymentDir!=null && jambiDeploymentDir.isDirectory()) {
    			Logger.getLogger("io.qt.autotests").log(Level.FINEST, "Cleaning lib directory "+jambiDeploymentDir);
    			clearAndDelete(jambiDeploymentDir);
    		}
    		if(!Boolean.getBoolean("qtjambi.deployer.skip.deletion")) {
    			Logger.getLogger("io.qt.autotests").log(Level.FINEST, ()->"Cleaning deployer directory "+targetDir);
    			clearAndDelete(targetDir);
    		}
    	}
    }
    
    static void clearAndDelete(File directory) {
    	if(directory!=null) {
    		if(directory.isDirectory()) {
		    	for(File file : directory.listFiles()) {
		    		if(file.getName().equals(".") || file.getName().equals(".."))
		    			continue;
		    		if(file.isDirectory() && !Files.isSymbolicLink(file.toPath())) {
		    			clearAndDelete(file);
		    		}else {
		    			file.delete();
		    		}
		    	}
    		}
	    	directory.delete();
    	}
    }
    
    private static Consumer<Runtime> runFinalization;
    static{
    	Consumer<Runtime> _runFinalization = null;
    	if(System.getProperty("os.name").toLowerCase().startsWith("linux")
    			&& System.getProperty("java.runtime.name").toLowerCase().startsWith("android")) {
	    	try{
	    		Method runFinalization = Runtime.class.getMethod("runFinalization");
	    		_runFinalization = runtime->{
	    			runtime.gc();
	    	    	try{
	    	    		runFinalization.invoke(runtime);
	    	    	}catch(Throwable t) {}
	    		};
	    	}catch(Throwable t) {}
    	}
    	if(_runFinalization==null)
    		runFinalization = Runtime::gc;
		else
			runFinalization = _runFinalization;
    }
    
    static void runGC() {
    	Thread.yield();
    	runFinalization.accept(Runtime.getRuntime());
    	Thread.yield();
    }
    
    public static String currentThreadToString(){
    	return threadToString(Thread.currentThread());
    }

    @Deprecated
	private static String threadToString(Thread thread){
		return ""+thread.getId();
	}

	public static void setTestClassName(String n) {
		testClassName = n;
	}

	public static String testClassName() {
		return testClassName;
	}
	public String testMethodName() {
		return testRule.methodName;
	}
}
