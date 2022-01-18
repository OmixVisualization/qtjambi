/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include "qtjambi_repository_p.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambitypemanager_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_interfaces.h"
#include "qtjambi_functionpointer.h"

#include <cstring>
#include <QThread>
#include <QtCore>

struct MetaData{
    MetaData(int _metaType = QMetaType::UnknownType,
            QHashFunction _hashFunction = nullptr,
            QLessFunction _lessFunction = nullptr,
            QEqualFunction _equalFunction = nullptr)
        : metaType(_metaType),
          hashFunction(_hashFunction),
          lessFunction(_lessFunction),
          equalFunction(_equalFunction)
    {}
    MetaData(const MetaData& d)
        : metaType(d.metaType),
          hashFunction(d.hashFunction),
          lessFunction(d.lessFunction),
          equalFunction(d.equalFunction)
    {}

    int metaType = QMetaType::UnknownType;
    QHashFunction hashFunction;
    QLessFunction lessFunction;
    QEqualFunction equalFunction;
};

struct Storages{
    QThreadStorage<MetaData> storage[2];
};

Q_GLOBAL_STATIC(Storages, gCurrentMetaData)

const MetaData& currentMetaData(uint i)
{
    return gCurrentMetaData->storage[i].localData();
}

void setCurrentMetaData(uint i, const MetaData& metaData)
{
    gCurrentMetaData->storage[i].setLocalData(metaData);
}

void constructContainerElement(uint i, void* ptr, const void* copy)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
//    if(copy)
//        fprintf(stderr, "copy %s from %p at %p\n", QMetaType::typeName(metaData.metaType), copy, ptr);
//    else
//        fprintf(stderr, "creating %s at %p\n", QMetaType::typeName(metaData.metaType), ptr);
    QMetaType::construct(metaData.metaType, ptr, copy);
}
void destructContainerElement(uint i, void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
//    fprintf(stderr, "delete %s at %p\n", QMetaType::typeName(metaData.metaType), ptr);
    QMetaType::destruct(metaData.metaType, ptr);
}

uint containerElementHash(uint i, const void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
    if(metaData.hashFunction){
        return metaData.hashFunction(ptr);
    }
    return 0;
}

bool containerElementEqual(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
    if(metaData.equalFunction){
        return metaData.equalFunction(ptr, ptr2);
    }
    return false;
}

bool containerElementLess(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
    if(metaData.lessFunction){
        return metaData.lessFunction(ptr, ptr2);
    }
    return false;
}

QMetaType::Destructor ContainerConverter::wrapDestructor(int metaType,
                                                        QHashFunction hashFunction,
                                                        QLessFunction lessFunction,
                                                        QEqualFunction equalFunction,
                                                        uint index,
                                                        QMetaType::Destructor destructor,
                                                        uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void(void*)>([index, metaType, hashFunction, lessFunction, equalFunction, destructor](void * ptr){
        MetaData oldMetaData = currentMetaData(index);
        setCurrentMetaData(index, MetaData(metaType, hashFunction, lessFunction, equalFunction));
        destructor(ptr);
        setCurrentMetaData(index, oldMetaData);
    }, hash);
}

QMetaType::Constructor ContainerConverter::wrapConstructor(int metaType,
                                                          QHashFunction hashFunction,
                                                          QLessFunction lessFunction,
                                                          QEqualFunction equalFunction,
                                                          uint index,
                                                          QMetaType::Constructor constructor, uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void*(void*, const void *)>([index, metaType, hashFunction, lessFunction, equalFunction, constructor](void *where, const void * copy)->void *{
        MetaData oldMetaData = currentMetaData(index);
        setCurrentMetaData(index, MetaData(metaType, hashFunction, lessFunction, equalFunction));
        void * result = constructor(where, copy);
        setCurrentMetaData(index, oldMetaData);
        return result;
    }, hash);
}

QMetaType::Destructor ContainerConverter::wrapDestructor(int metaType1,
                                                        QHashFunction hashFunction1,
                                                        QLessFunction lessFunction1,
                                                        QEqualFunction equalFunction1,
                                                        int metaType2,
                                                        QHashFunction hashFunction2,
                                                        QLessFunction lessFunction2,
                                                        QEqualFunction equalFunction2,
                                                        QMetaType::Destructor destructor,
                                                        uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void(void*)>([metaType1, hashFunction1, lessFunction1, equalFunction1,
                                                                          metaType2, hashFunction2, lessFunction2, equalFunction2,
                                                                          destructor](void * ptr){
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        destructor(ptr);
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
    }, hash);
}

QMetaType::Constructor ContainerConverter::wrapConstructor(int metaType1,
                                                          QHashFunction hashFunction1,
                                                          QLessFunction lessFunction1,
                                                          QEqualFunction equalFunction1,
                                                          int metaType2,
                                                          QHashFunction hashFunction2,
                                                          QLessFunction lessFunction2,
                                                          QEqualFunction equalFunction2,
                                                          QMetaType::Constructor constructor, uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void*(void*, const void *)>([metaType1, hashFunction1, lessFunction1, equalFunction1,
                                                                                         metaType2, hashFunction2, lessFunction2, equalFunction2,
                                                                                         constructor](void *where, const void * copy)->void *{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        void * result = constructor(where, copy);
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return result;
    }, hash);
}

int ContainerConverter::registerContainerType(const QString& typeName, size_t containerSize, bool isPointer, int metaType,
                                      QMetaType::Constructor constructor,
                                      QMetaType::Destructor destructor,
                                      HashWrapper hashWrapper,
                                      QEqualFunction equalWrapper)
{
    QHashFunction hashFunction = nullptr;
    QLessFunction lessFunction = nullptr;
    QEqualFunction equalFunction = nullptr;
    findFunction(hashFunction, lessFunction, equalFunction, isPointer, metaType);
    if(!isPointer){
        destructor = wrapDestructor(metaType, hashFunction, lessFunction, equalFunction, 0, destructor, qHash(typeName));
        constructor = wrapConstructor(metaType, hashFunction, lessFunction, equalFunction, 0, constructor, qHash(typeName));
    }
    int newMetaType = QMetaType::registerType(qPrintable(typeName),
                            nullptr,
                            nullptr,
                            destructor,
                            constructor,
                            int(containerSize),
                            QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction,
                            nullptr
                        );
    if(!hashFunctionByMetaType(newMetaType)){
        insertHashFunctionByMetaType(newMetaType,
                                            [metaType, hashFunction, lessFunction, equalFunction, hashWrapper]
                                            (const void* ptr)->uint{
                                                if(ptr){
                                                    MetaData oldMetaData = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
                                                    uint result = hashWrapper(ptr, hashFunction);
                                                    setCurrentMetaData(0, oldMetaData);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }

    if(!equalFunctionByMetaType(newMetaType)){
        insertEqualFunctionByMetaType(newMetaType,
                                            [metaType, hashFunction, lessFunction, equalFunction, equalWrapper]
                                            (const void* ptr1, const void* ptr2)->bool{
                                                if(ptr1 && ptr2){
                                                    MetaData oldMetaData = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
                                                    bool result = equalWrapper(ptr1, ptr2);
                                                    setCurrentMetaData(0, oldMetaData);
                                                    return result;
                                                }
                                                return false;
                                            });
    }
    return newMetaType;
}

int ContainerConverter::registerBiContainerType(const QString& typeName,
                                                size_t containerSize,
                                                bool isPointer1, int metaType1,
                                                bool isPointer2, int metaType2,
                                                QMetaType::Constructor constructor,
                                                QMetaType::Destructor destructor,
                                                HashWrapper2 hashWrapper,
                                                QEqualFunction equalWrapper)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, isPointer1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, isPointer2, metaType2);
    if(!isPointer1 && !isPointer2){
        destructor = wrapDestructor(metaType1, hashFunction1, lessFunction1, equalFunction1,
                                    metaType2, hashFunction2, lessFunction2, equalFunction2,
                                    destructor, qHash(typeName));
        constructor = wrapConstructor(metaType1, hashFunction1, lessFunction1, equalFunction1,
                                      metaType2, hashFunction2, lessFunction2, equalFunction2,
                                      constructor, qHash(typeName));
    }else if(!isPointer1){
        destructor = wrapDestructor(metaType1, hashFunction1, lessFunction1, equalFunction1, 0, destructor, qHash(typeName));
        constructor = wrapConstructor(metaType1, hashFunction1, lessFunction1, equalFunction1, 0, constructor, qHash(typeName));
    }else if(!isPointer2){
        destructor = wrapDestructor(metaType2, hashFunction2, lessFunction2, equalFunction2, 1, destructor, qHash(typeName));
        constructor = wrapConstructor(metaType2, hashFunction2, lessFunction2, equalFunction2, 1, constructor, qHash(typeName));
    }
    int newMetaType = QMetaType::registerType(qPrintable(typeName),
                            nullptr,
                            nullptr,
                            destructor,
                            constructor,
                            int(containerSize),
                            QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction,
                            nullptr
                        );
    if(!hashFunctionByMetaType(newMetaType)){
        insertHashFunctionByMetaType(newMetaType,
                                            [metaType1, hashFunction1, lessFunction1, equalFunction1,
                                             metaType2, hashFunction2, lessFunction2, equalFunction2, hashWrapper]
                                            (const void* ptr)->uint{
                                                if(ptr){
                                                    MetaData oldMetaData1 = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                                                    MetaData oldMetaData2 = currentMetaData(1);
                                                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                                                    uint result = hashWrapper(ptr, hashFunction1, hashFunction2);
                                                    setCurrentMetaData(0, oldMetaData1);
                                                    setCurrentMetaData(1, oldMetaData2);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }

    if(!equalFunctionByMetaType(newMetaType)){
        insertEqualFunctionByMetaType(newMetaType,
                                            [metaType1, hashFunction1, lessFunction1, equalFunction1,
                                             metaType2, hashFunction2, lessFunction2, equalFunction2, equalWrapper]
                                            (const void* ptr1, const void* ptr2)->bool{
                                                if(ptr1 && ptr2){
                                                    MetaData oldMetaData1 = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                                                    MetaData oldMetaData2 = currentMetaData(1);
                                                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                                                    bool result = equalWrapper(ptr1, ptr2);
                                                    setCurrentMetaData(0, oldMetaData1);
                                                    setCurrentMetaData(1, oldMetaData2);
                                                    return result;
                                                }
                                                return false;
                                            });
    }
    return newMetaType;
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter(
                                                                  const ExternalToInternalConverter& memberConverter1, int metaType1, size_t size1,
                                                                  const ExternalToInternalConverter& memberConverter2, int metaType2, size_t size2,
                                                                  QMetaType::Creator creator, QMetaType::Deleter deleter, MapInsertFunction insertFunction)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            creator, deleter, insertFunction]
           (JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        if(scope && !out){
            out = creator(nullptr);
            scope->addFinalAction([out, metaType1, hashFunction1, lessFunction1, equalFunction1,
                                        metaType2, hashFunction2, lessFunction2, equalFunction2, deleter](){
                MetaData oldMetaData1 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                MetaData oldMetaData2 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                deleter(out);
                setCurrentMetaData(0, oldMetaData1);
                setCurrentMetaData(1, oldMetaData2);
            });
        }
        ConvertResponse response = ConvertResponse::NotSuccessful;
        if(out){
            response = ConvertResponse::Successful;
            jobject iter = qtjambi_map_entryset_iterator(env, val.l);
            while(qtjambi_iterator_has_next(env, iter)){
                jobject entry = qtjambi_iterator_next(env, iter);
                jobject _key = qtjambi_map$entry_key(env, entry);
                jobject _val = qtjambi_map$entry_value(env, entry);
                response = insertFunction(out, env, scope, _key, _val, memberConverter1, memberConverter2);
                if(response!=ConvertResponse::Successful){
                    break;
                }
            }
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return response;
    };
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter_QPair(
                                                                  const ExternalToInternalConverter& memberConverter1, int metaType1, size_t size1,
                                                                  const ExternalToInternalConverter& memberConverter2, int metaType2, size_t size2,
                                                                  QMetaType::Creator creator, QMetaType::Deleter deleter, MapInsertFunction insertFunction)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            creator, deleter, insertFunction]
           (JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        if(scope && !out){
            out = creator(nullptr);
            scope->addFinalAction([out, metaType1, hashFunction1, lessFunction1, equalFunction1,
                                        metaType2, hashFunction2, lessFunction2, equalFunction2, deleter](){
                MetaData oldMetaData1 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                MetaData oldMetaData2 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                deleter(out);
                setCurrentMetaData(0, oldMetaData1);
                setCurrentMetaData(1, oldMetaData2);
            });
        }
        ConvertResponse response = ConvertResponse::NotSuccessful;
        if(out){
            jobject key = Java::Private::QtCore::QPair.first(env, val.l);
            jobject snd = Java::Private::QtCore::QPair.second(env, val.l);
            response = insertFunction(out, env, scope, key, snd, memberConverter1, memberConverter2);
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return response;
    };
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter(const ExternalToInternalConverter& memberConverter, int metaType, size_t size,
                                                                               QMetaType::Creator creator, QMetaType::Deleter deleter, ContainerInsertFunction insertFunction)
{
    QHashFunction hashFunction = nullptr;
    QLessFunction lessFunction = nullptr;
    QEqualFunction equalFunction = nullptr;
    findFunction(hashFunction, lessFunction, equalFunction, size, metaType);
    return [memberConverter, metaType, hashFunction, lessFunction, equalFunction, creator, deleter, insertFunction]
           (JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
        MetaData oldMetaData = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
        if(scope && !out){
            out = creator(nullptr);
            scope->addFinalAction([metaType, hashFunction, lessFunction, equalFunction, deleter, out](){
                MetaData oldMetaData = currentMetaData(0);
                setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
                deleter(out);
                setCurrentMetaData(0, oldMetaData);
            });
        }
        if(!out){
            setCurrentMetaData(0, oldMetaData);
            return ConvertResponse::NotSuccessful;
        }
        jobject iter = qtjambi_collection_iterator(env, val.l);
        while(qtjambi_iterator_has_next(env, iter)){
            ConvertResponse response = insertFunction(out, env, scope, qtjambi_iterator_next(env, iter), memberConverter);
            if(response!=ConvertResponse::Successful){
                setCurrentMetaData(0, oldMetaData);
                return response;
            }
        }
        setCurrentMetaData(0, oldMetaData);
        return ConvertResponse::Successful;
    };
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter(const InternalToExternalConverter& memberConverter,
                                                                               int metaType, size_t size, ContainerType containerType,
                                                                               FillJavaCollection fillJavaCollection)
{
    QHashFunction hashFunction = nullptr;
    QLessFunction lessFunction = nullptr;
    QEqualFunction equalFunction = nullptr;
    findFunction(hashFunction, lessFunction, equalFunction, size, metaType);
    return [memberConverter, containerType, metaType, hashFunction, lessFunction, equalFunction, fillJavaCollection]
            (JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
        if(in){
            MetaData oldMetaData = currentMetaData(0);
            setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
            switch(containerType){
            case ContainerType::Deque:
                p->l = Java::Private::Runtime::ArrayDeque.newInstance(env);
                break;
            case ContainerType::List:
                p->l = Java::Private::Runtime::ArrayList.newInstance(env);
                break;
            case ContainerType::Set:
                p->l = Java::Private::Runtime::HashSet.newInstance(env);
                break;
            default:
                p->l = Java::Private::Runtime::LinkedList.newInstance(env);
                break;
            }
            bool result = fillJavaCollection(env, scope, in, p->l, memberConverter);
            setCurrentMetaData(0, oldMetaData);
            return result;
        }
        return true;
    };
}

bool convertKeyValue(JNIEnv* env, QtJambiScope* scope,
        QList<QPair<jobject,jobject>>& elements,
        const void* _key, const void* _value,
        const InternalToExternalConverter& memberConverter1,
        const InternalToExternalConverter& memberConverter2,
        bool* areComparable, int* enumType)
{
    jvalue key;
    key.l = nullptr;
    if(!memberConverter1(env, scope, _key, &key, true)){
        return false;
    }
    jvalue val;
    val.l = nullptr;
    if(!memberConverter2(env, scope, _value, &val, true)){
        return false;
    }
    elements << QPair<jobject,jobject>(key.l, val.l);
    if(areComparable)
        *areComparable &= bool(Java::Private::Runtime::Comparable.isInstanceOf(env, key.l));
    if(enumType){
        if(*enumType>=0){
            if(Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, key.l))
                *enumType = 4;
            else if(Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, key.l))
                *enumType = 2;
            else if(Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, key.l))
                *enumType = 1;
            else if(Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, key.l))
                *enumType = 2;
        }else if(*enumType==1){
            if(!Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }else if(*enumType==2){
            if(!Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }else if(*enumType==4){
            if(!Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }else if(*enumType==8){
            if(!Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }
    }
    return true;
}

bool lessThan(JNIEnv *env, jobject k1, jobject k2);

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter(const InternalToExternalConverter& memberConverter1,
                                                                      int metaType1, size_t size1,
                                                                      const InternalToExternalConverter& memberConverter2,
                                                                      int metaType2, size_t size2,
                                                                      MapType mapType, MapPutMethod mapPutMethod, IterateMap iterateMap)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            mapType, mapPutMethod, iterateMap]
            (JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        bool result = true;
        if(in){
            jint size = 0;
            QList<QPair<jobject,jobject>> elements;
            bool areComparable = true;
            int enumType = 0;
            result = iterateMap(env, scope, elements, size,
                                mapType==MapType::TreeMap ? &areComparable : nullptr,
                                mapType==MapType::TreeMap ? &enumType : nullptr,
                                in, memberConverter1, memberConverter2, &convertKeyValue);

            if(result){
                switch(mapType){
                case MapType::TreeMap:{
                        jobject comparator = nullptr;
                        switch(enumType){
                        case 1:
                            comparator = Java::Private::QtJambi::QtByteEnumerator.comparator(env);
                            break;
                        case 2:
                            comparator = Java::Private::QtJambi::QtShortEnumerator.comparator(env);
                            break;
                        case 4:
                            comparator = Java::Private::QtJambi::QtEnumerator.comparator(env);
                            break;
                        case 8:
                            comparator = Java::Private::QtJambi::QtLongEnumerator.comparator(env);
                            break;
                        default:
                            if(!areComparable){
                                comparator = Java::Private::QtJambi::QtJambiInternal.createComparator(env, jlong(&lessThan));
                            }
                        }
                        if(comparator){
                            p->l = Java::Private::Runtime::TreeMap.newInstance2(env, comparator);
                        }else{
                            p->l = Java::Private::Runtime::TreeMap.newInstance(env);
                        }
                    }
                    break;
                default:
                    p->l = Java::Private::Runtime::HashMap.newInstance(env,size);
                }
                for(const QPair<jobject,jobject>& pair : elements){
                    mapPutMethod(env, p->l, pair.first, pair.second);
                }
            }
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return true;
    };
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter_QPair(const InternalToExternalConverter& memberConverter1,
                                                                      int metaType1, size_t size1,
                                                                      const InternalToExternalConverter& memberConverter2,
                                                                      int metaType2, size_t size2,
                                                                      GetElements getElements)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            getElements]
            (JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        bool result = true;
        if(in){
            const void* _first = nullptr;
            const void* _second = nullptr;
            getElements(in, _first, _second);
            jvalue first;
            first.l = nullptr;
            if(memberConverter1(env, scope, _first, &first, true)){
                jvalue second;
                second.l = nullptr;
                if(memberConverter2(env, scope, _second, &second, true)){
                    p->l = Java::Private::QtCore::QPair.newInstance(env, first.l, second.l);
                }else{
                    result = false;
                }
            }else{
                result = false;
            }
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return result;
    };
}

void ContainerConverter::findFunction(QHashFunction &hashFunction, QLessFunction &lessFunction, QEqualFunction &equalFunction, bool isPointer, int metaType)
{
    if(isPointer){
        hashFunction = &pointerHashFunction;
        lessFunction = &pointerLessFunction;
        equalFunction = &pointerEqualFunction;
    }else{
        if(const std::type_info* typeId = getTypeByMetaType(metaType)){
            hashFunction = registeredHashFunction(*typeId);
            lessFunction = registeredLessFunction(*typeId);
            equalFunction = registeredEqualFunction(*typeId);
        }
        if(!hashFunction)
            hashFunction = hashFunctionByMetaType(metaType);
        if(!lessFunction)
            lessFunction = lessFunctionByMetaType(metaType);
        if(!equalFunction)
            equalFunction = equalFunctionByMetaType(metaType);
    }
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter(const QString& container, const QString& internalTypeName, const InternalToExternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int memberMetaType, ContainerType containerType)
{
    if(isPointer)
        size = 0;
    if(container=="QVector"){
#define ELEMENT_SIZE_CASEACTION(SZ)\
        return ContainerConverter::getInternalToExternalConverter<QVector,0,SZ,true>(internalTypeName, memberConverter, memberMetaType, containerType);
        ELEMENT_SIZE_SWITCH(size)
#undef ELEMENT_SIZE_CASEACTION
    }else if(container=="QStack"){
#define ELEMENT_SIZE_CASEACTION(SZ)\
        return ContainerConverter::getInternalToExternalConverter<QStack,0,SZ,true>(internalTypeName, memberConverter, memberMetaType, containerType);
        ELEMENT_SIZE_SWITCH(size)
#undef ELEMENT_SIZE_CASEACTION
    }else if(container=="QList"){
#define ELEMENT_STATICSIZE_CASEACTION(ST,SZ)\
        return ContainerConverter::getInternalToExternalConverter<QList,0,SZ,ST>(internalTypeName, memberConverter, memberMetaType, containerType);
        ELEMENT_STATICSIZE_SWITCH(isStaticType, size)
#undef ELEMENT_STATICSIZE_CASEACTION
    }else if(container=="QQueue"){
#define ELEMENT_STATICSIZE_CASEACTION(ST,SZ)\
        return ContainerConverter::getInternalToExternalConverter<QQueue,0,SZ,ST>(internalTypeName, memberConverter, memberMetaType, containerType);
        ELEMENT_STATICSIZE_SWITCH(isStaticType, size)
#undef ELEMENT_STATICSIZE_CASEACTION
    }else if(container=="QLinkedList"){
#define ELEMENT_SIZE_CASEACTION(SZ)\
        return ContainerConverter::getInternalToExternalConverter<QLinkedList,0,SZ,true>(internalTypeName, memberConverter, memberMetaType, containerType);
        ELEMENT_SIZE_SWITCH(size)
#undef ELEMENT_SIZE_CASEACTION
    }else if(container=="QSet"){
#define ELEMENT_ALIGNSIZE_CASEACTION(AL,SZ)\
        return ContainerConverter::getInternalToExternalConverter<QSet,AL,SZ,true>(internalTypeName, memberConverter, memberMetaType, containerType);
        ELEMENT_ALIGNSIZE_SWITCH(align,size)
#undef ELEMENT_ALIGNSIZE_CASEACTION
    }
    return nullptr;
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter(const QString& container, const QString& internalTypeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int memberMetaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int memberMetaType2, MapType mapType)
{
    Q_UNUSED(isStaticType1)
    Q_UNUSED(isStaticType2)
    if(isPointer1)
        size1 = 0;
    if(isPointer2)
        size2 = 0;
    if(container=="QMap"){
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
        return ContainerConverter::getInternalToExternalConverter<QMap,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2, mapType);
        ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    }else if(container=="QMultiMap"){
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
        return ContainerConverter::getInternalToExternalConverter<QMultiMap,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2, mapType);
        ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    }else if(container=="QHash"){
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
        return ContainerConverter::getInternalToExternalConverter<QHash,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2, mapType);
        ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    }else if(container=="QMultiHash"){
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
        return ContainerConverter::getInternalToExternalConverter<QMultiHash,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2, mapType);
        ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    }
    return nullptr;
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter_QPair(const QString& container, const QString& internalTypeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int memberMetaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int memberMetaType2)
{
    Q_UNUSED(isStaticType1)
    Q_UNUSED(isStaticType2)
    if(isPointer1)
        size1 = 0;
    if(isPointer2)
        size2 = 0;
    if(container=="QPair"){
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
        return ContainerConverter::getInternalToExternalConverter_QPair<QPair,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2);
        ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    }else if(container=="std::pair"){
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
        return ContainerConverter::getInternalToExternalConverter_QPair<std::pair,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2);
        ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    }
    return nullptr;
}
