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

#include "pch_p.h"

namespace QtJambiPrivate {

template<typename JArray>
struct BoxedType;

template<>
struct BoxedType<jbyteArray>{
    static constexpr auto primitiveType = &Java::Runtime::Byte::primitiveType;
};

template<>
struct BoxedType<jshortArray>{
    static constexpr auto primitiveType = &Java::Runtime::Short::primitiveType;
};

template<>
struct BoxedType<jintArray>{
    static constexpr auto primitiveType = &Java::Runtime::Integer::primitiveType;
};

template<>
struct BoxedType<jlongArray>{
    static constexpr auto primitiveType = &Java::Runtime::Long::primitiveType;
};

template<>
struct BoxedType<jfloatArray>{
    static constexpr auto primitiveType = &Java::Runtime::Float::primitiveType;
};

template<>
struct BoxedType<jdoubleArray>{
    static constexpr auto primitiveType = &Java::Runtime::Double::primitiveType;
};

template<>
struct BoxedType<jcharArray>{
    static constexpr auto primitiveType = &Java::Runtime::Character::primitiveType;
};

template<>
struct BoxedType<jbooleanArray>{
    static constexpr auto primitiveType = &Java::Runtime::Boolean::primitiveType;
};

}

struct PersistentJArrayPointerPrivate{
    JObjectWrapper m_array;
    jsize m_size;
};

AbstractPersistentJArrayPointer::AbstractPersistentJArrayPointer(JNIEnv *env, jarray array)
    : m_data(new PersistentJArrayPointerPrivate{JObjectWrapper(env, array), array ? env->GetArrayLength(array) : 0})
{
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
}

AbstractPersistentJArrayPointer::~AbstractPersistentJArrayPointer(){
}

jsize AbstractPersistentJArrayPointer::size() const { return m_data ? m_data->m_size : 0; }

jarray AbstractPersistentJArrayPointer::array() const { return m_data ? jarray(jobject(m_data->m_array)) : nullptr; }

bool AbstractPersistentJArrayPointer::isNull() const { return m_data ? m_data->m_array.isNull() : true; }

struct PersistentPointerArrayPrivate{
    JObjectWrapper m_array;
    jsize m_size;
    bool m_isNewArray;
};

AbstractPersistentPointerArray::~AbstractPersistentPointerArray(){}
AbstractPersistentPointerArray::AbstractPersistentPointerArray(JNIEnv *env, jarray array, jsize size, bool isNewArray)
    : d(new PersistentPointerArrayPrivate{JObjectWrapper(env, array), size, isNewArray})
{
}

AbstractPersistentPointerArray::operator bool() const{
    return d && !d->m_array.isNull();
}

jarray AbstractPersistentPointerArray::array() const{
    return jarray(d->m_array);
}
jarray AbstractPersistentPointerArray::array(JNIEnv *env) const{
    return jarray(d->m_array.typedObject<jarray>(env));
}
jsize AbstractPersistentPointerArray::size() const{
    return d->m_size;
}

bool AbstractPersistentPointerArray::isNewArray() const{
    return d->m_isNewArray;
}

namespace QtJambiPrivate {

template<typename JArray, typename ArrayType>
auto findOrCreateArrayImpl(JNIEnv *env, ArrayType* pointer, jsize size){
    if(pointer){
        JArray array = (env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::NewArray)(size);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        (env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::SetArrayRegion)(array, 0, size, pointer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return PointerArrayInfo<JArray,true>{array,size,true};
    }else return PointerArrayInfo<JArray,true>{nullptr,0,false};
}

PointerArrayInfo<jintArray,true> findOrCreateArray(JNIEnv *env, const jint* pointer, jsize size){
    return findOrCreateArrayImpl<jintArray>(env, pointer, size);
}
PointerArrayInfo<jlongArray,true> findOrCreateArray(JNIEnv *env, const jlong* pointer, jsize size){
    return findOrCreateArrayImpl<jlongArray>(env, pointer, size);
}
PointerArrayInfo<jbyteArray,true> findOrCreateArray(JNIEnv *env, const jbyte* pointer, jsize size){
    return findOrCreateArrayImpl<jbyteArray>(env, pointer, size);
}
PointerArrayInfo<jshortArray,true> findOrCreateArray(JNIEnv *env, const jshort* pointer, jsize size){
    return findOrCreateArrayImpl<jshortArray>(env, pointer, size);
}
PointerArrayInfo<jcharArray,true> findOrCreateArray(JNIEnv *env, const jchar* pointer, jsize size){
    return findOrCreateArrayImpl<jcharArray>(env, pointer, size);
}
PointerArrayInfo<jbooleanArray,true> findOrCreateArray(JNIEnv *env, const jboolean* pointer, jsize size){
    return findOrCreateArrayImpl<jbooleanArray>(env, pointer, size);
}
PointerArrayInfo<jfloatArray,true> findOrCreateArray(JNIEnv *env, const jfloat* pointer, jsize size){
    return findOrCreateArrayImpl<jfloatArray>(env, pointer, size);
}
PointerArrayInfo<jdoubleArray,true> findOrCreateArray(JNIEnv *env, const jdouble* pointer, jsize size){
    return findOrCreateArrayImpl<jdoubleArray>(env, pointer, size);
}

}//namespace QtJambiPrivate

template class QTJAMBI_EXPORT PointerArray<false,jbyteArray,false,jbyte,true>;
template class QTJAMBI_EXPORT PointerArray<false,jshortArray,false,jshort,true>;
template class QTJAMBI_EXPORT PointerArray<false,jintArray,false,jint,true>;
template class QTJAMBI_EXPORT PointerArray<false,jlongArray,false,jlong,true>;
template class QTJAMBI_EXPORT PointerArray<false,jcharArray,false,jchar,true>;
template class QTJAMBI_EXPORT PointerArray<false,jfloatArray,false,jfloat,true>;
template class QTJAMBI_EXPORT PointerArray<false,jdoubleArray,false,jdouble,true>;
template class QTJAMBI_EXPORT PointerArray<false,jbooleanArray,false,jboolean,true>;
template class QTJAMBI_EXPORT PointerArray<false,jbyteArray,true,jbyte,true>;
template class QTJAMBI_EXPORT PointerArray<false,jshortArray,true,jshort,true>;
template class QTJAMBI_EXPORT PointerArray<false,jintArray,true,jint,true>;
template class QTJAMBI_EXPORT PointerArray<false,jlongArray,true,jlong,true>;
template class QTJAMBI_EXPORT PointerArray<false,jcharArray,true,jchar,true>;
template class QTJAMBI_EXPORT PointerArray<false,jfloatArray,true,jfloat,true>;
template class QTJAMBI_EXPORT PointerArray<false,jdoubleArray,true,jdouble,true>;
template class QTJAMBI_EXPORT PointerArray<false,jbooleanArray,true,jboolean,true>;

template class QTJAMBI_EXPORT PointerArray<true,jbyteArray,false,jbyte,true>;
template class QTJAMBI_EXPORT PointerArray<true,jshortArray,false,jshort,true>;
template class QTJAMBI_EXPORT PointerArray<true,jintArray,false,jint,true>;
template class QTJAMBI_EXPORT PointerArray<true,jlongArray,false,jlong,true>;
template class QTJAMBI_EXPORT PointerArray<true,jcharArray,false,jchar,true>;
template class QTJAMBI_EXPORT PointerArray<true,jfloatArray,false,jfloat,true>;
template class QTJAMBI_EXPORT PointerArray<true,jdoubleArray,false,jdouble,true>;
template class QTJAMBI_EXPORT PointerArray<true,jbooleanArray,false,jboolean,true>;
template class QTJAMBI_EXPORT PointerArray<true,jbyteArray,true,jbyte,true>;
template class QTJAMBI_EXPORT PointerArray<true,jshortArray,true,jshort,true>;
template class QTJAMBI_EXPORT PointerArray<true,jintArray,true,jint,true>;
template class QTJAMBI_EXPORT PointerArray<true,jlongArray,true,jlong,true>;
template class QTJAMBI_EXPORT PointerArray<true,jcharArray,true,jchar,true>;
template class QTJAMBI_EXPORT PointerArray<true,jfloatArray,true,jfloat,true>;
template class QTJAMBI_EXPORT PointerArray<true,jdoubleArray,true,jdouble,true>;
template class QTJAMBI_EXPORT PointerArray<true,jbooleanArray,true,jboolean,true>;

bool isValidPrimitiveArray(JNIEnv *env, jobject object, jclass contentType){
    if(!object)
        return false;
    jclass arrayClass = env->GetObjectClass(object);
    if(Java::Runtime::Class::isArray(env, arrayClass)){
        jclass componentType = Java::Runtime::Class::getComponentType(env, arrayClass);
        return env->IsSameObject(componentType, contentType);
    }
    return false;
}

#define ArrayPointerINIT(Type)\
    if(m_array && m_size>0){\
        m_array_elements = env->Get##Type##ArrayElements(m_array, &m_is_copy);\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
    }
#define ArrayPointerDEL(Type, arg)\
    if(m_array && m_size>0){\
        m_env->Release##Type##ArrayElements(m_array, m_array_elements, arg);\
        JavaException::check(m_env QTJAMBI_STACKTRACEINFO );\
    }
#define ArrayPointerCOMMIT(Type)\
    if(m_array && m_size>0){\
        m_env->Release##Type##ArrayElements(m_array, m_array_elements, 0);\
        m_array_elements = m_env->Get##Type##ArrayElements(m_array, &m_is_copy);\
        JavaException::check(m_env QTJAMBI_STACKTRACEINFO );\
    }
#define PersistentArrayPointerINIT(Type, type)\
if(m_data && m_data->m_array && m_data->m_size>0){\
        m_array_elements = env->Get##Type##ArrayElements(m_data->m_array.typedObject<j##type##Array>(env), &m_is_copy);\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
}
#define PersistentArrayPointerDEL(Type, type, arg)\
    if(m_data && m_data->m_array && m_data->m_size>0){\
        if(JniEnvironment env{100}){\
            auto _array = m_data->m_array.typedObject<j##type##Array>(env);\
            env->Release##Type##ArrayElements(_array, m_array_elements, arg);\
            JavaException::check(env QTJAMBI_STACKTRACEINFO );\
            m_data->m_array.clear(env);\
        }\
    }
#define PersistentArrayPointerCOMMIT(Type,type)\
    if(m_data && m_data->m_array && m_data->m_size>0){\
        auto _array = m_data->m_array.typedObject<j##type##Array>(env);\
        env->Release##Type##ArrayElements(_array, m_array_elements, 0);\
        m_array_elements = env->Get##Type##ArrayElements(_array, &m_is_copy);\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
    }

#define ArrayPointerStructors(Type,type)\
J##Type##ArrayPointer::J##Type##ArrayPointer(JNIEnv *env, j##type##Array array)\
    : JArrayPointer(env, array) {\
    ArrayPointerINIT(Type)\
}\
PersistentJ##Type##ArrayPointer::PersistentJ##Type##ArrayPointer(JNIEnv *env, j##type##Array array)\
    : PersistentJArrayPointer(env, array) {\
    PersistentArrayPointerINIT(Type,type)\
}\
JConst##Type##ArrayPointer::JConst##Type##ArrayPointer(JNIEnv *env, j##type##Array array)\
    : JArrayPointer(env, array) {\
    ArrayPointerINIT(Type)\
}\
PersistentJConst##Type##ArrayPointer::PersistentJConst##Type##ArrayPointer(JNIEnv *env, j##type##Array array)\
    : PersistentJArrayPointer(env, array) {\
    PersistentArrayPointerINIT(Type,type)\
}\
J##Type##ArrayPointer::~J##Type##ArrayPointer()\
{\
    try{\
        ArrayPointerDEL(Type, 0)\
    } catch (const std::exception& e) {\
        qCWarning(DebugAPI::internalCategory, "%s", e.what());\
    } catch (...) {\
    }\
}\
PersistentJ##Type##ArrayPointer::~PersistentJ##Type##ArrayPointer()\
{\
    try{\
            PersistentArrayPointerDEL(Type, type, 0)\
    } catch (const std::exception& e) {\
            qCWarning(DebugAPI::internalCategory, "%s", e.what());\
    } catch (...) {\
    }\
}\
void J##Type##ArrayPointer::commit()\
{\
    ArrayPointerCOMMIT(Type)\
}\
void PersistentJ##Type##ArrayPointer::commit(JNIEnv *env)\
{\
    PersistentArrayPointerCOMMIT(Type,type)\
}\
JConst##Type##ArrayPointer::~JConst##Type##ArrayPointer()\
{\
    try{\
        ArrayPointerDEL(Type, JNI_ABORT)\
    } catch (const std::exception& e) {\
        qCWarning(DebugAPI::internalCategory, "%s", e.what());\
    } catch (...) {\
    }\
}\
PersistentJConst##Type##ArrayPointer::~PersistentJConst##Type##ArrayPointer()\
{\
    try{\
        PersistentArrayPointerDEL(Type, type, JNI_ABORT)\
    } catch (const std::exception& e) {\
        qCWarning(DebugAPI::internalCategory, "%s", e.what());\
    } catch (...) {\
    }\
}\
const JConst##Type##ArrayPointer::ElementType* JConst##Type##ArrayPointer::pointer() const{\
        return m_array_elements;\
}\
const JConst##Type##ArrayPointer::ElementType& JConst##Type##ArrayPointer::operator[](int index) const{\
    return *(m_array_elements+index);\
}\
J##Type##ArrayPointer::ElementType* J##Type##ArrayPointer::pointer() {\
        return m_array_elements;\
}\
const J##Type##ArrayPointer::ElementType& J##Type##ArrayPointer::operator[](int index) const{\
    return *(m_array_elements+index);\
}\
J##Type##ArrayPointer::ElementType& J##Type##ArrayPointer::operator[](int index){\
    return *(m_array_elements+index);\
}\
const PersistentJConst##Type##ArrayPointer::ElementType* PersistentJConst##Type##ArrayPointer::pointer() const{\
        return m_array_elements;\
}\
const PersistentJConst##Type##ArrayPointer::ElementType& PersistentJConst##Type##ArrayPointer::operator[](int index) const{\
    return *(m_array_elements+index);\
}\
PersistentJ##Type##ArrayPointer::ElementType* PersistentJ##Type##ArrayPointer::pointer() {\
        return m_array_elements;\
}\
const PersistentJ##Type##ArrayPointer::ElementType& PersistentJ##Type##ArrayPointer::operator[](int index) const{\
    return *(m_array_elements+index);\
}\
PersistentJ##Type##ArrayPointer::ElementType& PersistentJ##Type##ArrayPointer::operator[](int index){\
    return *(m_array_elements+index);\
}

#define PointerArrayValid(Type)\
bool JConst##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return isValidPrimitiveArray(env, object, QtJambiPrivate::BoxedType<JArrayType>::primitiveType(env));\
}\
bool J##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return isValidPrimitiveArray(env, object, QtJambiPrivate::BoxedType<JArrayType>::primitiveType(env));\
}\
bool PersistentJConst##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return isValidPrimitiveArray(env, object, QtJambiPrivate::BoxedType<JArrayType>::primitiveType(env));\
}\
bool PersistentJ##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return isValidPrimitiveArray(env, object, QtJambiPrivate::BoxedType<JArrayType>::primitiveType(env));\
}

#define PointerArrayInitializerList(_const,type)\
    const type* array = reinterpret_cast<const type*>(m_array_elements);\
return QtJambiAPI::initializer_list<_const type>(array, size())

#define PointerArrayOperatorImpl(Type,type)\
JConst##Type##ArrayPointer::operator const type* () const { return reinterpret_cast<const type*>(m_array_elements); }\
JConst##Type##ArrayPointer::operator std::initializer_list<type> () const { PointerArrayInitializerList(,type); }\
JConst##Type##ArrayPointer::operator std::initializer_list<const type> () const { PointerArrayInitializerList(const,type); }\
J##Type##ArrayPointer::operator const type* () const { return reinterpret_cast<const type*>(m_array_elements); }\
J##Type##ArrayPointer::operator type* () { return reinterpret_cast<type*>(m_array_elements); }\
J##Type##ArrayPointer::operator std::initializer_list<type> () const { PointerArrayInitializerList(,type); }\
J##Type##ArrayPointer::operator std::initializer_list<const type> () const { PointerArrayInitializerList(const,type); }\
PersistentJConst##Type##ArrayPointer::operator const type* () const { return reinterpret_cast<const type*>(m_array_elements); }\
PersistentJConst##Type##ArrayPointer::operator std::initializer_list<type> () const { PointerArrayInitializerList(,type); }\
PersistentJConst##Type##ArrayPointer::operator std::initializer_list<const type> () const { PointerArrayInitializerList(const,type); }\
PersistentJ##Type##ArrayPointer::operator const type* () const { return reinterpret_cast<const type*>(m_array_elements); }\
PersistentJ##Type##ArrayPointer::operator type* () { return reinterpret_cast<type*>(m_array_elements); }\
PersistentJ##Type##ArrayPointer::operator std::initializer_list<type> () const { PointerArrayInitializerList(,type); }\
PersistentJ##Type##ArrayPointer::operator std::initializer_list<const type> () const { PointerArrayInitializerList(const,type); }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#define PointerArrayOperator(Type,type)\
PointerArrayOperatorImpl(Type,type)\
JConst##Type##ArrayPointer::operator QSpan<const type> () const { return m_array_elements ? QSpan<const type>(reinterpret_cast<const type*>(m_array_elements), reinterpret_cast<const type*>(m_array_elements)+size()) : QSpan<const type>(); }\
J##Type##ArrayPointer::operator QSpan<const type> () const { return m_array_elements ? QSpan<const type>(reinterpret_cast<const type*>(m_array_elements), reinterpret_cast<const type*>(m_array_elements)+size()) : QSpan<const type>(); }\
J##Type##ArrayPointer::operator QSpan<type> () { return m_array_elements ? QSpan<type>(reinterpret_cast<type*>(m_array_elements), reinterpret_cast<type*>(m_array_elements)+size()) : QSpan<type>(); }\
PersistentJConst##Type##ArrayPointer::operator QSpan<const type> () const { return m_array_elements ? QSpan<const type>(reinterpret_cast<const type*>(m_array_elements), reinterpret_cast<const type*>(m_array_elements)+size()) : QSpan<const type>(); }\
PersistentJ##Type##ArrayPointer::operator QSpan<const type> () const { return m_array_elements ? QSpan<const type>(reinterpret_cast<const type*>(m_array_elements), reinterpret_cast<const type*>(m_array_elements)+size()) : QSpan<const type>(); }\
PersistentJ##Type##ArrayPointer::operator QSpan<type> () { return m_array_elements ? QSpan<type>(reinterpret_cast<type*>(m_array_elements), reinterpret_cast<type*>(m_array_elements)+size()) : QSpan<type>(); }
#else
#define PointerArrayOperator(Type,type)\
    PointerArrayOperatorImpl(Type,type)
#endif

#define PointerArrayOperators_1(Type)\
    PointerArrayValid(Type)
#define PointerArrayOperators_2(Type, type1)\
    PointerArrayValid(Type)\
    PointerArrayOperator(Type,type1)
#define PointerArrayOperators_3(Type, type1, type2)\
    PointerArrayValid(Type)\
    PointerArrayOperator(Type,type1)\
    PointerArrayOperator(Type,type2)
#define PointerArrayOperators_4(Type, type1, type2, type3)\
    PointerArrayValid(Type)\
    PointerArrayOperator(Type,type1)\
    PointerArrayOperator(Type,type2)\
    PointerArrayOperator(Type,type3)
#define PointerArrayOperators_5(Type, type1, type2, type3, type4)\
    PointerArrayValid(Type)\
    PointerArrayOperator(Type,type1)\
    PointerArrayOperator(Type,type2)\
    PointerArrayOperator(Type,type3)\
    PointerArrayOperator(Type,type4)
#define PointerArrayOperators_6(Type, type1, type2, type3, type4, type5)\
    PointerArrayValid(Type)\
    PointerArrayOperator(Type,type1)\
    PointerArrayOperator(Type,type2)\
    PointerArrayOperator(Type,type3)\
    PointerArrayOperator(Type,type4)\
    PointerArrayOperator(Type,type5)

#define PointerArrayOperators(...) QTJAMBI_OVERLOADED_MACRO(PointerArrayOperators, __VA_ARGS__)

#define PointerArrayDeclaration(Type, type, ...)\
    ArrayPointerStructors(Type, type)\
    PointerArrayOperators(Type, __VA_ARGS__)

PointerArrayDeclaration(Byte, byte, char, qint8, quint8, std::byte )
PointerArrayDeclaration(Int, int, int, uint, char32_t )
PointerArrayDeclaration(Long, long, qint64, quint64)
PointerArrayDeclaration(Float, float, float)
PointerArrayDeclaration(Double, double, double)
PointerArrayDeclaration(Short, short, short)
PointerArrayDeclaration(Char, char, qint16, quint16, wchar_t, QChar, char16_t )

JConstByteArrayPointer::operator QByteArrayView() const { return QByteArrayView(reinterpret_cast<const char*>(m_array_elements), size()); }
JByteArrayPointer::operator QByteArrayView() const { return QByteArrayView(reinterpret_cast<const char*>(m_array_elements), size()); }
JConstCharArrayPointer::operator QStringView() const { return QStringView(reinterpret_cast<const QChar*>(m_array_elements), size()); }
JCharArrayPointer::operator QStringView() const { return QStringView(reinterpret_cast<const QChar*>(m_array_elements), size()); }
PersistentJConstByteArrayPointer::operator QByteArrayView() const { return QByteArrayView(reinterpret_cast<const char*>(m_array_elements), size()); }
PersistentJByteArrayPointer::operator QByteArrayView() const { return QByteArrayView(reinterpret_cast<const char*>(m_array_elements), size()); }
PersistentJConstCharArrayPointer::operator QStringView() const { return QStringView(reinterpret_cast<const QChar*>(m_array_elements), size()); }
PersistentJCharArrayPointer::operator QStringView() const { return QStringView(reinterpret_cast<const QChar*>(m_array_elements), size()); }
JConstByteArrayPointer::operator QByteArray() const { return QByteArray(reinterpret_cast<const char*>(m_array_elements), size()); }
JByteArrayPointer::operator QByteArray() const { return QByteArray(reinterpret_cast<const char*>(m_array_elements), size()); }
JConstCharArrayPointer::operator QString() const { return QString(reinterpret_cast<const QChar*>(m_array_elements), size()); }
JCharArrayPointer::operator QString() const { return QString(reinterpret_cast<const QChar*>(m_array_elements), size()); }
PersistentJConstByteArrayPointer::operator QByteArray() const { return QByteArray(reinterpret_cast<const char*>(m_array_elements), size()); }
PersistentJByteArrayPointer::operator QByteArray() const { return QByteArray(reinterpret_cast<const char*>(m_array_elements), size()); }
PersistentJConstCharArrayPointer::operator QString() const { return QString(reinterpret_cast<const QChar*>(m_array_elements), size()); }
PersistentJCharArrayPointer::operator QString() const { return QString(reinterpret_cast<const QChar*>(m_array_elements), size()); }

#undef ArrayPointerINIT
#undef PersistentArrayPointerINIT
#undef ArrayPointerDEL
#undef ArrayPointerCOMMIT
#undef PersistentArrayPointerDEL
#undef PersistentArrayPointerCOMMIT
#define ArrayPointerINIT(Type)\
if(m_array && m_size>0){\
    m_boolean_array = env->Get##Type##ArrayElements(m_array, &m_is_copy);\
    JavaException::check(env QTJAMBI_STACKTRACEINFO );\
    if(sizeof(jboolean)==sizeof(bool)){\
        m_array_elements = reinterpret_cast<bool*>(m_boolean_array);\
    }else{\
        m_array_elements = new bool[size_t(m_size)];\
        for (int i= 0; i < m_size; ++i) {\
            m_array_elements[i] = m_boolean_array[i]==JNI_TRUE;\
        }\
    }\
}
#define PersistentArrayPointerINIT(Type, type)\
    if(m_data && m_data->m_array && m_data->m_size>0){\
        auto _array = m_data->m_array.typedObject<j##type##Array>(env);\
        m_boolean_array = env->Get##Type##ArrayElements(_array, &m_is_copy);\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
        if(sizeof(jboolean)==sizeof(bool)){\
            m_array_elements = reinterpret_cast<bool*>(m_boolean_array);\
    }else{\
            m_array_elements = new bool[size_t(m_data->m_size)];\
            for (int i= 0; i < m_data->m_size; ++i) {\
                m_array_elements[i] = m_boolean_array[i]==JNI_TRUE;\
        }\
    }\
}

#define ArrayPointerDEL_INIT_JNI_ABORT()
#define PersistentArrayPointerDEL_INIT_JNI_ABORT()
#define ArrayPointerDEL_INIT_0()\
    if(sizeof(jboolean)!=sizeof(bool)){\
        for (int i= 0; i < m_size; ++i) {\
            m_boolean_array[i] = m_array_elements[i] ? JNI_TRUE : JNI_FALSE;\
        }\
    }
#define PersistentArrayPointerDEL_INIT_0()\
if(sizeof(jboolean)!=sizeof(bool)){\
        for (int i= 0; i < m_data->m_size; ++i) {\
            m_boolean_array[i] = m_array_elements[i] ? JNI_TRUE : JNI_FALSE;\
    }\
}

#define ArrayPointerDEL(Type, arg)\
try{\
    if(m_array && m_size>0){\
        ArrayPointerDEL_INIT_##arg()\
        m_env->ReleaseBooleanArrayElements(m_array, m_boolean_array, arg);\
        JavaException::check(m_env QTJAMBI_STACKTRACEINFO );\
        if(sizeof(jboolean)!=sizeof(bool)){\
            delete[] m_array_elements;\
        }\
    }\
} catch (const std::exception& e) {\
        qCWarning(DebugAPI::internalCategory, "%s", e.what());\
} catch (...) {\
}

#define ArrayPointerCOMMIT(Type)\
if(m_array && m_size>0){\
    ArrayPointerDEL_INIT_0()\
    m_env->Release##Type##ArrayElements(m_array, m_boolean_array, 0);\
    m_boolean_array = m_env->Get##Type##ArrayElements(m_array, &m_is_copy);\
    JavaException::check(m_env QTJAMBI_STACKTRACEINFO );\
}

#define PersistentArrayPointerDEL(Type, type, arg)\
    try{\
            if(m_data && m_data->m_array && m_data->m_size>0){\
                if(DefaultJniEnvironment env{100}){\
                    PersistentArrayPointerDEL_INIT_##arg()\
                    auto _array = m_data->m_array.typedObject<j##type##Array>(env);\
                    env->ReleaseBooleanArrayElements(_array, m_boolean_array, arg);\
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );\
                    m_data->m_array.clear(env);\
                }\
                if(sizeof(jboolean)!=sizeof(bool)){\
                    delete[] m_array_elements;\
            }\
        }\
    } catch (const std::exception& e) {\
            qCWarning(DebugAPI::internalCategory, "%s", e.what());\
    } catch (...) {\
    }

#define PersistentArrayPointerCOMMIT(Type,type)\
if(m_data && m_data->m_array && m_data->m_size>0){\
    PersistentArrayPointerDEL_INIT_0()\
    auto _array = m_data->m_array.typedObject<j##type##Array>(env);\
    env->Release##Type##ArrayElements(_array, m_boolean_array, 0);\
    m_boolean_array = env->Get##Type##ArrayElements(_array, &m_is_copy);\
    JavaException::check(env QTJAMBI_STACKTRACEINFO );\
}

PointerArrayDeclaration(Boolean, boolean, bool)

jboolean* JBooleanArrayPointer::booleanArray(){
    return m_boolean_array;
}

const jboolean* JBooleanArrayPointer::booleanArray() const{
    return m_boolean_array;
}

jboolean* PersistentJBooleanArrayPointer::booleanArray(){
    return m_boolean_array;
}

const jboolean* PersistentJBooleanArrayPointer::booleanArray() const{
    return m_boolean_array;
}

#undef ArrayPointerStructors
#undef ArrayPointerINIT
#undef ArrayPointerDEL
#undef PersistentArrayPointerDEL
