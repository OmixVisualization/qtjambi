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
class SuperTypeInfos;

struct JInvokableInfo{
    jclass declaringClass = nullptr;
    QVector<ParameterTypeInfo> parameterTypeInfos;
protected:
    mutable QString m_description;
};

struct JConstructorInfo : JInvokableInfo{
    jmethodID constructorId = nullptr;
    QtJambiAPI::ConstructorFn constructorFunction = nullptr;
    QList<jclass> constructorArgumentTypes;
    inline operator bool()const{return constructorId!=nullptr;}
    inline bool operator!()const{return constructorId==nullptr;}
    QString toString(JNIEnv* env) const;
};

class SuperInitializer;
class QtJambiMetaObject;

class InPlaceInitializer{
    const QtJambiMetaObject* m_metaObject;
    void* m_placement;
    size_t m_size = 0;
    size_t m_align = 0;
    QVector<ParameterTypeInfo> m_parameterTypeInfos;
    QtJambiAPI::ConstructorFn m_constructorFunction;
    QList<jclass> m_constructorArgumentFunctionTypes;
    const bool m_matches;
    QVector<jvalue> m_arguments;
    bool m_is_qml_call;
    QSharedPointer<bool> m_isInitialized;
    InPlaceInitializer* m_parentInitializer = nullptr;
    QList<SuperInitializer*> m_superInitializers;
public:
    InPlaceInitializer(const QtJambiMetaObject* metaObject, void* placement, size_t _size, size_t _align,
                       const JConstructorInfo& constructorInfo,
                       QVector<jvalue>&& arguments,
                       bool is_qml_call = false);
    InPlaceInitializer(const QtJambiMetaObject* metaObject, void* placement, size_t _size, size_t _align,
                       QtJambiAPI::ConstructorFn constructorFunction,
                       bool is_qml_call = false);
protected:
    InPlaceInitializer(InPlaceInitializer* parentInitializer,
                       const QtJambiMetaObject* metaObject, void* placement, size_t _size, size_t _align,
                       QVector<ParameterTypeInfo>&& parameterTypeInfos,
                       QtJambiAPI::ConstructorFn constructorFunction,
                       const QList<jclass>& constructorArgumentFunctionTypes,
                       QVector<jvalue> arguments,
                       bool is_qml_call,
                       bool matches);
    friend class SuperInitializer;
public:
    int parameterCount();
    jobject argumentAt(JNIEnv *env, int index);
    jclass parameterTypeAt(JNIEnv *env, int index);
    SuperInitializer* asArguments(JNIEnv *env, std::initializer_list<int> indexes, bool requireJava = true);
    void reset(JNIEnv *env);
    InPlaceInitializer* topLevel();
    friend class QtJambiShellImpl;
};

class SuperInitializer : public InPlaceInitializer{
    JObjectWrapper m_javaInPlaceObject;
public:
    SuperInitializer(JNIEnv *env,
                     InPlaceInitializer* parentInitializer,
                     const QtJambiMetaObject* metaObject, void* placement, size_t _size, size_t _align,
                     QVector<ParameterTypeInfo>&& parameterTypeInfos,
                     QtJambiAPI::ConstructorFn constructorFunction,
                     const QList<jclass>& constructorArgumentFunctionTypes,
                     QVector<jvalue> arguments,
                     bool is_qml_call,
                     bool matches, bool requireJava);
    jobject inPlaceObject(JNIEnv *env) const;
};

class QtJambiMetaObject final : public QMetaObject
{
public:
    int invokeSignalOrSlot(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int readProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int writeProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int resetProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int notifyProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;
    int bindableProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct = false) const;

    int invokeConstructor(JNIEnv *env, int _id, void **_a) const;
    int invokeInPlaceConstructor(JNIEnv *env, int _id, void **_a) const;
    int originalSignalOrSlotSignature(JNIEnv *env, int _id, QString *signature) const;
    int revision() const;

    jclass typeOfProperty(int index) const;
    QtJambiUtils::InternalToExternalConverter internalToExternalConverterOfProperty(int index) const;
    QtJambiUtils::ExternalToInternalConverter externalToInternalConverterOfProperty(int index) const;
    jclass javaClass() const;
    const SuperTypeInfos& superTypeInfos() const;
    jfieldID getQPropertyField(int index) const;
    void registerQPropertyField(int index, jfieldID field);
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
        JObjectWrapper signalTypes;
        jclass signalClass;
        SignalInfo()
            : metaObject(nullptr), methodIndex(-1), signalTypes(), signalClass(nullptr) {}
        SignalInfo(const QMetaObject* _metaObject, int _methodIndex, const JObjectWrapper& _signalTypes, jclass _signalClass)
            : metaObject(_metaObject), methodIndex(_methodIndex), signalTypes(_signalTypes), signalClass(_signalClass) {}
    };

    static SignalInfo signalInfo(JNIEnv *env, const QMetaObject* metaObject, jfieldID fieldId, jmethodID emitMethodID);
    static QVector<SignalInfo> signalInfos(JNIEnv *env, const QMetaObject* metaObject, jfieldID fieldId);
    static QVector<ParameterTypeInfo> methodParameterInfo(JNIEnv * env, const QMetaMethod& method);
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
    friend void clearQtJambiStorage(JNIEnv* env, bool regular);
};

#endif // QTDYNAMICMETAOBJECT_P_H
