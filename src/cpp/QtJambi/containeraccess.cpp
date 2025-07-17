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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#include <QtCore/QCryptographicHash>
#endif
#include <QtCore/private/qfactoryloader_p.h>

#include "containeraccess_p.h"
#if defined(QTJAMBI_GENERIC_ACCESS)
#include "containeraccess_pair.h"
#include "containeraccess_list.h"
#include "containeraccess_hash.h"
#include "containeraccess_map.h"
#include "containeraccess_multimap.h"
#include "containeraccess_multihash.h"
#include "containeraccess_set.h"
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include "containeraccess_linkedlist.h"
#include "containeraccess_vector.h"
#endif
#endif //defined(QTJAMBI_GENERIC_ACCESS)
#include "qtjambiapi.h"
#include "java_p.h"
#include "containerapi.h"
#include "qtjambi_cast.h"
#include "jobjectwrapper.h"
#include "typemanager_p.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"

Q_GLOBAL_STATIC(QReadWriteLock, gContainerAccessLock)
QReadWriteLock* containerAccessLock(){
    return gContainerAccessLock();
}
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
typedef SecureContainer<QMap<int, QtMetaContainerPrivate::QMetaAssociationInterface>, gContainerAccessLock> MetaAssociationHash;
Q_GLOBAL_STATIC(MetaAssociationHash, gMetaAssociationHash)
typedef SecureContainer<QMap<int, QtMetaContainerPrivate::QMetaSequenceInterface>, gContainerAccessLock> MetaSequenceHash;
Q_GLOBAL_STATIC(MetaSequenceHash, gMetaSequenceHash)

QMap<int, QtMetaContainerPrivate::QMetaAssociationInterface>& metaAssociationHash(){
    return *gMetaAssociationHash;
}

QMap<int, QtMetaContainerPrivate::QMetaSequenceInterface>& metaSequenceHash(){
    return *gMetaSequenceHash();
}
#endif

#if defined(QTJAMBI_GENERIC_ACCESS)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, gContainerAccessLoader, ("io.qt.qtjambi.ContainerAccess", QLatin1String("/containeraccess"), Qt::CaseInsensitive))
typedef SecureContainer<QMap<hash_type, ContainerAccessAPI::SequentialContainerAccessFactory>, gContainerAccessLock> SequentialContainerAccessFactoryHash;
typedef SecureContainer<QMap<hash_type, ContainerAccessAPI::AssociativeContainerAccessFactory>, gContainerAccessLock> AssociativeContainerAccessFactoryHash;
Q_GLOBAL_STATIC(SequentialContainerAccessFactoryHash, gSequentialContainerAccessFactoryHash)
Q_GLOBAL_STATIC(AssociativeContainerAccessFactoryHash, gAssociativeContainerAccessFactoryHash)

inline hash_type qHash(SequentialContainerType containerType, size_t align, size_t size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                       , bool isStatic
#endif
) {
    if(size==0)
        align=0;
    switch(containerType){
    case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        containerType = SequentialContainerType::QVector;
        break;
#endif
    case SequentialContainerType::QQueue:
        containerType = SequentialContainerType::QList;
        break;
    default: break;
    }

#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    return qHashMulti(0, int(containerType), align, size);
#else
    QCryptographicHash hashGenerator(
#ifndef QT_CRYPTOGRAPHICHASH_ONLY_SHA1
                            QCryptographicHash::Md5
#else
                            QCryptographicHash::Sha1
#endif
                );
    hashGenerator.addData(reinterpret_cast<char*>(&containerType), sizeof(containerType));
    hashGenerator.addData(reinterpret_cast<char*>(&align), sizeof(align));
    hashGenerator.addData(reinterpret_cast<char*>(&size), sizeof(size));
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    switch(containerType){
    case SequentialContainerType::QList:
        if(size <= sizeof(void*)){
            hashGenerator.addData(reinterpret_cast<char*>(&isStatic), sizeof(isStatic));
        }
        break;
    default: break;
    }
#endif
    QByteArray result = hashGenerator.result();
    hash_type h;
    if(size_t(result.size())==sizeof(hash_type)){
        h = *reinterpret_cast<hash_type*>(result.data());
    }else{
        h = qHash(result);
    }
    return h;
#endif
}

inline hash_type qHash(AssociativeContainerType containerType, size_t align1, size_t size1, size_t align2, size_t size2)
{
    if(size1==0)
        align1=0;
    if(size2==0)
        align2=0;
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    return qHashMulti(0, int(containerType), align1, size1, align2, size2);
#else
    QCryptographicHash hashGenerator(
#ifndef QT_CRYPTOGRAPHICHASH_ONLY_SHA1
                            QCryptographicHash::Md5
#else
                            QCryptographicHash::Sha1
#endif
                );
    hashGenerator.addData(reinterpret_cast<char*>(&containerType), sizeof(containerType));
    hashGenerator.addData(reinterpret_cast<char*>(&align1), sizeof(align1));
    hashGenerator.addData(reinterpret_cast<char*>(&size1), sizeof(size1));
    hashGenerator.addData(reinterpret_cast<char*>(&align2), sizeof(align2));
    hashGenerator.addData(reinterpret_cast<char*>(&size2), sizeof(size2));
    QByteArray result = hashGenerator.result();
    hash_type h;
    if(size_t(result.size())==sizeof(hash_type)){
        h = *reinterpret_cast<hash_type*>(result.data());
    }else{
        h = qHash(result);
    }
    return h;
#endif
}

ContainerAccessAPI::SequentialContainerAccessFactory findContainerAccessFactory(JNIEnv* env, SequentialContainerType containerType, size_t align, size_t size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                  bool isStatic,
#endif
                                                                  JavaException& occurredException){
    if(size!=0 && align==0){
        align = qMin<size_t>(size, alignof(std::max_align_t));
    }
    hash_type id = qHash(containerType, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                , isStatic
#endif
                        );
    QWriteLocker lock(gContainerAccessLock());
    ContainerAccessAPI::SequentialContainerAccessFactory result = gSequentialContainerAccessFactoryHash->value(id, nullptr);
    if(!result && size==0){
        size = sizeof(void*);
        align = alignof(void*);
        result = gSequentialContainerAccessFactoryHash->value(id = qHash(containerType, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                    ,isStatic
#endif
                                                    ), nullptr);
    }
    if(!result){
        QString containerName;
        switch(containerType){
        case SequentialContainerType::QSet: containerName = "QSet"; break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        case SequentialContainerType::QVector: containerName = "QVector"; break;
        case SequentialContainerType::QLinkedList: containerName = "QLinkedList"; break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList: containerName = "QList"; break;
        }
        QString lib = QString("%1Access_S%2_A%3").arg(containerName, QString::number(size), QString::number(align));
        Java::QtJambi::LibraryUtility::extractContainerAccessLib(env, qtjambi_cast<jstring>(env, lib));
        gContainerAccessLoader->update();
        int idx = gContainerAccessLoader->indexOf(lib);
        if(idx>=0){
            gContainerAccessLoader->instance(idx);
            if(!(result = gSequentialContainerAccessFactoryHash->value(id, nullptr))){
                occurredException = JavaException(env, Java::Runtime::RuntimeException::newInstance(env, qtjambi_cast<jstring>(env, QStringLiteral("Unable to load container access plugin %1.").arg(lib))));
            }
        }else{
            occurredException = JavaException(env, Java::Runtime::MissingResourceException::newInstance(env,
                                                                                qtjambi_cast<jstring>(env, QStringLiteral("Container access plugin %1Access_S%2_A%3 missing.").arg(containerName, QString::number(size), QString::number(align))),
                                                                                qtjambi_cast<jstring>(env, containerName),
                                                                                env->NewStringUTF("")));
        }
    }
    return result;
}

ContainerAccessAPI::AssociativeContainerAccessFactory findContainerAccessFactory(JNIEnv* env, AssociativeContainerType containerType, size_t align1, size_t size1, size_t align2, size_t size2, JavaException& occurredException){
    if(size1!=0 && align1==0){
        align1 = qMin<size_t>(size1, alignof(std::max_align_t));
    }
    if(size2!=0 && align2==0){
        align2 = qMin<size_t>(size2, alignof(std::max_align_t));
    }
    hash_type id = qHash(containerType, align1, size1, align2, size2);
    QWriteLocker lock(gContainerAccessLock());
    ContainerAccessAPI::AssociativeContainerAccessFactory result = gAssociativeContainerAccessFactoryHash->value(id, nullptr);
    if(!result && size1==0)
        result = gAssociativeContainerAccessFactoryHash->value(id = qHash(containerType, alignof(void*), sizeof(void*), align2, size2), nullptr);
    if(!result && size2==0){
        align2 = alignof(void*);
        size2 = sizeof(void*);
        result = gAssociativeContainerAccessFactoryHash->value(id = qHash(containerType, align1, size1, align2, size2), nullptr);
    }
    if(!result && size1==0){
        align1 = alignof(void*);
        size1 = sizeof(void*);
        result = gAssociativeContainerAccessFactoryHash->value(id = qHash(containerType, align1, size1, align2, size2), nullptr);
    }
    if(!result){
        QString containerName;
        switch(containerType){
        case AssociativeContainerType::QMap: containerName = "QMap"; break;
        case AssociativeContainerType::QHash: containerName = "QHash"; break;
        case AssociativeContainerType::QMultiMap: containerName = "QMultiMap"; break;
        case AssociativeContainerType::QMultiHash: containerName = "QMultiHash"; break;
        case AssociativeContainerType::QPair: containerName = "QPair"; break;
        }
        QString lib = QString("%1Access_S%2_A%3_S%4_A%5").arg(containerName, QString::number(size1), QString::number(align1), QString::number(size2), QString::number(align2));
        Java::QtJambi::LibraryUtility::extractContainerAccessLib(env, qtjambi_cast<jstring>(env, lib));
        gContainerAccessLoader->update();
        int idx = gContainerAccessLoader->indexOf(lib);
        if(idx>=0){
            gContainerAccessLoader->instance(idx);
            if(!(result = gAssociativeContainerAccessFactoryHash->value(id, nullptr))){
                occurredException = JavaException(env, Java::Runtime::RuntimeException::newInstance(env, qtjambi_cast<jstring>(env, QStringLiteral("Unable to load container access plugin %1.").arg(lib))));
            }
        }else{
            occurredException = JavaException(env, Java::Runtime::MissingResourceException::newInstance(env,
                                                                                qtjambi_cast<jstring>(env, QStringLiteral("Container access plugin %1Access_S%2_A%3_S%4_A%5 missing.").arg(containerName, QString::number(size1), QString::number(align1), QString::number(size2), QString::number(align2))),
                                                                                qtjambi_cast<jstring>(env, containerName),
                                                                                env->NewStringUTF("")));
        }
    }
    return result;
}

void ContainerAccessAPI::registerAccessFactory(SequentialContainerType containerType, size_t align, size_t size, bool
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     isStatic
#endif
                                                     , SequentialContainerAccessFactory factory){
    QWriteLocker lock(gContainerAccessLock());
    gSequentialContainerAccessFactoryHash->insert(qHash(containerType, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                              , isStatic
#endif
                                    ), factory);
}

void ContainerAccessAPI::registerAccessFactory(AssociativeContainerType containerType, size_t align1, size_t size1, size_t align2, size_t size2, AssociativeContainerAccessFactory factory){
    QWriteLocker lock(gContainerAccessLock());
    gAssociativeContainerAccessFactoryHash->insert(qHash(containerType, align1, size1, align2, size2), factory);
}

#endif //defined(QTJAMBI_GENERIC_ACCESS)

QPair<void*,AbstractContainerAccess*> ContainerAPI::fromNativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        QtJambiLink *lnk = reinterpret_cast<QtJambiLink *>(nativeId);
        return {lnk->pointer(), lnk->containerAccess()};
    }else{
        return {nullptr,nullptr};
    }
}

QPair<void*,AbstractContainerAccess*> ContainerAPI::fromJavaOwner(JNIEnv *env, jobject object){
    if(QSharedPointer<QtJambiLink> lnk = QtJambiLink::findLinkForJavaObject(env, object)){
        if(lnk->containerAccess()){
            return {lnk->pointer(), lnk->containerAccess()};
        }
    }
    return {nullptr,nullptr};
}

AbstractContainerAccess::DataType dataType(const QMetaType& metaType, const QSharedPointer<AbstractContainerAccess>& access){
    if(access){
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        if(dynamic_cast<AbstractSpanAccess*>(access.data()))
            return AbstractContainerAccess::Pointer;
#endif
        if(metaType.flags().testFlag(QMetaType::IsPointer))
            return AbstractContainerAccess::Pointer;
        return AbstractContainerAccess::Value;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    }else if(metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)){
#else
    }else if(metaType.flags().testFlag(QMetaType::PointerToQObject)){
#endif
        return AbstractContainerAccess::PointerToQObject;
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if(const std::type_info* typeId = getTypeByMetaType(metaType)){
            if(registeredFunctionalResolver(*typeId)){
                return AbstractContainerAccess::FunctionPointer;
            }
        }
        QByteArray typeName(metaType.name());
        if(typeName.contains("(*)")){
            return AbstractContainerAccess::FunctionPointer;
        }else if(typeName.endsWith("*")){
            return AbstractContainerAccess::Pointer;
#else
        if(metaType.flags().testFlag(QMetaType::IsPointer)){
            if(const std::type_info* typeId = getTypeByMetaType(metaType)){
                if(registeredFunctionalResolver(*typeId)){
                    return AbstractContainerAccess::FunctionPointer;
                }
            }else if(QByteArrayView(metaType.name()).contains("(*)")){
                return AbstractContainerAccess::FunctionPointer;
            }
            return AbstractContainerAccess::Pointer;
#endif
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
    template<typename Functor, bool = std::is_assignable<FunctionPointer&, Functor>::value, bool = std::is_same<Functor, OwnerFunctional>::value>
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

    template<typename Functor
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             , typename = std::enable_if_t<std::is_invocable_r_v<const QObject*, Functor, const void *>>
#endif
             >
    OwnerFunctional(Functor&& functor) noexcept
        : OwnerFunctional(Data<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::from(std::forward<Functor>(functor))){}

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

template<typename AccessType>
struct ContainerAccessWrapper{
};

template<>
struct ContainerAccessWrapper<AbstractListAccess>{
    typedef WrapperListAccess type;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
template<>
struct ContainerAccessWrapper<AbstractLinkedListAccess>{
    typedef WrapperLinkedListAccess type;
};

template<>
struct ContainerAccessWrapper<AbstractVectorAccess>{
    typedef WrapperVectorAccess type;
};
#endif

template<>
struct ContainerAccessWrapper<AbstractSetAccess>{
    typedef WrapperSetAccess type;
};

template<>
struct ContainerAccessWrapper<AbstractHashAccess>{
    typedef WrapperHashAccess type;
};

template<>
struct ContainerAccessWrapper<AbstractMapAccess>{
    typedef WrapperMapAccess type;
};

template<>
struct ContainerAccessWrapper<AbstractMultiHashAccess>{
    typedef WrapperMultiHashAccess type;
};

template<>
struct ContainerAccessWrapper<AbstractMultiMapAccess>{
    typedef WrapperMultiMapAccess type;
};

WrapperListAccess::WrapperListAccess(AbstractListAccess* containerAccess)
    : AbstractListAccess(), m_containerAccess(containerAccess) {}

WrapperListAccess::~WrapperListAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractListAccess* WrapperListAccess::clone() {
    return m_containerAccess->clone();
}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
AbstractSpanAccess* WrapperListAccess::createSpanAccess(bool isConst) {
    return m_containerAccess->createSpanAccess(isConst);
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)


const QObject* WrapperListAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperListAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractListAccess* WrapperListAccess::wrappedAccess(){
    return m_containerAccess;
}

bool WrapperListAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

std::unique_ptr<AbstractListAccess::ElementIterator> WrapperListAccess::elementIterator(const void* container) {
    return m_containerAccess->elementIterator(container);
}

std::unique_ptr<AbstractListAccess::ElementIterator> WrapperListAccess::elementIterator(void* container) {
    return m_containerAccess->elementIterator(container);
}

void WrapperListAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperListAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

void WrapperListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

bool WrapperListAccess::append(void* container, const void* entry) {
    return m_containerAccess->append(container, entry);
}

void WrapperListAccess::dispose()  { delete this; }

void WrapperListAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void WrapperListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

size_t WrapperListAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

void* WrapperListAccess::constructContainer(void* container) {
    return m_containerAccess->constructContainer(container);
}

void* WrapperListAccess::constructContainer(void* container, const void* copyOf) {
    return m_containerAccess->constructContainer(container, copyOf);
}

void* WrapperListAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperListAccess::constructContainer(void* container, void* moved) {
    return m_containerAccess->constructContainer(container, moved);
}

void* WrapperListAccess::constructContainer(JNIEnv * env, void* container, const ContainerAndAccessInfo& move) {
    return m_containerAccess->constructContainer(env, container, move);
}
#endif

bool WrapperListAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperListAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

const QMetaType& WrapperListAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

AbstractContainerAccess::DataType WrapperListAccess::elementType() {
    return m_containerAccess->elementType();
}

AbstractContainerAccess* WrapperListAccess::elementNestedContainerAccess() {
    return m_containerAccess->elementNestedContainerAccess();
}

bool WrapperListAccess::hasNestedContainerAccess() {
    return m_containerAccess->hasNestedContainerAccess();
}

bool WrapperListAccess::hasNestedPointers() {
    return m_containerAccess->hasNestedPointers();
}

void WrapperListAccess::appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    m_containerAccess->appendList(env, container, containerInfo);
}

jobject WrapperListAccess::at(JNIEnv * env, const void* container, jint index) {
    return m_containerAccess->at(env, container, index);
}

const void* WrapperListAccess::at(const void* container, qsizetype index) {
    return m_containerAccess->at(container, index);
}

void* WrapperListAccess::at(void* container, qsizetype index) {
    return m_containerAccess->at(container, index);
}

jobject WrapperListAccess::value(JNIEnv * env, const void* container, jint index) {
    return m_containerAccess->value(env, container, index);
}

jobject WrapperListAccess::value(JNIEnv * env, const void* container, jint index, jobject defaultValue) {
    return m_containerAccess->value(env, container, index, defaultValue);
}

void WrapperListAccess::swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) {
    m_containerAccess->swapItemsAt(env, container, index1, index2);
}

jboolean WrapperListAccess::startsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->startsWith(env, container, value);
}

qsizetype WrapperListAccess::size(const void* container) {
    return m_containerAccess->size(container);
}

jint WrapperListAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

void WrapperListAccess::reserve(JNIEnv * env, const ContainerInfo& container, jint size) {
    return m_containerAccess->reserve(env, container, size);
}

void WrapperListAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    m_containerAccess->replace(env, container, index, value);
}

void WrapperListAccess::replace(void* container, qsizetype index, const void* value) {
    m_containerAccess->replace(container, index, value);
}

void WrapperListAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    m_containerAccess->remove(env, container, index, n);
}

void WrapperListAccess::remove(void* container, qsizetype index, qsizetype n) {
    m_containerAccess->remove(container, index, n);
}

jint WrapperListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    return m_containerAccess->removeAll(env, container, value);
}

jboolean WrapperListAccess::equal(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->equal(env, container, other);
}

void WrapperListAccess::move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) {
    m_containerAccess->move(env, container, index1, index2);
}

ContainerAndAccessInfo WrapperListAccess::mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) {
    return m_containerAccess->mid(env, container, index1, index2);
}

jint WrapperListAccess::lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) {
    return m_containerAccess->lastIndexOf(env, container, value, index);
}

void WrapperListAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    m_containerAccess->insert(env, container, index, n, value);
}

void WrapperListAccess::insert(void* container, qsizetype index, qsizetype n, const void* value) {
    m_containerAccess->insert(container, index, n, value);
}

jint WrapperListAccess::indexOf(JNIEnv * env, const void* container, jobject value, jint index) {
    return m_containerAccess->indexOf(env, container, value, index);
}

jboolean WrapperListAccess::endsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->endsWith(env, container, value);
}

jobject WrapperListAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->begin(env, container);
}

jobject WrapperListAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperListAccess::end(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->end(env, container);
}

jobject WrapperListAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

jint WrapperListAccess::count(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->count(env, container, value);
}

jboolean WrapperListAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

void WrapperListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
jint WrapperListAccess::capacity(JNIEnv * env, const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperListAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) {
    m_containerAccess->fill(env, container, value, size);
}

void WrapperListAccess::resize(JNIEnv * env, const ContainerInfo& container, jint newSize) {
    return m_containerAccess->resize(env, container, newSize);
}

void WrapperListAccess::resize(void* container, qsizetype newSize) {
    return m_containerAccess->resize(container, newSize);
}

void WrapperListAccess::squeeze(JNIEnv * env, const ContainerInfo& container) {
    return m_containerAccess->squeeze(env, container);
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
WrapperVectorAccess::WrapperVectorAccess(AbstractVectorAccess* containerAccess)
    : AbstractVectorAccess(), m_containerAccess(containerAccess) {}

WrapperVectorAccess::~WrapperVectorAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractVectorAccess* WrapperVectorAccess::clone() {
    return m_containerAccess->clone();
}

const QObject* WrapperVectorAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperVectorAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractVectorAccess* WrapperVectorAccess::wrappedAccess(){
    return m_containerAccess;
}

bool WrapperVectorAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

void WrapperVectorAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperVectorAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

std::unique_ptr<AbstractVectorAccess::ElementIterator> WrapperVectorAccess::elementIterator(const void* container) {
    return m_containerAccess->elementIterator(container);
}

std::unique_ptr<AbstractVectorAccess::ElementIterator> WrapperVectorAccess::elementIterator(void* container) {
    return m_containerAccess->elementIterator(container);
}

void WrapperVectorAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

void WrapperVectorAccess::dispose()  { delete this; }

void WrapperVectorAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

size_t WrapperVectorAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

void* WrapperVectorAccess::constructContainer(void* container) {
    return m_containerAccess->constructContainer(container);
}

void* WrapperVectorAccess::constructContainer(void* container, const void* copyOf) {
    return m_containerAccess->constructContainer(container, copyOf);
}

void* WrapperVectorAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

bool WrapperVectorAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperVectorAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

const QMetaType& WrapperVectorAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

AbstractContainerAccess* WrapperVectorAccess::elementNestedContainerAccess() {
    return m_containerAccess->elementNestedContainerAccess();
}

bool WrapperVectorAccess::hasNestedContainerAccess() {
    return m_containerAccess->hasNestedContainerAccess();
}

bool WrapperVectorAccess::hasNestedPointers() {
    return m_containerAccess->hasNestedPointers();
}

AbstractContainerAccess::DataType WrapperVectorAccess::elementType() {
    return m_containerAccess->elementType();
}

void WrapperVectorAccess::appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    m_containerAccess->appendVector(env, container, containerInfo);
}

jobject WrapperVectorAccess::at(JNIEnv * env, const void* container, jint index) {
    return m_containerAccess->at(env, container, index);
}

jobject WrapperVectorAccess::value(JNIEnv * env, const void* container, jint index) {
    return m_containerAccess->value(env, container, index);
}

jobject WrapperVectorAccess::value(JNIEnv * env, const void* container, jint index, jobject defaultValue) {
    return m_containerAccess->value(env, container, index, defaultValue);
}

void WrapperVectorAccess::swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) {
    m_containerAccess->swapItemsAt(env, container, index1, index2);
}

jboolean WrapperVectorAccess::startsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->startsWith(env, container, value);
}

jint WrapperVectorAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

void WrapperVectorAccess::reserve(JNIEnv * env, const ContainerInfo& container, jint size) {
    return m_containerAccess->reserve(env, container, size);
}

void WrapperVectorAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    m_containerAccess->replace(env, container, index, value);
}

jint WrapperVectorAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    return m_containerAccess->removeAll(env, container, value);
}

jboolean WrapperVectorAccess::equal(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->equal(env, container, other);
}

void WrapperVectorAccess::move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) {
    m_containerAccess->move(env, container, index1, index2);
}

ContainerAndAccessInfo WrapperVectorAccess::mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) {
    return m_containerAccess->mid(env, container, index1, index2);
}

jint WrapperVectorAccess::lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) {
    return m_containerAccess->lastIndexOf(env, container, value, index);
}

jint WrapperVectorAccess::indexOf(JNIEnv * env, const void* container, jobject value, jint index) {
    return m_containerAccess->indexOf(env, container, value, index);
}

jboolean WrapperVectorAccess::endsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->endsWith(env, container, value);
}

jobject WrapperVectorAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->begin(env, container);
}

jobject WrapperVectorAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperVectorAccess::end(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->end(env, container);
}

jobject WrapperVectorAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

jint WrapperVectorAccess::count(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->count(env, container, value);
}

jboolean WrapperVectorAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

void WrapperVectorAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

jint WrapperVectorAccess::capacity(JNIEnv * env, const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperVectorAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) {
    m_containerAccess->fill(env, container, value, size);
}

void WrapperVectorAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    m_containerAccess->remove(env, container, index, n);
}

void WrapperVectorAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    m_containerAccess->insert(env, container, index, n, value);
}

void WrapperVectorAccess::resize(JNIEnv * env, const ContainerInfo& container, jint newSize) {
    return m_containerAccess->resize(env, container, newSize);
}

void WrapperVectorAccess::squeeze(JNIEnv * env, const ContainerInfo& container) {
    return m_containerAccess->squeeze(env, container);
}

void WrapperVectorAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

WrapperLinkedListAccess::WrapperLinkedListAccess(AbstractLinkedListAccess* containerAccess)
    : AbstractLinkedListAccess(), m_containerAccess(containerAccess) {}

WrapperLinkedListAccess::~WrapperLinkedListAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractLinkedListAccess* WrapperLinkedListAccess::clone() {
    return m_containerAccess->clone();
}

const QObject* WrapperLinkedListAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperLinkedListAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractLinkedListAccess* WrapperLinkedListAccess::wrappedAccess(){
    return m_containerAccess;
}

bool WrapperLinkedListAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

void WrapperLinkedListAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void WrapperLinkedListAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperLinkedListAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

void WrapperLinkedListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

void WrapperLinkedListAccess::dispose() { delete this; }

void WrapperLinkedListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

size_t WrapperLinkedListAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

void* WrapperLinkedListAccess::constructContainer(void* placement) {
    return m_containerAccess->constructContainer(placement);
}

void* WrapperLinkedListAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

void* WrapperLinkedListAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

bool WrapperLinkedListAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperLinkedListAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

std::unique_ptr<AbstractLinkedListAccess::ElementIterator> WrapperLinkedListAccess::elementIterator(const void* container) {
    return m_containerAccess->elementIterator(container);
}

std::unique_ptr<AbstractLinkedListAccess::ElementIterator> WrapperLinkedListAccess::elementIterator(void* container) {
    return m_containerAccess->elementIterator(container);
}

const QMetaType& WrapperLinkedListAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

AbstractContainerAccess* WrapperLinkedListAccess::elementNestedContainerAccess() {
    return m_containerAccess->elementNestedContainerAccess();
}

bool WrapperLinkedListAccess::hasNestedContainerAccess() {
    return m_containerAccess->hasNestedContainerAccess();
}

bool WrapperLinkedListAccess::hasNestedPointers() {
    return m_containerAccess->hasNestedPointers();
}

AbstractContainerAccess::DataType WrapperLinkedListAccess::elementType() {
    return m_containerAccess->elementType();
}

void WrapperLinkedListAccess::append(JNIEnv * env, const ContainerInfo& container, jobject value) {
    m_containerAccess->append(env, container, value);
}

jobject WrapperLinkedListAccess::first(JNIEnv * env, const void* container) {
    return m_containerAccess->first(env, container);
}

jobject WrapperLinkedListAccess::last(JNIEnv * env, const void* container) {
    return m_containerAccess->last(env, container);
}

jobject WrapperLinkedListAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->begin(env, container);
}

jobject WrapperLinkedListAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperLinkedListAccess::end(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->end(env, container);
}

jobject WrapperLinkedListAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

void WrapperLinkedListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperLinkedListAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperLinkedListAccess::count(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->count(env, container, value);
}

jboolean WrapperLinkedListAccess::endsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->endsWith(env, container, value);
}

jboolean WrapperLinkedListAccess::equal(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->equal(env, container, other);
}

void WrapperLinkedListAccess::prepend(JNIEnv * env, const ContainerInfo& container, jobject value) {
    return m_containerAccess->prepend(env, container, value);
}

void WrapperLinkedListAccess::removeFirst(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->removeFirst(env, container);
}

jint WrapperLinkedListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    return m_containerAccess->removeAll(env, container, value);
}

void WrapperLinkedListAccess::removeLast(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->removeLast(env, container);
}

jboolean WrapperLinkedListAccess::removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) {
    return m_containerAccess->removeOne(env, container, value);
}

jint WrapperLinkedListAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

jboolean WrapperLinkedListAccess::startsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->startsWith(env, container, value);
}

jobject WrapperLinkedListAccess::takeFirst(JNIEnv * env, const ContainerInfo& container) {
    return m_containerAccess->takeFirst(env, container);
}

jobject WrapperLinkedListAccess::takeLast(JNIEnv * env, const ContainerInfo& container) {
    return m_containerAccess->takeLast(env, container);
}
#endif

WrapperSetAccess::WrapperSetAccess(AbstractSetAccess* containerAccess)
    : AbstractSetAccess(), m_containerAccess(containerAccess) {}

WrapperSetAccess::~WrapperSetAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractSetAccess* WrapperSetAccess::clone() {
    return m_containerAccess->clone();
}

const QObject* WrapperSetAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperSetAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractSetAccess* WrapperSetAccess::wrappedAccess(){
    return m_containerAccess;
}

bool WrapperSetAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

void WrapperSetAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperSetAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

void WrapperSetAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

void WrapperSetAccess::dispose() { delete this; }

void* WrapperSetAccess::constructContainer(void* placement) {
    return m_containerAccess->constructContainer(placement);
}

void* WrapperSetAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

void* WrapperSetAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperSetAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
}

void* WrapperSetAccess::constructContainer(JNIEnv * env, void* container, const ContainerAndAccessInfo& move) {
    return m_containerAccess->constructContainer(env, container, move);
}
#endif

void WrapperSetAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void WrapperSetAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

size_t WrapperSetAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

bool WrapperSetAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperSetAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

const QMetaType& WrapperSetAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

AbstractContainerAccess* WrapperSetAccess::elementNestedContainerAccess() {
    return m_containerAccess->elementNestedContainerAccess();
}

bool WrapperSetAccess::hasNestedContainerAccess() {
    return m_containerAccess->hasNestedContainerAccess();
}

bool WrapperSetAccess::hasNestedPointers() {
    return m_containerAccess->hasNestedPointers();
}

AbstractContainerAccess::DataType WrapperSetAccess::elementType() {
    return m_containerAccess->elementType();
}

jint WrapperSetAccess::capacity(JNIEnv * env, const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperSetAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperSetAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jobject WrapperSetAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperSetAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

void WrapperSetAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject value) {
    m_containerAccess->insert(env, container, value);
}

void WrapperSetAccess::intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    m_containerAccess->intersect(env, container, other);
}

jboolean WrapperSetAccess::intersects(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->intersects(env, container, other);
}

jboolean WrapperSetAccess::equal(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->equal(env, container, other);
}

jboolean WrapperSetAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject value) {
    return m_containerAccess->remove(env, container, value);
}

void WrapperSetAccess::reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) {
    m_containerAccess->reserve(env, container, newSize);
}

jint WrapperSetAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

qsizetype WrapperSetAccess::size(const void* container) {
    return m_containerAccess->size(container);
}

void WrapperSetAccess::subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    m_containerAccess->subtract(env, container, other);
}

void WrapperSetAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    m_containerAccess->unite(env, container, other);
}

ContainerAndAccessInfo WrapperSetAccess::values(JNIEnv * env, const ConstContainerInfo& container) {
    return m_containerAccess->values(env, container);
}

std::unique_ptr<AbstractSetAccess::ElementIterator> WrapperSetAccess::elementIterator(const void* container) {
    return m_containerAccess->elementIterator(container);
}

std::unique_ptr<AbstractSetAccess::ElementIterator> WrapperSetAccess::elementIterator(void* container) {
    return m_containerAccess->elementIterator(container);
}

WrapperMapAccess::WrapperMapAccess(AbstractMapAccess* containerAccess)
    : AbstractMapAccess(), m_containerAccess(containerAccess) {}

WrapperMapAccess::~WrapperMapAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractMapAccess* WrapperMapAccess::clone() {
    return m_containerAccess->clone();
}

const QObject* WrapperMapAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperMapAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractMapAccess* WrapperMapAccess::wrappedAccess(){
    return m_containerAccess;
}

std::unique_ptr<AbstractMapAccess::KeyValueIterator> WrapperMapAccess::keyValueIterator(const void* container) {
    return m_containerAccess->keyValueIterator(container);
}
std::unique_ptr<AbstractMapAccess::KeyValueIterator> WrapperMapAccess::keyValueIterator(void* container) {
    return m_containerAccess->keyValueIterator(container);
}

bool WrapperMapAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

void WrapperMapAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperMapAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

void WrapperMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

void WrapperMapAccess::dispose()  { delete this; }

void WrapperMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

void WrapperMapAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperMapAccess::constructContainer(void* placement) {
    return m_containerAccess->constructContainer(placement);
}

void* WrapperMapAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

void* WrapperMapAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperMapAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
}

void* WrapperMapAccess::constructContainer(JNIEnv * env, void* container, const ContainerAndAccessInfo& move) {
    return m_containerAccess->constructContainer(env, container, move);
}
#endif

size_t WrapperMapAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

bool WrapperMapAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperMapAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

const QMetaType& WrapperMapAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperMapAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

AbstractContainerAccess::DataType WrapperMapAccess::keyType() {
    return m_containerAccess->keyType();
}

AbstractContainerAccess::DataType WrapperMapAccess::valueType() {
    return m_containerAccess->valueType();
}

AbstractContainerAccess* WrapperMapAccess::keyNestedContainerAccess() {
    return m_containerAccess->keyNestedContainerAccess();
}

AbstractContainerAccess* WrapperMapAccess::valueNestedContainerAccess() {
    return m_containerAccess->valueNestedContainerAccess();
}

bool WrapperMapAccess::hasKeyNestedContainerAccess() {
    return m_containerAccess->hasKeyNestedContainerAccess();
}
bool WrapperMapAccess::hasValueNestedContainerAccess() {
    return m_containerAccess->hasValueNestedContainerAccess();
}
bool WrapperMapAccess::hasKeyNestedPointers() {
    return m_containerAccess->hasKeyNestedPointers();
}
bool WrapperMapAccess::hasValueNestedPointers() {
    return m_containerAccess->hasValueNestedPointers();
}

jobject WrapperMapAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->begin(env, container);
}

jobject WrapperMapAccess::end(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->end(env, container);
}

jobject WrapperMapAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->find(env, container, key);
}

jobject WrapperMapAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperMapAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

jobject WrapperMapAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constFind(env, container, key);
}

void WrapperMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperMapAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperMapAccess::count(JNIEnv * env, const void* container, jobject key)  {
     return m_containerAccess->count(env, container, key);
}

jobject WrapperMapAccess::first(JNIEnv * env, const void* container)  {
     return m_containerAccess->first(env, container);
}

jobject WrapperMapAccess::firstKey(JNIEnv * env, const void* container)  {
     return m_containerAccess->firstKey(env, container);
}

void WrapperMapAccess::insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperMapAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
     return m_containerAccess->key(env, container, value, defaultKey);
}

ContainerAndAccessInfo WrapperMapAccess::keys(JNIEnv * env, const ConstContainerInfo& container)  {
     return m_containerAccess->keys(env, container);
}

ContainerAndAccessInfo WrapperMapAccess::keys(JNIEnv * env, const ConstContainerInfo& container, jobject value)  {
     return m_containerAccess->keys(env, container, value);
}

jobject WrapperMapAccess::last(JNIEnv * env, const void* container)  {
     return m_containerAccess->last(env, container);
}

jobject WrapperMapAccess::lastKey(JNIEnv * env, const void* container)  {
     return m_containerAccess->lastKey(env, container);
}

jobject WrapperMapAccess::constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constLowerBound(env, container, key);
}

jboolean WrapperMapAccess::equal(JNIEnv * env, const void* container, jobject other)  {
     return m_containerAccess->equal(env, container, other);
}

jint WrapperMapAccess::remove(JNIEnv * env, const ContainerInfo& container,jobject key)  {
    return m_containerAccess->remove(env, container, key);
}

jint WrapperMapAccess::size(JNIEnv * env, const void* container)  {
     return m_containerAccess->size(env, container);
}

qsizetype WrapperMapAccess::size(const void* container){
    return m_containerAccess->size(container);
}

jobject WrapperMapAccess::take(JNIEnv *env, const ContainerInfo& container,jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperMapAccess::constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constUpperBound(env, container, key);
}

jobject WrapperMapAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
     return m_containerAccess->value(env, container, key, defaultValue);
}

ContainerAndAccessInfo WrapperMapAccess::values(JNIEnv * env, const ConstContainerInfo& container)  {
     return m_containerAccess->values(env, container);
}

bool WrapperMapAccess::keyLessThan(JNIEnv *env, jobject key1, jobject key2)  {
    return m_containerAccess->keyLessThan(env, key1, key2);
}

bool WrapperMapAccess::contains(const void* container, const void* key) {
    return m_containerAccess->contains(container, key);
}

void WrapperMapAccess::insert(void* container,const void* key, const void* value) {
    m_containerAccess->insert(container, key, value);
}

const void* WrapperMapAccess::value(const void* container, const void* key, const void* defaultValue) {
    return m_containerAccess->value(container, key, defaultValue);
}

WrapperMultiMapAccess::WrapperMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : AbstractMultiMapAccess(), m_containerAccess(containerAccess) {}

WrapperMultiMapAccess::~WrapperMultiMapAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

std::unique_ptr<AbstractMapAccess::KeyValueIterator> WrapperMultiMapAccess::keyValueIterator(const void* container) { return m_containerAccess->keyValueIterator(container); }
std::unique_ptr<AbstractMapAccess::KeyValueIterator> WrapperMultiMapAccess::keyValueIterator(void* container) { return m_containerAccess->keyValueIterator(container); }

AbstractMultiMapAccess* WrapperMultiMapAccess::clone() {
    return m_containerAccess->clone();
}

const QObject* WrapperMultiMapAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperMultiMapAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractMultiMapAccess* WrapperMultiMapAccess::wrappedAccess(){
    return m_containerAccess;
}

bool WrapperMultiMapAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

void WrapperMultiMapAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperMultiMapAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

void WrapperMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

void WrapperMultiMapAccess::dispose()  { delete this; }

void WrapperMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

void WrapperMultiMapAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperMultiMapAccess::constructContainer(void* placement) {
    return m_containerAccess->constructContainer(placement);
}

void* WrapperMultiMapAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

void* WrapperMultiMapAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperMultiMapAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
}

void* WrapperMultiMapAccess::constructContainer(JNIEnv * env, void* container, const ContainerAndAccessInfo& move) {
    return m_containerAccess->constructContainer(env, container, move);
}
#endif

size_t WrapperMultiMapAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

bool WrapperMultiMapAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperMultiMapAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

const QMetaType& WrapperMultiMapAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperMultiMapAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

AbstractContainerAccess::DataType WrapperMultiMapAccess::keyType() {
    return m_containerAccess->keyType();
}

AbstractContainerAccess::DataType WrapperMultiMapAccess::valueType() {
    return m_containerAccess->valueType();
}

AbstractContainerAccess* WrapperMultiMapAccess::keyNestedContainerAccess() {
    return m_containerAccess->keyNestedContainerAccess();
}

AbstractContainerAccess* WrapperMultiMapAccess::valueNestedContainerAccess() {
    return m_containerAccess->valueNestedContainerAccess();
}

bool WrapperMultiMapAccess::hasKeyNestedContainerAccess() {
    return m_containerAccess->hasKeyNestedContainerAccess();
}
bool WrapperMultiMapAccess::hasValueNestedContainerAccess() {
    return m_containerAccess->hasValueNestedContainerAccess();
}
bool WrapperMultiMapAccess::hasKeyNestedPointers() {
    return m_containerAccess->hasKeyNestedPointers();
}
bool WrapperMultiMapAccess::hasValueNestedPointers() {
    return m_containerAccess->hasValueNestedPointers();
}

void WrapperMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperMultiMapAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperMultiMapAccess::count(JNIEnv * env,const void* container, jobject key) {
    return m_containerAccess->count(env, container, key);
}

jobject WrapperMultiMapAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->begin(env, container);
}

jobject WrapperMultiMapAccess::end(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->end(env, container);
}

jobject WrapperMultiMapAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->find(env, container, key);
}

jobject WrapperMultiMapAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperMultiMapAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

jobject WrapperMultiMapAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constFind(env, container, key);
}

jobject WrapperMultiMapAccess::first(JNIEnv * env, const void* container)  {
     return m_containerAccess->first(env, container);
}

jobject WrapperMultiMapAccess::firstKey(JNIEnv * env, const void* container)  {
     return m_containerAccess->firstKey(env, container);
}

void WrapperMultiMapAccess::insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperMultiMapAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
     return m_containerAccess->key(env, container, value, defaultKey);
}

ContainerAndAccessInfo WrapperMultiMapAccess::keys(JNIEnv * env, const ConstContainerInfo& container)  {
     return m_containerAccess->keys(env, container);
}

ContainerAndAccessInfo WrapperMultiMapAccess::keys(JNIEnv * env, const ConstContainerInfo& container, jobject value)  {
     return m_containerAccess->keys(env, container, value);
}

jobject WrapperMultiMapAccess::last(JNIEnv * env, const void* container)  {
     return m_containerAccess->last(env, container);
}

jobject WrapperMultiMapAccess::lastKey(JNIEnv * env, const void* container)  {
     return m_containerAccess->lastKey(env, container);
}

jobject WrapperMultiMapAccess::constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constLowerBound(env, container, key);
}

jboolean WrapperMultiMapAccess::equal(JNIEnv * env, const void* container, jobject other)  {
     return m_containerAccess->equal(env, container, other);
}

jint WrapperMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value)  {
    return m_containerAccess->remove(env, container, key, value);
}

jint WrapperMultiMapAccess::size(JNIEnv * env, const void* container)  {
     return m_containerAccess->size(env, container);
}

qsizetype WrapperMultiMapAccess::size(const void* container){
    return m_containerAccess->size(container);
}

jobject WrapperMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container,jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperMultiMapAccess::constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constUpperBound(env, container, key);
}

jobject WrapperMultiMapAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
     return m_containerAccess->value(env, container, key, defaultValue);
}

ContainerAndAccessInfo WrapperMultiMapAccess::values(JNIEnv * env, const ConstContainerInfo& container)  {
     return m_containerAccess->values(env, container);
}

bool WrapperMultiMapAccess::keyLessThan(JNIEnv *env, jobject key1, jobject key2)  {
    return m_containerAccess->keyLessThan(env, key1, key2);
}

ContainerAndAccessInfo WrapperMultiMapAccess::uniqueKeys(JNIEnv * env, const ConstContainerInfo& container)  {
    return m_containerAccess->uniqueKeys(env, container);
}

void WrapperMultiMapAccess::unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other)  {
     m_containerAccess->unite(env, container, other);
}

ContainerAndAccessInfo WrapperMultiMapAccess::values(JNIEnv * env, const ConstContainerInfo& container, jobject key)  {
    return m_containerAccess->values(env, container, key);
}

jboolean WrapperMultiMapAccess::contains(JNIEnv * env, const void* container, jobject key, jobject value) {
    return m_containerAccess->contains(env, container, key, value);
}

jint WrapperMultiMapAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)  {
     return m_containerAccess->count(env, container, key, value);
}

jobject WrapperMultiMapAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value)  {
    return m_containerAccess->find(env, container, key, value);
}

jobject WrapperMultiMapAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value)  {
    return m_containerAccess->constFind(env, container, key, value);
}

jint WrapperMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container,jobject key)  {
     return m_containerAccess->remove(env, container, key);
}

void WrapperMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value)  {
    m_containerAccess->replace(env, container, key, value);
}

bool WrapperMultiMapAccess::contains(const void* container, const void* key) {
    return m_containerAccess->contains(container, key);
}

void WrapperMultiMapAccess::insert(void* container,const void* key, const void* value) {
    m_containerAccess->insert(container, key, value);
}

const void* WrapperMultiMapAccess::value(const void* container, const void* key, const void* defaultValue) {
    return m_containerAccess->value(container, key, defaultValue);
}

WrapperHashAccess::WrapperHashAccess(AbstractHashAccess* containerAccess)
    : AbstractHashAccess(), m_containerAccess(containerAccess) {}

WrapperHashAccess::~WrapperHashAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractHashAccess* WrapperHashAccess::clone() {
    return m_containerAccess->clone();
}

const QObject* WrapperHashAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperHashAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractHashAccess* WrapperHashAccess::wrappedAccess(){
    return m_containerAccess;
}

bool WrapperHashAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> WrapperHashAccess::keyValueIterator(const void* container) {
    return m_containerAccess->keyValueIterator(container);
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> WrapperHashAccess::keyValueIterator(void* container) {
    return m_containerAccess->keyValueIterator(container);
}

void WrapperHashAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperHashAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

void WrapperHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

void WrapperHashAccess::dispose()  { delete this; }

void WrapperHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

void WrapperHashAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperHashAccess::constructContainer(void* placement) {
    return m_containerAccess->constructContainer(placement);
}

void* WrapperHashAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

void* WrapperHashAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperHashAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
}

void* WrapperHashAccess::constructContainer(JNIEnv * env, void* container, const ContainerAndAccessInfo& move) {
    return m_containerAccess->constructContainer(env, container, move);
}
#endif

size_t WrapperHashAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

bool WrapperHashAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperHashAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

const QMetaType& WrapperHashAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperHashAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

AbstractContainerAccess::DataType WrapperHashAccess::keyType() {
    return m_containerAccess->keyType();
}

AbstractContainerAccess::DataType WrapperHashAccess::valueType() {
    return m_containerAccess->valueType();
}

AbstractContainerAccess* WrapperHashAccess::keyNestedContainerAccess() {
    return m_containerAccess->keyNestedContainerAccess();
}

AbstractContainerAccess* WrapperHashAccess::valueNestedContainerAccess() {
    return m_containerAccess->valueNestedContainerAccess();
}

bool WrapperHashAccess::hasKeyNestedContainerAccess() {
    return m_containerAccess->hasKeyNestedContainerAccess();
}
bool WrapperHashAccess::hasValueNestedContainerAccess() {
    return m_containerAccess->hasValueNestedContainerAccess();
}
bool WrapperHashAccess::hasKeyNestedPointers() {
    return m_containerAccess->hasKeyNestedPointers();
}
bool WrapperHashAccess::hasValueNestedPointers() {
    return m_containerAccess->hasValueNestedPointers();
}

jint WrapperHashAccess::capacity(JNIEnv * env,const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperHashAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperHashAccess::count(JNIEnv * env, const void* container, jobject key)  {
    return m_containerAccess->count(env, container, key);
}

jobject WrapperHashAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->begin(env, container);
}

jobject WrapperHashAccess::end(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->end(env, container);
}

jobject WrapperHashAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->find(env, container, key);
}

jobject WrapperHashAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperHashAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

jobject WrapperHashAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constFind(env, container, key);
}

void WrapperHashAccess::insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperHashAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
    return m_containerAccess->key(env, container, value, defaultKey);
}

ContainerAndAccessInfo WrapperHashAccess::keys(JNIEnv * env, const ConstContainerInfo& container)  {
    return m_containerAccess->keys(env, container);
}

ContainerAndAccessInfo WrapperHashAccess::keys(JNIEnv * env, const ConstContainerInfo& container, jobject value)  {
    return m_containerAccess->keys(env, container, value);
}

jboolean WrapperHashAccess::equal(JNIEnv * env, const void* container, jobject other)  {
    return m_containerAccess->equal(env, container, other);
}

jint WrapperHashAccess::remove(JNIEnv * env, const ContainerInfo& container,jobject key)  {
    return m_containerAccess->remove(env, container, key);
}

void WrapperHashAccess::reserve(JNIEnv * env, const ContainerInfo& container, jint newSize)  {
    m_containerAccess->reserve(env, container, newSize);
}

jint WrapperHashAccess::size(JNIEnv * env, const void* container)  {
    return m_containerAccess->size(env, container);
}

qsizetype WrapperHashAccess::size(const void* container){
    return m_containerAccess->size(container);
}

jobject WrapperHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperHashAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
    return m_containerAccess->value(env, container, key, defaultValue);
}

ContainerAndAccessInfo WrapperHashAccess::values(JNIEnv * env, const ConstContainerInfo& container)  {
    return m_containerAccess->values(env, container);
}

bool WrapperHashAccess::contains(const void* container, const void* key) {
    return m_containerAccess->contains(container, key);
}

void WrapperHashAccess::insert(void* container,const void* key, const void* value) {
    m_containerAccess->insert(container, key, value);
}

const void* WrapperHashAccess::value(const void* container, const void* key, const void* defaultValue) {
    return m_containerAccess->value(container, key, defaultValue);
}

WrapperMultiHashAccess::WrapperMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : AbstractMultiHashAccess(), m_containerAccess(containerAccess) {}

WrapperMultiHashAccess::~WrapperMultiHashAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractMultiHashAccess* WrapperMultiHashAccess::clone() {
    return m_containerAccess->clone();
}

const QObject* WrapperMultiHashAccess::getOwner(const void* container){
    return m_containerAccess->getOwner(container);
}

bool WrapperMultiHashAccess::hasOwnerFunction(){
    return m_containerAccess->hasOwnerFunction();
}

AbstractMultiHashAccess* WrapperMultiHashAccess::wrappedAccess(){
    return m_containerAccess;
}

bool WrapperMultiHashAccess::isDetached(const void* container) {
    return m_containerAccess->isDetached(container);
}

void WrapperMultiHashAccess::detach(const ContainerInfo& container) {
    return m_containerAccess->detach(container);
}

bool WrapperMultiHashAccess::isSharedWith(const void* container, const void* container2) {
    return m_containerAccess->isSharedWith(container, container2);
}

void WrapperMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) {
    m_containerAccess->swap(env, container, container2);
}

void WrapperMultiHashAccess::dispose()  { delete this; }

void WrapperMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2) {
    m_containerAccess->assign(env, container, container2);
}

void WrapperMultiHashAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperMultiHashAccess::constructContainer(void* placement) {
    return m_containerAccess->constructContainer(placement);
}

void* WrapperMultiHashAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

void* WrapperMultiHashAccess::constructContainer(JNIEnv * env, void* container, const ConstContainerAndAccessInfo& copyOf) {
    return m_containerAccess->constructContainer(env, container, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperMultiHashAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
}

void* WrapperMultiHashAccess::constructContainer(JNIEnv * env, void* container, const ContainerAndAccessInfo& move) {
    return m_containerAccess->constructContainer(env, container, move);
}
#endif

size_t WrapperMultiHashAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

bool WrapperMultiHashAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperMultiHashAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

const QMetaType& WrapperMultiHashAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperMultiHashAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

AbstractContainerAccess::DataType WrapperMultiHashAccess::keyType() {
    return m_containerAccess->keyType();
}

AbstractContainerAccess::DataType WrapperMultiHashAccess::valueType() {
    return m_containerAccess->valueType();
}

AbstractContainerAccess* WrapperMultiHashAccess::keyNestedContainerAccess() {
    return m_containerAccess->keyNestedContainerAccess();
}

AbstractContainerAccess* WrapperMultiHashAccess::valueNestedContainerAccess() {
    return m_containerAccess->valueNestedContainerAccess();
}

bool WrapperMultiHashAccess::hasKeyNestedContainerAccess() {
    return m_containerAccess->hasKeyNestedContainerAccess();
}
bool WrapperMultiHashAccess::hasValueNestedContainerAccess() {
    return m_containerAccess->hasValueNestedContainerAccess();
}
bool WrapperMultiHashAccess::hasKeyNestedPointers() {
    return m_containerAccess->hasKeyNestedPointers();
}
bool WrapperMultiHashAccess::hasValueNestedPointers() {
    return m_containerAccess->hasValueNestedPointers();
}

jint WrapperMultiHashAccess::capacity(JNIEnv * env,const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperMultiHashAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperMultiHashAccess::count(JNIEnv * env, const void* container, jobject key)  {
    return m_containerAccess->count(env, container, key);
}

jobject WrapperMultiHashAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->begin(env, container);
}

jobject WrapperMultiHashAccess::end(JNIEnv * env, const ExtendedContainerInfo& container) {
    return m_containerAccess->end(env, container);
}

jobject WrapperMultiHashAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->find(env, container, key);
}

jobject WrapperMultiHashAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constBegin(env, container);
}

jobject WrapperMultiHashAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) {
    return m_containerAccess->constEnd(env, container);
}

jobject WrapperMultiHashAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)  {
     return m_containerAccess->constFind(env, container, key);
}

void WrapperMultiHashAccess::insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperMultiHashAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
    return m_containerAccess->key(env, container, value, defaultKey);
}

ContainerAndAccessInfo WrapperMultiHashAccess::keys(JNIEnv * env, const ConstContainerInfo& container)  {
    return m_containerAccess->keys(env, container);
}

ContainerAndAccessInfo WrapperMultiHashAccess::keys(JNIEnv * env, const ConstContainerInfo& container, jobject value)  {
    return m_containerAccess->keys(env, container, value);
}

jboolean WrapperMultiHashAccess::equal(JNIEnv * env, const void* container, jobject other)  {
    return m_containerAccess->equal(env, container, other);
}

jint WrapperMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container,jobject key)  {
    return m_containerAccess->remove(env, container, key);
}

void WrapperMultiHashAccess::reserve(JNIEnv * env, const ContainerInfo& container, jint newSize)  {
    m_containerAccess->reserve(env, container, newSize);
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> WrapperMultiHashAccess::keyValueIterator(const void* container) { return m_containerAccess->keyValueIterator(container); }
std::unique_ptr<AbstractHashAccess::KeyValueIterator> WrapperMultiHashAccess::keyValueIterator(void* container) { return m_containerAccess->keyValueIterator(container); }

jint WrapperMultiHashAccess::size(JNIEnv * env, const void* container)  {
    return m_containerAccess->size(env, container);
}

qsizetype WrapperMultiHashAccess::size(const void* container){
    return m_containerAccess->size(container);
}

jobject WrapperMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperMultiHashAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
    return m_containerAccess->value(env, container, key, defaultValue);
}

ContainerAndAccessInfo WrapperMultiHashAccess::values(JNIEnv * env, const ConstContainerInfo& container)  {
    return m_containerAccess->values(env, container);
}

ContainerAndAccessInfo WrapperMultiHashAccess::values(JNIEnv * env, const ConstContainerInfo& container, jobject key)  {
    return m_containerAccess->values(env, container, key);
}

jboolean WrapperMultiHashAccess::contains(JNIEnv * env, const void* container, jobject key, jobject value) {
    return m_containerAccess->contains(env, container, key, value);
}

jint WrapperMultiHashAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)  {
    return m_containerAccess->count(env, container, key, value);
}

jobject WrapperMultiHashAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value)  {
    return m_containerAccess->find(env, container, key, value);
}

jobject WrapperMultiHashAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value)  {
    return m_containerAccess->constFind(env, container, key, value);
}

jint WrapperMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value)  {
     return m_containerAccess->remove(env, container, key, value);
}

void WrapperMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value)  {
    m_containerAccess->replace(env, container, key, value);
}

ContainerAndAccessInfo WrapperMultiHashAccess::uniqueKeys(JNIEnv * env, const ConstContainerInfo& container)  {
    return m_containerAccess->uniqueKeys(env, container);
}

void WrapperMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other)  {
    m_containerAccess->unite(env, container, other);
}

bool WrapperMultiHashAccess::contains(const void* container, const void* key) {
    return m_containerAccess->contains(container, key);
}

void WrapperMultiHashAccess::insert(void* container,const void* key, const void* value) {
    m_containerAccess->insert(container, key, value);
}

const void* WrapperMultiHashAccess::value(const void* container, const void* key, const void* defaultValue) {
    return m_containerAccess->value(container, key, defaultValue);
}

bool AbstractContainerAccess::isPointerType(const QMetaType& metaType){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    if(metaType.sizeOf()==sizeof(void*)){
        const std::type_info* typeId = getTypeByMetaType(metaType.id());
        QByteArray name = metaType.name();
        if(!typeId)
            typeId = getTypeByQtName(name);
        if(typeId){
            if(OptionalBool op = isRegisteredAsPointerType(*typeId)){
                if(op.value()){
                    return true;
                }
            }
        }
        if(name.endsWith("*"))
            return true;
        int idx = name.indexOf("(*)");
        if(idx<0)
            idx = name.indexOf("(__cdecl*)");
        if(idx>0){
            QByteArray prefix = name.left(idx);
            return prefix.count('<')==prefix.count('>');
        }
    }
    return false;
#else
    return metaType.flags().testFlag(QMetaType::IsPointer);
#endif
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
bool AbstractContainerAccess::isStaticType(const QMetaType& metaType){
    {
        const std::type_info* typeId = getTypeByQtName(metaType.name());
        if(!typeId)
            typeId = getTypeByMetaType(metaType.id());
        if(typeId){
            if(OptionalBool op = isRegisteredAsStaticType(*typeId))
                return op.value();
        }
    }
    return QtJambiTypeManager::isStaticType(QLatin1String(metaType.name()));
}
#endif

std::function<AbstractContainerAccess*()> getSequentialContainerAccessFactory(SequentialContainerType containerType, const QMetaType& type);
std::function<AbstractContainerAccess*()> getSequentialContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType);
void registerContainerAccessFactory(SequentialContainerType containerType, const QMetaType& elementType, std::function<AbstractContainerAccess*()>&& factory);
void registerContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType, std::function<AbstractContainerAccess*()>&& factory);

namespace ContainerAccessAPI {

hash_type pointerHashFunction(const void* ptr, hash_type seed){ return !ptr ? 0 : ::qHash(*reinterpret_cast<QHashDummyValue*const*>(ptr), seed);}

#if defined(QTJAMBI_GENERIC_ACCESS)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool pointerLessFunction(const void* ptr1, const void* ptr2){ return !ptr1 || !ptr2 ? false : quintptr(*reinterpret_cast<void*const*>(ptr1)) < quintptr(*reinterpret_cast<void*const*>(ptr2));}
bool pointerEqualFunction(const void* ptr1, const void* ptr2){ return !ptr1 || !ptr2 ? false : quintptr(*reinterpret_cast<void*const*>(ptr1)) == quintptr(*reinterpret_cast<void*const*>(ptr2));}
#endif
#endif //defined(QTJAMBI_GENERIC_ACCESS)

AbstractContainerAccess* createContainerAccess(SequentialContainerType containerType, const QMetaType& memberMetaType){
    AbstractContainerAccess* containerAccess = nullptr;
    switch(memberMetaType.id()){
    case QMetaType::UnknownType:
        break;
    case QMetaType::Bool:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<bool>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<bool>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<bool>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<bool>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const bool>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<bool>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::Char:
    case QMetaType::SChar:
    case QMetaType::UChar:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint8>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint8>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint8>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint8>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const qint8>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<qint8>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::Short:
    case QMetaType::UShort:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint16>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint16>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint16>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint16>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const qint16>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<qint16>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::Int:
    case QMetaType::UInt:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint32>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint32>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint32>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint32>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const qint32>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<qint32>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint64>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint64>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint64>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint64>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const qint64>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<qint64>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::Double:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<double>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<double>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<double>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<double>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const double>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<double>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::Float:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<float>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<float>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<float>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<float>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const float>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<float>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::QChar:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<QChar>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QChar>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QChar>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QChar>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const QChar>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<QChar>::newInstance();
            break;
#endif
        }
        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    case QMetaType::Char16:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<char16_t>::newInstance();
            break;
        case SequentialContainerType::QStack:
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<char16_t>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const char16_t>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<char16_t>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::Char32:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<char32_t>::newInstance();
            break;
        case SequentialContainerType::QStack:
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<char32_t>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const char32_t>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<char32_t>::newInstance();
            break;
#endif
        }
        break;
#endif
    case QMetaType::QVariant:
        switch(containerType){
        case SequentialContainerType::QSet:
            // no qhash for qvariant
            // containerAccess = QtJambiPrivate::QSetAccess<QVariant>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QVariant>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QVariant>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QVariant>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const QVariant>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<QVariant>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::QString:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<QString>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QString>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QString>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QString>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const QString>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<QString>::newInstance();
            break;
#endif
        }
        break;
    case QMetaType::QObjectStar:
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<QObject*>::newInstance();
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case SequentialContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QObject*>::newInstance();
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QObject*>::newInstance();
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QObject*>::newInstance();
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<const QObject*>::newInstance();
            break;
        case SequentialContainerType::QSpan:
            containerAccess = QtJambiPrivate::QSpanAccess<QObject*>::newInstance();
            break;
#endif
        }
        break;
    default:
        break;
    }
    if(!containerAccess){
        if(std::function<AbstractContainerAccess*()> containerAccessFactory = getSequentialContainerAccessFactory(containerType, memberMetaType))
            containerAccess = containerAccessFactory();
    }
    return containerAccess;
}

AbstractContainerAccess* createContainerAccess(AssociativeContainerType mapType, const QMetaType& memberMetaType1, const QMetaType& memberMetaType2){
    AbstractContainerAccess* containerAccess = nullptr;
    if(std::function<AbstractContainerAccess*()> containerAccessFactory = getSequentialContainerAccessFactory(mapType, memberMetaType1, memberMetaType2))
        containerAccess = containerAccessFactory();
    return containerAccess;
}

AbstractContainerAccess* createContainerAccess(JNIEnv* env, SequentialContainerType containerType,
                                                         const QMetaType& metaType,
                                                         size_t align, size_t size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                         bool isStaticType,
#endif
                                                         bool isPointer,
                                                         const QtJambiUtils::QHashFunction& hashFunction,
                                                         const QtJambiUtils::InternalToExternalConverter& memberConverter,
                                                         const QtJambiUtils::ExternalToInternalConverter& memberReConverter,
                                                         const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess,
                                                         PtrOwnerFunction ownerFunction){
    Q_UNUSED(isPointer);
#if defined(QTJAMBI_GENERIC_ACCESS)
    if(isPointer){
        size = 0;
        align = 0;
    }
#elif QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    Q_UNUSED(size);
    Q_UNUSED(align);
#else
    if(isPointer){
        size = sizeof(void*);
        align = alignof(void*);
        isStaticType = false;
    }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
    Q_ASSERT(metaType.id()!=QMetaType::UnknownType && metaType.id()!=QMetaType::Void);
    AbstractContainerAccess* containerAccess = nullptr;
    AbstractContainerAccess::DataType elementType = dataType(metaType, memberNestedContainerAccess);
    bool hasNestedPointers = false;
    if(memberNestedContainerAccess && (elementType & AbstractContainerAccess::PointersMask)==0){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(memberNestedContainerAccess.data())){
            hasNestedPointers = (daccess->elementType() & AbstractContainerAccess::PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(memberNestedContainerAccess.data())){
            hasNestedPointers = (daccess->keyType() & AbstractContainerAccess::PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & AbstractContainerAccess::PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(memberNestedContainerAccess.data())){
            hasNestedPointers = (daccess->firstType() & AbstractContainerAccess::PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & AbstractContainerAccess::PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
#if defined(QTJAMBI_GENERIC_ACCESS)
    if(!isPointer){
#endif //defined(QTJAMBI_GENERIC_ACCESS)
        switch(containerType){
        case SequentialContainerType::QSet:
            switch(elementType){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointerRCAutoSetAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     metaType.id(),
                                                     align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
#else
                                                     metaType,
#endif
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                     elementType);
                break;
            default:{
                if(hasNestedPointers)
                    containerAccess = new NestedPointersRCAutoSetAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     metaType.id(),
                                                     align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
#else
                                                     metaType,
#endif
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                    elementType);
                else
                    containerAccess = new AutoSetAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                 metaType.id(),
                                                 align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
#else
                                                 metaType,
#endif
                                                 hashFunction,
                                                 memberConverter,
                                                 memberReConverter,
                                                 memberNestedContainerAccess,
                                                 ownerFunction,
                                                elementType);
                }break;
            }
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        case SequentialContainerType::QVector:
            switch(elementType){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointerRCAutoVectorAccess(
                            metaType.id(),
                            align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                            hashFunction,
                            memberConverter,
                            memberReConverter,
                            memberNestedContainerAccess,
                            ownerFunction,
                            elementType);
                break;
            default:{
                if(hasNestedPointers)
                    containerAccess = new NestedPointersRCAutoVectorAccess(
                                metaType.id(),
                                align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                hashFunction,
                                memberConverter,
                                memberReConverter,
                                memberNestedContainerAccess,
                                ownerFunction,
                                elementType);
                else
                    containerAccess = new AutoVectorAccess(
                                metaType.id(),
                                align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                hashFunction,
                                memberConverter,
                                memberReConverter,
                                memberNestedContainerAccess,
                                ownerFunction,
                                elementType);
                }break;
            }
            break;
        case SequentialContainerType::QLinkedList:
            switch(elementType){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointerRCAutoLinkedListAccess(
                            metaType.id(),
                            align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                            hashFunction,
                            memberConverter,
                            memberReConverter,
                            memberNestedContainerAccess,
                            ownerFunction,
                            elementType);
                break;
            default:{
                if(hasNestedPointers)
                    containerAccess = new NestedPointersRCAutoLinkedListAccess(
                                metaType.id(),
                                align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                hashFunction,
                                memberConverter,
                                memberReConverter,
                                memberNestedContainerAccess,
                                ownerFunction,
                                elementType);
                else
                    containerAccess = new AutoLinkedListAccess(
                                metaType.id(),
                                align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                hashFunction,
                                memberConverter,
                                memberReConverter,
                                memberNestedContainerAccess,
                                ownerFunction,
                                elementType);
                }break;
            }
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            switch(elementType){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointerRCAutoListAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     metaType.id(),
                                                     align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                                     isStaticType,
#else
                                                     metaType,
#endif
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                    elementType);
                break;
            default:{
                if(hasNestedPointers)
                    containerAccess = new NestedPointersRCAutoListAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     metaType.id(),
                                                     align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                                     isStaticType,
#else
                                                     metaType,
#endif
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                    elementType);
                else
                    containerAccess = new AutoListAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                 metaType.id(),
                                                 align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                                 isStaticType,
#else
                                                 metaType,
#endif
                                                 hashFunction,
                                                 memberConverter,
                                                 memberReConverter,
                                                 memberNestedContainerAccess,
                                                 ownerFunction,
                                                elementType);
                }break;
            }
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        case SequentialContainerType::QConstSpan:
            containerAccess = new AutoSpanAccess(
                                         metaType,
                                         hashFunction,
                                         memberConverter,
                                         memberReConverter,
                                         memberNestedContainerAccess,
                                         ownerFunction,
                                         elementType,
                                         false);
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
                if(hasNestedPointers)
                    containerAccess = new NestedPointersRCAutoSpanAccess(
                                                     metaType,
                                                     hashFunction,
                                                     memberConverter,
                                                     memberReConverter,
                                                     memberNestedContainerAccess,
                                                     ownerFunction,
                                                     elementType);
                else
                    containerAccess = new AutoSpanAccess(
                                                 metaType,
                                                 hashFunction,
                                                 memberConverter,
                                                 memberReConverter,
                                                 memberNestedContainerAccess,
                                                 ownerFunction,
                                                 elementType);
                }break;
            }
            break;
#endif
        default: break;
        }
#if defined(QTJAMBI_GENERIC_ACCESS)
    }
    if(!containerAccess){
        JavaException occurredException;
        if(SequentialContainerAccessFactory accessFactory = findContainerAccessFactory(env, containerType, align, size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                             isStaticType,
#endif
                                                                                             occurredException)){
            QMetaType voidPtr(QMetaType::VoidStar);
            containerAccess = accessFactory(isPointer && !metaType.isValid() ? voidPtr : metaType,
                                            hashFunction,
                                            memberConverter,
                                            memberReConverter,
                                            memberNestedContainerAccess,
                                            ownerFunction);
        }else{
            occurredException.raise();
        }
    }
#else
    Q_UNUSED(env)
#endif //defined(QTJAMBI_GENERIC_ACCESS)
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
#if defined(QTJAMBI_GENERIC_ACCESS)
    if(isPointer1){
        size1 = 0;
        align1 = 0;
    }
    if(isPointer2){
        size2 = 0;
        align2 = 0;
    }
#elif QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    Q_UNUSED(size1);
    Q_UNUSED(align1);
    Q_UNUSED(size2);
    Q_UNUSED(align2);
#else
    if(isPointer1){
        size1 = sizeof(void*);
        align1 = alignof(void*);
    }
    if(isPointer2){
        size2 = sizeof(void*);
        align2 = alignof(void*);
    }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
    AbstractContainerAccess::DataType memberType1 = dataType(memberMetaType1, memberNestedContainerAccess1);
    AbstractContainerAccess::DataType memberType2 = dataType(memberMetaType2, memberNestedContainerAccess2);
    bool hasNestedPointers1 = false;
    bool hasNestedPointers2 = false;
    if(memberNestedContainerAccess1 && (memberType1 & AbstractContainerAccess::PointersMask)==0){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(memberNestedContainerAccess1.data())){
            hasNestedPointers1 = (daccess->elementType() & AbstractContainerAccess::PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(memberNestedContainerAccess1.data())){
            hasNestedPointers1 = (daccess->keyType() & AbstractContainerAccess::PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & AbstractContainerAccess::PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(memberNestedContainerAccess1.data())){
            hasNestedPointers1 = (daccess->firstType() & AbstractContainerAccess::PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & AbstractContainerAccess::PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    if(memberNestedContainerAccess2 && (memberType2 & AbstractContainerAccess::PointersMask)==0){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(memberNestedContainerAccess2.data())){
            hasNestedPointers2 = (daccess->elementType() & AbstractContainerAccess::PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(memberNestedContainerAccess2.data())){
            hasNestedPointers2 = (daccess->keyType() & AbstractContainerAccess::PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & AbstractContainerAccess::PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(memberNestedContainerAccess2.data())){
            hasNestedPointers2 = (daccess->firstType() & AbstractContainerAccess::PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & AbstractContainerAccess::PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    AbstractContainerAccess* containerAccess = nullptr;
#if defined(QTJAMBI_GENERIC_ACCESS)
    if(!isPointer1 || !isPointer2){
#endif //defined(QTJAMBI_GENERIC_ACCESS)
        switch(mapType){
        case AssociativeContainerType::QPair:
            containerAccess = new AutoPairAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                         hashFunction1,
                                         memberConverter1,
                                         memberReConverter1,
                                         memberNestedContainerAccess1,
                                         ownerFunction1,
                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                    }else{
                        containerAccess = new KeyPointerRCAutoHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                            }else{
                                containerAccess = new AutoHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                             isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                             hashFunction1,
                                                             memberConverter1,
                                                             memberReConverter1,
                                                             memberNestedContainerAccess1,
                                                             ownerFunction1,
                                                             memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                             isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                    }else{
                        containerAccess = new KeyPointerRCAutoMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                            }else{
                                containerAccess = new AutoMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                             isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                             hashFunction1,
                                                             memberConverter1,
                                                             memberReConverter1,
                                                             memberNestedContainerAccess1,
                                                             ownerFunction1,
                                                             memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                             isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                    }else{
                        containerAccess = access = new KeyPointerRCAutoMultiHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                            }else{
                                containerAccess = access = new AutoMultiHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                             isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                             hashFunction1,
                                                             memberConverter1,
                                                             memberReConverter1,
                                                             memberNestedContainerAccess1,
                                                             ownerFunction1,
                                                             memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                             isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
//#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                    }else{
                        containerAccess = access = new KeyPointerRCAutoMultiMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                                         hashFunction1,
                                                                         memberConverter1,
                                                                         memberReConverter1,
                                                                         memberNestedContainerAccess1,
                                                                         ownerFunction1,
                                                                         memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                                                         hashFunction2,
                                                                         memberConverter2,
                                                                         memberReConverter2,
                                                                         memberNestedContainerAccess2,
                                                                         ownerFunction2,
                                                                         memberType2);
                            }else{
                                containerAccess = access = new AutoMultiMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                                             isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                                             isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                                             hashFunction1,
                                                             memberConverter1,
                                                             memberReConverter1,
                                                             memberNestedContainerAccess1,
                                                             ownerFunction1,
                                                             memberType1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                                             isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                                             isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
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

#if defined(QTJAMBI_GENERIC_ACCESS)
    }
    if(!containerAccess){
        JavaException occurredException;
        if(AssociativeContainerAccessFactory accessFactory = findContainerAccessFactory(env, mapType, align1, size1, align2, size2, occurredException)){
            QMetaType voidPtr(QMetaType::VoidStar);
            containerAccess = accessFactory(
                                           isPointer1 && !memberMetaType1.isValid() ? voidPtr : memberMetaType1,
                                           hashFunction1,
                                           memberConverter1,
                                           memberReConverter1,
                                           memberNestedContainerAccess1,
                                           ownerFunction1,
                                           isPointer2 && !memberMetaType2.isValid() ? voidPtr : memberMetaType2,
                                           hashFunction2,
                                           memberConverter2,
                                           memberReConverter2,
                                           memberNestedContainerAccess2,
                                           ownerFunction2);
        }else{
            occurredException.raise();
        }
    }
#else
    Q_UNUSED(env)
#endif //defined(QTJAMBI_GENERIC_ACCESS)
    return containerAccess;
}

}//namespace ContainerAccessAPI

void containerDisposer(AbstractContainerAccess* _access){_access->dispose();}

#if defined(QTJAMBI_GENERIC_ACCESS)

AbstractListAccess* checkContainerAccess(JNIEnv * env, AbstractListAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->elementType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            containerAccess = new PointerRCListAccess(containerAccess);
            break;
        default:
            if(containerAccess->hasNestedPointers())
                containerAccess = new NestedPointersRCListAccess(containerAccess);
            break;
        }
    }
    return containerAccess;
}

AbstractSetAccess* checkContainerAccess(JNIEnv * env, AbstractSetAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->elementType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            containerAccess = new PointerRCSetAccess(containerAccess);
            break;
        default:
            if(containerAccess->hasNestedPointers())
                containerAccess = new NestedPointersRCSetAccess(containerAccess);
            break;
        }
    }
    return containerAccess;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
AbstractLinkedListAccess* checkContainerAccess(JNIEnv * env, AbstractLinkedListAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->elementType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            containerAccess = new PointerRCLinkedListAccess(containerAccess);
            break;
        default:
            if(containerAccess->hasNestedPointers())
                containerAccess = new NestedPointersRCLinkedListAccess(containerAccess);
            break;
        }
    }
    return containerAccess;
}

AbstractVectorAccess* checkContainerAccess(JNIEnv * env, AbstractVectorAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->elementType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            containerAccess = new PointerRCVectorAccess(containerAccess);
            break;
        default:
            if(containerAccess->hasNestedPointers())
                containerAccess = new NestedPointersRCVectorAccess(containerAccess);
            break;
        }
    }
    return containerAccess;
}
#endif

AbstractHashAccess* checkContainerAccess(JNIEnv * env, AbstractHashAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->keyType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(containerAccess->valueType()){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointersRCHashAccess(containerAccess);
                break;
            default:
                if(containerAccess->hasValueNestedPointers()){
                    containerAccess = new NestedPointersRCHashAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCHashAccess(containerAccess);
                }
                break;
            }
            break;
        default:{
                if(containerAccess->hasKeyNestedPointers()){
                    containerAccess = new NestedPointersRCHashAccess(containerAccess);
                }else{
                    switch(containerAccess->valueType()){
                    case AbstractContainerAccess::Pointer:
                    case AbstractContainerAccess::FunctionPointer:
                    case AbstractContainerAccess::PointerToQObject:
                        containerAccess = new ValuePointerRCHashAccess(containerAccess);
                        break;
                    default:
                        if(containerAccess->hasValueNestedPointers()){
                            containerAccess = new NestedPointersRCHashAccess(containerAccess);
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
    return containerAccess;
}

AbstractMapAccess* checkContainerAccess(JNIEnv * env, AbstractMapAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->keyType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(containerAccess->valueType()){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointersRCMapAccess(containerAccess);
                break;
            default:
                if(containerAccess->hasValueNestedPointers()){
                    containerAccess = new NestedPointersRCMapAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMapAccess(containerAccess);
                }
                break;
            }
            break;
        default:
            if(containerAccess->hasKeyNestedPointers()){
                containerAccess = new NestedPointersRCMapAccess(containerAccess);
            }else{
                switch(containerAccess->valueType()){
                case AbstractContainerAccess::Pointer:
                case AbstractContainerAccess::FunctionPointer:
                case AbstractContainerAccess::PointerToQObject:
                    containerAccess = new ValuePointerRCMapAccess(containerAccess);
                    break;
                default:
                    if(containerAccess->hasValueNestedPointers()){
                        containerAccess = new NestedPointersRCMapAccess(containerAccess);
                    }
                    break;
                }
            }
            break;
        }
    }
    return containerAccess;
}

AbstractMultiHashAccess* checkContainerAccess(JNIEnv * env, AbstractMultiHashAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->keyType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(containerAccess->valueType()){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointersRCMultiHashAccess(containerAccess);
                break;
            default:
                if(containerAccess->hasValueNestedPointers()){
                    containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMultiHashAccess(containerAccess);
                }
                break;
            }
            break;
        default:
            if(containerAccess->hasKeyNestedPointers()){
                containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
            }else{
                switch(containerAccess->valueType()){
                case AbstractContainerAccess::Pointer:
                case AbstractContainerAccess::FunctionPointer:
                case AbstractContainerAccess::PointerToQObject:
                    containerAccess = new ValuePointerRCMultiHashAccess(containerAccess);
                    break;
                default:
                    if(containerAccess->hasValueNestedPointers()){
                        containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
                    }
                    break;
                }
            }
            break;
        }
    }
    return containerAccess;
}

AbstractMultiMapAccess* checkContainerAccess(JNIEnv * env, AbstractMultiMapAccess* containerAccess){
    QtJambiAPI::checkNullPointer(env, containerAccess);
    if(!dynamic_cast<AbstractReferenceCountingContainer*>(containerAccess)){
        switch(containerAccess->keyType()){
        case AbstractContainerAccess::Pointer:
        case AbstractContainerAccess::FunctionPointer:
        case AbstractContainerAccess::PointerToQObject:
            switch(containerAccess->valueType()){
            case AbstractContainerAccess::Pointer:
            case AbstractContainerAccess::FunctionPointer:
            case AbstractContainerAccess::PointerToQObject:
                containerAccess = new PointersRCMultiMapAccess(containerAccess);
                break;
            default:
                if(containerAccess->hasValueNestedPointers()){
                    containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMultiMapAccess(containerAccess);
                }
                break;
            }
            break;
        default:
            if(containerAccess->hasKeyNestedPointers()){
                containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
            }else{
                switch(containerAccess->valueType()){
                case AbstractContainerAccess::Pointer:
                case AbstractContainerAccess::FunctionPointer:
                case AbstractContainerAccess::PointerToQObject:
                    containerAccess = new ValuePointerRCMultiMapAccess(containerAccess);
                    break;
                default:
                    if(containerAccess->hasValueNestedPointers()){
                        containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
                    }
                    break;
                }
            }
            break;
        }
    }
    return containerAccess;
}
#endif //defined(QTJAMBI_GENERIC_ACCESS)

AbstractNestedSequentialAccess::~AbstractNestedSequentialAccess(){}
AbstractNestedAssociativeAccess::~AbstractNestedAssociativeAccess(){}
AbstractNestedPairAccess::~AbstractNestedPairAccess(){}
AbstractWrapperContainerAccess::~AbstractWrapperContainerAccess(){}

AbstractContainerAccess::AbstractContainerAccess(){}
AbstractContainerAccess::~AbstractContainerAccess(){}
void AbstractContainerAccess::dispose(){}
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* AbstractContainerAccess::createContainer(void* moved){
    size_t sz = sizeOf();
    if(sz>0){
        return constructContainer(operator new(sz), moved);
    }else{
        return nullptr;
    }
}
void* AbstractContainerAccess::createContainer(JNIEnv *env, const ContainerAndAccessInfo& moved){
    size_t sz = sizeOf();
    if(sz>0){
        return constructContainer(env, operator new(sz), moved);
    }else{
        return nullptr;
    }
}
#endif
void* AbstractContainerAccess::createContainer(){
    size_t sz = sizeOf();
    if(sz>0){
        return constructContainer(operator new(sz));
    }else{
        return nullptr;
    }
}
void* AbstractContainerAccess::createContainer(const void* copy){
    size_t sz = sizeOf();
    if(sz>0){
        return constructContainer(operator new(sz), copy);
    }else{
        return nullptr;
    }
}
void* AbstractContainerAccess::createContainer(JNIEnv *env, const ConstContainerAndAccessInfo& copy){
    size_t sz = sizeOf();
    if(sz>0){
        return constructContainer(env, operator new(sz), copy);
    }else{
        return nullptr;
    }
}
void AbstractContainerAccess::deleteContainer(void* container){
    if(destructContainer(container))
        operator delete(container);
}
AbstractSequentialConstIteratorAccess::~AbstractSequentialConstIteratorAccess(){}
AbstractSequentialConstIteratorAccess::AbstractSequentialConstIteratorAccess(){}
const QObject* AbstractContainerAccess::getOwner(const void*){ return nullptr; }
bool AbstractContainerAccess::hasOwnerFunction(){ return false; }
void AbstractSequentialConstIteratorAccess::assign(void*, const void*) {}
void AbstractSequentialConstIteratorAccess::assign(JNIEnv*, const ContainerInfo&, const ConstContainerAndAccessInfo&) {}
size_t AbstractSequentialConstIteratorAccess::sizeOf() {return 0;}
void* AbstractSequentialConstIteratorAccess::constructContainer(void*) {return nullptr;}
void* AbstractSequentialConstIteratorAccess::constructContainer(void*,const void*) {return nullptr;}
void* AbstractSequentialConstIteratorAccess::constructContainer(JNIEnv *,void*,const ConstContainerAndAccessInfo&) {return nullptr;}
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* AbstractSequentialConstIteratorAccess::constructContainer(void*,void*) {return nullptr;}
void* AbstractSequentialConstIteratorAccess::constructContainer(JNIEnv *,void*,const ContainerAndAccessInfo&) {return nullptr;}
#endif
bool AbstractSequentialConstIteratorAccess::destructContainer(void*) {return false;}
int AbstractSequentialConstIteratorAccess::registerContainer(const QByteArray&) {return QMetaType::UnknownType;}

AbstractAssociativeConstIteratorAccess::~AbstractAssociativeConstIteratorAccess(){}
AbstractAssociativeConstIteratorAccess::AbstractAssociativeConstIteratorAccess(){}

AbstractSequentialIteratorAccess::~AbstractSequentialIteratorAccess(){}
AbstractSequentialIteratorAccess::AbstractSequentialIteratorAccess(){}
AbstractAssociativeIteratorAccess::~AbstractAssociativeIteratorAccess(){}
AbstractAssociativeIteratorAccess::AbstractAssociativeIteratorAccess(){}

AbstractSequentialAccess::~AbstractSequentialAccess(){}
AbstractSequentialAccess::AbstractSequentialAccess(){}
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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
AbstractVectorAccess::~AbstractVectorAccess(){}
AbstractVectorAccess::AbstractVectorAccess(){}
AbstractLinkedListAccess::~AbstractLinkedListAccess(){}
AbstractLinkedListAccess::AbstractLinkedListAccess(){}
#endif

AbstractSetAccess::~AbstractSetAccess(){}
AbstractSetAccess::AbstractSetAccess(){}

AbstractAssociativeAccess::~AbstractAssociativeAccess(){}
AbstractAssociativeAccess::AbstractAssociativeAccess(){}
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
            ElementIterator(const ElementIterator& other) :
                m_keyMetaType(other.m_keyMetaType),
                m_valueMetaType(other.m_valueMetaType),
                m_keyType(other.m_keyType),
                m_valueType(other.m_valueType),
                m_keyNestedContainerAccess(other.m_keyNestedContainerAccess),
                m_valueNestedContainerAccess(other.m_valueNestedContainerAccess),
                m_hasKeyNestedPointers(other.m_hasKeyNestedPointers),
                m_hasValueNestedPointers(other.m_hasValueNestedPointers),
                pair(other.pair), index(other.index) {}
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
                pair(_iter->constNext()) {}
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
    };
    return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(std::move(iter)));
}

AbstractMapAccess::~AbstractMapAccess(){}
AbstractMapAccess::AbstractMapAccess(){}

AbstractMultiMapAccess::~AbstractMultiMapAccess(){}
AbstractMultiMapAccess::AbstractMultiMapAccess(){}

AbstractHashAccess::~AbstractHashAccess(){}
AbstractHashAccess::AbstractHashAccess(){}

AbstractMultiHashAccess::~AbstractMultiHashAccess(){}
AbstractMultiHashAccess::AbstractMultiHashAccess(){}

AbstractPairAccess::~AbstractPairAccess(){}
AbstractPairAccess::AbstractPairAccess(){}

AbstractReferenceCountingContainer::~AbstractReferenceCountingContainer(){}

bool hasReferenceCounts(JNIEnv * env, jobject container){
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        QReadLocker lock(gContainerAccessLock());
        jobject rc = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        if(Java::Runtime::Collection::isInstanceOf(env, rc))
            return Java::Runtime::Collection::size(env, rc)>0;
        if(Java::Runtime::Map::isInstanceOf(env, rc))
            return Java::Runtime::Map::size(env, rc)>0;
    }
    return false;
}

void AbstractReferenceCountingContainer::unfoldAndAddContainer(JNIEnv * env, jobject set, const void* data, AbstractContainerAccess::DataType dataType, const QMetaType& metaType, AbstractContainerAccess* access){
    switch(dataType){
    case AbstractContainerAccess::PointerToQObject:
        if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(data)))
            Java::Runtime::Collection::add(env, set, obj);
        break;
    case AbstractContainerAccess::FunctionPointer:
        if(const std::type_info* typeId = getTypeByMetaType(metaType)){
            if(jobject obj = QtJambiAPI::findFunctionPointerObject(env, data, *typeId)){
                Java::Runtime::Collection::add(env, set, obj);
            }
            break;
        }
        Q_FALLTHROUGH();
    case AbstractContainerAccess::Pointer:
        if(jobject obj = QtJambiAPI::findObject(env, data))
            Java::Runtime::Collection::add(env, set, obj);
        break;
    default:
        if(auto _access = dynamic_cast<AbstractPairAccess*>(access)){
            auto elements = _access->elements(data);
            auto firstAccess = _access->firstNestedContainerAccess();
            auto secondAccess = _access->secondNestedContainerAccess();
            unfoldAndAddContainer(env, set, elements.first, _access->firstType(), _access->firstMetaType(), firstAccess);
            unfoldAndAddContainer(env, set, elements.second, _access->secondType(), _access->secondMetaType(), secondAccess);
            if(firstAccess)
                firstAccess->dispose();
            if(secondAccess)
                secondAccess->dispose();
        }else if(auto _access = dynamic_cast<AbstractSequentialAccess*>(access)){
            auto elementAccess = _access->elementNestedContainerAccess();
            auto iterator = _access->constElementIterator(data);
            while(iterator->hasNext()){
                auto content = iterator->next();
                unfoldAndAddContainer(env, set, content, _access->elementType(), _access->elementMetaType(), elementAccess);
            }
            if(elementAccess)
                elementAccess->dispose();
        }else if(auto _access = dynamic_cast<AbstractAssociativeAccess*>(access)){
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
        break;
    }
}

void AbstractReferenceCountingContainer::unfoldAndAddContainer(JNIEnv * env, jobject set, jobject value){
    if(set){
        if(Java::QtCore::AbstractContainer::isInstanceOf(env, value)){
            jobject rc;
            {
                QReadLocker lock(gContainerAccessLock());
                rc = Java::QtCore::AbstractContainer::__rcContainer(env, value);
            }
            if(rc)
                unfoldAndAddContainer(env, set, rc);
        }else if(Java::Runtime::Collection::isInstanceOf(env, value)){
            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, value);
            while(Java::Runtime::Iterator::hasNext(env, iter)){
                unfoldAndAddContainer(env, set, Java::Runtime::Iterator::next(env, iter));
            }
        }else if(Java::Runtime::Map::isInstanceOf(env, value)){
            jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, value);
            while(Java::Runtime::Iterator::hasNext(env, iter)){
                jobject entry = Java::Runtime::Iterator::next(env, iter);
                unfoldAndAddContainer(env, set, Java::Runtime::Map$Entry::getKey(env, entry));
                unfoldAndAddContainer(env, set, Java::Runtime::Map$Entry::getValue(env, entry));
            }
        }else if(Java::QtCore::QPair::isInstanceOf(env, value)){
            unfoldAndAddContainer(env, set, Java::QtCore::QPair::first(env, value));
            unfoldAndAddContainer(env, set, Java::QtCore::QPair::second(env, value));
        }else if(value){
            Java::Runtime::Collection::add(env, set, value);
        }
    }
}

void ReferenceCountingSetContainer::addNestedValueRC(JNIEnv * env, jobject container, AbstractContainerAccess::DataType dataType, bool isContainer, jobject value){
    switch(dataType){
    case AbstractContainerAccess::Value:
        if(isContainer){
            if(Java::QtCore::AbstractContainer::isInstanceOf(env, value)){
                jobject otherRC;
                jobject set;
                {
                    QReadLocker lock(gContainerAccessLock());
                    otherRC = Java::QtCore::AbstractContainer::__rcContainer(env, value);
                    set = Java::QtCore::AbstractContainer::__rcContainer(env, container);
                }
                if(otherRC){
                    if(!set)
                        set = rcContainer(env, container);
                    if(set){
                        if(Java::Runtime::Collection::isInstanceOf(env, otherRC)){
                            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, otherRC);
                            while(Java::Runtime::Iterator::hasNext(env, iter)){
                                Java::Runtime::Collection::add(env, set, Java::Runtime::Iterator::next(env, iter));
                            }
                        }else if(Java::Runtime::Map::isInstanceOf(env, otherRC)){
                            jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, otherRC);
                            while(Java::Runtime::Iterator::hasNext(env, iter)){
                                jobject entry = Java::Runtime::Iterator::next(env, iter);
                                Java::Runtime::Collection::add(env, set, Java::Runtime::Map$Entry::getKey(env, entry));
                                Java::Runtime::Collection::add(env, set, Java::Runtime::Map$Entry::getValue(env, entry));
                            }
                        }
                    }
                }
                break;
            }else if(Java::Runtime::Collection::isInstanceOf(env, value)){
                if(jobject set = rcContainer(env, container)){
                    jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, value);
                    while(Java::Runtime::Iterator::hasNext(env, iter)){
                        unfoldAndAddContainer(env, set, Java::Runtime::Iterator::next(env, iter));
                    }
                }
                break;
            }else if(Java::Runtime::Map::isInstanceOf(env, value)){
                if(jobject set = rcContainer(env, container)){
                    jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, value);
                    while(Java::Runtime::Iterator::hasNext(env, iter)){
                        jobject entry = Java::Runtime::Iterator::next(env, iter);
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
            Java::Runtime::Collection::add(env, set, value);
        break;
    default:
        break;
    }
}

void AbstractReferenceCountingContainer::swapRC(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    if(container.object && container2.object){
        QWriteLocker lock(gContainerAccessLock());
        JniLocalFrame frame(env, 200);
        Q_ASSERT(container.object);
        Q_ASSERT(container2.object);
        jobject tmp = Java::QtCore::AbstractContainer::__rcContainer(env, container.object);
        jobject tmp2 = Java::QtCore::AbstractContainer::__rcContainer(env, container2.object);
        Java::QtCore::AbstractContainer::set___rcContainer(env, container.object, tmp2);
        Java::QtCore::AbstractContainer::set___rcContainer(env, container2.object, tmp);
    }
}

jobject AbstractReferenceCountingContainer::findContainer(JNIEnv * env, jobject container){
    jobject result{nullptr};
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        QReadLocker lock(gContainerAccessLock());
        result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
    }
    return result;
}

jobject ReferenceCountingSetContainer::rcContainer(JNIEnv * env, jobject container){
    jobject result{nullptr};
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        {
            QReadLocker lock(gContainerAccessLock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
        if(!result){
            QWriteLocker lock(gContainerAccessLock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
            if(!result){
                result = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
                Java::QtCore::AbstractContainer::set___rcContainer(env, container, result);
            }
        }
    }
    return result;
}

jobject ReferenceCountingMapContainer::rcContainer(JNIEnv * env, jobject container){
    jobject result{nullptr};
    if(Java::QtCore::AbstractContainer::isInstanceOf(env, container)){
        {
            QReadLocker lock(gContainerAccessLock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
        if(!result){
            QWriteLocker lock(gContainerAccessLock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
            if(!result){
                result = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
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
        {
            QReadLocker lock(gContainerAccessLock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
        }
        if(!result){
            QWriteLocker lock(gContainerAccessLock());
            result = Java::QtCore::AbstractContainer::__rcContainer(env, container);
            if(!result){
                result = Java::QtJambi::ReferenceUtility$RCMultiMap::newInstance(env);
                Java::QtCore::AbstractContainer::set___rcContainer(env, container, result);
            }
        }
    }
    return result;
}

void ReferenceCountingSetContainer::clearRC(JNIEnv * env, jobject container){
    if(jobject rc = findContainer(env, container)){
        Java::Runtime::Collection::clear(env, rc);
    }
}

void ReferenceCountingSetContainer::assignRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        jobject tmp2 = rcContainer(env, container2);
        if(tmp){
            Java::Runtime::Collection::clear(env, tmp);
            if(tmp2)
                Java::Runtime::Collection::addAll(env, tmp, tmp2);
        }
    }
}

void ReferenceCountingSetContainer::assignUniqueRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        jobject tmp = rcContainer(env, container);
        jobject tmp2 = rcContainer(env, container2);
        if(tmp){
            Java::Runtime::Collection::clear(env, tmp);
            if(tmp2){
                jobject iter = Java::Runtime::Collection::iterator(env, tmp2);
                while(Java::Runtime::Iterator::hasNext(env, iter)){
                    jobject value = Java::Runtime::Iterator::next(env, iter);
                    if(!Java::Runtime::Collection::contains(env, tmp, value))
                        Java::Runtime::Collection::addAll(env, tmp, value);
                }
            }
        }
    }
}

void ReferenceCountingSetContainer::addAllRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        if(jobject tmp = rcContainer(env, container))
            Java::Runtime::Collection::addAll(env, tmp, container2);
    }
}

void ReferenceCountingSetContainer::addAllUniqueRC(JNIEnv * env, jobject container, jobject container2){
    if(container && container2){
        JniLocalFrame frame(env, 200);
        if(jobject tmp = rcContainer(env, container)){
            jobject iter = Java::Runtime::Collection::iterator(env, container2);
            while(Java::Runtime::Iterator::hasNext(env, iter)){
                jobject value = Java::Runtime::Iterator::next(env, iter);
                if(!Java::Runtime::Collection::contains(env, tmp, value))
                    Java::Runtime::Collection::addAll(env, tmp, value);
            }
        }
    }
}

void ReferenceCountingSetContainer::addRC(JNIEnv * env, jobject container, jobject value){
    if(container){
        JniLocalFrame frame(env, 200);
        if(jobject rc = rcContainer(env, container))
            Java::Runtime::Collection::add(env, rc, value);
    }
}

void ReferenceCountingSetContainer::addUniqueRC(JNIEnv * env, jobject container, jobject value){
    if(container){
        JniLocalFrame frame(env, 200);
        if(jobject rc = rcContainer(env, container)){
            if(!Java::Runtime::Collection::contains(env, rc, value))
                Java::Runtime::Collection::add(env, rc, value);
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

PointerRCListAccess::~PointerRCListAccess(){}

PointerRCListAccess::PointerRCListAccess(AbstractListAccess* containerAccess)
    : WrapperListAccess(containerAccess), ReferenceCountingSetContainer() {
    Q_ASSERT(containerAccess!=this);
}

PointerRCListAccess::PointerRCListAccess(PointerRCListAccess& other)
    : WrapperListAccess(other.WrapperListAccess::clone()), ReferenceCountingSetContainer() {}

PointerRCListAccess* PointerRCListAccess::clone(){
    return new PointerRCListAccess(*this);
}

void PointerRCListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::ArrayList::newInstance(env);
    auto iterator = elementIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next();
        jobject obj{nullptr};
        switch(elementType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(elementMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void PointerRCListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperListAccess::swap(env, container, container2);
    if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperListAccess::assign(env, container, container2);
    if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCListAccess::appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    WrapperListAccess::appendList(env, container, containerInfo);
    addAllRC(env, container.object, findContainer(env, containerInfo.object));
}

void PointerRCListAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    jobject oldValue = WrapperListAccess::at(env, container.container, index);
    WrapperListAccess::replace(env, container, index, value);
    if(oldValue && !WrapperListAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
    if(value)
        addRC(env, container.object, value);
}

jint PointerRCListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = WrapperListAccess::removeAll(env, container, value);
    removeRC(env, container.object, value, result);
    return result;
}

void PointerRCListAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    WrapperListAccess::insert(env, container, index, n, value);
    if(value)
        addRC(env, container.object, value);
}

void PointerRCListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperListAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCListAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    if(n==1){
        jobject oldValue = WrapperListAccess::at(env, container.container, index);
        WrapperListAccess::remove(env, container, index, n);
        removeRC(env, container.object, oldValue);
    }else{
        jint size = WrapperListAccess::size(env, container.container);
        jobject removedValues = Java::Runtime::ArrayList::newInstance(env);
        for(jint i = index; i<=index+n && i<size; ++i){
            Java::Runtime::Collection::add(env, removedValues, WrapperListAccess::at(env, container.container, i));
        }
        WrapperListAccess::remove(env, container, index, n);
        jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            removeRC(env, container.object, value);
        }
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void PointerRCListAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    jint oldSize = WrapperListAccess::size(env, container.container);
    WrapperListAccess::fill(env, container, value, size);
    for(;oldSize<size;++oldSize){
        addRC(env, container.object, value);
    }
}
#endif

PointerRCSetAccess::~PointerRCSetAccess(){}

PointerRCSetAccess::PointerRCSetAccess(AbstractSetAccess* containerAccess)
    : WrapperSetAccess(containerAccess), ReferenceCountingSetContainer() {}

PointerRCSetAccess::PointerRCSetAccess(PointerRCSetAccess& other)
    : WrapperSetAccess(other.WrapperSetAccess::clone()), ReferenceCountingSetContainer() {}

PointerRCSetAccess* PointerRCSetAccess::clone(){
    return new PointerRCSetAccess(*this);
}

void PointerRCSetAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = elementIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next();
        jobject obj{nullptr};
        switch(elementType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(elementMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void PointerRCSetAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperSetAccess::swap(env, container, container2);
    if(PointerRCSetAccess* access = dynamic_cast<PointerRCSetAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCSetAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperSetAccess::assign(env, container, container2);
    if(PointerRCSetAccess* access = dynamic_cast<PointerRCSetAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCSetAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperSetAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCSetAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject value){
    WrapperSetAccess::insert(env, container, value);
    addUniqueRC(env, container.object, value);
}

jboolean PointerRCSetAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject value){
    jboolean result = WrapperSetAccess::remove(env, container, value);
    if(value)
        removeRC(env, container.object, value);
    return result;
}

void PointerRCSetAccess::intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    WrapperSetAccess::intersect(env, container, other);
    updateRC(env, container);
}

void PointerRCSetAccess::subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    WrapperSetAccess::subtract(env, container, other);
    updateRC(env, container);
}

void PointerRCSetAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    WrapperSetAccess::unite(env, container, other);
    updateRC(env, container);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

PointerRCLinkedListAccess::~PointerRCLinkedListAccess(){}

PointerRCLinkedListAccess::PointerRCLinkedListAccess(AbstractLinkedListAccess* containerAccess)
    : WrapperLinkedListAccess(containerAccess), ReferenceCountingSetContainer() {}

PointerRCLinkedListAccess::PointerRCLinkedListAccess(PointerRCLinkedListAccess& other)
    : WrapperLinkedListAccess(other.WrapperLinkedListAccess::clone()), ReferenceCountingSetContainer() {}

PointerRCLinkedListAccess* PointerRCLinkedListAccess::clone(){
    return new PointerRCLinkedListAccess(*this);
}

void PointerRCLinkedListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperLinkedListAccess::swap(env, container, container2);
    if(PointerRCLinkedListAccess* access = dynamic_cast<PointerRCLinkedListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCLinkedListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperLinkedListAccess::assign(env, container, container2);
    if(PointerRCLinkedListAccess* access = dynamic_cast<PointerRCLinkedListAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCLinkedListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::ArrayList::newInstance(env);
    auto iterator = elementIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next();
        jobject obj{nullptr};
        switch(elementType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(elementMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void PointerRCLinkedListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperLinkedListAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCLinkedListAccess::append(JNIEnv * env, const ContainerInfo& container, jobject value) {
    WrapperLinkedListAccess::append(env, container, value);
    if(value)
        addRC(env, container.object, value);
}

void PointerRCLinkedListAccess::prepend(JNIEnv * env, const ContainerInfo& container, jobject value) {
    WrapperLinkedListAccess::prepend(env, container, value);
    if(value)
        addRC(env, container.object, value);
}

jboolean PointerRCLinkedListAccess::removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jboolean result = WrapperLinkedListAccess::removeOne(env, container, value);
    if(result && !WrapperLinkedListAccess::contains(env, container.container, value)){
        removeRC(env, container.object, value);
    }
    return result;
}

void PointerRCLinkedListAccess::removeFirst(JNIEnv * env, const ContainerInfo& container) {
    jobject oldValue = WrapperLinkedListAccess::first(env, container.container);
    WrapperLinkedListAccess::removeFirst(env, container);
    if(!WrapperLinkedListAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
}

void PointerRCLinkedListAccess::removeLast(JNIEnv * env, const ContainerInfo& container) {
    jobject oldValue = WrapperLinkedListAccess::last(env, container.container);
    WrapperLinkedListAccess::removeLast(env, container);
    if(!WrapperLinkedListAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
}

jint PointerRCLinkedListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = WrapperLinkedListAccess::removeAll(env, container, value);
    removeRC(env, container.object, value, result);
    return result;
}

jobject PointerRCLinkedListAccess::takeFirst(JNIEnv * env, const ContainerInfo& container) {
    jobject result = WrapperLinkedListAccess::takeFirst(env, container);
    removeRC(env, container.object, result);
    return result;
}

jobject PointerRCLinkedListAccess::takeLast(JNIEnv * env, const ContainerInfo& container) {
    jobject result = WrapperLinkedListAccess::takeLast(env, container);
    removeRC(env, container.object, result);
    return result;
}

PointerRCVectorAccess::~PointerRCVectorAccess(){}

PointerRCVectorAccess::PointerRCVectorAccess(AbstractVectorAccess* containerAccess)
    : WrapperVectorAccess(containerAccess), ReferenceCountingSetContainer() {}

PointerRCVectorAccess::PointerRCVectorAccess(PointerRCVectorAccess& other)
    : WrapperVectorAccess(other.WrapperVectorAccess::clone()), ReferenceCountingSetContainer() {}

PointerRCVectorAccess* PointerRCVectorAccess::clone(){
    return new PointerRCVectorAccess(*this);
}

void PointerRCVectorAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::ArrayList::newInstance(env);
    auto iterator = elementIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next();
        jobject obj{nullptr};
        switch(elementType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(elementMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void PointerRCVectorAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperVectorAccess::swap(env, container, container2);
    if(PointerRCVectorAccess* access = dynamic_cast<PointerRCVectorAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCVectorAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperVectorAccess::assign(env, container, container2);
    if(PointerRCVectorAccess* access = dynamic_cast<PointerRCVectorAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCVectorAccess::appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    WrapperVectorAccess::appendVector(env, container, containerInfo);
    addAllRC(env, container.object, findContainer(env, containerInfo.object));
}

void PointerRCVectorAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    jobject oldValue = WrapperVectorAccess::at(env, container.container, index);
    WrapperVectorAccess::replace(env, container, index, value);
    if(oldValue && !WrapperVectorAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
    if(value)
        addRC(env, container.object, value);
}

jint PointerRCVectorAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = WrapperVectorAccess::removeAll(env, container, value);
    removeRC(env, container.object, value, result);
    return result;
}

void PointerRCVectorAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperVectorAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCVectorAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    jint size = WrapperVectorAccess::size(env, container.container);
    jobject removedValues = Java::Runtime::ArrayList::newInstance(env);
    for(jint i = index; i<=index+n && i<size; ++i){
        Java::Runtime::Collection::add(env, removedValues, WrapperVectorAccess::at(env, container.container, i));
    }
    WrapperVectorAccess::remove(env, container, index, n);
    jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
    while(Java::Runtime::Iterator::hasNext(env, iter)){
        jobject value = Java::Runtime::Iterator::next(env, iter);
        removeRC(env, container.object, value);
    }
}

void PointerRCVectorAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    WrapperVectorAccess::insert(env, container, index, n, value);
    addRC(env, container.object, value);
}

void PointerRCVectorAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    jint oldSize = WrapperVectorAccess::size(env, container.container);
    WrapperVectorAccess::fill(env, container, value, size);
    for(;oldSize<size;++oldSize){
        addRC(env, container.object, value);
    }
}

#endif

KeyPointerRCMapAccess::~KeyPointerRCMapAccess(){}

KeyPointerRCMapAccess::KeyPointerRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), ReferenceCountingSetContainer() {}

KeyPointerRCMapAccess::KeyPointerRCMapAccess(KeyPointerRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), ReferenceCountingSetContainer() {}

KeyPointerRCMapAccess* KeyPointerRCMapAccess::clone(){
    return new KeyPointerRCMapAccess(*this);
}

void KeyPointerRCMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next().first;
        jobject obj{nullptr};
        switch(keyType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void KeyPointerRCMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMapAccess::swap(env, container, container2);
    if(KeyPointerRCMapAccess* access = dynamic_cast<KeyPointerRCMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMapAccess::assign(env, container, container2);
    if(KeyPointerRCMapAccess* access = dynamic_cast<KeyPointerRCMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMapAccess::insert(env, container, key, value);
    addUniqueRC(env, container.object, key);
}

jint KeyPointerRCMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject KeyPointerRCMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

ValuePointerRCMapAccess::~ValuePointerRCMapAccess(){}

ValuePointerRCMapAccess::ValuePointerRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), ReferenceCountingSetContainer() {}

ValuePointerRCMapAccess::ValuePointerRCMapAccess(ValuePointerRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), ReferenceCountingSetContainer() {}

ValuePointerRCMapAccess* ValuePointerRCMapAccess::clone(){
    return new ValuePointerRCMapAccess(*this);
}

void ValuePointerRCMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next().second;
        jobject obj{nullptr};
        switch(valueType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void ValuePointerRCMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMapAccess::swap(env, container, container2);
    if(ValuePointerRCMapAccess* access = dynamic_cast<ValuePointerRCMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMapAccess::assign(env, container, container2);
    if(ValuePointerRCMapAccess* access = dynamic_cast<ValuePointerRCMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = WrapperMapAccess::value(env, container.container, key, nullptr);
    WrapperMapAccess::insert(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

jint ValuePointerRCMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jobject oldValue = WrapperMapAccess::value(env, container.container, key, nullptr);
    jint result = WrapperMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, oldValue);
    }
    return result;
}

jobject ValuePointerRCMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    removeRC(env, container.object, result);
    return result;
}

PointersRCMapAccess::PointersRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), ReferenceCountingMapContainer() {}

PointersRCMapAccess::PointersRCMapAccess(PointersRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), ReferenceCountingMapContainer(other) {}

PointersRCMapAccess* PointersRCMapAccess::clone(){
    return new PointersRCMapAccess(*this);
}

void PointersRCMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::Runtime::HashMap::newInstance(env, 0);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject key{nullptr};
        switch(keyType()){
        case PointerToQObject:
            key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                key = QtJambiAPI::findFunctionPointerObject(env, content.first, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            key = QtJambiAPI::findObject(env, content.first);
            break;
        default:
            break;
        }
        jobject value{nullptr};
        switch(valueType()){
        case PointerToQObject:
            value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                value = QtJambiAPI::findFunctionPointerObject(env, content.second, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            value = QtJambiAPI::findObject(env, content.second);
            break;
        default:
            break;
        }
        Java::Runtime::Map::put(env, map, key, value);
    }
    clearRC(env, container.object);
    putAllRC(env, container.object, map);
}

void PointersRCMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMapAccess::swap(env, container, container2);
    if(PointersRCMapAccess* access = dynamic_cast<PointersRCMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMapAccess::assign(env, container, container2);
    if(PointersRCMapAccess* access = dynamic_cast<PointersRCMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointersRCMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointersRCMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMapAccess::insert(env, container, key, value);
    putRC(env, container.object, key, value);
}

jint PointersRCMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, result);
    return result;
}

jobject PointersRCMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

KeyPointerRCMultiMapAccess::~KeyPointerRCMultiMapAccess(){}

KeyPointerRCMultiMapAccess::KeyPointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), ReferenceCountingSetContainer() {}

KeyPointerRCMultiMapAccess::KeyPointerRCMultiMapAccess(KeyPointerRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), ReferenceCountingSetContainer() {}

KeyPointerRCMultiMapAccess* KeyPointerRCMultiMapAccess::clone(){
    return new KeyPointerRCMultiMapAccess(*this);
}

void KeyPointerRCMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(keyType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content.first, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content.first);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void KeyPointerRCMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::swap(env, container, container2);
    if(KeyPointerRCMultiMapAccess* access = dynamic_cast<KeyPointerRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::assign(env, container, container2);
    if(KeyPointerRCMultiMapAccess* access = dynamic_cast<KeyPointerRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    addRC(env, container.object, key);
}

jint KeyPointerRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, result);
    return result;
}

jint KeyPointerRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    removeRC(env, container.object, key, result);
    return result;
}

jobject KeyPointerRCMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

void KeyPointerRCMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiMapAccess::replace(env, container, key, value);
    addRC(env, container.object, key);
}

void KeyPointerRCMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

void KeyPointerRCMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

ValuePointerRCMultiMapAccess::ValuePointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), ReferenceCountingSetContainer() {}

ValuePointerRCMultiMapAccess::ValuePointerRCMultiMapAccess(ValuePointerRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), ReferenceCountingSetContainer() {}

ValuePointerRCMultiMapAccess* ValuePointerRCMultiMapAccess::clone(){
    return new ValuePointerRCMultiMapAccess(*this);
}

void ValuePointerRCMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(valueType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content.second, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content.second);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void ValuePointerRCMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::swap(env, container, container2);
    if(ValuePointerRCMultiMapAccess* access = dynamic_cast<ValuePointerRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::assign(env, container, container2);
    if(ValuePointerRCMultiMapAccess* access = dynamic_cast<ValuePointerRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    addRC(env, container.object, value);
}

jint ValuePointerRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    ContainerAndAccessInfo oldValues = WrapperMultiMapAccess::values(env, container, key);
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    if(result>0){
        jobject iter = Java::Runtime::Collection::iterator(env, oldValues.object);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(Java::Runtime::Collection::size(env, WrapperMultiMapAccess::keys(env, container, value).object)==0){
                removeRC(env, container.object, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    if(Java::Runtime::Collection::size(env, WrapperMultiMapAccess::keys(env, container, result).object)==0){
        removeRC(env, container.object, result);
    }
    return result;
}

void ValuePointerRCMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = WrapperMultiMapAccess::value(env, container.container, key, nullptr);
    WrapperMultiMapAccess::replace(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

void ValuePointerRCMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

void ValuePointerRCMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

jint ValuePointerRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    removeRC(env, container.object, value, result);
    return result;
}

PointersRCMultiMapAccess::PointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), ReferenceCountingMultiMapContainer() {}

PointersRCMultiMapAccess::PointersRCMultiMapAccess(PointersRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), ReferenceCountingMultiMapContainer(other) {}

PointersRCMultiMapAccess* PointersRCMultiMapAccess::clone(){
    return new PointersRCMultiMapAccess(*this);
}

void PointersRCMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject key{nullptr};
        switch(keyType()){
        case PointerToQObject:
            key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                key = QtJambiAPI::findFunctionPointerObject(env, content.first, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            key = QtJambiAPI::findObject(env, content.first);
            break;
        default:
            break;
        }
        jobject value{nullptr};
        switch(valueType()){
        case PointerToQObject:
            value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                value = QtJambiAPI::findFunctionPointerObject(env, content.second, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            value = QtJambiAPI::findObject(env, content.second);
            break;
        default:
            break;
        }
        Java::Runtime::Map::put(env, map, key, value);
    }
    clearRC(env, container.object);
    putAllRC(env, container.object, map);
}

void PointersRCMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::swap(env, container, container2);
    if(PointersRCMultiMapAccess* access = dynamic_cast<PointersRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::assign(env, container, container2);
    if(PointersRCMultiMapAccess* access = dynamic_cast<PointersRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointersRCMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointersRCMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    putRC(env, container.object, key, value);
}

void PointersRCMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiMapAccess::replace(env, container, key, value);
    removeRC(env, container.object, key);
    putRC(env, container.object, key, value);
}

void PointersRCMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

jint PointersRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, 1);
    return result;
}

jint PointersRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    removeRC(env, container.object, key, value, result);
    return result;
}

jobject PointersRCMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    removeRC(env, key, result);
    return result;
}

KeyPointerRCHashAccess::~KeyPointerRCHashAccess(){}

KeyPointerRCHashAccess::KeyPointerRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), ReferenceCountingSetContainer() {}

KeyPointerRCHashAccess::KeyPointerRCHashAccess(KeyPointerRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), ReferenceCountingSetContainer() {}

KeyPointerRCHashAccess* KeyPointerRCHashAccess::clone(){
    return new KeyPointerRCHashAccess(*this);
}

void KeyPointerRCHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(keyType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content.first, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content.first);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void KeyPointerRCHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperHashAccess::swap(env, container, container2);
    if(KeyPointerRCHashAccess* access = dynamic_cast<KeyPointerRCHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperHashAccess::assign(env, container, container2);
    if(KeyPointerRCHashAccess* access = dynamic_cast<KeyPointerRCHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperHashAccess::insert(env, container, key, value);
    addUniqueRC(env, container.object, key);
}

jint KeyPointerRCHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperHashAccess::remove(env, container, key);
    removeRC(env, container.object, key, result);
    return result;
}

jobject KeyPointerRCHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

ValuePointerRCHashAccess::ValuePointerRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), ReferenceCountingSetContainer() {}

ValuePointerRCHashAccess::ValuePointerRCHashAccess(ValuePointerRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), ReferenceCountingSetContainer() {}

ValuePointerRCHashAccess* ValuePointerRCHashAccess::clone(){
    return new ValuePointerRCHashAccess(*this);
}

void ValuePointerRCHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(valueType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content.second, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content.second);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void ValuePointerRCHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperHashAccess::swap(env, container, container2);
    if(ValuePointerRCHashAccess* access = dynamic_cast<ValuePointerRCHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperHashAccess::assign(env, container, container2);
    if(ValuePointerRCHashAccess* access = dynamic_cast<ValuePointerRCHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = WrapperHashAccess::value(env, container.container, key, nullptr);
    WrapperHashAccess::insert(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

jint ValuePointerRCHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jobject oldValue = WrapperHashAccess::value(env, container.container, key, nullptr);
    jint result = WrapperHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, oldValue);
    }
    return result;
}

jobject ValuePointerRCHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, container.object, result);
    return result;
}

PointersRCHashAccess::PointersRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), ReferenceCountingMapContainer() {}

PointersRCHashAccess::PointersRCHashAccess(PointersRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), ReferenceCountingMapContainer(other) {}

PointersRCHashAccess* PointersRCHashAccess::clone(){
    return new PointersRCHashAccess(*this);
}

void PointersRCHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject key{nullptr};
        switch(keyType()){
        case PointerToQObject:
            key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                key = QtJambiAPI::findFunctionPointerObject(env, content.first, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            key = QtJambiAPI::findObject(env, content.first);
            break;
        default:
            break;
        }
        jobject value{nullptr};
        switch(valueType()){
        case PointerToQObject:
            value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                value = QtJambiAPI::findFunctionPointerObject(env, content.second, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            value = QtJambiAPI::findObject(env, content.second);
            break;
        default:
            break;
        }
        Java::Runtime::Map::put(env, map, key, value);
    }
    clearRC(env, container.object);
    putAllRC(env, container.object, map);
}

void PointersRCHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperHashAccess::swap(env, container, container2);
    if(PointersRCHashAccess* access = dynamic_cast<PointersRCHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperHashAccess::assign(env, container, container2);
    if(PointersRCHashAccess* access = dynamic_cast<PointersRCHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointersRCHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointersRCHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperHashAccess::insert(env, container, key, value);
    putRC(env, container.object, key, value);
}

jint PointersRCHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject PointersRCHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

KeyPointerRCMultiHashAccess::~KeyPointerRCMultiHashAccess(){}

KeyPointerRCMultiHashAccess::KeyPointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), ReferenceCountingSetContainer() {}

KeyPointerRCMultiHashAccess::KeyPointerRCMultiHashAccess(KeyPointerRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), ReferenceCountingSetContainer() {}

KeyPointerRCMultiHashAccess* KeyPointerRCMultiHashAccess::clone(){
    return new KeyPointerRCMultiHashAccess(*this);
}

void KeyPointerRCMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(keyType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content.first, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content.first);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void KeyPointerRCMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::swap(env, container, container2);
    if(KeyPointerRCMultiHashAccess* access = dynamic_cast<KeyPointerRCMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::assign(env, container, container2);
    if(KeyPointerRCMultiHashAccess* access = dynamic_cast<KeyPointerRCMultiHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    addRC(env, container.object, key);
}

jint KeyPointerRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jint KeyPointerRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject KeyPointerRCMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

void KeyPointerRCMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiHashAccess::replace(env, container, key, value);
}

ValuePointerRCMultiHashAccess::ValuePointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), ReferenceCountingSetContainer() {}

ValuePointerRCMultiHashAccess::ValuePointerRCMultiHashAccess(ValuePointerRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), ReferenceCountingSetContainer() {}

ValuePointerRCMultiHashAccess* ValuePointerRCMultiHashAccess::clone(){
    return new ValuePointerRCMultiHashAccess(*this);
}

void ValuePointerRCMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(valueType()){
        case PointerToQObject:
            obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                obj = QtJambiAPI::findFunctionPointerObject(env, content.second, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            obj = QtJambiAPI::findObject(env, content.second);
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void ValuePointerRCMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::swap(env, container, container2);
    if(ValuePointerRCMultiHashAccess* access = dynamic_cast<ValuePointerRCMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::assign(env, container, container2);
    if(ValuePointerRCMultiHashAccess* access = dynamic_cast<ValuePointerRCMultiHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    addRC(env, container.object, value);
}

jint ValuePointerRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    ContainerAndAccessInfo oldValues = WrapperMultiHashAccess::values(env, container, key);
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    if(result>0){
        jobject iter = Java::Runtime::Collection::iterator(env, oldValues.object);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(Java::Runtime::Collection::size(env, WrapperMultiHashAccess::keys(env, container, value).object)==0){
                removeRC(env, container.object, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    if(Java::Runtime::Collection::size(env, WrapperMultiHashAccess::keys(env, container, result).object)==0){
        removeRC(env, container.object, result);
    }
    return result;
}

void ValuePointerRCMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = WrapperMultiHashAccess::value(env, container.container, key, nullptr);
    WrapperMultiHashAccess::replace(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

jint ValuePointerRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    if(result>0)
        removeRC(env, container.object, value, result);
    return result;
}

PointersRCMultiHashAccess::PointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), ReferenceCountingMultiMapContainer() {}

PointersRCMultiHashAccess::PointersRCMultiHashAccess(PointersRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), ReferenceCountingMultiMapContainer(other) {}

PointersRCMultiHashAccess* PointersRCMultiHashAccess::clone(){
    return new PointersRCMultiHashAccess(*this);
}

void PointersRCMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject key{nullptr};
        switch(keyType()){
        case PointerToQObject:
            key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                key = QtJambiAPI::findFunctionPointerObject(env, content.first, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            key = QtJambiAPI::findObject(env, content.first);
            break;
        default:
            break;
        }
        jobject value{nullptr};
        switch(valueType()){
        case PointerToQObject:
            value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                value = QtJambiAPI::findFunctionPointerObject(env, content.second, *typeId);
                break;
            }
            Q_FALLTHROUGH();
        case Pointer:
            value = QtJambiAPI::findObject(env, content.second);
            break;
        default:
            break;
        }
        Java::Runtime::Map::put(env, map, key, value);
    }
    clearRC(env, container.object);
    putAllRC(env, container.object, map);
}

void PointersRCMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::swap(env, container, container2);
    if(PointersRCMultiHashAccess* access = dynamic_cast<PointersRCMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::assign(env, container, container2);
    updateRC(env, container);
}

void PointersRCMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiHashAccess::clear(env, container);
    updateRC(env, container);
}

void PointersRCMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    updateRC(env, container);
}

void PointersRCMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiHashAccess::replace(env, container, key, value);
    updateRC(env, container);
}

void PointersRCMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

jint PointersRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    updateRC(env, container);
    return result;
}

jint PointersRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    updateRC(env, container);
    return result;
}

jobject PointersRCMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    removeRC(env, key, result);
    return result;
}

NestedPointersRCListAccess::NestedPointersRCListAccess(AbstractListAccess* containerAccess)
    : WrapperListAccess(containerAccess), ReferenceCountingSetContainer() {
    Q_ASSERT(containerAccess!=this);
}

NestedPointersRCListAccess::NestedPointersRCListAccess(NestedPointersRCListAccess& other)
    : WrapperListAccess(other.WrapperListAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCListAccess* NestedPointersRCListAccess::clone(){
    return new NestedPointersRCListAccess(*this);
}

void NestedPointersRCListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperListAccess::swap(env, container, container2);
    if(NestedPointersRCListAccess* access = dynamic_cast<NestedPointersRCListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperListAccess::assign(env, container, container2);
    updateRC(env, container);
}

void NestedPointersRCListAccess::appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    WrapperListAccess::appendList(env, container, containerInfo);
    updateRC(env, container);
}

void NestedPointersRCListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access = elementNestedContainerAccess();
        auto iterator = elementIterator(container.container);
        while(iterator->hasNext()){
            unfoldAndAddContainer(env, set, iterator->next(), elementType(), elementMetaType(), access);
        }
        if(access)
            access->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCListAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    WrapperListAccess::replace(env, container, index, value);
    updateRC(env, container);
}

jint NestedPointersRCListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = WrapperListAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

ContainerAndAccessInfo NestedPointersRCListAccess::mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) {
    ContainerAndAccessInfo result = WrapperListAccess::mid(env, container, index1, index2);
    return result;
}

void NestedPointersRCListAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    WrapperListAccess::insert(env, container, index, n, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperListAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCListAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    WrapperListAccess::remove(env, container, index, n);
    updateRC(env, container);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void NestedPointersRCListAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    WrapperListAccess::fill(env, container, value, size);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}
#endif

NestedPointersRCSetAccess::NestedPointersRCSetAccess(AbstractSetAccess* containerAccess)
    : WrapperSetAccess(containerAccess), ReferenceCountingSetContainer() {}

NestedPointersRCSetAccess::NestedPointersRCSetAccess(NestedPointersRCSetAccess& other)
    : WrapperSetAccess(other.WrapperSetAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCSetAccess* NestedPointersRCSetAccess::clone(){
    return new NestedPointersRCSetAccess(*this);
}

void NestedPointersRCSetAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access = elementNestedContainerAccess();
        auto iterator = elementIterator(container.container);
        while(iterator->hasNext()){
            unfoldAndAddContainer(env, set, iterator->next(), elementType(), elementMetaType(), access);
        }
        if(access)
            access->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCSetAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperSetAccess::swap(env, container, container2);
    if(NestedPointersRCSetAccess* access = dynamic_cast<NestedPointersRCSetAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCSetAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperSetAccess::assign(env, container, container2);
    updateRC(env, container);
}

void NestedPointersRCSetAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperSetAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCSetAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject value){
    WrapperSetAccess::insert(env, container, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

jboolean NestedPointersRCSetAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject value){
    jboolean result = WrapperSetAccess::remove(env, container, value);
    updateRC(env, container);
    return result;
}

void NestedPointersRCSetAccess::intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    WrapperSetAccess::intersect(env, container, other);
    updateRC(env, container);
}

void NestedPointersRCSetAccess::subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    WrapperSetAccess::subtract(env, container, other);
    updateRC(env, container);
}

void NestedPointersRCSetAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    WrapperSetAccess::unite(env, container, other);
    updateRC(env, container);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

NestedPointersRCLinkedListAccess::NestedPointersRCLinkedListAccess(AbstractLinkedListAccess* containerAccess)
    : WrapperLinkedListAccess(containerAccess), ReferenceCountingSetContainer() {}

NestedPointersRCLinkedListAccess::NestedPointersRCLinkedListAccess(NestedPointersRCLinkedListAccess& other)
    : WrapperLinkedListAccess(other.WrapperLinkedListAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCLinkedListAccess* NestedPointersRCLinkedListAccess::clone(){
    return new NestedPointersRCLinkedListAccess(*this);
}

void NestedPointersRCLinkedListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access = elementNestedContainerAccess();
        auto iterator = elementIterator(container.container);
        while(iterator->hasNext()){
            unfoldAndAddContainer(env, set, iterator->next(), elementType(), elementMetaType(), access);
        }
        if(access)
            access->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCLinkedListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperLinkedListAccess::swap(env, container, container2);
    if(NestedPointersRCLinkedListAccess* access = dynamic_cast<NestedPointersRCLinkedListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCLinkedListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperLinkedListAccess::assign(env, container, container2);
    if(NestedPointersRCLinkedListAccess* access = dynamic_cast<NestedPointersRCLinkedListAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCLinkedListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperLinkedListAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCLinkedListAccess::append(JNIEnv * env, const ContainerInfo& container, jobject value) {
    WrapperLinkedListAccess::append(env, container, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCLinkedListAccess::prepend(JNIEnv * env, const ContainerInfo& container, jobject value) {
    WrapperLinkedListAccess::prepend(env, container, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

jboolean NestedPointersRCLinkedListAccess::removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jboolean result = WrapperLinkedListAccess::removeOne(env, container, value);
    if(result){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCLinkedListAccess::removeFirst(JNIEnv * env, const ContainerInfo& container) {
    WrapperLinkedListAccess::removeFirst(env, container);
    updateRC(env, container);
}

void NestedPointersRCLinkedListAccess::removeLast(JNIEnv * env, const ContainerInfo& container) {
    WrapperLinkedListAccess::removeLast(env, container);
    updateRC(env, container);
}

jint NestedPointersRCLinkedListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = WrapperLinkedListAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCLinkedListAccess::takeFirst(JNIEnv * env, const ContainerInfo& container) {
    jobject result = WrapperLinkedListAccess::takeFirst(env, container);
    updateRC(env, container);
    return result;
}

jobject NestedPointersRCLinkedListAccess::takeLast(JNIEnv * env, const ContainerInfo& container) {
    jobject result = WrapperLinkedListAccess::takeLast(env, container);
    updateRC(env, container);
    return result;
}

NestedPointersRCVectorAccess::NestedPointersRCVectorAccess(AbstractVectorAccess* containerAccess)
    : WrapperVectorAccess(containerAccess), ReferenceCountingSetContainer() {}

NestedPointersRCVectorAccess::NestedPointersRCVectorAccess(NestedPointersRCVectorAccess& other)
    : WrapperVectorAccess(other.WrapperVectorAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCVectorAccess* NestedPointersRCVectorAccess::clone(){
    return new NestedPointersRCVectorAccess(*this);
}

void NestedPointersRCVectorAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access = elementNestedContainerAccess();
        auto iterator = elementIterator(container.container);
        while(iterator->hasNext()){
            unfoldAndAddContainer(env, set, iterator->next(), elementType(), elementMetaType(), access);
        }
        if(access)
            access->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCVectorAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperVectorAccess::swap(env, container, container2);
    if(NestedPointersRCVectorAccess* access = dynamic_cast<NestedPointersRCVectorAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCVectorAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperVectorAccess::assign(env, container, container2);
    if(NestedPointersRCVectorAccess* access = dynamic_cast<NestedPointersRCVectorAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCVectorAccess::appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    WrapperVectorAccess::appendVector(env, container, containerInfo);
    updateRC(env, container);
}

jint NestedPointersRCVectorAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = WrapperVectorAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCVectorAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperVectorAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCVectorAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    WrapperVectorAccess::remove(env, container, index, n);
    updateRC(env, container);
}

void NestedPointersRCVectorAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    WrapperVectorAccess::insert(env, container, index, n, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCVectorAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    WrapperVectorAccess::fill(env, container, value, size);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCVectorAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value){
    WrapperVectorAccess::replace(env, container, index, value);
    updateRC(env, container);
}

#endif


NestedPointersRCMapAccess::NestedPointersRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), ReferenceCountingSetContainer() {}

NestedPointersRCMapAccess::NestedPointersRCMapAccess(NestedPointersRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCMapAccess* NestedPointersRCMapAccess::clone(){
    return new NestedPointersRCMapAccess(*this);
}

void NestedPointersRCMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access1 = keyNestedContainerAccess();
        auto access2 = valueNestedContainerAccess();
        auto iterator = constKeyValueIterator(container.container);
        while(iterator->hasNext()){
            auto current = iterator->next();
            unfoldAndAddContainer(env, set, current.first, keyType(), keyMetaType(), access1);
            unfoldAndAddContainer(env, set, current.second, valueType(), valueMetaType(), access2);
        }
        if(access1)
            access1->dispose();
        if(access2)
            access2->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMapAccess::swap(env, container, container2);
    if(NestedPointersRCMapAccess* access = dynamic_cast<NestedPointersRCMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMapAccess::assign(env, container, container2);
    if(NestedPointersRCMapAccess* access = dynamic_cast<NestedPointersRCMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMapAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

jint NestedPointersRCMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMapAccess::remove(env, container, key);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}

NestedPointersRCMultiMapAccess::NestedPointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), ReferenceCountingSetContainer() {}

NestedPointersRCMultiMapAccess::NestedPointersRCMultiMapAccess(NestedPointersRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCMultiMapAccess* NestedPointersRCMultiMapAccess::clone(){
    return new NestedPointersRCMultiMapAccess(*this);
}

void NestedPointersRCMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access1 = keyNestedContainerAccess();
        auto access2 = valueNestedContainerAccess();
        auto iterator = constKeyValueIterator(container.container);
        while(iterator->hasNext()){
            auto current = iterator->next();
            unfoldAndAddContainer(env, set, current.first, keyType(), keyMetaType(), access1);
            unfoldAndAddContainer(env, set, current.second, valueType(), valueMetaType(), access2);
        }
        if(access1)
            access1->dispose();
        if(access2)
            access2->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::swap(env, container, container2);
    if(NestedPointersRCMultiMapAccess* access = dynamic_cast<NestedPointersRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiMapAccess::assign(env, container, container2);
    if(NestedPointersRCMultiMapAccess* access = dynamic_cast<NestedPointersRCMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

void NestedPointersRCMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiMapAccess::replace(env, container, key, value);
    updateRC(env, container);
}


void NestedPointersRCMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

jint NestedPointersRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jint NestedPointersRCMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}

NestedPointersRCHashAccess::NestedPointersRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), ReferenceCountingSetContainer() {}

NestedPointersRCHashAccess::NestedPointersRCHashAccess(NestedPointersRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCHashAccess* NestedPointersRCHashAccess::clone(){
    return new NestedPointersRCHashAccess(*this);
}

void NestedPointersRCHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access1 = keyNestedContainerAccess();
        auto access2 = valueNestedContainerAccess();
        auto iterator = constKeyValueIterator(container.container);
        while(iterator->hasNext()){
            auto current = iterator->next();
            unfoldAndAddContainer(env, set, current.first, keyType(), keyMetaType(), access1);
            unfoldAndAddContainer(env, set, current.second, valueType(), valueMetaType(), access2);
        }
        if(access1)
            access1->dispose();
        if(access2)
            access2->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperHashAccess::swap(env, container, container2);
    if(NestedPointersRCHashAccess* access = dynamic_cast<NestedPointersRCHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperHashAccess::assign(env, container, container2);
    if(NestedPointersRCHashAccess* access = dynamic_cast<NestedPointersRCHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperHashAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

jint NestedPointersRCHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject NestedPointersRCHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

NestedPointersRCMultiHashAccess::NestedPointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), ReferenceCountingSetContainer() {}

NestedPointersRCMultiHashAccess::NestedPointersRCMultiHashAccess(NestedPointersRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), ReferenceCountingSetContainer() {}

NestedPointersRCMultiHashAccess* NestedPointersRCMultiHashAccess::clone(){
    return new NestedPointersRCMultiHashAccess(*this);
}

void NestedPointersRCMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access1 = keyNestedContainerAccess();
        auto access2 = valueNestedContainerAccess();
        auto iterator = constKeyValueIterator(container.container);
        while(iterator->hasNext()){
            auto current = iterator->next();
            unfoldAndAddContainer(env, set, current.first, keyType(), keyMetaType(), access1);
            unfoldAndAddContainer(env, set, current.second, valueType(), valueMetaType(), access2);
        }
        if(access1)
            access1->dispose();
        if(access2)
            access2->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::swap(env, container, container2);
    if(NestedPointersRCMultiHashAccess* access = dynamic_cast<NestedPointersRCMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    WrapperMultiHashAccess::assign(env, container, container2);
    if(NestedPointersRCMultiHashAccess* access = dynamic_cast<NestedPointersRCMultiHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    WrapperMultiHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

void NestedPointersRCMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    WrapperMultiHashAccess::replace(env, container, key, value);
    updateRC(env, container);
}

void NestedPointersRCMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    WrapperMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

jint NestedPointersRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jint NestedPointersRCMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}

#if defined(QTJAMBI_GENERIC_ACCESS)
void registerPointerContainerAccess(){
    using namespace ContainerAccessAPI;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    SequentialContainerAccessFactoryHelper<QLinkedList, 0, 0, false>::registerContainerAccessFactory();
    SequentialContainerAccessFactoryHelper<QVector, 0, 0, false>::registerContainerAccessFactory();
    SequentialContainerAccessFactoryHelper<QList, 0, 0, true>::registerContainerAccessFactory();
#endif
    SequentialContainerAccessFactoryHelper<QList, 0, 0, false>::registerContainerAccessFactory();
    SequentialContainerAccessFactoryHelper<QSet, 0, 0, false>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QHash,0, 0, 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMap,0, 0, 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMultiMap,0, 0, 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMultiHash,0, 0, 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QPair,0, 0, 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QHash, alignof(QString), sizeof(QString), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMap, alignof(QString), sizeof(QString), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMultiMap, alignof(QString), sizeof(QString), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMultiHash, alignof(QString), sizeof(QString), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QPair, alignof(QString), sizeof(QString), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QHash, alignof(int), sizeof(int), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMap, alignof(int), sizeof(int), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMultiMap, alignof(int), sizeof(int), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QMultiHash, alignof(int), sizeof(int), 0, 0>::registerContainerAccessFactory();
    AssociativeContainerAccessFactoryHelper<QPair, alignof(int), sizeof(int), 0, 0>::registerContainerAccessFactory();
}
#endif //defined(QTJAMBI_GENERIC_ACCESS)

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

struct SequentialContainerConverter;
typedef QVector<QSharedPointer<SequentialContainerConverter>> SequentialContainerConverterList;
Q_GLOBAL_STATIC(SequentialContainerConverterList, gSequentialContainerConverterList)
struct AssociativeContainerConverter;
typedef QVector<QSharedPointer<AssociativeContainerConverter>> AssociativeContainerConverterList;
Q_GLOBAL_STATIC(AssociativeContainerConverterList, gAssociativeContainerConverterList)
struct PairContainerConverter;
typedef QVector<QSharedPointer<PairContainerConverter>> PairConverterList;
Q_GLOBAL_STATIC(PairConverterList, gPairConverterList)

struct PairContainerConverter : QtPrivate::AbstractConverterFunction{
    explicit PairContainerConverter(QSharedPointer<AbstractPairAccess> pairAccess)
        : QtPrivate::AbstractConverterFunction(convert), m_pairAccess(pairAccess)
    {
        gPairConverterList->append(QSharedPointer<PairContainerConverter>(this));
    }

    static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
    {
        if(JniEnvironment env{500}){
            const PairContainerConverter* converter = static_cast<const PairContainerConverter *>(_this);
            const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(in);
            if(converter && javaObject && Java::QtCore::QPair::isInstanceOf(env, javaObject->object(env))){
                jobject first = Java::QtCore::QPair::first(env, javaObject->object(env));
                jobject second = Java::QtCore::QPair::second(env, javaObject->object(env));
                converter->m_pairAccess->setFirst(env, out, first);
                converter->m_pairAccess->setSecond(env, out, second);
                return true;
            }
        }
        return false;
    }
    QSharedPointer<AbstractPairAccess> m_pairAccess;
};

void registerContainerConverter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType){
    if(!QMetaType::hasRegisteredConverterFunction(qMetaTypeId<JObjectWrapper>(), containerMetaType.id())){
        registerConverter(new PairContainerConverter(pairAccess), qMetaTypeId<JObjectWrapper>(), containerMetaType.id());
    }
}

struct SequentialContainerConverter : QtPrivate::AbstractConverterFunction{
    explicit SequentialContainerConverter(SequentialContainerType containerType, const QMetaType& metaType)
        : QtPrivate::AbstractConverterFunction(convert),
          m_containerType(containerType),
          m_elementMetaType(metaType.id()
    ){
        gSequentialContainerConverterList->append(QSharedPointer<SequentialContainerConverter>(this));
    }

    static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
    {
        if(JniEnvironment env{500}){
            const SequentialContainerConverter* converter = static_cast<const SequentialContainerConverter *>(_this);
            const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(in);
            if(converter && javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, javaObject->object(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object(env))){
                    if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                        switch(converter->m_containerType){
                        case SequentialContainerType::QStack:
                        case SequentialContainerType::QVector:
                            if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(_containerAccess)){
                                if(converter->m_elementMetaType==containerAccess->elementMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case SequentialContainerType::QLinkedList:
                            if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(_containerAccess)){
                                if(converter->m_elementMetaType==containerAccess->elementMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case SequentialContainerType::QQueue:
                        case SequentialContainerType::QList:
                            if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(_containerAccess)){
                                if(converter->m_elementMetaType==containerAccess->elementMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case SequentialContainerType::QSet:
                            if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(_containerAccess)){
                                if(converter->m_elementMetaType==containerAccess->elementMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
        return false;
    }

    SequentialContainerType m_containerType;
    QMetaType m_elementMetaType;
};

void registerContainerConverter(SequentialContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& elementMetaType){
    if(!QMetaType::hasRegisteredConverterFunction(qMetaTypeId<JObjectWrapper>(), containerMetaType.id())){
        SequentialContainerConverter* converter = new SequentialContainerConverter(collectionType, elementMetaType);
        registerConverter(converter, qMetaTypeId<JObjectWrapper>(), containerMetaType.id());
        registerConverter(converter, qMetaTypeId<JCollectionWrapper>(), containerMetaType.id());
    }
}

struct AssociativeContainerConverter : QtPrivate::AbstractConverterFunction{
    explicit AssociativeContainerConverter(AssociativeContainerType containerType, const QMetaType& keyMetaType, const QMetaType& valueMetaType)
        : QtPrivate::AbstractConverterFunction(convert),
          m_containerType(containerType),
          m_keyMetaType(keyMetaType.id()),
          m_valueMetaType(valueMetaType.id())
    {
        gAssociativeContainerConverterList->append(QSharedPointer<AssociativeContainerConverter>(this));
    }

    static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
    {
        if(JniEnvironment env{500}){
            const AssociativeContainerConverter* converter = static_cast<const AssociativeContainerConverter *>(_this);
            const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(in);
            if(converter && javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, javaObject->object(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object(env))){
                    if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                        switch(converter->m_containerType){
                        case AssociativeContainerType::QMap:
                            if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(_containerAccess)){
                                if(converter->m_keyMetaType==containerAccess->keyMetaType()
                                        && converter->m_valueMetaType==containerAccess->valueMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case AssociativeContainerType::QMultiMap:
                            if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(_containerAccess)){
                                if(converter->m_keyMetaType==containerAccess->keyMetaType()
                                        && converter->m_valueMetaType==containerAccess->valueMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case AssociativeContainerType::QHash:
                            if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(_containerAccess)){
                                if(converter->m_keyMetaType==containerAccess->keyMetaType()
                                        && converter->m_valueMetaType==containerAccess->valueMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case AssociativeContainerType::QMultiHash:
                            if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(_containerAccess)){
                                if(converter->m_keyMetaType==containerAccess->keyMetaType()
                                        && converter->m_valueMetaType==containerAccess->valueMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        default: break;
                        }
                    }
                }
            }
        }
        return false;
    }

    AssociativeContainerType m_containerType;
    QMetaType m_keyMetaType;
    QMetaType m_valueMetaType;
};

void registerContainerConverter(AssociativeContainerType mapType, const QMetaType& containerMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType){
    if(!QMetaType::hasRegisteredConverterFunction(qMetaTypeId<JObjectWrapper>(), containerMetaType.id())){
        AssociativeContainerConverter* converter = new AssociativeContainerConverter(mapType, keyMetaType, valueMetaType);
        registerConverter(converter, qMetaTypeId<JObjectWrapper>(), containerMetaType.id());
        registerConverter(converter, qMetaTypeId<JCollectionWrapper>(), containerMetaType.id());
    }
}

#else

void registerContainerConverter(SequentialContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& _elementMetaType){
    QMetaType jCollectionWrapperType = QMetaType::fromType<JCollectionWrapper>();
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    QMetaType elementMetaType = _elementMetaType;
    if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
            || !QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        QMetaType::ConverterFunction converter = [collectionType,elementMetaType](const void *src, void *target) -> bool {
            if(src){
                if(JniEnvironment env{500}){
                    const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(src);
                    jobject jobj;
                    if(javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, jobj = javaObject->object(env))){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jobj)){
                            if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                switch(collectionType){
                                case SequentialContainerType::QStack:
                                case SequentialContainerType::QQueue:
                                case SequentialContainerType::QList:
                                    if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case SequentialContainerType::QSet:
                                    if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                                case SequentialContainerType::QConstSpan:
                                case SequentialContainerType::QSpan:
                                    if(AbstractSpanAccess* containerAccess = dynamic_cast<AbstractSpanAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                                }
                            }
                        }
                    }
                }
            }
            return false;
        };
        if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                && collectionType!=SequentialContainerType::QConstSpan
                && collectionType!=SequentialContainerType::QSpan
#endif
                )
            QMetaType::registerConverterFunction(converter, jCollectionWrapperType, containerMetaType);
        if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType))
            QMetaType::registerConverterFunction(converter, jObjectWrapperType, containerMetaType);
    }
}

void registerContainerConverter(AssociativeContainerType mapType, const QMetaType& containerMetaType, const QMetaType& _keyMetaType, const QMetaType& _valueMetaType){
    QMetaType jCollectionWrapperType = QMetaType::fromType<JCollectionWrapper>();
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    QMetaType keyMetaType = _keyMetaType;
    QMetaType valueMetaType = _valueMetaType;
    if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
            || !QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        QMetaType::ConverterFunction converter = [mapType,keyMetaType,valueMetaType](const void *src, void *target) -> bool {
            if(src){
                if(JniEnvironment env{500}){
                    const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(src);
                    jobject jobj;
                    if(javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, jobj = javaObject->object(env))){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jobj)){
                            if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                switch(mapType){
                                case AssociativeContainerType::QMap:
                                    if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(_containerAccess)){
                                        if(keyMetaType==containerAccess->keyMetaType() && valueMetaType==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case AssociativeContainerType::QHash:
                                    if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(_containerAccess)){
                                        if(keyMetaType==containerAccess->keyMetaType() && valueMetaType==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case AssociativeContainerType::QMultiMap:
                                    if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(_containerAccess)){
                                        if(keyMetaType==containerAccess->keyMetaType() && valueMetaType==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case AssociativeContainerType::QMultiHash:
                                    if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(_containerAccess)){
                                        if(keyMetaType==containerAccess->keyMetaType() && valueMetaType==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                default: break;
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

void registerContainerConverter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType){
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

Ref::Ref(const QtPrivate::QMetaTypeInterface *_iface) : iface(_iface), ptr(operator new(iface->size)) {
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

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
