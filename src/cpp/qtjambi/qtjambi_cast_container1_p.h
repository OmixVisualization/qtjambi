/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambi_cast_container1_util_p.h"
#include "qtjambi_cast_sharedpointer_p.h"
#ifdef QDBUSREPLY_H
#include <qtjambi_dbus/QDBusVariantReply.h>
#endif
#ifdef QQMLLISTPROPERTY
#include <qtjambi_qml/qqmllistproperty.h>
#endif

QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
QT_WARNING_DISABLE_DEPRECATED

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

//template from any type's QWeakPointer<T> to jobject
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QWeakPointer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QWeakPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         if(QSharedPointer<T> ptr = deref_ptr<is_pointer,NativeType_c>::deref(in).toStrongRef()){
              return qtjambi_scoped_cast<has_scope,jobject,QSharedPointer<T>>::cast(env, ptr, nullptr, scope);
         }else
             return nullptr;
     }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QWeakPointer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QWeakPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QWeakPointer<T> &");
         Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QWeakPointer<T> *");
         QSharedPointer<T> pointer = qtjambi_scoped_cast<has_scope,QSharedPointer<T>,jobject>::cast(env, in, nullptr, scope);
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

//template from any QFlags to jobject
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QFlags, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFlags<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_flags(env, deref_ptr<is_pointer,NativeType_c>::deref(in));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QFlags, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFlags<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QFlags<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QFlags<T> *");
        return QFlags<T>(qtjambi_to_enum(env, in));
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

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QList, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QList<T>,QList<T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        return qtjambi_from_QList(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Container>::ref(in),
                                  CloneContainer<QList,T, is_pointer>::function,
                                  DeleteContainer<QList,T>::function,
                                  ListType::QList,
                                  QListAccess<T,is_pointer && is_const>::newInstance()
                                );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QList, is_pointer, is_const, is_reference,
                                 T>{
    typedef QList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QList<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QList, T>::create(env, scope, list, "QList<T>");
    }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QList, true, is_const, false,
                                 T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QList<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QList, T>::create(env, scope, list, "QList<T>");
    }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, true,
                                 jobject,
                                 QList, false, is_const, true,
                                 T>
{
    typedef QList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to const QList<T>& without scope." QTJAMBI_STACKTRACEINFO );
        NativeType *list = nullptr;
        if (in) {
            if (qtjambi_is_QList<T>(env, in)) {
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
                    list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                }
            }
        }else{
            list = new NativeType();
            scope->addFinalAction([list](){ delete list; });
        }
        return *list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QSet, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QSet<T>,QSet<T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        return qtjambi_from_QSet(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Container>::ref(in),
                                  CloneContainer<QSet,T,is_pointer>::function,
                                  DeleteContainer<QSet,T>::function,
                                  QSetAccess<T,is_pointer && is_const>::newInstance()
                                );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QSet, is_pointer, is_const, is_reference,
                                 T>{
    typedef QSet<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QSet<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QSet, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QSet<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->insert(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QSet, T>::create(env, scope, list, "QSet<T>");
     }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QSet, true, is_const, false,
                                 T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QSet<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QSet<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QSet, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QSet<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->insert(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QSet, T>::create(env, scope, list, "QSet<T>");
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, true,
                                 jobject,
                                 QSet, false, is_const, true,
                                 T>
{
    typedef QSet<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QSet<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QSet<T>(env, in)) {
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
                     list->insert(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QLINKEDLIST_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QLinkedList, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QLinkedList<T>,QLinkedList<T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        return qtjambi_from_QLinkedList(env,
                                        scope ? scope->relatedNativeID() : InvalidNativeID,
                                        ref_ptr<is_pointer, Container>::ref(in),
                                        CloneContainer<QLinkedList,T,is_pointer>::function,
                                        DeleteContainer<QLinkedList,T>::function,
                                        QLinkedListAccess<T,is_pointer && is_const>::newInstance()
                                 );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QLinkedList, is_pointer, is_const, is_reference,
                                 T>{
    typedef QLinkedList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QLinkedList<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QLinkedList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QLinkedList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QLinkedList, T>::create(env, scope, list, "QLinkedList<T>");
     }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QLinkedList, true, is_const, false,
                                 T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QLinkedList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QLinkedList<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QLinkedList, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QLinkedList<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QLinkedList, T>::create(env, scope, list, "QLinkedList<T>");
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, true,
                                 jobject,
                                 QLinkedList, false, is_const, true,
                                 T>
{
    typedef QLinkedList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QLinkedList<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QLinkedList<T>(env, in)) {
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
                     list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};
#endif

#ifdef QVECTOR_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QVector, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QVector<T>,QVector<T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        return qtjambi_from_QVector(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<is_pointer, Container>::ref(in),
                                     CloneContainer<QVector,T,is_pointer>::function,
                                     DeleteContainer<QVector,T>::function,
                                     VectorType::QVector,
                                     QVectorAccess<T,is_pointer && is_const>::newInstance()
                                 );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QVector, is_pointer, is_const, is_reference,
                                 T>{
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QVector<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QVector, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QVector<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QVector, T>::create(env, scope, list, "QVector<T>");
    }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QVector, true, is_const, false,
                                 T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QVector<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QVector, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QVector<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QVector, T>::create(env, scope, list, "QVector<T>");
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, true,
                                 jobject,
                                 QVector, false, is_const, true,
                                 T>
{
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QVector<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QVector<T>(env, in)) {
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
                     list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};
#endif
#endif

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QStack, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QStack<T>,QStack<T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        return qtjambi_from_QVector(env,
                                    scope ? scope->relatedNativeID() : InvalidNativeID,
                                    ref_ptr<is_pointer, Container>::ref(in),
                                    CloneContainer<QStack,T,is_pointer>::function,
                                    DeleteContainer<QStack,T>::function,
                                    VectorType::QStack,
                                    QVectorAccess<T,is_pointer && is_const>::newInstance()
                                 );
#else
        return qtjambi_from_QList(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Container>::ref(in),
                                  CloneContainer<QStack,T,is_pointer>::function,
                                  DeleteContainer<QStack,T>::function,
                                  ListType::QStack,
                                  QListAccess<T,false>::newInstance()
                                 );
#endif
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QStack, is_pointer, is_const, is_reference,
                                 T>{
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QStack<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QStack, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QStack<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QStack, T>::create(env, scope, list, "QStack<T>");
    }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QStack, true, is_const, false,
                                 T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QStack<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QStack, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QStack<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QStack, T>::create(env, scope, list, "QStack<T>");
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, true,
                                 jobject,
                                 QStack, false, is_const, true,
                                 T>
{
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to const QStack<T>& without scope." QTJAMBI_STACKTRACEINFO );
        QStack<T> *list = nullptr;
        if (in) {
            if (qtjambi_is_QStack<T>(env, in)) {
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
                    T  __qt_element = qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope);
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

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QQueue, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QQueue<T>,QQueue<T>>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        return qtjambi_from_QList(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Container>::ref(in),
                                  CloneContainer<QQueue,T,is_pointer>::function,
                                  DeleteContainer<QQueue,T>::function,
                                  ListType::QQueue,
                                  QListAccess<T,is_pointer && is_const>::newInstance()
                                );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QQueue, is_pointer, is_const, is_reference,
                                 T>{
    typedef QQueue<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QQueue<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QQueue, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QQueue<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QQueue, T>::create(env, scope, list, "QQueue<T>");
     }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QQueue, true, is_const, false,
                                 T>
{
    static constexpr bool is_pointer = true;
    static constexpr bool is_reference = false;
    typedef QQueue<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         QScopedPointer<NativeType> list;
         if(in){
             if (qtjambi_is_QQueue<T>(env, in)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, QQueue, T>::create(
                             env, scope, reinterpret_cast<NativeType *>(qtjambi_to_object(env, in)), "QQueue<T>");
             }else{
                 list.reset(new NativeType());
                 jobject iterator = qtjambi_collection_iterator(env, in);
                 while(qtjambi_iterator_has_next(env, iterator)) {
                     jobject element = qtjambi_iterator_next(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, QQueue, T>::create(env, scope, list, "QQueue<T>");
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_container1_cast<false, true,
                                 jobject,
                                 QQueue, false, is_const, true,
                                 T>
{
    typedef QQueue<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QQueue<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (qtjambi_is_QQueue<T>(env, in)) {
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
                     list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addFinalAction([list](){ delete list; });
         }
         return *list;
     }
};

#ifdef QDBUSREPLY_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 T>{
    typedef QDBusReply<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         jobject result = qtjambi_from_object<QDBusReply<QVariant>>(env, QDBusReplyUtility<T>::createFrom(_in), false, false);
         qtjambi_set_java_ownership(env, result);
         return result;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 T>{
    typedef QDBusReply<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
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

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QDBusReply<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         jobject result = qtjambi_from_object<QDBusReply<QVariant>>(env, QDBusReplyUtility<QVariant>::createFrom(_in), false, false);
         qtjambi_set_java_ownership(env, result);
         return result;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QDBusReply<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QDBusReply<QVariant>* reply = reinterpret_cast<QDBusReply<QVariant>*>(qtjambi_to_object(env, in));
        return create_container_pointer<is_pointer, is_const, is_reference, false, QDBusReply, QVariant>::create(
                    env, scope, reply, "QDBusReply<QVariant>");
    }
};
#endif

#ifdef QQMLLISTPROPERTY
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QQmlListProperty, is_pointer, is_const, is_reference,
                                 T>{
    typedef QQmlListProperty<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         jobject result = qtjambi_from_object<QQmlListProperty<QObject>>(env, QmlListPropertyUtility<T>::createFrom(_in), false, false);
         qtjambi_set_java_ownership(env, result);
         return result;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QQmlListProperty, is_pointer, is_const, is_reference,
                                 T>{
    typedef QQmlListProperty<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QQmlListProperty, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QQmlListProperty<T>");
    }
};
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifdef QPROPERTY_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QBindable, is_pointer, is_const, is_reference,
                                 T>{
    typedef QBindable<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
         return qtjambi_from_object(env, _in, qtjambi_type<NativeType>::id(), true);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QBindable, is_pointer, is_const, is_reference,
                                 T>{
    typedef QBindable<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return reinterpret_cast<NativeType*>(qtjambi_to_object(env, in));
    }
};


template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QPropertyAlias, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyAlias<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
         return qtjambi_from_object(env, _in, qtjambi_type<NativeType>::id(), true);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QPropertyAlias, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyAlias<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return reinterpret_cast<NativeType*>(qtjambi_to_object(env, in));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QPropertyBinding, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyBinding<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
         return qtjambi_from_object(env, _in, qtjambi_type<NativeType>::id(), true);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QPropertyBinding, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyBinding<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return reinterpret_cast<NativeType*>(qtjambi_to_object(env, in));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QPropertyChangeHandler, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyChangeHandler<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
         return qtjambi_from_object(env, _in, qtjambi_type<NativeType>::id(), true);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QPropertyChangeHandler, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyChangeHandler<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return reinterpret_cast<NativeType*>(qtjambi_to_object(env, in));
    }
};
#endif
#endif

#ifdef QFUTUREWATCHER_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QFutureWatcher, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureWatcher<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_qobject(env, _in);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QFutureWatcher, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureWatcher<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return reinterpret_cast<QFutureWatcher<T>*>(qtjambi_to_qobject(env, in));
    }
};
#endif

#ifdef QPROMISE_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QPromise, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPromise<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object(env, &_in, false, false);
    }
};

template<bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, true,
                                 jobject,
                                 QPromise, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPromise<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         jobject result = qtjambi_from_object(env, &_in, false, false);
         if(scope){
             scope->addFinalAction([env, result](){qtjambi_invalidate_object(env, result);});
         }
         return result;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QPromise, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPromise<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QPromise, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QPromise<T>");
    }
};
#endif

#ifdef QFUTURE_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFuture<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object(env, &_in, true, false);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFuture<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QFuture, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QFuture<T>");
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QFutureIterator, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureIterator<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object(env, &_in, true, false);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QFutureIterator, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureIterator<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QFutureInterface, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QFutureInterface<T>");
    }
};
#endif

#ifdef QFUTUREINTERFACE_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QFutureInterface, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureInterface<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_from_object(env, &_in, true, false);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QFutureInterface, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureInterface<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QFutureInterface, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QFutureInterface<T>");
    }
};
#endif

#ifdef QFUTURESYNCHRONIZER_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QFutureSynchronizer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureSynchronizer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
         return qtjambi_from_object(env, _in, false, false);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QFutureSynchronizer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureSynchronizer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, false, QFutureSynchronizer, T>::create(
                    env, scope, reinterpret_cast<NativeType*>(qtjambi_to_object(env, in)), "QFutureSynchronizer<T>");
    }
};
#endif

#if defined(_OPTIONAL_) || defined(_OPTIONAL) || (defined(_LIBCPP_OPTIONAL) && _LIBCPP_STD_VER > 14) || defined(_GLIBCXX_OPTIONAL)

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::optional<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         jobject content = _in.has_value() ? qtjambi_scoped_cast<has_scope,jobject,decltype(_in.value())>::cast(env, _in.value(), nullptr, scope) : nullptr;
         return qtjambi_make_optional(env, _in.has_value(), content);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 qint32>{
    typedef std::optional<qint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_make_optionalint(env, _in.has_value(), _in.has_value() ? jint(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 quint32>{
    typedef std::optional<quint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_make_optionalint(env, _in.has_value(), _in.has_value() ? jint(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 qint64>{
    typedef std::optional<qint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_make_optionallong(env, _in.has_value(), _in.has_value() ? jlong(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 quint64>{
    typedef std::optional<quint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_make_optionallong(env, _in.has_value(), _in.has_value() ? jlong(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 double>{
    typedef std::optional<double> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_make_optionaldouble(env, _in.has_value(), _in.has_value() ? jdouble(_in.value()) : 0);
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::optional<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        bool isPresent = false;
        jobject value = qtjambi_read_optional(env, in, isPresent);
        if(isPresent){
            return NativeType(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope));
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 qint32>{
    typedef std::optional<qint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        bool isPresent = false;
        jint value = qtjambi_read_optionalint(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 quint32>{
    typedef std::optional<quint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        bool isPresent = false;
        jint value = qtjambi_read_optionalint(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 qint64>{
    typedef std::optional<qint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        bool isPresent = false;
        jlong value = qtjambi_read_optionallong(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 quint64>{
    typedef std::optional<quint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        bool isPresent = false;
        jlong value = qtjambi_read_optionallong(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, hasScope,
                                 jobject,
                                 std::optional, is_pointer, is_const, is_reference,
                                 double>{
    typedef std::optional<double> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        bool isPresent = false;
        jdouble value = qtjambi_read_optionaldouble(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};
#endif

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 QPointer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         return qtjambi_scoped_cast<has_scope,jobject,T*>::cast(env, _in.data(), nullptr, scope);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QPointer, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QPointer<T>&");
    Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QPointer<T>*");
    Q_STATIC_ASSERT_X(!is_const, "Cannot cast to const QPointer<T>");

    constexpr static NativeType_out cast(JNIEnv *, jobject, const char*, QtJambiScope*){
         return NativeType_out();
    }
};

template<bool has_scope,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 QPointer, false, false, false,
                                 T>{
    static QPointer<T> cast(JNIEnv * env, jobject o, const char*, QtJambiScope* scope){
        T* object = qtjambi_scoped_cast<has_scope,T*,jobject>::cast(env, o, nullptr, scope);
        return QPointer<T>(object);
    }
};

#if defined(_MEMORY_) || defined(_LIBCPP_MEMORY) || defined(_GLIBCXX_MEMORY)
//template from any type's std::weak_ptr<T> to jobject
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 std::weak_ptr, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::weak_ptr<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         if(std::shared_ptr<T> ptr = std::shared_ptr<T>(deref_ptr<is_pointer,NativeType_c>::deref(in))){
              return qtjambi_scoped_cast<has_scope,jobject,std::shared_ptr<T>>::cast(env, ptr, nullptr, scope);
         }else
             return nullptr;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 std::weak_ptr, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::weak_ptr<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
         Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::weak_ptr<T> &");
         Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::weak_ptr<T> *");
         std::shared_ptr<T> pointer = qtjambi_scoped_cast<has_scope,std::shared_ptr<T>,jobject>::cast(env, in, nativeTypeName, scope);
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
#endif //defined(_MEMORY_)

#if defined(_INITIALIZER_LIST_) || defined(_INITIALIZER_LIST) || defined(INITIALIZER_LIST) || defined(_LIBCPP_INITIALIZER_LIST) || defined(_GLIBCXX_INITIALIZER_LIST)

QT_WARNING_DISABLE_GCC("-Winit-list-lifetime")

//template from any std::initializer_list to jobject
template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::initializer_list<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<std::is_pointer<T>::value, T, typename std::add_const<T>::type>::type T_const;
    static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        return qtjambi_to_jobjectArray(env, qtjambi_type<T>::id(), ref_ptr<is_pointer, NativeType_c>::ref(in), jsize(in.size()),
                       [scope](JNIEnv * env,const void* in, jsize index)->jobject{
                            return qtjambi_scoped_cast<has_scope,jobject,T_const>::cast(env,
                                        reinterpret_cast<typename std::add_const<NativeType>::type*>(in)->begin()[index], nullptr, scope);
                       }
                   );
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::initializer_list<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<std::is_pointer<T>::value, T, typename std::add_const<T>::type>::type T_const;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<T> *");
        if(scope==nullptr){
            JavaException::raiseError(env, "Cannot cast to std::initializer_list<T>" QTJAMBI_STACKTRACEINFO );
            return NativeType();
        }else{
            JObjectArrayPointer<T>* data = nullptr;
            if (qtjambi_is_valid_array(env, in, qtjambi_type<T>::id())) {
                data = new JObjectArrayPointer<T>(env, jobjectArray(in), [scope](T& d,JNIEnv * env, jobject obj){
                    d = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, obj, nullptr, scope);
                });
                scope->addFinalAction([data](){delete data;});
            }
            std::initializer_list<T> out;
            if (data) {
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint32>{
    typedef std::initializer_list<qint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jintArray(env, reinterpret_cast<const jint*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint32>{
    typedef std::initializer_list<qint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint16>{
    typedef std::initializer_list<qint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jshortArray(env, reinterpret_cast<const jshort*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint16>{
    typedef std::initializer_list<qint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint8>{
    typedef std::initializer_list<qint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jbyteArray(env, reinterpret_cast<const jbyte*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint8>{
    typedef std::initializer_list<qint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint64>{
    typedef std::initializer_list<qint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jlongArray(env, reinterpret_cast<const jlong*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint64>{
    typedef std::initializer_list<qint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint32>{
    typedef std::initializer_list<quint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jintArray(env, reinterpret_cast<const jint*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint32>{
    typedef std::initializer_list<quint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint16>{
    typedef std::initializer_list<quint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jshortArray(env, reinterpret_cast<const jshort*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint16>{
    typedef std::initializer_list<quint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint8>{
    typedef std::initializer_list<quint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jbyteArray(env, reinterpret_cast<const jbyte*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint8>{
    typedef std::initializer_list<quint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint64>{
    typedef std::initializer_list<quint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jlongArray(env, reinterpret_cast<const jlong*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint64>{
    typedef std::initializer_list<quint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 float>{
    typedef std::initializer_list<float> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jfloatArray(env, reinterpret_cast<const jfloat*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 float>{
    typedef std::initializer_list<float> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 double>{
    typedef std::initializer_list<double> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jdoubleArray(env, reinterpret_cast<const jdouble*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 double>{
    typedef std::initializer_list<double> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 bool>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jbooleanArray(env, reinterpret_cast<const jboolean*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 bool>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 wchar_t>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jcharArray(env, reinterpret_cast<const jchar*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 wchar_t>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 QChar>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_to_jcharArray(env, reinterpret_cast<const jchar*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 QChar>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
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
#ifdef Q_CC_MSVC
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
#endif //defined(_INITIALIZER_LIST_)

#if defined(_FUNCTIONAL_) || defined(_FUNCTIONAL) || defined(_LIBCPP_FUNCTIONAL) || defined(_GLIBCXX_FUNCTIONAL)
//template from any std::function to jobject

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<true, has_scope,
                                 jobject,
                                 std::function, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::function<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char* nativeTypeName, QtJambiScope*){
        jobject result = nullptr;
        if(!qtjambi_convert_from_native(env, typeid(NativeType), nativeTypeName, &deref_ptr<is_pointer,NativeType_c>::deref(in), true, false, result)){
           JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot cast native type %1 to Java object.").arg(QLatin1String(qtjambi_type_name(typeid(NativeType))))) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_container1_cast<false, has_scope,
                                 jobject,
                                 std::function, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::function<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::function<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::function<T> *");
        NativeType fct;
        NativeType* result = &fct;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QString("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType))))) QTJAMBI_STACKTRACEINFO );
        }
        return result ? *result : NativeType();
    }
};
#endif // defined(_FUNCTIONAL_)

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER1_P_H
