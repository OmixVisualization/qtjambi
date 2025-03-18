/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_CAST_IMPL_TEMPLATE2_H
#define QTJAMBI_CAST_IMPL_TEMPLATE2_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_associative.h"
#include "qtjambi_cast_impl_time.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<K,T>>::type, NativeType<K,T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                 jobject,
                                 std::vector, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::vector<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
        for(const auto& entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            QtJambiAPI::addToJavaCollection(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                 jobject,
                                 std::vector, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::vector<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
        NativeType list;
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
            jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
            list.push_back(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
        }
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, std::move(list));
    }
};

#if defined(_LIST_) || defined(_LIST) || defined(_LIBCPP_LIST) || defined(_GLIBCXX_LIST)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                 jobject,
                                 std::list, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::list<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
        for(const auto& entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            QtJambiAPI::addToJavaCollection(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                 jobject,
                                 std::list, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::list<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
        NativeType list;
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
            jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
            list.push_back(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
        }
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, std::move(list));
    }
};
#endif

#if defined(_FORWARD_LIST_) || defined(_FORWARD_LIST) || defined(_LIBCPP_FORWARD_LIST) || defined(_GLIBCXX_FORWARD_LIST)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                 jobject,
                                 std::forward_list, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::forward_list<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
        for(const auto& entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            QtJambiAPI::addToJavaCollection(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                 jobject,
                                 std::forward_list, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::forward_list<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
        NativeType list;
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
            jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
            list.push_back(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
        }
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, std::move(list));
    }
};
#endif

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                        jobject,
                                        std::pair, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef std::pair<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<is_const, typename std::add_const<K>::type, K>::type K_const;
    typedef typename std::conditional<is_const, typename std::add_const<T>::type, T>::type T_const;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return QtJambiAPI::newQPair(env,
                                qtjambi_scoped_cast<has_scope,jobject,K_const>::cast(env, _in.first, nullptr, scope),
                                qtjambi_scoped_cast<has_scope,jobject,T_const>::cast(env, _in.second, nullptr, scope));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                        jobject,
                                        std::pair, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef std::pair<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<is_const, typename std::add_const<K>::type, K>::type K_const;
    typedef typename std::conditional<is_const, typename std::add_const<T>::type, T>::type T_const;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to std::pair<K,T> &");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
        jobject first = QtJambiAPI::getQPairFirst(env, in);
        jobject second = QtJambiAPI::getQPairSecond(env, in);
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(
            env,
            scope,
            std::pair<K,T>{qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, first, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, second, nullptr, scope)}
        );
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                        jobject,
                                        QPair, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QPair<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<is_const, typename std::add_const<K>::type, K>::type K_const;
    typedef typename std::conditional<is_const, typename std::add_const<T>::type, T>::type T_const;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return QtJambiAPI::newQPair(env,
                                qtjambi_scoped_cast<has_scope,jobject,K_const>::cast(env, _in.first, nullptr, scope),
                                qtjambi_scoped_cast<has_scope,jobject,T_const>::cast(env, _in.second, nullptr, scope));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                        jobject,
                                        QPair, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QPair<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<is_const, typename std::add_const<K>::type, K>::type K_const;
    typedef typename std::conditional<is_const, typename std::add_const<T>::type, T>::type T_const;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QPair<K,T> &");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
        jobject first = QtJambiAPI::getQPairFirst(env, in);
        jobject second = QtJambiAPI::getQPairSecond(env, in);
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(
            env,
            scope,
            QPair<K,T>{qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, first, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, second, nullptr, scope)}
        );
    }
};
#endif

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                        jobject,
                                        QScopedPointer, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QScopedPointer<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        K* qo = _in.take();
        jobject o = qtjambi_scoped_cast<has_scope,jobject,K*>::cast(env, qo, nullptr, scope);
        qtjambi_ownership_decider<K>::setJavaOwnership(env, o, qo);
        return o;
   }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                        jobject,
                                        QScopedPointer, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QScopedPointer<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast jobject to QScopedPointer<K,T>* without scope");

    static NativeType_out cast(JNIEnv * env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1%2 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType))), is_reference ? "&" : "*") QTJAMBI_STACKTRACEINFO );
        NativeType* scp = new NativeType(qtjambi_scoped_cast<has_scope,K*,jobject>::cast(env, in, nullptr, scope));
        scope->addDeletion(scp);
        qtjambi_ownership_decider<K>::setCppOwnershipAndInvalidate(env, in, scp->get(), scope);
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, scp);
    }
};

template<bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                      jobject,
                                      QScopedPointer, false, is_const, false,
                                      K, T>{
    typedef QScopedPointer<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef NativeType_c NativeType_cr;
    typedef NativeType_c NativeType_in;
    typedef NativeType_cr NativeType_out;

    static NativeType_out cast(JNIEnv * env, jobject in, const char*, QtJambiScope* scope){
        K* pointer{nullptr};
        if(in){
            pointer = qtjambi_scoped_cast<has_scope,K*,jobject>::cast(env, in, nullptr, scope);
            qtjambi_ownership_decider<K>::setCppOwnershipAndInvalidate(env, in, pointer, scope);
        }
        return QScopedPointer<K,T>(pointer);
    }
};

#ifdef QURL_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                        jobject,
                                        QUrlTwoFlags, is_pointer, is_const, is_reference,
                                        QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef QUrl::FormattingOptions NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &_in, typeid(QUrl::FormattingOptions));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                        jobject,
                                        QUrlTwoFlags, is_pointer, is_const, is_reference,
                                        QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef QUrl::FormattingOptions NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QUrl::FormattingOptions &");
    Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast jobject to QUrl::FormattingOptions *");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
         return QUrl::FormattingOptions(QtJambiAPI::enumValue<int>(env, in));
    }
};
#endif //QURL_H

#define QTJAMBI_CONTAINER2_CASTER(TYPE)\
template<bool has_scope,\
         bool is_pointer, bool is_const, bool is_reference,\
         typename K, typename T>\
struct qtjambi_jnitype_template2_cast<true, has_scope,\
                                        jobject,\
                                        TYPE, is_pointer, is_const, is_reference,\
                                        K, T>{\
    typedef typename std::conditional<is_const, const TYPE<K,T>,TYPE<K,T>>::type Container;\
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;\
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){\
        return  QtJambiAPI::convert##TYPE##ToJavaObject(env,\
                                           scope ? scope->relatedNativeID() : InvalidNativeID,\
                                           ref_ptr<is_pointer, Container>::ref(in),\
                                           CloneAssociativeContainer<TYPE,K,T, is_pointer && !is_const>::function,\
                                           DeleteAssociativeContainer<TYPE,K,T>::function,\
                                           TYPE##Access<K,T>::newInstance()\
                              );\
     }\
};\
\
template<bool has_scope,\
         bool is_pointer, bool is_const, bool is_reference,\
         typename K, typename T>\
struct qtjambi_jnitype_template2_cast<false, has_scope,\
                                        jobject,\
                                        TYPE, is_pointer, is_const, is_reference,\
                                        K, T>{\
    typedef TYPE<K,T> NativeType;\
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
    STATICASSERT\
\
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){\
        NativeType* pointer{nullptr};\
        if(in){\
            if (!ContainerAPI::getAs##TYPE<K,T>(env, in, pointer)) {\
                if(!scope)\
                    JavaException::raiseError(env, QStringLiteral("Cannot cast to %1%2 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType))), is_reference ? "&" : "*") QTJAMBI_STACKTRACEINFO );\
                if(is_const){\
                    pointer = new NativeType();\
                    scope->addDeletion(pointer);\
                }else{\
                    auto ipointer = new IntermediateAssociativeContainer<has_scope,TYPE,K,T>(env, in, scope);\
                    pointer = ipointer;\
                    scope->addDeletion(ipointer);\
                }\
                jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                    jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                    pointer->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));\
                }\
            }\
        }\
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, false, NativeType>::convert(env, scope, pointer);\
    }\
};\
\
template<bool has_scope,\
         bool is_const,\
         typename K, typename T>\
struct qtjambi_jnitype_template2_cast<false, has_scope,\
                                        jobject,\
                                        TYPE, false, is_const, false,\
                                        K, T>\
{\
    static constexpr bool is_pointer = false;\
    static constexpr bool is_reference = false;\
    typedef TYPE<K,T> NativeType;\
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
    STATICASSERT\
\
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){\
        if(!in)\
            return {};\
        NativeType* pointer{nullptr};\
        if (ContainerAPI::getAs##TYPE<K,T>(env, in, pointer)) {\
            return *pointer;\
        } else {\
            NativeType map;\
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                map.insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));\
            }\
            return map;\
        }\
    }\
};

#define STATICASSERT Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");
QTJAMBI_CONTAINER2_CASTER(QMap)
QTJAMBI_CONTAINER2_CASTER(QMultiMap)
#undef STATICASSERT
#define STATICASSERT
QTJAMBI_CONTAINER2_CASTER(QHash)
QTJAMBI_CONTAINER2_CASTER(QMultiHash)
#undef QTJAMBI_CONTAINER2_CASTER
#undef STATICASSERT

#if defined(_MEMORY_) || defined(_LIBCPP_MEMORY) || defined(_GLIBCXX_MEMORY)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<true, has_scope,
                                        jobject,
                                        std::unique_ptr, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef std::unique_ptr<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        K* qo = _in.release();
        jobject o = qtjambi_scoped_cast<has_scope,jobject,K*>::cast(env, qo, nullptr, scope);
        qtjambi_ownership_decider<K>::setJavaOwnership(env, o, qo);
        return o;
   }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<false, has_scope,
                                        jobject,
                                        std::unique_ptr, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef std::unique_ptr<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!(is_reference || is_pointer) || has_scope, "Cannot cast jobject to std::unique_ptr<K,T> pointer or reference");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
        NativeType pointer;
        pointer.reset(qtjambi_scoped_cast<has_scope,K*,jobject>::cast(env, in, nullptr, scope));
        qtjambi_ownership_decider<K>::setCppOwnershipAndInvalidate(env, in, pointer.get(), scope);
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, std::move(pointer));
    }
};
#endif // defined(_MEMORY_)

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<forward, has_scope,
                                      jobject,
                                      std::chrono::duration, is_pointer, is_const, is_reference,
                                      K, T> : qtjambi_jnitype_crono_duration_cast<forward, has_scope, is_pointer, is_const, is_reference, std::chrono::duration<K,T>>{
};


template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_template2_cast<forward, has_scope,
                                      jobject,
                                      std::chrono::time_point, is_pointer, is_const, is_reference,
                                      K, T> : qtjambi_jnitype_crono_time_point_cast<forward, has_scope, is_pointer, is_const, is_reference, std::chrono::time_point<K,T>>{
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_TEMPLATE2_H
