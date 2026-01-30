/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
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
** 
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.internal;

import java.io.File;
import java.net.URI;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import io.qt.QLibraryNotFoundError;
import io.qt.core.QCoreApplication;

public final class LibraryExtractor {
	
	private LibraryExtractor() {}
	private static final String MANIFEST_MF = "META-INF/MANIFEST.MF";

	public static void main(String[] args) throws Throwable {
		File targetDirectory = null;
		for (String arg : args) {
			if(!arg.startsWith("-")) {
				targetDirectory = new File(arg);
			}
		}
		if(targetDirectory==null) {
			String deploymentdir = System.getProperty("io.qt.deploymentdir", "");
			if(!deploymentdir.isEmpty()) {
				targetDirectory = new File(deploymentdir);
			}
		}
		if(targetDirectory==null) {
			System.err.println("Either specify target directory path as program argument or by using -Dio.qt.deploymentdir JVM argument.");
			System.exit(-1);
			return;
		}
		if(!targetDirectory.isDirectory()) {
			if(!targetDirectory.mkdirs()) {
				System.err.println("Unable to create directory "+targetDirectory.getAbsolutePath());
				System.exit(-1);
				return;
			}
		}
		System.setProperty("io.qt.deploymentdir", targetDirectory.getAbsolutePath());
		System.setProperty("io.qt.keep-temp-deployment", "true");
		System.setProperty("io.qt.no-library-shutdown-hook", "true");
		ClassLoader loader = Thread.currentThread().getContextClassLoader();
        if (loader == null) {
            loader = LibraryExtractor.class.getClassLoader();
            assert(loader != null);
        }
		
		List<URL> specsFound = new ArrayList<>();
		Enumeration<URL> enm = loader.getResources(MANIFEST_MF);
		while(enm.hasMoreElements()) {
			specsFound.add(enm.nextElement());
		}
		
		List<URL> urls = new ArrayList<>();
		for(URL _url : specsFound) {
        	String url = _url.toString();
        	if(url.startsWith("jar:file:") && url.endsWith(MANIFEST_MF)) {
        		url = url.substring(4, url.length()-MANIFEST_MF.length()-2);
        		urls.add(URI.create(url).toURL());
        	}
        }
		
		List<String> classNames = new ArrayList<>();
		for (URL url : urls) {
			try(JarFile jarFile = new JarFile(new File(url.toURI()))){
        		Enumeration<JarEntry> entries = jarFile.entries();
        		while(entries.hasMoreElements()) {
        			JarEntry jarEntry = entries.nextElement();
        			if(jarEntry.getName().endsWith("/QtJambi_LibraryUtilities.class")) {
        				String className = jarEntry.getName();
        				className = className.substring(0, className.length()-6).replace('/', '.');
        				classNames.add(className);
        			}
        		}
    		}
		}
		classNames.remove("io.qt.core.QtJambi_LibraryUtilities");
		System.out.println("Extracting libraries of io.qt.core");
		QCoreApplication.initialize(args);
		try{
			LibraryUtility.dontLoadLibraries = true;
			for (String className : classNames) {
				try {
					System.out.println("Extracting libraries of "+className.substring(0, className.length()-25));
					Class.forName(className, true, loader);
				} catch (UnsatisfiedLinkError e) {
				} catch (ExceptionInInitializerError e) {
				} catch (QLibraryNotFoundError e) {
					if(!e.getMessage().contains("was not found due to missing module") && !e.getMessage().contains("was not found in 'io.qt.library-path-override="))
						System.err.println(e.getMessage());
						
				}
			}
		}finally {
			QCoreApplication.shutdown();
		}
	}

}
