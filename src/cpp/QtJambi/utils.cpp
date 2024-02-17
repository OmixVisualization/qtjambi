/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

typedef QHash<int,const QtPrivate::AbstractComparatorFunction *> ComparatorFunctions;
Q_GLOBAL_STATIC(ComparatorFunctions, gComparatorFunctions)
typedef QHash<int,const QtPrivate::AbstractDebugStreamFunction *> DebugStreamFunctions;
Q_GLOBAL_STATIC(DebugStreamFunctions, gDebugStreamFunctions)

bool RegistryAPI::registerComparator(const QtPrivate::AbstractComparatorFunction *f, int typeId){
    if(QMetaType::hasRegisteredComparators(typeId))
        return true;
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerComparatorFunction(f, typeId);
#else
    gComparatorFunctions->insert(typeId, f);
    return true;
#endif
}

const QtPrivate::AbstractComparatorFunction * registeredComparator(int typeId){
    return gComparatorFunctions->value(typeId);
}

const QtPrivate::AbstractDebugStreamFunction * registeredDebugStreamOperator(int typeId){
    return gDebugStreamFunctions->value(typeId);
}

const QtPrivate::AbstractDebugStreamFunction *CoreAPI::registeredDebugStreamOperator(int typeId){
    return gDebugStreamFunctions->value(typeId);
}

bool RegistryAPI::registerDebugStreamOperator(const QtPrivate::AbstractDebugStreamFunction *f, int typeId){
    if(QMetaType::hasRegisteredDebugStreamOperator(typeId))
        return false;
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerDebugStreamOperatorFunction(f, typeId);
#else
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

class InternalToExternalConverterPointerData : public InternalToExternalConverterPrivate{
public:
    inline InternalToExternalConverterPointerData(InternalToExternalConverter::FunctionPointer functionPointer) noexcept
     : m_functionPointer(functionPointer){Q_ASSERT(functionPointer);}
    inline bool invoke(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType) const override
     { return m_functionPointer(env, scope, in, out, forceBoxedType); }
private:
    InternalToExternalConverter::FunctionPointer m_functionPointer;
};
InternalToExternalConverter::InternalToExternalConverter(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new InternalToExternalConverterPointerData(functor)){}

InternalToExternalConverterPrivate::InternalToExternalConverterPrivate() noexcept {}
InternalToExternalConverterPrivate::~InternalToExternalConverterPrivate() {}
InternalToExternalConverter::InternalToExternalConverter() noexcept : d(){}
InternalToExternalConverter::InternalToExternalConverter(const InternalToExternalConverter& other) noexcept : d(other.d) {}
InternalToExternalConverter::InternalToExternalConverter(InternalToExternalConverter&& other) noexcept : d(std::move(other.d)) {}
InternalToExternalConverter::InternalToExternalConverter(InternalToExternalConverterPrivate* _d) noexcept : d(_d) {}
InternalToExternalConverter& InternalToExternalConverter::operator=(InternalToExternalConverter& other) noexcept { d = other.d; return *this; }
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
    return d->invoke(env, scope, in, out, forceBoxedType);
}

class ExternalToInternalConverterPointerData : public ExternalToInternalConverterPrivate{
public:
    inline ExternalToInternalConverterPointerData(ExternalToInternalConverter::FunctionPointer functionPointer) noexcept
     : m_functionPointer(functionPointer){Q_ASSERT(functionPointer);}
    inline bool invoke(JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) const override
     { return m_functionPointer(env, scope, val, out, valueType); }
private:
    ExternalToInternalConverter::FunctionPointer m_functionPointer;
};
ExternalToInternalConverter::ExternalToInternalConverter(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new ExternalToInternalConverterPointerData(functor)){}

ExternalToInternalConverterPrivate::ExternalToInternalConverterPrivate() noexcept {}
ExternalToInternalConverterPrivate::~ExternalToInternalConverterPrivate() {}
ExternalToInternalConverter::ExternalToInternalConverter() noexcept : d(){}
ExternalToInternalConverter::ExternalToInternalConverter(const ExternalToInternalConverter& other) noexcept : d(other.d) {}
ExternalToInternalConverter::ExternalToInternalConverter(ExternalToInternalConverter&& other) noexcept : d(std::move(other.d)) {}
ExternalToInternalConverter::ExternalToInternalConverter(ExternalToInternalConverterPrivate* _d) noexcept : d(_d) {}
ExternalToInternalConverter& ExternalToInternalConverter::operator=(ExternalToInternalConverter& other) noexcept { d = other.d; return *this; }
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
    return d->invoke(env, scope, val, out, valueType);
}

class QHashFunctionPointerData : public QHashFunctionPrivate{
public:
    inline QHashFunctionPointerData(QHashFunction::FunctionPointer functionPointer) noexcept
     : m_functionPointer(functionPointer){Q_ASSERT(functionPointer);}
    inline hash_type invoke(const void* ptr, hash_type seed) const override
     { return m_functionPointer(ptr, seed); }
private:
    QHashFunction::FunctionPointer m_functionPointer;
};
QHashFunction::QHashFunction(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new QHashFunctionPointerData(functor)){}

QHashFunctionPrivate::QHashFunctionPrivate() noexcept {}
QHashFunctionPrivate::~QHashFunctionPrivate() {}
QHashFunction::QHashFunction() noexcept : d(){}
QHashFunction::QHashFunction(const QHashFunction& other) noexcept : d(other.d) {}
QHashFunction::QHashFunction(QHashFunction&& other) noexcept : d(std::move(other.d)) {}
QHashFunction::QHashFunction(QHashFunctionPrivate* _d) noexcept : d(_d) {}
QHashFunction& QHashFunction::operator=(QHashFunction& other) noexcept { d = other.d; return *this; }
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
    return d->invoke(ptr, seed);
}

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
