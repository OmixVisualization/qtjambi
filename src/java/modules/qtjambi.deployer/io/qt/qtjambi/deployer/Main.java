/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.qtjambi.deployer;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.JarURLConnection;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.LogManager;
import java.util.logging.Logger;

import io.qt.QtLongEnumerator;
import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QCommandLineOption;
import io.qt.core.QCommandLineParser;
import io.qt.core.QCoreApplication;
import io.qt.core.QList;
import io.qt.core.QScopeGuard;

/**
 * @hidden
 */
public class Main {
	
	static {
		try {
			String logLevel = System.getProperty("qtjambi.log-level", "");
			switch(logLevel) {
			case "ALL":
			case "FINE":
			case "FINEST":
			case "SEVERE":
			case "INFO":
			case "WARNING":
				ByteArrayOutputStream out = new ByteArrayOutputStream();
				Properties properties = new Properties();
				properties.setProperty("handlers", "java.util.logging.ConsoleHandler");
				properties.setProperty("java.util.logging.ConsoleHandler.level", logLevel);
				properties.setProperty(".level", logLevel);
				properties.setProperty("io.qt.internal.level", logLevel);
				properties.store(out, null);
				LogManager.getLogManager().readConfiguration(new ByteArrayInputStream(out.toByteArray()));
				break;
			default:
				break;
			}
		} catch (SecurityException | IOException e1) {
			e1.printStackTrace();
		}
	}
	
	enum JVMDetectionModes implements QtLongEnumerator{
	    AutoDetect,
	    UseRelativePath
	};

	enum Parameters implements QtLongEnumerator{
	    JVMDetectionMode,
	    JVMPath,
	    MinimumJVM,
	    MainClass,
	    ClassPath,
	    ModulePath,
	    LibraryPath,
	    JVMArguments,
	    JNIMinimumVersion,
	    JVMArgumentFilters
	};
	
    @SuppressWarnings("unused")
	private static void printout(QByteArray data) {
		for (int i = 0; i < data.size();) {
			for (int j = 0; j < 32 && i < data.size(); j++) {
				System.out.print(Byte.toUnsignedInt(data.at(i++)));
				System.out.print(',');
			}
			System.out.println();
		}
    }
	
	public static void main(String[] args) throws InterruptedException, IOException {
		if(args.length>0) {
			boolean doInit = QCoreApplication.instance()==null;
			if(doInit) {
				QCoreApplication.setApplicationName("deployer "+args[0]);
				QCoreApplication.setApplicationVersion(QtUtilities.qtjambiVersion().toString());
				QCoreApplication.initialize(args);
				QCoreApplication.setApplicationName("deployer");
			}
			try(QScopeGuard guard = new QScopeGuard(QCoreApplication::shutdown)){
				if(!doInit)
					guard.dismiss();
				QCommandLineParser parser = new QCommandLineParser();
				parser.setSingleDashWordOptionMode(QCommandLineParser.SingleDashWordOptionMode.ParseAsLongOptions);
				parser.setApplicationDescription(String.format("QtJambi Deployer version %1$s", QtUtilities.qtjambiVersion().toString()));
			    parser.addHelpOption();
			    parser.addVersionOption();
				//common
			    QCommandLineOption platformOption = new QCommandLineOption(QList.of("platform"), "Target platform", "platform");
			    QCommandLineOption dirOption = new QCommandLineOption(QList.of("d", "dir", "target-directory"), "Target directory", "dir");
			    QCommandLineOption classPathOption = new QCommandLineOption(QList.of("cp", "class-path"), "Class path for plugin/app execution", "path");
			    QCommandLineOption configurationOption = new QCommandLineOption(QList.of("c", "configuration"), "Library configuration", "debug|release", "release");
				switch(args[0]) {
				case "plugin":
				case "pluginlib":
					PluginGenerator.generate(parser, 
											args,
											platformOption,
											dirOption,
								    		classPathOption,
											configurationOption);
					break;
				case "qml":
				case "qmllib":
					QMLGenerator.generate(parser, 
											args,
											platformOption,
											dirOption,
								    		classPathOption,
											configurationOption);
					break;
				case "app":
				case "application":
					AppGenerator.generate(parser, 
											args,
											platformOption,
											dirOption,
								    		classPathOption,
											configurationOption);
					break;
				case "qt":
				case "qtlib":
				case "qtbundles":
					BundleGenerator.generate(parser, 
											args,
											platformOption,
											dirOption,
											configurationOption);
					break;
				case "container":
				case "containeraccess":
				case "ca":
					ContainerAccessGenerator.generate(parser, 
							args,
							dirOption);
					break;
				default:
					System.err.println("QtJambi Deployer, illegal argument: "+args[0]+", expected: one of plugin|qml|application|qtbundles|containeraccess");
					System.exit(-1);
					break;
				}
			}
		}else{
			String version = QtUtilities.qtjambiVersion().toString();
			System.out.println("Usage: deployer [plugin|qml|application|qt]");
			System.out.println(String.format("QtJambi Deployer version %1$s", version));
		}
	}

	static Enumeration<URL> findSpecs() throws MalformedURLException {
		Enumeration<URL> specsFound = Collections.emptyEnumeration();
        try {
			specsFound = Main.class.getClassLoader().getResources("META-INF/qtjambi-utilities.xml");
		} catch (IOException e) {
			Logger.getLogger("io.qt").log(Level.WARNING, "", e);
		}
        if(!specsFound.hasMoreElements()) {
        	URL url = Main.class.getResource("Main.class");
        	if(url!=null) {
	        	String classURL = url.toString();
		    	int index;
	    		File jarFile = null;
		    	if(classURL.startsWith("jar:file:") && (index = classURL.indexOf("!/"))>0) {
		    		String jarFileURL = classURL.substring(4, index);
		    		try {
						jarFile = new File(createURL(jarFileURL).toURI());
					} catch (URISyntaxException e) {
					}
		    	}else {
		    		try {
						URLConnection connection = url.openConnection();
						if(connection instanceof JarURLConnection) {
							jarFile = new File(((JarURLConnection) connection).getJarFile().getName());
						}
					} catch (Throwable e1) {
					}
		    	}
		    	if(jarFile!=null && jarFile.exists()) {
	    			File directory = jarFile.getParentFile();
	    			String fileName = jarFile.getName();
	    			String suffix = String.format("-%1$s.jar", QtUtilities.qtjambiVersion().toString());
	    			index = fileName.indexOf(suffix);
	    			fileName = fileName.substring(0, index) + "-native-";
	    			List<URL> foundURLs = new ArrayList<>();
	    			for(String jar : directory.list()) {
	    				if(jar.startsWith(fileName) && jar.endsWith(suffix)) {
	    					File nativeFile = new File(directory, jar);
	    					URL nativeFileURL = createURL("jar:"+nativeFile.toURI()+"!/META-INF/qtjambi-utilities.xml");
	    					foundURLs.add(nativeFileURL);
	    				}
	    			}
	    			if(foundURLs.isEmpty() && new File(directory, "native").isDirectory()) {
	    				for(String jar : new File(directory, "native").list()) {
		    				if(jar.startsWith(fileName) && jar.endsWith(suffix)) {
		    					File nativeFile = new File(new File(directory, "native"), jar);
		    					URL nativeFileURL = createURL("jar:"+nativeFile.toURI()+"!/META-INF/qtjambi-utilities.xml");
		    					foundURLs.add(nativeFileURL);
		    				}
		    			}
	    			}
	    			if(!foundURLs.isEmpty())
	    				specsFound = Collections.enumeration(foundURLs);
	    		}
        	}
        }
    	return specsFound;
	}
	
	static URL createURL(String url) throws MalformedURLException {
    	return new URL(url);
    }
}
