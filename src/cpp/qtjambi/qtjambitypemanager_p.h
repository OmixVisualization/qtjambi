/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Jambi API.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//
//

#include "qtjambi_global.h"

#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVector>

class QTJAMBI_EXPORT QtJambiTypeManager {
    public:
        enum Type {
            None = 0,
#if QT_VERSION >= 0x050000
            Void            = 0x100000,
#endif
            Primitive       = 0x000001,
            Integer         = 0x000002,
            Long            = 0x000004,
            Boolean         = 0x000008,
            Float           = 0x000010,
            Double          = 0x000020,
            Short           = 0x000040,
            Byte            = 0x000080,
            Char            = 0x000100,

            QObjectSubclass = 0x000200,
            Object          = 0x000400,

            NativePointer   = 0x000800,

            Value           = 0x001000,
            String          = 0x002000,
            QtClass         = 0x004000,
            Enum            = 0x008000,
            Flags           = 0x010000,
            Array           = 0x020000,
            QtSubClass      = 0x040000,

            TypeMask = Integer + Long + Boolean + Float + Double + Short + Byte + Char
        };

        enum VariableContext {
            ReturnType,
            ArgumentType
        };

        // A little hack to ensure safe conversions for QtDynamicMetaObject.
        //
        // When it uses the conversion methods, Java-subclasses of Qt classes
        // (e.g. QWidget) should not become QWidget* in the signatures of the
        // invokable methods in the metaobject, since a generic QWidget* cannot
        // be converted to the correct Java object of course. In the case of
        // QVariants, however, a subclass of e.g. QGraphicsScene should become
        // QGraphicsScene* in the variant, since Qt relies on this. To avoid
        // rewriting a lot of stuff, the Mode enum is used to switch between the
        // two behaviors in the locations where it makes sense.
        enum ConversionMode {
            QVariantMode,
            DynamicMetaObjectMode
        };

        QtJambiTypeManager(JNIEnv *env, bool convertEnums = false, ConversionMode conversionMode = QVariantMode);
        virtual ~QtJambiTypeManager();

        // Some convenience functions
        Type typeIdOfExternal(const QString &className, const QString &package) const;
        Type typeIdOfInternal(const QString &internalType) const;

        inline static void *jlongToPtr(jlong id);
        inline static jlong ptrToJlong(void *ptr);
        static QString demangle(const QString &mangledName);
        static QString mangle(const QString &demangledName);
        static QString toJNISignature(const QString &signature, QString *name);
        static jvalue convertToPrimitive(JNIEnv *env, jobject javaRef, Type typeId);
        static jvalue convertToComplex(JNIEnv *env, jvalue val, Type typeId,
                                       bool *success = 0);
        static jvalue callMethod(JNIEnv *env, jobject javaRef, jmethodID methodId,
                                 Type typeId, jvalue *params = 0);
        inline static QString className(const QString &qualifiedName);
        inline static QString package(const QString &qualifiedName);
        static QString closestQtSuperclass(JNIEnv *env, const QString &className,
                                           const QString &package);
        static bool isQObjectSubclass(JNIEnv *env, const QString &className, const QString &package);
        static bool isQtClass(JNIEnv *env, const QString &className, const QString &package);
        static bool isQtSubClass(JNIEnv *env, const QString &className, const QString &package);
        static QString complexTypeOf(Type type);
        static QString primitiveTypeOf(Type type);
        static Type valueTypePattern(const QString &javaName);

        // Reimplementations
        QString getExternalTypeName(const QString &internalTypeName, VariableContext ctx) const;
        QString getInternalTypeName(const QString &externalTypeName, VariableContext ctx) const;
        //QString resolveQFlags(const QString &className, const QString &package) const;
        bool convertInternalToExternal(const void *in, void **out, const QString &internalTypeName,
                                       const QString &externalTypeName, VariableContext ctx,
                                       bool allowValuePointers = false);
        bool convertExternalToInternal(const void *in, void **out, const QString &externalTypeName,
                                       const QString &internalTypeName, VariableContext ctx);

        QVector<QString> parseSignature(const QString &signature, QString *name = 0) const;

        void *constructExternal(const QString &externalTypeName, VariableContext ctx,
                                const void *copy = 0);
        void destroyExternal(void *value, VariableContext ctx);

        bool isEnumType(const QString &className, const QString &package) const;
        bool isEnumType(jclass clazz) const;
        bool isFlagsType(const QString &className, const QString &package) const;
        bool isFlagsType(jclass clazz) const;
        int intForQtEnumerator(jobject enum_value) const;
        jobject enumForInt(int value, const QString &className, const QString &package) const;
        jobject flagsForInt(int value, const QString &className, const QString &package) const;

        bool canConvertInternalToExternal(const QString &internalTypeName,
                                          const QString &externalTypeName, VariableContext ctx) const;
        bool canConvertExternalToInternal(const QString &externalTypeName,
                                          const QString &internalTypeName, VariableContext ctx) const;
        QString internalToExternalSignature(const QString &internalSignature) const;
        QString externalToInternalSignature(const QString &externalSignature) const;
        void destroyConstructedInternal(const QVector<void *> &in);
        void destroyConstructedExternal(const QVector<void *> &in);
        bool decodeArgumentList(const QVector<void *> &in,
                                QVector<void *> *out,
                                const QVector<QString> &internalTypeNames,
                                const QVector<QString> &externalTypeNames);
        bool encodeArgumentList(const QVector<void *> &in,  QVector<void *> *out,
                                const QVector<QString> &typeList);
        void destroyInternal(void *value, VariableContext ctx);
        void *constructInternal(const QString &internalTypeName, VariableContext ctx,
#if QT_VERSION < 0x050000
                                const void *copy = 0, int metaType = QMetaType::Void);
#else
                                const void *copy = 0, int metaType = QMetaType::UnknownType);
#endif
        int metaTypeOfInternal(const QString &internalTypeName, VariableContext) const;
        QVector<void *> initExternalToInternal(const QVector<void *> &externalVariables,
                                               const QVector<QString> &externalTypeNames);
        QVector<void *> initInternalToExternal(const QVector<void *> &internalVariables,
                                               const QVector<QString> &internalTypeNames,
                                               const QVector<QString> &externalTypeNames);


        bool convertEnums() const { return mConvertEnums; }
        void setConvertEnums(bool on) { mConvertEnums = on; }

    private:
        static QString processInternalTypeName(const QString &typeName, int *indirections = 0);
        bool conditionsMetForQtClass(uint type) const;

    private:
        QHash<void *, QString> mOwnedVariables_internal;
        QHash<jvalue *, bool> mOwnedVariables_external;
//        QSet<void *> mCreatedVariables_internal;

        JNIEnv *mEnvironment;
    uint mConvertEnums : 1;

        ConversionMode mConversionMode;
};

// *********** Implementations ***********
#if (defined(__LP64__))
// LP64 => (sizeof(void*) == sizeof(long)) even on 64bit architectures
inline void *QtJambiTypeManager::jlongToPtr(jlong id) { return reinterpret_cast<void *>(static_cast<long>(id)); }
inline jlong QtJambiTypeManager::ptrToJlong(void *ptr) { return static_cast<jlong>(reinterpret_cast<long>(ptr)); }
#elif (defined(__LLP64__) || defined(Q_OS_WIN64))
// LLP64 => (sizeof(void*) != sizeof(long)) ala Windows 64bit
inline void *QtJambiTypeManager::jlongToPtr(jlong id) { return reinterpret_cast<void *>(static_cast<long long>(id)); }
inline jlong QtJambiTypeManager::ptrToJlong(void *ptr) { return static_cast<jlong>(reinterpret_cast<long long>(ptr)); }
#else
inline void *QtJambiTypeManager::jlongToPtr(jlong id) { return reinterpret_cast<void *>(static_cast<long>(id)); }
inline jlong QtJambiTypeManager::ptrToJlong(void *ptr) { return static_cast<jlong>(reinterpret_cast<long>(ptr)); }
#endif

inline QString QtJambiTypeManager::className(const QString &qualifiedName) {
    int idx = qualifiedName.lastIndexOf(QLatin1Char('/'));
    if (idx >= 0)
        return qualifiedName.mid(idx + 1);
    else
        return qualifiedName;
}

inline QString QtJambiTypeManager::package(const QString &qualifiedName) {
    int idx = qualifiedName.lastIndexOf(QLatin1Char('/'));
    if (idx >= 0)
        return qualifiedName.left(idx + 1);
    else
        return QString();
}

#endif
