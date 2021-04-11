#ifndef QTJAMBI_CONTAINERACCESS_H
#define QTJAMBI_CONTAINERACCESS_H

#include <QtCore/QList>
#include <QtCore/QTypeInfo>
#include <QtCore/QDebug>
#include "qtjambi_core.h"
#include "qtjambi_typetests.h"
#include "qtjambi_containers.h"
#include "qtjambi_templates.h"
#include "qtjambi_registry.h"
#include "qtjambi_utils.h"
#include "qtjambi_cast_container_util_p.h"

QTJAMBI_EXPORT jobject qtjambi_from_QSet(JNIEnv *__jni_env,
                                     const void* listPtr,
                                     AbstractContainerAccess* setAccess);

QTJAMBI_EXPORT jobject qtjambi_from_QList(JNIEnv *__jni_env,
                                     const void* listPtr,
                                     AbstractContainerAccess* setAccess);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QTJAMBI_EXPORT jobject qtjambi_from_QVector(JNIEnv *__jni_env,
                                     const void* listPtr,
                                     AbstractContainerAccess* setAccess);
#endif

typedef AbstractContainerAccess*(*ContainerAccessFactory)(const QMetaType& metaType,
                                                          const QHashFunction& hashFunction,
                                                          const InternalToExternalConverter& internalToExternalConverter,
                                                          const ExternalToInternalConverter& externalToInternalConverter);

typedef AbstractContainerAccess*(*BiContainerAccessFactory)(const QMetaType& keyMetaType,
                                                            const QHashFunction& keyHashFunction,
                                                            const InternalToExternalConverter& keyInternalToExternalConverter,
                                                            const ExternalToInternalConverter& keyExternalToInternalConverter,
                                                            const QMetaType& valueMetaType,
                                                            const QHashFunction& valueHashFunction,
                                                            const InternalToExternalConverter& valueInternalToExternalConverter,
                                                            const ExternalToInternalConverter& valueExternalToInternalConverter);


enum class ContainerType{
    QList,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QVector,
    QLinkedList,
#endif
    QStack,
    QQueue,
    QSet
};

template<template<typename> class Container>
struct ContainerAnalyzer{
};

template<>
struct ContainerAnalyzer<QList>{
    static constexpr ContainerType type = ContainerType::QList;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#ifdef QVECTOR_H
template<>
struct ContainerAnalyzer<QVector>{
    static constexpr ContainerType type = ContainerType::QVector;
};
#endif
#ifdef QLINKEDLIST_H
template<>
struct ContainerAnalyzer<QLinkedList>{
    static constexpr ContainerType type = ContainerType::QLinkedList;
};
#endif
#endif

template<>
struct ContainerAnalyzer<QStack>{
    static constexpr ContainerType type = ContainerType::QStack;
};

template<>
struct ContainerAnalyzer<QQueue>{
    static constexpr ContainerType type = ContainerType::QQueue;
};

template<>
struct ContainerAnalyzer<QSet>{
    static constexpr ContainerType type = ContainerType::QSet;
};

enum class MapType{
    QHash,
    QMap,
    QMultiHash,
    QMultiMap,
    QPair
};

template<template<typename,typename> class Container>
struct MapAnalyzer{
};

template<>
struct MapAnalyzer<QMap>{
    static constexpr MapType type = MapType::QMap;
};

template<>
struct MapAnalyzer<QHash>{
    static constexpr MapType type = MapType::QHash;
};

template<>
struct MapAnalyzer<QMultiMap>{
    static constexpr MapType type = MapType::QMultiMap;
};

template<>
struct MapAnalyzer<QMultiHash>{
    static constexpr MapType type = MapType::QMultiHash;
};

template<>
struct MapAnalyzer<QPair>{
    static constexpr MapType type = MapType::QPair;
};

struct MetaData{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    MetaData(int _metaType,
             const QHashFunction& _hashFunction)
        : metaType(_metaType),
          hashFunction(_hashFunction)
    {}
#endif

    MetaData(const QMetaType& _metaType,
             const QHashFunction& _hashFunction
            )
        : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
              _metaType.id()
#else
              _metaType
#endif
            ),
          hashFunction(_hashFunction)
    {}
    MetaData(QMetaType&& _metaType = QMetaType(),
             QHashFunction&& _hashFunction = QHashFunction()
            )
        : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
              _metaType.id()
#else
              std::move(_metaType)
#endif
            ),
          hashFunction(std::move(_hashFunction))
    {}
    MetaData(const MetaData& d)
        : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                d.metaType.id()
#else
                d.metaType
#endif
            ),
          hashFunction(d.hashFunction)
    {}
    MetaData(MetaData&& d)
        : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                d.metaType.id()
#else
                std::move(d.metaType)
#endif
            ),
          hashFunction(std::move(d.hashFunction))
    {}

    QMetaType metaType;
    QHashFunction hashFunction;
};

QTJAMBI_EXPORT const MetaData& currentMetaData(uint i);
QTJAMBI_EXPORT void setCurrentMetaData(uint i, const MetaData& metaData);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

        template<typename T, bool enabled = QtJambiPrivate::supports_debugstream<T>::value>
        struct DebugStreamFunction : public QtPrivate::AbstractDebugStreamFunction
        {
            static QtPrivate::AbstractDebugStreamFunction* create(){ return new DebugStreamFunction(); }
        private:
            DebugStreamFunction()
                : AbstractDebugStreamFunction(stream, destroy) {}
            static void stream(const QtPrivate::AbstractDebugStreamFunction *, QDebug& dbg, const void *r)
            {
                const T *rhs = static_cast<const T *>(r);
                operator<<(dbg, *rhs);
            }

            static void destroy(QtPrivate::AbstractDebugStreamFunction *_this)
            {
                delete static_cast<DebugStreamFunction *>(_this);
            }
        };

        template<typename T>
        struct DebugStreamFunction<T,false>
        {
            static QtPrivate::AbstractDebugStreamFunction* create(){ return nullptr; }
        };
#endif

typedef hash_type(*HashWrapper)(const void* ptr, QHashFunction hashFunction);
typedef hash_type(*HashWrapper2)(const void* ptr, QHashFunction hashFunction1, QHashFunction hashFunction2);

template<typename Container>
hash_type qtjambi_container_hash(const void* ptr, QHashFunction hashFunction)
{
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    hash_type prime = 31;
    hash_type result = 1;
    result = prime * result + qHash(container.size());
    for(typename Container::const_iterator iter = container.begin();
        iter != container.end(); ++iter){
        result = prime * result + hashFunction(& (*iter));
    }
    return result;
}

template<typename Container>
hash_type qtjambi_bicontainer_hash(const void* ptr, QHashFunction hashFunction1, QHashFunction hashFunction2)
{
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    hash_type prime = 31;
    hash_type result = 1;
    result = prime * result + qHash(container.size());
    for(typename Container::const_iterator iter = container.begin();
        iter != container.end(); ++iter){
        result = prime * result + hashFunction1(&iter.key());
        result = prime * result + hashFunction2(&iter.value());
    }
    return result;
}

template<typename Container>
hash_type pairHashWrapper(const void* ptr, QHashFunction hashFunction1, QHashFunction hashFunction2){
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    hash_type prime = 31;
    hash_type result = 1;
    result = prime * result + hashFunction1(&container.first);
    result = prime * result + hashFunction2(&container.second);
    return result;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
typedef const QtPrivate::AbstractConverterFunction*(*ConverterFunctionFactory1)(int metaTypeId);
typedef const QtPrivate::AbstractConverterFunction*(*ConverterFunctionFactory2)(int metaTypeId1, int metaTypeId2);

QTJAMBI_EXPORT int qtjambi_register_container_type(const QByteArray& typeName,
                                                   size_t containerSize, size_t containerAlign,
                                                   bool isPointer,
                                                   const QMetaType& metaType,
                                                   QMetaType::Destructor destructor,
                                                   QMetaType::Constructor constructor,
                                                   QMetaType::SaveOperator saveOperator,
                                                   QMetaType::LoadOperator loadOperator,
                                                   const QtPrivate::AbstractDebugStreamFunction * debugStreamFunction,
                                                   const QtPrivate::AbstractComparatorFunction * comparator,
                                                   ConverterFunctionFactory1 createConverterFunction,
                                                   HashWrapper hashWrapper);

template<typename Container, size_t size>
int qtjambi_register_container_type(const QByteArray& typeName, const QMetaType& metaType){
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        static QtPrivate::BuiltInEqualsComparatorFunction<Container> comparator;
        newMetaType = qtjambi_register_container_type(typeName, sizeof(Container), Q_ALIGNOF(Container), size==0, metaType,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Destruct,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Construct,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size!=0>::Save,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size!=0>::Load,
                                                      DebugStreamFunction<Container>::create(),
                                                      &comparator,
                                                      [](int metaTypeId) -> const QtPrivate::AbstractConverterFunction* {
                                                              struct Converter : QtPrivate::AbstractConverterFunction
                                                              {
                                                                  explicit Converter(int metaType)
                                                                      : QtPrivate::AbstractConverterFunction(convert),
                                                                        m_metaType(metaType) {}
                                                                  static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                                                                  {
                                                                      QtMetaTypePrivate::QSequentialIterableImpl *t = static_cast<QtMetaTypePrivate::QSequentialIterableImpl *>(out);
                                                                      t->_iterable = in;
                                                                      t->_iterator = nullptr;
                                                                      t->_metaType_id = static_cast<const Converter *>(_this)->m_metaType;
                                                                      t->_metaType_flags = size==0;
                                                                      t->_iteratorCapabilities = QtMetaTypePrivate::ForwardCapability;
                                                                      t->_size = QtMetaTypePrivate::QSequentialIterableImpl::sizeImpl<Container>;
                                                                      t->_at = QtMetaTypePrivate::QSequentialIterableImpl::atImpl<Container>;
                                                                      t->_moveToBegin = QtMetaTypePrivate::QSequentialIterableImpl::moveToBeginImpl<Container>;
                                                                      t->_moveToEnd = QtMetaTypePrivate::QSequentialIterableImpl::moveToEndImpl<Container>;
                                                                      t->_advance = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::advance;
                                                                      t->_get = QtMetaTypePrivate::QSequentialIterableImpl::getImpl<Container>;
                                                                      t->_destroyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::destroy;
                                                                      t->_equalIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::equal;
                                                                      t->_copyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::assign;
                                                                      return true;
                                                                  }
                                                                  int m_metaType;
                                                              };
                                                              return new Converter(metaTypeId);
                                                          },
                                                      qtjambi_container_hash<Container>);
    }
    return newMetaType;
}

QTJAMBI_EXPORT int qtjambi_register_bicontainer_type(const QByteArray& typeName,
                                                     size_t containerSize, size_t containerAlign,
                                                     bool isPointer1, const QMetaType& metaType1,
                                                     bool isPointer2, const QMetaType& metaType2,
                                                     QMetaType::Destructor destructor,
                                                     QMetaType::Constructor constructor,
                                                     QMetaType::SaveOperator saveOperator,
                                                     QMetaType::LoadOperator loadOperator,
                                                     const QtPrivate::AbstractDebugStreamFunction * debugStreamFunction,
                                                     const QtPrivate::AbstractComparatorFunction * comparator,
                                                     bool isPair,
                                                     ConverterFunctionFactory2 createConverterFunction,
                                                     HashWrapper2 hashWrapper);

template<typename Container, size_t size1, size_t size2>
int qtjambi_register_bicontainer_type(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        static QtPrivate::BuiltInEqualsComparatorFunction<Container> comparator;
        newMetaType = qtjambi_register_bicontainer_type(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Destruct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Construct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Save,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Load,
                                                        DebugStreamFunction<Container>::create(),
                                                        &comparator,
                                                        false,
                                                        [](int metaTypeId1, int metaTypeId2) -> const QtPrivate::AbstractConverterFunction* {
                                                                  struct Converter : QtPrivate::AbstractConverterFunction
                                                                  {
                                                                      explicit Converter(int metaType_key, int metaType_value)
                                                                          : QtPrivate::AbstractConverterFunction(convert),
                                                                            m_metaType_key(metaType_key), m_metaType_value(metaType_value) {}
                                                                      static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                                                                      {
                                                                          QtMetaTypePrivate::QAssociativeIterableImpl *t = static_cast<QtMetaTypePrivate::QAssociativeIterableImpl *>(out);
                                                                          t->_iterable = in;
                                                                          t->_iterator = nullptr;
                                                                          t->_metaType_id_key = static_cast<const Converter *>(_this)->m_metaType_key;
                                                                          t->_metaType_flags_key = size1==0;
                                                                          t->_metaType_id_value = static_cast<const Converter *>(_this)->m_metaType_value;
                                                                          t->_metaType_flags_value = size2==0;
                                                                          t->_size = QtMetaTypePrivate::QAssociativeIterableImpl::sizeImpl<Container>;
                                                                          t->_find = QtMetaTypePrivate::QAssociativeIterableImpl::findImpl<Container>;
                                                                          t->_begin = QtMetaTypePrivate::QAssociativeIterableImpl::beginImpl<Container>;
                                                                          t->_end = QtMetaTypePrivate::QAssociativeIterableImpl::endImpl<Container>;
                                                                          t->_advance = QtMetaTypePrivate::QAssociativeIterableImpl::advanceImpl<Container>;
                                                                          t->_getKey = QtMetaTypePrivate::QAssociativeIterableImpl::getKeyImpl<Container>;
                                                                          t->_getValue = QtMetaTypePrivate::QAssociativeIterableImpl::getValueImpl<Container>;
                                                                          t->_destroyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::destroy;
                                                                          t->_equalIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::equal;
                                                                          t->_copyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::assign;
                                                                          return true;
                                                                      }
                                                                      int m_metaType_key;
                                                                      int m_metaType_value;
                                                                  };
                                                                  return new Converter(metaTypeId1, metaTypeId2);
                                                              },
                                                              qtjambi_bicontainer_hash<Container>
                                                        );
    }
    return newMetaType;
}

template<typename Container>
QtMetaTypePrivate::VariantData getFirstImpl(const void * const *pair, int metaTypeId, uint flags)
{
    return QtMetaTypePrivate::VariantData(metaTypeId, &static_cast<const Container*>(*pair)->first, flags);
}

template<typename Container>
QtMetaTypePrivate::VariantData getSecondImpl(const void * const *pair, int metaTypeId, uint flags)
{
    return QtMetaTypePrivate::VariantData(metaTypeId, &static_cast<const Container*>(*pair)->second, flags);
}

template<typename Container, size_t size1, size_t size2>
int qtjambi_register_qpair_type(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        static QtPrivate::BuiltInEqualsComparatorFunction<Container> comparator;
        newMetaType = qtjambi_register_bicontainer_type(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Destruct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Construct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Save,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Load,
                                                        DebugStreamFunction<Container>::create(),
                                                        &comparator,
                                                        true,
                                                        [](int metaType1Id, int metaType2Id) -> const QtPrivate::AbstractConverterFunction* {
                                                              struct Converter : QtPrivate::AbstractConverterFunction
                                                              {
                                                                  explicit Converter(int metaType_first, int metaType_second)
                                                                      : QtPrivate::AbstractConverterFunction(convert),
                                                                        m_metaType_first(metaType_first), m_metaType_second(metaType_second) {}
                                                                  static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                                                                  {
                                                                      struct _QPairVariantInterfaceImpl
                                                                      {
                                                                          const void *_pair;
                                                                          int _metaType_id_first;
                                                                          uint _metaType_flags_first;
                                                                          int _metaType_id_second;
                                                                          uint _metaType_flags_second;
                                                                          QtMetaTypePrivate::VariantData (*_getFirst)(const void * const *p, int metaTypeId, uint flags);
                                                                          QtMetaTypePrivate::VariantData (*_getSecond)(const void * const *p, int metaTypeId, uint flags);
                                                                      };

                                                                      _QPairVariantInterfaceImpl *t = static_cast<_QPairVariantInterfaceImpl *>(out);
                                                                      t->_pair = in;
                                                                      t->_metaType_id_first = static_cast<const Converter *>(_this)->m_metaType_first;
                                                                      t->_metaType_flags_first = size1==0;
                                                                      t->_metaType_id_second = static_cast<const Converter *>(_this)->m_metaType_second;
                                                                      t->_metaType_flags_second = size2==0;
                                                                      t->_getFirst = getFirstImpl<Container>;
                                                                      t->_getSecond = getSecondImpl<Container>;
                                                                      return true;
                                                                  }
                                                                  int m_metaType_first;
                                                                  int m_metaType_second;
                                                              };
                                                              return new Converter(metaType1Id, metaType2Id);
                                                        },
                                                        pairHashWrapper<Container>
                                                    );
    }
    return newMetaType;
}

#else
typedef bool(*MetaSequenceIteratorFactory)(void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface);
typedef bool(*MetaSequenceConstIteratorFactory)(const void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface);
typedef bool(*MetaAssociationIteratorFactory)(void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface);
typedef bool(*MetaAssociationConstIteratorFactory)(const void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface);
typedef bool(*PairAccessFactory)(const void *src, void *target, const QMetaType& metaType1, const QMetaType& metaType2);

QTJAMBI_EXPORT int qtjambi_register_container_type(const QByteArray& typeName,
                                                   size_t containerSize, size_t containerAlign,
                                                   bool isPointer, const QMetaType& metaType,
                                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                                   QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                                   QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                                   QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                                   QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                                   QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                                   QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                                   const QtMetaContainerPrivate::QMetaSequenceInterface *defaultInterface,
                                                   MetaSequenceIteratorFactory createMetaSequenceIterator,
                                                   MetaSequenceConstIteratorFactory createMetaSequenceConstIterator,
                                                   HashWrapper hashWrapper);

template<typename Container, size_t size>
static int qtjambi_register_container_type(const QByteArray& typeName, const QMetaType& metaType){
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
        static const QtMetaContainerPrivate::QMetaSequenceInterface defaultInterface = QtMetaContainerPrivate::QMetaSequenceInterface(QtMetaContainerPrivate::QMetaSequenceForContainer<Container>());
        newMetaType = qtjambi_register_container_type(typeName, sizeof(Container), Q_ALIGNOF(Container), size==0, metaType,
                                                QtPrivate::QMetaTypeForType<Container>::getDefaultCtr(),
                                                QtPrivate::QMetaTypeForType<Container>::getCopyCtr(),
                                                QtPrivate::QMetaTypeForType<Container>::getMoveCtr(),
                                                QtPrivate::QMetaTypeForType<Container>::getDtor(),
                                                QtPrivate::QEqualityOperatorForType<Container>::equals,
                                                QtPrivate::QLessThanOperatorForType<Container>::lessThan,
                                                QtPrivate::QDebugStreamOperatorForType<Container>::debugStream,
                                                QtPrivate::QDataStreamOperatorForType<Container>::dataStreamOut,
                                                QtPrivate::QDataStreamOperatorForType<Container>::dataStreamIn,
                                                &defaultInterface,
                                                [](void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface) ->bool {
                                                    *reinterpret_cast<QIterable<QMetaSequence>*>(target) = QIterable<QMetaSequence>(QMetaSequence(iface), reinterpret_cast<Container*>(src));
                                                    return true;
                                                },
                                                [](const void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface) ->bool {
                                                    *reinterpret_cast<QIterable<QMetaSequence>*>(target) = QIterable<QMetaSequence>(QMetaSequence(iface), reinterpret_cast<const Container*>(src));
                                                    return true;
                                                },
                                                qtjambi_container_hash<Container>);
    }
    return newMetaType;
}

QTJAMBI_EXPORT int qtjambi_register_bicontainer_type(const QByteArray& typeName,
                                                     size_t containerSize, size_t containerAlign,
                                                     bool isPointer1, const QMetaType& metaType1,
                                                     bool isPointer2, const QMetaType& metaType2,
                                                     QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                                     QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                                     QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                                     QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                                     QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                                     QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                                     QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                                     QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                                     QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                                     const QtMetaContainerPrivate::QMetaAssociationInterface *defaultInterface,
                                                     MetaAssociationIteratorFactory createMetaAssociationIterator,
                                                     MetaAssociationConstIteratorFactory createMetaAssociationConstIterator,
                                                     PairAccessFactory pairAccessFactory,
                                                     HashWrapper2 hashWrapper);

template<typename Container, size_t size1, size_t size2>
int qtjambi_register_bicontainer_type(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
        static const QtMetaContainerPrivate::QMetaAssociationInterface defaultInterface = QtMetaContainerPrivate::QMetaAssociationInterface(QtMetaContainerPrivate::QMetaAssociationForContainer<Container>());
        newMetaType = qtjambi_register_bicontainer_type(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QtPrivate::QMetaTypeForType<Container>::getDefaultCtr(),
                                                        QtPrivate::QMetaTypeForType<Container>::getCopyCtr(),
                                                        QtPrivate::QMetaTypeForType<Container>::getMoveCtr(),
                                                        QtPrivate::QMetaTypeForType<Container>::getDtor(),
                                                        QtPrivate::QEqualityOperatorForType<Container>::equals,
                                                        QtPrivate::QLessThanOperatorForType<Container>::lessThan,
                                                        QtPrivate::QDebugStreamOperatorForType<Container>::debugStream,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamOut,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamIn,
                                                        &defaultInterface,
                                                        [](void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface) ->bool {
                                                            *reinterpret_cast<QIterable<QMetaAssociation>*>(target) = QIterable<QMetaAssociation>(QMetaAssociation(iface), reinterpret_cast<Container*>(src));
                                                            return true;
                                                        },
                                                        [](const void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface) ->bool {
                                                            *reinterpret_cast<QIterable<QMetaAssociation>*>(target) = QIterable<QMetaAssociation>(QMetaAssociation(iface), reinterpret_cast<const Container*>(src));
                                                            return true;
                                                        },
                                                        nullptr,
                                                        qtjambi_bicontainer_hash<Container>
        );
    }
    return newMetaType;
}

template<typename Container, size_t size1, size_t size2>
static int qtjambi_register_qpair_type(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
        newMetaType = qtjambi_register_bicontainer_type(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QtPrivate::QMetaTypeForType<Container>::getDefaultCtr(),
                                                        QtPrivate::QMetaTypeForType<Container>::getCopyCtr(),
                                                        QtPrivate::QMetaTypeForType<Container>::getMoveCtr(),
                                                        QtPrivate::QMetaTypeForType<Container>::getDtor(),
                                                        QtPrivate::QEqualityOperatorForType<Container>::equals,
                                                        QtPrivate::QLessThanOperatorForType<Container>::lessThan,
                                                        QtPrivate::QDebugStreamOperatorForType<Container>::debugStream,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamOut,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamIn,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        [](const void *src, void *target, const QMetaType& metaType1, const QMetaType& metaType2) ->bool {
                                                              QtMetaTypePrivate::QPairVariantInterfaceImpl* result = reinterpret_cast<QtMetaTypePrivate::QPairVariantInterfaceImpl*>(target);
                                                              *result = QtMetaTypePrivate::QPairVariantInterfaceImpl(reinterpret_cast<const Container*>(src));
                                                              result->_metaType_first = metaType1;
                                                              result->_metaType_second = metaType2;
                                                              return true;
                                                        },
                                                        pairHashWrapper<Container>
                                                    );
    }
    return newMetaType;
}

#endif

template<int index, bool isPointer>
class MetaTypeInfo{
    QMetaType m_metaType;
    QHashFunction m_hashFunction;
public:
    MetaTypeInfo(const QMetaType& metaType,
                 const QHashFunction& hashFunction
            ) : m_metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                              metaType.id()
#else
                              metaType
#endif
                    ),
                m_hashFunction(hashFunction)
    {}

    MetaTypeInfo(const MetaTypeInfo<index,isPointer>& other)
        : m_metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    other.m_metaType.id()
#else
                    other.m_metaType
#endif
              ),
          m_hashFunction(other.m_hashFunction)
    {}

    const QMetaType& metaType() const {
        return m_metaType;
    }

    const QHashFunction& hashFunction() const {
        return m_hashFunction;
    }

    template<int, bool>
    friend struct MetaTypeInfoLocker;
};

QTJAMBI_EXPORT hash_type pointerHashFunction(const void* ptr);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool pointerLessFunction(const void* ptr1, const void* ptr2);
QTJAMBI_EXPORT bool pointerEqualFunction(const void* ptr1, const void* ptr2);
#endif

template<int index>
class MetaTypeInfo<index,true>{
    QMetaType m_metaType;
public:
    MetaTypeInfo(const QMetaType& metaType,
                 const QHashFunction&
            ) : m_metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        metaType.id()
#else
                        metaType
#endif
                    ){}
    MetaTypeInfo(const MetaTypeInfo<index,true>& other) : m_metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                              other.m_metaType.id()
#else
                                                              other.m_metaType
#endif
                                                              ){}

    const QMetaType& metaType() const {
        return m_metaType;
    }

    QHashFunction hashFunction() const {
        return &pointerHashFunction;
    }
};

template<int index, bool isPointer>
struct MetaTypeInfoLocker{
    MetaTypeInfoLocker(const MetaTypeInfo<index,isPointer>& info) :
        m_oldMetaData(currentMetaData(index))
    {
        setCurrentMetaData(index, MetaData(info.m_metaType, info.m_hashFunction));
    }
    ~MetaTypeInfoLocker(){
        setCurrentMetaData(index, m_oldMetaData);
    }
private:
    MetaData m_oldMetaData;
};

template<int index>
struct MetaTypeInfoLocker<index,true>{
    MetaTypeInfoLocker(const MetaTypeInfo<index,true>&){}
};

#define QTJAMBI_ELEMENT_LOCKER MetaTypeInfoLocker<0,_size==0> __elementLocker(m_elementMetaTypeInfo);\
    Q_UNUSED(__elementLocker)

#define QTJAMBI_KEY_VALUE_LOCKER MetaTypeInfoLocker<0,size1==0> __keyLocker(m_keyMetaTypeInfo);\
    Q_UNUSED(__keyLocker)\
    MetaTypeInfoLocker<1,size2==0> __valueLocker(m_valueMetaTypeInfo);\
    Q_UNUSED(__valueLocker)

QTJAMBI_EXPORT QDebug containerElementDebugStream(QDebug debug, uint i, const void* ptr);
QTJAMBI_EXPORT QDataStream & containerElementDataStreamIn(QDataStream & stream, uint i, void* ptr);
QTJAMBI_EXPORT QDataStream & containerElementDataStreamOut(QDataStream & stream, uint i, const void* ptr);
QTJAMBI_EXPORT hash_type containerElementHash(uint i, const void* ptr);
QTJAMBI_EXPORT bool containerElementEqual(uint i, const void* ptr, const void* ptr2);
QTJAMBI_EXPORT bool containerElementLess(uint i, const void* ptr, const void* ptr2);
QTJAMBI_EXPORT void constructContainerElement(uint i, void* ptr, const void* copy = nullptr);
QTJAMBI_EXPORT void destructContainerElement(uint i, void* ptr);

template<size_t size, uint i, bool isStatic, size_t align = 0, bool useAlignment = (align>0) >
struct ContainerElement{
};

template<size_t size, uint i, bool isStatic>
struct ContainerElement<size, i, isStatic, 0, false>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
private:
    quint8 data[size];
};

template<uint i, bool isStatic>
struct ContainerElement<1, i, isStatic, 0, false>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
private:
    quint8 data;
};

template<size_t size, uint i, bool isStatic, size_t align>
struct Q_DECL_ALIGN(align) ContainerElement<size, i, isStatic, align, true>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
private:
    quint8 data[size];
};

template<uint i, bool isStatic, size_t align>
struct Q_DECL_ALIGN(align) ContainerElement<1, i, isStatic, align, true>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
private:
    quint8 data;
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<size_t size, uint i, size_t align, bool useAlignment>
class QTypeInfo<ContainerElement<size, i, false, align, useAlignment>>
{
public:
    enum {
        isSpecialized = false,
        isPointer = false,
        isIntegral = false,
        isComplex = true,
        isStatic = false,
        isRelocatable = qIsRelocatable<ContainerElement<size,i,true, align, useAlignment>>(),
        isLarge = (size>sizeof(void*)),
        isDummy = false,
        sizeOf = size
    };
};

template<size_t size, uint i, size_t align, bool useAlignment>
class QTypeInfo<ContainerElement<size, i, true, align, useAlignment>>
{
public:
    enum {
        isSpecialized = false,
        isPointer = false,
        isIntegral = false,
        isComplex = true,
        isStatic = true,
        isRelocatable = qIsRelocatable<ContainerElement<size,i,true, align, useAlignment>>(),
        isLarge = (size>sizeof(void*)),
        isDummy = false,
        sizeOf = size
    };
};
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
QDebug operator<<(QDebug debug, const ContainerElement<size, i, isStatic, align, useAlignment>& value){
    return containerElementDebugStream(debug, i, &value);
}

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
QDataStream & operator<<(QDataStream & s, const ContainerElement<size, i, isStatic, align, useAlignment>& value){
    return containerElementDataStreamOut(s, i, &value);
}

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
QDataStream & operator>>(QDataStream & s, ContainerElement<size, i, isStatic, align, useAlignment>& value){
    return containerElementDataStreamIn(s, i, &value);
}

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
hash_type qHash(const ContainerElement<size, i, isStatic, align, useAlignment>& value){
    return containerElementHash(i, &value);
}

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
bool operator ==(const ContainerElement<size, i, isStatic, align, useAlignment>& value1, const ContainerElement<size, i, isStatic, align, useAlignment>& value2){
    return containerElementEqual(i, &value1, &value2);
}

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
bool operator <(const ContainerElement<size, i, isStatic, align, useAlignment>& value1, const ContainerElement<size, i, isStatic, align, useAlignment>& value2){
    return containerElementLess(i, &value1, &value2);
}

namespace QtJambiPrivate {

template<typename T, bool canLess = supports_less_than<T>::value>
struct TryLess{
    typedef jboolean (*LessThanFunction)(const T&, const T&);

    static LessThanFunction function(JNIEnv *){
        return [](const T& a, const T& b)->jboolean{return a < b;};
    }
};

template<typename T>
struct TryLess<T,false>{
    typedef jboolean (*LessThanFunction)(const T&, const T&);

    static LessThanFunction function(JNIEnv * env){
        JavaException::raiseQNoImplementationException(env, "operator <" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename> class Container, size_t _align, size_t _size, bool _isStatic>
class IteratorAccess : public AbstractIteratorAccess{
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, _isStatic, _align>>::type T;
    IteratorAccess(const IteratorAccess<Container, _align, _size, _isStatic>& other)
        : AbstractIteratorAccess(),
          m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
          m_internalToExternalConverter(other.m_internalToExternalConverter){}
public:
    IteratorAccess(MetaTypeInfo<0,_size==0> elementMetaTypeInfo, const InternalToExternalConverter& internalToExternalConverter)
        : AbstractIteratorAccess(),
          m_elementMetaTypeInfo(elementMetaTypeInfo),
          m_internalToExternalConverter(internalToExternalConverter){}

    IteratorAccess<Container, _align, _size, _isStatic>* clone() override{
        return new IteratorAccess<Container, _align, _size, _isStatic>(*this);
    }

    void dispose() override { delete this; }

    jobject value(JNIEnv * env, void* iterator) override {
        typename Container<T>::const_iterator* iter = reinterpret_cast<typename Container<T>::const_iterator*>(iterator);
        jvalue _value;
        _value.l = nullptr;
        QTJAMBI_ELEMENT_LOCKER
        bool success = m_internalToExternalConverter(env, nullptr, &(*(*iter)), &_value, true);
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    void increment(JNIEnv *env, void* iterator) override {
        QTJAMBI_ELEMENT_LOCKER
        IteratorIncrement<typename Container<T>::const_iterator>::function(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QTJAMBI_ELEMENT_LOCKER
        IteratorDecrement<typename Container<T>::const_iterator>::function(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, void* iterator, void* other) override {
        auto lessFunction = TryLess<typename Container<T>::const_iterator>::function(env);
        QTJAMBI_ELEMENT_LOCKER
        typename Container<T>::const_iterator* iter = reinterpret_cast<typename Container<T>::const_iterator*>(iterator);
        typename Container<T>::const_iterator* iter2 = reinterpret_cast<typename Container<T>::const_iterator*>(other);
        return lessFunction(*iter, *iter2);
    }
    bool canLess() override {
        return std::is_pointer<typename Container<T>::const_iterator>::value || supports_less_than<typename Container<T>::const_iterator>::value;
    }
    jboolean equals(JNIEnv *, void* iterator, void* other) override {
        QTJAMBI_ELEMENT_LOCKER
        typename Container<T>::const_iterator* iter = reinterpret_cast<typename Container<T>::const_iterator*>(iterator);
        typename Container<T>::const_iterator* iter2 = reinterpret_cast<typename Container<T>::const_iterator*>(other);
        return (*iter) == (*iter2);
    }
private:
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    InternalToExternalConverter m_internalToExternalConverter;
};

template<template<typename, typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2>
class BiIteratorAccess : public AbstractBiIteratorAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;

    BiIteratorAccess(const BiIteratorAccess& other)
        : AbstractBiIteratorAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter){}

public:
    BiIteratorAccess(MetaTypeInfo<0,size1==0> keyMetaTypeInfo,
                   const InternalToExternalConverter& keyInternalToExternalConverter,
                   MetaTypeInfo<1,size2==0> valueMetaTypeInfo,
                   const InternalToExternalConverter& valueInternalToExternalConverter)
        : AbstractBiIteratorAccess(),
          m_keyMetaTypeInfo(keyMetaTypeInfo),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_valueMetaTypeInfo(valueMetaTypeInfo),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter){}

    BiIteratorAccess<Container, align1, size1, align2, size2>* clone() override{
        return new BiIteratorAccess<Container, align1, size1, align2, size2>(*this);
    }

    void dispose() override { delete this; }

    jobject value(JNIEnv * env, void* iterator) override {
        typename Container<K,T>::const_iterator* iter = reinterpret_cast<typename Container<K,T>::const_iterator*>(iterator);
        jvalue _value;
        _value.l = nullptr;
        QTJAMBI_KEY_VALUE_LOCKER
        bool success = m_valueInternalToExternalConverter(env, nullptr, &(iter->value()), &_value, true);
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    jobject key(JNIEnv * env, void* iterator) override {
        typename Container<K,T>::const_iterator* iter = reinterpret_cast<typename Container<K,T>::const_iterator*>(iterator);
        jvalue _value;
        _value.l = nullptr;
        QTJAMBI_KEY_VALUE_LOCKER
        bool success = m_keyInternalToExternalConverter(env, nullptr, &(iter->key()), &_value, true);
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    void increment(JNIEnv *env, void* iterator) override {
        QTJAMBI_KEY_VALUE_LOCKER
        IteratorIncrement<typename Container<K,T>::const_iterator>::function(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QTJAMBI_KEY_VALUE_LOCKER
        IteratorDecrement<typename Container<K,T>::const_iterator>::function(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, void* iterator, void* other) override {
        auto lessFunction = TryLess<typename Container<K,T>::const_iterator>::function(env);
        QTJAMBI_KEY_VALUE_LOCKER
        typename Container<K,T>::const_iterator* iter = reinterpret_cast<typename Container<K,T>::const_iterator*>(iterator);
        typename Container<K,T>::const_iterator* iter2 = reinterpret_cast<typename Container<K,T>::const_iterator*>(other);
        return lessFunction(*iter, *iter2);
    }
    bool canLess() override {
        return std::is_pointer<typename Container<K,T>::const_iterator>::value || supports_less_than<typename Container<K,T>::const_iterator>::value;
    }
    jboolean equals(JNIEnv *, void* iterator, void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER
        typename Container<K,T>::const_iterator* iter = reinterpret_cast<typename Container<K,T>::const_iterator*>(iterator);
        typename Container<K,T>::const_iterator* iter2 = reinterpret_cast<typename Container<K,T>::const_iterator*>(other);
        return (*iter) == (*iter2);
    }
private:
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    InternalToExternalConverter m_valueInternalToExternalConverter;
};

template<template <typename> class Container, size_t align, size_t size, bool isStatic>
struct ContainerAccessFac{
};

template<template <typename,typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2>
struct BiContainerAccessFac{
};

}

class QTJAMBI_EXPORT ContainerAccessFactories{
public:
    static ContainerAccessFactory getAccessFactory(ContainerType containerType, size_t align, size_t size, bool isStatic);
    static BiContainerAccessFactory getAccessFactory(MapType containerType, size_t align1, size_t size1, size_t align2, size_t size2);
    static void registerAccessFactory(ContainerType containerType, size_t align, size_t size, bool isStatic, ContainerAccessFactory factory);
    static void registerAccessFactory(MapType containerType, size_t align1, size_t size1, size_t align2, size_t size2, BiContainerAccessFactory factory);
};

template<template<typename> class Container, size_t align, size_t size, bool isStatic, bool = (align<=size && align <= alignof(std::max_align_t) && (!isStatic || size <= sizeof(void*))) >
struct ContainerAccessFactoryHelper{
    static void registerContainerAccessFactory(){
        ContainerAccessFactories::registerAccessFactory(ContainerAnalyzer<Container>::type, align, size, isStatic, QtJambiPrivate::ContainerAccessFac<Container, align, size, isStatic>::factory());
    }
};

template<template<typename> class Container, size_t align, size_t size, bool isStatic>
struct ContainerAccessFactoryHelper<Container, align, size, isStatic, false>{
    static void registerContainerAccessFactory(){
    }
};

template<template<typename,typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2, bool = (align1<=size1 && align2<=size2 && align1 <= alignof(std::max_align_t) && align2 <= alignof(std::max_align_t)) >
struct BiContainerAccessFactoryHelper{
    static void registerContainerAccessFactory(){
        ContainerAccessFactories::registerAccessFactory(MapAnalyzer<Container>::type, align1, size1, align2, size2, QtJambiPrivate::BiContainerAccessFac<Container, align1, size1, align2, size2>::factory());
    }
};

template<template<typename,typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2>
struct BiContainerAccessFactoryHelper<Container, align1, size1, align2, size2, false>{
    static void registerContainerAccessFactory(){
    }
};

#define ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,D)\
    BiContainerAccessFactoryHelper<HASH,A,B,C,D>::registerContainerAccessFactory();

#define ELEMENT_CONVERTER_HASH_IMPL0(HASH,C,D)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,0,0,C,D)
#define ELEMENT_CONVERTER_HASH_IMPL2(HASH,A,B,C)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,0)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,1)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,2)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,4)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,8)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,16)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,24)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,32)\
    ELEMENT_CONVERTER_HASH_IMPL1(HASH,A,B,C,64)
#define ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,B)\
    ELEMENT_CONVERTER_HASH_IMPL2(HASH,A,B,0)\
    ELEMENT_CONVERTER_HASH_IMPL2(HASH,A,B,1)\
    ELEMENT_CONVERTER_HASH_IMPL2(HASH,A,B,2)\
    ELEMENT_CONVERTER_HASH_IMPL2(HASH,A,B,4)\
    ELEMENT_CONVERTER_HASH_IMPL2(HASH,A,B,8)\
    ELEMENT_CONVERTER_HASH_IMPL2(HASH,A,B,16)
#define ELEMENT_CONVERTER_HASH_IMPL4(HASH,A)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,0)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,1)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,2)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,4)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,8)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,16)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,24)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,32)\
    ELEMENT_CONVERTER_HASH_IMPL3(HASH,A,64)
#define ELEMENT_CONVERTER_HASH_IMPL(HASH)\
    ELEMENT_CONVERTER_HASH_IMPL4(HASH,0)\
    ELEMENT_CONVERTER_HASH_IMPL4(HASH,1)\
    ELEMENT_CONVERTER_HASH_IMPL4(HASH,2)\
    ELEMENT_CONVERTER_HASH_IMPL4(HASH,4)\
    ELEMENT_CONVERTER_HASH_IMPL4(HASH,8)\
    ELEMENT_CONVERTER_HASH_IMPL4(HASH,16)

#define ELEMENT_SIZE_ACTIONS\
    ELEMENT_SIZE_ACTION(0)\
    ELEMENT_SIZE_ACTION(1)\
    ELEMENT_SIZE_ACTION(2)\
    ELEMENT_SIZE_ACTION(4)\
    ELEMENT_SIZE_ACTION(6)\
    ELEMENT_SIZE_ACTION(8)\
    ELEMENT_SIZE_ACTION(10)\
    ELEMENT_SIZE_ACTION(12)\
    ELEMENT_SIZE_ACTION(14)\
    ELEMENT_SIZE_ACTION(16)\
    ELEMENT_SIZE_ACTION(24)\
    ELEMENT_SIZE_ACTION(32)\
    ELEMENT_SIZE_ACTION(48)\
    ELEMENT_SIZE_ACTION(56)\
    ELEMENT_SIZE_ACTION(64)\
    ELEMENT_SIZE_ACTION(72)\
    ELEMENT_SIZE_ACTION(96)\
    ELEMENT_SIZE_ACTION(128)

#define ELEMENT_ALIGNSIZE_ACTION2(size)\
    ELEMENT_ALIGNSIZE_ACTION(0,size);\
    ELEMENT_ALIGNSIZE_ACTION(1,size);\
    ELEMENT_ALIGNSIZE_ACTION(2,size);\
    ELEMENT_ALIGNSIZE_ACTION(4,size);\
    ELEMENT_ALIGNSIZE_ACTION(8,size);\
    ELEMENT_ALIGNSIZE_ACTION(16,size);

#define ELEMENT_ALIGNSIZE_ACTIONS\
    ELEMENT_ALIGNSIZE_ACTION2(0);\
    ELEMENT_ALIGNSIZE_ACTION2(1);\
    ELEMENT_ALIGNSIZE_ACTION2(2);\
    ELEMENT_ALIGNSIZE_ACTION2(4);\
    ELEMENT_ALIGNSIZE_ACTION2(6);\
    ELEMENT_ALIGNSIZE_ACTION2(8);\
    ELEMENT_ALIGNSIZE_ACTION2(10);\
    ELEMENT_ALIGNSIZE_ACTION2(12);\
    ELEMENT_ALIGNSIZE_ACTION2(14);\
    ELEMENT_ALIGNSIZE_ACTION2(16);\
    ELEMENT_ALIGNSIZE_ACTION2(24);\
    ELEMENT_ALIGNSIZE_ACTION2(32);\
    ELEMENT_ALIGNSIZE_ACTION2(48);\
    ELEMENT_ALIGNSIZE_ACTION2(56);\
    ELEMENT_ALIGNSIZE_ACTION2(64);\
    ELEMENT_ALIGNSIZE_ACTION2(72);\
    ELEMENT_ALIGNSIZE_ACTION2(96);\
    ELEMENT_ALIGNSIZE_ACTION2(128);

#endif // QTJAMBI_CONTAINERACCESS_H
