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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include "functionpointer.h"
#include "qtjambimetaobject_p.h"
#include "qtjambiapi.h"
#include "coreapi.h"
#include "utils.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "typemanager_p.h"
#include "threadutils_p.h"
#include "qtjambilink_p.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include "qmlapi.h"
#else
#include <cstring>
#endif

#include <QtCore/QHash>
#include <QtCore/QVarLengthArray>
#include <QtCore/QMetaEnum>
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qmetaobject_p.h>

#include "qtjambi_cast.h"

typedef QHash<const QMetaObject *, jweak> JavaMetaObjectHash;
Q_GLOBAL_STATIC(JavaMetaObjectHash, javaMetaObjects);
Q_GLOBAL_STATIC(QRecursiveMutex, javaMetaObjectsLock);


typedef QHash<hash_type,const QMetaObject *> MetaObjectByNameHash;
Q_GLOBAL_STATIC(MetaObjectByNameHash, metaObjectsByName);

#define QTJAMBI_MAX_NUMBER_OF_CLASSES 1024
#define QTJAMBI_MAX_NUMBER_OF_CLASSES_DIVISOR 100
#define QTJAMBI_MAX_NUMBER_OF_CLASSES_MAX 1024

static const char* QtJambiMetaObjectID = "QtJambiMetaObject";

enum class JMethodType {
    v = 0,
    z = int(jValueType::z),
    b = int(jValueType::b),
    c = int(jValueType::c),
    s = int(jValueType::s),
    i = int(jValueType::i),
    j = int(jValueType::j),
    f = int(jValueType::f),
    d = int(jValueType::d),
    l = int(jValueType::l),
};

struct JMethodInfo{
    JMethodType methodType = JMethodType::v;
    jmethodID methodId = nullptr;
    bool isStatic = false;
    jclass declaringClass = nullptr;
    QList<ParameterTypeInfo> parameterTypeInfos;
};

typedef QPair<jfieldID, JMethodInfo > FieldPair;

struct JMemberInfo{
    jfieldID member = nullptr;
    jValueType type = jValueType::l;
    ParameterTypeInfo memberTypeInfo;
    bool canWrite = false;
    bool isStatic = false;
    jclass declaringClass = nullptr;
};

struct JSignalInfo{
    jfieldID signalField = nullptr;
    jobject signalTypes = nullptr;
    JMethodInfo emitMethodInfo;
    bool isClone = false;
};

struct QPropertyInfo{
    jfieldID propertyField = nullptr;
    JMethodInfo valueMethod;
    JMethodInfo setValueMethod;
};

typedef QHash<hash_type, jobject> SignalTypesHash;
typedef QHash<hash_type, const QList<ParameterTypeInfo>*> ParameterTypeHash;
Q_GLOBAL_STATIC(QReadWriteLock, gJMethodInfoInfosLock)
Q_GLOBAL_STATIC(ParameterTypeHash, gParameterTypeInfos)
typedef QHash<int, const QMetaObject *> MetaObjectHash;
Q_GLOBAL_STATIC(MetaObjectHash, gMetaObjects);
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gMetaObjectsLock, (QReadWriteLock::Recursive));
Q_GLOBAL_STATIC(SignalTypesHash, gSignalTypes);

InternalToExternalConverter ParameterTypeInfo::default_internalToExternalConverter()
{
    return InternalToExternalConverter([](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{ return false; });
}

ExternalToInternalConverter ParameterTypeInfo::default_externalToInternalConverter()
{
    return ExternalToInternalConverter([](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) ->bool { return false; });
}

ParameterTypeInfo::ParameterTypeInfo()
    : m_qTypeId(QMetaType::UnknownType),
      m_typeName(),
      m_javaClass(nullptr),
      m_internalToExternalConverter(ParameterTypeInfo::default_internalToExternalConverter()),
      m_externalToInternalConverter(ParameterTypeInfo::default_externalToInternalConverter()),
      m_resolvedI2E(true),
      m_resolvedE2I(true)
{}

ParameterTypeInfo ParameterTypeInfo::voidTypeInfo(JNIEnv* env){
    ParameterTypeInfo info = ParameterTypeInfo(
                QMetaType::Void,
                getGlobalClassRef(env, Java::Runtime::Void::primitiveType(env))
          );
    info.m_externalToInternalConverter = ParameterTypeInfo::default_externalToInternalConverter();
    info.m_internalToExternalConverter = ParameterTypeInfo::default_internalToExternalConverter();
    info.m_resolvedI2E = true;
    info.m_resolvedE2I = true;
    return info;
}

ParameterTypeInfo::ParameterTypeInfo(
    int qTypeId,
    jclass _javaClass
    )
    :
    m_qTypeId(qTypeId),
    m_typeName(QLatin1String(QMetaType(qTypeId).name())),
    m_javaClass(_javaClass),
    m_internalToExternalConverter(),
    m_externalToInternalConverter(),
    m_resolvedI2E(false),
    m_resolvedE2I(false)
{
}

ParameterTypeInfo::ParameterTypeInfo(
        int qTypeId,
        const QString& typeName,
        jclass _javaClass
        )
    :
      m_qTypeId(qTypeId),
      m_typeName(typeName),
      m_javaClass(_javaClass),
      m_internalToExternalConverter(),
      m_externalToInternalConverter(),
      m_resolvedI2E(false),
      m_resolvedE2I(false)
{
}

ParameterTypeInfo::ParameterTypeInfo(
        int qTypeId,
        jclass _javaClass,
        InternalToExternalConverter&& internalToExternalConverter,
        ExternalToInternalConverter&& externalToInternalConverter
        )
    :
      m_qTypeId(qTypeId),
      m_typeName(QLatin1String(QMetaType(qTypeId).name())),
      m_javaClass(_javaClass),
      m_internalToExternalConverter(std::move(internalToExternalConverter)),
      m_externalToInternalConverter(std::move(externalToInternalConverter)),
      m_resolvedI2E(m_internalToExternalConverter),
      m_resolvedE2I(m_externalToInternalConverter)
{
}

ParameterTypeInfo::ParameterTypeInfo(
        int qTypeId,
        const QString& typeName,
        jclass _javaClass,
        InternalToExternalConverter&& internalToExternalConverter,
        ExternalToInternalConverter&& externalToInternalConverter
        )
    :
      m_qTypeId(qTypeId),
      m_typeName(typeName),
      m_javaClass(_javaClass),
      m_internalToExternalConverter(std::move(internalToExternalConverter)),
      m_externalToInternalConverter(std::move(externalToInternalConverter)),
      m_resolvedI2E(m_internalToExternalConverter),
      m_resolvedE2I(m_externalToInternalConverter)
{
}

ParameterTypeInfo::ParameterTypeInfo(const ParameterTypeInfo& other)
    :
      m_qTypeId(other.m_qTypeId),
      m_typeName(other.m_typeName),
      m_javaClass(other.m_javaClass),
      m_internalToExternalConverter(other.m_internalToExternalConverter),
      m_externalToInternalConverter(other.m_externalToInternalConverter),
      m_resolvedI2E(other.m_resolvedI2E),
      m_resolvedE2I(other.m_resolvedE2I)
{}

ParameterTypeInfo& ParameterTypeInfo::operator=(const ParameterTypeInfo& other){
    m_qTypeId = other.m_qTypeId;
    m_typeName = other.m_typeName;
    m_javaClass = other.m_javaClass;
    m_internalToExternalConverter = other.m_internalToExternalConverter;
    m_externalToInternalConverter = other.m_externalToInternalConverter;
    m_resolvedE2I = other.m_resolvedE2I;
    m_resolvedI2E = other.m_resolvedI2E;
    return *this;
}

ParameterTypeInfo::ParameterTypeInfo(ParameterTypeInfo&& other)
    :
      m_qTypeId(other.m_qTypeId),
      m_typeName(std::move(other.m_typeName)),
      m_javaClass(other.m_javaClass),
      m_internalToExternalConverter(std::move(other.m_internalToExternalConverter)),
      m_externalToInternalConverter(std::move(other.m_externalToInternalConverter)),
      m_resolvedI2E(other.m_resolvedI2E),
      m_resolvedE2I(other.m_resolvedE2I)
{}

ParameterTypeInfo& ParameterTypeInfo::operator=(ParameterTypeInfo&& other){
    m_qTypeId = other.m_qTypeId;
    m_typeName = std::move(other.m_typeName);
    m_javaClass = other.m_javaClass;
    m_internalToExternalConverter = std::move(other.m_internalToExternalConverter);
    m_externalToInternalConverter = std::move(other.m_externalToInternalConverter);
    m_resolvedE2I = other.m_resolvedE2I;
    m_resolvedI2E = other.m_resolvedI2E;
    return *this;
}

void ParameterTypeInfo::resolveI2E(JNIEnv* env){
    m_internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                            env,
                                                                                            m_typeName,
                                                                                            QMetaType(m_qTypeId),
                                                                                            m_javaClass,
                                                                                            true
                                                                                        );
    m_resolvedI2E = true;
}

void ParameterTypeInfo::resolveE2I(JNIEnv* env){
    m_externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                            env,
                                                                                            m_javaClass,
                                                                                            m_typeName,
                                                                                            QMetaType(m_qTypeId)
                                                                                        );
    m_resolvedE2I = true;
}

bool ParameterTypeInfo::convertInternalToExternal(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType) const {
    if(!m_resolvedI2E)
        const_cast<ParameterTypeInfo*>(this)->resolveI2E(env);
    return m_internalToExternalConverter && m_internalToExternalConverter(env, scope, in, out, forceBoxedType);
}

bool ParameterTypeInfo::convertExternalToInternal(JNIEnv* env, QtJambiScope* scope, jvalue in,void* & out, jValueType valueType) const {
    if(!m_resolvedE2I)
        const_cast<ParameterTypeInfo*>(this)->resolveE2I(env);
    return m_externalToInternalConverter && m_externalToInternalConverter(env, scope, in, out, valueType);
}

jclass ParameterTypeInfo::javaClass() const{
    return m_javaClass;
}

int ParameterTypeInfo::metaType() const{
    return m_qTypeId;
}

void static_metacall_QObject(const QtJambiMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv)
{
    switch(cl){
    case QMetaObject::InvokeMetaMethod:
        if(o){
            o->metaObject()->metacall(o, cl, idx+o->metaObject()->methodOffset(), argv);
        }else{
            q->static_metacall(cl, idx+q->methodOffset(), argv);
        }
        return;
    case QMetaObject::CreateInstance:
        if(JniEnvironment env{200}){
            q->invokeConstructor(env, idx, argv);
        }
        return;
    default: break;
    }
}

void static_metacall_QtSubType(const QtJambiMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv)
{
    if(JniEnvironment env{200}){
        jobject object = o ? QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, o, q->javaClass()) : nullptr;
        if(!env->IsSameObject(object, nullptr)){
            switch(cl){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case QMetaObject::QueryPropertyUser:
                q->queryPropertyUser(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyDesignable:
                q->queryPropertyDesignable(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyScriptable:
                q->queryPropertyScriptable(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyStored:
                q->queryPropertyStored(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyEditable:
                q->queryPropertyEditable(env, object, idx, argv, true);
                break;
#else
            case QMetaObject::BindableProperty:
                q->bindableProperty(env, object, idx, argv, true);
                break;
#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case QMetaObject::ResetProperty:
                q->resetProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::ReadProperty:
                q->readProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::WriteProperty:
                q->writeProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::InvokeMetaMethod:
                q->invokeSignalOrSlot(env, object, idx, argv, true);
                break;
#if 0
            case QMetaObject::IndexOfMethod:
                printf("QMetaObject::IndexOfMethod\n");
                break;
            case QMetaObject::RegisterPropertyMetaType:
                printf("QMetaObject::RegisterPropertyMetaType\n");
                break;
            case QMetaObject::RegisterMethodArgumentMetaType:
                printf("QMetaObject::RegisterMethodArgumentMetaType\n");
                break;
#endif
            default:
                break;
            }
        }else if(o){
            const QList<QSharedPointer<QtJambiLink>> linksForPointer = QtJambiLink::findLinksForPointer(o);
            for(const QSharedPointer<QtJambiLink>& link : linksForPointer){
                if(link->isDeleteLater())
                    return;
            }
            qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::metaCall: metaCall on incomplete object ot type %s", q->className());
        }else{
            switch(cl){
            case QMetaObject::InvokeMetaMethod:
                q->static_metacall(cl, idx+q->methodOffset(), argv);
                break;
            case QMetaObject::CreateInstance:
                q->invokeConstructor(env, idx, argv);
                break;
            default: break;
            }
        }
    }
}

void static_metacall_any_type(const QtJambiMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv)
{
    if(JniEnvironment env{200}){
        switch(cl){
        case QMetaObject::CreateInstance:
            q->invokeConstructor(env, idx, argv);
            return;
        case QMetaObject::InvokeMetaMethod:
            if(!o){
                q->static_metacall(cl, idx + q->methodOffset(), argv);
                return;
            }
            break;
        default:
            break;
        }
        if(jobject object = reinterpret_cast<JObjectWrapper*>(o)->object()){
            switch(cl){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case QMetaObject::QueryPropertyUser:
                q->queryPropertyUser(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyDesignable:
                q->queryPropertyDesignable(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyScriptable:
                q->queryPropertyScriptable(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyStored:
                q->queryPropertyStored(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyEditable:
                q->queryPropertyEditable(env, object, idx, argv, true);
                break;
#else
            case QMetaObject::BindableProperty:
                q->bindableProperty(env, object, idx, argv, true);
                break;
#endif
            case QMetaObject::ResetProperty:
                q->resetProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::ReadProperty:
                q->readProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::WriteProperty:
                q->writeProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::InvokeMetaMethod:
                q->invokeSignalOrSlot(env, object, idx, argv, true);
                break;
#if 0
            case QMetaObject::IndexOfMethod:
                printf("QMetaObject::IndexOfMethod\n");
                break;
            case QMetaObject::RegisterPropertyMetaType:
                printf("QMetaObject::RegisterPropertyMetaType\n");
                break;
            case QMetaObject::RegisterMethodArgumentMetaType:
                printf("QMetaObject::RegisterMethodArgumentMetaType\n");
                break;
#endif
            default:
                break;
            }
        }
    }
}

typedef void (*StaticMetacallFunction)(const QtJambiMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv);

hash_type computeHash(const QtJambiMetaObject* q, StaticMetacallFunction fct){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return qHashMulti(0, q, quintptr(fct));
#else
    QList<quintptr> range{quintptr(q),quintptr(fct)};
    return qHashRange(range.begin(), range.end());
#endif
}

StaticMetaCallFunction create_static_metacall(const QtJambiMetaObject* q, StaticMetacallFunction fct){
    return qtjambi_function_pointer<16,void(QObject *, QMetaObject::Call, int, void **)>(
                [q,fct](QObject * o, QMetaObject::Call cl, int idx, void ** argv){
                    fct(q, o, cl, idx, argv);
                },
                computeHash(q, fct));
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
struct QmlExtensionData{
    jmethodID wrapper;
    QHash<jmethodID,jmethodID> methodMappings;
    QHash<jfieldID,jfieldID> fieldMappings;
};
#endif

class QtJambiMetaObjectPrivate
{
public:
    QtJambiMetaObjectPrivate(QtJambiMetaObject *q, JNIEnv *env, jclass java_class);
    ~QtJambiMetaObjectPrivate();

    void initialize(JNIEnv *jni_env, const QMetaObject *original_meta_object, bool hasCustomMetaObject);
    void invokeMethod(JNIEnv *env, jobject object, const JMethodInfo& methodInfo, void **_a, bool forceObjectType = false) const;
    void invokeConstructor(JNIEnv *env, const JMethodInfo& methodInfo, void **_a) const;

    int methodFromJMethod(jmethodID methodId) const;
    QtJambiMetaObject::SignalInfo signalInfo(jfieldID fieldId, jmethodID emitMethodID) const;
    QVector<QtJambiMetaObject::SignalInfo> signalInfos(jfieldID fieldId) const;
    const JMethodInfo* methodInfo(int index) const;
    const JMethodInfo* constructorInfo(int index) const;
    jclass javaClass() const;
    jweak javaInstance() const;
    void setJavaInstance(jweak weak);
    static QtJambiMetaObjectPrivate* get(QtJambiMetaObject* obj);
    static const QtJambiMetaObjectPrivate* get(const QtJambiMetaObject* obj);
    static const QMetaObject *getQMetaObjectForJavaClass(JNIEnv *env, jclass object_class, const std::function<const QMetaObject *(bool&, bool&)>& original_meta_object_provider);
    static const QMetaObject *getQMetaObjectForJavaClass(JNIEnv *env, jclass object_class, const std::type_info& typeId);
    static void clearAtShutdown(JNIEnv * env);
private:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    /**
     * this list stores all texts describing the class meta object.
     */
    QList<char*> m_stringdataList;
#else
    QByteArray m_stringData;
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QScopedArrayPointer<jint> m_intData;
    QtJambiMetaObject *q_ptr;
    mutable QSharedPointer<const QtJambiMetaObject> m_this_ptr;
    Q_DECLARE_PUBLIC(QtJambiMetaObject)

    int m_method_count;
    int m_signal_count;
    int m_constructor_count;
    int m_property_count;

    jclass const m_clazz;
    QVector<JMethodInfo> m_methods;
    QHash<jmethodID,int> m_methodIndexes;
    QVector<JSignalInfo> m_signals;
    QHash<jfieldID,QHash<jmethodID,int>> m_signalIndexes;
    QVector<JMethodInfo> m_constructors;

    QMap<int,JMethodInfo> m_property_readers;
    QMap<int,JMethodInfo> m_property_writers;
    QMap<int,JMethodInfo> m_property_resetters;
    QMap<int,int> m_property_notifies;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMap<int,JMethodInfo> m_property_designable_resolvers;
    QMap<int,JMethodInfo> m_property_scriptable_resolvers;
    QMap<int,JMethodInfo> m_property_editable_resolvers;
    QMap<int,JMethodInfo> m_property_stored_resolvers;
    QMap<int,JMethodInfo> m_property_user_resolvers;
#else
    QMap<int,JMethodInfo> m_property_bindables;
    QMap<int,QPropertyInfo> m_property_QProperty_fields;
#endif
    QMap<int,JMemberInfo> m_propertyMembers;

    mutable jweak m_javaInstance;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QHash<jclass,QmlExtensionData> m_qmlExtensions;
    friend bool QmlAPI::registerQmlExtension(JNIEnv *env, const QMetaObject *extended_meta_object, jclass resolvedClass);
#endif
};

QtJambiMetaObjectPrivate::QtJambiMetaObjectPrivate(QtJambiMetaObject *q, JNIEnv *env, jclass java_class)
    :
      q_ptr(q), m_this_ptr(q), m_method_count(-1), m_signal_count(0), m_constructor_count(0), m_property_count(0),
      m_clazz(getGlobalClassRef(env, java_class)),
      m_methods(), m_methodIndexes(), m_signals(), m_signalIndexes(), m_constructors(),
      m_property_readers(), m_property_writers(), m_property_resetters(), m_property_notifies(),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_property_designable_resolvers(),
      m_property_scriptable_resolvers(),
      m_property_editable_resolvers(),
      m_property_stored_resolvers(),
      m_property_user_resolvers(),
#else
      m_property_bindables(),
      m_property_QProperty_fields(),
#endif
      m_propertyMembers(),
      m_javaInstance(nullptr)
{
    Q_ASSERT(env);
    Q_ASSERT(java_class);
}

QtJambiMetaObjectPrivate::~QtJambiMetaObjectPrivate()
{
    if(m_javaInstance){
        if (DefaultJniEnvironment env{100}) {
            env->DeleteWeakGlobalRef(m_javaInstance);
            m_javaInstance = nullptr;
            for(JSignalInfo& sigInfo : m_signals){
                if(!env->IsSameObject(sigInfo.signalTypes, nullptr)){
                    env->DeleteGlobalRef(sigInfo.signalTypes);
                    sigInfo.signalTypes = nullptr;
                }
            }
        }
    }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    for(char* strg : qAsConst(m_stringdataList)){
        delete[] strg;
    }
    m_stringdataList.clear();
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
}

QtJambiMetaObjectPrivate* QtJambiMetaObjectPrivate::get(QtJambiMetaObject* obj){
    return obj->d_ptr;
}

const QtJambiMetaObjectPrivate* QtJambiMetaObjectPrivate::get(const QtJambiMetaObject* obj){
    return obj->d_ptr;
}

template<class MethodInfoContainer>
void analyze_methods(JNIEnv *env, jobject classLoader, int count, jobject methodList, jobject listOfMetaTypes, MethodInfoContainer& methodInfoContainer, QHash<jmethodID,int>* methodIndexes = nullptr){
    Q_ASSERT(count == QtJambiAPI::sizeOfJavaCollection(env, methodList));
    Q_UNUSED(classLoader)
    QTJAMBI_JNI_LOCAL_FRAME(env, 32 + count);
    for(int i=0; i<count; ++i){
        jobject methodObject = Java::Runtime::List::get(env, methodList, i);
        if(methodObject){
            JObjectArrayWrapper metaTypes(env, listOfMetaTypes ? jobjectArray(Java::Runtime::List::get(env, listOfMetaTypes, i)) : nullptr, false);
            JMethodInfo info;
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            info.methodId = env->FromReflectedMethod(methodObject);
            jclass returnClassType(nullptr);
            jobjectArray parameterClassTypes(nullptr);
            if(Java::Runtime::Method::isInstanceOf(env, methodObject)){
                returnClassType = Java::Runtime::Method::getReturnType(env, methodObject);
                parameterClassTypes = Java::Runtime::Executable::getParameterTypes(env, methodObject);
                int modif = Java::Runtime::Executable::getModifiers(env, methodObject);
                info.declaringClass = getGlobalClassRef(env, Java::Runtime::Method::getDeclaringClass(env, methodObject));
                info.isStatic = Java::Runtime::Modifier::isStatic(env, modif);
            }else if(Java::Runtime::Constructor::isInstanceOf(env, methodObject)){
                returnClassType = Java::Runtime::Void::primitiveType(env);
                parameterClassTypes = Java::Runtime::Executable::getParameterTypes(env, methodObject);
                info.declaringClass = getGlobalClassRef(env, Java::Runtime::Constructor::getDeclaringClass(env, methodObject));
                info.isStatic = false;
            }else{
                JavaException::raiseNullPointerException(env, "Method is null." QTJAMBI_STACKTRACEINFO );
                continue;
            }
            int length = env->GetArrayLength(parameterClassTypes)+1;
            info.parameterTypeInfos.reserve(length);
            for(int j=0; j<length; ++j){
                jclass javaClass = j==0 ? returnClassType : jclass(env->GetObjectArrayElement(parameterClassTypes, j-1));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                QString typeName;
                QMetaType qMetaType( [&]() -> int {
                    if(Java::Runtime::Void::isPrimitiveType(env, javaClass)){
                        typeName = QStringLiteral("void");
                        return QMetaType::Void;
                    }else if(listOfMetaTypes){
                        if(j<metaTypes.length()){
                            jobject mt = metaTypes.at(env, j);
                            Java::QtJambi::MetaObjectData$MetaTypeInfo::metaTypeId(env, mt);
                            jstring jtypeName = Java::QtJambi::MetaObjectData$MetaTypeInfo::typeName(env, mt);
                            typeName = qtjambi_cast<QString>(env, jtypeName);
                            return Java::QtJambi::MetaObjectData$MetaTypeInfo::metaTypeId(env, mt);
                        }
                        return QMetaType::UnknownType;
                    }else if(Java::Runtime::Boolean::isPrimitiveType(env, javaClass)){
                        typeName = QStringLiteral("bool");
                        return QMetaType::Bool; // in case of property resolver
                    }else{
                        return QMetaType::UnknownType; // in case of property bindable
                    }
                } () );
                if(j==0){
                    if(Java::Runtime::Void::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::v;
                    }else if(Java::Runtime::Integer::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::i;
                    }else if(Java::Runtime::Long::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::j;
                    }else if(Java::Runtime::Short::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::s;
                    }else if(Java::Runtime::Byte::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::b;
                    }else if(Java::Runtime::Boolean::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::z;
                    }else if(Java::Runtime::Character::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::c;
                    }else if(Java::Runtime::Float::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::f;
                    }else if(Java::Runtime::Double::isPrimitiveType(env, javaClass)){
                        info.methodType = JMethodType::d;
                    }else{
                        info.methodType = JMethodType::l;
                    }
                }
                InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::tryGetInternalToExternalConverter(
                                                                env,
                                                                typeName,
                                                                qMetaType,
                                                                javaClass,
                                                                true);
                ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::tryGetExternalToInternalConverter(
                                                                env,
                                                                javaClass,
                                                                typeName,
                                                                qMetaType);
                info.parameterTypeInfos << ParameterTypeInfo{
                                               qMetaType.id(),
                                               typeName,
                                               getGlobalClassRef(env, javaClass),
                                               std::move(internalToExternalConverter),
                                               std::move(externalToInternalConverter)};
            }
            methodInfoContainer[i] = info;
            if(methodIndexes){
                methodIndexes->insert(info.methodId, i);
            }
        }
    }
}

void QtJambiMetaObjectPrivate::initialize(JNIEnv *env, const QMetaObject *original_meta_object, bool hasCustomMetaObject)
{
    Q_Q(QtJambiMetaObject);
    QTJAMBI_JNI_LOCAL_FRAME(env, 1024);
    jobject classLoader = Java::Runtime::Class::getClassLoader(env, m_clazz);
    jobject meta_data_struct = Java::QtJambi::MetaObjectUtility::analyze(env, m_clazz);
    if (!meta_data_struct)
        return;
    jobject intData = Java::QtJambi::MetaObjectData::intData(env, meta_data_struct);
    Q_ASSERT(intData);

    jobject stringData = Java::QtJambi::MetaObjectData::stringData(env,meta_data_struct);
    Q_ASSERT(stringData);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    jsize number_of_strings = QtJambiAPI::sizeOfJavaCollection(env, stringData);
    QByteArrayData* __stringdata = reinterpret_cast<QByteArrayData*>(new char[size_t(number_of_strings)*sizeof(QByteArrayData)]);
    {
        static int string_size = int(std::strlen(QtJambiMetaObjectID)+1);
        __stringdata[0].ref.atomic._q_value = -1L;
        __stringdata[0].alloc = 0;
        __stringdata[0].capacityReserved = 0;
        __stringdata[0].size = string_size;
        __stringdata[0].offset = reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(QtJambiMetaObjectID)) - reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(&(__stringdata[0])));
    }
    {
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + number_of_strings);
        for(int i=1; i<number_of_strings; i++){
            jstring stringData_entry = jstring(Java::Runtime::List::get(env, stringData, i));
            jsize stringData_len = env->GetStringUTFLength(stringData_entry);
            char* strg = new char[size_t(stringData_len+1)];
            strg[stringData_len] = '\0';
            env->GetStringUTFRegion(stringData_entry, 0, stringData_len, strg);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            m_stringdataList.append(strg);
            __stringdata[i].ref.atomic._q_value = -1L;
            __stringdata[i].alloc = 0;
            __stringdata[i].capacityReserved = 0;
            __stringdata[i].size = stringData_len;
            __stringdata[i].offset = reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(strg)) - reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(&(__stringdata[i])));
        }
    }
    q->d.stringdata = __stringdata;
#else
    jsize number_of_strings = QtJambiAPI::sizeOfJavaCollection(env, stringData);
    {
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + number_of_strings);
        size_t totalCharCount = sizeof(const char*) + 1;
        for(int i=1; i<number_of_strings; i++){
            jstring stringData_entry = jstring(Java::Runtime::List::get(env, stringData, i));
            totalCharCount += size_t(env->GetStringUTFLength(stringData_entry)) + 1;
        }
        m_stringData.fill('\0', int(totalCharCount + size_t(number_of_strings)*2*sizeof(uint)));
        QPair<uint,uint>* offsetsAndSize = reinterpret_cast<QPair<uint,uint>*>(m_stringData.data());
        uint totalOffset = uint(size_t(number_of_strings)*2*sizeof(uint));
        {
            offsetsAndSize[0].first = totalOffset;
            offsetsAndSize[0].second = sizeof(const char*);
            char* __stringdata = m_stringData.data()+totalOffset;
            const void*& pointer = *reinterpret_cast<const void**>(__stringdata);
            pointer = QtJambiMetaObjectID;
            totalOffset += sizeof(const char*) + 1;
        }
        for(int i=1; i<number_of_strings; i++){
            jstring stringData_entry = jstring(Java::Runtime::List::get(env, stringData, i));
            jsize stringData_len = env->GetStringUTFLength(stringData_entry);
            offsetsAndSize[i].first = totalOffset;
            offsetsAndSize[i].second = uint(stringData_len);
            char* __stringdata = m_stringData.data()+totalOffset;
            env->GetStringUTFRegion(stringData_entry, 0, stringData_len, __stringdata);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            totalOffset += uint(stringData_len) + 1;
        }
    }
    q->d.stringdata = reinterpret_cast<uint*>(m_stringData.data());
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

    {
        jclass java_super_class = env->GetSuperclass(m_clazz);
        if(java_super_class)
            q->d.superdata.direct = CoreAPI::metaObjectForClass(env, java_super_class, original_meta_object, hasCustomMetaObject);
        else
            q->d.superdata.direct = nullptr;
    }

    jsize intData_len = Java::QtJambi::MetaObjectData$IntArray::size(env, intData);
    m_intData.reset(new jint[size_t(intData_len)]);
    env->GetIntArrayRegion(Java::QtJambi::MetaObjectData$IntArray::array(env, intData), 0, intData_len, m_intData.data());
    q->d.data = reinterpret_cast<uint*>(m_intData.get());

    q->d.extradata = nullptr;
    q->d.relatedMetaObjects = nullptr;

    {
        jobject methods = Java::QtJambi::MetaObjectData::methods(env,meta_data_struct);
        jobject methodMetaTypes = Java::QtJambi::MetaObjectData::methodMetaTypes(env,meta_data_struct);
        m_method_count = QtJambiAPI::sizeOfJavaCollection(env, methods);
        m_methods.resize(m_method_count);
        analyze_methods(env, classLoader, m_method_count, methods, methodMetaTypes, m_methods, &m_methodIndexes);
    }

    {
        jobject signalInfos = Java::QtJambi::MetaObjectData::signalInfos(env,meta_data_struct);
        m_signal_count = QtJambiAPI::sizeOfJavaCollection(env, signalInfos);
        m_signals.resize(m_signal_count);
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + m_signal_count);
        for(int i=0; i<m_signal_count; ++i){
            jobject signalInfo = Java::Runtime::List::get(env, signalInfos, i);
            if(signalInfo){
                JSignalInfo& signal = m_signals[i];
                signal.emitMethodInfo.methodType = JMethodType::v;
                signal.emitMethodInfo.methodId = jmethodID(Java::QtJambi::MetaObjectData$SignalInfo::methodId(env, signalInfo));
                Q_ASSERT(signal.emitMethodInfo.methodId);
                signal.signalField = env->FromReflectedField(Java::QtJambi::MetaObjectData$SignalInfo::field(env, signalInfo));
                signal.signalTypes = env->NewGlobalRef(Java::QtJambi::MetaObjectData$SignalInfo::signalTypes(env, signalInfo));
                if(m_signalIndexes.contains(signal.signalField))
                    signal.isClone = true;

                m_signalIndexes[signal.signalField][signal.emitMethodInfo.methodId] = i;

                JIntArrayPointer signalMetaTypes(env, Java::QtJambi::MetaObjectData$SignalInfo::signalMetaTypes(env, signalInfo));
                int length = QtJambiAPI::sizeOfJavaCollection(env, signal.signalTypes);
                signal.emitMethodInfo.parameterTypeInfos.reserve(length+1);
                signal.emitMethodInfo.parameterTypeInfos.append(ParameterTypeInfo::voidTypeInfo(env));
                jobject parameterClassTypes = QtJambiAPI::newJavaArrayList(env, length);
                for(int j=0; j<length; ++j){
                    jobject signalParameterType = Java::Runtime::List::get(env, signal.signalTypes, j);
                    QMetaType qMetaType(signalMetaTypes.pointer()[j]);
                    jclass javaClass = Java::QtJambi::SignalUtility$SignalParameterType::type(env, signalParameterType);
                    QtJambiAPI::addToJavaCollection(env, parameterClassTypes, javaClass);
                    signal.emitMethodInfo.parameterTypeInfos.append({qMetaType.id(),
                                                    getGlobalClassRef(env, javaClass)});
                }
                signal.emitMethodInfo.isStatic = false;
                signal.emitMethodInfo.declaringClass = getGlobalClassRef(env, Java::QtJambi::MetaObjectData$SignalInfo::signalClass(env, signalInfo));
            }
        }
    }

    {
        jobject methods = Java::QtJambi::MetaObjectData::constructors(env,meta_data_struct);
        jobject constructorMetaTypes = Java::QtJambi::MetaObjectData::constructorMetaTypes(env,meta_data_struct);
        m_constructor_count = QtJambiAPI::sizeOfJavaCollection(env, methods);
        m_constructors.resize(m_constructor_count);
        analyze_methods(env, classLoader, m_constructor_count, methods, constructorMetaTypes, m_constructors);
    }

    {
        jobject property_readers = Java::QtJambi::MetaObjectData::propertyReaders(env,meta_data_struct);
        jobject propertyMetaTypes = Java::QtJambi::MetaObjectData::propertyMetaTypes(env,meta_data_struct);
        jobject propertyClassTypes = Java::QtJambi::MetaObjectData::propertyClassTypes(env,meta_data_struct);
        m_property_count = QtJambiAPI::sizeOfJavaCollection(env, property_readers);
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + m_property_count);
        analyze_methods(env, classLoader, m_property_count, property_readers, propertyMetaTypes, m_property_readers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyWriters(env,meta_data_struct),
                        propertyMetaTypes,
                        m_property_writers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyResetters(env,meta_data_struct),
                        nullptr,
                        m_property_resetters);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyDesignableResolvers(env,meta_data_struct),
                        nullptr,
                        m_property_designable_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyScriptableResolvers(env,meta_data_struct),
                        nullptr,
                        m_property_scriptable_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyEditableResolvers(env,meta_data_struct),
                        nullptr,
                        m_property_editable_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyUserResolvers(env,meta_data_struct),
                        nullptr,
                        m_property_user_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyStoredResolvers(env,meta_data_struct),
                        nullptr,
                        m_property_stored_resolvers);
#else
        analyze_methods(env, classLoader, m_property_count,
                        Java::QtJambi::MetaObjectData::propertyBindables(env,meta_data_struct),
                        nullptr,
                        m_property_bindables);
#endif
        jobject propertyNotifies = Java::QtJambi::MetaObjectData::propertyNotifies(env,meta_data_struct);
        Q_ASSERT(m_property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyNotifies));
        for(int i=0; i<m_property_count; ++i){
            jobject fieldIndex = Java::Runtime::List::get(env, propertyNotifies, i);
            if(fieldIndex){
                jint idx = QtJambiAPI::fromJavaIntegerObject(env, fieldIndex);
                m_property_notifies[i] = idx;
            }else{
                m_property_notifies[i] = -1;
            }
        }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        jobject propertyQPropertyFields = Java::QtJambi::MetaObjectData::propertyQPropertyFields(env,meta_data_struct);
        Q_ASSERT(m_property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyQPropertyFields));
        for(int i=0; i<m_property_count; ++i){
            jobject fieldObject = Java::Runtime::List::get(env, propertyQPropertyFields, i);
            if(fieldObject){
                JObjectArrayWrapper metaTypes(env, jobjectArray(Java::Runtime::List::get(env, propertyMetaTypes, i)));
                jclass javaClass = jclass(Java::Runtime::List::get(env, propertyClassTypes, i));
                jobject mt = metaTypes.at(env, 0);
                QMetaType qMetaType(Java::QtJambi::MetaObjectData$MetaTypeInfo::metaTypeId(env, mt));
                jstring jtypeName = Java::QtJambi::MetaObjectData$MetaTypeInfo::typeName(env, mt);
                QString typeName = jtypeName ? qtjambi_cast<QString>(env, jtypeName) : QLatin1String(qMetaType.name());
                ParameterTypeInfo propertyTypeInfo{qMetaType.id(),
                                                   typeName,
                                                   getGlobalClassRef(env, javaClass)};
                m_property_QProperty_fields[i].propertyField = env->FromReflectedField(fieldObject);
                m_property_QProperty_fields[i].valueMethod.parameterTypeInfos << propertyTypeInfo;
                jclass propertyClassType = Java::Runtime::Field::getType(env, fieldObject);
                if(Java::QtCore::QObject$QAbstractComputedProperty::isAssignableFrom(env, propertyClassType)){
                    if(Java::QtCore::QObject$QComputedBooleanProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::z;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedBooleanProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedBooleanProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedByteProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::b;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedByteProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedByteProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedShortProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::s;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedShortProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedShortProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedIntProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::i;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedIntProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedIntProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedLongProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::j;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedLongProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedLongProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedFloatProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::f;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedFloatProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedFloatProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedDoubleProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::d;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedDoubleProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedDoubleProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedCharProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::c;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedCharProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedCharProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::l;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QComputedProperty::value_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedProperty::getClass(env));
                    }
                }else{
                    m_property_QProperty_fields[i].setValueMethod.parameterTypeInfos << ParameterTypeInfo::voidTypeInfo(env) << propertyTypeInfo;
                    m_property_QProperty_fields[i].setValueMethod.methodType = JMethodType::v;
                    if(Java::QtCore::QObject$QBooleanProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::z;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QBooleanProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QBooleanProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QBooleanProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QByteProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::b;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QByteProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QByteProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QByteProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QShortProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::s;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QShortProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QShortProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QShortProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QIntProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::i;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QIntProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QIntProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QIntProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QLongProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::j;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QLongProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QLongProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QLongProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QFloatProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::f;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QFloatProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QFloatProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QFloatProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QDoubleProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::d;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QDoubleProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QDoubleProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QDoubleProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QCharProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::c;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QCharProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QCharProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QCharProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QProperty::isAssignableFrom(env, propertyClassType)){
                        m_property_QProperty_fields[i].valueMethod.methodType = JMethodType::l;
                        m_property_QProperty_fields[i].valueMethod.methodId = Java::QtCore::QObject$QProperty::value_ID(env);
                        m_property_QProperty_fields[i].setValueMethod.methodId = Java::QtCore::QObject$QProperty::setValue_ID(env);
                        m_property_QProperty_fields[i].valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QProperty::getClass(env));
                        m_property_QProperty_fields[i].setValueMethod.declaringClass = m_property_QProperty_fields[i].valueMethod.declaringClass;
                    }
                }
            }
        }
#endif

        jobject propertyMemberFields = Java::QtJambi::MetaObjectData::propertyMemberFields(env,meta_data_struct);
        Q_ASSERT(m_property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyMemberFields));
        for(int i=0; i<m_property_count; ++i){
            jobject fieldObject = Java::Runtime::List::get(env, propertyMemberFields, i);
            if(fieldObject){
                JObjectArrayWrapper metaTypes(env, jobjectArray(Java::Runtime::List::get(env, propertyMetaTypes, i)));
                jobject mt = metaTypes.at(env, 0);
                QMetaType qMetaType(Java::QtJambi::MetaObjectData$MetaTypeInfo::metaTypeId(env, mt));
                jstring jtypeName = Java::QtJambi::MetaObjectData$MetaTypeInfo::typeName(env, mt);
                QString typeName = jtypeName ? qtjambi_cast<QString>(env, jtypeName) : QLatin1String(qMetaType.name());
                jclass javaClass = jclass(Java::Runtime::List::get(env, propertyClassTypes, i));
                jclass fieldType = Java::Runtime::Field::getType(env, fieldObject);
                m_propertyMembers[i].member = env->FromReflectedField(fieldObject);
                int modifier = Java::Runtime::Field::getModifiers(env, fieldObject);
                m_propertyMembers[i].canWrite = !Java::Runtime::Modifier::isFinal(env, modifier);
                m_propertyMembers[i].isStatic = Java::Runtime::Modifier::isStatic(env, modifier);
                m_propertyMembers[i].declaringClass = Java::Runtime::Field::getDeclaringClass(env, fieldObject);
                m_propertyMembers[i].declaringClass = getGlobalClassRef(env, m_propertyMembers[i].declaringClass);
                if(Java::Runtime::Boolean::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::z;
                }else if(Java::Runtime::Byte::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::b;
                }else if(Java::Runtime::Short::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::s;
                }else if(Java::Runtime::Integer::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::i;
                }else if(Java::Runtime::Long::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::j;
                }else if(Java::Runtime::Float::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::f;
                }else if(Java::Runtime::Double::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::d;
                }else if(Java::Runtime::Character::isPrimitiveType(env, fieldType)){
                    m_propertyMembers[i].type = jValueType::c;
                }else{
                    m_propertyMembers[i].type = jValueType::l;
                    m_propertyMembers[i].memberTypeInfo = ParameterTypeInfo{qMetaType.id(),
                                                                            typeName,
                                                                            getGlobalClassRef(env, javaClass)};
                }
            }
        }
    }

    jobject extra_data = Java::QtJambi::MetaObjectData::relatedMetaObjects(env, meta_data_struct);
    jsize extra_data_count = extra_data != nullptr ? QtJambiAPI::sizeOfJavaCollection(env, extra_data) : 0;
    if (extra_data_count == 1) {
        jclass el = jclass(Java::Runtime::List::get(env, extra_data, 0));
        QMetaObject const* superType = CoreAPI::metaObjectForClass(env, el);
        if(q->d.relatedMetaObjects)
            delete[] q->d.relatedMetaObjects;
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        q->d.relatedMetaObjects = new QMetaObject const*(superType);
#else
        q->d.relatedMetaObjects = new QMetaObject::SuperData(superType);
#endif
    }else if (extra_data_count > 1) {
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + extra_data_count);
        // ensure to not have a pointer to a static_metacall method
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        QMetaObject  const* * _relatedMetaObjects = new QMetaObject const*[size_t(extra_data_count)];
#else
        QMetaObject::SuperData* _relatedMetaObjects = new QMetaObject::SuperData[size_t(extra_data_count)];
#endif
        for (jsize i=0; i<extra_data_count; ++i) {
            jclass el = jclass(Java::Runtime::List::get(env, extra_data, i));
            QMetaObject const* superType = CoreAPI::metaObjectForClass(env, el);
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
            _relatedMetaObjects[i] = superType;
#else
            _relatedMetaObjects[i].direct = superType;
#endif
        }
        q->d.relatedMetaObjects = _relatedMetaObjects;
    }

    bool hasStaticMembers = Java::QtJambi::MetaObjectData::hasStaticMembers(env,meta_data_struct);
    const QMetaObjectPrivate* priv = QMetaObjectPrivate::get(q);
    if(hasStaticMembers || (priv->flags & PropertyAccessInStaticMetaCall) == PropertyAccessInStaticMetaCall){
        if(Java::QtCore::QObject::isAssignableFrom(env, m_clazz)){
            q->d.static_metacall = create_static_metacall(q, &static_metacall_QObject);
        }else if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, m_clazz)){
            q->d.static_metacall = create_static_metacall(q, &static_metacall_QtSubType);
        }else{
            q->d.static_metacall = create_static_metacall(q, &static_metacall_any_type);
        }
    }else{
        q->d.static_metacall = nullptr;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    jobject metaTypesList = Java::QtJambi::MetaObjectData::metaTypes(env, meta_data_struct);
    jint parameterCount = QtJambiAPI::sizeOfJavaCollection(env, metaTypesList);
    {
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + parameterCount);
        const QtPrivate::QMetaTypeInterface ** metaTypes = parameterCount==0 ? nullptr : new const QtPrivate::QMetaTypeInterface *[size_t(parameterCount)];
        for(jint i=0; i<parameterCount; ++i){
            jobject entry = Java::Runtime::List::get(env, metaTypesList, i);
            jint metaTypeId = QtJambiAPI::fromJavaIntegerObject(env, entry);
            QMetaType t(metaTypeId);
            metaTypes[i] = t.iface();
        }
        q->d.metaTypes = metaTypes;
    }
#endif
}

void QtJambiMetaObjectPrivate::invokeMethod(JNIEnv *env, jobject object, const JMethodInfo& methodInfo, void **_a, bool forceObjectType) const
{
    QtJambiExceptionHandler __exceptionHandler;
    QTJAMBI_JNI_LOCAL_FRAME(env, 32+methodInfo.parameterTypeInfos.size());
    QtJambiScope scope(nullptr);
    try{
        Q_ASSERT(methodInfo.methodId);
        QVector<jvalue> converted_arguments(methodInfo.parameterTypeInfos.size()-1);

        bool success = true;
        for (int i = 0; i < converted_arguments.size(); ++i) {
            const ParameterTypeInfo& parameterTypeInfo = methodInfo.parameterTypeInfos[i + 1];
            converted_arguments[i].l = nullptr;
            if (!parameterTypeInfo.convertInternalToExternal(env, &scope, _a[i+1], converted_arguments[i], forceObjectType || !Java::Runtime::Class::isPrimitive(env, parameterTypeInfo.javaClass()))) {
                success = false;
                break;
            }
        }

        if (success) {
            jvalue returnValue;
            returnValue.j = 0;
            jvalue *args = converted_arguments.data();

            bool isVoid = false;
            if(methodInfo.isStatic){
                switch(methodInfo.methodType){
                case JMethodType::v:
                    isVoid = true;
                    env->CallStaticVoidMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::z:
                    returnValue.z = env->CallStaticBooleanMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::b:
                    returnValue.b = env->CallStaticByteMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::i:
                    returnValue.i = env->CallStaticIntMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::s:
                    returnValue.s = env->CallStaticShortMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::j:
                    returnValue.j = env->CallStaticLongMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::f:
                    returnValue.f = env->CallStaticFloatMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::d:
                    returnValue.d = env->CallStaticDoubleMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                case JMethodType::c:
                    returnValue.c = env->CallStaticCharMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                default:
                    returnValue.l = env->CallStaticObjectMethodA(methodInfo.declaringClass, methodInfo.methodId, args); break;
                }
            }else{
                if(env->IsSameObject(object, nullptr)){
                    qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeMethod: Cannot call %s on null",
                              J2CStringBuffer(env, Java::Runtime::Object::toString(env, env->ToReflectedMethod(methodInfo.declaringClass, methodInfo.methodId, methodInfo.isStatic))).constData());
                    return;
                }
                jmethodID methodId = methodInfo.methodId;
                if(!env->IsInstanceOf(object, methodInfo.declaringClass)){
                    bool found = false;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    const QList<jclass> classes = m_qmlExtensions.keys();
                    for(jclass cls : classes){
                        if(env->IsInstanceOf(object, cls)){
                            const QmlExtensionData& data = m_qmlExtensions[cls];
                            found = true;
                            if(data.wrapper){
                                object = env->CallStaticObjectMethod(methodInfo.declaringClass, data.wrapper, object);
                            }else{
                                methodId = data.methodMappings[methodInfo.methodId];
                            }
                            break;
                        }
                    }
#endif
                    if(!found || !methodId){
                        qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeMethod: Object type mismatch (%s), expected: %s",
                                 qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                                 qPrintable(QtJambiAPI::getClassName(env, methodInfo.declaringClass).replace('$', '.')));
                        return;
                    }
                }
                switch(methodInfo.methodType){
                case JMethodType::v:
                    isVoid = true;
                    env->CallVoidMethodA(object, methodId, args); break;
                case JMethodType::z:
                    returnValue.z = env->CallBooleanMethodA(object, methodId, args); break;
                case JMethodType::b:
                    returnValue.b = env->CallByteMethodA(object, methodId, args); break;
                case JMethodType::i:
                    returnValue.i = env->CallIntMethodA(object, methodId, args); break;
                case JMethodType::s:
                    returnValue.s = env->CallShortMethodA(object, methodId, args); break;
                case JMethodType::j:
                    returnValue.j = env->CallLongMethodA(object, methodId, args); break;
                case JMethodType::f:
                    returnValue.f = env->CallFloatMethodA(object, methodId, args); break;
                case JMethodType::d:
                    returnValue.d = env->CallDoubleMethodA(object, methodId, args); break;
                case JMethodType::c:
                    returnValue.c = env->CallCharMethodA(object, methodId, args); break;
                default:
                    returnValue.l = env->CallObjectMethodA(object, methodId, args); break;
                }
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            if(_a[0]){
                if(!isVoid){
                    methodInfo.parameterTypeInfos[0].convertExternalToInternal(env, nullptr, returnValue, _a[0], jValueType(methodInfo.methodType));
                }
            }
        }
    }catch(const JavaException& exn){
        __exceptionHandler.handle(env, exn, nullptr);
    }
}

/**
 * This method calls the constructor of a Java class caused by a meta object constructor call.
 */
void QtJambiMetaObjectPrivate::invokeConstructor(JNIEnv *env, const JMethodInfo& methodInfo, void **_a) const
{
    QtJambiExceptionHandler __exceptionHandler;
    JniLocalFrame __jniLocalFrame(env, 32 + methodInfo.parameterTypeInfos.size());
    QtJambiScope scope(nullptr);
    try{
        QVector<jvalue> converted_arguments(methodInfo.parameterTypeInfos.size() - 1);
        bool success = true;

        for (int i = 0; i < converted_arguments.size(); ++i) {
            const ParameterTypeInfo& parameterTypeInfo = methodInfo.parameterTypeInfos[i + 1];
            converted_arguments[i].l = nullptr;
            if (!parameterTypeInfo.convertInternalToExternal(env, &scope, _a[i+1], converted_arguments[i], !Java::Runtime::Class::isPrimitive(env, parameterTypeInfo.javaClass()))) {
                success = false;
                break;
            }
        }

        if (success) {
            jvalue *args = converted_arguments.data();
            jobject object = env->NewObjectA(m_clazz, methodInfo.methodId, args);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, m_clazz)){
                void* &pointer = *reinterpret_cast<void**>(_a[0]);
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object))
                    pointer = link->pointer();
            }else{
                jobject &pointer = *reinterpret_cast<jobject*>(_a[0]);
                pointer = object;
            }
        } else {
            qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeConstructor: Failed to convert arguments");
        }
    }catch(const JavaException& exn){
        __exceptionHandler.handle(env, exn, nullptr);
    }
}


QtJambiMetaObject::QtJambiMetaObject(JNIEnv *jni_env, jclass java_class)
    : d_ptr(new QtJambiMetaObjectPrivate(this, jni_env, java_class)) {
}

void QtJambiMetaObject::initialize(JNIEnv *env, const QMetaObject *original_meta_object, bool hasCustomMetaObject){
    d_ptr->initialize(env, original_meta_object, hasCustomMetaObject);
}

QtJambiMetaObject::~QtJambiMetaObject()
{
    delete d_ptr;
}

jweak QtJambiMetaObjectPrivate::javaInstance() const{
    return m_javaInstance;
}

void QtJambiMetaObjectPrivate::setJavaInstance(jweak weak){
    m_javaInstance = weak;
}

jclass QtJambiMetaObjectPrivate::javaClass() const{
    return m_clazz;
}

jweak QtJambiMetaObject::javaInstance(const QtJambiMetaObject* metaObject){
    return metaObject->d_ptr->javaInstance();
}

void QtJambiMetaObject::setJavaInstance(const QtJambiMetaObject* metaObject, jweak weak){
    metaObject->d_ptr->setJavaInstance(weak);
}

const QtJambiMetaObject* QtJambiMetaObject::cast(const QMetaObject* metaObject){
    return isInstance(metaObject) ? static_cast<const QtJambiMetaObject*>(metaObject) : nullptr;
}

bool QtJambiMetaObject::isInstance(const QMetaObject* metaObject){
    if(!metaObject)
        return false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return metaObject->d.stringdata[0].data()==QtJambiMetaObjectID;
#else
    uint offset = metaObject->d.stringdata[0];
    uint length = metaObject->d.stringdata[1];
    if(length==sizeof(const char*)){
        const char* stringdata = reinterpret_cast<const char*>(metaObject->d.stringdata);
        const char* __stringdata = stringdata + offset;
        const void*const pointer = *reinterpret_cast<const void*const*>(__stringdata);
        return pointer==QtJambiMetaObjectID;
    }
    return false;
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
}

const QMetaObject *QtJambiMetaObject::findFirstStaticMetaObject(const QMetaObject *meta_object)
{
    while (meta_object && QtJambiMetaObject::isInstance(meta_object))
        meta_object = meta_object->superClass();
    return meta_object;
}

jclass QtJambiMetaObject::javaClass() const{
    return d_ptr->javaClass();
}

bool QtJambiMetaObject::hasSignals() const{
    return d_ptr->m_signal_count>0;
}

jobject QtJambiMetaObject::signalTypes(int index) const{
    return d_ptr->m_signals[index].signalTypes;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
jfieldID QtJambiMetaObject::getQPropertyField(int index) const{
    return d_ptr->m_property_QProperty_fields[index].propertyField;
}

void QtJambiMetaObject::registerQPropertyField(int index, jfieldID field){
    if(!d_ptr->m_property_QProperty_fields[index].propertyField)
        d_ptr->m_property_QProperty_fields[index].propertyField = field;
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

jclass QtJambiMetaObject::javaClass(JNIEnv * env, const QMetaObject* metaObject, bool exactOrNull){
    if(!metaObject){
        return nullptr;
    }else if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return jclass(env->NewLocalRef(dynamicMetaObject->javaClass()));
    }else{
        if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
            jclass result = JavaAPI::resolveClass(env, getJavaName(*typeId));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return result;
        }else if(const std::type_info* typeId = getTypeByQtName(metaObject->className())){
            if(!registeredOriginalMetaObject(*typeId)){
                jclass result = JavaAPI::resolveClass(env, getJavaName(*typeId));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                return result;
            }
        }
        if(exactOrNull){
            return nullptr;
        }else if(metaObject->superClass()){
            return javaClass(env, metaObject->superClass(), exactOrNull);
        }else{
            return Java::QtJambi::QtGadget::getClass(env);
        }
    }
}

jobject QtJambiMetaObject::toReflected(JNIEnv * env, const QMetaMethod& method)
{
    Q_ASSERT(method.isValid());
    const QMetaObject* metaObject = method.enclosingMetaObject();
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        if(method.methodType()==QMetaMethod::Constructor){
            for(int i=0; i<method.enclosingMetaObject()->constructorCount(); ++i){
                if(method==method.enclosingMetaObject()->constructor(i)){
                    if(const JMethodInfo* info = dynamicMetaObject->d_ptr->constructorInfo(i)){
                        if(info->methodId)
                            return env->ToReflectedMethod(info->declaringClass, info->methodId, info->isStatic);
                    }
                }
            }
        }else{
            if(const JMethodInfo* info = dynamicMetaObject->d_ptr->methodInfo(method.methodIndex() - method.enclosingMetaObject()->methodOffset())){
                if(info->methodId)
                    return env->ToReflectedMethod(info->declaringClass, info->methodId, info->isStatic);
            }
        }
    }
    return nullptr;
}

hash_type qHash(const QMetaMethod& method, hash_type seed = 0){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return qHashMulti(seed, method.enclosingMetaObject(), method.methodIndex());
#else
    QtPrivate::QHashCombine hash;
    seed = hash(seed, qintptr(method.enclosingMetaObject()));
    seed = hash(seed, method.methodIndex());
    return seed;
#endif
}

const QList<ParameterTypeInfo>& QtJambiMetaObject::methodParameterInfo(JNIEnv * env, const QMetaMethod& method)
{
    Q_ASSERT(method.isValid());
    const QMetaObject* metaObject = method.enclosingMetaObject();
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        if(method.methodType()==QMetaMethod::Constructor){
            for(int i=0; i<method.enclosingMetaObject()->constructorCount(); ++i){
                if(method==method.enclosingMetaObject()->constructor(i)){
                    if(const JMethodInfo* info = dynamicMetaObject->d_ptr->constructorInfo(i)){
                        return info->parameterTypeInfos;
                    }
                }
            }
        }else{
            if(const JMethodInfo* info = dynamicMetaObject->d_ptr->methodInfo(method.methodIndex() - method.enclosingMetaObject()->methodOffset())){
                return info->parameterTypeInfos;
            }
        }
    }
    const QList<ParameterTypeInfo>* cresult = nullptr;
    hash_type key = qHash(method);
    {
        QReadLocker locker(gJMethodInfoInfosLock());
        Q_UNUSED(locker)
        cresult = (*gParameterTypeInfos)[key];
    }
    if(!cresult){
        QList<ParameterTypeInfo>* result = new QList<ParameterTypeInfo>();
        ParameterInfoProvider ptip = registeredParameterInfoProvider(method.enclosingMetaObject());
        QList<ParameterInfo> parameterInfos;
        if(ptip && ptip(method, parameterInfos)){
            result->reserve(parameterInfos.size());
            for(const ParameterInfo& info : parameterInfos){
                if(!info.javaClass){
                    switch(info.metaTypeId){
                    case QMetaType::Void:
                        result->append(ParameterTypeInfo::voidTypeInfo(env));
                        break;
                    case QMetaType::UChar:
                    case QMetaType::Char:
                    case QMetaType::SChar:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Byte::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.b = *reinterpret_cast<const qint8*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaByteObject(env, out.b);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               qint8 _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaByteObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.b;
                                               break;
                                               }
                                               if(scope && !out){
                                                   qint8* _out = new qint8;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<qint8*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::UShort:
                    case QMetaType::Short:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Short::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.s = *reinterpret_cast<const qint16*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaShortObject(env, out.s);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               qint16 _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaShortObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.s;
                                               break;
                                               }
                                               if(scope && !out){
                                                   qint16* _out = new qint16;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<qint16*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    case QMetaType::Char32:
#endif
                    case QMetaType::UInt:
                    case QMetaType::Int:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Integer::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.i = *reinterpret_cast<const qint32*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaIntegerObject(env, out.i);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               qint32 _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaIntegerObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.i;
                                               break;
                                               }
                                               if(scope && !out){
                                                   qint32* _out = new qint32;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<qint32*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::ULongLong:
                    case QMetaType::LongLong:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Long::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.j = *reinterpret_cast<const qint64*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaLongObject(env, out.j);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               qint64 _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaLongObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.j;
                                               break;
                                               }
                                               if(scope && !out){
                                                   qint64* _out = new qint64;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<qint64*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::ULong:
                    case QMetaType::Long:
                        if(QMetaType(info.metaTypeId).sizeOf()==4){
                            result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Integer::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.i = *reinterpret_cast<const qint32*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaIntegerObject(env, out.i);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               qint32 _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaIntegerObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.i;
                                               break;
                                               }
                                               if(scope && !out){
                                                   qint32* _out = new qint32;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<qint32*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        }else{
                            result->append(ParameterTypeInfo(
                                    info.metaTypeId,
                                    getGlobalClassRef(env, Java::Runtime::Long::primitiveType(env)),
                                    [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                           out.j = *reinterpret_cast<const qint64*>(in);
                                           if(forceBoxedType)
                                               out.l = QtJambiAPI::toJavaLongObject(env, out.j);
                                           return true;
                                    },
                                    [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                           qint64 _in;
                                           switch(valueType){
                                           case jValueType::l:
                                               _in = QtJambiAPI::fromJavaLongObject(env, in.l);
                                           break;
                                           default:
                                               _in = in.j;
                                           break;
                                           }
                                           if(scope && !out){
                                               qint64* _out = new qint64;
                                               scope->addDeletion(_out);
                                               out = _out;
                                           }
                                           if(!out)
                                               return false;
                                           *reinterpret_cast<qint64*>(out) = _in;
                                           return true;
                                    }
                              ));
                        }
                        break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    case QMetaType::Char16:
#endif
                    case QMetaType::QChar:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Character::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.c = jchar(reinterpret_cast<const QChar*>(in)->unicode());
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaCharacterObject(env, out.c);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               QChar _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaCharacterObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.c;
                                               break;
                                               }
                                               if(scope && !out){
                                                   QChar* _out = new QChar;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<QChar*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::Float:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Float::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.f = *reinterpret_cast<const float*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaFloatObject(env, out.f);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               float _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaFloatObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.f;
                                               break;
                                               }
                                               if(scope && !out){
                                                   float* _out = new float;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<float*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::Double:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Double::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.d = *reinterpret_cast<const double*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaDoubleObject(env, out.d);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               double _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaDoubleObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.d;
                                               break;
                                               }
                                               if(scope && !out){
                                                   double* _out = new double;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<double*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::Bool:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Boolean::primitiveType(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool forceBoxedType)->bool{
                                               out.z = *reinterpret_cast<const bool*>(in);
                                               if(forceBoxedType)
                                                   out.l = QtJambiAPI::toJavaBooleanObject(env, out.z);
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool {
                                               bool _in;
                                               switch(valueType){
                                               case jValueType::l:
                                                   _in = QtJambiAPI::fromJavaBooleanObject(env, in.l);
                                               break;
                                               default:
                                                   _in = in.z;
                                               break;
                                               }
                                               if(scope && !out){
                                                   bool* _out = new bool;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<bool*>(out) = _in;
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::QString:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::String::getClass(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool)->bool{
                                               out.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QString*>(in));
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                               if(scope && !out){
                                                   QString* _out = new QString;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<QString*>(out) = qtjambi_cast<QString>(env, in.l);
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::QByteArray:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::QtCore::QByteArray::getClass(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool)->bool{
                                               out.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QByteArray*>(in));
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                               if(scope && !out){
                                                   QByteArray* _out = new QByteArray;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<QByteArray*>(out) = qtjambi_cast<QByteArray>(env, in.l);
                                               return true;
                                        }
                                  ));
                        break;
                    case QMetaType::QVariant:
                        result->append(ParameterTypeInfo(
                                        info.metaTypeId,
                                        getGlobalClassRef(env, Java::Runtime::Object::getClass(env)),
                                        [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& out, bool)->bool{
                                               out.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariant*>(in));
                                               return true;
                                        },
                                        [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                               if(scope && !out){
                                                   QVariant* _out = new QVariant;
                                                   scope->addDeletion(_out);
                                                   out = _out;
                                               }
                                               if(!out)
                                                   return false;
                                               *reinterpret_cast<QVariant*>(out) = qtjambi_cast<QVariant>(env, in.l);
                                               return true;
                                        }
                                  ));
                        break;
                    default:
                        break;
                    }
                }else{
                    result->append({info.metaTypeId,
                                    JavaAPI::resolveClass(env, info.javaClass),
                                    info.qtToJavaConverterFunction,
                                    info.javaToQtConverterFunction});
                }
            }
        }else{
            int length = method.parameterCount()+1;
            result->reserve(length);
            jobject classLoader = nullptr;
            QList<QByteArray> parameterTypes = method.parameterTypes();
            for (int i=0; i<length; ++i){
                QByteArray qTypeName;
                int metaType;
                if(i==0){
                    if(method.methodType()==QMetaMethod::Constructor){
                        result->append({QMetaType::UnknownType,nullptr,nullptr,nullptr});
                        continue;
                    }
                    qTypeName = method.typeName();
                    metaType = method.returnType();
                }else{
                    qTypeName = parameterTypes.at(i-1).data();
                    metaType = method.parameterType(i-1);
                }
                if(metaType==QMetaType::UnknownType)
                    metaType = QMetaType::type(qTypeName);
                else if(metaType!=QMetaType::UnknownType && qTypeName.isEmpty())
                    qTypeName = QMetaType::typeName(metaType);
                QMetaType _metaType(metaType);
                QString externalTypeName = QtJambiTypeManager::getExternalTypeName(env, qTypeName, method.enclosingMetaObject(), _metaType);
                jclass javaClass = JavaAPI::resolveClass(env, qPrintable(externalTypeName), classLoader);
                //Q_ASSERT(javaClass);
                if(javaClass){
                    // Find usage pattern
                    InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::tryGetInternalToExternalConverter(
                                                                    env,
                                                                    QLatin1String(qTypeName),
                                                                    _metaType,
                                                                    javaClass,
                                                                    true);
                    ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::tryGetExternalToInternalConverter(
                                                                    env,
                                                                    javaClass,
                                                                    QLatin1String(qTypeName),
                                                                    _metaType);

                    if(Java::Runtime::Map::isAssignableFrom(env, javaClass)){
                        javaClass = getGlobalClassRef(env, Java::Runtime::Map::getClass(env));
                    }else if(Java::Runtime::Collection::isAssignableFrom(env, javaClass)){
                        javaClass = getGlobalClassRef(env, Java::Runtime::Collection::getClass(env));
                    }

                    result->append({metaType,
                                    QLatin1String(qTypeName),
                                    javaClass,
                                    std::move(internalToExternalConverter),
                                    std::move(externalToInternalConverter)});
                }else{
                    InternalToExternalConverter internalToExternalConverter;
                    if(method.methodType()==QMetaMethod::Signal && i>0){
                        internalToExternalConverter = [](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool) -> bool{
                            p.l = Java::QtJambi::QNativePointer::fromNative(env, jlong(in), int(QNativePointer::Type::Pointer), jlong(-1), 0, true);
                            Java::QtJambi::QNativePointer::setVerificationEnabled(env, p.l, true);
                            if(scope){
                                JObjectWrapper obj(env, p.l);
                                scope->addFinalAction([obj](){
                                    if(JniEnvironment env{200}){
                                        Java::QtJambi::QNativePointer::invalidate(env, obj.object());
                                    }
                                });
                            }
                            return true;
                        };
                    }
                    result->append({QMetaType::UnknownType,nullptr,std::move(internalToExternalConverter),nullptr});
                }
            }
        }
        QWriteLocker locker(gJMethodInfoInfosLock());
        Q_UNUSED(locker)
        gParameterTypeInfos->insert(key, result);
        cresult = result;
    }
    Q_ASSERT(cresult);
    return *cresult;
}

int QtJambiMetaObject::methodFromJMethod(const QMetaObject* metaObject, jmethodID methodId){
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return dynamicMetaObject->d_ptr->methodFromJMethod(methodId);
    }
    return -1;
}

QtJambiMetaObject::SignalInfo QtJambiMetaObject::signalInfo(const QMetaObject* metaObject, jfieldID fieldId, jmethodID emitMethodID){
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return dynamicMetaObject->d_ptr->signalInfo(fieldId, emitMethodID);
    }
    return SignalInfo();
}

QVector<QtJambiMetaObject::SignalInfo> QtJambiMetaObject::signalInfos(const QMetaObject* metaObject, jfieldID fieldId){
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return dynamicMetaObject->d_ptr->signalInfos(fieldId);
    }
    return {};
}

const JMethodInfo* QtJambiMetaObjectPrivate::constructorInfo(int index) const{
    const JMethodInfo* result = nullptr;
    if(index>=0 && index<m_constructors.size()){
        result = &m_constructors[index];
    }
    return result;
}

const JMethodInfo* QtJambiMetaObjectPrivate::methodInfo(int index) const{
    const JMethodInfo* result = nullptr;
    if(index>=0){
        if(index<m_signals.size()){
            result = &m_signals[index].emitMethodInfo;
        }else{
            index -= m_signal_count;
            if(index>=0 && index<m_methods.size()){
                result = &m_methods[index];
            }
        }
    }
    return result;
}

int QtJambiMetaObjectPrivate::methodFromJMethod(jmethodID methodId) const{
    Q_Q(const QtJambiMetaObject);
    int index = m_methodIndexes.value(methodId, -1);
    if(index>=0){
        index += m_signal_count + q->methodOffset();
    }
    return index;
}

QtJambiMetaObject::SignalInfo QtJambiMetaObjectPrivate::signalInfo(jfieldID fieldId, jmethodID emitMethodId) const{
    Q_Q(const QtJambiMetaObject);
    QHash<jmethodID,int> indexesByEmit = m_signalIndexes.value(fieldId);
    int index(-1);
    if(emitMethodId){
        index = indexesByEmit.value(emitMethodId, -1);
    }else if(!indexesByEmit.isEmpty()){
        index = *indexesByEmit.begin();
    }
    if(index>=0){
        return QtJambiMetaObject::SignalInfo(q, index+q->methodOffset(), m_signals[index].signalTypes, m_signals[index].emitMethodInfo.declaringClass);
    }
    return QtJambiMetaObject::SignalInfo();
}

QVector<QtJambiMetaObject::SignalInfo> QtJambiMetaObjectPrivate::signalInfos(jfieldID fieldId) const{
    Q_Q(const QtJambiMetaObject);
    QHash<jmethodID,int> indexesByEmit = m_signalIndexes.value(fieldId);
    QVector<QtJambiMetaObject::SignalInfo> result;
    for(int index : qAsConst(indexesByEmit)){
        result << QtJambiMetaObject::SignalInfo(q, index+q->methodOffset(), m_signals[index].signalTypes, m_signals[index].emitMethodInfo.declaringClass);
    }
    return result;
}

int QtJambiMetaObject::metaCall(QObject * o, QMetaObject::Call cl, int idx, void **argv){
    if(JniEnvironment env{200}){
        switch(cl){
        case QMetaObject::CreateInstance:
            return invokeConstructor(env, idx, argv);
        case QMetaObject::InvokeMetaMethod:
            if(!o){
                return static_metacall(cl, idx + methodOffset(), argv);
            }
            break;
        default:
            break;
        }
        jobject object = o ? QtJambiAPI::convertQObjectToJavaObject(env, o) : nullptr;
        if(!env->IsSameObject(object, nullptr)){
            switch(cl){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case QMetaObject::QueryPropertyUser:
                queryPropertyUser(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyDesignable:
                queryPropertyDesignable(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyScriptable:
                queryPropertyScriptable(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyStored:
                queryPropertyStored(env, object, idx, argv, true);
                break;
            case QMetaObject::QueryPropertyEditable:
                queryPropertyEditable(env, object, idx, argv, true);
                break;
#else
            case QMetaObject::BindableProperty:
                bindableProperty(env, object, idx, argv, true);
                break;
#endif
            case QMetaObject::ResetProperty:
                resetProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::ReadProperty:
                readProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::WriteProperty:
                writeProperty(env, object, idx, argv, true);
                break;
            case QMetaObject::InvokeMetaMethod:
                invokeSignalOrSlot(env, object, idx, argv, true);
                break;
#if 0
            case QMetaObject::IndexOfMethod:
                printf("QMetaObject::IndexOfMethod\n");
                break;
            case QMetaObject::RegisterPropertyMetaType:
                printf("QMetaObject::RegisterPropertyMetaType\n");
                break;
            case QMetaObject::RegisterMethodArgumentMetaType:
                printf("QMetaObject::RegisterMethodArgumentMetaType\n");
                break;
#endif
            default:
                break;
            }
        }else if(o){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(o)){
                if(link->isDeleteLater())
                    return idx;
            }
            qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::metaCall: metaCall on incomplete object ot type %s",
                      o->metaObject()->className());
        }
    }
    return idx;
}

int QtJambiMetaObject::metaCall(QMetaObject::Call cl, int idx, void ** argv){
    if(JniEnvironment env{200}){
        switch(cl){
        case QMetaObject::CreateInstance:
            return invokeConstructor(env, idx, argv);
        case QMetaObject::InvokeMetaMethod:
            return static_metacall(cl, idx + methodOffset(), argv);
        default:
            break;
        }
    }
    return idx;
}

int QtJambiMetaObject::invokeSignalOrSlot(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->invokeSignalOrSlot(env, object, _id, _a);
    if (_id < 0) return _id;

    // Emit the correct signal
    if (_id < d->m_signal_count) {
        const JSignalInfo& signal = d->m_signals[_id];
        if(Java::QtCore::QObject::isInstanceOf(env, object) && !signal.isClone){
            QMetaObject::activate(qtjambi_cast<QObject*>(env, object), this, _id, _a);
        }else{
            if(env->IsSameObject(object, nullptr)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeMethod: Object is null");
                return -1;
            }
            if(!env->IsInstanceOf(object, d->m_clazz)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeMethod: Object type mismatch (%s), expected: %s",
                         qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                         qPrintable(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.')));
                return -1;
            }
            jobject signal_object = env->GetObjectField(object, signal.signalField);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            Q_ASSERT(signal_object);
            d->invokeMethod(env, signal_object, signal.emitMethodInfo, _a);
        }
    } else if (_id < d->m_signal_count + d->m_method_count) { // Call the correct method
        d->invokeMethod(env, object, d->m_methods[_id - d->m_signal_count], _a);
    } else if (_id < d->m_signal_count + d->m_method_count + d->m_constructor_count) { // Call the correct constructor
        d->invokeConstructor(env, d->m_constructors[_id - d->m_signal_count - d->m_method_count], _a);
    }
    JavaException::check(env QTJAMBI_STACKTRACEINFO );

    return _id - d->m_method_count - d->m_signal_count - d->m_constructor_count;
}

int QtJambiMetaObject::invokeConstructor(JNIEnv *env, int _id, void **_a) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_signal_count + d->m_method_count + d->m_constructor_count) { // Call the correct constructor
        d->invokeConstructor(env, d->m_constructors[_id], _a);
    }
    return _id - d->m_constructor_count;
}

int QtJambiMetaObject::readProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->readProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        if(d->m_property_readers[_id].methodId){
            d->invokeMethod(env, object, d->m_property_readers[_id], _a);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        }else if (d->m_property_QProperty_fields[_id].propertyField && d->m_property_QProperty_fields[_id].valueMethod.methodId){
            if(env->IsSameObject(object, nullptr)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::readProperty: Object is null");
                return -1;
            }
            if(!env->IsInstanceOf(object, d->m_clazz)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::readProperty: Object type mismatch (%s), expected: %s",
                         qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                         qPrintable(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.')));
                return -1;
            }
            jobject qProperty = env->GetObjectField(object, d->m_property_QProperty_fields[_id].propertyField);
            d->invokeMethod(env, qProperty, d->m_property_QProperty_fields[_id].valueMethod, _a);
#endif
        }else if (d->m_propertyMembers[_id].member){
            QtJambiExceptionHandler __exceptionHandler;
            try{
                if(d->m_propertyMembers[_id].isStatic){
                    switch(d->m_propertyMembers[_id].type){
                    case jValueType::z:
                        *reinterpret_cast<jboolean*>(_a[0]) = env->GetStaticBooleanField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::b:
                        *reinterpret_cast<jbyte*>(_a[0]) = env->GetStaticByteField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::s:
                        *reinterpret_cast<jshort*>(_a[0]) = env->GetStaticShortField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::i:
                        *reinterpret_cast<jint*>(_a[0]) = env->GetStaticIntField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::j:
                        *reinterpret_cast<jlong*>(_a[0]) = env->GetStaticLongField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::f:
                        *reinterpret_cast<float*>(_a[0]) = env->GetStaticFloatField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::d:
                        *reinterpret_cast<double*>(_a[0]) = env->GetStaticDoubleField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::c:
                        *reinterpret_cast<jchar*>(_a[0]) = env->GetStaticCharField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::l:{
                            jvalue value;
                            value.l = env->GetStaticObjectField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            QtJambiScope scope;
                            d->m_propertyMembers[_id].memberTypeInfo.convertExternalToInternal(env, &scope, value, _a[0], jValueType::l);
                        }
                        break;
                    }
                }else{
                    jfieldID fieldId = d->m_propertyMembers[_id].member;
                    if(!env->IsInstanceOf(object, d->m_clazz)){
                        bool found = false;
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        const QList<jclass> classes = d->m_qmlExtensions.keys();
                        for(jclass cls : classes){
                            if(env->IsInstanceOf(object, cls)){
                                const QmlExtensionData& data = d->m_qmlExtensions[cls];
                                found = true;
                                if(data.wrapper){
                                    object = env->CallStaticObjectMethod(d->m_clazz, data.wrapper, object);
                                }else{
                                    fieldId = data.fieldMappings[fieldId];
                                }
                                break;
                            }
                        }
    #endif
                        if(!found || !fieldId){
                            qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::readProperty: Object type mismatch (%s), expected: %s",
                                     qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                                     qPrintable(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.')));
                            return _id - d->m_property_count;
                        }
                    }

                    switch(d->m_propertyMembers[_id].type){
                    case jValueType::z:
                        *reinterpret_cast<jboolean*>(_a[0]) = env->GetBooleanField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::b:
                        *reinterpret_cast<jbyte*>(_a[0]) = env->GetByteField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::s:
                        *reinterpret_cast<jshort*>(_a[0]) = env->GetShortField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::i:
                        *reinterpret_cast<jint*>(_a[0]) = env->GetIntField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::j:
                        *reinterpret_cast<jlong*>(_a[0]) = env->GetLongField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::f:
                        *reinterpret_cast<float*>(_a[0]) = env->GetFloatField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::d:
                        *reinterpret_cast<double*>(_a[0]) = env->GetDoubleField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::c:
                        *reinterpret_cast<jchar*>(_a[0]) = env->GetCharField(object, fieldId);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::l:{
                            jvalue value;
                            value.l = env->GetObjectField(object, fieldId);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            QtJambiScope scope;
                            d->m_propertyMembers[_id].memberTypeInfo.convertExternalToInternal(env, &scope, value, _a[0], jValueType::l);
                        }
                        break;
                    }
                }
            }catch(const JavaException& exn){
                __exceptionHandler.handle(env, exn, nullptr);
            }
        }
    }
    return _id - d->m_property_count;
}

int QtJambiMetaObject::writeProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->writeProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        if(d->m_property_writers[_id].methodId){
            // invokeMethod expects a place holder for return value, but write property meta calls
            // do not since all property writers return void by convention.
            void *a[2] = { nullptr, _a[0] };
            d->invokeMethod(env, object, d->m_property_writers[_id], a);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        }else if (d->m_property_QProperty_fields[_id].propertyField && d->m_property_QProperty_fields[_id].setValueMethod.methodId){
            if(env->IsSameObject(object, nullptr)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::writeProperty: Object is null");
                return -1;
            }
            if(!env->IsInstanceOf(object, d->m_clazz)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::writeProperty: Object type mismatch (%s), expected: %s",
                         qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                         qPrintable(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.')));
                return -1;
            }
            jobject qProperty = env->GetObjectField(object, d->m_property_QProperty_fields[_id].propertyField);
            void *a[2] = { nullptr, _a[0] };
            d->invokeMethod(env, qProperty, d->m_property_QProperty_fields[_id].setValueMethod, a, d->m_property_QProperty_fields[_id].valueMethod.methodType==JMethodType::l);
#endif
        }else if (d->m_propertyMembers[_id].member && d->m_propertyMembers[_id].canWrite){
            QtJambiExceptionHandler __exceptionHandler;
            try{
                if(d->m_propertyMembers[_id].isStatic){
                    switch(d->m_propertyMembers[_id].type){
                    case jValueType::z:
                        env->SetStaticBooleanField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, *reinterpret_cast<jboolean*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::b:
                        env->SetStaticByteField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, *reinterpret_cast<jbyte*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::s:
                        env->SetStaticShortField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, *reinterpret_cast<jshort*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::i:
                        env->SetStaticIntField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, *reinterpret_cast<jint*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::j:
                        env->SetStaticLongField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, *reinterpret_cast<jlong*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::f:
                        env->SetStaticFloatField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, *reinterpret_cast<float*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::d:
                        env->SetStaticDoubleField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, *reinterpret_cast<double*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::c:
                        env->SetStaticCharField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, jchar(*reinterpret_cast<jchar*>(_a[0])));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::l:
                        jvalue value;
                        value.l = nullptr;
                        if (d->m_propertyMembers[_id].memberTypeInfo.convertInternalToExternal(env, nullptr, _a[0], value, true)) {
                            env->SetStaticObjectField(d->m_propertyMembers[_id].declaringClass, d->m_propertyMembers[_id].member, value.l);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            break;
                        }
                        break;
                    }
                }else{
                    jfieldID fieldId = d->m_propertyMembers[_id].member;
                    if(!env->IsInstanceOf(object, d->m_clazz)){
                        bool found = false;
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        const QList<jclass> classes = d->m_qmlExtensions.keys();
                        for(jclass cls : classes){
                            if(env->IsInstanceOf(object, cls)){
                                const QmlExtensionData& data = d->m_qmlExtensions[cls];
                                found = true;
                                if(data.wrapper){
                                    object = env->CallStaticObjectMethod(d->m_clazz, data.wrapper, object);
                                }else{
                                    fieldId = data.fieldMappings[fieldId];
                                }
                                break;
                            }
                        }
    #endif
                        if(!found || !fieldId){
                            qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::writeProperty: Object type mismatch (%s), expected: %s",
                                     qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                                     qPrintable(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.')));
                            return _id - d->m_property_count;
                        }
                    }

                    switch(d->m_propertyMembers[_id].type){
                    case jValueType::z:
                        env->SetBooleanField(object, fieldId, *reinterpret_cast<jboolean*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::b:
                        env->SetByteField(object, fieldId, *reinterpret_cast<jbyte*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::s:
                        env->SetShortField(object, fieldId, *reinterpret_cast<jshort*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::i:
                        env->SetIntField(object, fieldId, *reinterpret_cast<jint*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::j:
                        env->SetLongField(object, fieldId, *reinterpret_cast<jlong*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::f:
                        env->SetFloatField(object, fieldId, *reinterpret_cast<float*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::d:
                        env->SetDoubleField(object, fieldId, *reinterpret_cast<double*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::c:
                        env->SetCharField(object, fieldId, jchar(*reinterpret_cast<jchar*>(_a[0])));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::l:
                        jvalue value;
                        value.l = nullptr;
                        if (d->m_propertyMembers[_id].memberTypeInfo.convertInternalToExternal(env, nullptr, _a[0], value, true)) {
                            env->SetObjectField(object, fieldId, value.l);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            break;
                        }
                        break;
                    }
                }
            }catch(const JavaException& exn){
                __exceptionHandler.handle(env, exn, nullptr);
            }
        }
    }

    return _id - d->m_property_count;
}

int QtJambiMetaObject::resetProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->resetProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_resetters.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_resetters[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtJambiMetaObject::notifyProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->notifyProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count
            && d->m_property_notifies.contains(_id)
            && d->m_property_notifies[_id]>=0
            && d->m_property_notifies[_id]<=d->m_signals.size()) {
        const JSignalInfo& signal = d->m_signals[d->m_property_notifies[_id]];
        if(signal.signalField && signal.emitMethodInfo.methodId){
            if(env->IsSameObject(object, nullptr)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::notifyProperty: Object is null");
                return -1;
            }
            if(!env->IsInstanceOf(object, d->m_clazz)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::notifyProperty: Object type mismatch (%s), expected: %s",
                         qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                         qPrintable(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.')));
                return -1;
            }
            jobject signal_object = env->GetObjectField(object, signal.signalField);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            Q_ASSERT(signal_object);
            d->invokeMethod(env, signal_object, signal.emitMethodInfo, _a);
        }
    }

    return _id - d->m_property_count;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
int QtJambiMetaObject::queryPropertyDesignable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyDesignable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_designable_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_designable_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtJambiMetaObject::queryPropertyScriptable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyScriptable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_scriptable_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_scriptable_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtJambiMetaObject::queryPropertyStored(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyStored(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_stored_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_stored_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtJambiMetaObject::queryPropertyUser(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyUser(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_user_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_user_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtJambiMetaObject::queryPropertyEditable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyEditable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_editable_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_editable_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}
#else
int QtJambiMetaObject::bindableProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->bindableProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        if(d->m_property_bindables[_id].methodId){
            d->invokeMethod(env, object, d->m_property_bindables[_id], _a);
        }else if (d->m_property_QProperty_fields[_id].propertyField){
            if(env->IsSameObject(object, nullptr)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::bindableProperty: Object is null");
                return -1;
            }
            if(!env->IsInstanceOf(object, d->m_clazz)){
                qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::bindableProperty: Object type mismatch (%s), expected: %s",
                         qPrintable(QtJambiAPI::getObjectClassName(env, object).replace('$', '.')),
                         qPrintable(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.')));
                return -1;
            }
            jobject qProperty = env->GetObjectField(object, d->m_property_QProperty_fields[_id].propertyField);
            jobject bindable = Java::QtCore::QBindable::createBinding(env, qProperty);
            *reinterpret_cast<QUntypedBindable*>(_a[0]) = qtjambi_cast<QUntypedBindable>(env, bindable);
        }
    }

    return _id - d->m_property_count;
}
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

jclass QtJambiMetaObject::typeOfProperty(int _id) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_property_count) {
        if(d->m_property_readers[_id].methodId){
            Q_ASSERT(d->m_property_readers[_id].parameterTypeInfos.size()>0);
            return d->m_property_readers[_id].parameterTypeInfos[0].javaClass();
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (d->m_property_QProperty_fields[_id].valueMethod.methodId) {
            Q_ASSERT(d->m_property_QProperty_fields[_id].valueMethod.parameterTypeInfos.size()>0);
            return d->m_property_QProperty_fields[_id].valueMethod.parameterTypeInfos[0].javaClass();
        }
#endif
        if (d->m_propertyMembers[_id].member) {
            return d->m_propertyMembers[_id].memberTypeInfo.javaClass();
        }
        if (d->m_property_writers[_id].methodId) {
            Q_ASSERT(d->m_property_writers[_id].parameterTypeInfos.size()>1);
            return d->m_property_writers[_id].parameterTypeInfos[1].javaClass();
        }
    }
    return nullptr;
}

InternalToExternalConverter QtJambiMetaObject::internalToExternalConverterOfProperty(int _id) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_property_count) {
        if(d->m_property_readers[_id].methodId){
            Q_ASSERT(d->m_property_readers[_id].parameterTypeInfos.size()>0);
            return d->m_property_readers[_id].parameterTypeInfos[0].m_internalToExternalConverter;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (d->m_property_QProperty_fields[_id].valueMethod.methodId) {
            Q_ASSERT(d->m_property_QProperty_fields[_id].valueMethod.parameterTypeInfos.size()>0);
            return d->m_property_QProperty_fields[_id].valueMethod.parameterTypeInfos[0].m_internalToExternalConverter;
        }
#endif
        if (d->m_propertyMembers[_id].member) {
            return d->m_propertyMembers[_id].memberTypeInfo.m_internalToExternalConverter;
        }
        if (d->m_property_writers[_id].methodId) {
            Q_ASSERT(d->m_property_writers[_id].parameterTypeInfos.size()>1);
            return d->m_property_writers[_id].parameterTypeInfos[1].m_internalToExternalConverter;
        }
    }
    return nullptr;
}

ExternalToInternalConverter QtJambiMetaObject::externalToInternalConverterOfProperty(int _id) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_property_count) {
        if(d->m_property_readers[_id].methodId){
            Q_ASSERT(d->m_property_readers[_id].parameterTypeInfos.size()>0);
            return d->m_property_readers[_id].parameterTypeInfos[0].m_externalToInternalConverter;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (d->m_property_QProperty_fields[_id].valueMethod.methodId) {
            Q_ASSERT(d->m_property_QProperty_fields[_id].valueMethod.parameterTypeInfos.size()>0);
            return d->m_property_QProperty_fields[_id].valueMethod.parameterTypeInfos[0].m_externalToInternalConverter;
        }
#endif
        if (d->m_propertyMembers[_id].member) {
            return d->m_propertyMembers[_id].memberTypeInfo.m_externalToInternalConverter;
        }
        if (d->m_property_writers[_id].methodId) {
            Q_ASSERT(d->m_property_writers[_id].parameterTypeInfos.size()>1);
            return d->m_property_writers[_id].parameterTypeInfos[1].m_externalToInternalConverter;
        }
    }
    return nullptr;
}

const QSharedPointer<const QtJambiMetaObject>& QtJambiMetaObject::thisPointer() const
{
    Q_D(const QtJambiMetaObject);
    return d->m_this_ptr;
}

QSharedPointer<const QtJambiMetaObject> QtJambiMetaObject::dispose(JNIEnv * env) const
{
    Q_D(const QtJambiMetaObject);
    QSharedPointer<const QtJambiMetaObject> pointer = d->m_this_ptr;
    d->m_this_ptr.clear();
    if(d->m_javaInstance){
        env->DeleteWeakGlobalRef(d->m_javaInstance);
        d->m_javaInstance = nullptr;
    }
    return pointer;
}

const QMetaObject *QtJambiMetaObjectPrivate::getQMetaObjectForJavaClass(JNIEnv *env, jclass object_class, const std::function<const QMetaObject *(bool&,bool&)>& original_meta_object_provider)
{
    Q_ASSERT(object_class != nullptr);
    Java::QtJambi::QtUtilities::initializePackage(env, object_class);

    // If original_meta_object is null then we have to look it up

    int classHash = Java::Runtime::Object::hashCode(env,object_class);
    {
        QReadLocker locker(gMetaObjectsLock());
        Q_UNUSED(locker)
        if(gMetaObjects->contains(classHash))
            return gMetaObjects->value(classHash, nullptr);
    }
    const QMetaObject *returned = nullptr;
    {
        // Return original meta object for generated classes, and
        // create a new dynamic meta object for subclasses
        QtJambiMetaObject* dynamicResult = nullptr;
        bool basedOnCustomMetaObject = false;
        bool isNamespace = false;
        if (Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(env, object_class)) {
            returned = original_meta_object_provider(basedOnCustomMetaObject, isNamespace);
            if(!returned && !basedOnCustomMetaObject && !isNamespace){
                JavaException exceptionHandler;
                dynamicResult = new QtJambiMetaObject(env, object_class);
                try{
                    dynamicResult->initialize(env, nullptr, false);
                }catch(const JavaException& exn){
                    (void)dynamicResult->dispose(env);
                    exn.raise();
                    throw;
                }
                returned = dynamicResult;
            }
        } else {
            const QMetaObject *original_meta_object = original_meta_object_provider(basedOnCustomMetaObject, isNamespace);
            if (isNamespace) {
                returned = original_meta_object;
            }else {
                dynamicResult = new QtJambiMetaObject(env, object_class);
                try{
                    dynamicResult->initialize(env, original_meta_object, basedOnCustomMetaObject);
                }catch(const JavaException& exn){
                    (void)dynamicResult->dispose(env);
                    exn.raise();
                    throw;
                }
                if(basedOnCustomMetaObject){
                    int signal_count = dynamicResult->d_ptr->m_signal_count>0;
                    (void)dynamicResult->dispose(env);
                    if(signal_count>0){
                        QString class_name = QtJambiAPI::getClassName(env, object_class).replace("$", ".");
                        Java::Runtime::UnsupportedOperationException::throwNew(env, QStringLiteral("Cannot define signals in class %1 because it extends type with dynamic meta object.").arg(class_name) QTJAMBI_STACKTRACEINFO );
                    }
                    returned = original_meta_object;
                }else{
                    returned = dynamicResult;
                }
            }
        }
        Q_ASSERT(returned || isNamespace);
        QWriteLocker locker(gMetaObjectsLock());
        Q_UNUSED(locker)
        // check if someone added a meta object meanwhile
        if(gMetaObjects->contains(classHash)){
            const QMetaObject *_returned = gMetaObjects->value(classHash, nullptr);
            if(dynamicResult && dynamicResult!=_returned){
                dynamicResult->dispose(env);
            }
            return _returned;
        }
        gMetaObjects->insert(classHash, returned);
    }
    return returned;
}

const QMetaObject *CoreAPI::metaObjectForClass(JNIEnv *env, jclass object_class, const QMetaObject *original_meta_object, bool _hasCustomMetaObject)
{
    return QtJambiMetaObjectPrivate::getQMetaObjectForJavaClass(env, object_class,
                                        [env, object_class, original_meta_object, _hasCustomMetaObject](bool& basedOnCustomMetaObject, bool& isNamespace) -> const QMetaObject * {
                                            if (original_meta_object == nullptr) {
                                                QString class_name = QtJambiAPI::getClassName(env, object_class).replace(".", "/");
                                                if((isNamespace = isJavaNameNamespace(class_name))){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                                    return registeredNamespaceMetaObject(class_name);
#else
                                                    if (class_name=="io/qt/core/Qt")
                                                        return qt_getQtMetaObject();
#endif
                                                } else {
                                                    Q_ASSERT(!class_name.isEmpty());
                                                    if(const std::type_info* typeId = getTypeByJavaName(class_name)){
                                                        const QMetaObject* superType = superTypeForCustomMetaObject(*typeId);
                                                        return (basedOnCustomMetaObject = superType) ? superType : registeredOriginalMetaObject(*typeId);
                                                    }else if(jclass _object_class = JavaAPI::resolveClosestQtSuperclass(env, object_class)){
                                                        class_name = QtJambiAPI::getClassName(env, _object_class).replace(".", "/");
                                                        Q_ASSERT(!class_name.isEmpty());
                                                        if(const std::type_info* typeId = getTypeByJavaName(class_name)){
                                                            const QMetaObject* superType = superTypeForCustomMetaObject(*typeId);
                                                            return (basedOnCustomMetaObject = superType) ? superType : registeredOriginalMetaObject(*typeId);
                                                        }
                                                    }
                                                }
                                            }
                                            basedOnCustomMetaObject = _hasCustomMetaObject;
                                            return original_meta_object;
                                        }
                                    );
}

const QMetaObject *QtJambiMetaObjectPrivate::getQMetaObjectForJavaClass(JNIEnv *env, jclass object_class, const std::type_info& typeId)
{
    return QtJambiMetaObjectPrivate::getQMetaObjectForJavaClass(env, object_class,
                                        [&typeId](bool& basedOnCustomMetaObject, bool&) -> const QMetaObject * {
                                            const QMetaObject* superType = superTypeForCustomMetaObject(typeId);
                                            return (basedOnCustomMetaObject = superType) ? superType : registeredOriginalMetaObject(typeId);
                                        }
                                    );
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

jmethodID searchForMethod(JNIEnv *env, jclass declaringClass, jmethodID method, jclass replacementClass){
    jobject reflectMethod = env->ToReflectedMethod(declaringClass, method, false);
    jstring name = Java::Runtime::Method::getName(env, reflectMethod);
    jobjectArray parameters = Java::Runtime::Executable::getParameterTypes(env, reflectMethod);
    jobject replacementMethod;
    try{
        replacementMethod = Java::Runtime::Class::getMethod(env, replacementClass, name, parameters);
    }catch(...){
        replacementMethod = Java::Runtime::Class::getDeclaredMethod(env, replacementClass, name, parameters);
    }
    return env->FromReflectedMethod(replacementMethod);
}

jfieldID searchForField(JNIEnv *env, jclass declaringClass, jfieldID field, jclass replacementClass){
    jobject reflectField = env->ToReflectedField(declaringClass, field, false);
    jstring name = Java::Runtime::Field::getName(env, reflectField);
    jobject replacementField = Java::Runtime::Class::getDeclaredField(env, replacementClass, name);
    return env->FromReflectedField(replacementField);
}

void searchForMethods(JNIEnv *env, jclass declaringClass, const QVector<JMethodInfo>& methods, jclass replacementClass, QHash<jmethodID,jmethodID>& methodMappings){
    for(const JMethodInfo& m : methods){
        if(!m.isStatic && !methodMappings.contains(m.methodId))
            methodMappings[m.methodId] = searchForMethod(env, declaringClass, m.methodId, replacementClass);
    }
}

void searchForFields(JNIEnv *env, jclass declaringClass, const QVector<JMemberInfo>& members, jclass replacementClass, QHash<jfieldID,jfieldID>& fieldMappings){
    for(const JMemberInfo& m : members){
        if(!m.isStatic && !fieldMappings.contains(m.member))
            fieldMappings[m.member] = searchForField(env, declaringClass, m.member, replacementClass);
    }
}

bool QmlAPI::registerQmlExtension(JNIEnv *env, const QMetaObject *extended_meta_object, jclass clazz){
    if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(extended_meta_object)){
        QtJambiMetaObjectPrivate* p = const_cast<QtJambiMetaObjectPrivate*>(QtJambiMetaObjectPrivate::get(mo));
        jmethodID wrapper = JavaAPI::resolveMethod(env, "wrap", qPrintable(QStringLiteral("(L%1;)L%2;").arg(QtJambiAPI::getClassName(env, clazz).replace('.', '/'), QtJambiAPI::getClassName(env, p->m_clazz).replace('.', '/'))), p->m_clazz, true);
        QHash<jmethodID,jmethodID> methodMappings;
        QHash<jfieldID,jfieldID> fieldMappings;
        if(!wrapper){
            searchForMethods(env, p->m_clazz, p->m_methods, clazz, methodMappings);
            searchForMethods(env, p->m_clazz, p->m_property_readers.values(), clazz, methodMappings);
            searchForMethods(env, p->m_clazz, p->m_property_writers.values(), clazz, methodMappings);
            searchForMethods(env, p->m_clazz, p->m_property_bindables.values(), clazz, methodMappings);
            searchForMethods(env, p->m_clazz, p->m_property_resetters.values(), clazz, methodMappings);
        }
        p->m_qmlExtensions[getGlobalClassRef(env, clazz)] = {wrapper, methodMappings, fieldMappings};
        return true;
    }else{
        return false;
    }
}
#endif

jobject QtJambiMetaObject::getSignalTypes(JNIEnv *env, jobject signal, const QMetaMethod& metaMethod){
    jobject result = nullptr;
    hash_type key = qHash(metaMethod);
    {
        QReadLocker locker(gJMethodInfoInfosLock());
        Q_UNUSED(locker)
        if(gSignalTypes->contains(key)){
            result = (*gSignalTypes)[key];
            if(result)
                return result;
        }
    }
    int count = metaMethod.isValid() ? metaMethod.parameterCount() : -1;
    jbooleanArray argumentIsPointerType = count > 0 ? env->NewBooleanArray(count) : nullptr;
    jbooleanArray argumentIsReferenceType = count > 0 ? env->NewBooleanArray(count) : nullptr;
    if(count>0){
        JBooleanArrayPointer isPointer(env, argumentIsPointerType, true);
        JBooleanArrayPointer isReference(env, argumentIsReferenceType, true);
        QList<QByteArray> parameterTypeNames = metaMethod.parameterTypes();
        for(int i=0; i<count; ++i){
            QByteArray& parameterTypeName = parameterTypeNames[i];
            if(parameterTypeName.endsWith("&")){
                isReference[i] = true;
            }else if(parameterTypeName.endsWith("*")){
                parameterTypeName.chop(1);
                parameterTypeName = parameterTypeName.trimmed();
                if(const std::type_info* typeId = getTypeByQtName(parameterTypeName)){
                    switch(getEntryType(*typeId)){
                    case EntryTypes::ValueTypeInfo:
                    case EntryTypes::InterfaceValueTypeInfo:
                        isPointer[i] = true;
                        break;
                    default: break;
                    }
                }
            }
        }
    }
    if(!signal){ // -> is extra signal
        const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, metaMethod);
        jobjectArray signalParameterTypes = env->NewObjectArray(jsize(parameterTypeInfos.size()-1), Java::Runtime::Class::getClass(env), nullptr);
        for (decltype(parameterTypeInfos.size()) i=1; i<parameterTypeInfos.size(); ++i){
            env->SetObjectArrayElement(signalParameterTypes, jsize(i-1), parameterTypeInfos[i].javaClass());
        }
        signal = signalParameterTypes;
    }
    result = Java::QtJambi::SignalUtility::resolveSignal(env, signal, argumentIsPointerType, argumentIsReferenceType);
    {
        QWriteLocker locker(gJMethodInfoInfosLock());
        Q_UNUSED(locker)
        if(gSignalTypes->contains(key)){
            result = (*gSignalTypes)[key];
            if(result)
                return result;
        }
        gSignalTypes->insert(key, env->NewGlobalRef(result));
    }
    return result;
}

void QtJambiMetaObjectPrivate::clearAtShutdown(JNIEnv * env){
    SignalTypesHash signalTypes;
    QList<const QList<ParameterTypeInfo>*> list;
    {
        QWriteLocker locker(gJMethodInfoInfosLock());
        Q_UNUSED(locker)
        if(!gParameterTypeInfos.isDestroyed()){
            list = gParameterTypeInfos->values();
            gParameterTypeInfos->clear();
        }
        if(!gSignalTypes.isDestroyed())
            signalTypes.swap(*gSignalTypes);
    }
    qDeleteAll(list);
    if(env){
        for(jobject obj : signalTypes){
            if(obj)
                env->DeleteGlobalRef(obj);
        }
    }
    QList<QSharedPointer<const QtJambiMetaObject>> metaObjects;
    MetaObjectHash metaObjectsHash;
    {
        QWriteLocker locker(gMetaObjectsLock());
        Q_UNUSED(locker)
        if(!gMetaObjects.isDestroyed())
            metaObjectsHash.swap(*gMetaObjects);
    }
    if(env){
        for(const QMetaObject* mo : metaObjectsHash){
            if(const QtJambiMetaObject* dynamo = QtJambiMetaObject::cast(mo)){
                // delete self-reference
                metaObjects << dynamo->dispose(env);
            }
        }
    }
    if(QThread *thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
        if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
            data->doAtThreadEnd([metaObjects](){/*delete captured list after function call*/});
        }
    }
}

void clearMetaObjectsAtShutdown(JNIEnv * env)
{
    QtJambiMetaObjectPrivate::clearAtShutdown(env);
}

const QMetaObject* QtJambiMetaObject::findMetaObject(const char* name){
    const QMetaObject* metaObject = nullptr;
    if(const std::type_info* typeId = getTypeByQtName(name)){
        metaObject = registeredOriginalMetaObject(*typeId);
    }
    if(!metaObject)
        metaObject = metaObjectsByName()->value(qHash(name), nullptr);
    return metaObject;
}

jobject QtJambiMetaObject::convertToJavaObject(JNIEnv *env, const QMetaObject *metaObject){
    if(const QtJambiMetaObject* dyMetaObject = QtJambiMetaObject::cast(metaObject)){
        jweak javaInstance = QtJambiMetaObject::javaInstance(dyMetaObject);
        if(env->IsSameObject(javaInstance, nullptr)){
            jobject jmetaObject = Java::QtCore::QMetaObject::newInstance(env, jlong(reinterpret_cast<const void*>(metaObject)));
            QtJambiMetaObject::setJavaInstance(dyMetaObject, env->NewWeakGlobalRef(jmetaObject));
            return jmetaObject;
        }else{
            return env->NewLocalRef(javaInstance);
        }
    }else{
        QRecursiveMutexLocker locker(javaMetaObjectsLock());
        Q_UNUSED(locker)
        jweak javaInstance = javaMetaObjects->value(metaObject, nullptr);
        if(env->IsSameObject(javaInstance, nullptr)){
            jobject jmetaObject = Java::QtCore::QMetaObject::newInstance(env, jlong(reinterpret_cast<const void*>(metaObject)));
            javaMetaObjects->insert(metaObject, env->NewWeakGlobalRef(jmetaObject));
            QLatin1String javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if(javaName.isEmpty()){
                if(const std::type_info* typeId = getTypeByQtName(metaObject->className()))
                    javaName = QLatin1String(getJavaName(*typeId));
            }
            if(javaName.isEmpty()){
                metaObjectsByName->insert(qHash(metaObject->className()), metaObject);
            }
            return jmetaObject;
        }else{
            return env->NewLocalRef(javaInstance);
        }
    }
}

void QtJambiMetaObject::resolveSignals(JNIEnv *env, jobject java_object, const QMetaObject* metaObject, JavaException& ocurredException){
    if(!ocurredException.object()){
        try{
            QMap<int,QMetaMethod> metaMethods;
            {
                const int methodCount = metaObject->methodCount();
                for(int i=0; i<methodCount; i++){
                    QMetaMethod method = metaObject->method(i);
                    if(method.methodType()==QMetaMethod::Signal)
                        metaMethods.insert(method.methodIndex(), method);
                }
            }
            do{
                if(const QVector<const SignalMetaInfo>* infos = signalMetaInfos(metaObject)){
                    QList<QString> signalOrder;
                    QMap<QString,QList<const SignalMetaInfo*> > sortedSignals;
                    for(const SignalMetaInfo& info : *infos){
                        QString key = QLatin1String(info.signal_name);
                        if(!signalOrder.contains(key))
                            signalOrder.append(key);
                        sortedSignals[key].append(&info);
                    }
                    for(const QString& key : signalOrder){
                        const QList<const SignalMetaInfo*>& signalsByName = sortedSignals[key];
                        if(signalsByName.size()==1){
                            const SignalMetaInfo& info = *signalsByName.first();
                            int signal_method_index = -1;
                            if(info.signal_method_index_provider){
                                signal_method_index = info.signal_method_index_provider();
                            }
                            if(signal_method_index>=0){
                                jclass clazz(nullptr);
                                if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                                    if(const char* javaClass = getJavaName(*typeId))
                                        clazz = JavaAPI::resolveClass(env, javaClass);
                                }
                                if(!clazz){
                                    clazz = Java::QtJambi::ClassAnalyzerUtility::findGeneratedSuperclass(env, env->GetObjectClass(java_object));
                                }
                                Q_ASSERT_X(clazz, "QtJambiMetaObject::resolveSignals", qPrintable(QString("unable to find class").arg(metaObject->className())));

                                jthrowable exceptionOccurred = nullptr;
                                jfieldID fieldId = resolveField(env, info.signal_name, info.signal_signature, clazz, false, &exceptionOccurred);
                                if(exceptionOccurred)
                                    JavaException(env, exceptionOccurred).raise();
                                Q_ASSERT_X(fieldId, "QtJambiMetaObject::resolveSignals", qPrintable(QString("unable to find signal field %1").arg(info.signal_name)));

                                jobject signal = env->GetObjectField(java_object, fieldId);
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                Q_ASSERT_X(signal, "QtJambiMetaObject::resolveSignals", qPrintable(QString("unable to fetch signal field %1").arg(info.signal_name)));

                                if (signal) {  // everything went well
                                    QMetaMethod metaMethod = metaMethods.take(signal_method_index);
                                    jobject signalField = env->ToReflectedField(clazz, fieldId, false);
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    jobject signalTypes = QtJambiMetaObject::getSignalTypes(env, signalField, metaMethod);
                                    Java::QtJambi::SignalUtility$AbstractSignal::initializeSignal(env, signal, clazz, signalTypes, signal_method_index, jlong(metaMethod.enclosingMetaObject()));
                                }
                                env->DeleteLocalRef(signal);
                            }
                        }else if(signalsByName.size()>1){
                            const SignalMetaInfo& first = *signalsByName.first();
                            jclass clazz(nullptr);
                            if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                                if(const char* javaClass = getJavaName(*typeId))
                                    clazz = JavaAPI::resolveClass(env, javaClass);
                            }
                            if(!clazz){
                                clazz = Java::QtJambi::ClassAnalyzerUtility::findGeneratedSuperclass(env, env->GetObjectClass(java_object));
                            }
                            Q_ASSERT_X(clazz, "QtJambiMetaObject::resolveSignals", qPrintable(QString("unable to find class").arg(metaObject->className())));

                            jthrowable exceptionOccurred = nullptr;
                            jfieldID fieldId = resolveField(env, first.signal_name, qPrintable(QString("L%1$MultiSignal_%2;").arg(QtJambiAPI::getClassName(env, clazz).replace('.', '/')).arg(first.signal_name)), clazz, false, &exceptionOccurred);
                            if(exceptionOccurred)
                                JavaException(env, exceptionOccurred).raise();
                            if(!fieldId)
                                fieldId = resolveField(env, first.signal_name, "Lio/qt/core/QObject$MultiSignal;", clazz, false, &exceptionOccurred);
                            if(exceptionOccurred)
                                JavaException(env, exceptionOccurred).raise();
                            Q_ASSERT_X(fieldId, "QtJambiMetaObject::resolveSignals", qPrintable(QString("unable to find signal field %1").arg(first.signal_name)));

                            jobject reflectedField = env->ToReflectedField(clazz, fieldId, false);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );

                            jobject multiSignal = env->GetObjectField(java_object, fieldId);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            Q_ASSERT_X(multiSignal, "QtJambiMetaObject::resolveSignals", qPrintable(QString("unable to fetch signal field %1").arg(first.signal_name)));

                            jintArray methodIndexes = env->NewIntArray(jsize(signalsByName.size()));
                            jlongArray metaObjects = env->NewLongArray(jsize(signalsByName.size()));
                            jobjectArray signalParameterTypes = env->NewObjectArray(jsize(signalsByName.size()), Java::Runtime::List::getClass(env), nullptr);
                            jobjectArray signalObjectTypes = env->NewObjectArray(jsize(signalsByName.size()), Java::Runtime::Class::getClass(env), nullptr);
                            if(signalsByName.size()>0){
                                JIntArrayPointer methodIndexesPtr(env, methodIndexes, true);
                                JLongArrayPointer metaObjectsPtr(env, metaObjects, true);
                                for(int i=0; i<signalsByName.size(); i++){
                                    const SignalMetaInfo& info = *signalsByName.at(i);
                                    int signal_method_index = -1;
                                    if(info.signal_method_index_provider){
                                        signal_method_index = info.signal_method_index_provider();
                                    }
                                    QString signalSignature = QLatin1String(info.signal_signature);
                                    if(signalSignature.startsWith('L') && signalSignature.endsWith(';'))
                                        signalSignature = signalSignature.mid(1, signalSignature.length()-2);
                                    env->SetObjectArrayElement(signalObjectTypes, jsize(i), JavaAPI::resolveClass(env, qPrintable(signalSignature), nullptr));
                                    QMetaMethod metaMethod = metaMethods.take(signal_method_index);
                                    const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, metaMethod);
                                    jobjectArray signalParameterClasses = env->NewObjectArray(jsize(parameterTypeInfos.size()-1), Java::Runtime::Class::getClass(env), nullptr);
                                    for (decltype(parameterTypeInfos.size()) i=1; i<parameterTypeInfos.size(); ++i){
                                        env->SetObjectArrayElement(signalParameterClasses, jsize(i-1), parameterTypeInfos[i].javaClass());
                                    }
                                    env->SetObjectArrayElement(signalParameterTypes, jsize(i), QtJambiMetaObject::getSignalTypes(env, Java::QtCore::QPair::newInstance(env, multiSignal, signalParameterClasses), metaMethod));
                                    methodIndexesPtr.pointer()[i] = signal_method_index;
                                    metaObjectsPtr.pointer()[i] = jlong(metaMethod.enclosingMetaObject());
                                }
                            }
                            Java::QtJambi::SignalUtility$AbstractMultiSignal::initializeSignals(env, multiSignal, reflectedField, methodIndexes, metaObjects, signalParameterTypes, signalObjectTypes);
                            env->DeleteLocalRef(reflectedField);
                        }else{
                            Q_ASSERT_X(false, "QtJambiMetaObject::resolveSignals", "should never reach this");
                        }
                    }
                }
                metaObject = metaObject->superClass();
            }while(metaObject);

            if(!metaMethods.isEmpty()){
                for(QMap<int,QMetaMethod>::const_iterator i = metaMethods.cbegin(); i!=metaMethods.cend(); ++i){
                    const QMetaMethod& metaMethod = i.value();
                    metaObject = metaMethod.enclosingMetaObject();
                    if (const QtJambiMetaObject* mo = QtJambiMetaObject::cast(metaObject)){
                        jclass java_class = mo->javaClass();
                        jobject signalField = Java::Runtime::Class::getDeclaredField(env, java_class, env->NewStringUTF(metaMethod.name().data()));
                        if(signalField){
                            jfieldID signalFieldId = env->FromReflectedField(signalField);
                            jobject signalTypes = mo->signalTypes(metaMethod.methodIndex()-metaObject->methodOffset());
                            jobject signal = env->GetObjectField(java_object, signalFieldId);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            if(signal){
                                if(!signalTypes){
                                    signalTypes = QtJambiMetaObject::getSignalTypes(env, signalField, metaMethod);
                                }
                                Java::QtJambi::SignalUtility$AbstractSignal::initializeSignal(env, signal, signalTypes, metaMethod.methodIndex(), jlong(mo));
                            }
                        }
                    }
                }
            }
        }catch(const JavaException& exn){
            ocurredException.addSuppressed(env, exn);
        }
    }
}

JObjectWrapper QtJambiMetaObject::resolveExtraSignal(JNIEnv *env, jobject java_object, const QMetaMethod& method){
    QSet<const QMetaObject*> convertedMetaObjects;
    jobject signalObject = nullptr;
    switch(method.parameterCount()){
    case 0:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal0::newInstance(env, java_object);
        break;
    case 1:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal1::newInstance(env, java_object);
        break;
    case 2:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal2::newInstance(env, java_object);
        break;
    case 3:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal3::newInstance(env, java_object);
        break;
    case 4:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal4::newInstance(env, java_object);
        break;
    case 5:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5::newInstance(env, java_object);
        break;
    case 6:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6::newInstance(env, java_object);
        break;
    case 7:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7::newInstance(env, java_object);
        break;
    case 8:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8::newInstance(env, java_object);
        break;
    case 9:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9::newInstance(env, java_object);
        break;
    default:
        break;
    }
    if(signalObject){
        {
            const QMetaObject* parentMetaObject = method.enclosingMetaObject();
            while(parentMetaObject && !convertedMetaObjects.contains(parentMetaObject)){
                qtjambi_cast<jobject>(env, parentMetaObject);
                convertedMetaObjects.insert(parentMetaObject);
                parentMetaObject = parentMetaObject->superClass();
            }
        }
        jobject signalTypes = QtJambiMetaObject::getSignalTypes(env, nullptr, method);
        Java::QtJambi::SignalUtility$AbstractSignal::initializeSignal(env, signalObject, env->GetObjectClass(java_object), signalTypes, method.methodIndex(), jlong(method.enclosingMetaObject()));
        return JObjectWrapper(env, signalObject, true);
    }else{
        return JObjectWrapper();
    }
}

