/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef FUTURE_P_H
#define FUTURE_P_H

#include <QtJambi/CoreAPI>
#include <QtJambi/JObjectWrapper>
#include "utils_p.h"

inline auto future_createValueFunction(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](const QVariant& value)->QVariant{
        if(JniEnvironment env{300}){
            jobject _value = qtjambi_cast<jobject>(env, value);
            jobject result = Java::QtCore::QFuture$Function::apply(env, _function, _value);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

inline auto future_createVoidFunction(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)]()->QVariant{
        if(JniEnvironment env{300}){
            jobject result = Java::QtCore::QFuture$Function::apply(env, _function, nullptr);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

inline auto future_createValueConsumer(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](const QVariant& value){
        if(JniEnvironment env{300}){
            jobject _value = qtjambi_cast<jobject>(env, value);
            Java::QtCore::QFuture$Consumer::accept(env, _function, _value);
        }
    };
}

inline auto future_createVoidConsumer(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](){
        if(JniEnvironment env{300}){
            Java::QtCore::QFuture$Consumer::accept(env, _function, nullptr);
        }
    };
}

inline auto future_createVoidRunnable(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](){
        if(JniEnvironment env{300}){
            Java::QtCore::QFuture$Runnable::run(env, _function);
        }
    };
}

inline auto future_createValueRunnable(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](const QVariant&){
        if(JniEnvironment env{300}){
            Java::QtCore::QFuture$Runnable::run(env, _function);
        }
    };
}

inline auto future_createVoidSupplier(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](){
        if(JniEnvironment env{300}){
            jobject result = Java::QtCore::QFuture$Supplier::get(env, _function);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

inline auto future_createValueSupplier(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](const QVariant&){
        if(JniEnvironment env{300}){
            jobject result = Java::QtCore::QFuture$Supplier::get(env, _function);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

template<class T>
auto future_createFutureFunction(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](QFuture<T> parentFuture)->QVariant{
        if(JniEnvironment env{300}){
            jobject _parentFuture = qtjambi_cast<jobject>(env, std::move(parentFuture));
            jobject result = Java::QtCore::QFuture$FutureFunction::apply(env, _function, _parentFuture);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

template<class T>
auto future_createFutureConsumer(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](QFuture<T> parentFuture){
        if(JniEnvironment env{300}){
            jobject _parentFuture = qtjambi_cast<jobject>(env, std::move(parentFuture));
            Java::QtCore::QFuture$FutureConsumer::accept(env, _function, _parentFuture);
        }
    };
}

inline auto future_createExceptionHandlerSupplier(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)]()->QVariant{
        if(JniEnvironment env{300}){
            jobject result = Java::QtCore::QFuture$Supplier::get(env, _function);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

inline auto future_createExceptionHandlerFunction(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](const std::exception& exception)->QVariant{
        if(JniEnvironment env{300}){
            jthrowable _exception = nullptr;
            const std::exception* exceptionPtr = &exception;
            const std::type_info* ti = QtJambiPrivate::CheckPointer<std::exception>::trySupplyType(exceptionPtr);// avoids crash when RTTI is missing
            if(ti){
                if(const JavaException* javaException = dynamic_cast<const JavaException*>(&exception)){
                    _exception = javaException->throwable(env);
                }else if(const QException* qException = dynamic_cast<const QException*>(&exception)){
                    _exception = Java::QtCore::QException::newInstanceWithMessage(env, qException->what());
                }else{
                    _exception = Java::Runtime::Error::newInstanceWithMessage(env, exception.what());
                }
            }else{
                QTJAMBI_TRY{
                    throw exception;
                }QTJAMBI_CATCH(const JavaException& javaException){
                    _exception = javaException.throwable(env);
                }QTJAMBI_CATCH(const QException& qexception){
                    _exception = Java::QtCore::QException::newInstanceWithMessage(env, qexception.what());
                }QTJAMBI_CATCH(...){
                    _exception = Java::Runtime::Error::newInstanceWithMessage(env, exception.what());
                }QTJAMBI_TRY_END
            }
            jobject result = Java::QtCore::QFuture$Function::apply(env, _function, _exception);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

inline auto futurevoid_createExceptionHandlerConsumer(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](const std::exception& exception){
        if(JniEnvironment env{300}){
            jthrowable _exception = nullptr;
            const std::exception* exceptionPtr = &exception;
            const std::type_info* ti = QtJambiPrivate::CheckPointer<std::exception>::trySupplyType(exceptionPtr);// avoids crash when RTTI is missing
            if(ti){
                if(const JavaException* javaException = dynamic_cast<const JavaException*>(&exception)){
                    _exception = javaException->throwable(env);
                }else if(const QException* qException = dynamic_cast<const QException*>(&exception)){
                    _exception = Java::QtCore::QException::newInstanceWithMessage(env, qException->what());
                }else{
                    _exception = Java::Runtime::Error::newInstanceWithMessage(env, exception.what());
                }
            }else{
                QTJAMBI_TRY{
                    throw exception;
                }QTJAMBI_CATCH(const JavaException& javaException){
                    _exception = javaException.throwable(env);
                }QTJAMBI_CATCH(const QException& qexception){
                    _exception = Java::QtCore::QException::newInstanceWithMessage(env, qexception.what());
                }QTJAMBI_CATCH(...){
                    _exception = Java::Runtime::Error::newInstanceWithMessage(env, exception.what());
                }QTJAMBI_TRY_END
            }
            Java::QtCore::QFuture$Consumer::accept(env, _function, _exception);
        }
    };
}

inline auto futurevoid_createExceptionHandlerSupplier(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](){
        if(JniEnvironment env{300}){
            Java::QtCore::QFuture$Supplier::get(env, _function);
        }
    };
}

inline auto futurevoid_createExceptionHandlerFunction(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](const std::exception& exception){
        if(JniEnvironment env{300}){
            jthrowable _exception = nullptr;
            const std::exception* exceptionPtr = &exception;
            const std::type_info* ti = QtJambiPrivate::CheckPointer<std::exception>::trySupplyType(exceptionPtr);// avoids crash when RTTI is missing
            if(ti){
                if(const JavaException* javaException = dynamic_cast<const JavaException*>(&exception)){
                    _exception = javaException->throwable(env);
                }else if(const QException* qException = dynamic_cast<const QException*>(&exception)){
                    _exception = Java::QtCore::QException::newInstanceWithMessage(env, qException->what());
                }else{
                    _exception = Java::Runtime::Error::newInstanceWithMessage(env, exception.what());
                }
            }else{
                QTJAMBI_TRY{
                    throw exception;
                }QTJAMBI_CATCH(const JavaException& javaException){
                    _exception = javaException.throwable(env);
                }QTJAMBI_CATCH(const QException& qexception){
                    _exception = Java::QtCore::QException::newInstanceWithMessage(env, qexception.what());
                }QTJAMBI_CATCH(...){
                    _exception = Java::Runtime::Error::newInstanceWithMessage(env, exception.what());
                }QTJAMBI_TRY_END
            }
            Java::QtCore::QFuture$Function::apply(env, _function, _exception);
        }
    };
}

inline auto future_createTypedExceptionHandlerFunction(JNIEnv * env, jclass exceptionType, jobject function){
    return [_function = JObjectWrapper(env, function), exceptionType = JavaAPI::toGlobalReference(env, exceptionType)](const JavaException& exception)->QVariant{
        if(JniEnvironment env{300}){
            jthrowable _exception = exception.throwable(env);
            if(_exception && env->IsInstanceOf(_exception, exceptionType)){
                jobject result = Java::QtCore::QFuture$Function::apply(env, _function, _exception);
                return qtjambi_cast<QVariant>(env, result);
            }else{
                throw exception;
            }
        }
        return QVariant();
    };
}

inline auto future_createTypedExceptionHandlerSupplier(JNIEnv * env, jclass exceptionType, jobject function){
    return [_function = JObjectWrapper(env, function), exceptionType = JavaAPI::toGlobalReference(env, exceptionType)](const JavaException& exception)->QVariant{
        if(JniEnvironment env{300}){
            jthrowable _exception = exception.throwable(env);
            if(_exception && env->IsInstanceOf(_exception, exceptionType)){
                jobject result = Java::QtCore::QFuture$Supplier::get(env, _function);
                return qtjambi_cast<QVariant>(env, result);
            }else{
                throw exception;
            }
        }
        return QVariant();
    };
}

inline auto futurevoid_createExceptionHandlerRunnable(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](){
        if(JniEnvironment env{300}){
            Java::QtCore::QFuture$Runnable::run(env, _function);
        }
    };
}

inline auto futurevoid_createTypedExceptionHandlerConsumer(JNIEnv * env, jclass exceptionType, jobject function){
    return [_function = JObjectWrapper(env, function), exceptionType = JavaAPI::toGlobalReference(env, exceptionType)](const JavaException& exception){
        if(JniEnvironment env{300}){
            jthrowable _exception = exception.throwable(env);
            if(_exception && env->IsInstanceOf(_exception, exceptionType)){
                Java::QtCore::QFuture$Consumer::accept(env, _function, _exception);
            }else{
                throw exception;
            }
        }
    };
}

inline auto futurevoid_createTypedExceptionHandlerSupplier(JNIEnv * env, jclass exceptionType, jobject function){
    return [_function = JObjectWrapper(env, function), exceptionType = JavaAPI::toGlobalReference(env, exceptionType)](const JavaException& exception){
        if(JniEnvironment env{300}){
            jthrowable _exception = exception.throwable(env);
            if(_exception && env->IsInstanceOf(_exception, exceptionType)){
                Java::QtCore::QFuture$Supplier::get(env, _function);
            }else{
                throw exception;
            }
        }
    };
}

inline auto futurevoid_createTypedExceptionHandlerRunnable(JNIEnv * env, jclass exceptionType, jobject function){
    return [_function = JObjectWrapper(env, function), exceptionType = JavaAPI::toGlobalReference(env, exceptionType)](const JavaException& exception){
        if(JniEnvironment env{300}){
            jthrowable _exception = exception.throwable(env);
            if(_exception && env->IsInstanceOf(_exception, exceptionType)){
                Java::QtCore::QFuture$Runnable::run(env, _function);
            }else{
                throw exception;
            }
        }
    };
}

inline auto futurevoid_createTypedExceptionHandlerFunction(JNIEnv * env, jclass exceptionType, jobject function){
    return [_function = JObjectWrapper(env, function), exceptionType = JavaAPI::toGlobalReference(env, exceptionType)](const JavaException& exception){
        if(JniEnvironment env{300}){
            jthrowable _exception = exception.throwable(env);
            if(_exception && env->IsInstanceOf(_exception, exceptionType)){
                Java::QtCore::QFuture$Function::apply(env, _function, _exception);
            }else{
                throw exception;
            }
        }
    };
}

inline auto future_createCancelHandlerSupplier(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)]()->QVariant{
        if(JniEnvironment env{300}){
            jobject result = Java::QtCore::QFuture$Supplier::get(env, _function);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

inline auto futurevoid_createCancelHandlerRunnable(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](){
        if(JniEnvironment env{300}){
            Java::QtCore::QFuture$Runnable::run(env, _function);
        }
    };
}

inline auto futurevoid_createCancelHandlerSupplier(JNIEnv * env, jobject function){
    return [_function = JObjectWrapper(env, function)](){
        if(JniEnvironment env{300}){
            Java::QtCore::QFuture$Supplier::get(env, _function);
        }
    };
}

namespace QtPrivate
{

template<>
struct MetaTypeQFutureHelper<QFuture<QVariant>>
{
    static bool registerConverter() {
        if(QMetaType::hasRegisteredConverterFunction<QFuture<QVariant>, QFuture<void>>())
            return false;
        return QMetaType::registerConverter<QFuture<QVariant>, QFuture<void>>(
            [](const QFuture<QVariant> &future) { return QFuture<void>(future); });
    }
};

}  // namespace QtPrivate

#endif // FUTURE_P_H
