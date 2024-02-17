/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.internal;

import java.lang.reflect.*;
import io.qt.*;
import io.qt.core.*;

class AbstractMetaObjectUtility {
    static final PropertyFlags ResolveDesignable = PropertyFlags.ResolveDesignable;
    static final PropertyFlags ResolveScriptable = PropertyFlags.ResolveScriptable;
    static final PropertyFlags ResolveEditable = PropertyFlags.ResolveEditable;
    static final PropertyFlags Editable = PropertyFlags.Editable;
    static final PropertyFlags Notify = PropertyFlags.Notify;
    static final PropertyFlags ResolveStored = PropertyFlags.ResolveStored;
    static final PropertyFlags ResolveUser = PropertyFlags.ResolveUser;
    static final PropertyFlags Required;
    
    static{
        PropertyFlags _Required = null;
        try{
            _Required = PropertyFlags.valueOf("Required");
        }catch(Throwable t){}
        Required = _Required;
    }
    
    static final PropertyFlags Bindable = null;
    static final PropertyFlags Alias = null;
    static final MetaObjectFlags PropertyAccessInStaticMetaCall = MetaObjectFlags.PropertyAccessInStaticMetaCall;
    
    static MetaObjectUtility.AnnotationInfo analyzeBindableAnnotation(Method method) {
        return null;
    }
    
    static boolean isValidBindable(Method method) {
        return false;
    }
    
    static boolean isValidQProperty(Field field) {
        return false;
    }
    
    static MetaObjectUtility.QPropertyTypeInfo getQPropertyTypeInfo(Field field) {
        return null;
    }
    
    static MetaObjectUtility.QPropertyTypeInfo getQPropertyTypeInfo(Method method) {
        return null;
    }
    
    static <PropertyInfo> PropertyInfo analyzeProperty(QObject object, QtObject property, Class<PropertyInfo> propertyInfoClass) {
        return null;
    }
    
    static void registerPropertyField(QMetaProperty metaProperty, Field field){}
    
    static boolean isListType(Class<?> cls) {
        return cls==QList.class
                || cls==QQueue.class
                || cls==QStack.class
                || cls==QVector.class
                || cls==QLinkedList.class
                || cls==QSet.class;
    }
	
	static boolean isSequentialContainer(Class<?> cls) {
    	return QList.class.isAssignableFrom(cls)
    			|| QSet.class.isAssignableFrom(cls)
    			|| QVector.class.isAssignableFrom(cls)
    			|| QLinkedList.class.isAssignableFrom(cls);
    }
}
