/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_CAST_IMPL_ARRAY_H
#define QTJAMBI_CAST_IMPL_ARRAY_H

#include "qtjambi_cast_impl.h"

namespace QtJambiPrivate {

template<bool has_scope, typename O, bool o_is_jni_array_type,
                         typename T, bool t_is_jni_array_type,
                         typename I>
struct qtjambi_array_cast_decider{
    static O cast(JNIEnv *, T, I&, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
};

template<bool has_scope, typename O,
                         typename T, bool t_is_const,
                         typename I>
struct qtjambi_nativearray_caster{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_in;
    static O cast(JNIEnv *, T_in*, jsize, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !t_is_const, "Cannot cast types");
    }
};

template<bool has_scope, typename O,
                         bool is_const,
                         typename JArray>
struct qtjambi_jarray_caster{
    static O* cast(JNIEnv *, JArray, jsize&, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, typename O, typename T, typename I>
struct qtjambi_array_cast_decider<has_scope, O, true, T, false, I>
        : qtjambi_nativearray_caster<has_scope, O, typename std::remove_const<typename std::remove_pointer<T>::type>::type, std::is_const<typename std::remove_pointer<T>::type>::value, I>
{
};

template<bool has_scope, typename O, typename T, typename I>
struct qtjambi_array_cast_decider<has_scope, O, false, T, true, I>
{
    static O cast(JNIEnv *env, T in, I& size, const char* nativeTypeName, QtJambiScope* scope){
        jsize _size = jsize(size);
        jsize* _sizePtr = &_size;
        I* sizePtr = &size;
        DoFinally __qt_do_finally([_sizePtr,sizePtr](){ *sizePtr = I(*_sizePtr); });
        Q_UNUSED(__qt_do_finally)
        return qtjambi_jarray_caster<
                has_scope,
                typename std::remove_const<typename std::remove_pointer<O>::type>::type,
                std::is_const<typename std::remove_pointer<O>::type>::value,
                T
            >::cast(env, in, _size, nativeTypeName, scope);
    }
};

template<bool has_scope, typename O, typename T>
struct qtjambi_array_cast_decider<has_scope, O, false, T, true, jsize>
        : qtjambi_jarray_caster<has_scope, typename std::remove_const<typename std::remove_pointer<O>::type>::type, std::is_const<typename std::remove_pointer<O>::type>::value, T>
{
};

template<bool has_scope, typename T, typename I>
struct qtjambi_nativearray_caster<has_scope, jobjectArray, T, true, I>{
    static jobjectArray cast(JNIEnv *env, const T* in, I size, const char* type, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        ConstObjectPointerArray<T>* array = new ConstObjectPointerArray<T>(env, in, jsize(size), [scope,type](JNIEnv * env,const T& in)->jobject{
                                                                                        return qtjambi_cast<jobject>(env, *scope, in, type);
                                                                                    });
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename T, typename I>
struct qtjambi_nativearray_caster<false, jobjectArray, T, true, I>{
    static jobjectArray cast(JNIEnv *env, const T* in, I size, const char* type, QtJambiScope*){
        ConstObjectPointerArray<T> array(env, in, jsize(size), [type](JNIEnv * env,const T& in)->jobject{
            return qtjambi_cast<jobject>(env, in, type);
        });
        return array.array(env);
    }
};

template<bool has_scope, typename T, typename I>
struct qtjambi_nativearray_caster<has_scope, jobjectArray, T, false, I>{
    static jobjectArray cast(JNIEnv *env, T* in, I size, const char* type, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        ObjectPointerArray<T>* array = new ObjectPointerArray<T>(env, in, jsize(size), [scope,type](JNIEnv * env,const T& in)->jobject{
                                                                            return qtjambi_cast<jobject>(env, *scope, in, type);
                                                                        }, [scope,type](T& out,JNIEnv * env,jobject in){
                                                                            out = qtjambi_cast<T>(env, *scope, in, type);
                                                                        });
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jintArray, qint32, true, I>{
    static jintArray cast(JNIEnv *env, const qint32* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt32PointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstInt32PointerArray* array = new ConstInt32PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jintArray, qint32, true, I>{
    static jintArray cast(JNIEnv *env, const qint32* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJIntArray(env, reinterpret_cast<const jint *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jintArray, qint32, false, I>{
    static jintArray cast(JNIEnv *env, qint32* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int32PointerArray* array = new Int32PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jintArray, quint32, true, I>{
    static jintArray cast(JNIEnv *env, const quint32* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt32PointerArray array(env, reinterpret_cast<const qint32*>(in), jsize(size));
            return array.array(env);
        }
        ConstInt32PointerArray* array = new ConstInt32PointerArray(env, reinterpret_cast<const qint32*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jintArray, quint32, true, I>{
    static jintArray cast(JNIEnv *env, const quint32* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJIntArray(env, reinterpret_cast<const jint *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jintArray, quint32, false, I>{
    static jintArray cast(JNIEnv *env, quint32* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int32PointerArray* array = new Int32PointerArray(env, reinterpret_cast<qint32*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jlongArray, qint64, true, I>{
    static jlongArray cast(JNIEnv *env, const qint64* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt64PointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstInt64PointerArray* array = new ConstInt64PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jlongArray, qint64, true, I>{
    static jlongArray cast(JNIEnv *env, const qint64* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJLongArray(env, reinterpret_cast<const jlong *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jlongArray, qint64, false, I>{
    static jlongArray cast(JNIEnv *env, qint64* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int64PointerArray* array = new Int64PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jlongArray, quint64, true, I>{
    static jlongArray cast(JNIEnv *env, const quint64* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt64PointerArray array(env, reinterpret_cast<const qint64*>(in), jsize(size));
            return array.array(env);
        }
        ConstInt64PointerArray* array = new ConstInt64PointerArray(env, reinterpret_cast<const qint64*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jlongArray, quint64, true, I>{
    static jlongArray cast(JNIEnv *env, const quint64* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJLongArray(env, reinterpret_cast<const jlong *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jlongArray, quint64, false, I>{
    static jlongArray cast(JNIEnv *env, quint64* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int64PointerArray* array = new Int64PointerArray(env, reinterpret_cast<qint64*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jshortArray, qint16, true, I>{
    static jshortArray cast(JNIEnv *env, const qint16* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt16PointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstInt16PointerArray* array = new ConstInt16PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jshortArray, qint16, true, I>{
    static jshortArray cast(JNIEnv *env, const qint16* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJShortArray(env, reinterpret_cast<const jshort *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jshortArray, qint16, false, I>{
    static jshortArray cast(JNIEnv *env, qint16* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int16PointerArray* array = new Int16PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jshortArray, quint16, true, I>{
    static jshortArray cast(JNIEnv *env, const quint16* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt16PointerArray array(env, reinterpret_cast<const qint16*>(in), jsize(size));
            return array.array(env);
        }
        ConstInt16PointerArray* array = new ConstInt16PointerArray(env, reinterpret_cast<const qint16*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jshortArray, quint16, true, I>{
    static jshortArray cast(JNIEnv *env, const quint16* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJShortArray(env, reinterpret_cast<const jshort *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jshortArray, quint16, false, I>{
    static jshortArray cast(JNIEnv *env, quint16* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int16PointerArray* array = new Int16PointerArray(env, reinterpret_cast<qint16*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbyteArray, qint8, true, I>{
    static jbyteArray cast(JNIEnv *env, const qint8* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt8PointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstInt8PointerArray* array = new ConstInt8PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jbyteArray, qint8, true, I>{
    static jbyteArray cast(JNIEnv *env, const qint8* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbyteArray, qint8, false, I>{
    static jbyteArray cast(JNIEnv *env, qint8* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int8PointerArray* array = new Int8PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbyteArray, quint8, true, I>{
    static jbyteArray cast(JNIEnv *env, const quint8* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstInt8PointerArray array(env, reinterpret_cast<const qint8*>(in), jsize(size));
            return array.array(env);
        }
        ConstInt8PointerArray* array = new ConstInt8PointerArray(env, reinterpret_cast<const qint8*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jbyteArray, quint8, true, I>{
    static jbyteArray cast(JNIEnv *env, const quint8* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbyteArray, quint8, false, I>{
    static jbyteArray cast(JNIEnv *env, quint8* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Int8PointerArray* array = new Int8PointerArray(env, reinterpret_cast<qint8*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbyteArray, char, true, I>{
    static jbyteArray cast(JNIEnv *env, const char* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstCharPointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstCharPointerArray* array = new ConstCharPointerArray(env, reinterpret_cast<const char*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jbyteArray, char, true, I>{
    static jbyteArray cast(JNIEnv *env, const char* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbyteArray, char, false, I>{
    static jbyteArray cast(JNIEnv *env, char* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        CharPointerArray* array = new CharPointerArray(env, reinterpret_cast<char*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jdoubleArray, double, true, I>{
    static jdoubleArray cast(JNIEnv *env, const double* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstDoublePointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstDoublePointerArray* array = new ConstDoublePointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jdoubleArray, double, true, I>{
    static jdoubleArray cast(JNIEnv *env, const double* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJDoubleArray(env, reinterpret_cast<const jdouble *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jdoubleArray, double, false, I>{
    static jdoubleArray cast(JNIEnv *env, double* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        DoublePointerArray* array = new DoublePointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jfloatArray, float, true, I>{
    static jfloatArray cast(JNIEnv *env, const float* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstFloatPointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstFloatPointerArray* array = new ConstFloatPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jfloatArray, float, true, I>{
    static jfloatArray cast(JNIEnv *env, const float* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJFloatArray(env, reinterpret_cast<const jfloat *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jfloatArray, float, false, I>{
    static jfloatArray cast(JNIEnv *env, float* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        FloatPointerArray* array = new FloatPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbooleanArray, bool, true, I>{
    static jbooleanArray cast(JNIEnv *env, const bool* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstBoolPointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstBoolPointerArray* array = new ConstBoolPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jbooleanArray, bool, true, I>{
    static jbooleanArray cast(JNIEnv *env, const bool* in, I size, const char*, QtJambiScope*){
        ConstBoolPointerArray array(env, in, jsize(size));
        return array.array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbooleanArray, bool, false, I>{
    static jbooleanArray cast(JNIEnv *env, bool* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        BoolPointerArray* array = new BoolPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbooleanArray, uchar, true, I>{
    static jbooleanArray cast(JNIEnv *env, const uchar* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstBool2PointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstBool2PointerArray* array = new ConstBool2PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jbooleanArray, uchar, true, I>{
    static jbooleanArray cast(JNIEnv *env, const uchar* in, I size, const char*, QtJambiScope*){
        ConstBool2PointerArray array(env, in, jsize(size));
        return array.array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jbooleanArray, uchar, false, I>{
    static jbooleanArray cast(JNIEnv *env, uchar* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Bool2PointerArray* array = new Bool2PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, ushort, true, I>{
    static jcharArray cast(JNIEnv *env, const ushort* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstWCharPointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstWCharPointerArray* array = new ConstWCharPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jcharArray, ushort, true, I>{
    static jcharArray cast(JNIEnv *env, const ushort* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, ushort, false, I>{
    static jcharArray cast(JNIEnv *env, ushort* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        WCharPointerArray* array = new WCharPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, wchar_t, true, I>{
    static jcharArray cast(JNIEnv *env, const wchar_t* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstWCharPointerArray array(env, reinterpret_cast<const ushort*>(in), jsize(size));
            return array.array(env);
        }
        ConstWCharPointerArray* array = new ConstWCharPointerArray(env, reinterpret_cast<const ushort*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jcharArray, wchar_t, true, I>{
    static jcharArray cast(JNIEnv *env, const wchar_t* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, wchar_t, false, I>{
    static jcharArray cast(JNIEnv *env, wchar_t* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        WCharPointerArray* array = new WCharPointerArray(env, reinterpret_cast<ushort*>(in), jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, QChar, true, I>{
    static jcharArray cast(JNIEnv *env, const QChar* in, I size, const char*, QtJambiScope* scope){
        if(scope==nullptr){
            ConstQCharPointerArray array(env, in, jsize(size));
            return array.array(env);
        }
        ConstQCharPointerArray* array = new ConstQCharPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<typename I>
struct qtjambi_nativearray_caster<false, jcharArray, QChar, true, I>{
    static jcharArray cast(JNIEnv *env, const QChar* in, I size, const char*, QtJambiScope*){
        return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(in), jsize(size));
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, QChar, false, I>{
    static jcharArray cast(JNIEnv *env, QChar* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        QCharPointerArray* array = new QCharPointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename O>
struct qtjambi_jarray_caster<has_scope, O, false, jobjectArray>{
    static O* cast(JNIEnv * env, jobjectArray in, jsize& size, const char* type, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JObjectArrayPointer<O>* array = new JObjectArrayPointer<O>(env, in, [scope,type](O& out,JNIEnv * env,jobject in){
                                                                                    out = qtjambi_cast<O>(env, *scope, in, type);
                                                                                }, [scope,type](JNIEnv * env,const O& in)->jobject{
                                                                                    return qtjambi_cast<jobject>(env, *scope, in, type);
                                                                                });
        size = array->size();
        scope->addDeletion(array);
        return array->pointer();
    }
};

template<bool has_scope, typename O>
struct qtjambi_jarray_caster<has_scope, O, true, jobjectArray>{
    static const O* cast(JNIEnv * env, jobjectArray in, jsize& size, const char* type, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JObjectArrayPointer<O>* array = new JObjectArrayPointer<O>(env, in, [scope,type](O& out,JNIEnv * env,jobject in){
                                                                                        out = qtjambi_cast<O>(env, *scope, in, type);
                                                                                    });
        size = array->size();
        scope->addDeletion(array);
        return array->pointer();
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, qint32, is_const, jintArray>{
    typedef qint32 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jintArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JIntArrayPointer* array = new JIntArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, quint32, is_const, jintArray>{
    typedef quint32 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jintArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JIntArrayPointer* array = new JIntArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, qint64, is_const, jlongArray>{
    typedef qint64 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jlongArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JLongArrayPointer* array = new JLongArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, quint64, is_const, jlongArray>{
    typedef quint64 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jlongArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JLongArrayPointer* array = new JLongArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, qint16, is_const, jshortArray>{
    typedef qint16 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jshortArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JShortArrayPointer* array = new JShortArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, quint16, is_const, jshortArray>{
    typedef quint16 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jshortArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JShortArrayPointer* array = new JShortArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, qint8, is_const, jbyteArray>{
    typedef qint8 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jbyteArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JByteArrayPointer* array = new JByteArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, quint8, is_const, jbyteArray>{
    typedef quint8 O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jbyteArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JByteArrayPointer* array = new JByteArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, char, is_const, jbyteArray>{
    typedef char O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jbyteArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JByteArrayPointer* array = new JByteArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, ushort, is_const, jcharArray>{
    typedef ushort O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jcharArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JCharArrayPointer* array = new JCharArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, wchar_t, is_const, jcharArray>{
    typedef wchar_t O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jcharArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JCharArrayPointer* array = new JCharArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, QChar, is_const, jcharArray>{
    typedef QChar O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jcharArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JCharArrayPointer* array = new JCharArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, double, is_const, jdoubleArray>{
    typedef double O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jdoubleArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JDoubleArrayPointer* array = new JDoubleArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, float, is_const, jfloatArray>{
    typedef float O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jfloatArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JFloatArrayPointer* array = new JFloatArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, bool, is_const, jbooleanArray>{
    typedef bool O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jbooleanArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JBooleanArrayPointer* array = new JBooleanArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, jboolean, is_const, jbooleanArray>{
    typedef jboolean O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jbooleanArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JBooleanArrayPointer* array = new JBooleanArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->booleanArray());
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, char16_t, is_const, jcharArray>{
    typedef char16_t O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jcharArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JCharArrayPointer* array = new JCharArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jarray_caster<has_scope, char32_t, is_const, jintArray>{
    typedef char32_t O;
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    static Out* cast(JNIEnv * env, jintArray in, jsize& size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        JIntArrayPointer* array = new JIntArrayPointer(env, in, !is_const);
        size = jsize(array->size());
        scope->addDeletion(array);
        return reinterpret_cast<Out*>(array->pointer());
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, char16_t, true, I>{
    static jcharArray cast(JNIEnv *env, const char16_t* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        ConstChar16PointerArray* array = new ConstChar16PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jcharArray, char16_t, false, I>{
    static jcharArray cast(JNIEnv *env, char16_t* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Char16PointerArray* array = new Char16PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jintArray, char32_t, true, I>{
    static jintArray cast(JNIEnv *env, const char32_t* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        ConstChar32PointerArray* array = new ConstChar32PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};

template<bool has_scope, typename I>
struct qtjambi_nativearray_caster<has_scope, jintArray, char32_t, false, I>{
    static jintArray cast(JNIEnv *env, char32_t* in, I size, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast array without scope.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast array without scope." QTJAMBI_STACKTRACEINFO );
        Char32PointerArray* array = new Char32PointerArray(env, in, jsize(size));
        scope->addDeletion(array);
        return array->array(env);
    }
};
#endif
} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_ARRAY_H
