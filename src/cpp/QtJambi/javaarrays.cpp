/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambiapi.h"
#include "javaarrays.h"
#include "java_p.h"
#include "utils_p.h"
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#include <QtCore/QSpan>
#endif

#define PointerArrayINIT(Type,type)\
     : PointerArray(env, pointer, env->New##Type##Array(pointer ? size : 0), pointer ? size : 0) {\
    if(pointer){\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
        env->Set##Type##ArrayRegion(array(), 0, size, reinterpret_cast<const j##type *>(pointer));\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
    }\
}

#define PointerArrayDEL(Type,type)\
{\
    if(m_array){\
    if(JniEnvironment env{100}){\
            env->Get##Type##ArrayRegion(m_array, 0, m_size, reinterpret_cast<j##type *>(m_pointer));\
            JavaException::check(env QTJAMBI_STACKTRACEINFO );\
        }\
    }\
}

CharPointerArray::CharPointerArray(JNIEnv *env, char* pointer, jsize size) PointerArrayINIT(Byte,byte)

CharPointerArray::~CharPointerArray() PointerArrayDEL(Byte,byte)

WCharPointerArray::WCharPointerArray(JNIEnv *env, ushort* pointer, jsize size) PointerArrayINIT(Char,char)

WCharPointerArray::~WCharPointerArray() PointerArrayDEL(Char,char)

QCharPointerArray::QCharPointerArray(JNIEnv *env, QChar* pointer, jsize size) PointerArrayINIT(Char,char)

QCharPointerArray::~QCharPointerArray() PointerArrayDEL(Char,char)

DoublePointerArray::DoublePointerArray(JNIEnv *env, double* pointer, jsize size) PointerArrayINIT(Double,double)

DoublePointerArray::~DoublePointerArray() PointerArrayDEL(Double,double)

FloatPointerArray::FloatPointerArray(JNIEnv *env, float* pointer, jsize size) PointerArrayINIT(Float,float)

FloatPointerArray::~FloatPointerArray() PointerArrayDEL(Float,float)

Int8PointerArray::Int8PointerArray(JNIEnv *env, qint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

Int8PointerArray::~Int8PointerArray() PointerArrayDEL(Byte,byte)

Int16PointerArray::Int16PointerArray(JNIEnv *env, qint16* pointer, jsize size) PointerArrayINIT(Short,short)

Int16PointerArray::~Int16PointerArray() PointerArrayDEL(Short,short)

Int32PointerArray::Int32PointerArray(JNIEnv *env, qint32* pointer, jsize size) PointerArrayINIT(Int,int)

Int32PointerArray::~Int32PointerArray() PointerArrayDEL(Int,int)

Int64PointerArray::Int64PointerArray(JNIEnv *env, qint64* pointer, jsize size) PointerArrayINIT(Long,long)

Int64PointerArray::~Int64PointerArray() PointerArrayDEL(Long,long)

UInt8PointerArray::UInt8PointerArray(JNIEnv *env, quint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

UInt8PointerArray::~UInt8PointerArray() PointerArrayDEL(Byte,byte)

UInt16PointerArray::UInt16PointerArray(JNIEnv *env, quint16* pointer, jsize size) PointerArrayINIT(Short,short)

UInt16PointerArray::~UInt16PointerArray() PointerArrayDEL(Short,short)

UInt32PointerArray::UInt32PointerArray(JNIEnv *env, quint32* pointer, jsize size) PointerArrayINIT(Int,int)

UInt32PointerArray::~UInt32PointerArray() PointerArrayDEL(Int,int)

UInt64PointerArray::UInt64PointerArray(JNIEnv *env, quint64* pointer, jsize size) PointerArrayINIT(Long,long)

UInt64PointerArray::~UInt64PointerArray() PointerArrayDEL(Long,long)

BoolPointerArray::BoolPointerArray(JNIEnv *env, bool* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

BoolPointerArray::~BoolPointerArray() PointerArrayDEL(Boolean,boolean)

Bool2PointerArray::Bool2PointerArray(JNIEnv *env, uchar* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

Bool2PointerArray::~Bool2PointerArray() PointerArrayDEL(Boolean,boolean)

ConstCharPointerArray::ConstCharPointerArray(JNIEnv *env, const char* pointer, jsize size) PointerArrayINIT(Byte,byte)

ConstInt8PointerArray::ConstInt8PointerArray(JNIEnv *env, const qint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

ConstInt16PointerArray::ConstInt16PointerArray(JNIEnv *env, const qint16* pointer, jsize size) PointerArrayINIT(Short,short)

ConstInt32PointerArray::ConstInt32PointerArray(JNIEnv *env, const qint32* pointer, jsize size) PointerArrayINIT(Int,int)

ConstInt64PointerArray::ConstInt64PointerArray(JNIEnv *env, const qint64* pointer, jsize size) PointerArrayINIT(Long,long)

ConstUInt8PointerArray::ConstUInt8PointerArray(JNIEnv *env, const quint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

ConstUInt16PointerArray::ConstUInt16PointerArray(JNIEnv *env, const quint16* pointer, jsize size) PointerArrayINIT(Short,short)

ConstUInt32PointerArray::ConstUInt32PointerArray(JNIEnv *env, const quint32* pointer, jsize size) PointerArrayINIT(Int,int)

ConstUInt64PointerArray::ConstUInt64PointerArray(JNIEnv *env, const quint64* pointer, jsize size) PointerArrayINIT(Long,long)

ConstBoolPointerArray::ConstBoolPointerArray(JNIEnv *env, const bool* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

ConstBool2PointerArray::ConstBool2PointerArray(JNIEnv *env, const uchar* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

ConstWCharPointerArray::ConstWCharPointerArray(JNIEnv *env, const ushort* pointer, jsize size) PointerArrayINIT(Char,char)

ConstDoublePointerArray::ConstDoublePointerArray(JNIEnv *env, const double* pointer, jsize size) PointerArrayINIT(Double,double)

ConstFloatPointerArray::ConstFloatPointerArray(JNIEnv *env, const float* pointer, jsize size) PointerArrayINIT(Float,float)

ConstQCharPointerArray::ConstQCharPointerArray(JNIEnv *env, const QChar* pointer, jsize size) PointerArrayINIT(Char,char)

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
Char16PointerArray::Char16PointerArray(JNIEnv *env, char16_t* pointer, jsize size) PointerArrayINIT(Char,char)
Char16PointerArray::~Char16PointerArray() PointerArrayDEL(Char,char)
ConstChar16PointerArray::ConstChar16PointerArray(JNIEnv *env, const char16_t* pointer, jsize size) PointerArrayINIT(Char,char)
Char32PointerArray::Char32PointerArray(JNIEnv *env, char32_t* pointer, jsize size) PointerArrayINIT(Int,int)
Char32PointerArray::~Char32PointerArray() PointerArrayDEL(Int,int)
ConstChar32PointerArray::ConstChar32PointerArray(JNIEnv *env, const char32_t* pointer, jsize size) PointerArrayINIT(Int,int)
#endif

#undef PointerArrayINIT
#undef PointerArrayDEL

bool isValidArray(JNIEnv *env, jobject object, jclass contentType){
    if(!object)
        return false;
    jclass arrayClass = env->GetObjectClass(object);
    if(Java::Runtime::Class::isArray(env, arrayClass)){
        jclass componentType = Java::Runtime::Class::getComponentType(env, arrayClass);
        return env->IsAssignableFrom(componentType, contentType);
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
        if(JniEnvironment env{100}){\
            env->Release##Type##ArrayElements(m_array, m_array_elements, arg);\
            JavaException::check(env QTJAMBI_STACKTRACEINFO );\
        }\
    }
#define ArrayPointerCOMMIT(Type)\
    if(m_array && m_size>0){\
        env->Release##Type##ArrayElements(m_array, m_array_elements, 0);\
        m_array_elements = env->Get##Type##ArrayElements(m_array, &m_is_copy);\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
    }

#define ArrayPointerStructors(Type,type)\
J##Type##ArrayPointer::J##Type##ArrayPointer(JNIEnv *env, j##type##Array array)\
    : JArrayPointer(env, array) {\
    ArrayPointerINIT(Type)\
}\
JConst##Type##ArrayPointer::JConst##Type##ArrayPointer(JNIEnv *env, j##type##Array array)\
    : JArrayPointer(env, array) {\
    ArrayPointerINIT(Type)\
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
void J##Type##ArrayPointer::commit(JNIEnv *env)\
{\
    ArrayPointerCOMMIT(Type)\
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
JConst##Type##ArrayPointer::ElementType JConst##Type##ArrayPointer::operator[](int index) const{\
    return *(m_array_elements+index);\
}\
J##Type##ArrayPointer::ElementType J##Type##ArrayPointer::operator[](int index) const{\
    return *(m_array_elements+index);\
}\
J##Type##ArrayPointer::ElementType& J##Type##ArrayPointer::operator[](int index){\
    return *(m_array_elements+index);\
}

#define PointerArrayValid(Type)\
bool JConst##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return ::isValidArray(env, object, QtJambiPrivate::ElementForArray<JArrayType>::RuntimeType::primitiveType(env));\
}\
bool J##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return ::isValidArray(env, object, QtJambiPrivate::ElementForArray<JArrayType>::RuntimeType::primitiveType(env));\
}

#define PointerArrayInitializerList(_const,type)\
    const type* array = reinterpret_cast<const type*>(m_array_elements);\
return QtJambiAPI::createIterable<std::initializer_list<_const type>>(array, m_size)

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define CHAR32 , char32_t
#define CHAR16 , char16_t
#define STD_BYTE , std::byte
#else
#define CHAR32
#define CHAR16
#define STD_BYTE
#endif

#define PointerArrayOperatorImpl(Type,type)\
JConst##Type##ArrayPointer::operator const type* () const { return reinterpret_cast<const type*>(m_array_elements); }\
JConst##Type##ArrayPointer::operator std::initializer_list<type> () const { PointerArrayInitializerList(,type); }\
JConst##Type##ArrayPointer::operator std::initializer_list<const type> () const { PointerArrayInitializerList(const,type); }\
J##Type##ArrayPointer::operator const type* () const { return reinterpret_cast<const type*>(m_array_elements); }\
J##Type##ArrayPointer::operator type* () { return reinterpret_cast<type*>(m_array_elements); }\
J##Type##ArrayPointer::operator std::initializer_list<type> () const { PointerArrayInitializerList(,type); }\
J##Type##ArrayPointer::operator std::initializer_list<const type> () const { PointerArrayInitializerList(const,type); }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#define PointerArrayOperator(Type,type)\
PointerArrayOperatorImpl(Type,type)\
JConst##Type##ArrayPointer::operator QSpan<const type> () const { return m_array_elements ? QSpan<const type>(reinterpret_cast<const type*>(m_array_elements), reinterpret_cast<const type*>(m_array_elements)+size()) : QSpan<const type>(); }\
J##Type##ArrayPointer::operator QSpan<const type> () const { return m_array_elements ? QSpan<const type>(reinterpret_cast<const type*>(m_array_elements), reinterpret_cast<const type*>(m_array_elements)+size()) : QSpan<const type>(); }\
J##Type##ArrayPointer::operator QSpan<type> () { return m_array_elements ? QSpan<type>(reinterpret_cast<type*>(m_array_elements), reinterpret_cast<type*>(m_array_elements)+size()) : QSpan<type>(); }
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

PointerArrayDeclaration(Byte, byte, char, qint8, quint8 STD_BYTE )
PointerArrayDeclaration(Int, int, int, uint CHAR32 )
PointerArrayDeclaration(Long, long, qint64, quint64)
PointerArrayDeclaration(Float, float, float)
PointerArrayDeclaration(Double, double, double)
PointerArrayDeclaration(Short, short, short)
PointerArrayDeclaration(Char, char, qint16, quint16, wchar_t, QChar CHAR16 )

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
JConstByteArrayPointer::operator QByteArrayView() const { return QByteArrayView(reinterpret_cast<const char*>(m_array_elements), size()); }
JByteArrayPointer::operator QByteArrayView() const { return QByteArrayView(reinterpret_cast<const char*>(m_array_elements), size()); }
JConstCharArrayPointer::operator QStringView() const { return QStringView(reinterpret_cast<const QChar*>(m_array_elements), size()); }
JCharArrayPointer::operator QStringView() const { return QStringView(reinterpret_cast<const QChar*>(m_array_elements), size()); }
#endif
JConstByteArrayPointer::operator QByteArray() const { return QByteArray(reinterpret_cast<const char*>(m_array_elements), size()); }
JByteArrayPointer::operator QByteArray() const { return QByteArray(reinterpret_cast<const char*>(m_array_elements), size()); }
JConstCharArrayPointer::operator QString() const { return QString(reinterpret_cast<const QChar*>(m_array_elements), size()); }
JCharArrayPointer::operator QString() const { return QString(reinterpret_cast<const QChar*>(m_array_elements), size()); }

#undef ArrayPointerINIT
#undef ArrayPointerDEL
#undef ArrayPointerCOMMIT
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

#define ArrayPointerDEL_INIT_JNI_ABORT()
#define ArrayPointerDEL_INIT_0()\
    if(sizeof(jboolean)!=sizeof(bool)){\
        for (int i= 0; i < m_size; ++i) {\
            m_boolean_array[i] = m_array_elements[i] ? JNI_TRUE : JNI_FALSE;\
        }\
    }

#define ArrayPointerDEL(Type, arg)\
    try{\
        if(m_array && m_size>0){\
            if(DefaultJniEnvironment env{100}){\
                ArrayPointerDEL_INIT_##arg()\
                env->ReleaseBooleanArrayElements(m_array, m_boolean_array, arg);\
                JavaException::check(env QTJAMBI_STACKTRACEINFO );\
            }\
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
        env->Release##Type##ArrayElements(m_array, m_boolean_array, 0);\
        m_boolean_array = env->Get##Type##ArrayElements(m_array, &m_is_copy);\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
}

PointerArrayDeclaration(Boolean, boolean, bool)

jboolean* JBooleanArrayPointer::booleanArray(){
    return m_boolean_array;
}

const jboolean* JBooleanArrayPointer::booleanArray() const{
    return m_boolean_array;
}

#undef ArrayPointerStructors
#undef ArrayPointerINIT
#undef ArrayPointerDEL
#undef CHAR32
#undef CHAR16
