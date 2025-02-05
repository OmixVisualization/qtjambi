/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QMetaType>
#include <QtCore/QHash>
#include "registryapi.h"
#include "utils_p.h"
#include "coreapi.h"
#include "containerapi.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
namespace QtPrivate{
class DummyType{};
template<>
struct MetaTypeSmartPointerHelper<DummyType,DummyType>{
    static bool registerComparatorFunction(const QtPrivate::AbstractComparatorFunction *f, int typeId){
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
        if(!QMetaType::hasRegisteredComparators(typeId))
            return QMetaType::registerComparatorFunction(f, typeId);
#else
        Q_UNUSED(typeId)
        Q_UNUSED(f)
#endif
        return false;
    }
    static bool registerConverterFunction(const AbstractConverterFunction *f, int from, int to)
    {
        if(QMetaType::hasRegisteredConverterFunction(from, to))
            return false;
        return QMetaType::registerConverterFunction(f, from, to);
    }
    static void unregisterConverterFunction(int from, int to)
    {
        QMetaType::unregisterConverterFunction(from, to);
    }
    static bool registerDebugStreamOperatorFunction(const AbstractDebugStreamFunction *f, int typeId)
    {
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
        if(!QMetaType::hasRegisteredDebugStreamOperator(typeId))
            return QMetaType::registerDebugStreamOperatorFunction(f, typeId);
#else
        Q_UNUSED(typeId)
        Q_UNUSED(f)
#endif
        return false;
    }
};
}

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gFunctionsLock, (QReadWriteLock::Recursive))
typedef SecureContainer<QHash<int,const QtPrivate::AbstractComparatorFunction *>, gFunctionsLock> ComparatorFunctions;
Q_GLOBAL_STATIC(ComparatorFunctions, gComparatorFunctions)
typedef SecureContainer<QHash<int,const QtPrivate::AbstractDebugStreamFunction *>, gFunctionsLock> DebugStreamFunctions;
Q_GLOBAL_STATIC(DebugStreamFunctions, gDebugStreamFunctions)

bool RegistryAPI::registerComparator(const QtPrivate::AbstractComparatorFunction *f, int typeId){
    if(QMetaType::hasRegisteredComparators(typeId))
        return true;
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerComparatorFunction(f, typeId);
#else
    QWriteLocker locker(gFunctionsLock());
    gComparatorFunctions->insert(typeId, f);
    return true;
#endif
}

const QtPrivate::AbstractComparatorFunction * registeredComparator(int typeId){
    QReadLocker locker(gFunctionsLock());
    return gComparatorFunctions->value(typeId);
}

const QtPrivate::AbstractDebugStreamFunction * registeredDebugStreamOperator(int typeId){
    QReadLocker locker(gFunctionsLock());
    return gDebugStreamFunctions->value(typeId);
}

const QtPrivate::AbstractDebugStreamFunction *CoreAPI::registeredDebugStreamOperator(int typeId){
    QReadLocker locker(gFunctionsLock());
    return gDebugStreamFunctions->value(typeId);
}

bool RegistryAPI::registerDebugStreamOperator(const QtPrivate::AbstractDebugStreamFunction *f, int typeId){
    if(QMetaType::hasRegisteredDebugStreamOperator(typeId))
        return false;
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerDebugStreamOperatorFunction(f, typeId);
#else
    QWriteLocker locker(gFunctionsLock());
    gDebugStreamFunctions->insert(typeId, f);
    return true;
#endif
}

bool registerConverter(const QtPrivate::AbstractConverterFunction *f, int from, int to){
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerConverterFunction(f, from, to);
#else
    if(QMetaType::hasRegisteredConverterFunction(from, to))
        return false;
    return QMetaType::registerConverterFunction(f, from, to);
#endif
}

void qtjambi_unregister_converter(int from, int to){
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::unregisterConverterFunction(from, to);
#else
    QMetaType::unregisterConverterFunction(from, to);
#endif
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
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
#else
QVariant VariantUtility::createVariant(const QMetaType& type, const void *data){
    return QVariant(type.id(), data);
}
#endif


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
            [](void* data, const void* ptr, hash_type seed) -> hash_type{
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

hash_type QHashFunction::operator()(const void* ptr, hash_type seed) const{
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool isLessThan(const QMetaType& keyMetaType, const void * ptr, const void* ptr2){
    int r = 0;
    if(QMetaType::compare(ptr, ptr2, keyMetaType.id(), &r))
        return r<0;
    const QtPrivate::AbstractComparatorFunction * comparator = registeredComparator(keyMetaType.id());
    if(comparator && comparator->lessThan)
        return comparator->lessThan(comparator, ptr, ptr2);
    if(keyMetaType.id()<QMetaType::User)
        return QVariant(keyMetaType.id(), ptr)<QVariant(keyMetaType.id(), ptr2);
    if(keyMetaType.flags() & QMetaType::IsEnumeration){
        switch(keyMetaType.sizeOf()){
        case 1: return *reinterpret_cast<const qint8*>(ptr) < *reinterpret_cast<const qint8*>(ptr2);
        case 2: return *reinterpret_cast<const qint16*>(ptr) < *reinterpret_cast<const qint16*>(ptr2);
        case 4: return *reinterpret_cast<const qint32*>(ptr) < *reinterpret_cast<const qint32*>(ptr2);
        case 8: return *reinterpret_cast<const qint64*>(ptr) < *reinterpret_cast<const qint64*>(ptr2);
        default: break;
        }
    }else if(AbstractContainerAccess::isPointerType(keyMetaType)){
        return *reinterpret_cast<const quintptr*>(ptr) < *reinterpret_cast<const quintptr*>(ptr2);
    }
    return false;
}

bool isEquals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2){
    int r = 0;
    if(QMetaType::equals(ptr, ptr2, keyMetaType.id(), &r))
        return r==0;
    const QtPrivate::AbstractComparatorFunction * comparator = registeredComparator(keyMetaType.id());
    if(comparator && comparator->equals)
        return comparator->equals(comparator, ptr, ptr2);
    if(comparator && comparator->lessThan)
        return comparator->lessThan(comparator, ptr, ptr2) && comparator->lessThan(comparator, ptr2, ptr);
    if(keyMetaType.id()<QMetaType::User)
        return QVariant(keyMetaType.id(), ptr)==QVariant(keyMetaType.id(), ptr2);
    if(keyMetaType.flags() & QMetaType::IsEnumeration){
        switch(keyMetaType.sizeOf()){
        case 1: return *reinterpret_cast<const qint8*>(ptr) == *reinterpret_cast<const qint8*>(ptr2);
        case 2: return *reinterpret_cast<const qint16*>(ptr) == *reinterpret_cast<const qint16*>(ptr2);
        case 4: return *reinterpret_cast<const qint32*>(ptr) == *reinterpret_cast<const qint32*>(ptr2);
        case 8: return *reinterpret_cast<const qint64*>(ptr) == *reinterpret_cast<const qint64*>(ptr2);
        default: break;
        }
    }else if(AbstractContainerAccess::isPointerType(keyMetaType)){
        return *reinterpret_cast<const quintptr*>(ptr) == *reinterpret_cast<const quintptr*>(ptr2);
    }
    return false;
}

QDebug& CoreAPI::appendToDebugStream(QDebug& debug, uint typeId, const void* ptr)
{
    if(!QMetaType::debugStream(debug, ptr, typeId)){
        if(const QtPrivate::AbstractDebugStreamFunction *function = registeredDebugStreamOperator(typeId)){
            function->stream(function, debug, ptr);
        }else if(typeId<QMetaType::User){
            QString tmp;
            {
                QDebug(&tmp) << QVariant(typeId, ptr);
            }
            tmp = tmp.trimmed();
            QString suffix = QLatin1String("QVariant(%1, ");
            suffix = suffix.arg(QLatin1String(QMetaType::typeName(typeId)));
            if(tmp.startsWith(suffix) && tmp.endsWith(")")){
                tmp = tmp.mid(suffix.length()).chopped(1);
            }
            debug.noquote() << tmp;
        }else if(AbstractContainerAccess::isPointerType(QMetaType(typeId))){
            debug << QMetaType::typeName(typeId) << "@" << QString::number(*reinterpret_cast<const quintptr*>(ptr), 16);
        }else if(QMetaType::typeFlags(typeId) & QMetaType::IsEnumeration){
            switch(QMetaType::sizeOf(typeId)){
            case 1: return debug << QMetaType::typeName(typeId) << "(" << *reinterpret_cast<const qint8*>(ptr) << ")";
            case 2: return debug << QMetaType::typeName(typeId) << "(" << *reinterpret_cast<const qint16*>(ptr) << ")";
            case 4: return debug << QMetaType::typeName(typeId) << "(" << *reinterpret_cast<const qint32*>(ptr) << ")";
            case 8: return debug << QMetaType::typeName(typeId) << "(" << *reinterpret_cast<const qint64*>(ptr) << ")";
            default: break;
            }
        }
    }
    return debug;
}
#endif
