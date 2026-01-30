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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include "pch_p.h"
#include "qmlapi.h"

#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qmetaobject_p.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#define QTJAMBI_INTERNAL_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, methodname, __FILE__, __LINE__, Q_FUNC_INFO);

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

struct JMethodInfo : JInvokableInfo{
    JMethodType methodType = JMethodType::v;
    jmethodID methodId = nullptr;
    bool isStatic = false;
    inline operator bool()const{return methodId!=nullptr;}
    inline bool operator!()const{return methodId==nullptr;}
    QString toString(JNIEnv* env) const;
};

QString JMethodInfo::toString(JNIEnv* env) const{
    if(m_description.isEmpty() && declaringClass && methodId){
        jobject method = env->ToReflectedMethod(declaringClass, methodId, isStatic);
        if(method){
            jstring strg = Java::Runtime::Object::toString(env, method);
            m_description = qtjambi_cast<QString>(env, strg);
        }
    }
    return m_description;
}

QString JConstructorInfo::toString(JNIEnv* env) const{
    if(m_description.isEmpty() && declaringClass && constructorId){
        jobject method = env->ToReflectedMethod(declaringClass, constructorId, false);
        if(method){
            jstring strg = Java::Runtime::Object::toString(env, method);
            m_description = qtjambi_cast<QString>(env, strg);
        }
    }
    return m_description;
}

InPlaceInitializer::InPlaceInitializer(const QtJambiMetaObject* metaObject, void* placement, size_t _size, size_t _align,
                                       const JConstructorInfo& constructorInfo,
                                       QVector<jvalue>&& arguments,
                                       bool is_qml_call)
  : m_metaObject(metaObject),
    m_placement(placement),
    m_size(_size),
    m_align(_align),
    m_parameterTypeInfos(constructorInfo.parameterTypeInfos),
    m_constructorFunction(constructorInfo.constructorFunction),
    m_constructorArgumentFunctionTypes(constructorInfo.constructorArgumentTypes),
    m_matches(m_constructorFunction),
    m_arguments(std::move(arguments)),
    m_is_qml_call(is_qml_call),
    m_isInitialized(new bool(false))
{
    if(!m_parameterTypeInfos.isEmpty())
        m_parameterTypeInfos.takeFirst();
}

InPlaceInitializer::InPlaceInitializer(const QtJambiMetaObject* metaObject, void* placement, size_t _size, size_t _align,
                                       QtJambiAPI::ConstructorFn constructorFunction,
                                       bool is_qml_call)
    : m_metaObject(metaObject),
    m_placement(placement),
    m_size(_size),
    m_align(_align),
    m_parameterTypeInfos(),
    m_constructorFunction(constructorFunction),
    m_constructorArgumentFunctionTypes(),
    m_matches(m_constructorFunction),
    m_arguments(),
    m_is_qml_call(is_qml_call),
    m_isInitialized(new bool(false))
{
}

InPlaceInitializer::InPlaceInitializer(InPlaceInitializer* parentInitializer,
                                       const QtJambiMetaObject* metaObject,
                                       void* placement, size_t _size, size_t _align,
                                       QVector<ParameterTypeInfo>&& parameterTypeInfos,
                                       QtJambiAPI::ConstructorFn constructorFunction,
                                       const QList<jclass>& constructorArgumentFunctionTypes,
                                       QVector<jvalue> arguments,
                                       bool is_qml_call,
                                       bool matches)
    : m_metaObject(metaObject),
    m_placement(placement),
    m_size(_size),
    m_align(_align),
    m_parameterTypeInfos(std::move(parameterTypeInfos)),
    m_constructorFunction(constructorFunction),
    m_constructorArgumentFunctionTypes(constructorArgumentFunctionTypes),
    m_matches(matches),
    m_arguments(std::move(arguments)),
    m_is_qml_call(is_qml_call),
    m_isInitialized(parentInitializer->m_isInitialized),
    m_parentInitializer(parentInitializer)
{
}

SuperInitializer::SuperInitializer(JNIEnv *env,
                                   InPlaceInitializer* parentInitializer,
                                   const QtJambiMetaObject* metaObject, void* placement, size_t _size, size_t _align,
                                   QVector<ParameterTypeInfo>&& parameterTypeInfos,
                                   QtJambiAPI::ConstructorFn constructorFunction,
                                   const QList<jclass>& constructorArgumentFunctionTypes,
                                   QVector<jvalue> arguments,
                                   bool is_qml_call,
                                   bool matches,
                                   bool requireJava)
    : InPlaceInitializer(parentInitializer,
                         metaObject,
                         placement, _size, _align,
                         std::move(parameterTypeInfos),
                         constructorFunction,
                         constructorArgumentFunctionTypes,
                         std::move(arguments),
                         is_qml_call,
                         matches),
    m_javaInPlaceObject()
{
    if(requireJava)
        m_javaInPlaceObject.assign(env, Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(this)));
}

jobject SuperInitializer::inPlaceObject(JNIEnv *env) const{
    return m_javaInPlaceObject.object(env);
}

int InPlaceInitializer::parameterCount(){
    return m_parameterTypeInfos.count();
}

jobject InPlaceInitializer::argumentAt(JNIEnv *env, int index){
    if(m_metaObject){
        if(index >= m_arguments.size())
            Java::Runtime::IndexOutOfBoundsException::throwNew(env, QString::asprintf("%d", index) QTJAMBI_STACKTRACEINFO );
        jclass type = m_parameterTypeInfos[index].javaClass();
        jvalue arg = m_arguments[index];
        if(Java::Runtime::Class::isPrimitive(env, type)){
            if(Java::Runtime::Integer::isPrimitiveType(env, type)){
                return Java::Runtime::Integer::valueOf(env, arg.i);
            }else if(Java::Runtime::Byte::isPrimitiveType(env, type)){
                return Java::Runtime::Byte::valueOf(env, arg.b);
            }else if(Java::Runtime::Short::isPrimitiveType(env, type)){
                return Java::Runtime::Short::valueOf(env, arg.s);
            }else if(Java::Runtime::Long::isPrimitiveType(env, type)){
                return Java::Runtime::Long::valueOf(env, arg.j);
            }else if(Java::Runtime::Boolean::isPrimitiveType(env, type)){
                return Java::Runtime::Boolean::valueOf(env, arg.z);
            }else if(Java::Runtime::Character::isPrimitiveType(env, type)){
                return Java::Runtime::Character::valueOf(env, arg.c);
            }else if(Java::Runtime::Float::isPrimitiveType(env, type)){
                return Java::Runtime::Float::valueOf(env, arg.f);
            }else if(Java::Runtime::Double::isPrimitiveType(env, type)){
                return Java::Runtime::Double::valueOf(env, arg.d);
            }else{
                return nullptr;
            }
        }else{
            return env->NewLocalRef(arg.l);
        }
    }else{
        Java::Runtime::IndexOutOfBoundsException::throwNew(env, QString::asprintf("%d", index) QTJAMBI_STACKTRACEINFO );
    }
    return nullptr;
}

jclass InPlaceInitializer::parameterTypeAt(JNIEnv *env, int index){
    if(m_metaObject){
        if(index >= m_parameterTypeInfos.size())
            Java::Runtime::IndexOutOfBoundsException::throwNew(env, QString::asprintf("%d", index) QTJAMBI_STACKTRACEINFO );
        return m_parameterTypeInfos[index].javaClass();
    }else{
        Java::Runtime::IndexOutOfBoundsException::throwNew(env, QString::asprintf("%d", index) QTJAMBI_STACKTRACEINFO );
    }
    return nullptr;
}

SuperInitializer* InPlaceInitializer::asArguments(JNIEnv *env, std::initializer_list<int> indexes, bool requireJava){
    if(*m_isInitialized.get()){
        Java::Runtime::IllegalStateException::throwNew(env, "Calling asArguments(...) after initialize(Object) not allowed." QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }else{
        QVector<ParameterTypeInfo> parameterTypeInfos;
        QVector<jvalue> arguments;
        bool matches = m_matches;
        for(int index : indexes){
            if(index >= m_parameterTypeInfos.size())
                Java::Runtime::IndexOutOfBoundsException::throwNew(env, QString::asprintf("%d", index) QTJAMBI_STACKTRACEINFO );
            parameterTypeInfos << m_parameterTypeInfos[index];
            if(index<m_constructorArgumentFunctionTypes.size() && arguments.size()!=index)
                matches = false;
            else if(m_constructorArgumentFunctionTypes.isEmpty() && !parameterTypeInfos.isEmpty())
                matches = false;
            arguments << m_arguments[index];
        }
        if(!matches && !m_constructorArgumentFunctionTypes.isEmpty()){
            matches = true;
            if(parameterTypeInfos.size()>=m_constructorArgumentFunctionTypes.size()){
                for(int i=0, l=m_constructorArgumentFunctionTypes.size(); i<l; ++i){
                    if(!env->IsAssignableFrom(m_constructorArgumentFunctionTypes[i], parameterTypeInfos[i].javaClass())){
                        matches = false;
                        break;
                    }
                }
            }
        }
        SuperInitializer* superInitializer = new SuperInitializer(env,
                                                  this,
                                                  m_metaObject,
                                                  m_placement,
                                                  m_size, m_align,
                                                  std::move(parameterTypeInfos),
                                                  m_constructorFunction,
                                                  m_constructorArgumentFunctionTypes,
                                                  std::move(arguments),
                                                  m_is_qml_call,
                                                  matches,
                                                  requireJava);
        m_superInitializers << superInitializer;
        return superInitializer;
    }
}

InPlaceInitializer* InPlaceInitializer::topLevel(){
    if(m_parentInitializer)
        return m_parentInitializer->topLevel();
    else return this;
}

void InPlaceInitializer::reset(JNIEnv *env){
    while(!m_superInitializers.isEmpty()){
        SuperInitializer* superInitializer = m_superInitializers.takeLast();
        if(jobject o = superInitializer->inPlaceObject(env))
            Java::QtJambi::QtConstructInPlace::set_native_id(env, o, 0);
        superInitializer->reset(env);
        delete superInitializer;
    }
}

class MetaObjectExceptionHandler : public QtJambiExceptionHandler{
public:
    MetaObjectExceptionHandler() = default;
    void handle(JNIEnv *env, const JavaException& exn, const QObject* qobject){
        bool blockException = false;
        if(qobject && *reinterpret_cast<quint8*>(this)==0){
            const QObjectPrivate* p = QObjectPrivate::get(qobject);
            blockException = !p->isDeletingChildren && p->declarativeData;
        }
        static ResettableBoolFlag noExceptionForwardingByMetaObject(env, "io.qt.no-exception-forwarding-from-meta-calls");
        if(blockException || noExceptionForwardingByMetaObject){
            QtJambiExceptionBlocker __blocker;
            {
                QtJambiExceptionHandler __handler;
                __handler.handle(env, exn, nullptr);
            }
            __blocker.release(env);
            return;
        }
        QtJambiExceptionHandler::handle(env, exn, nullptr);
    }
private:
    Q_DISABLE_COPY_MOVE(MetaObjectExceptionHandler)
};

typedef QPair<jfieldID, JMethodInfo > FieldPair;

struct JMemberInfo{
    QString name;
    jfieldID member = nullptr;
    jValueType type = jValueType::l;
    ParameterTypeInfo memberTypeInfo;
    bool canWrite = false;
    bool isStatic = false;
    jclass declaringClass = nullptr;
    inline operator bool()const{return member!=nullptr;}
    inline bool operator!()const{return member==nullptr;}
};

struct JSignalInfo{
    QString signalName;
    jfieldID signalField = nullptr;
    JMethodInfo emitMethodInfo;
    bool isClone = false;
    JObjectWrapper m_signalTypes;
    inline operator bool()const{return signalField!=nullptr;}
    inline bool operator!()const{return signalField==nullptr;}
private:
};

struct QPropertyInfo{
    QString name;
    jfieldID propertyField = nullptr;
    JMethodInfo valueMethod;
    JMethodInfo setValueMethod;
    inline operator bool()const{return propertyField!=nullptr;}
    inline bool operator!()const{return propertyField==nullptr;}
};

QtJambiUtils::InternalToExternalConverter ParameterTypeInfo::default_internalToExternalConverter()
{
    return QtJambiUtils::InternalToExternalConverter([](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{ return false; });
}

QtJambiUtils::ExternalToInternalConverter ParameterTypeInfo::default_externalToInternalConverter()
{
    return QtJambiUtils::ExternalToInternalConverter([](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) ->bool { return false; });
}

ParameterTypeInfo::ParameterTypeInfo()
    : m_metaType(QMetaType::UnknownType),
      m_typeName(),
      m_javaClass(nullptr),
      m_internalToExternalConverter(ParameterTypeInfo::default_internalToExternalConverter()),
      m_externalToInternalConverter(ParameterTypeInfo::default_externalToInternalConverter()),
      m_resolvedI2E(true),
      m_resolvedE2I(true)
{}

ParameterTypeInfo ParameterTypeInfo::voidTypeInfo(JNIEnv* env){
    ParameterTypeInfo info = ParameterTypeInfo(
                QMetaType(QMetaType::Void),
                getGlobalClassRef(env, Java::Runtime::Void::primitiveType(env))
          );
    info.m_externalToInternalConverter = ParameterTypeInfo::default_externalToInternalConverter();
    info.m_internalToExternalConverter = ParameterTypeInfo::default_internalToExternalConverter();
    info.m_resolvedI2E = true;
    info.m_resolvedE2I = true;
    return info;
}

ParameterTypeInfo::ParameterTypeInfo(
    const QMetaType& metaType,
    jclass _javaClass
    )
    :
    m_metaType(metaType),
    m_typeName(QLatin1String(m_metaType.name())),
    m_javaClass(_javaClass),
    m_internalToExternalConverter(),
    m_externalToInternalConverter(),
    m_resolvedI2E(false),
    m_resolvedE2I(false)
{
}

ParameterTypeInfo::ParameterTypeInfo(
        const QMetaType& metaType,
        const QString& typeName,
        jclass _javaClass
        )
    :
      m_metaType(metaType),
      m_typeName(typeName),
      m_javaClass(_javaClass),
      m_internalToExternalConverter(),
      m_externalToInternalConverter(),
      m_resolvedI2E(false),
      m_resolvedE2I(false)
{
}

ParameterTypeInfo::ParameterTypeInfo(
        const QMetaType& metaType,
        jclass _javaClass,
        QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
        QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
        )
    :
      m_metaType(metaType),
      m_typeName(QLatin1String(m_metaType.name())),
      m_javaClass(_javaClass),
      m_internalToExternalConverter(std::move(internalToExternalConverter)),
      m_externalToInternalConverter(std::move(externalToInternalConverter)),
      m_resolvedI2E(m_internalToExternalConverter),
      m_resolvedE2I(m_externalToInternalConverter)
{
}

ParameterTypeInfo::ParameterTypeInfo(
        const QMetaType& metaType,
        const QString& typeName,
        jclass _javaClass,
        QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
        QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
        )
    :
      m_metaType(metaType),
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
      m_metaType(other.m_metaType),
      m_typeName(other.m_typeName),
      m_javaClass(other.m_javaClass),
      m_internalToExternalConverter(other.m_internalToExternalConverter),
      m_externalToInternalConverter(other.m_externalToInternalConverter),
      m_resolvedI2E(other.m_resolvedI2E),
      m_resolvedE2I(other.m_resolvedE2I)
{}

ParameterTypeInfo& ParameterTypeInfo::operator=(const ParameterTypeInfo& other){
    m_metaType = other.m_metaType;
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
      m_metaType(other.m_metaType),
      m_typeName(std::move(other.m_typeName)),
      m_javaClass(other.m_javaClass),
      m_internalToExternalConverter(std::move(other.m_internalToExternalConverter)),
      m_externalToInternalConverter(std::move(other.m_externalToInternalConverter)),
      m_resolvedI2E(other.m_resolvedI2E),
      m_resolvedE2I(other.m_resolvedE2I)
{}

ParameterTypeInfo& ParameterTypeInfo::operator=(ParameterTypeInfo&& other){
    m_metaType = other.m_metaType;
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
                                                                                            QMetaType(m_metaType),
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
                                                                                            QMetaType(m_metaType)
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

const QMetaType& ParameterTypeInfo::metaType() const{
    return m_metaType;
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
#if QT_VERSION >= QT_VERSION_CHECK(6,6,0)
    case QMetaObject::ConstructInPlace:
        if(JniEnvironment env{200}){
            q->invokeInPlaceConstructor(env, idx, argv);
        }
        return;
#endif
    default: break;
    }
}

void static_metacall_QtSubType(const QtJambiMetaObject* q, QObject * o, QMetaObject::Call cl, int idx, void ** argv)
{
    if(JniEnvironment env{200}){
        jobject object{nullptr};
        if(o){
            MetaObjectExceptionHandler __exceptionHandler;
            try{
                object = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, o, q->javaClass());
            }catch(const JavaException& exn){
                __exceptionHandler.handle(env, exn, nullptr);
            }
        }
        if(!env->IsSameObject(object, nullptr)){
            switch(cl){
            case QMetaObject::BindableProperty:
                q->bindableProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::ResetProperty:
                q->resetProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::ReadProperty:
                q->readProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::WriteProperty:
                q->writeProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::InvokeMetaMethod:
                q->invokeSignalOrSlot(env, object, nullptr, idx, argv, true);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,6,0)
            case QMetaObject::ConstructInPlace:
                q->invokeInPlaceConstructor(env, idx, argv);
                break;
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6,6,0)
        case QMetaObject::ConstructInPlace:
            q->invokeInPlaceConstructor(env, idx, argv);
            return;
#endif
        case QMetaObject::InvokeMetaMethod:
            if(!o){
                q->static_metacall(cl, idx + q->methodOffset(), argv);
                return;
            }
            break;
        default:
            break;
        }
        if(jobject object = reinterpret_cast<JObjectWrapper*>(o)->object(env)){
            switch(cl){
            case QMetaObject::BindableProperty:
                q->bindableProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::ResetProperty:
                q->resetProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::ReadProperty:
                q->readProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::WriteProperty:
                q->writeProperty(env, object, nullptr, idx, argv, true);
                break;
            case QMetaObject::InvokeMetaMethod:
                q->invokeSignalOrSlot(env, object, nullptr, idx, argv, true);
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

size_t computeHash(const QtJambiMetaObject* q, StaticMetacallFunction fct){
    return qHashMulti(0, q, quintptr(fct));
}

StaticMetaCallFunction create_static_metacall(const QtJambiMetaObject* q, StaticMetacallFunction fct){
    return qtjambi_function_pointer<512,void(QObject *, QMetaObject::Call, int, void **)>(
                [q,fct](QObject * o, QMetaObject::Call cl, int idx, void ** argv){
                    fct(q, o, cl, idx, argv);
                },
                computeHash(q, fct));
}

struct QmlExtensionData{
    jmethodID wrapper;
    QHash<jmethodID,jmethodID> methodMappings;
    QHash<jfieldID,jfieldID> fieldMappings;
};

class QtJambiMetaObjectPrivate
{
public:
    QtJambiMetaObjectPrivate(QtJambiMetaObject *q, JNIEnv *env, jclass java_class);
    ~QtJambiMetaObjectPrivate();

    void initialize(JNIEnv *jni_env, const QMetaObject *original_meta_object, bool hasCustomMetaObject);
    void invokeJavaMethod(JNIEnv *env, QtJambiScope& scope, jobject object, const JMethodInfo& methodInfo, bool keepOwnership, void **_a, bool forceObjectType = false) const;
    void invokeConstructor(JNIEnv *env, QtJambiScope& scope, const JConstructorInfo& methodInfo, void **_a) const;
    void invokeInPlaceConstructor(JNIEnv *env, QtJambiScope& scope, const JConstructorInfo& methodInfo, void **_a) const;

    int methodFromJMethod(jmethodID methodId) const;
    QtJambiMetaObject::SignalInfo signalInfo(JNIEnv *env, jfieldID fieldId, jmethodID emitMethodID) const;
    QVector<QtJambiMetaObject::SignalInfo> signalInfos(JNIEnv *env, jfieldID fieldId) const;
    const JMethodInfo* methodInfo(int index) const;
    const JConstructorInfo* constructorInfo(int index) const;
    jobject javaInstance(JNIEnv *env) const;
    void setJavaInstance(JNIEnv *env, jobject instance);
    static QtJambiMetaObjectPrivate* get(QtJambiMetaObject* obj);
    static const QtJambiMetaObjectPrivate* get(const QtJambiMetaObject* obj);
    static const QMetaObject *getQMetaObjectForJavaClass(JNIEnv *env, jclass object_class, const std::function<const QMetaObject *(bool&, bool&)>& original_meta_object_provider);
    static const QMetaObject *getQMetaObjectForJavaClass(JNIEnv *env, jclass object_class, const std::type_info& typeId);
private:
    template<class MethodInfoContainer>
    void analyzeMethods(JNIEnv *env, jobject classLoader, int count, jobject methodList, jobject listOfMetaTypes, MethodInfoContainer methodInfoContainer, QHash<jmethodID,int>* methodIndexes = nullptr);
    QByteArray m_stringData;
    QScopedArrayPointer<uint> m_intData;
    QtJambiMetaObject *q_ptr;
    mutable QSharedPointer<const QtJambiMetaObject> m_this_ptr;
    Q_DECLARE_PUBLIC(QtJambiMetaObject)

    jclass const m_clazz;
    QVector<JMethodInfo> m_methods;
    QHash<jmethodID,int> m_methodIndexes;
    QVector<JSignalInfo> m_signals;
    QHash<jfieldID,QHash<jmethodID,int>> m_signalIndexes;
    QVector<JConstructorInfo> m_constructors;
    JConstructorInfo m_privateConstructor;
    JConstructorInfo m_inPlaceConstructor;

    struct Property{
        JMethodInfo m_reader;
        JMethodInfo m_writer;
        JMethodInfo m_resetter;
        JMethodInfo m_bindable;
        QPropertyInfo m_QProperty_field;
        JMemberInfo m_member;
        inline JMethodInfo& reader(){return m_reader;}
        inline JMethodInfo& writer(){return m_writer;}
        inline JMethodInfo& resetter(){return m_resetter;}
        inline JMethodInfo& bindable(){return m_bindable;}
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
        typedef JMethodInfo value_type;
    };

    mutable jweak m_javaInstance;
    SuperTypeInfos m_superTypeInfos;

    QHash<jclass,QmlExtensionData> m_qmlExtensions;
    friend bool QmlAPI::registerQmlExtension(JNIEnv *env, const QMetaObject *extended_meta_object, jclass resolvedClass);
    friend void searchForMethods(JNIEnv *env, jclass declaringClass, const QVector<QtJambiMetaObjectPrivate::Property>& properties, jclass replacementClass, QHash<jmethodID,jmethodID>& methodMappings);
    friend QmlAPI::InPlaceConstructorInfo QmlAPI::findInPlaceConstructor(JNIEnv *env, jclass type, const QMetaObject *meta_object);
    friend void* QmlAPI::beginInPlaceConstruction(void* placement, const QMetaObject *meta_object, QtJambiAPI::ConstructorFn constructorFunction);
    friend void clearQtJambiStorage(JNIEnv* env, bool regular);
};

QtJambiMetaObjectPrivate::QtJambiMetaObjectPrivate(QtJambiMetaObject *q, JNIEnv *env, jclass java_class)
    :
      q_ptr(q), m_this_ptr(q),
      m_clazz(getGlobalClassRef(env, java_class)),
      m_methods(), m_methodIndexes(), m_signals(), m_signalIndexes(), m_constructors(),
      m_privateConstructor(), m_inPlaceConstructor(), m_properties(),
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
                sigInfo.m_signalTypes.clear(env);
            }
        }
    }else{
        if (DefaultJniEnvironment env{100}) {
            for(JSignalInfo& sigInfo : m_signals){
                sigInfo.m_signalTypes.clear(env);
            }
        }
    }

    m_methods.clear();
    m_methodIndexes.clear();
    m_signals.clear();
    m_signalIndexes.clear();
    m_constructors.clear();
    m_properties.clear();
    m_stringData.clear();
    m_qmlExtensions.clear();
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
void QtJambiMetaObjectPrivate::analyzeMethods(JNIEnv *env, jobject classLoader, int count, jobject methodList, jobject listOfMetaTypes, MethodInfoContainer methodInfoContainer, QHash<jmethodID,int>* methodIndexes){
    using MethodInfo = typename std::remove_reference_t<MethodInfoContainer>::value_type;
    Q_ASSERT(count == QtJambiAPI::sizeOfJavaCollection(env, methodList));
    Q_UNUSED(classLoader)
    QTJAMBI_JNI_LOCAL_FRAME(env, 32 + count);
    for(int i=0; i<count; ++i){
        jobject methodObject = Java::Runtime::List::get(env, methodList, i);
        if(methodObject){
            JConstObjectArrayPointer<jobject> metaTypes(env, listOfMetaTypes ? jobjectArray(Java::Runtime::List::get(env, listOfMetaTypes, i)) : nullptr);
            MethodInfo info;
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            jclass returnClassType(nullptr);
            jobjectArray parameterClassTypes(nullptr);
            if constexpr (std::is_same_v<MethodInfo,JConstructorInfo>){
                if(!Java::Runtime::Constructor::isInstanceOf(env, methodObject))
                    JavaException::raiseError(env, "Member is not a constructor." QTJAMBI_STACKTRACEINFO );
                info.constructorId = env->FromReflectedMethod(methodObject);
                returnClassType = Java::Runtime::Void::primitiveType(env);
                parameterClassTypes = Java::Runtime::Executable::getParameterTypes(env, methodObject);
                info.declaringClass = getGlobalClassRef(env, Java::Runtime::Constructor::getDeclaringClass(env, methodObject));
                if(m_superTypeInfos.size()>=1){
                    int length = env->GetArrayLength(parameterClassTypes);
                    auto superTypeInfo = m_superTypeInfos[0];
                    QMap<int,ResolvedConstructorInfo> foundConstructorFunction;
                    for(int j=0; j<superTypeInfo.constructorInfos().size(); ++j){
                        const ResolvedConstructorInfo & constructorInfo = superTypeInfo.constructorInfos().at(j);
                        if(constructorInfo.argumentTypes.size()<length
                            && !foundConstructorFunction.contains(length-constructorInfo.argumentTypes.size())){
                            bool matches = true;
                            for(int a=0; a<constructorInfo.argumentTypes.size(); ++a){
                                jclass given = jclass(env->GetObjectArrayElement(parameterClassTypes, a));
                                if(!env->IsAssignableFrom(constructorInfo.argumentTypes[a], given)){
                                    if(Java::Runtime::Collection::isSameClass(env, constructorInfo.argumentTypes[a])
                                        && Java::Runtime::Collection::isAssignableFrom(env, given)){
                                        continue;
                                    }else if(Java::Runtime::Map::isSameClass(env, constructorInfo.argumentTypes[a])
                                               && Java::Runtime::Map::isAssignableFrom(env, given)){
                                        continue;
                                    }else{
                                        matches = false;
                                        break;
                                    }
                                }
                            }
                            if(matches){
                                foundConstructorFunction[length-constructorInfo.argumentTypes.size()] = constructorInfo;
                            }
                        }
                    }
                    if(!foundConstructorFunction.isEmpty()){
                        ResolvedConstructorInfo res = *foundConstructorFunction.constBegin();
                        info.constructorFunction = res.constructorFunction;
                        info.constructorArgumentTypes = res.argumentTypes;
                    }
                }
            }else{
                info.methodId = env->FromReflectedMethod(methodObject);
                if(!Java::Runtime::Method::isInstanceOf(env, methodObject))
                    JavaException::raiseError(env, "Member is not a method." QTJAMBI_STACKTRACEINFO );
                returnClassType = Java::Runtime::Method::getReturnType(env, methodObject);
                parameterClassTypes = Java::Runtime::Executable::getParameterTypes(env, methodObject);
                int modif = Java::Runtime::Executable::getModifiers(env, methodObject);
                info.declaringClass = getGlobalClassRef(env, Java::Runtime::Method::getDeclaringClass(env, methodObject));
                info.isStatic = Java::Runtime::Modifier::isStatic(env, modif);
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
                            jobject mt = metaTypes[j];
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
                    if constexpr (std::is_same_v<MethodInfo,JConstructorInfo>){
                        info.parameterTypeInfos << ParameterTypeInfo::voidTypeInfo(env);
                        continue;
                    }else{
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
                                               qMetaType,
                                               typeName,
                                               getGlobalClassRef(env, javaClass),
                                               std::move(internalToExternalConverter),
                                               std::move(externalToInternalConverter)};
            }
            methodInfoContainer[i] = info;
            if(methodIndexes){
                if constexpr (std::is_same_v<MethodInfo,JConstructorInfo>){
                    methodIndexes->insert(info.constructorId, i);
                }else{
                    methodIndexes->insert(info.methodId, i);
                }
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
    m_superTypeInfos = SuperTypeInfos::fromClass(env, m_clazz);
    if(!m_superTypeInfos.isEmpty()){
        jobject privateConstructor = Java::QtJambi::MetaObjectData::privateConstructor(env, meta_data_struct);
        jobject inPlaceConstructor = Java::QtJambi::MetaObjectData::inPlaceConstructor(env, meta_data_struct);
        if(privateConstructor || inPlaceConstructor){
            for(int j=0; j<m_superTypeInfos[0].constructorInfos().size(); ++j){
                const ResolvedConstructorInfo & constructorInfo = m_superTypeInfos[0].constructorInfos().at(j);
                if(constructorInfo.argumentTypes.size()==0){
                    if(privateConstructor){
                        m_privateConstructor.constructorId = env->FromReflectedMethod(privateConstructor);
                        m_privateConstructor.constructorFunction = constructorInfo.constructorFunction;
                        m_privateConstructor.constructorArgumentTypes = constructorInfo.argumentTypes;
                        m_privateConstructor.declaringClass = m_clazz;
                        m_inPlaceConstructor.parameterTypeInfos.append(ParameterTypeInfo::voidTypeInfo(env));
                    }
                    if(inPlaceConstructor){
                        m_inPlaceConstructor.constructorId = env->FromReflectedMethod(inPlaceConstructor);
                        m_inPlaceConstructor.constructorFunction = constructorInfo.constructorFunction;
                        m_inPlaceConstructor.constructorArgumentTypes = constructorInfo.argumentTypes;
                        m_inPlaceConstructor.declaringClass = m_clazz;
                        m_inPlaceConstructor.parameterTypeInfos.append(ParameterTypeInfo::voidTypeInfo(env));
                    }
                    break;
                }
            }
        }
    }

    {
        jobject methods = Java::QtJambi::MetaObjectData::methods(env,meta_data_struct);
        jobject methodMetaTypes = Java::QtJambi::MetaObjectData::methodMetaTypes(env,meta_data_struct);
        int method_count = QtJambiAPI::sizeOfJavaCollection(env, methods);
        m_methods.resize(method_count);
        analyzeMethods<QVector<JMethodInfo>&>(env, classLoader, method_count, methods, methodMetaTypes, m_methods, &m_methodIndexes);
    }

    {
        jobject signalInfos = Java::QtJambi::MetaObjectData::signalInfos(env, meta_data_struct);
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
                jobject field = Java::QtJambi::MetaObjectData$SignalInfo::field(env, signalInfo);
                signal.signalName = qtjambi_cast<QString>(env, Java::Runtime::Field::getName(env, field));
                signal.signalField = env->FromReflectedField(field);
                jobject signalTypes = Java::QtJambi::MetaObjectData$SignalInfo::signalTypes(env, signalInfo);
                signal.m_signalTypes = JObjectWrapper(env, signalTypes);
                if(m_signalIndexes.contains(signal.signalField))
                    signal.isClone = true;

                m_signalIndexes[signal.signalField][signal.emitMethodInfo.methodId] = i;

                JIntArrayPointer signalMetaTypes(env, Java::QtJambi::MetaObjectData$SignalInfo::signalMetaTypes(env, signalInfo));
                int length = QtJambiAPI::sizeOfJavaCollection(env, signalTypes);
                signal.emitMethodInfo.parameterTypeInfos.reserve(length+1);
                signal.emitMethodInfo.parameterTypeInfos.append(ParameterTypeInfo::voidTypeInfo(env));
                jobject parameterClassTypes = QtJambiAPI::newJavaArrayList(env, length);
                for(int j=0; j<length; ++j){
                    jobject signalParameterType = Java::Runtime::List::get(env, signalTypes, j);
                    QMetaType qMetaType(signalMetaTypes.pointer()[j]);
                    jclass javaClass = Java::QtJambi::SignalUtility$SignalParameterType::type(env, signalParameterType);
                    QtJambiAPI::addToJavaCollection(env, parameterClassTypes, javaClass);
                    signal.emitMethodInfo.parameterTypeInfos.append({qMetaType,
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
        analyzeMethods<QVector<JConstructorInfo>&>(env, classLoader, constructor_count, methods, constructorMetaTypes, m_constructors);
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
        analyzeMethods<PropertyCollector<&Property::reader>>(env, classLoader, property_count, property_readers, propertyMetaTypes, m_properties);
        analyzeMethods<PropertyCollector<&Property::writer>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyWriters(env,meta_data_struct),
                        propertyMetaTypes, m_properties);
        analyzeMethods<PropertyCollector<&Property::resetter>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyResetters(env,meta_data_struct),
                        nullptr, m_properties);
        analyzeMethods<PropertyCollector<&Property::bindable>>(env, classLoader, property_count,
                        Java::QtJambi::MetaObjectData::propertyBindables(env,meta_data_struct),
                        nullptr, m_properties);
        jobject propertyQPropertyFields = Java::QtJambi::MetaObjectData::propertyQPropertyFields(env,meta_data_struct);
        Q_ASSERT(property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyQPropertyFields));
        for(int i=0; i<property_count; ++i){
            jobject fieldObject = Java::Runtime::List::get(env, propertyQPropertyFields, i);
            if(fieldObject){
                JConstObjectArrayPointer<jobject> metaTypes(env, jobjectArray(Java::Runtime::List::get(env, propertyMetaTypes, i)));
                jclass javaClass = jclass(Java::Runtime::List::get(env, propertyClassTypes, i));
                jobject mt = metaTypes[0];
                QMetaType qMetaType(Java::QtJambi::MetaObjectData$MetaTypeInfo::metaTypeId(env, mt));
                jstring jtypeName = Java::QtJambi::MetaObjectData$MetaTypeInfo::typeName(env, mt);
                QString typeName = jtypeName ? qtjambi_cast<QString>(env, jtypeName) : QLatin1String(qMetaType.name());
                ParameterTypeInfo propertyTypeInfo{qMetaType,
                                                   typeName,
                                                   getGlobalClassRef(env, javaClass)};
                QPropertyInfo& info = m_properties[i].m_QProperty_field;
                info.name = qtjambi_cast<QString>(env, Java::Runtime::Field::getName(env, fieldObject));
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

        jobject propertyMemberFields = Java::QtJambi::MetaObjectData::propertyMemberFields(env,meta_data_struct);
        Q_ASSERT(property_count == QtJambiAPI::sizeOfJavaCollection(env, propertyMemberFields));
        for(int i=0; i<property_count; ++i){
            jobject fieldObject = Java::Runtime::List::get(env, propertyMemberFields, i);
            if(fieldObject){
                JConstObjectArrayPointer<jobject> metaTypes(env, jobjectArray(Java::Runtime::List::get(env, propertyMetaTypes, i)));
                jobject mt = metaTypes[0];
                QMetaType qMetaType(Java::QtJambi::MetaObjectData$MetaTypeInfo::metaTypeId(env, mt));
                jstring jtypeName = Java::QtJambi::MetaObjectData$MetaTypeInfo::typeName(env, mt);
                QString typeName = jtypeName ? qtjambi_cast<QString>(env, jtypeName) : QLatin1String(qMetaType.name());
                jclass javaClass = jclass(Java::Runtime::List::get(env, propertyClassTypes, i));
                jclass fieldType = Java::Runtime::Field::getType(env, fieldObject);
                JMemberInfo& member = m_properties[i].m_member;
                member.name = qtjambi_cast<QString>(env, Java::Runtime::Field::getName(env, fieldObject));
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
                    member.memberTypeInfo = ParameterTypeInfo{qMetaType,
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
        q->d.relatedMetaObjects = new QMetaObject::SuperData(superType);
    }else if (extra_data_count > 1) {
        QTJAMBI_JNI_LOCAL_FRAME(env, 32 + extra_data_count);
        // ensure to not have a pointer to a static_metacall method
        QMetaObject::SuperData* _relatedMetaObjects = new QMetaObject::SuperData[size_t(extra_data_count)];
        for (jsize i=0; i<extra_data_count; ++i) {
            jclass el = jclass(Java::Runtime::List::get(env, extra_data, i));
            QMetaObject const* superType = CoreAPI::metaObjectForClass(env, el);
            _relatedMetaObjects[i].direct = superType;
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
}

bool isQmlExplicitCppOwnership(QObject * obj);

void QtJambiMetaObjectPrivate::invokeJavaMethod(JNIEnv *env, QtJambiScope& scope, jobject object, const JMethodInfo& methodInfo, bool keepOwnership, void **_a, bool forceObjectType) const
{
    QTJAMBI_JNI_LOCAL_FRAME(env, 32+methodInfo.parameterTypeInfos.size());
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
        if(env->ExceptionCheck()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        bool isVoid = false;
        if(methodInfo.isStatic){
            QTJAMBI_INTERNAL_METHOD_CALL_PRINT([&](QDebug& dbg){
                dbg << "JNIEnv::CallStaticMethod(" << methodInfo.toString(env) << ")";
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
                JavaException::raiseNullPointerException(env, QStringLiteral("Cannot invoke \"%1\" because object is null").arg(methodInfo.toString(env)) QTJAMBI_STACKTRACEINFO );
                return;
            }
            jmethodID methodId = methodInfo.methodId;
            if(!env->IsInstanceOf(object, methodInfo.declaringClass)){
                bool found = false;
                const QList<jclass> classes = m_qmlExtensions.keys();
                for(jclass cls : classes){
                    if(env->IsInstanceOf(object, cls)){
                        const QmlExtensionData& data = m_qmlExtensions[cls];
                        found = true;
                        if(data.wrapper){
                            if(env->ExceptionCheck()){
                                env->ExceptionDescribe();
                                env->ExceptionClear();
                            }
                            object = env->CallStaticObjectMethod(methodInfo.declaringClass, data.wrapper, object);
                        }else{
                            methodId = data.methodMappings[methodInfo.methodId];
                        }
                        break;
                    }
                }
                if(!found || !methodId){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot invoke \"%1\" on object of type \"%2\"").arg(
                                                                     methodInfo.toString(env),
                                                                     QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }
            }
            QTJAMBI_INTERNAL_METHOD_CALL_PRINT([&](QDebug& dbg){
                dbg << "JNIEnv::CallMethod(" << methodInfo.toString(env) << ")";
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
                if(!keepOwnership){
                    if(QMetaType(methodInfo.parameterTypeInfos[0].metaType()).flags() & (QMetaType::PointerToQObject)){
                        QObject* qobject = *reinterpret_cast<QObject**>(_a[0]);
                        if(qobject && !isQmlExplicitCppOwnership(qobject))
                            QtJambiAPI::setCppOwnershipForTopLevelObject(env, qobject);
                    }else if(QMetaType(methodInfo.parameterTypeInfos[0].metaType()).flags() & QMetaType::IsPointer){
                        void* pointer = *reinterpret_cast<void**>(_a[0]);
                        if(pointer){
                            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(pointer)){
                                link->setCppOwnership(env);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }else{
        qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeJavaMethod: Failed to convert arguments");
    }
}

/**
 * This method calls the constructor of a Java class caused by a meta object constructor call.
 */
void QtJambiMetaObjectPrivate::invokeConstructor(JNIEnv *env, QtJambiScope& scope, const JConstructorInfo& constructorInfo, void **_a) const
{
    JniLocalFrame __jniLocalFrame(env, 32 + constructorInfo.parameterTypeInfos.size());
    QVector<jvalue> converted_arguments(constructorInfo.parameterTypeInfos.size() - 1);
    bool success = true;
    for (int i = 1; i < constructorInfo.parameterTypeInfos.size(); ++i) {
        const ParameterTypeInfo& parameterTypeInfo = constructorInfo.parameterTypeInfos[i];
        jvalue& converted_argument = converted_arguments[i - 1];
        converted_argument.l = nullptr;
        if (!parameterTypeInfo.convertInternalToExternal(env, &scope, _a[i], converted_argument, !Java::Runtime::Class::isPrimitive(env, parameterTypeInfo.javaClass()))) {
            success = false;
            break;
        }
    }

    if (success) {
        jvalue *args = converted_arguments.data();
        QTJAMBI_INTERNAL_METHOD_CALL_PRINT([&](QDebug& dbg){
            dbg << "JNIEnv::NewObject(" << constructorInfo.toString(env) << ")";
        })
        jobject object = env->NewObjectA(m_clazz, constructorInfo.constructorId, args);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        QMetaType metaType = q_ptr->metaType();
        if(Java::QtJambi::QtObject::isAssignableFrom(env, m_clazz)){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
                if(!link->isQObject()){
                    link->setCppOwnership(env);
                }else if(!link->qobject()->parent()
                         && !isQmlExplicitCppOwnership(link->qobject())
                         ){
                    link->setCppOwnership(env);
                }
                if(!metaType.isValid() || (metaType.flags() & QMetaType::IsPointer)){
                    void* &pointer = *reinterpret_cast<void**>(_a[0]);
                    pointer = link->pointer();
                }else if(JObjectValueWrapper::isValueType(metaType)){
                    JObjectValueWrapper &val = *reinterpret_cast<JObjectValueWrapper*>(_a[0]);
                    val.assign(env, object);
                }else if(link->isQObject() && isNativeWrapperMetaType(metaType)){
                    JQObjectWrapper &val = *reinterpret_cast<JQObjectWrapper*>(_a[0]);
                    val = JQObjectWrapper(env, std::move(link));
                }else if(isJObjectWrappedMetaType(metaType)){
                    JObjectWrapper &val = *reinterpret_cast<JObjectWrapper*>(_a[0]);
                    val.assign(env, object);
                }
            }
        }else if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, m_clazz)){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object)){
                link->setCppOwnership(env);
                if(!metaType.isValid() || (metaType.flags() & QMetaType::IsPointer)){
                    void* &pointer = *reinterpret_cast<void**>(_a[0]);
                    pointer = link->pointer();
                }else if(JObjectValueWrapper::isValueType(metaType)){
                    JObjectValueWrapper &val = *reinterpret_cast<JObjectValueWrapper*>(_a[0]);
                    val.assign(env, object);
                }else if(isJObjectWrappedMetaType(metaType)){
                    JObjectWrapper &val = *reinterpret_cast<JObjectWrapper*>(_a[0]);
                    val.assign(env, object);
                }
            }
        }else if(JObjectValueWrapper::isValueType(metaType)){
            JObjectValueWrapper &val = *reinterpret_cast<JObjectValueWrapper*>(_a[0]);
            val.assign(JObjectValueWrapper::create(env, object, metaType));
        }else if(isJObjectWrappedMetaType(metaType)){
            JObjectWrapper &val = *reinterpret_cast<JObjectWrapper*>(_a[0]);
            val.assign(env, object);
        }else if(!metaType.isValid()){
            if(!Java::QtJambi::QtObjectInterface::isAssignableFrom(env, m_clazz)){
                JObjectWrapper &val = *reinterpret_cast<JObjectWrapper*>(_a[0]);
                val.assign(env, object);
            }
        }
    } else {
        qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeConstructor: Failed to convert arguments");
    }
}

/**
 * This method calls the constructor of a Java class caused by a meta object constructor call.
 */
void QtJambiMetaObjectPrivate::invokeInPlaceConstructor(JNIEnv *env, QtJambiScope& scope, const JConstructorInfo& constructorInfo, void **_a) const
{
    JniLocalFrame __jniLocalFrame(env, 32 + constructorInfo.parameterTypeInfos.size());
    QVector<jvalue> converted_arguments(constructorInfo.parameterTypeInfos.size());
    bool success = true;
    for (int i = 1; i < constructorInfo.parameterTypeInfos.size(); ++i) {
        const ParameterTypeInfo& parameterTypeInfo = constructorInfo.parameterTypeInfos[i];
        jvalue& converted_argument = converted_arguments[i];
        converted_argument.l = nullptr;
        if (!parameterTypeInfo.convertInternalToExternal(env, &scope, _a[i], converted_argument, !Java::Runtime::Class::isPrimitive(env, parameterTypeInfo.javaClass()))) {
            success = false;
            break;
        }
    }

    if (success) {
        if(m_inPlaceConstructor.constructorId){
            converted_arguments.takeFirst();
            InPlaceInitializer initializer(q_ptr, _a[0], 0, 0, constructorInfo, std::move(converted_arguments));
            jobject ipc = Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(&initializer));
            jobject object = env->NewObject(m_clazz, m_inPlaceConstructor.constructorId, ipc);
            QtJambiShellImpl::initializeNativeInterface(env, object, &initializer);
            Java::QtJambi::QtConstructInPlace::set_native_id(env, ipc, 0);
            initializer.reset(env);
        } else if(m_privateConstructor.constructorId){
            converted_arguments.takeFirst();
            InPlaceInitializer initializer(q_ptr, _a[0], 0, 0, constructorInfo, std::move(converted_arguments));
            jobject object = env->NewObject(m_clazz, m_privateConstructor.constructorId, nullptr);
            QtJambiShellImpl::initializeNativeInterface(env, object, &initializer);
            initializer.reset(env);
        } else {
            QMetaType metaType = q_ptr->metaType();
            if(JObjectValueWrapper::isValueType(metaType)){
                converted_arguments.remove(0);
                jvalue *args = converted_arguments.data();
                jobject object = env->NewObjectA(m_clazz, constructorInfo.constructorId, args);
                new (_a[0]) JObjectValueWrapper(JObjectValueWrapper::create(env, object, metaType));
            }else if(isJObjectWrappedMetaType(metaType)){
                converted_arguments.remove(0);
                jvalue *args = converted_arguments.data();
                jobject object = env->NewObjectA(m_clazz, constructorInfo.constructorId, args);
                new (_a[0]) JObjectWrapper(env, object);
            }else if(!metaType.isValid()){
                if(!Java::QtJambi::QtObjectInterface::isAssignableFrom(env, m_clazz)){
                    converted_arguments.remove(0);
                    jvalue *args = converted_arguments.data();
                    jobject object = env->NewObjectA(m_clazz, constructorInfo.constructorId, args);
                    new (_a[0]) JObjectWrapper(env, object);
                }
            }
        }
    }else{
        qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::invokeConstructor: Failed to convert arguments");
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
    uint offset = metaObject->d.stringdata[0];
    uint length = metaObject->d.stringdata[1];
    if(length==sizeof(const char*)){
        const char* stringdata = reinterpret_cast<const char*>(metaObject->d.stringdata);
        const char* __stringdata = stringdata + offset;
        const void*const pointer = *reinterpret_cast<const void*const*>(__stringdata);
        return pointer==QtJambiMetaObjectID;
    }
    return false;
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

const SuperTypeInfos& QtJambiMetaObject::superTypeInfos() const{
    return d_ptr->m_superTypeInfos;
}

bool QtJambiMetaObject::hasSignals() const{
    return !d_ptr->m_signals.isEmpty();
}

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
                    if(const JConstructorInfo* info = dynamicMetaObject->d_ptr->constructorInfo(i)){
                        if(info->constructorId)
                            return env->ToReflectedMethod(info->declaringClass, info->constructorId, false);
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

size_t qHash(const QMetaMethod& method, size_t seed = 0){
    return qHashMulti(seed, method.enclosingMetaObject(), method.methodIndex());
}

QVector<ParameterTypeInfo> QtJambiMetaObject::methodParameterInfo(JNIEnv * env, const QMetaMethod& method)
{
    QVector<ParameterTypeInfo> result;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        Q_ASSERT(method.isValid());
        const QMetaObject* metaObject = method.enclosingMetaObject();
        if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
            if(method.methodType()==QMetaMethod::Constructor){
                for(int i=0; i<metaObject->constructorCount(); ++i){
                    if(method==metaObject->constructor(i)){
                        if(const JConstructorInfo* info = dynamicMetaObject->d_ptr->constructorInfo(i)){
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
        size_t key = qHash(method);
        {
            QReadLocker locker(storage->lock());
            if(storage->parameterTypeInfos().contains(key))
                return storage->parameterTypeInfos()[key];
        }
        RegistryAPI::ParameterInfoProviderFn ptip = registeredParameterInfoProviderFn(metaObject);
        QList<RegistryAPI::ParameterInfo> parameterInfos;
        if(ptip && ptip(method, parameterInfos)){
            result.reserve(parameterInfos.size());
            for(const RegistryAPI::ParameterInfo& info : parameterInfos){
                if(!info.javaClass){
                    switch(info.metaTypeId){
                    case QMetaType::Void:
                        result.append(ParameterTypeInfo::voidTypeInfo(env));
                        break;
                    case QMetaType::UChar:
                    case QMetaType::Char:
                    case QMetaType::SChar:
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                    case QMetaType::Char32:
                    case QMetaType::UInt:
                    case QMetaType::Int:
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                            result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                            result.append(ParameterTypeInfo(
                                    QMetaType(info.metaTypeId),
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
                    case QMetaType::Char16:
                    case QMetaType::QChar:
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                        result.append(ParameterTypeInfo(
                                        QMetaType(info.metaTypeId),
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
                    result.append({QMetaType(info.metaTypeId),
                                    JavaAPI::resolveClass(env, info.javaClass),
                                    info.qtToJavaConverterFunction,
                                    info.javaToQtConverterFunction});
                }
            }
        }else{
            int length = method.parameterCount()+1;
            result.reserve(length);
            jobject classLoader = nullptr;
            QList<QByteArray> parameterTypes = method.parameterTypes();
            for (int i=0; i<length; ++i){
                QByteArray qTypeName;
                QMetaType metaType;
                if(i==0){
                    if(method.methodType()==QMetaMethod::Constructor){
                        result.append({QMetaType{QMetaType::UnknownType},nullptr,nullptr,nullptr});
                        continue;
                    }
                    qTypeName = method.typeName();
                    metaType = method.returnMetaType();
                }else{
                    qTypeName = parameterTypes.at(i-1).data();
                    metaType = method.parameterMetaType(i-1);
                }
                if(metaType.id()==QMetaType::UnknownType)
                    metaType = QMetaType::fromName(qTypeName);
                else if(metaType.id()!=QMetaType::UnknownType && qTypeName.isEmpty())
                    qTypeName = metaType.name();
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

                    result.append({metaType,
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
                                        Java::QtJambi::QNativePointer::invalidate(env, obj.object(env));
                                    }
                                });
                            }
                            return true;
                        };
                    }
                    result.append({QMetaType{QMetaType::UnknownType},nullptr,std::move(internalToExternalConverter),nullptr});
                }
            }
        }
        {
            QWriteLocker locker(storage->lock());
            storage->parameterTypeInfos()[key] = result;
        }
    }
    return result;
}

int QtJambiMetaObject::methodFromJMethod(const QMetaObject* metaObject, jmethodID methodId){
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return dynamicMetaObject->d_ptr->methodFromJMethod(methodId);
    }
    return -1;
}

QtJambiMetaObject::SignalInfo QtJambiMetaObject::signalInfo(JNIEnv *env, const QMetaObject* metaObject, jfieldID fieldId, jmethodID emitMethodID){
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return dynamicMetaObject->d_ptr->signalInfo(env, fieldId, emitMethodID);
    }
    return SignalInfo();
}

QVector<QtJambiMetaObject::SignalInfo> QtJambiMetaObject::signalInfos(JNIEnv *env, const QMetaObject* metaObject, jfieldID fieldId){
    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(metaObject)){
        return dynamicMetaObject->d_ptr->signalInfos(env, fieldId);
    }
    return {};
}

const JConstructorInfo* QtJambiMetaObjectPrivate::constructorInfo(int index) const{
    const JConstructorInfo* result = nullptr;
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

QtJambiMetaObject::SignalInfo QtJambiMetaObjectPrivate::signalInfo(JNIEnv *, jfieldID fieldId, jmethodID emitMethodId) const{
    Q_Q(const QtJambiMetaObject);
    QHash<jmethodID,int> indexesByEmit = m_signalIndexes.value(fieldId);
    int index(-1);
    if(emitMethodId){
        index = indexesByEmit.value(emitMethodId, -1);
    }else if(!indexesByEmit.isEmpty()){
        index = *indexesByEmit.begin();
    }
    if(index>=0){
        return QtJambiMetaObject::SignalInfo(q, index+q->methodOffset(), m_signals[index].m_signalTypes, m_signals[index].emitMethodInfo.declaringClass);
    }
    return QtJambiMetaObject::SignalInfo();
}

QVector<QtJambiMetaObject::SignalInfo> QtJambiMetaObjectPrivate::signalInfos(JNIEnv *, jfieldID fieldId) const{
    Q_Q(const QtJambiMetaObject);
    QHash<jmethodID,int> indexesByEmit = m_signalIndexes.value(fieldId);
    QVector<QtJambiMetaObject::SignalInfo> result;
    for(int index : qAsConst(indexesByEmit)){
        result << QtJambiMetaObject::SignalInfo(q, index+q->methodOffset(), m_signals[index].m_signalTypes, m_signals[index].emitMethodInfo.declaringClass);
    }
    return result;
}

int QtJambiMetaObject::metaCall(QObject * o, QMetaObject::Call cl, int idx, void **argv){
    if(JniEnvironment env{200}){
        switch(cl){
        case QMetaObject::CreateInstance:
            return invokeConstructor(env, idx, argv);
#if QT_VERSION >= QT_VERSION_CHECK(6,6,0)
        case QMetaObject::ConstructInPlace:
            return invokeInPlaceConstructor(env, idx, argv);
#endif
        case QMetaObject::InvokeMetaMethod:
            if(!o){
                return static_metacall(cl, idx + methodOffset(), argv);
            }
            break;
        default:
            break;
        }
        jobject object{nullptr};
        if(o){
            MetaObjectExceptionHandler __exceptionHandler;
            try{
                object = QtJambiAPI::convertQObjectToJavaObject(env, o);
            }catch(const JavaException& exn){
                __exceptionHandler.handle(env, exn, o);
            }
        }
        if(!env->IsSameObject(object, nullptr)){
            switch(cl){
            case QMetaObject::BindableProperty:
                bindableProperty(env, object, o, idx, argv, true);
                break;
            case QMetaObject::ResetProperty:
                resetProperty(env, object, o, idx, argv, true);
                break;
            case QMetaObject::ReadProperty:
                readProperty(env, object, o, idx, argv, true);
                break;
            case QMetaObject::WriteProperty:
                writeProperty(env, object, o, idx, argv, true);
                break;
            case QMetaObject::InvokeMetaMethod:
                invokeSignalOrSlot(env, object, o, idx, argv, true);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,6,0)
        case QMetaObject::ConstructInPlace:
            return invokeInPlaceConstructor(env, idx, argv);
#endif
        case QMetaObject::InvokeMetaMethod:
            return static_metacall(cl, idx + methodOffset(), argv);
        default:
            break;
        }
    }
    return idx;
}

int QtJambiMetaObject::invokeSignalOrSlot(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->invokeSignalOrSlot(env, object, qobject, _id, _a);
    if (_id < 0) return _id;

    // Emit the correct signal
    if (_id < d->m_signals.size()) {
        const JSignalInfo& signal = d->m_signals[_id];
        if(qobject && !signal.isClone){
            QMetaObject::activate(qobject, this, _id, _a);
        }else{
            QtJambiScope scope(nullptr);
            MetaObjectExceptionHandler __exceptionHandler;
            try{
                if(env->IsSameObject(object, nullptr)){
                    JavaException::raiseNullPointerException(env, QStringLiteral("Cannot read field \"%1.%2\" because object is null").arg(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), signal.signalName) QTJAMBI_STACKTRACEINFO );
                }else if(!env->IsInstanceOf(object, d->m_clazz)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot read field \"%1.%2\" on object of type \"%3\"").arg(
                                                                     QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), signal.signalName,
                                                                     QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    jobject signal_object{nullptr};
                    MetaObjectExceptionHandler __exceptionHandler;
                    try{
                        signal_object = env->GetObjectField(object, signal.signalField);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    }catch(const JavaException& exn){
                        __exceptionHandler.handle(env, exn, qobject);
                    }
                    d->invokeJavaMethod(env, scope, signal_object, signal.emitMethodInfo, false, _a);
                }
            }catch(const JavaException& exn){
                __exceptionHandler.handle(env, exn, qobject);
            }
        }
    } else if (_id < d->m_signals.size() + d->m_methods.size()) { // Call the correct method
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            d->invokeJavaMethod(env, scope, object, d->m_methods[_id - d->m_signals.size()], false, _a);
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, qobject);
        }
    }
    return _id - d->m_methods.size() - d->m_signals.size() - d->m_constructors.size();
}

int QtJambiMetaObject::invokeConstructor(JNIEnv *env, int _id, void **_a) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_constructors.size()) { // Call the correct constructor
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            d->invokeConstructor(env, scope, d->m_constructors[_id], _a);
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, nullptr);
        }
    }
    return _id - d->m_constructors.size();
}

int QtJambiMetaObject::invokeInPlaceConstructor(JNIEnv *env, int _id, void **_a) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_constructors.size()) { // Call the correct constructor
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            d->invokeInPlaceConstructor(env, scope, d->m_constructors[_id], _a);
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, nullptr);
        }
    }
    return _id - d->m_constructors.size();
}

int QtJambiMetaObject::readProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->readProperty(env, object, qobject, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            if(property.m_reader){
                d->invokeJavaMethod(env, scope, object, property.m_reader, true, _a);
            }else if (property.m_QProperty_field && property.m_QProperty_field.propertyField && property.m_QProperty_field.valueMethod.methodId){
                if(env->IsSameObject(object, nullptr)){
                    JavaException::raiseNullPointerException(env, QStringLiteral("Cannot read field \"%1.%2\" because object is null").arg(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_QProperty_field.name) QTJAMBI_STACKTRACEINFO );
                }else if(!env->IsInstanceOf(object, d->m_clazz)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot read field \"%1.%2\" on object of type \"%3\"").arg(
                                                                     QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_QProperty_field.name,
                                                                     QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    jobject qProperty = env->GetObjectField(object, property.m_QProperty_field.propertyField);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    d->invokeJavaMethod(env, scope, qProperty, property.m_QProperty_field.valueMethod, true, _a);
                }
            }else if (property.m_member){
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
                        const QList<jclass> classes = d->m_qmlExtensions.keys();
                        for(jclass cls : classes){
                            if(env->IsInstanceOf(object, cls)){
                                const QmlExtensionData& data = d->m_qmlExtensions[cls];
                                found = true;
                                if(data.wrapper){
                                    if(env->ExceptionCheck()){
                                        env->ExceptionDescribe();
                                        env->ExceptionClear();
                                    }
                                    object = env->CallStaticObjectMethod(d->m_clazz, data.wrapper, object);
                                }else{
                                    fieldId = data.fieldMappings[fieldId];
                                }
                                break;
                            }
                        }
                        if(!found || !fieldId){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot read field \"%1.%2\" on object of type \"%3\"").arg(
                                                                             QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_member.name,
                                                                             QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
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
            }
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, qobject);
        }
    }
    return _id - d->m_properties.size();
}

int QtJambiMetaObject::writeProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->writeProperty(env, object, qobject, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            if(property.m_writer){
                // invokeJavaMethod expects a place holder for return value, but write property meta calls
                // do not since all property writers return void by convention.
                void *a[2] = { nullptr, _a[0] };
                d->invokeJavaMethod(env, scope, object, property.m_writer, true, a);
            }else if (property.m_QProperty_field && property.m_QProperty_field.propertyField && property.m_QProperty_field.setValueMethod.methodId){
                if(env->IsSameObject(object, nullptr)){
                    JavaException::raiseNullPointerException(env, QStringLiteral("Cannot read field \"%1.%2\" because object is null").arg(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_QProperty_field.name) QTJAMBI_STACKTRACEINFO );
                }else if(!env->IsInstanceOf(object, d->m_clazz)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot read field \"%1.%2\" on object of type \"%3\"").arg(
                                                                     QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_QProperty_field.name,
                                                                     QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    jobject qProperty = env->GetObjectField(object, property.m_QProperty_field.propertyField);
                    void *a[2] = { nullptr, _a[0] };
                    d->invokeJavaMethod(env, scope, qProperty, property.m_QProperty_field.setValueMethod, true, a, property.m_QProperty_field.valueMethod.methodType==JMethodType::l);
                }
            }else if (property.m_member && property.m_member.member && property.m_member.canWrite){
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
                        const QList<jclass> classes = d->m_qmlExtensions.keys();
                        for(jclass cls : classes){
                            if(env->IsInstanceOf(object, cls)){
                                const QmlExtensionData& data = d->m_qmlExtensions[cls];
                                found = true;
                                if(data.wrapper){
                                    if(env->ExceptionCheck()){
                                        env->ExceptionDescribe();
                                        env->ExceptionClear();
                                    }
                                    object = env->CallStaticObjectMethod(d->m_clazz, data.wrapper, object);
                                }else{
                                    fieldId = data.fieldMappings[fieldId];
                                }
                                break;
                            }
                        }
                        if(!found || !fieldId){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot assign field \"%1.%2\" on object of type \"%3\"").arg(
                                                                             QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_member.name,
                                                                             QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
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
            }
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, qobject);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::resetProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->resetProperty(env, object, qobject, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            if(property.m_resetter){
                d->invokeJavaMethod(env, scope, object, property.m_resetter, true, _a);
            }
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, qobject);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::notifyProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->notifyProperty(env, object, qobject, _id, _a);
    if (_id < 0) return _id;

    int property_notifier;
    if (_id < d->m_properties.size()
            && (property_notifier = d->m_properties[_id].m_notifier)>=0
            && property_notifier<=d->m_signals.size()) {
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            const JSignalInfo& signal = d->m_signals[property_notifier];
            if(signal.signalField && signal.emitMethodInfo.methodId){
                if(env->IsSameObject(object, nullptr)){
                    JavaException::raiseNullPointerException(env, QStringLiteral("Cannot read field \"%1.%2\" because object is null").arg(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), signal.signalName) QTJAMBI_STACKTRACEINFO );
                }else if(!env->IsInstanceOf(object, d->m_clazz)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot read field \"%1.%2\" on object of type \"%3\"").arg(
                                                                     QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), signal.signalName,
                                                                     QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    jobject signal_object = env->GetObjectField(object, signal.signalField);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    d->invokeJavaMethod(env, scope, signal_object, signal.emitMethodInfo, true, _a);
                }
            }
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, qobject);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::bindableProperty(JNIEnv *env, jobject object, QObject* qobject, int _id, void **_a, bool direct) const
{
    Q_D(const QtJambiMetaObject);

    const QMetaObject *super_class = superClass();
    if (!direct && super_class && QtJambiMetaObject::isInstance(super_class))
        _id = static_cast<const QtJambiMetaObject *>(super_class)->bindableProperty(env, object, qobject, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        QtJambiScope scope(nullptr);
        MetaObjectExceptionHandler __exceptionHandler;
        try{
            if(property.m_bindable){
                d->invokeJavaMethod(env, scope, object, property.m_bindable, true, _a);
            }else if (property.m_QProperty_field){
                const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
                if(env->IsSameObject(object, nullptr)){
                    JavaException::raiseNullPointerException(env, QStringLiteral("Cannot read field \"%1.%2\" because object is null").arg(QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_QProperty_field.name) QTJAMBI_STACKTRACEINFO );
                }else if(!env->IsInstanceOf(object, d->m_clazz)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot read field \"%1.%2\" on object of type \"%3\"").arg(
                                                                     QtJambiAPI::getClassName(env, d->m_clazz).replace('$', '.'), property.m_QProperty_field.name,
                                                                     QtJambiAPI::getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    jobject qProperty = env->GetObjectField(object, property.m_QProperty_field.propertyField);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    jobject bindable = Java::QtCore::QBindable::createBinding(env, qProperty);
                    *reinterpret_cast<QUntypedBindable*>(_a[0]) = qtjambi_cast<QUntypedBindable>(env, bindable);
                }
            }
        }catch(const JavaException& exn){
            __exceptionHandler.handle(env, exn, qobject);
        }
    }

    return _id - d->m_properties.size();
}

int QtJambiMetaObject::revision() const{
    return d_ptr->m_intData[0];
}

jclass QtJambiMetaObject::typeOfProperty(int _id) const
{
    Q_D(const QtJambiMetaObject);
    if (_id < d->m_properties.size()) {
        const QtJambiMetaObjectPrivate::Property& property = d->m_properties[_id];
        if (property.m_reader) {
            Q_ASSERT(property.m_reader.parameterTypeInfos.size()>1);
            return property.m_reader.parameterTypeInfos[1].javaClass();
        }
        if (property.m_QProperty_field && property.m_QProperty_field.valueMethod.methodId) {
            Q_ASSERT(property.m_QProperty_field.valueMethod.parameterTypeInfos.size()>0);
            return property.m_QProperty_field.valueMethod.parameterTypeInfos[0].javaClass();
        }
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
        if (property.m_QProperty_field && property.m_QProperty_field.valueMethod.methodId) {
            Q_ASSERT(property.m_QProperty_field.valueMethod.parameterTypeInfos.size()>0);
            return property.m_QProperty_field.valueMethod.parameterTypeInfos[0].m_internalToExternalConverter;
        }
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
        if (property.m_QProperty_field && property.m_QProperty_field.valueMethod.methodId) {
            Q_ASSERT(property.m_QProperty_field.valueMethod.parameterTypeInfos.size()>0);
            return property.m_QProperty_field.valueMethod.parameterTypeInfos[0].m_externalToInternalConverter;
        }
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
        const_cast<JSignalInfo&>(sigInfo).m_signalTypes.clear(env);
    }
    return pointer;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
QList<const QtPrivate::QMetaTypeInterface *> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass);
void registerMetaObjectByMetaTypeInterface(const QtPrivate::QMetaTypeInterface* iface, const QMetaObject* mo);
#endif


const QMetaObject *QtJambiMetaObjectPrivate::getQMetaObjectForJavaClass(JNIEnv *env, jclass object_class, const std::function<const QMetaObject *(bool&,bool&)>& original_meta_object_provider)
{
    const QMetaObject *returned = nullptr;
    QtJambiStorage* storage = getQtJambiStorage();
    Q_ASSERT(object_class != nullptr);
    Java::QtJambi::QtUtilities::initializePackage(env, object_class);

    // If original_meta_object is null then we have to look it up

    int classHash = Java::Runtime::Object::hashCode(env,object_class);
    {
        QReadLocker locker(storage->lock());
        if(storage->metaObjects().contains(classHash))
            return storage->metaObjects().value(classHash, nullptr);
    }
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
            QWriteLocker locker(storage->lock());
            // check if someone added a meta object meanwhile
            if(storage->metaObjects().contains(classHash)){
                const QMetaObject *_returned = storage->metaObjects().value(classHash, nullptr);
                if(dynamicResult && dynamicResult!=_returned){
                    locker.unlock();
                    (void)dynamicResult->dispose(env);
                    locker.unlock();
                }
                return _returned;
            }
            storage->metaObjects().insert(classHash, returned);
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
        for(const QtPrivate::QMetaTypeInterface * iface : registeredCustomMetaTypesForJavaClass(QtJambiAPI::getClassName(env, object_class).toLatin1().replace('.', '/'))){
            registerMetaObjectByMetaTypeInterface(iface, returned);
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
                                                    return registeredNamespaceMetaObject(class_name);
                                                } else {
                                                    Q_ASSERT(!class_name.isEmpty());
                                                    if(const std::type_info* typeId = getTypeByJavaName(class_name)){
                                                        const QMetaObject* superType = customMetaObject(*typeId);
                                                        return (basedOnCustomMetaObject = superType) ? superType : registeredOriginalMetaObject(*typeId);
                                                    }else if(jclass _object_class = JavaAPI::resolveClosestQtSuperclass(env, object_class)){
                                                        class_name = QtJambiAPI::getClassName(env, _object_class).replace(".", "/");
                                                        Q_ASSERT(!class_name.isEmpty());
                                                        if(const std::type_info* typeId = getTypeByJavaName(class_name)){
                                                            const QMetaObject* superType = customMetaObject(*typeId);
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
                                            const QMetaObject* superType = customMetaObject(typeId);
                                            return (basedOnCustomMetaObject = superType) ? superType : registeredOriginalMetaObject(typeId);
                                        }
                                    );
}

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
        searchForMethods(env, declaringClass, prop.m_bindable, replacementClass, methodMappings);
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

jobject QtJambiMetaObject::getSignalTypes(JNIEnv *env, jobject signal, const QMetaMethod& metaMethod){
    jobject result = nullptr;
    QtJambiStorage* storage = getQtJambiStorage();
    size_t key = qHash(metaMethod);
    {
        QReadLocker locker(storage->lock());
        if(storage->signalTypes().contains(key))
            result = storage->signalTypes()[key].object(env);
    }
    if(!env->IsSameObject(result, nullptr))
        return result;
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
        const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, metaMethod);
        jobjectArray signalParameterTypes = env->NewObjectArray(jsize(parameterTypeInfos.size()-1), Java::Runtime::Class::getClass(env), nullptr);
        for (decltype(parameterTypeInfos.size()) i=1; i<parameterTypeInfos.size(); ++i){
            env->SetObjectArrayElement(signalParameterTypes, jsize(i-1), parameterTypeInfos[i].javaClass());
        }
        signal = signalParameterTypes;
    }
    result = Java::QtJambi::SignalUtility::resolveSignal(env, signal, argumentIsPointerType, argumentIsReferenceType);
    {
        QWriteLocker locker(storage->lock());
        bool isFound = false;
        if(storage->signalTypes().contains(key)){
            jobject _result = storage->signalTypes()[key].object(env);
            if(!env->IsSameObject(result, nullptr)){
                isFound = true;
                result = _result;
            }
        }
        if(!isFound)
            storage->signalTypes().insert(key, JObjectWrapper(env, result));
    }
    return result;
}

const QMetaObject* QtJambiMetaObject::findMetaObject(const char* name){
    const QMetaObject* metaObject = nullptr;
    if(const std::type_info* typeId = getTypeByQtName(name)){
        metaObject = registeredOriginalMetaObject(*typeId);
    }
    if(!metaObject){
        auto hash = qHash(name);
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker locker(storage->lock());
            metaObject = storage->metaObjectsByName().value(hash, nullptr);
        }
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
        QtJambiStorage* storage = getQtJambiStorage();
#if !defined(QTJAMBI_NO_GLOBALREFS)
        jweak javaInstance;
        {
            QReadLocker locker(storage->lock());
            javaInstance = storage->javaMetaObjects().value(metaObject, nullptr);
        }
#else
        jobject javaInstance = Java::QtJambi::NativeUtility::getWeakGlobalReference(env, jlong(reinterpret_cast<const void*>(metaObject)));
#endif
        if(env->IsSameObject(javaInstance, nullptr)){
#if !defined(QTJAMBI_NO_GLOBALREFS)
            if(javaInstance)
                env->DeleteWeakGlobalRef(javaInstance);
#endif
            jobject jmetaObject = Java::QtCore::QMetaObject::newInstance(env, jlong(reinterpret_cast<const void*>(metaObject)));
            QLatin1String javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if(javaName.isEmpty()){
                if(const std::type_info* typeId = getTypeByQtName(metaObject->className()))
                    javaName = QLatin1String(getJavaName(*typeId));
            }
            {
                QWriteLocker locker(storage->lock());
                if(javaName.isEmpty()){
                    storage->metaObjectsByName().insert(qHash(metaObject->className()), metaObject);
                }
#if !defined(QTJAMBI_NO_GLOBALREFS)
                storage->javaMetaObjects().insert(metaObject, env->NewWeakGlobalRef(jmetaObject));
            }
#else
            }
            Java::QtJambi::NativeUtility::pushWeakGlobalReference(env, jlong(reinterpret_cast<const void*>(metaObject)), jmetaObject);
#endif
            return jmetaObject;
        }else{
#if !defined(QTJAMBI_NO_GLOBALREFS)
            return env->NewLocalRef(javaInstance);
#else
            return javaInstance;
#endif
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
                if(const QVector<const RegistryAPI::SignalMetaInfo>* infos = signalMetaInfos(metaObject)){
                    QList<QString> signalOrder;
                    QMap<QString,QList<const RegistryAPI::SignalMetaInfo*> > sortedSignals;
                    for(const RegistryAPI::SignalMetaInfo& info : *infos){
                        QString key = QLatin1String(info.signal_name);
                        if(!signalOrder.contains(key))
                            signalOrder.append(key);
                        sortedSignals[key].append(&info);
                    }
                    for(const QString& key : signalOrder){
                        const QList<const RegistryAPI::SignalMetaInfo*>& signalsByName = sortedSignals[key];
                        if(signalsByName.size()==1){
                            const RegistryAPI::SignalMetaInfo& info = *signalsByName.first();
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
                            const RegistryAPI::SignalMetaInfo& first = *signalsByName.first();
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
                                    const RegistryAPI::SignalMetaInfo& info = *signalsByName.at(i);
                                    int signal_method_index = -1;
                                    if(info.signal_method_index_provider){
                                        signal_method_index = info.signal_method_index_provider();
                                    }
                                    QString signalSignature = QLatin1String(info.signal_signature);
                                    if(signalSignature.startsWith('L') && signalSignature.endsWith(';'))
                                        signalSignature = signalSignature.mid(1, signalSignature.length()-2);
                                    env->SetObjectArrayElement(signalObjectTypes, jsize(i), JavaAPI::resolveClass(env, qPrintable(signalSignature), nullptr));
                                    QMetaMethod metaMethod = metaMethods.take(signal_method_index);
                                    const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, metaMethod);
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
                            jobject signal = env->GetObjectField(java_object, signalFieldId);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            if(signal){
                                jobject signalTypes = mo->d_ptr->m_signals[metaMethod.methodIndex()-metaObject->methodOffset()].m_signalTypes.object(env);
                                if(!signalTypes){
                                    signalTypes = QtJambiMetaObject::getSignalTypes(env, signalField, metaMethod);
                                }
                                Java::QtJambi::SignalUtility$AbstractSignal::initializeSignal(env, signal, java_class, signalTypes, metaMethod.methodIndex(), jlong(mo));
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

QmlAPI::InPlaceConstructorInfo QmlAPI::findInPlaceConstructor(JNIEnv *env, jclass type, const QMetaObject *meta_object){
    if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(meta_object)){
        const QtJambiMetaObjectPrivate* p = QtJambiMetaObjectPrivate::get(mo);
        if(p->m_inPlaceConstructor.constructorId){
            return {p->m_inPlaceConstructor.constructorId, false, nullptr};
        }
        if(p->m_privateConstructor.constructorId){
            return {p->m_privateConstructor.constructorId, true, nullptr};
        }
    }else{
        jmethodID constructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/QtObject$QPrivateConstructor;)V", type);
#ifdef Q_OS_ANDROID
        if(constructor){
            jobject mtd = env->ToReflectedMethod(type, constructor, false);
            if(!env->IsSameObject(type, Java::Runtime::Constructor::getDeclaringClass(env, mtd))){
                constructor = nullptr;
            }
        }
#endif
        if(constructor){
            if(const std::type_info* typeId = getTypeByMetaObject(meta_object)){
                if(const QVector<const RegistryAPI::ConstructorInfo>* infos = registeredConstructorInfos(*typeId)){
                    for(const RegistryAPI::ConstructorInfo& info : *infos){
                        if(!info.signature && info.constructorFunction){
                            return {constructor, true, info.constructorFunction};
                        }
                    }
                }
            }
            SuperTypeInfos typeInfos = SuperTypeInfos::fromClass(env, type);
            if(!typeInfos.isEmpty()){
                for(const ResolvedConstructorInfo& constructorInfo : typeInfos[0].constructorInfos()){
                    if(constructorInfo.argumentTypes.isEmpty()){
                        return {constructor, true, constructorInfo.constructorFunction};
                    }
                }
            }
        }
    }
    return {};
}

void* QmlAPI::beginInPlaceConstruction(void* placement, const QMetaObject *meta_object, QtJambiAPI::ConstructorFn constructorFunction){
    if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(meta_object)){
        const QtJambiMetaObjectPrivate* p = QtJambiMetaObjectPrivate::get(mo);
        if(p->m_inPlaceConstructor.constructorId){
            return new InPlaceInitializer(mo, placement, 0, 0, p->m_inPlaceConstructor, {}, true);
        }else{
            return new InPlaceInitializer(mo, placement, 0, 0, p->m_privateConstructor, {}, true);
        }
    }else{
        return new InPlaceInitializer(mo, placement, 0, 0, constructorFunction, true);
    }
}

void QmlAPI::endInPlaceConstruction(JNIEnv *env, jobject object, void* pointer){
    InPlaceInitializer* initializer = reinterpret_cast<InPlaceInitializer*>(pointer);
    QtJambiShellImpl::initializeNativeInterface(env, object, initializer);
    initializer->reset(env);
    delete initializer;
}
