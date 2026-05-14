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

#ifndef QTJAMBICORE_CAST_H
#define QTJAMBICORE_CAST_H

#include <QtCore/QCborStreamReader>
#include <QtCore/QUrl>
#include <QtJambi/Cast>
#include <QtJambi/SmartPointerCast>

namespace QtJambiPrivate {
template<bool forward,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast;

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QCborStreamReader::StringResult, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QCborStreamReader::StringResult<T> NativeType;
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
            if constexpr (std::is_same_v<QVariant, T>){
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
            if constexpr (std::is_same_v<QVariant, T>){
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

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                      jobject,
                                      QUrlTwoFlags, is_pointer, is_const, is_reference, is_rvalue,
                                      QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption, Args...>{
    typedef QUrl::FormattingOptions NativeType;
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
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &_in, typeid(QUrl::FormattingOptions));
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions& without scope");
                Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions* without scope");
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
                            QtJambiAPI::setFlagsValue(env, o, jint(int(*result)));
                        }
                        delete result;
                    });
#else
                    cast_var_args<Args...>::scope(args...).addFinalAction([result, env, in](){
                        QtJambiAPI::setFlagsValue(env, in, jint(int(*result)));
                        delete result;
                    });
#endif //defined(QTJAMBI_JOBJECTWRAPPER_H)
                }

                if constexpr(is_pointer){
                    return result;
                }else{
                    return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
                }
            }else{
                NativeType result;
                if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to QUrl::FormattingOptions").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")) QTJAMBI_STACKTRACEINFO );
                }
                return result;
            }
        }
    }
};

template<bool forward,
         typename ArithmeticType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_arithmetic_container2_cast;

template<bool forward, typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_container2_cast<forward,
                                          ArithmeticType,
                                          QUrlTwoFlags, is_pointer, is_const, is_reference, is_rvalue, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption, Args...>{
    typedef QUrl::FormattingOptions NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, ArithmeticType> In;
    typedef std::conditional_t<forward, ArithmeticType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if constexpr(is_pointer){
                return ArithmeticType(*in);
            }else{
                return ArithmeticType(in);
            }
        }else{
            Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions reference without scope");
            Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions pointer without scope");
            if constexpr(is_pointer || is_reference){
                QUrl::FormattingOptions* result = new QUrl::FormattingOptions(int(in));
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else{
                    return *result;
                }
            }else{
                return QUrl::FormattingOptions(int(in));
            }
        }
    }
};

template<bool forward,
         typename EnumType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_enum_container2_cast;

template<bool forward, typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_container2_cast<forward,
                                    EnumType,
                                    QUrlTwoFlags, is_pointer, is_const, is_reference, is_rvalue, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption, Args...>{
    typedef QUrl::FormattingOptions NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef NativeType_cr NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, EnumType> In;
    typedef std::conditional_t<forward, EnumType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            return EnumType(int(deref_ptr<is_pointer,const NativeType>::deref(in)));
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions*");
                NativeType* result = new NativeType(in);
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else{
                    auto env = cast_var_args<Args...>::env(args...);
                    return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
                }
            }else{
                return NativeType(int(in));
            }
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBICORE_CAST_H
