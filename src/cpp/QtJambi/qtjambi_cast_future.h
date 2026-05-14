/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_FUTURE_H
#define QTJAMBI_CAST_FUTURE_H

#include <QtCore/QFuture>
#include <QtCore/QFutureInterface>
#include <QtCore/QFutureWatcher>
#include <QtCore/QPromise>
#include "qtjambi_cast.h"
#include "qtjambiapi.h"

namespace QtJambiAPI{
QTJAMBI_EXPORT QFutureInterface<void>* asVoidFutureInterface(QFutureInterfaceBase* base);
QTJAMBI_EXPORT const QFutureInterface<void>* asVoidFutureInterface(const QFutureInterfaceBase* base);
QTJAMBI_EXPORT QFutureInterface<QVariant>* asVariantFutureInterface(QFutureInterfaceBase* base);
QTJAMBI_EXPORT const QFutureInterface<QVariant>* asVariantFutureInterface(const QFutureInterfaceBase* base);
QTJAMBI_EXPORT bool isVoidFutureInterface(const QFutureInterfaceBase* base);
QTJAMBI_EXPORT bool isVariantFutureInterface(const QFutureInterfaceBase* base);

typedef void(*ResultTranslator)(const QtPrivate::ResultStoreBase &, QtPrivate::ResultStoreBase &, int, int);

typedef bool(*FutureInterfaceTypeTest)(const QFutureInterfaceBase*);

QTJAMBI_EXPORT QFutureInterfaceBase* translateQFutureInterface(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, ResultTranslator resultTranslator, ResultTranslator resultRetranslator, FutureInterfaceTypeTest futureInterfaceTypeTest);

typedef void(*FutureSetter)(JNIEnv *, QFutureWatcherBase*, jobject);
typedef jobject(*FutureResult)(JNIEnv *, QFutureWatcherBase*, int);
typedef jobject(*FutureGetter)(JNIEnv *, QFutureWatcherBase*);
typedef std::unique_ptr<QFutureInterfaceBase>(*FutureInterfaceGetter)(QFutureWatcherBase*);
QTJAMBI_EXPORT jobject convertQFutureWatcherToJavaObject(JNIEnv* env, const QFutureWatcherBase* futureWatcher,
                                                         FutureSetter futureSetter, FutureResult futureResult, FutureGetter futureGetter, FutureInterfaceGetter futureInterfaceGetter);

QTJAMBI_EXPORT std::unique_ptr<QFutureInterfaceBase> getQFutureInterfaceFromQFutureWatcher(JNIEnv* env, jobject future);
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
void copy_future_interface_results(const QtPrivate::ResultStoreBase & sourceStoreBase, QtPrivate::ResultStoreBase & targetBase, int beginIndex, int count){
    if constexpr((!is_copy_constructible_v<TargetType>
                   && is_move_constructible_v<TargetType>)
                  || (!is_copy_constructible_v<SourceType>
                      && is_move_constructible_v<SourceType>)){
        if(JniEnvironment env{200}){
            for(int i=0; i<count; ++i){
                jobject obj = ::qtjambi_cast<jobject>(env, sourceStoreBase.resultAt(beginIndex + i).template value<SourceType>());
                if constexpr(std::is_same_v<SourceType,QVariant>){
                    TargetType* n = ::qtjambi_cast<TargetType*>(env, obj);
                    if(n)
                        targetBase.moveResult<TargetType>(beginIndex + i, std::move(*n));
                    else
                        targetBase.addResult(beginIndex + i, nullptr);
                }else{
                    targetBase.moveResult<TargetType>(beginIndex + i, ::qtjambi_cast<TargetType>(env, obj));
                }
            }
        }
    }else{
        for(int i=0; i<count; ++i){
            targetBase.moveResult<TargetType>(beginIndex + i, convert_variant<TargetType,SourceType>(sourceStoreBase.resultAt(beginIndex + i).template value<SourceType>()));
        }
    }
}

template<typename TargetType>
struct FutureInterfaceTypeTest{
    static bool test(const QFutureInterfaceBase* base){
        if(dynamic_cast<const QFutureInterface<TargetType>*>(base))
            return true;
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
        QByteArray baseType = QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base));
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
    Q_STATIC_ASSERT_X((is_copy_constructible_v<TargetType> || is_move_constructible_v<TargetType>)
                      && is_destructible_v<TargetType>, "Cannot cast QFutureInterface<T> with non-constructible and non-movable result type T.");
    Q_STATIC_ASSERT_X((is_copy_constructible_v<SourceType> || is_move_constructible_v<SourceType>)
                          && is_destructible_v<SourceType>, "Cannot cast QFutureInterface<T> with non-constructible and non-movable result type T.");
    QFutureInterface<TargetType> result;
    if(future){
        if(QFutureInterfaceBase* availableResult = QtJambiAPI::translateQFutureInterface(
                QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<SourceType>(*future)),
                QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<TargetType>(result)),
                &copy_future_interface_results<TargetType,SourceType>,
                &copy_future_interface_results<SourceType,TargetType>,
                FutureInterfaceTypeTest<TargetType>::fn)){
            if constexpr (std::is_same_v<void, TargetType>){
                if(QFutureInterface<TargetType>* newResult = QtJambiAPI::asVoidFutureInterface(availableResult)){
                    return *newResult;
                }else{
                    QByteArray baseType = QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(availableResult));
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1<void> to %2.").arg(translatedType, baseType) QTJAMBI_STACKTRACEINFO );
                }
            }else if constexpr (std::is_same_v<QVariant, TargetType>){
                if(QFutureInterface<TargetType>* newResult = QtJambiAPI::asVariantFutureInterface(availableResult)){
                    return *newResult;
                }else{
                    QByteArray baseType = QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(availableResult));
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1<Variant> to %2.").arg(translatedType, baseType) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                if(QFutureInterface<TargetType>* newResult = dynamic_cast<QFutureInterface<TargetType>*>(availableResult)){
                    return *newResult;
                }else{
                    QByteArray baseType = QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(availableResult));
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
         template<typename T> class Future, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_future_cast;

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_future_cast<forward,
                                   QFutureInterface, is_pointer, is_const, is_reference, is_rvalue,
                                   T, Args...>{
    typedef QFutureInterface<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr (std::is_same_v<void, T> || std::is_same_v<QVariant, T>){
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
            QFutureInterfaceBase* base = qtjambi_cast_with_args<QFutureInterfaceBase*>(in, std::forward<Args>(args)...);
            if constexpr (std::is_same_v<void, T>){
                if(QFutureInterface<void>* fi = QtJambiAPI::asVoidFutureInterface(base)){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(fi, args...);
                }else{
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(NativeType(*base), args...);
                }
            }else if constexpr (std::is_same_v<QVariant, T>){
                if(QFutureInterface<QVariant>* futureInterface = QtJambiAPI::asVariantFutureInterface(base)){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(futureInterface, args...);
                }else if(QtJambiAPI::isVoidFutureInterface(base)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFutureInterface<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }else{
                    QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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
                    QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_future_cast<forward,
                                   QFuture, is_pointer, is_const, is_reference, is_rvalue,
                                   T, Args...>{
    typedef QFuture<T> NativeType;
    typedef QFutureInterface<T> FutureInterfaceT;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr (std::is_same_v<QVariant, T> || std::is_same_v<void, T>){
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
            if constexpr (std::is_same_v<void, T>){
                if(QtJambiAPI::isVoidFutureInterface(base)){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(reinterpret_cast<QFuture<T>*>(future), args...);
                }else{
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(NativeType(base), args...);
                }
            }else if constexpr (std::is_same_v<QVariant, T>){
                if(QtJambiAPI::isVariantFutureInterface(base)){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(reinterpret_cast<QFuture<T>*>(future), args...);
                }else if(QtJambiAPI::isVoidFutureInterface(base)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QFuture<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }else{
                    QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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
                    QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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

template<typename T, typename... Args>
struct FutureResultFactory{
    static jobject futureResult(JNIEnv * env, QFutureWatcherBase* base, int index) {
        QFutureWatcher<T>* watcher = dynamic_cast<QFutureWatcher<T>*>(base);
        QVariant value = QVariant::fromValue<T>(watcher->resultAt(index));
        return qtjambi_cast<jobject>(env, value);
    };
};

template<>
struct FutureResultFactory<void>{
    static constexpr QtJambiAPI::FutureResult futureResult = nullptr;
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_future_cast<forward,
                                   QFutureWatcher, is_pointer, is_const, is_reference, is_rvalue,
                                   T, Args...>{
    typedef QFutureWatcher<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if(NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in)){
                return QtJambiAPI::convertQFutureWatcherToJavaObject(env, _in,
                                                                     [](JNIEnv * env, QFutureWatcherBase* base, jobject future){
                                                                         QFutureWatcher<T>* watcher = static_cast<QFutureWatcher<T>*>(base);
                                                                         QFuture<T> ft = qtjambi_jobject_future_cast<false,QFuture,false,false,false,false,T,JNIEnv*>::cast(future, env);
                                                                         watcher->setFuture(ft);
                                                                     },
                                                                     FutureResultFactory<T>::futureResult,
                                                                     [](JNIEnv * env, QFutureWatcherBase* base) -> jobject {
                                                                         QFutureWatcher<T>* watcher = static_cast<QFutureWatcher<T>*>(base);
                                                                         QFuture<T> ft = watcher->future();
                                                                         return qtjambi_jobject_future_cast<true,QFuture,false,false,false,false,T,JNIEnv*>::cast(ft, env);
                                                                     },
                                                                     [](QFutureWatcherBase* base) -> std::unique_ptr<QFutureInterfaceBase> {
                                                                         QFutureWatcher<T>* watcher = static_cast<QFutureWatcher<T>*>(base);
                                                                         QFuture<T> ft = watcher->future();
                                                                         return std::unique_ptr<QFutureInterfaceBase>{new QFutureInterface<T>(QtPrivate::Continuation<T,bool,bool>::sourceFuture(ft))};
                                                                     }
                                                                     );
            }else return nullptr;
        }else{
            QFutureWatcherBase* watcher = qtjambi_cast_with_args<QFutureWatcherBase*>(in, std::forward<Args>(args)...);
            if constexpr (std::is_same_v<void, T>){
                return ptr2ref<is_reference,NativeType>::value(env, reinterpret_cast<NativeType*>(watcher));
            }else if constexpr (std::is_same_v<QVariant, T>){
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
                            QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base.get())));
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
                        QString watcherType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureWatcherBase>::trySupplyType(watcher)));
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
                            QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base.get())));
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
                        QString watcherType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureWatcherBase>::trySupplyType(watcher)));
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

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_future_cast<forward,
                                   QPromise, is_pointer, is_const, is_reference, is_rvalue,
                                   T, Args...>{
    typedef QPromise<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        Q_STATIC_ASSERT_X(is_reference || is_pointer, "Cannot cast QPromise<T> without reference or pointer");
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if(NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in)){
                if constexpr (std::is_same_v<void, T> || std::is_same_v<QVariant, T>){
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
                if constexpr (std::is_same_v<QVariant, T>){
                    if(QtJambiAPI::isVariantFutureInterface(base)){
                        result = reinterpret_cast<QPromise<T>*>(promise);
                    }else if(QtJambiAPI::isVoidFutureInterface(base)){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast QPromise<void> to %1.").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }else {
                        QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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
                }else if constexpr (std::is_same_v<void, T>){
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
                            QString baseType = QLatin1String(QtJambiAPI::typeName(CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
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

}

extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QPromise<QVariant>&>(JNIEnv *, QPromise<QVariant>&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QPromise<void>&>(JNIEnv *, QPromise<void>&);

extern template QTJAMBI_EXPORT QFuture<QVariant> qtjambi_cast<QFuture<QVariant>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT const QFuture<QVariant>& qtjambi_cast<const QFuture<QVariant>&,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QFuture<QVariant>>(JNIEnv *, QFuture<QVariant>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QFuture<QVariant>&>(JNIEnv *, const QFuture<QVariant>&);
extern template QTJAMBI_EXPORT QFuture<void> qtjambi_cast<QFuture<void>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT const QFuture<void>& qtjambi_cast<const QFuture<void>&,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QFuture<void>>(JNIEnv *, QFuture<void>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QFuture<void>&>(JNIEnv *, const QFuture<void>&);

#ifdef QTJAMBI_JOBJECTWRAPPER_H
extern template QTJAMBI_EXPORT QFuture<JObjectWrapper> qtjambi_cast<QFuture<JObjectWrapper>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT const QFuture<JObjectWrapper>& qtjambi_cast<const QFuture<JObjectWrapper>&,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QFuture<JObjectWrapper>>(JNIEnv *, QFuture<JObjectWrapper>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QFuture<JObjectWrapper>&>(JNIEnv *, const QFuture<JObjectWrapper>&);
#endif

#endif // QTJAMBI_CAST_FUTURE_H
