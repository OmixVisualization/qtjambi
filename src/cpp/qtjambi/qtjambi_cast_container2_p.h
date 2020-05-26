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

#ifndef QTJAMBI_CAST_CONTAINER2_P_H
#define QTJAMBI_CAST_CONTAINER2_P_H

#include "qtjambi_core.h"
#include "qtjambi_cast_util_p.h"
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
    static JniType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
    static NativeType_out castbck(JNIEnv *, JniType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
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
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return qtjambi_pair_new(env,
                                qtjambi_scoped_cast<has_scope,jobject,K_const>::cast(env, _in.first, nullptr, scope),
                                qtjambi_scoped_cast<has_scope,jobject,T_const>::cast(env, _in.second, nullptr, scope));
    }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QPair<K,T> &");
        if(in){
            jobject first = qtjambi_pair_get(env, in, 0);
            jobject second = qtjambi_pair_get(env, in, 1);
            QScopedPointer<QPair<K,T>> pair(new QPair<K,T>(qtjambi_cast<K>(env, first), qtjambi_cast<T>(env, second)));
            return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QPair, K, T>::create(
                        env, scope, pair, "QPair<K,T>");
        }else{
            return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QPair, K, T>::create(
                        env, scope, nullptr, "QPair<K,T>");
        }
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QScopedPointer, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QScopedPointer<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject o = qtjambi_scoped_cast<has_scope,jobject,K*>::cast(env, _in.data(), nullptr, scope);
        if(scope){
            scope->addFinalAction([env, o](){ qtjambi_invalidate_object(env, o, false); });
        }
        return o;
   }

    static NativeType_out castbck(JNIEnv * env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QScopedPointer<K,T> &");
        if(in){
            QScopedPointer<QScopedPointer<K,T>> scp(new QScopedPointer<K,T>(qtjambi_cast<K>(env, in)));
            if(scope){
                scope->addFinalAction([env, in](){ qtjambi_invalidate_object(env, in, false); });
            }
            return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QScopedPointer, K, T>::create(
                        env, scope, scp, "QScopedPointer<K,T>");
        }else{
            return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QScopedPointer, K, T>::create(
                        env, scope, nullptr, "QScopedPointer<K,T>");
        }
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        std::unique_ptr, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef std::unique_ptr<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject o = qtjambi_scoped_cast<has_scope,jobject,K*>::cast(env, _in.get(), nullptr, scope);
        if(scope){
            scope->addFinalAction([env, o](){ qtjambi_invalidate_object(env, o, false); });
        }
        return o;
   }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to std::unique_ptr<K,T> &");
        if(in){
            QScopedPointer<std::unique_ptr<K,T>> scp(new std::unique_ptr<K,T>(qtjambi_cast<K>(env, in)));
            if(scope){
                scope->addFinalAction([env, in](){ qtjambi_invalidate_object(env, in, false); });
            }
            return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, std::unique_ptr, K, T>::create(
                        env, scope, scp, "std::unique_ptr<K,T>");
        }else{
            return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, std::unique_ptr, K, T>::create(
                        env, scope, nullptr, "std::unique_ptr<K,T>");
        }
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QUrlTwoFlags, is_pointer, is_const, is_reference,
                                        QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef QUrl::FormattingOptions NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return qtjambi_from_object(env, &_in, typeid(QUrl::FormattingOptions), true, false);
   }
   static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
         Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QUrl::FormattingOptions &");
         Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast jobject to QUrl::FormattingOptions *");
         return QUrl::FormattingOptions(qtjambi_to_enum(env, in));
   }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QMap, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
          NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
          LessThanFunction lessThanFct = LessThan<K,supports_less_than<K>::value>::function();
          jobject list = lessThanFct ? qtjambi_treemap_new(env, lessThanFct) : qtjambi_hashmap_new(env, _in.size());
          qtjambi_bicollection_fill<has_scope, NativeType, is_const, K, T, qtjambi_map_put>::fill(env, _in, list, scope);
          return list;
     }

     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not spport operator <.");
          QScopedPointer<NativeType> map;
          if(in){
              if (qtjambi_is_QMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                  return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QMap, K, T>::create(
                              env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QMap<K,T>");
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                  }
              }
          }
          return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QMap, K, T>::create(env, scope, map, "QMap<K,T>");
     }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QMap, true, is_const, false,
                                        K, T>
        : qtjambi_QMap_caster<true, is_const, K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not spport operator <.");
          QScopedPointer<NativeType> map;
          if(in){
              if (qtjambi_is_QMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                  return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QMap, K, T>::create(
                              env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QMap<K,T>");
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                  }
              }
          }
          return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QMap, K, T>::create(env, scope, map, "QMap<K,T>");
     }
};

template<bool forward,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, true,
                                        jobject,
                                        QMap, false, is_const, true,
                                        K, T>
    : qtjambi_QMap_caster<false, is_const, K, T>
{
    typedef QMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not spport operator <.");
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to QMap<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *map = nullptr;
         if (in) {
             if (qtjambi_is_QMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                 map = reinterpret_cast<NativeType*>(qtjambi_to_object(env, in));
             } else {
                 if(is_const){
                     map = new NativeType();
                     scope->addFinalAction([map](){delete map;});
                 }else{
                     map = new IntermediateBiContainer<QMap,K,T>(env, in, *scope);
                     scope->addFinalAction([map](){ delete static_cast<IntermediateBiContainer<QMap,K,T>*>(map); });
                 }
                 jobject iterator = qtjambi_map_entryset_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject entry = qtjambi_iterator_next(env, iterator);
                     jobject key = qtjambi_map$entry_key(env, entry);
                     jobject val = qtjambi_map$entry_value(env, entry);
                     map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                 }
             }
         }else{
             map = new NativeType();
             scope->addFinalAction([map](){delete map;});
         }
         return *map;
     }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QMultiMap, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QMultiMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        LessThanFunction lessThanFct = LessThan<K,supports_less_than<K>::value>::function();
        jobject list = lessThanFct ? qtjambi_treemap_new(env, lessThanFct) : qtjambi_hashmap_new(env, _in.size());
        qtjambi_bicollection_fill<has_scope, NativeType, is_const, K, T, qtjambi_multimap_put>::fill(env, _in, list, scope);
        return list;
   }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not spport operator <.");
        QScopedPointer<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QMultiMap, K, T>::create(
                            env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QMultiMap<K,T>");
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
        }
        return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QMultiMap, K, T>::create(env, scope, map, "QMultiMap<K,T>");
    }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QMultiMap, true, is_const, false,
                                        K, T>
        : qtjambi_QMultiMap_caster<true, is_const, K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QMultiMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not spport operator <.");
        QScopedPointer<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QMultiMap, K, T>::create(
                            env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QMultiMap<K,T>");
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
        }
        return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QMultiMap, K, T>::create(env, scope, map, "QMultiMap<K,T>");
    }
};

template<bool forward,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, true,
                                        jobject,
                                        QMultiMap, false, is_const, true,
                                        K, T>
        : qtjambi_QMultiMap_caster<false, is_const, K, T>
{
    typedef QMultiMap<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not spport operator <.");
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to QMultiMap<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
        NativeType *map = nullptr;
        if (in) {
            if (qtjambi_is_QMultiMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                map = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
            } else {
                if(is_const){
                    map = new NativeType();
                    scope->addFinalAction([map](){delete map;});
                }else{
                    map = new IntermediateBiContainer<QMultiMap,K,T>(env, in, *scope);
                    scope->addFinalAction([map](){ delete static_cast<IntermediateBiContainer<QMultiMap,K,T>*>(map); });
                }
                scope->addFinalAction([map](){delete map;});
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
        }else{
            map = new NativeType();
            scope->addFinalAction([map](){delete map;});
        }
        return *map;
    }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QHash, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
          NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
          jobject list = qtjambi_hashmap_new(env, _in.size());
          qtjambi_bicollection_fill<has_scope, NativeType, is_const, K, T, qtjambi_map_put>::fill(env, _in, list, scope);
          return list;
     }

      static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
          QScopedPointer<NativeType> map;
          if(in){
              if (qtjambi_is_QHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                  return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QHash, K, T>::create(
                              env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QHash<K,T>");
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                  }
              }
          }
          return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QHash, K, T>::create(env, scope, map, "QHash<K,T>");
      }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QHash, true, is_const, false,
                                        K, T>
        : qtjambi_QHash_caster<true, is_const, K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
          QScopedPointer<NativeType> map;
          if(in){
              if (qtjambi_is_QHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                  return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QHash, K, T>::create(
                              env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QHash<K,T>");
              } else {
                  map.reset(new NativeType());
                  jobject iterator = qtjambi_map_entryset_iterator(env, in);
                  while(qtjambi_iterator_has_next(env, iterator)) {
                      jobject entry = qtjambi_iterator_next(env, iterator);
                      jobject key = qtjambi_map$entry_key(env, entry);
                      jobject val = qtjambi_map$entry_value(env, entry);
                      map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                  }
              }
          }
          return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QHash, K, T>::create(env, scope, map, "QHash<K,T>");
      }
};

template<bool forward,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, true,
                                        jobject,
                                        QHash, false, is_const, true,
                                        K, T>
        : qtjambi_QHash_caster<false, is_const, K, T>
{
    typedef QHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to QHash<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *map = nullptr;
         if (in) {
            if (qtjambi_is_QHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                map = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
            } else {
                if(is_const){
                    map = new NativeType();
                    scope->addFinalAction([map](){delete map;});
                }else{
                    map = new IntermediateBiContainer<QHash,K,T>(env, in, *scope);
                    scope->addFinalAction([map](){ delete static_cast<IntermediateBiContainer<QHash,K,T>*>(map); });
                }
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
         }else{
            map = new NativeType();
            scope->addFinalAction([map](){delete map;});
         }
         return *map;
     }
};

template<bool forward, bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QMultiHash, is_pointer, is_const, is_reference,
                                        K, T>{
    typedef QMultiHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashmap_new(env, _in.size());
        qtjambi_bicollection_fill<has_scope, NativeType, is_const, K, T, qtjambi_multimap_put>::fill(env, _in, list, scope);
        return list;
   }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QMultiHash, K, T>::create(
                            env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QMultiHash<K,T>");
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
        }
        return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QMultiHash, K, T>::create(env, scope, map, "QMultiHash<K,T>");
    }
};

template<bool forward, bool has_scope,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, has_scope,
                                        jobject,
                                        QMultiHash, true, is_const, false,
                                        K, T>
        : qtjambi_QMultiHash_caster<true, is_const, K, T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QMultiHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashmap_new(env, _in.size());
        qtjambi_bicollection_fill<has_scope, NativeType, is_const, K, T, qtjambi_multimap_put>::fill(env, _in, list, scope);
        return list;
   }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> map;
        if(in){
            if (qtjambi_is_QMultiHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                return create_bicontainer_pointer<is_pointer, is_const, is_reference, false, QMultiHash, K, T>::create(
                            env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QMultiHash<K,T>");
            } else {
                map.reset(new NativeType());
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
        }
        return create_bicontainer_pointer<is_pointer, is_const, is_reference, has_scope, QMultiHash, K, T>::create(env, scope, map, "QMultiHash<K,T>");
    }
};

template<bool forward,
         bool is_const,
         typename K, typename T>
struct qtjambi_jnitype_container2_cast<forward, true,
                                        jobject,
                                        QMultiHash, false, is_const, true,
                                        K, T>
        : qtjambi_QMultiHash_caster<false, is_const, K, T>
{
    typedef QMultiHash<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to QMultiHash<K,T>& without scope." QTJAMBI_STACKTRACEINFO );
        NativeType *map = nullptr;
        if (in) {
           if (qtjambi_is_QMultiHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
               map = reinterpret_cast<NativeType *>(qtjambi_to_object(env, in));
           } else {
               if(is_const){
                   map = new NativeType();
                   scope->addFinalAction([map](){delete map;});
               }else{
                   map = new IntermediateBiContainer<QMultiHash,K,T>(env, in, *scope);
                   scope->addFinalAction([map](){ delete static_cast<IntermediateBiContainer<QMultiHash,K,T>*>(map); });
               }
               jobject iterator = qtjambi_map_entryset_iterator(env, in);
               while(qtjambi_iterator_has_next(env, iterator)) {
                   jobject entry = qtjambi_iterator_next(env, iterator);
                   jobject key = qtjambi_map$entry_key(env, entry);
                   jobject val = qtjambi_map$entry_value(env, entry);
                   map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
               }
           }
        }else{
           map = new NativeType();
           scope->addFinalAction([map](){delete map;});
        }
        return *map;
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER2_P_H
