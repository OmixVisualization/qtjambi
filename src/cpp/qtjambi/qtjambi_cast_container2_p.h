/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_CONTAINER2_P_H
#define QTJAMBI_CAST_CONTAINER2_P_H

#include "qtjambi_cast_container2_util_p.h"
#include "qtjambi_cast_map_p.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<K,T>>::type, NativeType<K,T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        jobject list = qtjambi_arraylist_new(env, jint(_in.size()));
        for(decltype(*(_in.begin())) entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            qtjambi_collection_add(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                 jobject,
                                 std::vector, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::vector<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> list;
        if(in){
            list.reset(new NativeType());
            jobject iterator = qtjambi_collection_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject element = qtjambi_iterator_next(env, iterator);
                list->push_back(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};

#if defined(_LIST_) || defined(_LIST) || defined(_LIBCPP_LIST) || defined(_GLIBCXX_LIST)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        jobject list = qtjambi_arraylist_new(env, jint(_in.size()));
        for(decltype(*(_in.begin())) entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            qtjambi_collection_add(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                 jobject,
                                 std::list, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::list<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> list;
        if(in){
            list.reset(new NativeType());
            jobject iterator = qtjambi_collection_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject element = qtjambi_iterator_next(env, iterator);
                list->push_back(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};
#endif

#if defined(_FORWARD_LIST_) || defined(_FORWARD_LIST) || defined(_LIBCPP_FORWARD_LIST) || defined(_GLIBCXX_FORWARD_LIST)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        jobject list = qtjambi_arraylist_new(env, jint(_in.size()));
        for(decltype(*(_in.begin())) entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            qtjambi_collection_add(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, class _Alloc>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                 jobject,
                                 std::forward_list, is_pointer, is_const, is_reference,
                                 T, _Alloc>{
    typedef std::forward_list<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> list;
        if(in){
            list.reset(new NativeType());
            jobject iterator = qtjambi_collection_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject element = qtjambi_iterator_next(env, iterator);
                list->push_back(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};
#endif

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        return qtjambi_pair_new(env,
                                qtjambi_scoped_cast<has_scope,jobject,K_const>::cast(env, _in.first, nullptr, scope),
                                qtjambi_scoped_cast<has_scope,jobject,T_const>::cast(env, _in.second, nullptr, scope));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
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
        std::unique_ptr<std::pair<K,T>> pair;
        if(in){
            jobject first = qtjambi_pair_get(env, in, 0);
            jobject second = qtjambi_pair_get(env, in, 1);
            pair.reset(new std::pair<K,T>(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, first, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, second, nullptr, scope)));
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, pair);
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        return qtjambi_pair_new(env,
                                qtjambi_scoped_cast<has_scope,jobject,K_const>::cast(env, _in.first, nullptr, scope),
                                qtjambi_scoped_cast<has_scope,jobject,T_const>::cast(env, _in.second, nullptr, scope));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
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
        std::unique_ptr<QPair<K,T>> pair;
        if(in){
            jobject first = qtjambi_pair_get(env, in, 0);
            jobject second = qtjambi_pair_get(env, in, 1);
            pair.reset(new QPair<K,T>(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, first, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, second, nullptr, scope)));
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, pair);
    }
};
#endif

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        jobject o = qtjambi_scoped_cast<has_scope,jobject,K*>::cast(env, _in.data(), nullptr, scope);
        if(scope)
            scope->addObjectInvalidation(env, o, false, false);
        return o;
   }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QScopedPointer, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QScopedPointer<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QScopedPointer<K,T> &");

    static NativeType_out cast(JNIEnv * env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<QScopedPointer<K,T>> scp;
        if(in){
            scp.reset(new QScopedPointer<K,T>(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, in, nullptr, scope)));
            if(scope)
                scope->addObjectInvalidation(env, in, false, false);
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, scp);
    }
};

#ifdef QURL_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        return qtjambi_from_object(env, &_in, typeid(QUrl::FormattingOptions), true, false);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container2_cast<false, has_scope,
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
         return QUrl::FormattingOptions(qtjambi_to_enum(env, in));
    }
};
#endif //QURL_H

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
                                        jobject,
                                        QMap, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef typename std::conditional<is_const, const QMap<K,T>,QMap<K,T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
          return  qtjambi_from_QMap(env,
                                           scope ? scope->relatedNativeID() : InvalidNativeID,
                                           ref_ptr<is_pointer, Container>::ref(in),
                                           CloneBiContainer<QMap,K,T, is_pointer>::function,
                                           DeleteBiContainer<QMap,K,T>::function,
                                           QMapAccess<K,T,is_pointer && is_const>::newInstance()
                              );
     }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QMap, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
          std::unique_ptr<NativeType> map;
          if(in){
              if (qtjambi_is_QMap<K,T>(env, in)) {
                  return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                              env, scope, qtjambi_to_object<NativeType>(env, in));
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                  }
              }
          }
          return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};

template<bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QMap, true, is_const, false,
                                        K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
          std::unique_ptr<NativeType> map;
          if(in){
              if (qtjambi_is_QMap<K,T>(env, in)) {
                  return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                              env, scope, qtjambi_to_object<NativeType>(env, in));
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                  }
              }
          }
          return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
     }
};

template<bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, true,
                                        jobject,
                                        QMap, false, is_const, true,
                                        K, T>
{
    typedef QMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to QMap<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *map = nullptr;
         if (in) {
             if (qtjambi_is_QMap<K,T>(env, in)) {
                 map = qtjambi_to_object<NativeType>(env, in);
             } else {
                 if(is_const){
                     map = new NativeType();
                     scope->addDeletion(map);
                 }else{
                     map = new IntermediateBiContainer<QMap,K,T>(env, in, *scope);
                     scope->addDeletion(static_cast<IntermediateBiContainer<QMap,K,T>*>(map));
                 }
                 jobject iterator = qtjambi_map_entryset_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject entry = qtjambi_iterator_next(env, iterator);
                     jobject key = qtjambi_map$entry_key(env, entry);
                     jobject val = qtjambi_map$entry_value(env, entry);
                     map->insert(qtjambi_scoped_cast<true,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<true,T,jobject>::cast(env, val, nullptr, scope));
                 }
             }
         }else{
             map = new NativeType();
             scope->addDeletion(map);
         }
         return *map;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
                                        jobject,
                                        QMultiMap, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef typename std::conditional<is_const, const QMultiMap<K,T>,QMultiMap<K,T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
          return  qtjambi_from_QMultiMap(env,
                                           scope ? scope->relatedNativeID() : InvalidNativeID,
                                           ref_ptr<is_pointer, Container>::ref(in),
                                           CloneBiContainer<QMultiMap,K,T,is_pointer>::function,
                                           DeleteBiContainer<QMultiMap,K,T>::function,
                                           QMultiMapAccess<K,T, is_pointer && is_const>::newInstance()
                              );
     }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QMultiMap, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QMultiMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiMap<K,T>(env, in)) {
                return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                            env, scope, qtjambi_to_object<NativeType>(env, in));
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};

template<bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QMultiMap, true, is_const, false,
                                        K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QMultiMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiMap<K,T>(env, in)) {
                return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                            env, scope, qtjambi_to_object<NativeType>(env, in));
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};

template<bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, true,
                                        jobject,
                                        QMultiMap, false, is_const, true,
                                        K, T>
{
    typedef QMultiMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to QMultiMap<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
        NativeType *map = nullptr;
        if (in) {
            if (qtjambi_is_QMultiMap<K,T>(env, in)) {
                map = qtjambi_to_object<NativeType>(env, in);
            } else {
                if(is_const){
                    map = new NativeType();
                    scope->addDeletion(map);
                }else{
                    map = new IntermediateBiContainer<QMultiMap,K,T>(env, in, *scope);
                    scope->addDeletion(static_cast<IntermediateBiContainer<QMultiMap,K,T>*>(map));
                }
                scope->addDeletion(map);
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<true,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<true,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }else{
            map = new NativeType();
            scope->addDeletion(map);
        }
        return *map;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
                                        jobject,
                                        QHash, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef typename std::conditional<is_const, const QHash<K,T>,QHash<K,T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
          return  qtjambi_from_QHash(env,
                                           scope ? scope->relatedNativeID() : InvalidNativeID,
                                           ref_ptr<is_pointer, Container>::ref(in),
                                           CloneBiContainer<QHash,K,T, is_pointer>::function,
                                           DeleteBiContainer<QHash,K,T>::function,
                                           QHashAccess<K,T,is_pointer && is_const>::newInstance()
                              );
     }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QHash, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
          std::unique_ptr<NativeType> map;
          if(in){
              if (qtjambi_is_QHash<K,T>(env, in)) {
                  return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                              env, scope, qtjambi_to_object<NativeType>(env, in));
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                  }
              }
          }
          return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};

template<bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QHash, true, is_const, false,
                                        K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
          std::unique_ptr<NativeType> map;
          if(in){
              if (qtjambi_is_QHash<K,T>(env, in)) {
                  return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                              env, scope, qtjambi_to_object<NativeType>(env, in));
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                  }
              }
          }
          return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
      }
};

template<bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, true,
                                        jobject,
                                        QHash, false, is_const, true,
                                        K, T>
{
    typedef QHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to QHash<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *map = nullptr;
         if (in) {
            if (qtjambi_is_QHash<K,T>(env, in)) {
                map = qtjambi_to_object<NativeType>(env, in);
            } else {
                if(is_const){
                    map = new NativeType();
                    scope->addDeletion(map);
                }else{
                    map = new IntermediateBiContainer<QHash,K,T>(env, in, *scope);
                    scope->addDeletion(static_cast<IntermediateBiContainer<QHash,K,T>*>(map));
                }
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<true,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<true,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
         }else{
            map = new NativeType();
            scope->addDeletion(map);
         }
         return *map;
     }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
                                        jobject,
                                        QMultiHash, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef typename std::conditional<is_const, const QMultiHash<K,T>,QMultiHash<K,T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
          return  qtjambi_from_QMultiHash(env,
                                           scope ? scope->relatedNativeID() : InvalidNativeID,
                                           ref_ptr<is_pointer, Container>::ref(in),
                                           CloneBiContainer<QMultiHash,K,T, is_pointer>::function,
                                           DeleteBiContainer<QMultiHash,K,T>::function,
                                           QMultiHashAccess<K,T,is_pointer && is_const>::newInstance()
                              );
     }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QMultiHash, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QMultiHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiHash<K,T>(env, in)) {
                return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                            env, scope, qtjambi_to_object<NativeType>(env, in));
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};

template<bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        QMultiHash, true, is_const, false,
                                        K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QMultiHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    /*static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashmap_new(env, _in.size());
        qtjambi_bicollection_fill<has_scope, NativeType, is_const, K, T, qtjambi_multimap_put>::fill(env, _in, list, scope);
        return list;
    }*/

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiHash<K,T>(env, in)) {
                return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(
                            env, scope, qtjambi_to_object<NativeType>(env, in));
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};

template<bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, true,
                                        jobject,
                                        QMultiHash, false, is_const, true,
                                        K, T>
{
    typedef QMultiHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to QMultiHash<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
        NativeType *map = nullptr;
        if (in) {
           if (qtjambi_is_QMultiHash<K,T>(env, in)) {
               map = qtjambi_to_object<NativeType>(env, in);
           } else {
               if(is_const){
                   map = new NativeType();
                   scope->addDeletion(map);
               }else{
                   map = new IntermediateBiContainer<QMultiHash,K,T>(env, in, *scope);
                   scope->addDeletion(static_cast<IntermediateBiContainer<QMultiHash,K,T>*>(map));
               }
               jobject iterator = qtjambi_map_entryset_iterator(env, in);
               while(qtjambi_iterator_has_next(env, iterator)) {
                   jobject entry = qtjambi_iterator_next(env, iterator);
                   jobject key = qtjambi_map$entry_key(env, entry);
                   jobject val = qtjambi_map$entry_value(env, entry);
                   map->insert(qtjambi_scoped_cast<true,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<true,T,jobject>::cast(env, val, nullptr, scope));
               }
           }
        }else{
           map = new NativeType();
           scope->addDeletion(map);
        }
        return *map;
    }
};

#if defined(_MEMORY_) || defined(_LIBCPP_MEMORY) || defined(_GLIBCXX_MEMORY)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<true, has_scope,
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
        jobject o = qtjambi_scoped_cast<has_scope,jobject,K*>::cast(env, _in.get(), nullptr, scope);
        if(scope)
            scope->addObjectInvalidation(env, o, false, false);
        return o;
   }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<false, has_scope,
                                        jobject,
                                        std::unique_ptr, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef std::unique_ptr<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to std::unique_ptr<K,T> &");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(in){
            QScopedPointer<std::unique_ptr<K,T>> scp(new std::unique_ptr<K,T>(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, in, nullptr, scope)));
            if(scope)
                scope->addObjectInvalidation(env, in, false, false);
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(
                        env, scope, scp);
        }else{
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(
                        env, scope, nullptr);
        }
    }
};
#endif // defined(_MEMORY_)

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER2_P_H
