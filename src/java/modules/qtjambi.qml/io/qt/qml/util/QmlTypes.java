/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.qml.util;

import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.logging.Logger;

import io.qt.NativeAccess;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QList;
import io.qt.core.QResource;
import io.qt.core.QStringList;
import io.qt.core.QTextStream;
import io.qt.core.QUrl;
import io.qt.qml.QQmlExtensionPlugin;
import io.qt.qml.QtQml;

/**
 * This class allows to automatically load QML types from package.
 */
public final class QmlTypes {
	
	static {
		QtJambi_LibraryUtilities.initialize();
	}
	
	private QmlTypes() throws java.lang.InstantiationError { throw new java.lang.InstantiationError("Cannot instantiate class QmlTypes."); }
	
	private static class QmlClassLoader extends URLClassLoader{
		public QmlClassLoader() {
			super(new URL[0]);
		}
		protected void addURL(URL url) {
			super.addURL(url);
			QResource.addClassPath(url.toString());
		}
	}
	private static final QmlClassLoader qmlClassLoader = new QmlClassLoader();
	
	@SuppressWarnings("unused")
	private static class JarImport extends QQmlExtensionPlugin{
		@Override
		public void registerTypes(String uri) {
			registerModule(libraryPath, uri);
		}
		
		private final String libraryPath;

		private JarImport(String libraryPath) {
			super();
			this.libraryPath = new QFileInfo(libraryPath).absolutePath();
		}
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * The package needs to be annotated with {@link QmlImport}.
	 * @param pkg java package and qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImport} not available
	 */
	public static void registerPackage(Package pkg) {
		registerPackage(pkg, null);
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * @param pkg java package and qml import namespace
	 * @param versionMajor major version for qml import
	 */
	public static void registerPackage(Package pkg, int versionMajor) {
		registerPackage(pkg, null, versionMajor);
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * The package needs to be annotated with {@link QmlImport}.
	 * @param pkg java package
	 * @param uri qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImport} not available
	 */
	@SuppressWarnings("deprecation")
	public static void registerPackage(Package pkg, String uri) {
		QmlImportMajorVersion importMajorVersion = pkg.getAnnotation(QmlImportMajorVersion.class);
		QmlImport qmlimport = pkg.getAnnotation(QmlImport.class);
		if(importMajorVersion==null && qmlimport==null) {
			try {
				Class<?> infoClass = QmlTypes.class.getClassLoader().loadClass(pkg.getName()+".package-info");
				importMajorVersion = infoClass.getAnnotation(QmlImportMajorVersion.class);
				qmlimport = infoClass.getAnnotation(QmlImport.class);
			} catch (Throwable e) {}
			if(importMajorVersion==null && qmlimport==null)
				throw new QmlNoMajorVersionException("No QmlImport annotation in package "+pkg);
		}
		if(qmlimport!=null) {
			registerPackage(pkg, uri, qmlimport.majorVersion(), qmlimport.classes());
		}else {
			registerPackage(pkg, uri, importMajorVersion.value());
		}
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * @param pkg java package
	 * @param uri qml import namespace
	 * @param versionMajor major version for qml import
	 */
	public static void registerPackage(Package pkg, String uri, int versionMajor, Class<?>... classes) {
		boolean isModule = uri==null;
		if(uri==null)
			uri = pkg.getName();
		List<Class<?>> registeredClasses = new ArrayList<>();
		for(Class<?> cls : classes) {
			if(!registeredClasses.contains(cls))
				registeredClasses.add(cls);
		}
		QDir classPath = new QDir(":"+pkg.getName().replace('.', '/'));
		for(String className : classPath.entryList(Collections.singletonList("*.class"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
			if(className.endsWith(".class") && !className.endsWith("-info.class") && !className.contains("$")){
				className = pkg.getName()+"."+className.substring(0, className.length()-6);
				try {
					Class<?> cls;
					try {
						cls = qmlClassLoader.loadClass(className);
					} catch (Exception e) {
						try {
							cls = ClassLoader.getSystemClassLoader().loadClass(className);
						} catch (Exception e1) {
							try {
								cls = Thread.currentThread().getContextClassLoader().loadClass(className);
							} catch (Exception e2) {
								cls = Class.forName(className);
							}
						}
					}
					if(!registeredClasses.contains(cls))
						registeredClasses.add(cls);
				} catch (Exception e) {
				}
			}
		}
		try{
			analyzeType(uri, versionMajor, null, registeredClasses.toArray(new Class[registeredClasses.size()]));
		} catch (Exception e) {
		}
		for(String qmlFile : classPath.entryList(Collections.singletonList("*.qml"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
			QtQml.qmlRegisterType(new QUrl("qrc:"+pkg.getName().replace('.', '/')+"/"+qmlFile), uri, versionMajor, 0, qmlFile.substring(0, qmlFile.length()-4));
		}
		if(isModule) {
			QtQml.qmlRegisterModule(uri, versionMajor, 0);
			QtQml.qmlProtectModule(uri, versionMajor);
		}
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * The package needs to be annotated with {@link QmlImport}.
	 * @param pkg java package and qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImport} not available
	 */
	public static void registerPackage(String pkg) {
		registerPackage(pkg, null);
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * The package needs to be annotated with {@link QmlImport}.
	 * @param pkg java package
	 * @param uri qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImport} not available
	 */
	public static void registerPackage(String pkg, String uri) {
		java.lang.Package _package = QtJambi_LibraryUtilities.internal.getDefinedPackage(qmlClassLoader, pkg);
		if(_package==null) {
			_package = QtJambi_LibraryUtilities.internal.getDefinedPackage(ClassLoader.getSystemClassLoader(), pkg);
		}
		if(_package==null) {
			_package = QtJambi_LibraryUtilities.internal.getDefinedPackage(Thread.currentThread().getContextClassLoader(), pkg);
		}
		if(_package==null){
			QDir classPath = new QDir(":"+pkg.replace('.', '/'));
			for(String className : classPath.entryList(Collections.singletonList("*.class"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
				if(className.endsWith(".class") && !className.endsWith("-info.class") && !className.contains("$")){
					className = pkg+"."+className.substring(0, className.length()-6);
					try {
						Class<?> cls;
						try {
							cls = qmlClassLoader.loadClass(className);
						} catch (Exception e) {
							try {
								cls = ClassLoader.getSystemClassLoader().loadClass(className);
							} catch (Exception e1) {
								try {
									cls = Thread.currentThread().getContextClassLoader().loadClass(className);
								} catch (Exception e2) {
									cls = Class.forName(className);
								}
							}
						}
						_package = cls.getPackage();
						break;
					} catch (Exception e) {
					}
				}
			}
		}
		if(_package==null) {
			throw new IllegalArgumentException("No such package "+pkg);
		}
		registerPackage(_package, uri);
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * @param pkg java package and qml import namespace
	 * @param versionMajor major version for qml import
	 */
	public static void registerPackage(String pkg, int versionMajor) {
		registerPackage(pkg, null, versionMajor);
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * @param pkg java package
	 * @param uri qml import namespace
	 * @param versionMajor major version for qml import
	 */
	public static void registerPackage(String pkg, String uri, int versionMajor) {
		java.lang.Package _package = QtJambi_LibraryUtilities.internal.getDefinedPackage(qmlClassLoader, pkg);
		if(_package==null) {
			_package = QtJambi_LibraryUtilities.internal.getDefinedPackage(ClassLoader.getSystemClassLoader(), pkg);
		}
		if(_package==null) {
			_package = QtJambi_LibraryUtilities.internal.getDefinedPackage(Thread.currentThread().getContextClassLoader(), pkg);
		}
		if(_package==null){
			QDir classPath = new QDir(":"+pkg.replace('.', '/'));
			for(String className : classPath.entryList(Collections.singletonList("*.class"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
				if(className.endsWith(".class") && !className.endsWith("-info.class") && !className.contains("$")){
					className = pkg+"."+className.substring(0, className.length()-6);
					try {
						Class<?> cls;
						try {
							cls = qmlClassLoader.loadClass(className);
						} catch (Exception e) {
							try {
								cls = ClassLoader.getSystemClassLoader().loadClass(className);
							} catch (Exception e1) {
								try {
									cls = Thread.currentThread().getContextClassLoader().loadClass(className);
								} catch (Exception e2) {
									cls = Class.forName(className);
								}
							}
						}
						_package = cls.getPackage();
						break;
					} catch (Exception e) {
					}
				}
			}
		}
		if(_package==null) {
			throw new IllegalArgumentException("No such package "+pkg);
		}
		registerPackage(_package, uri, versionMajor);
	}
	
	/**
	 * Registers the class as qml scriptable type.
	 * The class package needs to be annotated with {@link QmlImport}.
	 * @param type registered class
	 * @return the new type id
	 * @throws QmlNoMajorVersionException if {@link QmlImport} not available in the package
	 * @throws QmlTypeRegistrationException when class cannot be registered as qml type
	 */
	@SuppressWarnings("deprecation")
	public static int registerType(Class<?> type) {
		if(type.getPackage()==null)
			throw new QmlTypeRegistrationException("Cannot register classes from default package.");
		QmlImportMajorVersion importMajorVersion = type.getPackage().getAnnotation(QmlImportMajorVersion.class);
		QmlImport qmlimport = type.getPackage().getAnnotation(QmlImport.class);
		if(importMajorVersion==null && qmlimport==null)
			throw new QmlNoMajorVersionException("No QmlImport annotation in package "+type.getPackage());
		return qmlimport!=null ? registerType(type, qmlimport.majorVersion()) : registerType(type, importMajorVersion.value());
	}
	
	/**
	 * Registers the class as qml scriptable type.
	 * @param type registered class
	 * @param versionMajor major version for qml import
	 * @return the new type id
	 * @throws QmlTypeRegistrationException when class cannot be registered as qml type
	 */
	public static int registerType(Class<?> type, int versionMajor) {
		return registerType(type, type.getPackage().getName(), versionMajor);
	}
	
	/**
	 * Registers the class as qml scriptable type.
	 * @param type registered class
	 * @param uri qml import namespace
	 * @param versionMajor major version for qml import
	 * @return the new type id
	 * @throws QmlTypeRegistrationException when class cannot be registered as qml type
	 */
	public static int registerType(Class<?> type, String uri, int versionMajor) {
		try {
			QList<Integer> resultTypeIDs = new QList<>(int.class);
			analyzeType(uri, versionMajor, resultTypeIDs, type);
			return resultTypeIDs.isEmpty() ? -1 : resultTypeIDs.first();
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Exception e) {
			throw new QmlTypeRegistrationException("Cannot register class "+type.getName(), e);
		}
	}
	
	private static native void analyzeType(String uri, int versionMajor, Collection<Integer> resultTypeIDs, Class<?>... cls) throws Exception;
	
	private static int getMinorVersion(Class<?> cls) {
		int versionMinor = 0;
		QmlAddedInMinorVersion addedInMinorVersion = cls.getAnnotation(QmlAddedInMinorVersion.class);
		if(addedInMinorVersion!=null)
			versionMinor = addedInMinorVersion.value();
		return versionMinor;
	}
	
	@SuppressWarnings("deprecation")
	@NativeAccess
	private static void registerModule(String libraryDir, String uri) {
		QDir directory = new QDir(libraryDir);
		if(!directory.exists("qmldir")) {
			directory = new QDir(":qml/"+uri.replace(".", "/"));
			if(!directory.exists("qmldir")) {
				directory = new QDir(":qt-project.org/imports/"+uri.replace(".", "/"));
	        }
        }
		QFile qmldir = new QFile(directory.filePath("qmldir"));
        if(qmldir.exists() && qmldir.open(QIODevice.OpenModeFlag.ReadOnly)){
        	QStringList classPath = new QStringList();
            QStringList libraryPath = new QStringList();
        	try {
	            QTextStream s = new QTextStream(qmldir);
	            while(!s.atEnd()){
	                String line = s.readLine();
	                if(line.startsWith("classpath ")){
	                	classPath.clear();
	                    classPath.append(Arrays.asList(line.substring(10).trim().split(",")));
	                }else if(line.startsWith("librarypath ")){
	                	libraryPath.append(Arrays.asList(System.getProperty("java.library.path", "").split(File.pathSeparator)));
						for(String lp : line.substring(12).trim().split(",")) {
							lp = QDir.toNativeSeparators(directory.absoluteFilePath(lp));
							if(!libraryPath.contains(lp)) {
								libraryPath.append(lp);
							}
						}
	                }
	            }
        	}finally {
        		qmldir.close();
        	}
			if(!libraryPath.isEmpty()) {
				System.setProperty("java.library.path", libraryPath.join(File.pathSeparatorChar));
			}
			if(classPath.isEmpty()) {
				classPath.append(directory.entryList(Arrays.asList("*.jar"), QDir.Filter.Files));
			}
			for(String cp : classPath) {
				File file = new File(QDir.toNativeSeparators(directory.absoluteFilePath(cp)));
				try {
					URL url = file.toURI().toURL();
					if(!Arrays.asList(qmlClassLoader.getURLs()).contains(url)) {
						qmlClassLoader.addURL(url);
					}
				} catch (Throwable e) {
					Logger.getLogger("internal").throwing(QmlTypes.class.getName(), "registerTypes()", e);
				}
			}
			java.lang.Package _package = QtJambi_LibraryUtilities.internal.getDefinedPackage(qmlClassLoader, uri);
			if(_package==null) {
				_package = QtJambi_LibraryUtilities.internal.getDefinedPackage(ClassLoader.getSystemClassLoader(), uri);
			}
			if(_package==null) {
				_package = QtJambi_LibraryUtilities.internal.getDefinedPackage(Thread.currentThread().getContextClassLoader(), uri);
			}
			if(_package==null){
				QDir classPathDir = new QDir(":"+uri.replace('.', '/'));
				for(String className : classPathDir.entryList(Collections.singletonList("*.class"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
					if(className.endsWith(".class") && !className.endsWith("-info.class") && !className.contains("$")){
						className = uri+"."+className.substring(0, className.length()-6);
						try {
							Class<?> cls = qmlClassLoader.loadClass(className);
							_package = cls.getPackage();
							break;
						} catch (Exception e) {
						}
					}
				}
			}
			int version = 1;
			if(_package!=null) {
				QmlImportMajorVersion importMajorVersion = _package.getAnnotation(QmlImportMajorVersion.class);
				QmlImport qmlimport = _package.getAnnotation(QmlImport.class);
				if(importMajorVersion==null && qmlimport==null) {
					try {
						Class<?> infoClass = QmlTypes.class.getClassLoader().loadClass(_package.getName()+".package-info");
						importMajorVersion = infoClass.getAnnotation(QmlImportMajorVersion.class);
						qmlimport = infoClass.getAnnotation(QmlImport.class);
					} catch (Throwable e) {}
					if(importMajorVersion==null && qmlimport==null)
						throw new QmlNoMajorVersionException("No QmlImport annotation in package "+_package.getName());
				}
				if(qmlimport!=null) {
					version = qmlimport.majorVersion();
					registerPackage(_package, uri, version, qmlimport.classes());
				}else {
					version = importMajorVersion.value();
					registerPackage(_package, uri, version);
				}
			}
			for(String qmlFile : directory.entryList(Collections.singletonList("*.qml"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
				QUrl url;
				if(directory.path().startsWith(":"))
					url = new QUrl("qrc"+directory.absoluteFilePath(qmlFile));
				else
					url = QUrl.fromLocalFile(directory.absoluteFilePath(qmlFile));
				QtQml.qmlRegisterType(url, uri, version, 0, qmlFile.substring(0, qmlFile.length()-4));
			}
			QtQml.qmlRegisterModule(uri, version, 0);
			QtQml.qmlProtectModule(uri, version);
        }
	}
}
