/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_IMPL_TEMPLATE1_H
#define QTJAMBI_CAST_IMPL_TEMPLATE1_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_smartpointer.h"
#ifdef QDBUSREPLY_H
#include "qtjambi_cast_impl_dbus.h"
#endif

QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(QLINKEDLIST_H)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

//template from any type's QWeakPointer<T> to jobject
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
struct qtjambi_jnitype_template1_cast<forward, has_scope,
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
         typename E>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFlags, is_pointer, is_const, is_reference,
                                 E>{
    typedef QFlags<E> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::convertQFlagsToJavaObject<E>(env, deref_ptr<is_pointer,NativeType_c>::deref(in));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename E>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFlags, is_pointer, is_const, is_reference,
                                 E>{
    typedef QFlags<E> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QFlags<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QFlags<T> *");
        return QFlags<E>(QtJambiAPI::enumValue<int>(env, in));
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QList, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QList<T>,QList<T>>::type Template;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Template>::type, typename std::add_lvalue_reference<Template>::type>::type Template_in;
    static jobject cast(JNIEnv *env, Template_in in, const char*, QtJambiScope* scope){
        return QtJambiAPI::convertQListToJavaObject(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Template>::ref(in),
                                  CloneContainer<QList,T, is_pointer>::function,
                                  DeleteContainer<QList,T>::function,
                                  QtJambiAPI::ListType::QList,
                                  QListAccess<T,is_pointer && is_const>::newInstance()
                                );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QList, is_pointer, is_const, is_reference,
                                 T>{
    typedef QList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQList<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQList<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, true,
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
            if (!ContainerAPI::getAsQList<T>(env, in, list)) {
                if(is_const){
                    list = new NativeType();
                    scope->addDeletion(list);
                }else{
                    list = new IntermediateSequentialContainer<QList,T>(env, in, *scope);
                    scope->addDeletion(static_cast<IntermediateSequentialContainer<QList,T>*>(list));
                }
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                }
            }
        }else{
            list = new NativeType();
            scope->addDeletion(list);
        }
        return *list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QSet, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QSet<T>,QSet<T>>::type Template;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Template>::type, typename std::add_lvalue_reference<Template>::type>::type Template_in;
    static jobject cast(JNIEnv *env, Template_in in, const char*, QtJambiScope* scope){
        return QtJambiAPI::convertQSetToJavaObject(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Template>::ref(in),
                                  CloneContainer<QSet,T,is_pointer>::function,
                                  DeleteContainer<QSet,T>::function,
                                  QSetAccess<T,is_pointer && is_const>::newInstance()
                                );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QSet, is_pointer, is_const, is_reference,
                                 T>{
    typedef QSet<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQSet<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->insert(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQSet<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->insert(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, true,
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
             if (!ContainerAPI::getAsQSet<T>(env, in, list)) {
                 if(is_const){
                     list = new NativeType();
                     scope->addDeletion(list);
                 }else{
                     list = new IntermediateSequentialContainer<QSet,T>(env, in, *scope);
                     scope->addDeletion(static_cast<IntermediateSequentialContainer<QSet,T>*>(list));
                 }
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->insert(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addDeletion(list);
         }
         return *list;
     }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QLINKEDLIST_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QLinkedList, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QLinkedList<T>,QLinkedList<T>>::type Template;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Template>::type, typename std::add_lvalue_reference<Template>::type>::type Template_in;
    static jobject cast(JNIEnv *env, Template_in in, const char*, QtJambiScope* scope){
        return QtJambiAPI::convertQLinkedListToJavaObject(env,
                                        scope ? scope->relatedNativeID() : InvalidNativeID,
                                        ref_ptr<is_pointer, Template>::ref(in),
                                        CloneContainer<QLinkedList,T,is_pointer>::function,
                                        DeleteContainer<QLinkedList,T>::function,
                                        QLinkedListAccess<T,is_pointer && is_const>::newInstance()
                                 );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QLinkedList, is_pointer, is_const, is_reference,
                                 T>{
    typedef QLinkedList<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQLinkedList<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQLinkedList<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, true,
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
             if (!ContainerAPI::getAsQLinkedList<T>(env, in, list)) {
                 if(is_const){
                     list = new NativeType();
                     scope->addDeletion(list);
                 }else{
                     list = new IntermediateSequentialContainer<QLinkedList,T>(env, in, *scope);
                     scope->addDeletion(static_cast<IntermediateSequentialContainer<QLinkedList,T>*>(list));
                 }
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addDeletion(list);
         }
         return *list;
     }
};
#endif

#ifdef QVECTOR_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QVector, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QVector<T>,QVector<T>>::type Template;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Template>::type, typename std::add_lvalue_reference<Template>::type>::type Template_in;
    static jobject cast(JNIEnv *env, Template_in in, const char*, QtJambiScope* scope){
        return QtJambiAPI::convertQVectorToJavaObject(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<is_pointer, Template>::ref(in),
                                     CloneContainer<QVector,T,is_pointer>::function,
                                     DeleteContainer<QVector,T>::function,
                                     QtJambiAPI::VectorType::QVector,
                                     QVectorAccess<T,is_pointer && is_const>::newInstance()
                                 );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QVector, is_pointer, is_const, is_reference,
                                 T>{
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQVector<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
    Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQVector<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, true,
                                 jobject,
                                 QVector, false, is_const, true,
                                 T>
{
    typedef QVector<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         if(scope==nullptr)
             JavaException::raiseError(env, "Cannot cast to const QVector<T>& without scope." QTJAMBI_STACKTRACEINFO );
         NativeType *list = nullptr;
         if (in) {
             if (!ContainerAPI::getAsQVector<T>(env, in, list)) {
                 if(is_const){
                     list = new NativeType();
                     scope->addDeletion(list);
                 }else{
                     list = new IntermediateSequentialContainer<QVector,T>(env, in, *scope);
                     scope->addDeletion(static_cast<IntermediateSequentialContainer<QVector,T>*>(list));
                 }
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addDeletion(list);
         }
         return *list;
     }
};
#endif
#endif

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QStack, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QStack<T>,QStack<T>>::type Template;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Template>::type, typename std::add_lvalue_reference<Template>::type>::type Template_in;
    static jobject cast(JNIEnv *env, Template_in in, const char*, QtJambiScope* scope){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        return QtJambiAPI::convertQVectorToJavaObject(env,
                                    scope ? scope->relatedNativeID() : InvalidNativeID,
                                    ref_ptr<is_pointer, Template>::ref(in),
                                    CloneContainer<QStack,T,is_pointer>::function,
                                    DeleteContainer<QStack,T>::function,
                                    QtJambiAPI::VectorType::QStack,
                                    QVectorAccess<T,is_pointer && is_const>::newInstance()
                                 );
#else
        return QtJambiAPI::convertQListToJavaObject(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Template>::ref(in),
                                  CloneContainer<QStack,T,is_pointer>::function,
                                  DeleteContainer<QStack,T>::function,
                                  QtJambiAPI::ListType::QStack,
                                  QListAccess<T,false>::newInstance()
                                 );
#endif
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QStack, is_pointer, is_const, is_reference,
                                 T>{
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQStack<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
    Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQStack<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, true,
                                 jobject,
                                 QStack, false, is_const, true,
                                 T>
{
    typedef QStack<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(scope==nullptr)
            JavaException::raiseError(env, "Cannot cast to const QStack<T>& without scope." QTJAMBI_STACKTRACEINFO );
        QStack<T> *list = nullptr;
        if (in) {
            if (!ContainerAPI::getAsQStack<T>(env, in, list)) {
                if(is_const){
                    list = new QStack<T>();
                    scope->addDeletion(list);
                }else{
                    list = new IntermediateSequentialContainer<QStack,T>(env, in, *scope);
                    scope->addDeletion(static_cast<IntermediateSequentialContainer<QStack,T>*>(list));
                }
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    T  __qt_element = qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope);
                    (*list) << __qt_element;
                }
            }
        }else{
            list = new QStack<T>();
            scope->addDeletion(list);
        }
        return *list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QQueue, is_pointer, is_const, is_reference,
                                 T>{
    typedef typename std::conditional<is_const, const QQueue<T>,QQueue<T>>::type Template;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Template>::type, typename std::add_lvalue_reference<Template>::type>::type Template_in;
    static jobject cast(JNIEnv *env, Template_in in, const char*, QtJambiScope* scope){
        return QtJambiAPI::convertQListToJavaObject(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Template>::ref(in),
                                  CloneContainer<QQueue,T,is_pointer>::function,
                                  DeleteContainer<QQueue,T>::function,
                                  QtJambiAPI::ListType::QQueue,
                                  QListAccess<T,is_pointer && is_const>::newInstance()
                                );
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QQueue, is_pointer, is_const, is_reference,
                                 T>{
    typedef QQueue<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
     static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQQueue<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool has_scope,
         bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
         std::unique_ptr<NativeType> list;
         if(in){
             NativeType* pointer{nullptr};
             if (ContainerAPI::getAsQQueue<T>(env, in, pointer)) {
                 return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, pointer);
             }else{
                 list.reset(new NativeType());
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }
         return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
     }
};

template<bool is_const,
         typename T>
struct qtjambi_jnitype_template1_cast<false, true,
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
             if (!ContainerAPI::getAsQQueue<T>(env, in, list)) {
                 if(is_const){
                     list = new NativeType();
                     scope->addDeletion(list);
                 }else{
                     list = new IntermediateSequentialContainer<QQueue,T>(env, in, *scope);
                     scope->addDeletion(static_cast<IntermediateSequentialContainer<QQueue,T>*>(list));
                 }
                 jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
                 while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                     jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                     list->append(qtjambi_scoped_cast<true,T,jobject>::cast(env, element, nullptr, scope));
                 }
             }
         }else{
             list = new NativeType();
             scope->addDeletion(list);
         }
         return *list;
     }
};

#ifdef QDBUSREPLY_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, QDBusReplyUtility<T>::createFrom(_in));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 T>{
    typedef QDBusReply<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QDBusReply<QVariant>* reply = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(env, in);
        if(reply){
            std::unique_ptr<NativeType> reply2(QDBusReplyUtility<T>::reverseCreateFrom(*reply));
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, reply2);
        }else{
            return create_container_pointer<is_pointer, is_const, is_reference, false, NativeType>::create(env, scope, nullptr);
        }
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, QDBusReplyUtility<QVariant>::createFrom(_in));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QDBusReply, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QDBusReply<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QDBusReply<QVariant>* reply = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(env, in);
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, reply);
    }
};
#endif

#ifdef QQMLLIST_H
template<class T, bool = std::is_base_of<QObject,T>::value>
struct QmlListPropertyUtility{
    static jobject toJavaObject(JNIEnv *env, const QQmlListProperty<T>& d)
    {
        return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &d, typeid(QQmlListProperty<void>));
    }
};

template<class T>
struct QmlListPropertyUtility<T,false>{
    static jobject toJavaObject(JNIEnv *env, const QQmlListProperty<T>& d)
    {
        jobject result = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &d, typeid(QQmlListProperty<void>));
        jobject mt = qtjambi_cast<jobject>(env, QMetaType::fromType<T*>());
        QtJambiAPI::setQQmlListPropertyElementType(env, result, mt);
        return result;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QmlListPropertyUtility<T>::toJavaObject(env, _in);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QQmlListProperty, is_pointer, is_const, is_reference,
                                 T>{
    typedef QQmlListProperty<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(
                    env, scope, QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in));
    }
};
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifdef QPROPERTY_H
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QBindable, is_pointer, is_const, is_reference,
                                 T>{
    typedef QBindable<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
    }
};


template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QPropertyAlias, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyAlias<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QPropertyBinding, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyBinding<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QPropertyChangeHandler, is_pointer, is_const, is_reference,
                                 T>{
    typedef QPropertyChangeHandler<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
    }
};
#endif
#endif

#ifdef QFUTUREINTERFACE_H

template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFutureInterface, true, is_const, is_reference,
                                 void>{
    typedef QFutureInterface<void> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in);
    }
};

template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFutureInterface, false, is_const, is_reference,
                                 void>{
    typedef QFutureInterface<void> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFutureInterface, is_pointer, is_const, is_reference,
                                 void>{
    typedef QFutureInterface<void> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(
                    env, scope, QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in));
    }
};

template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFutureInterface, true, is_const, is_reference,
                                 QVariant>{
    typedef QFutureInterface<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in);
    }
};

template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFutureInterface, false, is_const, is_reference,
                                 QVariant>{
    typedef QFutureInterface<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFutureInterface, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QFutureInterface<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(
                    env, scope, QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in));
    }
};

template<typename T>
QFutureInterface<QVariant> convert_T_future_interface(const QFutureInterface<T>& future, const char* translatedType = "QFutureInterface"){
    QFutureInterface<QVariant> jpromise;
    QFutureInterfaceBase* availableResult = QtJambiAPI::translateQFutureInterface(
                                 QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<T>(future)),
                                 QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<QVariant>(jpromise)),
                                 translatedType,
                                 [](JNIEnv *, const QSharedPointer<QFutureInterfaceBase>& sourceFuture, const QSharedPointer<QFutureInterfaceBase>& targetFuture, int beginIndex, int count){
                                    if(count==1){
                                        dynamic_cast<QFutureInterface<QVariant>*>(targetFuture.get())->reportResult(QVariant::fromValue<T>(sourceFuture->resultStoreBase().resultAt(beginIndex).template value<T>()), beginIndex);
                                    }else{
                                        QVector<QVariant> results(count);
                                        for(int i=0; i<count; ++i){
                                            results << QVariant::fromValue<T>(sourceFuture->resultStoreBase().resultAt(beginIndex).template value<T>());
                                        }
                                        dynamic_cast<QFutureInterface<QVariant>*>(targetFuture.get())->reportResults(results, count);
                                    }
                                },
                                [](JNIEnv *, const QSharedPointer<QFutureInterfaceBase>& sourceFuture, const QSharedPointer<QFutureInterfaceBase>&targetFuture, int beginIndex, int count){
                                    if(count==1){
                                        dynamic_cast<QFutureInterface<T>*>(sourceFuture.get())->reportResult(targetFuture->resultStoreBase().resultAt(beginIndex).template value<QVariant>().template value<T>(), beginIndex);
                                    }else{
                                        QVector<T> results(count);
                                        for(int i=0; i<count; ++i){
                                            results << targetFuture->resultStoreBase().resultAt(i).template value<QVariant>().template value<T>();
                                        }
                                        dynamic_cast<QFutureInterface<T>*>(sourceFuture.get())->reportResults(results, count);
                                    }
                                });
    if(QFutureInterface<QVariant>* newResult = dynamic_cast<QFutureInterface<QVariant>*>(availableResult)){
        return *newResult;
    }
    return jpromise;
}

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         QFutureInterface<QVariant>* jpromise = new QFutureInterface<QVariant>(convert_T_future_interface<T>(_in));
         return QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, jpromise);
    }
};

template<typename T, bool isConstructible = std::is_copy_constructible<T>::value && std::is_destructible<T>::value, bool isMovable = std::is_move_constructible<T>::value && std::is_destructible<T>::value>
struct FutureVariantHandler{
    Q_STATIC_ASSERT_X(isConstructible || isMovable, "Cannot cast QFuture with non-constructible and non-movable result type.");
    static void reportNativeResults(JNIEnv *, const QSharedPointer<QFutureInterfaceBase>& sourceFuture, const QSharedPointer<QFutureInterfaceBase>& targetFuture, int beginIndex, int count){
       if(count==1){
           dynamic_cast<QFutureInterface<QVariant>*>(sourceFuture.get())->reportResult(QVariant::fromValue<T>(targetFuture->resultStoreBase().resultAt(beginIndex).template value<T>()), beginIndex);
       }else{
           QVector<QVariant> results(count);
           for(int i=0; i<count; ++i){
               results << QVariant::fromValue<T>(targetFuture->resultStoreBase().resultAt(beginIndex + i).template value<T>());
           }
           dynamic_cast<QFutureInterface<QVariant>*>(sourceFuture.get())->reportResults(results, beginIndex, count);
       }
    }

    static void reportJavaResults(JNIEnv *, const QSharedPointer<QFutureInterfaceBase>& sourceFuture, const QSharedPointer<QFutureInterfaceBase>&targetFuture, int beginIndex, int count){
        if(count==1){
            dynamic_cast<QFutureInterface<T>*>(targetFuture.get())->reportResult(sourceFuture->resultStoreBase().resultAt(beginIndex).template value<QVariant>().template value<T>(), beginIndex);
        }else{
            QVector<T> results(count);
            for(int i=0; i<count; ++i){
                results << sourceFuture->resultStoreBase().resultAt(beginIndex + i).template value<QVariant>().template value<T>();
            }
            dynamic_cast<QFutureInterface<T>*>(targetFuture.get())->reportResults(results, beginIndex, count);
        }
    }
};

template<typename T>
struct FutureVariantHandler<T,false,true>{
    static void reportNativeResults(JNIEnv * env, const QSharedPointer<QFutureInterfaceBase>& sourceFuture, const QSharedPointer<QFutureInterfaceBase>& targetFuture, int beginIndex, int count){
        for(int i=0; i<count; ++i){
            jobject obj = qtjambi_cast<jobject>(env, targetFuture->resultStoreBase().resultAt(beginIndex + i).template value<T>());
            dynamic_cast<QFutureInterface<QVariant>*>(sourceFuture.get())->reportResult(QtJambiAPI::convertJavaObjectToQVariant(env, obj), beginIndex + i);
        }
    }

    static void reportJavaResults(JNIEnv *env, const QSharedPointer<QFutureInterfaceBase>& sourceFuture, const QSharedPointer<QFutureInterfaceBase>&targetFuture, int beginIndex, int count){
        for(int i=0; i<count; ++i){
            QVariant v = sourceFuture->resultStoreBase().resultAt(beginIndex + i).template value<QVariant>();
            jobject o = QtJambiAPI::convertQVariantToJavaObject(env, v);
            T* n = qtjambi_cast<T*>(env, o);
            if(n)
                dynamic_cast<QFutureInterface<T>*>(targetFuture.get())->reportResult(std::move(*n), beginIndex + i);
        }
    }
};

template<typename T>
QFutureInterface<T> convert_future_interface(QFutureInterface<QVariant>* future, const char* translatedType = "QFutureInterface"){
    QFutureInterface<T> result;
    if(future){
        QFutureInterfaceBase* availableResult = QtJambiAPI::translateQFutureInterface(
                    QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<QVariant>(*future)),
                    QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<T>(result)),
                    translatedType,
                    FutureVariantHandler<T>::reportJavaResults,
                    FutureVariantHandler<T>::reportNativeResults);
        if(QFutureInterface<T>* newResult = dynamic_cast<QFutureInterface<T>*>(availableResult)){
            return *newResult;
        }
    }
    return result;
}

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFutureInterface, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureInterface<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference || has_scope, "Cannot cast to QFutureInterface & without scope.");
    Q_STATIC_ASSERT_X(!is_pointer || has_scope, "Cannot cast to QFutureInterface* without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QFutureInterface<T> tfuture = convert_future_interface<T>(QtJambiAPI::convertJavaObjectToNative<QFutureInterface<QVariant>>(env, in));
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, &tfuture);
    }
};
#endif // defined(QFUTUREINTERFACE_H)

#if defined(QFUTUREWATCHER_H)

template<typename T>
struct FutureResultFactory{
    static jobject futureResult(JNIEnv * env, QFutureWatcherBase* base, int index) {
        QFutureWatcher<T>* watcher = dynamic_cast<QFutureWatcher<T>*>(base);
        QVariant value = QVariant::fromValue<T>(watcher->resultAt(index));
        return QtJambiAPI::convertQVariantToJavaObject(env, value);
    };
};

template<>
struct FutureResultFactory<void>{
    static constexpr QtJambiAPI::FutureResult futureResult = nullptr;
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFutureWatcher, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureWatcher<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         if(NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in)){
            return QtJambiAPI::convertQFutureWatcherToJavaObject(env, _in,
                                               [](JNIEnv * env, QFutureWatcherBase* base, jobject future){
                                                    QFutureWatcher<T>* watcher = dynamic_cast<QFutureWatcher<T>*>(base);
                                                    QFuture<T> ft = qtjambi_jnitype_template1_cast<false,false,jobject,QFuture,false,false,false,T>::cast(env, future, nullptr, nullptr);
                                                    watcher->setFuture(ft);
                                                },
                                               FutureResultFactory<T>::futureResult,
                                               [](JNIEnv * env, QFutureWatcherBase* base) -> jobject {
                                                    QFutureWatcher<T>* watcher = dynamic_cast<QFutureWatcher<T>*>(base);
                                                    QFuture<T> ft = watcher->future();
                                                    return qtjambi_jnitype_template1_cast<true,false,jobject,QFuture,false,false,false,T>::cast(env, ft, nullptr, nullptr);
                                                }
                                            );
         }else return nullptr;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFutureWatcher, is_pointer, is_const, is_reference,
                                 void>{
    typedef QFutureWatcher<void> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QFutureWatcherBase* watcher = qtjambi_scoped_cast<has_scope,QFutureWatcherBase*,jobject>::cast(env, in, nullptr, scope);
        return ptr2ref<is_reference,NativeType>::value(env, reinterpret_cast<NativeType*>(watcher));
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFutureWatcher, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFutureWatcher<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QFutureWatcherBase* watcher = qtjambi_scoped_cast<has_scope,QFutureWatcherBase*,jobject>::cast(env, in, nullptr, scope);
        QFutureWatcher<T>* result = dynamic_cast<QFutureWatcher<T>*>(watcher);
        if(!result){
            if(jobject jfutureInterface = QtJambiAPI::getQFutureInterfaceFromQFuture(env, in)){
                if(QFutureInterfaceBase* base = qtjambi_scoped_cast<has_scope,QFutureInterfaceBase*,jobject>::cast(env, jfutureInterface, nullptr, scope)){
                    if(QFutureInterface<T>* futureInterface = dynamic_cast<QFutureInterface<T>*>(base)){
                        NativeType* _watcher = new NativeType(watcher);
                        _watcher->setFuture(QFuture<T>(futureInterface));
                        return _watcher;
                    }else if(QFutureInterface<QVariant>* futureInterface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
                        QFutureInterface<T> _futureInterface = convert_future_interface<T>(futureInterface, "QFutureWatcher");
                        NativeType* _watcher = new NativeType(watcher);
                        _watcher->setFuture(_futureInterface.future());
                        return _watcher;
                    }else if(QFutureInterface<void>* futureInterface = dynamic_cast<QFutureInterface<void>*>(base)){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QString baseType = QLatin1String(QtJambiAPI::typeName(typeid(*base)));
#if defined(Q_OS_ANDROID)
                        if(baseType==QLatin1String("QFutureInterface<QVariant>")
                                || baseType==QLatin1String("QFutureInterface_shell<QVariant>")){
                            QFutureInterface<QVariant>* futureInterface = reinterpret_cast<QFutureInterface<QVariant>*>(base);
                            QFutureInterface<T> _futureInterface = convert_future_interface<T>(futureInterface, "QFutureWatcher");
                            NativeType* _watcher = new NativeType(watcher);
                            _watcher->setFuture(_futureInterface.future());
                            return _watcher;
                        }else if(baseType!=QString("QFutureInterface<void>")
                                 && baseType!=QString("QFutureInterface_shell<void>")
                                 && baseType==QString("QFutureInterface<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                            QFutureInterface<T>* futureInterface = reinterpret_cast<QFutureInterface<T>*>(base);
                            NativeType* _watcher = new NativeType(watcher);
                            _watcher->setFuture(QFuture<T>(futureInterface));
                            return _watcher;
                        }else
#endif
                        if(baseType==QLatin1String("QFutureInterfaceBase")
                                || baseType==QLatin1String("QFutureInterfaceBase_shell")){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher%1 to %2.").arg(baseType.mid(16)).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }else if(baseType.startsWith(QLatin1String("QFutureInterface_shell<"))){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher%1 to %2.").arg(baseType.mid(22)).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }
            }
            if(watcher){
                QString watcherType = QLatin1String(QtJambiAPI::typeName(typeid(*watcher)));
                if(watcherType==QLatin1String("QFutureWatcher_shell")){
                    watcherType = QLatin1String("QFutureWatcher<?>");
                }
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to %2.").arg(watcherType).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
            }else if(is_reference)
                JavaException::raiseNullPointerException(env, "Cannot cast null to QFutureWatcher&." QTJAMBI_STACKTRACEINFO );
        }
        return ptr2ref<is_reference,NativeType>::value(env, result);
    }
};
#endif //defined(QFUTUREWATCHER_H)


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && defined(QFUTURE_H)
}

namespace QtPrivate{
    template<class T>
    class Continuation<T,bool,bool>{
    public:
        typedef decltype(std::declval<QFuture<T>>().d) FutureInterface;
        static FutureInterface &sourceFuture(const QFuture<T>& future){return future.d;}
    };
}

namespace QtJambiPrivate {
#endif

#ifdef QPROMISE_H

template<bool has_scope, typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QPromise, false, false, true,
                                 T>{
    typedef QPromise<T> NativeType;
    static jobject cast(JNIEnv *env, QPromise<T>& in, const char*, QtJambiScope* scope){
         QFuture<T> future = in.future();
         auto futureInterface = QtPrivate::Continuation<T,bool,bool>::sourceFuture(future);
         jobject jfutureInterface = qtjambi_scoped_cast<has_scope,jobject,decltype(futureInterface)>::cast(env, futureInterface, nullptr, scope);
         return QtJambiAPI::createQPromise(env, jfutureInterface, &in, scope);
    }
};

template<bool has_scope, typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QPromise, false, false, true,
                                 T>{
    static QPromise<T>& cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            JavaException::raiseNullPointerException(env, "Cannot cast null to QPromise&." QTJAMBI_STACKTRACEINFO );
        void* promisePtr = QtJambiAPI::getNativeQPromise(env, in);
        if(promisePtr){
            QFutureInterfaceBase* base = reinterpret_cast<QFutureInterfaceBase*>(promisePtr);
            if(QFutureInterface<T>* futureInterface = dynamic_cast<QFutureInterface<T>*>(base)){
                return *reinterpret_cast<QPromise<T>*>(promisePtr);
            }else if constexpr (std::is_same<void, T>::value){
                QFutureInterface<T> _futureInterface(*base);
                if(scope==nullptr)
                    JavaException::raiseError(env, "Cannot cast QPromise without scope." QTJAMBI_STACKTRACEINFO );
                QPromise<T>* promise = new QPromise<T>(_futureInterface);
                QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                scope->addDeletion(promiseContent);
                return *promise;
            }else if(QFutureInterface<QVariant>* futureInterface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
                if(scope==nullptr)
                    JavaException::raiseError(env, "Cannot cast QPromise without scope." QTJAMBI_STACKTRACEINFO );
                QFutureInterface<T> _futureInterface = convert_future_interface<T>(futureInterface, "QPromise");
                QPromise<T>* promise = new QPromise<T>(_futureInterface);
                QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                scope->addDeletion(promiseContent);
                return *promise;
            }else if(base){
                QString baseType = QLatin1String(QtJambiAPI::typeName(typeid(*base)));
#if defined(Q_OS_ANDROID)
                if(baseType==QLatin1String("QFutureInterface<QVariant>")
                        || baseType==QLatin1String("QFutureInterface_shell<QVariant>")){
                    QFutureInterface<QVariant>* futureInterface = reinterpret_cast<QFutureInterface<QVariant>*>(base);
                    if(scope==nullptr)
                        JavaException::raiseError(env, "Cannot cast QPromise without scope." QTJAMBI_STACKTRACEINFO );
                    QFutureInterface<T> _futureInterface = convert_future_interface<T>(futureInterface, "QPromise");
                    QPromise<T>* promise = new QPromise<T>(_futureInterface);
                    QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                    scope->addDeletion(promiseContent);
                    return *promise;
                }else if(baseType==QString("QFutureInterface<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                    return *reinterpret_cast<QPromise<T>*>(promisePtr);
                }else
#endif
                if(baseType==QLatin1String("QFutureInterfaceBase")
                        || baseType==QLatin1String("QFutureInterfaceBase_shell")){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(QPromise<T>)))) QTJAMBI_STACKTRACEINFO );
                }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise%1 to %2.").arg(baseType.mid(16)).arg(QLatin1String(QtJambiAPI::typeName(typeid(QPromise<T>)))) QTJAMBI_STACKTRACEINFO );
                }else if(baseType.startsWith(QLatin1String("QFutureInterface_shell<"))){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise%1 to %2.").arg(baseType.mid(22)).arg(QLatin1String(QtJambiAPI::typeName(typeid(QPromise<T>)))) QTJAMBI_STACKTRACEINFO );
                }
            }
        }else{
            if(jobject jfutureInterface = QtJambiAPI::getQFutureInterfaceFromQFuture(env, in)){
                if(QFutureInterfaceBase* base = qtjambi_scoped_cast<has_scope,QFutureInterfaceBase*,jobject>::cast(env, jfutureInterface, nullptr, scope)){
                    if(QFutureInterface<T>* futureInterface = dynamic_cast<QFutureInterface<T>*>(base)){
                        if(scope==nullptr)
                            JavaException::raiseError(env, "Cannot cast QPromise without scope." QTJAMBI_STACKTRACEINFO );
                        QPromise<T>* promise = new QPromise<T>(*futureInterface);
                        QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                        scope->addDeletion(promiseContent);
                        return *promise;
                    }else if constexpr (std::is_same<void, T>::value){
                        QFutureInterface<T> _futureInterface(*base);
                        if(scope==nullptr)
                            JavaException::raiseError(env, "Cannot cast QPromise without scope." QTJAMBI_STACKTRACEINFO );
                        QPromise<T>* promise = new QPromise<T>(_futureInterface);
                        QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                        scope->addDeletion(promiseContent);
                        return *promise;
                    }else if(QFutureInterface<QVariant>* futureInterface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
                        if(scope==nullptr)
                            JavaException::raiseError(env, "Cannot cast QPromise without scope." QTJAMBI_STACKTRACEINFO );
                        QFutureInterface<T> _futureInterface = convert_future_interface<T>(futureInterface, "QPromise");
                        QPromise<T>* promise = new QPromise<T>(_futureInterface);
                        QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                        scope->addDeletion(promiseContent);
                        return *promise;
                    }else{
                        QString baseType = QLatin1String(QtJambiAPI::typeName(typeid(*base)));
#if defined(Q_OS_ANDROID)
                        if(baseType==QLatin1String("QFutureInterface<QVariant>")
                                || baseType==QLatin1String("QFutureInterface_shell<QVariant>")){
                            QFutureInterface<QVariant>* futureInterface = reinterpret_cast<QFutureInterface<QVariant>*>(base);
                            QFutureInterface<T> _futureInterface = convert_future_interface<T>(futureInterface, "QPromise");
                            QPromise<T>* promise = new QPromise<T>(_futureInterface);
                            QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                            scope->addDeletion(promiseContent);
                            return *promise;
                        }else if(baseType==QString("QFutureInterface<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                            QFutureInterface<T>* futureInterface = reinterpret_cast<QFutureInterface<T>*>(base);
                            if(scope==nullptr)
                                JavaException::raiseError(env, "Cannot cast QPromise without scope." QTJAMBI_STACKTRACEINFO );
                            QPromise<T>* promise = new QPromise<T>(*futureInterface);
                            QFutureInterface<T>* promiseContent = reinterpret_cast<QFutureInterface<T>*>(promise);
                            scope->addDeletion(promiseContent);
                            return *promise;
                        }else
#endif
                        if(baseType==QLatin1String("QFutureInterfaceBase")
                                || baseType==QLatin1String("QFutureInterfaceBase_shell")){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(QPromise<T>)))) QTJAMBI_STACKTRACEINFO );
                        }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise%1 to %2.").arg(baseType.mid(16)).arg(QLatin1String(QtJambiAPI::typeName(typeid(QPromise<T>)))) QTJAMBI_STACKTRACEINFO );
                        }else if(baseType.startsWith(QLatin1String("QFutureInterface_shell<"))){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise%1 to %2.").arg(baseType.mid(22)).arg(QLatin1String(QtJambiAPI::typeName(typeid(QPromise<T>)))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }
            }else{
                JavaException::raiseNullPointerException(env, "Cannot cast null to QPromise&." QTJAMBI_STACKTRACEINFO );
            }
        }
        JavaException::raiseIllegalArgumentException(env, "Cannot cast QPromise<A> to QPromise<B>." QTJAMBI_STACKTRACEINFO );
        throw "Cannot cast QPromise<A> to QPromise<B>.";
    }
};

#endif //def QPROMISE_H

#ifdef QFUTURE_H

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 void>{
    typedef QFuture<void> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
         auto fi = QtJambiAPI::getQFutureInterfaceFromQFuture(_in);
#else
         auto fi = QtPrivate::Continuation<void,bool,bool>::sourceFuture(_in);
#endif
         jobject futureInterface = qtjambi_scoped_cast<has_scope,jobject,decltype(fi)>::cast(
                                            env, fi, nullptr, scope);
        return QtJambiAPI::createQFutureFromQFutureInterface(env, futureInterface);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 void>{
    typedef QFuture<void> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        jobject futureInterface = QtJambiAPI::getQFutureInterfaceFromQFuture(env, in);
        QFutureInterfaceBase* base = qtjambi_scoped_cast<has_scope,QFutureInterfaceBase*,jobject>::cast(env, futureInterface, nullptr, scope);
        if(base){
            QFuture<void> future(base);
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, &future);
        }else{
            QFuture<void> future;
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, &future);
        }
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QFuture<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         jobject futureInterface = qtjambi_jnitype_template1_cast<true,has_scope,jobject,QFutureInterface,false,false,false,QVariant>::cast(env,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                            _in.d
#else
                                            QtPrivate::Continuation<QVariant,bool,bool>::sourceFuture(_in)
#endif
                , nullptr, scope);
        return QtJambiAPI::createQFutureFromQFutureInterface(env, futureInterface);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QFuture<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        jobject futureInterface = QtJambiAPI::getQFutureInterfaceFromQFuture(env, in);
        QFutureInterface<QVariant>* _futureInterface = qtjambi_scoped_cast<has_scope,QFutureInterface<QVariant>*,jobject>::cast(env, futureInterface, nullptr, scope);
        if(_futureInterface){
            QFuture<QVariant> future(_futureInterface);
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, &future);
        }else{
            QFuture<QVariant> future;
            return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, &future);
        }
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFuture<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         const QFuture<T>& future = reinterpret_cast<const QFuture<T>&>(_in);
         QFutureInterface<QVariant> jpromise = convert_T_future_interface<T>(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                            future.d,
#else
                                            QtPrivate::Continuation<T,bool,bool>::sourceFuture(future),
#endif
                                            "QFuture"
                                        );
         jobject futureInterface = qtjambi_jnitype_template1_cast<true,has_scope,jobject,QFutureInterface,false,false,false,QVariant>::cast(env, jpromise, nullptr, scope);
         return QtJambiAPI::createQFutureFromQFutureInterface(env, futureInterface);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QFuture, is_pointer, is_const, is_reference,
                                 T>{
    typedef QFuture<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference || has_scope, "Cannot cast to QFuture & without scope.");
    Q_STATIC_ASSERT_X(!is_pointer || has_scope, "Cannot cast to QFuture* without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        jobject futureInterface = QtJambiAPI::getQFutureInterfaceFromQFuture(env, in);
        QFutureInterfaceBase* base = qtjambi_scoped_cast<has_scope,QFutureInterfaceBase*,jobject>::cast(env, futureInterface, nullptr, scope);
        QFutureInterface<QVariant>* future = dynamic_cast<QFutureInterface<QVariant>*>(base);
#if defined(Q_OS_ANDROID)
        if(!future){
            QString baseType = QLatin1String(QtJambiAPI::typeName(typeid(*base)));
            if(baseType==QLatin1String("QFutureInterface<QVariant>")
                    || baseType==QLatin1String("QFutureInterface_shell<QVariant>")){
                future = reinterpret_cast<QFutureInterface<QVariant>*>(base);
            }
        }
#endif
        QFutureInterface<T> tpromise = convert_future_interface<T>(future, "QFuture");
        QFuture<T> ft = tpromise.future();
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, &ft);
    }
};
#endif // QFUTURE_H

#ifdef QCBORSTREAMREADER_H

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QCborStreamReader::StringResult, is_pointer, is_const, is_reference,
                                 T>{
    typedef QCborStreamReader::StringResult<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
         NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
         QCborStreamReader::StringResult<QVariant>* ___qt_return_value = new QCborStreamReader::StringResult<QVariant>;
         ___qt_return_value->data = _in.data;
         ___qt_return_value->status = _in.status;
         return QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ___qt_return_value);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 QCborStreamReader::StringResult, is_pointer, is_const, is_reference,
                                 QVariant> : qtjambi_move_or_copy_decider<QCborStreamReader::StringResult<QVariant>, is_const>{
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QCborStreamReader::StringResult, is_pointer, is_const, is_reference,
                                 QVariant>{
    typedef QCborStreamReader::StringResult<QVariant> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_cr>::type, NativeType_cr>::type NativeType_ptr;

    static NativeType_ptr cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        QCborStreamReader::StringResult<QVariant>* result = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, result);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 QCborStreamReader::StringResult, is_pointer, is_const, is_reference,
                                 T>{
    typedef QCborStreamReader::StringResult<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_cr>::type, NativeType_cr>::type NativeType_ptr;

    static NativeType_ptr cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        QCborStreamReader::StringResult<QVariant>* result = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(QCborStreamReader::StringResult<QVariant>), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        QCborStreamReader::StringResult<T> tresult;
        tresult.status = result->status;
        tresult.data = result->data.value<T>();
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, &tresult);
    }
};

#endif //def QCBORSTREAMREADER_H

#if defined(_OPTIONAL_) || defined(_OPTIONAL) || (defined(_LIBCPP_OPTIONAL) && _LIBCPP_STD_VER > 14) || defined(_GLIBCXX_OPTIONAL)

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
         return QtJambiAPI::newJavaOptional(env, _in.has_value(), content);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, hasScope,
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
         return QtJambiAPI::newJavaOptionalInt(env, _in.has_value(), _in.has_value() ? jint(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, hasScope,
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
         return QtJambiAPI::newJavaOptionalInt(env, _in.has_value(), _in.has_value() ? jint(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, hasScope,
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
         return QtJambiAPI::newJavaOptionalLong(env, _in.has_value(), _in.has_value() ? jlong(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, hasScope,
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
         return QtJambiAPI::newJavaOptionalLong(env, _in.has_value(), _in.has_value() ? jlong(_in.value()) : 0);
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<true, hasScope,
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
         return QtJambiAPI::newJavaOptionalDouble(env, _in.has_value(), _in.has_value() ? jdouble(_in.value()) : 0);
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
        jobject value = QtJambiAPI::readJavaOptional(env, in, isPresent);
        if(isPresent){
            return NativeType(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope));
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, hasScope,
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
        jint value = QtJambiAPI::readJavaOptionalInt(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, hasScope,
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
        jint value = QtJambiAPI::readJavaOptionalInt(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, hasScope,
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
        jlong value = QtJambiAPI::readJavaOptionalLong(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, hasScope,
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
        jlong value = QtJambiAPI::readJavaOptionalLong(env, in, isPresent);
        if(isPresent){
            return NativeType(value);
        }else{
            return std::nullopt;
        }
    }
};

template<bool hasScope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, hasScope,
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
        jdouble value = QtJambiAPI::readJavaOptionalDouble(env, in, isPresent);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
struct qtjambi_jnitype_template1_cast<forward, has_scope,
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
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
        return QtJambiAPI::toJObjectArray(env, qtjambi_type<T>::id(), ref_ptr<is_pointer, NativeType_c>::ref(in), jsize(in.size()),
                       [scope](JNIEnv * env,const void* in, jsize index)->jobject{
                            const auto& element = reinterpret_cast<const std::initializer_list<T>*>(in)->begin()[index];
                            return qtjambi_scoped_cast<has_scope,jobject,decltype(element)>::cast(env, element, nullptr, scope);
                       }
                   );
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
            if (QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
                data = new JObjectArrayPointer<T>(env, jobjectArray(in), [scope](T& d,JNIEnv * env, jobject obj){
                    d = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, obj, nullptr, scope);
                });
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint32>{
    typedef std::initializer_list<qint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJIntArray(env, reinterpret_cast<const jint*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint16>{
    typedef std::initializer_list<qint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJShortArray(env, reinterpret_cast<const jshort*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint8>{
    typedef std::initializer_list<qint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 qint64>{
    typedef std::initializer_list<qint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJLongArray(env, reinterpret_cast<const jlong*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint32>{
    typedef std::initializer_list<quint32> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJIntArray(env, reinterpret_cast<const jint*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint16>{
    typedef std::initializer_list<quint16> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJShortArray(env, reinterpret_cast<const jshort*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint8>{
    typedef std::initializer_list<quint8> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 quint64>{
    typedef std::initializer_list<quint64> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJLongArray(env, reinterpret_cast<const jlong*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 float>{
    typedef std::initializer_list<float> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJFloatArray(env, reinterpret_cast<const jfloat*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 double>{
    typedef std::initializer_list<double> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJDoubleArray(env, reinterpret_cast<const jdouble*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 bool>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJBooleanArray(env, reinterpret_cast<const jboolean*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 wchar_t>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
              JavaException::raiseError(env, "Cannot cast to std::initializer_list<T> without scope." QTJAMBI_STACKTRACEINFO );
              return NativeType();
        }else{
            JCharArrayPointer * data = nullptr;
            if (JCharArrayPointer::isValidArray(env, in)) {
                data = new JCharArrayPointer(env, jcharArray(in), false);
                scope->addDeletion(data);
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
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 JniType,
                                 std::initializer_list, is_pointer, is_const, is_reference,
                                 QChar>{
    typedef std::initializer_list<bool> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar*>(in.begin()), jsize(in.size()));
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_template1_cast<false, has_scope,
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
                scope->addDeletion(data);
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
         bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 std::function, true, is_const, is_reference,
                                 T>{
    typedef std::function<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char* nativeTypeName, QtJambiScope* scope){
        return in ? qtjambi_move_or_copy_decider<NativeType, is_const>::cast(env, *in, nativeTypeName, scope) : nullptr;
    }
};

template<bool has_scope,
         bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<true, has_scope,
                                 jobject,
                                 std::function, false, is_const, is_reference,
                                 T>
        : qtjambi_move_or_copy_decider<std::function<T>, is_const>{};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<false, has_scope,
                                 jobject,
                                 std::function, is_pointer, is_const, is_reference,
                                 T>{
    typedef std::function<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::function<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::function<T> *");
        NativeType fct;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &fct, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return fct;
    }
};
#endif // defined(_FUNCTIONAL_)

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_TEMPLATE1_H
