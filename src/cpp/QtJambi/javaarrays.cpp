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

#define PointerArrayINIT(Type,type)\
     : PointerArray(env, pointer, env->New##Type##Array(pointer ? size : 0), pointer ? size : 0) {\
    if(pointer){\
        JavaException::check(m_env QTJAMBI_STACKTRACEINFO );\
        m_env->Set##Type##ArrayRegion(array(), 0, size, reinterpret_cast<const j##type *>(pointer));\
        JavaException::check(m_env QTJAMBI_STACKTRACEINFO );\
    }\
}

#define PointerArrayDEL(Type,type)\
{\
    if(m_array){\
        m_env->Get##Type##ArrayRegion(m_array, 0, m_size, reinterpret_cast<j##type *>(m_pointer));\
        JavaException::check(m_env QTJAMBI_STACKTRACEINFO );\
    }\
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

struct PersistentPointerArrayPrivate{
    JObjectWrapper m_array;
    jsize m_size;
};

AbstractPersistentPointerArray::~AbstractPersistentPointerArray(){}
AbstractPersistentPointerArray::AbstractPersistentPointerArray(JNIEnv *env, jarray array, jsize size)
    : d(new PersistentPointerArrayPrivate{JObjectWrapper(env, array), size})
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

Char16PointerArray::Char16PointerArray(JNIEnv *env, char16_t* pointer, jsize size) PointerArrayINIT(Char,char)

Char16PointerArray::~Char16PointerArray() PointerArrayDEL(Char,char)

ConstChar16PointerArray::ConstChar16PointerArray(JNIEnv *env, const char16_t* pointer, jsize size) PointerArrayINIT(Char,char)

Char32PointerArray::Char32PointerArray(JNIEnv *env, char32_t* pointer, jsize size) PointerArrayINIT(Int,int)

Char32PointerArray::~Char32PointerArray() PointerArrayDEL(Int,int)

ConstChar32PointerArray::ConstChar32PointerArray(JNIEnv *env, const char32_t* pointer, jsize size) PointerArrayINIT(Int,int)

#undef PointerArrayINIT
#undef PointerArrayDEL

#define PointerArrayINIT(Type,type)\
     : PersistentPointerArray(env, pointer, env->New##Type##Array(pointer ? size : 0), pointer ? size : 0) {\
    if(pointer){\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
        env->Set##Type##ArrayRegion(array(env), 0, size, reinterpret_cast<const j##type *>(pointer));\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
    }\
}

#define PointerArrayDEL(Type,type)\
{\
    if(*this){\
        if(JniEnvironment env{100}){\
            env->Get##Type##ArrayRegion(array(env), 0, size(), reinterpret_cast<j##type *>(m_pointer));\
            JavaException::check(env QTJAMBI_STACKTRACEINFO );\
        }\
    }\
}

PersistentCharPointerArray::PersistentCharPointerArray(JNIEnv *env, char* pointer, jsize size) PointerArrayINIT(Byte,byte)

PersistentCharPointerArray::~PersistentCharPointerArray() PointerArrayDEL(Byte,byte)

PersistentWCharPointerArray::PersistentWCharPointerArray(JNIEnv *env, ushort* pointer, jsize size) PointerArrayINIT(Char,char)

PersistentWCharPointerArray::~PersistentWCharPointerArray() PointerArrayDEL(Char,char)

PersistentQCharPointerArray::PersistentQCharPointerArray(JNIEnv *env, QChar* pointer, jsize size) PointerArrayINIT(Char,char)

PersistentQCharPointerArray::~PersistentQCharPointerArray() PointerArrayDEL(Char,char)

PersistentDoublePointerArray::PersistentDoublePointerArray(JNIEnv *env, double* pointer, jsize size) PointerArrayINIT(Double,double)

PersistentDoublePointerArray::~PersistentDoublePointerArray() PointerArrayDEL(Double,double)

PersistentFloatPointerArray::PersistentFloatPointerArray(JNIEnv *env, float* pointer, jsize size) PointerArrayINIT(Float,float)

PersistentFloatPointerArray::~PersistentFloatPointerArray() PointerArrayDEL(Float,float)

PersistentInt8PointerArray::PersistentInt8PointerArray(JNIEnv *env, qint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

PersistentInt8PointerArray::~PersistentInt8PointerArray() PointerArrayDEL(Byte,byte)

PersistentInt16PointerArray::PersistentInt16PointerArray(JNIEnv *env, qint16* pointer, jsize size) PointerArrayINIT(Short,short)

PersistentInt16PointerArray::~PersistentInt16PointerArray() PointerArrayDEL(Short,short)

PersistentInt32PointerArray::PersistentInt32PointerArray(JNIEnv *env, qint32* pointer, jsize size) PointerArrayINIT(Int,int)

PersistentInt32PointerArray::~PersistentInt32PointerArray() PointerArrayDEL(Int,int)

PersistentInt64PointerArray::PersistentInt64PointerArray(JNIEnv *env, qint64* pointer, jsize size) PointerArrayINIT(Long,long)

PersistentInt64PointerArray::~PersistentInt64PointerArray() PointerArrayDEL(Long,long)

PersistentUInt8PointerArray::PersistentUInt8PointerArray(JNIEnv *env, quint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

PersistentUInt8PointerArray::~PersistentUInt8PointerArray() PointerArrayDEL(Byte,byte)

PersistentUInt16PointerArray::PersistentUInt16PointerArray(JNIEnv *env, quint16* pointer, jsize size) PointerArrayINIT(Short,short)

PersistentUInt16PointerArray::~PersistentUInt16PointerArray() PointerArrayDEL(Short,short)

PersistentUInt32PointerArray::PersistentUInt32PointerArray(JNIEnv *env, quint32* pointer, jsize size) PointerArrayINIT(Int,int)

PersistentUInt32PointerArray::~PersistentUInt32PointerArray() PointerArrayDEL(Int,int)

PersistentUInt64PointerArray::PersistentUInt64PointerArray(JNIEnv *env, quint64* pointer, jsize size) PointerArrayINIT(Long,long)

PersistentUInt64PointerArray::~PersistentUInt64PointerArray() PointerArrayDEL(Long,long)

PersistentBoolPointerArray::PersistentBoolPointerArray(JNIEnv *env, bool* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

PersistentBoolPointerArray::~PersistentBoolPointerArray() PointerArrayDEL(Boolean,boolean)

PersistentBool2PointerArray::PersistentBool2PointerArray(JNIEnv *env, uchar* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

PersistentBool2PointerArray::~PersistentBool2PointerArray() PointerArrayDEL(Boolean,boolean)

PersistentConstCharPointerArray::PersistentConstCharPointerArray(JNIEnv *env, const char* pointer, jsize size) PointerArrayINIT(Byte,byte)

PersistentConstInt8PointerArray::PersistentConstInt8PointerArray(JNIEnv *env, const qint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

PersistentConstInt16PointerArray::PersistentConstInt16PointerArray(JNIEnv *env, const qint16* pointer, jsize size) PointerArrayINIT(Short,short)

PersistentConstInt32PointerArray::PersistentConstInt32PointerArray(JNIEnv *env, const qint32* pointer, jsize size) PointerArrayINIT(Int,int)

PersistentConstInt64PointerArray::PersistentConstInt64PointerArray(JNIEnv *env, const qint64* pointer, jsize size) PointerArrayINIT(Long,long)

PersistentConstUInt8PointerArray::PersistentConstUInt8PointerArray(JNIEnv *env, const quint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

PersistentConstUInt16PointerArray::PersistentConstUInt16PointerArray(JNIEnv *env, const quint16* pointer, jsize size) PointerArrayINIT(Short,short)

PersistentConstUInt32PointerArray::PersistentConstUInt32PointerArray(JNIEnv *env, const quint32* pointer, jsize size) PointerArrayINIT(Int,int)

PersistentConstUInt64PointerArray::PersistentConstUInt64PointerArray(JNIEnv *env, const quint64* pointer, jsize size) PointerArrayINIT(Long,long)

PersistentConstBoolPointerArray::PersistentConstBoolPointerArray(JNIEnv *env, const bool* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

PersistentConstBool2PointerArray::PersistentConstBool2PointerArray(JNIEnv *env, const uchar* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

PersistentConstWCharPointerArray::PersistentConstWCharPointerArray(JNIEnv *env, const ushort* pointer, jsize size) PointerArrayINIT(Char,char)

PersistentConstDoublePointerArray::PersistentConstDoublePointerArray(JNIEnv *env, const double* pointer, jsize size) PointerArrayINIT(Double,double)

PersistentConstFloatPointerArray::PersistentConstFloatPointerArray(JNIEnv *env, const float* pointer, jsize size) PointerArrayINIT(Float,float)

PersistentConstQCharPointerArray::PersistentConstQCharPointerArray(JNIEnv *env, const QChar* pointer, jsize size) PointerArrayINIT(Char,char)

PersistentChar16PointerArray::PersistentChar16PointerArray(JNIEnv *env, char16_t* pointer, jsize size) PointerArrayINIT(Char,char)
PersistentChar16PointerArray::~PersistentChar16PointerArray() PointerArrayDEL(Char,char)
PersistentConstChar16PointerArray::PersistentConstChar16PointerArray(JNIEnv *env, const char16_t* pointer, jsize size) PointerArrayINIT(Char,char)
PersistentChar32PointerArray::PersistentChar32PointerArray(JNIEnv *env, char32_t* pointer, jsize size) PointerArrayINIT(Int,int)
PersistentChar32PointerArray::~PersistentChar32PointerArray() PointerArrayDEL(Int,int)
PersistentConstChar32PointerArray::PersistentConstChar32PointerArray(JNIEnv *env, const char32_t* pointer, jsize size) PointerArrayINIT(Int,int)

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
    return ::isValidArray(env, object, QtJambiPrivate::ElementForArray<JArrayType>::RuntimeType::primitiveType(env));\
}\
bool J##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return ::isValidArray(env, object, QtJambiPrivate::ElementForArray<JArrayType>::RuntimeType::primitiveType(env));\
}\
bool PersistentJConst##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return ::isValidArray(env, object, QtJambiPrivate::ElementForArray<JArrayType>::RuntimeType::primitiveType(env));\
}\
bool PersistentJ##Type##ArrayPointer::isValidArray(JNIEnv *env, jobject object){\
    return ::isValidArray(env, object, QtJambiPrivate::ElementForArray<JArrayType>::RuntimeType::primitiveType(env));\
}

#define PointerArrayInitializerList(_const,type)\
    const type* array = reinterpret_cast<const type*>(m_array_elements);\
return QtJambiAPI::createIterable<std::initializer_list<_const type>>(array, size())

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
