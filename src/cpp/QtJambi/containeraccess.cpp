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
#include "qtjambiapi.h"
#include "java_p.h"
#include "containerapi.h"
#include "qtjambi_cast.h"
#include "jobjectwrapper.h"
#include "typemanager_p.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, gContainerAccessLoader, ("io.qt.qtjambi.ContainerAccess", QLatin1String("/containeraccess"), Qt::CaseInsensitive))
Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

typedef QMap<hash_type, ContainerAccessAPI::SequentialContainerAccessFactory> SequentialContainerAccessFactoryHash;
typedef QMap<hash_type, ContainerAccessAPI::AssociativeContainerAccessFactory> AssociativeContainerAccessFactoryHash;
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
    QRecursiveMutexLocker lock(gMutex());
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
    QRecursiveMutexLocker lock(gMutex());
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
    gSequentialContainerAccessFactoryHash->insert(qHash(containerType, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                              , isStatic
#endif
                                    ), factory);
}

void ContainerAccessAPI::registerAccessFactory(AssociativeContainerType containerType, size_t align1, size_t size1, size_t align2, size_t size2, AssociativeContainerAccessFactory factory){
    gAssociativeContainerAccessFactoryHash->insert(qHash(containerType, align1, size1, align2, size2), factory);
}

QPair<void*,AbstractContainerAccess*> ContainerAPI::fromNativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        QtJambiLink *lnk = reinterpret_cast<QtJambiLink *>(nativeId);
        return {lnk->pointer(), lnk->containerAccess()};
    }else{
        return {nullptr,nullptr};
    }
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

template<typename AccessType>
class SequentialOwnedAccess : public ContainerAccessWrapper<AccessType>::type{
    typedef typename ContainerAccessWrapper<AccessType>::type SuperType;
    OwnerFunctional m_elementOwnerFunction;
public:
    ~SequentialOwnedAccess() override = default;
    SequentialOwnedAccess(AccessType* containerAccess, OwnerFunctional&& elementOwnerFunction)
        : SuperType(containerAccess), m_elementOwnerFunction(std::move(elementOwnerFunction)) {
        Q_ASSERT(m_elementOwnerFunction);
    }
    SequentialOwnedAccess(AccessType* containerAccess, const OwnerFunctional& elementOwnerFunction)
        : SuperType(containerAccess), m_elementOwnerFunction(elementOwnerFunction) {
        Q_ASSERT(m_elementOwnerFunction);
    }

    SequentialOwnedAccess<AccessType>* clone() override{
        return new SequentialOwnedAccess<AccessType>(SuperType::clone(), m_elementOwnerFunction);
    }

    const QObject* getOwner(const void* container) override{
        struct Data{
            const OwnerFunctional& elementOwnerFunction;
            const QObject* result;
        };
        Data data{m_elementOwnerFunction,nullptr};

        this->analyzeElements(container, [](const void* element, void* d) -> bool{
            Data* data = reinterpret_cast<Data*>(d);
            data->result = data->elementOwnerFunction(element);
            return !data->result;
        }, &data);
        return data.result;
    }
};

template<typename AccessType>
class AssociativeOwnedAccess : public ContainerAccessWrapper<AccessType>::type{
    typedef typename ContainerAccessWrapper<AccessType>::type SuperType;
    OwnerFunctional m_keyOwnerFunction;
    OwnerFunctional m_valueOwnerFunction;
public:
    ~AssociativeOwnedAccess() override = default;
    AssociativeOwnedAccess(AccessType* containerAccess, OwnerFunctional&& keyOwnerFunction, OwnerFunctional&& valueOwnerFunction)
        : SuperType(containerAccess), m_keyOwnerFunction(std::move(keyOwnerFunction)), m_valueOwnerFunction(std::move(valueOwnerFunction)) {
        Q_ASSERT(m_keyOwnerFunction || m_valueOwnerFunction);
    }
    AssociativeOwnedAccess(AccessType* containerAccess, const OwnerFunctional& keyOwnerFunction, const OwnerFunctional& valueOwnerFunction)
        : SuperType(containerAccess), m_keyOwnerFunction(keyOwnerFunction), m_valueOwnerFunction(valueOwnerFunction) {
        Q_ASSERT(m_keyOwnerFunction || m_valueOwnerFunction);
    }

    AssociativeOwnedAccess<AccessType>* clone() override{
        return new AssociativeOwnedAccess<AccessType>(SuperType::clone(), m_keyOwnerFunction, m_valueOwnerFunction);
    }

    const QObject* getOwner(const void* container) override{
        struct Data{
            const OwnerFunctional& keyOwnerFunction;
            const OwnerFunctional& valueOwnerFunction;
            const QObject* result;
        };
        Data data{m_keyOwnerFunction,m_valueOwnerFunction,nullptr};

        this->analyzeEntries(container, [](const void* key, const void* value, void* d) -> bool{
            Data* data = reinterpret_cast<Data*>(d);
            if(data->keyOwnerFunction)
                data->result = data->keyOwnerFunction(key);
            if(!data->result && data->valueOwnerFunction)
                data->result = data->valueOwnerFunction(value);
            return !data->result;
        }, &data);
        return data.result;
    }
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

void WrapperListAccess::dispose()  { delete this; }

void WrapperListAccess::analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) {
    m_containerAccess->analyzeElements(container, analyzer, data);
}

void WrapperListAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

size_t WrapperListAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

void* WrapperListAccess::constructContainer(void* container, const void* copyOf) {
    return m_containerAccess->constructContainer(container, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperListAccess::constructContainer(void* container, void* moved) {
    return m_containerAccess->constructContainer(container, moved);
}
#endif

bool WrapperListAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperListAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

bool WrapperListAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperListAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

void WrapperListAccess::appendList(JNIEnv * env, void* container, jobject list) {
    m_containerAccess->appendList(env, container, list);
}

jobject WrapperListAccess::at(JNIEnv * env, const void* container, jint index) {
    return m_containerAccess->at(env, container, index);
}

jobject WrapperListAccess::value(JNIEnv * env, const void* container, jint index) {
    return m_containerAccess->value(env, container, index);
}

jobject WrapperListAccess::value(JNIEnv * env, const void* container, jint index, jobject defaultValue) {
    return m_containerAccess->value(env, container, index, defaultValue);
}

void WrapperListAccess::swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) {
    m_containerAccess->swapItemsAt(env, container, index1, index2);
}

jboolean WrapperListAccess::startsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->startsWith(env, container, value);
}

jint WrapperListAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

void WrapperListAccess::reserve(JNIEnv * env, void* container, jint size) {
    return m_containerAccess->reserve(env, container, size);
}

void WrapperListAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    m_containerAccess->replace(env, container, index, value);
}

void WrapperListAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
    m_containerAccess->remove(env, container, index, n);
}

jint WrapperListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    return m_containerAccess->removeAll(env, container, value);
}

jboolean WrapperListAccess::equal(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->equal(env, container, other);
}

void WrapperListAccess::move(JNIEnv * env, void* container, jint index1, jint index2) {
    m_containerAccess->move(env, container, index1, index2);
}

jobject WrapperListAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    return m_containerAccess->mid(env, container, index1, index2);
}

jint WrapperListAccess::lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) {
    return m_containerAccess->lastIndexOf(env, container, value, index);
}

void WrapperListAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    m_containerAccess->insert(env, container, index, n, value);
}

jint WrapperListAccess::indexOf(JNIEnv * env, const void* container, jobject value, jint index) {
    return m_containerAccess->indexOf(env, container, value, index);
}

jboolean WrapperListAccess::endsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->endsWith(env, container, value);
}

jobject WrapperListAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->begin(env, ownerId, container);
}

jobject WrapperListAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperListAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->end(env, ownerId, container);
}

jobject WrapperListAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

jint WrapperListAccess::count(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->count(env, container, value);
}

jboolean WrapperListAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

void WrapperListAccess::clear(JNIEnv * env, void* container) {
    m_containerAccess->clear(env, container);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
jint WrapperListAccess::capacity(JNIEnv * env, const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperListAccess::fill(JNIEnv * env, void* container, jobject value, jint size) {
    m_containerAccess->fill(env, container, value, size);
}

void WrapperListAccess::resize(JNIEnv * env, void* container, jint newSize) {
    return m_containerAccess->resize(env, container, newSize);
}

void WrapperListAccess::squeeze(JNIEnv * env, void* container) {
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

void WrapperVectorAccess::dispose()  { delete this; }

void WrapperVectorAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

size_t WrapperVectorAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

void* WrapperVectorAccess::constructContainer(void* container, const void* copyOf) {
    return m_containerAccess->constructContainer(container, copyOf);
}

bool WrapperVectorAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperVectorAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

bool WrapperVectorAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperVectorAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

void WrapperVectorAccess::appendVector(JNIEnv * env, void* container, jobject list) {
    m_containerAccess->appendVector(env, container, list);
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

void WrapperVectorAccess::swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) {
    m_containerAccess->swapItemsAt(env, container, index1, index2);
}

jboolean WrapperVectorAccess::startsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->startsWith(env, container, value);
}

jint WrapperVectorAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

void WrapperVectorAccess::reserve(JNIEnv * env, void* container, jint size) {
    return m_containerAccess->reserve(env, container, size);
}

void WrapperVectorAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    m_containerAccess->replace(env, container, index, value);
}

jint WrapperVectorAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    return m_containerAccess->removeAll(env, container, value);
}

jboolean WrapperVectorAccess::equal(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->equal(env, container, other);
}

void WrapperVectorAccess::move(JNIEnv * env, void* container, jint index1, jint index2) {
    m_containerAccess->move(env, container, index1, index2);
}

jobject WrapperVectorAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
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

jobject WrapperVectorAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->begin(env, ownerId, container);
}

jobject WrapperVectorAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperVectorAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->end(env, ownerId, container);
}

jobject WrapperVectorAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

jint WrapperVectorAccess::count(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->count(env, container, value);
}

jboolean WrapperVectorAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

void WrapperVectorAccess::clear(JNIEnv * env, void* container) {
    m_containerAccess->clear(env, container);
}

jint WrapperVectorAccess::capacity(JNIEnv * env, const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperVectorAccess::fill(JNIEnv * env, void* container, jobject value, jint size) {
    m_containerAccess->fill(env, container, value, size);
}

void WrapperVectorAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
    m_containerAccess->remove(env, container, index, n);
}

void WrapperVectorAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    m_containerAccess->insert(env, container, index, n, value);
}

void WrapperVectorAccess::resize(JNIEnv * env, void* container, jint newSize) {
    return m_containerAccess->resize(env, container, newSize);
}

void WrapperVectorAccess::squeeze(JNIEnv * env, void* container) {
    return m_containerAccess->squeeze(env, container);
}

void WrapperVectorAccess::analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) {
    m_containerAccess->analyzeElements(container, analyzer, data);
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

void WrapperLinkedListAccess::dispose() { delete this; }

void WrapperLinkedListAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

size_t WrapperLinkedListAccess::sizeOf() {
    return m_containerAccess->sizeOf();
}

void* WrapperLinkedListAccess::constructContainer(void* container, const void* copyOf) {
    return m_containerAccess->constructContainer(container, copyOf);
}

bool WrapperLinkedListAccess::destructContainer(void* container) {
    return m_containerAccess->destructContainer(container);
}

int WrapperLinkedListAccess::registerContainer(const QByteArray& containerTypeName) {
    return m_containerAccess->registerContainer(containerTypeName);
}

bool WrapperLinkedListAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperLinkedListAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

void WrapperLinkedListAccess::append(JNIEnv * env, void* container, jobject value) {
    m_containerAccess->append(env, container, value);
}

jobject WrapperLinkedListAccess::first(JNIEnv * env, const void* container) {
    return m_containerAccess->first(env, container);
}

jobject WrapperLinkedListAccess::last(JNIEnv * env, const void* container) {
    return m_containerAccess->last(env, container);
}

jobject WrapperLinkedListAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->begin(env, ownerId, container);
}

jobject WrapperLinkedListAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperLinkedListAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->end(env, ownerId, container);
}

jobject WrapperLinkedListAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

void WrapperLinkedListAccess::clear(JNIEnv * env, void* container) {
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

void WrapperLinkedListAccess::prepend(JNIEnv * env, void* container, jobject value) {
    return m_containerAccess->prepend(env, container, value);
}

void WrapperLinkedListAccess::removeFirst(JNIEnv * env, void* container) {
    m_containerAccess->removeFirst(env, container);
}

jint WrapperLinkedListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    return m_containerAccess->removeAll(env, container, value);
}

void WrapperLinkedListAccess::removeLast(JNIEnv * env, void* container) {
    m_containerAccess->removeLast(env, container);
}

jboolean WrapperLinkedListAccess::removeOne(JNIEnv * env, void* container, jobject value) {
    return m_containerAccess->removeOne(env, container, value);
}

jint WrapperLinkedListAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

jboolean WrapperLinkedListAccess::startsWith(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->startsWith(env, container, value);
}

jobject WrapperLinkedListAccess::takeFirst(JNIEnv * env, void* container) {
    return m_containerAccess->takeFirst(env, container);
}

jobject WrapperLinkedListAccess::takeLast(JNIEnv * env, void* container) {
    return m_containerAccess->takeLast(env, container);
}

void WrapperLinkedListAccess::analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) {
    m_containerAccess->analyzeElements(container, analyzer, data);
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

void WrapperSetAccess::dispose() { delete this; }

void* WrapperSetAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperSetAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
}
#endif

void WrapperSetAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
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

bool WrapperSetAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperSetAccess::elementMetaType() {
    return m_containerAccess->elementMetaType();
}

jint WrapperSetAccess::capacity(JNIEnv * env, const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperSetAccess::clear(JNIEnv * env, void* container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperSetAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jobject WrapperSetAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperSetAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

void WrapperSetAccess::insert(JNIEnv * env, void* container, jobject value) {
    m_containerAccess->insert(env, container, value);
}

void WrapperSetAccess::intersect(JNIEnv * env, void* container, jobject other) {
    m_containerAccess->intersect(env, container, other);
}

jboolean WrapperSetAccess::intersects(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->intersects(env, container, other);
}

jboolean WrapperSetAccess::equal(JNIEnv * env, const void* container, jobject other) {
    return m_containerAccess->equal(env, container, other);
}

jboolean WrapperSetAccess::remove(JNIEnv * env, void* container, jobject value) {
    return m_containerAccess->remove(env, container, value);
}

void WrapperSetAccess::reserve(JNIEnv * env, void* container, jint newSize) {
    m_containerAccess->reserve(env, container, newSize);
}

jint WrapperSetAccess::size(JNIEnv * env, const void* container) {
    return m_containerAccess->size(env, container);
}

void WrapperSetAccess::subtract(JNIEnv * env, void* container, jobject other) {
    m_containerAccess->subtract(env, container, other);
}

void WrapperSetAccess::unite(JNIEnv * env, void* container, jobject other) {
    m_containerAccess->unite(env, container, other);
}

jobject WrapperSetAccess::values(JNIEnv * env, const void* container) {
    return m_containerAccess->values(env, container);
}

void WrapperSetAccess::analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) {
    m_containerAccess->analyzeElements(container, analyzer, data);
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

void WrapperMapAccess::dispose()  { delete this; }

void WrapperMapAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperMapAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperMapAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
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

bool WrapperMapAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperMapAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperMapAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

jobject WrapperMapAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->begin(env, ownerId, container);
}

jobject WrapperMapAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->end(env, ownerId, container);
}

jobject WrapperMapAccess::find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key)  {
     return m_containerAccess->find(env, ownerId, container, key);
}

jobject WrapperMapAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperMapAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

jobject WrapperMapAccess::constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constFind(env, ownerId, container, key);
}

void WrapperMapAccess::clear(JNIEnv * env, void* container) {
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

void WrapperMapAccess::insert(JNIEnv * env, void* container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperMapAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
     return m_containerAccess->key(env, container, value, defaultKey);
}

jobject WrapperMapAccess::keys(JNIEnv * env, const void* container)  {
     return m_containerAccess->keys(env, container);
}

jobject WrapperMapAccess::keys(JNIEnv * env, const void* container, jobject value)  {
     return m_containerAccess->keys(env, container, value);
}

jobject WrapperMapAccess::last(JNIEnv * env, const void* container)  {
     return m_containerAccess->last(env, container);
}

jobject WrapperMapAccess::lastKey(JNIEnv * env, const void* container)  {
     return m_containerAccess->lastKey(env, container);
}

jobject WrapperMapAccess::constLowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constLowerBound(env, ownerId, container, key);
}

jboolean WrapperMapAccess::equal(JNIEnv * env, const void* container, jobject other)  {
     return m_containerAccess->equal(env, container, other);
}

jint WrapperMapAccess::remove(JNIEnv * env, void* container,jobject key)  {
    return m_containerAccess->remove(env, container, key);
}

jint WrapperMapAccess::size(JNIEnv * env, const void* container)  {
     return m_containerAccess->size(env, container);
}

jobject WrapperMapAccess::take(JNIEnv *env, void* container,jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperMapAccess::constUpperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constUpperBound(env, ownerId, container, key);
}

jobject WrapperMapAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
     return m_containerAccess->value(env, container, key, defaultValue);
}

jobject WrapperMapAccess::values(JNIEnv * env, const void* container)  {
     return m_containerAccess->values(env, container);
}

bool WrapperMapAccess::keyLessThan(JNIEnv *env, jobject key1, jobject key2)  {
    return m_containerAccess->keyLessThan(env, key1, key2);
}

void WrapperMapAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data)  {
    m_containerAccess->analyzeEntries(container, analyzer, data);
}

WrapperMultiMapAccess::WrapperMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : AbstractMultiMapAccess(), m_containerAccess(containerAccess) {}

WrapperMultiMapAccess::~WrapperMultiMapAccess() {
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

AbstractMultiMapAccess* WrapperMultiMapAccess::clone() {
    return m_containerAccess->clone();
}

void WrapperMultiMapAccess::dispose()  { delete this; }

void WrapperMultiMapAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperMultiMapAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperMultiMapAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
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

bool WrapperMultiMapAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperMultiMapAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperMultiMapAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

void WrapperMultiMapAccess::clear(JNIEnv * env, void* container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperMultiMapAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperMultiMapAccess::count(JNIEnv * env,const void* container, jobject key) {
    return m_containerAccess->count(env, container, key);
}

jobject WrapperMultiMapAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->begin(env, ownerId, container);
}

jobject WrapperMultiMapAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->end(env, ownerId, container);
}

jobject WrapperMultiMapAccess::find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key)  {
     return m_containerAccess->find(env, ownerId, container, key);
}

jobject WrapperMultiMapAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperMultiMapAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

jobject WrapperMultiMapAccess::constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constFind(env, ownerId, container, key);
}

jobject WrapperMultiMapAccess::first(JNIEnv * env, const void* container)  {
     return m_containerAccess->first(env, container);
}

jobject WrapperMultiMapAccess::firstKey(JNIEnv * env, const void* container)  {
     return m_containerAccess->firstKey(env, container);
}

void WrapperMultiMapAccess::insert(JNIEnv *env, void* container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperMultiMapAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
     return m_containerAccess->key(env, container, value, defaultKey);
}

jobject WrapperMultiMapAccess::keys(JNIEnv * env, const void* container)  {
     return m_containerAccess->keys(env, container);
}

jobject WrapperMultiMapAccess::keys(JNIEnv * env, const void* container, jobject value)  {
     return m_containerAccess->keys(env, container, value);
}

jobject WrapperMultiMapAccess::last(JNIEnv * env, const void* container)  {
     return m_containerAccess->last(env, container);
}

jobject WrapperMultiMapAccess::lastKey(JNIEnv * env, const void* container)  {
     return m_containerAccess->lastKey(env, container);
}

jobject WrapperMultiMapAccess::constLowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constLowerBound(env, ownerId, container, key);
}

jboolean WrapperMultiMapAccess::equal(JNIEnv * env, const void* container, jobject other)  {
     return m_containerAccess->equal(env, container, other);
}

jint WrapperMultiMapAccess::remove(JNIEnv * env, void* container, jobject key, jobject value)  {
    return m_containerAccess->remove(env, container, key, value);
}

jint WrapperMultiMapAccess::size(JNIEnv * env, const void* container)  {
     return m_containerAccess->size(env, container);
}

jobject WrapperMultiMapAccess::take(JNIEnv *env, void* container,jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperMultiMapAccess::constUpperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constUpperBound(env, ownerId, container, key);
}

jobject WrapperMultiMapAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
     return m_containerAccess->value(env, container, key, defaultValue);
}

jobject WrapperMultiMapAccess::values(JNIEnv * env, const void* container)  {
     return m_containerAccess->values(env, container);
}

bool WrapperMultiMapAccess::keyLessThan(JNIEnv *env, jobject key1, jobject key2)  {
    return m_containerAccess->keyLessThan(env, key1, key2);
}

jobject WrapperMultiMapAccess::uniqueKeys(JNIEnv * env, const void* container)  {
    return m_containerAccess->uniqueKeys(env, container);
}

void WrapperMultiMapAccess::unite(JNIEnv *env, void* container, jobject other)  {
     m_containerAccess->unite(env, container, other);
}

jobject WrapperMultiMapAccess::values(JNIEnv * env, const void* container, jobject key)  {
    return m_containerAccess->values(env, container, key);
}

jboolean WrapperMultiMapAccess::contains(JNIEnv * env, const void* container, jobject key, jobject value) {
    return m_containerAccess->contains(env, container, key, value);
}

jint WrapperMultiMapAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)  {
     return m_containerAccess->count(env, container, key, value);
}

jobject WrapperMultiMapAccess::find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value)  {
    return m_containerAccess->find(env, ownerId, container, key, value);
}

jobject WrapperMultiMapAccess::constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value)  {
    return m_containerAccess->constFind(env, ownerId, container, key, value);
}

jint WrapperMultiMapAccess::remove(JNIEnv * env, void* container,jobject key)  {
     return m_containerAccess->remove(env, container, key);
}

void WrapperMultiMapAccess::replace(JNIEnv * env, void* container,jobject key, jobject value)  {
    m_containerAccess->replace(env, container, key, value);
}

void WrapperMultiMapAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data)  {
    m_containerAccess->analyzeEntries(container, analyzer, data);
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

void WrapperHashAccess::dispose()  { delete this; }

void WrapperHashAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperHashAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperHashAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
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

bool WrapperHashAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperHashAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperHashAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

jint WrapperHashAccess::capacity(JNIEnv * env,const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperHashAccess::clear(JNIEnv * env, void* container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperHashAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperHashAccess::count(JNIEnv * env, const void* container, jobject key)  {
    return m_containerAccess->count(env, container, key);
}

jobject WrapperHashAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->begin(env, ownerId, container);
}

jobject WrapperHashAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->end(env, ownerId, container);
}

jobject WrapperHashAccess::find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key)  {
     return m_containerAccess->find(env, ownerId, container, key);
}

jobject WrapperHashAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperHashAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

jobject WrapperHashAccess::constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constFind(env, ownerId, container, key);
}

void WrapperHashAccess::insert(JNIEnv *env, void* container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperHashAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
    return m_containerAccess->key(env, container, value, defaultKey);
}

jobject WrapperHashAccess::keys(JNIEnv * env, const void* container)  {
    return m_containerAccess->keys(env, container);
}

jobject WrapperHashAccess::keys(JNIEnv * env, const void* container, jobject value)  {
    return m_containerAccess->keys(env, container, value);
}

jboolean WrapperHashAccess::equal(JNIEnv * env, const void* container, jobject other)  {
    return m_containerAccess->equal(env, container, other);
}

jint WrapperHashAccess::remove(JNIEnv * env, void* container,jobject key)  {
    return m_containerAccess->remove(env, container, key);
}

void WrapperHashAccess::reserve(JNIEnv * env,void* container, jint newSize)  {
    m_containerAccess->reserve(env, container, newSize);
}

jint WrapperHashAccess::size(JNIEnv * env, const void* container)  {
    return m_containerAccess->size(env, container);
}

jobject WrapperHashAccess::take(JNIEnv *env, void* container, jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperHashAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
    return m_containerAccess->value(env, container, key, defaultValue);
}

jobject WrapperHashAccess::values(JNIEnv * env, const void* container)  {
    return m_containerAccess->values(env, container);
}

void WrapperHashAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data)  {
    m_containerAccess->analyzeEntries(container, analyzer, data);
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

void WrapperMultiHashAccess::dispose()  { delete this; }

void WrapperMultiHashAccess::assign(void* container, const void* other) {
    m_containerAccess->assign(container, other);
}

void* WrapperMultiHashAccess::constructContainer(void* placement, const void* copyOf) {
    return m_containerAccess->constructContainer(placement, copyOf);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* WrapperMultiHashAccess::constructContainer(void* placement, void* move) {
    return m_containerAccess->constructContainer(placement, move);
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

bool WrapperMultiHashAccess::isConstant() {
    return m_containerAccess->isConstant();
}

const QMetaType& WrapperMultiHashAccess::keyMetaType() {
    return m_containerAccess->keyMetaType();
}

const QMetaType& WrapperMultiHashAccess::valueMetaType() {
    return m_containerAccess->valueMetaType();
}

jint WrapperMultiHashAccess::capacity(JNIEnv * env,const void* container) {
    return m_containerAccess->capacity(env, container);
}

void WrapperMultiHashAccess::clear(JNIEnv * env, void* container) {
    m_containerAccess->clear(env, container);
}

jboolean WrapperMultiHashAccess::contains(JNIEnv * env, const void* container, jobject value) {
    return m_containerAccess->contains(env, container, value);
}

jint WrapperMultiHashAccess::count(JNIEnv * env, const void* container, jobject key)  {
    return m_containerAccess->count(env, container, key);
}

jobject WrapperMultiHashAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->begin(env, ownerId, container);
}

jobject WrapperMultiHashAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container) {
    return m_containerAccess->end(env, ownerId, container);
}

jobject WrapperMultiHashAccess::find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key)  {
     return m_containerAccess->find(env, ownerId, container, key);
}

jobject WrapperMultiHashAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constBegin(env, ownerId, container);
}

jobject WrapperMultiHashAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) {
    return m_containerAccess->constEnd(env, ownerId, container);
}

jobject WrapperMultiHashAccess::constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)  {
     return m_containerAccess->constFind(env, ownerId, container, key);
}

void WrapperMultiHashAccess::insert(JNIEnv *env, void* container,jobject key,jobject value)  {
    m_containerAccess->insert(env, container, key, value);
}

jobject WrapperMultiHashAccess::key(JNIEnv * env, const void* container, jobject value, jobject defaultKey)  {
    return m_containerAccess->key(env, container, value, defaultKey);
}

jobject WrapperMultiHashAccess::keys(JNIEnv * env, const void* container)  {
    return m_containerAccess->keys(env, container);
}

jobject WrapperMultiHashAccess::keys(JNIEnv * env, const void* container, jobject value)  {
    return m_containerAccess->keys(env, container, value);
}

jboolean WrapperMultiHashAccess::equal(JNIEnv * env, const void* container, jobject other)  {
    return m_containerAccess->equal(env, container, other);
}

jint WrapperMultiHashAccess::remove(JNIEnv * env, void* container,jobject key)  {
    return m_containerAccess->remove(env, container, key);
}

void WrapperMultiHashAccess::reserve(JNIEnv * env,void* container, jint newSize)  {
    m_containerAccess->reserve(env, container, newSize);
}

jint WrapperMultiHashAccess::size(JNIEnv * env, const void* container)  {
    return m_containerAccess->size(env, container);
}

jobject WrapperMultiHashAccess::take(JNIEnv *env, void* container, jobject key)  {
    return m_containerAccess->take(env, container, key);
}

jobject WrapperMultiHashAccess::value(JNIEnv * env, const void* container, jobject key, jobject defaultValue)  {
    return m_containerAccess->value(env, container, key, defaultValue);
}

jobject WrapperMultiHashAccess::values(JNIEnv * env, const void* container)  {
    return m_containerAccess->values(env, container);
}

jobject WrapperMultiHashAccess::values(JNIEnv * env, const void* container, jobject key)  {
    return m_containerAccess->values(env, container, key);
}

jboolean WrapperMultiHashAccess::contains(JNIEnv * env, const void* container, jobject key, jobject value) {
    return m_containerAccess->contains(env, container, key, value);
}

jint WrapperMultiHashAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)  {
    return m_containerAccess->count(env, container, key, value);
}

jobject WrapperMultiHashAccess::find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value)  {
    return m_containerAccess->find(env, ownerId, container, key, value);
}

jobject WrapperMultiHashAccess::constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value)  {
    return m_containerAccess->constFind(env, ownerId, container, key, value);
}

jint WrapperMultiHashAccess::remove(JNIEnv * env, void* container, jobject key, jobject value)  {
     return m_containerAccess->remove(env, container, key, value);
}

void WrapperMultiHashAccess::replace(JNIEnv * env, void* container, jobject key, jobject value)  {
    m_containerAccess->replace(env, container, key, value);
}

jobject WrapperMultiHashAccess::uniqueKeys(JNIEnv * env,const void* container)  {
    return m_containerAccess->uniqueKeys(env, container);
}

void WrapperMultiHashAccess::unite(JNIEnv * env, void* container, jobject other)  {
    m_containerAccess->unite(env, container, other);
}

void WrapperMultiHashAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data)  {
    m_containerAccess->analyzeEntries(container, analyzer, data);
}

template<typename AccessType>
AccessType* createOwnedAccess(AccessType* containerAccess, OwnerFunctional&& ownerFunction)
{
    if(ownerFunction)
        return new SequentialOwnedAccess<AccessType>(containerAccess, std::move(ownerFunction));
    else
        return containerAccess;
}

template<typename AccessType>
AccessType* createOwnedAccess(AccessType* containerAccess, OwnerFunctional&& keyOwnerFunction, OwnerFunctional&& valueOwnerFunction)
{
    if(keyOwnerFunction || valueOwnerFunction)
        return new AssociativeOwnedAccess<AccessType>(containerAccess, std::move(keyOwnerFunction), std::move(valueOwnerFunction));
    else
        return containerAccess;
}


bool AbstractContainerAccess::isPointerType(const QMetaType& metaType){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    if(metaType.sizeOf()==sizeof(void*)){
        const std::type_info* typeId = getTypeByMetaType(metaType.id());
        QByteArray name = metaType.name();
        if(!typeId)
            typeId = getTypeByQtName(name);
        if(typeId){
            if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*typeId)){
                if(typeInfo->isPointer){
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
            if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*typeId))
                return typeInfo->isStatic;
        }
    }
    return QtJambiTypeManager::isStaticType(QLatin1String(metaType.name()));
}
#endif

std::function<AbstractContainerAccess*()> getSequentialContainerAccessFactory(SequentialContainerType containerType, const QMetaType& type);
std::function<AbstractContainerAccess*()> getSequentialContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType);
void registerContainerAccessFactory(SequentialContainerType containerType, const QMetaType& elementType, std::function<AbstractContainerAccess*()>&& factory);
void registerContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType, std::function<AbstractContainerAccess*()>&& factory);

AbstractContainerAccess* AbstractContainerAccess::create(JNIEnv* env, SequentialContainerType containerType, const QMetaType& memberMetaType){
    Q_UNUSED(env)
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
        }
        break;
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

AbstractContainerAccess* AbstractContainerAccess::create(JNIEnv* env, AssociativeContainerType mapType, const QMetaType& memberMetaType1, const QMetaType& memberMetaType2){
    Q_UNUSED(env)
    AbstractContainerAccess* containerAccess = nullptr;
    if(std::function<AbstractContainerAccess*()> containerAccessFactory = getSequentialContainerAccessFactory(mapType, memberMetaType1, memberMetaType2))
        containerAccess = containerAccessFactory();
    return containerAccess;
}

AbstractContainerAccess* AbstractContainerAccess::create(JNIEnv* env, SequentialContainerType containerType,
                                                         const QMetaType& metaType,
                                                         size_t align, size_t size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                         bool isStaticType,
#endif
                                                         bool isPointer,
                                                         const QHashFunction& hashFunction,
                                                         const InternalToExternalConverter& memberConverter,
                                                         const ExternalToInternalConverter& memberReConverter){
    if(isPointer){
        size = 0;
        align = 0;
    }
    AbstractContainerAccess* containerAccess = nullptr;
    if(!isPointer){
        switch(containerType){
        case SequentialContainerType::QSet:
            containerAccess = new AutoSetAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         metaType.id(),
                                         align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
#else
                                         metaType,
#endif
                                         hashFunction,
                                         memberConverter,
                                         memberReConverter);
            break;
        case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        case SequentialContainerType::QVector:
            containerAccess = new AutoVectorAccess(
                                         metaType.id(),
                                         align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                         hashFunction,
                                         memberConverter,
                                         memberReConverter);
            break;
        case SequentialContainerType::QLinkedList:
            containerAccess = new AutoLinkedListAccess(
                                         metaType.id(),
                                         align ? align : qMin<size_t>(size, alignof(std::max_align_t)),
                                         hashFunction,
                                         memberConverter,
                                         memberReConverter);
            break;
#endif
        case SequentialContainerType::QQueue:
        case SequentialContainerType::QList:
            containerAccess = new AutoListAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         metaType.id(),
                                         align ? align : qMin<size_t>(size, alignof(std::max_align_t)), isStaticType,
#else
                                         metaType,
#endif
                                         hashFunction,
                                         memberConverter,
                                         memberReConverter);
            break;
        default: break;
        }
    }

    if(!containerAccess){
        JavaException occurredException;
        if(ContainerAccessAPI::SequentialContainerAccessFactory accessFactory = findContainerAccessFactory(env, containerType, align, size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                             isStaticType,
#endif
                                                                                             occurredException)){
            QMetaType voidPtr(QMetaType::VoidStar);
            containerAccess = accessFactory(isPointer && !metaType.isValid() ? voidPtr : metaType,
                                            hashFunction,
                                            memberConverter,
                                            memberReConverter);
        }else{
            occurredException.raise();
        }
    }
    if(containerAccess && metaType.isValid()){
        QSharedPointer<AbstractContainerAccess> access(containerAccess->clone(), &containerDisposer);
        registerContainerAccessFactory(containerType, metaType, [access]() -> AbstractContainerAccess* {
                                           return access->clone();
                                       });
    }
    return containerAccess;
}

AbstractContainerAccess* AbstractContainerAccess::create(JNIEnv* env, AssociativeContainerType mapType,
                                                         const QMetaType& memberMetaType1,
                                                         size_t align1, size_t size1,
                                                         bool isPointer1,
                                                         const QHashFunction& hashFunction1,
                                                         const InternalToExternalConverter& memberConverter1,
                                                         const ExternalToInternalConverter& memberReConverter1,
                                                         const QMetaType& memberMetaType2,
                                                         size_t align2, size_t size2,
                                                         bool isPointer2,
                                                         const QHashFunction& hashFunction2,
                                                         const InternalToExternalConverter& memberConverter2,
                                                         const ExternalToInternalConverter& memberReConverter2){
    if(isPointer1){
        size1 = 0;
        align1 = 0;
    }
    if(isPointer2){
        size2 = 0;
        align2 = 0;
    }
    AbstractContainerAccess* containerAccess = nullptr;
    if(!isPointer1 || !isPointer2){
        switch(mapType){
        case AssociativeContainerType::QPair:
            containerAccess =new AutoPairAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                         hashFunction1,
                                         memberConverter1,
                                         memberReConverter1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                         hashFunction2,
                                         memberConverter2,
                                         memberReConverter2);
            break;
        case AssociativeContainerType::QHash:
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                         hashFunction2,
                                         memberConverter2,
                                         memberReConverter2);
            break;
        case AssociativeContainerType::QMap:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                         hashFunction2,
                                         memberConverter2,
                                         memberReConverter2);
#endif
            break;
        case AssociativeContainerType::QMultiHash:
        {
            AbstractMultiHashAccess* access = new AutoMultiHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                         hashFunction1,
                                         memberConverter1,
                                         memberReConverter1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                         hashFunction2,
                                         memberConverter2,
                                         memberReConverter2);
            containerAccess = access;
        }
            break;
        case AssociativeContainerType::QMultiMap:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            AbstractMultiMapAccess* access = new AutoMultiMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType::VoidStar : memberMetaType1.id(),
                                         isPointer1 ? alignof(void*) : (align1 ? align1 : qMin<size_t>(size1, alignof(std::max_align_t))),
#else
                                         isPointer1 && !memberMetaType1.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType1,
#endif
                                         hashFunction1,
                                         memberConverter1,
                                         memberReConverter1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType::VoidStar : memberMetaType2.id(),
                                         isPointer2 ? alignof(void*) : (align2 ? align2 : qMin<size_t>(size2, alignof(std::max_align_t))),
#else
                                         isPointer2 && !memberMetaType2.isValid() ? QMetaType(QMetaType::VoidStar) : memberMetaType2,
#endif
                                         hashFunction2,
                                         memberConverter2,
                                         memberReConverter2);
            containerAccess = access;
        }
#endif
            break;
        default: break;
        }
    }

    if(!containerAccess){
        JavaException occurredException;
        if(ContainerAccessAPI::AssociativeContainerAccessFactory accessFactory = findContainerAccessFactory(env, mapType, align1, size1, align2, size2, occurredException)){
            QMetaType voidPtr(QMetaType::VoidStar);
            containerAccess = accessFactory(
                                           isPointer1 && !memberMetaType1.isValid() ? voidPtr : memberMetaType1,
                                           hashFunction1,
                                           memberConverter1,
                                           memberReConverter1,
                                           isPointer2 && !memberMetaType2.isValid() ? voidPtr : memberMetaType2,
                                           hashFunction2,
                                           memberConverter2,
                                           memberReConverter2);
        }else{
            occurredException.raise();
        }
    }
    return containerAccess;
}

AbstractContainerAccess* createPreparedContainerAccess(JNIEnv* env, SequentialContainerType containerType, const QMetaType& memberMetaType){
    AbstractContainerAccess* containerAccess = AbstractContainerAccess::create(env, containerType, memberMetaType);
    if(!containerAccess){
        bool isPointer = AbstractContainerAccess::isPointerType(memberMetaType);
        size_t size = 0;
        size_t align = 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        bool isStaticType = false;
#endif
        if(!isPointer){
            size = size_t(memberMetaType.sizeOf());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            const std::type_info* t = getTypeByMetaType(memberMetaType.id());
            if(!t)
                t = getTypeByQtName(memberMetaType.name());
            if(t){
                align = getValueAlignment(*t);
                if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t))
                    isStaticType = typeInfo->isStatic;
            }
            if(align==0)
                align = QtJambiTypeManager::getInternalAlignment(memberMetaType.name());
#else
            align = size_t(memberMetaType.alignOf());
#endif
        }
        QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, memberMetaType.id());
        containerAccess = AbstractContainerAccess::create(env, containerType,
                                                                 memberMetaType,
                                                                 align, size,
        #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                 isStaticType,
        #endif
                                                                 isPointer,
                                                                 hashFunction,
                                                                 [](JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)->bool{
                                                                      return false;
                                                                 },
                                                                 [](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType) -> bool{
                                                                      return false;
                                                                 });
    }
    return containerAccess;
}

AbstractContainerAccess* createPreparedContainerAccess(JNIEnv* env, AssociativeContainerType mapType, const QMetaType& memberMetaType1, const QMetaType& memberMetaType2){
    AbstractContainerAccess* containerAccess = AbstractContainerAccess::create(env, mapType, memberMetaType1, memberMetaType2);
    if(containerAccess)
        return containerAccess;
    bool isPointer1 = AbstractContainerAccess::isPointerType(memberMetaType1);
    size_t size1 = 0;
    size_t align1 = 0;
    if(!isPointer1){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        const std::type_info* t = getTypeByMetaType(memberMetaType1.id());
        if(!t)
            t = getTypeByQtName(memberMetaType1.name());
        if(t){
            align1 = getValueAlignment(*t);
        }
        if(align1==0)
            align1 = QtJambiTypeManager::getInternalAlignment(memberMetaType1.name());
#else
        align1 = size_t(memberMetaType1.alignOf());
#endif
    }
    bool isPointer2 = AbstractContainerAccess::isPointerType(memberMetaType2);
    size_t size2 = 0;
    size_t align2 = 0;
    if(!isPointer2){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        const std::type_info* t = getTypeByMetaType(memberMetaType2.id());
        if(!t)
            t = getTypeByQtName(memberMetaType2.name());
        if(t){
            align2 = getValueAlignment(*t);
        }
        if(align2==0)
            align2 = QtJambiTypeManager::getInternalAlignment(memberMetaType2.name());
#else
        align2 = size_t(memberMetaType2.alignOf());
#endif
    }
    QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, memberMetaType1.id());
    QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, memberMetaType2.id());
    QMetaType voidPtr(QMetaType::VoidStar);
    containerAccess = AbstractContainerAccess::create(
                                   env, mapType,
                                   isPointer1 && !memberMetaType1.isValid() ? voidPtr : memberMetaType1,
                                   align1, size1,
                                   isPointer1,
                                   hashFunction1,
                                   [](JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)->bool{
                                        return false;
                                   },
                                   [](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType) -> bool{
                                        return false;
                                   },
                                   isPointer2 && !memberMetaType2.isValid() ? voidPtr : memberMetaType2,
                                   align2,
                                   size2,
                                   isPointer2,
                                   hashFunction2,
                                   [](JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)->bool{
                                        return false;
                                   },
                                   [](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType) -> bool{
                                        return false;
                                   });
    return containerAccess;
}

struct TypeAnalysisResult{
    OwnerFunctional ownerFunctional;
    bool hasPointers;
    TypeAnalysisResult(bool _hasPointers, OwnerFunctional&& _ownerFunctional)
        : ownerFunctional(std::move(_ownerFunctional)), hasPointers(_hasPointers) {}
    TypeAnalysisResult(bool _hasPointers)
        : ownerFunctional(), hasPointers(_hasPointers) {}
};

void containerDisposer(AbstractContainerAccess* _access){_access->dispose();}

TypeAnalysisResult analyzeType(JNIEnv* env, const QMetaType& metaType, const QByteArray& typeName){
    bool hasPointer = false;
    if((typeName.startsWith("QList<")
            || typeName.startsWith("QSet<")
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            || typeName.startsWith("QLinkedList<")
            || typeName.startsWith("QVector<")
#endif
            || typeName.startsWith("QQueue<")
            || typeName.startsWith("QStack<")) && typeName.endsWith('>')){
        auto idx = typeName.indexOf('<');
        QByteArray instantiation = typeName.mid(idx+1).chopped(1).trimmed();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QMetaType instantiationMetaType(QMetaType::type(instantiation));
#else
        QMetaType instantiationMetaType = QMetaType::fromName(instantiation);
#endif
        TypeAnalysisResult result = analyzeType(env, instantiationMetaType, instantiation);
        hasPointer = result.hasPointers;
        if(result.ownerFunctional){
            QSharedPointer<AbstractContainerAccess> sharedAccess;
            if(typeName.startsWith("QList<")
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    || typeName.startsWith("QStack<")
#endif
                    || typeName.startsWith("QQueue<")){
                if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(createPreparedContainerAccess(env, SequentialContainerType::QList, instantiationMetaType))){
                    sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result.ownerFunctional)), &containerDisposer);
                }
            }else if(typeName.startsWith("QSet<")){
                if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(createPreparedContainerAccess(env, SequentialContainerType::QSet, instantiationMetaType))){
                    sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result.ownerFunctional)), &containerDisposer);
                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            }else if(typeName.startsWith("QLinkedList<")){
                if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(createPreparedContainerAccess(env, SequentialContainerType::QLinkedList, instantiationMetaType))){
                    sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result.ownerFunctional)), &containerDisposer);
                }
            }else if(typeName.startsWith("QVector<")
                     || typeName.startsWith("QStack<")){
                if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(createPreparedContainerAccess(env, SequentialContainerType::QVector, instantiationMetaType))){
                    sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result.ownerFunctional)), &containerDisposer);
                }
#endif
            }
            if(sharedAccess){
                return TypeAnalysisResult(hasPointer, [sharedAccess](const void * container) -> const QObject* {
                    return sharedAccess->getOwner(container);
                });
            }
        }
    }else if((typeName.startsWith("QMap<")
            || typeName.startsWith("QHash<")
            || typeName.startsWith("QMultiMap<")
            || typeName.startsWith("QMultiHash<")) && typeName.endsWith('>')){
            auto idx = typeName.indexOf('<');
            QByteArrayList instantiations = typeName.mid(idx+1).chopped(1).trimmed().split(',');
            if(instantiations.size()==2){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                QMetaType instantiationMetaType1(QMetaType::type(instantiations[0]));
                QMetaType instantiationMetaType2(QMetaType::type(instantiations[1]));
#else
                QMetaType instantiationMetaType1 = QMetaType::fromName(instantiations[0]);
                QMetaType instantiationMetaType2 = QMetaType::fromName(instantiations[1]);
#endif
                TypeAnalysisResult result1 = analyzeType(env, instantiationMetaType1, instantiations[0]);
                TypeAnalysisResult result2 = analyzeType(env, instantiationMetaType2, instantiations[1]);
                hasPointer = result1.hasPointers || result2.hasPointers;
                if(result1.ownerFunctional || result2.ownerFunctional){
                    QSharedPointer<AbstractContainerAccess> sharedAccess;
                    if(typeName.startsWith("QMap<")){
                        if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(createPreparedContainerAccess(env, AssociativeContainerType::QMap, instantiationMetaType1, instantiationMetaType2))){
                            sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional)), &containerDisposer);
                        }
                    }else if(typeName.startsWith("QHash<")){
                        if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(createPreparedContainerAccess(env, AssociativeContainerType::QHash, instantiationMetaType1, instantiationMetaType2))){
                            sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional)), &containerDisposer);
                        }
                    }else if(typeName.startsWith("QMultiMap<")){
                        if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(createPreparedContainerAccess(env, AssociativeContainerType::QMultiMap, instantiationMetaType1, instantiationMetaType2))){
                            sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional)), &containerDisposer);
                        }
                    }else if(typeName.startsWith("QMultiHash<")){
                        if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(createPreparedContainerAccess(env, AssociativeContainerType::QMultiHash, instantiationMetaType1, instantiationMetaType2))){
                            sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional)), &containerDisposer);
                        }
                    }
                    if(sharedAccess){
                        return TypeAnalysisResult(hasPointer, [sharedAccess](const void * container) -> const QObject* {
                            return sharedAccess->getOwner(container);
                        });
                    }
                }
            }
    }else{
        const std::type_info* typeId = metaType.isValid() ? getTypeByMetaType(metaType.id()) : nullptr;
        if(!typeId)
            typeId = getTypeByQtName(typeName);
        if(typeId){
            if(isFunctional(*typeId)){
                QString typeName = QtJambiAPI::typeName(*typeId);
                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                    hasPointer = true;
                }
            }else{
                if(metaType.isValid()){
                    hasPointer = AbstractContainerAccess::isPointerType(metaType);
                }else{
                    hasPointer = typeName.endsWith("*");
                }
                if(PtrOwnerFunction elementOwnerFunction = registeredOwnerFunction(*typeId)){
                    return TypeAnalysisResult(hasPointer, elementOwnerFunction);
                }
            }
        }else{
            if(metaType.isValid()){
                hasPointer = AbstractContainerAccess::isPointerType(metaType);
            }else{
                hasPointer = typeName.endsWith("*");
            }
        }
    }
    return TypeAnalysisResult(hasPointer);
}

AbstractListAccess* checkContainerAccess(JNIEnv * env, AbstractListAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperListAccess*>(containerAccess)){
        if(containerAccess->elementMetaType().id() >= QMetaType::HighestInternalId
                || containerAccess->elementMetaType().id() == QMetaType::QObjectStar
                || containerAccess->elementMetaType().id() == QMetaType::VoidStar){
            if(AbstractContainerAccess::isPointerType(containerAccess->elementMetaType())){
                containerAccess = new PointerRCListAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(env, containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
                if(result.hasPointers)
                    containerAccess = new NestedPointersRCListAccess(containerAccess);
                containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional));
            }
        }
    }
    return containerAccess;
}

AbstractSetAccess* checkContainerAccess(JNIEnv * env, AbstractSetAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperSetAccess*>(containerAccess)){
        int id = containerAccess->elementMetaType().id();
        if(id != QMetaType::QPoint
            && id != QMetaType::QPointF
            && id != QMetaType::QString){
            if(AbstractContainerAccess::isPointerType(containerAccess->elementMetaType())){
                containerAccess = new PointerRCSetAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(env, containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
                if(result.hasPointers)
                    containerAccess = new NestedPointersRCSetAccess(containerAccess);
                containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional));
            }
        }
    }
    return containerAccess;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
AbstractLinkedListAccess* checkContainerAccess(JNIEnv * env, AbstractLinkedListAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperLinkedListAccess*>(containerAccess)){
        if(AbstractContainerAccess::isPointerType(containerAccess->elementMetaType())){
            containerAccess = new PointerRCLinkedListAccess(containerAccess);
        }else{
            TypeAnalysisResult result = analyzeType(env, containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
            if(result.hasPointers)
                containerAccess = new NestedPointersRCLinkedListAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractVectorAccess* checkContainerAccess(JNIEnv * env, AbstractVectorAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperVectorAccess*>(containerAccess)){
        if(AbstractContainerAccess::isPointerType(containerAccess->elementMetaType())){
            containerAccess = new PointerRCVectorAccess(containerAccess);
        }else{
            TypeAnalysisResult result = analyzeType(env, containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
            if(result.hasPointers)
                containerAccess = new NestedPointersRCVectorAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional));
        }
    }
    return containerAccess;
}
#endif
AbstractHashAccess* checkContainerAccess(JNIEnv * env, AbstractHashAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperHashAccess*>(containerAccess)){
        if(AbstractContainerAccess::isPointerType(containerAccess->keyMetaType())){
            if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCHashAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCHashAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCHashAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCHashAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCHashAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
            if(result1.hasPointers || result2.hasPointers)
                containerAccess = new NestedPointersRCHashAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractMapAccess* checkContainerAccess(JNIEnv * env, AbstractMapAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperMapAccess*>(containerAccess)){
        if(AbstractContainerAccess::isPointerType(containerAccess->keyMetaType())){
            if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCMapAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCMapAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMapAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCMapAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCMapAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
            if(result1.hasPointers || result2.hasPointers)
                containerAccess = new NestedPointersRCMapAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractMultiHashAccess* checkContainerAccess(JNIEnv * env, AbstractMultiHashAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperMultiHashAccess*>(containerAccess)){
        if(AbstractContainerAccess::isPointerType(containerAccess->keyMetaType())){
            if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCMultiHashAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMultiHashAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCMultiHashAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
            if(result1.hasPointers || result2.hasPointers)
                containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractMultiMapAccess* checkContainerAccess(JNIEnv * env, AbstractMultiMapAccess* containerAccess){
    QtJambiAPI::checkPointer(env, containerAccess);
    if(!dynamic_cast<WrapperMultiMapAccess*>(containerAccess)){
        if(AbstractContainerAccess::isPointerType(containerAccess->keyMetaType())){
            if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCMultiMapAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMultiMapAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(AbstractContainerAccess::isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCMultiMapAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(env, containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(env, containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
            if(result1.hasPointers || result2.hasPointers)
                containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional));
        }
    }
    return containerAccess;
}

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
#endif
void* AbstractContainerAccess::createContainer(const void* copy){
    size_t sz = sizeOf();
    if(sz>0){
        return constructContainer(operator new(sz), copy);
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
void AbstractSequentialConstIteratorAccess::assign(void*, const void*) {}
size_t AbstractSequentialConstIteratorAccess::sizeOf() {return 0;}
void* AbstractSequentialConstIteratorAccess::constructContainer(void*,const void*) {return nullptr;}
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* AbstractSequentialConstIteratorAccess::constructContainer(void*,void*) {return nullptr;}
#endif
bool AbstractSequentialConstIteratorAccess::destructContainer(void*) {return false;}
int AbstractSequentialConstIteratorAccess::registerContainer(const QByteArray&) {return QMetaType::UnknownType;}

AbstractAssociativeConstIteratorAccess::~AbstractAssociativeConstIteratorAccess(){}
AbstractAssociativeConstIteratorAccess::AbstractAssociativeConstIteratorAccess(){}

AbstractSequentialIteratorAccess::~AbstractSequentialIteratorAccess(){}
AbstractSequentialIteratorAccess::AbstractSequentialIteratorAccess(){}
AbstractAssociativeIteratorAccess::~AbstractAssociativeIteratorAccess(){}
AbstractAssociativeIteratorAccess::AbstractAssociativeIteratorAccess(){}

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

RCSet::RCSet() {};
RCSet::~RCSet() {}

RCSet::RCSet(const RCSet& other)
      : RCSet() {
    if(other.m_rcSet.object()){
        if(JniEnvironment env{500}){
            addAllRC(env, other.m_rcSet.object());
        }
    }
}

//RCSet::~RCSet(){}

void RCSet::assignRC(JNIEnv * env, const RCSet& other){
    if(other.m_rcSet.object()){
        clearRC(env);
        addAllRC(env, other.m_rcSet.object());
    }else if(m_rcSet.object()){
        Java::Runtime::Collection::clear(env, m_rcSet.object());
    }
}

jobject RCSet::rcSet(JNIEnv * env){
    if(!m_rcSet.object()){
        m_rcSet = JObjectWrapper(env, Java::QtJambi::ReferenceUtility$RCSet::newInstance(env));
    }
    return env->NewLocalRef(m_rcSet.object());
}

void RCSet::clearRC(JNIEnv * env){
    if(m_rcSet.object()){
        Java::Runtime::Collection::clear(env, m_rcSet.object());
    }
}

void RCSet::addRC(JNIEnv * env, jobject value){
    Java::Runtime::Collection::add(env, rcSet(env), value);
}

void RCSet::addAllRC(JNIEnv * env, jobject list){
    Java::Runtime::Collection::addAll(env, rcSet(env), list);
}

void RCSet::removeRC(JNIEnv * env, jobject value){
    if(m_rcSet.object()){
        Java::Runtime::Collection::remove(env, m_rcSet.object(), value);
    }
}

RCMap::RCMap() {}
RCMap::~RCMap() {}

RCMap::RCMap(JNIEnv * env, jobject otherRCMap)
      : RCMap() {
    Java::Runtime::Map::putAll(env, rcMap(env), otherRCMap);
}

void RCMap::assignRC(JNIEnv * env, const RCMap& other){
    if(other.m_rcMap.object()){
        clearRC(env);
        putAllRC(env, other.m_rcMap.object());
    }else if(m_rcMap.object()){
        Java::Runtime::Map::clear(env, m_rcMap.object());
    }
}

jobject RCMap::rcMap(JNIEnv * env){
    if(!m_rcMap.object()){
        m_rcMap = JObjectWrapper(env, Java::QtJambi::ReferenceUtility$RCMap::newInstance(env));
    }
    return env->NewLocalRef(m_rcMap.object());
}

void RCMap::clearRC(JNIEnv * env){
    if(m_rcMap.object()){
        Java::Runtime::Map::clear(env, m_rcMap.object());
    }
}

void RCMap::putRC(JNIEnv * env, jobject key, jobject value){
    Java::Runtime::Map::put(env, rcMap(env), key, value);
}

void RCMap::putAllRC(JNIEnv * env, jobject values){
    Java::Runtime::Map::putAll(env, rcMap(env), values);
}

void RCMap::removeRC(JNIEnv * env, jobject key){
    if(m_rcMap.object()){
        Java::Runtime::Map::remove(env, m_rcMap.object(), key);
    }
}

RCMultiMap::RCMultiMap(){}
RCMultiMap::~RCMultiMap(){}

RCMultiMap::RCMultiMap(JNIEnv * env, jobject otherRCMap)
      : RCMultiMap() {
    jobject entrySet = Java::Runtime::Map::entrySet(env, otherRCMap);
    jobject iter = Java::Runtime::Collection::iterator(env, entrySet);
    while(Java::Runtime::Iterator::hasNext(env, iter)){
        jobject entry = Java::Runtime::Iterator::next(env, iter);
        jobject value = Java::Runtime::ArrayList::newInstance(env);
        Java::Runtime::Collection::addAll(env, value, Java::Runtime::Map$Entry::getValue(env, entry));
        Java::Runtime::Map::put(env, rcMap(env), Java::Runtime::Map$Entry::getKey(env, entry), value);
    }
}

jobject RCMultiMap::rcMap(JNIEnv * env){
    if(!m_rcMap.object()){
        m_rcMap = JObjectWrapper(env, Java::QtJambi::ReferenceUtility$RCMap::newInstance(env));
    }
    return env->NewLocalRef(m_rcMap.object());
}

void RCMultiMap::assignRC(JNIEnv * env, const RCMultiMap& other){
    if(other.m_rcMap.object()){
        clearRC(env);
        putAllRC(env, other.m_rcMap.object());
    }else if(m_rcMap.object()){
        Java::Runtime::Map::clear(env, m_rcMap.object());
    }
}

void RCMultiMap::uniteRC(JNIEnv * env, const RCMultiMap& other){
    if(other.m_rcMap.object()){
        putAllRC(env, other.m_rcMap.object());
    }
}

void RCMultiMap::clearRC(JNIEnv * env){
    if(m_rcMap.object()){
        Java::Runtime::Map::clear(env, m_rcMap.object());
    }
}

void RCMultiMap::putRC(JNIEnv * env, jobject key, jobject value){
    jobject values = Java::Runtime::Map::get(env, rcMap(env), key);
    if(!values){
        values = Java::Runtime::ArrayList::newInstance(env);
        Java::Runtime::Map::put(env, rcMap(env), key, values);
    }
    Java::Runtime::Collection::add(env, values, value);
}

void RCMultiMap::replaceRC(JNIEnv * env, jobject key, jobject value){
    jobject values = Java::Runtime::ArrayList::newInstance(env);
    Java::Runtime::Collection::add(env, values, value);
    Java::Runtime::Map::put(env, rcMap(env), key, values);
}

void RCMultiMap::putAllRC(JNIEnv * env, jobject values){
    jobject entrySet = Java::Runtime::Map::entrySet(env, values);
    jobject iter = Java::Runtime::Collection::iterator(env, entrySet);
    while(Java::Runtime::Iterator::hasNext(env, iter)){
        jobject entry = Java::Runtime::Iterator::next(env, iter);
        jobject value = Java::Runtime::ArrayList::newInstance(env);
        Java::Runtime::Collection::addAll(env, value, Java::Runtime::Map$Entry::getValue(env, entry));
        Java::Runtime::Map::put(env, rcMap(env), Java::Runtime::Map$Entry::getKey(env, entry), value);
    }
}

void RCMultiMap::removeRC(JNIEnv * env, jobject key){
    if(m_rcMap.object()){
        Java::Runtime::Map::remove(env, m_rcMap.object(), key);
    }
}

void RCMultiMap::removeRC(JNIEnv * env, jobject key, jobject value){
    if(m_rcMap.object()){
        jobject values = Java::Runtime::Map::get(env, m_rcMap.object(), key);
        if(values){
            Java::Runtime::Collection::remove(env, values, value);
        }
    }
}

PointerRCListAccess::~PointerRCListAccess(){}

PointerRCListAccess::PointerRCListAccess(AbstractListAccess* containerAccess)
    : WrapperListAccess(containerAccess), RCSet() {
    Q_ASSERT(containerAccess!=this);
}

PointerRCListAccess::PointerRCListAccess(PointerRCListAccess& other)
    : WrapperListAccess(other.WrapperListAccess::clone()), RCSet(other) {}

PointerRCListAccess* PointerRCListAccess::clone(){
    return new PointerRCListAccess(*this);
}

void PointerRCListAccess::assign(void* container, const void* other){
    WrapperListAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* PointerRCListAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperListAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* PointerRCListAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperListAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void PointerRCListAccess::appendList(JNIEnv * env, void* container, jobject list) {
    WrapperListAccess::appendList(env, container, list);
    if(list)
        addAllRC(env, list);
}

void PointerRCListAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    jobject oldValue = WrapperListAccess::at(env, container, index);
    WrapperListAccess::replace(env, container, index, value);
    if(oldValue && !WrapperListAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
    if(value)
        addRC(env, value);
}

jint PointerRCListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperListAccess::removeAll(env, container, value);
    if(result>0){
        removeRC(env, value);
    }
    return result;
}

jobject PointerRCListAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperListAccess::mid(env, container, index1, index2);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(result)){
        if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void PointerRCListAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    WrapperListAccess::insert(env, container, index, n, value);
    if(value)
        addRC(env, value);
}

void PointerRCListAccess::clear(JNIEnv * env, void* container) {
    WrapperListAccess::clear(env, container);
    clearRC(env);
}

void PointerRCListAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
    if(n==1){
        jobject oldValue = WrapperListAccess::at(env, container, index);
        WrapperListAccess::remove(env, container, index, n);
        if(!WrapperListAccess::contains(env, container, oldValue))
            removeRC(env, oldValue);
    }else{
        jint size = WrapperListAccess::size(env, container);
        jobject removedValues = Java::Runtime::ArrayList::newInstance(env);
        for(jint i = index; i<=index+n && i<size; ++i){
            Java::Runtime::Collection::add(env, removedValues, WrapperListAccess::at(env, container, i));
        }
        WrapperListAccess::remove(env, container, index, n);
        jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(!WrapperListAccess::contains(env, container, value))
                removeRC(env, value);
        }
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void PointerRCListAccess::fill(JNIEnv * env, void* container, jobject value, jint size){
    jint oldSize = WrapperListAccess::size(env, container);
    WrapperListAccess::fill(env, container, value, size);
    if(value && oldSize<size)
        addRC(env, value);
}
#endif

PointerRCSetAccess::~PointerRCSetAccess(){}

PointerRCSetAccess::PointerRCSetAccess(AbstractSetAccess* containerAccess)
    : WrapperSetAccess(containerAccess), RCSet() {}

PointerRCSetAccess::PointerRCSetAccess(PointerRCSetAccess& other)
    : WrapperSetAccess(other.WrapperSetAccess::clone()), RCSet(other) {}

PointerRCSetAccess* PointerRCSetAccess::clone(){
    return new PointerRCSetAccess(*this);
}

void PointerRCSetAccess::assign(void* container, const void* other){
    WrapperSetAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointerRCSetAccess* access = dynamic_cast<PointerRCSetAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* PointerRCSetAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperSetAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(PointerRCSetAccess* access = dynamic_cast<PointerRCSetAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* PointerRCSetAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperSetAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(PointerRCSetAccess* access = dynamic_cast<PointerRCSetAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void PointerRCSetAccess::clear(JNIEnv * env, void* container) {
    WrapperSetAccess::clear(env, container);
    clearRC(env);
}

void PointerRCSetAccess::insert(JNIEnv * env, void* container, jobject value){
    WrapperSetAccess::insert(env, container, value);
    if(value)
        addRC(env, value);
}

jboolean PointerRCSetAccess::remove(JNIEnv * env, void* container, jobject value){
    jboolean result = WrapperSetAccess::remove(env, container, value);
    if(value)
        removeRC(env, value);
    return result;
}

void PointerRCSetAccess::intersect(JNIEnv * env, void* container, jobject other){
    WrapperSetAccess::intersect(env, container, other);
    if(Java::Runtime::Collection::isInstanceOf(env, other)){
        clearRC(env);
        jobject iter = Java::Runtime::Collection::iterator(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(value && WrapperSetAccess::contains(env, container, value))
                addRC(env, value);
        }
    }
}

void PointerRCSetAccess::subtract(JNIEnv * env, void* container, jobject other){
    WrapperSetAccess::subtract(env, container, other);
    if(Java::Runtime::Collection::isInstanceOf(env, other)){
        jobject iter = Java::Runtime::Collection::iterator(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(value)
                removeRC(env, value);
        }
    }
}

void PointerRCSetAccess::unite(JNIEnv * env, void* container, jobject other){
    WrapperSetAccess::unite(env, container, other);
    if(Java::Runtime::Collection::isInstanceOf(env, other)){
        jobject iter = Java::Runtime::Collection::iterator(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(value)
                addRC(env, value);
        }
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

PointerRCLinkedListAccess::~PointerRCLinkedListAccess(){}

PointerRCLinkedListAccess::PointerRCLinkedListAccess(AbstractLinkedListAccess* containerAccess)
    : WrapperLinkedListAccess(containerAccess), RCSet() {}

PointerRCLinkedListAccess::PointerRCLinkedListAccess(PointerRCLinkedListAccess& other)
    : WrapperLinkedListAccess(other.WrapperLinkedListAccess::clone()), RCSet(other) {}

PointerRCLinkedListAccess* PointerRCLinkedListAccess::clone(){
    return new PointerRCLinkedListAccess(*this);
}

void PointerRCLinkedListAccess::assign(void* container, const void* other){
    WrapperLinkedListAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointerRCLinkedListAccess* access = dynamic_cast<PointerRCLinkedListAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* PointerRCLinkedListAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperLinkedListAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(placement)){
            if(PointerRCLinkedListAccess* access = dynamic_cast<PointerRCLinkedListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

void PointerRCLinkedListAccess::clear(JNIEnv * env, void* container) {
    WrapperLinkedListAccess::clear(env, container);
    clearRC(env);
}

void PointerRCLinkedListAccess::append(JNIEnv * env, void* container, jobject value) {
    WrapperLinkedListAccess::append(env, container, value);
    if(value)
        addRC(env, value);
}

void PointerRCLinkedListAccess::prepend(JNIEnv * env, void* container, jobject value) {
    WrapperLinkedListAccess::prepend(env, container, value);
    if(value)
        addRC(env, value);
}

jboolean PointerRCLinkedListAccess::removeOne(JNIEnv * env, void* container, jobject value) {
    jboolean result = WrapperLinkedListAccess::removeOne(env, container, value);
    if(result && !WrapperLinkedListAccess::contains(env, container, value)){
        removeRC(env, value);
    }
    return result;
}

void PointerRCLinkedListAccess::removeFirst(JNIEnv * env, void* container) {
    jobject oldValue = WrapperLinkedListAccess::first(env, container);
    WrapperLinkedListAccess::removeFirst(env, container);
    if(!WrapperLinkedListAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
}

void PointerRCLinkedListAccess::removeLast(JNIEnv * env, void* container) {
    jobject oldValue = WrapperLinkedListAccess::last(env, container);
    WrapperLinkedListAccess::removeLast(env, container);
    if(!WrapperLinkedListAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
}

jint PointerRCLinkedListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperLinkedListAccess::removeAll(env, container, value);
    if(result>0){
        removeRC(env, value);
    }
    return result;
}

jobject PointerRCLinkedListAccess::takeFirst(JNIEnv * env, void* container) {
    jobject result = WrapperLinkedListAccess::takeFirst(env, container);
    if(result && !WrapperLinkedListAccess::contains(env, container, result))
        removeRC(env, result);
    return result;
}

jobject PointerRCLinkedListAccess::takeLast(JNIEnv * env, void* container) {
    jobject result = WrapperLinkedListAccess::takeLast(env, container);
    if(result && !WrapperLinkedListAccess::contains(env, container, result))
        removeRC(env, result);
    return result;
}

PointerRCVectorAccess::~PointerRCVectorAccess(){}

PointerRCVectorAccess::PointerRCVectorAccess(AbstractVectorAccess* containerAccess)
    : WrapperVectorAccess(containerAccess), RCSet() {}

PointerRCVectorAccess::PointerRCVectorAccess(PointerRCVectorAccess& other)
    : WrapperVectorAccess(other.WrapperVectorAccess::clone()), RCSet(other) {}

PointerRCVectorAccess* PointerRCVectorAccess::clone(){
    return new PointerRCVectorAccess(*this);
}

void* PointerRCVectorAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperVectorAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(placement)){
            if(PointerRCVectorAccess* access = dynamic_cast<PointerRCVectorAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

void PointerRCVectorAccess::assign(void* container, const void* other){
    WrapperVectorAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointerRCVectorAccess* access = dynamic_cast<PointerRCVectorAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void PointerRCVectorAccess::appendVector(JNIEnv * env, void* container, jobject list) {
    WrapperVectorAccess::appendVector(env, container, list);
    if(list)
        addAllRC(env, list);
}

void PointerRCVectorAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    jobject oldValue = WrapperVectorAccess::at(env, container, index);
    WrapperVectorAccess::replace(env, container, index, value);
    if(oldValue && !WrapperVectorAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
    if(value)
        addRC(env, value);
}

jint PointerRCVectorAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperVectorAccess::removeAll(env, container, value);
    if(result>0){
        removeRC(env, value);
    }
    return result;
}

jobject PointerRCVectorAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperVectorAccess::mid(env, container, index1, index2);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(result)){
        if(PointerRCVectorAccess* access = dynamic_cast<PointerRCVectorAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void PointerRCVectorAccess::clear(JNIEnv * env, void* container) {
    WrapperVectorAccess::clear(env, container);
    clearRC(env);
}

void PointerRCVectorAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
    jint size = WrapperVectorAccess::size(env, container);
    jobject removedValues = Java::Runtime::ArrayList::newInstance(env);
    for(jint i = index; i<=index+n && i<size; ++i){
        Java::Runtime::Collection::add(env, removedValues, WrapperVectorAccess::at(env, container, i));
    }
    WrapperVectorAccess::remove(env, container, index, n);
    jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
    while(Java::Runtime::Iterator::hasNext(env, iter)){
        jobject value = Java::Runtime::Iterator::next(env, iter);
        if(!WrapperVectorAccess::contains(env, container, value))
            removeRC(env, value);
    }
}

void PointerRCVectorAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    WrapperVectorAccess::insert(env, container, index, n, value);
    addRC(env, value);
}

void PointerRCVectorAccess::fill(JNIEnv * env, void* container, jobject value, jint size){
    jint oldSize = WrapperVectorAccess::size(env, container);
    WrapperVectorAccess::fill(env, container, value, size);
    if(value && oldSize<size)
        addRC(env, value);
}

#endif

KeyPointerRCMapAccess::~KeyPointerRCMapAccess(){}

KeyPointerRCMapAccess::KeyPointerRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), RCSet() {}

KeyPointerRCMapAccess::KeyPointerRCMapAccess(KeyPointerRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), RCSet(other) {}

KeyPointerRCMapAccess* KeyPointerRCMapAccess::clone(){
    return new KeyPointerRCMapAccess(*this);
}

void KeyPointerRCMapAccess::assign(void* container, const void* other){
    WrapperMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(KeyPointerRCMapAccess* access = dynamic_cast<KeyPointerRCMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* KeyPointerRCMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(KeyPointerRCMapAccess* access = dynamic_cast<KeyPointerRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* KeyPointerRCMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(KeyPointerRCMapAccess* access = dynamic_cast<KeyPointerRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void KeyPointerRCMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env);
}

void KeyPointerRCMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMapAccess::insert(env, container, key, value);
    addRC(env, key);
}

jint KeyPointerRCMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jobject KeyPointerRCMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    removeRC(env, key);
    return result;
}

ValuePointerRCMapAccess::~ValuePointerRCMapAccess(){}

ValuePointerRCMapAccess::ValuePointerRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), RCSet() {}

ValuePointerRCMapAccess::ValuePointerRCMapAccess(ValuePointerRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), RCSet(other) {}

ValuePointerRCMapAccess* ValuePointerRCMapAccess::clone(){
    return new ValuePointerRCMapAccess(*this);
}

void ValuePointerRCMapAccess::assign(void* container, const void* other){
    WrapperMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(ValuePointerRCMapAccess* access = dynamic_cast<ValuePointerRCMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* ValuePointerRCMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(ValuePointerRCMapAccess* access = dynamic_cast<ValuePointerRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* ValuePointerRCMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(ValuePointerRCMapAccess* access = dynamic_cast<ValuePointerRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void ValuePointerRCMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env);
}

void ValuePointerRCMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    jobject oldValue = WrapperMapAccess::value(env, container, key, nullptr);
    WrapperMapAccess::insert(env, container, key, value);
    removeRC(env, oldValue);
    addRC(env, value);
}

jint ValuePointerRCMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jobject oldValue = WrapperMapAccess::value(env, container, key, nullptr);
    jint result = WrapperMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, oldValue);
    }
    return result;
}

jobject ValuePointerRCMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    removeRC(env, result);
    return result;
}

PointersRCMapAccess::PointersRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), RCMap() {}

PointersRCMapAccess::PointersRCMapAccess(PointersRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), RCMap(other) {}

PointersRCMapAccess* PointersRCMapAccess::clone(){
    return new PointersRCMapAccess(*this);
}

void PointersRCMapAccess::assign(void* container, const void* other){
    WrapperMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointersRCMapAccess* access = dynamic_cast<PointersRCMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* PointersRCMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(PointersRCMapAccess* access = dynamic_cast<PointersRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* PointersRCMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(PointersRCMapAccess* access = dynamic_cast<PointersRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void PointersRCMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env);
}

void PointersRCMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMapAccess::insert(env, container, key, value);
    putRC(env, key, value);
}

jint PointersRCMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jobject PointersRCMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    removeRC(env, key);
    return result;
}

KeyPointerRCMultiMapAccess::~KeyPointerRCMultiMapAccess(){}

KeyPointerRCMultiMapAccess::KeyPointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), RCSet() {}

KeyPointerRCMultiMapAccess::KeyPointerRCMultiMapAccess(KeyPointerRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), RCSet(other) {}

KeyPointerRCMultiMapAccess* KeyPointerRCMultiMapAccess::clone(){
    return new KeyPointerRCMultiMapAccess(*this);
}

void KeyPointerRCMultiMapAccess::assign(void* container, const void* other){
    WrapperMultiMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(KeyPointerRCMultiMapAccess* access = dynamic_cast<KeyPointerRCMultiMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* KeyPointerRCMultiMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(KeyPointerRCMultiMapAccess* access = dynamic_cast<KeyPointerRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* KeyPointerRCMultiMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(KeyPointerRCMultiMapAccess* access = dynamic_cast<KeyPointerRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void KeyPointerRCMultiMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env);
}

void KeyPointerRCMultiMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    addRC(env, key);
}

jint KeyPointerRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jint KeyPointerRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key, jobject value) {
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jobject KeyPointerRCMultiMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    removeRC(env, key);
    return result;
}

void KeyPointerRCMultiMapAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiMapAccess::replace(env, container, key, value);
    addRC(env, key);
}

void KeyPointerRCMultiMapAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiMapAccess::unite(env, container, other);
    if(Java::Runtime::Map::isInstanceOf(env, other)){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            addRC(env, Java::Runtime::Map$Entry::getKey(env, entry));
        }
    }
}

void KeyPointerRCMultiHashAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiHashAccess::unite(env, container, other);
    if(Java::Runtime::Map::isInstanceOf(env, other)){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            addRC(env, Java::Runtime::Map$Entry::getKey(env, entry));
        }
    }
}

ValuePointerRCMultiMapAccess::ValuePointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), RCSet() {}

ValuePointerRCMultiMapAccess::ValuePointerRCMultiMapAccess(ValuePointerRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), RCSet(other) {}

ValuePointerRCMultiMapAccess* ValuePointerRCMultiMapAccess::clone(){
    return new ValuePointerRCMultiMapAccess(*this);
}

void ValuePointerRCMultiMapAccess::assign(void* container, const void* other){
    WrapperMultiMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(ValuePointerRCMultiMapAccess* access = dynamic_cast<ValuePointerRCMultiMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* ValuePointerRCMultiMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(ValuePointerRCMultiMapAccess* access = dynamic_cast<ValuePointerRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* ValuePointerRCMultiMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(ValuePointerRCMultiMapAccess* access = dynamic_cast<ValuePointerRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void ValuePointerRCMultiMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env);
}

void ValuePointerRCMultiMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    addRC(env, value);
}

jint ValuePointerRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jobject oldValues = WrapperMultiMapAccess::values(env, container, key);
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    if(result>0){
        jobject iter = Java::Runtime::Collection::iterator(env, oldValues);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(Java::Runtime::Collection::size(env, WrapperMultiMapAccess::keys(env, container, value))==0){
                removeRC(env, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCMultiMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    if(Java::Runtime::Collection::size(env, WrapperMultiMapAccess::keys(env, container, result))==0){
        removeRC(env, result);
    }
    return result;
}

void ValuePointerRCMultiMapAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    jobject oldValue = WrapperMultiMapAccess::value(env, container, key, nullptr);
    WrapperMultiMapAccess::replace(env, container, key, value);
    removeRC(env, oldValue);
    addRC(env, value);
}

void ValuePointerRCMultiMapAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiMapAccess::unite(env, container, other);
    if(Java::Runtime::Map::isInstanceOf(env, other)){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            jobject values = Java::Runtime::Map$Entry::getValue(env, entry);
            addAllRC(env, values);
        }
    }
}

void ValuePointerRCMultiHashAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiHashAccess::unite(env, container, other);
    if(Java::Runtime::Map::isInstanceOf(env, other)){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            jobject values = Java::Runtime::Map$Entry::getValue(env, entry);
            addAllRC(env, values);
        }
    }
}

jint ValuePointerRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key, jobject value){
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    if(result>0)
        removeRC(env, value);
    return result;
}

PointersRCMultiMapAccess::PointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), RCMultiMap() {}

PointersRCMultiMapAccess::PointersRCMultiMapAccess(PointersRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), RCMultiMap(other) {}

PointersRCMultiMapAccess* PointersRCMultiMapAccess::clone(){
    return new PointersRCMultiMapAccess(*this);
}

void PointersRCMultiMapAccess::assign(void* container, const void* other){
    WrapperMultiMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointersRCMultiMapAccess* access = dynamic_cast<PointersRCMultiMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* PointersRCMultiMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(PointersRCMultiMapAccess* access = dynamic_cast<PointersRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* PointersRCMultiMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(PointersRCMultiMapAccess* access = dynamic_cast<PointersRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void PointersRCMultiMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env);
}

void PointersRCMultiMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    putRC(env, key, value);
}

void PointersRCMultiMapAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiMapAccess::replace(env, container, key, value);
    replaceRC(env, key, value);
}

void PointersRCMultiMapAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiMapAccess::unite(env, container, other);
    if(Java::Runtime::Map::isInstanceOf(env, other)){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            jobject values = Java::Runtime::Map$Entry::getValue(env, entry);
            jobject iter2 = Java::Runtime::Collection::iterator(env, values);
            while(Java::Runtime::Iterator::hasNext(env, iter2)){
                jobject value = Java::Runtime::Iterator::next(env, iter2);
                putRC(env, Java::Runtime::Map$Entry::getKey(env, entry), value);
            }
        }
    }
}

jint PointersRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jint PointersRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key, jobject value) {
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    if(result>0){
        removeRC(env, key, value);
    }
    return result;
}

jobject PointersRCMultiMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    removeRC(env, key, result);
    return result;
}

KeyPointerRCHashAccess::~KeyPointerRCHashAccess(){}

KeyPointerRCHashAccess::KeyPointerRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), RCSet() {}

KeyPointerRCHashAccess::KeyPointerRCHashAccess(KeyPointerRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), RCSet(other) {}

KeyPointerRCHashAccess* KeyPointerRCHashAccess::clone(){
    return new KeyPointerRCHashAccess(*this);
}

void KeyPointerRCHashAccess::assign(void* container, const void* other){
    WrapperHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(KeyPointerRCHashAccess* access = dynamic_cast<KeyPointerRCHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* KeyPointerRCHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(KeyPointerRCHashAccess* access = dynamic_cast<KeyPointerRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* KeyPointerRCHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(KeyPointerRCHashAccess* access = dynamic_cast<KeyPointerRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void KeyPointerRCHashAccess::clear(JNIEnv * env, void* container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env);
}

void KeyPointerRCHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperHashAccess::insert(env, container, key, value);
    addRC(env, key);
}

jint KeyPointerRCHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jobject KeyPointerRCHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, key);
    return result;
}

ValuePointerRCHashAccess::ValuePointerRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), RCSet() {}

ValuePointerRCHashAccess::ValuePointerRCHashAccess(ValuePointerRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), RCSet(other) {}

ValuePointerRCHashAccess* ValuePointerRCHashAccess::clone(){
    return new ValuePointerRCHashAccess(*this);
}

void ValuePointerRCHashAccess::assign(void* container, const void* other){
    WrapperHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(ValuePointerRCHashAccess* access = dynamic_cast<ValuePointerRCHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* ValuePointerRCHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(ValuePointerRCHashAccess* access = dynamic_cast<ValuePointerRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* ValuePointerRCHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(ValuePointerRCHashAccess* access = dynamic_cast<ValuePointerRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void ValuePointerRCHashAccess::clear(JNIEnv * env, void* container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env);
}

void ValuePointerRCHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    jobject oldValue = WrapperHashAccess::value(env, container, key, nullptr);
    WrapperHashAccess::insert(env, container, key, value);
    removeRC(env, oldValue);
    addRC(env, value);
}

jint ValuePointerRCHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jobject oldValue = WrapperHashAccess::value(env, container, key, nullptr);
    jint result = WrapperHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, oldValue);
    }
    return result;
}

jobject ValuePointerRCHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, result);
    return result;
}

PointersRCHashAccess::PointersRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), RCMap() {}

PointersRCHashAccess::PointersRCHashAccess(PointersRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), RCMap(other) {}

PointersRCHashAccess* PointersRCHashAccess::clone(){
    return new PointersRCHashAccess(*this);
}

void PointersRCHashAccess::assign(void* container, const void* other){
    WrapperHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointersRCHashAccess* access = dynamic_cast<PointersRCHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* PointersRCHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(PointersRCHashAccess* access = dynamic_cast<PointersRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* PointersRCHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(PointersRCHashAccess* access = dynamic_cast<PointersRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void PointersRCHashAccess::clear(JNIEnv * env, void* container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env);
}

void PointersRCHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperHashAccess::insert(env, container, key, value);
    putRC(env, key, value);
}

jint PointersRCHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jobject PointersRCHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, key);
    return result;
}

KeyPointerRCMultiHashAccess::~KeyPointerRCMultiHashAccess(){}

KeyPointerRCMultiHashAccess::KeyPointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), RCSet() {}

KeyPointerRCMultiHashAccess::KeyPointerRCMultiHashAccess(KeyPointerRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), RCSet(other) {}

KeyPointerRCMultiHashAccess* KeyPointerRCMultiHashAccess::clone(){
    return new KeyPointerRCMultiHashAccess(*this);
}

void KeyPointerRCMultiHashAccess::assign(void* container, const void* other){
    WrapperMultiHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(KeyPointerRCMultiHashAccess* access = dynamic_cast<KeyPointerRCMultiHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* KeyPointerRCMultiHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(KeyPointerRCMultiHashAccess* access = dynamic_cast<KeyPointerRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* KeyPointerRCMultiHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(KeyPointerRCMultiHashAccess* access = dynamic_cast<KeyPointerRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void KeyPointerRCMultiHashAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiHashAccess::clear(env, container);
    clearRC(env);
}

void KeyPointerRCMultiHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    addRC(env, key);
}

jint KeyPointerRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jint KeyPointerRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key, jobject value) {
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jobject KeyPointerRCMultiHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    removeRC(env, key);
    return result;
}

void KeyPointerRCMultiHashAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiHashAccess::replace(env, container, key, value);
    addRC(env, key);
}

ValuePointerRCMultiHashAccess::ValuePointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), RCSet() {}

ValuePointerRCMultiHashAccess::ValuePointerRCMultiHashAccess(ValuePointerRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), RCSet(other) {}

ValuePointerRCMultiHashAccess* ValuePointerRCMultiHashAccess::clone(){
    return new ValuePointerRCMultiHashAccess(*this);
}

void ValuePointerRCMultiHashAccess::assign(void* container, const void* other){
    WrapperMultiHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(ValuePointerRCMultiHashAccess* access = dynamic_cast<ValuePointerRCMultiHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* ValuePointerRCMultiHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(ValuePointerRCMultiHashAccess* access = dynamic_cast<ValuePointerRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* ValuePointerRCMultiHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(ValuePointerRCMultiHashAccess* access = dynamic_cast<ValuePointerRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void ValuePointerRCMultiHashAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiHashAccess::clear(env, container);
    clearRC(env);
}

void ValuePointerRCMultiHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    addRC(env, value);
}

jint ValuePointerRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jobject oldValues = WrapperMultiHashAccess::values(env, container, key);
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    if(result>0){
        jobject iter = Java::Runtime::Collection::iterator(env, oldValues);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(Java::Runtime::Collection::size(env, WrapperMultiHashAccess::keys(env, container, value))==0){
                removeRC(env, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCMultiHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    if(Java::Runtime::Collection::size(env, WrapperMultiHashAccess::keys(env, container, result))==0){
        removeRC(env, result);
    }
    return result;
}

void ValuePointerRCMultiHashAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    jobject oldValue = WrapperMultiHashAccess::value(env, container, key, nullptr);
    WrapperMultiHashAccess::replace(env, container, key, value);
    removeRC(env, oldValue);
    addRC(env, value);
}

jint ValuePointerRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key, jobject value){
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    if(result>0)
        removeRC(env, value);
    return result;
}

PointersRCMultiHashAccess::PointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), RCMultiMap() {}

PointersRCMultiHashAccess::PointersRCMultiHashAccess(PointersRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), RCMultiMap(other) {}

PointersRCMultiHashAccess* PointersRCMultiHashAccess::clone(){
    return new PointersRCMultiHashAccess(*this);
}

void PointersRCMultiHashAccess::assign(void* container, const void* other){
    WrapperMultiHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(PointersRCMultiHashAccess* access = dynamic_cast<PointersRCMultiHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* PointersRCMultiHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(PointersRCMultiHashAccess* access = dynamic_cast<PointersRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* PointersRCMultiHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(PointersRCMultiHashAccess* access = dynamic_cast<PointersRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void PointersRCMultiHashAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiHashAccess::clear(env, container);
    clearRC(env);
}

void PointersRCMultiHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    putRC(env, key, value);
}

void PointersRCMultiHashAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiHashAccess::replace(env, container, key, value);
    replaceRC(env, key, value);
}

void PointersRCMultiHashAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiHashAccess::unite(env, container, other);
    if(Java::Runtime::Map::isInstanceOf(env, other)){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            jobject values = Java::Runtime::Map$Entry::getValue(env, entry);
            jobject iter2 = Java::Runtime::Collection::iterator(env, values);
            while(Java::Runtime::Iterator::hasNext(env, iter2)){
                jobject value = Java::Runtime::Iterator::next(env, iter2);
                putRC(env, Java::Runtime::Map$Entry::getKey(env, entry), value);
            }
        }
    }
}

jint PointersRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jint PointersRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key, jobject value) {
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    if(result>0){
        removeRC(env, key, value);
    }
    return result;
}

jobject PointersRCMultiHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    removeRC(env, key, result);
    return result;
}

NestedPointersRCListAccess::NestedPointersRCListAccess(AbstractListAccess* containerAccess)
    : WrapperListAccess(containerAccess), RCSet() {
    Q_ASSERT(containerAccess!=this);
}

NestedPointersRCListAccess::NestedPointersRCListAccess(NestedPointersRCListAccess& other)
    : WrapperListAccess(other.WrapperListAccess::clone()), RCSet(other) {}

NestedPointersRCListAccess* NestedPointersRCListAccess::clone(){
    return new NestedPointersRCListAccess(*this);
}

void NestedPointersRCListAccess::assign(void* container, const void* other){
    WrapperListAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCListAccess* access = dynamic_cast<NestedPointersRCListAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* NestedPointersRCListAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperListAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(NestedPointersRCListAccess* access = dynamic_cast<NestedPointersRCListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* NestedPointersRCListAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperListAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(NestedPointersRCListAccess* access = dynamic_cast<NestedPointersRCListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void unfoldAndAddContainer(JNIEnv * env, jobject set, jobject value){
    if(Java::Runtime::Collection::isInstanceOf(env, value)){
        jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, value);
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

void NestedPointersRCListAccess::appendList(JNIEnv * env, void* container, jobject list) {
    WrapperListAccess::appendList(env, container, list);
    unfoldAndAddContainer(env, rcSet(env), list);
}

void NestedPointersRCListAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    jint size = WrapperListAccess::size(env, container);
    for(jint i=0; i<size; ++i){
        jobject value = WrapperListAccess::at(env, container, i);
        unfoldAndAddContainer(env, set, value);
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCListAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    WrapperListAccess::replace(env, container, index, value);
    updateRC(env, container);
}

jint NestedPointersRCListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperListAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCListAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperListAccess::mid(env, container, index1, index2);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(result)){
        if(NestedPointersRCListAccess* access = dynamic_cast<NestedPointersRCListAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void NestedPointersRCListAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    WrapperListAccess::insert(env, container, index, n, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCListAccess::clear(JNIEnv * env, void* container) {
    WrapperListAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCListAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
    WrapperListAccess::remove(env, container, index, n);
    updateRC(env, container);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void NestedPointersRCListAccess::fill(JNIEnv * env, void* container, jobject value, jint size){
    WrapperListAccess::fill(env, container, value, size);
    unfoldAndAddContainer(env, rcSet(env), value);
}
#endif

NestedPointersRCSetAccess::NestedPointersRCSetAccess(AbstractSetAccess* containerAccess)
    : WrapperSetAccess(containerAccess), RCSet() {}

NestedPointersRCSetAccess::NestedPointersRCSetAccess(NestedPointersRCSetAccess& other)
    : WrapperSetAccess(other.WrapperSetAccess::clone()), RCSet(other) {}

NestedPointersRCSetAccess* NestedPointersRCSetAccess::clone(){
    return new NestedPointersRCSetAccess(*this);
}

void NestedPointersRCSetAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCSetAccess::assign(void* container, const void* other){
    WrapperSetAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCSetAccess* access = dynamic_cast<NestedPointersRCSetAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* NestedPointersRCSetAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperSetAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(NestedPointersRCSetAccess* access = dynamic_cast<NestedPointersRCSetAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* NestedPointersRCSetAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperSetAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(NestedPointersRCSetAccess* access = dynamic_cast<NestedPointersRCSetAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void NestedPointersRCSetAccess::clear(JNIEnv * env, void* container) {
    WrapperSetAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCSetAccess::insert(JNIEnv * env, void* container, jobject value){
    WrapperSetAccess::insert(env, container, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

jboolean NestedPointersRCSetAccess::remove(JNIEnv * env, void* container, jobject value){
    jboolean result = WrapperSetAccess::remove(env, container, value);
    updateRC(env, container);
    return result;
}

void NestedPointersRCSetAccess::intersect(JNIEnv * env, void* container, jobject other){
    WrapperSetAccess::intersect(env, container, other);
    updateRC(env, container);
}

void NestedPointersRCSetAccess::subtract(JNIEnv * env, void* container, jobject other){
    WrapperSetAccess::subtract(env, container, other);
    updateRC(env, container);
}

void NestedPointersRCSetAccess::unite(JNIEnv * env, void* container, jobject other){
    WrapperSetAccess::unite(env, container, other);
    updateRC(env, container);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

NestedPointersRCLinkedListAccess::NestedPointersRCLinkedListAccess(AbstractLinkedListAccess* containerAccess)
    : WrapperLinkedListAccess(containerAccess), RCSet() {}

NestedPointersRCLinkedListAccess::NestedPointersRCLinkedListAccess(NestedPointersRCLinkedListAccess& other)
    : WrapperLinkedListAccess(other.WrapperLinkedListAccess::clone()), RCSet(other) {}

NestedPointersRCLinkedListAccess* NestedPointersRCLinkedListAccess::clone(){
    return new NestedPointersRCLinkedListAccess(*this);
}

void NestedPointersRCLinkedListAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCLinkedListAccess::assign(void* container, const void* other){
    WrapperLinkedListAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCLinkedListAccess* access = dynamic_cast<NestedPointersRCLinkedListAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* NestedPointersRCLinkedListAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperLinkedListAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(placement)){
            if(NestedPointersRCLinkedListAccess* access = dynamic_cast<NestedPointersRCLinkedListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

void NestedPointersRCLinkedListAccess::clear(JNIEnv * env, void* container) {
    WrapperLinkedListAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCLinkedListAccess::append(JNIEnv * env, void* container, jobject value) {
    WrapperLinkedListAccess::append(env, container, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCLinkedListAccess::prepend(JNIEnv * env, void* container, jobject value) {
    WrapperLinkedListAccess::prepend(env, container, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

jboolean NestedPointersRCLinkedListAccess::removeOne(JNIEnv * env, void* container, jobject value) {
    jboolean result = WrapperLinkedListAccess::removeOne(env, container, value);
    if(result){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCLinkedListAccess::removeFirst(JNIEnv * env, void* container) {
    WrapperLinkedListAccess::removeFirst(env, container);
    updateRC(env, container);
}

void NestedPointersRCLinkedListAccess::removeLast(JNIEnv * env, void* container) {
    WrapperLinkedListAccess::removeLast(env, container);
    updateRC(env, container);
}

jint NestedPointersRCLinkedListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperLinkedListAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCLinkedListAccess::takeFirst(JNIEnv * env, void* container) {
    jobject result = WrapperLinkedListAccess::takeFirst(env, container);
    updateRC(env, container);
    return result;
}

jobject NestedPointersRCLinkedListAccess::takeLast(JNIEnv * env, void* container) {
    jobject result = WrapperLinkedListAccess::takeLast(env, container);
    updateRC(env, container);
    return result;
}

NestedPointersRCVectorAccess::NestedPointersRCVectorAccess(AbstractVectorAccess* containerAccess)
    : WrapperVectorAccess(containerAccess), RCSet() {}

NestedPointersRCVectorAccess::NestedPointersRCVectorAccess(NestedPointersRCVectorAccess& other)
    : WrapperVectorAccess(other.WrapperVectorAccess::clone()), RCSet(other) {}

NestedPointersRCVectorAccess* NestedPointersRCVectorAccess::clone(){
    return new NestedPointersRCVectorAccess(*this);
}

void NestedPointersRCVectorAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void* NestedPointersRCVectorAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperVectorAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(placement)){
            if(NestedPointersRCVectorAccess* access = dynamic_cast<NestedPointersRCVectorAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

void NestedPointersRCVectorAccess::assign(void* container, const void* other){
    WrapperVectorAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCVectorAccess* access = dynamic_cast<NestedPointersRCVectorAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void NestedPointersRCVectorAccess::appendVector(JNIEnv * env, void* container, jobject list) {
    WrapperVectorAccess::appendVector(env, container, list);
    unfoldAndAddContainer(env, rcSet(env), list);
}

jint NestedPointersRCVectorAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperVectorAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCVectorAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperVectorAccess::mid(env, container, index1, index2);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(result)){
        if(NestedPointersRCVectorAccess* access = dynamic_cast<NestedPointersRCVectorAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void NestedPointersRCVectorAccess::clear(JNIEnv * env, void* container) {
    WrapperVectorAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCVectorAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
    WrapperVectorAccess::remove(env, container, index, n);
    updateRC(env, container);
}

void NestedPointersRCVectorAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    WrapperVectorAccess::insert(env, container, index, n, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCVectorAccess::fill(JNIEnv * env, void* container, jobject value, jint size){
    WrapperVectorAccess::fill(env, container, value, size);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCVectorAccess::replace(JNIEnv * env, void* container, jint index, jobject value){
    WrapperVectorAccess::replace(env, container, index, value);
    updateRC(env, container);
}

#endif


NestedPointersRCMapAccess::NestedPointersRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), RCSet() {}

NestedPointersRCMapAccess::NestedPointersRCMapAccess(NestedPointersRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), RCSet(other) {}

NestedPointersRCMapAccess* NestedPointersRCMapAccess::clone(){
    return new NestedPointersRCMapAccess(*this);
}

void NestedPointersRCMapAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCMapAccess::assign(void* container, const void* other){
    WrapperMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCMapAccess* access = dynamic_cast<NestedPointersRCMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* NestedPointersRCMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(NestedPointersRCMapAccess* access = dynamic_cast<NestedPointersRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* NestedPointersRCMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(NestedPointersRCMapAccess* access = dynamic_cast<NestedPointersRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void NestedPointersRCMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMapAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMapAccess::insert(env, container, key, value);
    unfoldAndAddContainer(env, rcSet(env), key);
    unfoldAndAddContainer(env, rcSet(env), value);
}

jint NestedPointersRCMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMapAccess::remove(env, container, key);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMapAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}

NestedPointersRCMultiMapAccess::NestedPointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), RCSet() {}

NestedPointersRCMultiMapAccess::NestedPointersRCMultiMapAccess(NestedPointersRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), RCSet(other) {}

NestedPointersRCMultiMapAccess* NestedPointersRCMultiMapAccess::clone(){
    return new NestedPointersRCMultiMapAccess(*this);
}

void NestedPointersRCMultiMapAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCMultiMapAccess::assign(void* container, const void* other){
    WrapperMultiMapAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCMultiMapAccess* access = dynamic_cast<NestedPointersRCMultiMapAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* NestedPointersRCMultiMapAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiMapAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(NestedPointersRCMultiMapAccess* access = dynamic_cast<NestedPointersRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* NestedPointersRCMultiMapAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiMapAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(NestedPointersRCMultiMapAccess* access = dynamic_cast<NestedPointersRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void NestedPointersRCMultiMapAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiMapAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCMultiMapAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiMapAccess::insert(env, container, key, value);
    unfoldAndAddContainer(env, rcSet(env), key);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCMultiMapAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiMapAccess::replace(env, container, key, value);
    updateRC(env, container);
}

void NestedPointersRCMultiMapAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

jint NestedPointersRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMultiMapAccess::remove(env, container, key);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jint NestedPointersRCMultiMapAccess::remove(JNIEnv * env, void* container, jobject key, jobject value) {
    jint result = WrapperMultiMapAccess::remove(env, container, key, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCMultiMapAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiMapAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}

NestedPointersRCHashAccess::NestedPointersRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), RCSet() {}

NestedPointersRCHashAccess::NestedPointersRCHashAccess(NestedPointersRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), RCSet(other) {}

NestedPointersRCHashAccess* NestedPointersRCHashAccess::clone(){
    return new NestedPointersRCHashAccess(*this);
}

void NestedPointersRCHashAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCHashAccess::assign(void* container, const void* other){
    WrapperHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCHashAccess* access = dynamic_cast<NestedPointersRCHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* NestedPointersRCHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(NestedPointersRCHashAccess* access = dynamic_cast<NestedPointersRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* NestedPointersRCHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(NestedPointersRCHashAccess* access = dynamic_cast<NestedPointersRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void NestedPointersRCHashAccess::clear(JNIEnv * env, void* container) {
    WrapperHashAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperHashAccess::insert(env, container, key, value);
    unfoldAndAddContainer(env, rcSet(env), key);
    unfoldAndAddContainer(env, rcSet(env), value);
}

jint NestedPointersRCHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jobject NestedPointersRCHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperHashAccess::take(env, container, key);
    removeRC(env, key);
    return result;
}

NestedPointersRCMultiHashAccess::NestedPointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), RCSet() {}

NestedPointersRCMultiHashAccess::NestedPointersRCMultiHashAccess(NestedPointersRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), RCSet(other) {}

NestedPointersRCMultiHashAccess* NestedPointersRCMultiHashAccess::clone(){
    return new NestedPointersRCMultiHashAccess(*this);
}

void NestedPointersRCMultiHashAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::ReferenceUtility$RCSet::newInstance(env);
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCMultiHashAccess::assign(void* container, const void* other){
    WrapperMultiHashAccess::assign(container, other);
    if(JniEnvironment env{500}){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(other)){
            if(NestedPointersRCMultiHashAccess* access = dynamic_cast<NestedPointersRCMultiHashAccess*>(link->containerAccess())){
                if(access!=this){
                    clearRC(env);
                    assignRC(env, *access);
                }
                return;
            }
        }
        clearRC(env);
    }
}

void* NestedPointersRCMultiHashAccess::constructContainer(void* placement, const void* copyOf){
    void* result = WrapperMultiHashAccess::constructContainer(placement, copyOf);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(copyOf)){
            if(NestedPointersRCMultiHashAccess* access = dynamic_cast<NestedPointersRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* NestedPointersRCMultiHashAccess::constructContainer(void* placement, void* moved){
    void* result = WrapperMultiHashAccess::constructContainer(placement, moved);
    if(JniEnvironment env{500}){
        clearRC(env);
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(moved)){
            if(NestedPointersRCMultiHashAccess* access = dynamic_cast<NestedPointersRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                access->clearRC(env);
                break;
            }
        }
    }
    return result;
}
#endif

void NestedPointersRCMultiHashAccess::clear(JNIEnv * env, void* container) {
    WrapperMultiHashAccess::clear(env, container);
    clearRC(env);
}

void NestedPointersRCMultiHashAccess::insert(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiHashAccess::insert(env, container, key, value);
    unfoldAndAddContainer(env, rcSet(env), key);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCMultiHashAccess::replace(JNIEnv * env, void* container, jobject key, jobject value) {
    WrapperMultiHashAccess::replace(env, container, key, value);
    updateRC(env, container);
}

void NestedPointersRCMultiHashAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

jint NestedPointersRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key) {
    jint result = WrapperMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, key);
    }
    return result;
}

jint NestedPointersRCMultiHashAccess::remove(JNIEnv * env, void* container, jobject key, jobject value) {
    jint result = WrapperMultiHashAccess::remove(env, container, key, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCMultiHashAccess::take(JNIEnv *env, void* container, jobject key) {
    jobject result = WrapperMultiHashAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}

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
            if(converter && javaObject && Java::QtCore::QPair::isInstanceOf(env, javaObject->object())){
                jobject first = Java::QtCore::QPair::first(env, javaObject->object());
                jobject second = Java::QtCore::QPair::second(env, javaObject->object());
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
            if(converter && javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, javaObject->object())){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
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
            if(converter && javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, javaObject->object())){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
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
                    if(javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, javaObject->object())){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
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
                    if(javaObject && Java::QtJambi::NativeUtility$Object::isInstanceOf(env, javaObject->object())){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
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
                                        if(javaObject && javaObject->object()){
                                            jobject first = Java::QtCore::QPair::first(env, javaObject->object());
                                            jobject second = Java::QtCore::QPair::second(env, javaObject->object());
                                            pairAccess->setFirst(env, target, first);
                                            pairAccess->setSecond(env, target, second);
                                            return true;
                                        }
                                    }
                                }
                                return false;
                            }
                    , jObjectWrapperType, containerMetaType);
    }
}

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
