/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "utils.h"
#include "utils_p.h"

class QtJambiTypeManager {
public:
    enum TypePattern : quint64 {
        None = 0,
#if QT_VERSION >= 0x050000
        Void            = 0x00100000,
#endif
        Primitive       = 0x00000001,
        Integer         = 0x00000002,
        Long            = 0x00000004,
        Boolean         = 0x00000008,
        Float           = 0x00000010,
        Double          = 0x00000020,
        Short           = 0x00000040,
        Byte            = 0x00000080,
        Char            = 0x00000100,

        QObjectSubclass = 0x00000200,
        Object          = 0x00000400,

        NativePointer   = 0x00000800,

        Value           = 0x00001000,
        String          = 0x00002000,
        QtClass         = 0x00004000,
        Enum            = 0x00008000,
        Flags           = 0x00010000,
        Array           = 0x00020000,
        QtSubClass      = 0x00040000,
        SharedPointerWrapped = 0x00080000,
        WeakPointerWrapped   = 0x00200000,
        initializerListWrapper = 0x00400000,
        VariantPattern  = 0x00800000,
        Reference       = 0x01000000,
        Pointer         = 0x02000000,
        SharedPtrWrapped = 0x04000000,
        WeakPtrWrapped   = 0x08000000,
        UniquePtrWrapped = 0x10000000,
        ScopedPointerWrapped = 0x20000000,

        TypeMask = Integer + Long + Boolean + Float + Double + Short + Byte + Char
    };

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static bool hasRegisteredComparators(int id);
#endif
    static void splitClassName(QString& className, QString& package, const QString &qualifiedName, QChar separator = QLatin1Char('/'));

    static size_t getInternalSize(const QString &internalTypeName);
    static size_t getInternalAlignment(const QString &internalTypeName);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static bool isStaticType(const QString &internalTypeName);
#endif
    static QString getExternalTypeName(JNIEnv* env, const QString &internalTypeName, const QMetaObject * metaObject, const QMetaType& metaType = QMetaType());
    static QString getExternalTypeName(JNIEnv* env, const QString &internalTypeName, const QMetaType& metaType = QMetaType());
    static QString getInternalTypeName(JNIEnv* env, const QString &externalTypeName, jobject classLoader = nullptr, bool useNextSuperclass = true);
    static QString getInternalTypeName(JNIEnv* env, jclass externalClass, bool useNextSuperclass = true);
    static InternalToExternalConverter getInternalToExternalConverter(
                                   JNIEnv* env,
                                   const QString &internalTypeName,
                                   const QMetaType& internalMetaType,
                                   jclass externalClass,
                                   bool allowValuePointers = false);
    static ExternalToInternalConverter getExternalToInternalConverter(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType);

    static InternalToExternalConverter tryGetInternalToExternalConverter(
                                   JNIEnv* env,
                                   const QString &internalTypeName,
                                   const QMetaType& internalMetaType,
                                   jclass externalClass,
                                   bool allowValuePointers = false);
    static ExternalToInternalConverter tryGetExternalToInternalConverter(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType);

    static QStringList parseSignature(const QString &signature, QString *name = nullptr);
    static QHashFunction findHashFunction(bool isPointer, int metaType);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static void registerInternalAlignment(const QByteArray &internalTypeName, size_t align);
#endif

private:
    enum class PointerType{
        NoPointer,
        SharedPointer,
        WeakPointer,
        ScopedPointer,
        unique_ptr,
        shared_ptr,
        weak_ptr,
        initializer_list
    };

    static void checkClassName(QObject* qobject, QString className, QString &_externalClassName, QString &_externalClassPackage);

    static TypePattern valueTypePattern(const QString &javaName);
    static QString closestQtSuperclass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);
    static bool isQObjectSubclass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);
    static bool isQtClass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);
    static bool isQtSubClass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);

    static InternalToExternalConverter getInternalToExternalConverterImpl(
                                   JNIEnv* env,
                                   QString internalTypeName,
                                   const QMetaType& internalMetaType,
                                   jclass externalClass,
                                   bool allowValuePointers = false);
    static ExternalToInternalConverter getExternalToInternalConverterImpl(JNIEnv* env, jclass externalClass, QString internalTypeName, const QMetaType& internalMetaType);
    static QString processInternalTypeName(const QString &typeName, PointerType &pointerType);
};

#endif
