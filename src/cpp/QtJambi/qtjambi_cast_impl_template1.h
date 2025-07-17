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

    template<bool forward, bool has_scope, typename JniType, bool is_pointer, bool is_const, bool is_reference, typename T, std::size_t E, bool t_is_const>
    struct qtjambi_jnitype_span_cast{
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
        : qtjambi_smart_pointer_caster<forward, has_scope,
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

#define LISTTYPE(TYPE) QtJambiAPI::ListType::TYPE,

#define QTJAMBI_CONTAINER1_CASTER(TYPE,SUPERTYPE,append)\
        template<bool has_scope,\
                 bool is_pointer, bool is_const, bool is_reference,\
                 typename T>\
        struct qtjambi_jnitype_template1_cast<true, has_scope,\
                                              jobject,\
                                              TYPE, is_pointer, is_const, is_reference,\
                                              T>{\
            typedef typename std::conditional<is_const, const TYPE<T>,TYPE<T>>::type Template;\
            typedef typename std::conditional<is_pointer, typename std::add_pointer<Template>::type, typename std::add_lvalue_reference<Template>::type>::type Template_in;\
            static jobject cast(JNIEnv *env, Template_in in, const char*, QtJambiScope* scope){\
                return QtJambiAPI::convert##SUPERTYPE##ToJavaObject(env,\
                                                                    scope ? scope->relatedNativeID() : InvalidNativeID,\
                                                                    ref_ptr<is_pointer, Template>::ref(in),\
                                                                    CloneContainer<TYPE,T, is_pointer && !is_const>::function,\
                                                                    DeleteContainer<TYPE,T>::function,\
                                                                    LISTTYPE(TYPE)\
                                                                    SUPERTYPE##Access<T>::newInstance()\
                                                                    );\
        }\
    };\
\
        template<bool has_scope,\
                 bool is_pointer, bool is_const, bool is_reference,\
                 typename T>\
        struct qtjambi_jnitype_template1_cast<false, has_scope,\
                                              jobject,\
                                              TYPE, is_pointer, is_const, is_reference,\
                                              T>{\
            typedef TYPE<T> NativeType;\
            typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
            typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
            typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
            typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
            STATICASSERT\
            static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){\
                NativeType* pointer{nullptr};\
                if(in){\
                    if (!ContainerAPI::getAs##TYPE<T>(env, in, pointer)) {\
                        if(!scope)\
                        JavaException::raiseError(env, QStringLiteral("Cannot cast to %1%2 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType))), is_reference ? "&" : "*") QTJAMBI_STACKTRACEINFO );\
                        if(is_const){\
                            pointer = new NativeType();\
                            scope->addDeletion(pointer);\
                        }else{\
                            auto ipointer = new IntermediateSequentialContainer<has_scope, TYPE,T>(env, in, scope);\
                            pointer = ipointer;\
                            scope->addDeletion(ipointer);\
                        }\
                        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                            jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                            pointer->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));\
                    }\
                }\
            }\
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, pointer);\
        }\
    };\
\
        template<bool has_scope,\
                 bool is_const,\
                 typename T>\
        struct qtjambi_jnitype_template1_cast<false, has_scope,\
                                              jobject,\
                                              TYPE, false, is_const, false,\
                                              T>\
    {\
            static constexpr bool is_pointer = false;\
            static constexpr bool is_reference = false;\
            typedef TYPE<T> NativeType;\
            typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
            typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
            typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
            typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
            STATICASSERT\
            static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){\
                if(!in)\
                return {};\
                NativeType* pointer{nullptr};\
                if (ContainerAPI::getAs##TYPE<T>(env, in, pointer)) {\
                    return *pointer;\
            }else{\
                    NativeType list;\
                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                        list.append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));\
                }\
                    return list;\
            }\
        }\
    };

#define STATICASSERT
    QTJAMBI_CONTAINER1_CASTER(QList,QList,append)
    QTJAMBI_CONTAINER1_CASTER(QQueue,QList,append)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QLINKEDLIST_H
#undef LISTTYPE
#define LISTTYPE(TYPE)
    QTJAMBI_CONTAINER1_CASTER(QLinkedList,QLinkedList,append)
#endif
#ifdef QVECTOR_H
#undef LISTTYPE
#define LISTTYPE(TYPE) QtJambiAPI::VectorType::TYPE,
#undef STATICASSERT
#define STATICASSERT Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");
    QTJAMBI_CONTAINER1_CASTER(QStack,QVector,append)
#undef STATICASSERT
#define STATICASSERT Q_STATIC_ASSERT_X(is_default_constructible<T>::value || is_copy_assignable<T>::value || is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");
    QTJAMBI_CONTAINER1_CASTER(QVector,QVector,append)
#undef STATICASSERT
#define STATICASSERT
#endif
#else
QTJAMBI_CONTAINER1_CASTER(QStack,QList,append)
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#undef LISTTYPE
#define LISTTYPE(TYPE)

    QTJAMBI_CONTAINER1_CASTER(QSet,QSet,insert)
#undef LISTTYPE
#undef STATICASSERT
#undef QTJAMBI_CONTAINER1_CASTER

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
            QDBusReply<QVariant> reply = QDBusReplyUtility<T>::createFrom(_in);
            return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &reply);
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
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, QDBusReplyUtility<T>::reverseCreateFrom(*reply));
            }else{
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, false, NativeType>::convert(env, scope, nullptr);
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
            return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &_in);
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
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, reply);
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
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(
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

#if QT_VERSION < QT_VERSION_CHECK(6, 6, 0) || defined(QTJAMBI_CAST_DEPRECATED_TYPES)

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

#endif //QT_VERSION < QT_VERSION_CHECK(6, 6, 0) || defined(QTJAMBI_CAST_DEPRECATED_TYPES)

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
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(
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
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(
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
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, std::move(tfuture));
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
                            Q_UNUSED(futureInterface)
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }else{
                            QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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
                            }else if(baseType==QLatin1String("QFutureInterfaceBase")
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
                    QString watcherType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureWatcherBase>::trySupplyType(watcher)));
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
class Continuation<T,bool,bool>{
#else
class CompactContinuation<T,bool,bool>{
#endif
public:
    typedef decltype(std::declval<QFuture<T>>().d) FutureInterface;
    static FutureInterface &sourceFuture(const QFuture<T>& future){return future.d;}
};
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
template<class T,class T2,class T3>
class Continuation : public CompactContinuation<T,T2,T3>{};
#endif
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
                QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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
                }else if(baseType==QLatin1String("QFutureInterfaceBase")
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
                        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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
                        }else if(baseType==QLatin1String("QFutureInterfaceBase")
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
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, QFuture<void>(base));
        }else{
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
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
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, QFuture<QVariant>(_futureInterface));
        }else{
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, nullptr);
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
        if(!future){
            QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
            if(baseType==QLatin1String("QFutureInterface<QVariant>")
                || baseType==QLatin1String("QFutureInterface_shell<QVariant>")){
                future = reinterpret_cast<QFutureInterface<QVariant>*>(base);
            }
        }
        QFutureInterface<T> tpromise = convert_future_interface<T>(future, "QFuture");
        QFuture<T> ft = tpromise.future();
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, std::move(ft));
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
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, result);
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
        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, has_scope, NativeType>::convert(env, scope, std::move(tresult));
    }
};

#endif //def QCBORSTREAMREADER_H

#if defined(_OPTIONAL_) || defined(_OPTIONAL) || (defined(_LIBCPP_OPTIONAL) && _LIBCPP_STD_VER > 14) || defined(_GLIBCXX_OPTIONAL)

template<bool forward, bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename T>
struct qtjambi_jnitype_optional_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename T>
struct qtjambi_jnitype_template1_cast<forward, has_scope,
                                      jobject,
                                      std::optional, is_pointer, is_const, is_reference, T>
    : qtjambi_jnitype_optional_cast<forward, has_scope,
                                    is_pointer, is_const, is_reference, T>{
};

template<bool has_scope, typename ElementType>
struct qtjambi_optional{
    template<typename Optional>
    static ElementType value(Optional& optional, JNIEnv *, QtJambiScope*){
        return optional.has_value() ? ElementType(optional.value()) : ElementType(0);
    }
    template<typename T>
    static std::optional<T> create(ElementType value, JNIEnv *, QtJambiScope*){
        return std::optional<T>(value);
    }
};

template<bool has_scope>
struct qtjambi_optional<has_scope, jobject>{
    template<typename Optional>
    static jobject value(Optional& optional, JNIEnv *env, QtJambiScope* scope){
        return optional.has_value() ? qtjambi_scoped_cast<has_scope,jobject,decltype(optional.value())>::cast(env, optional.value(), nullptr, scope) : nullptr;
    }
    template<typename T>
    static std::optional<T> create(jobject value, JNIEnv *env, QtJambiScope* scope){
        return std::optional<T>(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope));
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename T>
struct qtjambi_jnitype_optional_cast<true, has_scope, is_pointer, is_const, is_reference, T>{
    typedef std::optional<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename result_of<decltype(jni_type_decider<T>::readJavaOptional)>::type ElementType;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return jni_type_decider<T>::newJavaOptional(env, _in.has_value(), qtjambi_optional<has_scope, ElementType>::value(_in, env, scope));
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename T>
struct qtjambi_jnitype_optional_cast<false, has_scope, is_pointer, is_const, is_reference, T>{
    typedef std::optional<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename jni_type_decider<T>::JType JType;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        bool isPresent = false;
        auto value = jni_type_decider<T>::readJavaOptional(env, in, isPresent);
        if(isPresent){
            return qtjambi_optional<has_scope,decltype(value)>::template create<T>(value, env, scope);
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
    : qtjambi_smart_pointer_caster<forward, has_scope,
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

template<bool has_scope, typename T, bool is_const>
struct value_range_converter_from_java_buffer{
    template<typename JniType>
    static auto JBufferPointer(JNIEnv *, JniType){
        return nullptr;
    }
    static auto JBufferPointer(JNIEnv *env, jobject in){
        PersistentJBufferData * data = nullptr;
        if (JBufferData::isBuffer(env, in)) {
            data = new PersistentJBufferData(env, in);
        }
        return data;
    }
};

template<bool has_scope, typename T>
struct value_range_converter_from_java_buffer<has_scope, T, true>{
    template<typename JniType>
    static auto JBufferPointer(JNIEnv *, JniType){
        return nullptr;
    }
    static auto JBufferPointer(JNIEnv *env, jobject in){
        PersistentJBufferConstData * data = nullptr;
        if (JBufferConstData::isBuffer(env, in)) {
            data = new PersistentJBufferConstData(env, in);
        }
        return data;
    }
};

template<bool has_scope, typename NativeType, typename T, bool is_const, typename JniType>
struct qtjambi_collection_to_container_impl{
    static NativeType convert(JNIEnv *env, JniType in, QtJambiScope* scope){
        if(auto data = value_range_converter_from_java_buffer<has_scope, T, is_const>::JBufferPointer(env, in)){
            if(scope==nullptr){
                delete data;
                JavaException::raiseError(env, QStringLiteral("Cannot cast to %1 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
            }else{
                scope->addDeletion(data);
                return *data;
            }
        }else {
            QVector<T>* pointer{nullptr};
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if (!ContainerAPI::getAsQList<T>(env, in, pointer)) {
#else
            if (!ContainerAPI::getAsQVector<T>(env, in, pointer)) {
#endif
                if(scope==nullptr)
                    JavaException::raiseError(env, QStringLiteral("Cannot cast to %1 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                pointer = new QVector<T>();
                scope->addDeletion(pointer);
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
                if(iterator){
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                        pointer->append(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
                    }
                }
            }
            if(pointer)
                return qtjambi_construct_from_argument<NativeType>::create(*pointer);
        }
        return NativeType();
    }
};

template<bool has_scope, typename NativeType, typename T, bool is_const, typename JniType, bool isArrayType = is_jni_array_type<JniType>::value>
struct qtjambi_collection_to_container{
    static NativeType convert(JNIEnv *, JniType, QtJambiScope*){
        return NativeType();
    }
};

template<bool has_scope, typename NativeType, typename T, bool is_const, typename JniType>
struct qtjambi_collection_to_container<has_scope, NativeType, T, is_const, JniType, false>
    : qtjambi_collection_to_container_impl<has_scope, NativeType, T, is_const, JniType>{
    using qtjambi_collection_to_container_impl<has_scope, NativeType, T, is_const, JniType>::convert;
};

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

#if defined(_INITIALIZER_LIST_) || defined(_INITIALIZER_LIST) || defined(INITIALIZER_LIST) || defined(_LIBCPP_INITIALIZER_LIST) || defined(_GLIBCXX_INITIALIZER_LIST)

QT_WARNING_DISABLE_GCC("-Winit-list-lifetime")

template<bool forward, bool has_scope, typename JniType, bool is_pointer, bool is_const, bool is_reference, typename T>
struct qtjambi_jnitype_initializer_list_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<typename T>
struct qtjambi_construct_from_argument<std::initializer_list<T>>{
    static std::initializer_list<T> create(const QVector<T>& pointer){
        return QtJambiAPI::createIterable<std::initializer_list<T>>(pointer.data(), pointer.size());
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_template1_cast<forward, has_scope,
                                      JniType,
                                      std::initializer_list, is_pointer, is_const, is_reference,
                                      T>
    : qtjambi_jnitype_initializer_list_cast<forward, has_scope,
                                            JniType,
                                            is_pointer, is_const, is_reference,
                                            T>{
};

template<bool has_scope, typename TArray, typename T, bool is_const>
struct value_range_converter_base{};

template<bool has_scope, typename TArray, typename T, bool is_const>
struct value_range_converter : value_range_converter_base<has_scope, TArray, T, is_const>{
    using value_range_converter_base<has_scope, TArray, T, is_const>::JArrayPointer;
    using value_range_converter_base<has_scope, TArray, T, is_const>::NativePointerArray;
    using value_range_converter_base<has_scope, TArray, T, is_const>::toJavaArray;
    template<typename NativeType, typename JniType>
    static NativeType toNativeContainer(JNIEnv *env, JniType in, QtJambiScope* scope){
        if(in){
            if(auto data = JArrayPointer(env, in, scope)){
                if(scope==nullptr){
                    delete data;
                    JavaException::raiseError(env, QStringLiteral("Cannot cast to %1 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }else{
                    scope->addDeletion(data);
                    return *data;
                }
            }else{
                return qtjambi_collection_to_container<has_scope, NativeType, T, is_const, JniType>::convert(env, in, scope);
            }
        }
        return NativeType();
    }
};

template<bool has_scope, typename TArray, typename T, bool is_const>
struct value_range_converter_from_java_array{
    template<typename JniType>
    static auto JArrayPointer(JNIEnv *env, JniType in, QtJambiScope*){
        typedef typename jni_type<TArray>::template NativeFactory<T,is_const>::type NativeFactory;
        NativeFactory * data = nullptr;
        if (NativeFactory::isValidArray(env, in)) {
            data = new NativeFactory(env, TArray(in));
        }
        return data;
    }

    template<typename NativeType>
    static auto NativePointerArray(JNIEnv *env, NativeType* container, QtJambiScope*){
        typedef typename jni_type<TArray>::template JavaFactory<T,is_const>::type JavaFactory;
        return new JavaFactory(env, container->begin(), jsize(container->size()));
    }
};

template<bool has_scope, typename TArray, typename T>
struct value_range_converter_base<has_scope, TArray, T, true>
    : value_range_converter_from_java_array<has_scope, TArray, T, true>{
    using value_range_converter_from_java_array<has_scope, TArray, T, true>::NativePointerArray;
    template<typename NativeType>
    static TArray toJavaArray(JNIEnv *env, const NativeType* container, QtJambiScope*){
        typedef typename jni_type<TArray>::ElementType ElementType;
        return jni_type<TArray>::converterFunction(env, reinterpret_cast<const ElementType*>(container->begin()), jsize(container->size()));
    }
};

template<bool has_scope, typename TArray, typename T>
struct value_range_converter_base<has_scope, TArray, T, false>
    : value_range_converter_from_java_array<has_scope, TArray, T, false>{
    using value_range_converter_from_java_array<has_scope, TArray, T, false>::NativePointerArray;
    template<typename NativeType>
    static TArray toJavaArray(JNIEnv *env, const NativeType* container, QtJambiScope* scope){
        if(!container)
            return nullptr;
        if(scope){
            auto array = NativePointerArray(env, container, scope);
            scope->addDeletion(array);
            return array->array(env);
        }
        return value_range_converter<has_scope, TArray, T, true>::toJavaArray(env, container, scope);
    }
};

template<bool has_scope, typename T>
struct value_range_converter_base<has_scope, jobjectArray, T, true>{
    template<typename NativeType>
    static jobjectArray toJavaArray(JNIEnv *env, const NativeType* container, QtJambiScope* scope){
        if(!container)
            return nullptr;
        return QtJambiAPI::toJObjectArray(env, qtjambi_type<T>::id(), container, jsize(container->size()),
                                          [scope](JNIEnv * env,const void* in, jsize index)->jobject{
                                              const auto& element = reinterpret_cast<const NativeType*>(in)->begin()[index];
                                              return qtjambi_scoped_cast<has_scope,jobject,decltype(element)>::cast(env, element, nullptr, scope);
                                          }
                                          );
    }

    template<typename NativeType>
    static auto NativePointerArray(JNIEnv *env, const NativeType* container, QtJambiScope* scope){
        return new PersistentConstObjectPointerArray<T>(env, container->begin(), jsize(container->size()),
                                              [scope](JNIEnv * env,const T& in)->jobject{
                                                  return qtjambi_scoped_cast<true,jobject,T>::cast(env, in, nullptr, scope);
                                              });
    }

    template<typename JniType>
    static auto JArrayPointer(JNIEnv *env, JniType in, QtJambiScope* scope){
        PersistentJConstObjectArrayPointer<T>* data = nullptr;
        if (QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            data = new PersistentJConstObjectArrayPointer<T>(env, jobjectArray(in), [scope](T& d,JNIEnv * env, jobject obj){
                d = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, obj, nullptr, scope);
            });
        }
        return data;
    }
};

template<bool has_scope, typename T>
struct value_range_converter_base<has_scope, jobjectArray, T, false>{
    template<typename NativeType>
    static jobjectArray toJavaArray(JNIEnv *env, const NativeType* container, QtJambiScope* scope){
        if(!container)
            return nullptr;
        if(scope){
            auto array = NativePointerArray(env, container, scope);
            scope->addDeletion(array);
            return array->array(env);
        }
        return value_range_converter<has_scope, jobjectArray, T, true>::toJavaArray(env, container, scope);
    }

    template<typename NativeType>
    static auto NativePointerArray(JNIEnv *env, NativeType* container, QtJambiScope* scope){
        return new PersistentObjectPointerArray<T>(env, container->begin(), jsize(container->size()),
            [scope](JNIEnv * env,const T& in)->jobject{
                return qtjambi_scoped_cast<true,jobject,const T&>::cast(env, in, nullptr, scope);
            },
            [scope](T& d,JNIEnv * env, jobject obj){
                d = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, obj, nullptr, scope);
            });
    }

    template<typename JniType>
    static auto JArrayPointer(JNIEnv *env, JniType in, QtJambiScope* scope){
        PersistentJObjectArrayPointer<T>* data = nullptr;
        if (QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            data = new PersistentJObjectArrayPointer<T>(env, jobjectArray(in), [scope](T& d,JNIEnv * env, jobject obj){
                    d = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, obj, nullptr, scope);
                },
                [scope](JNIEnv * env,const T& in)->jobject{
                    return qtjambi_scoped_cast<true,jobject,const T&>::cast(env, in, nullptr, scope);
                });
        }
        return data;
    }
};

//template from any std::initializer_list to jobject

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_initializer_list_cast<true, has_scope,
                                             JniType,
                                             is_pointer, is_const, is_reference, T>{
    typedef std::initializer_list<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::remove_cv<T>::type T_noconst;
    typedef typename jni_type_decider<T_noconst>::JArrayType TArray;
    typedef typename jni_type<TArray>::ElementType ElementType;
    constexpr static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        return value_range_converter<has_scope, TArray, T, true>::toJavaArray(env, ref_ptr<is_pointer, NativeType_c>::ref(in), scope);
    }
};
template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_jnitype_initializer_list_cast<false, has_scope,
                                             JniType,
                                             is_pointer, is_const, is_reference, T>{
    typedef std::initializer_list<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::remove_cv<T>::type T_noconst;
    typedef typename jni_type_decider<T_noconst>::JArrayType TArray;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<T> *");
        return value_range_converter<has_scope, TArray, T, true>::template toNativeContainer<NativeType>(env, in, scope);
    }
};

#endif //defined(_INITIALIZER_LIST_)

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0) //&& defined(QSPAN_H)

//template from any QSpan to jobject

template<bool has_scope, typename T, bool t_is_const, std::size_t E>
struct value_range_converter_from_iterable{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_const;
    template<typename JniType>
    static QSpan<T_const,E>* Iterable(JNIEnv *, JniType, QtJambiScope*){
        return nullptr;
    }
};

template<typename Container>
class Result{
private:
    Container container;
    bool success = false;
public:
    template<typename V>
    Result& operator=(V& v) {
        container = v;
        success = true;
        return *this;
    }
    Container& operator*(){return container;}
    operator bool(){return success;}
};

template<typename T, bool t_is_const, std::size_t E>
struct value_range_converter_from_char_iterable{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_const;
    template<typename JniType>
    static QSpan<T_const,E>* Iterable(JNIEnv *, JniType, QtJambiScope*){
        return nullptr;
    }
    static auto Iterable(JNIEnv *env, jobject in, QtJambiScope*){
        Result<QSpan<T_const,E>> result;
        if (QtJambiAPI::isQByteArrayObject(env, in)) {
            if(QByteArray* ptr = qtjambi_cast<QByteArray*>(env, in))
                result = *ptr;
        }else{
            if constexpr(t_is_const){
                if (QtJambiAPI::isQByteArrayViewObject(env, in)) {
                    if(QByteArrayView* ptr = qtjambi_cast<QByteArrayView*>(env, in))
                        result = *ptr;
                }
            }
        }
        return result;
    }
};

template<bool has_scope, typename T, bool t_is_const, std::size_t E>
struct value_range_converter_from_char16_iterable{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_const;
    template<typename JniType>
    static QSpan<T_const,E>* Iterable(JNIEnv *, JniType, QtJambiScope*){
        return nullptr;
    }
    static auto Iterable(JNIEnv *env, jobject in, QtJambiScope* scope){
        Result<QSpan<T_const,E>> result;
        if (QtJambiAPI::isQStringObject(env, in)) {
            if(QString* ptr = qtjambi_cast<QString*>(env, in))
                result = *ptr;
        }else{
            if constexpr(t_is_const && has_scope){
                if(scope){
                    if (QtJambiAPI::isJavaString(env, in)) {
                        QStringView string = qtjambi_scoped_cast<has_scope,QStringView,jstring>::cast(env, jstring(in), nullptr, scope);
                        result = string;
                    }else if (QtJambiAPI::isJavaCharSequence(env, in)) {
                        QStringView string = qtjambi_scoped_cast<has_scope,QStringView,jobject>::cast(env, in, nullptr, scope);
                        result = string;
                    }
                }
            }
        }
        return result;
    }
};

template<bool has_scope, bool t_is_const, std::size_t E>
struct value_range_converter_from_iterable<has_scope, char, t_is_const, E>
    : value_range_converter_from_char_iterable<char, t_is_const, E>{
};

template<bool has_scope, bool t_is_const, std::size_t E>
struct value_range_converter_from_iterable<has_scope, qint8, t_is_const, E>
    : value_range_converter_from_char_iterable<qint8, t_is_const, E>{
};

template<bool has_scope, bool t_is_const, std::size_t E>
struct value_range_converter_from_iterable<has_scope, quint8, t_is_const, E>
    : value_range_converter_from_char_iterable<quint8, t_is_const, E>{
};

template<bool has_scope, bool t_is_const, std::size_t E>
struct value_range_converter_from_iterable<has_scope, wchar_t, t_is_const, E>
    : value_range_converter_from_char16_iterable<has_scope, wchar_t, t_is_const, E>{
};

template<bool has_scope, bool t_is_const, std::size_t E>
struct value_range_converter_from_iterable<has_scope, QChar, t_is_const, E>
    : value_range_converter_from_char16_iterable<has_scope, QChar, t_is_const, E>{
};

template<bool has_scope, bool t_is_const, std::size_t E>
struct value_range_converter_from_iterable<has_scope, char16_t, t_is_const, E>
    : value_range_converter_from_char16_iterable<has_scope, char16_t, t_is_const, E>{
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, std::size_t E, bool t_is_const>
struct qtjambi_jnitype_span_cast<true, has_scope,
                                 JniType,
                                 is_pointer, is_const, is_reference,
                                 T, E, t_is_const>{
    typedef QSpan<typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type,E> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<std::is_pointer<T>::value, T, typename std::add_const<T>::type>::type T_const;
    typedef typename jni_type_decider<T>::JArrayType TArray;
    typedef typename jni_type<TArray>::ElementType ElementType;
    static JniType cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        if constexpr(is_jni_array_type<JniType>::value){
            return value_range_converter<has_scope, TArray, T, t_is_const>::toJavaArray(env, ref_ptr<is_pointer, NativeType_c>::ref(in), scope);
        }else{
            if constexpr(has_scope){
                if(scope==nullptr)
                    return QtJambiAPI::convertQSpanToJavaObject<T, t_is_const, E>(env, in);
                else
                    return QtJambiAPI::convertQSpanToJavaObject<T, t_is_const, E>(env, in, scope->relatedNativeID());
            }else{
                return QtJambiAPI::convertQSpanToJavaObject<T, t_is_const, E>(env, in);
            }
        }
    }
};

template<bool has_scope,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference,
         typename T, std::size_t E, bool t_is_const>
struct qtjambi_jnitype_span_cast<false, has_scope,
                                 JniType,
                                 is_pointer, is_const, is_reference,
                                 T, E, t_is_const>{
    typedef QSpan<typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type,E> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<std::is_pointer<T>::value, T, typename std::add_const<T>::type>::type T_const;
    typedef typename jni_type_decider<T>::JArrayType TArray;
    typedef typename jni_type<TArray>::ElementType ElementType;
    static NativeType_out cast(JNIEnv *env, JniType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QSpan<T> &");
        Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QSpan<T> *");
        if constexpr(is_jni_array_type<JniType>::value){
            return value_range_converter<has_scope, TArray, T, t_is_const>::template toNativeContainer<NativeType>(env, in, scope);
        }else{
            if(QtJambiAPI::isQSpanObject(env, in)){
                QPair<void*,jlong> data = QtJambiAPI::fromQSpanObject(env, in, t_is_const, QMetaType::fromType<T>());
                if constexpr (has_scope && !t_is_const){
                    if(scope)
                        scope->addFinalAction([env, in](){QtJambiAPI::commitQSpanObject(env, in);});
                }
                if(data.first && data.second>0)
                    return NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), qsizetype(data.second));
            }
            return NativeType();
        }
    }
};

#endif // defined(QSPAN_H)

} // namespace QtJambiPrivate

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0) //&& defined(QSPAN_H)
namespace QtJambiAPI{
template<typename T, std::size_t E>
jobject convertQSpanToDetachedJavaObject(JNIEnv *env, const QSpan<T,E>& span){
    typedef typename std::remove_cv<T>::type T_;
    return QtJambiAPI::convertQSpanFromQListToJavaObject(env,
                                                &span,
                                                [](const void* ptr) -> void* {
                                                    const QSpan<T,E>& _span = *reinterpret_cast<const QSpan<T,E>*>(ptr);
                                                    return new QList<T_>(_span.begin(), _span.end());
                                                },
                                                QtJambiPrivate::DeleteContainer<QList,T_>::function,
                                                QtJambiPrivate::QListAccess<T_>::newInstance(),
                                                std::is_const<T>::value);
}

template<typename T, bool t_is_const, std::size_t E>
jobject convertQSpanToJavaObject(JNIEnv *env,
                                 const QSpan<typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type,E>& span,
                                 QtJambiNativeID owner){
    return QtJambiAPI::convertQSpanToJavaObject(env,
                                                owner,
                                                QtJambiPrivate::QSpanAccess<typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type,E>::newInstance(),
                                                span.begin(),
                                                jlong(span.size())
                                                );
}

} // namespace QtJambiAPI

#endif // defined(QSPAN_H)

#endif // QTJAMBI_CAST_IMPL_TEMPLATE1_H
