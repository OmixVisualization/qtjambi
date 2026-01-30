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

#include "pch_p.h"

QExplicitlySharedDataPointer<QmlAPI::CreatorFunctionMetaData> QmlAPI::creatorFunctionMetaData(JNIEnv * env, const QMetaObject *meta_object, jclass clazz, jmethodID constructor, QmlAPI::ConstructorKind constructorKind, QtJambiAPI::ConstructorFn constructorFunction, size_t objectSize, int psCast, int vsCast, int viCast, int fhCast){
    size_t hash = qHashMulti(0, meta_object, QtJambiAPI::getJavaObjectHashCode(env, clazz), qint64(constructor), constructorKind, qint64(constructorFunction), objectSize, psCast, vsCast, viCast, fhCast);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker lock(storage->lock());
        if(storage->creatorFunctionMetaDataHash().contains(hash)){
            return storage->creatorFunctionMetaDataHash()[hash];
        }
    }
    {
        clazz = JavaAPI::toGlobalReference(env, clazz);
        QExplicitlySharedDataPointer<QmlAPI::CreatorFunctionMetaData> data{new QmlAPI::CreatorFunctionMetaData{QSharedData{},clazz, meta_object, constructor, constructorKind, constructorFunction, objectSize, psCast, vsCast, viCast, fhCast}};
        QWriteLocker lock(storage->lock());
        storage->creatorFunctionMetaDataHash()[hash] = data;
        return data;
    }
}

int QmlAPI::getInterfaceOffset(JNIEnv *env, jclass cls, const std::type_info& interfacetype){
    if(const InterfaceOffsetInfo* info = getInterfaceOffsets(env, cls)){
        if(info->offsets.contains(unique_id(interfacetype)))
            return int(info->offsets.value(unique_id(interfacetype), 0));
    }
    return -1;
}
