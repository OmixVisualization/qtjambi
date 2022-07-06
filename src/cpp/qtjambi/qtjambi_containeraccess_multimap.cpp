#include "qtjambi_core.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_repository_p.h"

AbstractContainerAccess* qtjambi_create_container_access(JNIEnv* env, ContainerType containerType,
                                                         const QMetaType& metaType,
                                                         size_t align, size_t size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                         bool isStaticType,
#endif
                                                         bool isPointer,
                                                         const QHashFunction& hashFunction,
                                                         const InternalToExternalConverter& memberConverter,
                                                         const ExternalToInternalConverter& memberReConverter);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QDebug& debug_stream(QDebug& debug, uint typeId, const void* ptr);
const QtPrivate::AbstractComparatorFunction * qtjambi_registered_comparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId);
bool qtjambi_is_static_type(const QMetaType& metaType);
bool is_equals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
#endif


AutoMultiMapAccess::AutoMultiMapAccess(const AutoMultiMapAccess & other)
    : AbstractMultiMapAccess(), AutoMapAccess(other)
{
}

AutoMultiMapAccess::~AutoMultiMapAccess(){}

AutoMultiMapAccess::AutoMultiMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int keyMetaType,
                size_t keyAlign,
#else
                const QMetaType& keyMetaType,
#endif
                const QHashFunction& keyHashFunction,
                const InternalToExternalConverter& keyInternalToExternalConverter,
                const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int valueMetaType,
                size_t valueAlign,
#else
                const QMetaType& valueMetaType,
#endif
                const QHashFunction& valueHashFunction,
                const InternalToExternalConverter& valueInternalToExternalConverter,
                const ExternalToInternalConverter& valueExternalToInternalConverter
        )
    : AbstractMultiMapAccess(),
      AutoMapAccess(
          keyMetaType,
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          keyAlign,
    #endif
          keyHashFunction,
          keyInternalToExternalConverter,
          keyExternalToInternalConverter,
          valueMetaType,
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          valueAlign,
    #endif
          valueHashFunction,
          valueInternalToExternalConverter,
          valueExternalToInternalConverter)
{
}

void* AutoMultiMapAccess::createContainer() {return AutoMapAccess::createContainer();}
void* AutoMultiMapAccess::copyContainer(const void* container) {return AutoMapAccess::copyContainer(container);}
void AutoMultiMapAccess::assign(void* container, const void* other) {AutoMapAccess::assign(container, other);}
void AutoMultiMapAccess::deleteContainer(void* container) {AutoMapAccess::deleteContainer(container);}
int AutoMultiMapAccess::registerContainer(const QByteArray& containerTypeName) {return AutoMapAccess::registerContainer(containerTypeName);}
void AutoMultiMapAccess::dispose() {delete this;}
void AutoMultiMapAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) {AutoMapAccess::analyzeEntries(container, analyzer, data);}
const QMetaType& AutoMultiMapAccess::keyMetaType() {return AutoMapAccess::keyMetaType();}
const QMetaType& AutoMultiMapAccess::valueMetaType() {return AutoMapAccess::valueMetaType();}
jobject AutoMultiMapAccess::begin(JNIEnv *env, QtJambiNativeID ownerId, const void* container) {return AutoMapAccess::begin(env, ownerId, container);}
void AutoMultiMapAccess::clear(JNIEnv *env, void* container) {AutoMapAccess::clear(env, container);}
jboolean AutoMultiMapAccess::contains(JNIEnv *env, const void* container, jobject key) {return AutoMapAccess::contains(env, container, key);}
jint AutoMultiMapAccess::count(JNIEnv *env, const void* container, jobject key) {return AutoMapAccess::count(env, container, key);}
jobject AutoMultiMapAccess::end(JNIEnv *env, QtJambiNativeID ownerId, const void* container) {return AutoMapAccess::end(env, ownerId, container);}
jobject AutoMultiMapAccess::find(JNIEnv *env, QtJambiNativeID ownerId, const void* container, jobject key) {return AutoMapAccess::find(env, ownerId, container, key);}
jobject AutoMultiMapAccess::first(JNIEnv *env, const void* container) {return AutoMapAccess::first(env, container);}
jobject AutoMultiMapAccess::firstKey(JNIEnv *env, const void* container) {return AutoMapAccess::firstKey(env, container);}

void AutoMultiMapAccess::insert(JNIEnv *env, void* container, jobject key, jobject value){
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;

    detach(map);

    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l);

    Node *n = rootNode(map);
    Node *y = endNode(map);
    bool  left = true;
    while (n) {
        y = n;
        left = !qMapLessThanKey(*n, akey);
        n = left ? n->left : n->right;
    }
    Node* newNode = d->createNode(int(m_size), int(m_align), y, left);
    char* newNodeData = reinterpret_cast<char*>(newNode);
    m_keyMetaType.construct(newNodeData + m_offset1, akey);
    m_valueMetaType.construct(newNodeData + m_offset2);
    assignValue(env, value, newNode);
}

IsBiContainerFunction AutoMultiMapAccess::getIsBiContainerFunction(){
    return qtjambi_is_QMultiMap;
}

jobject AutoMultiMapAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey) { return AutoMapAccess::key(env, container, value, defaultKey); }
jobject AutoMultiMapAccess::keys(JNIEnv *env, const void* container) {return AutoMapAccess::keys(env, container);}
jobject AutoMultiMapAccess::keys(JNIEnv *env, const void* container, jobject value) {return AutoMapAccess::keys(env, container, value);}
jobject AutoMultiMapAccess::last(JNIEnv *env, const void* container) {return AutoMapAccess::last(env, container);}
jobject AutoMultiMapAccess::lastKey(JNIEnv *env, const void* container) {return AutoMapAccess::lastKey(env, container);}
jobject AutoMultiMapAccess::lowerBound(JNIEnv *env, QtJambiNativeID ownerId, const void* container, jobject key) {return AutoMapAccess::lowerBound(env, ownerId, container, key);}
jboolean AutoMultiMapAccess::equal(JNIEnv *env, const void* container, jobject other) {return AutoMapAccess::equal(env, container, other);}
jint AutoMultiMapAccess::remove(JNIEnv *env, void* container, jobject key) {return AutoMapAccess::remove(env, container, key);}
jint AutoMultiMapAccess::size(JNIEnv *env, const void* container) {return AutoMapAccess::size(env, container);}
jobject AutoMultiMapAccess::take(JNIEnv *env, void* container, jobject key) {return AutoMapAccess::take(env, container, key);}
jobject AutoMultiMapAccess::upperBound(JNIEnv *env, QtJambiNativeID ownerId, const void* container, jobject key) {return AutoMapAccess::upperBound(env, ownerId, container, key);}
jobject AutoMultiMapAccess::value(JNIEnv *env, const void* container, jobject key,jobject defaultValue) {return AutoMapAccess::value(env, container, key, defaultValue);}
jobject AutoMultiMapAccess::values(JNIEnv *env, const void* container) {return AutoMapAccess::values(env, container);}
bool AutoMultiMapAccess::keyLessThan(JNIEnv *env, jobject k1, jobject k2) {return AutoMapAccess::keyLessThan(env, k1, k2);}
AbstractMultiMapAccess* AutoMultiMapAccess::clone() {return new AutoMultiMapAccess(*this);}

jobject AutoMultiMapAccess::uniqueKeys(JNIEnv *env, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(
                                                                           env,
                                                                           ContainerType::QList,
                                                                           m_keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_keyAlign,
                                                                           m_keyMetaType.sizeOf(),
                                                                           qtjambi_is_static_type(m_keyMetaType),
#else
                                                                           m_keyMetaType.alignOf(),
                                                                           m_keyMetaType.sizeOf(),
#endif
                                                                           qtjambi_is_pointer_type(m_keyMetaType),
                                                                           m_keyHashFunction,
                                                                           m_keyInternalToExternalConverter,
                                                                           m_keyExternalToInternalConverter
                                                                           ));
    if(listAccess){
        void* listContainer = listAccess->createContainer();
        listAccess = checkContainerAccess(env, listAccess);
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        listAccess->reserve(env, listContainer, d->size);
        while(firstNode != lastNode){
            void* key1 = reinterpret_cast<char*>(firstNode)+m_offset1;
            listAccess->append(env, listContainer, nodeKey(env, firstNode));
            firstNode = firstNode->nextNode();
            while(firstNode != lastNode && !qMapLessThanKey(key1, *firstNode)){
                firstNode = firstNode->nextNode();
            }
        }
        return qtjambi_from_QList(env, listContainer, listAccess);
    }
    return nullptr;
}

void AutoMultiMapAccess::unite(JNIEnv *env, void* container, jobject other)
{
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    if (qtjambi_is_QMultiMap(env, other, keyMetaType(), valueMetaType())
            || qtjambi_is_QMap(env, other, keyMetaType(), valueMetaType())) {
        if(void* ptr = qtjambi_to_object(env, other)){
            QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(ptr);
            Node *firstNode2(beginNode(map2));
            Node *lastNode2(endNode(map2));
            while (firstNode2 != lastNode2) {
                void* key2 = reinterpret_cast<char*>(firstNode2)+m_offset1;
                void* value2 = reinterpret_cast<char*>(firstNode2)+m_offset2;

                Node *n = rootNode(map);
                Node *y = endNode(map);
                bool  left = true;
                while (n) {
                    y = n;
                    left = !qMapLessThanKey(*n, key2);
                    n = left ? n->left : n->right;
                }
                Node* newNode = d->createNode(int(m_size), int(m_align), y, left);
                char* newNodeData = reinterpret_cast<char*>(newNode);
                m_keyMetaType.construct(newNodeData + m_offset1, key2);
                m_valueMetaType.construct(newNodeData + m_offset2, value2);
                firstNode2 = firstNode2->nextNode();
            }
        }
    }else{
        jobject iterator = qtjambi_map_entryset_iterator(env, other);
        while(qtjambi_iterator_has_next(env, iterator)){
            jobject entry = qtjambi_iterator_next(env, iterator);
            jobject value = qtjambi_map$entry_value(env, entry);
            if(Java::Runtime::Collection::isInstanceOf(env, value)){
                jobject iterator2 = qtjambi_collection_iterator(env, value);
                while(qtjambi_iterator_has_next(env, iterator2)){
                    jobject cvalue = qtjambi_iterator_next(env, iterator2);
                    insert(env, container, qtjambi_map$entry_key(env, entry), cvalue);
                }
            }
        }
    }
}

jobject AutoMultiMapAccess::values(JNIEnv *env, const void* container, jobject key)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        if(Node *firstNode = findNode(d, akey)){
            AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(
                                                                                   env,
                                                                                   ContainerType::QList,
                                                                                   m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                   m_valueAlign,
                                                                                   m_valueMetaType.sizeOf(),
                                                                                   qtjambi_is_static_type(m_valueMetaType),
#else
                                                                                   m_valueMetaType.alignOf(),
                                                                                   m_valueMetaType.sizeOf(),
#endif
                                                                                   qtjambi_is_pointer_type(m_valueMetaType),
                                                                                   m_valueHashFunction,
                                                                                   m_valueInternalToExternalConverter,
                                                                                   m_valueExternalToInternalConverter
                                                                                   ));
            if(listAccess){
                void* listContainer = listAccess->createContainer();
                listAccess = checkContainerAccess(env, listAccess);
                Node *lastNode(endNode(map));
                do{
                    listAccess->append(env, listContainer, nodeValue(env, firstNode));
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
                return qtjambi_from_QList(env, listContainer, listAccess);
            }
        }
    }
    return nullptr;
}

jboolean AutoMultiMapAccess::contains(JNIEnv *env, const void* container, jobject key, jobject value) {
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        if(Node *firstNode = findNode(d, akey)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                Node *lastNode(endNode(map));
                do{
                    if(is_equals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        return true;
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
        }
    }
    return false;
}

jint AutoMultiMapAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)
{
    jint c = 0;
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        if(Node *firstNode = findNode(d, akey)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                Node *lastNode(endNode(map));
                do{
                    if(is_equals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        ++c;
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
        }
    }
    return c;
}

jobject AutoMultiMapAccess::find(JNIEnv *env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        if(Node *firstNode = findNode(d, akey)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                Node *lastNode(endNode(map));
                do{
                    if(is_equals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        return createIterator(env, ownerId, new void*(firstNode));
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
        }
    }
    return createIterator(env, ownerId, new void*(endNode(map)));
}

jint AutoMultiMapAccess::remove(JNIEnv *env, void* container, jobject key, jobject value)
{
    jint c = 0;
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
            bool found;
            do{
                found = false;
                if(Node *firstNode = findNode(d, akey)){
                    Node *lastNode(endNode(map));
                    do{
                        if(is_equals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2)){
                            ++c;
                            deleteNode(d, firstNode);
                            found = true;
                            break;
                        }
                        firstNode = firstNode->nextNode();
                    }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
                }
            }while(found);
        }
    }
    return c;
}

void AutoMultiMapAccess::replace(JNIEnv *env, void* container, jobject key, jobject value) {AutoMapAccess::insert(env, container, key, value);}

uint AutoMultiMapAccess::sizeOf() const{
    return uint(sizeof(QMultiMap<char,char>));
}

ushort AutoMultiMapAccess::alignOf() const{
    return ushort(sizeof(QMultiMap<char,char>));
}
