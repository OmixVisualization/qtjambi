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

BoolPointerArray::BoolPointerArray(JNIEnv *env, bool* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

BoolPointerArray::~BoolPointerArray() PointerArrayDEL(Boolean,boolean)

Bool2PointerArray::Bool2PointerArray(JNIEnv *env, uchar* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

Bool2PointerArray::~Bool2PointerArray() PointerArrayDEL(Boolean,boolean)

ConstCharPointerArray::ConstCharPointerArray(JNIEnv *env, const char* pointer, jsize size) PointerArrayINIT(Byte,byte)

ConstInt8PointerArray::ConstInt8PointerArray(JNIEnv *env, const qint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

ConstInt16PointerArray::ConstInt16PointerArray(JNIEnv *env, const qint16* pointer, jsize size) PointerArrayINIT(Short,short)

ConstInt32PointerArray::ConstInt32PointerArray(JNIEnv *env, const qint32* pointer, jsize size) PointerArrayINIT(Int,int)

ConstInt64PointerArray::ConstInt64PointerArray(JNIEnv *env, const qint64* pointer, jsize size) PointerArrayINIT(Long,long)

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

#define ArrayPointerINIT(Type)\
    if(m_array && m_size>0){\
        m_array_elements = env->Get##Type##ArrayElements(m_array, &m_is_copy);\
        JavaException::check(env QTJAMBI_STACKTRACEINFO );\
    }
#define ArrayPointerDEL(Type)\
    if(m_array && m_size>0){\
    if(JniEnvironment env{100}){\
            env->Release##Type##ArrayElements(m_array, m_array_elements, m_writable ? 0 : JNI_ABORT);\
            JavaException::check(env QTJAMBI_STACKTRACEINFO );\
        }\
    }

#define ArrayPointerStructors(Type,type)\
J##Type##ArrayPointer::J##Type##ArrayPointer(JNIEnv *env, j##type##Array array, bool writable)\
    : JArrayPointer(env, array, writable) {\
    ArrayPointerINIT(Type)\
}\
J##Type##ArrayPointer::~J##Type##ArrayPointer()\
{\
    try{\
        ArrayPointerDEL(Type)\
    } catch (const std::exception& e) {\
        qCWarning(DebugAPI::internalCategory, "%s", e.what());\
    } catch (...) {\
    }\
}

ArrayPointerStructors(Byte,byte)
ArrayPointerStructors(Int,int)
ArrayPointerStructors(Short,short)
ArrayPointerStructors(Long,long)
ArrayPointerStructors(Char,char)
ArrayPointerStructors(Double,double)
ArrayPointerStructors(Float,float)

#undef ArrayPointerStructors
#undef ArrayPointerINIT
#undef ArrayPointerDEL

JBooleanArrayPointer::JBooleanArrayPointer(JNIEnv *env, jbooleanArray array, bool writable)
    : JArrayPointer(env, array, writable) {
    if(m_array && m_size>0){
        m_boolean_array = env->GetBooleanArrayElements(m_array, &m_is_copy);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        if(sizeof(jboolean)==sizeof(bool)){
            m_array_elements = reinterpret_cast<bool*>(m_boolean_array);
        }else{
            m_array_elements = new bool[size_t(m_size)];
            for (int i= 0; i < m_size; ++i) {
                m_array_elements[i] = m_boolean_array[i]==JNI_TRUE;
            }
        }
    }
}

JBooleanArrayPointer::~JBooleanArrayPointer()
{
    try{
        if(m_array && m_size>0){
            if(DefaultJniEnvironment env{100}){
                if(sizeof(jboolean)!=sizeof(bool) && m_writable){
                    for (int i= 0; i < m_size; ++i) {
                        m_boolean_array[i] = m_array_elements[i] ? JNI_TRUE : JNI_FALSE;
                    }
                }
                env->ReleaseBooleanArrayElements(m_array, m_boolean_array, m_writable ? 0 : JNI_ABORT);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
            }
            if(sizeof(jboolean)!=sizeof(bool)){
                delete[] m_array_elements;
            }
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

jboolean* JBooleanArrayPointer::booleanArray(){
    return m_boolean_array;
}

const jboolean* JBooleanArrayPointer::booleanArray() const{
    return m_boolean_array;
}

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

bool JBooleanArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Boolean::primitiveType(env));
}

bool JIntArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Integer::primitiveType(env));
}

bool JShortArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Short::primitiveType(env));
}

bool JByteArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Byte::primitiveType(env));
}

bool JCharArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Character::primitiveType(env));
}

bool JLongArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Long::primitiveType(env));
}

bool JDoubleArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Double::primitiveType(env));
}

bool JFloatArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return ::isValidArray(env, object, Java::Runtime::Float::primitiveType(env));
}
