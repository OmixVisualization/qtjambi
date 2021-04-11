#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#endif

#include "qtjambi_containeraccess_p.h"
#include "qtjambi_core.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_containers.h"
#include "qtjambi_cast.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambi_registry_p.h"
#include "qtjambilink_p.h"

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
struct qtjambi_access_wrapper{
};

template<>
struct qtjambi_access_wrapper<AbstractListAccess>{
    typedef WrapperListAccess type;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
template<>
struct qtjambi_access_wrapper<AbstractLinkedListAccess>{
    typedef WrapperLinkedListAccess type;
};

template<>
struct qtjambi_access_wrapper<AbstractVectorAccess>{
    typedef WrapperVectorAccess type;
};
#endif

template<>
struct qtjambi_access_wrapper<AbstractSetAccess>{
    typedef WrapperSetAccess type;
};

template<>
struct qtjambi_access_wrapper<AbstractHashAccess>{
    typedef WrapperHashAccess type;
};

template<>
struct qtjambi_access_wrapper<AbstractMapAccess>{
    typedef WrapperMapAccess type;
};

template<>
struct qtjambi_access_wrapper<AbstractMultiHashAccess>{
    typedef WrapperMultiHashAccess type;
};

template<>
struct qtjambi_access_wrapper<AbstractMultiMapAccess>{
    typedef WrapperMultiMapAccess type;
};

template<typename AccessType>
class OwnedAccess : public qtjambi_access_wrapper<AccessType>::type{
    typedef typename qtjambi_access_wrapper<AccessType>::type SuperType;
    OwnerFunctional m_elementOwnerFunction;
public:
    ~OwnedAccess() override = default;
    OwnedAccess(AccessType* containerAccess, OwnerFunctional&& elementOwnerFunction)
        : SuperType(containerAccess), m_elementOwnerFunction(std::move(elementOwnerFunction)) {
        Q_ASSERT(m_elementOwnerFunction);
    }
    OwnedAccess(AccessType* containerAccess, const OwnerFunctional& elementOwnerFunction)
        : SuperType(containerAccess), m_elementOwnerFunction(elementOwnerFunction) {
        Q_ASSERT(m_elementOwnerFunction);
    }

    OwnedAccess<AccessType>* clone() override{
        return new OwnedAccess<AccessType>(SuperType::clone(), m_elementOwnerFunction);
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
class OwnedBiAccess : public qtjambi_access_wrapper<AccessType>::type{
    typedef typename qtjambi_access_wrapper<AccessType>::type SuperType;
    OwnerFunctional m_keyOwnerFunction;
    OwnerFunctional m_valueOwnerFunction;
public:
    ~OwnedBiAccess() override = default;
    OwnedBiAccess(AccessType* containerAccess, OwnerFunctional&& keyOwnerFunction, OwnerFunctional&& valueOwnerFunction)
        : SuperType(containerAccess), m_keyOwnerFunction(std::move(keyOwnerFunction)), m_valueOwnerFunction(std::move(valueOwnerFunction)) {
        Q_ASSERT(m_keyOwnerFunction || m_valueOwnerFunction);
    }
    OwnedBiAccess(AccessType* containerAccess, const OwnerFunctional& keyOwnerFunction, const OwnerFunctional& valueOwnerFunction)
        : SuperType(containerAccess), m_keyOwnerFunction(keyOwnerFunction), m_valueOwnerFunction(valueOwnerFunction) {
        Q_ASSERT(m_keyOwnerFunction || m_valueOwnerFunction);
    }

    OwnedBiAccess<AccessType>* clone() override{
        return new OwnedBiAccess<AccessType>(SuperType::clone(), m_keyOwnerFunction, m_valueOwnerFunction);
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

template<typename AccessType>
AccessType* createOwnedAccess(AccessType* containerAccess, OwnerFunctional&& ownerFunction)
{
    if(ownerFunction)
        return new OwnedAccess<AccessType>(containerAccess, std::move(ownerFunction));
    else
        return containerAccess;
}

template<typename AccessType>
AccessType* createOwnedAccess(AccessType* containerAccess, OwnerFunctional&& keyOwnerFunction, OwnerFunctional&& valueOwnerFunction)
{
    if(keyOwnerFunction || valueOwnerFunction)
        return new OwnedBiAccess<AccessType>(containerAccess, std::move(keyOwnerFunction), std::move(valueOwnerFunction));
    else
        return containerAccess;
}



bool isPointerType(const QMetaType& metaType){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QByteArray name = metaType.name();
    return name.endsWith("*");
#else
    return metaType.flags().testFlag(QMetaType::IsPointer);
#endif
}

AbstractContainerAccess* createContainerAccess(ContainerType containerType, const QMetaType& memberMetaType){
    AbstractContainerAccess* containerAccess = nullptr;
    switch(memberMetaType.id()){
    case QMetaType::UnknownType:
        break;
    case QMetaType::Bool:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<bool>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<bool>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<bool>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<bool>::newInstance();
            break;
        }
        break;
    case QMetaType::Char:
    case QMetaType::SChar:
    case QMetaType::UChar:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint8>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint8>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint8>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint8>::newInstance();
            break;
        }
        break;
    case QMetaType::Short:
    case QMetaType::UShort:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint16>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint16>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint16>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint16>::newInstance();
            break;
        }
        break;
    case QMetaType::Int:
    case QMetaType::UInt:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint32>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint32>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint32>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint32>::newInstance();
            break;
        }
        break;
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<qint64>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<qint64>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<qint64>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<qint64>::newInstance();
            break;
        }
        break;
    case QMetaType::Double:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<double>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<double>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<double>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<double>::newInstance();
            break;
        }
        break;
    case QMetaType::Float:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<float>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<float>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<float>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<float>::newInstance();
            break;
        }
        break;
    case QMetaType::QChar:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<QChar>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QChar>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QChar>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QChar>::newInstance();
            break;
        }
        break;
    case QMetaType::QVariant:
        switch(containerType){
        case ContainerType::QSet:
            // no qhash for qvariant
            // containerAccess = QtJambiPrivate::QSetAccess<QVariant>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QVariant>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QVariant>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QVariant>::newInstance();
            break;
        }
        break;
    case QMetaType::QString:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<QString>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QString>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QString>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QString>::newInstance();
            break;
        }
        break;
    case QMetaType::QObjectStar:
        switch(containerType){
        case ContainerType::QSet:
            containerAccess = QtJambiPrivate::QSetAccess<QObject*>::newInstance();
            break;
        case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        case ContainerType::QVector:
            containerAccess = QtJambiPrivate::QVectorAccess<QObject*>::newInstance();
            break;
        case ContainerType::QLinkedList:
            containerAccess = QtJambiPrivate::QLinkedListAccess<QObject*>::newInstance();
            break;
#endif
        case ContainerType::QQueue:
        case ContainerType::QList:
            containerAccess = QtJambiPrivate::QListAccess<QObject*>::newInstance();
            break;
        }
        break;
    default:{
            bool isPointer = isPointerType(memberMetaType);
            size_t size = 0;
            size_t align = 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            bool isStaticType = true;
#endif
            if(!isPointer){
                size = size_t(memberMetaType.sizeOf());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                const std::type_info* t = getTypeByMetaType(memberMetaType.id());
                if(!t)
                    t = getTypeByQtName(memberMetaType.name());
                if(t){
                    align = getValueAlignment(*t);
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t)){
                        isStaticType = typeInfo->isStatic;
                    }
                }
#else
                align = size_t(memberMetaType.alignOf());
#endif
            }
            QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, memberMetaType.id());
            if(ContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(containerType, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                                 , isStaticType
#else
                                                                                                 , false
#endif
                                                                                                 )){
                containerAccess = accessFactory(memberMetaType,
                                                hashFunction,
                                                InternalToExternalConverter(),
                                                ExternalToInternalConverter());
            }
        }
        break;
    }
    return containerAccess;
}

AbstractContainerAccess* createContainerAccess(MapType mapType, const QMetaType& memberMetaType1, const QMetaType& memberMetaType2){
    bool isPointer1 = isPointerType(memberMetaType1);
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
#else
        align1 = size_t(memberMetaType1.alignOf());
#endif
    }
    bool isPointer2 = isPointerType(memberMetaType2);
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
#else
        align2 = size_t(memberMetaType2.alignOf());
#endif
    }
    AbstractContainerAccess* containerAccess = nullptr;
    if(BiContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(mapType, align1, size1, align2, size2)){
        QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, memberMetaType1.id());
        QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, memberMetaType2.id());
        containerAccess = accessFactory( memberMetaType1,
                                         hashFunction1,
                                         InternalToExternalConverter(),
                                         ExternalToInternalConverter(),
                                         memberMetaType2,
                                         hashFunction2,
                                         InternalToExternalConverter(),
                                         ExternalToInternalConverter());
    }
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

TypeAnalysisResult analyzeType(const QMetaType& metaType, const QByteArray& typeName){
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
        TypeAnalysisResult result = analyzeType(instantiationMetaType, instantiation);
        hasPointer = result.hasPointers;
        if(result.ownerFunctional){
            QSharedPointer<AbstractContainerAccess> sharedAccess;
            if(typeName.startsWith("QList<")
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    || typeName.startsWith("QStack<")
#endif
                    || typeName.startsWith("QQueue<")){
                if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(createContainerAccess(ContainerType::QList, instantiationMetaType))){
                    sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result.ownerFunctional)), &containerDisposer);
                }
            }else if(typeName.startsWith("QSet<")){
                if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(createContainerAccess(ContainerType::QSet, instantiationMetaType))){
                    sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result.ownerFunctional)), &containerDisposer);
                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            }else if(typeName.startsWith("QLinkedList<")){
                if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(createContainerAccess(ContainerType::QLinkedList, instantiationMetaType))){
                    sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result.ownerFunctional)), &containerDisposer);
                }
            }else if(typeName.startsWith("QVector<")
                     || typeName.startsWith("QStack<")){
                if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(createContainerAccess(ContainerType::QVector, instantiationMetaType))){
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
                TypeAnalysisResult result1 = analyzeType(instantiationMetaType1, instantiations[0]);
                TypeAnalysisResult result2 = analyzeType(instantiationMetaType2, instantiations[1]);
                hasPointer = result1.hasPointers || result2.hasPointers;
                if(result1.ownerFunctional || result2.ownerFunctional){
                    QSharedPointer<AbstractContainerAccess> sharedAccess;
                    if(typeName.startsWith("QMap<")){
                        if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(createContainerAccess(MapType::QMap, instantiationMetaType1, instantiationMetaType2))){
                            sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional)), &containerDisposer);
                        }
                    }else if(typeName.startsWith("QHash<")){
                        if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(createContainerAccess(MapType::QHash, instantiationMetaType1, instantiationMetaType2))){
                            sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional)), &containerDisposer);
                        }
                    }else if(typeName.startsWith("QMultiMap<")){
                        if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(createContainerAccess(MapType::QMultiMap, instantiationMetaType1, instantiationMetaType2))){
                            sharedAccess.reset(createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional)), &containerDisposer);
                        }
                    }else if(typeName.startsWith("QMultiHash<")){
                        if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(createContainerAccess(MapType::QMultiHash, instantiationMetaType1, instantiationMetaType2))){
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
                QString typeName = qtjambi_type_name(*typeId);
                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                    hasPointer = true;
                }
            }else{
                if(metaType.isValid()){
                    hasPointer = isPointerType(metaType);
                }else{
                    hasPointer = typeName.endsWith("*");
                }
                if(PtrOwnerFunction elementOwnerFunction = registeredOwnerFunction(*typeId)){
                    return TypeAnalysisResult(hasPointer, elementOwnerFunction);
                }
            }
        }else{
            hasPointer = typeName.endsWith("*");
        }
    }
    return TypeAnalysisResult(hasPointer);
}

AbstractListAccess* checkContainerAccess(JNIEnv * env, AbstractListAccess* containerAccess){
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperListAccess*>(containerAccess)){
        if(containerAccess->elementMetaType().id() >= QMetaType::HighestInternalId
                || containerAccess->elementMetaType().id() == QMetaType::QObjectStar
                || containerAccess->elementMetaType().id() == QMetaType::VoidStar){
            if(isPointerType(containerAccess->elementMetaType())){
                containerAccess = new PointerRCListAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
                if(result.hasPointers)
                    containerAccess = new NestedPointersRCListAccess(containerAccess);
                containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional));
            }
        }
    }
    return containerAccess;
}

AbstractSetAccess* checkContainerAccess(JNIEnv * env, AbstractSetAccess* containerAccess){
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperSetAccess*>(containerAccess)){
        int id = containerAccess->elementMetaType().id();
        if(id != QMetaType::QPoint
            && id != QMetaType::QPointF
            && id != QMetaType::QString){
            if(isPointerType(containerAccess->elementMetaType())){
                containerAccess = new PointerRCSetAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
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
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperLinkedListAccess*>(containerAccess)){
        if(isPointerType(containerAccess->elementMetaType())){
            containerAccess = new PointerRCLinkedListAccess(containerAccess);
        }else{
            TypeAnalysisResult result = analyzeType(containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
            if(result.hasPointers)
                containerAccess = new NestedPointersRCLinkedListAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractVectorAccess* checkContainerAccess(JNIEnv * env, AbstractVectorAccess* containerAccess){
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperVectorAccess*>(containerAccess)){
        if(isPointerType(containerAccess->elementMetaType())){
            containerAccess = new PointerRCVectorAccess(containerAccess);
        }else{
            TypeAnalysisResult result = analyzeType(containerAccess->elementMetaType(), containerAccess->elementMetaType().name());
            if(result.hasPointers)
                containerAccess = new NestedPointersRCVectorAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional));
        }
    }
    return containerAccess;
}
#endif
AbstractHashAccess* checkContainerAccess(JNIEnv * env, AbstractHashAccess* containerAccess){
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperHashAccess*>(containerAccess)){
        if(isPointerType(containerAccess->keyMetaType())){
            if(isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCHashAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCHashAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCHashAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCHashAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCHashAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
            if(result1.hasPointers || result2.hasPointers)
                containerAccess = new NestedPointersRCHashAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractMapAccess* checkContainerAccess(JNIEnv * env, AbstractMapAccess* containerAccess){
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperMapAccess*>(containerAccess)){
        if(isPointerType(containerAccess->keyMetaType())){
            if(isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCMapAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCMapAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMapAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCMapAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCMapAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
            if(result1.hasPointers || result2.hasPointers)
                containerAccess = new NestedPointersRCMapAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractMultiHashAccess* checkContainerAccess(JNIEnv * env, AbstractMultiHashAccess* containerAccess){
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperMultiHashAccess*>(containerAccess)){
        if(isPointerType(containerAccess->keyMetaType())){
            if(isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCMultiHashAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMultiHashAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCMultiHashAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
            if(result1.hasPointers || result2.hasPointers)
                containerAccess = new NestedPointersRCMultiHashAccess(containerAccess);
            containerAccess = createOwnedAccess(containerAccess, std::move(result1.ownerFunctional), std::move(result2.ownerFunctional));
        }
    }
    return containerAccess;
}

AbstractMultiMapAccess* checkContainerAccess(JNIEnv * env, AbstractMultiMapAccess* containerAccess){
    Q_UNUSED(env)
    Q_ASSERT(containerAccess);
    if(!dynamic_cast<WrapperMultiMapAccess*>(containerAccess)){
        if(isPointerType(containerAccess->keyMetaType())){
            if(isPointerType(containerAccess->valueMetaType())){
                containerAccess = new PointersRCMultiMapAccess(containerAccess);
            }else{
                TypeAnalysisResult result = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
                if(result.hasPointers){
                    containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
                }else{
                    containerAccess = new KeyPointerRCMultiMapAccess(containerAccess);
                }
                containerAccess = createOwnedAccess(containerAccess, OwnerFunctional(), std::move(result.ownerFunctional));
            }
        }else if(isPointerType(containerAccess->valueMetaType())){
            TypeAnalysisResult result = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            if(result.hasPointers){
                containerAccess = new NestedPointersRCMultiMapAccess(containerAccess);
            }else{
                containerAccess = new ValuePointerRCMultiMapAccess(containerAccess);
            }
            containerAccess = createOwnedAccess(containerAccess, std::move(result.ownerFunctional), OwnerFunctional());
        }else{
            TypeAnalysisResult result1 = analyzeType(containerAccess->keyMetaType(), containerAccess->keyMetaType().name());
            TypeAnalysisResult result2 = analyzeType(containerAccess->valueMetaType(), containerAccess->valueMetaType().name());
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
const QObject* AbstractContainerAccess::getOwner(const void*){ return nullptr; }

RCSet::RCSet(const RCSet& other)
      : RCSet() {
    if(other.m_rcSet.object()){
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 500)
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
        m_rcSet = JObjectWrapper(env, Java::QtJambi::QtJambiInternal$RCSet::newInstance(env));
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
        m_rcMap = JObjectWrapper(env, Java::QtJambi::QtJambiInternal$RCMap::newInstance(env));
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
        m_rcMap = JObjectWrapper(env, Java::QtJambi::QtJambiInternal$RCMap::newInstance(env));
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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* PointerRCListAccess::copyContainer(const void* container){
    void* result = WrapperListAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

void PointerRCListAccess::append(JNIEnv * env, void* container, jobject value) {
    WrapperListAccess::append(env, container, value);
    if(value)
        addRC(env, value);
}

void PointerRCListAccess::appendList(JNIEnv * env, void* container, jobject list) {
    WrapperListAccess::appendList(env, container, list);
    if(list)
        addAllRC(env, list);
}

jobject PointerRCListAccess::takeAt(JNIEnv * env, void* container, jint index) {
    jobject result = WrapperListAccess::takeAt(env, container, index);
    if(result && !WrapperListAccess::contains(env, container, result))
        removeRC(env, result);
    return result;
}

void PointerRCListAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    jobject oldValue = WrapperListAccess::at(env, container, index);
    WrapperListAccess::replace(env, container, index, value);
    if(oldValue && !WrapperListAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
    if(value)
        addRC(env, value);
}

jboolean PointerRCListAccess::removeOne(JNIEnv * env, void* container, jobject value) {
    jboolean result = WrapperListAccess::removeOne(env, container, value);
    if(result && !WrapperListAccess::contains(env, container, value)){
        removeRC(env, value);
    }
    return result;
}

void PointerRCListAccess::removeAt(JNIEnv * env, void* container, jint index) {
    jobject oldValue = WrapperListAccess::at(env, container, index);
    WrapperListAccess::removeAt(env, container, index);
    if(!WrapperListAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
}

jint PointerRCListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperListAccess::removeAll(env, container, value);
    if(result>0){
        removeRC(env, value);
    }
    return result;
}

void PointerRCListAccess::prepend(JNIEnv * env, void* container, jobject value) {
    WrapperListAccess::prepend(env, container, value);
    if(value)
        addRC(env, value);
}

jobject PointerRCListAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperListAccess::mid(env, container, index1, index2);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(result)){
        if(PointerRCListAccess* access = dynamic_cast<PointerRCListAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void PointerRCListAccess::insert(JNIEnv * env, void* container, jint index, jobject value) {
    WrapperListAccess::insert(env, container, index, value);
    if(value)
        addRC(env, value);
}

void PointerRCListAccess::clear(JNIEnv * env, void* container) {
    WrapperListAccess::clear(env, container);
    clearRC(env);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void PointerRCListAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
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

void PointerRCListAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    WrapperListAccess::insert(env, container, index, n, value);
    addRC(env, value);
}

void PointerRCListAccess::fill(JNIEnv * env, void* container, jobject value, jint size){
    jint oldSize = WrapperListAccess::size(env, container);
    WrapperListAccess::fill(env, container, value, size);
    if(value && oldSize<size)
        addRC(env, value);
}
#endif

PointerRCSetAccess::PointerRCSetAccess(AbstractSetAccess* containerAccess)
    : WrapperSetAccess(containerAccess), RCSet() {}

PointerRCSetAccess::PointerRCSetAccess(PointerRCSetAccess& other)
    : WrapperSetAccess(other.WrapperSetAccess::clone()), RCSet(other) {}

PointerRCSetAccess* PointerRCSetAccess::clone(){
    return new PointerRCSetAccess(*this);
}

void PointerRCSetAccess::assign(void* container, const void* other){
    WrapperSetAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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



void* PointerRCSetAccess::copyContainer(const void* container){
    void* result = WrapperSetAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(PointerRCSetAccess* access = dynamic_cast<PointerRCSetAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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

PointerRCLinkedListAccess::PointerRCLinkedListAccess(AbstractLinkedListAccess* containerAccess)
    : WrapperLinkedListAccess(containerAccess), RCSet() {}

PointerRCLinkedListAccess::PointerRCLinkedListAccess(PointerRCLinkedListAccess& other)
    : WrapperLinkedListAccess(other.WrapperLinkedListAccess::clone()), RCSet(other) {}

PointerRCLinkedListAccess* PointerRCLinkedListAccess::clone(){
    return new PointerRCLinkedListAccess(*this);
}

void PointerRCLinkedListAccess::assign(void* container, const void* other){
    WrapperLinkedListAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* PointerRCLinkedListAccess::copyContainer(const void* container){
    void* result = WrapperLinkedListAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
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

PointerRCVectorAccess::PointerRCVectorAccess(AbstractVectorAccess* containerAccess)
    : WrapperVectorAccess(containerAccess), RCSet() {}

PointerRCVectorAccess::PointerRCVectorAccess(PointerRCVectorAccess& other)
    : WrapperVectorAccess(other.WrapperVectorAccess::clone()), RCSet(other) {}

PointerRCVectorAccess* PointerRCVectorAccess::clone(){
    return new PointerRCVectorAccess(*this);
}

void* PointerRCVectorAccess::copyContainer(const void* container){
    void* result = WrapperVectorAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void PointerRCVectorAccess::append(JNIEnv * env, void* container, jobject value) {
    WrapperVectorAccess::append(env, container, value);
    if(value)
        addRC(env, value);
}

void PointerRCVectorAccess::appendVector(JNIEnv * env, void* container, jobject list) {
    WrapperVectorAccess::appendVector(env, container, list);
    if(list)
        addAllRC(env, list);
}

jobject PointerRCVectorAccess::takeAt(JNIEnv * env, void* container, jint index) {
    jobject result = WrapperVectorAccess::takeAt(env, container, index);
    if(result && !WrapperVectorAccess::contains(env, container, result))
        removeRC(env, result);
    return result;
}

void PointerRCVectorAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    jobject oldValue = WrapperVectorAccess::at(env, container, index);
    WrapperVectorAccess::replace(env, container, index, value);
    if(oldValue && !WrapperVectorAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
    if(value)
        addRC(env, value);
}

jboolean PointerRCVectorAccess::removeOne(JNIEnv * env, void* container, jobject value) {
    jboolean result = WrapperVectorAccess::removeOne(env, container, value);
    if(result && !WrapperVectorAccess::contains(env, container, value)){
        removeRC(env, value);
    }
    return result;
}

void PointerRCVectorAccess::removeAt(JNIEnv * env, void* container, jint index) {
    jobject oldValue = WrapperVectorAccess::at(env, container, index);
    WrapperVectorAccess::removeAt(env, container, index);
    if(!WrapperVectorAccess::contains(env, container, oldValue))
        removeRC(env, oldValue);
}

jint PointerRCVectorAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperVectorAccess::removeAll(env, container, value);
    if(result>0){
        removeRC(env, value);
    }
    return result;
}

void PointerRCVectorAccess::prepend(JNIEnv * env, void* container, jobject value) {
    WrapperVectorAccess::prepend(env, container, value);
    if(value)
        addRC(env, value);
}

jobject PointerRCVectorAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperVectorAccess::mid(env, container, index1, index2);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(result)){
        if(PointerRCVectorAccess* access = dynamic_cast<PointerRCVectorAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void PointerRCVectorAccess::insert(JNIEnv * env, void* container, jint index, jobject value) {
    WrapperVectorAccess::insert(env, container, index, value);
    if(value)
        addRC(env, value);
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

KeyPointerRCMapAccess::KeyPointerRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), RCSet() {}

KeyPointerRCMapAccess::KeyPointerRCMapAccess(KeyPointerRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), RCSet(other) {}

KeyPointerRCMapAccess* KeyPointerRCMapAccess::clone(){
    return new KeyPointerRCMapAccess(*this);
}

void KeyPointerRCMapAccess::assign(void* container, const void* other){
    WrapperMapAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* KeyPointerRCMapAccess::copyContainer(const void* container){
    void* result = WrapperMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(KeyPointerRCMapAccess* access = dynamic_cast<KeyPointerRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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

ValuePointerRCMapAccess::ValuePointerRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), RCSet() {}

ValuePointerRCMapAccess::ValuePointerRCMapAccess(ValuePointerRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), RCSet(other) {}

ValuePointerRCMapAccess* ValuePointerRCMapAccess::clone(){
    return new ValuePointerRCMapAccess(*this);
}

void ValuePointerRCMapAccess::assign(void* container, const void* other){
    WrapperMapAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* ValuePointerRCMapAccess::copyContainer(const void* container){
    void* result = WrapperMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(ValuePointerRCMapAccess* access = dynamic_cast<ValuePointerRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* PointersRCMapAccess::copyContainer(const void* container){
    void* result = WrapperMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(PointersRCMapAccess* access = dynamic_cast<PointersRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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

KeyPointerRCMultiMapAccess::KeyPointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess)
    : WrapperMultiMapAccess(containerAccess), RCSet() {}

KeyPointerRCMultiMapAccess::KeyPointerRCMultiMapAccess(KeyPointerRCMultiMapAccess& other)
    : WrapperMultiMapAccess(other.WrapperMultiMapAccess::clone()), RCSet(other) {}

KeyPointerRCMultiMapAccess* KeyPointerRCMultiMapAccess::clone(){
    return new KeyPointerRCMultiMapAccess(*this);
}

void KeyPointerRCMultiMapAccess::assign(void* container, const void* other){
    WrapperMultiMapAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* KeyPointerRCMultiMapAccess::copyContainer(const void* container){
    void* result = WrapperMultiMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(KeyPointerRCMultiMapAccess* access = dynamic_cast<KeyPointerRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
        jobject iter = qtjambi_map_entryset_iterator(env, other);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            addRC(env, Java::Runtime::Map$Entry::getKey(env, entry));
        }
    }
}

void KeyPointerRCMultiHashAccess::unite(JNIEnv * env, void* container, jobject other) {
    WrapperMultiHashAccess::unite(env, container, other);
    if(Java::Runtime::Map::isInstanceOf(env, other)){
        jobject iter = qtjambi_map_entryset_iterator(env, other);
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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* ValuePointerRCMultiMapAccess::copyContainer(const void* container){
    void* result = WrapperMultiMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(ValuePointerRCMultiMapAccess* access = dynamic_cast<ValuePointerRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
        jobject iter = qtjambi_map_entryset_iterator(env, other);
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
        jobject iter = qtjambi_map_entryset_iterator(env, other);
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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* PointersRCMultiMapAccess::copyContainer(const void* container){
    void* result = WrapperMultiMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(PointersRCMultiMapAccess* access = dynamic_cast<PointersRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
        jobject iter = qtjambi_map_entryset_iterator(env, other);
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


KeyPointerRCHashAccess::KeyPointerRCHashAccess(AbstractHashAccess* containerAccess)
    : WrapperHashAccess(containerAccess), RCSet() {}

KeyPointerRCHashAccess::KeyPointerRCHashAccess(KeyPointerRCHashAccess& other)
    : WrapperHashAccess(other.WrapperHashAccess::clone()), RCSet(other) {}

KeyPointerRCHashAccess* KeyPointerRCHashAccess::clone(){
    return new KeyPointerRCHashAccess(*this);
}

void KeyPointerRCHashAccess::assign(void* container, const void* other){
    WrapperHashAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* KeyPointerRCHashAccess::copyContainer(const void* container){
    void* result = WrapperHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(KeyPointerRCHashAccess* access = dynamic_cast<KeyPointerRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* ValuePointerRCHashAccess::copyContainer(const void* container){
    void* result = WrapperHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(ValuePointerRCHashAccess* access = dynamic_cast<ValuePointerRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* PointersRCHashAccess::copyContainer(const void* container){
    void* result = WrapperHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(PointersRCHashAccess* access = dynamic_cast<PointersRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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

KeyPointerRCMultiHashAccess::KeyPointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess)
    : WrapperMultiHashAccess(containerAccess), RCSet() {}

KeyPointerRCMultiHashAccess::KeyPointerRCMultiHashAccess(KeyPointerRCMultiHashAccess& other)
    : WrapperMultiHashAccess(other.WrapperMultiHashAccess::clone()), RCSet(other) {}

KeyPointerRCMultiHashAccess* KeyPointerRCMultiHashAccess::clone(){
    return new KeyPointerRCMultiHashAccess(*this);
}

void KeyPointerRCMultiHashAccess::assign(void* container, const void* other){
    WrapperMultiHashAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* KeyPointerRCMultiHashAccess::copyContainer(const void* container){
    void* result = WrapperMultiHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(KeyPointerRCMultiHashAccess* access = dynamic_cast<KeyPointerRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* ValuePointerRCMultiHashAccess::copyContainer(const void* container){
    void* result = WrapperMultiHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(ValuePointerRCMultiHashAccess* access = dynamic_cast<ValuePointerRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* PointersRCMultiHashAccess::copyContainer(const void* container){
    void* result = WrapperMultiHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(PointersRCMultiHashAccess* access = dynamic_cast<PointersRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
        jobject iter = qtjambi_map_entryset_iterator(env, other);
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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* NestedPointersRCListAccess::copyContainer(const void* container){
    void* result = WrapperListAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(NestedPointersRCListAccess* access = dynamic_cast<NestedPointersRCListAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

void unfoldAndAddContainer(JNIEnv * env, jobject set, jobject value){
    if(Java::Runtime::Collection::isInstanceOf(env, value)){
        jobject iter = qtjambi_collection_iterator(env, value);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            unfoldAndAddContainer(env, set, Java::Runtime::Iterator::next(env, iter));
        }
    }else if(Java::Runtime::Map::isInstanceOf(env, value)){
        jobject iter = qtjambi_map_entryset_iterator(env, value);
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

void NestedPointersRCListAccess::append(JNIEnv * env, void* container, jobject value) {
    WrapperListAccess::append(env, container, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCListAccess::appendList(JNIEnv * env, void* container, jobject list) {
    WrapperListAccess::appendList(env, container, list);
    unfoldAndAddContainer(env, rcSet(env), list);
}

void NestedPointersRCListAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    jint size = WrapperListAccess::size(env, container);
    for(jint i=0; i<size; ++i){
        jobject value = WrapperListAccess::at(env, container, i);
        unfoldAndAddContainer(env, set, value);
    }
    clearRC(env);
    addAllRC(env, set);
}

jobject NestedPointersRCListAccess::takeAt(JNIEnv * env, void* container, jint index) {
    jobject result = WrapperListAccess::takeAt(env, container, index);
    updateRC(env, container);
    return result;
}

void NestedPointersRCListAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    WrapperListAccess::replace(env, container, index, value);
    updateRC(env, container);
}

jboolean NestedPointersRCListAccess::removeOne(JNIEnv * env, void* container, jobject value) {
    jboolean result = WrapperListAccess::removeOne(env, container, value);
    if(result){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCListAccess::removeAt(JNIEnv * env, void* container, jint index) {
    WrapperListAccess::removeAt(env, container, index);
    updateRC(env, container);
}

jint NestedPointersRCListAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperListAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCListAccess::prepend(JNIEnv * env, void* container, jobject value) {
    WrapperListAccess::prepend(env, container, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

jobject NestedPointersRCListAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperListAccess::mid(env, container, index1, index2);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(result)){
        if(NestedPointersRCListAccess* access = dynamic_cast<NestedPointersRCListAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void NestedPointersRCListAccess::insert(JNIEnv * env, void* container, jint index, jobject value) {
    WrapperListAccess::insert(env, container, index, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCListAccess::clear(JNIEnv * env, void* container) {
    WrapperListAccess::clear(env, container);
    clearRC(env);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void NestedPointersRCListAccess::remove(JNIEnv * env, void* container, jint index, jint n) {
    WrapperListAccess::remove(env, container, index, n);
    updateRC(env, container);
}

void NestedPointersRCListAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value) {
    WrapperListAccess::insert(env, container, index, n, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

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
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCSetAccess::assign(void* container, const void* other){
    WrapperSetAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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



void* NestedPointersRCSetAccess::copyContainer(const void* container){
    void* result = WrapperSetAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(NestedPointersRCSetAccess* access = dynamic_cast<NestedPointersRCSetAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCLinkedListAccess::assign(void* container, const void* other){
    WrapperLinkedListAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* NestedPointersRCLinkedListAccess::copyContainer(const void* container){
    void* result = WrapperLinkedListAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
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
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void* NestedPointersRCVectorAccess::copyContainer(const void* container){
    void* result = WrapperVectorAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
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
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void NestedPointersRCVectorAccess::append(JNIEnv * env, void* container, jobject value) {
    WrapperVectorAccess::append(env, container, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

void NestedPointersRCVectorAccess::appendVector(JNIEnv * env, void* container, jobject list) {
    WrapperVectorAccess::appendVector(env, container, list);
    unfoldAndAddContainer(env, rcSet(env), list);
}

jobject NestedPointersRCVectorAccess::takeAt(JNIEnv * env, void* container, jint index) {
    jobject result = WrapperVectorAccess::takeAt(env, container, index);
    updateRC(env, container);
    return result;
}

void NestedPointersRCVectorAccess::replace(JNIEnv * env, void* container, jint index, jobject value) {
    WrapperVectorAccess::replace(env, container, index, value);
    updateRC(env, container);
}

jboolean NestedPointersRCVectorAccess::removeOne(JNIEnv * env, void* container, jobject value) {
    jboolean result = WrapperVectorAccess::removeOne(env, container, value);
    if(result){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCVectorAccess::removeAt(JNIEnv * env, void* container, jint index) {
    WrapperVectorAccess::removeAt(env, container, index);
    updateRC(env, container);
}

jint NestedPointersRCVectorAccess::removeAll(JNIEnv * env, void* container, jobject value) {
    jint result = WrapperVectorAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCVectorAccess::prepend(JNIEnv * env, void* container, jobject value) {
    WrapperVectorAccess::prepend(env, container, value);
    unfoldAndAddContainer(env, rcSet(env), value);
}

jobject NestedPointersRCVectorAccess::mid(JNIEnv * env, const void* container, jint index1, jint index2) {
    jobject result = WrapperVectorAccess::mid(env, container, index1, index2);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(result)){
        if(NestedPointersRCVectorAccess* access = dynamic_cast<NestedPointersRCVectorAccess*>(link->containerAccess())){
            access->assignRC(env, *this);
            break;
        }
    }
    return result;
}

void NestedPointersRCVectorAccess::insert(JNIEnv * env, void* container, jint index, jobject value) {
    WrapperVectorAccess::insert(env, container, index, value);
    unfoldAndAddContainer(env, rcSet(env), value);
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

#endif


NestedPointersRCMapAccess::NestedPointersRCMapAccess(AbstractMapAccess* containerAccess)
    : WrapperMapAccess(containerAccess), RCSet() {}

NestedPointersRCMapAccess::NestedPointersRCMapAccess(NestedPointersRCMapAccess& other)
    : WrapperMapAccess(other.WrapperMapAccess::clone()), RCSet(other) {}

NestedPointersRCMapAccess* NestedPointersRCMapAccess::clone(){
    return new NestedPointersRCMapAccess(*this);
}

void NestedPointersRCMapAccess::updateRC(JNIEnv * env, const void* container){
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCMapAccess::assign(void* container, const void* other){
    WrapperMapAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* NestedPointersRCMapAccess::copyContainer(const void* container){
    void* result = WrapperMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(NestedPointersRCMapAccess* access = dynamic_cast<NestedPointersRCMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCMultiMapAccess::assign(void* container, const void* other){
    WrapperMultiMapAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* NestedPointersRCMultiMapAccess::copyContainer(const void* container){
    void* result = WrapperMultiMapAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(NestedPointersRCMultiMapAccess* access = dynamic_cast<NestedPointersRCMultiMapAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCHashAccess::assign(void* container, const void* other){
    WrapperHashAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* NestedPointersRCHashAccess::copyContainer(const void* container){
    void* result = WrapperHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(NestedPointersRCHashAccess* access = dynamic_cast<NestedPointersRCHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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
    jobject set = Java::QtJambi::QtJambiInternal$RCSet::newInstance(env);
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
        if(link->containerAccess()==this){
            unfoldAndAddContainer(env, set, link->getJavaObjectLocalRef(env));
        }
    }
    clearRC(env);
    addAllRC(env, set);
}

void NestedPointersRCMultiHashAccess::assign(void* container, const void* other){
    WrapperMultiHashAccess::assign(container, other);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(other)){
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

void* NestedPointersRCMultiHashAccess::copyContainer(const void* container){
    void* result = WrapperMultiHashAccess::copyContainer(container);
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
        clearRC(env);
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(container)){
            if(NestedPointersRCMultiHashAccess* access = dynamic_cast<NestedPointersRCMultiHashAccess*>(link->containerAccess())){
                assignRC(env, *access);
                break;
            }
        }
    }
    return result;
}

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

struct ContainerConverter;
typedef QVector<QSharedPointer<ContainerConverter>> ConverterList;
Q_GLOBAL_STATIC(ConverterList, gConverterList)
struct BiContainerConverter;
typedef QVector<QSharedPointer<BiContainerConverter>> BiConverterList;
Q_GLOBAL_STATIC(BiConverterList, gBiConverterList)
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
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 500)
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

void qtjambi_register_container_converter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType){
    if(!QMetaType::hasRegisteredConverterFunction(qMetaTypeId<JObjectWrapper>(), containerMetaType.id())){
        qtjambi_register_converter(new PairContainerConverter(pairAccess), qMetaTypeId<JObjectWrapper>(), containerMetaType.id());
    }
}

struct ContainerConverter : QtPrivate::AbstractConverterFunction{
    explicit ContainerConverter(ContainerType containerType, const QMetaType& metaType)
        : QtPrivate::AbstractConverterFunction(convert),
          m_containerType(containerType),
          m_elementMetaType(metaType.id()
    ){
        gConverterList->append(QSharedPointer<ContainerConverter>(this));
    }

    static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
    {
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 500)
            const ContainerConverter* converter = static_cast<const ContainerConverter *>(_this);
            const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(in);
            if(converter && javaObject && javaObject->object()){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
                    if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                        switch(converter->m_containerType){
                        case ContainerType::QStack:
                        case ContainerType::QVector:
                            if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(_containerAccess)){
                                if(converter->m_elementMetaType==containerAccess->elementMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case ContainerType::QLinkedList:
                            if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(_containerAccess)){
                                if(converter->m_elementMetaType==containerAccess->elementMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case ContainerType::QQueue:
                        case ContainerType::QList:
                            if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(_containerAccess)){
                                if(converter->m_elementMetaType==containerAccess->elementMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case ContainerType::QSet:
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

    ContainerType m_containerType;
    QMetaType m_elementMetaType;
};

void qtjambi_register_container_converter(ContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& elementMetaType){
    if(!QMetaType::hasRegisteredConverterFunction(qMetaTypeId<JObjectWrapper>(), containerMetaType.id())){
        ContainerConverter* converter = new ContainerConverter(collectionType, elementMetaType);
        qtjambi_register_converter(converter, qMetaTypeId<JObjectWrapper>(), containerMetaType.id());
        qtjambi_register_converter(converter, qMetaTypeId<JCollectionWrapper>(), containerMetaType.id());
    }
}

struct BiContainerConverter : QtPrivate::AbstractConverterFunction{
    explicit BiContainerConverter(MapType containerType, const QMetaType& keyMetaType, const QMetaType& valueMetaType)
        : QtPrivate::AbstractConverterFunction(convert),
          m_containerType(containerType),
          m_keyMetaType(keyMetaType.id()),
          m_valueMetaType(valueMetaType.id())
    {
        gBiConverterList->append(QSharedPointer<BiContainerConverter>(this));
    }

    static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
    {
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 500)
            const BiContainerConverter* converter = static_cast<const BiContainerConverter *>(_this);
            const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(in);
            if(converter && javaObject && javaObject->object()){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
                    if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                        switch(converter->m_containerType){
                        case MapType::QMap:
                            if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(_containerAccess)){
                                if(converter->m_keyMetaType==containerAccess->keyMetaType()
                                        && converter->m_valueMetaType==containerAccess->valueMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case MapType::QMultiMap:
                            if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(_containerAccess)){
                                if(converter->m_keyMetaType==containerAccess->keyMetaType()
                                        && converter->m_valueMetaType==containerAccess->valueMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case MapType::QHash:
                            if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(_containerAccess)){
                                if(converter->m_keyMetaType==containerAccess->keyMetaType()
                                        && converter->m_valueMetaType==containerAccess->valueMetaType()){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }
                            }
                            break;
                        case MapType::QMultiHash:
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

    MapType m_containerType;
    QMetaType m_keyMetaType;
    QMetaType m_valueMetaType;
};

void qtjambi_register_container_converter(MapType mapType, const QMetaType& containerMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType){
    if(!QMetaType::hasRegisteredConverterFunction(qMetaTypeId<JObjectWrapper>(), containerMetaType.id())){
        BiContainerConverter* converter = new BiContainerConverter(mapType, keyMetaType, valueMetaType);
        qtjambi_register_converter(converter, qMetaTypeId<JObjectWrapper>(), containerMetaType.id());
        qtjambi_register_converter(converter, qMetaTypeId<JCollectionWrapper>(), containerMetaType.id());
    }
}

#else

void qtjambi_register_container_converter(ContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& _elementMetaType){
    QMetaType jCollectionWrapperType = QMetaType::fromType<JCollectionWrapper>();
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    QMetaType elementMetaType = _elementMetaType;
    if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
            || !QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        QMetaType::ConverterFunction converter = [collectionType,elementMetaType](const void *src, void *target) -> bool {
            if(src){
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                    const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(src);
                    if(javaObject && javaObject->object()){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
                            if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                switch(collectionType){
                                case ContainerType::QStack:
                                case ContainerType::QQueue:
                                case ContainerType::QList:
                                    if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case ContainerType::QSet:
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

void qtjambi_register_container_converter(MapType mapType, const QMetaType& containerMetaType, const QMetaType& _keyMetaType, const QMetaType& _valueMetaType){
    QMetaType jCollectionWrapperType = QMetaType::fromType<JCollectionWrapper>();
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    QMetaType keyMetaType = _keyMetaType;
    QMetaType valueMetaType = _valueMetaType;
    if(!QMetaType::hasRegisteredConverterFunction(jCollectionWrapperType, containerMetaType)
            || !QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        QMetaType::ConverterFunction converter = [mapType,keyMetaType,valueMetaType](const void *src, void *target) -> bool {
            if(src){
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                    const JObjectWrapper* javaObject = reinterpret_cast<const JObjectWrapper*>(src);
                    if(javaObject && javaObject->object()){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaObject->object())){
                            if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                switch(mapType){
                                case MapType::QMap:
                                    if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(_containerAccess)){
                                        if(keyMetaType==containerAccess->keyMetaType() && valueMetaType==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case MapType::QHash:
                                    if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(_containerAccess)){
                                        if(keyMetaType==containerAccess->keyMetaType() && valueMetaType==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case MapType::QMultiMap:
                                    if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(_containerAccess)){
                                        if(keyMetaType==containerAccess->keyMetaType() && valueMetaType==containerAccess->valueMetaType()){
                                            containerAccess->assign(target, link->pointer());
                                            return true;
                                        }
                                    }
                                    break;
                                case MapType::QMultiHash:
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

void qtjambi_register_container_converter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType){
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, containerMetaType)){
        QMetaType::registerConverterFunction(
                    [pairAccess](const void *src, void *target) -> bool {
                                if(src){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
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
