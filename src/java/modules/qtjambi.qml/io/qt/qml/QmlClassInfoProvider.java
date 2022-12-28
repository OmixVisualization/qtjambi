/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.qml;

import java.util.*;

import io.qt.QtMetaType;
import io.qt.core.QMetaType;
import io.qt.qml.util.*;

final class QmlClassInfoProvider {
	
	static final Map<String,Integer> packageVersions = Collections.synchronizedMap(new TreeMap<>());
	
	@SuppressWarnings("deprecation")
	static int majorVersion(Package pkg) {
		return packageVersions.computeIfAbsent(pkg.getName(), packageName->{
			QmlImportMajorVersion importMajorVersion = pkg.getAnnotation(QmlImportMajorVersion.class);
    		QmlImport qmlimport = pkg.getAnnotation(QmlImport.class);
    		if(importMajorVersion==null && qmlimport==null) {
    			try {
    				Class<?> infoClass = QtJambi_LibraryUtilities.class.getClassLoader().loadClass(packageName+".package-info");
    				importMajorVersion = infoClass.getAnnotation(QmlImportMajorVersion.class);
    				qmlimport = infoClass.getAnnotation(QmlImport.class);
    			} catch (Throwable e) {}
    		}
    		if(qmlimport!=null) {
    			return qmlimport.majorVersion();
    		}
    		if(importMajorVersion!=null) {
    			return importMajorVersion.value();
    		}
    		return 1;
		});
	}
	
	static Map<String,String> provide(Class<?> cls) {
		Map<String,String> map = Collections.emptyMap();
		if(cls.isAnnotationPresent(QmlListPropertyAssignBehavior.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.ListPropertyAssignBehavior", cls.getAnnotation(QmlListPropertyAssignBehavior.class).value().name());
    	}
		if(cls.isAnnotationPresent(QmlElement.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Element", cls.getSimpleName());
    	}
    	if(cls.isAnnotationPresent(QmlNamedElement.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Element", cls.getAnnotation(QmlNamedElement.class).name());
    	}
    	if(cls.isAnnotationPresent(QmlValueType.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Element", cls.getAnnotation(QmlValueType.class).name());
    		map.put("QML.Creatable", "false");
    		map.put("QML.UncreatableReason", "Value types cannot be created.");
    	}
    	if(cls.isAnnotationPresent(QmlAnonymous.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Element", "anonymous");
    	}
    	if(cls.isAnnotationPresent(QmlInterface.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Element", "anonymous");
    	}
    	if(cls.isAnnotationPresent(QmlSingleton.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Singleton", "true");
    	}
    	if(cls.isAnnotationPresent(QmlUncreatable.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Creatable", "false");
    		map.put("QML.UncreatableReason", cls.getAnnotation(QmlUncreatable.class).reason());
    	}
    	if(cls.isAnnotationPresent(QmlAddedInMinorVersion.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		int majorVersion = majorVersion(cls.getPackage());
    		map.put("QML.AddedInVersion", majorVersion+"."+cls.getAnnotation(QmlAddedInMinorVersion.class).value());
    	}
    	if(cls.isAnnotationPresent(QmlAddedInVersion.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		QmlAddedInVersion v = cls.getAnnotation(QmlAddedInVersion.class);
    		map.put("QML.AddedInVersion", v.majorVersion()+"."+v.minorVersion());
    	}
    	if(cls.isAnnotationPresent(QmlRemovedInMinorVersion.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		int majorVersion = majorVersion(cls.getPackage());
    		map.put("QML.RemovedInVersion", majorVersion+"."+cls.getAnnotation(QmlRemovedInMinorVersion.class).value());
    	}
    	if(cls.isAnnotationPresent(QmlRemovedInVersion.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		QmlRemovedInVersion v = cls.getAnnotation(QmlRemovedInVersion.class);
    		map.put("QML.RemovedInVersion", v.majorVersion()+"."+v.minorVersion());
    	}
    	if(cls.isAnnotationPresent(QmlExtraVersion.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		QmlExtraVersion v = cls.getAnnotation(QmlExtraVersion.class);
    		map.put("QML.ExtraVersion", v.majorVersion()+"."+v.minorVersion());
    	}
    	if(cls.isAnnotationPresent(QmlExtended.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		Class<?> extendedType = cls.getAnnotation(QmlExtended.class).value();
    		if(extendedType==cls) {
    			String name = map.get("QML.Element");
	    		if(name!=null) {
	    			switch(name) {
	    			case "auto":
	    			case "anonymous":
	    				name = extendedType.getSimpleName();
	    			}
	    		}else {
	    			name = extendedType.getSimpleName();
	    		}
	    		map.put("QML.Extended", name);
    		}else {
	    		Map<String,String> extendedInfo = provide(extendedType);
	    		String name = extendedInfo.get("QML.Element");
	    		if(name!=null) {
	    			switch(name) {
	    			case "auto":
	    			case "anonymous":
	    				name = extendedType.getSimpleName();
	    			}
	    		}else {
	    			name = extendedType.getSimpleName();
	    		}
	    		map.put("QML.Extended", name);
    		}
    	}
    	if(cls.isAnnotationPresent(QmlAttached.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		Class<?> attachedType = cls.getAnnotation(QmlAttached.class).value();
    		Map<String,String> extendedInfo = provide(attachedType);
    		String name = extendedInfo.get("QML.Element");
    		if(name!=null) {
    			switch(name) {
    			case "auto":
    			case "anonymous":
    				name = attachedType.getSimpleName();
    			}
    		}else {
    			name = attachedType.getSimpleName();
    		}
    		map.put("QML.Attached", name);
    	}
    	if(cls.isAnnotationPresent(QmlForeign.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		QtMetaType metaType = cls.getAnnotation(QmlForeign.class).metaType();
    		if(!metaType.name().isEmpty()) {
    			map.put("QML.Foreign", metaType.name());
    		}else if(metaType.id()!=0) {
    			map.put("QML.Foreign", new QMetaType(metaType.id()).name().toString());
    		}else if(metaType.type()!=QMetaType.Type.UnknownType) {
    			map.put("QML.Foreign", new QMetaType(metaType.type()).name().toString());
    		}else {
        		Class<?> foreignType = cls.getAnnotation(QmlForeign.class).value();
        		Map<String,String> extendedInfo = provide(foreignType);
        		String name = extendedInfo.get("QML.Element");
        		if(name!=null) {
        			switch(name) {
        			case "auto":
        			case "anonymous":
        				name = foreignType.getSimpleName();
        			}
        		}else {
        			name = foreignType.getSimpleName();
        		}
        		map.put("QML.Foreign", name);
    		}
    	}
    	if(cls.isAnnotationPresent(QmlUnavailable.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		map.put("QML.Foreign", "QQmlTypeNotAvailable");
    	}
    	if(cls.isAnnotationPresent(QmlSequencialContainer.class)) {
    		if(map.isEmpty())
    			map = new java.util.Hashtable<>();
    		QtMetaType valueType = cls.getAnnotation(QmlSequencialContainer.class).valueType();
    		if(!valueType.name().isEmpty()) {
    			map.put("QML.Sequence", valueType.name());
    		}else if(valueType.id()!=0) {
    			map.put("QML.Sequence", new QMetaType(valueType.id()).name().toString());
    		}else if(valueType.type()!=QMetaType.Type.UnknownType) {
    			map.put("QML.Sequence", new QMetaType(valueType.type()).name().toString());
    		}else {
    			Class<?> foreignType = cls.getAnnotation(QmlSequencialContainer.class).value();
        		Map<String,String> extendedInfo = provide(foreignType);
        		String name = extendedInfo.get("QML.Element");
        		if(name!=null) {
        			switch(name) {
        			case "auto":
        			case "anonymous":
        				name = foreignType.getSimpleName();
        			}
        		}else {
        			name = foreignType.getSimpleName();
        		}
        		map.put("QML.Sequence", name);
    		}
    	}
    	return map;
	}
}
