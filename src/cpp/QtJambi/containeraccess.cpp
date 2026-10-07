
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include "pch_p.h"
#include <QtCore/QtGlobal>
#include <QtCore/private/qfactoryloader_p.h>

#include "containeraccess_p.h"
#include "containeraccess_associative.h"
#include "containeraccess_export_objectlist.h"
#include "qtjambi_cast_template2.h"
#include "qtjambi_cast_container.h"

QPair<void*,AbstractContainerAccess*> ContainerAPI::fromNativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        QtJambiLink *lnk = reinterpret_cast<QtJambiLink *>(nativeId);
        return {lnk->pointer(), lnk->containerAccess()};
    }else{
        return {nullptr,nullptr};
    }
}

QPair<void*,AbstractContainerAccess*> ContainerAPI::fromJavaOwner(JNIEnv *env, jobject object){
    if(Java::QtJambi::QtObject::isInstanceOf(env, object)){
        if(QSharedPointer<QtJambiLink> lnk = QtJambiLink::findLinkForJavaObject(env, object)){
            if(lnk->containerAccess()){
                return {lnk->pointer(), lnk->containerAccess()};
            }
        }
    }
    return {nullptr,nullptr};
}

AbstractContainerAccess::DataType dataType(const QMetaType& metaType, const QSharedPointer<AbstractContainerAccess>& access){
    if(access){
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        if(access->isSpan())
            return AbstractContainerAccess::Pointer;
#endif
        if(metaType.flags().testFlag(QMetaType::IsPointer))
            return AbstractContainerAccess::Pointer;
        return AbstractContainerAccess::Value;
    }else if(metaType.flags().testFlag(QMetaType::PointerToQObject)){
        return AbstractContainerAccess::PointerToQObject;
    }else{
        if(metaType.flags().testFlag(QMetaType::IsPointer)){
            if(const std::type_info* typeId = getTypeByMetaType(metaType)){
                if(registeredFunctionalResolver(*typeId)){
                    return AbstractContainerAccess::FunctionPointer;
                }
            }else if(QByteArrayView(metaType.name()).contains("(*)")){
                return AbstractContainerAccess::FunctionPointer;
            }
            return AbstractContainerAccess::Pointer;
        }else{
            return AbstractContainerAccess::Value;
        }
    }
}

AbstractContainerAccess::DataType AbstractContainerAccess::dataType(const QMetaType& metaType, const QSharedPointer<AbstractContainerAccess>& access){
    return ::dataType(metaType, access);
}

class OwnerFunctionalPrivate : public QSharedData{
protected:
    OwnerFunctionalPrivate() noexcept;
public:
    virtual ~OwnerFunctionalPrivate();
    virtual const QObject* invoke(const void *) const = 0;
    friend class OwnerFunctional;
};

class OwnerFunctional{
public:
    typedef const QObject*(*FunctionPointer)(const void *);

private:
    explicit OwnerFunctional(OwnerFunctionalPrivate* _d) noexcept;
    template<typename Functor, bool = std::is_assignable<FunctionPointer&, Functor>::value, bool = std::is_same_v<Functor, OwnerFunctional>>
    class Data : public OwnerFunctionalPrivate{
    public:
        inline static OwnerFunctionalPrivate* from(Functor&& functor){
            return new Data(std::forward<Functor>(functor));
        }
        inline const QObject* invoke(const void *container) const override {
            return m_functor(container);
        }
    private:
        inline Data(Functor&& functor) noexcept : m_functor(std::forward<Functor>(functor)){}
        Functor m_functor;
    };
    template<typename Functor>
    struct Data<Functor,false,true>{
        inline static const OwnerFunctional& from(const OwnerFunctional& function){
            return function;
        }
        inline static OwnerFunctional&& from(OwnerFunctional&& function){
            return std::move(function);
        }
    };
    template<typename Functor>
    struct Data<Functor,true,false>{
        inline static FunctionPointer from(Functor&& functor){
            return FunctionPointer(functor);
        }
    };
public:
    OwnerFunctional() noexcept;
    OwnerFunctional(const OwnerFunctional& other) noexcept;
    OwnerFunctional(OwnerFunctional&& other) noexcept;
    OwnerFunctional(FunctionPointer functor) noexcept;

    OwnerFunctional& operator=(const OwnerFunctional& other) noexcept;
    OwnerFunctional& operator=(OwnerFunctional& other) noexcept;
    OwnerFunctional& operator=(OwnerFunctional&& other) noexcept;

    template<typename Functor, typename = std::enable_if_t<std::is_invocable_r_v<const QObject*, Functor, const void *>>>
    OwnerFunctional(Functor&& functor) noexcept
        : OwnerFunctional(Data<std::remove_cv_t<std::remove_reference_t<Functor>>>::from(std::forward<Functor>(functor))){}

    bool operator==(const OwnerFunctional& other) const noexcept;
    const QObject* operator()(const void *container) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    template<typename, bool, bool> friend class Data;
    QExplicitlySharedDataPointer<OwnerFunctionalPrivate> d;
};

class OwnerFunctionalPointerData : public OwnerFunctionalPrivate{
public:
    inline OwnerFunctionalPointerData(OwnerFunctional::FunctionPointer functionPointer) noexcept
     : m_functionPointer(functionPointer){Q_ASSERT(functionPointer);}
    inline const QObject* invoke(const void *container) const override
     { return m_functionPointer(container); }
private:
    OwnerFunctional::FunctionPointer m_functionPointer;
};
OwnerFunctional::OwnerFunctional(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new OwnerFunctionalPointerData(functor)){}

OwnerFunctionalPrivate::OwnerFunctionalPrivate() noexcept {}
OwnerFunctionalPrivate::~OwnerFunctionalPrivate() {}
OwnerFunctional::OwnerFunctional() noexcept : d(){}
OwnerFunctional::OwnerFunctional(const OwnerFunctional& other) noexcept : d(other.d) {}
OwnerFunctional::OwnerFunctional(OwnerFunctional&& other) noexcept : d(std::move(other.d)) {}
OwnerFunctional::OwnerFunctional(OwnerFunctionalPrivate* _d) noexcept : d(_d) {}
OwnerFunctional& OwnerFunctional::operator=(OwnerFunctional& other) noexcept { d = other.d; return *this; }
OwnerFunctional& OwnerFunctional::operator=(const OwnerFunctional& other) noexcept { d = other.d; return *this; }
OwnerFunctional& OwnerFunctional::operator=(OwnerFunctional&& other) noexcept { d = std::move(other.d); return *this; }
bool OwnerFunctional::operator==(const OwnerFunctional& other) const noexcept { return d == other.d; }

OwnerFunctional::operator bool() const noexcept{
    return d;
}

bool OwnerFunctional::operator !() const noexcept{
    return !d;
}

const QObject* OwnerFunctional::operator()(const void *container) const{
    return d->invoke(container);
}

bool AbstractContainerAccess::isPointerType(const QMetaType& metaType){
    return metaType.flags().testFlag(QMetaType::IsPointer);
}

std::function<AbstractContainerAccess*()> getContainerAccessFactory(SequentialContainerType containerType, const QMetaType& type);
std::function<AbstractContainerAccess*()> getContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType);
void registerContainerAccessFactory(SequentialContainerType containerType, const QMetaType& elementType, std::function<AbstractContainerAccess*()>&& factory);
void registerContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType, std::function<AbstractContainerAccess*()>&& factory);

size_t pointerHashFunction(const void* ptr, size_t seed){ return !ptr ? 0 : ::qHash(*reinterpret_cast<QHashDummyValue*const*>(ptr), seed);}

AbstractSetAccess* createSetAccess(const QMetaType& memberMetaType);
AbstractListAccess* createListAccess(const QMetaType& memberMetaType);
AbstractSpanAccess* createConstSpanAccess(const QMetaType& memberMetaType);
AbstractSpanAccess* createSpanAccess(const QMetaType& memberMetaType);

AbstractContainerAccess* createContainerAccess(SequentialContainerType containerType, const QMetaType& memberMetaType){
    AbstractContainerAccess* containerAccess = nullptr;
    switch(containerType){
    case SequentialContainerType::QSet:
        containerAccess = createSetAccess(memberMetaType);
        break;
    case SequentialContainerType::QStack:
    case SequentialContainerType::QQueue:
    case SequentialContainerType::QList:
        containerAccess = createListAccess(memberMetaType);
        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    case SequentialContainerType::QConstSpan:
        containerAccess = createConstSpanAccess(memberMetaType);
        break;
    case SequentialContainerType::QSpan:
        containerAccess = createSpanAccess(memberMetaType);
        break;
    default: break;
#endif
    }
    if(!containerAccess){
        if(std::function<AbstractContainerAccess*()> containerAccessFactory = getContainerAccessFactory(containerType, memberMetaType))
            containerAccess = containerAccessFactory();
    }
    return containerAccess;
}

AbstractHashAccess* createHashAccess(const QMetaType& memberMetaType1, const QMetaType& memberMetaType2);
AbstractMapAccess* createMapAccess(const QMetaType& memberMetaType1, const QMetaType& memberMetaType2);
AbstractMultiHashAccess* createMultiHashAccess(const QMetaType& memberMetaType1, const QMetaType& memberMetaType2);
AbstractMultiMapAccess* createMultiMapAccess(const QMetaType& memberMetaType1, const QMetaType& memberMetaType2);
AbstractPairAccess* createPairAccess(const QMetaType& memberMetaType1, const QMetaType& memberMetaType2);

AbstractContainerAccess* createContainerAccess(AssociativeContainerType mapType, const QMetaType& memberMetaType1, const QMetaType& memberMetaType2){
    AbstractContainerAccess* containerAccess = nullptr;
    switch(mapType){
    case AssociativeContainerType::QHash:
        containerAccess = createHashAccess(memberMetaType1, memberMetaType2);
        break;
    case AssociativeContainerType::QMap:
        containerAccess = createMapAccess(memberMetaType1, memberMetaType2);
        break;
    case AssociativeContainerType::QMultiHash:
        containerAccess = createMultiHashAccess(memberMetaType1, memberMetaType2);
        break;
    case AssociativeContainerType::QMultiMap:
        containerAccess = createMultiMapAccess(memberMetaType1, memberMetaType2);
        break;
    case AssociativeContainerType::QPair:
        containerAccess = createPairAccess(memberMetaType1, memberMetaType2);
        break;
    default: break;
    }

    if(std::function<AbstractContainerAccess*()> containerAccessFactory = getContainerAccessFactory(mapType, memberMetaType1, memberMetaType2))
        containerAccess = containerAccessFactory();
    return containerAccess;
}

AbstractContainerAccess* createContainerAccess(JNIEnv* env, SequentialContainerType containerType,
                                                         const QMetaType& metaType,
                                                         size_t align, size_t size,
                                                         bool isPointer,
                                                         const QtJambiUtils::QHashFunction& hashFunction,
                                                         const QtJambiUtils::InternalToExternalConverter& memberConverter,
                                                         const QtJambiUtils::ExternalToInternalConverter& memberReConverter,
                                                         const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess,
                                                         PtrOwnerFunction ownerFunction){
    Q_UNUSED(isPointer);
    Q_UNUSED(size);
    Q_UNUSED(align);
    Q_ASSERT(metaType.id()!=QMetaType::UnknownType && metaType.id()!=QMetaType::Void);
    AbstractContainerAccess* containerAccess = nullptr;
    AbstractContainerAccess::DataType elementType = dataType(metaType, memberNestedContainerAccess);
    bool hasNestedPointers = false;
    if(memberNestedContainerAccess && (elementType & AbstractContainerAccess::PointersMask)==0){
        if(memberNestedContainerAccess->isSequential()){
            auto daccess = static_cast<AbstractSequentialAccess*>(memberNestedContainerAccess.data());
            hasNestedPointers = (daccess->elementType() & AbstractContainerAccess::PointersMask) || daccess->hasNestedPointers();
        }else if(memberNestedContainerAccess->isAssociative()){
            auto daccess = static_cast<AbstractAssociativeAccess*>(memberNestedContainerAccess.data());
            hasNestedPointers = (daccess->keyType() & AbstractContainerAccess::PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & AbstractContainerAccess::PointersMask) || daccess->hasValueNestedPointers();
        }else if(memberNestedContainerAccess->isPair()){
            auto daccess = static_cast<AbstractPairAccess*>(memberNestedContainerAccess.data());
            hasNestedPointers = (daccess->firstType() & AbstractContainerAccess::PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & AbstractContainerAccess::PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    switch(containerType){
    case SequentialContainerType::QSet:
        switch(elementType){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            containerAccess = new PointerRCAutoSetAccess(
                                                 metaType,
                                                 hashFunction,
                                                 memberConverter,
                                                 memberReConverter,
                                                 memberNestedContainerAccess,
                                                 ownerFunction,
                                                 elementType);
            break;
        default:{
                if(hasNestedPointers){
                    containerAccess = new NestedPointersRCAutoSetAccess(
                                                     metaType,
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                    elementType);
                }else{
                    containerAccess = new AutoSetAccess(
                                                     metaType,
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                    elementType);
                }
            }break;
        }
        break;
    case SequentialContainerType::QStack:
    case SequentialContainerType::QQueue:
    case SequentialContainerType::QList:
        switch(elementType){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            containerAccess = new PointerRCAutoListAccess(
                                                 metaType,
                                                 hashFunction,
                                                 memberConverter,
                                                 memberReConverter,
                                                 memberNestedContainerAccess,
                                                 ownerFunction,
                                                elementType);
            break;
        default:{
                if(hasNestedPointers){
                    containerAccess = new NestedPointersRCAutoListAccess(
                                                     metaType,
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                    elementType);
                }else{
                    containerAccess = new AutoListAccess(
                                                     metaType,
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                    elementType);
                }
            }break;
        }
        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    case SequentialContainerType::QConstSpan:
        containerAccess = new AutoSpanAccess(
                                         metaType,
                                         hashFunction,
                                         memberConverter,
                                         memberNestedContainerAccess,
                                         ownerFunction,
                                         elementType);
        break;
    case SequentialContainerType::QSpan:
        switch(elementType){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            containerAccess = new PointerRCAutoSpanAccess(
                                             metaType,
                                             hashFunction,
                                             memberConverter,
                                             memberReConverter,
                                             memberNestedContainerAccess,
                                             ownerFunction,
                                             elementType);
            break;
        default:{
                if(hasNestedPointers){
                    containerAccess = new NestedPointersRCAutoSpanAccess(
                                                     metaType,
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                     elementType);
                }else{
                    containerAccess = new AutoSpanAccess(
                                                     metaType,
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                     elementType);
                }
            }break;
        }
        break;
#endif
    default: break;
    }
    Q_UNUSED(env)
    if(containerAccess && metaType.isValid()){
        QSharedPointer<AbstractContainerAccess> access(containerAccess->clone(), &containerDisposer);
        registerContainerAccessFactory(containerType, metaType, [access]() -> AbstractContainerAccess* {
                                           return access->clone();
                                       });
    }
    return containerAccess;
}

AbstractContainerAccess* createContainerAccess(JNIEnv* env, AssociativeContainerType mapType,
                                                         const QMetaType& memberMetaType1,
                                                         size_t align1, size_t size1,
                                                         bool isPointer1,
                                                         const QtJambiUtils::QHashFunction& hashFunction1,
                                                         const QtJambiUtils::InternalToExternalConverter& memberConverter1,
                                                         const QtJambiUtils::ExternalToInternalConverter& memberReConverter1,
                                                         const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess1,
                                                         PtrOwnerFunction ownerFunction1,
                                                         const QMetaType& memberMetaType2,
                                                         size_t align2, size_t size2,
                                                         bool isPointer2,
                                                         const QtJambiUtils::QHashFunction& hashFunction2,
                                                         const QtJambiUtils::InternalToExternalConverter& memberConverter2,
                                                         const QtJambiUtils::ExternalToInternalConverter& memberReConverter2,
                                                         const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess2,
                                                         PtrOwnerFunction ownerFunction2){
    Q_UNUSED(size1);
    Q_UNUSED(align1);
    Q_UNUSED(size2);
    Q_UNUSED(align2);
    AbstractContainerAccess::DataType memberType1 = dataType(memberMetaType1, memberNestedContainerAccess1);
    AbstractContainerAccess::DataType memberType2 = dataType(memberMetaType2, memberNestedContainerAccess2);
    bool hasNestedPointers1 = false;
    bool hasNestedPointers2 = false;
    if(memberNestedContainerAccess1 && (memberType1 & AbstractContainerAccess::PointersMask)==0){
        if(memberNestedContainerAccess1->isSequential()){
            auto daccess = static_cast<AbstractSequentialAccess*>(memberNestedContainerAccess1.data());
            hasNestedPointers1 = (daccess->elementType() & AbstractContainerAccess::PointersMask) || daccess->hasNestedPointers();
        }else if(memberNestedContainerAccess1->isAssociative()){
            auto daccess = static_cast<AbstractAssociativeAccess*>(memberNestedContainerAccess1.data());
            hasNestedPointers1 = (daccess->keyType() & AbstractContainerAccess::PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & AbstractContainerAccess::PointersMask) || daccess->hasValueNestedPointers();
        }else if(memberNestedContainerAccess1->isPair()){
            auto daccess = static_cast<AbstractPairAccess*>(memberNestedContainerAccess1.data());
            hasNestedPointers1 = (daccess->firstType() & AbstractContainerAccess::PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & AbstractContainerAccess::PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    if(memberNestedContainerAccess2 && (memberType2 & AbstractContainerAccess::PointersMask)==0){
        if(memberNestedContainerAccess2->isSequential()){
            auto daccess = static_cast<AbstractSequentialAccess*>(memberNestedContainerAccess2.data());
            hasNestedPointers2 = (daccess->elementType() & AbstractContainerAccess::PointersMask) || daccess->hasNestedPointers();
        }else if(memberNestedContainerAccess2->isAssociative()){
            auto daccess = static_cast<AbstractAssociativeAccess*>(memberNestedContainerAccess2.data());
            hasNestedPointers2 = (daccess->keyType() & AbstractContainerAccess::PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & AbstractContainerAccess::PointersMask) || daccess->hasValueNestedPointers();
        }else if(memberNestedContainerAccess2->isPair()){
            auto daccess = static_cast<AbstractPairAccess*>(memberNestedContainerAccess2.data());
            hasNestedPointers2 = (daccess->firstType() & AbstractContainerAccess::PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & AbstractContainerAccess::PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    AbstractContainerAccess* containerAccess = nullptr;
    switch(mapType){
    case AssociativeContainerType::QPair:
        containerAccess = new AutoPairAccess(
                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                     hashFunction1,
                                     memberConverter1,
                                     memberReConverter1,
                                     memberNestedContainerAccess1,
                                     ownerFunction1,
                                     memberType1,
                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                     hashFunction2,
                                     memberConverter2,
                                     memberReConverter2,
                                     memberNestedContainerAccess2,
                                     ownerFunction2,
                                     memberType2);
        break;
    case AssociativeContainerType::QHash:
        switch(memberType1){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(memberType2){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointersRCAutoHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                break;
            default:
                if(hasNestedPointers2){
                    containerAccess = new NestedPointersRCAutoHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    containerAccess = new KeyPointerRCAutoHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }
                break;
            }
            break;
        default:{
                if(hasNestedPointers1){
                    containerAccess = new NestedPointersRCAutoHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    switch(memberType2){
                    case AbstractContainerAccess::Pointer:
                    case AbstractContainerAccess::FunctionPointer:
                    case AbstractContainerAccess::PointerToQObject:
                        containerAccess = new ValuePointerRCAutoHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        break;
                    default:
                        if(hasNestedPointers2){
                            containerAccess = new NestedPointersRCAutoHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        }else{
                            containerAccess = new AutoHashAccess(
                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                         hashFunction1,
                                                         memberConverter1,
                                                         memberReConverter1,
                                                         memberNestedContainerAccess1,
                                                         ownerFunction1,
                                                         memberType1,
                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                         hashFunction2,
                                                         memberConverter2,
                                                         memberReConverter2,
                                                         memberNestedContainerAccess2,
                                                         ownerFunction2,
                                                         memberType2);
                        }
                        break;
                    }
                }
            }
            break;
        }
        break;
    case AssociativeContainerType::QMap:
        switch(memberType1){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(memberType2){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointersRCAutoMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                break;
            default:
                if(hasNestedPointers2){
                    containerAccess = new NestedPointersRCAutoMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    containerAccess = new KeyPointerRCAutoMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }
                break;
            }
            break;
        default:{
                if(hasNestedPointers1){
                    containerAccess = new NestedPointersRCAutoMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    switch(memberType2){
                    case AbstractContainerAccess::Pointer:
                    case AbstractContainerAccess::FunctionPointer:
                    case AbstractContainerAccess::PointerToQObject:
                        containerAccess = new ValuePointerRCAutoMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        break;
                    default:
                        if(hasNestedPointers2){
                            containerAccess = new NestedPointersRCAutoMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        }else{
                            containerAccess = new AutoMapAccess(
                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                         hashFunction1,
                                                         memberConverter1,
                                                         memberReConverter1,
                                                         memberNestedContainerAccess1,
                                                         ownerFunction1,
                                                         memberType1,
                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                         hashFunction2,
                                                         memberConverter2,
                                                         memberReConverter2,
                                                         memberNestedContainerAccess2,
                                                         ownerFunction2,
                                                         memberType2);
                        }
                        break;
                    }
                }
            }
            break;
        }
        break;
    case AssociativeContainerType::QMultiHash:
    {
        AbstractMultiHashAccess* access;
        switch(memberType1){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(memberType2){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = access = new PointersRCAutoMultiHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                break;
            default:
                if(hasNestedPointers2){
                    containerAccess = access = new NestedPointersRCAutoMultiHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    containerAccess = access = new KeyPointerRCAutoMultiHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }
                break;
            }
            break;
        default:{
                if(hasNestedPointers1){
                    containerAccess = access = new NestedPointersRCAutoMultiHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    switch(memberType2){
                    case AbstractContainerAccess::Pointer:
                    case AbstractContainerAccess::FunctionPointer:
                    case AbstractContainerAccess::PointerToQObject:
                        containerAccess = access = new ValuePointerRCAutoMultiHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        break;
                    default:
                        if(hasNestedPointers2){
                            containerAccess = access = new NestedPointersRCAutoMultiHashAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        }else{
                            containerAccess = access = new AutoMultiHashAccess(
                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                         hashFunction1,
                                                         memberConverter1,
                                                         memberReConverter1,
                                                         memberNestedContainerAccess1,
                                                         ownerFunction1,
                                                         memberType1,
                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                         hashFunction2,
                                                         memberConverter2,
                                                         memberReConverter2,
                                                         memberNestedContainerAccess2,
                                                         ownerFunction2,
                                                         memberType2);
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
        break;
    case AssociativeContainerType::QMultiMap:
    {
        AbstractMultiMapAccess* access;
        switch(memberType1){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(memberType2){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = access = new PointersRCAutoMultiMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                break;
            default:
                if(hasNestedPointers2){
                    containerAccess = access = new NestedPointersRCAutoMultiMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    containerAccess = access = new KeyPointerRCAutoMultiMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }
                break;
            }
            break;
        default:{
                if(hasNestedPointers1){
                    containerAccess = access = new NestedPointersRCAutoMultiMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                }else{
                    switch(memberType2){
                    case AbstractContainerAccess::Pointer:
                    case AbstractContainerAccess::FunctionPointer:
                    case AbstractContainerAccess::PointerToQObject:
                        containerAccess = access = new ValuePointerRCAutoMultiMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        break;
                    default:
                        if(hasNestedPointers2){
                            containerAccess = access = new NestedPointersRCAutoMultiMapAccess(
                                                                     isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                                     hashFunction1,
                                                                     memberConverter1,
                                                                     memberReConverter1,
                                                                     memberNestedContainerAccess1,
                                                                     ownerFunction1,
                                                                     memberType1,
                                                                     isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                                     hashFunction2,
                                                                     memberConverter2,
                                                                     memberReConverter2,
                                                                     memberNestedContainerAccess2,
                                                                     ownerFunction2,
                                                                     memberType2);
                        }else{
                            containerAccess = access = new AutoMultiMapAccess(
                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
                                                         hashFunction1,
                                                         memberConverter1,
                                                         memberReConverter1,
                                                         memberNestedContainerAccess1,
                                                         ownerFunction1,
                                                         memberType1,
                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
                                                         hashFunction2,
                                                         memberConverter2,
                                                         memberReConverter2,
                                                         memberNestedContainerAccess2,
                                                         ownerFunction2,
                                                         memberType2);
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
//#endif
        break;
    default: break;
    }
    Q_UNUSED(env)
    return containerAccess;
}

void containerDisposer(AbstractContainerAccess* _access){
    if(_access)
        _access->dispose();
}

AbstractNestedSequentialAccess::~AbstractNestedSequentialAccess(){}
AbstractNestedAssociativeAccess::~AbstractNestedAssociativeAccess(){}
AbstractNestedPairAccess::~AbstractNestedPairAccess(){}

AbstractContainerAccess::AbstractContainerAccess(){}
AbstractContainerAccess::~AbstractContainerAccess(){}
AbstractReferenceCountingContainer* AbstractContainerAccess::asRC() {return nullptr;}
void AbstractContainerAccess::dispose(){}
void* AbstractContainerAccess::createContainer(void* moved){
    size_t sz = sizeOf();
    size_t al = alignOf();
    if(sz>0){
        void* placement;
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__){
            placement = operator new(sz, std::align_val_t(al));
        }else{
            placement = operator new(sz);
        }
        void* result = constructContainer(placement, moved);
        if(!result){
            if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz, std::align_val_t(al));
#else
                operator delete(result, std::align_val_t(al));
#endif
            } else {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz);
#else
                operator delete(result);
#endif
            }
        }
        return result;
    }else{
        return nullptr;
    }
}
void* AbstractContainerAccess::createContainer(JNIEnv *env, const ContainerAndAccessInfo& moved){
    size_t sz = sizeOf();
    size_t al = alignOf();
    if(sz>0){
        void* placement;
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__){
            placement = operator new(sz, std::align_val_t(al));
        }else{
            placement = operator new(sz);
        }
        void* result = constructContainer(env, placement, moved);
        if(!result){
            if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz, std::align_val_t(al));
#else
                operator delete(result, std::align_val_t(al));
#endif
            } else {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz);
#else
                operator delete(result);
#endif
            }
        }
        return result;
    }else{
        return nullptr;
    }
}
void* AbstractContainerAccess::createContainer(){
    size_t sz = sizeOf();
    size_t al = alignOf();
    if(sz>0){
        void* placement;
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__){
            placement = operator new(sz, std::align_val_t(al));
        }else{
            placement = operator new(sz);
        }
        void* result = constructContainer(placement);
        if(!result){
            if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz, std::align_val_t(al));
#else
                operator delete(result, std::align_val_t(al));
#endif
            } else {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz);
#else
                operator delete(result);
#endif
            }
        }
        return result;
    }else{
        return nullptr;
    }
}
void* AbstractContainerAccess::createContainer(const void* copy){
    size_t sz = sizeOf();
    size_t al = alignOf();
    if(sz>0){
        void* placement;
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__){
            placement = operator new(sz, std::align_val_t(al));
        }else{
            placement = operator new(sz);
        }
        void* result = constructContainer(placement, copy);
        if(!result){
            if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz, std::align_val_t(al));
#else
                operator delete(result, std::align_val_t(al));
#endif
            } else {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz);
#else
                operator delete(result);
#endif
            }
        }
        return result;
    }else{
        return nullptr;
    }
}
void* AbstractContainerAccess::createContainer(JNIEnv *env, const ConstContainerAndAccessInfo& copy){
    size_t sz = sizeOf();
    size_t al = alignOf();
    if(sz>0){
        void* placement;
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__){
            placement = operator new(sz, std::align_val_t(al));
        }else{
            placement = operator new(sz);
        }
        void* result = constructContainer(env, placement, copy);
        if(!result){
            if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz, std::align_val_t(al));
#else
                operator delete(result, std::align_val_t(al));
#endif
            } else {
#ifdef __cpp_sized_deallocation
                operator delete(result, sz);
#else
                operator delete(result);
#endif
            }
        }
        return result;
    }else{
        return nullptr;
    }
}
void AbstractContainerAccess::deleteContainer(void* container){
    if(destructContainer(container)){
#ifdef __cpp_sized_deallocation
        size_t sz = sizeOf();
#endif
        size_t al = alignOf();
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
            operator delete(container, sz, std::align_val_t(al));
#else
            operator delete(container, std::align_val_t(al));
#endif
        } else {
#ifdef __cpp_sized_deallocation
            operator delete(container, sz);
#else
            operator delete(container);
#endif
        }
    }
}

const QObject* AbstractContainerAccess::getOwner(const void*){ return nullptr; }
bool AbstractContainerAccess::hasOwnerFunction(){ return false; }
void AbstractSequentialConstIteratorAccess::assign(void*, const void*) {}
void AbstractSequentialConstIteratorAccess::assign(JNIEnv*, const ContainerInfo& c, const ConstContainerAndAccessInfo& o) {assign(c.container,o.container);}
void* AbstractSequentialConstIteratorAccess::constructContainer(void*) {return nullptr;}
void* AbstractSequentialConstIteratorAccess::constructContainer(void*,const void*) {return nullptr;}
void* AbstractSequentialConstIteratorAccess::constructContainer(JNIEnv *, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}
void* AbstractSequentialConstIteratorAccess::constructContainer(void*,void*) {return nullptr;}
void* AbstractSequentialConstIteratorAccess::constructContainer(JNIEnv *, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}
bool AbstractSequentialConstIteratorAccess::destructContainer(void*) {return false;}
QMetaType AbstractSequentialConstIteratorAccess::registerContainer(QByteArrayView) {return QMetaType(QMetaType::UnknownType);}

AbstractAssociativeConstIteratorAccess::~AbstractAssociativeConstIteratorAccess(){}
AbstractAssociativeConstIteratorAccess::AbstractAssociativeConstIteratorAccess(){}
AbstractContainerAccess::ContainerType AbstractAssociativeConstIteratorAccess::containerType() const { return ContainerType::AssociativeConstIterator; }
AbstractSequentialConstIteratorAccess::IteratorType AbstractAssociativeConstIteratorAccess::iteratorType() const { return IteratorType::const_iterator; }
AbstractSequentialConstIteratorAccess::IteratorStorage AbstractAssociativeConstIteratorAccess::iteratorStorage() const { return IteratorStorage::Clone; }

AbstractSequentialIteratorAccess::~AbstractSequentialIteratorAccess(){}
AbstractSequentialIteratorAccess::AbstractSequentialIteratorAccess(){}
AbstractContainerAccess::ContainerType AbstractSequentialIteratorAccess::containerType() const { return ContainerType::SequentialIterator; }
AbstractSequentialConstIteratorAccess::IteratorType AbstractSequentialIteratorAccess::iteratorType() const { return IteratorType::iterator; }
AbstractSequentialConstIteratorAccess::IteratorStorage AbstractSequentialIteratorAccess::iteratorStorage() const { return IteratorStorage::Ref; }

AbstractAssociativeIteratorAccess::~AbstractAssociativeIteratorAccess(){}
AbstractAssociativeIteratorAccess::AbstractAssociativeIteratorAccess(){}
AbstractContainerAccess::ContainerType AbstractAssociativeIteratorAccess::containerType() const { return ContainerType::AssociativeIterator; }
AbstractSequentialConstIteratorAccess::IteratorType AbstractAssociativeIteratorAccess::iteratorType() const { return IteratorType::iterator; }
AbstractSequentialConstIteratorAccess::IteratorStorage AbstractAssociativeIteratorAccess::iteratorStorage() const { return IteratorStorage::Ref; }

AbstractSequentialAccess::~AbstractSequentialAccess(){}
AbstractSequentialAccess::AbstractSequentialAccess(){}
AbstractNestedSequentialAccess* AbstractSequentialAccess::asNested() { return nullptr; }
AbstractContainerAccess::ContainerType AbstractSequentialAccess::containerType() const { return ContainerType::Sequential; }
AbstractSequentialAccess::ElementIterator::~ElementIterator(){}

const QMetaType& AbstractSequentialAccess::ElementIterator::elementMetaType() { return access()->elementMetaType(); }
AbstractContainerAccess::DataType AbstractSequentialAccess::ElementIterator::elementType() { return access()->elementType(); }
AbstractContainerAccess* AbstractSequentialAccess::ElementIterator::elementNestedContainerAccess() { return access()->elementNestedContainerAccess(); }
bool AbstractSequentialAccess::ElementIterator::hasNestedContainerAccess() { return access()->hasNestedContainerAccess(); }
bool AbstractSequentialAccess::ElementIterator::hasNestedPointers() { return access()->hasNestedPointers(); }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
AbstractSpanAccess::~AbstractSpanAccess(){}
AbstractSpanAccess::AbstractSpanAccess(){}

bool AbstractSpanAccess::isDetached(const void*){
    return false;
}
AbstractContainerAccess::ContainerType AbstractSpanAccess::containerType() const { return ContainerType::Span; }
void AbstractSpanAccess::detach(const ContainerInfo&){}
bool AbstractSpanAccess::isSharedWith(const void*, const void*){return false;}
void AbstractSpanAccess::swap(JNIEnv * env, const ContainerInfo&, const ContainerAndAccessInfo&){
    JavaException::raiseUnsupportedOperationException(env, "QSpan::swap" QTJAMBI_STACKTRACEINFO );
}
void AbstractSpanAccess::clear(JNIEnv * env, const ContainerInfo&){
    JavaException::raiseUnsupportedOperationException(env, "QSpan::clear" QTJAMBI_STACKTRACEINFO );
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

AbstractListAccess::~AbstractListAccess(){}
AbstractListAccess::AbstractListAccess(){}
AbstractContainerAccess::ContainerType AbstractListAccess::containerType() const { return ContainerType::List; }

AbstractSetAccess::~AbstractSetAccess(){}
AbstractSetAccess::AbstractSetAccess(){}
AbstractContainerAccess::ContainerType AbstractSetAccess::containerType() const { return ContainerType::Set; }

AbstractAssociativeAccess::~AbstractAssociativeAccess(){}
AbstractAssociativeAccess::AbstractAssociativeAccess(){}
AbstractNestedAssociativeAccess* AbstractAssociativeAccess::asNested() { return nullptr; }
AbstractContainerAccess::ContainerType AbstractAssociativeAccess::containerType() const { return ContainerType::Associative; }
AbstractAssociativeAccess::KeyValueIterator::~KeyValueIterator(){}
const QMetaType& AbstractAssociativeAccess::KeyValueIterator::keyMetaType() { return access()->keyMetaType(); }
const QMetaType& AbstractAssociativeAccess::KeyValueIterator::valueMetaType() { return access()->valueMetaType(); }
AbstractContainerAccess::DataType AbstractAssociativeAccess::KeyValueIterator::keyType() { return access()->keyType(); }
AbstractContainerAccess::DataType AbstractAssociativeAccess::KeyValueIterator::valueType() { return access()->valueType(); }
AbstractContainerAccess* AbstractAssociativeAccess::KeyValueIterator::keyNestedContainerAccess() { return access()->keyNestedContainerAccess(); }
AbstractContainerAccess* AbstractAssociativeAccess::KeyValueIterator::valueNestedContainerAccess() { return access()->valueNestedContainerAccess(); }
bool AbstractAssociativeAccess::KeyValueIterator::hasKeyNestedContainerAccess() { return access()->hasKeyNestedContainerAccess(); }
bool AbstractAssociativeAccess::KeyValueIterator::hasValueNestedContainerAccess() { return access()->hasValueNestedContainerAccess(); }
bool AbstractAssociativeAccess::KeyValueIterator::hasKeyNestedPointers() { return access()->hasKeyNestedPointers(); }
bool AbstractAssociativeAccess::KeyValueIterator::hasValueNestedPointers() { return access()->hasValueNestedPointers(); }

std::unique_ptr<AbstractSequentialAccess::ElementIterator> AbstractAssociativeAccess::KeyValueIterator::nextAsIterator(){
    if(hasNext()){
        class ElementIterator : public AbstractSequentialAccess::ElementIterator{
            QMetaType m_keyMetaType;
            QMetaType m_valueMetaType;
            DataType m_keyType;
            DataType m_valueType;
            AbstractContainerAccess* m_keyNestedContainerAccess;
            AbstractContainerAccess* m_valueNestedContainerAccess;
            bool m_hasKeyNestedPointers;
            bool m_hasValueNestedPointers;
            QPair<const void*,const void*> pair;
            uint index = 0;
            std::function<jobject(JNIEnv*,const void*)> m_keyConverter;
            std::function<jobject(JNIEnv*,const void*)> m_valueConverter;
            ElementIterator(const ElementIterator& other) :
                m_keyMetaType(other.m_keyMetaType),
                m_valueMetaType(other.m_valueMetaType),
                m_keyType(other.m_keyType),
                m_valueType(other.m_valueType),
                m_keyNestedContainerAccess(other.m_keyNestedContainerAccess),
                m_valueNestedContainerAccess(other.m_valueNestedContainerAccess),
                m_hasKeyNestedPointers(other.m_hasKeyNestedPointers),
                m_hasValueNestedPointers(other.m_hasValueNestedPointers),
                pair(other.pair), index(other.index),
                m_keyConverter(other.m_keyConverter),
                m_valueConverter(other.m_valueConverter) {}
        public:
            ElementIterator(AbstractAssociativeAccess::KeyValueIterator* _iter) :
                m_keyMetaType(_iter->keyMetaType()),
                m_valueMetaType(_iter->valueMetaType()),
                m_keyType(_iter->keyType()),
                m_valueType(_iter->valueType()),
                m_keyNestedContainerAccess(_iter->keyNestedContainerAccess()),
                m_valueNestedContainerAccess(_iter->valueNestedContainerAccess()),
                m_hasKeyNestedPointers(_iter->hasKeyNestedPointers()),
                m_hasValueNestedPointers(_iter->hasValueNestedPointers()),
                pair(_iter->constNext()),
                m_keyConverter(_iter->valueConverter()),
                m_valueConverter(_iter->keyConverter()) {}
        protected:
            AbstractSequentialAccess* access() override { return nullptr; }
        public:
            const QMetaType& elementMetaType() override {
                switch(index){
                case 0:
                    return m_keyMetaType;
                default:
                    return m_valueMetaType;
                }
            }
            DataType elementType() override {
                switch(index){
                case 0:
                    return m_keyType;
                default:
                    return m_valueType;
                }
            }
            AbstractContainerAccess* elementNestedContainerAccess() override {
                switch(index){
                case 0:
                    return m_keyNestedContainerAccess;
                case 1:
                    return m_valueNestedContainerAccess;
                default:
                    return nullptr;
                }
            }
            bool hasNestedContainerAccess() override {
                return elementNestedContainerAccess();
            }
            bool hasNestedPointers() override {
                switch(index){
                case 0:
                    return m_hasKeyNestedPointers;
                default:
                    return m_hasValueNestedPointers;
                }
            }
            bool hasNext() override{
                return index<2;
            }
            bool isConst() override{
                return true;
            }
            const void* constNext() override {
                switch(index){
                case 0:
                    ++index;
                    return pair.first;
                case 1:
                    ++index;
                    return pair.second;
                default:
                    return nullptr;
                }
            }
            void* mutableNext() override {
                return nullptr;
            }
            jobject next(JNIEnv *) override{
                return nullptr;
            }
            const void* next() override {
                switch(index){
                case 0:
                    if(elementType() & AbstractContainerAccess::PointersMask){
                        ++index;
                        return *reinterpret_cast<void*const*>(pair.first);
                    }else{
                        ++index;
                        return pair.first;
                    }
                    return pair.first;
                case 1:
                    if(elementType() & AbstractContainerAccess::PointersMask){
                        ++index;
                        return *reinterpret_cast<void*const*>(pair.second);
                    }else{
                        ++index;
                        return pair.second;
                    }
                default:
                    return nullptr;
                }
            }
            bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
                return pair==reinterpret_cast<const ElementIterator&>(other).pair && index==reinterpret_cast<const ElementIterator&>(other).index;
            }
            std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
                return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
            }
            std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
                switch(index){
                case 0:
                    return m_keyConverter;
                default:
                    return m_valueConverter;
                }
            }
        };
        return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(this));
    }
    return {};
}

std::unique_ptr<AbstractSequentialAccess::ElementIterator> AbstractAssociativeAccess::asKeyIterator(std::unique_ptr<KeyValueIterator>&& iter){
    class ElementIterator : public AbstractSequentialAccess::ElementIterator{
        std::unique_ptr<KeyValueIterator> iter;
        ElementIterator(const ElementIterator& other)
            :iter(other.iter->clone()) {}
    public:
        ElementIterator(std::unique_ptr<KeyValueIterator>&& _iter) : iter(std::move(_iter)) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        const QMetaType& elementMetaType() override { return iter->keyMetaType(); }
        DataType elementType() override { return iter->keyType(); }
        AbstractContainerAccess* elementNestedContainerAccess() override { return iter->keyNestedContainerAccess(); }
        bool hasNestedContainerAccess() override { return iter->hasKeyNestedContainerAccess(); }
        bool hasNestedPointers() override { return iter->hasKeyNestedPointers(); }
        bool hasNext() override{
            return iter->hasNext();
        }
        bool isConst() override{
            return true;
        }
        const void* constNext() override {
            return iter->constNext().first;
        }
        void* mutableNext() override {
            return nullptr;
        }
        jobject next(JNIEnv * env) override{
            return iter->next(env).first;
        }
        const void* next() override {
            return iter->next().first;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return iter==reinterpret_cast<const ElementIterator&>(other).iter;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            return iter->keyConverter();
        }
    };
    return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(std::move(iter)));
}

std::unique_ptr<AbstractSequentialAccess::ElementIterator> AbstractAssociativeAccess::asValueIterator(std::unique_ptr<KeyValueIterator>&& iter){
    class ElementIterator : public AbstractSequentialAccess::ElementIterator{
        std::unique_ptr<KeyValueIterator> iter;
        ElementIterator(const ElementIterator& other)
            :iter(other.iter->clone()) {}
    public:
        ElementIterator(std::unique_ptr<KeyValueIterator>&& _iter) : iter(std::move(_iter)) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        const QMetaType& elementMetaType() override { return iter->valueMetaType(); }
        DataType elementType() override { return iter->valueType(); }
        AbstractContainerAccess* elementNestedContainerAccess() override { return iter->valueNestedContainerAccess(); }
        bool hasNestedContainerAccess() override { return iter->hasValueNestedContainerAccess(); }
        bool hasNestedPointers() override { return iter->hasValueNestedPointers(); }
        bool hasNext() override{
            return iter->hasNext();
        }
        bool isConst() override{
            return iter->isConst();
        }
        const void* constNext() override {
            return iter->constNext().second;
        }
        void* mutableNext() override {
            return iter->mutableNext().second;
        }
        jobject next(JNIEnv * env) override{
            return iter->next(env).second;
        }
        const void* next() override {
            return iter->next().second;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return iter==reinterpret_cast<const ElementIterator&>(other).iter;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            return iter->valueConverter();
        }
    };
    return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(std::move(iter)));
}

AbstractMapAccess::~AbstractMapAccess(){}
AbstractMapAccess::AbstractMapAccess(){}
AbstractContainerAccess::ContainerType AbstractMapAccess::containerType() const { return ContainerType::Map; }

AbstractMultiMapAccess::~AbstractMultiMapAccess(){}
AbstractMultiMapAccess::AbstractMultiMapAccess(){}
AbstractContainerAccess::ContainerType AbstractMultiMapAccess::containerType() const { return ContainerType::MultiMap; }

AbstractHashAccess::~AbstractHashAccess(){}
AbstractHashAccess::AbstractHashAccess(){}
AbstractContainerAccess::ContainerType AbstractHashAccess::containerType() const { return ContainerType::Hash; }

AbstractMultiHashAccess::~AbstractMultiHashAccess(){}
AbstractMultiHashAccess::AbstractMultiHashAccess(){}
AbstractContainerAccess::ContainerType AbstractMultiHashAccess::containerType() const { return ContainerType::MultiHash; }

AbstractPairAccess::~AbstractPairAccess(){}
AbstractPairAccess::AbstractPairAccess(){}
AbstractNestedPairAccess* AbstractPairAccess::asNested() { return nullptr; }
AbstractContainerAccess::ContainerType AbstractPairAccess::containerType() const { return ContainerType::Pair; }

AbstractReferenceCountingContainer::~AbstractReferenceCountingContainer(){}

ReferenceCountingSetContainer* AbstractReferenceCountingContainer::asRCSet() { return nullptr; }
ReferenceCountingMapContainer* AbstractReferenceCountingContainer::asRCMap() { return nullptr; }
ReferenceCountingMultiMapContainer* AbstractReferenceCountingContainer::asRCMultiMap() { return nullptr; }


bool hasReferenceCounts(JNIEnv * env, jobject container){
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        QtJambiStorage* storage = getQtJambiStorage();
        jobject rc;
        {
            QReadLocker lock(storage->lock());
            rc = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
        if(Java::Runtime::Collection::isInstanceOf(env, rc))
            return QtJambiAPI::sizeOfJavaCollection(env, rc)>0;
        if(Java::Runtime::Map::isInstanceOf(env, rc))
            return Java::Runtime::Map::size(env, rc)>0;
    }
    return false;
}

void AbstractReferenceCountingContainer::unfoldAndAddContainer(JNIEnv * env, jobject set, const void* data, AbstractContainerAccess::DataType dataType, const QMetaType& metaType, AbstractContainerAccess* access){
    switch(dataType){
    case AbstractContainerAccess::PointerToQObject:
        if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(data)))
            QtJambiAPI::addToJavaCollection(env, set, obj);
        break;
    case AbstractContainerAccess::FunctionPointer:
        if(const std::type_info* typeId = getTypeByMetaType(metaType)){
            if(jobject obj = QtJambiPrivate::findFunctionPointerObject(env, data, *typeId)){
                QtJambiAPI::addToJavaCollection(env, set, obj);
            }
            break;
        }
        Q_FALLTHROUGH();
    case AbstractContainerAccess::Pointer:
        if(jobject obj = QtJambiAPI::findObject(env, data))
            QtJambiAPI::addToJavaCollection(env, set, obj);
        break;
    default:
        if(access){
            if(access->isPair()){
                auto _access = static_cast<AbstractPairAccess*>(access);
                auto elements = _access->elements(data);
                auto firstAccess = _access->firstNestedContainerAccess();
                auto secondAccess = _access->secondNestedContainerAccess();
                unfoldAndAddContainer(env, set, elements.first, _access->firstType(), _access->firstMetaType(), firstAccess);
                unfoldAndAddContainer(env, set, elements.second, _access->secondType(), _access->secondMetaType(), secondAccess);
                if(firstAccess)
                    firstAccess->dispose();
                if(secondAccess)
                    secondAccess->dispose();
            }else if(access->isSequential()){
                auto _access = static_cast<AbstractSequentialAccess*>(access);
                auto elementAccess = _access->elementNestedContainerAccess();
                auto iterator = _access->constElementIterator(data);
                while(iterator->hasNext()){
                    auto content = iterator->next();
                    unfoldAndAddContainer(env, set, content, _access->elementType(), _access->elementMetaType(), elementAccess);
                }
                if(elementAccess)
                    elementAccess->dispose();
            }else if(access->isAssociative()){
                auto _access = static_cast<AbstractAssociativeAccess*>(access);
                auto keyAccess = _access->keyNestedContainerAccess();
                auto valueAccess = _access->valueNestedContainerAccess();
                auto iterator = _access->constKeyValueIterator(data);
                while(iterator->hasNext()){
                    auto content = iterator->next();
                    unfoldAndAddContainer(env, set, content.first, _access->keyType(), _access->keyMetaType(), keyAccess);
                    unfoldAndAddContainer(env, set, content.second, _access->valueType(), _access->valueMetaType(), valueAccess);
                }
                if(keyAccess)
                    keyAccess->dispose();
                if(valueAccess)
                    valueAccess->dispose();
            }
        }
        break;
    }
}

void AbstractReferenceCountingContainer::unfoldAndAddContainer(JNIEnv * env, jobject set, jobject value){
    if(set){
        if(Java::QtCore::AbstractContainer::isInstanceOf(env, value)){
            jobject rc{nullptr};
            QtJambiStorage* storage = getQtJambiStorage();
            {
                QReadLocker lock(storage->lock());
                rc = Java::QtCore::AbstractContainer::__rcContainer(env, value);
            }
            if(rc)
                unfoldAndAddContainer(env, set, rc);
        }else if(Java::Runtime::Collection::isInstanceOf(env, value)){
            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, value);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                unfoldAndAddContainer(env, set, QtJambiAPI::nextOfJavaIterator(env, iter));
            }
        }else if(Java::Runtime::Map::isInstanceOf(env, value)){
            jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, value);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                unfoldAndAddContainer(env, set, Java::Runtime::Map$Entry::getKey(env, entry));
                unfoldAndAddContainer(env, set, Java::Runtime::Map$Entry::getValue(env, entry));
            }
        }else if(Java::QtCore::QPair::isInstanceOf(env, value)){
            unfoldAndAddContainer(env, set, Java::QtCore::QPair::first(env, value));
            unfoldAndAddContainer(env, set, Java::QtCore::QPair::second(env, value));
        }else if(value){
            QtJambiAPI::addToJavaCollection(env, set, value);
        }
    }
}

void ReferenceCountingSetContainer::addNestedValueRC(JNIEnv * env, jobject container, AbstractContainerAccess::DataType dataType, bool isContainer, jobject value){
    switch(dataType){
    case AbstractContainerAccess::Value:
        if(isContainer){
            if(Java::QtCore::AbstractContainer::isInstanceOf(env, value)){
                jobject otherRC{nullptr};
                jobject set{nullptr};
                QtJambiStorage* storage = getQtJambiStorage();
                {
                    QReadLocker lock(storage->lock());
                    otherRC = Java::QtCore::AbstractContainer::__rcContainer(env, value);
                    set = Java::QtCore::AbstractContainer::__rcContainer(env, container);
                }
                if(otherRC){
                    if(!set)
                        set = rcContainer(env, container);
                    if(set){
                        if(Java::Runtime::Collection::isInstanceOf(env, otherRC)){
                            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, otherRC);
                            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                                QtJambiAPI::addToJavaCollection(env, set, QtJambiAPI::nextOfJavaIterator(env, iter));
                            }
                        }else if(Java::Runtime::Map::isInstanceOf(env, otherRC)){
                            jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, otherRC);
                            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                                QtJambiAPI::addToJavaCollection(env, set, Java::Runtime::Map$Entry::getKey(env, entry));
                                QtJambiAPI::addToJavaCollection(env, set, Java::Runtime::Map$Entry::getValue(env, entry));
                            }
                        }
                    }
                }
                break;
            }else if(Java::Runtime::Collection::isInstanceOf(env, value)){
                if(jobject set = rcContainer(env, container)){
                    jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, value);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                        unfoldAndAddContainer(env, set, QtJambiAPI::nextOfJavaIterator(env, iter));
                    }
                }
                break;
            }else if(Java::Runtime::Map::isInstanceOf(env, value)){
                if(jobject set = rcContainer(env, container)){
                    jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, value);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                        jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                        unfoldAndAddContainer(env, set, Java::Runtime::Map$Entry::getKey(env, entry));
                        unfoldAndAddContainer(env, set, Java::Runtime::Map$Entry::getValue(env, entry));
                    }
                }
                break;
            }else if(Java::QtCore::QPair::isInstanceOf(env, value)){
                if(jobject set = rcContainer(env, container)){
                    unfoldAndAddContainer(env, set, Java::QtCore::QPair::first(env, value));
                    unfoldAndAddContainer(env, set, Java::QtCore::QPair::second(env, value));
                }
                break;
            }
        }
        break;
    case AbstractContainerAccess::PointerToQObject:
    case AbstractContainerAccess::FunctionPointer:
    case AbstractContainerAccess::Pointer:
        if(jobject set = rcContainer(env, container))
            QtJambiAPI::addToJavaCollection(env, set, value);
        break;
    default:
        break;
    }
}

void AbstractReferenceCountingContainer::swapRC(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    if(container.object && container2.object){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            JniLocalFrame frame(env, 200);
            Q_ASSERT(container.object);
            Q_ASSERT(container2.object);
            QWriteLocker lock(storage->lock());
            jobject tmp = Java::QtCore::AbstractContainer::__rcContainer(env, container.object);
            jobject tmp2 = Java::QtCore::AbstractContainer::__rcContainer(env, container2.object);
            Java::QtCore::AbstractContainer::set___rcContainer(env, container.object, tmp2);
            Java::QtCore::AbstractContainer::set___rcContainer(env, container2.object, tmp);
        }
    }
}

jobject AbstractReferenceCountingContainer::findContainer(JNIEnv * env, jobject container){
    jobject result{nullptr};
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker lock(storage->lock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
    }
    return result;
}

jobject ReferenceCountingSetContainer::rcContainer(JNIEnv * env, jobject container){
    jobject result{nullptr};
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker lock(storage->lock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
        if(!result){
            jobject newInstance = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
            QWriteLocker lock(storage->lock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
            if(!result){
                result = newInstance;
                Java::QtCore::AbstractContainer::set___rcContainer(env, container, result);
            }
        }
    }
    return result;
}

jobject ReferenceCountingMapContainer::rcContainer(JNIEnv * env, jobject container){
    jobject result{nullptr};
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker lock(storage->lock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
        if(!result){
            jobject newInstance = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
            QWriteLocker lock(storage->lock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
            if(!result){
                result = newInstance;
                Java::QtCore::AbstractContainer::set___rcContainer(env, container, result);
            }
        }
    }
    return result;
}

jobject ReferenceCountingMultiMapContainer::newRCMultiMap(JNIEnv * env){
    return Java::QtJambi::ReferenceUtility$RCMultiMap::newInstance(env);
}

jobject ReferenceCountingMultiMapContainer::rcContainer(JNIEnv * env, jobject container){
    jobject result{nullptr};
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker lock(storage->lock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
        if(!result){
            jobject newInstance = Java::QtJambi::ReferenceUtility$RCMultiMap::newInstance(env);
            QWriteLocker lock(storage->lock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
            if(!result){
                result = newInstance;
                Java::QtCore::AbstractContainer::set___rcContainer(env, container, result);
            }
        }
    }
    return result;
}

void ReferenceCountingSetContainer::clearRC(JNIEnv * env, jobject container){
    if(jobject rc = findContainer(env, container)){
        QtJambiAPI::clearJavaCollection(env, rc);
    }
}

void ReferenceCountingSetContainer::assignRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        jobject tmp2 = rcContainer(env, container2);
        if(tmp){
            QtJambiAPI::clearJavaCollection(env, tmp);
            if(tmp2)
                QtJambiAPI::addAllToJavaCollection(env, tmp, tmp2);
        }
    }
}

void ReferenceCountingSetContainer::assignUniqueRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        jobject tmp2 = rcContainer(env, container2);
        if(tmp){
            QtJambiAPI::clearJavaCollection(env, tmp);
            if(tmp2){
                jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, tmp2);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject value = QtJambiAPI::nextOfJavaIterator(env, iter);
                    if(!Java::Runtime::Collection::contains(env, tmp, value))
                        QtJambiAPI::addAllToJavaCollection(env, tmp, value);
                }
            }
        }
    }
}

void ReferenceCountingSetContainer::addAllRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        if(jobject tmp = rcContainer(env, container))
            QtJambiAPI::addAllToJavaCollection(env, tmp, container2);
    }
}

void ReferenceCountingSetContainer::addAllUniqueRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        if(jobject tmp = rcContainer(env, container)){
            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, container2);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                jobject value = QtJambiAPI::nextOfJavaIterator(env, iter);
                if(!Java::Runtime::Collection::contains(env, tmp, value))
                    QtJambiAPI::addAllToJavaCollection(env, tmp, value);
            }
        }
    }
}

void ReferenceCountingSetContainer::addRC(JNIEnv * env, jobject container, jobject value){
    if(container){
        JniLocalFrame frame(env, 200);
        if(jobject rc = rcContainer(env, container))
            QtJambiAPI::addToJavaCollection(env, rc, value);
    }
}

void ReferenceCountingSetContainer::addUniqueRC(JNIEnv * env, jobject container, jobject value){
    if(container){
        JniLocalFrame frame(env, 200);
        if(jobject rc = rcContainer(env, container)){
            if(!Java::Runtime::Collection::contains(env, rc, value))
                QtJambiAPI::addToJavaCollection(env, rc, value);
        }
    }
}

void ReferenceCountingSetContainer::removeRC(JNIEnv * env, jobject container, jobject value){
    if(container){
        JniLocalFrame frame(env, 200);
        Java::Runtime::Collection::remove(env, rcContainer(env, container), value);
    }
}

void ReferenceCountingSetContainer::removeRC(JNIEnv * env, jobject container, jobject value, int n){
    if(container){
        JniLocalFrame frame(env, 200);
        jobject rc = rcContainer(env, container);
        for(int i=0; i<n; ++i)
            Java::Runtime::Collection::remove(env, rc, value);
    }
}

void ReferenceCountingMapContainer::clearRC(JNIEnv * env, jobject container){
    if(jobject rc = findContainer(env, container)){
        Java::Runtime::Map::clear(env, rc);
    }
}

void ReferenceCountingMultiMapContainer::clearRC(JNIEnv * env, jobject container){
    if(jobject rc = findContainer(env, container)){
        Java::Runtime::Map::clear(env, rc);
    }
}

void ReferenceCountingMapContainer::assignRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        jobject tmp2 = rcContainer(env, container2);
        Java::Runtime::Map::clear(env, tmp);
        Java::Runtime::Map::putAll(env, tmp, tmp2);
    }
}

void ReferenceCountingMultiMapContainer::assignRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        jobject tmp2 = rcContainer(env, container2);
        Java::Runtime::Map::clear(env, tmp);
        Java::Runtime::Map::putAll(env, tmp, tmp2);
    }
}

void ReferenceCountingMapContainer::putAllRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        Java::Runtime::Map::putAll(env, tmp, container2);
    }
}

void ReferenceCountingMultiMapContainer::putAllRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        Java::Runtime::Map::putAll(env, tmp, container2);
    }
}

void ReferenceCountingMapContainer::putRC(JNIEnv * env, jobject container, jobject key, jobject value){
    if(container){
        JniLocalFrame frame(env, 200);
        jobject rc = rcContainer(env, container);
        Java::Runtime::Map::put(env, rc, key, value);
    }
}

void ReferenceCountingMultiMapContainer::putRC(JNIEnv * env, jobject container, jobject key, jobject value){
    if(container){
        JniLocalFrame frame(env, 200);
        jobject rc = rcContainer(env, container);
        Java::Runtime::Map::put(env, rc, key, value);
    }
}

void ReferenceCountingMapContainer::removeRC(JNIEnv * env, jobject container, jobject key, int n){
    if(container){
        JniLocalFrame frame(env, 200);
        jobject rc = rcContainer(env, container);
        for(int i=0; i<n; ++i)
            Java::Runtime::Map::remove(env, rc, key);
    }
}

void ReferenceCountingMultiMapContainer::removeRC(JNIEnv * env, jobject container, jobject key, int n){
    if(container){
        JniLocalFrame frame(env, 200);
        jobject rc = rcContainer(env, container);
        for(int i=0; i<n; ++i)
            Java::Runtime::Map::remove(env, rc, key);
    }
}

void ReferenceCountingMultiMapContainer::removeRC(JNIEnv * env, jobject container, jobject key, jobject value, int n){
    if(container){
        JniLocalFrame frame(env, 200);
        jobject rc = rcContainer(env, container);
        for(int i=0; i<n; ++i)
            Java::Runtime::Map::removePair(env, rc, key, value);
    }
}

ReferenceCountingSetContainer* ReferenceCountingSetContainer::asRCSet() { return this; }
ReferenceCountingMapContainer* ReferenceCountingMapContainer::asRCMap() { return this; }
ReferenceCountingMultiMapContainer* ReferenceCountingMultiMapContainer::asRCMultiMap() { return this; }

void registerContainerConverter(QSharedPointer<AbstractSequentialAccess>&& containerAccess, const QMetaType& containerMetaType){
    QMetaType jCollectionWrapperType = QMetaType::fromType<JCollectionWrapper>();
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
        || !QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        bool isSpan = containerAccess->isSpan();
        QMetaType::ConverterFunction converter = [containerAccess = std::move(containerAccess)](const void *src, void *target) -> bool {
            if(src){
                if(JniEnvironment env{500}){
                    const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(src);
                    jobject jobj;
                    if(javaObject && (jobj = javaObject->object(env))){
                        if(Java::QtJambi::NativeUtility$Object::isInstanceOf(env, jobj)){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jobj)){
                                if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                    if((containerAccess->isList() && _containerAccess->isList())
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                                        || (containerAccess->isSpan() && _containerAccess->isSpan())
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                                        || (containerAccess->isSet() && _containerAccess->isSet())){
                                        AbstractSequentialAccess* __containerAccess = static_cast<AbstractSequentialAccess*>(_containerAccess);
                                        if(__containerAccess->elementMetaType()==containerAccess->elementMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                }
                            }else{
                                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, jobj)) QTJAMBI_STACKTRACEINFO );
                            }
                        }
                        if(containerAccess->isList()){
                            jobject targetObject = QtJambiAPI::findObject(env, target);
                            AbstractListAccess* __containerAccess = static_cast<AbstractListAccess*>(containerAccess.get());
                            __containerAccess->clear(env, ContainerInfo{targetObject, target});
                            ContainerAndAccessInfo other(jobj);
                            __containerAccess->appendList(env, ContainerInfo{targetObject, target}, other);
                            return true;
                        }else if(containerAccess->isSet()){
                            jobject targetObject = QtJambiAPI::findObject(env, target);
                            AbstractSetAccess* __containerAccess = static_cast<AbstractSetAccess*>(containerAccess.get());
                            __containerAccess->clear(env, ContainerInfo{targetObject, target});
                            ContainerAndAccessInfo other(jobj);
                            __containerAccess->unite(env, ContainerInfo{targetObject, target}, other);
                            return true;
                        }
                    }
                }
            }
            return false;
        };
        if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
            && !isSpan
#endif
            )
            QMetaType::registerConverterFunction(converter, jCollectionWrapperType, containerMetaType);
        if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType))
            QMetaType::registerConverterFunction(converter, jObjectWrapperType, containerMetaType);
    }
}

void registerContainerConverter(QSharedPointer<AbstractAssociativeAccess>&& containerAccess, const QMetaType& containerMetaType){
    QMetaType jCollectionWrapperType = QMetaType::fromType<JMapWrapper>();
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
        || !QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        QMetaType::ConverterFunction converter = [containerAccess = std::move(containerAccess)](const void *src, void *target) -> bool {
            if(src){
                if(JniEnvironment env{500}){
                    const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(src);
                    jobject jobj;
                    if(javaObject && (jobj = javaObject->object(env))){
                        if(Java::QtJambi::NativeUtility$Object::isInstanceOf(env, jobj)){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jobj)){
                                if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                    if((containerAccess->isMap() && _containerAccess->isMap())
                                        || (containerAccess->isHash() && _containerAccess->isHash())
                                        || (containerAccess->isMultiMap() && _containerAccess->isMultiMap())
                                        || (containerAccess->isMultiHash() && _containerAccess->isMultiHash())){
                                        AbstractAssociativeAccess* __containerAccess = static_cast<AbstractAssociativeAccess*>(containerAccess.get());
                                        if(__containerAccess->keyMetaType()==containerAccess->keyMetaType() && __containerAccess->valueMetaType()==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                        if(Java::Runtime::Map::isInstanceOf(env, jobj)){
                            jobject targetObject = QtJambiAPI::findObject(env, target);
                            if(containerAccess->isMap()){
                                AbstractMapAccess* __containerAccess = static_cast<AbstractMapAccess*>(containerAccess.get());
                                jobject it = Java::Runtime::Iterable::iterator(env, Java::Runtime::Map::entrySet(env, jobj));
                                while(Java::Runtime::Iterator::hasNext(env, it)){
                                    jobject o = Java::Runtime::Iterator::next(env, it);
                                    __containerAccess->insert(env, ContainerInfo{targetObject, target}, Java::Runtime::Map$Entry::getKey(env, o), Java::Runtime::Map$Entry::getValue(env, o));
                                }
                            }else if(containerAccess->isHash()){
                                AbstractHashAccess* __containerAccess = static_cast<AbstractHashAccess*>(containerAccess.get());
                                jobject it = Java::Runtime::Iterable::iterator(env, Java::Runtime::Map::entrySet(env, jobj));
                                while(Java::Runtime::Iterator::hasNext(env, it)){
                                    jobject o = Java::Runtime::Iterator::next(env, it);
                                    __containerAccess->insert(env, ContainerInfo{targetObject, target}, Java::Runtime::Map$Entry::getKey(env, o), Java::Runtime::Map$Entry::getValue(env, o));
                                }
                            }else if(containerAccess->isMultiMap()){
                                AbstractMultiMapAccess* __containerAccess = static_cast<AbstractMultiMapAccess*>(containerAccess.get());
                                jobject it = Java::Runtime::Iterable::iterator(env, Java::Runtime::Map::entrySet(env, jobj));
                                while(Java::Runtime::Iterator::hasNext(env, it)){
                                    jobject o = Java::Runtime::Iterator::next(env, it);
                                    jobject key = Java::Runtime::Map$Entry::getKey(env, o);
                                    jobject value = Java::Runtime::Map$Entry::getValue(env, o);
                                    if(Java::Runtime::Collection::isInstanceOf(env, value)){
                                        jobject it2 = Java::Runtime::Iterable::iterator(env, value);
                                        while(Java::Runtime::Iterator::hasNext(env, it2)){
                                            jobject v = Java::Runtime::Iterator::next(env, it2);
                                            __containerAccess->insert(env, ContainerInfo{targetObject, target}, key, v);
                                        }
                                    }else{
                                        __containerAccess->insert(env, ContainerInfo{targetObject, target}, key, value);
                                    }
                                }
                            }else if(containerAccess->isMultiHash()){
                                AbstractMultiHashAccess* __containerAccess = static_cast<AbstractMultiHashAccess*>(containerAccess.get());
                                jobject it = Java::Runtime::Iterable::iterator(env, Java::Runtime::Map::entrySet(env, jobj));
                                while(Java::Runtime::Iterator::hasNext(env, it)){
                                    jobject o = Java::Runtime::Iterator::next(env, it);
                                    jobject key = Java::Runtime::Map$Entry::getKey(env, o);
                                    jobject value = Java::Runtime::Map$Entry::getValue(env, o);
                                    if(Java::Runtime::Collection::isInstanceOf(env, value)){
                                        jobject it2 = Java::Runtime::Iterable::iterator(env, value);
                                        while(Java::Runtime::Iterator::hasNext(env, it2)){
                                            jobject v = Java::Runtime::Iterator::next(env, it2);
                                            __containerAccess->insert(env, ContainerInfo{targetObject, target}, key, v);
                                        }
                                    }else{
                                        __containerAccess->insert(env, ContainerInfo{targetObject, target}, key, value);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return false;
        };
        if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType))
            QMetaType::registerConverterFunction(converter, jCollectionWrapperType, containerMetaType);
        if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType))
            QMetaType::registerConverterFunction(converter, jObjectWrapperType, containerMetaType);
    }
}

void registerContainerConverter(QSharedPointer<AbstractPairAccess>&& pairAccess, const QMetaType& containerMetaType){
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        QMetaType::registerConverterFunction(
                    [pairAccess](const void *src, void *target) -> bool {
                                if(src){
                                    if(JniEnvironment env{500}){
                                        const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(src);
                                        if(javaObject){
                                            if(jobject jobj = javaObject->object(env)){
                                                jobject first = Java::QtCore::QPair::first(env, jobj);
                                                jobject second = Java::QtCore::QPair::second(env, jobj);
                                                pairAccess->setFirst(env, target, first);
                                                pairAccess->setSecond(env, target, second);
                                                return true;
                                            }
                                        }
                                    }
                                }
                                return false;
                            }
                    , jObjectWrapperType, containerMetaType);
    }
}

namespace QtJambiPrivate{

thread_local QList<std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *>> Ref::refMetaTypes;

Ref::Ref(const QtPrivate::QMetaTypeInterface *_iface) : iface(_iface), ptr(iface->alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__
                                                                           ? operator new(iface->size, std::align_val_t(iface->alignment))
                                                                           : operator new(iface->size)) {
}

Ref::Ref() : Ref(refMetaTypes.constLast().first) {
}

SecondRef::SecondRef() : Ref(refMetaTypes.constLast().second) {
}

QDataStream& operator>>(QDataStream& s, Ref& ref){
    if(ref.iface->dataStreamIn){
        if(ref.iface->defaultCtr)
            ref.iface->defaultCtr(ref.iface, ref.ptr);
        ref.iface->dataStreamIn(ref.iface, s, ref.ptr);
    }else if(ref.iface->flags & QMetaType::IsEnumeration){
        switch(ref.iface->size){
        case 1: s >> *reinterpret_cast<qint8*>(ref.ptr); break;
        case 2: s >> *reinterpret_cast<qint16*>(ref.ptr); break;
        case 4: s >> *reinterpret_cast<qint32*>(ref.ptr); break;
        case 8: s >> *reinterpret_cast<qint64*>(ref.ptr); break;
        default: break;
        }
    }else{
        QMetaType metaType(ref.iface);
        QVariant v(metaType);
        v.load(s);
        metaType.construct(ref.ptr, v.data());
    }
    return s;
}

QDataStream& operator<<(QDataStream& s, const ConstRef& ref){
    if(ref.iface->dataStreamOut)
        ref.iface->dataStreamOut(ref.iface, s, ref.ptr);
    else if(ref.iface->flags & QMetaType::IsEnumeration){
        switch(ref.iface->size){
        case 1: s << *reinterpret_cast<const qint8*>(ref.ptr); break;
        case 2: s << *reinterpret_cast<const qint16*>(ref.ptr); break;
        case 4: s << *reinterpret_cast<const qint32*>(ref.ptr); break;
        case 8: s << *reinterpret_cast<const qint64*>(ref.ptr); break;
        default: break;
        }
    }else
        QVariant(QMetaType(ref.iface), ref.ptr).save(s);
    return s;
}

QDebug& operator<<(QDebug& dbg, const ConstRef& ref){
    if(ref.iface->debugStream)
        ref.iface->debugStream(ref.iface, dbg, ref.ptr);
    else if(ref.iface->flags & QMetaType::IsPointer){
        if(ref.iface->metaObjectFn && ref.iface->metaObjectFn(ref.iface))
            dbg << ref.iface->metaObjectFn(ref.iface)->className() << "(";
        else if(QLatin1String(ref.iface->name).endsWith('*'))
            dbg << QLatin1String(ref.iface->name).chopped(1) << "(";
        else
            dbg << ref.iface->name << "(";
        dbg << "0x" << QString::number(*reinterpret_cast<const qint64*>(ref.ptr), 16);
        dbg << ")";
    }else if(ref.iface->flags & QMetaType::IsEnumeration){
        dbg << ref.iface->name << "(";
        switch(ref.iface->size){
        case 1: dbg << *reinterpret_cast<const qint8*>(ref.ptr); break;
        case 2: dbg << *reinterpret_cast<const qint16*>(ref.ptr); break;
        case 4: dbg << *reinterpret_cast<const qint32*>(ref.ptr); break;
        case 8: dbg << *reinterpret_cast<const qint64*>(ref.ptr); break;
        default: break;
        }
        dbg << ")";
    }else
        dbg << QVariant(QMetaType(ref.iface), ref.ptr);
    return dbg;
}

bool operator==(const ConstRef& ref1, const ConstRef& ref2){
    if(ref1.iface==ref2.iface){
        if(ref1.iface->equals){
            return ref1.iface->equals(ref1.iface, ref1.ptr, ref2.ptr);
        }else if(ref1.iface->lessThan){
            return !ref1.iface->lessThan(ref1.iface, ref1.ptr, ref2.ptr) && !ref1.iface->lessThan(ref1.iface, ref2.ptr, ref1.ptr);
        }
    }
    return false;
}

}

