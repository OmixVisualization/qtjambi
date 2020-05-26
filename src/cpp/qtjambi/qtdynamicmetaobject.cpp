/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtdynamicmetaobject_p.h"
#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_exceptions.h"
#include "qtjambi_thread_p.h"

#include <cstring>
#include <QtCore>
#include <QtCore/QHash>
#include <QtCore/QVarLengthArray>
#include <QtCore/QMetaEnum>
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qmetaobject_p.h>

QT_WARNING_DISABLE_DEPRECATED

#define QTJAMBI_MAX_NUMBER_OF_CLASSES 1024
#define QTJAMBI_MAX_NUMBER_OF_CLASSES_DIVISOR 100
#define QTJAMBI_MAX_NUMBER_OF_CLASSES_MAX 1024

static const char* QtDynamicMetaObjectID = "QtDynamicMetaObject";

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
    JMethodType methodType;
    jmethodID methodId;
    QString methodName;
    jclass staticAccessContext;
    QVector<ParameterTypeInfo> parameterTypeInfos;
};

typedef QPair<jfieldID, JMethodInfo > FieldPair;

typedef QHash<uint, JMethodInfo> JMethodInfoInfoHash;
typedef QHash<uint, QVector<ParameterTypeInfo>> ParameterTypeHash;
Q_GLOBAL_STATIC(QReadWriteLock, gJMethodInfoInfosLock)
Q_GLOBAL_STATIC(ParameterTypeHash, gParameterTypeInfos)
typedef QHash<int, const QMetaObject *> MetaObjectHash;
Q_GLOBAL_STATIC(MetaObjectHash, gMetaObjects);
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gMetaObjectsLock, (QReadWriteLock::Recursive));

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

static InternalToExternalConverter _default_internalToExternalConverter = [](JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)->bool{ return false; };
static ExternalToInternalConverter _default_externalToInternalConverter = [](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType) ->ConvertResponse { return ConvertResponse::NotSuccessful; };

const InternalToExternalConverter& ParameterTypeInfo::default_internalToExternalConverter()
{
    return _default_internalToExternalConverter;
}

const ExternalToInternalConverter& ParameterTypeInfo::default_externalToInternalConverter()
{
    return _default_externalToInternalConverter;
}

ParameterTypeInfo::ParameterTypeInfo()
    : m_qTypeId(QMetaType::UnknownType),
      m_qTypeName(),
      m_javaClass(nullptr),
      m_internalToExternalConverter(ParameterTypeInfo::default_internalToExternalConverter()),
      m_externalToInternalConverter(ParameterTypeInfo::default_externalToInternalConverter())
{}

ParameterTypeInfo ParameterTypeInfo::voidTypeInfo(JNIEnv* env){
    return ParameterTypeInfo(
                env,
                QMetaType::Void,
                "void",
                "void",
                ParameterTypeInfo::default_internalToExternalConverter(),
                ParameterTypeInfo::default_externalToInternalConverter()
          );
}

ParameterTypeInfo::ParameterTypeInfo(
        JNIEnv* env,
        int qTypeId,
        const QByteArray& _qTypeName,
        const char* _javaClass,
        const InternalToExternalConverter& _internalToExternalConverter,
        const ExternalToInternalConverter& _externalToInternalConverter
        )
    :
      m_qTypeId(qTypeId),
      m_qTypeName(_qTypeName),
      m_javaClass(resolveClass(env, _javaClass)),
      m_internalToExternalConverter(_internalToExternalConverter),
      m_externalToInternalConverter(_externalToInternalConverter)
{}

ParameterTypeInfo::ParameterTypeInfo(
        JNIEnv* env,
        int qTypeId,
        const QByteArray& _qTypeName,
        jclass _javaClass,
        const InternalToExternalConverter& _internalToExternalConverter,
        const ExternalToInternalConverter& _externalToInternalConverter
        )
    :
      m_qTypeId(qTypeId),
      m_qTypeName(_qTypeName),
      m_javaClass(getGlobalClassRef(env, _javaClass)),
      m_internalToExternalConverter(_internalToExternalConverter),
      m_externalToInternalConverter(_externalToInternalConverter)
{}


bool ParameterTypeInfo::convertInternalToExternal(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* out, bool forceBoxedType) const {
    Q_ASSERT(m_internalToExternalConverter);
    if(m_internalToExternalConverter(env, scope, in, out, forceBoxedType)){
        return true;
    }else{
        qWarning("Cannot convert internal type '%s' to '%s'",
                 m_qTypeName.data(),
                 qPrintable(qtjambi_class_name(env, javaClass())));
        return false;
    }
}

ConvertResponse ParameterTypeInfo::convertExternalToInternal(JNIEnv* env, QtJambiScope* scope, const jvalue& in,void* & out, jValueType valueType) const {
    Q_ASSERT(m_externalToInternalConverter);
    ConvertResponse result = m_externalToInternalConverter(env, scope, in, out, valueType);
    switch(result){
    case ConvertResponse::NotSuccessful:
        qWarning("Cannot convert external type '%s' to '%s'",
                 qPrintable(qtjambi_class_name(env, javaClass())),
                 qPrintable(m_qTypeName));
        break;
    case ConvertResponse::JavaObjectNotLinked:
        qWarning("Java object of type '%s' not linked to C++ object",
                 qPrintable(qtjambi_class_name(env, javaClass())));
        break;
    default:
        break;
    }
    return result;
}

jclass ParameterTypeInfo::javaClass() const{
    return m_javaClass;
}

int ParameterTypeInfo::metaType() const{
    return m_qTypeId;
}

void static_metacall_QObject(const QtDynamicMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv)
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
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            q->invokeConstructor(env, idx, argv);
        }
        return;
    default: break;
    }
}

void static_metacall_QtSubType(const QtDynamicMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        if(jobject object = o ? qtjambi_from_object(env, o, q->javaClass(), false, false) : nullptr){
            switch(cl){
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
            default: break;
            }
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

void static_metacall_any_type(const QtDynamicMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        switch(cl){
        case QMetaObject::CreateInstance:
            q->invokeConstructor(env, idx, argv);
            return;
        case QMetaObject::InvokeMetaMethod:
            if(!o){
                q->static_metacall(cl, idx + q->methodOffset(), argv);
                return;
            }
        default:
            break;
        }
        if(jobject object = reinterpret_cast<jobject>(o)){
            switch(cl){
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
            default: break;
            }
        }
    }
}

uint hashSum(std::initializer_list<uint> list);

typedef void (*StaticMetacallFunction)(const QtDynamicMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv);

StaticMetaCallFunction create_static_metacall(const QtDynamicMetaObject* q, StaticMetacallFunction fct){
#ifdef QT_DEBUG
#define COUNT 32
#else
#define COUNT 512  /* = 32768 options */
#endif
    return qtjambi_function_pointer<COUNT,void(QObject *, QMetaObject::Call, int, void **)>(
                [q,fct](QObject * o, QMetaObject::Call cl, int idx, void ** argv){
                    fct(q, o, cl, idx, argv);
                },
                hashSum({qHash(qint64(q)), qHash(qint64(fct))}));
}


class QtDynamicMetaObjectPrivate
{
public:
    QtDynamicMetaObjectPrivate(QtDynamicMetaObject *q, JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object);
    ~QtDynamicMetaObjectPrivate();

    void initialize(JNIEnv *jni_env, jclass java_class, const QMetaObject *original_meta_object);
    void invokeMethod(JNIEnv *env, jobject object, const JMethodInfo& methodInfo, void **_a) const;
    void invokeConstructor(JNIEnv *env, const JMethodInfo& methodInfo, void **_a) const;

    int methodFromJMethod(jmethodID methodId) const;
    const JMethodInfo* methodInfo(int index) const;
    const JMethodInfo* constructorInfo(int index) const;
    jclass javaClass() const;
    jweak javaInstance() const;
    void setJavaInstance(jweak weak);
private:
    /**
     * this list stores all texts describing the class meta object.
     */
    QList<char*> stringdataList;
    QtDynamicMetaObject *q_ptr;
    mutable QSharedPointer<const QtDynamicMetaObject> m_this_ptr;
    Q_DECLARE_PUBLIC(QtDynamicMetaObject)

    int m_method_count;
    int m_signal_count;
    int m_constructor_count;
    int m_property_count;

    jclass const m_clazz;
    QVector<JMethodInfo> m_methods;
    QMap<jmethodID,int> m_methodIndexes;
    QVector<FieldPair> m_signals;
    QVector<JMethodInfo> m_constructors;

    QVector<JMethodInfo> m_property_readers;
    QMap<int,JMethodInfo> m_property_writers;
    QMap<int,JMethodInfo> m_property_resetters;
    QMap<int,FieldPair> m_property_notifies;
    QMap<int,JMethodInfo> m_property_designable_resolvers;
    QMap<int,JMethodInfo> m_property_scriptable_resolvers;
    QMap<int,JMethodInfo> m_property_editable_resolvers;
    QMap<int,JMethodInfo> m_property_stored_resolvers;
    QMap<int,JMethodInfo> m_property_user_resolvers;

    QString *m_original_signatures;
    mutable jweak m_javaInstance;
};

QtDynamicMetaObjectPrivate::QtDynamicMetaObjectPrivate(QtDynamicMetaObject *q, JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object)
    : q_ptr(q), m_this_ptr(q), m_method_count(-1), m_signal_count(0), m_constructor_count(0), m_property_count(0),
      m_clazz(getGlobalClassRef(env, java_class)),
      m_methods(), m_methodIndexes(), m_signals(), m_constructors(),
      m_property_readers(), m_property_writers(), m_property_resetters(), m_property_notifies(), m_property_designable_resolvers(),
      m_property_scriptable_resolvers(), m_property_editable_resolvers(), m_property_stored_resolvers(),
      m_property_user_resolvers(), m_original_signatures(nullptr), m_javaInstance(nullptr)
{
    Q_ASSERT(env);
    Q_ASSERT(java_class);
    try{
        initialize(env, java_class, original_meta_object);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

QtDynamicMetaObjectPrivate::~QtDynamicMetaObjectPrivate()
{
    if(m_javaInstance){
        if (JNIEnv *env = qtjambi_current_environment(false)) {
            env->DeleteWeakGlobalRef(m_javaInstance);
            m_javaInstance = nullptr;
        }
    }

    for(char* strg : stringdataList){
        delete[] strg;
    }
    stringdataList.clear();

    delete[] m_original_signatures;
}

template<class MethodInfoContainer>
void analyze_methods(JNIEnv *env, jobject classLoader, int count, jobjectArray array, MethodInfoContainer& methodInfoContainer, QMap<jmethodID,int>* methodIndexes = nullptr){
    Q_ASSERT(count == env->GetArrayLength(array));
    Q_UNUSED(classLoader)
    for(int i=0; i<count; ++i){
        jobject methodObject = env->GetObjectArrayElement(array, i);
        qtjambi_throw_java_exception(env)
        if(methodObject){
            jobjectArray methodTypesStringAndClasses = Java::Private::QtJambi::MetaObjectTools.methodTypes(env, methodObject);
            if(methodTypesStringAndClasses){
                jobjectArray methodTypeClasses = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 0));
                qtjambi_throw_java_exception(env)
                jobjectArray methodTypeStrings = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 1));
                qtjambi_throw_java_exception(env)
                jobjectArray qtMethodTypeStrings = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 2));
                qtjambi_throw_java_exception(env)
                Q_ASSERT(methodTypeClasses);
                Q_ASSERT(methodTypeStrings);
                Q_ASSERT(qtMethodTypeStrings);
                JMethodInfo info;
                int length = env->GetArrayLength(methodTypeStrings);
                qtjambi_throw_java_exception(env)
                info.methodId = env->FromReflectedMethod(methodObject);
                if(Java::Private::Runtime::Method.isInstanceOf(env, methodObject)){
                    jstring methodName = jstring(Java::Private::Runtime::Method.getName(env, methodObject));
                    qtjambi_to_qstring(info.methodName, env, methodName);
                    int modif = Java::Private::Runtime::Executable.getModifiers(env, methodObject);
                    if(Java::Private::Runtime::Modifier.isStatic(env, modif)){
                        info.staticAccessContext = Java::Private::Runtime::Method.getDeclaringClass(env, methodObject);
                        info.staticAccessContext = getGlobalClassRef(env, info.staticAccessContext);
                    }else{
                        info.staticAccessContext = nullptr;
                    }
                }else if(Java::Private::Runtime::Constructor.isInstanceOf(env, methodObject)){
                    jstring methodName = jstring(Java::Private::Runtime::Constructor.getName(env, methodObject));
                    qtjambi_to_qstring(info.methodName, env, methodName);
                    info.staticAccessContext = Java::Private::Runtime::Constructor.getDeclaringClass(env, methodObject);
                    info.staticAccessContext = getGlobalClassRef(env, info.staticAccessContext);
                }else{
                    info.staticAccessContext = nullptr;
                }
                info.parameterTypeInfos.reserve(length);
                for(int j=0; j<length; ++j){
                    jstring el = jstring(env->GetObjectArrayElement(methodTypeStrings, j));
                    qtjambi_throw_java_exception(env)
                    QString externalType = qtjambi_to_qstring(env, el).replace(QLatin1Char('.'), QLatin1Char('/'));
                    jstring qel = jstring(env->GetObjectArrayElement(qtMethodTypeStrings, j));
                    qtjambi_throw_java_exception(env)
                    QByteArray qTypeName = qtjambi_to_qstring(env, qel).toLatin1();
                    int qMetaType = QMetaType::type(qPrintable(qTypeName));
                    QtJambiTypeManager::TypePattern internalTypePattern = QtJambiTypeManager::typeIdOfInternal(env, externalType, qTypeName, qMetaType, nullptr);
                    jclass javaClass = jclass(env->GetObjectArrayElement(methodTypeClasses, j));
                    qtjambi_throw_java_exception(env)
                    if(j==0){
                        if(Java::Runtime::Void.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::v;
                        }else if(Java::Runtime::Integer.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::i;
                        }else if(Java::Runtime::Long.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::j;
                        }else if(Java::Runtime::Short.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::s;
                        }else if(Java::Runtime::Byte.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::b;
                        }else if(Java::Runtime::Boolean.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::z;
                        }else if(Java::Runtime::Character.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::c;
                        }else if(Java::Runtime::Float.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::f;
                        }else if(Java::Runtime::Double.isPrimitiveType(env, javaClass)){
                            info.methodType = JMethodType::d;
                        }else{
                            info.methodType = JMethodType::l;
                        }
                    }
                    const InternalToExternalConverter& internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                        env,
                                                                                                                        qTypeName,
                                                                                                                        internalTypePattern,
                                                                                                                        qMetaType,
                                                                                                                        javaClass,
                                                                                                                        true
                                                                                                                    );
                    const ExternalToInternalConverter& externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                        env,
                                                                                                                        javaClass,
                                                                                                                        internalTypePattern,
                                                                                                                        qTypeName,
                                                                                                                        qMetaType
                                                                                                                    );
                    info.parameterTypeInfos.append(ParameterTypeInfo(
                                                  env,
                                                  qMetaType,
                                                  qTypeName,
                                                  javaClass,
                                                  internalToExternalConverter,
                                                  externalToInternalConverter));
                }
                methodInfoContainer[i] = info;
                if(methodIndexes){
                    methodIndexes->insert(info.methodId, i);
                }
            }
        }
    }
}

void QtDynamicMetaObjectPrivate::initialize(JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object)
{
    Q_Q(QtDynamicMetaObject);

    JniLocalFrame __jniLocalFrame(env, 1000);
    Q_UNUSED(__jniLocalFrame)
    jobject classLoader = Java::Private::Runtime::Class.getClassLoader(env, java_class);
    jobject meta_data_struct = Java::Private::QtJambi::MetaObjectTools.buildMetaData(env, java_class);
    if (!meta_data_struct)
        return;
    jintArray meta_data = Java::Private::QtJambi::MetaObjectTools$MetaData.metaData(env, meta_data_struct);
    Q_ASSERT(meta_data);

    jobjectArray string_data = Java::Private::QtJambi::MetaObjectTools$MetaData.stringData(env,meta_data_struct);
    Q_ASSERT(string_data);

    jsize number_of_strings = env->GetArrayLength(string_data);
    QByteArrayData* __stringdata = new QByteArrayData[size_t(number_of_strings)];
    {
        static int string_size = int(std::strlen(QtDynamicMetaObjectID)+1);
        __stringdata[0].ref.atomic._q_value = -1L;
        __stringdata[0].alloc = 0;
        __stringdata[0].capacityReserved = 0;
        __stringdata[0].size = string_size;
        __stringdata[0].offset = reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(QtDynamicMetaObjectID)) - reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(&(__stringdata[0])));
    }
    for(int i=1; i<number_of_strings; i++){
        jbyteArray string_data_entry = jbyteArray(env->GetObjectArrayElement(string_data, i));
        jsize string_data_len = env->GetArrayLength(string_data_entry);
        char* stringdata = new char[size_t(string_data_len+1)];
        stringdata[string_data_len] = '\0';
        env->GetByteArrayRegion(string_data_entry, 0, string_data_len, reinterpret_cast<jbyte *>(stringdata));
        qtjambi_throw_java_exception(env)
        stringdataList.append(stringdata);
        __stringdata[i].ref.atomic._q_value = -1L;
        __stringdata[i].alloc = 0;
        __stringdata[i].capacityReserved = 0;
        __stringdata[i].size = string_data_len;
        __stringdata[i].offset = reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(stringdata)) - reinterpret_cast<qptrdiff>(reinterpret_cast<const void*>(&(__stringdata[i])));
    }
    q->d.stringdata = __stringdata;

    {
        jclass java_super_class = env->GetSuperclass(java_class);
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        if(java_super_class)
            q->d.superdata = qtjambi_metaobject_for_class(env, java_super_class, original_meta_object);
        else
            q->d.superdata = nullptr;
#else
        if(java_super_class)
            q->d.superdata.direct = qtjambi_metaobject_for_class(env, java_super_class, original_meta_object);
        else
            q->d.superdata.direct = nullptr;
#endif
    }

    jsize meta_data_len = env->GetArrayLength(meta_data);
    jint* dataArray = new jint[size_t(meta_data_len)];
    q->d.data = reinterpret_cast<uint*>(dataArray);
    q->d.extradata = nullptr;
    q->d.relatedMetaObjects = nullptr;

    env->GetIntArrayRegion(meta_data, 0, meta_data_len, dataArray);

    {
        jobjectArray methods = Java::Private::QtJambi::MetaObjectTools$MetaData.slotsArray(env,meta_data_struct);
        m_method_count = env->GetArrayLength(methods);
        qtjambi_throw_java_exception(env)
        m_methods.resize(m_method_count);
        analyze_methods(env, classLoader, m_method_count, methods, m_methods, &m_methodIndexes);
    }

    {
        jobjectArray signalArray = Java::Private::QtJambi::MetaObjectTools$MetaData.signalsArray(env,meta_data_struct);
        m_signal_count = env->GetArrayLength(signalArray);
        m_signals.resize(m_signal_count);
        for(int i=0; i<m_signal_count; ++i){
            jobject fieldObject = env->GetObjectArrayElement(signalArray, i);
            qtjambi_throw_java_exception(env)
            if(fieldObject){
                JMethodInfo info;
                info.methodType = JMethodType::v;
                jstring methodName = jstring(Java::Private::Runtime::Field.getName(env,fieldObject));
                qtjambi_to_qstring(info.methodName, env, methodName);
                jfieldID field_id = env->FromReflectedField(fieldObject);
                jobjectArray methodTypesStringAndClasses = jobjectArray(Java::Private::QtJambi::MetaObjectTools.methodTypes(env, fieldObject));
                if(methodTypesStringAndClasses){
                    jobjectArray methodTypeClasses = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 0));
                    qtjambi_throw_java_exception(env)
                    jobjectArray methodTypeStrings = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 1));
                    qtjambi_throw_java_exception(env)
                    jobjectArray qtMethodTypeStrings = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 2));
                    qtjambi_throw_java_exception(env)
                    Q_ASSERT(methodTypeClasses);
                    Q_ASSERT(methodTypeStrings);
                    Q_ASSERT(qtMethodTypeStrings);
                    int length = env->GetArrayLength(methodTypeClasses);
                    info.parameterTypeInfos.reserve(length);
                    for(int j=0; j<length; ++j){
                        jclass javaClass = jclass(env->GetObjectArrayElement(methodTypeClasses, j));
                        qtjambi_throw_java_exception(env)
                        jstring el = jstring(env->GetObjectArrayElement(methodTypeStrings, j));
                        qtjambi_throw_java_exception(env)
                        QString externalType = qtjambi_to_qstring(env, el).replace(".", "/");
                        jstring qel = jstring(env->GetObjectArrayElement(qtMethodTypeStrings, j));
                        qtjambi_throw_java_exception(env)
                        QByteArray qTypeName = qtjambi_to_qstring(env, qel).toLatin1();
                        int qMetaType = QMetaType::type(qPrintable(qTypeName));
                        QtJambiTypeManager::TypePattern internalTypePattern = QtJambiTypeManager::typeIdOfInternal(env, externalType, qTypeName, qMetaType, classLoader);
                        const InternalToExternalConverter& internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                   env,
                                                                   qTypeName,
                                                                   internalTypePattern,
                                                                   qMetaType,
                                                                   javaClass,
                                                                   true
                                                                );
                        const ExternalToInternalConverter& externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                    env,
                                                                    javaClass,
                                                                    internalTypePattern,
                                                                    qTypeName,
                                                                    qMetaType
                                );
                        info.parameterTypeInfos.append(ParameterTypeInfo(
                                                      env,
                                                      qMetaType,
                                                      qTypeName,
                                                      javaClass,
                                                      internalToExternalConverter,
                                                      externalToInternalConverter));
                    }
                    jclass fieldType = Java::Private::Runtime::Field.getType(env,fieldObject);
                    Q_ASSERT(fieldType);
                    info.methodId = findEmitMethod(env, fieldType);
                    Q_ASSERT(info.methodId);
                    info.staticAccessContext = nullptr;
                    m_signals[i] = FieldPair(field_id, info);
                }
            }
        }
    }

    {
        jobjectArray methods = Java::Private::QtJambi::MetaObjectTools$MetaData.constructorsArray(env,meta_data_struct);
        m_constructor_count = env->GetArrayLength(methods);
        m_constructors.resize(m_constructor_count);
        analyze_methods(env, classLoader, m_constructor_count, methods, m_constructors);
    }

    if (m_method_count + m_signal_count + m_constructor_count > 0) {
        m_original_signatures = new QString[size_t(m_method_count + m_signal_count + m_constructor_count)];
        jobjectArray original_signatures = Java::Private::QtJambi::MetaObjectTools$MetaData.originalSignatures(env,meta_data_struct);
        for (int i=0; i<m_method_count + m_signal_count + m_constructor_count; ++i)
            m_original_signatures[i] = qtjambi_to_qstring(env, jstring(env->GetObjectArrayElement(original_signatures, i)));
    }


    {
        jobjectArray property_readers = Java::Private::QtJambi::MetaObjectTools$MetaData.propertyReadersArray(env,meta_data_struct);
        m_property_count = env->GetArrayLength(property_readers);
        m_property_readers.resize(m_property_count);
        analyze_methods(env, classLoader, m_property_count, property_readers, m_property_readers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::Private::QtJambi::MetaObjectTools$MetaData.propertyWritersArray(env,meta_data_struct),
                        m_property_writers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::Private::QtJambi::MetaObjectTools$MetaData.propertyResettersArray(env,meta_data_struct),
                        m_property_resetters);
        analyze_methods(env, classLoader, m_property_count,
                        Java::Private::QtJambi::MetaObjectTools$MetaData.propertyDesignableResolverArray(env,meta_data_struct),
                        m_property_designable_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::Private::QtJambi::MetaObjectTools$MetaData.propertyScriptableResolverArray(env,meta_data_struct),
                        m_property_scriptable_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::Private::QtJambi::MetaObjectTools$MetaData.propertyEditableResolverArray(env,meta_data_struct),
                        m_property_editable_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::Private::QtJambi::MetaObjectTools$MetaData.propertyUserResolverArray(env,meta_data_struct),
                        m_property_user_resolvers);
        analyze_methods(env, classLoader, m_property_count,
                        Java::Private::QtJambi::MetaObjectTools$MetaData.propertyStoredResolverArray(env,meta_data_struct),
                        m_property_stored_resolvers);
        jobjectArray propertyNotifiesArray = Java::Private::QtJambi::MetaObjectTools$MetaData.propertyNotifiesArray(env,meta_data_struct);
        Q_ASSERT(m_property_count == env->GetArrayLength(propertyNotifiesArray));
        for(int i=0; i<m_property_count; ++i){
            jobject fieldObject = env->GetObjectArrayElement(propertyNotifiesArray, i);
            if(fieldObject){
                JMethodInfo info;
                info.methodType = JMethodType::v;
                jstring methodName = jstring(Java::Private::Runtime::Field.getName(env,fieldObject));
                qtjambi_to_qstring(info.methodName, env, methodName);
                jfieldID field_id = env->FromReflectedField(fieldObject);
                jobjectArray methodTypesStringAndClasses = jobjectArray(Java::Private::QtJambi::MetaObjectTools.methodTypes(env, fieldObject));
                if(methodTypesStringAndClasses){
                    jobjectArray methodTypeClasses = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 0));
                    qtjambi_throw_java_exception(env)
                    jobjectArray methodTypeStrings = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 1));
                    qtjambi_throw_java_exception(env)
                    jobjectArray qtMethodTypeStrings = jobjectArray(env->GetObjectArrayElement(methodTypesStringAndClasses, 2));
                    qtjambi_throw_java_exception(env)
                    Q_ASSERT(methodTypeClasses);
                    Q_ASSERT(methodTypeStrings);
                    Q_ASSERT(qtMethodTypeStrings);
                    int length = env->GetArrayLength(methodTypeClasses);
                    info.parameterTypeInfos.reserve(length);
                    for(int j=0; j<length; ++j){
                        jclass javaClass = jclass(env->GetObjectArrayElement(methodTypeClasses, j));
                        qtjambi_throw_java_exception(env)
                        jstring el = jstring(env->GetObjectArrayElement(methodTypeStrings, j));
                        qtjambi_throw_java_exception(env)
                        QString externalType = qtjambi_to_qstring(env, el).replace(".", "/");
                        jstring qel = jstring(env->GetObjectArrayElement(qtMethodTypeStrings, j));
                        qtjambi_throw_java_exception(env)
                        QByteArray qTypeName = qtjambi_to_qstring(env, qel).toLatin1();
                        int qMetaType = QMetaType::type(qPrintable(qTypeName));
                        QtJambiTypeManager::TypePattern internalTypePattern = QtJambiTypeManager::typeIdOfInternal(env, externalType, qTypeName, qMetaType, nullptr);
                        const InternalToExternalConverter& internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                   env,
                                                                   qTypeName,
                                                                   internalTypePattern,
                                                                   qMetaType,
                                                                   javaClass,
                                                                   true
                                                                );
                        const ExternalToInternalConverter& externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                    env,
                                                                    javaClass,
                                                                    internalTypePattern,
                                                                    qTypeName,
                                                                    qMetaType
                                                                );
                        info.parameterTypeInfos.append(ParameterTypeInfo(
                                                           env,
                                                           qMetaType,
                                                           qTypeName,
                                                           javaClass,
                                                           internalToExternalConverter,
                                                           externalToInternalConverter
                                                        ));
                    }
                    jclass fieldType = Java::Private::Runtime::Field.getType(env,fieldObject);
                    info.methodId = findEmitMethod(env, fieldType);
                    Q_ASSERT(info.methodId);
                    info.staticAccessContext = nullptr;
                    m_property_notifies.insert(i, FieldPair(field_id, info));
                }
            }
        }
    }

    jobjectArray extra_data = Java::Private::QtJambi::MetaObjectTools$MetaData.extraDataArray(env, meta_data_struct);
    jsize extra_data_count = extra_data != nullptr ? env->GetArrayLength(extra_data) : 0;
    if (extra_data_count == 1) {
        jclass el = jclass(env->GetObjectArrayElement(extra_data, 0));
        qtjambi_throw_java_exception(env)
        QMetaObject const* superType = qtjambi_metaobject_for_class(env, el, nullptr);
        if(q->d.relatedMetaObjects)
            delete[] q->d.relatedMetaObjects;
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        q->d.relatedMetaObjects = new QMetaObject const*(superType);
#else
        q->d.relatedMetaObjects = new QMetaObject::SuperData(superType);
#endif
    }else if (extra_data_count > 1) {
        // ensure to not have a pointer to a static_metacall method
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        QMetaObject  const* * _relatedMetaObjects = new QMetaObject const*[size_t(extra_data_count)];
#else
        QMetaObject::SuperData* _relatedMetaObjects = new QMetaObject::SuperData[size_t(extra_data_count)];
#endif
        for (jsize i=0; i<extra_data_count; ++i) {
            jclass el = jclass(env->GetObjectArrayElement(extra_data, i));
            qtjambi_throw_java_exception(env)
            QMetaObject const* superType = qtjambi_metaobject_for_class(env, el, nullptr);
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
            _relatedMetaObjects[i] = superType;
#else
            _relatedMetaObjects[i].direct = superType;
#endif
        }
        q->d.relatedMetaObjects = _relatedMetaObjects;
    }

    bool hasStaticMembers = Java::Private::QtJambi::MetaObjectTools$MetaData.hasStaticMembers(env,meta_data_struct);
    const QMetaObjectPrivate* priv = QMetaObjectPrivate::get(q);
    if(hasStaticMembers || (priv->flags & PropertyAccessInStaticMetaCall) == PropertyAccessInStaticMetaCall){
        if(Java::Private::QtCore::QObject.isAssignableFrom(env, java_class)){
            q->d.static_metacall = create_static_metacall(q, &static_metacall_QObject);
        }else if(Java::Private::QtJambi::QtObjectInterface.isAssignableFrom(env, java_class)){
            q->d.static_metacall = create_static_metacall(q, &static_metacall_QtSubType);
        }else{
            q->d.static_metacall = create_static_metacall(q, &static_metacall_any_type);
        }
    }else{
        q->d.static_metacall = nullptr;
    }
}

void QtDynamicMetaObjectPrivate::invokeMethod(JNIEnv *env, jobject object, const JMethodInfo& methodInfo, void **_a) const
{
    QtJambiScope scope(nullptr);
    try{
        //fprintf(stderr, "QtDynamicMetaObjectPrivate::invokeMethod: %s.%s(...[%i])\n", qPrintable(qtjambi_object_class_name(env, object)), qPrintable(methodInfo.methodName), methodInfo.parameterTypeInfos.size());
        Q_ASSERT(methodInfo.parameterTypeInfos.size()==methodInfo.parameterTypeInfos.size());
        if(env->IsSameObject(object, nullptr)){
            qWarning("QtDynamicMetaObject::invokeMethod: Object is null");
            return;
        }
        Q_ASSERT(methodInfo.methodId);
        QVector<jvalue> converted_arguments(methodInfo.parameterTypeInfos.size()-1);

        JniLocalFrame __jniLocalFrame(env, 1000);
        Q_UNUSED(__jniLocalFrame)
        bool success = true;
        for (int i = 0; i < converted_arguments.size(); ++i) {
            const ParameterTypeInfo& parameterTypeInfo = methodInfo.parameterTypeInfos[i + 1];
            converted_arguments[i].l = nullptr;
            bool forceBoxedType = Java::Private::Runtime::Object.isAssignableFrom(env, parameterTypeInfo.javaClass());
            if (!parameterTypeInfo.convertInternalToExternal(env, &scope, _a[i+1], &converted_arguments[i], forceBoxedType)) {
                success = false;
                break;
            }
        }

        if (success) {
            jvalue returnValue;
            returnValue.j = 0;
            jvalue *args = converted_arguments.data();

            bool isVoid = false;
            if(methodInfo.staticAccessContext){
                switch(methodInfo.methodType){
                case JMethodType::v:
                    isVoid = true;
                    env->CallStaticVoidMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::z:
                    returnValue.z = env->CallStaticBooleanMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::b:
                    returnValue.b = env->CallStaticByteMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::i:
                    returnValue.i = env->CallStaticIntMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::s:
                    returnValue.s = env->CallStaticShortMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::j:
                    returnValue.j = env->CallStaticLongMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::f:
                    returnValue.f = env->CallStaticFloatMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::d:
                    returnValue.d = env->CallStaticDoubleMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                case JMethodType::c:
                    returnValue.c = env->CallStaticCharMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                default:
                    returnValue.l = env->CallStaticObjectMethodA(methodInfo.staticAccessContext, methodInfo.methodId, args); break;
                }
            }else{
                switch(methodInfo.methodType){
                case JMethodType::v:
                    isVoid = true;
                    env->CallVoidMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::z:
                    returnValue.z = env->CallBooleanMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::b:
                    returnValue.b = env->CallByteMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::i:
                    returnValue.i = env->CallIntMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::s:
                    returnValue.s = env->CallShortMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::j:
                    returnValue.j = env->CallLongMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::f:
                    returnValue.f = env->CallFloatMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::d:
                    returnValue.d = env->CallDoubleMethodA(object, methodInfo.methodId, args); break;
                case JMethodType::c:
                    returnValue.c = env->CallCharMethodA(object, methodInfo.methodId, args); break;
                default:
                    returnValue.l = env->CallObjectMethodA(object, methodInfo.methodId, args); break;
                }
            }
            qtjambi_throw_java_exception(env)
            if(_a[0]){
                if(!isVoid){
                    methodInfo.parameterTypeInfos[0].convertExternalToInternal(env, nullptr, returnValue, _a[0], jValueType(methodInfo.methodType));
                }
            }
        }
    }catch(const JavaException& exn){
        qtjambi_push_blocked_exception(env, exn);
    }
}

/**
 * This method calls the constructor of a Java class caused by a meta object constructor call.
 */
void QtDynamicMetaObjectPrivate::invokeConstructor(JNIEnv *env, const JMethodInfo& methodInfo, void **_a) const
{
    QtJambiScope scope(nullptr);
    QVector<jvalue> converted_arguments(methodInfo.parameterTypeInfos.size() - 1);
    bool success = true;
    JniLocalFrame __jniLocalFrame(env, 100);
    Q_UNUSED(__jniLocalFrame)

    for (int i = 0; i < converted_arguments.size(); ++i) {
        const ParameterTypeInfo& parameterTypeInfo = methodInfo.parameterTypeInfos[i + 1];
        converted_arguments[i].l = nullptr;
        bool forceBoxedType = Java::Private::Runtime::Object.isAssignableFrom(env, parameterTypeInfo.javaClass());
        if (!parameterTypeInfo.convertInternalToExternal(env, &scope, _a[i+1], &converted_arguments[i], forceBoxedType)) {
            success = false;
            break;
        }
    }

    if (success) {
        jvalue *args = converted_arguments.data();
        jobject object = env->NewObjectA(m_clazz, methodInfo.methodId, args);
        qtjambi_throw_java_exception(env)
        if(Java::Private::QtJambi::QtObjectInterface.isAssignableFrom(env, m_clazz)){
            void* &pointer = *reinterpret_cast<void**>(_a[0]);
            pointer = qtjambi_to_object(env, object);
        }else{
            jobject &pointer = *reinterpret_cast<jobject*>(_a[0]);
            pointer = object;
        }
    } else {
        qWarning("QtDynamicMetaObject::invokeConstructor: Failed to convert arguments");
    }
}


QtDynamicMetaObject::QtDynamicMetaObject(JNIEnv *jni_env, jclass java_class, const QMetaObject *original_meta_object)
    : d_ptr(new QtDynamicMetaObjectPrivate(this, jni_env, java_class, original_meta_object)) {
}

QtDynamicMetaObject::~QtDynamicMetaObject()
{
    delete d_ptr;
}

jweak QtDynamicMetaObjectPrivate::javaInstance() const{
    return m_javaInstance;
}

void QtDynamicMetaObjectPrivate::setJavaInstance(jweak weak){
    m_javaInstance = weak;
}

jclass QtDynamicMetaObjectPrivate::javaClass() const{
    return m_clazz;
}

jweak QtDynamicMetaObject::javaInstance(const QtDynamicMetaObject* metaObject){
    return metaObject->d_ptr->javaInstance();
}

void QtDynamicMetaObject::setJavaInstance(const QtDynamicMetaObject* metaObject, jweak weak){
    metaObject->d_ptr->setJavaInstance(weak);
}

bool QtDynamicMetaObject::isDynamic(const QMetaObject* metaObject){
    Q_ASSERT(metaObject);
    return metaObject->d.stringdata[0].data()==QtDynamicMetaObjectID;
}

jclass QtDynamicMetaObject::javaClass() const{
    return d_ptr->javaClass();
}

jclass QtDynamicMetaObject::javaClass(JNIEnv * env, const QMetaObject* metaObject, bool exactOrNull){
    if(qtjambi_metaobject_is_dynamic(metaObject)){
        const QtDynamicMetaObject* dynamicMetaObject = static_cast<const QtDynamicMetaObject*>(metaObject);
        return jclass(env->NewLocalRef(dynamicMetaObject->javaClass()));
    }else{
        if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
            jclass result = env->FindClass(getJavaName(*typeId));
            qtjambi_throw_java_exception(env)
            return result;
        }else if(exactOrNull){
            return nullptr;
        }else if(metaObject->superClass()){
            return javaClass(env, metaObject->superClass(), exactOrNull);
        }else{
            return Java::Private::QtJambi::QtGadget.getClass(env);
        }
    }
}

const QVector<ParameterTypeInfo>* QtDynamicMetaObject::methodParameterInfo(JNIEnv * env, const QMetaMethod& method)
{
    Q_ASSERT(method.isValid());
    const QMetaObject* metaObject = method.enclosingMetaObject();
    if(qtjambi_metaobject_is_dynamic(metaObject)){
        const QtDynamicMetaObject* dynamicMetaObject = static_cast<const QtDynamicMetaObject*>(metaObject);
        if(method.methodType()==QMetaMethod::Constructor){
            for(int i=0; i<method.enclosingMetaObject()->constructorCount(); ++i){
                if(method==method.enclosingMetaObject()->constructor(i)){
                    if(const JMethodInfo* info = dynamicMetaObject->d_ptr->constructorInfo(i)){
                        return &info->parameterTypeInfos;
                    }
                }
            }
        }else{
            if(const JMethodInfo* info = dynamicMetaObject->d_ptr->methodInfo(method.methodIndex() - method.enclosingMetaObject()->methodOffset())){
                return &info->parameterTypeInfos;
            }
        }
    }
    QVector<ParameterTypeInfo>* result = nullptr;
    uint key = hashSum({qHash(qintptr(method.enclosingMetaObject())), qHash(method.methodIndex())});
    {
        QReadLocker locker(gJMethodInfoInfosLock());
        Q_UNUSED(locker)
        if(gParameterTypeInfos->contains(key)){
            result = &(*gParameterTypeInfos)[key];
        }
    }
    if(!result){
        QWriteLocker locker(gJMethodInfoInfosLock());
        Q_UNUSED(locker)
        gParameterTypeInfos->insert(key, QVector<ParameterTypeInfo>());
        result = &(*gParameterTypeInfos)[key];
        ParameterTypeInfoProvider ptip = registeredParameterTypeInfoProvider(method.enclosingMetaObject());
        if(!ptip || !ptip(env, method, *result)){
            int length = method.parameterCount()+1;
            result->reserve(length);
            jobject classLoader = nullptr;
            QList<QByteArray> parameterTypes = method.parameterTypes();
            for (int i=0; i<length; ++i){
                QByteArray qTypeName;
                int metaType;
                if(i==0){
                    qTypeName = method.typeName();
                    metaType = method.returnType();
                    if(qTypeName.isEmpty())
                        qTypeName = QMetaType::typeName(metaType);
                }else{
                    qTypeName = parameterTypes.at(i-1).data();
                    metaType = method.parameterType(i-1);
                    if(qTypeName.isEmpty())
                        qTypeName = QMetaType::typeName(metaType);
                }
                QString externalTypeName = QtJambiTypeManager::getExternalTypeNameNoWarn(env, qTypeName, method.enclosingMetaObject(), QtJambiTypeManager::ArgumentType, metaType);
                if(externalTypeName.isEmpty()){
                    if(qTypeName.endsWith("*")){
                        externalTypeName = "io/qt/QNativePointer";
                    }else if(QMetaType::hasRegisteredConverterFunction(metaType, qMetaTypeId<int>())){
                        externalTypeName = "java/lang/Integer";
                    }else{
                        externalTypeName = "java/lang/Object";
                    }
                }
                jclass javaClass = resolveClass(env, qPrintable(externalTypeName), classLoader);

                // Find usage pattern
                QtJambiTypeManager::TypePattern internalTypePattern = QtJambiTypeManager::typeIdOfInternal(env, externalTypeName, qTypeName, metaType, classLoader);

                const InternalToExternalConverter& internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                qTypeName,
                                                                internalTypePattern,
                                                                metaType,
                                                                javaClass,
                                                                true);
                const ExternalToInternalConverter& externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                env,
                                                                javaClass,
                                                                internalTypePattern,
                                                                qTypeName,
                                                                metaType);
                result->append(ParameterTypeInfo(
                                              env,
                                              metaType,
                                              qTypeName,
                                              javaClass,
                                              internalToExternalConverter,
                                              externalToInternalConverter));
            }
        }
    }
    return result;
}

int QtDynamicMetaObject::methodFromJMethod(const QMetaObject* metaObject, jmethodID methodId){
    if(qtjambi_metaobject_is_dynamic(metaObject)){
        const QtDynamicMetaObject* dynamicMetaObject = static_cast<const QtDynamicMetaObject*>(metaObject);
        return dynamicMetaObject->d_ptr->methodFromJMethod(methodId);
    }
    return -1;
}

const JMethodInfo* QtDynamicMetaObjectPrivate::constructorInfo(int index) const{
    const JMethodInfo* result = nullptr;
    if(index>=0 && index<m_constructors.size()){
        result = &m_constructors[index];
    }
    return result;
}

const JMethodInfo* QtDynamicMetaObjectPrivate::methodInfo(int index) const{
    const JMethodInfo* result = nullptr;
    if(index>=0){
        if(index<m_signals.size()){
            result = &m_signals[index].second;
        }else{
            index -= m_signal_count;
            if(index>=0 && index<m_methods.size()){
                result = &m_methods[index];
            }
        }
    }
    return result;
}

int QtDynamicMetaObjectPrivate::methodFromJMethod(jmethodID methodId) const{
    Q_Q(const QtDynamicMetaObject);
    int index = m_methodIndexes.value(methodId, -1);
    if(index>=0){
        index += m_signal_count + q->methodOffset();
    }
    return index;
}

int QtDynamicMetaObject::invokeSignalOrSlot(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->invokeSignalOrSlot(env, object, _id, _a);
    if (_id < 0) return _id;

    // Emit the correct signal
    if (_id < d->m_signal_count) {
#ifndef QT_QTJAMBI_PORT
        if(Java::Private::QtCore::QObject.isInstanceOf(env, object)){
            QMetaObject::activate(qtjambi_cast<QObject*>(env, object), this, _id, _a);
        }else{
#endif
            jobject signal_object = env->GetObjectField(object, d->m_signals[_id].first);
            qtjambi_throw_java_exception(env)
            Q_ASSERT(signal_object);
            d->invokeMethod(env, signal_object, d->m_signals[_id].second, _a);
#ifndef QT_QTJAMBI_PORT
        }
#endif
    } else if (_id < d->m_signal_count + d->m_method_count) { // Call the correct method
        d->invokeMethod(env, object, d->m_methods[_id - d->m_signal_count], _a);
    } else if (_id < d->m_signal_count + d->m_method_count + d->m_constructor_count) { // Call the correct constructor
        d->invokeConstructor(env, d->m_constructors[_id - d->m_signal_count - d->m_method_count], _a);
    }
    qtjambi_throw_java_exception(env)

    return _id - d->m_method_count - d->m_signal_count - d->m_constructor_count;
}

int QtDynamicMetaObject::invokeConstructor(JNIEnv *env, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);
    if (_id < d->m_signal_count + d->m_method_count + d->m_constructor_count) { // Call the correct constructor
        d->invokeConstructor(env, d->m_constructors[_id], _a);
    }
    return _id - d->m_constructor_count;
}

int QtDynamicMetaObject::readProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->readProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        d->invokeMethod(env, object, d->m_property_readers[_id], _a);
    }
    return _id - d->m_property_count;
}

int QtDynamicMetaObject::writeProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->writeProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_writers.contains(_id)) {
        // invokeMethod expects a place holder for return value, but write property meta calls
        // do not since all property writers return void by convention.
        void *a[2] = { nullptr, _a[0] };
        d->invokeMethod(env, object, d->m_property_writers[_id], a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::resetProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->resetProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_resetters.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_resetters[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::notifyProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->notifyProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_notifies.contains(_id)) {
        jfieldID field_id = d->m_property_notifies[_id].first;
        qtjambi_throw_java_exception(env)
        Q_ASSERT(field_id);

        jobject signal_object = env->GetObjectField(object, field_id);
        qtjambi_throw_java_exception(env)
        Q_ASSERT(signal_object);
        if(d->m_property_notifies[_id].second.methodId)
            d->invokeMethod(env, signal_object, d->m_property_notifies[_id].second, _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyDesignable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyDesignable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_designable_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_designable_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyScriptable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyScriptable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_scriptable_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_scriptable_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyStored(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyStored(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_stored_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_stored_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyUser(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyUser(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_user_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_user_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyEditable(JNIEnv *env, jobject object, int _id, void **_a, bool direct) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyEditable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count && d->m_property_editable_resolvers.contains(_id)) {
        d->invokeMethod(env, object, d->m_property_editable_resolvers[_id], _a);
    }

    return _id - d->m_property_count;
}

jclass QtDynamicMetaObject::typeOfProperty(int _id) const
{
    Q_D(const QtDynamicMetaObject);
    if (_id < d->m_property_count) {
        Q_ASSERT(d->m_property_readers[_id].parameterTypeInfos.size()>0);
        return d->m_property_readers[_id].parameterTypeInfos[0].javaClass();
    }
    if (_id < d->m_property_count && d->m_property_writers.contains(_id)) {
        Q_ASSERT(d->m_property_writers[_id].parameterTypeInfos.size()>1);
        return d->m_property_writers[_id].parameterTypeInfos[1].javaClass();
    }
    return nullptr;
}

const QSharedPointer<const QtDynamicMetaObject>& QtDynamicMetaObject::thisPointer() const
{
    Q_D(const QtDynamicMetaObject);
    return d->m_this_ptr;
}

QSharedPointer<const QtDynamicMetaObject> QtDynamicMetaObject::dispose() const
{
    Q_D(const QtDynamicMetaObject);
    QSharedPointer<const QtDynamicMetaObject> pointer = d->m_this_ptr;
    d->m_this_ptr.clear();
    return pointer;
}

const QMetaObject *qtjambi_metaobject_for_class(JNIEnv *env, jclass object_class, const std::function<const QMetaObject *()>& original_meta_object_provider)
{
    Q_ASSERT(object_class != nullptr);

    // If original_meta_object is null then we have to look it up

    int classHash = Java::Private::Runtime::Object.hashCode(env,object_class);
    const QMetaObject *returned = nullptr;
    {
        QReadLocker locker(gMetaObjectsLock());
        Q_UNUSED(locker)
        returned = gMetaObjects->value(classHash, nullptr);
    }
    {
        if (returned == nullptr) {
            // Return original meta object for generated classes, and
            // create a new dynamic meta object for subclasses
            QtDynamicMetaObject* dynamicResult = nullptr;
            if (Java::Private::QtJambi::QtJambiInternal.isGeneratedClass(env, object_class)) {
                returned = original_meta_object_provider();
                if(!returned)
                    returned = dynamicResult = new QtDynamicMetaObject(env, object_class, nullptr);
            } else {
                const QMetaObject *original_meta_object = original_meta_object_provider();

                if (original_meta_object==qt_getQtMetaObject()) {
                    returned = original_meta_object;
                }else {
                    returned = dynamicResult = new QtDynamicMetaObject(env, object_class, original_meta_object);
                }
            }
            QWriteLocker locker(gMetaObjectsLock());
            Q_UNUSED(locker)
            // check if someone added a meta object meanwhile
            if(const QMetaObject *_returned = gMetaObjects->value(classHash, nullptr)){
                if(dynamicResult){
                    dynamicResult->dispose();
                }
                return _returned;
            }
            gMetaObjects->insert(classHash, returned);
        }
    }
    Q_ASSERT(returned);
    return returned;
}

const QMetaObject *qtjambi_metaobject_for_class(JNIEnv *env, jclass object_class, const QMetaObject *original_meta_object)
{
    return qtjambi_metaobject_for_class(env, object_class,
                                        [&]() -> const QMetaObject * {
                                            if (original_meta_object == nullptr) {
                                                QString class_name = qtjambi_class_name(env, object_class).replace(".", "/");
                                                if (class_name=="io/qt/core/Qt") {
                                                    original_meta_object = qt_getQtMetaObject();
                                                } else {
                                                    Q_ASSERT(!class_name.isEmpty());
                                                    if(const std::type_info* typeId = getTypeByJavaName(class_name)){
                                                        original_meta_object = registeredOriginalMetaObject(*typeId);
                                                    }else if(jclass _object_class = resolveClosestQtSuperclass(env, object_class)){
                                                        class_name = qtjambi_class_name(env, _object_class).replace(".", "/");
                                                        Q_ASSERT(!class_name.isEmpty());
                                                        if(const std::type_info* typeId = getTypeByJavaName(class_name)){
                                                            original_meta_object = registeredOriginalMetaObject(*typeId);
                                                        }
                                                    }
                                                }
                                            }
                                            return original_meta_object;
                                        }
                                    );
}

const QMetaObject *qtjambi_metaobject_for_class(JNIEnv *env, jclass object_class, const std::type_info& typeId)
{
    return qtjambi_metaobject_for_class(env, object_class,
                                        [&typeId]() -> const QMetaObject * {
                                            return registeredOriginalMetaObject(typeId);
                                        }
                                    );
}

void clear_metaobjects_at_shutdown()
{
    if(!gParameterTypeInfos.isDestroyed())
        gParameterTypeInfos->clear();
    QList<QSharedPointer<const QtDynamicMetaObject>> metaObjects;
    {
        QWriteLocker locker(gMetaObjectsLock());
        Q_UNUSED(locker)
        for(const QMetaObject* mo : *gMetaObjects){
            if(QtDynamicMetaObject::isDynamic(mo)){
                const QtDynamicMetaObject* dynamo = static_cast<const QtDynamicMetaObject*>(mo);
                // delete self-reference
                metaObjects << dynamo->dispose();
            }
        }
        gMetaObjects->clear();
    }
    if(QThread *thread = QCoreApplicationPrivate::mainThread()){
        if(QThreadUserData* data = static_cast<QThreadUserData*>(thread->userData(QThreadUserData::id()))){
            data->doAtThreadEnd([metaObjects](){/*delete captured list after function call*/});
        }
    }
}

