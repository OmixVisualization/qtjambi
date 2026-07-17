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

#if !defined(QTJAMBIAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_H

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QException>
#include <QtCore/QMetaEnum>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaProperty>
#include <typeinfo>

#include "jnienvironment.h"
#include "debugapi.h"
#include "qtjambishell.h"
#include "exception.h"
#include "javainvalidate.h"
#include "typetests.h"
#include "scope.h"
#include "qtjambiapi_nativeid.h"
#include "qtjambiapi_ownership.h"
#include "qtjambiapi_thread.h"

QT_WARNING_DISABLE_CLANG("-Wshift-count-overflow")

#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
#define unique_id(id) qHash(QLatin1String((id).name()))
#define typeid_equals(t1, t2) unique_id(t1)==unique_id(t2)
#define typeid_not_equals(t1, t2) unique_id(t1)!=unique_id(t2)
#else
#define unique_id(id) (id).hash_code()
#define typeid_equals(t1, t2) t1==t2
#define typeid_not_equals(t1, t2) t1!=t2
#endif

class QtJambiScope;

namespace QtJambiAPI{
typedef const std::type_info* (*TypeInfoSupplier)(const void *object);
void QTJAMBI_EXPORT checkNullPointer(JNIEnv *env, const void* ptr, const std::type_info& typeId);
void QTJAMBI_EXPORT checkNullPointer(JNIEnv *env, const void* ptr, const std::type_info& typeId, TypeInfoSupplier typeInfoSupplier);
void QTJAMBI_EXPORT checkDanglingPointer(JNIEnv *env, const void* ptr, const std::type_info& typeId, TypeInfoSupplier typeInfoSupplier);
}//QtJambiAPI

namespace QtJambiPrivate{
template<typename T, bool = std::is_polymorphic<T>::value>
struct CheckPointer{
    static const std::type_info* supplyType(const void *ptr) {
        const T* object = reinterpret_cast<const T*>(ptr);
        try{
            const std::type_info* typeId = &typeid(*object);
            if(!typeId)
                typeId = &typeid(T);
            return typeId;
        }catch(const std::bad_typeid&){
            return nullptr;
        }catch(...){
            return nullptr;
        }
    };
    static const std::type_info* trySupplyType(const T *object) {
        try{
            const std::type_info* typeId = &typeid(*object);
            return typeId;
        }catch(const std::bad_typeid&){
            return nullptr;
        }catch(...){
            return nullptr;
        }
    };
    static void checkNullPointer(JNIEnv *env, const T* ptr){
        QtJambiAPI::checkNullPointer(env, ptr, typeid(T), supplyType);
    }
    static void checkDanglingPointer(JNIEnv *env, const T* ptr){
        QtJambiAPI::checkDanglingPointer(env, ptr, typeid(T), supplyType);
    }
};
template<typename T>
struct CheckPointer<T,false>{
    static constexpr QtJambiAPI::TypeInfoSupplier supplyType = nullptr;
    static void checkNullPointer(JNIEnv *env, const T* ptr){
        QtJambiAPI::checkNullPointer(env, ptr, typeid(T));
    }
    static void checkDanglingPointer(JNIEnv *, const T*){
    }
};
}//QtJambiPrivate

namespace QtJambiAPI{
template<typename T>
void deletePointer(void* ptr,bool) { delete reinterpret_cast<T*>(ptr); }

template<typename T>
void checkNullPointer(JNIEnv *env, const T* ptr)
{
    QtJambiPrivate::CheckPointer<T>::checkNullPointer(env, ptr);
}

template<typename T>
void checkDanglingPointer(JNIEnv *env, const T* ptr)
{
    QtJambiPrivate::CheckPointer<T>::checkDanglingPointer(env, ptr);
}

template<class T>
T& checkedAddressOf(JNIEnv *env, T * ptr)
{
    QtJambiPrivate::CheckPointer<T>::checkNullPointer(env, ptr);
    return *ptr;
}

QTJAMBI_EXPORT bool enumValue(JNIEnv *env, jobject java_object, void* ptr, size_t size);

template<typename I = int>
I enumValue(JNIEnv *env, jobject object){
    I i{};
    enumValue(env, object, &i, sizeof(I));
    return i;
}

typedef const void* (*DefaultValueCreator)();
QTJAMBI_EXPORT const void* getDefaultValue(const std::type_info& type_info, DefaultValueCreator creator);

template<typename T>
const T& getDefaultValue(){
    return *reinterpret_cast<const T*>(getDefaultValue(typeid(T), []()->const void*{return new T();}));
}

QTJAMBI_EXPORT void setFlagsValue(JNIEnv *env, jobject flagsObject, jint value);
#if QT_VERSION >= QT_VERSION_CHECK(6,9,0)
QTJAMBI_EXPORT void setFlagsValue(JNIEnv *env, jobject flagsObject, jlong value);
#endif

}//namespace QtJambiAPI

namespace QtJambiAPI{
QTJAMBI_EXPORT jobject newQPair(JNIEnv *env, jobject first, jobject second);
QTJAMBI_EXPORT jobject getQPairFirst(JNIEnv *env, jobject pair);
QTJAMBI_EXPORT jobject getQPairSecond(JNIEnv *env, jobject pair);

QTJAMBI_EXPORT jobject newJavaHashSet(JNIEnv *env);
QTJAMBI_EXPORT jobject newJavaHashMap(JNIEnv *env, int size = 0);
QTJAMBI_EXPORT jobject newJavaTreeMap(JNIEnv *env);

QTJAMBI_EXPORT void putJavaMap(JNIEnv *env, jobject map, jobject key, jobject val);
QTJAMBI_EXPORT void putJavaMultiMap(JNIEnv *env, jobject map, jobject key, jobject val);
QTJAMBI_EXPORT void clearJavaMap(JNIEnv *env, jobject map);
QTJAMBI_EXPORT jobject entrySetIteratorOfJavaMap(JNIEnv *env, jobject map);
QTJAMBI_EXPORT jobject keyOfJavaMapEntry(JNIEnv *env, jobject entry);
QTJAMBI_EXPORT jobject valueOfJavaMapEntry(JNIEnv *env, jobject entry);
QTJAMBI_EXPORT jobject newJavaArrayList(JNIEnv *env, int size = 0);

QTJAMBI_EXPORT void addToJavaCollection(JNIEnv *env, jobject list, jobject obj);
QTJAMBI_EXPORT void addAllToJavaCollection(JNIEnv *env, jobject list, jobject obj);
QTJAMBI_EXPORT void clearJavaCollection(JNIEnv *env, jobject collection);
QTJAMBI_EXPORT int sizeOfJavaCollection(JNIEnv *env, jobject col);
QTJAMBI_EXPORT jobject iteratorOfJavaIterable(JNIEnv *env, jobject col);
QTJAMBI_EXPORT jobject nextOfJavaIterator(JNIEnv *env, jobject col);
QTJAMBI_EXPORT bool hasJavaIteratorNext(JNIEnv *env, jobject col);
QTJAMBI_EXPORT void setAtJavaList(JNIEnv *env, jobject list, int index, jobject obj);
QTJAMBI_EXPORT jobject getAtJavaList(JNIEnv *env, jobject list, int index);

QTJAMBI_EXPORT jobject findObject(JNIEnv *env, const void * pointer);
QTJAMBI_EXPORT jobject findObject(JNIEnv *env, const void * pointer, const std::type_info& typeId);
QTJAMBI_EXPORT jobject findObject(JNIEnv *env, const QObject* pointer);
template<typename T>
jobject findObject(JNIEnv *env, const T* pointer){
    if constexpr(std::is_base_of_v<QObject, T>){
        return findObject(env, static_cast<const QObject*>(pointer));
    }else{
        return findObject(env, pointer, typeid(T));
    }
}

class DeclarativeUtil{
static QTJAMBI_EXPORT void reportDestruction(QObject * obj);
template<typename T>
friend class DeclarativeShellElement;
template<typename T>
friend class DeclarativeElement;
};

template<typename T>
class DeclarativeShellElement final : public T
{
public:
    template<typename... Args>
    DeclarativeShellElement(Args... args) : T(std::forward(args)...) {}
    ~DeclarativeShellElement() override {
        DeclarativeUtil::reportDestruction(this);
    }
    static void operator delete(void * ptr) Q_DECL_NOTHROW{
        T::operator delete(ptr);
    }
};

template<typename T>
class DeclarativeElement final : public T
{
public:
    DeclarativeElement() : T() {}
    ~DeclarativeElement() override {
        DeclarativeUtil::reportDestruction(this);
    }
};

QTJAMBI_EXPORT void registerNonShellDeletion(void* ptr);

QTJAMBI_EXPORT uint getJavaObjectHashCode(JNIEnv *env, jobject object);

QTJAMBI_EXPORT const QObject* mainThreadOwner(const void *);

QTJAMBI_EXPORT const QObject* getPixmapOwner(const void *);

QTJAMBI_EXPORT QMetaObject::Connection connect(const QObject *sender, const char *signal,
                                const QObject *receiver, const char *member, Qt::ConnectionType = Qt::AutoConnection);
QTJAMBI_EXPORT QMetaObject::Connection connect(const QObject *sender, const QMetaMethod &signal,
                        const QObject *receiver, const QMetaMethod &method,
                        Qt::ConnectionType type = Qt::AutoConnection);

QTJAMBI_EXPORT void putReferenceCount(JNIEnv *__jni_env, jobject owner, jclass declaringClass, jstring fieldName, bool isThreadSafe, bool isStatic, jobject key, jobject value);
QTJAMBI_EXPORT void setReferenceCount(JNIEnv *__jni_env, jobject owner, jclass declaringClass, jstring fieldName, bool isThreadSafe, bool isStatic, jobject value);
QTJAMBI_EXPORT void addAllReferenceCount(JNIEnv *__jni_env, jobject owner, jclass declaringClass, jstring fieldName, bool isThreadSafe, bool isStatic, jobject values);
QTJAMBI_EXPORT void addReferenceCount(JNIEnv *__jni_env, jobject owner, jclass declaringClass, jstring fieldName, bool isThreadSafe, bool isStatic, jobject value);
QTJAMBI_EXPORT void copyReferenceCount(JNIEnv *__jni_env, jobject owner, jclass declaringClass, jstring fieldName, jobject copy);

template<class O, class T, size_t N>
void copyArrayInto(JNIEnv *env, O javaArray, T(&nativeArray)[N]){
    Q_STATIC_ASSERT_X(QtJambiPrivate::is_jni_array_type_v<O>, "qtjambi_copy_into can only be used for java array types");
    jsize size{0};
    if((size = javaArray ? env->GetArrayLength(javaArray) : 0) != N)
        JavaException::raiseIllegalArgumentException(env, QString("Wrong number of elements in array. Found: %1, expected: %2").arg(size).arg(N) QTJAMBI_STACKTRACEINFO);
    if constexpr(QtJambiPrivate::is_jni_primitive_array_type_v<O>){
        if constexpr(std::is_same_v<O, jbooleanArray> && sizeof(T)!=sizeof(jboolean)){
            jboolean buffer[N];
            (env->*QtJambiPrivate::jni_primitive_array_functions<O>::GetArrayRegion)(javaArray, 0, jsize(N), buffer);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(size_t i=0; i<N; ++i){
                nativeArray[i] = buffer[i];
            }
        }else{
            Q_STATIC_ASSERT_X(sizeof(T)==sizeof(QtJambiPrivate::jni_array_element_type_t<O>), "array element size mismatch");
            (env->*QtJambiPrivate::jni_primitive_array_functions<O>::GetArrayRegion)(javaArray, 0, jsize(N), reinterpret_cast<QtJambiPrivate::jni_array_element_type_t<O>*>(nativeArray));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }else{
        for(size_t i=0; i<N; ++i){
            jobject element = env->GetObjectArrayElement(javaArray, jsize(i));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            nativeArray[i] = qtjambi_cast<T>(env, element);
        }
    }
}

} // namespace QtJambiAPI

inline bool operator<(const QVariant& v1, const QVariant& v2){
    if(v1.userType()==v2.userType()){
        QPartialOrdering result = QMetaType(v1.userType()).compare(v1.data(), v2.data());
        return result==QPartialOrdering::Less;
    }
    return false;
}

template<class T>
const T& reinterpret_value_cast(const void * ptr)
{
    const T* _ptr = reinterpret_cast<const T*>(ptr);
    if(_ptr)
    return *_ptr;
    return QtJambiAPI::getDefaultValue<T>();
}

template<class T>
const T& reinterpret_deref_cast(JNIEnv *env, const void * ptr)
{
    return QtJambiAPI::checkedAddressOf<const T>(env, reinterpret_cast<const T*>(ptr));
}

template<class T>
T& reinterpret_deref_cast(JNIEnv *env, void * ptr)
{
    return QtJambiAPI::checkedAddressOf<T>(env, reinterpret_cast<T*>(ptr));
}

#endif // QTJAMBIAPI_H
