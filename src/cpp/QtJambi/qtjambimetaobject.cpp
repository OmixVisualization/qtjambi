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

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#define QTJAMBI_INTERNAL_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, methodname, __FILE__, __LINE__, Q_FUNC_INFO);


Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gMetaObjectsLock, (QReadWriteLock::Recursive));
typedef QHash<const QMetaObject *, jweak> JavaMetaObjectHash;
Q_GLOBAL_STATIC(JavaMetaObjectHash, javaMetaObjects);


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
    inline operator bool()const{return methodId!=nullptr;}
    inline bool operator!()const{return methodId==nullptr;}
    QString methodString(JNIEnv* env) const;
private:
    mutable QString m_methodString;
};

QString JMethodInfo::methodString(JNIEnv* env) const{
    if(m_methodString.isEmpty() && declaringClass && methodId){
        jobject method = env->ToReflectedMethod(declaringClass, methodId, isStatic);
        if(method){
            jstring strg = Java::Runtime::Object::toString(env, method);
            m_methodString = qtjambi_cast<QString>(env, strg);
        }
    }
    return m_methodString;
}

typedef QPair<jfieldID, JMethodInfo > FieldPair;

struct JMemberInfo{
    jfieldID member = nullptr;
    jValueType type = jValueType::l;
    ParameterTypeInfo memberTypeInfo;
    bool canWrite = false;
    bool isStatic = false;
    jclass declaringClass = nullptr;
    QString memberString;
    inline operator bool()const{return member!=nullptr;}
    inline bool operator!()const{return member==nullptr;}
};

struct JSignalInfo{
    jfieldID signalField = nullptr;
    jobject signalTypes = nullptr;
    JMethodInfo emitMethodInfo;
    bool isClone = false;
    inline operator bool()const{return signalField!=nullptr;}
    inline bool operator!()const{return signalField==nullptr;}
};

struct QPropertyInfo{
    jfieldID propertyField = nullptr;
    JMethodInfo valueMethod;
    JMethodInfo setValueMethod;
    inline operator bool()const{return propertyField!=nullptr;}
    inline bool operator!()const{return propertyField==nullptr;}
};

typedef QHash<hash_type, jobject> SignalTypesHash;
typedef QHash<hash_type, const QList<ParameterTypeInfo>*> ParameterTypeHash;
Q_GLOBAL_STATIC(ParameterTypeHash, gParameterTypeInfos)
typedef QHash<int, const QMetaObject *> MetaObjectHash;
Q_GLOBAL_STATIC(MetaObjectHash, gMetaObjects);
Q_GLOBAL_STATIC(SignalTypesHash, gSignalTypes);

QtJambiUtils::InternalToExternalConverter ParameterTypeInfo::default_internalToExternalConverter()
{
    return QtJambiUtils::InternalToExternalConverter([](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{ return false; });
}

QtJambiUtils::ExternalToInternalConverter ParameterTypeInfo::default_externalToInternalConverter()
{
    return QtJambiUtils::ExternalToInternalConverter([](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) ->bool { return false; });
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
        QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
        QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
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
        QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
        QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
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
    jobject javaInstance(JNIEnv *env) const;
    void setJavaInstance(JNIEnv *env, jobject instance);
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
    QScopedArrayPointer<uint> m_intData;
    QtJambiMetaObject *q_ptr;
    mutable QSharedPointer<const QtJambiMetaObject> m_this_ptr;
    Q_DECLARE_PUBLIC(QtJambiMetaObject)

    jclass const m_clazz;
    QVector<JMethodInfo> m_methods;
    QHash<jmethodID,int> m_methodIndexes;
    QVector<JSignalInfo> m_signals;
    QHash<jfieldID,QHash<jmethodID,int>> m_signalIndexes;
    QVector<JMethodInfo> m_constructors;

    struct Property{
        JMethodInfo m_reader;
        JMethodInfo m_writer;
        JMethodInfo m_resetter;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        JMethodInfo m_designable_resolver;
        JMethodInfo m_scriptable_resolver;
        JMethodInfo m_editable_resolver;
        JMethodInfo m_stored_resolver;
        JMethodInfo m_user_resolver;
#else
        JMethodInfo m_bindable;
        QPropertyInfo m_QProperty_field;
#endif
        JMemberInfo m_member;
        inline JMethodInfo& reader(){return m_reader;}
        inline JMethodInfo& writer(){return m_writer;}
        inline JMethodInfo& resetter(){return m_resetter;}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        inline JMethodInfo& designable_resolver(){return m_designable_resolver;}
        inline JMethodInfo& scriptable_resolver(){return m_scriptable_resolver;}
        inline JMethodInfo& editable_resolver(){return m_editable_resolver;}
        inline JMethodInfo& stored_resolver(){return m_stored_resolver;}
        inline JMethodInfo& user_resolver(){return m_user_resolver;}
#else
        inline JMethodInfo& bindable(){return m_bindable;}
#endif
        int m_notifier = -1;
    };

    QVector<Property> m_properties;
    template<JMethodInfo&(Property::*member)()>
    struct PropertyCollector{
        QVector<Property>& m_properties;
        PropertyCollector(QVector<Property>& properties) : m_properties(properties){}
        JMethodInfo& operator[](int i){
            Property& property = m_properties[i];
            return (property.*member)();
        }
    };

    mutable jweak m_javaInstance;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QHash<jclass,QmlExtensionData> m_qmlExtensions;
    friend bool QmlAPI::registerQmlExtension(JNIEnv *env, const QMetaObject *extended_meta_object, jclass resolvedClass);
    friend void searchForMethods(JNIEnv *env, jclass declaringClass, const QVector<QtJambiMetaObjectPrivate::Property>& properties, jclass replacementClass, QHash<jmethodID,jmethodID>& methodMappings);
#endif
};

QtJambiMetaObjectPrivate::QtJambiMetaObjectPrivate(QtJambiMetaObject *q, JNIEnv *env, jclass java_class)
    :
      q_ptr(q), m_this_ptr(q),
      m_clazz(getGlobalClassRef(env, java_class)),
      m_methods(), m_methodIndexes(), m_signals(), m_signalIndexes(), m_constructors(), m_properties(),
      m_javaInstance(nullptr)
{
    Q_ASSERT(env);
    Q_ASSERT(java_class);
}

QtJambiMetaObjectPrivate::~QtJambiMetaObjectPrivate()
{
    QTJAMBI_INTERNAL_METHOD_CALL("QtJambiMetaObjectPrivate::~QtJambiMetaObjectPrivate()")
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
    }else{
        if (DefaultJniEnvironment env{100}) {
            for(JSignalInfo& sigInfo : m_signals){
                if(!env->IsSameObject(sigInfo.signalTypes, nullptr)){
                    env->DeleteGlobalRef(sigInfo.signalTypes);
                    sigInfo.signalTypes = nullptr;
                }
            }
        }
    }

    m_methods.clear();
    m_methodIndexes.clear();
    m_signals.clear();
    m_signalIndexes.clear();
    m_constructors.clear();
    m_properties.clear();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    for(char* strg : qAsConst(m_stringdataList)){
        delete[] strg;
    }
    m_stringdataList.clear();
#else
    m_stringData.clear();
    m_qmlExtensions.clear();
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if(m_intData){
        for(size_t i=0; i<14; ++i){
            m_intData[i] = 0;
        }
        m_intData.reset();
    }
}

QtJambiMetaObjectPrivate* QtJambiMetaObjectPrivate::get(QtJambiMetaObject* obj){
    return obj->d_ptr;
}

const QtJambiMetaObjectPrivate* QtJambiMetaObjectPrivate::get(const QtJambiMetaObject* obj){
    return obj->d_ptr;
}

template<class MethodInfoContainer>
void analyze_methods(JNIEnv *env, jobject classLoader, int count, jobject methodList, jobject listOfMetaTypes, MethodInfoContainer methodInfoContainer, QHash<jmethodID,int>* methodIndexes = nullptr){
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
                QtJambiUtils::InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::tryGetInternalToExternalConverter(
                                                                env,
                                                                typeName,
                                                                qMetaType,
                                                                javaClass,
                                                                true);
                QtJambiUtils::ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::tryGetExternalToInternalConverter(
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
    registerSwitchTableFields(env, Java::QtJambi::MetaObjectData::switchTableFields(env,meta_data_struct));
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
    m_intData.reset(new uint[size_t(intData_len)]);
    env->GetIntArrayRegion(Java::QtJambi::MetaObjectData$IntArray::array(env, intData), 0, intData_len, reinterpret_cast<jint*>(m_intData.data()));
    q->d.data = m_intData.get();
    q->d.extradata = nullptr;
    q->d.relatedMetaObjects = nullptr;

    {
        jobject methods = Java::QtJambi::MetaObjectData::methods(env,meta_data_struct);
        jobject methodMetaTypes = Java::QtJambi::MetaObjectData::methodMetaTypes(env,meta_data_struct);
        int method_count = QtJambiAPI::sizeOfJavaCollection(env, methods);
        m_methods.resize(method_count);
        analyze_methods<QVector<JMethodInfo>&>(env, classLoader, method_count, methods, methodMetaTypes, m_methods, &m_methodIndexes);
    }

    {
        jobject signalInfos = Java::QtJambi::MetaObjectData::signalInfos(env,meta_data_struct);
        int signal_count = QtJambiAPI::sizeOfJavaCollection(env, signalInfos);
        m_signals.resize(signal_count);
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + signal_count);
        for(int i=0; i<signal_count; ++i){
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
        int constructor_count = QtJambiAPI::sizeOfJavaCollection(env, methods);
        m_constructors.resize(constructor_count);
        analyze_methods<QVector<JMethodInfo>&>(env, classLoader, constructor_count, methods, constructorMetaTypes, m_constructors);
    }

    {
        int property_count = 0;
        jobject property_readers = Java::QtJambi::MetaObjectData::propertyReaders(env,meta_data_struct);
        jobject propertyMetaTypes = Java::QtJambi::MetaObjectData::propertyMetaTypes(env,meta_data_struct);
        jobject propertyClassTypes = Java::QtJambi::MetaObjectData::propertyClassTypes(env,meta_data_struct);
        property_count = QtJambiAPI::sizeOfJavaCollection(env, property_readers);
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + property_count);
        jobject propertyNotifies = Java::QtJambi::MetaObjectData::propertyNotifies(env,meta_data_struct);
        Q_ASSERT(property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyNotifies));
        m_properties.resize(property_count);
        for(int i=0; i<property_count; ++i){
            jobject fieldIndex = Java::Runtime::List::get(env, propertyNotifies, i);
            if(fieldIndex){
                jint idx = QtJambiAPI::fromJavaIntegerObject(env, fieldIndex);
                m_properties[i].m_notifier = idx;
            }
        }
        analyze_methods<PropertyCollector<&Property::reader>>(env, classLoader, property_count, property_readers, propertyMetaTypes, m_properties);
        analyze_methods<PropertyCollector<&Property::writer>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyWriters(env,meta_data_struct),
                        propertyMetaTypes, m_properties);
        analyze_methods<PropertyCollector<&Property::resetter>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyResetters(env,meta_data_struct),
                        nullptr, m_properties);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        analyze_methods<PropertyCollector<&Property::designable_resolver>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyDesignableResolvers(env,meta_data_struct),
                        nullptr, m_properties);
        analyze_methods<PropertyCollector<&Property::scriptable_resolver>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyScriptableResolvers(env,meta_data_struct),
                        nullptr, m_properties);
        analyze_methods<PropertyCollector<&Property::editable_resolver>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyEditableResolvers(env,meta_data_struct),
                        nullptr, m_properties);
        analyze_methods<PropertyCollector<&Property::user_resolver>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyUserResolvers(env,meta_data_struct),
                        nullptr, m_properties);
        analyze_methods<PropertyCollector<&Property::stored_resolver>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyStoredResolvers(env,meta_data_struct),
                        nullptr, m_properties);
#else
        analyze_methods<PropertyCollector<&Property::bindable>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyBindables(env,meta_data_struct),
                        nullptr, m_properties);
        jobject propertyQPropertyFields = Java::QtJambi::MetaObjectData::propertyQPropertyFields(env,meta_data_struct);
        Q_ASSERT(property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyQPropertyFields));
        for(int i=0; i<property_count; ++i){
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
                QPropertyInfo& info = m_properties[i].m_QProperty_field;
                info.propertyField = env->FromReflectedField(fieldObject);
                info.valueMethod.parameterTypeInfos << propertyTypeInfo;
                jclass propertyClassType = Java::Runtime::Field::getType(env, fieldObject);
                if(Java::QtCore::QObject$QAbstractComputedProperty::isAssignableFrom(env, propertyClassType)){
                    if(Java::QtCore::QObject$QComputedBooleanProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::z;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedBooleanProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedBooleanProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedByteProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::b;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedByteProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedByteProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedShortProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::s;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedShortProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedShortProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedIntProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::i;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedIntProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedIntProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedLongProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::j;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedLongProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedLongProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedFloatProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::f;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedFloatProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedFloatProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedDoubleProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::d;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedDoubleProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedDoubleProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedCharProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::c;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedCharProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedCharProperty::getClass(env));
                    }else if(Java::QtCore::QObject$QComputedProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::l;
                        info.valueMethod.methodId = Java::QtCore::QObject$QComputedProperty::value_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QComputedProperty::getClass(env));
                    }
                }else{
                    info.setValueMethod.parameterTypeInfos << ParameterTypeInfo::voidTypeInfo(env) << propertyTypeInfo;
                    info.setValueMethod.methodType = JMethodType::v;
                    if(Java::QtCore::QObject$QBooleanProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::z;
                        info.valueMethod.methodId = Java::QtCore::QObject$QBooleanProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QBooleanProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QBooleanProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QByteProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::b;
                        info.valueMethod.methodId = Java::QtCore::QObject$QByteProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QByteProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QByteProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QShortProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::s;
                        info.valueMethod.methodId = Java::QtCore::QObject$QShortProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QShortProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QShortProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QIntProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::i;
                        info.valueMethod.methodId = Java::QtCore::QObject$QIntProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QIntProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QIntProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QLongProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::j;
                        info.valueMethod.methodId = Java::QtCore::QObject$QLongProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QLongProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QLongProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QFloatProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::f;
                        info.valueMethod.methodId = Java::QtCore::QObject$QFloatProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QFloatProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QFloatProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QDoubleProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::d;
                        info.valueMethod.methodId = Java::QtCore::QObject$QDoubleProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QDoubleProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QDoubleProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QCharProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::c;
                        info.valueMethod.methodId = Java::QtCore::QObject$QCharProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QCharProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QCharProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }else if(Java::QtCore::QObject$QProperty::isAssignableFrom(env, propertyClassType)){
                        info.valueMethod.methodType = JMethodType::l;
                        info.valueMethod.methodId = Java::QtCore::QObject$QProperty::value_ID(env);
                        info.setValueMethod.methodId = Java::QtCore::QObject$QProperty::setValue_ID(env);
                        info.valueMethod.declaringClass = getGlobalClassRef(env, Java::QtCore::QObject$QProperty::getClass(env));
                        info.setValueMethod.declaringClass = info.valueMethod.declaringClass;
                    }
                }
            }
        }
#endif

        jobject propertyMemberFields = Java::QtJambi::MetaObjectData::propertyMemberFields(env,meta_data_struct);
        Q_ASSERT(property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyMemberFields));
        for(int i=0; i<property_count; ++i){
            jobject fieldObject = Java::Runtime::List::get(env, propertyMemberFields, i);
            if(fieldObject){
                JObjectArrayWrapper metaTypes(env, jobjectArray(Java::Runtime::List::get(env, propertyMetaTypes, i)));
                jobject mt = metaTypes.at(env, 0);
                QMetaType qMetaType(Java::QtJambi::MetaObjectData$MetaTypeInfo::metaTypeId(env, mt));
                jstring jtypeName = Java::QtJambi::MetaObjectData$MetaTypeInfo::typeName(env, mt);
                QString typeName = jtypeName ? qtjambi_cast<QString>(env, jtypeName) : QLatin1String(qMetaType.name());
                jclass javaClass = jclass(Java::Runtime::List::get(env, propertyClassTypes, i));
                jclass fieldType = Java::Runtime::Field::getType(env, fieldObject);
                JMemberInfo& member = m_properties[i].m_member;
                member.member = env->FromReflectedField(fieldObject);
                int modifier = Java::Runtime::Field::getModifiers(env, fieldObject);
                member.canWrite = !Java::Runtime::Modifier::isFinal(env, modifier);
                member.isStatic = Java::Runtime::Modifier::isStatic(env, modifier);
                member.declaringClass = Java::Runtime::Field::getDeclaringClass(env, fieldObject);
                member.declaringClass = getGlobalClassRef(env, member.declaringClass);
                if(Java::Runtime::Boolean::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::z;
                }else if(Java::Runtime::Byte::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::b;
                }else if(Java::Runtime::Short::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::s;
                }else if(Java::Runtime::Integer::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::i;
                }else if(Java::Runtime::Long::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::j;
                }else if(Java::Runtime::Float::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::f;
                }else if(Java::Runtime::Double::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::d;
                }else if(Java::Runtime::Character::isPrimitiveType(env, fieldType)){
                    member.type = jValueType::c;
                }else{
                    member.type = jValueType::l;
                    member.memberTypeInfo = ParameterTypeInfo{qMetaType.id(),
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
    QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL("QtJambiMetaObjectPrivate::invokeMethod(JNIEnv *, jobject, const JMethodInfo&, void **, bool) const", this)
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
                QTJAMBI_INTERNAL_METHOD_CALL_PRINT([&](QDebug& dbg){
                    dbg << "JNIEnv::CallStaticMethod(" << methodInfo.methodString(env) << ")";
                })
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
                QTJAMBI_INTERNAL_METHOD_CALL_PRINT([&](QDebug& dbg){
                    dbg << "JNIEnv::CallMethod(" << methodInfo.methodString(env) << ")";
                })
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

jweak QtJambiMetaObjectPrivate::javaInstance(JNIEnv *env) const{
    return env->NewLocalRef(m_javaInstance);
}

void QtJambiMetaObjectPrivate::setJavaInstance(JNIEnv *env, jobject instance){
    m_javaInstance = env->NewWeakGlobalRef(instance);
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
    return d_ptr->m_clazz;
}

bool QtJambiMetaObject::hasSignals() const{
    return !d_ptr->m_signals.isEmpty();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
jfieldID QtJambiMetaObject::getQPropertyField(int index) const{
    QtJambiMetaObjectPrivate::Property& property = d_ptr->m_properties[index];
    if(property.m_QProperty_field)
        return property.m_QProperty_field.propertyField;
    return nullptr;
}

void QtJambiMetaObject::registerQPropertyField(int index, jfieldID field){
    QtJambiMetaObjectPrivate::Property& property = d_ptr->m_properties[index];
    if(property.m_QProperty_field && !property.m_QProperty_field.propertyField)
        property.m_QProperty_field.propertyField = field;
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

jclass QtJambiMetaObject::javaClass(JNIEnv * env, const QMetaObject* metaObject, bool exactOrNull){
    if(!metaObject){
        return nullptr;
    }else if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return dynamicMetaObject->javaClass();
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
        QReadLocker locker(gMetaObjectsLock());
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
                    QtJambiUtils::InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::tryGetInternalToExternalConverter(
                                                                    env,
                                                                    QLatin1String(qTypeName),
                                                                    _metaType,
                                                                    javaClass,
                                                                    true);
                    QtJambiUtils::ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::tryGetExternalToInternalConverter(
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
                    QtJambiUtils::InternalToExternalConverter internalToExternalConverter;
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
        QWriteLocker locker(gMetaObjectsLock());
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
            index -= m_signals.size();
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
        index += m_signals.size() + q->methodOffset();
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
    if (_id < d->m_signals.size()) {
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
    } else if (_id < d->m_signals.size() + d->m_methods.size()) { // Call the correct method
        d->invokeMethod(env, object, d->m_methods[_id - d->m_signals.size()], _a);
    } else if (_id < d->m_signals.size() + d->m_methods.size() + d->m_constructors.size()) { // Call the correct constructor
        d->invokeConstructor(env, d->m_constructors[_id - d->m_signals.size() - d->m_methods.size()], _a);
    }
    JavaException::check(env QTJAMBI_STACKTRACEINFO );

    return _id - d->m_methods.size() - d->m_signals.size() - d->m_constructors.size();
}

int QtJambiMetaObject::invokeConstructor(JNIEnv *env, int _id, void **_a) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_signals.size() + d->m_methods.size() + d->m_constructors.size()) { // Call the correct constructor
        d->invokeConstructor(env, d->m_constructors[_id], _a);
    }
    return _id - d->m_constructors.size();
}

int QtJambiMetaObject::readProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->readProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_reader){
            d->invokeMethod(env, object, property.m_reader, _a);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        }else if (property.m_QProperty_field && property.m_QProperty_field.propertyField && property.m_QProperty_field.valueMethod.methodId){
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
            jobject qProperty = env->GetObjectField(object, property.m_QProperty_field.propertyField);
            d->invokeMethod(env, qProperty, property.m_QProperty_field.valueMethod, _a);
#endif
        }else if (property.m_member){
            QtJambiExceptionHandler __exceptionHandler;
            try{
                if(property.m_member.isStatic){
                    switch(property.m_member.type){
                    case jValueType::z:
                        *reinterpret_cast<jboolean*>(_a[0]) = env->GetStaticBooleanField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::b:
                        *reinterpret_cast<jbyte*>(_a[0]) = env->GetStaticByteField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::s:
                        *reinterpret_cast<jshort*>(_a[0]) = env->GetStaticShortField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::i:
                        *reinterpret_cast<jint*>(_a[0]) = env->GetStaticIntField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::j:
                        *reinterpret_cast<jlong*>(_a[0]) = env->GetStaticLongField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::f:
                        *reinterpret_cast<float*>(_a[0]) = env->GetStaticFloatField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::d:
                        *reinterpret_cast<double*>(_a[0]) = env->GetStaticDoubleField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::c:
                        *reinterpret_cast<jchar*>(_a[0]) = env->GetStaticCharField(property.m_member.declaringClass, property.m_member.member);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::l:{
                            jvalue value;
                            value.l = env->GetStaticObjectField(property.m_member.declaringClass, property.m_member.member);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            QtJambiScope scope;
                            property.m_member.memberTypeInfo.convertExternalToInternal(env, &scope, value, _a[0], jValueType::l);
                        }
                        break;
                    }
                }else{
                    jfieldID fieldId = property.m_member.member;
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
                            return _id - d->m_properties.size();
                        }
                    }

                    switch(property.m_member.type){
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
                            property.m_member.memberTypeInfo.convertExternalToInternal(env, &scope, value, _a[0], jValueType::l);
                        }
                        break;
                    }
                }
            }catch(const JavaException& exn){
                __exceptionHandler.handle(env, exn, nullptr);
            }
        }
    }
    return _id - d->m_properties.size();
}

int QtJambiMetaObject::writeProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->writeProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_writer){
            // invokeMethod expects a place holder for return value, but write property meta calls
            // do not since all property writers return void by convention.
            void *a[2] = { nullptr, _a[0] };
            d->invokeMethod(env, object, property.m_writer, a);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        }else if (property.m_QProperty_field && property.m_QProperty_field.propertyField && property.m_QProperty_field.setValueMethod.methodId){
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
            jobject qProperty = env->GetObjectField(object, property.m_QProperty_field.propertyField);
            void *a[2] = { nullptr, _a[0] };
            d->invokeMethod(env, qProperty, property.m_QProperty_field.setValueMethod, a, property.m_QProperty_field.valueMethod.methodType==JMethodType::l);
#endif
        }else if (property.m_member && property.m_member.member && property.m_member.canWrite){
            QtJambiExceptionHandler __exceptionHandler;
            try{
                if(property.m_member.isStatic){
                    switch(property.m_member.type){
                    case jValueType::z:
                        env->SetStaticBooleanField(property.m_member.declaringClass, property.m_member.member, *reinterpret_cast<jboolean*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::b:
                        env->SetStaticByteField(property.m_member.declaringClass, property.m_member.member, *reinterpret_cast<jbyte*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::s:
                        env->SetStaticShortField(property.m_member.declaringClass, property.m_member.member, *reinterpret_cast<jshort*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::i:
                        env->SetStaticIntField(property.m_member.declaringClass, property.m_member.member, *reinterpret_cast<jint*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::j:
                        env->SetStaticLongField(property.m_member.declaringClass, property.m_member.member, *reinterpret_cast<jlong*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::f:
                        env->SetStaticFloatField(property.m_member.declaringClass, property.m_member.member, *reinterpret_cast<float*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::d:
                        env->SetStaticDoubleField(property.m_member.declaringClass, property.m_member.member, *reinterpret_cast<double*>(_a[0]));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::c:
                        env->SetStaticCharField(property.m_member.declaringClass, property.m_member.member, jchar(*reinterpret_cast<jchar*>(_a[0])));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        break;
                    case jValueType::l:
                        jvalue value;
                        value.l = nullptr;
                        if (property.m_member.memberTypeInfo.convertInternalToExternal(env, nullptr, _a[0], value, true)) {
                            env->SetStaticObjectField(property.m_member.declaringClass, property.m_member.member, value.l);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            break;
                        }
                        break;
                    }
                }else{
                    jfieldID fieldId = property.m_member.member;
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
                            return _id - d->m_properties.size();
                        }
                    }

                    switch(property.m_member.type){
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
                        if (property.m_member.memberTypeInfo.convertInternalToExternal(env, nullptr, _a[0], value, true)) {
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

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::resetProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->resetProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_resetter){
            d->invokeMethod(env, object, property.m_resetter, _a);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::notifyProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->notifyProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    int property_notifier;
    if (_id < d->m_properties.size()
            && (property_notifier = d->m_properties[_id].m_notifier)>=0
            && property_notifier<=d->m_signals.size()) {
        const JSignalInfo& signal = d->m_signals[property_notifier];
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

    return _id - d->m_properties.size();
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
int QtJambiMetaObject::queryPropertyDesignable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyDesignable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_designable_resolver){
            d->invokeMethod(env, object, property.m_designable_resolver, _a);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::queryPropertyScriptable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyScriptable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_scriptable_resolver){
            d->invokeMethod(env, object, property.m_scriptable_resolver, _a);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::queryPropertyStored(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyStored(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_stored_resolver){
            d->invokeMethod(env, object, property.m_stored_resolver, _a);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::queryPropertyUser(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyUser(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_user_resolver){
            d->invokeMethod(env, object, property.m_user_resolver, _a);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::queryPropertyEditable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->queryPropertyEditable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_editable_resolver){
            d->invokeMethod(env, object, property.m_editable_resolver, _a);
        }
    }

    return _id - d->m_properties.size();
}
#else
int QtJambiMetaObject::bindableProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->bindableProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if(property.m_bindable){
            d->invokeMethod(env, object, property.m_bindable, _a);
        }else if (property.m_QProperty_field){
            const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
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
            jobject qProperty = env->GetObjectField(object, property.m_QProperty_field.propertyField);
            jobject bindable = Java::QtCore::QBindable::createBinding(env, qProperty);
            *reinterpret_cast<QUntypedBindable*>(_a[0]) = qtjambi_cast<QUntypedBindable>(env, bindable);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::revision() const{
    return d_ptr->m_intData[0];
}

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

jclass QtJambiMetaObject::typeOfProperty(int _id) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if (property.m_reader) {
            Q_ASSERT(property.m_reader.parameterTypeInfos.size()>1);
            return property.m_reader.parameterTypeInfos[1].javaClass();
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (property.m_QProperty_field && property.m_QProperty_field.valueMethod.methodId) {
            Q_ASSERT(property.m_QProperty_field.valueMethod.parameterTypeInfos.size()>0);
            return property.m_QProperty_field.valueMethod.parameterTypeInfos[0].javaClass();
        }
#endif
        if (property.m_member) {
            return property.m_member.memberTypeInfo.javaClass();
        }
        if (property.m_writer) {
            Q_ASSERT(property.m_writer.parameterTypeInfos.size()>1);
            return property.m_writer.parameterTypeInfos[1].javaClass();
        }
    }
    return nullptr;
}

QtJambiUtils::InternalToExternalConverter QtJambiMetaObject::internalToExternalConverterOfProperty(int _id) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if (property.m_reader) {
            Q_ASSERT(property.m_reader.parameterTypeInfos.size()>0);
            return property.m_reader.parameterTypeInfos[0].m_internalToExternalConverter;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (property.m_QProperty_field && property.m_QProperty_field.valueMethod.methodId) {
            Q_ASSERT(property.m_QProperty_field.valueMethod.parameterTypeInfos.size()>0);
            return property.m_QProperty_field.valueMethod.parameterTypeInfos[0].m_internalToExternalConverter;
        }
#endif
        if (property.m_member) {
            return property.m_member.memberTypeInfo.m_internalToExternalConverter;
        }
        if (property.m_writer) {
            Q_ASSERT(property.m_writer.parameterTypeInfos.size()>1);
            return property.m_writer.parameterTypeInfos[1].m_internalToExternalConverter;
        }
    }
    return nullptr;
}

QtJambiUtils::ExternalToInternalConverter QtJambiMetaObject::externalToInternalConverterOfProperty(int _id) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if (property.m_reader) {
            Q_ASSERT(property.m_reader.parameterTypeInfos.size()>0);
            return property.m_reader.parameterTypeInfos[0].m_externalToInternalConverter;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (property.m_QProperty_field && property.m_QProperty_field.valueMethod.methodId) {
            Q_ASSERT(property.m_QProperty_field.valueMethod.parameterTypeInfos.size()>0);
            return property.m_QProperty_field.valueMethod.parameterTypeInfos[0].m_externalToInternalConverter;
        }
#endif
        if (property.m_member) {
            return property.m_member.memberTypeInfo.m_externalToInternalConverter;
        }
        if (property.m_writer) {
            Q_ASSERT(property.m_writer.parameterTypeInfos.size()>1);
            return property.m_writer.parameterTypeInfos[1].m_externalToInternalConverter;
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
    for(const JSignalInfo& sigInfo : d->m_signals){
        if(!env->IsSameObject(sigInfo.signalTypes, nullptr)){
            env->DeleteGlobalRef(sigInfo.signalTypes);
            const_cast<JSignalInfo&>(sigInfo).signalTypes = nullptr;
        }
    }
    return pointer;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
QList<QMetaType> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass);
void registerMetaObjectByMetaTypeInterface(const QtPrivate::QMetaTypeInterface* iface, const QMetaObject* mo);
#endif


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
                    int signal_count = dynamicResult->d_ptr->m_signals.size()>0;
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
        {
            QWriteLocker locker(gMetaObjectsLock());
            Q_UNUSED(locker)
            // check if someone added a meta object meanwhile
            if(gMetaObjects->contains(classHash)){
                const QMetaObject *_returned = gMetaObjects->value(classHash, nullptr);
                if(dynamicResult && dynamicResult!=_returned){
                    (void)dynamicResult->dispose(env);
                }
                return _returned;
            }
            gMetaObjects->insert(classHash, returned);
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
        for(QMetaType mtype : registeredCustomMetaTypesForJavaClass(QtJambiAPI::getClassName(env, object_class).toLatin1().replace('.', '/'))){
            registerMetaObjectByMetaTypeInterface(mtype.iface(), returned);
        }
#endif
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

void searchForMethods(JNIEnv *env, jclass declaringClass, const JMethodInfo& pointer, jclass replacementClass, QHash<jmethodID,jmethodID>& methodMappings){
    if(pointer && !pointer.isStatic && !methodMappings.contains(pointer.methodId))
        methodMappings[pointer.methodId] = searchForMethod(env, declaringClass, pointer.methodId, replacementClass);
}

void searchForMethods(JNIEnv *env, jclass declaringClass, const QVector<QtJambiMetaObjectPrivate::Property>& properties, jclass replacementClass, QHash<jmethodID,jmethodID>& methodMappings){
    for(const QtJambiMetaObjectPrivate::Property& prop : properties){
        searchForMethods(env, declaringClass, prop.m_reader, replacementClass, methodMappings);
        searchForMethods(env, declaringClass, prop.m_writer, replacementClass, methodMappings);
        searchForMethods(env, declaringClass, prop.m_resetter, replacementClass, methodMappings);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        searchForMethods(env, declaringClass, prop.m_designable_resolver, replacementClass, methodMappings);
        searchForMethods(env, declaringClass, prop.m_scriptable_resolver, replacementClass, methodMappings);
        searchForMethods(env, declaringClass, prop.m_editable_resolver, replacementClass, methodMappings);
        searchForMethods(env, declaringClass, prop.m_stored_resolver, replacementClass, methodMappings);
        searchForMethods(env, declaringClass, prop.m_user_resolver, replacementClass, methodMappings);
#else
        searchForMethods(env, declaringClass, prop.m_bindable, replacementClass, methodMappings);
#endif
    }
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
            searchForMethods(env, p->m_clazz, p->m_properties, clazz, methodMappings);
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
        QReadLocker locker(gMetaObjectsLock());
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
        JBooleanArrayPointer isPointer(env, argumentIsPointerType);
        JBooleanArrayPointer isReference(env, argumentIsReferenceType);
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
        QWriteLocker locker(gMetaObjectsLock());
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
    QList<QSharedPointer<const QtJambiMetaObject>> metaObjects;
    MetaObjectHash metaObjectsHash;
    JavaMetaObjectHash hash;
    MetaObjectByNameHash mhash;
    {
        QWriteLocker locker(gMetaObjectsLock());
        if(!gParameterTypeInfos.isDestroyed()){
            list = gParameterTypeInfos->values();
            gParameterTypeInfos->clear();
        }
        if(!gSignalTypes.isDestroyed())
            signalTypes.swap(*gSignalTypes);
        if(!gMetaObjects.isDestroyed())
            metaObjectsHash.swap(*gMetaObjects);
        if(!javaMetaObjects.isDestroyed())
            javaMetaObjects->swap(hash);
        if(!metaObjectsByName.isDestroyed())
            metaObjectsByName->swap(mhash);
    }
    qDeleteAll(list);
    if(env){
        for(const QMetaObject* mo : metaObjectsHash){
            if(const QtJambiMetaObject* dynamo = QtJambiMetaObject::cast(mo)){
                // delete self-reference
                metaObjects << dynamo->dispose(env);
            }
        }
        for(jweak w : qAsConst(hash)){
            env->DeleteWeakGlobalRef(w);
        }
        for(jobject obj : signalTypes){
            if(obj)
                env->DeleteGlobalRef(obj);
        }
    }
    if(QThread *thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
        if(thread->isRunning()){
            if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
                data->doAtThreadEnd([metaObjects](){/*delete captured list after function call*/});
            }
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
    if(!metaObject){
        auto hash = qHash(name);
        QReadLocker locker(gMetaObjectsLock());
        metaObject = metaObjectsByName()->value(hash, nullptr);
    }
    return metaObject;
}

jobject QtJambiMetaObject::convertToJavaObject(JNIEnv *env, const QMetaObject *metaObject){
    if(const QtJambiMetaObject* dyMetaObject = QtJambiMetaObject::cast(metaObject)){
        jobject javaInstance = dyMetaObject->d_ptr->javaInstance(env);
        if(env->IsSameObject(javaInstance, nullptr)){
            jobject jmetaObject = Java::QtCore::QMetaObject::newInstance(env, jlong(reinterpret_cast<const void*>(metaObject)));
            dyMetaObject->d_ptr->setJavaInstance(env, jmetaObject);
            return jmetaObject;
        }else{
            return javaInstance;
        }
    }else{
        jweak javaInstance;
        {
            QReadLocker locker(gMetaObjectsLock());
            javaInstance = javaMetaObjects->value(metaObject, nullptr);
        }
        if(env->IsSameObject(javaInstance, nullptr)){
            jobject jmetaObject = Java::QtCore::QMetaObject::newInstance(env, jlong(reinterpret_cast<const void*>(metaObject)));
            QLatin1String javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if(javaName.isEmpty()){
                if(const std::type_info* typeId = getTypeByQtName(metaObject->className()))
                    javaName = QLatin1String(getJavaName(*typeId));
            }
            QWriteLocker locker(gMetaObjectsLock());
            if(javaName.isEmpty()){
                metaObjectsByName->insert(qHash(metaObject->className()), metaObject);
            }
            javaMetaObjects->insert(metaObject, env->NewWeakGlobalRef(jmetaObject));
            return jmetaObject;
        }else{
            return env->NewLocalRef(javaInstance);
        }
    }
}

void QtJambiMetaObject::resolveSignals(JNIEnv *env, jobject java_object, const QMetaObject* metaObject, JavaException& ocurredException){
    if(!ocurredException){
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
                                JIntArrayPointer methodIndexesPtr(env, methodIndexes);
                                JLongArrayPointer metaObjectsPtr(env, metaObjects);
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
                        JniLocalFrame frame{env, 100};
                        jclass java_class = mo->javaClass();
                        jobject signalField = Java::Runtime::Class::getDeclaredField(env, java_class, env->NewStringUTF(metaMethod.name().constData()));
                        if(signalField){
                            jfieldID signalFieldId = env->FromReflectedField(signalField);
                            jobject signalTypes = nullptr;
                            if(mo->d_ptr->m_signals[metaMethod.methodIndex()-metaObject->methodOffset()].signalTypes)
                                signalTypes = env->NewLocalRef(mo->d_ptr->m_signals[metaMethod.methodIndex()-metaObject->methodOffset()].signalTypes);
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
    int cloneCount = 0;
    for(int i = method.methodIndex()+1;; ++i){
        QMetaMethod clone = method.enclosingMetaObject()->method(i);
        if(clone.isValid() && clone.name()==method.name() && (clone.attributes() & QMetaMethod::Cloned)){
            ++cloneCount;
        }else{
            break;
        }
    }
    switch(method.parameterCount()){
    case 0:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal0::newInstance(env, java_object);
        break;
    case 1:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal1Default1::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal1::newInstance(env, java_object);
            break;
        }
        break;
    case 2:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal2Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal2Default2::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal2::newInstance(env, java_object);
            break;
        }
        break;
    case 3:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal3Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal3Default2::newInstance(env, java_object);
            break;
        case 3:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal3Default3::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal3::newInstance(env, java_object);
            break;
        }
        break;
    case 4:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal4Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal4Default2::newInstance(env, java_object);
            break;
        case 3:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal4Default3::newInstance(env, java_object);
            break;
        case 4:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal4Default4::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal4::newInstance(env, java_object);
            break;
        }
        break;
    case 5:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5Default2::newInstance(env, java_object);
            break;
        case 3:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5Default3::newInstance(env, java_object);
            break;
        case 4:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5Default4::newInstance(env, java_object);
            break;
        case 5:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5Default5::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5::newInstance(env, java_object);
            break;
        }
        break;
    case 6:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6Default2::newInstance(env, java_object);
            break;
        case 3:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6Default3::newInstance(env, java_object);
            break;
        case 4:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6Default4::newInstance(env, java_object);
            break;
        case 5:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6Default5::newInstance(env, java_object);
            break;
        case 6:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6Default6::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6::newInstance(env, java_object);
            break;
        }
        break;
    case 7:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7Default2::newInstance(env, java_object);
            break;
        case 3:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7Default3::newInstance(env, java_object);
            break;
        case 4:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7Default4::newInstance(env, java_object);
            break;
        case 5:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7Default5::newInstance(env, java_object);
            break;
        case 6:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7Default6::newInstance(env, java_object);
            break;
        case 7:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7Default7::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7::newInstance(env, java_object);
            break;
        }
        break;
    case 8:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default2::newInstance(env, java_object);
            break;
        case 3:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default3::newInstance(env, java_object);
            break;
        case 4:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default4::newInstance(env, java_object);
            break;
        case 5:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default5::newInstance(env, java_object);
            break;
        case 6:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default6::newInstance(env, java_object);
            break;
        case 7:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default7::newInstance(env, java_object);
            break;
        case 8:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8Default8::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8::newInstance(env, java_object);
            break;
        }
        break;
    case 9:
        switch(cloneCount){
        case 1:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default1::newInstance(env, java_object);
            break;
        case 2:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default2::newInstance(env, java_object);
            break;
        case 3:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default3::newInstance(env, java_object);
            break;
        case 4:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default4::newInstance(env, java_object);
            break;
        case 5:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default5::newInstance(env, java_object);
            break;
        case 6:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default6::newInstance(env, java_object);
            break;
        case 7:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default7::newInstance(env, java_object);
            break;
        case 8:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default8::newInstance(env, java_object);
            break;
        case 9:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9Default9::newInstance(env, java_object);
            break;
        default:
            signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9::newInstance(env, java_object);
            break;
        }
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

