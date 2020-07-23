/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.invoke.MethodHandle;
import java.lang.reflect.Constructor;
import java.util.List;

import io.qt.QtObjectInterface;
import io.qt.core.QDir;
import io.qt.core.QObject;
import io.qt.core.QUrl;
import io.qt.internal.QtJambiInternal;
import io.qt.qml.QtQml;

/**
 * This class allows to automatically load QML types from package.
 * @author Dr. Peter Droste, Omix Visualization GmbH & Co. KG
 *
 */
public class QmlTypes {
	
	public static void registerPackage(Package pkg) {
		registerPackage(pkg, null);
	}
	
	public static void registerPackage(Package pkg, int versionMajor) {
		registerPackage(pkg, null, versionMajor);
	}
	
	public static void registerPackage(Package pkg, String uri) {
		QmlImportMajorVersion importMajorVersion = pkg.getAnnotation(QmlImportMajorVersion.class);
		if(importMajorVersion==null)
			throw new QmlNoMajorVersionException("No QmlImportMajorVersion annotation in package "+pkg);
		registerPackage(pkg, uri, importMajorVersion.value());
	}
	
	public static void registerPackage(Package pkg, String uri, int versionMajor) {
		if(uri==null)
			uri = pkg.getName();
		QDir classPath = new QDir("classpath:"+pkg.getName().replace('.', '/'));
		for(String className : classPath.entryList(List.of("*.class"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
			if(className.endsWith(".class") && !className.endsWith("-info.class") && !className.contains("$")){
				try {
					Class<?> cls = Class.forName(pkg.getName()+"."+className.substring(0, className.length()-6));
					analyzeType(cls, uri, versionMajor);
				} catch (Exception e) {
				}
			}
		}
		for(String qmlFile : classPath.entryList(List.of("*.qml"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
			QtQml.qmlRegisterType(QUrl.fromClassPath(pkg.getName().replace('.', '/')+"/"+qmlFile), uri, versionMajor, 0, qmlFile.substring(0, qmlFile.length()-4));
		}
	}
	
	public static void registerPackage(String pkg) {
		registerPackage(pkg, null);
	}
	
	public static void registerPackage(String pkg, String uri) {
		java.lang.Package _package = ClassLoader.getSystemClassLoader().getDefinedPackage(pkg);
		if(_package==null) {
			_package = ClassLoader.getPlatformClassLoader().getDefinedPackage(pkg);
		}
		if(_package==null){
			QDir classPath = new QDir("classpath:"+pkg.replace('.', '/'));
			for(String className : classPath.entryList(List.of("*.class"), new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks))) {
				if(className.endsWith(".class") && !className.endsWith("-info.class") && !className.contains("$")){
					try {
						Class<?> cls = Class.forName(pkg+"."+className.substring(0, className.length()-6));
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
	
	public static void registerPackage(String pkg, int versionMajor) {
		registerPackage(pkg, null, versionMajor);
	}
	
	public static void registerPackage(String pkg, String uri, int versionMajor) {
		java.lang.Package _package = ClassLoader.getSystemClassLoader().getDefinedPackage(pkg);
		if(_package==null) {
			_package = ClassLoader.getPlatformClassLoader().getDefinedPackage(pkg);
		}
		if(_package==null) {
			throw new IllegalArgumentException("No such package "+pkg);
		}
		registerPackage(_package, uri, versionMajor);
	}
	
	public static int registerType(Class<? extends QtObjectInterface> type) {
		if(type.getPackage()==null)
			throw new QmlTypeRegistrationException("Cannot register classes from default package.");
		QmlImportMajorVersion importMajorVersion = type.getPackage().getAnnotation(QmlImportMajorVersion.class);
		if(importMajorVersion==null)
			throw new QmlNoMajorVersionException("No QmlImportMajorVersion annotation in package "+type.getPackage());
		return registerType(type, importMajorVersion.value());
	}
	
	public static int registerType(Class<? extends QtObjectInterface> type, int versionMajor) {
		return registerType(type, type.getPackage().getName(), versionMajor);
	}
	
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
					MethodHandle constructorHandle = QtJambiInternal.getConstructorHandle(constructor);
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
}
