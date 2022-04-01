/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
package io.qt.internal;

import static io.qt.internal.NativeLibraryManager.*;

import java.io.File;
import java.io.FileInputStream;
import java.util.Properties;

import io.qt.QtUninvokable;

final class QtJambi_LibraryUtilities {
	
    static {
    	Thread shutdownHook = null;
        try {
        	if(!Boolean.getBoolean("io.qt.no-library-shutdown-hook")) {
        		shutdownHook = RetroHelper.newShutdownThread(QtJambi_LibraryUtilities::shutdown, "QtJambi_LibraryShutdown");
        		shutdownHook.setContextClassLoader(null);
        		Runtime.getRuntime().addShutdownHook(shutdownHook);
        	}
        	File coreLib = loadQtCore();
        	try{
        		loadQtJambiLibrary();
            } catch(UnsatisfiedLinkError t) {
            	switch(NativeLibraryManager.operatingSystem) {
            	case MacOSX:
            		if(coreLib!=null) {
            			File prl = new File(coreLib.getParentFile(), "Resources/QtCore.prl");
            			if(prl.exists()) {
            				Properties prlProp = new Properties();
            			    try(FileInputStream inStream = new FileInputStream(prl)){
            			    	prlProp.load(inStream);
            			    } catch(Throwable t2) {}
            			    String version = prlProp.getProperty("QMAKE_PRL_VERSION", "");
            			    if(!version.isEmpty()) {
            			    	if(!version.startsWith(QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + ".")) {
            			    		throw new LinkageError("Cannot combine QtJambi " + QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + " with Qt " + version + ".", t);
            			    	}
            			    }
            			}
            		}
            		break;
				case Windows:
					if(coreLib!=null) {
						File prl = new File(coreLib.getParentFile(), "Qt"+QtJambiVersion.qtMajorVersion+"Core.prl");
						if(!prl.exists()) {
							prl = new File(coreLib.getParentFile().getParentFile(), "lib\\Qt"+QtJambiVersion.qtMajorVersion+"Core.prl");
						}
            			if(prl.exists()) {
            				Properties prlProp = new Properties();
            			    try(FileInputStream inStream = new FileInputStream(prl)){
            			    	prlProp.load(inStream);
            			    } catch(Throwable t2) {}
            			    String version = prlProp.getProperty("QMAKE_PRL_VERSION", "");
            			    if(!version.isEmpty()) {
            			    	if(!version.startsWith(QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + ".")) {
            			    		throw new LinkageError("Cannot combine QtJambi " + QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + " with Qt " + version + ".", t);
            			    	}
            			    }
            			}
						if(new File(coreLib.getParentFile(), "libstdc++-6.dll").exists() || isMinGWBuilt()) {
							throw new LinkageError("Cannot combine msvc-based QtJambi with mingw-based Qt library. Please install and use Qt (MSVC 2019 x64) instead.", t);
						}else {
							throw new LinkageError("Cannot combine mingw-based QtJambi with msvc-based Qt library. Please install and use Qt (MinGW x64) instead.", t);
						}
					}
					break;
				default:
					if(coreLib!=null) {
            			File prl = new File(coreLib.getParentFile(), "Qt"+QtJambiVersion.qtMajorVersion+"Core.prl");
            			if(prl.exists()) {
            				Properties prlProp = new Properties();
            			    try(FileInputStream inStream = new FileInputStream(prl)){
            			    	prlProp.load(inStream);
            			    } catch(Throwable t2) {}
            			    String version = prlProp.getProperty("QMAKE_PRL_VERSION", "");
            			    if(!version.isEmpty()) {
            			    	if(!version.startsWith(QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + ".")) {
            			    		throw new LinkageError("Cannot combine QtJambi " + QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + " with Qt " + version + ".", t);
            			    	}
            			    }
            			}
            		}
					break;
            	}
                throw t;
        	}
        } catch(Error t) {
    		if(shutdownHook!=null)
        		Runtime.getRuntime().removeShutdownHook(shutdownHook);
        	resetDeploymentSpecs();
            throw t;
        } catch(Throwable t) {
    		if(shutdownHook!=null)
        		Runtime.getRuntime().removeShutdownHook(shutdownHook);
        	resetDeploymentSpecs();
            throw new ExceptionInInitializerError(t);
        }
    }

    @QtUninvokable
    private static native void shutdown();
    
    static void initialize() { };

    private QtJambi_LibraryUtilities() throws java.lang.InstantiationError { throw new java.lang.InstantiationError("Cannot instantiate QtJambi_LibraryUtilities."); }
}
