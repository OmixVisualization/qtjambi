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


#ifndef CONTAINERACCESS_P_H
#define CONTAINERACCESS_P_H

#include <QtCore/QtGlobal>
#include <QtCore/QThreadStorage>

QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wextra")
QT_WARNING_DISABLE_CLANG("-Wextra")
QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

#include "utils_p.h"
#include "objectdata.h"
#include "qtjambilink_p.h"
#include "containeraccess.h"
#include "containeraccess_iterator.h"

class QtJambiLink;

class AutoMultiMapAccess;
class AutoMultiHashAccess;

namespace QtJambiPrivate{

template<typename, typename, typename, bool, bool, bool, typename...>
struct qtjambi_ContainerIterator_cast;

jobject convertIteratorToJavaObject(JNIEnv *env,
                                    const QSharedPointer<QtJambiLink>& owner,
                                    void* iteratorPtr,
                                    PtrDeleterFunction destructor_function,
                                    AbstractAssociativeConstIteratorAccess* containerAccess);
jobject convertMapIteratorToJavaObject(JNIEnv *env,
                                    const QSharedPointer<QtJambiLink>& owner,
                                    void* iteratorPtr,
                                    PtrDeleterFunction destructor_function,
                                    AbstractAssociativeConstIteratorAccess* containerAccess);
jobject convertMultiMapIteratorToJavaObject(JNIEnv *env,
                                       const QSharedPointer<QtJambiLink>& owner,
                                       void* iteratorPtr,
                                       PtrDeleterFunction destructor_function,
                                       AbstractAssociativeConstIteratorAccess* containerAccess);
jobject convertHashIteratorToJavaObject(JNIEnv *env,
                                       const QSharedPointer<QtJambiLink>& owner,
                                       void* iteratorPtr,
                                       PtrDeleterFunction destructor_function,
                                       AbstractAssociativeConstIteratorAccess* containerAccess);
jobject convertMultiHashIteratorToJavaObject(JNIEnv *env,
                                            const QSharedPointer<QtJambiLink>& owner,
                                            void* iteratorPtr,
                                            PtrDeleterFunction destructor_function,
                                            AbstractAssociativeConstIteratorAccess* containerAccess);
jobject convertMapKeyIteratorToJavaObject(JNIEnv *env,
                                          const QSharedPointer<QtJambiLink>& owner,
                                          void* iteratorPtr,
                                          PtrDeleterFunction destructor_function,
                                          AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertMultiMapKeyIteratorToJavaObject(JNIEnv *env,
                                            const QSharedPointer<QtJambiLink>& owner,
                                            void* iteratorPtr,
                                            PtrDeleterFunction destructor_function,
                                            AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertHashKeyIteratorToJavaObject(JNIEnv *env,
                                        const QSharedPointer<QtJambiLink>& owner,
                                        void* iteratorPtr,
                                        PtrDeleterFunction destructor_function,
                                        AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertMultiHashKeyIteratorToJavaObject(JNIEnv *env,
                                             const QSharedPointer<QtJambiLink>& owner,
                                             void* iteratorPtr,
                                             PtrDeleterFunction destructor_function,
                                             AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertMultiMapIteratorToJavaObject(JNIEnv *env,
                                            const QSharedPointer<QtJambiLink>& owner,
                                            void* iteratorPtr,
                                            PtrDeleterFunction destructor_function,
                                            AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                        const QSharedPointer<QtJambiLink>& owner,
                                        void* iteratorPtr,
                                        PtrDeleterFunction destructor_function,
                                        AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertMultiMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                             const QSharedPointer<QtJambiLink>& owner,
                                             void* iteratorPtr,
                                             PtrDeleterFunction destructor_function,
                                             AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertMultiHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                     const QSharedPointer<QtJambiLink>& owner,
                                                     void* iteratorPtr,
                                                     PtrDeleterFunction destructor_function,
                                                     AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                               const QSharedPointer<QtJambiLink>& owner,
                                               void* iteratorPtr,
                                               PtrDeleterFunction destructor_function,
                                               AbstractSequentialConstIteratorAccess* containerAccess);

jobject convertListIteratorToJavaObject(JNIEnv *env,
                                    const QSharedPointer<QtJambiLink>& owner,
                                    void* iteratorPtr,
                                    PtrDeleterFunction destructor_function,
                                    AbstractSequentialConstIteratorAccess* containerAccess);
jobject convertListReverseIteratorToJavaObject(JNIEnv *env,
                                        const QSharedPointer<QtJambiLink>& owner,
                                        void* iteratorPtr,
                                        PtrDeleterFunction destructor_function,
                                        AbstractSequentialConstIteratorAccess* containerAccess);

jobject convertSetIteratorToJavaObject(JNIEnv *env,
                                        const QSharedPointer<QtJambiLink>& owner,
                                        void* iteratorPtr,
                                        PtrDeleterFunction destructor_function,
                                        AbstractSequentialConstIteratorAccess* containerAccess);

jobject convertSpanIteratorToJavaObject(JNIEnv *env,
                                        const QSharedPointer<QtJambiLink>& owner,
                                        void* iteratorPtr,
                                        PtrDeleterFunction destructor_function,
                                        AbstractSequentialConstIteratorAccess* containerAccess);

jobject convertSpanReverseIteratorToJavaObject(JNIEnv *env,
                                        const QSharedPointer<QtJambiLink>& owner,
                                        void* iteratorPtr,
                                        PtrDeleterFunction destructor_function,
                                        AbstractSequentialConstIteratorAccess* containerAccess);


jobject convertIteratorToJavaObject(JNIEnv *env,
                                    const QSharedPointer<QtJambiLink>& owner,
                                    void* iteratorPtr,
                                    PtrDeleterFunction destructor_function,
                                    AbstractSequentialConstIteratorAccess* containerAccess);

jobject convertIteratorToJavaObject(JNIEnv *env,
                                    const std::type_info& containerTypeId,
                                    const std::type_info& iteratorTypeId,
                                    const QSharedPointer<QtJambiLink>& owner,
                                    void* iteratorPtr,
                                    PtrDeleterFunction destructor_function,
                                    AbstractAssociativeConstIteratorAccess* containerAccess);

jobject convertIteratorToJavaObject(JNIEnv *env,
                                    const std::type_info& containerTypeId,
                                    const std::type_info& iteratorTypeId,
                                    const QSharedPointer<QtJambiLink>& owner,
                                    void* iteratorPtr,
                                    PtrDeleterFunction destructor_function,
                                    AbstractSequentialConstIteratorAccess* containerAccess);

template<typename Access>
class ContainerClonePrivate : public QSharedData{
    Access* m_access;
    void* m_container;
public:
    template<typename _Access = Access, std::enable_if_t<std::is_same_v<_Access,AutoMultiMapAccess>,bool> = true>
    ContainerClonePrivate(_Access* access, const void* container)
        : m_access(dynamic_cast<_Access*>(access->clone())),
        m_container(m_access->createContainer(container))
    {
    }
    template<typename _Access = Access, std::enable_if_t<std::is_same_v<_Access,AutoMultiHashAccess>,bool> = true>
    ContainerClonePrivate(_Access* access, const void* container)
        : m_access(dynamic_cast<_Access*>(access->clone())),
        m_container(m_access->createContainer(container))
    {
    }
    template<typename _Access = Access, std::enable_if_t<!std::is_same_v<_Access,AutoMultiMapAccess> && !std::is_same_v<_Access,AutoMultiHashAccess>,bool> = true>
    ContainerClonePrivate(_Access* access, const void* container)
        : m_access(static_cast<_Access*>(access->clone())),
        m_container(m_access->createContainer(container))
    {
    }
    ContainerClonePrivate(ContainerClonePrivate&& other)
        : m_access(other.m_access),
        m_container(other.m_container)
    {
        other.m_access = nullptr;
        other.m_container = nullptr;
    }
    ContainerClonePrivate& operator=(ContainerClonePrivate&& other){
        m_access = other.m_access;
        m_container = other.m_container;
        other.m_access = nullptr;
        other.m_container = nullptr;
        return *this;
    }
    ContainerClonePrivate(const ContainerClonePrivate&) = delete;
    ContainerClonePrivate& operator=(const ContainerClonePrivate&) = delete;
    ~ContainerClonePrivate(){
        if(m_access){
            if(m_container)
                m_access->deleteContainer(m_container);
            m_access->dispose();
        }
    }
    Access* containerAccess() const {return m_access;}
    void* pointer() const {return m_container;}
};

template<typename Access>
class ContainerClone{
    QExplicitlySharedDataPointer<QtJambiPrivate::ContainerClonePrivate<Access>> d;
public:
    using value_type = char;
    using difference_type = qsizetype;
    using pointer = char*;
    using reference = char&;
    ContainerClone(Access* access, const void* container)
        : d(new QtJambiPrivate::ContainerClonePrivate<Access>(access, container))
    {
    }
    ContainerClone(Access* access, const ConstExtendedContainerInfo& container)
        : d(new QtJambiPrivate::ContainerClonePrivate<Access>(access, container.container))
    {
    }
    ContainerClone() = delete;
    ContainerClone(const ContainerClone& cl) : d(cl.d){}
    ContainerClone(ContainerClone&& cl) : d(std::move(cl.d)){}
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_isSharedWith_v<_Access,const void*,const void*>,bool> = true>
    bool isSharedWith(const ContainerClone& clone) const{
        return d && d->containerAccess()->isSharedWith(d->pointer(), clone.d->pointer());
    }
    auto begin() const {
        Q_ASSERT(d);
        return d->containerAccess()->constBegin(d->pointer());
    }
    auto end() const {
        Q_ASSERT(d);
        return d->containerAccess()->constEnd(d->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constReverseBegin_v<_Access,const void*>,bool> = true>
    auto crbegin() const {
        Q_ASSERT(d);
        return containerAccess()->constReverseBegin(d->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constReverseEnd_v<_Access,const void*>,bool> = true>
    auto crend() const {
        Q_ASSERT(d);
        return containerAccess()->constReverseEnd(d->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constKeyValueBegin_v<_Access,const void*>,bool> = true>
    auto constKeyValueBegin() const {
        Q_ASSERT(d);
        return containerAccess()->constKeyValueBegin(d->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constKeyValueEnd_v<_Access,const void*>,bool> = true>
    auto constKeyValueEnd() const {
        Q_ASSERT(d);
        return containerAccess()->constKeyValueEnd(d->pointer());
    }
    Access* containerAccess() const {return d ? d->containerAccess() : nullptr;}
};

template<typename Access,
         bool = supports_less_than_v<decltype(std::declval<Access&>().constBegin(std::declval<const void*>()))&>>
struct InitialItersAccessContainer{
    InitialItersAccessContainer(Access* access, const void * container)
        : m_initialBegin(access->constBegin(container)),
          m_initialEnd(access->constEnd(container))
    {}
    auto initialBegin() const {
        return m_initialBegin;
    }
    auto initialEnd() const {
        return m_initialEnd;
    }
private:
    decltype(std::declval<Access&>().constBegin(std::declval<const void*>())) m_initialBegin;
    decltype(std::declval<Access&>().constEnd(std::declval<const void*>())) m_initialEnd;
};

template<typename Access>
struct InitialItersAccessContainer<Access,true>{
    InitialItersAccessContainer(const Access*, const void *){}
};

template<typename Access>
struct ContainerAccessLink : InitialItersAccessContainer<Access>{
    using value_type = char;
    using difference_type = qsizetype;
    using pointer = char*;
    using reference = char&;
    ContainerAccessLink(Access* access, QSharedPointer<QtJambiLink>&& link)
        : InitialItersAccessContainer<Access>(access, link->pointer()),
          m_link(std::move(link)) {}
    ContainerAccessLink(Access* access, const ExtendedContainerInfo& container)
        : InitialItersAccessContainer<Access>(access, container.container),
        m_link(QtJambiLink::fromNativeId(container.nativeId)) {}
    bool operator==(const ContainerAccessLink& other) const {
        return m_link==other.m_link;
    }
    template<typename _Access = Access, std::enable_if_t<supports_isSharedWith_v<_Access,const void*,const void*>,bool> = true>
    bool isSharedWith(const ContainerAccessLink& clone) const{
        return containerAccess()->isSharedWith(m_link->pointer(), clone.m_link->pointer());
    }
    auto begin() const {
        return containerAccess()->constBegin(m_link->pointer());
    }
    auto end() const {
        return containerAccess()->constEnd(m_link->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constReverseBegin_v<_Access,const void*>,bool> = true>
    auto crbegin() const {
        return containerAccess()->constReverseBegin(m_link->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constReverseEnd_v<_Access,const void*>,bool> = true>
    auto crend() const {
        return containerAccess()->constReverseEnd(m_link->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constKeyValueBegin_v<_Access,const void*>,bool> = true>
    auto constKeyValueBegin() const {
        return containerAccess()->constKeyValueBegin(m_link->pointer());
    }
    template<typename _Access = Access, std::enable_if_t<QtJambiPrivate::supports_constKeyValueEnd_v<_Access,const void*>,bool> = true>
    auto constKeyValueEnd() const {
        return containerAccess()->constKeyValueEnd(m_link->pointer());
    }
    const QSharedPointer<QtJambiLink>& link() const {
        return m_link;
    }
    Access* containerAccess() const{
        if constexpr(std::is_same_v<Access,AutoMultiMapAccess>){
            return dynamic_cast<Access*>(m_link->containerAccess());
        }else if constexpr(std::is_same_v<Access,AutoMultiHashAccess>){
            return dynamic_cast<Access*>(m_link->containerAccess());
        }else{
            return static_cast<Access*>(m_link->containerAccess());
        }
    }
private:
    QSharedPointer<QtJambiLink> m_link;
};

template<typename, typename, bool>
struct CreateConstIterator;

template<typename Access, bool b>
struct CreateConstIterator<ContainerIterator<ContainerAccessLink<Access>,typename Access::iterator>, Access, b>{
    using Iter = ContainerIterator<ContainerAccessLink<Access>,typename Access::iterator>;
    using type = ContainerIterator<ContainerClone<Access>,typename Access::const_iterator>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        return new type(typename Access::const_iterator(iterator.iterator()), ContainerClone<Access>(iterator.storage().containerAccess(), iterator.storage().link()->pointer()));
    }
};

template<typename Access, bool b>
struct CreateConstIterator<ContainerIterator<ContainerAccessLink<Access>,typename Access::key_value_iterator>, Access, b>{
    using Iter = ContainerIterator<ContainerAccessLink<Access>,typename Access::key_value_iterator>;
    using type = ContainerIterator<ContainerClone<Access>,typename Access::const_key_value_iterator>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        return new type(typename Access::const_key_value_iterator(iterator.iterator()), ContainerClone<Access>(iterator.storage().containerAccess(), iterator.storage().link()->pointer()));
    }
};

template<typename Access, bool b>
struct CreateConstIterator<ContainerIterator<ContainerAccessLink<Access>,std::reverse_iterator<typename Access::iterator>>, Access, b>{
    using Iter = ContainerIterator<ContainerAccessLink<Access>,std::reverse_iterator<typename Access::iterator>>;
    using type = ContainerIterator<ContainerClone<Access>,std::reverse_iterator<typename Access::const_iterator>>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        return new type(std::reverse_iterator<typename Access::const_iterator>(iterator.iterator()), ContainerClone<Access>(iterator.storage().containerAccess(), iterator.storage().link()->pointer()));
    }
};

}

template<typename,typename,typename>
struct ContainerIterator;

template<typename Access, typename Iter>
ContainerIterator(Iter&& iter, Access* c, const ExtendedContainerInfo& container) -> ContainerIterator<QtJambiPrivate::ContainerAccessLink<Access>, Iter>;

template<typename Access, typename Iter>
ContainerIterator(Iter&& iter, Access* c, const ConstExtendedContainerInfo& container) -> ContainerIterator<QtJambiPrivate::ContainerClone<Access>, Iter>;

QMetaType registerContainerMetaType(QByteArrayView typeName,
                                    QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                    QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                    QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                    QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                    QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                    QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                    QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                    QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                    QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                    QtPrivate::QMetaTypeInterface::LegacyRegisterOp legacyRegisterOp,
                                    uint size,
                                    ushort align,
                                    int builtInTypeId,
                                    QMetaType::TypeFlags flags,
                                    const QMetaObject *metaObject,
                                    AfterRegistrationFunction afterRegistrationFunction,
                                    const QSharedPointer<AbstractContainerAccess>& sharedAccess);
QSharedPointer<AbstractContainerAccess> findContainerAccess(const QMetaType& metaType);

void registerContainerConverter(SequentialContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& elementMetaType);
void registerContainerConverter(AssociativeContainerType mapType, const QMetaType& containerMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType);
void registerContainerConverter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType);
void insertHashFunctionByMetaType(const QtPrivate::QMetaTypeInterface * type, const QtJambiUtils::QHashFunction& fct);
void insertHashFunctionByMetaType(const QtPrivate::QMetaTypeInterface * type, QtJambiUtils::QHashFunction&& fct);
void containerDisposer(AbstractContainerAccess* _access);

class AutoPairAccess : public AbstractPairAccess, public AbstractNestedPairAccess {
    QMetaType m_keyMetaType;
    QtJambiUtils::QHashFunction m_keyHashFunction;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    QMetaType m_valueMetaType;
    QtJambiUtils::QHashFunction m_valueHashFunction;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    size_t m_align;
    size_t m_offset;
    size_t m_size;
    PtrOwnerFunction m_keyOwnerFunction;
    PtrOwnerFunction m_valueOwnerFunction;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;

protected:
    AutoPairAccess(const AutoPairAccess& other);
public:
    AutoPairAccess(
            const QMetaType& keyMetaType,
            const QtJambiUtils::QHashFunction& keyHashFunction,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
            PtrOwnerFunction keyOwnerFunction,
            AbstractContainerAccess::DataType keyDataType,
            const QMetaType& valueMetaType,
            const QtJambiUtils::QHashFunction& valueHashFunction,
            const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
            PtrOwnerFunction valueOwnerFunction,
            AbstractContainerAccess::DataType valueDataType
            );

    void dispose() override;
    AutoPairAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
private:
    AbstractNestedPairAccess* asNested() override;
    bool equals(const void* p1, const void* p2);
    void debugStream(QDebug &s, const void *ptr);
    void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
public:
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    QMetaType registerContainer(QByteArrayView typeName) override;
    jobject first(JNIEnv * env, const void* container) override;
    void setFirst(JNIEnv *env, void* container, jobject first) override;
    jobject second(JNIEnv * env, const void* container) override;
    void setSecond(JNIEnv *env, void* container, jobject second) override;
    const void* first(const void*) override;
    const void* second(const void*) override;
    void* first(void*) override;
    void* second(void*) override;
    void setFirst(void*,const void*) override;
    void setSecond(void*,const void*) override;

    QPair<const void*,const void*> elements(const void* container) override;
    const QMetaType& firstMetaType() override;
    const QMetaType& secondMetaType() override;
    DataType firstType() override;
    DataType secondType() override;
    AbstractContainerAccess* firstNestedContainerAccess() override;
    AbstractContainerAccess* secondNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedFirstNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedSecondNestedContainerAccess() override;
    bool hasFirstNestedContainerAccess() override;
    bool hasSecondNestedContainerAccess() override;
    bool hasFirstNestedPointers() override;
    bool hasSecondNestedPointers() override;
    std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(const void*) override;
    std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(void*) override;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(const void*) override;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(void*) override;
};


class OptionalAccess : public AbstractPairAccess, public AbstractNestedPairAccess {
    QMetaType m_keyMetaType;
    QtJambiUtils::QHashFunction m_keyHashFunction;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    size_t m_align;
    size_t m_offset;
    size_t m_size;
    PtrOwnerFunction m_keyOwnerFunction;
    AbstractContainerAccess::DataType m_keyDataType;

protected:
    OptionalAccess(const OptionalAccess& other);
public:
    OptionalAccess(
        const QMetaType& keyMetaType,
        const QtJambiUtils::QHashFunction& keyHashFunction,
        const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
        PtrOwnerFunction keyOwnerFunction,
        AbstractContainerAccess::DataType keyDataType);
    bool hasValue(const void* container);

    void dispose() override;
    OptionalAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
private:
    AbstractNestedPairAccess* asNested() override;
    bool equals(const void* p1, const void* p2);
    void debugStream(QDebug &s, const void *ptr);
    void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    jobject second(JNIEnv * env, const void* container) override;
    void setSecond(JNIEnv *env, void* container, jobject second) override;
public:
    jobject first(JNIEnv * env, const void* container) override;
    void setFirst(JNIEnv *env, void* container, jobject first) override;
    const void* first(const void*) override;
    const void* second(const void*) override;
    void* first(void*) override;
    void* second(void*) override;
    void setFirst(void*,const void*) override;
    void setSecond(void*,const void*) override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    QMetaType registerContainer(QByteArrayView typeName) override;

    QPair<const void*,const void*> elements(const void* container) override;
    const QMetaType& firstMetaType() override;
    const QMetaType& secondMetaType() override;
    DataType firstType() override;
    DataType secondType() override;
    AbstractContainerAccess* firstNestedContainerAccess() override;
    AbstractContainerAccess* secondNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedFirstNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedSecondNestedContainerAccess() override;
    bool hasFirstNestedContainerAccess() override;
    bool hasSecondNestedContainerAccess() override;
    bool hasFirstNestedPointers() override;
    bool hasSecondNestedPointers() override;
    std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(const void*) override;
    std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(void*) override;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(const void*) override;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(void*) override;
};

template<typename SuperType>
struct AbstractAutoContainerIteratorAccess : SuperType{
    AbstractAutoContainerIteratorAccess() {}
    ~AbstractAutoContainerIteratorAccess() {}
    AbstractContainerAccess::ContainerType containerType() const override { return AbstractContainerAccess::ContainerType(SuperType::containerType() | AbstractContainerAccess::AutoAccess); }
};

template<typename Access, typename Iterator, AbstractSequentialConstIteratorAccess::IteratorType type, typename SuperType>
struct AbstractAutoConstIteratorAccess : AbstractAutoContainerIteratorAccess<SuperType>{
    AbstractSequentialConstIteratorAccess::IteratorType iteratorType() const override {
        return type;
    }
    AbstractAutoConstIteratorAccess() : AbstractAutoContainerIteratorAccess<SuperType>() {}
    void dispose() final override {}
    void advance(JNIEnv *env, void* iterator, qsizetype n) override {
        QtJambiPrivate::IteratorAdvance<Iterator>::function(env, iterator, n);
    }
    bool advance(void* iterator, qsizetype n) override {
        return QtJambiPrivate::IteratorAdvance<Iterator>::function(iterator, n);
    }
    void increment(JNIEnv *env, void* iterator) override {
        QtJambiPrivate::IteratorIncrement<Iterator>::function(env, iterator);
    }
    void increment(void* iterator) override {
        QtJambiPrivate::IteratorIncrement<Iterator>::function(iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QtJambiPrivate::IteratorDecrement<Iterator>::function(env, iterator);
    }
    void decrement(void* iterator) override {
        QtJambiPrivate::IteratorDecrement<Iterator>::function(iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return QtJambiPrivate::IteratorLessThan<Iterator>::function(env, iterator, other);
    }
    std::optional<bool> lessThan(const void* iterator, const void* other) override {
        return QtJambiPrivate::IteratorLessThan<Iterator>::function(iterator, other);
    }
    std::optional<size_t> distance(const void* iterator, const void* other) override {
        return QtJambiPrivate::IteratorDistance<Iterator>::function(iterator, other);
    }
    bool canDistance() override {
        return QtJambiPrivate::IteratorDistance<Iterator>::value;
    }
    bool canLess() override {
        return QtJambiPrivate::IteratorLessThan<Iterator>::value;
    }
    bool isBidirectionalIterator() override {
        return QtJambiPrivate::IteratorDecrement<Iterator>::value;
    }
    bool isBegin(JNIEnv *env, const void* iterator) override {
        return QtJambiPrivate::IteratorIsBegin<Iterator>::function(env, iterator);
    }
    std::optional<bool> isBegin(const void* iterator) override {
        return QtJambiPrivate::IteratorIsBegin<Iterator>::function(iterator);
    }
    bool isEnd(JNIEnv *env, const void* iterator) override {
        return QtJambiPrivate::IteratorIsEnd<Iterator>::function(env, iterator);
    }
    std::optional<bool> isEnd(const void* iterator) override {
        return QtJambiPrivate::IteratorIsEnd<Iterator>::function(iterator);
    }
    bool isValid(JNIEnv *env, const void* iterator) override {
        return QtJambiPrivate::IteratorIsValid<Iterator>::function(env, iterator);
    }
    std::optional<bool> isValid(const void* iterator) override {
        return QtJambiPrivate::IteratorIsValid<Iterator>::function(iterator);
    }
    jboolean equals(JNIEnv *, const void* ptr, const void* ptr2) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const Iterator* iterator2 = static_cast<const Iterator*>(ptr2);
        return (*iterator)==(*iterator2);
    }
    bool equals(const void* ptr, const void* ptr2) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const Iterator* iterator2 = static_cast<const Iterator*>(ptr2);
        return (*iterator)==(*iterator2);
    }
    size_t sizeOf() const override {return sizeof(Iterator);}
    size_t alignOf() const override {return alignof(Iterator);}
    void* constructContainer(void* placement, const void* copyOf) override{
        if constexpr(QtJambiPrivate::is_copy_constructible_v<Iterator>){
            return new(placement)Iterator(*static_cast<const Iterator*>(copyOf));
        }else{
            Q_UNUSED(placement)
            Q_UNUSED(copyOf)
            return nullptr;
        }
    }
    bool canCopy() const override {return QtJambiPrivate::is_copy_constructible_v<Iterator>;}
};

template<typename Access, typename Iterator = void,
         AbstractSequentialConstIteratorAccess::IteratorType type = AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,
         typename SuperType = AbstractSequentialConstIteratorAccess>
class AutoSequentialConstIteratorAccess : public AbstractAutoConstIteratorAccess<Access,Iterator,type,SuperType>{
    using Super = AbstractAutoConstIteratorAccess<Access,Iterator,type,SuperType>;
protected:
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QMetaType m_valueMetaType;
    QtJambiUtils::QHashFunction m_hashFunction;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    PtrOwnerFunction m_elementOwnerFunction;
    AbstractContainerAccess::DataType m_elementDataType;
public:
    using Super::equals;
    AutoSequentialConstIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QMetaType& valueMetaType,
            const QtJambiUtils::QHashFunction& hashFunction,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction,
            AbstractContainerAccess::DataType elementDataType
        )
        : Super(),
        m_internalToExternalConverter(internalToExternalConverter),
        m_valueMetaType(valueMetaType),
        m_hashFunction(hashFunction),
        m_elementNestedContainerAccess(elementNestedContainerAccess),
        m_elementOwnerFunction(elementOwnerFunction),
        m_elementDataType(elementDataType)
    {
    }
    using Super::value;

    SuperType* clone() override{
        if constexpr(std::is_same_v<SuperType,AbstractSequentialConstIteratorAccess>){
            return new AutoSequentialConstIteratorAccess<Access,Iterator,type,SuperType>(
                        m_internalToExternalConverter,
                        m_valueMetaType,
                        m_hashFunction,
                        m_elementNestedContainerAccess,
                        m_elementOwnerFunction,
                        m_elementDataType);
        }else return nullptr;
    }

    jobject value(JNIEnv * env, const void* ptr) override {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const void* v = &*iterator;
        jvalue jval;
        jval.l = nullptr;
        if(m_internalToExternalConverter(env, nullptr, v, jval, true))
            return jval.l;
        return nullptr;
    }
    std::optional<const void*> value(const void* ptr) override {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        return std::make_optional<const void*>(&*iterator);
    }
    template<typename T>
    std::optional<T> value(const void* ptr) {
        if constexpr(QtJambiPrivate::supports_assign_v<T, std::add_lvalue_reference_t<decltype(*std::declval<Iterator>())>>){
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            const void* d = &*iterator;
            return std::make_optional<T>(*reinterpret_cast<const T*>(d));
        }else{
            Q_UNUSED(ptr)
            return std::nullopt;
        }
    }
    std::optional<jint> intValue(const void* ptr) override {
        return value<jint>(ptr);
    }
    std::optional<jlong> longValue(const void* ptr) override {
        return value<jlong>(ptr);
    }
    std::optional<jshort> shortValue(const void* ptr) override {
        return value<jshort>(ptr);
    }
    std::optional<jbyte> byteValue(const void* ptr) override {
        return value<jbyte>(ptr);
    }
    std::optional<jfloat> floatValue(const void* ptr) override {
        return value<jfloat>(ptr);
    }
    std::optional<jdouble> doubleValue(const void* ptr) override {
        return value<jdouble>(ptr);
    }
    std::optional<jchar> charValue(const void* ptr) override {
        return value<jchar>(ptr);
    }
    std::optional<jboolean> booleanValue(const void* ptr) override {
        return value<jboolean>(ptr);
    }
    QVariant variantValue(const void* ptr) override {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const void* d = &*iterator;
        return QVariant(m_valueMetaType, d);
    }
    const QMetaType& valueMetaType() override{
        return m_valueMetaType;
    }
    jboolean equals(JNIEnv *env, const void* ptr, const ConstContainerAndAccessInfo& ptr2) override{
        return Access::iteratorEquals(env, ptr, type, ptr2);
    }
    void* asIterator(void* iterator) override{
        return Access::asIterator(iterator, type);
    }
    bool findIterator(const void* iterator, const std::type_info& typeId, void* output) override{
        return Access::findIterator(iterator, type, typeId, output);
    }
    bool isContiguousIterator() override{
        return std::is_pointer_v<Iterator>;
    }
    bool isRandomAccessIterator() override{
        return QtJambiPrivate::is_random_access_iterator_v<Iterator>;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AbstractSpanAccess* createSpanAccess() override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    void assign(void* iterator, const void* other) override{
        if constexpr(QtJambiPrivate::supports_assign_v<Iterator&,const Iterator&>){
            *static_cast<Iterator*>(iterator) = *static_cast<const Iterator*>(other);
        }
        Q_UNUSED(iterator)
        Q_UNUSED(other)
    }
    void* constructContainer(void* placement) override{
        if constexpr(QtJambiPrivate::is_default_constructible_v<Iterator>){
            return new(placement)Iterator();
        }else{
            Q_UNUSED(placement)
            return nullptr;
        }
    }
    void* constructContainer(void* placement,const void* copyOf) override{
        if constexpr(QtJambiPrivate::is_copy_constructible_v<Iterator>){
            return new(placement)Iterator(*static_cast<const Iterator*>(copyOf));
        }else{
            Q_UNUSED(placement)
            Q_UNUSED(copyOf)
            return nullptr;
        }
    }
    void* constructContainer(void* placement,void* moveOf) override{
        if constexpr(QtJambiPrivate::is_move_constructible_v<Iterator>){
            return new(placement)Iterator(std::move(*static_cast<Iterator*>(moveOf)));
        }else{
            Q_UNUSED(placement)
            Q_UNUSED(moveOf)
            return nullptr;
        }
    }
    bool destructContainer(void* ptr) override {
        static_cast<const Iterator*>(ptr)->~Iterator();
        return true;
    }
    std::pair<void*,AbstractSequentialConstIteratorAccess*> createConstIterator(const void* iterator) override {
        if constexpr(std::is_same_v<typename QtJambiPrivate::CreateConstIterator<Iterator,Access>::type,void>){
            return {nullptr,nullptr};
        }else{
            return {
                    QtJambiPrivate::CreateConstIterator<Iterator,Access>::function(iterator),
                    new AutoSequentialConstIteratorAccess<Access,typename QtJambiPrivate::CreateConstIterator<Iterator,Access>::type,type>(
                                    m_internalToExternalConverter,
                                    m_valueMetaType,
                                    m_hashFunction,
                                    m_elementNestedContainerAccess,
                                    m_elementOwnerFunction,
                                    m_elementDataType)
            };
        }
    }
};

template<typename Access, typename Iterator = void,
         AbstractSequentialConstIteratorAccess::IteratorType type = AbstractSequentialConstIteratorAccess::IteratorType::iterator>
class AutoSequentialIteratorAccess : public AutoSequentialConstIteratorAccess<Access,Iterator,type,AbstractSequentialIteratorAccess>{
    using Super = AutoSequentialConstIteratorAccess<Access,Iterator,type,AbstractSequentialIteratorAccess>;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    using Super::m_internalToExternalConverter;
    using Super::m_valueMetaType;
    using Super::m_hashFunction;
    using Super::m_elementNestedContainerAccess;
    using Super::m_elementOwnerFunction;
    using Super::m_elementDataType;
public:
    using Super::Super;
    using Super::value;
    using Super::equals;
    AutoSequentialIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            const QMetaType& valueMetaType,
            const QtJambiUtils::QHashFunction& hashFunction,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction,
            AbstractContainerAccess::DataType elementDataType
        )
        : Super(internalToExternalConverter, valueMetaType, hashFunction, elementNestedContainerAccess, elementOwnerFunction, elementDataType),
        m_externalToInternalConverter(externalToInternalConverter)
    {
    }
    AbstractSequentialIteratorAccess* clone() override{
        return new AutoSequentialIteratorAccess<Access,Iterator>(
                    m_internalToExternalConverter,
                    m_externalToInternalConverter,
                    m_valueMetaType,
                    m_hashFunction,
                    m_elementNestedContainerAccess,
                    m_elementOwnerFunction,
                    m_elementDataType);
    }
    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        void* newval = &*iterator;
        jvalue jval;
        jval.l = newValue;
        m_externalToInternalConverter(env, nullptr, jval, newval, jValueType::l);
    }

    std::optional<void*> value(void* ptr) override {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        return std::make_optional<void*>(&*iterator);
    }
    template<typename T>
    bool setValue(void* ptr, T value) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        if constexpr(std::is_same_v<T,QVariant>){
            if(value.convert(m_valueMetaType)){
                void* pos = &*iterator;
                m_valueMetaType.destruct(pos);
                m_valueMetaType.construct(pos, value.data());
                return true;
            }
        }else{
            QMetaType mt = QMetaType::fromType<T>();
            if(m_valueMetaType==mt){
                void* pos = &*iterator;
                *reinterpret_cast<T*>(pos) = value;
                return true;
            }else if(QMetaType::canConvert(m_valueMetaType,mt)){
                QVariant v = QVariant::fromValue(value);
                if(v.convert(m_valueMetaType)){
                    void* pos = &*iterator;
                    m_valueMetaType.destruct(pos);
                    m_valueMetaType.construct(pos, v.data());
                    return true;
                }
            }
        }
        return false;
    }
    bool setIntValue(void* ptr, jint value) override {
        return setValue(ptr, value);
    }
    bool setLongValue(void* ptr, jlong value) override {
        return setValue(ptr, value);
    }
    bool setShortValue(void* ptr, jshort value) override {
        return setValue(ptr, value);
    }
    bool setByteValue(void* ptr, jbyte value) override {
        return setValue(ptr, value);
    }
    bool setFloatValue(void* ptr, jfloat value) override {
        return setValue(ptr, value);
    }
    bool setDoubleValue(void* ptr, jdouble value) override {
        return setValue(ptr, value);
    }
    bool setCharValue(void* ptr, jchar value) override {
        return setValue(ptr, value);
    }
    bool setBooleanValue(void* ptr, jboolean value) override {
        return setValue(ptr, value);
    }
    bool setVariantValue(void* ptr, const QVariant& value) override {
        return setValue<QVariant>(ptr, value);
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AbstractSpanAccess* createSpanAccess() override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
};

template<typename Access, typename Iterator = void,
         AbstractSequentialConstIteratorAccess::IteratorType type = AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,
         typename SuperType = AbstractAssociativeConstIteratorAccess>
class AutoAssociativeConstIteratorAccess : public AbstractAutoConstIteratorAccess<Access,Iterator,type,SuperType>{
    using Super = AbstractAutoConstIteratorAccess<Access,Iterator,type,SuperType>;
protected:
    using Super::equals;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QMetaType m_valueMetaType;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QMetaType m_keyMetaType;
public:
    AutoAssociativeConstIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            const QMetaType& keyMetaType,
            const QMetaType& valueMetaType)
        : Super(),
        m_valueInternalToExternalConverter(valueInternalToExternalConverter),
        m_valueMetaType(valueMetaType),
        m_keyInternalToExternalConverter(keyInternalToExternalConverter),
        m_keyMetaType(keyMetaType)
    {}
    using Super::value;

    SuperType* clone() override{
        if constexpr(std::is_same_v<SuperType,AbstractAssociativeConstIteratorAccess>){
            return new AutoAssociativeConstIteratorAccess<Access,Iterator,type,SuperType>(
                        m_valueInternalToExternalConverter,
                        m_keyInternalToExternalConverter,
                        m_keyMetaType,
                        m_valueMetaType);
        }else return nullptr;
    }

    jobject value(JNIEnv * env, const void* ptr) override {
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator){
            return key(env, ptr);
        }else{
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            const void* v;
            jvalue jval;
            jval.l = nullptr;
            if constexpr(QtJambiPrivate::supports_key_v<Iterator> && QtJambiPrivate::supports_value_v<Iterator>){
                v = &iterator.value();
            }else{
                v = &((*iterator).second);
            }
            if(m_valueInternalToExternalConverter(env, nullptr, v, jval, true))
                return jval.l;
        }
        return nullptr;
    }
    std::optional<const void*> value(const void* ptr) override {
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator){
            return key(ptr);
        }else{
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            const void* v;
            if constexpr(QtJambiPrivate::supports_key_v<Iterator> && QtJambiPrivate::supports_value_v<Iterator>){
                v = &iterator.value();
            }else{
                v = &((*iterator).second);
            }
            return std::make_optional<const void*>(v);
        }
    }
    jobject key(JNIEnv * env, const void* ptr) override {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const void* v{nullptr};
        jvalue jval;
        jval.l = nullptr;
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator){
            if constexpr(QtJambiPrivate::supports_key_v<const Iterator&>){
                v = &iterator.key();
            } else if constexpr(QtJambiPrivate::supports_deref_v<const Iterator&>){
                v = &*iterator;
            }
        }else if constexpr(QtJambiPrivate::supports_key_v<Iterator> && QtJambiPrivate::supports_value_v<Iterator>){
            v = &iterator.key();
        }else{
            v = &((*iterator).first);
        }
        if(m_keyInternalToExternalConverter(env, nullptr, v, jval, true))
            return jval.l;
        return nullptr;
    }
    std::optional<const void*> key(const void* ptr) override {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const void* v{nullptr};
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator){
            if constexpr(QtJambiPrivate::supports_key_v<const Iterator&>){
                v = &iterator.key();
            } else if constexpr(QtJambiPrivate::supports_deref_v<const Iterator&>){
                v = &*iterator;
            }
        }else if constexpr(QtJambiPrivate::supports_key_v<Iterator> && QtJambiPrivate::supports_value_v<Iterator>){
            v = &iterator.key();
        }else{
            v = &((*iterator).first);
        }
        return std::make_optional<const void*>(v);
    }
    template<typename T>
    std::optional<T> key(const void* ptr) {
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator){
            if constexpr(QtJambiPrivate::supports_key_v<Iterator>){
                if constexpr(QtJambiPrivate::supports_assign_v<T, std::add_lvalue_reference_t<decltype(std::declval<Iterator>().key())>>){
                    const Iterator& iterator = *static_cast<const Iterator*>(ptr);
                    const void* d = &iterator.key();
                    return std::make_optional<T>(*reinterpret_cast<const T*>(d));
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            } else if constexpr(QtJambiPrivate::supports_deref_v<const Iterator&>){
                if constexpr(QtJambiPrivate::supports_assign_v<T, std::add_lvalue_reference_t<decltype(*std::declval<Iterator>())>>){
                    const Iterator& iterator = *static_cast<const Iterator*>(ptr);
                    const void* d = &*iterator;
                    return std::make_optional<T>(*reinterpret_cast<const T*>(d));
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            }else{
                Q_UNUSED(ptr)
                return std::nullopt;
            }
        } else if constexpr(QtJambiPrivate::supports_key_v<Iterator>){
            if constexpr(QtJambiPrivate::supports_assign_v<T, std::add_lvalue_reference_t<decltype(std::declval<Iterator>().key())>>){
                const Iterator& iterator = *static_cast<const Iterator*>(ptr);
                const void* d = &iterator.key();
                return std::make_optional<T>(*reinterpret_cast<const T*>(d));
            }else{
                Q_UNUSED(ptr)
                return std::nullopt;
            }
        }else if constexpr(QtJambiPrivate::supports_assign_v<T, std::add_lvalue_reference_t<decltype((*std::declval<Iterator>()).first)>>){
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            const void* d = &((*iterator).first);
            return std::make_optional<T>(*reinterpret_cast<const T*>(d));
        }else{
            Q_UNUSED(ptr)
            return std::nullopt;
        }
    }
    template<typename T>
    std::optional<T> value(const void* ptr) {
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return key<T>(ptr);
        else if constexpr(QtJambiPrivate::supports_value_v<Iterator>){
            if constexpr(QtJambiPrivate::supports_assign_v<T, std::add_lvalue_reference_t<decltype(std::declval<Iterator>().value())>>){
                const Iterator& iterator = *static_cast<const Iterator*>(ptr);
                const void* d = &iterator.value();
                return std::make_optional<T>(*reinterpret_cast<const T*>(d));
            }else{
                Q_UNUSED(ptr)
                return std::nullopt;
            }
        }else if constexpr(QtJambiPrivate::supports_assign_v<T, std::add_lvalue_reference_t<decltype((*std::declval<Iterator>()).second)>>){
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            const void* d = &((*iterator).second);
            return std::make_optional<T>(*reinterpret_cast<const T*>(d));
        }else{
            Q_UNUSED(ptr)
            return std::nullopt;
        }
    }
    std::optional<jint> intValue(const void* ptr) override {
        return value<jint>(ptr);
    }
    std::optional<jlong> longValue(const void* ptr) override {
        return value<jlong>(ptr);
    }
    std::optional<jshort> shortValue(const void* ptr) override {
        return value<jshort>(ptr);
    }
    std::optional<jbyte> byteValue(const void* ptr) override {
        return value<jbyte>(ptr);
    }
    std::optional<jfloat> floatValue(const void* ptr) override {
        return value<jfloat>(ptr);
    }
    std::optional<jdouble> doubleValue(const void* ptr) override {
        return value<jdouble>(ptr);
    }
    std::optional<jchar> charValue(const void* ptr) override {
        return value<jchar>(ptr);
    }
    std::optional<jboolean> booleanValue(const void* ptr) override {
        return value<jboolean>(ptr);
    }
    QVariant variantValue(const void* ptr) override {
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return variantKey(ptr);
        else{
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            const void* d;
            if constexpr(QtJambiPrivate::supports_value_v<Iterator>){
                d = &iterator.value();
            }else{
                d = &((*iterator).second);
            }
            return QVariant(m_valueMetaType, d);
        }
    }
    std::optional<jint> intKey(const void* ptr) override {
        return key<jint>(ptr);
    }
    std::optional<jlong> longKey(const void* ptr) override {
        return key<jlong>(ptr);
    }
    std::optional<jshort> shortKey(const void* ptr) override {
        return key<jshort>(ptr);
    }
    std::optional<jbyte> byteKey(const void* ptr) override {
        return key<jbyte>(ptr);
    }
    std::optional<jfloat> floatKey(const void* ptr) override {
        return key<jfloat>(ptr);
    }
    std::optional<jdouble> doubleKey(const void* ptr) override {
        return key<jdouble>(ptr);
    }
    std::optional<jchar> charKey(const void* ptr) override {
        return key<jchar>(ptr);
    }
    std::optional<jboolean> booleanKey(const void* ptr) override {
        return key<jboolean>(ptr);
    }
    QVariant variantKey(const void* ptr) override {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const void* d{nullptr};
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator){
            if constexpr(QtJambiPrivate::supports_key_v<const Iterator&>){
                d = &iterator.key();
            } else if constexpr(QtJambiPrivate::supports_deref_v<const Iterator&>){
                d = &*iterator;
            }
        } else if constexpr(QtJambiPrivate::supports_key_v<const Iterator&>){
            d = &iterator.key();
        }else{
            d = &((*iterator).first);
        }
        return QVariant(m_keyMetaType, d);
    }

    const QMetaType& keyMetaType() override{
        return m_keyMetaType;
    }

    const QMetaType& valueMetaType() override{
        if constexpr(type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator){
            return keyMetaType();
        }else{
            return m_valueMetaType;
        }
    }
    jboolean equals(JNIEnv *env, const void* ptr, const ConstContainerAndAccessInfo& ptr2) override{
        return Access::iteratorEquals(env, ptr, type, ptr2);
    }
    void* asIterator(void* iterator) override{
        return Access::asIterator(iterator, type);
    }
    bool findIterator(const void* iterator, const std::type_info& typeId, void* output) override{
        return Access::findIterator(iterator, type, typeId, output);
    }
    std::pair<void*,AbstractSequentialConstIteratorAccess*> createConstIterator(const void* iterator) override {
        if constexpr(std::is_same_v<typename QtJambiPrivate::CreateConstIterator<Iterator,Access>::type,void>){
            return {nullptr,nullptr};
        }else{
            return {
                    QtJambiPrivate::CreateConstIterator<Iterator,Access>::function(iterator),
                    new AutoAssociativeConstIteratorAccess<Access,typename QtJambiPrivate::CreateConstIterator<Iterator,Access>::type,type>(
                                    m_valueInternalToExternalConverter,
                                    m_keyInternalToExternalConverter,
                                    m_keyMetaType,
                                    m_valueMetaType)
            };
        }
    }
};

template<typename Access, typename Iterator = void,
         AbstractSequentialConstIteratorAccess::IteratorType type = AbstractSequentialConstIteratorAccess::IteratorType::iterator>
class AutoAssociativeIteratorAccess : public AutoAssociativeConstIteratorAccess<Access,Iterator,type,AbstractAssociativeIteratorAccess> {
    using Super = AutoAssociativeConstIteratorAccess<Access,Iterator,type,AbstractAssociativeIteratorAccess>;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    using Super::m_valueInternalToExternalConverter;
    using Super::m_keyInternalToExternalConverter;
    using Super::m_keyMetaType;
    using Super::m_valueMetaType;
public:
    AutoAssociativeIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            const QMetaType& keyMetaType,
            const QMetaType& valueMetaType)
        : Super(valueInternalToExternalConverter,keyInternalToExternalConverter,keyMetaType,valueMetaType),
        m_valueExternalToInternalConverter(valueExternalToInternalConverter)
    {}
    using Super::value;
    using Super::equals;
    AbstractAssociativeIteratorAccess* clone() override{
        return new AutoAssociativeIteratorAccess<Access,Iterator,type>(
            m_valueInternalToExternalConverter,
            m_valueExternalToInternalConverter,
            m_keyInternalToExternalConverter,
            m_keyMetaType,
            m_valueMetaType);
    }
    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        void* v;
        if constexpr(QtJambiPrivate::supports_key_v<Iterator> && QtJambiPrivate::supports_value_v<Iterator>){
            v = &iterator.value();
        }else{
            v = &(*iterator).second;
        }
        jvalue jval;
        jval.l = newValue;
        m_valueExternalToInternalConverter(env, nullptr, jval, v, jValueType::l);
    }
    std::optional<void*> value(void* ptr) override {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        void* v;
        if constexpr(QtJambiPrivate::supports_key_v<Iterator> && QtJambiPrivate::supports_value_v<Iterator>){
            v = &iterator.value();
        }else{
            v = &(*iterator).second;
        }
        return std::make_optional<void*>(v);
    }
    template<typename T>
    bool setValue(void* ptr, T value) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        void* pos;
        if constexpr(QtJambiPrivate::supports_key_v<Iterator> && QtJambiPrivate::supports_value_v<Iterator>){
            pos = &iterator.value();
        }else{
            pos = &(*iterator).second;
        }
        if constexpr(std::is_same_v<T,QVariant>){
            if(value.convert(m_valueMetaType)){
                m_valueMetaType.destruct(pos);
                m_valueMetaType.construct(pos, value.data());
                return true;
            }
        }else{
            QMetaType mt = QMetaType::fromType<T>();
            if(m_valueMetaType==mt){
                *reinterpret_cast<T*>(pos) = value;
                return true;
            }else if(QMetaType::canConvert(m_valueMetaType,mt)){
                QVariant v = QVariant::fromValue(value);
                if(v.convert(m_valueMetaType)){
                    m_valueMetaType.destruct(pos);
                    m_valueMetaType.construct(pos, v.data());
                    return true;
                }
            }
        }
        Q_UNUSED(pos)
        return false;
    }
    bool setIntValue(void* ptr, jint value) override {
        return setValue(ptr, value);
    }
    bool setLongValue(void* ptr, jlong value) override {
        return setValue(ptr, value);
    }
    bool setShortValue(void* ptr, jshort value) override {
        return setValue(ptr, value);
    }
    bool setByteValue(void* ptr, jbyte value) override {
        return setValue(ptr, value);
    }
    bool setFloatValue(void* ptr, jfloat value) override {
        return setValue(ptr, value);
    }
    bool setDoubleValue(void* ptr, jdouble value) override {
        return setValue(ptr, value);
    }
    bool setCharValue(void* ptr, jchar value) override {
        return setValue(ptr, value);
    }
    bool setBooleanValue(void* ptr, jboolean value) override {
        return setValue(ptr, value);
    }
    bool setVariantValue(void* ptr, const QVariant& value) override {
        return setValue<QVariant>(ptr, value);
    }
};

template<typename Access, typename Super>
class AutoSequentialConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,Super> : public AbstractAutoContainerIteratorAccess<Super>{
    using Self = AutoSequentialConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,Super>;
public:
    typedef void(*IncrementFn)(Self*,void*);
    typedef void(*DecrementFn)(Self*,void*);
    typedef const void*(*ValueFn)(Self*,const void*);
    typedef bool(*LessThanFn)(Self*,const void*,const void*);
    typedef bool(*EqualsFn)(Self*,const void*,const void*);
    typedef bool(*IsBeginFn)(Self*,const void*);
    typedef bool(*IsEndFn)(Self*,const void*);
    typedef bool(*IsValidFn)(Self*,const void*);
    typedef void*(*CloneFn)(Self*,void*,const void*);
protected:
    size_t m_sizeOf;
    size_t m_alignOf;
    CloneFn m_clone;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    IncrementFn m_increment;
    DecrementFn m_decrement;
    ValueFn m_value;
    LessThanFn m_lessThan;
    EqualsFn m_equals;
    IsBeginFn m_isBegin;
    IsEndFn m_isEnd;
    IsValidFn m_isValid;
    QMetaType m_valueMetaType;
public:
    using Super::value;
    ~AutoSequentialConstIteratorAccess() override = default;
    AutoSequentialConstIteratorAccess(
            size_t sizeOf,
            size_t alignOf,
            CloneFn clone,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            IsBeginFn isBegin,
            IsEndFn isEnd,
            IsValidFn isValid,
            const QMetaType& valueMetaType
        ) : Super(),
        m_sizeOf(sizeOf),
        m_alignOf(alignOf),
        m_clone(clone),
        m_internalToExternalConverter(internalToExternalConverter),
        m_increment(increment),
        m_decrement(decrement),
        m_value(value),
        m_lessThan(lessThan),
        m_equals(equals),
        m_isBegin(isBegin),
        m_isEnd(isEnd),
        m_isValid(isValid),
        m_valueMetaType(valueMetaType)
    {
        Q_ASSERT(m_value);
    }
    void dispose() override  {delete this;}
    Super* clone() override{
        if constexpr(std::is_same_v<Super,AbstractSequentialConstIteratorAccess>){
            return new Self(
                        m_sizeOf,
                        m_alignOf,
                        m_clone,
                        m_internalToExternalConverter,
                        m_increment,
                        m_decrement,
                        m_value,
                        m_lessThan,
                        m_equals,
                        m_isBegin,
                        m_isEnd,
                        m_isValid,
                        m_valueMetaType);
        }else return nullptr;
    }
    jobject value(JNIEnv * env, const void* iterator) override{
        const void* v = m_value(this, iterator);
        jvalue jval;
        jval.l = nullptr;
        if(m_internalToExternalConverter(env, nullptr, v, jval, true))
            return jval.l;
        return nullptr;
    }
    std::optional<const void*> value(const void* iterator) override{
        return std::make_optional<const void*>(m_value(this, iterator));
    }
    void increment(JNIEnv * env, void* iterator) override{
        if(m_increment)
            m_increment(this, iterator);
        else
            JavaException::raiseUnsupportedOperationException(env, "Iterator::operator++()" QTJAMBI_STACKTRACEINFO );
    }
    void increment(void* iterator) override{
        if(m_increment)
            m_increment(this, iterator);
    }
    void decrement(JNIEnv * env, void* iterator) override{
        if(m_decrement)
            m_decrement(this, iterator);
        else
            JavaException::raiseUnsupportedOperationException(env, "Iterator::operator--()" QTJAMBI_STACKTRACEINFO );
    }
    void decrement(void* iterator) override{
        if(m_decrement)
            m_decrement(this, iterator);
    }
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override{
        if(m_lessThan)
            return m_lessThan(this, iterator, other);
        JavaException::raiseUnsupportedOperationException(env, "Iterator::operator<(Iterator)" QTJAMBI_STACKTRACEINFO );
    }
    std::optional<bool> lessThan(const void* iterator, const void* other) override{
        if(m_lessThan)
            return std::make_optional<bool>(m_lessThan(this, iterator, other));
        return std::nullopt;
    }
    std::optional<size_t> distance(const void* ptr, const void* ptr2) override {
        return std::nullopt;
    }
    bool canLess() override{
        return m_lessThan;
    }
    bool canDistance() override{
        return false;
    }
    bool isBidirectionalIterator() override{
        return m_decrement;
    }
    bool isContiguousIterator() override{
        return false;
    }
    bool isRandomAccessIterator() override{
        return false;
    }
    bool isBegin(JNIEnv *, const void*iterator) override{
        return m_isBegin && m_isBegin(this, iterator);
    }
    std::optional<bool> isBegin(const void*iterator) override{
        if(m_isBegin)
            return m_isBegin(this, iterator);
        return std::nullopt;
    }
    bool isEnd(JNIEnv *, const void*iterator) override{
        return m_isEnd && m_isEnd(this, iterator);
    }
    std::optional<bool> isEnd(const void*iterator) override{
        if(m_isEnd)
            return m_isEnd(this, iterator);
        return std::nullopt;
    }
    bool isValid(JNIEnv *, const void*iterator) override{
        return m_isValid && m_isValid(this, iterator);
    }
    std::optional<bool> isValid(const void*iterator) override{
        if(m_isValid)
            return m_isValid(this, iterator);
        return std::nullopt;
    }
    jboolean equals(JNIEnv *, const void* iterator, const void* other) override{
        return m_equals(this, iterator, other);
    }
    bool equals(const void* iterator, const void* other) override{
        return m_equals(this, iterator, other);
    }
    jboolean equals(JNIEnv *env, const void* ptr, const ConstContainerAndAccessInfo& ptr2) override{
        return equals(env, ptr, ptr2.container);
    }
    const QMetaType& valueMetaType() override{
        return m_valueMetaType;
    }
    size_t sizeOf() const override {return m_sizeOf;}
    size_t alignOf() const override {return m_alignOf;}
    void* constructContainer(void* placement, const void* copyOf) override{
        return m_clone(this, placement, copyOf);
    }
    bool canCopy() const override {return m_clone;}
private:
    Q_DISABLE_COPY_MOVE(AutoSequentialConstIteratorAccess)
};

template<typename Access>
class AutoSequentialIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator>
    : public AutoSequentialConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator,AbstractSequentialIteratorAccess>{
    using Super = AutoSequentialConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator,AbstractSequentialIteratorAccess>;
    using Self = AutoSequentialIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator>;
public:
    using Super::value;
    typedef void(*IncrementFn)(Self*,void*);
    typedef void(*DecrementFn)(Self*,void*);
    typedef const void*(*ValueFn)(Self*,const void*);
    typedef bool(*LessThanFn)(Self*,const void*,const void*);
    typedef bool(*EqualsFn)(Self*,const void*,const void*);
    typedef bool(*IsBeginFn)(Self*,const void*);
    typedef bool(*IsEndFn)(Self*,const void*);
    typedef bool(*IsValidFn)(Self*,const void*);
    typedef void*(*SetValueFn)(Self*,void*);
    typedef void*(*CloneFn)(Self*,void*,const void*);
    ~AutoSequentialIteratorAccess() override = default;
    AutoSequentialIteratorAccess(
            size_t sizeOf,
            size_t alignOf,
            CloneFn clone,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            IsBeginFn isBegin,
            IsEndFn isEnd,
            IsValidFn isValid,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            SetValueFn setValue,
            const QMetaType& valueMetaType
        )    : Super(sizeOf, alignOf,
              Super::CloneFn(clone),
              internalToExternalConverter,
              Super::IncrementFn(increment),
              Super::DecrementFn(decrement),
              Super::ValueFn(value),
              Super::LessThanFn(lessThan),
              Super::EqualsFn(equals),
              Super::IsBeginFn(isBegin),
              Super::IsEndFn(isEnd),
              Super::IsValidFn(isValid),
              valueMetaType),
        m_externalToInternalConverter(externalToInternalConverter),
        m_setValue(setValue)
    {
        Q_ASSERT(Super::m_value);
        Q_ASSERT(m_setValue);
    }
    void setValue(JNIEnv * env, void* iterator, jobject newValue) override{
        void* newval = m_setValue(this, iterator);
        jvalue jval;
        jval.l = newValue;
        m_externalToInternalConverter(env, nullptr, jval, newval, jValueType::l);
    }
    std::optional<void*> value(void* iterator) override{
        return std::make_optional<void*>(m_setValue(this, iterator));
    }
    Self* clone() override{
        return new Self(
                    Super::m_sizeOf,
                    Super::m_alignOf,
                    CloneFn(Super::m_clone),
                    Super::m_internalToExternalConverter,
                    IncrementFn(Super::m_increment),
                    DecrementFn(Super::m_decrement),
                    ValueFn(Super::m_value),
                    LessThanFn(Super::m_lessThan),
                    EqualsFn(Super::m_equals),
                    IsBeginFn(Super::m_isBegin),
                    IsEndFn(Super::m_isEnd),
                    IsValidFn(Super::m_isValid),
                    m_externalToInternalConverter,
                    m_setValue,
                    Super::m_valueMetaType);
    }
private:
    Q_DISABLE_COPY_MOVE(AutoSequentialIteratorAccess)
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    SetValueFn m_setValue;
};

template<typename Access, typename Super>
class AutoAssociativeConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,Super> : public AutoSequentialConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,Super>{
    using Self = AutoAssociativeConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,Super>;
    using SuperType = AutoSequentialConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::const_iterator,Super>;
public:
    typedef void(*IncrementFn)(Self*,void*);
    typedef void(*DecrementFn)(Self*,void*);
    typedef const void*(*ValueFn)(Self*,const void*);
    typedef bool(*LessThanFn)(Self*,const void*,const void*);
    typedef bool(*EqualsFn)(Self*,const void*,const void*);
    typedef bool(*IsBeginFn)(Self*,const void*);
    typedef bool(*IsEndFn)(Self*,const void*);
    typedef bool(*IsValidFn)(Self*,const void*);
    typedef const void*(*KeyFn)(Self*,const void*);
    typedef void*(*CloneFn)(Self*,void*,const void*);
protected:
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    KeyFn m_key;
    QMetaType m_keyMetaType;
public:
    using SuperType::value;
    ~AutoAssociativeConstIteratorAccess() override = default;
    AutoAssociativeConstIteratorAccess(
            size_t sizeOf,
            size_t alignOf,
            CloneFn clone,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            IsBeginFn isBegin,
            IsEndFn isEnd,
            IsValidFn isValid,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            KeyFn key,
            const QMetaType& keyMetaType,
            const QMetaType& valueMetaType
        ) : SuperType(sizeOf,
                                                         alignOf,
                                                         typename SuperType::CloneFn(clone),
                                                         internalToExternalConverter,
                                                         typename SuperType::IncrementFn(increment),
                                                         typename SuperType::DecrementFn(decrement),
                                                         typename SuperType::ValueFn(value),
                                                         typename SuperType::LessThanFn(lessThan),
                                                         typename SuperType::EqualsFn(equals),
                                                         typename SuperType::IsBeginFn(isBegin),
                                                         typename SuperType::IsEndFn(isEnd),
                                                         typename SuperType::IsValidFn(isValid),
                                                         valueMetaType),
        m_keyInternalToExternalConverter(keyInternalToExternalConverter),
        m_key(std::move(key)),
        m_keyMetaType(keyMetaType)
    {
        Q_ASSERT(m_key);
    }
    Self* clone() override{
        if constexpr(std::is_same_v<Super,AbstractAssociativeConstIteratorAccess>){
            return new Self(
                        Super::m_sizeOf,
                        Super::m_alignOf,
                        CloneFn(Super::m_clone),
                        Super::m_internalToExternalConverter,
                        IncrementFn(Super::m_increment),
                        DecrementFn(Super::m_decrement),
                        ValueFn(Super::m_value),
                        LessThanFn(Super::m_lessThan),
                        EqualsFn(Super::m_equals),
                        IsBeginFn(Super::m_isBegin),
                        IsEndFn(Super::m_isEnd),
                        IsValidFn(Super::m_isValid),
                        m_keyInternalToExternalConverter,
                        m_key,
                        m_keyMetaType,
                        Super::m_valueMetaType);
        }else return nullptr;
    }
    jobject key(JNIEnv * env, const void* iterator) override{
        const void* v = m_key(this, iterator);
        jvalue jval;
        jval.l = nullptr;
        if(m_keyInternalToExternalConverter(env, nullptr, v, jval, true))
            return jval.l;
        return nullptr;
    }
    std::optional<const void*> key(const void* iterator) override{
        return std::make_optional<const void*>(m_key(this, iterator));
    }
    const QMetaType& keyMetaType() override{
        return m_keyMetaType;
    }
private:
    Q_DISABLE_COPY_MOVE(AutoAssociativeConstIteratorAccess)
};

template<typename Access>
class AutoAssociativeIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator> : public AutoAssociativeConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator,AbstractAssociativeIteratorAccess>{
    using Self = AutoAssociativeIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator>;
    using Super = AutoAssociativeConstIteratorAccess<Access,void,AbstractSequentialConstIteratorAccess::IteratorType::iterator,AbstractAssociativeIteratorAccess>;
    typedef void(*IncrementFn)(Self*,void*);
    typedef void(*DecrementFn)(Self*,void*);
    typedef const void*(*ValueFn)(Self*,const void*);
    typedef bool(*LessThanFn)(Self*,const void*,const void*);
    typedef bool(*EqualsFn)(Self*,const void*,const void*);
    typedef bool(*IsBeginFn)(Self*,const void*);
    typedef bool(*IsEndFn)(Self*,const void*);
    typedef bool(*IsValidFn)(Self*,const void*);
    typedef void*(*SetValueFn)(Self*,void*);
    typedef const void*(*KeyFn)(Self*,const void*);
    typedef void*(*CloneFn)(Self*,void*,const void*);
public:
    using Super::value;
    ~AutoAssociativeIteratorAccess() override = default;
    AutoAssociativeIteratorAccess(
            size_t sizeOf,
            size_t alignOf,
            CloneFn clone,
            const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            IsBeginFn isBegin,
            IsEndFn isEnd,
            IsValidFn isValid,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            KeyFn key,
            const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
            SetValueFn setValue,
            const QMetaType& keyMetaType,
            const QMetaType& valueMetaType
            ) : Super(
              sizeOf, alignOf,
              Super::CloneFn(clone),
              valueInternalToExternalConverter,
              Super::IncrementFn(increment),
              Super::DecrementFn(decrement),
              Super::ValueFn(value),
              Super::LessThanFn(lessThan),
              Super::EqualsFn(equals),
              Super::IsBeginFn(isBegin),
              Super::IsEndFn(isEnd),
              Super::IsValidFn(isValid),
              keyInternalToExternalConverter,
              Super::KeyFn(key),
              keyMetaType,
              valueMetaType),
        m_valueExternalToInternalConverter(valueExternalToInternalConverter),
        m_setValue(setValue)
    {
        Q_ASSERT(setValue);
    }
    void setValue(JNIEnv * env, void* iterator, jobject newValue) override{
        void* newval = m_setValue(this, iterator);
        jvalue jval;
        jval.l = newValue;
        m_valueExternalToInternalConverter(env, nullptr, jval, newval, jValueType::l);
    }
    std::optional<void*> value(void* iterator) override{
        return std::make_optional<void*>(m_setValue(this, iterator));
    }
    Self* clone() override{
        return new Self(
                    Super::m_sizeOf,
                    Super::m_alignOf,
                    CloneFn(Super::m_clone),
                    Super::m_internalToExternalConverter,
                    IncrementFn(Super::m_increment),
                    DecrementFn(Super::m_decrement),
                    ValueFn(Super::m_value),
                    LessThanFn(Super::m_lessThan),
                    EqualsFn(Super::m_equals),
                    IsBeginFn(Super::m_isBegin),
                    IsEndFn(Super::m_isEnd),
                    IsValidFn(Super::m_isValid),
                    Super::m_keyInternalToExternalConverter,
                    KeyFn(Super::m_key),
                    m_valueExternalToInternalConverter,
                    m_setValue,
                    Super::m_keyMetaType,
                    Super::m_valueMetaType);
    }
private:
    Q_DISABLE_COPY_MOVE(AutoAssociativeIteratorAccess)
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    SetValueFn m_setValue;
};

class AutoListAccess;
class AutoMapAccess;
class AutoHashAccess;

namespace QtJambiPrivate{

template<typename Container, typename Iterator, typename Storage, bool rvalue, bool sequential, bool isMutable, typename... Args>
struct qtjambi_ContainerIterator_cast;

template<typename Access, typename Iterator, bool rvalue, bool sequential, bool isMutable, typename... Args>
struct qtjambi_ContainerIterator_cast<ContainerAccessLink<Access>, Iterator, ContainerAccessLink<Access>, rvalue, sequential, isMutable, Args...>{
    using iterator_type = ContainerIterator<ContainerAccessLink<Access>,Iterator>;
    using In = std::conditional_t<rvalue,iterator_type&&,const iterator_type&>;
    static jobject cast(In iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        QSharedPointer<QtJambiLink> owner = iter.storage().link();
        Access* access = iter.storage().containerAccess();
        if constexpr(sequential){
            if constexpr(isMutable){
                return QtJambiPrivate::convertIteratorToJavaObject(env,
                                                                   owner,
                                                                   new iterator_type(std::move(iter)),
                                                                   &QtJambiAPI::deletePointer<iterator_type>,
                                                                   new AutoSequentialIteratorAccess<Access,iterator_type,IteratorTypeDecider<iterator_type,isMutable>::type>(
                                                                                                                        access->m_internalToExternalConverter,
                                                                                                                        access->m_externalToInternalConverter,
                                                                                                                        access->m_valueMetaType,
                                                                                                                        access->m_hashFunction,
                                                                                                                        access->m_elementNestedContainerAccess,
                                                                                                                        access->m_elementOwnerFunction,
                                                                                                                        access->m_elementDataType)
                                                                   );
            }else{
                return QtJambiPrivate::convertIteratorToJavaObject(env,
                                                                   owner,
                                                                   new iterator_type(std::move(iter)),
                                                                   &QtJambiAPI::deletePointer<iterator_type>,
                                                                   new AutoSequentialConstIteratorAccess<Access,iterator_type,IteratorTypeDecider<iterator_type,isMutable>::type,AbstractSequentialConstIteratorAccess>(
                                                                                                                        access->m_internalToExternalConverter,
                                                                                                                        access->m_valueMetaType,
                                                                                                                        access->m_hashFunction,
                                                                                                                        access->m_elementNestedContainerAccess,
                                                                                                                        access->m_elementOwnerFunction,
                                                                                                                        access->m_elementDataType)
                                                                   );
            }
        }else{
            if constexpr(isMutable){
                return QtJambiPrivate::convertIteratorToJavaObject(env,
                                                                   owner,
                                                                   new iterator_type(std::move(iter)),
                                                                   &QtJambiAPI::deletePointer<iterator_type>,
                                                                   new AutoAssociativeIteratorAccess<Access,iterator_type,IteratorTypeDecider<iterator_type,isMutable>::type>(
                                                                       access->m_valueInternalToExternalConverter,
                                                                       access->m_valueExternalToInternalConverter,
                                                                       access->m_keyInternalToExternalConverter,
                                                                       access->m_keyMetaType,
                                                                       access->m_valueMetaType)
                                                                   );
            }else{
                return QtJambiPrivate::convertIteratorToJavaObject(env,
                                                                   owner,
                                                                   new iterator_type(std::move(iter)),
                                                                   &QtJambiAPI::deletePointer<iterator_type>,
                                                                   new AutoAssociativeConstIteratorAccess<Access,iterator_type,IteratorTypeDecider<iterator_type,isMutable>::type,AbstractAssociativeConstIteratorAccess>(
                                                                        access->m_valueInternalToExternalConverter,
                                                                        access->m_keyInternalToExternalConverter,
                                                                        access->m_keyMetaType,
                                                                        access->m_valueMetaType)
                                                                   );
            }
        }
    }
};

struct ConstRef{
    const void *ptr;
    const QtPrivate::QMetaTypeInterface *iface;
};

struct Ref{
    Ref();
    const QtPrivate::QMetaTypeInterface *iface;
    mutable void *ptr;
private:
    static thread_local QList<std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *>> refMetaTypes;
    friend AutoListAccess;
    friend AutoMapAccess;
    friend AutoHashAccess;
    friend struct SecondRef;
    template<typename Access>
    friend struct AbstractMutableContainer;
protected:
    Ref(const QtPrivate::QMetaTypeInterface *_iface);
};

struct SecondRef : Ref{
    SecondRef();
};

template<typename Access, typename Container = const void*>
struct AbstractContainerBase{
    Container container;
    Access* access;
    AbstractContainerBase(Container _container, Access* _access) : container(_container), access(_access) {
    }
    qint64 size() const{
        return access->size(container);
    }
    Q_DISABLE_COPY_MOVE(AbstractContainerBase)
};

template<typename Access>
struct AbstractMutableContainer : AbstractContainerBase<Access, void*>{
    AbstractMutableContainer(void* _container, Access* _access) : AbstractContainerBase<Access, void*>{_container, _access} {
        QtJambiPrivate::Ref::refMetaTypes.append(access->metaTypes());
    }
    ~AbstractMutableContainer(){
        QtJambiPrivate::Ref::refMetaTypes.takeLast();
    }
    using AbstractContainerBase<Access, void*>::access;
    using AbstractContainerBase<Access, void*>::container;
    void clear(){
        access->clear(container);
    }
};

template<typename Access>
struct ConstAssociativeContainer : AbstractContainerBase<Access>{
    using AbstractContainerBase<Access>::AbstractContainerBase;
    using AbstractContainerBase<Access>::access;
    using AbstractContainerBase<Access>::container;
    typedef QtJambiPrivate::Ref key_type;
    typedef QtJambiPrivate::SecondRef mapped_type;
    struct ConstIterator{
        typedef typename Access::const_iterator const_iterator;
        const_iterator iter;
        Access* access;
        ConstIterator(Access* _access) : iter(), access(_access) {}
        ConstIterator(const const_iterator& _i, Access* _access) : iter(_i), access(_access) {}
        ConstIterator(const ConstIterator& iter) : iter(iter.iter), access(iter.access) {}
        using iterator_category = std::forward_iterator_tag;
        using value_type      = QtJambiPrivate::ConstRef;
        using difference_type = qsizetype;
        using pointer         = QtJambiPrivate::ConstRef*;
        using reference       = QtJambiPrivate::ConstRef&;
        ConstIterator& operator=(const ConstIterator& o) {
            iter = o.iter;
            access = o.access;
            return *this;
        }
        ConstIterator& operator++() noexcept {
            ++iter;
            return *this;
        }
        ConstIterator operator++(int) noexcept {
            ConstIterator _this = *this;
            ++iter;
            return _this;
        }
        bool operator==(ConstIterator other) const noexcept{
            return iter==other.iter;
        }
        bool operator!=(ConstIterator other) const noexcept { return !(*this == other); }
        QtJambiPrivate::ConstRef key()const{
            return QtJambiPrivate::ConstRef{&iter.key(), access->metaTypes().first};
        }
        QtJambiPrivate::ConstRef value()const{
            return QtJambiPrivate::ConstRef{&iter.value(), access->metaTypes().second};
        }
        QtJambiPrivate::ConstRef operator*()const {return key();}
    };
    typedef ConstIterator const_iterator;
    ConstIterator constBegin() const{
        return ConstIterator(access->constBegin(container), access);
    }
    ConstIterator constEnd() const{
        return ConstIterator(access->constEnd(container), access);
    }
    ConstIterator begin() const {return constBegin();}
    ConstIterator end() const {return constEnd();}
};

template<typename Access>
struct AssociativeContainer : AbstractMutableContainer<Access>{
    using AbstractMutableContainer<Access>::AbstractMutableContainer;
    using AbstractMutableContainer<Access>::access;
    using AbstractMutableContainer<Access>::container;
    typedef QtJambiPrivate::Ref key_type;
    typedef QtJambiPrivate::SecondRef mapped_type;
    void insert(const QtJambiPrivate::Ref& key, const QtJambiPrivate::SecondRef& value){
        access->insert(container, key.ptr, value.ptr);
    }
};

template<typename Access>
struct ConstSequentialContainer : AbstractContainerBase<Access>{
    using AbstractContainerBase<Access>::AbstractContainerBase;
    using AbstractContainerBase<Access>::access;
    using AbstractContainerBase<Access>::container;
    typedef QtJambiPrivate::ConstRef value_type;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = qsizetype;
    using difference_type = qptrdiff;

    struct ConstIterator{
        using iterator_category = std::forward_iterator_tag;
        using value_type      = QtJambiPrivate::ConstRef;
        using difference_type = qsizetype;
        using pointer         = QtJambiPrivate::ConstRef*;
        using reference       = QtJambiPrivate::ConstRef&;
        typename Access::const_iterator iter;
        Access* access;
        ConstIterator(typename Access::const_iterator _i, Access* _access) : iter(_i), access(_access) {}

        ConstIterator(const ConstIterator& iter) : iter(iter.iter), access(iter.access) {}

        ConstIterator& operator=(const ConstIterator& o) {
            iter = o.iter;
            access = o.access;
            return *this;
        }
        ConstIterator& operator++() noexcept{
            ++iter;
            return *this;
        }
        ConstIterator operator++(int) noexcept{
            ConstIterator result = this;
            ++iter;
            return result;
        }
        bool operator==(ConstIterator other) const noexcept{
            return access == other.access && iter == other.iter;
        }
        inline bool operator!=(ConstIterator other) const noexcept { return !(*this == other); }
        QtJambiPrivate::ConstRef operator*()const{
            return QtJambiPrivate::ConstRef{&*iter, access->metaTypes().first};
        }
    };
    typedef ConstIterator const_iterator;
    ConstIterator constBegin() const{
        return ConstIterator(access->constBegin(container), access);
    }
    ConstIterator constEnd() const{
        return ConstIterator(access->constEnd(container), access);
    }
    ConstIterator begin() const {return constBegin();}
    ConstIterator end() const {return constEnd();}
};

template<typename Access>
struct SequentialContainer : AbstractMutableContainer<Access>{
    using AbstractMutableContainer<Access>::AbstractMutableContainer;
    using AbstractMutableContainer<Access>::access;
    using AbstractMutableContainer<Access>::container;
    void append(const QtJambiPrivate::Ref& value){
        access->append(container, value.ptr);
    }
    void reserve(qsizetype size){
        access->reserve(container, size);
    }
    SequentialContainer& operator<<(const QtJambiPrivate::Ref& value){
        access->insert(container, value.ptr);
        return *this;
    }
    typedef QtJambiPrivate::Ref value_type;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = qsizetype;
    using difference_type = qptrdiff;
};

QDataStream& operator<<(QDataStream& stream, const ConstRef& ref);
QDebug& operator<<(QDebug& sbg, const ConstRef& ref);
QDataStream& operator>>(QDataStream& s, Ref& ref);
bool operator==(const ConstRef& ref1, const ConstRef& ref2);

}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
class AutoSpanAccess : public AbstractSpanAccess, public AbstractNestedSequentialAccess {
    QMetaType m_elementMetaType;
    QtJambiUtils::QHashFunction m_hashFunction;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    size_t m_offset;
    PtrOwnerFunction m_elementOwnerFunction;
    AbstractContainerAccess::DataType m_elementDataType;
    template<typename, typename, typename, bool, bool, bool, typename...>
    friend struct qtjambi_ContainerIterator_cast;
public:
    struct iterator;
    struct const_iterator{
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type      = const char&;
        using difference_type = qsizetype;
        using pointer         = const char*;
        using reference       = const char&;

        const_iterator(size_t _offset, const char* _ptr = nullptr);
        const_iterator(const_iterator&&) = default;
        const_iterator(const const_iterator&) = default;
        const_iterator(const iterator&);
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
        const_iterator& operator+=(size_t);
        const_iterator& operator-=(size_t);
        bool operator<(const const_iterator& right) const;
        bool operator>(const const_iterator& right) const;
        bool operator<=(const const_iterator& right) const;
        bool operator>=(const const_iterator& right) const;
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const { return !operator==(right);}
        bool operator<(const iterator& right) const;
        bool operator>(const iterator& right) const;
        bool operator<=(const iterator& right) const;
        bool operator>=(const iterator& right) const;
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const { return !operator==(right);}
        const char* operator->() const;
        const char& operator*() const;
        const char& operator[](qsizetype j) const;
        qsizetype operator-(const const_iterator& j) const;
        qsizetype operator-(const iterator& j) const;
        const char* data() const;
    private:
        size_t offset;
        const char* ptr;
        friend iterator;
    };
    struct iterator{
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type      = char&;
        using difference_type = qsizetype;
        using pointer         = char*;
        using reference       = char&;

        iterator(size_t _offset, char* _ptr = nullptr);
        iterator(iterator&&) = default;
        iterator(const iterator&) = default;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        iterator& operator+=(size_t);
        iterator& operator-=(size_t);
        bool operator<(const iterator& right) const;
        bool operator>(const iterator& right) const;
        bool operator<=(const iterator& right) const;
        bool operator>=(const iterator& right) const;
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const { return !operator==(right);}
        bool operator<(const const_iterator& right) const;
        bool operator>(const const_iterator& right) const;
        bool operator<=(const const_iterator& right) const;
        bool operator>=(const const_iterator& right) const;
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const { return !operator==(right);}
        const char* operator->() const;
        const char& operator*() const;
        const char& operator[](qsizetype j) const;
        qsizetype operator-(const const_iterator& j) const;
        qsizetype operator-(const iterator& j) const;
        const char* data() const;
        char* operator->();
        char& operator*();
        char* data();
    private:
        size_t offset;
        char* ptr;
        friend const_iterator;
    };
    const_iterator begin(const void* container);
    const_iterator end(const void* container);
    inline const_iterator constBegin(const void* container) {return begin(container);}
    inline const_iterator constEnd(const void* container) {return end(container);}
    iterator begin(void* container);
    iterator end(void* container);
    inline std::reverse_iterator<const_iterator> reverseBegin(const void* container) {return std::reverse_iterator<const_iterator>{end(container)};}
    inline std::reverse_iterator<const_iterator> reverseEnd(const void* container) {return std::reverse_iterator<const_iterator>{begin(container)};}
    inline std::reverse_iterator<const_iterator> constReverseBegin(const void* container) {return reverseBegin(container);}
    inline std::reverse_iterator<const_iterator> constReverseEnd(const void* container) {return reverseEnd(container);}
    inline std::reverse_iterator<iterator> reverseBegin(void* container) {return std::reverse_iterator<iterator>{end(container)};}
    inline std::reverse_iterator<iterator> reverseEnd(void* container) {return std::reverse_iterator<iterator>{begin(container)};}
protected:
    AutoSpanAccess(const AutoSpanAccess& other);
public:
    AutoSpanAccess(
        const QMetaType& elementMetaType,
        const QtJambiUtils::QHashFunction& hashFunction,
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
        PtrOwnerFunction elementOwnerFunction,
        AbstractContainerAccess::DataType elementDataType
    );
    AutoSpanAccess(
        const QMetaType& elementMetaType,
        const QtJambiUtils::QHashFunction& hashFunction,
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
        PtrOwnerFunction elementOwnerFunction,
        AbstractContainerAccess::DataType elementDataType
        );
    void dispose() override;
    AutoSpanAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    static jboolean iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2);
    static void* asIterator(void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType);
    static bool findIterator(const void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const std::type_info& typeId, void* output);
private:
    AbstractNestedSequentialAccess* asNested() override;
    bool equals(const void* p1, const void* p2);
    void debugStream(QDebug &s, const void *ptr);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(QMetaType newMetaType);
    qsizetype size(const void* container) override;
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoSpanAccess>, const_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoSpanAccess>, iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoSpanAccess>, std::reverse_iterator<const_iterator>>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoSpanAccess>, std::reverse_iterator<iterator>>&& iterator);
public:
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    QMetaType registerContainer(QByteArrayView typeName) override;
    bool isConst() override;
    qsizetype size(JNIEnv * env, const void* container) override;
    qsizetype size_bytes(JNIEnv * env, const void* container) override;
    jobject get(JNIEnv *,const void*,qsizetype) override;
    bool set(JNIEnv *,const ContainerInfo&,qsizetype,jobject) override;
    const void* get(const void*,qsizetype) override;
    bool set(void*,qsizetype,const void*) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constReverseBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constReverseEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject reverseBegin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject reverseEnd(JNIEnv * env, const ExtendedContainerInfo& container) override;
    std::unique_ptr<AbstractSpanAccess::ElementIterator> elementIterator(void* container) override;
    std::unique_ptr<AbstractSpanAccess::ElementIterator> elementIterator(const void* container) override;

    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
};

template<typename Access, typename Iterator, AbstractSequentialConstIteratorAccess::IteratorType type, typename SuperType>
AbstractSpanAccess* AutoSequentialConstIteratorAccess<Access,Iterator,type,SuperType>::createSpanAccess(){
    return new AutoSpanAccess(
                            m_valueMetaType,
                            m_hashFunction,
                            m_internalToExternalConverter,
                            m_elementNestedContainerAccess,
                            m_elementOwnerFunction,
                            m_elementDataType
                        );
}
template<typename Access, typename Iterator, AbstractSequentialConstIteratorAccess::IteratorType type>
AbstractSpanAccess* AutoSequentialIteratorAccess<Access,Iterator,type>::createSpanAccess(){
    return new AutoSpanAccess(
                            m_valueMetaType,
                            m_hashFunction,
                            m_internalToExternalConverter,
                            m_externalToInternalConverter,
                            m_elementNestedContainerAccess,
                            m_elementOwnerFunction,
                            m_elementDataType
                        );
}


class PointerRCAutoSpanAccess : public AutoSpanAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoSpanAccess(PointerRCAutoSpanAccess& _this);
public:
    using AutoSpanAccess::AutoSpanAccess;
    PointerRCAutoSpanAccess* clone() override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    bool set(JNIEnv *,const ContainerInfo&,qsizetype,jobject) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class NestedPointersRCAutoSpanAccess : public AutoSpanAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoSpanAccess(NestedPointersRCAutoSpanAccess& _this);
public:
    using AutoSpanAccess::AutoSpanAccess;
    NestedPointersRCAutoSpanAccess* clone() override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    bool set(JNIEnv *,const ContainerInfo&,qsizetype,jobject) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

class AutoListAccess : public AbstractListAccess, public AbstractNestedSequentialAccess {
    typedef QArrayDataPointer<char> QListData;
    QMetaType m_elementMetaType;
    QtJambiUtils::QHashFunction m_hashFunction;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    size_t m_offset;
    PtrOwnerFunction m_elementOwnerFunction;
    AbstractContainerAccess::DataType m_elementDataType;
    template<typename, typename, typename, bool, bool, bool, typename...>
    friend struct qtjambi_ContainerIterator_cast;
protected:
    AutoListAccess(const AutoListAccess& other);
public:
    AutoListAccess(
            const QMetaType& elementMetaType,
            const QtJambiUtils::QHashFunction& hashFunction,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction,
            AbstractContainerAccess::DataType elementDataType
            );
    void dispose() override;
    static jboolean iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2);
    static void* asIterator(void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType);
    static bool findIterator(const void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const std::type_info& typeId, void* output);
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    AutoListAccess* clone() override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AutoSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    struct iterator;
    struct const_iterator{
        using iterator_category = std::random_access_iterator_tag;
        using value_type      = const char&;
        using difference_type = qsizetype;
        using pointer         = const char*;
        using reference       = const char&;

        const_iterator(size_t _offset, const char* _ptr = nullptr);
        const_iterator(const const_iterator&) = default;
        const_iterator(const_iterator&&) = default;
        const_iterator(const iterator&);
        const_iterator& operator=(const const_iterator&) = default;
        const_iterator& operator=(const_iterator&&) = default;
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
        bool operator<(const const_iterator& right) const;
        bool operator>(const const_iterator& right) const;
        bool operator==(const const_iterator& right) const;
        bool operator<=(const const_iterator& right) const;
        bool operator>=(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const { return !operator==(right);}
        bool operator<(const iterator& right) const;
        bool operator>(const iterator& right) const;
        bool operator<=(const iterator& right) const;
        bool operator>=(const iterator& right) const;
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const { return !operator==(right);}
        const char* operator->() const;
        const char& operator*() const;
        const char&operator[](qsizetype j) const;
        const_iterator& operator+=(size_t n);
        const_iterator& operator-=(size_t n);
        qsizetype operator-(const const_iterator& j) const;
        qsizetype operator-(const iterator& j) const;
        const char* data() const;
    private:
        size_t offset;
        const char* ptr;
        friend iterator;
    };
    struct iterator{
        using iterator_category = std::random_access_iterator_tag;
        using value_type      = char&;
        using difference_type = qsizetype;
        using pointer         = char*;
        using reference       = char&;

        iterator(size_t _offset, char* _ptr = nullptr);
        iterator(const iterator&) = default;
        iterator(iterator&&) = default;
        iterator& operator=(const iterator&) = default;
        iterator& operator=(iterator&&) = default;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator<(const iterator& right) const;
        bool operator>(const iterator& right) const;
        bool operator<=(const iterator& right) const;
        bool operator>=(const iterator& right) const;
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const { return !operator==(right);}
        bool operator<(const const_iterator& right) const;
        bool operator>(const const_iterator& right) const;
        bool operator<=(const const_iterator& right) const;
        bool operator>=(const const_iterator& right) const;
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const { return !operator==(right);}
        const char* operator->() const;
        const char& operator*() const;
        const char&operator[](qsizetype j) const;
        iterator& operator+=(size_t n);
        iterator& operator-=(size_t n);
        const char* data() const;
        qsizetype operator-(const const_iterator& j) const;
        qsizetype operator-(const iterator& j) const;
        char* operator->();
        char& operator*();
        char&operator[](qsizetype j);
        char* data();
    private:
        size_t offset;
        char* ptr;
        friend const_iterator;
    };
    const_iterator begin(const void* container);
    const_iterator end(const void* container);
    inline const_iterator constBegin(const void* container) {return begin(container);}
    inline const_iterator constEnd(const void* container) {return end(container);}
    iterator begin(void* container);
    iterator end(void* container);
    inline std::reverse_iterator<const_iterator> reverseBegin(const void* container) {return std::reverse_iterator<const_iterator>{end(container)};}
    inline std::reverse_iterator<const_iterator> reverseEnd(const void* container) {return std::reverse_iterator<const_iterator>{begin(container)};}
    inline std::reverse_iterator<const_iterator> constReverseBegin(const void* container) {return reverseBegin(container);}
    inline std::reverse_iterator<const_iterator> constReverseEnd(const void* container) {return reverseEnd(container);}
    inline std::reverse_iterator<iterator> reverseBegin(void* container) {return std::reverse_iterator<iterator>{end(container)};}
    inline std::reverse_iterator<iterator> reverseEnd(void* container) {return std::reverse_iterator<iterator>{begin(container)};}
private:
    AbstractNestedSequentialAccess* asNested() override;
    bool equals(const void* p1, const void* p2);
    void debugStream(QDebug &s, const void *ptr);
    void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    using ConstContainer = QtJambiPrivate::ConstSequentialContainer<AutoListAccess>;
    using Container = QtJambiPrivate::SequentialContainer<AutoListAccess>;
    friend ConstContainer;
    friend Container;
public:
    void assign(void* container, const void* other) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    QMetaType registerContainer(QByteArrayView containerTypeName) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoListAccess>, const_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoListAccess>, iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoListAccess>, std::reverse_iterator<const_iterator>>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoListAccess>, std::reverse_iterator<iterator>>&& iterator);
    void reserve(void*,qsizetype) override;
    std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> metaTypes();
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constReverseBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constReverseEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject reverseBegin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject reverseEnd(JNIEnv * env, const ExtendedContainerInfo& container) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    jobject at(JNIEnv * env, const void* container, qsizetype index) override;
    const void* at(const void* container, qsizetype index) override;
    void* at(void* container, qsizetype index) override;
    jobject value(JNIEnv * env, const void* container, qsizetype index) override;
    jobject value(JNIEnv * env, const void* container, qsizetype index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv *, const ContainerInfo& container, qsizetype index1, qsizetype index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    qsizetype size(JNIEnv *, const void* container) override;
    void reserve(JNIEnv *, const ContainerInfo& container, qsizetype size) override;
    void replace(JNIEnv * env, const ContainerInfo& container, qsizetype index, jobject value) override;
    void replace(void* container, qsizetype index, const void* value) override;
    void remove(JNIEnv *, const ContainerInfo& container, qsizetype index, qsizetype n) override;
    void remove(void* container, qsizetype index, qsizetype n) override;
    qsizetype removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv *, const ContainerInfo& container, qsizetype index1, qsizetype index2) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, qsizetype index1, qsizetype index2) override;
    qsizetype lastIndexOf(JNIEnv * env, const void* container, jobject value, qsizetype index) override;
    qsizetype indexOf(JNIEnv * env, const void* container, jobject value, qsizetype index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    qsizetype count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv *, const ContainerInfo& container) override;
    void clear(void* container);
    void insert(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n, jobject value) override;
    void insert(void* container, qsizetype index, qsizetype n, const void* entry) override;
    bool append(void* container, const void* value) override;
    inline void insert(void* container, const void* value){
        append(container, value);
    }
    qsizetype size(const void* container) override;
    void resize(void* container, qsizetype newSize) override;
    qsizetype capacity(JNIEnv *, const void* container) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, qsizetype size) override;
    void resize(JNIEnv *, const ContainerInfo& container, qsizetype newSize) override;
    void squeeze(JNIEnv *, const ContainerInfo& container) override;
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(const void* container) override;
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(void* container) override;
private:
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(QMetaType newMetaType);
    void emplace(QListData* p, JNIEnv * env, qsizetype index, jobject value, qsizetype n);
    void emplace(QListData* p, qsizetype index, const void* value, qsizetype n);
    void *createHole(QListData *p, QArrayData::GrowthPosition pos, qsizetype where, qsizetype n);
    qsizetype freeSpaceAtBegin(const QListData* p);
    qsizetype freeSpaceAtEnd(const QListData* p);
    void detach(QListData* p, QListData *old = nullptr);
    void detachAndGrow(QListData* p, QArrayData::GrowthPosition where, qsizetype n, const void **data,
                       QListData *old);
    void reallocate(QListData* p, qsizetype alloc, QArrayData::AllocationOption option);
    void reallocateAndGrow(QListData* p, QArrayData::GrowthPosition, qsizetype n,
                           QListData *old = nullptr);
    QListData allocateGrow(const QListData &from, qsizetype n, QArrayData::GrowthPosition position);
    QListData allocate(qsizetype capacity, QArrayData::AllocationOption option = QArrayData::KeepSize);
    void swapAndDestroy(QListData* p, QListData&& other);
    friend class PointerRCAutoListAccess;
    friend class NestedPointersRCAutoListAccess;
};

typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&, void*& ptr);

class AutoMapAccess : public AbstractMapAccess, public AbstractNestedAssociativeAccess {
    QMetaType m_keyMetaType;
    QtJambiUtils::QHashFunction m_keyHashFunction;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    QMetaType m_valueMetaType;
    QtJambiUtils::QHashFunction m_valueHashFunction;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    size_t m_align;
    size_t m_offset1;
    size_t m_offset2;
    size_t m_size;
    PtrOwnerFunction m_keyOwnerFunction;
    PtrOwnerFunction m_valueOwnerFunction;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;
    template<typename, typename, typename, bool, bool, bool, typename...>
    friend struct qtjambi_ContainerIterator_cast;
protected:
    AutoMapAccess(const AutoMapAccess &);
public:
    ~AutoMapAccess() override;
    AutoMapAccess(
                    const QMetaType& keyMetaType,
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
                    const QMetaType& valueMetaType,
                    const QtJambiUtils::QHashFunction& valueHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                    PtrOwnerFunction valueOwnerFunction,
                    AbstractContainerAccess::DataType valueDataType
            );
    static jboolean iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2);
    static void* asIterator(void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType);
    static bool findIterator(const void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const std::type_info& typeId, void* output);
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void assign(void* container, const void* other) override;
    QMetaType registerContainer(QByteArrayView containerTypeName) override;
    void dispose() override;
    AbstractMapAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    void clear(void*);
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    qsizetype count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyValueBegin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject keyValueEnd(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constKeyValueBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constKeyValueEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jobject constLowerBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jobject constUpperBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jobject lowerBound(JNIEnv *,const ExtendedContainerInfo& container,jobject) override;
    jobject upperBound(JNIEnv *,const ExtendedContainerInfo& container,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    qsizetype remove(JNIEnv *,const ContainerInfo&,jobject) override;
    qsizetype size(JNIEnv *,const void*) override;
    qsizetype size(const void* container) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
    bool destructContainer(void* container) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    virtual void debugStream(QDebug &s, const void *ptr);
    virtual void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> metaTypes();
private:
    AbstractNestedAssociativeAccess* asNested() override;
    virtual IsBiContainerFunction getIsBiContainerFunction();
    virtual bool isMulti() const;
    bool equal(const void* containerA, const void* containerB);
    static QtMetaContainerPrivate::QMetaAssociationInterface* createMetaAssociationInterface(QMetaType newMetaType);

    enum Colors {
        Red,
        Black
    };

#if defined(_LIBCPP_VERSION)
    struct TreeNode;
    struct TreeEndNode{
        TreeNode* left = nullptr;
    };

    struct TreeNode : TreeEndNode{
        TreeNode* right = nullptr;
        TreeEndNode* parent = nullptr;
        bool color = Red;
#elif defined(Q_CC_MSVC)
    struct TreeNode{
        TreeNode* left = nullptr;
        TreeNode* parent = nullptr;
        TreeNode* right = nullptr;
        char color = Red;
        char isNil = false;
#elif defined(__GLIBCXX__)
    struct TreeNode{
        Colors color = Red;
        TreeNode* parent = nullptr;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
#endif // defined(_LIBCPP_VERSION)
        char* data(qsizetype offset);
        const char* data(qsizetype offset) const;
    };
#if defined(Q_CC_MSVC)
#if _ITERATOR_DEBUG_LEVEL == 0
    struct MapData : QSharedData, std::_Container_base0{
#else
    struct MapData : QSharedData, std::_Container_base12{
#endif // _ITERATOR_DEBUG_LEVEL == 0
        TreeNode* head = nullptr;
        quint64 size = 0;
    };
#else // !defined(Q_CC_MSVC)
    struct MapData : QSharedData{
#if defined(_LIBCPP_VERSION)
        TreeNode* begin = nullptr;
        mutable TreeEndNode header;
        std::size_t size = 0;
        TreeNode* end();
#elif defined(__GLIBCXX__)
        quintptr compare = 0;
        mutable TreeNode header;
        std::size_t size = 0;
#endif // defined(_LIBCPP_VERSION)
    };
#endif // defined(Q_CC_MSVC)
    typedef QtPrivate::QExplicitlySharedDataPointerV2<MapData> MapDataPointer;

    struct node_iterator{
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type      = TreeNode;
        using difference_type = qsizetype;
        using pointer         = TreeNode*;
        using reference       = TreeNode&;

        node_iterator(TreeNode* _node = nullptr);
        node_iterator(const node_iterator&);
        inline node_iterator& operator=(const node_iterator& o) {
            node = o.node;
            return *this;
        }
        node_iterator& operator++();
        node_iterator operator++(int);
        node_iterator& operator--();
        node_iterator operator--(int);
        bool operator==(const node_iterator& right) const;
        bool operator!=(const node_iterator& right) const;
        TreeNode* operator->() const;
        TreeNode& operator*() const;
        inline TreeNode* data() const{return node;};
    private:
        TreeNode* next(TreeNode*);
        TreeNode* prev(TreeNode*);
        TreeNode* node;
    };

public:
    struct const_key_value_iterator;
    struct key_value_iterator;
    struct const_iterator;
    struct key_iterator;

    struct iterator{
        node_iterator m_iter;
        size_t m_offset1 = 0;
        size_t m_offset2 = 0;
        iterator(node_iterator iter, size_t offset1, size_t offset2);
        iterator() = default;
        iterator(const iterator&) = default;
        iterator& operator=(const iterator& o) = default;
        iterator(iterator&&) = default;
        iterator& operator=(iterator&& o) = default;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const {return !(*this==right);}
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const {return !(*this==right);}
        bool operator==(const key_iterator& right) const;
        inline bool operator!=(const key_iterator& right) const {return !(*this==right);}
        bool operator==(const key_value_iterator& right) const;
        inline bool operator!=(const key_value_iterator& right) const {return !(*this==right);}
        bool operator==(const const_key_value_iterator& right) const;
        inline bool operator!=(const const_key_value_iterator& right) const {return !(*this==right);}
        const char& key()const;
        const char& value()const;
        char& key();
        char& value();
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef char value_type;
        typedef const char *pointer;
        typedef const char &reference;
        friend const_iterator;
        friend key_value_iterator;
        friend const_key_value_iterator;
        friend key_iterator;
    };

    struct const_iterator{
        node_iterator m_iter;
        size_t m_offset1 = 0;
        size_t m_offset2 = 0;
        const_iterator(node_iterator iter, size_t offset1, size_t offset2);
        const_iterator() = default;
        const_iterator(const const_iterator&) = default;
        const_iterator(const iterator&);
        const_iterator& operator=(const const_iterator& o) = default;
        const_iterator(const_iterator&&) = default;
        const_iterator& operator=(const_iterator&& o) = default;
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const {return !(*this==right);}
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const {return !(*this==right);}
        bool operator==(const key_iterator& right) const;
        inline bool operator!=(const key_iterator& right) const {return !(*this==right);}
        bool operator==(const key_value_iterator& right) const;
        inline bool operator!=(const key_value_iterator& right) const {return !(*this==right);}
        bool operator==(const const_key_value_iterator& right) const;
        inline bool operator!=(const const_key_value_iterator& right) const {return !(*this==right);}
        const char& key()const;
        const char& value()const;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef char value_type;
        typedef const char *pointer;
        typedef const char &reference;
        friend iterator;
        friend key_value_iterator;
        friend const_key_value_iterator;
        friend key_iterator;
    };

    struct key_iterator{
        node_iterator m_iter;
        size_t m_offset1 = 0;
        key_iterator(node_iterator iter, size_t offset1);
        key_iterator() = default;
        key_iterator(const key_iterator&) = default;
        key_iterator& operator=(const key_iterator& o) = default;
        key_iterator(key_iterator&&) = default;
        key_iterator& operator=(key_iterator&& o) = default;
        key_iterator& operator++();
        key_iterator operator++(int);
        key_iterator& operator--();
        key_iterator operator--(int);
        bool operator==(const key_iterator& right) const;
        inline bool operator!=(const key_iterator& right) const {return !(*this==right);}
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const {return !(*this==right);}
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const {return !(*this==right);}
        bool operator==(const key_value_iterator& right) const;
        inline bool operator!=(const key_value_iterator& right) const {return !(*this==right);}
        bool operator==(const const_key_value_iterator& right) const;
        inline bool operator!=(const const_key_value_iterator& right) const {return !(*this==right);}
        const char& operator*()const;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef const char& value_type;
        typedef const char *pointer;
        typedef const char &reference;
        friend const_iterator;
        friend iterator;
        friend key_value_iterator;
        friend const_key_value_iterator;
    };

    struct key_value_iterator{
        node_iterator m_iter;
        size_t m_offset1 = 0;
        size_t m_offset2 = 0;
        key_value_iterator(node_iterator iter, size_t offset1, size_t offset2);
        key_value_iterator() = default;
        key_value_iterator(const key_value_iterator&) = default;
        key_value_iterator& operator=(const key_value_iterator& o) = default;
        key_value_iterator(key_value_iterator&&) = default;
        key_value_iterator& operator=(key_value_iterator&& o) = default;
        key_value_iterator& operator++();
        key_value_iterator operator++(int);
        key_value_iterator& operator--();
        key_value_iterator operator--(int);
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const {return !(*this==right);}
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const {return !(*this==right);}
        bool operator==(const key_iterator& right) const;
        inline bool operator!=(const key_iterator& right) const {return !(*this==right);}
        bool operator==(const key_value_iterator& right) const;
        inline bool operator!=(const key_value_iterator& right) const {return !(*this==right);}
        bool operator==(const const_key_value_iterator& right) const;
        inline bool operator!=(const const_key_value_iterator& right) const {return !(*this==right);}
        std::pair<const char&,const char&> operator*()const;
        std::pair<const char&,char&> operator*();
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef char value_type;
        typedef const char *pointer;
        typedef const char &reference;
        friend const_iterator;
        friend const_key_value_iterator;
        friend key_iterator;
        friend iterator;
    };

    struct const_key_value_iterator{
        node_iterator m_iter;
        size_t m_offset1 = 0;
        size_t m_offset2 = 0;
        const_key_value_iterator(node_iterator iter, size_t offset1, size_t offset2);
        const_key_value_iterator() = default;
        const_key_value_iterator(const key_value_iterator&);
        const_key_value_iterator(const const_key_value_iterator&) = default;
        const_key_value_iterator& operator=(const const_key_value_iterator& o) = default;
        const_key_value_iterator(const_key_value_iterator&&) = default;
        const_key_value_iterator& operator=(const_key_value_iterator&& o) = default;
        const_key_value_iterator& operator++();
        const_key_value_iterator operator++(int);
        const_key_value_iterator& operator--();
        const_key_value_iterator operator--(int);
        bool operator==(const const_iterator& right) const;
        inline bool operator!=(const const_iterator& right) const {return !(*this==right);}
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const {return !(*this==right);}
        bool operator==(const key_iterator& right) const;
        inline bool operator!=(const key_iterator& right) const {return !(*this==right);}
        bool operator==(const key_value_iterator& right) const;
        inline bool operator!=(const key_value_iterator& right) const {return !(*this==right);}
        bool operator==(const const_key_value_iterator& right) const;
        inline bool operator!=(const const_key_value_iterator& right) const {return !(*this==right);}
        std::pair<const char&,const char&> operator*()const;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef char value_type;
        typedef const char *pointer;
        typedef const char &reference;
        friend iterator;
    };

    key_iterator keyBegin(const void* container);
    key_iterator keyEnd(const void* container);

    const_key_value_iterator keyValueEnd(const void* container);
    const_key_value_iterator keyValueBegin(const void* container);
    inline const_key_value_iterator constKeyValueBegin(const void* container) {return keyValueBegin(container);}
    inline const_key_value_iterator constKeyValueEnd(const void* container) {return keyValueEnd(container);}
    key_value_iterator keyValueEnd(void* container);
    key_value_iterator keyValueBegin(void* container);

    const_iterator end(const void* container);
    const_iterator begin(const void* container);
    inline const_iterator constBegin(const void* container) {return begin(container);}
    inline const_iterator constEnd(const void* container) {return end(container);}
    iterator end(void* container);
    iterator begin(void* container);
private:
    struct extended_iterator : node_iterator{
        extended_iterator(const void* _container, const QSharedPointer<class AutoMapAccess>& _access, TreeNode* _node = nullptr);
        extended_iterator(const void* _container, const QSharedPointer<class AutoMapAccess>& _access, const node_iterator& other);
        extended_iterator(const extended_iterator&) = default;
        const void* container;
        QSharedPointer<class AutoMapAccess> access;
    };

    MapData& detach(MapDataPointer& container);
    void copyNode(QList<TreeNode*>& nextNodes, QHash<TreeNode*,TreeNode*> &nodesMap, TreeNode* node);
    MapData* createMapData();
    void clear(MapData& data);
    node_iterator end(const MapData& data);
    node_iterator begin(const MapData& data);
    void eraseTree(MapData& data, TreeNode* node);
    qsizetype erase(MapData& data, const void* key);
    qsizetype erase(MapData& data, const void* key, const void* value);
    qsizetype erase(MapData& data, QPair<TreeNode*,TreeNode*> pair);
    void destroy(TreeNode*);
    TreeNode* extract(MapData& data, node_iterator iter);
    void insertOrAssign(MapData&,const void*,JNIEnv *,jobject value);
    void insertOrAssign(MapData&,const void*,const void* value);
    qsizetype copyIfNotEquivalentTo(MapData& data, const MapData& copyFrom, const void* key);
    qsizetype copyIfNotEquivalentTo(MapData& data, const MapData& copyFrom, const void* key, const void* value);
    node_iterator erase(MapData& data, node_iterator iter);
    TreeNode* erase(MapData& data, node_iterator first, node_iterator last);
    QPair<TreeNode*,TreeNode*> eqrange(const MapData& data, const void* key);
    node_iterator find(const MapData& data, const void* key);
#if defined(Q_CC_MSVC)
    enum class TreeChild {
        Right,
        Left,
        Unused
    };

    struct Location{
        TreeNode* parent = nullptr;
        TreeChild child = TreeChild::Unused;
    };

    struct TreeFindResult{
        Location location;
        TreeNode* bound = nullptr;
        inline operator TreeNode*() {return bound;}
    };
    TreeFindResult findLowerBound(const MapData& data, const void* key);
    TreeNode* insertNode(MapData& data, Location loc, TreeNode* newNode);
    static TreeNode* treeMin(TreeNode*);
    static TreeNode* treeMax(TreeNode*);
    void rotateLeft(MapData& data, TreeNode*node);
    void rotateRight(MapData& data, TreeNode*node);
    void orphanPtr(MapData& data, const TreeNode* node);
#elif defined(_LIBCPP_VERSION)
    TreeNode* findLowerBound(const MapData& data, const void* key);
    TreeNode*& findLeafHigh(MapData& data, TreeEndNode*& parent, const void* key);
    TreeNode*& findLeafLow(MapData& data, TreeEndNode*& parent, const void* key);
    TreeNode*& findLeaf(MapData& data, node_iterator node, TreeEndNode*& parent, const void* key);
    TreeNode*& findEqual(MapData& data, TreeEndNode*& parent, const void* key);
    TreeNode*& findEqualHint(MapData& data, node_iterator node, TreeEndNode*& parent, TreeNode*& dummy, const void* key);
#elif defined(__GLIBCXX__)
    TreeNode* findLowerBound(const MapData& data, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertUniquePos(MapData& data, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertHintUniquePos(MapData& data, TreeNode* node, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertEqualPos(MapData& data, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertHintEqualPos(MapData& data, TreeNode* node, const void* key);
#endif // defined(Q_CC_MSVC) || defined(_LIBCPP_VERSION)
    TreeNode* findUpperBound(const MapData& data, const void* key);

    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMapAccess>, const_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMapAccess>, iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMapAccess>, const_key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMapAccess>, key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMapAccess>, key_iterator>&& iterator);
    friend class AutoMultiMapAccess;

    using ConstContainer = QtJambiPrivate::ConstAssociativeContainer<AutoMapAccess>;
    using Container = QtJambiPrivate::AssociativeContainer<AutoMapAccess>;
    friend Container;
    friend ConstContainer;
public:
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(void* container) override;
};

class AutoMultiMapAccess : public virtual AbstractMultiMapAccess, public AutoMapAccess{
    template<typename, typename, typename, bool, bool, bool, typename...>
    friend struct qtjambi_ContainerIterator_cast;
protected:
    AutoMultiMapAccess(const AutoMultiMapAccess&);
public:
    ~AutoMultiMapAccess() override;
    AutoMultiMapAccess(
                    const QMetaType& keyMetaType,
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
                    const QMetaType& valueMetaType,
                    const QtJambiUtils::QHashFunction& valueHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                    PtrOwnerFunction valueOwnerFunction,
                    AbstractContainerAccess::DataType valueDataType
            );
    using AutoMapAccess::take;
    using AutoMapAccess::value;
    using AutoMapAccess::values;
    using AutoMapAccess::key;
    using AutoMapAccess::keys;
    using AutoMapAccess::equal;
    using AutoMapAccess::size;
    using AutoMapAccess::insert;
    using AutoMapAccess::hasOwnerFunction;
    using AutoMapAccess::getOwner;
    using AutoMapAccess::isSharedWith;
    using AutoMapAccess::detach;
    using AutoMapAccess::isDetached;
    using AutoMapAccess::constFind;
    using AutoMapAccess::constEnd;
    using AutoMapAccess::constBegin;
    using AutoMapAccess::find;
    using AutoMapAccess::end;
    using AutoMapAccess::begin;
    using AutoMapAccess::keyBegin;
    using AutoMapAccess::keyEnd;
    using AutoMapAccess::keyValueBegin;
    using AutoMapAccess::keyValueEnd;
    using AutoMapAccess::constKeyValueBegin;
    using AutoMapAccess::constKeyValueEnd;
    using AutoMapAccess::count;
    using AutoMapAccess::contains;
    using AutoMapAccess::clear;
    using AutoMapAccess::hasValueNestedPointers;
    using AutoMapAccess::hasValueNestedContainerAccess;
    using AutoMapAccess::hasKeyNestedPointers;
    using AutoMapAccess::hasKeyNestedContainerAccess;
    using AutoMapAccess::sharedValueNestedContainerAccess;
    using AutoMapAccess::sharedKeyNestedContainerAccess;
    using AutoMapAccess::valueNestedContainerAccess;
    using AutoMapAccess::keyNestedContainerAccess;
    using AutoMapAccess::valueType;
    using AutoMapAccess::keyType;
    using AutoMapAccess::valueMetaType;
    using AutoMapAccess::keyMetaType;
    using AutoMapAccess::registerContainer;
    using AutoMapAccess::keyValueIterator;
    using AutoMapAccess::destructContainer;
    using AutoMapAccess::constructContainer;
    using AutoMapAccess::assign;
    using AutoMapAccess::constLowerBound;
    using AutoMapAccess::constUpperBound;
    using AutoMapAccess::deleteContainer;
    using AutoMapAccess::createContainer;
    static jboolean iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2);
    static void* asIterator(void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType);
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    QMetaType registerContainer(QByteArrayView containerTypeName) override;
    void dispose() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    qsizetype count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyValueBegin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject keyValueEnd(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constKeyValueBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constKeyValueEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    qsizetype remove(JNIEnv *,const ContainerInfo&,jobject) override;
    qsizetype size(JNIEnv *,const void*) override;
    qsizetype size(const void* container) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject constLowerBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jobject constUpperBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jobject lowerBound(JNIEnv *,const ExtendedContainerInfo& container,jobject) override;
    jobject upperBound(JNIEnv *,const ExtendedContainerInfo& container,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
    AbstractMultiMapAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    ContainerAndAccessInfo uniqueKeys(JNIEnv *,const ConstContainerInfo&) override;
    void unite(JNIEnv *,const ContainerInfo&,ContainerAndAccessInfo&) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean contains(JNIEnv *,const void*,jobject,jobject) override;
    qsizetype count(JNIEnv *,const void*,jobject,jobject) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override;
    qsizetype remove(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    void replace(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    void dataStreamOut(QDataStream &s, const void *ptr) override;
    void debugStream(QDebug &s, const void *ptr) override;
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(void* container) override;
    void* createContainer(const void* copy);
    void deleteContainer(void* container);
private:
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, key_iterator>&& iterator);
    IsBiContainerFunction getIsBiContainerFunction() override;
    bool isMulti() const override;
};

class AutoHashAccess : public AbstractHashAccess, public AbstractNestedAssociativeAccess {
    QMetaType m_keyMetaType;
    QtJambiUtils::QHashFunction m_keyHashFunction;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    QMetaType m_valueMetaType;
    QtJambiUtils::QHashFunction m_valueHashFunction;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    size_t m_align;
    size_t m_offset2;
    size_t m_size;
    PtrOwnerFunction m_keyOwnerFunction;
    PtrOwnerFunction m_valueOwnerFunction;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;
    template<typename, typename, typename, bool, bool, bool, typename...>
    friend struct qtjambi_ContainerIterator_cast;
protected:
    AutoHashAccess(const AutoHashAccess&);
public:
    ~AutoHashAccess() override;
    AutoHashAccess(
                    const QMetaType& keyMetaType,
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
                    const QMetaType& valueMetaType = QMetaType(QMetaType::Void),
                    const QtJambiUtils::QHashFunction& valueHashFunction = {},
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter = {},
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter = {},
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess = {},
                    PtrOwnerFunction valueOwnerFunction = nullptr,
                    AbstractContainerAccess::DataType valueDataType = AbstractContainerAccess::Value
            );
    static jboolean iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2);
    static void* asIterator(void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType);
    static bool findIterator(const void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const std::type_info& typeId, void* output);
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    virtual void assign(void* container, const void* other) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    QMetaType registerContainer(QByteArrayView containerTypeName) override;
    void dispose() override;
    AbstractHashAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    qsizetype capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    void clear(void*);
    jboolean contains(JNIEnv *,const void*,jobject) override;
    qsizetype count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyValueBegin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject keyValueEnd(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constKeyValueBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constKeyValueEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    qsizetype remove(JNIEnv *,const ContainerInfo&,jobject) override;
    void reserve(JNIEnv *,const ContainerInfo&,qsizetype) override;
    void reserve(void* container, qsizetype size) override;
    qsizetype size(JNIEnv *,const void*) override;
    qsizetype size(const void*) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> metaTypes();
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value = nullptr) override;
    const void* value(const void*, const void*, const void*) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override;
private:
    AbstractNestedAssociativeAccess* asNested() override;
    virtual IsBiContainerFunction getIsBiContainerFunction();
    static QtMetaContainerPrivate::QMetaAssociationInterface* createMetaAssociationInterface(QMetaType newMetaType);
    virtual void debugStream(QDebug &s, const void *ptr);
    virtual void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    struct Span {
        enum {
            NEntries = 128,
            LocalBucketMask = (NEntries - 1),
            UnusedEntry = 0xff
        };
        unsigned char offsets[NEntries];
        char *entries = nullptr;
        unsigned char allocated = 0;
        unsigned char nextFree = 0;

        Span() noexcept
        {
            memset(offsets, UnusedEntry, sizeof(offsets));
        }
        ~Span()
        {
        }

        void freeData(const AutoHashAccess& access);
        char* insert(const AutoHashAccess& access, size_t i);
        void addStorage(const AutoHashAccess& access);
        size_t offset(size_t i) const noexcept;
        bool hasNode(size_t i) const noexcept;

        char* at(const AutoHashAccess& access, size_t i) noexcept;
        char* at(const AutoHashAccess& access, size_t i) const noexcept;
        char* atOffset(const AutoHashAccess& access, size_t o) noexcept;
        char* atOffset(const AutoHashAccess& access, size_t o) const noexcept;
        void erase(const AutoHashAccess& access, size_t bucket, qsizetype* count = nullptr);

        void moveLocal(size_t from, size_t to) noexcept;
        void moveFromSpan(const AutoHashAccess& access, Span &fromSpan, size_t fromIndex, size_t to);
    };

    struct QHashData{
        struct iterator{
            QSharedPointer<const AutoHashAccess> access;
            struct QHashData const* d = nullptr;
            size_t bucket = 0;
            iterator(const AutoHashAccess& _access, struct QHashData const* _d = nullptr, size_t _bucket = 0);
            iterator(const iterator& iter);
            iterator& operator=(const iterator& iter);
            size_t span() const noexcept;
            size_t index() const noexcept;
            bool isUnused() const noexcept;
            char* node() const noexcept;
            char* key() const noexcept;
            char* value() const noexcept;
            bool atEnd() const noexcept;
            iterator operator++() noexcept;
            bool operator==(const iterator& other) const noexcept;
            inline bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        };

        QtPrivate::RefCount ref = {{1}};
        size_t size = 0;
        size_t numBuckets = 0;
        size_t seed = 0;
        Span *spans = nullptr;
        QHashData(const AutoHashAccess& access, size_t reserve = 0);
        QHashData(const AutoHashAccess& access, const QHashData &other, size_t reserved = 0);
        void destroy(const AutoHashAccess& access);
        static QHashData* detached(const AutoHashAccess& access, QHashData* data, size_t size = 0);

        iterator find(const AutoHashAccess& access, const void *key) const noexcept;

        size_t nextBucket(size_t bucket) const noexcept
        {
            ++bucket;
            if (bucket == numBuckets)
                bucket = 0;
            return bucket;
        }

        bool shouldGrow() const noexcept
        {
            return size >= (numBuckets >> 1);
        }

        void rehash(const AutoHashAccess& access, size_t sizeHint = 0);

        struct InsertionResult
        {
            iterator it;
            bool initialized;
        };

        InsertionResult findOrInsert(const AutoHashAccess& access, const void*key) noexcept;

        char* findNode(const AutoHashAccess& access, const void *key) const noexcept;

        iterator detachedIterator(const AutoHashAccess& access, iterator other) const noexcept;

        iterator begin(const AutoHashAccess& access) const noexcept;

        iterator end(const AutoHashAccess& access) const noexcept;

        iterator erase(const AutoHashAccess& access, iterator it, qsizetype* count = nullptr);
    };
    using ConstContainer = QtJambiPrivate::ConstAssociativeContainer<AutoHashAccess>;
    using Container = QtJambiPrivate::AssociativeContainer<AutoHashAccess>;
    friend ConstContainer;
    friend Container;
    using SetConstContainer = QtJambiPrivate::ConstSequentialContainer<AutoHashAccess>;
    using SetContainer = QtJambiPrivate::SequentialContainer<AutoHashAccess>;
    friend SetConstContainer;
    friend SetContainer;
public:

    class const_iterator;
    class key_iterator;
    class key_value_iterator;
    class const_key_value_iterator;

    class abstract_iterator{
        QHashData::iterator i;
        void* e = nullptr;
    protected:
        explicit abstract_iterator(const QHashData::iterator& _i);
        abstract_iterator(abstract_iterator&& iter) = default;
        abstract_iterator(const abstract_iterator& iter) = default;
        abstract_iterator& operator=(const abstract_iterator& iter) = default;
        abstract_iterator& operator=(abstract_iterator&& iter) = default;
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
        friend class AutoSetAccess;
    };

    class iterator : public abstract_iterator {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef char& value_type;
        typedef const char *pointer;
        typedef const char &reference;
        iterator(const QHashData::iterator& _i);

        iterator(iterator&& iter) = default;
        iterator(const iterator& iter) = default;
        iterator& operator=(const iterator& iter) = default;
        iterator& operator=(iterator&& iter) = default;
        iterator& operator++() noexcept;
        iterator operator++(int) noexcept;
        bool operator==(const iterator& other) const noexcept;
        inline bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_iterator& other) const noexcept;
        inline bool operator!=(const const_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_iterator& other) const noexcept;
        inline bool operator!=(const key_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_value_iterator& other) const noexcept;
        inline bool operator!=(const key_value_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_key_value_iterator& other) const noexcept;
        inline bool operator!=(const const_key_value_iterator& other) const noexcept { return !(*this == other); }
        inline const char& key() const noexcept {return *i.key();}
        inline char& operator*() const noexcept {return *i.key();}
        char& value() const noexcept;
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
        friend const_iterator;
        friend key_iterator;
        friend key_value_iterator;
        friend const_key_value_iterator;
    };

    class const_iterator : public abstract_iterator {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef const char& value_type;
        typedef const char *pointer;
        typedef const char &reference;

        const_iterator(const QHashData::iterator& _i);
        const_iterator(const iterator& other);

        const_iterator(const_iterator&& iter) = default;
        const_iterator(const const_iterator& iter) = default;
        const_iterator& operator=(const const_iterator& iter) = default;
        const_iterator& operator=(const_iterator&& iter) = default;
        const_iterator& operator++() noexcept;
        const_iterator operator++(int) noexcept;
        bool operator==(const const_iterator& other) const noexcept;
        inline bool operator!=(const const_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_iterator& other) const noexcept;
        inline bool operator!=(const key_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const iterator& other) const noexcept;
        inline bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_value_iterator& other) const noexcept;
        inline bool operator!=(const key_value_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_key_value_iterator& other) const noexcept;
        inline bool operator!=(const const_key_value_iterator& other) const noexcept { return !(*this == other); }
        inline const char& key() const noexcept {return *i.key();}
        inline const char& operator*() const noexcept {return *i.key();}
        const char& value() const noexcept;
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
        friend class AutoSetAccess;
        friend key_value_iterator;
        friend key_iterator;
        friend const_key_value_iterator;
        friend iterator;
    };

    class key_iterator : public abstract_iterator {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef const char& value_type;
        typedef const char *pointer;
        typedef const char &reference;

        key_iterator(const QHashData::iterator& _i);

        key_iterator(key_iterator&& iter) = default;
        key_iterator(const key_iterator& iter) = default;
        key_iterator& operator=(const key_iterator& iter) = default;
        key_iterator& operator=(key_iterator&& iter) = default;
        key_iterator& operator++() noexcept;
        key_iterator operator++(int) noexcept;
        bool operator==(const key_iterator& other) const noexcept;
        inline bool operator!=(const key_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_iterator& other) const noexcept;
        inline bool operator!=(const const_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const iterator& other) const noexcept;
        inline bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_value_iterator& other) const noexcept;
        inline bool operator!=(const key_value_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_key_value_iterator& other) const noexcept;
        inline bool operator!=(const const_key_value_iterator& other) const noexcept { return !(*this == other); }
        inline const char& operator*() const noexcept {return *i.key();}
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
    };

    class key_value_iterator : public abstract_iterator {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef std::pair<const char&,char&> value_type;
        typedef const char *pointer;
        typedef const char &reference;
        key_value_iterator(const QHashData::iterator& _i);

        key_value_iterator(key_value_iterator&& iter) = default;
        key_value_iterator(const key_value_iterator& iter) = default;
        key_value_iterator& operator=(const key_value_iterator& iter) = default;
        key_value_iterator& operator=(key_value_iterator&& iter) = default;
        key_value_iterator& operator++() noexcept;
        key_value_iterator operator++(int) noexcept;
        bool operator==(const iterator& other) const noexcept;
        inline bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_iterator& other) const noexcept;
        inline bool operator!=(const const_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_iterator& other) const noexcept;
        inline bool operator!=(const key_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_value_iterator& other) const noexcept;
        inline bool operator!=(const key_value_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_key_value_iterator& other) const noexcept;
        inline bool operator!=(const const_key_value_iterator& other) const noexcept { return !(*this == other); }
        std::pair<const char&,char&> operator*() const noexcept;
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
        friend const_iterator;
        friend key_iterator;
        friend iterator;
        friend const_key_value_iterator;
    };

    class const_key_value_iterator : public abstract_iterator {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef std::pair<const char&,const char&> value_type;
        typedef const char *pointer;
        typedef const char &reference;

        const_key_value_iterator(const QHashData::iterator& _i);
        const_key_value_iterator(const key_value_iterator& other);

        const_key_value_iterator(const_key_value_iterator&& iter) = default;
        const_key_value_iterator(const const_key_value_iterator& iter) = default;
        const_key_value_iterator& operator=(const const_key_value_iterator& iter) = default;
        const_key_value_iterator& operator=(const_key_value_iterator&& iter) = default;
        const_key_value_iterator& operator++() noexcept;
        const_key_value_iterator operator++(int) noexcept;
        bool operator==(const const_iterator& other) const noexcept;
        inline bool operator!=(const const_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_iterator& other) const noexcept;
        inline bool operator!=(const key_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const iterator& other) const noexcept;
        inline bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const key_value_iterator& other) const noexcept;
        inline bool operator!=(const key_value_iterator& other) const noexcept { return !(*this == other); }
        bool operator==(const const_key_value_iterator& other) const noexcept;
        inline bool operator!=(const const_key_value_iterator& other) const noexcept { return !(*this == other); }
        std::pair<const char&,const char&> operator*() const noexcept;
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
        friend const_iterator;
        friend key_iterator;
        friend iterator;
        friend key_value_iterator;
    };

    key_iterator keyBegin(const void* container);
    key_iterator keyEnd(const void* container);

    const_key_value_iterator keyValueEnd(const void* container);
    const_key_value_iterator keyValueBegin(const void* container);
    inline const_key_value_iterator constKeyValueBegin(const void* container) {return keyValueBegin(container);}
    inline const_key_value_iterator constKeyValueEnd(const void* container) {return keyValueEnd(container);}
    key_value_iterator keyValueEnd(void* container);
    key_value_iterator keyValueBegin(void* container);

    iterator begin(void* container);
    iterator end(void* container);
    const_iterator begin(const void* container);
    const_iterator end(const void* container);
    inline const_iterator constBegin(const void* container) {return begin(container);}
    inline const_iterator constEnd(const void* container) {return end(container);}
private:
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoHashAccess>, const_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoHashAccess>, iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoHashAccess>, key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoHashAccess>, const_key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoHashAccess>, key_iterator>&& iterator);

    virtual bool equalSpanEntries(const char* value1, const char* value2) const;
    virtual void copySpanEntry(char* value1, const char* value2) const;
    virtual void eraseSpanEntry(char* value, qsizetype* count = nullptr) const;
    virtual void emplace(void* container, const void* akey, JNIEnv *env, jobject value);
    virtual void emplace(void* container, const void* akey, const void* value);
    virtual char& iteratorValue(const abstract_iterator& it) const;
    virtual void incrementIterator(abstract_iterator& it) const;
    virtual void initializeIterator(abstract_iterator& it) const;
    virtual bool iteratorEquals(const abstract_iterator& it1, const abstract_iterator& it2) const;
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    static void debugStreamSetFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutSetFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInSetFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    friend iterator;

    virtual bool isMulti() const;
    bool equal(const void* a, const void* b);
    void detach(QHashData ** map);
    struct LinkDeleter{
        void operator()(AutoHashAccess*);
    };
    typedef QtSharedPointer::ExternalRefCountWithCustomDeleter<AutoHashAccess, LinkDeleter> ExternalRefCountData;
    QtSharedPointer::ExternalRefCountData *const m_refCount;
    friend QtSharedPointer::ExternalRefCountWithCustomDeleter<AutoHashAccess, QtSharedPointer::NormalDeleter>;
    friend QtSharedPointer::ExternalRefCountWithCustomDeleter<const AutoHashAccess, QtSharedPointer::NormalDeleter>;
    friend QtSharedPointer::ExternalRefCountWithCustomDeleter<AutoMultiHashAccess, QtSharedPointer::NormalDeleter>;
    friend QtSharedPointer::ExternalRefCountWithCustomDeleter<const AutoMultiHashAccess, QtSharedPointer::NormalDeleter>;

    friend class AutoMultiHashAccess;
    friend class AutoSetAccess;
};

template<>
struct QtSharedPointer::ExternalRefCountWithCustomDeleter<AutoHashAccess, QtSharedPointer::NormalDeleter>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    static ExternalRefCountData* create(AutoHashAccess*, QtSharedPointer::NormalDeleter, DestroyerFn);
};

template<>
struct QtSharedPointer::ExternalRefCountWithCustomDeleter<const AutoHashAccess, QtSharedPointer::NormalDeleter>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    static ExternalRefCountData* create(const AutoHashAccess*, QtSharedPointer::NormalDeleter, DestroyerFn);
};

class AutoMultiHashAccess : public virtual AbstractMultiHashAccess, public AutoHashAccess{
    template<typename, typename, typename, bool, bool, bool, typename...>
    friend struct qtjambi_ContainerIterator_cast;
protected:
    AutoMultiHashAccess(const AutoMultiHashAccess&);
public:
    ~AutoMultiHashAccess() override;
    AutoMultiHashAccess(
                    const QMetaType& keyMetaType,
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
                    const QMetaType& valueMetaType,
                    const QtJambiUtils::QHashFunction& valueHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                    PtrOwnerFunction valueOwnerFunction,
                    AbstractContainerAccess::DataType valueDataType
            );
    using AutoHashAccess::take;
    using AutoHashAccess::value;
    using AutoHashAccess::values;
    using AutoHashAccess::key;
    using AutoHashAccess::keys;
    using AutoHashAccess::equal;
    using AutoHashAccess::size;
    using AutoHashAccess::insert;
    using AutoHashAccess::hasOwnerFunction;
    using AutoHashAccess::getOwner;
    using AutoHashAccess::isSharedWith;
    using AutoHashAccess::detach;
    using AutoHashAccess::isDetached;
    using AutoHashAccess::constFind;
    using AutoHashAccess::constEnd;
    using AutoHashAccess::constBegin;
    using AutoHashAccess::find;
    using AutoHashAccess::end;
    using AutoHashAccess::begin;
    using AutoHashAccess::keyBegin;
    using AutoHashAccess::keyEnd;
    using AutoHashAccess::keyValueBegin;
    using AutoHashAccess::keyValueEnd;
    using AutoHashAccess::constKeyValueBegin;
    using AutoHashAccess::constKeyValueEnd;
    using AutoHashAccess::count;
    using AutoHashAccess::contains;
    using AutoHashAccess::clear;
    using AutoHashAccess::hasValueNestedPointers;
    using AutoHashAccess::hasValueNestedContainerAccess;
    using AutoHashAccess::hasKeyNestedPointers;
    using AutoHashAccess::hasKeyNestedContainerAccess;
    using AutoHashAccess::sharedValueNestedContainerAccess;
    using AutoHashAccess::sharedKeyNestedContainerAccess;
    using AutoHashAccess::valueNestedContainerAccess;
    using AutoHashAccess::keyNestedContainerAccess;
    using AutoHashAccess::valueType;
    using AutoHashAccess::keyType;
    using AutoHashAccess::valueMetaType;
    using AutoHashAccess::keyMetaType;
    using AutoHashAccess::registerContainer;
    using AutoHashAccess::keyValueIterator;
    using AutoHashAccess::destructContainer;
    using AutoHashAccess::deleteContainer;
    using AutoHashAccess::createContainer;
    using AutoHashAccess::reserve;
    using AutoHashAccess::capacity;
    static jboolean iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2);
    static void* asIterator(void* iterator, AbstractSequentialConstIteratorAccess::IteratorType iteratorType);
    void assign(void* container, const void* other) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    QMetaType registerContainer(QByteArrayView containerTypeName) override;
    void dispose() override;
    AbstractMultiHashAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    ContainerAndAccessInfo uniqueKeys(JNIEnv *,const ConstContainerInfo&) override;
    void unite(JNIEnv *,const ContainerInfo&,ContainerAndAccessInfo&) override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    qsizetype capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    qsizetype count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyValueBegin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject keyValueEnd(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constKeyValueBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constKeyValueEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject keyEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    qsizetype remove(JNIEnv *,const ContainerInfo&,jobject) override;
    void reserve(JNIEnv *,const ContainerInfo&,qsizetype) override;
    void reserve(void* container, qsizetype size) override;
    qsizetype size(const void*) override;
    qsizetype size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean contains(JNIEnv *,const void*,jobject,jobject) override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    qsizetype count(JNIEnv *,const void*,jobject,jobject) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override;
    qsizetype remove(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    void replace(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override;
    void* createContainer(const void* copy);
    void deleteContainer(void* container);
private:
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>&& iterator);
    jobject createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, key_iterator>&& iterator);
    friend class AutoHashAccess;
    typedef AutoHashAccess::QHashData QHashData;
    struct MultiHashData{
        QHashData* d = nullptr;
        qsizetype m_size = 0;
    };
    size_t m_chainOffset;
    size_t m_chainAlign;
    size_t m_chainSize;
    struct Chain{
        char* value();
        const char* value() const;
        Chain*& next(const AutoMultiHashAccess& access);
        const Chain* next(const AutoMultiHashAccess& access) const;
        qsizetype free(const AutoMultiHashAccess& access);
        void destroy(const AutoMultiHashAccess& access);
    };
    struct multi_iterator{
        QHashData::iterator i;
        Chain** e;
    };
    void dataStreamOut(QDataStream &s, const void *ptr) override;
    void debugStream(QDebug &s, const void *ptr) override;
    bool equalSpanEntries(const char* value1, const char* value2) const override;
    void copySpanEntry(char* value1, const char* value2) const override;
    void eraseSpanEntry(char* value, qsizetype* count = nullptr) const override;
    void emplace(void* container, const void* akey, JNIEnv *env, jobject value) override;
    void emplace(void* container, const void* akey, const void* value) override;
    void initializeIterator(abstract_iterator& it) const override;
    char& iteratorValue(const abstract_iterator& it) const override;
    void incrementIterator(abstract_iterator& it) const override;
    bool iteratorEquals(const abstract_iterator& it1, const abstract_iterator& it2) const override;
    IsBiContainerFunction getIsBiContainerFunction() override;
    bool isMulti() const override;
};

template<>
struct QtSharedPointer::ExternalRefCountWithCustomDeleter<AutoMultiHashAccess, QtSharedPointer::NormalDeleter>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    static ExternalRefCountData* create(AutoMultiHashAccess*, QtSharedPointer::NormalDeleter, DestroyerFn);
};

template<>
struct QtSharedPointer::ExternalRefCountWithCustomDeleter<const AutoMultiHashAccess, QtSharedPointer::NormalDeleter>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    static ExternalRefCountData* create(const AutoMultiHashAccess*, QtSharedPointer::NormalDeleter, DestroyerFn);
};

class AutoSetAccess : public AbstractSetAccess, public AbstractNestedSequentialAccess {
    template<typename, typename, typename, bool, bool, bool, typename...>
    friend struct qtjambi_ContainerIterator_cast;
    QSharedPointer<AutoHashAccess> m_hashAccess;
    QSharedPointer<QtMetaContainerPrivate::QMetaSequenceInterface> m_metaSequenceInterface;
protected:
    AutoSetAccess(const AutoSetAccess&);
public:
    ~AutoSetAccess() override;
    AutoSetAccess(
                    const QMetaType& elementMetaType,
                    const QtJambiUtils::QHashFunction& hashFunction,
                    const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                    PtrOwnerFunction elementOwnerFunction,
                    AbstractContainerAccess::DataType elementDataType
            );
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    QMetaType registerContainer(QByteArrayView containerTypeName) override;
    static constexpr jboolean (&iteratorEquals)(JNIEnv *, const void*, AbstractSequentialConstIteratorAccess::IteratorType, const ConstContainerAndAccessInfo&) = AutoHashAccess::iteratorEquals;
    void dispose() override;
    AutoSetAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    qsizetype capacity(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void insert(void* container, const void* entry) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    jboolean intersects(JNIEnv * env, const void* container, jobject other) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void reserve(JNIEnv * env, const ContainerInfo& container, qsizetype newSize) override;
    void reserve(void* container, qsizetype size) override;
    qsizetype size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    size_t sizeOf() const override;
    size_t alignOf() const override;
    std::unique_ptr<AbstractSetAccess::ElementIterator> elementIterator(const void* container) override;
    std::unique_ptr<AbstractSetAccess::ElementIterator> elementIterator(void* container) override;
private:
    AbstractNestedSequentialAccess* asNested() override;
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(QMetaType newMetaType);
    typedef AutoHashAccess::QHashData QHashData;
    typedef AutoHashAccess::iterator iterator;
};

class PointerRCAutoListAccess : public AutoListAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoListAccess(PointerRCAutoListAccess& _this);
public:
    using AutoListAccess::AutoListAccess;
    PointerRCAutoListAccess* clone() override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AutoSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, qsizetype index, jobject value) override;
    qsizetype removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n, jobject value) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, qsizetype size) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class NestedPointersRCAutoListAccess : public AutoListAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoListAccess(NestedPointersRCAutoListAccess& _this);
public:
    using AutoListAccess::AutoListAccess;
    NestedPointersRCAutoListAccess* clone() override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AutoSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, qsizetype index, jobject value) override;
    qsizetype removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, qsizetype index1, qsizetype index2) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n, jobject value) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, qsizetype size) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class PointerRCAutoSetAccess : public AutoSetAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoSetAccess(PointerRCAutoSetAccess& _this);
public:
    using AutoSetAccess::AutoSetAccess;
    PointerRCAutoSetAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class KeyPointerRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoMapAccess(KeyPointerRCAutoMapAccess& _this);
public:
    using AutoMapAccess::AutoMapAccess;
    KeyPointerRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class KeyPointerRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoMultiMapAccess(KeyPointerRCAutoMultiMapAccess& _this);
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class KeyPointerRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoHashAccess(KeyPointerRCAutoHashAccess& _this);
public:
    using AutoHashAccess::AutoHashAccess;
    KeyPointerRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class KeyPointerRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoMultiHashAccess(KeyPointerRCAutoMultiHashAccess& _this);
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class ValuePointerRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoMapAccess(ValuePointerRCAutoMapAccess& _this);
public:
    using AutoMapAccess::AutoMapAccess;
    ValuePointerRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class ValuePointerRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoMultiMapAccess(ValuePointerRCAutoMultiMapAccess& _this);
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class ValuePointerRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoHashAccess(ValuePointerRCAutoHashAccess& _this);
public:
    using AutoHashAccess::AutoHashAccess;
    ValuePointerRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class ValuePointerRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoMultiHashAccess(ValuePointerRCAutoMultiHashAccess& _this);
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class PointersRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingMapContainer{
private:
    PointersRCAutoMapAccess(PointersRCAutoMapAccess& _this);
public:
    using AutoMapAccess::AutoMapAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    PointersRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class PointersRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingMultiMapContainer{
private:
    PointersRCAutoMultiMapAccess(PointersRCAutoMultiMapAccess& _this);
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class PointersRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingMapContainer{
private:
    PointersRCAutoHashAccess(PointersRCAutoHashAccess& _this);
public:
    using AutoHashAccess::AutoHashAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    PointersRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class PointersRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingMultiMapContainer{
private:
    PointersRCAutoMultiHashAccess(PointersRCAutoMultiHashAccess& _this);
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class NestedPointersRCAutoSetAccess : public AutoSetAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoSetAccess(NestedPointersRCAutoSetAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoSetAccess::AutoSetAccess;
    NestedPointersRCAutoSetAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class NestedPointersRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoMapAccess(NestedPointersRCAutoMapAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoMapAccess::AutoMapAccess;
    NestedPointersRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class NestedPointersRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoMultiMapAccess(NestedPointersRCAutoMultiMapAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class NestedPointersRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoHashAccess(NestedPointersRCAutoHashAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoHashAccess::AutoHashAccess;
    NestedPointersRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    AbstractReferenceCountingContainer* asRC() override;
};

class NestedPointersRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoMultiHashAccess(NestedPointersRCAutoMultiHashAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    qsizetype remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    AbstractReferenceCountingContainer* asRC() override;
};

#endif // CONTAINERACCESS_P_H
