/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "pch_p.h"

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_PropertyUtility_getPropertyForField(JNIEnv *env, jclass, jobject jmetaObject, jobject reflectField){
    const QMetaObject* metaObject = qtjambi_cast<const QMetaObject*>(env, jmetaObject);
    if(metaObject){
        jfieldID field = env->FromReflectedField(reflectField);
        for(int i=0; i<metaObject->propertyCount(); ++i){
            QMetaProperty prop = metaObject->property(i);
            if(prop.isValid()){
                if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(prop.enclosingMetaObject())){
                    if(field==dynamicMetaObject->getQPropertyField(prop.relativePropertyIndex())){
                        return qtjambi_cast<jobject>(env, std::move(prop));
                    }
                }
            }
        }
    }
    return nullptr;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_PropertyUtility_registerPropertyField(JNIEnv *env, jclass, QtJambiNativeID metaPropertyId, jobject reflectField){
    QMetaProperty property = QtJambiAPI::valueReferenceFromNativeId<QMetaProperty>(metaPropertyId);
    if(property.isValid()){
        if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(property.enclosingMetaObject())){
            jfieldID field = env->FromReflectedField(reflectField);
            const_cast<QtJambiMetaObject*>(dynamicMetaObject)->registerQPropertyField(property.relativePropertyIndex(), field);
        }
    }
}
