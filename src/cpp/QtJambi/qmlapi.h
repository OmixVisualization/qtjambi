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

#ifndef QMLAPI_H
#define QMLAPI_H

#include "global.h"

class QObject;
struct QMetaObject;
enum class SequentialContainerType;

namespace QmlAPI{

typedef void (*QmlReportDestruction)(QObject * obj);

QTJAMBI_EXPORT void setQmlReportDestruction(QmlReportDestruction fct);

typedef QObject* (* CreateQmlErrorDummyObject) (const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT void registerCreateQmlErrorDummyObjectFunction(CreateQmlErrorDummyObject createQmlErrorDummyObject);

QTJAMBI_EXPORT QObject* createQmlErrorDummyObject(const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT size_t extendedSizeForClass(JNIEnv *env, jclass object_class);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT void registerJavaClassForCustomMetaType(JNIEnv *env, int metaType, jclass javaClass);
#else
QTJAMBI_EXPORT int registerMetaType(JNIEnv *env, jclass clazz, const QString& javaClassName);

QTJAMBI_EXPORT int registerMetaType(JNIEnv *env, SequentialContainerType containerType, const QMetaType& elementType);
QTJAMBI_EXPORT int registerQmlMetaType(JNIEnv *env, jclass clazz,
                                             const QByteArray& typeName,
                                             QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                             QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                             QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                             QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                             QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                             QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                             QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                             QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                             QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                             QtPrivate::QMetaTypeInterface::LegacyRegisterOp legacyRegisterOp,
                                             uint size,
                                             ushort align,
                                             int builtInTypeId,
                                             QMetaType::TypeFlags flags,
                                             const QMetaObject *metaObject,
                                             QtPrivate::QMetaTypeInterface::MetaObjectFn metaObjectFn);
QTJAMBI_EXPORT bool registerMetaTypeConverter(JNIEnv *env, const QMetaType& metaType1, jclass jsvalueClass, const QMetaType& metaType2, jclass targetClass, jmethodID constructor);
QTJAMBI_EXPORT bool registerQmlExtension(JNIEnv *env, const QMetaObject *extended_meta_object, jclass javaClass);
#endif

QTJAMBI_EXPORT int getInterfaceOffset(JNIEnv *env, jclass cls, const std::type_info& interfacetype);

}

#endif // QMLAPI_H
