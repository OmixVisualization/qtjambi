/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_CONTAINER1_P_H
#define QTJAMBI_CAST_CONTAINER1_P_H

#include "qtjambi_core.h"
#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_sharedpointer_p.h"
#ifndef QT_NO_DBUS
#include <qtjambi_dbus/QDBusVariantReply.h>
#endif
#include <qtjambi_qml/qqmllistproperty.h>

QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<T>>::type, NativeType<T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
    static NativeType_out castbck(JNIEnv *, JniType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
};

//template from any type's QWeakPointer<T> to jobject
template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QWeakPointer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QWeakPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         if(QSharedPointer<T> ptr = deref_ptr<is_pointer,NativeType_c>::deref(in).toStrongRef()){
              return qtjambi_scoped_cast<has_scope,jobject,QSharedPointer<T>>::cast(env, ptr, nullptr, scope);
         }else
             return nullptr;
     }
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
         Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QWeakPointer<T> &");
         Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QWeakPointer<T> *");
         QSharedPointer<T> pointer = qtjambi_cast<QSharedPointer<T>>(env, in);
         return pointer.toWeakRef();
     }
};

//template from any type's QSharedPointer<T> to jobject
template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QSharedPointer, is_pointer, is_const, is_reference,
                                 T>
        : qtjambi_shared_pointer_caster<forward, has_scope,
                QSharedPointer, is_pointer, is_const, is_reference,
                typename std::remove_pointer<typename std::remove_cv<T>::type>::type,
                std::is_arithmetic<T>::value,
                std::is_enum<T>::value,
                std::is_pointer<T>::value,
                std::is_const<T>::value,
                std::is_function<typename std::remove_pointer<typename std::remove_cv<T>::type>::type>::value,
                std::is_base_of<QObject, typename std::remove_pointer<typename std::remove_cv<T>::type>::type>::value,
                is_template<typename std::remove_pointer<typename std::remove_cv<T>::type>::type>::value >
{
};

//template from any type's std::weak_ptr<T> to jobject
template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 std::weak_ptr, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::weak_ptr<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         if(std::shared_ptr<T> ptr = std::shared_ptr<T>(deref_ptr<is_pointer,NativeType_c>::deref(in))){
              return qtjambi_scoped_cast<has_scope,jobject,std::shared_ptr<T>>::cast(env, ptr, nullptr, scope);
         }else
             return nullptr;
     }
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
         Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::weak_ptr<T> &");
         Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::weak_ptr<T> *");
         std::shared_ptr<T> pointer = qtjambi_cast<std::shared_ptr<T>>(env, in);
         return NativeType(pointer);
     }
};

//template from any type's std::shared_ptr<T> to jobject
template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 std::shared_ptr, is_pointer, is_const, is_reference,
                                 T>
        : qtjambi_shared_pointer_caster<forward, has_scope,
                std::shared_ptr, is_pointer, is_const, is_reference,
                typename std::remove_pointer<typename std::remove_cv<T>::type>::type,
                std::is_arithmetic<T>::value,
                std::is_enum<T>::value,
                std::is_pointer<T>::value,
                std::is_const<T>::value,
                std::is_function<typename std::remove_pointer<typename std::remove_cv<T>::type>::type>::value,
                std::is_base_of<QObject, typename std::remove_pointer<typename std::remove_cv<T>::type>::type>::value,
                is_template<typename std::remove_pointer<typename std::remove_cv<T>::type>::type>::value >
{
};

//template from any QFlags to jobject
template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QFlags, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFlags<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_flags(env, deref_ptr<is_pointer,NativeType_c>::deref(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QFlags<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QFlags<T> *");
        return QFlags<T>(qtjambi_to_enum(env, in));
    }
};

//template from any std::initializer_list to jobject
template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::initializer_list<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<std::is_pointer<T>::value, T, typename std::add_const<T>::type>::type T_const;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        return qtjambi_to_jobjectArray(env, qtjambi_type<T>::id(), ref_ptr<is_pointer, NativeType_c>::ref(in), jsize(in.size()),
                       [scope](JNIEnv * env,const void* in, jsize index)->jobject{
                            return qtjambi_scoped_cast<has_scope,jobject,T_const>::cast(env,
                                        reinterpret_cast<typename std::add_const<NativeType>::type*>(in)->begin()[index], nullptr, scope);
                       }
                   );
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<T> *");
        if(scope==nullptr){
            JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
            return NativeType();
        }else{
            JObjectArrayPointer<T>* data = nullptr;
            if (qtjambi_is_valid_array(env, in, qtjambi_type<T>::id())) {
                data = new JObjectArrayPointer<T>(env, jobjectArray(in), [](T& d,JNIEnv * env, jobject obj){
                    d = qtjambi_cast<T>(env, obj);
                });
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<T> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<T>(data->operator const T *(), data->operator const T *() + data->size());
#else
                struct{
                    T* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator T *();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<T>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint32>{
    typedef std::initializer_list<qint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jintArray(env, reinterpret_cast<const jint*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<qint32> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<qint32> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JIntArrayPointer * data = nullptr;
            if (JIntArrayPointer::isValidArray(env, in)) {
                data = new JIntArrayPointer(env, jintArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<qint32> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<qint32>(data->operator const qint32* (), data->operator const qint32* () + data->size());
#else
                struct{
                    qint32* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator qint32* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<qint32>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint16>{
    typedef std::initializer_list<qint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jshortArray(env, reinterpret_cast<const jshort*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<qint16> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<qint16> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JShortArrayPointer * data = nullptr;
            if (JShortArrayPointer::isValidArray(env, in)) {
                data = new JShortArrayPointer(env, jshortArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<qint16> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<qint16>(data->operator const qint16* (), data->operator const qint16* () + data->size());
#else
                struct{
                    qint16* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator qint16* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<qint16>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint8>{
    typedef std::initializer_list<qint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jbyteArray(env, reinterpret_cast<const jbyte*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<qint8> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<qint8> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JByteArrayPointer * data = nullptr;
            if (JByteArrayPointer::isValidArray(env, in)) {
                data = new JByteArrayPointer(env, jbyteArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<qint8> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<qint8>(data->operator const qint8* (), data->operator const qint8* () + data->size());
#else
                struct{
                    qint8* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator qint8* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<qint8>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint64>{
    typedef std::initializer_list<qint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jlongArray(env, reinterpret_cast<const jlong*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<qint64> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<qint64> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JLongArrayPointer * data = nullptr;
            if (JLongArrayPointer::isValidArray(env, in)) {
                data = new JLongArrayPointer(env, jlongArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<qint64> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<qint64>(data->operator const qint64* (), data->operator const qint64* () + data->size());
#else
                struct{
                    qint64* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator qint64* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<qint64>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint32>{
    typedef std::initializer_list<quint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jintArray(env, reinterpret_cast<const jint*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<quint32> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<quint32> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JIntArrayPointer * data = nullptr;
            if (JIntArrayPointer::isValidArray(env, in)) {
                data = new JIntArrayPointer(env, jintArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<quint32> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<quint32>(data->operator const quint32* (), data->operator const quint32* () + data->size());
#else
                struct{
                    quint32* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator quint32* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<quint32>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint16>{
    typedef std::initializer_list<quint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jshortArray(env, reinterpret_cast<const jshort*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<quint16> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<quint16> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JShortArrayPointer * data = nullptr;
            if (JShortArrayPointer::isValidArray(env, in)) {
                data = new JShortArrayPointer(env, jshortArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<quint16> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<quint16>(data->operator const quint16* (), data->operator const quint16* () + data->size());
#else
                struct{
                    quint16* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator quint16* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<quint16>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint8>{
    typedef std::initializer_list<quint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jbyteArray(env, reinterpret_cast<const jbyte*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<quint8> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<quint8> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JByteArrayPointer * data = nullptr;
            if (JByteArrayPointer::isValidArray(env, in)) {
                data = new JByteArrayPointer(env, jbyteArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<quint8> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<quint8>(data->operator const quint8* (), data->operator const quint8* () + data->size());
#else
                struct{
                    quint8* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator quint8* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<quint8>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint64>{
    typedef std::initializer_list<quint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jlongArray(env, reinterpret_cast<const jlong*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<quint64> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<quint64> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JLongArrayPointer * data = nullptr;
            if (JLongArrayPointer::isValidArray(env, in)) {
                data = new JLongArrayPointer(env, jlongArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<quint64> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<quint64>(data->operator const quint64* (), data->operator const quint64* () + data->size());
#else
                struct{
                    quint64* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator quint64* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<quint64>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 float>{
    typedef std::initializer_list<float> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jfloatArray(env, reinterpret_cast<const jfloat*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<float> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<float> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JFloatArrayPointer * data = nullptr;
            if (JFloatArrayPointer::isValidArray(env, in)) {
                data = new JFloatArrayPointer(env, jfloatArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<float> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<float>(data->operator const float* (), data->operator const float* () + data->size());
#else
                struct{
                    float* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator float* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<float>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 double>{
    typedef std::initializer_list<double> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jdoubleArray(env, reinterpret_cast<const jdouble*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<double> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<double> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JDoubleArrayPointer * data = nullptr;
            if (JDoubleArrayPointer::isValidArray(env, in)) {
                data = new JDoubleArrayPointer(env, jdoubleArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<double> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<double>(data->operator const double* (), data->operator const double* () + data->size());
#else
                struct{
                    double* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator double* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<double>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 bool>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jbooleanArray(env, reinterpret_cast<const jboolean*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<bool> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<bool> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JBooleanArrayPointer * data = nullptr;
            if (JBooleanArrayPointer::isValidArray(env, in)) {
                data = new JBooleanArrayPointer(env, jbooleanArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<bool> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<bool>(data->operator const bool* (), data->operator const bool* () + data->size());
#else
                struct{
                    bool* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = const_cast<bool*>(data->operator const bool* ());
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<bool>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 wchar_t>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jcharArray(env, reinterpret_cast<const jchar*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<bool> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<bool> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JCharArrayPointer * data = nullptr;
            if (JCharArrayPointer::isValidArray(env, in)) {
                data = new JCharArrayPointer(env, jcharArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<wchar_t> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<wchar_t>(data->operator const wchar_t* (), data->operator const wchar_t* () + data->size());
#else
                struct{
                    wchar_t* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = data->operator wchar_t* ();
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<wchar_t>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 QChar>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jcharArray(env, reinterpret_cast<const jchar*>(in.begin()), jsize(in.size()));
    }
    static NativeType_out castbck(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<bool> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<bool> *");
        if(scope==nullptr){
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JCharArrayPointer * data = nullptr;
            if (JCharArrayPointer::isValidArray(env, in)) {
                data = new JCharArrayPointer(env, jcharArray(in), false);
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<QChar> out;
            if (data) {
#ifdef Q_OS_WIN32
                out = std::initializer_list<QChar>(reinterpret_cast<const QChar*>(data->operator const wchar_t* ()), reinterpret_cast<const QChar*>(data->operator const wchar_t* ()) + data->size());
#else
                struct{
                    QChar* _M_array;
                    size_t _M_len;
                } __initializer_tmp;
                __initializer_tmp._M_array = reinterpret_cast<QChar*>(data->operator wchar_t* ());
                __initializer_tmp._M_len = size_t(data->size());
                out = *reinterpret_cast<std::initializer_list<QChar>*>(&__initializer_tmp);
#endif
            }
            return out;
        }
    }
};

//template from any std::function to jobject

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 std::function, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::function<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char* nativeTypeName, QtJambiScope*){
        jobject result = nullptr;
        if(!qtjambi_convert_from_native(env, typeid(NativeType), nativeTypeName, &deref_ptr<is_pointer,NativeType_c>::deref(in), true, false, result)){
           JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast from type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::function<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::function<T> *");
        NativeType fct;
        NativeType* result = &fct;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result ? *result : NativeType();
    }
};

template<bool is_pointer,typename O>
struct deref_ptr_if{
};

template<typename O>
struct deref_ptr_if<false,O>{
    static O& deref(O* o){
        return *o;
    }
};

template<typename O>
struct deref_ptr_if<true,O>{
    static O* deref(O* o){
        return o;
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QList, is_pointer, is_const, is_reference,
                                 T>{
    typedef QList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_arraylist_new(env, _in.size());
        qtjambi_collection_fill<has_scope, NativeType, is_const, T>::fill(env, _in, list, scope);
        return list;
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QList(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QList, T>::create(env, scope, list, "QList<T>");
    }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QList, true, is_const, false,
                                 T>
        : qtjambi_QList_caster<true, is_const, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QList(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QList, T>::create(env, scope, list, "QList<T>");
    }
};

template<bool forward, bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, true,
                                 jobject,
                                 QList, false, is_const, true,
                                 T>
        : qtjambi_QList_caster<false, is_const, T>
{
    typedef QList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to const QList<T>& without scope." QTJAMBI_STACKTRACEINFO );
        NativeType *list = nullptr;
        if (in) {
            if (qtjambi_is_QList(env, in, qtjambi_type<T>::id())) {
                list = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
            } else {
                if(is_const){
                    list = new NativeType();
                    scope->addFinalAction([list](){ delete list; });
                }else{
                    list = new IntermediateContainer<QList,T>(env, in, *scope);
                    scope->addFinalAction([list](){ delete static_cast<IntermediateContainer<QList,T>*>(list); });
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_cast<T>(env, element);
                    list->append(__qt_element);
                }
            }
        }else{
            list = new NativeType();
            scope->addFinalAction([list](){ delete list; });
        }
        return *list;
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QSet, is_pointer, is_const, is_reference,
                                 T>{
    typedef QSet<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         LessThanFunction lessThanFct = LessThan<T,supports_less_than<T>::value>::function();
         jobject list = lessThanFct ? qtjambi_treeset_new(env, lessThanFct) : qtjambi_hashset_new(env);
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         qtjambi_collection_fill<has_scope, NativeType, is_const, T>::fill(env, _in, list, scope);
         return list;
    }
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QSet(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QSet, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QSet<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->insert(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QSet, T>::create(env, scope, list, "QSet<T>");
     }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QSet, true, is_const, false,
                                 T>
        : qtjambi_QSet_caster<true, is_const, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QSet<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QSet(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QSet, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QSet<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->insert(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QSet, T>::create(env, scope, list, "QSet<T>");
     }
};

template<bool forward, bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, true,
                                 jobject,
                                 QSet, false, is_const, true,
                                 T>
        : qtjambi_QSet_caster<false, is_const, T>
{
    typedef QSet<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QSet<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QSet(env, in, qtjambi_type<T>::id())) {
                 list = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
             } else {
                 if(is_const){
                     list = new NativeType();
                     scope->addFinalAction([list](){ delete list; });
                 }else{
                     list = new IntermediateContainer<QSet,T>(env, in, *scope);
                     scope->addFinalAction([list](){ delete static_cast<IntermediateContainer<QSet,T>*>(list); });
                 }
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     T  __qt_element = qtjambi_cast<T>(env, element);
                     list->insert(__qt_element);
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QLinkedList, is_pointer, is_const, is_reference,
                                 T>{
    typedef QLinkedList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         jobject list = qtjambi_linkedlist_new(env);
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         qtjambi_collection_fill<has_scope, NativeType, is_const, T>::fill(env, _in, list, scope);
         return list;
    }
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QLinkedList(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QLinkedList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QLinkedList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QLinkedList, T>::create(env, scope, list, "QLinkedList<T>");
     }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QLinkedList, true, is_const, false,
                                 T>
        : qtjambi_QLinkedList_caster<true, is_const, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QLinkedList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QLinkedList(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QLinkedList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QLinkedList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QLinkedList, T>::create(env, scope, list, "QLinkedList<T>");
     }
};

template<bool forward, bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, true,
                                 jobject,
                                 QLinkedList, false, is_const, true,
                                 T>
        : qtjambi_QLinkedList_caster<false, is_const, T>
{
    typedef QLinkedList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QLinkedList<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QLinkedList(env, in, qtjambi_type<T>::id())) {
                 list = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
             } else {
                 if(is_const){
                     list = new NativeType();
                     scope->addFinalAction([list](){ delete list; });
                 }else{
                     list = new IntermediateContainer<QLinkedList,T>(env, in, *scope);
                     scope->addFinalAction([list](){ delete static_cast<IntermediateContainer<QLinkedList,T>*>(list); });
                 }
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     T  __qt_element = qtjambi_cast<T>(env, element);
                     list->append(__qt_element);
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QStack, is_pointer, is_const, is_reference,
                                 T>{
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         jobject list = qtjambi_deque_new(env);
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         qtjambi_collection_fill<has_scope, NativeType, is_const, T>::fill(env, _in, list, scope);
         return list;
    }
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QStack(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QStack, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QStack<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QStack, T>::create(env, scope, list, "QStack<T>");
     }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QStack, true, is_const, false,
                                 T>
        : qtjambi_QStack_caster<true, is_const, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QStack(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QStack, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QStack<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QStack, T>::create(env, scope, list, "QStack<T>");
     }
};

template<bool forward, bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, true,
                                 jobject,
                                 QStack, false, is_const, true,
                                 T>
        : qtjambi_QStack_caster<false, is_const, T>
{
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to const QStack<T>& without scope." QTJAMBI_STACKTRACEINFO );
        QStack<T> *list = nullptr;
        if (in) {
            if (qtjambi_is_QStack(env, in, qtjambi_type<T>::id())) {
                list = reinterpret_cast<QStack<T> *>(qtjambi_to_object(env, in));
            } else {
                if(is_const){
                    list = new QStack<T>();
                    scope->addFinalAction([list](){delete list;});
                }else{
                    list = new IntermediateContainer<QStack,T>(env, in, *scope);
                    scope->addFinalAction([list](){ delete static_cast<IntermediateContainer<QStack,T>*>(list); });
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_cast<T>(env, element);
                    (*list) << __qt_element;
                }
            }
        }else{
            list = new QStack<T>();
            scope->addFinalAction([list](){delete list;});
        }
        return *list;
     }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QQueue, is_pointer, is_const, is_reference,
                                 T>{
    typedef QQueue<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         jobject list = qtjambi_linkedlist_new(env);
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         qtjambi_collection_fill<has_scope, NativeType, is_const, T>::fill(env, _in, list, scope);
         return list;
    }

     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QQueue(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QQueue, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QQueue<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QQueue, T>::create(env, scope, list, "QQueue<T>");
     }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QQueue, true, is_const, false,
                                 T>
        : qtjambi_QQueue_caster<true, is_const, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QQueue<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QQueue(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QQueue, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QQueue<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QQueue, T>::create(env, scope, list, "QQueue<T>");
     }
};

template<bool forward, bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, true,
                                 jobject,
                                 QQueue, false, is_const, true,
                                 T>
        : qtjambi_QQueue_caster<false, is_const, T>
{
    typedef QQueue<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QQueue<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QQueue(env, in, qtjambi_type<T>::id())) {
                 list = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
             } else {
                 if(is_const){
                     list = new NativeType();
                     scope->addFinalAction([list](){ delete list; });
                 }else{
                     list = new IntermediateContainer<QQueue,T>(env, in, *scope);
                     scope->addFinalAction([list](){ delete static_cast<IntermediateContainer<QQueue,T>*>(list); });
                 }
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     T  __qt_element = qtjambi_cast<T>(env, element);
                     list->append(__qt_element);
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QVector, is_pointer, is_const, is_reference,
                                 T>{
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         jobject list = qtjambi_arraylist_new(env, _in.size());
         qtjambi_collection_fill<has_scope, NativeType, is_const, T>::fill(env, _in, list, scope);
         return list;
    }

     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QVector(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QVector, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QVector<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QVector, T>::create(env, scope, list, "QVector<T>");
     }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QVector, true, is_const, false,
                                 T>
        : qtjambi_QVector_caster<true, is_const, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QVector(env, in, qtjambi_type<T>::id())) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QVector, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QVector<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_cast<T>(env, element));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QVector, T>::create(env, scope, list, "QVector<T>");
     }
};

template<bool forward, bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, true,
                                 jobject,
                                 QVector, false, is_const, true,
                                 T>
        : qtjambi_QVector_caster<false, is_const, T>
{
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QVector<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QVector(env, in, qtjambi_type<T>::id())) {
                 list = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
             } else {
                 if(is_const){
                     list = new NativeType();
                     scope->addFinalAction([list](){ delete list; });
                 }else{
                     list = new IntermediateContainer<QVector,T>(env, in, *scope);
                     scope->addFinalAction([list](){ delete static_cast<IntermediateContainer<QVector,T>*>(list); });
                 }
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     T  __qt_element = qtjambi_cast<T>(env, element);
                     list->append(__qt_element);
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};

#ifndef QT_NO_DBUS
template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 T>{
    typedef QDBusReply<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object<QDBusReply<QVariant>>(env, QDBusReplyUtility<T>::createFrom(_in), false, false);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QDBusReply<QVariant>* reply = reinterpret_cast<QDBusReply<QVariant>*>(qtjambi_to_object(env, in));
        if(reply){
            QScopedPointer<NativeType> reply2(QDBusReplyUtility<T>::reverseCreateFrom(*reply));
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QDBusReply, T>::create(
                        env, scope, reply2, "QDBusReply<T>");
        }else{
            return create_container_pointer<is_pointer, is_const, is_reference, false, QDBusReply, T>::create(
                        env, scope, nullptr, "QDBusReply<T>");
        }
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QDBusReply<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object<QDBusReply<QVariant>>(env, QDBusReplyUtility<QVariant>::createFrom(_in), false, false);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QDBusReply<QVariant>* reply = reinterpret_cast<QDBusReply<QVariant>*>(qtjambi_to_object(env, in));
        return create_container_pointer<is_pointer, is_const, is_reference, false, QDBusReply, QVariant>::create(
                    env, scope, reply, "QDBusReply<QVariant>");
    }
};
#endif

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QQmlListProperty, is_pointer, is_const, is_reference,
                                 T>{
    typedef QQmlListProperty<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object<QQmlListProperty<QObject>>(env, QmlListPropertyUtility<T>::createFrom(_in), false, false);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QQmlListProperty, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QQmlListProperty<T>");
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QFutureWatcher, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureWatcher<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_qobject(env, _in);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return reinterpret_cast<QFutureWatcher<T>*>(qtjambi_to_qobject(env, in));
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFuture<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object(env, &_in, true, false);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QFuture, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QFuture<T>");
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QFutureSynchronizer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureSynchronizer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object(env, _in, false, false);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QFutureSynchronizer, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QFutureSynchronizer<T>");
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<forward, has_scope,
                                 jobject,
                                 QPointer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_scoped_cast<has_scope,jobject,T*>::cast(env, _in.data(), nullptr, scope);
    }
    static NativeType_out castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
         Q_STATIC_ASSERT_X(false && is_const, "Cannot cast to QPointer<T>");
         return NativeType_out();
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER1_P_H
