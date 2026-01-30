/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

namespace QtJambiPrivate {

    template<bool forward,
             typename JniType,
             template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast{
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    };

    //template from any QFlags to jobject
    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QFlags, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QFlags<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args... args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                return QtJambiAPI::convertQFlagsToJavaObject<T>(env, deref_ptr<is_pointer,NativeType_c>::deref(in));
            }else{
                if constexpr(is_pointer || is_reference){
                    Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QFlags<T>& without scope");
                    Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QFlags<T>* without scope");
                    NativeType* result = new NativeType(0);
                    if(!QtJambiAPI::convertJavaToNative(env, in, result, typeid(NativeType))){
                        delete result;
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                    if constexpr(is_const){
                        cast_var_args<Args...>::scope(args...).addDeletion(result);
                    }else{
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
                        cast_var_args<Args...>::scope(args...).addFinalAction([result, wrapper = JObjectWrapper(env, in)](){
                            if(JniEnvironment env{128}){
                                jobject o = wrapper.object(env);
                                if constexpr(sizeof(NativeType::Int)==sizeof(jlong))
                                    QtJambiAPI::setFlagsValue(env, o, jlong(result->toInt()));
                                else
                                    QtJambiAPI::setFlagsValue(env, o, jint(result->toInt()));
                            }
                            delete result;
                        });
#else
                        cast_var_args<Args...>::scope(args...).addFinalAction([result, env, in](){
                            if constexpr(sizeof(NativeType::Int)==sizeof(jlong))
                                QtJambiAPI::setFlagsValue(env, in, jlong(result->toInt()));
                            else
                                QtJambiAPI::setFlagsValue(env, in, jint(result->toInt()));
                            delete result;
                        });
#endif //defined(QTJAMBI_JOBJECTWRAPPER_H)
                    }

                    if constexpr(is_pointer){
                        return result;
                    }else{
                        return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
                    }
                }else{
                    NativeType result;
                    if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                    return result;
                }
            }
        }
    };

#if defined(QTJAMBI_JOBJECTWRAPPER_H)
    template<bool forward,
             typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          JniType,
                                          JArrayWrapper, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef JArrayWrapper<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
        typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;
        static constexpr bool isObjectOrArray = std::is_same<JniType,jobject>::value
                                                || std::is_same<JniType,jarray>::value
                                                || (jni_type<JniType>::isArray && std::is_same<typename jni_type<JniType>::ElementType,T>::value);

        static Out cast(In in, Args... args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(isObjectOrArray, "Cannot cast this JArrayWrapper to JniType.");
                if constexpr(is_pointer){
                    return JniType(in->object(env));
                }else{
                    return JniType(in.object(env));
                }
            }else{
                Q_STATIC_ASSERT_X(isObjectOrArray, "Cannot cast this JniType to JArrayWrapper.");
                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast to non-const JArrayWrapper& without scope");
                if constexpr(is_pointer){
                    if(!in)
                        return nullptr;
                }
                NativeType result;
                result.assign(env, in);
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
            }
        }
    };
#endif //defined(QTJAMBI_JOBJECTWRAPPER_H)

#define LISTTYPE(TYPE) QtJambiAPI::ListType::TYPE,

#define QTJAMBI_CONTAINER1_CASTER(TYPE,SUPERTYPE,append)\
        template<bool forward,\
                 bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,\
                 typename T, typename... Args>\
        struct qtjambi_jobject_template1_cast<forward,\
                                              jobject,\
                                              TYPE, is_pointer, is_const, is_reference, is_rvalue,\
                                              T, Args...>{\
            typedef TYPE<T> NativeType;\
            typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
            typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;\
            typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;\
            typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
            typedef typename std::conditional<forward, NativeType_in, jobject>::type In;\
            typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;\
            \
            static Out cast(In in, Args... args){\
                auto env = cast_var_args<Args...>::env(args...);\
                if constexpr(forward){\
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");\
                    return QtJambiAPI::convert##SUPERTYPE##ToJavaObject(env,\
                                                                    cast_var_args<Args...>::relatedNativeID(args...),\
                                                                    ref_ptr<is_pointer, NativeType_c>::ref(in),\
                                                                    CloneContainer<TYPE,T, is_pointer && !is_const>::function,\
                                                                    DeleteContainer<TYPE,T>::function,\
                                                                    LISTTYPE(TYPE)\
                                                                    SUPERTYPE##Access<T>::newInstance()\
                                                                );\
                }else{\
                    if constexpr(!is_pointer && !is_reference){\
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
                                list.append(qtjambi_cast<T,jobject, Args...>::cast(element, args...));\
                            }\
                            return list;\
                        }\
                    }else{\
                        NativeType* pointer{nullptr};\
                        if(in){\
                            if (!ContainerAPI::getAs##TYPE<T>(env, in, pointer)) {\
                                if constexpr(!is_reference && !is_pointer){\
                                    NativeType result;\
                                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                                        result.append(qtjambi_cast<T,jobject, Args...>::cast(element, args...));\
                                    }\
                                    return result;\
                                }else if constexpr(cast_var_args<Args...>::hasScope){\
                                    if(is_const){\
                                        pointer = new NativeType();\
                                        cast_var_args<Args...>::scope(args...).addDeletion(pointer);\
                                    }else{\
                                        auto ipointer = new IntermediateSequentialContainer<TYPE,T, Args...>(in, args...);\
                                        pointer = ipointer;\
                                        cast_var_args<Args...>::scope(args...).addDeletion(ipointer);\
                                    }\
                                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                                        pointer->append(qtjambi_cast<T,jobject, Args...>::cast(element, args...));\
                                    }\
                                }else {\
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );\
                                }\
                            }\
                        }\
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(pointer, args...);\
                    }\
                }\
            }\
        };

#define STATICASSERT
    QTJAMBI_CONTAINER1_CASTER(QList,QList,append)
    QTJAMBI_CONTAINER1_CASTER(QQueue,QList,append)
    QTJAMBI_CONTAINER1_CASTER(QStack,QList,append)

#undef LISTTYPE
#define LISTTYPE(TYPE)

    QTJAMBI_CONTAINER1_CASTER(QSet,QSet,insert)
#undef LISTTYPE
#undef STATICASSERT
#undef QTJAMBI_CONTAINER1_CASTER

#ifdef QDBUSREPLY_H
    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QDBusReply, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QDBusReply<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args... args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                if constexpr (std::is_same<QVariant, T>::value){
                    NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                    return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &_in);
                }else{
                    NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                    QDBusReply<QVariant> reply = QDBusReplyUtility<T>::createFrom(_in);
                    return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &reply);
                }
            }else{
                if constexpr (std::is_same<QVariant, T>::value){
                    QDBusReply<QVariant>* reply = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(env, in);
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(reply, args...);
                }else{
                    QDBusReply<QVariant>* reply = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(env, in);
                    if(reply){
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(QDBusReplyUtility<T>::reverseCreateFrom(*reply), args...);
                    }else{
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
                    }
                }
            }
        }
    };
#endif // QDBUSREPLY_H

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
            jobject mt = ::qtjambi_cast<jobject>(env, QMetaType::fromType<T*>());
            QtJambiAPI::setQQmlListPropertyElementType(env, result, mt);
            return result;
        }
    };

    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QQmlListProperty, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QQmlListProperty<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args... args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                return QmlListPropertyUtility<T>::toJavaObject(env, _in);
            }else{
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(
                    QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in), args...);
            }
        }
    };
#endif // QQMLLIST_H

#ifdef QPROPERTY_H
    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QBindable, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QBindable<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args...args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
            }else{
                return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
            }
        }
    };

#if QT_VERSION < QT_VERSION_CHECK(6, 6, 0) || defined(QTJAMBI_CAST_DEPRECATED_TYPES)

    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QPropertyAlias, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QPropertyAlias<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args...args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
            }else{
                return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
            }
        }
    };

#endif //QT_VERSION < QT_VERSION_CHECK(6, 6, 0) || defined(QTJAMBI_CAST_DEPRECATED_TYPES)

    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QPropertyBinding, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QPropertyBinding<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args...args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
            }else{
                return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
            }
        }
    };

    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QPropertyChangeHandler, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QPropertyChangeHandler<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args...args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
            }else{
                return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
            }
        }
    };
#endif // QPROPERTY_H

#ifdef QFUTUREINTERFACE_H

#if defined(QFUTURE_H)
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
        static FutureInterface *sourceFuture(const QFuture<T>* future){return future ? &future->d : nullptr;}
        static FutureInterface *sourceFuture(QFuture<T>* future){return future ? &future->d : nullptr;}
    };
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    template<class T,class T2,class T3>
    class Continuation : public CompactContinuation<T,T2,T3>{};
#endif
    }

    namespace QtJambiPrivate {
#endif

    template<typename TargetType, typename SourceType>
    TargetType convert_variant(const SourceType& source){
        if constexpr(std::is_same_v<SourceType,QVariant>){
            return source.template value<TargetType>();
        }else if constexpr(std::is_same_v<TargetType,QVariant>){
            return TargetType::template fromValue<SourceType>(std::move(source));
        }else{
            return TargetType(std::move(source));
        }
    }

    template<typename TargetType, typename SourceType>
    void copy_future_interface_results(const QFutureInterfaceBase* sourceBase, QFutureInterfaceBase* targetBase, int beginIndex, int count){
        const QFutureInterface<SourceType>* source = dynamic_cast<const QFutureInterface<SourceType>*>(sourceBase);
        QFutureInterface<TargetType>* target = dynamic_cast<QFutureInterface<TargetType>*>(targetBase);
        Q_ASSERT(source);
        Q_ASSERT(target);
        if constexpr((!std::is_copy_constructible<TargetType>::value
                        && std::is_move_constructible<TargetType>::value)
                    || (!std::is_copy_constructible<SourceType>::value
                        && std::is_move_constructible<SourceType>::value)){
            if(JniEnvironment env{200}){
                for(int i=0; i<count; ++i){
                    jobject obj = qtjambi_cast<jobject,SourceType,JNIEnv*>::cast(source->resultStoreBase().resultAt(beginIndex + i).template value<SourceType>(), env);
                    if constexpr(std::is_same_v<SourceType,QVariant>){
                        TargetType* n = qtjambi_cast<TargetType*,jobject,JNIEnv*>::cast(obj, env);
                        if(n)
                            target->reportResult(std::move(*n), beginIndex + i);
                    }else{
                        target->reportResult(qtjambi_cast<TargetType,jobject,JNIEnv*>::cast(obj, env), beginIndex + i);
                    }
                }
            }
        }else{
            if(count==1){
                target->reportResult(convert_variant<TargetType,SourceType>(source->resultStoreBase().resultAt(beginIndex).template value<SourceType>()), beginIndex);
            }else{
                QVector<TargetType> results(count);
                for(int i=0; i<count; ++i){
                    results << convert_variant<TargetType,SourceType>(source->resultStoreBase().resultAt(beginIndex).template value<SourceType>());
                }
                target->reportResults(results, count);
            }
        }
    }

    template<typename TargetType>
    struct FutureInterfaceTypeTest{
        static bool test(const QFutureInterfaceBase* base){
            if(dynamic_cast<const QFutureInterface<TargetType>*>(base))
                return true;
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
            QByteArray baseType = QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base));
            QByteArray requiredType = QtJambiAPI::typeName(typeid(QFutureInterface<TargetType>));
            return baseType==requiredType;
#else
            return false;
#endif
        };
        static constexpr QtJambiAPI::FutureInterfaceTypeTest fn = &test;
    };

    template<>
    struct FutureInterfaceTypeTest<QVariant>{
        static constexpr QtJambiAPI::FutureInterfaceTypeTest fn = &QtJambiAPI::isVariantFutureInterface;
    };

    template<>
    struct FutureInterfaceTypeTest<void>{
        static constexpr QtJambiAPI::FutureInterfaceTypeTest fn = &QtJambiAPI::isVoidFutureInterface;
    };

    template<typename TargetType, typename SourceType>
    QFutureInterface<TargetType> convert_future_interface(JNIEnv *env, const QFutureInterface<SourceType>* future, const char* translatedType = "QFutureInterface"){
        Q_STATIC_ASSERT_X((std::is_copy_constructible<TargetType>::value || std::is_move_constructible<TargetType>::value)
                              && std::is_destructible<TargetType>::value, "Cannot cast QFutureInterface<T> with non-constructible and non-movable result type T.");
        Q_STATIC_ASSERT_X((std::is_copy_constructible<SourceType>::value || std::is_move_constructible<SourceType>::value)
                              && std::is_destructible<SourceType>::value, "Cannot cast QFutureInterface<T> with non-constructible and non-movable result type T.");
        QFutureInterface<TargetType> result;
        if(future){
            if(QFutureInterfaceBase* availableResult = QtJambiAPI::translateQFutureInterface(
                    QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<SourceType>(*future)),
                    QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<TargetType>(result)),
                    &copy_future_interface_results<TargetType,SourceType>,
                    &copy_future_interface_results<SourceType,TargetType>,
                    FutureInterfaceTypeTest<TargetType>::fn)){
                if constexpr (std::is_same<void, TargetType>::value){
                    if(QFutureInterface<TargetType>* newResult = QtJambiAPI::asVoidFutureInterface(availableResult)){
                        return *newResult;
                    }else{
                        QByteArray baseType = QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(availableResult));
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1<void> to %2.").arg(translatedType, baseType) QTJAMBI_STACKTRACEINFO );
                    }
                }else if constexpr (std::is_same<QVariant, TargetType>::value){
                    if(QFutureInterface<TargetType>* newResult = QtJambiAPI::asVariantFutureInterface(availableResult)){
                        return *newResult;
                    }else{
                        QByteArray baseType = QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(availableResult));
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1<Variant> to %2.").arg(translatedType, baseType) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    if(QFutureInterface<TargetType>* newResult = dynamic_cast<QFutureInterface<TargetType>*>(availableResult)){
                        return *newResult;
                    }else{
                        QByteArray baseType = QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(availableResult));
                        QByteArray requiredType = QtJambiAPI::typeName(typeid(QFutureInterface<TargetType>));
                        auto idx = requiredType.indexOf('<');
                        if(idx>0){
                            requiredType = translatedType + requiredType.mid(idx);
                        }else{
                            requiredType = translatedType;
                        }
                        if(baseType==requiredType){
                            return *static_cast<QFutureInterface<TargetType>*>(availableResult);
                        }else if(baseType=="QFutureInterfaceBase"
                                   || baseType=="QFutureInterfaceBase_shell"
                                   || baseType=="QFutureInterface_vshell"){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1<void> to %2.").arg(translatedType, requiredType) QTJAMBI_STACKTRACEINFO );
                        }else if(baseType.startsWith("QFutureInterface<")){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1%2 to %3.").arg(translatedType, baseType.mid(16), requiredType) QTJAMBI_STACKTRACEINFO );
                        }else if(baseType=="QFutureInterface_shell"){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1<T> to %2.").arg(translatedType, requiredType) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }
            }
        }
        return result;
    }

    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QFutureInterface, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QFutureInterface<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args... args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                if constexpr (std::is_same<void, T>::value || std::is_same<QVariant, T>::value){
                    if constexpr(is_pointer){
                        if constexpr(cast_var_args<Args...>::hasScope)
                            return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                        else
                            return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
                    }else if constexpr(is_reference && !is_rvalue && !is_const && cast_var_args<Args...>::hasScope){
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in, typeid(NativeType));
                    }else{
                        return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType));
                    }
                }else{
                    NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                    std::unique_ptr<QFutureInterface<QVariant>> ptr = std::make_unique<QFutureInterface<QVariant>>(convert_future_interface<QVariant>(env, &_in));
                    jobject out = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ptr.get(), typeid(QFutureInterface<QVariant>));
                    if(out)
                        (void)ptr.release();
                    return out;
                }
            }else{
                QFutureInterfaceBase* base = qtjambi_cast<QFutureInterfaceBase*,jobject, Args...>::cast(in, args...);
                if constexpr (std::is_same<void, T>::value){
                    if(QFutureInterface<void>* fi = QtJambiAPI::asVoidFutureInterface(base)){
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(fi, args...);
                    }else{
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(NativeType(*base), args...);
                    }
                }else if constexpr (std::is_same<QVariant, T>::value){
                    if(QFutureInterface<QVariant>* futureInterface = QtJambiAPI::asVariantFutureInterface(base)){
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(futureInterface, args...);
                    }else if(QtJambiAPI::isVoidFutureInterface(base)){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureInterface<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 QFutureInterface<QVariant>.").arg(baseType) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    if(QFutureInterface<QVariant>* futureInterface = QtJambiAPI::asVariantFutureInterface(base)){
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(convert_future_interface<T>(env, futureInterface), args...);
                    }else if(QtJambiAPI::isVoidFutureInterface(base)){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureInterface<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else if(NativeType* fi = dynamic_cast<NativeType*>(base)){
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(fi, args...);
                    }else {
                        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
                        if(baseType==QString("QFutureInterface<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                            QFutureInterface<T>* futureInterface = reinterpret_cast<QFutureInterface<T>*>(base);
                            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(futureInterface, args...);
                        }else
#endif
                        {
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 QFutureInterface<%2>.").arg(baseType, QLatin1String(QtJambiAPI::typeName(typeid(T)))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }
            }
        }
    };
#endif // defined(QFUTUREINTERFACE_H)

#if defined(QFUTUREWATCHER_H)

    template<typename T, typename... Args>
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

    template<bool forward,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
             typename T, typename... Args>
    struct qtjambi_jobject_template1_cast<forward,
                                          jobject,
                                          QFutureWatcher, is_pointer, is_const, is_reference, is_rvalue,
                                          T, Args...>{
        typedef QFutureWatcher<T> NativeType;
        typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
        typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
        typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
        typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
        typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

        static Out cast(In in, Args... args){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                if(NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in)){
                    return QtJambiAPI::convertQFutureWatcherToJavaObject(env, _in,
                        [](JNIEnv * env, QFutureWatcherBase* base, jobject future){
                            QFutureWatcher<T>* watcher = static_cast<QFutureWatcher<T>*>(base);
                            QFuture<T> ft = qtjambi_jobject_template1_cast<false,jobject,QFuture,false,false,false,false,T,JNIEnv*>::cast(future, env);
                            watcher->setFuture(ft);
                        },
                        FutureResultFactory<T>::futureResult,
                        [](JNIEnv * env, QFutureWatcherBase* base) -> jobject {
                            QFutureWatcher<T>* watcher = static_cast<QFutureWatcher<T>*>(base);
                            QFuture<T> ft = watcher->future();
                            return qtjambi_jobject_template1_cast<true,jobject,QFuture,false,false,false,false,T,JNIEnv*>::cast(ft, env);
                        },
                        [](QFutureWatcherBase* base) -> std::unique_ptr<QFutureInterfaceBase> {
                            QFutureWatcher<T>* watcher = static_cast<QFutureWatcher<T>*>(base);
                            QFuture<T> ft = watcher->future();
                            return std::unique_ptr<QFutureInterfaceBase>{new QFutureInterface<T>(QtPrivate::Continuation<T,bool,bool>::sourceFuture(ft))};
                        }
                        );
                }else return nullptr;
            }else{
                QFutureWatcherBase* watcher = qtjambi_cast<QFutureWatcherBase*,jobject, Args...>::cast(in, args...);
                if constexpr (std::is_same<void, T>::value){
                    return ptr2ref<is_reference,NativeType>::value(env, reinterpret_cast<NativeType*>(watcher));
                }else if constexpr (std::is_same<QVariant, T>::value){
                    NativeType* result = dynamic_cast<NativeType*>(watcher);
                    if(result){
                        return ptr2ref<is_reference,NativeType>::value(env, result);
                    }else{
                        if(std::unique_ptr<QFutureInterfaceBase> base = QtJambiAPI::getQFutureInterfaceFromQFutureWatcher(env, in)){
                            if(QtJambiAPI::isVoidFutureInterface(base.get())){
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            }else if(QFutureInterface<QVariant>* futureInterface = QtJambiAPI::asVariantFutureInterface(base.get())){
                                if constexpr(!cast_var_args<Args...>::hasScope)
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                                QFutureInterface<T> _futureInterface = convert_future_interface<T>(env, futureInterface, "QFutureWatcher");
                                NativeType* _watcher = new NativeType(watcher);
                                cast_var_args<Args...>::scope(args...).addDeletion(_watcher);
                                _watcher->setFuture(_futureInterface.future());
                                return _watcher;
                            }else{
                                QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base.get())));
                                if(baseType==QLatin1String("QFutureInterfaceBase")
                                    || baseType==QLatin1String("QFutureInterfaceBase_shell")
                                    || baseType==QLatin1String("QFutureInterface_vshell")){
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher%1 to %2.").arg(baseType.mid(16), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                                }else{
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to %2.").arg(baseType, QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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
                    return nullptr;
                }else{
                    QFutureWatcher<T>* result = dynamic_cast<QFutureWatcher<T>*>(watcher);
                    if(!result){
                        if(std::unique_ptr<QFutureInterfaceBase> base = QtJambiAPI::getQFutureInterfaceFromQFutureWatcher(env, in)){
                            if(QFutureInterface<T>* futureInterface = dynamic_cast<QFutureInterface<T>*>(base.get())){
                                return ptr2ref<is_reference,NativeType>::value(env, reinterpret_cast<QFutureWatcher<T>*>(watcher));
                            }else if(QtJambiAPI::isVoidFutureInterface(base.get())){
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            }else if(QFutureInterface<QVariant>* futureInterface = QtJambiAPI::asVariantFutureInterface(base.get())){
                                if constexpr(!cast_var_args<Args...>::hasScope)
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1 without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                                QFutureInterface<T> _futureInterface = convert_future_interface<T>(env, futureInterface, "QFutureWatcher");
                                NativeType* _watcher = new NativeType(watcher);
                                _watcher->setFuture(_futureInterface.future());
                                cast_var_args<Args...>::scope(args...).addDeletion(_watcher);
                                return ptr2ref<is_reference,NativeType>::value(env, _watcher);
                            }else{
                                QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base.get())));
                                if(baseType==QString("QFutureInterface<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                                    return ptr2ref<is_reference,NativeType>::value(env, reinterpret_cast<QFutureWatcher<T>*>(watcher));
                                }else if(baseType==QLatin1String("QFutureInterfaceBase")
                                         || baseType==QLatin1String("QFutureInterfaceBase_shell")
                                         || baseType==QLatin1String("QFutureInterface_vshell")){
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureWatcher%1 to %2.").arg(baseType.mid(16), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                                }else{
                                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to %2.").arg(baseType, QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                        }
                        if(watcher){
                            QString watcherType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureWatcherBase>::trySupplyType(watcher)));
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
                            if(watcherType==QString("QFutureWatcher<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                                return ptr2ref<is_reference,NativeType>::value(env, reinterpret_cast<QFutureWatcher<T>*>(watcher));
                            }
#endif
                            if(watcherType==QLatin1String("QFutureWatcher_shell")){
                                watcherType = QLatin1String("QFutureWatcher<?>");
                            }
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to %2.").arg(watcherType).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }else if constexpr(is_reference)
                            JavaException::raiseNullPointerException(env, "Cannot cast null to QFutureWatcher&." QTJAMBI_STACKTRACEINFO );
                    }
                    return ptr2ref<is_reference,NativeType>::value(env, result);
                }
            }
        }
    };
#endif //defined(QFUTUREWATCHER_H)

#ifdef QPROMISE_H

template<bool forward, typename T,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QPromise, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QPromise<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        Q_STATIC_ASSERT_X(is_reference || is_pointer, "Cannot cast QPromise<T> without reference or pointer");
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if(NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in)){
                if constexpr (std::is_same<void, T>::value || std::is_same<QVariant, T>::value){
                    if constexpr(is_rvalue && !is_const){
                        std::unique_ptr<NativeType> ptr = std::make_unique<NativeType>(std::move(in));
                        jobject out = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ptr.get(), typeid(NativeType));
                        if(out)
                            (void)ptr.release();
                        return out;
                    }else{
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, _in, typeid(NativeType));
                    }
                }else{
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QPromise without scope.");
                    QFutureInterface<T>* base = reinterpret_cast<QFutureInterface<T>*>(_in);
                    QFutureInterface<QVariant>* _futureInterface = new QFutureInterface<QVariant>(convert_future_interface<QVariant>(env, base, "QPromise"));
                    cast_var_args<Args...>::scope(args...).addDeletion(_futureInterface);
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), reinterpret_cast<QPromise<QVariant>*>(_futureInterface), typeid(QPromise<QVariant>));
                }
            }else return nullptr;
        }else{
            if(!in)
                JavaException::raiseNullPointerException(env, "Cannot cast null to QPromise&." QTJAMBI_STACKTRACEINFO );
            QPromise<QVariant>* promise = QtJambiAPI::convertJavaObjectToNative<QPromise<QVariant>>(env, in);
            QPromise<T>* result{nullptr};
            if(promise){
                QFutureInterfaceBase* base = reinterpret_cast<QFutureInterfaceBase*>(promise);
                if constexpr (std::is_same<QVariant, T>::value){
                    if(QtJambiAPI::isVariantFutureInterface(base)){
                        result = reinterpret_cast<QPromise<T>*>(promise);
                    }else if(QtJambiAPI::isVoidFutureInterface(base)){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else {
                        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
                        if(baseType==QLatin1String("QFutureInterfaceBase")
                                   || baseType==QLatin1String("QFutureInterfaceBase_shell")
                                   || baseType==QLatin1String("QFutureInterface_vshell")){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise%1 to %2.").arg(baseType.mid(16), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }else {
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to %2.").arg(baseType, QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }else if constexpr (std::is_same<void, T>::value){
                    if(QtJambiAPI::isVoidFutureInterface(base)){
                        result = reinterpret_cast<QPromise<T>*>(promise);
                    }else{
                        QFutureInterface<T>* _futureInterface = new QFutureInterface<T>(*base);
                        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QPromise<void> without scope.");
                        cast_var_args<Args...>::scope(args...).addDeletion(_futureInterface);
                        result = reinterpret_cast<QPromise<T>*>(_futureInterface);
                    }
                }else{
                    if(QFutureInterface<T>* futureInterface = dynamic_cast<QFutureInterface<T>*>(base)){
                        result = reinterpret_cast<QPromise<T>*>(promise);
                    }else if(QFutureInterface<QVariant>* futureInterface = QtJambiAPI::asVariantFutureInterface(base)){
                        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QPromise without scope.");
                        QFutureInterface<T>* _futureInterface = new QFutureInterface<T>(convert_future_interface<T>(env, futureInterface, "QPromise"));
                        cast_var_args<Args...>::scope(args...).addDeletion(_futureInterface);
                        result = reinterpret_cast<QPromise<T>*>(_futureInterface);
                    }else if(base){
                        if(QtJambiAPI::isVoidFutureInterface(base)){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }else {
                            QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
                            if(baseType==QString("QFutureInterface<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                                QFutureInterface<T>* futureInterface = reinterpret_cast<QFutureInterface<T>*>(base);
                                result = reinterpret_cast<QPromise<T>*>(futureInterface);
                            }else
#endif
                            if(baseType==QLatin1String("QFutureInterfaceBase")
                                       || baseType==QLatin1String("QFutureInterfaceBase_shell")){
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise%1 to %2.").arg(baseType.mid(16), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            }else {
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to %2.").arg(baseType, QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            }
                        }
                    }
                }
            }
            if constexpr(is_pointer){
                return result;
            }else if constexpr(is_reference){
                if(!result)
                    JavaException::raiseNullPointerException(env, "Cannot cast null to QPromise<A>&." QTJAMBI_STACKTRACEINFO );
                return *result;
            }else{
                JavaException::raiseIllegalArgumentException(env, "Cannot cast QPromise<A> to QPromise<B>." QTJAMBI_STACKTRACEINFO );
                throw "Cannot cast QPromise<A> to QPromise<B>.";
            }
        }
    }
};

#endif //def QPROMISE_H

#ifdef QFUTURE_H

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QFuture, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QFuture<T> NativeType;
    typedef QFutureInterface<T> FutureInterfaceT;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr (std::is_same<QVariant, T>::value || std::is_same<void, T>::value){
                if constexpr(is_pointer){
                    if constexpr(cast_var_args<Args...>::hasScope)
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                    else
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
                }else if constexpr(is_reference && !is_rvalue && !is_const && cast_var_args<Args...>::hasScope){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in, typeid(NativeType));
                }else{
                    return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType));
                }
            }else{
                NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                QFutureInterface<QVariant> jpromise = convert_future_interface<QVariant>(
                    env,
                    &QtPrivate::Continuation<T,bool,bool>::sourceFuture(_in),
                    "QFuture"
                    );
                QFuture<QVariant> ft = jpromise.future();
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &ft, typeid(QFuture<QVariant>));
            }
        }else{
            QFuture<QVariant>* future = QtJambiAPI::convertJavaObjectToNative<QFuture<QVariant>>(env, in);
            QFutureInterfaceBase* base = QtPrivate::Continuation<QVariant,bool,bool>::sourceFuture(future);
            if constexpr (std::is_same<void, T>::value){
                if(QtJambiAPI::isVoidFutureInterface(base)){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(reinterpret_cast<QFuture<T>*>(future), args...);
                }else{
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(NativeType(base), args...);
                }
            }else if constexpr (std::is_same<QVariant, T>::value){
                if(QtJambiAPI::isVariantFutureInterface(base)){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(reinterpret_cast<QFuture<T>*>(future), args...);
                }else if(QtJambiAPI::isVoidFutureInterface(base)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }else{
                    QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
                    if(baseType==QLatin1String("QFutureInterfaceBase")
                        || baseType==QLatin1String("QFutureInterfaceBase_shell")){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture%1 to %2.").arg(baseType.mid(16), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else{
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture<T> to %2.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }else{
                if(QFutureInterface<QVariant>* futureInterface = QtJambiAPI::asVariantFutureInterface(base)){
                    QFutureInterface<T> fi = convert_future_interface<T>(env, futureInterface);
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(NativeType(&fi), args...);
                }else if(QtJambiAPI::isVoidFutureInterface(base)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }else if(dynamic_cast<FutureInterfaceT*>(base)){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(reinterpret_cast<QFuture<T>*>(future), args...);
                }else {
                    QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
                    if(baseType==QString("QFutureInterface<%1>").arg(QLatin1String(QtJambiAPI::typeName(typeid(T))))){
                        QFutureInterface<T>* futureInterface = reinterpret_cast<QFutureInterface<T>*>(base);
                        QFutureInterface<T> fi = convert_future_interface<T>(env, futureInterface);
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(NativeType(&fi), args...);
                    }else
#endif
                    if(baseType==QLatin1String("QFutureInterfaceBase")
                               || baseType==QLatin1String("QFutureInterfaceBase_shell")){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture%1 to %2.").arg(baseType.mid(16), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else {
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to %2.").arg(baseType, QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
            JavaException::raiseIllegalArgumentException(env, "Cannot cast QFuture<A> to QFuture<B>." QTJAMBI_STACKTRACEINFO );
            throw "Cannot cast QFuture<A> to QFuture<B>.";
        }
    }
};
#endif // QFUTURE_H

#ifdef QCBORSTREAMREADER_H

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QCborStreamReader::StringResult, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QCborStreamReader::StringResult<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr (std::is_same<QVariant, T>::value){
                if constexpr(is_pointer){
                    if constexpr(cast_var_args<Args...>::hasScope)
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                    else
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
                }else if constexpr(is_reference && !is_rvalue && !is_const && cast_var_args<Args...>::hasScope){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in, typeid(NativeType));
                }else{
                    std::unique_ptr<NativeType> ptr;
                    if constexpr(is_rvalue && !is_const)
                        ptr = std::make_unique<NativeType>(std::move(in));
                    else
                        ptr = std::make_unique<NativeType>(in);
                    jobject out = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ptr.get(), typeid(NativeType));
                    if(out)
                        (void)ptr.release();
                    return out;
                }
            }else{
                NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                if constexpr((is_reference || is_pointer) && !is_rvalue && !is_const && cast_var_args<Args...>::hasScope){
                    std::shared_ptr<QCborStreamReader::StringResult<QVariant>> ptr = std::make_shared<QCborStreamReader::StringResult<QVariant>>();
                    ptr->data = _in.data;
                    ptr->status = _in.status;
                    cast_var_args<Args...>::scope(args...).addFinalAction([ptr, out = &_in](){
                        out->status = ptr->status;
                        out->data = ptr->data.value<T>();
                    });
                    return QtJambiAPI::convertSmartPointerToJavaObject(env, ptr);
                }else{
                    std::unique_ptr<QCborStreamReader::StringResult<QVariant>> ptr = std::make_unique<QCborStreamReader::StringResult<QVariant>>();
                    ptr->data = _in.data;
                    ptr->status = _in.status;
                    jobject out = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ptr.get(), typeid(QCborStreamReader::StringResult<QVariant>));
                    if(out)
                        (void)ptr.release();
                    return out;
                }
            }
        }else{
            if constexpr(is_pointer){
                if(!in)
                    return nullptr;
            }
            QCborStreamReader::StringResult<QVariant>* result = nullptr;
            if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(QCborStreamReader::StringResult<QVariant>))){
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
            }
            if constexpr (std::is_same<QVariant, T>::value){
                if(!result)
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(QCborStreamReader::StringResult<QVariant>(), args...);
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(result, args...);
            }else{
                if constexpr((is_reference || is_pointer) && !is_rvalue && !is_const && cast_var_args<Args...>::hasScope){
                    QCborStreamReader::StringResult<T>* tresult = new QCborStreamReader::StringResult<T>;
                    if(result){
                        tresult->status = result->status;
                        tresult->data = result->data.value<T>();
                        cast_var_args<Args...>::scope(args...).addFinalAction([tresult, result](){
                            result->status = tresult->status;
                            result->data = tresult->data;
                            delete tresult;
                        });
                    }
                    if constexpr(is_pointer){
                        return tresult;
                    }else{
                        return *tresult;
                    }
                }else{
                    QCborStreamReader::StringResult<T> tresult;
                    if(result){
                        tresult.status = result->status;
                        tresult.data = result->data.value<T>();
                    }
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(tresult), args...);
                }
            }
        }
    }
};

#endif //def QCBORSTREAMREADER_H

#if defined(_OPTIONAL_) || defined(_OPTIONAL) || (defined(_LIBCPP_OPTIONAL) && _LIBCPP_STD_VER > 14) || defined(_GLIBCXX_OPTIONAL)

template<typename ElementType>
struct qtjambi_optional{
    template<typename Optional, typename... Args>
    static ElementType value(Optional& optional, Args...){
        return optional.has_value() ? ElementType(optional.value()) : ElementType(0);
    }
    template<typename T, typename... Args>
    static std::optional<T> create(ElementType value, Args...){
        return std::optional<T>(value);
    }
};

template<>
struct qtjambi_optional<jobject>{
    template<typename Optional, typename... Args>
    static jobject value(Optional& optional, Args... args){
        return optional.has_value() ? qtjambi_cast<jobject,decltype(optional.value()), Args...>::cast(optional.value(), args...) : nullptr;
    }
    template<typename T, typename... Args>
    static std::optional<T> create(jobject value, Args... args){
        return std::optional<T>(qtjambi_cast<T,jobject, Args...>::cast(value, args...));
    }
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      std::optional, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef std::optional<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;
    typedef typename result_of<decltype(jni_type_decider<T>::readJavaOptional)>::type ElementType;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return jni_type_decider<T>::newJavaOptional(env, _in.has_value(), qtjambi_optional<ElementType>::template value<NativeType_c, Args...>(_in, args...));
        }else{
            bool isPresent = false;
            auto value = jni_type_decider<T>::readJavaOptional(env, in, isPresent);
            if(isPresent){
                return qtjambi_optional<decltype(value)>::template create<T, Args...>(value, args...);
            }else{
                return std::nullopt;
            }
        }
    }
};
#endif

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QPointer, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef QPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return qtjambi_cast<jobject,T*, Args...>::cast(_in.data(), args...);
        }else{
            T* object = qtjambi_cast<T*,jobject, Args...>::cast(in, args...);
            NativeType ptr{object};
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(ptr), args...);
        }
    }
};

//template from any type's QWeakPointer<T> to jobject
template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QWeakPointer, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QWeakPointer<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if(QSharedPointer<T> ptr = deref_ptr<is_pointer,NativeType_c>::deref(in).toStrongRef()){
                return qtjambi_cast<jobject,QSharedPointer<T>, Args...>::cast(ptr, args...);
            }else
                return nullptr;
        }else{
            QSharedPointer<T> pointer = qtjambi_cast<QSharedPointer<T>,jobject, Args...>::cast(in, args...);
            NativeType ptr = pointer.toWeakRef();
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(ptr), args...);
        }
    }
};

//template from any type's QSharedPointer<T> to jobject
template<bool forward,
         typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      QSharedPointer, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>
    : qtjambi_smart_pointer_cast<forward, JniType, QSharedPointer, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
};

#if defined(_MEMORY_) || defined(_LIBCPP_MEMORY) || defined(_GLIBCXX_MEMORY)
//template from any type's std::weak_ptr<T> to jobject

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      std::weak_ptr, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef std::weak_ptr<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if(std::shared_ptr<T> ptr = std::shared_ptr<T>(deref_ptr<is_pointer,NativeType_c>::deref(in))){
                return qtjambi_cast<jobject,std::shared_ptr<T>, Args...>::cast(ptr, args...);
            }else
                return nullptr;
        }else{
            std::shared_ptr<T> pointer = qtjambi_cast<std::shared_ptr<T>,jobject, Args...>::cast(in, args...);
            NativeType ptr(pointer);
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(ptr), args...);
        }
    }
};

//template from any type's std::shared_ptr<T> to jobject
template<bool forward,
         typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      std::shared_ptr, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>
    : qtjambi_smart_pointer_cast<forward, JniType, std::shared_ptr, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
};
#endif //defined(_MEMORY_)

template<typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_from_java_buffer{
    static auto JBufferPointer(JniType in, Args...args){
        PersistentJBufferData * data = nullptr;
        if constexpr(std::is_same_v<JniType, jobject>){
            auto env = cast_var_args<Args...>::env(args...);
            if (JBufferData::isBuffer(env, in)) {
                data = new PersistentJBufferData(env, in);
            }
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            Q_UNUSED(in)
        }
        return data;
    }
};

template<typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_from_java_buffer<T, true, JniType, NativeType, Args...>{
    static auto JBufferPointer(JniType in, Args...args){
        PersistentJBufferConstData * data = nullptr;
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(std::is_same_v<JniType, jobject>){
            if (JBufferConstData::isBuffer(env, in)) {
                data = new PersistentJBufferConstData(env, in);
            }
        }else{
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return data;
    }
};

template<typename Out>
struct qtjambi_construct_from_argument{
    typedef Out create;
};

template<typename NativeType>
struct value_range_requires_scope : std::true_type {

};

template<>
struct value_range_requires_scope<QByteArray> : std::false_type {

};

template<>
struct value_range_requires_scope<QString> : std::false_type {

};

template<typename T, size_t N>
struct value_range_requires_scope<std::array<T,N>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QList<T>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QSet<T>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QQueue<T>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QStack<T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QMap<K,T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QMultiMap<K,T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QHash<K,T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QMultiHash<K,T>> : std::false_type {

};

template<typename NativeType, typename T, bool is_const, typename JniType, typename... Args>
struct qtjambi_collection_to_container_impl{

    static NativeType convert(JniType in, Args... args){
        if(auto data = value_range_converter_from_java_buffer<T, is_const, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !value_range_requires_scope<NativeType>::value, "Cannot cast without scope.");
            if constexpr(value_range_requires_scope<NativeType>::value)
                cast_var_args<Args...>::scope(args...).addDeletion(data);
            return data->operator NativeType();
        }else {
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(cast_var_args<Args...>::hasScope){
                QVector<T>* pointer{nullptr};
                if (!ContainerAPI::getAsQList<T>(env, in, pointer)) {
                    pointer = new QVector<T>();
                    cast_var_args<Args...>::scope(args...).addDeletion(pointer);
                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
                    if(iterator){
                        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                            jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                            pointer->append(qtjambi_cast<T,jobject, Args...>::cast(element, args...));
                        }
                    }
                }
                if(pointer)
                    return qtjambi_construct_from_argument<NativeType>::create(*pointer);
            }else{
                QVector<T>* pointer{nullptr};
                if (ContainerAPI::getAsQList<T>(env, in, pointer)) {
                    if(pointer)
                        return qtjambi_construct_from_argument<NativeType>::create(*pointer);
                }
                QVector<T> container;
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
                if(iterator){
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                        container.append(qtjambi_cast<T,jobject, Args...>::cast(element, args...));
                    }
                }
                return qtjambi_construct_from_argument<NativeType>::create(container);
            }
        }
        return NativeType();
    }
};

template<typename NativeType, typename T, bool is_const, typename JniType, bool isArrayType, typename... Args>
struct qtjambi_collection_to_container{
    static NativeType convert(JniType, Args...){
        return NativeType();
    }
};

template<typename NativeType, typename T, bool is_const, typename JniType, typename... Args>
struct qtjambi_collection_to_container<NativeType, T, is_const, JniType, false, Args...>
    : qtjambi_collection_to_container_impl<NativeType, T, is_const, JniType, Args...>{
    using qtjambi_collection_to_container_impl<NativeType, T, is_const, JniType, Args...>::convert;
};

#if defined(_FUNCTIONAL_) || defined(_FUNCTIONAL) || defined(_LIBCPP_FUNCTIONAL) || defined(_GLIBCXX_FUNCTIONAL)
//template from any std::function to jobject

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      std::function, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef std::function<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(!std::is_same_v<JniType, jobject>){
            Q_STATIC_ASSERT_X(false && !cast_var_args<Args...>::hasScope, "Cannot cast types");
        }else if constexpr(forward){
            if constexpr(is_pointer){
                if constexpr(cast_var_args<Args...>::hasScope)
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...));
                else
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...));
            }else{
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...));
            }
        }else{
            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast to non-const std::function<R(Args...)>*");
            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast to non-const std::function<R(Args...)>&");
            if constexpr(is_pointer){
                if(!in)
                    return nullptr;
            }
            NativeType result;
            if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...))){
                if constexpr(cast_var_args<Args...>::hasNativeTypeName){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), cast_var_args<Args...>::nativeTypeName(args...)) QTJAMBI_STACKTRACEINFO );
                }else{
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
        }
    }
};

#endif // defined(_FUNCTIONAL_)

#if defined(_INITIALIZER_LIST_) || defined(_INITIALIZER_LIST) || defined(INITIALIZER_LIST) || defined(_LIBCPP_INITIALIZER_LIST) || defined(_GLIBCXX_INITIALIZER_LIST)

QT_WARNING_DISABLE_GCC("-Winit-list-lifetime")

template<typename T>
struct qtjambi_construct_from_argument<std::initializer_list<T>>{
    static std::initializer_list<T> create(const QVector<T>& pointer){
        return QtJambiAPI::createIterable<std::initializer_list<T>>(pointer.data(), pointer.size());
    }
};

template<typename O>
struct arg_pointer{
};

template<typename O>
struct arg_pointer<O*>{
    typedef O* type;
    static constexpr type ref(type o) {
        return o;
    }
    static constexpr type deref(type o,...) {
        return o;
    }
};

template<>
struct arg_pointer<JNIEnv*>{
    typedef JNIEnv* type;
    static constexpr type deref(type,type env) {
        return env;
    }
    static constexpr type ref(type o) {
        return o;
    }
    static constexpr type deref(type o) {
        return o;
    }
};

template<typename O>
struct arg_pointer<O&>{
    typedef O* type;
    static constexpr type ref(O& o) {
        return &o;
    }
    static constexpr O& deref(type o,...) {
        return *o;
    }
};

template<typename TArray, typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base{};

template<typename TArray, typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter : value_range_converter_base<TArray, T, is_const, JniType, NativeType, Args...>{
    using value_range_converter_base<TArray, T, is_const, JniType, NativeType, Args...>::JArrayPointer;
    static NativeType toNativeContainer(JniType in, jsize expectedSize, typename arg_pointer<Args>::type... args){
        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !value_range_requires_scope<NativeType>::value, "Cannot cast without scope.");
        if(in){
            if(auto data = value_range_converter_base<TArray, T, is_const, JniType, NativeType, Args...>::JArrayPointer(in, args...)){
                if constexpr(value_range_requires_scope<NativeType>::value)
                    cast_var_args<Args...>::scope(args...).addDeletion(data);
                if(expectedSize>=0 && data->size()>0 && data->size()<expectedSize){
                    JavaException::raiseIllegalArgumentException(cast_var_args<typename arg_pointer<Args>::type...>::env(args...), QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(data->size()), QString::number(expectedSize)) QTJAMBI_STACKTRACEINFO );
                }
                return data->operator NativeType();
            }else{
                return qtjambi_collection_to_container<NativeType, T, is_const, JniType, is_jni_array_type<JniType>::value, Args...>::convert(in, arg_pointer<Args>::deref(args)...);
            }
        }
        return NativeType();
    }
};

template<typename TArray, typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_from_java_array{
    static auto JArrayPointer(JniType in, typename arg_pointer<Args>::type...args){
        typedef typename jni_type<TArray>::template NativeFactory<T,is_const>::PersistentConverter NativeFactory;
        NativeFactory * data = nullptr;
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        if (NativeFactory::isValidArray(env, in)) {
            data = new NativeFactory(env, TArray(in));
        }
        return data;
    }

    static auto NativePointerArray(NativeType* container, typename arg_pointer<Args>::type...args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        typedef typename jni_type<TArray>::template JavaFactory<T,is_const>::PersistentConverter JavaFactory;
        return new JavaFactory(env, container->begin(), jsize(container->size()));
    }
};

template<typename TArray, typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<TArray, T, true, JniType, NativeType, Args...>
    : value_range_converter_from_java_array<TArray, T, true, JniType, NativeType, Args...>{
    using value_range_converter_from_java_array<TArray, T, true, JniType, NativeType, Args...>::JArrayPointer;
    using value_range_converter_from_java_array<TArray, T, true, JniType, NativeType, Args...>::NativePointerArray;

    static TArray toJavaArray(const NativeType* container, typename arg_pointer<Args>::type...args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        typedef typename jni_type<TArray>::ElementType ElementType;
        return jni_type<TArray>::converterFunction(env, reinterpret_cast<const ElementType*>(container->begin()), jsize(container->size()));
    }
};

template<typename TArray, typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<TArray, T, false, JniType, NativeType, Args...>
    : value_range_converter_from_java_array<TArray, T, false, JniType, NativeType, Args...>{
    using value_range_converter_from_java_array<TArray, T, false, JniType, NativeType, Args...>::JArrayPointer;
    using value_range_converter_from_java_array<TArray, T, false, JniType, NativeType, Args...>::NativePointerArray;

    static TArray toJavaArray(const NativeType* container, typename arg_pointer<Args>::type... args){
        if(!container)
            return nullptr;
        if constexpr(cast_var_args<Args...>::hasScope){
            auto array = value_range_converter_from_java_array<TArray, T, false, JniType, const NativeType, Args...>::NativePointerArray(container, args...);
            cast_var_args<Args...>::scope(args...).addDeletion(array);
            auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
            return array->array(env);
        }
        return value_range_converter<TArray, T, true, JniType, NativeType, Args...>::toJavaArray(container, args...);
    }
};

template<typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<jobjectArray, T, true, JniType, NativeType, Args...>{

    static jobjectArray toJavaArray(const NativeType* container, typename arg_pointer<Args>::type... args){
        if(!container)
            return nullptr;
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        return QtJambiAPI::toJObjectArray(env, qtjambi_type<T>::id(), container, jsize(container->size()),
                                          [args...](JNIEnv * env,const void* in, jsize index)->jobject{
                                              const auto& element = reinterpret_cast<const NativeType*>(in)->begin()[index];
                                              return qtjambi_cast<jobject,decltype(element), Args...>::cast(element, arg_pointer<Args>::deref(args, env)...);
                                          }
                                        );
    }

    static auto NativePointerArray(const NativeType* container, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        return new PersistentConstObjectPointerArray<T>(env, container->begin(), jsize(container->size()),
                                                        [args...](JNIEnv * env,const T& in)->jobject{
                                                            return qtjambi_cast<jobject,T, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                        });
    }

    static auto JArrayPointer(JniType in, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        PersistentJConstObjectArrayPointer<T>* data = nullptr;
        if (QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            data = new PersistentJConstObjectArrayPointer<T>(env, jobjectArray(in), [args...](T& d,JNIEnv * env, jobject obj){
                d = qtjambi_cast<T,jobject, Args...>::cast(obj, arg_pointer<Args>::deref(args, env)...);
            });
        }
        return data;
    }
};

template<typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<jobjectArray, T, false, JniType, NativeType, Args...>{

    static jobjectArray toJavaArray(const NativeType* container, typename arg_pointer<Args>::type... args){
        if(!container)
            return nullptr;
        if constexpr(cast_var_args<Args...>::hasScope){
            auto array = NativePointerArray(container, args...);
            cast_var_args<Args...>::scope(args...).addDeletion(array);
            auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
            return array->array(env);
        }
        return value_range_converter<jobjectArray, T, true, JniType, NativeType, Args...>::toJavaArray(container, args...);
    }

    static auto NativePointerArray(NativeType* container, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        return new PersistentObjectPointerArray<T>(env, container->begin(), jsize(container->size()),
                                                   [args...](JNIEnv * env,const T& in)->jobject{
                                                       return qtjambi_cast<jobject,const T&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                   },
                                                   [args...](T& d,JNIEnv * env, jobject obj){
                                                       d = qtjambi_cast<T,jobject, Args...>::cast(obj, arg_pointer<Args>::deref(args, env)...);
                                                   });
    }

    static auto JArrayPointer(JniType in, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        PersistentJObjectArrayPointer<T>* data = nullptr;
        if (QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            data = new PersistentJObjectArrayPointer<T>(env, jobjectArray(in), [args...](T& d,JNIEnv * env, jobject obj){
                                                            d = qtjambi_cast<T,jobject, Args...>::cast(obj, arg_pointer<Args>::deref(args, env)...);
                                                        },
                                                        [args...](JNIEnv * env,const T& in)->jobject{
                                                            return qtjambi_cast<jobject,const T&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                        });
        }
        return data;
    }
};

//template from any std::initializer_list to jobject

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      std::initializer_list, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef std::initializer_list<T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;
    typedef typename std::remove_cv<T>::type T_noconst;
    typedef typename jni_type_decider<T_noconst>::JArrayType TArray;
    typedef typename jni_type<TArray>::ElementType ElementType;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            return value_range_converter<TArray, T, true, JniType, NativeType_c, Args...>::toJavaArray(ref_ptr<is_pointer, NativeType_c>::ref(in), arg_pointer<Args>::ref(args)...);
        }else{
            Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<T> &");
            Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<T> *");
            return value_range_converter<TArray, T, true, JniType, NativeType, Args...>::toNativeContainer(in, -1, arg_pointer<Args>::ref(args)...);
        }
    }
};

#endif //defined(_INITIALIZER_LIST_)

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

//template from any span to jobject

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

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, std::size_t E, template<typename,std::size_t> typename Span, bool t_is_const, typename... Args>
struct qtjambi_jobject_span_cast{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type Tconst;
    typedef Span<Tconst,E> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<std::is_pointer<T>::value, T, typename std::add_const<T>::type>::type T_const;
    typedef typename jni_type_decider<T>::JArrayType TArray;
    typedef typename jni_type<TArray>::ElementType ElementType;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if constexpr(is_jni_array_type<JniType>::value){
                return value_range_converter<TArray, T, t_is_const, JniType, NativeType_c, Args...>::toJavaArray(ref_ptr<is_pointer, NativeType_c>::ref(in), arg_pointer<Args>::ref(args)...);
            }else{
                auto env = cast_var_args<Args...>::env(args...);
                if constexpr(t_is_const){
                    const void * spanPtr;
                    if constexpr(is_pointer){
                        if(!in)
                            return nullptr;
                        spanPtr = in;
                    }else
                        spanPtr = &in;
                    return QtJambiAPI::convertQSpanFromQListToJavaObject(env,
                                                                         spanPtr,
                                                                         [](const void* ptr) -> void* {
                                                                             const Span<Tconst,E>& _span = *reinterpret_cast<const Span<Tconst,E>*>(ptr);
                                                                             return new QList<T>(_span.begin(), _span.end());
                                                                         },
                                                                         QtJambiPrivate::DeleteContainer<QList,T>::function,
                                                                         QtJambiPrivate::QListAccess<T>::newInstance(),
                                                                         t_is_const);
                }else{
                    QtJambiNativeID owner{InvalidNativeID};
                    if constexpr(cast_var_args<Args...>::hasScope){
                        owner = cast_var_args<Args...>::scope(args...).relatedNativeID();
                    }
                    if constexpr(is_pointer)
                        return in ? QtJambiAPI::convertQSpanToJavaObject(env,
                                                                         owner,
                                                                         QtJambiPrivate::QSpanAccess<Tconst,E>::newInstance(),
                                                                         in->begin(),
                                                                         jlong(in->size())
                                                                         ) : nullptr;
                    else
                        return QtJambiAPI::convertQSpanToJavaObject(env,
                                                                owner,
                                                                QtJambiPrivate::QSpanAccess<Tconst,E>::newInstance(),
                                                                in.begin(),
                                                                jlong(in.size())
                                                                );
                }
            }
        }else{
            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast to non-const QSpan<T,E>&");
            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast to non-const QSpan<T,E>*");
            if constexpr(is_pointer){
                if(!in)
                    return nullptr;
            }
            if constexpr(is_jni_array_type<JniType>::value){
                typedef Span<Tconst,q20::dynamic_extent> UnlimitedSpan;
                if constexpr(E==q20::dynamic_extent){
                    UnlimitedSpan span = value_range_converter<TArray, T, t_is_const, JniType, UnlimitedSpan, Args...>::toNativeContainer(in, -1, arg_pointer<Args>::ref(args)...);
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(span), args...);
                }else{
                    UnlimitedSpan span = value_range_converter<TArray, T, t_is_const, JniType, UnlimitedSpan, Args...>::toNativeContainer(in, jsize(E), arg_pointer<Args>::ref(args)...);
                    if constexpr(is_pointer || is_reference){
                        NativeType* result = new NativeType(span.template first<E>());
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(result, args...);
                    }else{
                        return span.template first<E>();
                    }
                }
            }else{
                auto env = cast_var_args<Args...>::env(args...);
                if constexpr(E==q20::dynamic_extent){
                    NativeType result;
                    if(QtJambiAPI::isQSpanObject(env, in)){
                        QPair<void*,jlong> data = QtJambiAPI::fromQSpanObject(env, in, t_is_const, QMetaType::fromType<T>());
                        if(data.first && data.second>0){
                            if constexpr (cast_var_args<Args...>::hasScope && !t_is_const){
                                if constexpr (std::is_copy_constructible_v<decltype(env)>){
                                    cast_var_args<Args...>::scope(args...).addFinalAction([env, in](){QtJambiAPI::commitQSpanObject(env, in);});
                                }else{
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in = JObjectWrapper(env, in)](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in.object(env));
                                    });
#else
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in);
                                    });
#endif
                                }
                            }
                            result = NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), qsizetype(data.second));
                        }
                    }else if(in){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(QtJambiAPI::getObjectClassName(env, in), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
                }else{
                    if(QtJambiAPI::isQSpanObject(env, in)){
                        QPair<void*,jlong> data = QtJambiAPI::fromQSpanObject(env, in, t_is_const, QMetaType::fromType<T>());
                        if(data.first && data.second>0){
                            if constexpr (cast_var_args<Args...>::hasScope && !t_is_const){
                                if constexpr (std::is_copy_constructible_v<decltype(env)>){
                                    cast_var_args<Args...>::scope(args...).addFinalAction([env, in](){QtJambiAPI::commitQSpanObject(env, in);});
                                }else{
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in = JObjectWrapper(env, in)](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in.object(env));
                                    });
#else
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in);
                                    });
#endif
                                }
                            }
                            if(data.second<E){
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast span of size %1 to %2").arg(QString::number(data.second), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            }
                            if constexpr(is_pointer || is_reference){
                                NativeType* result = new NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), E);
                                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(result, args...);
                            }else{
                                return NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), E);
                            }
                        }
                    }else{
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
        }
    }
};

#endif // QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_TEMPLATE1_H
