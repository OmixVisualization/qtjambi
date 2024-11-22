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

#ifndef QTDYNAMICMETAOBJECT_P_H
#define QTDYNAMICMETAOBJECT_P_H

#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/private/qobject_p.h>
#include "qtjambiapi.h"
#include "typeutils.h"
#include "jobjectwrapper.h"

typedef void (*StaticMetaCallFunction)(QObject *, QMetaObject::Call, int, void **);

class QtJambiMetaObjectPrivate;

class ParameterTypeInfo{
public:
    ParameterTypeInfo();
    ParameterTypeInfo(
            int metaTypeId,
            jclass _javaClass
        );
    ParameterTypeInfo(
        int metaTypeId,
        const QString& typeName,
        jclass _javaClass
        );
    ParameterTypeInfo(
            int metaTypeId,
            jclass _javaClass,
            QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
            QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
        );
    ParameterTypeInfo(
            int metaTypeId,
            const QString& typeName,
            jclass _javaClass,
            QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
            QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
        );
    ParameterTypeInfo(const ParameterTypeInfo& other);
    ParameterTypeInfo& operator=(const ParameterTypeInfo& other);
    ParameterTypeInfo(ParameterTypeInfo&& other);
    ParameterTypeInfo& operator=(ParameterTypeInfo&& other);

    bool convertInternalToExternal(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType) const;
    bool convertExternalToInternal(JNIEnv* env, QtJambiScope* scope, jvalue in,void* & out, jValueType valueType) const;
    jclass javaClass() const;
    int metaType() const;
    static QtJambiUtils::InternalToExternalConverter default_internalToExternalConverter();
    static QtJambiUtils::ExternalToInternalConverter default_externalToInternalConverter();
    static ParameterTypeInfo voidTypeInfo(JNIEnv* env);
private:
    void resolveI2E(JNIEnv* env);
    void resolveE2I(JNIEnv* env);
    int m_qTypeId;
    QString m_typeName;
    jclass m_javaClass;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    uint m_resolvedI2E : 1;
    uint m_resolvedE2I : 1;
    friend class QtJambiMetaObject;
};

class QtJambiMetaObject final : public QMetaObject
{
public:
    int invokeSignalOrSlot(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int readProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int writeProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int resetProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int notifyProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int queryPropertyDesignable(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int queryPropertyScriptable(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
#else
    int bindableProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#if QT_VERSION >= 0x050000
    int invokeConstructor(JNIEnv *env, int _id, void **_a) const;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int queryPropertyUser(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int queryPropertyStored(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int queryPropertyEditable(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#endif
    int originalSignalOrSlotSignature(JNIEnv *env, int _id, QString *signature) const;
    int revision() const;

    jclass typeOfProperty(int index) const;
    QtJambiUtils::InternalToExternalConverter internalToExternalConverterOfProperty(int index) const;
    QtJambiUtils::ExternalToInternalConverter externalToInternalConverterOfProperty(int index) const;
    jclass javaClass() const;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    jfieldID getQPropertyField(int index) const;
    void registerQPropertyField(int index, jfieldID field);
#endif
    bool hasSignals() const;
    const QSharedPointer<const QtJambiMetaObject>& thisPointer() const;
    static jclass javaClass(JNIEnv * env, const QMetaObject* metaObject, bool exactOrNull = false);
    static bool isInstance(const QMetaObject* metaObject);
    static const QtJambiMetaObject* cast(const QMetaObject* metaObject);
    static int methodFromJMethod(const QMetaObject* metaObject, jmethodID methodId);
    static const QMetaObject *findFirstStaticMetaObject(const QMetaObject *meta_object);
    struct SignalInfo{
        const QMetaObject* metaObject;
        int methodIndex;
        jobject signalTypes;
        jclass signalClass;
        SignalInfo()
            : metaObject(nullptr), methodIndex(-1), signalTypes(nullptr), signalClass(nullptr) {}
        SignalInfo(const QMetaObject* _metaObject, int _methodIndex, jobject _signalTypes, jclass _signalClass)
            : metaObject(_metaObject), methodIndex(_methodIndex), signalTypes(_signalTypes), signalClass(_signalClass) {}
    };

    static SignalInfo signalInfo(JNIEnv *env, const QMetaObject* metaObject, jfieldID fieldId, jmethodID emitMethodID);
    static QVector<SignalInfo> signalInfos(JNIEnv *env, const QMetaObject* metaObject, jfieldID fieldId);
    static const QList<ParameterTypeInfo>& methodParameterInfo(JNIEnv * env, const QMetaMethod& method);
    static jobject toReflected(JNIEnv * env, const QMetaMethod& method);
    static void resolveSignals(JNIEnv *env, jobject java_object, const QMetaObject* metaObject, JavaException& ocurredException);
    static JObjectWrapper resolveExtraSignal(JNIEnv *env, jobject java_object, const QMetaMethod& method);
    static jobject convertToJavaObject(JNIEnv *env, const QMetaObject *metaObject);
    static const QMetaObject* findMetaObject(const char* name);
private:
    QtJambiMetaObject(JNIEnv *jni_env, jclass java_class);
    static jobject getSignalTypes(JNIEnv *env, jobject signal, const QMetaMethod& metaMethod);
    void initialize(JNIEnv *env, const QMetaObject *original_meta_object, bool hasCustomMetaObject);
    void objectDestroyed(QObject *) /*override*/ {}
    ~QtJambiMetaObject() /*override*/;
    int metaCall(QObject *, QMetaObject::Call c, int _id, void **a) /*override*/;
    int metaCall(QMetaObject::Call, int _id, void **) /*override*/;
    QSharedPointer<const QtJambiMetaObject> dispose(JNIEnv * env) const;
    QtJambiMetaObjectPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtJambiMetaObject)
    Q_DISABLE_COPY_MOVE(QtJambiMetaObject)
    friend QtJambiMetaObjectPrivate;
    friend QtSharedPointer::CustomDeleter<QtJambiMetaObject,QtSharedPointer::NormalDeleter>;
};

#endif // QTDYNAMICMETAOBJECT_P_H
