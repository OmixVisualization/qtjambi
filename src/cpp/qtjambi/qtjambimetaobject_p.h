/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTDYNAMICMETAOBJECT_P_H
#define QTDYNAMICMETAOBJECT_P_H

#include "qtjambi_core.h"
#include "qtjambitypemanager_p.h"
#include "qtjambi_registry.h"
#include <QtCore/QMetaObject>
#include <QtCore/QSharedPointer>

typedef void (*StaticMetaCallFunction)(QObject *, QMetaObject::Call, int, void **);

class QtJambiMetaObjectPrivate;

class ParameterTypeInfo{
public:
    ParameterTypeInfo();
    ParameterTypeInfo(
            int metaTypeId,
            jclass _javaClass,
            const InternalToExternalConverter& _internalToExternalConverter,
            const ExternalToInternalConverter& _externalToInternalConverter
            );
    ParameterTypeInfo(const ParameterTypeInfo& other);
    ParameterTypeInfo& operator=(const ParameterTypeInfo& other);

    bool convertInternalToExternal(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* out, bool forceBoxedType) const;
    bool convertExternalToInternal(JNIEnv* env, QtJambiScope* scope, const jvalue& in,void* & out, jValueType valueType) const;
    jclass javaClass() const;
    int metaType() const;
    static const InternalToExternalConverter& default_internalToExternalConverter();
    static const ExternalToInternalConverter& default_externalToInternalConverter();
    static ParameterTypeInfo voidTypeInfo(JNIEnv* env);
private:
    int m_qTypeId;
    jclass m_javaClass;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
    friend class QtJambiMetaObject;
};

class QtJambiMetaObject: public QMetaObject
{
public:
    QtJambiMetaObject(JNIEnv *jni_env, jclass java_class, const QMetaObject *original_meta_object, JavaException& exceptionHandler);

    int invokeSignalOrSlot(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int readProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int writeProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int resetProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int notifyProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int queryPropertyDesignable(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int queryPropertyScriptable(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
#else
    int bindableProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#if QT_VERSION >= 0x050000
    int invokeConstructor(JNIEnv *env, int _id, void **_a) const;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int queryPropertyUser(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int queryPropertyStored(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int queryPropertyEditable(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#endif
    int originalSignalOrSlotSignature(JNIEnv *env, int _id, QString *signature) const;

    jclass typeOfProperty(int index) const;
    InternalToExternalConverter internalToExternalConverterOfProperty(int index) const;
    ExternalToInternalConverter externalToInternalConverterOfProperty(int index) const;
    jclass javaClass() const;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    jfieldID getQPropertyField(int index) const;
    void registerQPropertyField(int index, jfieldID field);
#endif
    static jweak javaInstance(const QtJambiMetaObject* metaObject);
    static void setJavaInstance(const QtJambiMetaObject* metaObject, jweak weak);
    static jclass javaClass(JNIEnv * env, const QMetaObject* metaObject, bool exactOrNull = false);
    static bool isInstance(const QMetaObject* metaObject);
    static const QtJambiMetaObject* cast(const QMetaObject* metaObject);
    static int methodFromJMethod(const QMetaObject* metaObject, jmethodID methodId);
    static const QList<ParameterTypeInfo>& methodParameterInfo(JNIEnv * env, const QMetaMethod& method);
    static jobject toReflected(JNIEnv * env, const QMetaMethod& method);
private:
    ~QtJambiMetaObject();
    const QSharedPointer<const QtJambiMetaObject>& thisPointer() const;
    QSharedPointer<const QtJambiMetaObject> dispose(JNIEnv * env) const;
    QtJambiMetaObjectPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtJambiMetaObject)
    Q_DISABLE_COPY_MOVE(QtJambiMetaObject)
    friend void clear_metaobjects_at_shutdown(JNIEnv * env);
    friend const QMetaObject *qtjambi_metaobject_for_class(JNIEnv *env, jclass object_class, const std::function<const QMetaObject *()>& original_meta_object_provider);
    friend class DynamicMetaObjectFunctionTable;
    friend class DynamicMetaObjectInterfaceFunctionTable;
    friend QtSharedPointer::CustomDeleter<QtJambiMetaObject,QtSharedPointer::NormalDeleter>;
};

#endif // QTDYNAMICMETAOBJECT_P_H
