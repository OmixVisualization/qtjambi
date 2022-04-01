/****************************************************************************
**
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

package io.qt.qml.util;

import static io.qt.qml.util.RetroHelper.getDefinedPackage;

import java.io.File;
import java.lang.invoke.MethodHandle;
import java.lang.reflect.Constructor;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Arrays;
import java.util.Collections;
import java.util.logging.Logger;

import io.qt.NativeAccess;
import io.qt.QtObjectInterface;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QObject;
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
		QtJambi_LibraryInitializer.init();
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
	 * The package needs to be annotated with {@link QmlImportMajorVersion}.
	 * @param pkg java package and qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImportMajorVersion} not available
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
	 * The package needs to be annotated with {@link QmlImportMajorVersion}.
	 * @param pkg java package
	 * @param uri qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImportMajorVersion} not available
	 */
	public static void registerPackage(Package pkg, String uri) {
		QmlImportMajorVersion importMajorVersion = pkg.getAnnotation(QmlImportMajorVersion.class);
		if(importMajorVersion==null)
			throw new QmlNoMajorVersionException("No QmlImportMajorVersion annotation in package "+pkg);
		registerPackage(pkg, uri, importMajorVersion.value());
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * @param pkg java package
	 * @param uri qml import namespace
	 * @param versionMajor major version for qml import
	 */
	public static void registerPackage(Package pkg, String uri, int versionMajor) {
		if(uri==null)
			uri = pkg.getName();
		QDir classPath = new QDir("classpath:"+pkg.getName().replace('.', '/'));
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
					analyzeType(cls, uri, versionMajor);
				} catch (Exception e) {
				}
			}
		}
		for(String qmlFile : classPath.entryList(Collections.singletonList("*.qml"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
			QtQml.qmlRegisterType(QUrl.fromClassPath(pkg.getName().replace('.', '/')+"/"+qmlFile), uri, versionMajor, 0, qmlFile.substring(0, qmlFile.length()-4));
		}
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * The package needs to be annotated with {@link QmlImportMajorVersion}.
	 * @param pkg java package and qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImportMajorVersion} not available
	 */
	public static void registerPackage(String pkg) {
		registerPackage(pkg, null);
	}
	
	/**
	 * Registers all scriptable classes and qml types in the given package.
	 * The package needs to be annotated with {@link QmlImportMajorVersion}.
	 * @param pkg java package
	 * @param uri qml import namespace
	 * @throws QmlNoMajorVersionException if {@link QmlImportMajorVersion} not available
	 */
	public static void registerPackage(String pkg, String uri) {
		java.lang.Package _package = getDefinedPackage(qmlClassLoader, pkg);
		if(_package==null) {
			_package = getDefinedPackage(ClassLoader.getSystemClassLoader(), pkg);
		}
		if(_package==null) {
			_package = getDefinedPackage(Thread.currentThread().getContextClassLoader(), pkg);
		}
		if(_package==null){
			QDir classPath = new QDir("classpath:"+pkg.replace('.', '/'));
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
		java.lang.Package _package = getDefinedPackage(qmlClassLoader, pkg);
		if(_package==null) {
			_package = getDefinedPackage(ClassLoader.getSystemClassLoader(), pkg);
		}
		if(_package==null) {
			_package = getDefinedPackage(Thread.currentThread().getContextClassLoader(), pkg);
		}
		if(_package==null) {
			throw new IllegalArgumentException("No such package "+pkg);
		}
		registerPackage(_package, uri, versionMajor);
	}
	
	/**
	 * Registers the class as qml scriptable type.
	 * The class package needs to be annotated with {@link QmlImportMajorVersion}.
	 * @param type registered class
	 * @return the new type id
	 * @throws QmlNoMajorVersionException if {@link QmlImportMajorVersion} not available in the package
	 * @throws QmlTypeRegistrationException when class cannot be registered as qml type
	 */
	public static int registerType(Class<? extends QtObjectInterface> type) {
		if(type.getPackage()==null)
			throw new QmlTypeRegistrationException("Cannot register classes from default package.");
		QmlImportMajorVersion importMajorVersion = type.getPackage().getAnnotation(QmlImportMajorVersion.class);
		if(importMajorVersion==null)
			throw new QmlNoMajorVersionException("No QmlImportMajorVersion annotation in package "+type.getPackage());
		return registerType(type, importMajorVersion.value());
	}
	
	/**
	 * Registers the class as qml scriptable type.
	 * @param type registered class
	 * @param versionMajor major version for qml import
	 * @return the new type id
	 * @throws QmlTypeRegistrationException when class cannot be registered as qml type
	 */
	public static int registerType(Class<? extends QtObjectInterface> type, int versionMajor) {
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
	public static int registerType(Class<? extends QtObjectInterface> type, String uri, int versionMajor) {
		try {
			return analyzeType(type, uri, versionMajor);
		} catch (Exception e) {
			throw new QmlTypeRegistrationException("Cannot register class "+type.getName(), e);
		}
	}
	
	@SuppressWarnings("unchecked")
	private static int analyzeType(Class<?> cls, String uri, int versionMajor) throws Exception {
		if(cls.isInterface()) {
			if(QtObjectInterface.class.isAssignableFrom(cls) && cls.isAnnotationPresent(QmlInterface.class))
				return QtQml.qmlRegisterInterface((Class<? extends QtObjectInterface>)cls, uri, versionMajor);
		}else if(QObject.class.isAssignableFrom(cls)){
			if(cls.isAnnotationPresent(QmlAnonymous.class)){
				return QtQml.qmlRegisterAnonymousType((Class<? extends QObject>)cls, uri, versionMajor);
			}else if(cls.isAnnotationPresent(QmlElement.class) || cls.isAnnotationPresent(QmlNamedElement.class)) {
				String qmlName = cls.getSimpleName();
				QmlNamedElement namedElement = cls.getAnnotation(QmlNamedElement.class);
				if(namedElement!=null)
					qmlName = namedElement.name();
				if(cls.isAnnotationPresent(QmlUncreatable.class)) {
					String reason = null;
					QmlUncreatable unc = cls.getAnnotation(QmlUncreatable.class);
					if(unc!=null)
						reason = unc.reason();
					return QtQml.qmlRegisterUncreatableType((Class<? extends QObject>)cls, uri, versionMajor, getMinorVersion(cls), qmlName, reason);
				}else if(cls.isAnnotationPresent(QmlSingleton.class)) {
					Constructor<?> constructor = cls.getConstructor();
					MethodHandle constructorHandle = io.qt.internal.QtJambiInternal.getConstructorHandle(constructor);
					return QtQml.qmlRegisterSingletonType((Class<? extends QObject>)cls, uri, versionMajor, getMinorVersion(cls), qmlName, (qengine,jsengine)->{
						try {
							return (QObject)constructorHandle.invoke();
						} catch (Throwable e) {
							throw new RuntimeException(e);
						}
					});
				}else {
					return QtQml.qmlRegisterType((Class<? extends QObject>)cls, uri, versionMajor, getMinorVersion(cls), qmlName);
				}
			}
		}
		return -1;
	}
	
	private static int getMinorVersion(Class<?> cls) {
		int versionMinor = 0;
		QmlAddedInMinorVersion addedInMinorVersion = cls.getAnnotation(QmlAddedInMinorVersion.class);
		if(addedInMinorVersion!=null)
			versionMinor = addedInMinorVersion.value();
		return versionMinor;
	}
	
	@NativeAccess
	private static void registerModule(String libraryDir, String uri) {
		QDir directory = new QDir(libraryDir);
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
			java.lang.Package _package = getDefinedPackage(qmlClassLoader, uri);
			if(_package==null){
				QDir classPathDir = new QDir("classpath:"+uri.replace('.', '/'));
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
				if(importMajorVersion==null)
					throw new QmlNoMajorVersionException("No QmlImportMajorVersion annotation in package "+_package.getName());
				version = importMajorVersion.value();
				registerPackage(_package, uri, version);
			}
			for(String qmlFile : directory.entryList(Collections.singletonList("*.qml"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
				QtQml.qmlRegisterType(new QUrl(directory.absoluteFilePath(qmlFile)), uri, version, 0, qmlFile.substring(0, qmlFile.length()-4));
			}
			QtQml.qmlRegisterModule(uri, version, 0);
        }
	}
}
