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

#ifndef QTJAMBITYPEMANAGER_H
#define QTJAMBITYPEMANAGER_H

#include <QtCore/QMetaType>
#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QReadWriteLock>
#include <functional>
#include "containerutils.h"
#include "utils_p.h"

class QtJambiTypeManager {
public:
    static size_t getInternalSize(QByteArrayView internalTypeName);
    static size_t getInternalAlignment(QByteArrayView internalTypeName);
    static QByteArray getExternalTypeName(JNIEnv* env, QByteArrayView internalTypeName, const QMetaObject * metaObject, const QMetaType& metaType = QMetaType());
    static QByteArray getExternalTypeName(JNIEnv* env, QByteArrayView internalTypeName, const QMetaType& metaType = QMetaType());
    static QByteArray getInternalTypeName(JNIEnv* env, QByteArrayView externalTypeName, jobject classLoader = nullptr, bool useNextSuperclass = true);
    static QByteArray getInternalTypeName(JNIEnv* env, jclass externalClass, bool useNextSuperclass = true);
    static QtJambiUtils::InternalToExternalConverter getInternalToExternalConverter(
                                   JNIEnv* env,
                                   QByteArrayView internalTypeName,
                                   const QMetaType& internalMetaType,
                                   jclass externalClass,
                                   bool allowValuePointers = false);
    static QtJambiUtils::ExternalToInternalConverter getExternalToInternalConverter(JNIEnv* env, jclass externalClass, QByteArrayView internalTypeName, const QMetaType& internalMetaType);

    static QtJambiUtils::InternalToExternalConverter tryGetInternalToExternalConverter(
                                   JNIEnv* env,
                                   QByteArrayView internalTypeName,
                                   const QMetaType& internalMetaType,
                                   jclass externalClass,
                                   bool allowValuePointers = false);
    static QtJambiUtils::ExternalToInternalConverter tryGetExternalToInternalConverter(JNIEnv* env, jclass externalClass, QByteArrayView internalTypeName, const QMetaType& internalMetaType);

    static QtJambiUtils::QHashFunction findHashFunction(bool isPointer, QMetaType metaType);

private:
    enum class PointerType{
        NoPointer,
        SharedPointer,
        WeakPointer,
        ScopedPointer,
        ScopedArrayPointer,
        TrackingPointer,
        unique_ptr,
        shared_ptr,
        weak_ptr,
        initializer_list
    };

    static QtJambiUtils::InternalToExternalConverter getInternalToExternalConverterImpl(
                                   JNIEnv* env,
                                   QByteArrayView internalTypeName,
                                   const QMetaType& internalMetaType,
                                   jclass externalClass,
                                   bool allowValuePointers = false);
    static QtJambiUtils::ExternalToInternalConverter getExternalToInternalConverterImpl(JNIEnv* env, jclass externalClass, QByteArrayView internalTypeName, const QMetaType& internalMetaType);
    static QByteArrayView processInternalTypeName(QByteArrayView typeName, PointerType &pointerType);
};

#endif
