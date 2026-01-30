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

#include "pch_p.h"
#include <QtCore/private/qmetaobject_p.h>
#include <QtCore/private/qmetaobjectbuilder_p.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
typedef QVariant (*MoveConstruct)(QMetaType type, void *data);
template<> inline MoveConstruct qvariant_cast<MoveConstruct>(const QVariant &)
{
    return QVariant::moveConstruct;
}
QVariant VariantUtility::createVariant(QMetaType type, void *data){
    static MoveConstruct moveConstruct = qvariant_cast<MoveConstruct>(*reinterpret_cast<QVariant*>(1));
    if(type.iface()->moveCtr)
        return moveConstruct(type, data);
    else
        return QVariant(type, data);
}
#endif
QVariant VariantUtility::createVariant(const QMetaType& type, const void *data){
    return QVariant(type, data);
}


struct InternalToExternalConverterPrivate : QSharedData{
    void* data = nullptr;
    QtJambiUtils::InternalToExternalConverter::Invoker invoker;
    QtJambiUtils::InternalToExternalConverter::Deleter deleter;
    InternalToExternalConverterPrivate(void* _data, QtJambiUtils::InternalToExternalConverter::Invoker _invoker, QtJambiUtils::InternalToExternalConverter::Deleter _deleter = nullptr)
        : QSharedData(), data(_data), invoker(_invoker), deleter(_deleter){
        Q_ASSERT(_data);
        Q_ASSERT(_invoker);
    }
    ~InternalToExternalConverterPrivate(){ if(deleter) deleter(data); }
};

namespace QtJambiUtils{

InternalToExternalConverter::InternalToExternalConverter(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new InternalToExternalConverterPrivate(
            reinterpret_cast<void*>(functor),
            [](void* data, JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType) -> bool {
                FunctionPointer task = reinterpret_cast<FunctionPointer>(data);
                return (*task)(env, scope, in, out, forceBoxedType);
            })){}

InternalToExternalConverter::InternalToExternalConverter() noexcept : d(){}
InternalToExternalConverter::~InternalToExternalConverter() noexcept {}
InternalToExternalConverter::InternalToExternalConverter(const InternalToExternalConverter& other) noexcept : d(other.d) {}
InternalToExternalConverter::InternalToExternalConverter(InternalToExternalConverter&& other) noexcept : d(std::move(other.d)) {}
InternalToExternalConverter::InternalToExternalConverter(void* data, Invoker invoker, Deleter deleter) noexcept
    : d(!data ? nullptr : new InternalToExternalConverterPrivate(data, invoker, deleter)) {}
InternalToExternalConverter& InternalToExternalConverter::operator=(const InternalToExternalConverter& other) noexcept { d = other.d; return *this; }
InternalToExternalConverter& InternalToExternalConverter::operator=(InternalToExternalConverter&& other) noexcept { d = std::move(other.d); return *this; }
bool InternalToExternalConverter::operator==(const InternalToExternalConverter& other) const noexcept { return d == other.d; }

InternalToExternalConverter::operator bool() const noexcept{
    return d;
}

bool InternalToExternalConverter::operator !() const noexcept{
    return !d;
}

bool InternalToExternalConverter::operator()(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType) const{
    Q_ASSERT(d);
    return d->invoker(d->data, env, scope, in, out, forceBoxedType);
}

} // namespace QtJambiPrivate

struct ExternalToInternalConverterPrivate : QSharedData{
    void* data = nullptr;
    QtJambiUtils::ExternalToInternalConverter::Invoker invoker;
    QtJambiUtils::ExternalToInternalConverter::Deleter deleter;
    ExternalToInternalConverterPrivate(void* _data, QtJambiUtils::ExternalToInternalConverter::Invoker _invoker, QtJambiUtils::ExternalToInternalConverter::Deleter _deleter = nullptr)
        : QSharedData(), data(_data), invoker(_invoker), deleter(_deleter){
        Q_ASSERT(_data);
        Q_ASSERT(_invoker);
    }
    ~ExternalToInternalConverterPrivate(){ if(deleter) deleter(data); }
};

namespace QtJambiUtils{

ExternalToInternalConverter::ExternalToInternalConverter(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new ExternalToInternalConverterPrivate(
            reinterpret_cast<void*>(functor),
            [](void* data, JNIEnv* env, QtJambiScope* scope, jvalue in, void*& out, jValueType type) -> bool {
                FunctionPointer task = reinterpret_cast<FunctionPointer>(data);
                return (*task)(env, scope, in, out, type);
            })){}

ExternalToInternalConverter::ExternalToInternalConverter() noexcept : d(){}
ExternalToInternalConverter::~ExternalToInternalConverter() noexcept {}
ExternalToInternalConverter::ExternalToInternalConverter(const ExternalToInternalConverter& other) noexcept : d(other.d) {}
ExternalToInternalConverter::ExternalToInternalConverter(ExternalToInternalConverter&& other) noexcept : d(std::move(other.d)) {}
ExternalToInternalConverter::ExternalToInternalConverter(void* data, Invoker invoker, Deleter deleter) noexcept
    : d(!data ? nullptr : new ExternalToInternalConverterPrivate(data, invoker, deleter)) {}
ExternalToInternalConverter& ExternalToInternalConverter::operator=(const ExternalToInternalConverter& other) noexcept { d = other.d; return *this; }
ExternalToInternalConverter& ExternalToInternalConverter::operator=(ExternalToInternalConverter&& other) noexcept { d = std::move(other.d); return *this; }
bool ExternalToInternalConverter::operator==(const ExternalToInternalConverter& other) const noexcept { return d == other.d; }

ExternalToInternalConverter::operator bool() const noexcept{
    return d;
}

bool ExternalToInternalConverter::operator !() const noexcept{
    return !d;
}

bool ExternalToInternalConverter::operator()(JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) const{
    Q_ASSERT(d);
    return d->invoker(d->data, env, scope, val, out, valueType);
}

} // namespace QtJambiPrivate

struct QHashFunctionPrivate : QSharedData{
    void* data = nullptr;
    QtJambiUtils::QHashFunction::Invoker invoker;
    QtJambiUtils::QHashFunction::Deleter deleter;
    QHashFunctionPrivate(void* _data, QtJambiUtils::QHashFunction::Invoker _invoker, QtJambiUtils::QHashFunction::Deleter _deleter = nullptr)
        : QSharedData(), data(_data), invoker(_invoker), deleter(_deleter){
        Q_ASSERT(_data);
        Q_ASSERT(_invoker);
    }
    ~QHashFunctionPrivate(){ if(deleter) deleter(data); }
};

namespace QtJambiUtils{

QHashFunction::QHashFunction(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new QHashFunctionPrivate(
            reinterpret_cast<void*>(functor),
            [](void* data, const void* ptr, size_t seed) -> size_t{
                FunctionPointer task = reinterpret_cast<FunctionPointer>(data);
                return (*task)(ptr, seed);
            })){}

QHashFunction::QHashFunction() noexcept : d(){}
QHashFunction::~QHashFunction() noexcept {}
QHashFunction::QHashFunction(const QHashFunction& other) noexcept : d(other.d) {}
QHashFunction::QHashFunction(QHashFunction&& other) noexcept : d(std::move(other.d)) {}
QHashFunction::QHashFunction(void* data, Invoker invoker, Deleter deleter) noexcept
    : d(!data ? nullptr : new QHashFunctionPrivate(data, invoker, deleter)) {}
QHashFunction& QHashFunction::operator=(const QHashFunction& other) noexcept { d = other.d; return *this; }
QHashFunction& QHashFunction::operator=(QHashFunction&& other) noexcept { d = std::move(other.d); return *this; }
bool QHashFunction::operator==(const QHashFunction& other) const noexcept { return d == other.d; }

QHashFunction::operator bool() const noexcept{
    return d;
}

bool QHashFunction::operator !() const noexcept{
    return !d;
}

size_t QHashFunction::operator()(const void* ptr, size_t seed) const{
    Q_ASSERT(d);
    return d->invoker(d->data, ptr, seed);
}

} // namespace QtJambiPrivate

struct RunnablePrivate : QSharedData{
    void* data = nullptr;
    QtJambiUtils::Runnable::Invoker invoker;
    QtJambiUtils::Runnable::Deleter deleter;
    RunnablePrivate(void* _data, QtJambiUtils::Runnable::Invoker _invoker, QtJambiUtils::Runnable::Deleter _deleter = nullptr)
        : QSharedData(), data(_data), invoker(_invoker), deleter(_deleter){
        Q_ASSERT(_data);
        Q_ASSERT(_invoker);
    }
    ~RunnablePrivate(){ if(deleter) deleter(data); }
};

namespace QtJambiUtils{

Runnable::Runnable(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new RunnablePrivate(
            reinterpret_cast<void*>(functor),
            [](void* data){
                FunctionPointer task = reinterpret_cast<FunctionPointer>(data);
                (*task)();
            })){}

Runnable::Runnable() noexcept : d(){}
Runnable::~Runnable() noexcept {}
Runnable::Runnable(const Runnable& other) noexcept : d(other.d) {}
Runnable::Runnable(Runnable&& other) noexcept : d(std::move(other.d)) {}
Runnable::Runnable(void* data, Invoker invoker, Deleter deleter) noexcept
    : d(!data ? nullptr : new RunnablePrivate(data, invoker, deleter)) {}
Runnable& Runnable::operator=(const Runnable& other) noexcept { d = other.d; return *this; }
Runnable& Runnable::operator=(Runnable&& other) noexcept { d = std::move(other.d); return *this; }
bool Runnable::operator==(const Runnable& other) const noexcept { return d == other.d; }

Runnable::operator bool() const noexcept{
    return d;
}

bool Runnable::operator !() const noexcept{
    return !d;
}

void Runnable::operator()() const{
    if(d)
        d->invoker(d->data);
}

} // namespace QtJambiUtils

struct JObjectValueWrapperPrivate : QSharedData{
    jclass clazz = nullptr;
    jmethodID constructor = 0;
    jmethodID clone = 0;
    jmethodID writeTo = 0;
    jmethodID readFrom = 0;
    bool isSerializable = false;
    QByteArray name = {};
    JObjectValueWrapperPrivate() = default;
    const QMetaObject * metaObject = reinterpret_cast<const QMetaObject *>(quintptr(std::numeric_limits<quintptr>::max()));
    static constexpr auto metaObjectFn = &JObjectValueWrapper::metaObject;
    static constexpr auto defaultCtr = &JObjectValueWrapper::defaultCtr;
    static constexpr auto copyCtr = &JObjectValueWrapper::copyCtr;
    static constexpr auto moveCtr = &JObjectValueWrapper::moveCtr;
    static constexpr auto dtor = &JObjectValueWrapper::dtor;
    static constexpr auto equals = &JObjectValueWrapper::equals;
    static constexpr auto lessThan = &JObjectValueWrapper::lessThan;
    static constexpr auto debugStream = &JObjectValueWrapper::debugStream;
    static constexpr auto dataStreamOut = &JObjectValueWrapper::dataStreamOut;
    static constexpr auto dataStreamIn = &JObjectValueWrapper::dataStreamIn;
    static constexpr auto serializableDataStreamOut = &JObjectValueWrapper::serializableDataStreamOut;
    static constexpr auto serializableDataStreamIn = &JObjectValueWrapper::serializableDataStreamIn;
    template <typename T>
    class QExplicitlySharedDataPointer;
};

JObjectWrapperData* createJObjectValueWrapperData(JNIEnv *env, jobject obj);

JObjectWrapperData* qtjambiCreateObject(const JObjectValueWrapperPrivate* methods){
    if(methods){
        if(JniEnvironment env{200}){
            try{
                return createJObjectValueWrapperData(env, env->NewObject(methods->clazz, methods->constructor));
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }
    return nullptr;
}

JObjectWrapper qtjambiCopyObject(JNIEnv* env, const JObjectValueWrapperPrivate* methods, const JObjectWrapper& other){
    if(methods && env){
        try{
            jobject o = env->CallObjectMethod(other.object(env), methods->clone);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return JObjectWrapper(env, o);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
    return JObjectWrapper();
}

JObjectWrapper qtjambiCopyObject(const JObjectValueWrapperPrivate* methods, const JObjectWrapper& other){
    if(JniEnvironment env{200}){
        return qtjambiCopyObject(env, methods, other);
    }
    return JObjectWrapper();
}

JObjectWrapperData* qtjambiCopyObjectData(JNIEnv* env, const JObjectValueWrapperPrivate* methods, const JObjectWrapper& other){
    if(methods && env){
        try{
            jobject o = env->CallObjectMethod(other.object(env), methods->clone);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return createJObjectValueWrapperData(env, o);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
    return nullptr;
}

JObjectWrapperData* qtjambiCopyObjectData(const JObjectValueWrapperPrivate* methods, const JObjectWrapper& other){
    if(JniEnvironment env{200}){
        return qtjambiCopyObjectData(env, methods, other);
    }
    return nullptr;
}

bool JObjectValueWrapper::isValueType(QMetaType metaType){
    QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
    QtJambiStorage* cache = getQtJambiStorage();
    {
        auto iface = metaType.iface();
        QReadLocker locker(cache->lock());
        if(cache->metaTypeInterfaces().contains(iface))
            methods = cache->metaTypeInterfaces()[iface];
    }
    return methods.get();
}

bool JObjectValueWrapper::hasCustomDebugStreamOperator(QMetaType metaType){
    return metaType.iface()->debugStream!=JObjectValueWrapper::debugStream;
}

JObjectValueWrapper JObjectValueWrapper::create(JNIEnv* env, jobject object, QMetaType metaType){
    QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
    QtJambiStorage* cache = getQtJambiStorage();
    {
        {
            auto iface = metaType.iface();
            QReadLocker locker(cache->lock());
            if(cache->metaTypeInterfaces().contains(iface))
                methods = cache->metaTypeInterfaces()[iface];
        }
    }
    if(methods){
        jobject o = env->IsSameObject(object, nullptr) ? nullptr : env->CallObjectMethod(object, methods->clone);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return JObjectValueWrapper(env, o, std::move(methods));
    }
    return {};
}

JObjectValueWrapper::JObjectValueWrapper() : JObjectWrapper(), p(nullptr) {}
JObjectValueWrapper::JObjectValueWrapper(QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> _p)
    : JObjectWrapper(qtjambiCreateObject(_p.data())),
    p(std::move(_p)){}

JObjectValueWrapper::JObjectValueWrapper(JNIEnv* env, jobject object, QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> _p)
    : JObjectWrapper(createJObjectValueWrapperData(env, object)),
    p(std::move(_p)){}

JObjectValueWrapper::JObjectValueWrapper(const JObjectValueWrapper& other)
    : JObjectWrapper(qtjambiCopyObjectData(other.p.data(), other)),
    p(other.p){}

JObjectValueWrapper::JObjectValueWrapper(JObjectValueWrapper&& other)
    : JObjectWrapper(std::move(other)),
    p(std::move(other.p)){}

JObjectValueWrapper::~JObjectValueWrapper() {}

jobject JObjectValueWrapper::value(JNIEnv* env) const{
    jobject o = env->CallObjectMethod(object(env), p->clone);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return o;
}

JObjectValueWrapper& JObjectValueWrapper::operator=(const JObjectValueWrapper &wrapper){
    assign(wrapper);
    return *this;
}

JObjectValueWrapper& JObjectValueWrapper::operator=(JObjectValueWrapper &&wrapper){
    assign(std::move(wrapper));
    return *this;
}

void JObjectValueWrapper::assign(JObjectValueWrapper &&wrapper){
    JObjectWrapper::assign(std::move(wrapper));
    p = std::move(wrapper.p);
}

void JObjectValueWrapper::assign(const JObjectValueWrapper &wrapper){
    JObjectWrapper::assign(qtjambiCopyObject(wrapper.p.data(), wrapper));
    p = wrapper.p;
}

bool isJObjectValueWrapper(const JObjectWrapper& wrapper);

void JObjectValueWrapper::assign(const JObjectWrapper &wrapper){
    if(isJObjectValueWrapper(wrapper)){
        assign(static_cast<const JObjectValueWrapper&>(wrapper));
    }else if(p){
        if(JniEnvironment env{200}){
            if(env->IsInstanceOf(wrapper.object(env), p->clazz)){
                JObjectWrapper::assign(qtjambiCopyObject(env, p.data(), wrapper.object(env)));
            }
        }
    }
}

void JObjectValueWrapper::assign(JObjectWrapper &&wrapper){
    if(isJObjectValueWrapper(wrapper)){
        assign(std::move(static_cast<JObjectValueWrapper&&>(wrapper)));
    }else if(p){
        if(JniEnvironment env{200}){
            jobject obj = wrapper.object(env);
            if(env->IsInstanceOf(obj, p->clazz)){
                JObjectWrapper::assign(qtjambiCopyObject(env, p.data(), obj));
            }
        }
    }
}

void JObjectValueWrapper::assign(JNIEnv* env, jobject obj){
    if(p){
        if(env->IsInstanceOf(obj, p->clazz)){
            JObjectWrapper::assign(env, obj);
        }
    }
}

bool JObjectValueWrapper::operator==(const JObjectValueWrapper &other) const{
    return p==other.p && JObjectWrapper::operator==(other);
}

bool JObjectValueWrapper::operator<(const JObjectValueWrapper &other) const{
    return p==other.p && JObjectWrapper::operator<(other);

}

bool JObjectValueWrapper::isNull()const{return !p;}

void JObjectValueWrapper::writeTo(QDataStream &d)const{
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            env->CallVoidMethod(object(env), p->writeTo, _d);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}
void JObjectValueWrapper::readFrom(QDataStream &d){
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            env->CallVoidMethod(object(env), p->readFrom, _d);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}

void JObjectValueWrapper::serializeTo(QDataStream &d)const{
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            Java::QtJambi::MetaTypeUtility::writeSerializableJavaObject(env, _d, object(env));
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}
void JObjectValueWrapper::serializeFrom(QDataStream &d){
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            jobject newValue = Java::QtJambi::MetaTypeUtility::readSerializableJavaObject(env, _d);
            *this = JObjectValueWrapper(env, newValue, p);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}

const QMetaObject *JObjectValueWrapper::metaObject(const QtPrivate::QMetaTypeInterface *iface){
    QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
    QtJambiStorage* cache = getQtJambiStorage();
    {
        QReadLocker locker(cache->lock());
        if(cache->metaTypeInterfaces().contains(iface))
            methods = cache->metaTypeInterfaces()[iface];
    }
    if(methods){
        if(quintptr(methods->metaObject) == std::numeric_limits<quintptr>::max()){
            if(JniEnvironment env{200}){
                const_cast<JObjectValueWrapperPrivate*>(methods.get())->metaObject = CoreAPI::metaObjectForClass(env, methods->clazz);
                if(!methods->metaObject){
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->flags &= ~QMetaType::IsGadget;
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->flags &= ~QMetaType::PointerToGadget;
                }
            }else return nullptr;
        }
        return methods->metaObject;
    }
    return nullptr;
}

void JObjectValueWrapper::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *target){
    QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
    QtJambiStorage* cache = getQtJambiStorage();
    {
        QReadLocker locker(cache->lock());
        if(cache->metaTypeInterfaces().contains(iface))
            methods = cache->metaTypeInterfaces()[iface];
    }
    new (target) JObjectValueWrapper(std::move(methods));
}

void JObjectValueWrapper::copyCtr(const QtPrivate::QMetaTypeInterface *, void *target, const void *other){
    const JObjectValueWrapper* _other = reinterpret_cast<const JObjectValueWrapper*>(other);
    new (target) JObjectValueWrapper(*_other);
}

void JObjectValueWrapper::moveCtr(const QtPrivate::QMetaTypeInterface *, void *target, void *other){
    JObjectValueWrapper* _other = reinterpret_cast<JObjectValueWrapper*>(other);
    new (target) JObjectValueWrapper(std::move(*_other));
}

void JObjectValueWrapper::dtor(const QtPrivate::QMetaTypeInterface *, void *target){
    reinterpret_cast<JObjectValueWrapper*>(target)->~JObjectValueWrapper();
}

bool JObjectValueWrapper::equals(const QtPrivate::QMetaTypeInterface *iface, const void *value1, const void *value2){
    const JObjectValueWrapper* _value1 = reinterpret_cast<const JObjectValueWrapper*>(value1);
    const JObjectValueWrapper* _value2 = reinterpret_cast<const JObjectValueWrapper*>(value2);
    if(_value1->isNull() || _value2->isNull()){
        QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
        QtJambiStorage* cache = getQtJambiStorage();
        {
            QReadLocker locker(cache->lock());
            if(cache->metaTypeInterfaces().contains(iface))
                methods = cache->metaTypeInterfaces()[iface];
        }
        if(methods){
            if(_value1->isNull())
                *const_cast<JObjectValueWrapper*>(_value1) = JObjectValueWrapper(std::move(methods));
            if(_value2->isNull())
                *const_cast<JObjectValueWrapper*>(_value2) = JObjectValueWrapper(std::move(methods));
        }else return false;
    }
    try{
        return *_value1 == *_value2;
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
        return false;
    }
}

bool JObjectValueWrapper::lessThan(const QtPrivate::QMetaTypeInterface *iface, const void *value1, const void *value2){
    const JObjectValueWrapper* _value1 = reinterpret_cast<const JObjectValueWrapper*>(value1);
    const JObjectValueWrapper* _value2 = reinterpret_cast<const JObjectValueWrapper*>(value2);
    if(_value1->isNull() || _value2->isNull()){
        QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
        QtJambiStorage* cache = getQtJambiStorage();
        {
            QReadLocker locker(cache->lock());
            if(cache->metaTypeInterfaces().contains(iface))
                methods = cache->metaTypeInterfaces()[iface];
        }
        if(methods){
            if(_value1->isNull())
                *const_cast<JObjectValueWrapper*>(_value1) = JObjectValueWrapper(std::move(methods));
            if(_value2->isNull())
                *const_cast<JObjectValueWrapper*>(_value2) = JObjectValueWrapper(std::move(methods));
        }else return false;
    }
    try{
        return *_value1 < *_value2;
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
        return false;
    }
}

void JObjectValueWrapper::debugStream(const QtPrivate::QMetaTypeInterface *iface, QDebug &d, const void *value){
    const JObjectValueWrapper* _value = reinterpret_cast<const JObjectValueWrapper*>(value);
    if(_value->isNull()){
        QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
        QtJambiStorage* cache = getQtJambiStorage();
        {
            QReadLocker locker(cache->lock());
            if(cache->metaTypeInterfaces().contains(iface))
                methods = cache->metaTypeInterfaces()[iface];
        }
        if(methods)
            *const_cast<JObjectValueWrapper*>(_value) = JObjectValueWrapper(std::move(methods));
        else return;
    }
    try{
        bool ok = false;
        QString strg = _value->toString(&ok);
        if(ok)
            d << strg;
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::dataStreamOut(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
    try{
        const JObjectValueWrapper* _value = reinterpret_cast<const JObjectValueWrapper*>(value);
        if(_value->isNull()){
            QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
            QtJambiStorage* cache = getQtJambiStorage();
            {
                QReadLocker locker(cache->lock());
                if(cache->metaTypeInterfaces().contains(iface))
                    methods = cache->metaTypeInterfaces()[iface];
            }
            if(methods)
                *const_cast<JObjectValueWrapper*>(_value) = JObjectValueWrapper(std::move(methods));
            else return;
        }
        _value->writeTo(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::dataStreamIn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
    try{
        JObjectValueWrapper* _value = reinterpret_cast<JObjectValueWrapper*>(value);
        if(_value->isNull()){
            QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
            QtJambiStorage* cache = getQtJambiStorage();
            {
                QReadLocker locker(cache->lock());
                if(cache->metaTypeInterfaces().contains(iface))
                    methods = cache->metaTypeInterfaces()[iface];
            }
            if(methods)
                *_value = JObjectValueWrapper(std::move(methods));
            else return;
        }
        _value->readFrom(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::serializableDataStreamOut(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
    try{
        const JObjectValueWrapper* _value = reinterpret_cast<const JObjectValueWrapper*>(value);
        if(_value->isNull()){
            QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
            QtJambiStorage* cache = getQtJambiStorage();
            {
                QReadLocker locker(cache->lock());
                if(cache->metaTypeInterfaces().contains(iface))
                    methods = cache->metaTypeInterfaces()[iface];
            }
            if(methods)
                *const_cast<JObjectValueWrapper*>(_value) = JObjectValueWrapper(std::move(methods));
            else return;
        }
        _value->serializeTo(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::serializableDataStreamIn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
    try{
        JObjectValueWrapper* _value = reinterpret_cast<JObjectValueWrapper*>(value);
        if(_value->isNull()){
            QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
            QtJambiStorage* cache = getQtJambiStorage();
            {
                QReadLocker locker(cache->lock());
                if(cache->metaTypeInterfaces().contains(iface))
                    methods = cache->metaTypeInterfaces()[iface];
            }
            if(methods)
                *_value = JObjectValueWrapper(std::move(methods));
            else return;
        }
        _value->serializeFrom(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
#define QMetaTypeInterface_CurrentRevision 0
#else
#define QMetaTypeInterface_CurrentRevision QtPrivate::QMetaTypeInterface::CurrentRevision
#endif

bool getFunctions(JNIEnv *env, const QMetaType& elementType, QtJambiUtils::QHashFunction& hashFunction, QtJambiUtils::InternalToExternalConverter& internalToExternalConverter, QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter){
    QtJambiStorage* cache = getQtJambiStorage();
    QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
    const QtPrivate::QMetaTypeInterface *iface = META_TYPE_ACCESS(elementType).iface();
    {
        QReadLocker locker(cache->lock());
        if(cache->metaTypeInterfaces().contains(iface))
            methods = cache->metaTypeInterfaces()[iface];
    }
    if(methods){
        hashFunction = [](const void* ptr, size_t seed)->size_t{
            if(JniEnvironment env{200}){
                try{
                    seed = qHash(Java::Runtime::Object::hashCode(env, reinterpret_cast<const JObjectValueWrapper*>(ptr)->object(env)), seed);
                }catch(const JavaException& exn){
                    exn.report(env);
                }
            }
            return seed;
        };
        internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(env, QLatin1String(elementType.name()), elementType, methods->clazz);
        externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(env, methods->clazz, QLatin1String(elementType.name()), elementType);
        return true;
    }
    return false;
}

QMetaType registerJObjectValueWrapper(JNIEnv *env, jclass clazz, const QString& javaClassName, const QByteArray& metaTypeName){
    JObjectValueWrapperPrivate* p = new JObjectValueWrapperPrivate;
    p->constructor = env->GetMethodID(clazz, "<init>", "()V");
    if(env->ExceptionCheck())
        env->ExceptionClear();
    p->clone = env->GetMethodID(clazz, "clone", qPrintable(QString("()L%1;").arg(javaClassName)));
    if(env->ExceptionCheck())
        env->ExceptionClear();
    if(!p->clone && p->constructor && Java::Runtime::Cloneable::isAssignableFrom(env, clazz)){
        jobject newObject = env->NewObject(clazz, p->constructor);
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(!newObject){
            p->constructor = nullptr;
        }else{
            p->clone = Java::Runtime::Object::clone_ID(env);
            jobject clonedObject = env->CallObjectMethod(newObject, p->clone);
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(!env->IsInstanceOf(clonedObject, clazz))
                p->clone = nullptr;
        }
    }
    if(p->constructor && p->clone){
        p->writeTo = env->GetMethodID(clazz, "writeTo", "(Lio/qt/core/QDataStream;)V");
        if(env->ExceptionCheck())
            env->ExceptionClear();
        p->readFrom = env->GetMethodID(clazz, "readFrom", "(Lio/qt/core/QDataStream;)V");
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(!p->writeTo && !p->readFrom){
            p->isSerializable = Java::Runtime::Serializable::isAssignableFrom(env, clazz);
        }
        p->clazz = getGlobalClassRef(env, clazz, qPrintable(javaClassName));
        p->name = metaTypeName;
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
        uint flags = QtPrivate::QMetaTypeTypeFlags<JObjectValueWrapper>::Flags;
#else
        uint flags = QtPrivate::QMetaTypeForType<JObjectValueWrapper>::flags();
#endif
        flags |= QMetaType::IsGadget;
        QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
            /*.revision=*/ QMetaTypeInterface_CurrentRevision,
            /*.alignment=*/ alignof(JObjectValueWrapper),
            /*.size=*/ sizeof(JObjectValueWrapper),
            /*.flags=*/ flags,
            /*.typeId=*/ QMetaType::UnknownType,
            /*.metaObject=*/ JObjectValueWrapperPrivate::metaObjectFn,
            /*.name=*/ p->name.constData(),
            /*.defaultCtr=*/ JObjectValueWrapperPrivate::defaultCtr,
            /*.copyCtr=*/ JObjectValueWrapperPrivate::copyCtr,
            /*.moveCtr=*/ JObjectValueWrapperPrivate::moveCtr,
            /*.dtor=*/ JObjectValueWrapperPrivate::dtor,
            /*.equals=*/ JObjectValueWrapperPrivate::equals,
            /*.lessThan=*/ Java::Runtime::Comparable::isInstanceOf(env, clazz) ? JObjectValueWrapperPrivate::lessThan : nullptr,
            /*.debugStream=*/ JObjectValueWrapperPrivate::debugStream,
            /*.dataStreamOutFn=*/ p->writeTo ? JObjectValueWrapperPrivate::dataStreamOut : (p->isSerializable ? JObjectValueWrapperPrivate::serializableDataStreamOut : nullptr),
            /*.dataStreamInFn=*/ p->readFrom ? JObjectValueWrapperPrivate::dataStreamIn : (p->isSerializable ? JObjectValueWrapperPrivate::serializableDataStreamIn : nullptr),
            /*.legacyRegisterOp=*/ nullptr
        };
        QtJambiStorage* cache = getQtJambiStorage();
        {
            auto hashcode = qHash(javaClassName);
            QWriteLocker locker(cache->lock());
            if(const QtPrivate::QMetaTypeInterface * iface = cache->classMetaTypeInterfaces().value(hashcode)){
                delete metaTypeInterface;
                return QMetaType(iface);
            }
            cache->classMetaTypeInterfaces().insert(hashcode, metaTypeInterface);
            cache->metaTypeInterfaces()[metaTypeInterface] = QExplicitlySharedDataPointer<JObjectValueWrapperPrivate>(p);
        }
        QMetaType metaType(metaTypeInterface);
        if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<QString>())){
            QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                bool ok = false;
                if(src){
                    new (target)QString(reinterpret_cast<const JObjectValueWrapper*>(src)->toString(&ok));
                }else{
                    ok = true;
                    new (target)QString();
                }
                return ok;
            }, metaType, QMetaType::fromType<QString>());
        }
        if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<JObjectWrapper>())){
            QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                if(src){
                    new (target)JObjectWrapper(*reinterpret_cast<const JObjectValueWrapper*>(src));
                }else{
                    new (target)JObjectWrapper();
                }
                return true;
            }, metaType, QMetaType::fromType<JObjectWrapper>());
        }
        if(!QMetaType::hasRegisteredConverterFunction(QMetaType::fromType<JObjectWrapper>(), metaType)){
            QMetaType::registerConverterFunction([metaType](const void *src, void *target) -> bool {
                if(src){
                    if(JniEnvironment env{200}){
                        jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object(env);
                        if(value!=nullptr){
                            QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods;
                            QtJambiStorage* cache = getQtJambiStorage();
                            {
                                auto iface = QMetaType(metaType).iface();
                                QReadLocker locker(cache->lock());
                                if(cache->metaTypeInterfaces().contains(iface))
                                    methods = cache->metaTypeInterfaces()[iface];
                            }
                            if(!methods || !env->IsInstanceOf(value, methods->clazz))
                                return false;
                            new (target)JObjectValueWrapper(JObjectValueWrapper::create(env, *reinterpret_cast<const JObjectWrapper*>(src), metaType));
                            return true;
                        }else{
                            return false;
                        }
                    }
                }
                new (target)JObjectValueWrapper();
                return true;
            }, QMetaType::fromType<JObjectWrapper>(), metaType);
        }
        registerJavaClassForCustomMetaType(env, metaType, clazz);
        registerConverterVariant(env, metaType, QLatin1String(metaTypeInterface->name), javaClassName, clazz);
        return metaType;
    }
    return {};
}

#define CACHE_MEMBER_DECL(Type,variable)\
Type& QtJambiStorage::variable() {return m_##variable;}

CACHE_MEMBER_DECL(QtJambiStorage::BindableInterfacesHash, bindableInterfacesHash);
CACHE_MEMBER_DECL(QtJambiStorage::MetaTypesByPointerHash, metaTypesByPointer);
CACHE_MEMBER_DECL(QtJambiStorage::CreatorFunctionMetaDataHash, creatorFunctionMetaDataHash);
CACHE_MEMBER_DECL(QtJambiStorage::EnumClassValuesHash, enumClassValuesFields);
CACHE_MEMBER_DECL(QtJambiStorage::EnumClassExtensibleHash, enumClassExtensible);
CACHE_MEMBER_DECL(QtJambiStorage::SwitchTableFieldHash, switchTableFields);
CACHE_MEMBER_DECL(QtJambiStorage::ObjectsByFunctionPointerHash, objectsByFunctionPointers);
CACHE_MEMBER_DECL(QtJambiStorage::InternalToExternalConverterHash, internalToExternalConverters);
CACHE_MEMBER_DECL(QtJambiStorage::ExternalToInternalConverterHash, externalToInternalConverters);
CACHE_MEMBER_DECL(QtJambiStorage::HashFunctionByMetaTypeHash, hashFunctionByMetaTypeHash);
CACHE_MEMBER_DECL(QtJambiStorage::MetaEnumHash, metaEnumByMetaTypeHash);
CACHE_MEMBER_DECL(QtJambiStorage::TypeEntryHash, typeEntries);
CACHE_MEMBER_DECL(QtJambiStorage::SuperTypeInfoHash, superTypeInfos);
CACHE_MEMBER_DECL(QtJambiStorage::VTableHash, virtualTables);
CACHE_MEMBER_DECL(QtJambiStorage::MetaObjectByNameHash, metaObjectsByName);
CACHE_MEMBER_DECL(QtJambiStorage::ParameterTypeHash, parameterTypeInfos);
CACHE_MEMBER_DECL(QtJambiStorage::MetaObjectByMetaTypeHash, metaObjects);
CACHE_MEMBER_DECL(QtJambiStorage::SignalTypesHash, signalTypes);
CACHE_MEMBER_DECL(QtJambiStorage::ResettableBoolFlagHash, resettableBoolFlags);
CACHE_MEMBER_DECL(QtJambiStorage::SupportedMessageTypes, supportedMessageTypes);
CACHE_MEMBER_DECL(JObjectWrapper, signalEmitThreadCheckHandler);
CACHE_MEMBER_DECL(QtJambiStorage::MetaAssociationByMetaTypeHash, metaAssociationsByMetaType);
CACHE_MEMBER_DECL(QtJambiStorage::MetaSequenceByMetaTypeHash, metaSequencesByMetaType);
CACHE_MEMBER_DECL(QtJambiStorage::DependencyHash, dependencies);
CACHE_MEMBER_DECL(QtJambiStorage::DependencyHash, conDestroyedDependencies);
CACHE_MEMBER_DECL(QtJambiStorage::FinalizationDataHash, finalizationData);
CACHE_MEMBER_DECL(QtJambiStorage::FinalizationHash, finalizations);
CACHE_MEMBER_DECL(QtJambiStorage::UserObjectHash, userObjectCache);
CACHE_MEMBER_DECL(QtJambiStorage::QObjectHash, qObjectCache);
CACHE_MEMBER_DECL(QtJambiStorage::TypeStringHash, typeQtNameHash)
CACHE_MEMBER_DECL(QtJambiStorage::TypeEntryTypesHash, entryTypesHash)
CACHE_MEMBER_DECL(QtJambiStorage::StrintypeHash, qtNameTypeHash)
CACHE_MEMBER_DECL(QtJambiStorage::StringStringHash, qtFunctionalJavaNameHash)
CACHE_MEMBER_DECL(QtJambiStorage::StringStringHash, qtFunctionalQtNameHash)
CACHE_MEMBER_DECL(QtJambiStorage::StringStringHash, qtNamespaceJavaNameHash)
CACHE_MEMBER_DECL(QtJambiStorage::StringStringHash, javaNamespaceQtNameHash)
CACHE_MEMBER_DECL(QtJambiStorage::NamespaceMetaObjectHash, namespaceMetaObjectHash)
CACHE_MEMBER_DECL(QtJambiStorage::TypeStringHash, typeJavaNameHash)
CACHE_MEMBER_DECL(QtJambiStorage::StrinbTypesHash, javaNameTypeHash)
CACHE_MEMBER_DECL(QtJambiStorage::StrintypeHash, iidTypeHash)
CACHE_MEMBER_DECL(QtJambiStorage::StringStringHash, javaClassIIDHash)
CACHE_MEMBER_DECL(QtJambiStorage::IIDByteArrayHash, iidByteArrayHash)
CACHE_MEMBER_DECL(QtJambiStorage::ID2IDHash, flagEnumIDHash)
CACHE_MEMBER_DECL(QtJambiStorage::ID2IDHash, enumFlagIDHash)
CACHE_MEMBER_DECL(QtJambiStorage::TypeStringHash, typeJavaInterfaceHash)
CACHE_MEMBER_DECL(QtJambiStorage::ContainerAccessFactoryHash, containerAccessFactories)
CACHE_MEMBER_DECL(QtJambiStorage::SequentialContainerAccessFactoryByMetaTypeHash, sequentialContainerAccessFactoryByMetaTypes)
CACHE_MEMBER_DECL(QtJambiStorage::AssociativeContainerAccessFactoryByMetaTypeHash, associativeContainerAccessFactoryByMetaTypes)
CACHE_MEMBER_DECL(QtJambiStorage::MetaTypeByTypeInfoMap, metaTypeByTypeInfos)
CACHE_MEMBER_DECL(QtJambiStorage::TypeInfoByMetaTypeHash, typeInfoByMetaTypes)
CACHE_MEMBER_DECL(QtJambiStorage::DefaultValueHash, defaultValueHash)
CACHE_MEMBER_DECL(QtJambiStorage::FunctionInfoHash, virtualFunctionInfos)
CACHE_MEMBER_DECL(QtJambiStorage::ConstructorInfoHash, constructorInfos)
CACHE_MEMBER_DECL(QtJambiStorage::ReturnScopeHash, returnScopes)
CACHE_MEMBER_DECL(QtJambiStorage::DestructorHash, destructorHash)
CACHE_MEMBER_DECL(QtJambiStorage::NameHash, flagEnumNameHash)
CACHE_MEMBER_DECL(QtJambiStorage::NameHash, interfaceHash)
CACHE_MEMBER_DECL(QtJambiStorage::NameHash, interfaceIIDsHash)
CACHE_MEMBER_DECL(QtJambiStorage::HashSet, functionalHash)
CACHE_MEMBER_DECL(QtJambiStorage::DeleterHash, deleterHash)
CACHE_MEMBER_DECL(QtJambiStorage::ContainerAccessMap, containerAccessMap)
CACHE_MEMBER_DECL(QtJambiStorage::JObjectNativeWrappedMetaTypeHash, jobjectNativeWrapperMetaTypes)
CACHE_MEMBER_DECL(QtJambiStorage::JObjectNativeWrappedMetaTypeReverseHash, metaTypesForJObjectNativeWrappers)
CACHE_MEMBER_DECL(QtJambiStorage::JavaTypeByMetaTypeHash, javaTypeByCustomMetaTypes)
CACHE_MEMBER_DECL(QtJambiStorage::JavaNameByMetaTypeHash, javaNameByMetaTypes)
CACHE_MEMBER_DECL(QtJambiStorage::MetaTypesByJavaTypeNameHash, metaTypesByJavaTypeNames)
CACHE_MEMBER_DECL(QtJambiStorage::JObjectWrappedMetaTypeHash, jobjectWrappedMetaTypes)
CACHE_MEMBER_DECL(QtJambiStorage::OwnerFunctionHash, ownerFunctionHash)
CACHE_MEMBER_DECL(QtJambiStorage::PolymorphicIdHash, polymorphicIDs)
CACHE_MEMBER_DECL(QtJambiStorage::PolymorphicBasesHash, polymorphicBasesHash)
CACHE_MEMBER_DECL(QtJambiStorage::SizeHash, valueSizeHash)
CACHE_MEMBER_DECL(QtJambiStorage::SizeHash, shellSizeHash)
CACHE_MEMBER_DECL(QtJambiStorage::HashFunctionHash, hashFunctionHash)
CACHE_MEMBER_DECL(QtJambiStorage::OffsetHash, offsetHash)
CACHE_MEMBER_DECL(QtJambiStorage::InterfacesHash, interfacesHash)
CACHE_MEMBER_DECL(QtJambiStorage::InterfaceOffsetHash, interfaceOffsetHash)
CACHE_MEMBER_DECL(QtJambiStorage::MetaObjectHash, originalMetaObjectHash)
CACHE_MEMBER_DECL(QtJambiStorage::MetaObjectHash, customMetaObject)
CACHE_MEMBER_DECL(QtJambiStorage::SignalMetaInfoHash, signalMetaInfos)
CACHE_MEMBER_DECL(QtJambiStorage::ExtraSignalsOfMetaObjectHash, extraSignalsOfMetaObject)
CACHE_MEMBER_DECL(QtJambiStorage::TypeIdsByMetaObjectHash, typeIdsByMetaObject)
CACHE_MEMBER_DECL(QtJambiStorage::QMetaMethodRenamerFnByMetaObjectHash, metaMethodRenamerFnByMetaObject)
CACHE_MEMBER_DECL(QtJambiStorage::ValueOwnerSet, valueOwners)
CACHE_MEMBER_DECL(QtJambiStorage::ValueOwnerHash, valueOwnerHash)
CACHE_MEMBER_DECL(QtJambiStorage::ParameterTypeInfoProviderHash, parameterTypeInfoProviders)
CACHE_MEMBER_DECL(QtJambiStorage::RenamedMethodsHash, renamedMethodsHash)
CACHE_MEMBER_DECL(QtJambiStorage::FunctionalResolverHash, functionalResolverHash)
CACHE_MEMBER_DECL(QtJambiStorage::TypeInfoSupplierHash, typeInfoSupplierHash)
CACHE_MEMBER_DECL(QtJambiStorage::TypeStringHash, mediaControlIIDHash)
CACHE_MEMBER_DECL(QtJambiStorage::StrintypeHash, mediaControlIIDClassHash)
CACHE_MEMBER_DECL(QtJambiStorage::ClassIdHash, classHash)
CACHE_MEMBER_DECL(QtJambiStorage::FieldIdHash, fieldHash)
CACHE_MEMBER_DECL(QtJambiStorage::MethodIdHash, methodHash)
CACHE_MEMBER_DECL(QtJambiStorage::ClassIdHash, qtSuperclassHash)
CACHE_MEMBER_DECL(QtJambiStorage::MetaTypeMetaObjectHash, metaTypeMetaObjectHash)
CACHE_MEMBER_DECL(QtJambiStorage::TypeNameSet, typeNames)
CACHE_MEMBER_DECL(QtJambiStorage::ClassMetaTypeInterfaceHash, classMetaTypeInterfaces)
CACHE_MEMBER_DECL(QtJambiStorage::MetaTypeInterfacesHash, metaTypeInterfaces)
CACHE_MEMBER_DECL(QtJambiStorage::MetaTypeEnumClassesHash, metaTypeEnumClasses)
CACHE_MEMBER_DECL(QtJambiStorage::GlobalClassPointers, globalClassPointers)
#if !defined(QTJAMBI_NO_GLOBALREFS)
CACHE_MEMBER_DECL(QtJambiStorage::JavaMetaObjectHash, javaMetaObjects);
#endif
#if defined(QTJAMBI_DEBUG_TOOLS)
CACHE_MEMBER_DECL(QtJambiStorage::LinkSet, linkSet);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
CACHE_MEMBER_DECL(QtJambiStorage::NativeInterfaceMap, nativeInterfaceMap)
#endif
#ifdef QTJAMBI_LOG_CLASSNAMES
CACHE_MEMBER_DECL(QtJambiStorage::ClassNameHash, classNameHash)
#endif
#if defined(QTJAMBI_GENERIC_ACCESS)
CACHE_MEMBER_DECL(QtJambiStorage::MetaSequenceHash, metaSequenceHash);
CACHE_MEMBER_DECL(QtJambiStorage::MetaAssociationHash, metaAssociationHash);
CACHE_MEMBER_DECL(QtJambiStorage::ContainerTypeInfoHash, containerTypeInfos);
CACHE_MEMBER_DECL(QtJambiStorage::BiContainerTypeInfoHash, biContainerTypeInfos);
CACHE_MEMBER_DECL(QtJambiStorage::SequentialContainerAccessFactoryHash, sequentialContainerAccessFactoryHash);
CACHE_MEMBER_DECL(QtJambiStorage::AssociativeContainerAccessFactoryHash, associativeContainerAccessFactoryHash);
#endif

QReadWriteLock* QtJambiStorage::lock(){return m_lock;}
QReadWriteLock* QtJambiStorage::registryLock(){return m_registryLock;}
QReadWriteLock* QtJambiStorage::linkLock(){return m_linkLock;}

QtJambiStorage::QtJambiStorage(bool init)
  : m_lock(init ? new QReadWriteLock() : nullptr),
    m_linkLock(init ? new QReadWriteLock() : nullptr),
    m_registryLock(init ? new QReadWriteLock() : nullptr)
{}

bool QtJambiStorage::ref(){
    return Q_LIKELY(hash==QtJambiPrivate::CACHE_HASH) && c.ref();
}
void QtJambiStorage::deref(){
    if(Q_LIKELY(hash==QtJambiPrivate::CACHE_HASH) && !c.deref()){
        hash = 0;
        delete std::exchange(INSTANCE, nullptr);
    }
}

#define CACHE_DESTRUCTOR_VAR(Type,variable) Type variable;
#define CACHE_DESTRUCTOR_SWAP(variable) m_##variable.swap(variable);

void QtJambiStorage::cleanup(GlobalClassPointers& globalClassPointers,
                               ClassIdHash& classHash,
                               JObjectWrapper& signalEmitThreadCheckHandler,
                               ResettableBoolFlagHash& resettableBoolFlags,
                               MetaObjectByMetaTypeHash& metaObjects,
                               SignalTypesHash& signalTypes,
#ifdef QTJAMBI_LOG_CLASSNAMES
                               ClassNameHash& classNameHash,
#endif
#if !defined(QTJAMBI_NO_GLOBALREFS)
                               JavaMetaObjectHash javaMetaObjects,
#endif
                               ObjectsByFunctionPointerHash& objectsByFunctionPointers,
                               SuperTypeInfoHash& superTypeInfos){
    CACHE_DESTRUCTOR_VAR(PolymorphicIdHash, polymorphicIDs);
    CACHE_DESTRUCTOR_VAR(BindableInterfacesHash, bindableInterfacesHash);
    CACHE_DESTRUCTOR_VAR(MetaTypesByPointerHash, metaTypesByPointer);
    CACHE_DESTRUCTOR_VAR(CreatorFunctionMetaDataHash, creatorFunctionMetaDataHash);
    CACHE_DESTRUCTOR_VAR(EnumClassValuesHash, enumClassValuesFields);
    CACHE_DESTRUCTOR_VAR(EnumClassExtensibleHash, enumClassExtensible);
    CACHE_DESTRUCTOR_VAR(SwitchTableFieldHash, switchTableFields);
    CACHE_DESTRUCTOR_VAR(InternalToExternalConverterHash, internalToExternalConverters);
    CACHE_DESTRUCTOR_VAR(ExternalToInternalConverterHash, externalToInternalConverters);
    CACHE_DESTRUCTOR_VAR(HashFunctionByMetaTypeHash, hashFunctionByMetaTypeHash);
    CACHE_DESTRUCTOR_VAR(MetaEnumHash, metaEnumByMetaTypeHash);
    CACHE_DESTRUCTOR_VAR(TypeEntryHash, typeEntries);
    CACHE_DESTRUCTOR_VAR(VTableHash, virtualTables);
    CACHE_DESTRUCTOR_VAR(MetaObjectByNameHash, metaObjectsByName);
    CACHE_DESTRUCTOR_VAR(ParameterTypeHash, parameterTypeInfos);
    CACHE_DESTRUCTOR_VAR(SupportedMessageTypes, supportedMessageTypes);
    CACHE_DESTRUCTOR_VAR(MetaAssociationByMetaTypeHash, metaAssociationsByMetaType);
    CACHE_DESTRUCTOR_VAR(MetaSequenceByMetaTypeHash, metaSequencesByMetaType);
    CACHE_DESTRUCTOR_VAR(DependencyHash, dependencies);
    CACHE_DESTRUCTOR_VAR(DependencyHash, conDestroyedDependencies);
    CACHE_DESTRUCTOR_VAR(FinalizationHash, finalizations);
    CACHE_DESTRUCTOR_VAR(FinalizationDataHash, finalizationData);
    CACHE_DESTRUCTOR_VAR(UserObjectHash, userObjectCache);
    CACHE_DESTRUCTOR_VAR(QObjectHash, qObjectCache);
    CACHE_DESTRUCTOR_VAR(TypeStringHash, typeQtNameHash)
    CACHE_DESTRUCTOR_VAR(TypeEntryTypesHash, entryTypesHash)
    CACHE_DESTRUCTOR_VAR(StrintypeHash, qtNameTypeHash)
    CACHE_DESTRUCTOR_VAR(StringStringHash, qtFunctionalJavaNameHash)
    CACHE_DESTRUCTOR_VAR(StringStringHash, qtFunctionalQtNameHash)
    CACHE_DESTRUCTOR_VAR(StringStringHash, qtNamespaceJavaNameHash)
    CACHE_DESTRUCTOR_VAR(StringStringHash, javaNamespaceQtNameHash)
    CACHE_DESTRUCTOR_VAR(NamespaceMetaObjectHash, namespaceMetaObjectHash)
    CACHE_DESTRUCTOR_VAR(TypeStringHash, typeJavaNameHash)
    CACHE_DESTRUCTOR_VAR(StrinbTypesHash, javaNameTypeHash)
    CACHE_DESTRUCTOR_VAR(StrintypeHash, iidTypeHash)
    CACHE_DESTRUCTOR_VAR(StringStringHash, javaClassIIDHash)
    CACHE_DESTRUCTOR_VAR(IIDByteArrayHash, iidByteArrayHash)
    CACHE_DESTRUCTOR_VAR(ContainerAccessMap, containerAccessMap)
    CACHE_DESTRUCTOR_VAR(ID2IDHash, flagEnumIDHash)
    CACHE_DESTRUCTOR_VAR(ID2IDHash, enumFlagIDHash)
    CACHE_DESTRUCTOR_VAR(TypeStringHash, typeJavaInterfaceHash)
    CACHE_DESTRUCTOR_VAR(ContainerAccessFactoryHash, containerAccessFactories)
    CACHE_DESTRUCTOR_VAR(SequentialContainerAccessFactoryByMetaTypeHash, sequentialContainerAccessFactoryByMetaTypes)
    CACHE_DESTRUCTOR_VAR(AssociativeContainerAccessFactoryByMetaTypeHash, associativeContainerAccessFactoryByMetaTypes)
    CACHE_DESTRUCTOR_VAR(MetaTypeByTypeInfoMap, metaTypeByTypeInfos)
    CACHE_DESTRUCTOR_VAR(TypeInfoByMetaTypeHash, typeInfoByMetaTypes)
    CACHE_DESTRUCTOR_VAR(DefaultValueHash, defaultValueHash)
    CACHE_DESTRUCTOR_VAR(FunctionInfoHash, virtualFunctionInfos)
    CACHE_DESTRUCTOR_VAR(ConstructorInfoHash, constructorInfos)
    CACHE_DESTRUCTOR_VAR(ReturnScopeHash, returnScopes)
    CACHE_DESTRUCTOR_VAR(DestructorHash, destructorHash)
    CACHE_DESTRUCTOR_VAR(NameHash, flagEnumNameHash)
    CACHE_DESTRUCTOR_VAR(NameHash, interfaceHash)
    CACHE_DESTRUCTOR_VAR(NameHash, interfaceIIDsHash)
    CACHE_DESTRUCTOR_VAR(HashSet, functionalHash)
    CACHE_DESTRUCTOR_VAR(DeleterHash, deleterHash)
    CACHE_DESTRUCTOR_VAR(JObjectNativeWrappedMetaTypeHash, jobjectNativeWrapperMetaTypes)
    CACHE_DESTRUCTOR_VAR(JObjectNativeWrappedMetaTypeReverseHash, metaTypesForJObjectNativeWrappers)
    CACHE_DESTRUCTOR_VAR(JavaTypeByMetaTypeHash, javaTypeByCustomMetaTypes)
    CACHE_DESTRUCTOR_VAR(JavaNameByMetaTypeHash, javaNameByMetaTypes)
    CACHE_DESTRUCTOR_VAR(MetaTypesByJavaTypeNameHash, metaTypesByJavaTypeNames)
    CACHE_DESTRUCTOR_VAR(JObjectWrappedMetaTypeHash, jobjectWrappedMetaTypes)
    CACHE_DESTRUCTOR_VAR(OwnerFunctionHash, ownerFunctionHash)
    CACHE_DESTRUCTOR_VAR(PolymorphicBasesHash, polymorphicBasesHash)
    CACHE_DESTRUCTOR_VAR(SizeHash, valueSizeHash)
    CACHE_DESTRUCTOR_VAR(SizeHash, shellSizeHash)
    CACHE_DESTRUCTOR_VAR(HashFunctionHash, hashFunctionHash)
    CACHE_DESTRUCTOR_VAR(OffsetHash, offsetHash)
    CACHE_DESTRUCTOR_VAR(InterfacesHash, interfacesHash)
    CACHE_DESTRUCTOR_VAR(InterfaceOffsetHash, interfaceOffsetHash)
    CACHE_DESTRUCTOR_VAR(MetaObjectHash, originalMetaObjectHash)
    CACHE_DESTRUCTOR_VAR(MetaObjectHash, customMetaObject)
    CACHE_DESTRUCTOR_VAR(SignalMetaInfoHash, signalMetaInfos)
    CACHE_DESTRUCTOR_VAR(ExtraSignalsOfMetaObjectHash, extraSignalsOfMetaObject)
    CACHE_DESTRUCTOR_VAR(TypeIdsByMetaObjectHash, typeIdsByMetaObject)
    CACHE_DESTRUCTOR_VAR(QMetaMethodRenamerFnByMetaObjectHash, metaMethodRenamerFnByMetaObject)
    CACHE_DESTRUCTOR_VAR(ValueOwnerSet, valueOwners)
    CACHE_DESTRUCTOR_VAR(ValueOwnerHash, valueOwnerHash)
    CACHE_DESTRUCTOR_VAR(ParameterTypeInfoProviderHash, parameterTypeInfoProviders)
    CACHE_DESTRUCTOR_VAR(RenamedMethodsHash, renamedMethodsHash)
    CACHE_DESTRUCTOR_VAR(FunctionalResolverHash, functionalResolverHash)
    CACHE_DESTRUCTOR_VAR(TypeInfoSupplierHash, typeInfoSupplierHash)
    CACHE_DESTRUCTOR_VAR(TypeStringHash, mediaControlIIDHash)
    CACHE_DESTRUCTOR_VAR(StrintypeHash, mediaControlIIDClassHash)
    CACHE_DESTRUCTOR_VAR(FieldIdHash, fieldHash)
    CACHE_DESTRUCTOR_VAR(MethodIdHash, methodHash)
    CACHE_DESTRUCTOR_VAR(ClassIdHash, qtSuperclassHash)
    CACHE_DESTRUCTOR_VAR(MetaTypeMetaObjectHash, metaTypeMetaObjectHash)
    CACHE_DESTRUCTOR_VAR(TypeNameSet, typeNames)
    CACHE_DESTRUCTOR_VAR(ClassMetaTypeInterfaceHash, classMetaTypeInterfaces)
    CACHE_DESTRUCTOR_VAR(MetaTypeInterfacesHash, metaTypeInterfaces)
    CACHE_DESTRUCTOR_VAR(MetaTypeEnumClassesHash, metaTypeEnumClasses)
#if defined(ALLOW_SCOPED_POINTER_METATYPE)
    CACHE_DESTRUCTOR_VAR(ElementMetaTypesOfSmartPointersHash, elementMetaTypesOfSmartPointers);
    CACHE_DESTRUCTOR_VAR(ElementDeletersOfSmartPointersHash, elementDeletersOfSmartPointers);
#endif
#if defined(QTJAMBI_GENERIC_ACCESS)
    CACHE_DESTRUCTOR_VAR(MetaSequenceHash, metaSequenceHash);
    CACHE_DESTRUCTOR_VAR(MetaAssociationHash, metaAssociationHash);
    CACHE_DESTRUCTOR_VAR(ContainerTypeInfoHash, containerTypeInfos);
    CACHE_DESTRUCTOR_VAR(BiContainerTypeInfoHash, biContainerTypeInfos);
#endif
#if defined(QTJAMBI_GENERIC_ACCESS)
    CACHE_DESTRUCTOR_VAR(SequentialContainerAccessFactoryHash, sequentialContainerAccessFactoryHash);
    CACHE_DESTRUCTOR_VAR(AssociativeContainerAccessFactoryHash, associativeContainerAccessFactoryHash);
#endif
#if defined(QTJAMBI_DEBUG_TOOLS)
    CACHE_DESTRUCTOR_VAR(LinkSet, linkSet);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    CACHE_DESTRUCTOR_VAR(NativeInterfaceMap, nativeInterfaceMap)
#endif
#ifdef QTJAMBI_LOG_CLASSNAMES
    CACHE_DESTRUCTOR_VAR(ClassNameHash, classNameHash)
#endif
    {
        QWriteLocker locker(m_lock);
        // coreapi
        CACHE_DESTRUCTOR_SWAP(bindableInterfacesHash);
        CACHE_DESTRUCTOR_SWAP(metaTypesByPointer);
#if defined(ALLOW_SCOPED_POINTER_METATYPE)
        CACHE_DESTRUCTOR_SWAP(elementMetaTypesOfSmartPointers);
        CACHE_DESTRUCTOR_SWAP(elementDeletersOfSmartPointers);
#endif
        // qmlapi
        CACHE_DESTRUCTOR_SWAP(creatorFunctionMetaDataHash);
        // enums
        CACHE_DESTRUCTOR_SWAP(enumClassValuesFields);
        CACHE_DESTRUCTOR_SWAP(enumClassExtensible);
        CACHE_DESTRUCTOR_SWAP(switchTableFields);
        // jna
        CACHE_DESTRUCTOR_SWAP(objectsByFunctionPointers);
        // typemanager
        CACHE_DESTRUCTOR_SWAP(internalToExternalConverters);
        CACHE_DESTRUCTOR_SWAP(externalToInternalConverters);
        CACHE_DESTRUCTOR_SWAP(hashFunctionByMetaTypeHash);
        CACHE_DESTRUCTOR_SWAP(metaEnumByMetaTypeHash);
#if defined(QTJAMBI_GENERIC_ACCESS)
        CACHE_DESTRUCTOR_SWAP(metaSequenceHash);
        CACHE_DESTRUCTOR_SWAP(metaAssociationHash);
        CACHE_DESTRUCTOR_SWAP(containerTypeInfos);
        CACHE_DESTRUCTOR_SWAP(biContainerTypeInfos);
#endif
        // typeentry
        CACHE_DESTRUCTOR_SWAP(typeEntries);
        // supertypeinfo
        CACHE_DESTRUCTOR_SWAP(superTypeInfos);
        // qtjambishell
        CACHE_DESTRUCTOR_SWAP(virtualTables);
        // qtjambimetaobject
        CACHE_DESTRUCTOR_SWAP(metaObjectsByName);
        CACHE_DESTRUCTOR_SWAP(parameterTypeInfos);
        CACHE_DESTRUCTOR_SWAP(metaObjects);
        CACHE_DESTRUCTOR_SWAP(signalTypes);
#if !defined(QTJAMBI_NO_GLOBALREFS)
        CACHE_DESTRUCTOR_SWAP(javaMetaObjects);
#endif
        // jnienvironment
        CACHE_DESTRUCTOR_SWAP(resettableBoolFlags);
        // messagehandler
        CACHE_DESTRUCTOR_SWAP(supportedMessageTypes);
        // containeraccess
        CACHE_DESTRUCTOR_SWAP(metaAssociationsByMetaType);
        CACHE_DESTRUCTOR_SWAP(metaSequencesByMetaType);
#if defined(QTJAMBI_GENERIC_ACCESS)
        CACHE_DESTRUCTOR_SWAP(sequentialContainerAccessFactoryHash);
        CACHE_DESTRUCTOR_SWAP(associativeContainerAccessFactoryHash);
#endif
        // qtjambilink lock()
        CACHE_DESTRUCTOR_SWAP(dependencies);
        CACHE_DESTRUCTOR_SWAP(conDestroyedDependencies);
        CACHE_DESTRUCTOR_SWAP(finalizations);
        CACHE_DESTRUCTOR_SWAP(finalizationData);
#if defined(QTJAMBI_DEBUG_TOOLS)
        CACHE_DESTRUCTOR_SWAP(linkSet);
#endif
        signalEmitThreadCheckHandler = std::move(m_signalEmitThreadCheckHandler);
    }
    {
        QWriteLocker locker(m_linkLock);
        // qtjambilink linkLock()
        CACHE_DESTRUCTOR_SWAP(userObjectCache);
        CACHE_DESTRUCTOR_SWAP(qObjectCache);
    }
    {
        QWriteLocker locker(m_registryLock);
        // registry
        CACHE_DESTRUCTOR_SWAP(jobjectNativeWrapperMetaTypes)
        CACHE_DESTRUCTOR_SWAP(typeQtNameHash)
        CACHE_DESTRUCTOR_SWAP(entryTypesHash)
        CACHE_DESTRUCTOR_SWAP(qtNameTypeHash)
        CACHE_DESTRUCTOR_SWAP(qtFunctionalJavaNameHash)
        CACHE_DESTRUCTOR_SWAP(qtFunctionalQtNameHash)
        CACHE_DESTRUCTOR_SWAP(qtNamespaceJavaNameHash)
        CACHE_DESTRUCTOR_SWAP(javaNamespaceQtNameHash)
        CACHE_DESTRUCTOR_SWAP(namespaceMetaObjectHash)
        CACHE_DESTRUCTOR_SWAP(typeJavaNameHash)
        CACHE_DESTRUCTOR_SWAP(javaNameTypeHash)
        CACHE_DESTRUCTOR_SWAP(iidTypeHash)
        CACHE_DESTRUCTOR_SWAP(javaClassIIDHash)
        CACHE_DESTRUCTOR_SWAP(iidByteArrayHash)
        CACHE_DESTRUCTOR_SWAP(flagEnumIDHash)
        CACHE_DESTRUCTOR_SWAP(enumFlagIDHash)
        CACHE_DESTRUCTOR_SWAP(typeJavaInterfaceHash)
        CACHE_DESTRUCTOR_SWAP(containerAccessFactories)
        CACHE_DESTRUCTOR_SWAP(sequentialContainerAccessFactoryByMetaTypes)
        CACHE_DESTRUCTOR_SWAP(associativeContainerAccessFactoryByMetaTypes)
        CACHE_DESTRUCTOR_SWAP(metaTypeByTypeInfos)
        CACHE_DESTRUCTOR_SWAP(typeInfoByMetaTypes)
        CACHE_DESTRUCTOR_SWAP(defaultValueHash)
        CACHE_DESTRUCTOR_SWAP(virtualFunctionInfos)
        CACHE_DESTRUCTOR_SWAP(constructorInfos)
        CACHE_DESTRUCTOR_SWAP(returnScopes)
        CACHE_DESTRUCTOR_SWAP(destructorHash)
        CACHE_DESTRUCTOR_SWAP(flagEnumNameHash)
        CACHE_DESTRUCTOR_SWAP(interfaceHash)
        CACHE_DESTRUCTOR_SWAP(interfaceIIDsHash)
        CACHE_DESTRUCTOR_SWAP(functionalHash)
        CACHE_DESTRUCTOR_SWAP(deleterHash)
        CACHE_DESTRUCTOR_SWAP(containerAccessMap)
        CACHE_DESTRUCTOR_SWAP(metaTypesForJObjectNativeWrappers)
        CACHE_DESTRUCTOR_SWAP(javaTypeByCustomMetaTypes)
        CACHE_DESTRUCTOR_SWAP(javaNameByMetaTypes)
        CACHE_DESTRUCTOR_SWAP(metaTypesByJavaTypeNames)
        CACHE_DESTRUCTOR_SWAP(jobjectWrappedMetaTypes)
        CACHE_DESTRUCTOR_SWAP(ownerFunctionHash)
        CACHE_DESTRUCTOR_SWAP(polymorphicIDs)
        CACHE_DESTRUCTOR_SWAP(polymorphicBasesHash)
        CACHE_DESTRUCTOR_SWAP(valueSizeHash)
        CACHE_DESTRUCTOR_SWAP(shellSizeHash)
        CACHE_DESTRUCTOR_SWAP(hashFunctionHash)
        CACHE_DESTRUCTOR_SWAP(offsetHash)
        CACHE_DESTRUCTOR_SWAP(interfacesHash)
        CACHE_DESTRUCTOR_SWAP(interfaceOffsetHash)
        CACHE_DESTRUCTOR_SWAP(originalMetaObjectHash)
        CACHE_DESTRUCTOR_SWAP(customMetaObject)
        CACHE_DESTRUCTOR_SWAP(signalMetaInfos)
        CACHE_DESTRUCTOR_SWAP(extraSignalsOfMetaObject)
        CACHE_DESTRUCTOR_SWAP(typeIdsByMetaObject)
        CACHE_DESTRUCTOR_SWAP(metaMethodRenamerFnByMetaObject)
        CACHE_DESTRUCTOR_SWAP(valueOwners)
        CACHE_DESTRUCTOR_SWAP(valueOwnerHash)
        CACHE_DESTRUCTOR_SWAP(parameterTypeInfoProviders)
        CACHE_DESTRUCTOR_SWAP(renamedMethodsHash)
        CACHE_DESTRUCTOR_SWAP(functionalResolverHash)
        CACHE_DESTRUCTOR_SWAP(typeInfoSupplierHash)
        CACHE_DESTRUCTOR_SWAP(mediaControlIIDHash)
        CACHE_DESTRUCTOR_SWAP(mediaControlIIDClassHash)
        CACHE_DESTRUCTOR_SWAP(classHash)
        CACHE_DESTRUCTOR_SWAP(fieldHash)
        CACHE_DESTRUCTOR_SWAP(methodHash)
        CACHE_DESTRUCTOR_SWAP(qtSuperclassHash)
        CACHE_DESTRUCTOR_SWAP(metaTypeMetaObjectHash)
        CACHE_DESTRUCTOR_SWAP(typeNames)
        CACHE_DESTRUCTOR_SWAP(classMetaTypeInterfaces)
        CACHE_DESTRUCTOR_SWAP(metaTypeInterfaces)
        CACHE_DESTRUCTOR_SWAP(metaTypeEnumClasses)
        CACHE_DESTRUCTOR_SWAP(globalClassPointers)
#ifdef QTJAMBI_LOG_CLASSNAMES
        CACHE_DESTRUCTOR_SWAP(classNameHash)
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
        CACHE_DESTRUCTOR_SWAP(nativeInterfaceMap)
#endif
    }
}

QtJambiStorage::~QtJambiStorage(){
    hash = 0;
    CACHE_DESTRUCTOR_VAR(ClassIdHash, classHash)
    CACHE_DESTRUCTOR_VAR(ObjectsByFunctionPointerHash, objectsByFunctionPointers)
    CACHE_DESTRUCTOR_VAR(SuperTypeInfoHash, superTypeInfos)
    CACHE_DESTRUCTOR_VAR(MetaObjectByMetaTypeHash, metaObjects)
    CACHE_DESTRUCTOR_VAR(SignalTypesHash, signalTypes)
    CACHE_DESTRUCTOR_VAR(ResettableBoolFlagHash, resettableBoolFlags)
    CACHE_DESTRUCTOR_VAR(JObjectWrapper, signalEmitThreadCheckHandler)
    CACHE_DESTRUCTOR_VAR(GlobalClassPointers, globalClassPointers)
#ifdef QTJAMBI_LOG_CLASSNAMES
    CACHE_DESTRUCTOR_VAR(ClassNameHash, classNameHash)
#endif
#if !defined(QTJAMBI_NO_GLOBALREFS)
    CACHE_DESTRUCTOR_VAR(JavaMetaObjectHash, javaMetaObjects)
#endif
    cleanup(globalClassPointers,
            classHash,
            signalEmitThreadCheckHandler,
            resettableBoolFlags,
            metaObjects,
            signalTypes,
#ifdef QTJAMBI_LOG_CLASSNAMES
            classNameHash,
#endif
#if !defined(QTJAMBI_NO_GLOBALREFS)
            javaMetaObjects,
#endif
            objectsByFunctionPointers,
            superTypeInfos);
    delete m_lock;
}

QtJambiStorage* QtJambiStorage::INSTANCE = new QtJambiStorage(true);

QtJambiStorage* getQtJambiStorage(){
    static thread_local QtJambiStorageRef cache;
    return cache.get();
}

void clearQtJambiStorage(JNIEnv* env, bool regular){
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::ClassIdHash, classHash)
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::ObjectsByFunctionPointerHash, objectsByFunctionPointers)
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::SuperTypeInfoHash, superTypeInfos)
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::MetaObjectByMetaTypeHash, metaObjects)
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::SignalTypesHash, signalTypes)
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::ResettableBoolFlagHash, resettableBoolFlags)
    CACHE_DESTRUCTOR_VAR(JObjectWrapper, signalEmitThreadCheckHandler)
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::GlobalClassPointers, globalClassPointers)
#ifdef QTJAMBI_LOG_CLASSNAMES
    CACHE_DESTRUCTOR_VAR(ClassNameHash, classNameHash)
#endif
#if !defined(QTJAMBI_NO_GLOBALREFS)
    CACHE_DESTRUCTOR_VAR(QtJambiStorage::JavaMetaObjectHash, javaMetaObjects)
#endif
    if(QtJambiStorage* cache = QtJambiStorage::INSTANCE){
        cache->cleanup(globalClassPointers,
                       classHash,
                       signalEmitThreadCheckHandler,
                       resettableBoolFlags,
                       metaObjects,
                       signalTypes,
#ifdef QTJAMBI_LOG_CLASSNAMES
                       classNameHash,
#endif
#if !defined(QTJAMBI_NO_GLOBALREFS)
                       javaMetaObjects,
#endif
                       objectsByFunctionPointers,
                       superTypeInfos);
        cache->deref();
    }
    for(auto iter = resettableBoolFlags.keyValueBegin(), end = resettableBoolFlags.keyValueEnd(); iter!=end; ++iter){
        iter->first->value = false;
        iter->first->registered = false;
    }
    QList<QSharedPointer<const QtJambiMetaObject>> metaObjectList;
    if(env){
        for(QList<JObjectWrapper>& objects : objectsByFunctionPointers){
            for(JObjectWrapper& o : objects){
                o.clear(env);
            }
        }
        for(SuperTypeInfos& info : superTypeInfos){
            info.m_interfaceInfos.clear(env);
        }
        for(const QMetaObject* mo : std::as_const(metaObjects)){
            if(const QtJambiMetaObject* dynamo = QtJambiMetaObject::cast(mo)){
                // delete self-reference
                metaObjectList << dynamo->dispose(env);
            }
        }
#if !defined(QTJAMBI_NO_GLOBALREFS)
        for(jweak w : qAsConst(javaMetaObjects)){
            env->DeleteWeakGlobalRef(w);
        }
#endif
        for(JObjectWrapper& obj : signalTypes){
            obj.clear(env);
        }
        signalEmitThreadCheckHandler.clear(env);
#ifndef QTJAMBI_NOCACHE
        if(regular && Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.purge-globalrefs-at-shutdown"))){
            for(void** ptr : std::as_const(globalClassPointers)){
                *ptr = nullptr;
            }
#ifdef QTJAMBI_LOG_CLASSNAMES
            printf("DeleteGlobalRef of %i classes:\n", classHash.keys().size());
            int counter = 0;
#endif
#ifdef QTJAMBI_LOG_CLASSNAMES
            for(size_t key : classHash.keys()){
                jclass cls = classHash[key];
                printf("%i.: class %s\n", ++counter, qPrintable((*classNameHash)[key]));
#else
            for(jclass cls : std::as_const(classHash)){
#endif
                if(env->GetObjectRefType(cls)==JNIGlobalRefType)
                    env->DeleteGlobalRef(cls);
            }
        }
#endif // QTJAMBI_NOCACHE
    }
    if(QThread *thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
        if(thread->isRunning()){
            if(QThreadObjectData* data = QtJambiObjectData::userData<QThreadObjectData>(thread)){
                data->doAtThreadEnd([metaObjectList = std::move(metaObjectList)](){/*delete captured list after function call*/});
            }
        }
    }
}

const QtPrivate::QMetaTypeInterface * getOriginalForNativeWrapperMetaType(const QtPrivate::QMetaTypeInterface *iface){
    const QtPrivate::QMetaTypeInterface *original{nullptr};
    QtJambiStorage* cache = getQtJambiStorage();
    {
        QReadLocker locker(cache->lock());
        original = cache->metaTypesForJObjectNativeWrappers()[iface];
    }
    return original;
}

QMetaType getNativeWrapperType(const QMetaType& metaType){
    int originalMetaTypeId = metaType.id();
    const QtPrivate::QMetaTypeInterface *originalMetaTypeInterface{META_TYPE_ACCESS(metaType).iface()};
    const QtPrivate::QMetaTypeInterface * nativeWrapperType{nullptr};
    if(metaType.flags().testFlag(QMetaType::IsPointer)){
        QtJambiStorage* cache = getQtJambiStorage();
        {
            {
                QReadLocker locker(cache->registryLock());
                nativeWrapperType = cache->jobjectNativeWrapperMetaTypes()[originalMetaTypeId];
            }
            if(!nativeWrapperType){
                bool isQObject = metaType.flags().testFlag(QMetaType::PointerToQObject);
                QByteArray typeName = QByteArray("JObjectWrapper<") + QByteArray(metaType.name()) + ">";
                const char* name = registerMetaTypeName(typeName);
                QtPrivate::QMetaTypeInterface* metaTypeInterface;
                const QMetaObject *originalMetaObject{nullptr};
                if(isQObject){
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                    uint flags = QtPrivate::QMetaTypeTypeFlags<JQObjectWrapper>::Flags;
#else
                    uint flags = QtPrivate::QMetaTypeForType<JQObjectWrapper>::flags();
#endif
                    flags |= QMetaType::PointerToQObject;
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
                    if(metaType.flags().testFlag(QMetaType::IsConst))
                        flags |= QMetaType::IsConst;
#endif
                    metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                        /*.revision=*/ QMetaTypeInterface_CurrentRevision,
                        /*.alignment=*/ alignof(JQObjectWrapper),
                        /*.size=*/ sizeof(JQObjectWrapper),
                        /*.flags=*/ flags,
                        /*.typeId=*/ QMetaType::UnknownType,
                        /*.metaObject=*/ originalMetaTypeInterface->metaObjectFn,
                        /*.name=*/ name,
                        /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getDefaultCtr(),
                        /*.copyCtr=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getCopyCtr(),
                        /*.moveCtr=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getMoveCtr(),
                        /*.dtor=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getDtor(),
                        /*.equals=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                            const JQObjectWrapper* _value1 = reinterpret_cast<const JQObjectWrapper*>(value1);
                            const JQObjectWrapper* _value2 = reinterpret_cast<const JQObjectWrapper*>(value2);
                            return _value1->qobject()==_value2->qobject();
                        },
                        /*.lessThan=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                            const JQObjectWrapper* _value1 = reinterpret_cast<const JQObjectWrapper*>(value1);
                            const JQObjectWrapper* _value2 = reinterpret_cast<const JQObjectWrapper*>(value2);
                            return _value1->qobject()<_value2->qobject();
                        },
                        /*.debugStream=*/ originalMetaTypeInterface->debugStream ? [](const QtPrivate::QMetaTypeInterface *iface, QDebug &d, const void *value){
                            if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                                const JQObjectWrapper* _value = reinterpret_cast<const JQObjectWrapper*>(value);
                                original->debugStream(original, d, _value->qobject());
                            }
                        } : QtPrivate::QMetaTypeInterface::DebugStreamFn(nullptr),
                        /*.dataStreamOutFn=*/ originalMetaTypeInterface->dataStreamOut ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
                            if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                                const JQObjectWrapper* _value = reinterpret_cast<const JQObjectWrapper*>(value);
                                original->dataStreamOut(original, d, _value->qobject());
                            }
                        } : QtPrivate::QMetaTypeInterface::DataStreamOutFn(nullptr),
                        /*.dataStreamInFn=*/ originalMetaTypeInterface->dataStreamIn ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
                            if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                                JQObjectWrapper* _value = reinterpret_cast<JQObjectWrapper*>(value);
                                original->dataStreamIn(original, d, _value->qobject());
                            }
                        } : QtPrivate::QMetaTypeInterface::DataStreamInFn(nullptr),
                        /*.legacyRegisterOp=*/ nullptr
                    };
                }else{
                    if(originalMetaTypeInterface->metaObjectFn)
                        originalMetaObject = originalMetaTypeInterface->metaObjectFn(originalMetaTypeInterface);
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                    uint flags = QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags;
#else
                    uint flags = QtPrivate::QMetaTypeForType<JObjectWrapper>::flags();
#endif
                    if(originalMetaObject)
                        flags |= QMetaType::IsGadget;
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
                    if(metaType.flags().testFlag(QMetaType::IsConst))
                        flags |= QMetaType::IsConst;
#endif
                    metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                        /*.revision=*/ QMetaTypeInterface_CurrentRevision,
                        /*.alignment=*/ alignof(JObjectWrapper),
                        /*.size=*/ sizeof(JObjectWrapper),
                        /*.flags=*/ flags,
                        /*.typeId=*/ QMetaType::UnknownType,
                        /*.metaObject=*/ originalMetaObject ? &metaobjectByMetaTypeInterface : nullptr,
                        /*.name=*/ name,
                        /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getDefaultCtr(),
                        /*.copyCtr=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getCopyCtr(),
                        /*.moveCtr=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getMoveCtr(),
                        /*.dtor=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getDtor(),
                        /*.equals=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                            const JObjectWrapper* _value1 = reinterpret_cast<const JObjectWrapper*>(value1);
                            const JObjectWrapper* _value2 = reinterpret_cast<const JObjectWrapper*>(value2);
                            if(JniEnvironment env{200}){
                                if (QSharedPointer<QtJambiLink> link1 = QtJambiLink::findLinkForJavaInterface(env, _value1->object(env))){
                                    if (QSharedPointer<QtJambiLink> link2 = QtJambiLink::findLinkForJavaInterface(env, _value2->object(env))){
                                        return link1->pointer()==link2->pointer();
                                    }
                                }
                            }
                            return false;
                        },
                        /*.lessThan=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                            const JObjectWrapper* _value1 = reinterpret_cast<const JObjectWrapper*>(value1);
                            const JObjectWrapper* _value2 = reinterpret_cast<const JObjectWrapper*>(value2);
                            if(JniEnvironment env{200}){
                                if (QSharedPointer<QtJambiLink> link1 = QtJambiLink::findLinkForJavaInterface(env, _value1->object(env))){
                                    if (QSharedPointer<QtJambiLink> link2 = QtJambiLink::findLinkForJavaInterface(env, _value2->object(env))){
                                        return link1->pointer()<link2->pointer();
                                    }
                                }
                            }
                            return false;
                        },
                        /*.debugStream=*/ originalMetaTypeInterface->debugStream ? [](const QtPrivate::QMetaTypeInterface *iface, QDebug &d, const void *value){
                            if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                                const JObjectWrapper* _value = reinterpret_cast<const JObjectWrapper*>(value);
                                if(JniEnvironment env{200}){
                                    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, _value->object(env))){
                                        original->debugStream(original, d, link->pointer());
                                    }
                                }
                            }
                        } : QtPrivate::QMetaTypeInterface::DebugStreamFn(nullptr),
                        /*.dataStreamOutFn=*/ originalMetaTypeInterface->dataStreamOut ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
                            if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                                const JObjectWrapper* _value = reinterpret_cast<const JObjectWrapper*>(value);
                                if(JniEnvironment env{200}){
                                    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, _value->object(env))){
                                        original->dataStreamOut(original, d, link->pointer());
                                    }
                                }
                            }
                        } : QtPrivate::QMetaTypeInterface::DataStreamOutFn(nullptr),
                        /*.dataStreamInFn=*/ originalMetaTypeInterface->dataStreamIn ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
                            if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                                JObjectWrapper* _value = reinterpret_cast<JObjectWrapper*>(value);
                                if(JniEnvironment env{200}){
                                    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, _value->object(env))){
                                        original->dataStreamIn(original, d, link->pointer());
                                    }
                                }
                            }
                        } : QtPrivate::QMetaTypeInterface::DataStreamInFn(nullptr),
                        /*.legacyRegisterOp=*/ nullptr
                    };
                }
                {
                    QWriteLocker locker(cache->registryLock());
                    if(const QtPrivate::QMetaTypeInterface * iface = cache->jobjectNativeWrapperMetaTypes()[originalMetaTypeId]){
                        delete metaTypeInterface;
                        nativeWrapperType = iface;
                    }else{
                        cache->jobjectNativeWrapperMetaTypes()[originalMetaTypeId] = metaTypeInterface;
                        cache->metaTypesForJObjectNativeWrappers()[metaTypeInterface] = originalMetaTypeInterface;
                        if(originalMetaObject){
                            locker.unlock();
                            QMetaObjectBuilder builder;
                            builder.setClassName(name);
                            for(int i=0, l=originalMetaObject->methodCount(); i<l; ++i){
                                QMetaMethod method = originalMetaObject->method(i);
                                builder.addMethod(method);
                            }
                            for(int i=0, l=originalMetaObject->constructorCount(); i<l; ++i){
                                QMetaMethod constructor = originalMetaObject->constructor(i);
                                builder.addConstructor(constructor);
                            }
                            for(int i=0, l=originalMetaObject->propertyCount(); i<l; ++i){
                                QMetaProperty property = originalMetaObject->property(i);
                                builder.addProperty(property);
                            }
                            if(isQObject){
                                builder.setFlags(PropertyAccessInStaticMetaCall);
                                builder.setStaticMetacallFunction([](QObject *gadget, QMetaObject::Call call, int argc, void **argv){
                                    JObjectWrapper* wrapper = reinterpret_cast<JObjectWrapper*>(gadget);
                                    if(JniEnvironment env{200}){
                                        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, wrapper->object(env))){
                                            Q_ASSERT(link->isQObject());
                                            link->qobject()->qt_metacall(call, argc, argv);
                                        }
                                    }
                                });
                            }else if(originalMetaObject->d.static_metacall){
                                builder.setFlags(PropertyAccessInStaticMetaCall);
                                builder.setStaticMetacallFunction([](QObject *gadget, QMetaObject::Call call, int argc, void **argv){
                                    JObjectWrapper* wrapper = reinterpret_cast<JObjectWrapper*>(gadget);
                                    if(JniEnvironment env{200}){
                                        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, wrapper->object(env))){
                                            QMetaType metaType;
                                            if(const QMetaType* metaTypePtr = link->metaType()){
                                                metaType = *metaTypePtr;
                                            }else if(jclass object_class = env->GetObjectClass(wrapper->object(env))){
                                                QString qtName = QtJambiTypeManager::getInternalTypeName(env, object_class, false);
                                                metaType = QMetaType::fromName(qPrintable(qtName));
                                            }
                                            if(metaType.metaObject() && metaType.metaObject()->d.static_metacall)
                                                metaType.metaObject()->d.static_metacall(reinterpret_cast<QObject*>(link->pointer()), call, argc, argv);
                                        }
                                    }
                                });
                            }
                            QMetaObject* mo = builder.toMetaObject();
                            mo->d.superdata.direct = nullptr;
#ifdef QT_NO_DATA_RELOCATION
                            mo->d.superdata.indirect = nullptr;
#endif
                            locker.relock();
                            cache->metaTypeMetaObjectHash().insert(metaTypeInterface, mo);
                        }
                        nativeWrapperType = metaTypeInterface;
                        QMetaType nativeWrapperMetaType(nativeWrapperType);
                        (void)nativeWrapperMetaType.id();
                        locker.unlock();
                        if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperMetaType, QMetaType::fromType<QString>())){
                            if(isQObject){
                                QMetaType::registerConverterFunction([metaType](const void *src, void *target) -> bool {
                                    new (target)QString();
                                    if(src){
                                        QObject* obj = reinterpret_cast<const JQObjectWrapper*>(src)->qobject();
                                        return QMetaType::convert(metaType, &obj, QMetaType::fromType<QString>(), target);
                                    }
                                    return true;
                                }, nativeWrapperMetaType, QMetaType::fromType<QString>());
                            }else{
                                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                    bool ok = false;
                                    if(src){
                                        new (target)QString(reinterpret_cast<const JObjectWrapper*>(src)->toString(&ok));
                                    }else{
                                        ok = true;
                                        new (target)QString();
                                    }
                                    return ok;
                                }, nativeWrapperMetaType, QMetaType::fromType<QString>());
                            }
                        }
                        locker.relock();
                    }
                }
            }
        }
    }
    return QMetaType(nativeWrapperType);
}

struct JQObjectWrapperPrivate{
    JQObjectWrapperPrivate(QObject*& object, JNIEnv* env, QSharedPointer<QtJambiLink>&& link);
    void unref(QObject*& pointer);
    void ref(QObject*& pointer);
    void exchange(QObject*& pointer, QObject*& byPointer);
    QSharedPointer<QtJambiLink> m_link;
    QMutex m_mutex;
    QSet<QObject**> m_pointers;
private:
    ~JQObjectWrapperPrivate();
};

JQObjectWrapperPrivate::JQObjectWrapperPrivate(QObject*& object, JNIEnv* env, QSharedPointer<QtJambiLink>&& link)
    : m_link(std::move(link)), m_mutex(), m_pointers() {
    Q_ASSERT(!m_link.isNull());
    Q_ASSERT(m_link->ownership()==QtJambiLink::Ownership::Java);
    m_link->setCppOwnership(env);
    m_pointers.insert(&object);
    if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object)){
        dm->addFinalization(this,
                            [](JNIEnv*, void* data){
                                JQObjectWrapperPrivate* p = reinterpret_cast<JQObjectWrapperPrivate*>(data);
                                QSet<QObject**> pointers;
                                {
                                    QMutexLocker<QMutex> locker(&p->m_mutex);
                                    p->m_pointers.swap(pointers);
                                }
                                for(QObject** pointer : std::as_const(pointers)){
                                    *pointer = nullptr;
                                }
                            }, nullptr);
    }
}

void JQObjectWrapperPrivate::unref(QObject*& pointer){
    bool isEmpty;
    {
        QMutexLocker<QMutex> locker(&m_mutex);
        m_pointers.remove(&pointer);
        isEmpty = m_pointers.isEmpty();
    }
    if(isEmpty)
        delete this;
}

void JQObjectWrapperPrivate::ref(QObject*& pointer){
    {
        QMutexLocker<QMutex> locker(&m_mutex);
        m_pointers.insert(&pointer);
    }
}

void JQObjectWrapperPrivate::exchange(QObject*& pointer, QObject*& byPointer){
    {
        QMutexLocker<QMutex> locker(&m_mutex);
        m_pointers.remove(&pointer);
        m_pointers.insert(&byPointer);
    }
}

JQObjectWrapperPrivate::~JQObjectWrapperPrivate(){
    try{
        if(QObject* object = m_link->qobject()){
            JniEnvironment env{200};
            try{
                if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object, false)){
                    dm->removeFinalization(env, this);
                }
                if(!isQmlJavaScriptOwnership(object) && env)
                    m_link->setJavaOwnership(env);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){
    }
}

JQObjectWrapper::JQObjectWrapper()
    : m_qobject(nullptr), p(nullptr) {
}

JQObjectWrapper::JQObjectWrapper(JNIEnv* env, QSharedPointer<QtJambiLink>&& link)
    : m_qobject(link->qobject()), p(new JQObjectWrapperPrivate(m_qobject, env, std::move(link))){
}

JQObjectWrapper::~JQObjectWrapper(){
    if(p)
        p->unref(m_qobject);
    m_qobject = nullptr;
    p = nullptr;
}

JQObjectWrapper::JQObjectWrapper(const JQObjectWrapper& other)
    : m_qobject(other.m_qobject), p(other.p){
    if(p)
        p->ref(m_qobject);
}

JQObjectWrapper::JQObjectWrapper(JQObjectWrapper&& other)
    : m_qobject(other.m_qobject), p(other.p){
    if(p)
        p->exchange(other.m_qobject, m_qobject);
    other.p = nullptr;
    other.m_qobject = nullptr;
}

JQObjectWrapper& JQObjectWrapper::operator=(const JQObjectWrapper &other){
    if(other.p!=p){
        if(p)
            p->unref(m_qobject);
        m_qobject = other.m_qobject;
        p = other.p;
        if(p){
            p->ref(m_qobject);
        }
    }
    return *this;
}

JQObjectWrapper& JQObjectWrapper::operator=(JQObjectWrapper &&other){
    if(other.p!=p){
        if(p)
            p->unref(m_qobject);
        m_qobject = other.m_qobject;
        p = other.p;
        if(p){
            p->exchange(other.m_qobject, m_qobject);
        }
        other.p = nullptr;
        other.m_qobject = nullptr;
    }
    return *this;
}

QSharedPointer<QtJambiLink> JQObjectWrapper::link() const{
    if(p)
        return  p->m_link;
    else
        return {};
}

jobject JQObjectWrapper::javaObject(JNIEnv* env) const{
    if(p)
        return  p->m_link->getJavaObjectLocalRef(env);
    else
        return nullptr;
}
