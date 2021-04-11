#include <QtCore/QIODevice>
#include <QtCore/QFuture>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QModelRoleData>
#endif
#include <QtCore/QVariant>
#include <QtCore/QCalendar>
#include <qtjambi/qtjambi_application.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_thread.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include "qtjambi_core_repository.h"
#include "qtjambi_core_qhashes.h"
#include "qtjambiqfuture.h"
#include <qtjambi/qtjambi_cast.h>

QT_WARNING_DISABLE_GCC("-Wfloat-equal")
QT_WARNING_DISABLE_CLANG("-Wfloat-equal")

extern "C" JNIEXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QAbstractItemModel_setData_1native)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jint row, jint col, jobject value, jint role)
{
    QAbstractItemModel *__qt_this = qtjambi_object_from_nativeId<QAbstractItemModel>(__this_nativeId);
    qtjambi_check_resource(__jni_env, __qt_this);
    QVariant qt_value = qtjambi_to_qvariant(__jni_env, value);
    QModelIndex index = __qt_this->index(row, col, QModelIndex());
    return __qt_this->setData(index, qt_value, role);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QAbstractItemModel_data_1native)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jint row, jint col, jint role)
{
    QAbstractItemModel *__qt_this = qtjambi_object_from_nativeId<QAbstractItemModel>(__this_nativeId);
    qtjambi_check_resource(__jni_env, __qt_this);
    QModelIndex index = __qt_this->index(row, col, QModelIndex());
    return qtjambi_from_qvariant(__jni_env, __qt_this->data(index, role));
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCalendar_unspecified__)
(JNIEnv *, jclass)
{
    return jint(QCalendar::Unspecified);
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_bin)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::bin;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_oct)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::oct;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_dec)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::dec;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_hex)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::hex;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_showbase)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::showbase;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_forcesign)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::forcesign;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_forcepoint)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::forcepoint;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_noshowbase)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::noshowbase;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_noforcesign)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::noforcesign;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_noforcepoint)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::noforcepoint;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_uppercasebase)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::uppercasebase;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_uppercasedigits)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::uppercasedigits;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_lowercasebase)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::lowercasebase;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_lowercasedigits)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::lowercasedigits;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_fixed)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::fixed;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_scientific)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::scientific;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_left)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::left;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_right)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::right;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_center)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::center;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_endl)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::endl;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_flush)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::flush;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_reset)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::reset;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_bom)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::bom;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_ws)
(JNIEnv *env, jclass, QtJambiNativeID debugId)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::ws;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_setFieldWidth)
(JNIEnv *env, jclass, QtJambiNativeID debugId, jint width)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << qSetFieldWidth(width);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_setRealNumberPrecision)
(JNIEnv *env, jclass, QtJambiNativeID debugId, jint precision)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << qSetRealNumberPrecision(precision);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_setPadChar)
(JNIEnv *env, jclass, QtJambiNativeID debugId, jchar ch)
{
    try{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << qSetPadChar(ch);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_debugStream)
(JNIEnv *env, jclass, QtJambiNativeID debugId, QtJambiNativeID metaTypeId, jobject value)
{
    QDebug& debug = qtjambi_object_from_nativeId_deref<QDebug>(env, debugId);
    const QMetaType& metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
    try{
        bool success = false;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(metaType.hasRegisteredDebugStreamOperator()){
            QVariant variant = qtjambi_cast<QVariant>(env, value);
            if(variant.metaType()==metaType || variant.convert(metaType)){
                success = QMetaType(metaType).debugStream(debug, variant.constData());
            }
        }
#else
        if(QMetaType::hasRegisteredDebugStreamOperator(metaType.id())){
            QVariant variant = qtjambi_cast<QVariant>(env, value);
            if(variant.userType()==metaType.id() || variant.convert(metaType.id())){
                success = QMetaType::debugStream(debug, variant.constData(), metaType.id());
            }
        }else if(const QtPrivate::AbstractDebugStreamFunction * fct = qtjambi_registered_debugstream_operator(metaType.id())){
            QVariant variant = qtjambi_cast<QVariant>(env, value);
            if(variant.userType()==metaType.id() || variant.convert(metaType.id())){
                fct->stream(fct, debug, variant.constData());
                success = true;
            }
        }
#endif
        if(!success){
            if(value)
                debug << qtjambi_to_qstring(env, Java::Runtime::Object::toString(env, value));
            else
                debug << nullptr;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_qInstallMessageHandler)
(JNIEnv *env, jclass, jobject supportedMessageTypes, jobject handler)
{
    try{
        env->PushLocalFrame(500);
        return qtjambi_install_message_handler(env, supportedMessageTypes, handler);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_showCMessageFromSupplier)
(JNIEnv *env, jclass, jint messageType, QtJambiNativeID categoryId, jobject messageSupplier)
{
#if !defined(QT_NO_DEBUG_OUTPUT)
    try{
        QLoggingCategory& category = qtjambi_object_from_nativeId_deref<QLoggingCategory>(env, categoryId);
        jstring message = Java::QtCore::QLogging$Supplier::get(env, messageSupplier);
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 4);
        jclass declaringClass = Java::QtJambi::QtJambiInternal$InvocationInfo::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::QtJambiInternal$InvocationInfo::methodName(env, invocationInfo);
        line = Java::QtJambi::QtJambiInternal$InvocationInfo::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QMessageLogger logger(_className, line, _method, category.categoryName());
        switch(messageType){
        case QtMsgType::QtWarningMsg:
            logger.warning(category, "%s", _message);
            break;
        case QtMsgType::QtCriticalMsg:
            logger.critical(category, "%s", _message);
            break;
        case QtMsgType::QtDebugMsg:
            logger.debug(category, "%s", _message);
            break;
        case QtMsgType::QtInfoMsg:
            logger.info(category, "%s", _message);
            break;
        case QtMsgType::QtFatalMsg:
            logger.fatal("%s", _message);
            break;
        }
#if !defined (QT_NO_DEBUG)
        env->ReleaseStringUTFChars(method, _method);
        env->ReleaseStringUTFChars(className, _className);
#endif
        env->ReleaseStringUTFChars(message, _message);
        qtjambi_throw_java_exception(env)
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
    Q_UNUSED(messageType)
    Q_UNUSED(message)
    Q_UNUSED(category)
#endif
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_getDebug)
(JNIEnv *env, jclass, jint messageType)
{
    jobject result = nullptr;
#if !defined(QT_NO_DEBUG_OUTPUT)
    try{
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 4);
        jclass declaringClass = Java::QtJambi::QtJambiInternal$InvocationInfo::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::QtJambiInternal$InvocationInfo::methodName(env, invocationInfo);
        line = Java::QtJambi::QtJambiInternal$InvocationInfo::lineNumber(env, invocationInfo);
        jobject _data = qtjambi_cast<jobject>(env, QByteArray());
        QByteArray* data = qtjambi_cast<QByteArray*>(env, _data);
        _method = env->GetStringUTFChars(method, nullptr);
        data->append(_method);
        data->append('\0');
        env->ReleaseStringUTFChars(method, _method);
        auto size = data->size();
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
        data->append(_className);
        env->ReleaseStringUTFChars(className, _className);
        _method = data->constData();
        _className = data->constData()+size;
#endif
        QMessageLogger logger(_className, line, _method);
        switch(messageType){
        case QtMsgType::QtWarningMsg:
            result = qtjambi_cast<jobject>(env, logger.warning());
            break;
        case QtMsgType::QtCriticalMsg:
            result = qtjambi_cast<jobject>(env, logger.critical());
            break;
        case QtMsgType::QtDebugMsg:
            result = qtjambi_cast<jobject>(env, logger.debug());
            break;
        case QtMsgType::QtInfoMsg:
            result = qtjambi_cast<jobject>(env, logger.info());
            break;
        default:
            break;
        }
#if !defined (QT_NO_DEBUG)
        if(result)
            Java::QtCore::QDebug::set___rcDevice(env, result, _data);
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
    Q_UNUSED(messageType)
    Q_UNUSED(category)
#endif
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_getCDebug)
(JNIEnv *env, jclass, jint messageType, QtJambiNativeID categoryId)
{
    jobject result = nullptr;
#if !defined(QT_NO_DEBUG_OUTPUT)
    try{
        QLoggingCategory& category = qtjambi_object_from_nativeId_deref<QLoggingCategory>(env, categoryId);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 4);
        jclass declaringClass = Java::QtJambi::QtJambiInternal$InvocationInfo::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::QtJambiInternal$InvocationInfo::methodName(env, invocationInfo);
        line = Java::QtJambi::QtJambiInternal$InvocationInfo::lineNumber(env, invocationInfo);
        jobject _data = qtjambi_cast<jobject>(env, QByteArray());
        QByteArray* data = qtjambi_cast<QByteArray*>(env, _data);
        _method = env->GetStringUTFChars(method, nullptr);
        data->append(_method);
        data->append('\0');
        env->ReleaseStringUTFChars(method, _method);
        auto size = data->size();
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
        data->append(_className);
        env->ReleaseStringUTFChars(className, _className);
        _method = data->constData();
        _className = data->constData()+size;
#endif
        QMessageLogger logger(_className, line, _method, category.categoryName());
        switch(messageType){
        case QtMsgType::QtWarningMsg:
            result = qtjambi_cast<jobject>(env, logger.warning(category));
            break;
        case QtMsgType::QtCriticalMsg:
            result = qtjambi_cast<jobject>(env, logger.critical(category));
            break;
        case QtMsgType::QtDebugMsg:
            result = qtjambi_cast<jobject>(env, logger.debug(category));
            break;
        case QtMsgType::QtInfoMsg:
            result = qtjambi_cast<jobject>(env, logger.info(category));
            break;
        default:
            break;
        }
#if !defined (QT_NO_DEBUG)
        if(result)
            Java::QtCore::QDebug::set___rcDevice(env, result, _data);
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
    Q_UNUSED(messageType)
    Q_UNUSED(category)
#endif
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_showCMessage)
(JNIEnv *env, jclass, jint messageType, QtJambiNativeID categoryId, jstring message)
{
#if !defined(QT_NO_DEBUG_OUTPUT)
    try{
        QLoggingCategory& category = qtjambi_object_from_nativeId_deref<QLoggingCategory>(env, categoryId);
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 4);
        jclass declaringClass = Java::QtJambi::QtJambiInternal$InvocationInfo::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::QtJambiInternal$InvocationInfo::methodName(env, invocationInfo);
        line = Java::QtJambi::QtJambiInternal$InvocationInfo::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QMessageLogger logger(_className, line, _method, category.categoryName());
        switch(messageType){
        case QtMsgType::QtWarningMsg:
            logger.warning(category, "%s", _message);
            break;
        case QtMsgType::QtCriticalMsg:
            logger.critical(category, "%s", _message);
            break;
        case QtMsgType::QtDebugMsg:
            logger.debug(category, "%s", _message);
            break;
        case QtMsgType::QtInfoMsg:
            logger.info(category, "%s", _message);
            break;
        case QtMsgType::QtFatalMsg:
            logger.fatal("%s", _message);
            break;
        }
#if !defined (QT_NO_DEBUG)
        env->ReleaseStringUTFChars(method, _method);
        env->ReleaseStringUTFChars(className, _className);
#endif
        env->ReleaseStringUTFChars(message, _message);
        qtjambi_throw_java_exception(env)
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
    Q_UNUSED(messageType)
    Q_UNUSED(message)
    Q_UNUSED(category)
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_showMessageFromSupplier)
(JNIEnv *env, jclass, jint messageType, jobject messageSupplier)
{
#if !defined(QT_NO_DEBUG_OUTPUT)
    try{
        jstring message = Java::QtCore::QLogging$Supplier::get(env, messageSupplier);
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 4);
        jclass declaringClass = Java::QtJambi::QtJambiInternal$InvocationInfo::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::QtJambiInternal$InvocationInfo::methodName(env, invocationInfo);
        line = Java::QtJambi::QtJambiInternal$InvocationInfo::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QMessageLogger logger(_className, line, _method);
        switch(messageType){
        case QtMsgType::QtWarningMsg:
            logger.warning("%s", _message);
            break;
        case QtMsgType::QtCriticalMsg:
            logger.critical("%s", _message);
            break;
        case QtMsgType::QtDebugMsg:
            logger.debug("%s", _message);
            break;
        case QtMsgType::QtInfoMsg:
            logger.info("%s", _message);
            break;
        case QtMsgType::QtFatalMsg:
            logger.fatal("%s", _message);
            break;
        }
#if !defined (QT_NO_DEBUG)
        env->ReleaseStringUTFChars(method, _method);
        env->ReleaseStringUTFChars(className, _className);
#endif
        env->ReleaseStringUTFChars(message, _message);
        qtjambi_throw_java_exception(env)
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
    Q_UNUSED(messageType)
    Q_UNUSED(message)
    Q_UNUSED(category)
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_showMessage)
(JNIEnv *env, jclass, jint messageType, jstring message)
{
#if !defined(QT_NO_DEBUG_OUTPUT)
    try{
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 4);
        jclass declaringClass = Java::QtJambi::QtJambiInternal$InvocationInfo::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::QtJambiInternal$InvocationInfo::methodName(env, invocationInfo);
        line = Java::QtJambi::QtJambiInternal$InvocationInfo::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QMessageLogger logger(_className, line, _method);
        switch(messageType){
        case QtMsgType::QtWarningMsg:
            logger.warning("%s", _message);
            break;
        case QtMsgType::QtCriticalMsg:
            logger.critical("%s", _message);
            break;
        case QtMsgType::QtDebugMsg:
            logger.debug("%s", _message);
            break;
        case QtMsgType::QtInfoMsg:
            logger.info("%s", _message);
            break;
        case QtMsgType::QtFatalMsg:
            logger.fatal("%s", _message);
            break;
        }
#if !defined (QT_NO_DEBUG)
        env->ReleaseStringUTFChars(method, _method);
        env->ReleaseStringUTFChars(className, _className);
#endif
        env->ReleaseStringUTFChars(message, _message);
        qtjambi_throw_java_exception(env)
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
    Q_UNUSED(messageType)
    Q_UNUSED(message)
    Q_UNUSED(category)
#endif
}

// QByteArrayList::join()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass,
 jobject list0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    try{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        QByteArray __qt_return_value = __qt_list0.join();
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QByteArrayList::join(char sep)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2B)
(JNIEnv *__jni_env,
 jclass,
 jobject list0,
 jbyte sep1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    try{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        QByteArray __qt_return_value = __qt_list0.join(char(sep1));
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QByteArrayList::join(const QByteArray & sep)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 jobject list0,
 QtJambiNativeID sep1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    try{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        const QByteArray&  __qt_sep1 = qtjambi_value_from_nativeId<QByteArray>(sep1);
        QByteArray __qt_return_value = __qt_list0.join(__qt_sep1);
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}


extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIODevice_fromDirectBuffer)
(JNIEnv *env,
 jclass,
 jobject buffer)
{
    try{
        bool isDirect = Java::Runtime::Buffer::isDirect(env, buffer);
        if(!isDirect){
            return nullptr;
        }
        jlong capacity = env->GetDirectBufferCapacity(buffer);
        void* address = env->GetDirectBufferAddress(buffer);
        bool readOnly = Java::Runtime::Buffer::isReadOnly(env, buffer);
        if(address && capacity>0){
            QIODevice* device = qtjambi_new_direct_address_device(capacity, reinterpret_cast<char*>(address), readOnly, env, buffer);
            return qtjambi_cast<jobject>(env, device);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

// QCborValue::operator[](const QString & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborValue__1_1qt_1QCborValue_1setValue_1cref_1QString__JLjava_lang_String_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborValue::operator[](const QString & key)")
    try{
        QtJambiScope __qtjambi_scope(__this_nativeId);
        Q_UNUSED(__qtjambi_scope)
        QCborValue *__qt_this = qtjambi_object_from_nativeId<QCborValue>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QString&  __qt_key0 = qtjambi_cast<QString >(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborValue::operator[](long long key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborValue__1_1qt_1QCborValue_1setValue_1long_1long__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborValue::operator[](long long key)")
    try{
        QtJambiScope __qtjambi_scope(__this_nativeId);
        Q_UNUSED(__qtjambi_scope)
        QCborValue *__qt_this = qtjambi_object_from_nativeId<QCborValue>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[static_cast<qlonglong>(key0)] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborArray::operator[](qsizetype i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborArray__1_1qt_1QCborArray_1setValue_1qsizetype__JJLio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong i0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborArray::operator[](qsizetype i)")
    try{
        QtJambiScope __qtjambi_scope(__this_nativeId);
        Q_UNUSED(__qtjambi_scope)
        QCborArray *__qt_this = qtjambi_object_from_nativeId<QCborArray>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[static_cast<qsizetype>(i0)] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborMap::operator[](const QCborValue & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap__1_1qt_1QCborMap_1setValue_1cref_1QCborValue__JJLio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](const QCborValue & key)")
    try{
        QCborMap *__qt_this = qtjambi_object_from_nativeId<QCborMap>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QCborValue&  __qt_key0 = qtjambi_object_from_nativeId_deref<QCborValue>(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborMap::operator[](const QString & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap__1_1qt_1QCborMap_1setValue_1cref_1QString__JLjava_lang_String_2Lio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](const QString & key)")
    try{
        QCborMap *__qt_this = qtjambi_object_from_nativeId<QCborMap>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QString&  __qt_key0 = qtjambi_cast<QString >(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborMap::operator[](long long key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap__1_1qt_1QCborMap_1setValue_1long_1long__JJLio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](long long key)")
    try{
        QCborMap *__qt_this = qtjambi_object_from_nativeId<QCborMap>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborStreamReader::readByteArray()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader__1_1qt_1QCborStreamReader_1readByteArray__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readByteArray()")
    try{
        QCborStreamReader *__qt_this = qtjambi_object_from_nativeId<QCborStreamReader>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborStreamReader::StringResult<QByteArray> __qt_return_value = __qt_this->readByteArray();
        return Java::QtCore::QCborStreamReader$StringResult::newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QCborStreamReader::readString()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader__1_1qt_1QCborStreamReader_1readString__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readString()")
    try{
        QCborStreamReader *__qt_this = qtjambi_object_from_nativeId<QCborStreamReader>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborStreamReader::StringResult<QString> __qt_return_value = __qt_this->readString();
        return Java::QtCore::QCborStreamReader$StringResult::newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QCborStreamReader::readChunk()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader__1_1qt_1QCborStreamReader_1readChunk__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject buffer)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readChunk()")
    try{
        QCborStreamReader *__qt_this = qtjambi_object_from_nativeId<QCborStreamReader>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        JBufferData __qt_buffer(__jni_env, buffer);
        QCborStreamReader::StringResult<qsizetype> __qt_return_value = __qt_this->readStringChunk(__qt_buffer, __qt_buffer.size());
        return Java::QtCore::QCborStreamReader$StringResult::newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QObject::metaObject() const
extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_metaObject)
    (JNIEnv * env, jclass, QtJambiNativeID native__id)
{
    try{
        QObject *object = qtjambi_object_from_nativeId<QObject>(native__id);
        Q_ASSERT(object);
        return qtjambi_cast<jobject>(env, object->metaObject());
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

// QObject::findChild(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChild__JLjava_lang_Class_2JLjava_lang_String_2I)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jclass cl0,
 jlong metaObjectPointer1,
 jobject name2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChild(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        QString __qt_name2(qtjambi_to_qstring(__jni_env, jstring(name2)));
        QObject * __qt_return = qt_qFindChild_helper(__qt_this, __qt_name2, *__qt_metaObjectPointer1, Qt::FindChildOptions(options3));
        return qtjambi_from_qobject(__jni_env, __qt_return, cl0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

// QObject::findChildren(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenString__JJLjava_lang_String_2I)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jlong metaObjectPointer1,
 jobject name2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        QString __qt_name2(qtjambi_to_qstring(__jni_env, jstring(name2)));
        QList<QObject *> list;
        qt_qFindChildren_helper(__qt_this, __qt_name2, *__qt_metaObjectPointer1, reinterpret_cast<QList<void*>*>(&list), Qt::FindChildOptions(options3));
        return qtjambi_cast<jobject>(__jni_env, list);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
// QObject::findChildren(const QRegExp &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenQRegExp__JJJI)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jlong metaObjectPointer1,
 QtJambiNativeID regExp2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QRegExp &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        //JavaException::raiseQNoImplementationException(__jni_env, "Deprecated method removed." QTJAMBI_STACKTRACEINFO );
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        const QRegExp& __qt_regExp2 = qtjambi_value_from_nativeId<QRegExp>(regExp2);
        QList<QObject *> list;
        qt_qFindChildren_helper(__qt_this, __qt_regExp2, *__qt_metaObjectPointer1, reinterpret_cast<QList<void*>*>(&list), Qt::FindChildOptions(options3));
        return qtjambi_cast<jobject>(__jni_env, list);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}
#endif

// QObject::findChildren(const QRegularExpression &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenQRegularExpression__JJJI)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jlong metaObjectPointer1,
 QtJambiNativeID regExp2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QRegularExpression &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        const QRegularExpression& __qt_regExp2 = qtjambi_value_from_nativeId<QRegularExpression>(regExp2);
        QList<QObject *> list;
        qt_qFindChildren_helper(__qt_this, __qt_regExp2, *__qt_metaObjectPointer1, reinterpret_cast<QList<void*>*>(&list), Qt::FindChildOptions(options3));
        return qtjambi_cast<jobject>(__jni_env, list);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

#if !defined(QT_QTJAMBI_PORT)

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_javaThread)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_from_thread(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_thread)
(JNIEnv *__jni_env, jclass, jobject thread)
{
    try{
        return qtjambi_get_qthread_for_jthread(__jni_env, thread);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getThreadGroup)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_thread_get_group(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setUncaughtExceptionHandler)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jobject handler)
{
    try{
        qtjambi_thread_set_UncaughtExceptionHandler(__jni_env, __this_nativeId, handler);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getUncaughtExceptionHandler)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_thread_get_UncaughtExceptionHandler(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setContextClassLoader)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jobject cl)
{
    try{
        qtjambi_thread_set_ContextClassLoader(__jni_env, __this_nativeId, cl);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getContextClassLoader)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_thread_get_ContextClassLoader(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setName)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jstring name)
{
    try{
        qtjambi_thread_set_name(__jni_env, __this_nativeId, name);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getName)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_thread_get_name(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setDaemon)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jboolean daemon)
{
    try{
        qtjambi_thread_set_daemon(__jni_env, __this_nativeId, daemon);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_isDaemon)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_thread_is_daemon(__qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return false;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_initialize__JLjava_lang_ThreadGroup_2)
(JNIEnv *__jni_env, jclass, QtJambiNativeID __this_nativeId, jobject group)
{
    try{
        qtjambi_initialize_thread(__jni_env, __this_nativeId, group);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_initialize__)
(JNIEnv *env, jclass)
{
    try{
        qtjambi_initialize_current_thread(env);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

#endif // !defined(QT_QTJAMBI_PORT)

// QStaticPlugin::instance()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QStaticPlugin_instance)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QStaticPlugin::instance()")
    try{
        QStaticPlugin *__qt_this = qtjambi_object_from_nativeId<QStaticPlugin>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->instance());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaType_buffer)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID nativeId,
 jint type0)
{
    try{
        if(void* ptr = qtjambi_from_nativeId(nativeId)){
            jlong size = QMetaType::sizeOf(type0);
            if(size>0){
                return __jni_env->NewDirectByteBuffer(ptr, size);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaType_00024GenericValue_clone)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID nativeId,
 jint type0)
{
    try{
        if(void* ptr = qtjambi_from_nativeId(nativeId)){
            jobject result = qtjambi_from_object(__jni_env, QMetaType::create(type0, ptr), Java::QtCore::QMetaType$GenericValue::getClass(__jni_env), false);
            if(result)
                Java::QtCore::QMetaType$GenericValue::set_type(__jni_env, result, type0);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_toReflected)
    (JNIEnv * env, jclass, QtJambiNativeID object_id)
{
    return qtjambi_metamethod_to_reflected(env, object_id);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_parameterClassTypes)
    (JNIEnv * env, jclass, QtJambiNativeID object_id)
{
    return qtjambi_metamethod_parameter_class_types(env, object_id);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_returnClassType)
    (JNIEnv * env, jclass, QtJambiNativeID object_id)
{
    return qtjambi_metamethod_return_class_type(env, object_id);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_methodFromMethod)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jobjectArray ok)
{
    try{
        return qtjambi_method_from_javamethod(env, metaObjectPointer, ok);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod__1invoke)
    (JNIEnv * env, jclass, QtJambiNativeID object__id, QtJambiNativeID metaMethodId, jobjectArray argClassTypeArray, jint connection, jobjectArray args)
{
    return qtjambi_metamethod_invoke(env, object__id, metaMethodId, argClassTypeArray, connection, args);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod__1invokeOnGadget)
    (JNIEnv * env, jclass, QtJambiNativeID object_id, jobject object, QtJambiNativeID metaMethodId, jobjectArray argClassTypeArray, jobjectArray args)
{
    return qtjambi_metamethod_invoke_on_gadget(env, object_id, object, metaMethodId, argClassTypeArray, args);
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_resetOnGadget)
    (JNIEnv *env, jclass, QtJambiNativeID __this_nativeId, jobject gadget){
        return qtjambi_metaproperty_reset_on_gadget(env, __this_nativeId, gadget);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_readOnGadget)
    (JNIEnv *env, jclass, QtJambiNativeID __this_nativeId, jobject gadget){
        return qtjambi_metaproperty_read_on_gadget(env, __this_nativeId, gadget);
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_writeOnGadget)
    (JNIEnv *env, jclass, QtJambiNativeID __this_nativeId, jobject gadget, jobject value){
        return qtjambi_metaproperty_write_on_gadget(env, __this_nativeId, gadget, value);
}

extern "C" Q_DECL_EXPORT jclass JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_classType)
    (JNIEnv *env, jclass, QtJambiNativeID __this_nativeId){
        return qtjambi_metaproperty_class(env, __this_nativeId);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_resolveEntry)
    (JNIEnv * env, jclass, jclass enumClass, jint value, jbyte bitSize)
{
    try{
        switch ( bitSize ) {
        case 1:  return qtjambi_from_enum(env, qint8(value), enumClass);
        case 2:  return qtjambi_from_enum(env, qint16(value), enumClass);
        case 8:  return qtjambi_from_enum(env, qint64(value), enumClass);
        default: return qtjambi_from_enum(env, qint32(value), enumClass);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_flags)
    (JNIEnv * env, jclass, jclass flagsClass, jint value)
{
    try{
        return qtjambi_from_flags(env, value, qPrintable(qtjambi_class_name(env, flagsClass).replace(".", "/")));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumeratorIndex)
    (JNIEnv *env, jclass, jlong metaObjectPointer, QtJambiNativeID enumID)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            Q_ASSERT(metaObject != nullptr);
            const QMetaEnum *__qt_this = qtjambi_object_from_nativeId<QMetaEnum>(enumID);
            return metaObject->indexOfEnumerator(__qt_this->name());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_userProperty)
    (JNIEnv *env, jobject, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            Q_ASSERT(metaObject != nullptr);
            return qtjambi_cast<jobject>(env, metaObject->userProperty());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_properties)
    (JNIEnv *env, jobject, jlong metaObjectPointer)
{
    try{
        QList<QMetaProperty> propertyList;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int count = metaObject->propertyCount();
            for (int i=0; i<count; ++i) {
                propertyList << metaObject->property(i);
            }
        }
        return qtjambi_cast<jobject>(env, propertyList);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_propertyCount)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            return metaObject->propertyCount();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_property)
    (JNIEnv *env, jobject, jlong metaObjectPointer, jobject name)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int i = metaObject->indexOfProperty(qPrintable(qtjambi_to_qstring(env, jstring(name))));
            return qtjambi_cast<jobject>(env, metaObject->property(i));
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

static const QSet<QMetaMethod> forbidddenMethods = []() -> QSet<QMetaMethod> {
        QSet<QMetaMethod> methods;
        int idx;
        //idx = QObject::staticMetaObject.indexOfMethod("destroyed(QObject*)");
        //if(idx>=0)
        //    methods.insert(QObject::staticMetaObject.method(idx));
        //idx = QObject::staticMetaObject.indexOfMethod("destroyed()");
        //if(idx>=0)
        //    methods.insert(QObject::staticMetaObject.method(idx));
        //idx = QObject::staticMetaObject.indexOfMethod("deleteLater()");
        //if(idx>=0)
        //    methods.insert(QObject::staticMetaObject.method(idx));
        idx = QObject::staticMetaObject.indexOfMethod("_q_reregisterTimers(void*)");
        if(idx>=0)
            methods.insert(QObject::staticMetaObject.method(idx));
        return methods;
    }();

bool method_is_valid(const QMetaMethod& method){
    return method.isValid() && !forbidddenMethods.contains(method);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_connect)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jstring signalName, QtJambiNativeID receiver__id, jstring slotName, jbyte connectionType)
{
    jobject result = nullptr;
    try{
        QObject *sender = qtjambi_object_from_nativeId<QObject>(sender__id);
        QObject *receiver = qtjambi_object_from_nativeId<QObject>(receiver__id);
        if(sender && receiver){
            QByteArray normalizedSignal = QMetaObject::normalizedSignature(qPrintable(qtjambi_to_qstring(env, signalName)));
            QByteArray normalizedSlot = QMetaObject::normalizedSignature(qPrintable(qtjambi_to_qstring(env, slotName)));
            QMetaMethod signalMethod = sender->metaObject()->method(sender->metaObject()->indexOfMethod(normalizedSignal.constData()));
            QMetaMethod slotMethod = receiver->metaObject()->method(receiver->metaObject()->indexOfMethod(normalizedSlot.constData()));
            if(!signalMethod.isValid()){
                for(int i=0; i<sender->metaObject()->methodCount(); ++i){
                    QMetaMethod method = sender->metaObject()->method(i);
                    if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                        if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                            QByteArray methodSignature = method.methodSignature();
                            auto idxFirstParen = methodSignature.indexOf('(');
                            if(idxFirstParen>=0){
                                methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                            }
                            methodSignature.prepend(newName);
                            if(normalizedSignal==methodSignature){
                                signalMethod = method;
                                break;
                            }
                        }
                    }
                }
            }
            if(!slotMethod.isValid()){
                for(int i=0; i<receiver->metaObject()->methodCount(); ++i){
                    QMetaMethod method = receiver->metaObject()->method(i);
                    if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                        if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                            QByteArray methodSignature = method.methodSignature();
                            auto idxFirstParen = methodSignature.indexOf('(');
                            if(idxFirstParen>=0){
                                methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                            }
                            methodSignature.prepend(newName);
                            if(normalizedSlot==methodSignature){
                                slotMethod = method;
                                break;
                            }
                        }
                    }
                }
            }
            if(method_is_valid(signalMethod) && method_is_valid(slotMethod)){
                QMetaObject::Connection connection = QObject::connect(sender, signalMethod, receiver, slotMethod, Qt::ConnectionType(connectionType));
                if(connection){
                    result = qtjambi_cast<jobject>(env, connection);
                    qtjambi_set_java_ownership(env, result);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_connectMethods)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jint signalIdx, jlong signalEnclosingMetaObjectPointer, QtJambiNativeID receiver__id, jint slotIdx, jlong receiverEnclosingMetaObjectPointer, jbyte connectionType)
{
    jobject result = nullptr;
    try{
        QObject *sender = qtjambi_object_from_nativeId<QObject>(sender__id);
        QObject *receiver = qtjambi_object_from_nativeId<QObject>(receiver__id);
        const QMetaObject *signalEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(signalEnclosingMetaObjectPointer);
        const QMetaObject *receiverEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(receiverEnclosingMetaObjectPointer);
        if(signalEnclosingMetaObject && receiverEnclosingMetaObjectPointer && sender && receiver){
            QMetaMethod signalMethod = signalEnclosingMetaObject->method(signalIdx);
            QMetaMethod slotMethod = receiverEnclosingMetaObject->method(slotIdx);
            if(method_is_valid(signalMethod) && method_is_valid(slotMethod)){
                QMetaObject::Connection connection = QObject::connect(sender, signalMethod, receiver, slotMethod, Qt::ConnectionType(connectionType));
                if(connection){
                    result = qtjambi_cast<jobject>(env, connection);
                    qtjambi_set_java_ownership(env, result);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}



extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_disconnect)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jstring signalName, QtJambiNativeID receiver__id, jstring slotName)
{
    bool result = false;
    try{
        QObject *sender = qtjambi_object_from_nativeId<QObject>(sender__id);
        if(sender){
            QObject *receiver = qtjambi_object_from_nativeId<QObject>(receiver__id);
            QString qsignalName = qtjambi_to_qstring(env, signalName);
            QString qslotName = qtjambi_to_qstring(env, slotName);
            QByteArray normalizedSignal = QMetaObject::normalizedSignature(qPrintable(qsignalName));
            QByteArray normalizedSlot = QMetaObject::normalizedSignature(qPrintable(qslotName));
            QMetaMethod signalMethod;
            if(!normalizedSignal.isEmpty()){
                signalMethod = sender->metaObject()->method(sender->metaObject()->indexOfMethod(normalizedSignal.constData()));
                if(!signalMethod.isValid()){
                    for(int i=0; i<sender->metaObject()->methodCount(); ++i){
                        QMetaMethod method = sender->metaObject()->method(i);
                        if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                            if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                                QByteArray methodSignature = method.methodSignature();
                                auto idxFirstParen = methodSignature.indexOf('(');
                                if(idxFirstParen>=0){
                                    methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                                }
                                methodSignature.prepend(newName);
                                if(normalizedSignal==methodSignature){
                                    signalMethod = method;
                                    normalizedSignal = methodSignature;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            QMetaMethod slotMethod;
            if(receiver){
                slotMethod = receiver->metaObject()->method(receiver->metaObject()->indexOfMethod(normalizedSlot.constData()));
                if(!slotMethod.isValid()){
                    for(int i=0; i<receiver->metaObject()->methodCount(); ++i){
                        QMetaMethod method = receiver->metaObject()->method(i);
                        if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                            if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                                QByteArray methodSignature = method.methodSignature();
                                auto idxFirstParen = methodSignature.indexOf('(');
                                if(idxFirstParen>=0){
                                    methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                                }
                                methodSignature.prepend(newName);
                                if(normalizedSlot==methodSignature){
                                    slotMethod = method;
                                    normalizedSlot = methodSignature;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if((!signalMethod.isValid() || method_is_valid(signalMethod))
                    && (!slotMethod.isValid() ||method_is_valid(slotMethod))){
                result = QObject::disconnect(sender, signalMethod, receiver, slotMethod);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_disconnectMethods)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jint signalIdx, jlong signalEnclosingMetaObjectPointer, QtJambiNativeID receiver__id, jint slotIdx, jlong receiverEnclosingMetaObjectPointer)
{
    bool result = false;
    try{
        QObject *sender = qtjambi_object_from_nativeId<QObject>(sender__id);
        if(sender){
            QObject *receiver = qtjambi_object_from_nativeId<QObject>(receiver__id);
            const QMetaObject *signalEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(signalEnclosingMetaObjectPointer);
            const QMetaObject *receiverEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(receiverEnclosingMetaObjectPointer);
            QMetaMethod signalMethod;
            QMetaMethod slotMethod;
            if(signalEnclosingMetaObject && signalIdx>=0){
                signalMethod = signalEnclosingMetaObject->method(signalIdx);
            }
            if(receiverEnclosingMetaObject && slotIdx>=0){
                slotMethod = receiverEnclosingMetaObject->method(slotIdx);
            }
            if((!signalMethod.isValid() || method_is_valid(signalMethod))
                    && (!slotMethod.isValid() ||method_is_valid(slotMethod))){
                result = QObject::disconnect(sender, signalMethod, receiver, slotMethod);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_findEnumForFlags)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        QMetaEnum enumerator = metaObject->enumerator(enumeratorIndex);
        QStringList keys;
        for (int i=0;i<enumerator.keyCount();++i) {
            keys << QLatin1String(enumerator.key(i));
        }
        for(int e=0; e<metaObject->enumeratorCount(); ++e){
            if(e!=enumeratorIndex){
                QMetaEnum enm = metaObject->enumerator(enumeratorIndex);
                if(!enm.isFlag()){
                    QStringList keys2;
                    for (int i=0;i<enm.keyCount();++i) {
                        keys2 << QLatin1String(enm.key(i));
                    }
                    if(keys==keys2){
                        return qtjambi_cast<jobject>(__jni_env, enumerator);
                    }
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methodByIndex)
    (JNIEnv * env, jobject, jlong metaObjectPointer, jint index)
{
    try{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->method(index);
            if(method.isValid()){
                return qtjambi_cast<jobject>(env, method);
            }else{
                JavaException::raiseIndexOutOfBoundsException(env, qPrintable(QString("Index %1 is not a valid method of class %2").arg(index).arg(metaObject->className())) QTJAMBI_STACKTRACEINFO );
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methods)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    try{
        env->EnsureLocalCapacity(500);
        QList<QMetaMethod> methods;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            const int methodCount = metaObject->methodCount();
            for(int i=0; i<methodCount; ++i){
                methods << metaObject->method(i);
            }
        }
        return qtjambi_cast<jobject>(env, methods);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methodCount)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer)){
            return metaObject->methodCount();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructor)
    (JNIEnv * env, jobject, jlong metaObjectPointer, jobject normalizedSignature)
{
    try{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->constructor(metaObject->indexOfConstructor(qPrintable(qtjambi_to_qstring(env, jstring(normalizedSignature)))));
            if(method.isValid() && method.methodType()==QMetaMethod::Constructor){
                return qtjambi_cast<jobject>(env, method);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructorCount)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            return metaObject->constructorCount();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructors)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    try{
        env->EnsureLocalCapacity(100);
        QList<QMetaMethod> methods;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            for(int i=0; i<metaObject->constructorCount(); ++i){
                methods << metaObject->constructor(i);
            }
        }
        return qtjambi_cast<jobject>(env, methods);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumerator)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jobject name)
{
    try{
        if(metaObjectPointer){
            QString enumName = qtjambi_to_qstring(env, jstring(name));
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int idx = metaObject->indexOfEnumerator(qPrintable(enumName));
            QMetaEnum enumerator = metaObject->enumerator(idx);
            return qtjambi_cast<jobject>(env, enumerator);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumeratorCount)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer)){
            return metaObject->enumeratorCount();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumerators)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        QList<QMetaEnum> javaEnums;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int count = metaObject->enumeratorCount();
            for (int i=0; i<count; ++i) {
                javaEnums << metaObject->enumerator(i);
            }
        }
        return qtjambi_cast<jobject>(env, javaEnums);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_superClass)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject && metaObject->superClass()){
            return qtjambi_cast<jobject>(env, metaObject->superClass());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_className)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            return qtjambi_from_qstring(env, QLatin1String(metaObject->className()));
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfos)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            QList<QPair<QString,QString>> list;
            for (int var = 0; var < metaObject->classInfoCount(); ++var) {
                list << QPair<QString,QString>{QLatin1String(metaObject->classInfo(var).name()), QLatin1String(metaObject->classInfo(var).value())};
            }
            return qtjambi_cast<jobject>(env, list);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfoCount)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer)){
            return metaObject->classInfoCount();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfo)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jstring name)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            int var = metaObject->indexOfClassInfo(qPrintable(qtjambi_to_qstring(env, name)));
            if(var){
                return qtjambi_from_qstring(env, QLatin1String(metaObject->classInfo(var).value()));
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_inherits)
    (JNIEnv *env, jclass, jlong metaObjectPointer, jlong other)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            return metaObject->inherits(reinterpret_cast<const QMetaObject *>(other));
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_checkConnectArgsString)
    (JNIEnv * env, jclass, jobject signal, jobject method)
{
    try{
        return QMetaObject::checkConnectArgs(qPrintable(qtjambi_to_qstring(env, jstring(signal))), qPrintable(qtjambi_to_qstring(env, jstring(method))));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_checkConnectArgsMethods)
    (JNIEnv *env, jclass, jlong signalMetaObjectPointer, jint signalMethodIndex, jlong methodMetaObjectPointer, jint methodMethodIndex)
{
    try{
        const QMetaObject *signalMetaObject = reinterpret_cast<const QMetaObject *>(signalMetaObjectPointer);
        const QMetaObject *methodMetaObject = reinterpret_cast<const QMetaObject *>(methodMetaObjectPointer);
        return QMetaObject::checkConnectArgs(signalMetaObject->method(signalMethodIndex), methodMetaObject->method(methodMethodIndex));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject__1_1qt_1forType)
    (JNIEnv * env, jclass, jclass type)
{
    try{
        const QMetaObject *metaObject = qtjambi_metaobject_for_class(env, type, nullptr);
        return qtjambi_cast<jobject>(env, metaObject);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}



extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_type)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            return qtjambi_metaobject_javatype(env, metaObject);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_exactType)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            return qtjambi_metaobject_javatype(env, metaObject, true);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_cast)
    (JNIEnv *env, jclass, QtJambiNativeID object__id, jclass targetType)
{
    try{
        return qtjambi_metaobject_cast(env, object__id, targetType);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_method)
    (JNIEnv * env, jobject, jlong metaObjectPointer, jstring normalizedSignature)
{
    try{
        if(metaObjectPointer){
            QString sig = qtjambi_to_qstring(env, jstring(normalizedSignature));
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);

            QMetaMethod method = qtjambi_metaobject_get_method(metaObject, sig);
            return qtjambi_cast<jobject>(env, method);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_newInstanceImpl)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jint methodIndex, jobject argTypes, jobjectArray args)
{
    try{
        return qtjambi_metaobject_new_instance(env, metaObjectPointer, methodIndex, argTypes, args);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterfaceVoid__1_1qt_1future__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterfaceVoid::future() const")
    try{
        QFutureInterface<void> *__qt_this = qtjambi_object_from_nativeId<QFutureInterface<void>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->future());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface__1_1qt_1future__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface::future() const")
    try{
        QFutureInterface<JObjectWrapper> *__qt_this = qtjambi_object_from_nativeId<QFutureInterface<JObjectWrapper>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->future());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureWatcherVoid__1_1qt_1future__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureWatcherVoid::future() const")
    try{
        QFutureWatcher<void> *__qt_this = qtjambi_object_from_nativeId<QFutureWatcher<void>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->future());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureWatcher__1_1qt_1future__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureWatcher::future() const")
    try{
        QFutureWatcher<JObjectWrapper> *__qt_this = qtjambi_object_from_nativeId<QFutureWatcher<JObjectWrapper>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->future());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureSynchronizerVoid__1_1qt_1futures__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureSynchronizerVoid::futures() const")
    try{
        QFutureSynchronizer<void> *__qt_this = qtjambi_object_from_nativeId<QFutureSynchronizer<void>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->futures());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureSynchronizer__1_1qt_1futures__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureSynchronizer::futures() const")
    try{
        QFutureSynchronizer<JObjectWrapper> *__qt_this = qtjambi_object_from_nativeId<QFutureSynchronizer<JObjectWrapper>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->futures());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_futureInterface)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureSynchronizer::futures() const")
    try{
        QFuture<void> *__qt_this = qtjambi_object_from_nativeId<QFuture<void>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, reinterpret_cast<QFutureInterfaceBase*>(__qt_this));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_futureInterface)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureSynchronizer::futures() const")
    try{
        QFuture<JObjectWrapper> *__qt_this = qtjambi_object_from_nativeId<QFuture<JObjectWrapper>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, reinterpret_cast<QFutureInterface<JObjectWrapper>*>(__qt_this));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

template<int functorType>
class BindingFunctionVTableHelper : public QtPrivate::BindingFunctionVTable{
public:
    static QtPrivate::BindingFunctionVTable create(){
        return {
                     [](QMetaType metaType, QUntypedPropertyData *dataPtr, void *f) -> bool {
                         JObjectWrapper* _f = reinterpret_cast<JObjectWrapper*>(f);
                         try{
                             if(JNIEnv* env = qtjambi_current_environment()){
                                 QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                 try{
                                     if constexpr(functorType=='L'){
                                         jobject result = Java::QtJambi::QtUtilities$Supplier::get(env, env->NewLocalRef(_f->object()));
                                         if(metaType.flags() & QMetaType::IsPointer){
                                             jobject propertyObject = qtjambi_find_object(env, dataPtr);
                                             if(Java::QtCore::QPropertyData::isInstanceOf(env, propertyObject)){
                                                 jobject oldValue = Java::QtCore::QPropertyData::getValueBypassingBindings(env, propertyObject);
                                                 if(!oldValue){
                                                     if(!result)
                                                         return false;
                                                 }else{
                                                     if(Java::Runtime::Object::equals(env, oldValue, result))
                                                         return false;
                                                 }
                                                 return Java::QtCore::QPropertyData::setValueBypassingBindings(env, propertyObject, result);
                                             }else{
                                                 QPropertyData<void*> *propertyPtr = static_cast<QPropertyData<void*> *>(dataPtr);
                                                 void* _result;
                                                 if(result){
                                                     QVariant variant = qtjambi_to_qvariant(env, result);
                                                     if(variant.metaType()!=metaType && !variant.convert(metaType))
                                                         return false;
                                                     _result = *reinterpret_cast<void*const*>(variant.constData());
                                                 }else{
                                                     _result = nullptr;
                                                 }
                                                 if (_result == propertyPtr->valueBypassingBindings())
                                                     return false;
                                                 propertyPtr->setValueBypassingBindings(_result);
                                                 return true;
                                             }
                                         }else{
                                             switch(metaType.id()){
                                             case QMetaType::QByteArray: {
                                                     QPropertyData<QByteArray> *propertyPtr = static_cast<QPropertyData<QByteArray> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArray>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QByteArrayList: {
                                                     QPropertyData<QByteArrayList> *propertyPtr = static_cast<QPropertyData<QByteArrayList> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArrayList>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QString: {
                                                     QPropertyData<QString> *propertyPtr = static_cast<QPropertyData<QString> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QString>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QStringList: {
                                                     QPropertyData<QStringList> *propertyPtr = static_cast<QPropertyData<QStringList> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QStringList>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantList: {
                                                     QPropertyData<QVariantList> *propertyPtr = static_cast<QPropertyData<QVariantList> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantList>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantMap: {
                                                     QPropertyData<QVariantMap> *propertyPtr = static_cast<QPropertyData<QVariantMap> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantMap>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantHash: {
                                                     QPropertyData<QVariantHash> *propertyPtr = static_cast<QPropertyData<QVariantHash> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantHash>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantPair: {
                                                     QPropertyData<QVariantPair> *propertyPtr = static_cast<QPropertyData<QVariantPair> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantPair>(env, result));
                                                     return true;
                                                 }
                                                 break;
                                             case QMetaType::QVariant: {
                                                    QPropertyData<QVariant> *propertyPtr = static_cast<QPropertyData<QVariant> *>(dataPtr);
                                                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariant>(env, result));
                                                    return true;
                                                }
                                             default: {
                                                    QVariant variant = result ? qtjambi_to_qvariant(env, result) : QVariant(metaType);
                                                    if(variant.metaType()!=metaType && !variant.convert(metaType))
                                                         return false;
                                                    if(metaType.isEqualityComparable() && metaType.equals(dataPtr, variant.constData()))
                                                         return false;
                                                    metaType.destruct(dataPtr);
                                                    metaType.construct(dataPtr, variant.constData());
                                                }
                                                return true;
                                             }
                                         }
                                     }else if constexpr(functorType=='I'){
                                         jint result = Java::Runtime::IntSupplier::getAsInt(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint32> *propertyPtr = static_cast<QPropertyData<qint32> *>(dataPtr);
                                         if (result == propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }else if constexpr(functorType=='S'){
                                         jshort result = Java::QtJambi::QtUtilities$ShortSupplier::getAsShort(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint16> *propertyPtr = static_cast<QPropertyData<qint16> *>(dataPtr);
                                         if (result == propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }else if constexpr(functorType=='B'){
                                         jbyte result = Java::QtJambi::QtUtilities$ByteSupplier::getAsByte(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint8> *propertyPtr = static_cast<QPropertyData<qint8> *>(dataPtr);
                                         if (result == propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }else if constexpr(functorType=='J'){
                                         jlong result = Java::Runtime::LongSupplier::getAsLong(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint64> *propertyPtr = static_cast<QPropertyData<qint64> *>(dataPtr);
                                         if (result == propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }else if constexpr(functorType=='D'){
                                         jdouble result = Java::Runtime::DoubleSupplier::getAsDouble(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<double> *propertyPtr = static_cast<QPropertyData<double> *>(dataPtr);
                                         if (result==propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }else if constexpr(functorType=='F'){
                                         jfloat result = Java::QtJambi::QtUtilities$FloatSupplier::getAsFloat(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<float> *propertyPtr = static_cast<QPropertyData<float> *>(dataPtr);
                                         if (result==propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }else if constexpr(functorType=='C'){
                                         char16_t result = char16_t(Java::QtJambi::QtUtilities$CharSupplier::getAsChar(env, env->NewLocalRef(_f->object())));
                                         QPropertyData<char16_t> *propertyPtr = static_cast<QPropertyData<char16_t> *>(dataPtr);
                                         if (result == propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }else if constexpr(functorType=='Z'){
                                         bool result = Java::Runtime::BooleanSupplier::getAsBoolean(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<bool> *propertyPtr = static_cast<QPropertyData<bool> *>(dataPtr);
                                         if (result == propertyPtr->valueBypassingBindings())
                                             return false;
                                         propertyPtr->setValueBypassingBindings(result);
                                         return true;
                                     }
                                 }catch(const JavaException& exn){
                                     exn.report(env);
                                 }
                             }
                         }catch(...){}
                         return false;
                     },
                     /*destroy*/[](void *f){ static_cast<JObjectWrapper *>(f)->~JObjectWrapper(); },
                     /*moveConstruct*/[](void *addr, void *other){
                         new (addr) JObjectWrapper(std::move(*static_cast<JObjectWrapper *>(other)));
                     },
                     /*size*/sizeof(JObjectWrapper)};
    }
};

// new QUntypedPropertyBinding(QMetaType, const BindingFunctionVTable *, void *, const QPropertyBindingSourceLocation &)
void __qt_construct_QUntypedPropertyBinding_2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    static_assert (alignof(JObjectWrapper) <= alignof(std::max_align_t), "Bindings do not support overaligned functors!");
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QUntypedPropertyBinding(const QUntypedPropertyBinding & other)")
    QMetaType metaType;
    QtPrivate::BindingFunctionVTable* vtable(nullptr);
    switch(__java_arguments[1].c){
    case 'L':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'L'>::create();
        vtable = &_vtable;
        metaType = qtjambi_cast<QMetaType>(__jni_env, Java::QtCore::QPropertyBinding::analyzeMetaType(__jni_env, __java_arguments[0].l));
        break;
    }
    case 'I':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'I'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::Int);
        break;
    }
    case 'B':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'B'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::UChar);
        break;
    }
    case 'S':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'S'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::Short);
        break;
    }
    case 'J':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'J'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::LongLong);
        break;
    }
    case 'D':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'D'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::Double);
        break;
    }
    case 'F':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'F'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::Float);
        break;
    }
    case 'Z':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'Z'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::Bool);
        break;
    }
    case 'C':{
        static QtPrivate::BindingFunctionVTable _vtable = BindingFunctionVTableHelper<'C'>::create();
        vtable = &_vtable;
        metaType = QMetaType(QMetaType::QChar);
        break;
    }
    default:
        JavaException::raiseIllegalArgumentException(__jni_env, "Unknown type flag." QTJAMBI_STACKTRACEINFO );
        break;
    }
    JObjectWrapper f(__jni_env, __java_arguments[0].l);
    QUntypedPropertyBinding *__qt_this = new(__qtjambi_ptr) QUntypedPropertyBinding(metaType, vtable, &f, QPropertyBindingSourceLocation());
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qt_this)
}

// new QUntypedPropertyBinding(QPropertyBindingPrivate *)
void __qt_construct_QUntypedPropertyBinding_3(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QUntypedPropertyBinding(QPropertyBindingPrivate *)")
    jobject data0 = __java_arguments[0].l;
    QtPrivate::QPropertyBindingData * __qt_data0 = qtjambi_cast<QtPrivate::QPropertyBindingData * >(__jni_env, data0);
    QUntypedPropertyBinding *__qt_this = new(__qtjambi_ptr) QUntypedPropertyBinding(__qt_data0 ? __qt_data0->binding() : nullptr);
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qt_this)
}

// QUntypedPropertyBinding::QUntypedPropertyBinding(QMetaType, const BindingFunctionVTable *, void *, const QPropertyBindingSourceLocation &)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedPropertyBinding__1_1qt_1QUntypedPropertyBinding_1new_1functor)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject functor)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedPropertyBinding::QUntypedPropertyBinding(QMetaType, const BindingFunctionVTable *, void *, const QPropertyBindingSourceLocation &)")
    try{
        jvalue arguments[2];
        arguments[0].l = functor;

        const std::type_info* typeId;
        if(Java::QtCore::QBooleanPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBooleanPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<bool>);
            arguments[1].c = 'Z';
        }else if(Java::QtCore::QBytePropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBytePropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint8>);
            arguments[1].c = 'B';
        }else if(Java::QtCore::QShortPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QShortPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint16>);
            arguments[1].c = 'S';
        }else if(Java::QtCore::QIntPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QIntPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint32>);
            arguments[1].c = 'I';
        }else if(Java::QtCore::QLongPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QLongPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint64>);
            arguments[1].c = 'J';
        }else if(Java::QtCore::QFloatPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QFloatPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<float>);
            arguments[1].c = 'F';
        }else if(Java::QtCore::QDoublePropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QDoublePropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<double>);
            arguments[1].c = 'D';
        }else if(Java::QtCore::QCharPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QCharPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<QChar>);
            arguments[1].c = 'C';
        }else if(Java::QtCore::QPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            if(!Java::QtJambi::QtUtilities$Supplier::isInstanceOf(__jni_env, functor))
                functor = nullptr;
            __jni_class = Java::QtCore::QPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<QVariant>);
            arguments[1].c = 'L';
        }else{
            typeId = &typeid(QUntypedPropertyBinding);
        }

        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QUntypedPropertyBinding_2, sizeof(QUntypedPropertyBinding), *typeId, false, arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QUntypedPropertyBinding::QUntypedPropertyBinding(QPropertyBindingPrivate *)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedPropertyBinding__1_1qt_1QUntypedPropertyBinding_1new_1bindingData)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject bindingData)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedPropertyBinding::QUntypedPropertyBinding(QPropertyBindingPrivate *)")
    try{
        jvalue arguments;
        arguments.l = bindingData;
        const std::type_info* typeId;
        if(Java::QtCore::QBooleanPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBooleanPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<bool>);
        }else if(Java::QtCore::QBytePropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBytePropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint8>);
        }else if(Java::QtCore::QShortPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QShortPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint16>);
        }else if(Java::QtCore::QIntPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QIntPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint32>);
        }else if(Java::QtCore::QLongPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QLongPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint64>);
        }else if(Java::QtCore::QFloatPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QFloatPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<float>);
        }else if(Java::QtCore::QDoublePropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QDoublePropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<double>);
        }else if(Java::QtCore::QCharPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QCharPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<QChar>);
        }else if(Java::QtCore::QPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<QVariant>);
        }else{
            typeId = &typeid(QUntypedPropertyBinding);
        }
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QUntypedPropertyBinding_3, sizeof(QUntypedPropertyBinding), *typeId, false, &arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

void __qt_construct_QUntypedPropertyBinding_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);

// QUntypedPropertyBinding::QUntypedPropertyBinding(QUntypedPropertyBinding)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedPropertyBinding__1_1qt_1QUntypedPropertyBinding_1new_1copy)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject other)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedPropertyBinding::QUntypedPropertyBinding(QUntypedPropertyBinding)")
    try{
        jvalue arguments;
        arguments.l = other;
        const std::type_info* typeId;
        if(Java::QtCore::QBooleanPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBooleanPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<bool>);
        }else if(Java::QtCore::QBytePropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBytePropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint8>);
        }else if(Java::QtCore::QShortPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QShortPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint16>);
        }else if(Java::QtCore::QIntPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QIntPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint32>);
        }else if(Java::QtCore::QLongPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QLongPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<qint64>);
        }else if(Java::QtCore::QFloatPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QFloatPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<float>);
        }else if(Java::QtCore::QDoublePropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QDoublePropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<double>);
        }else if(Java::QtCore::QCharPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QCharPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<QChar>);
        }else if(Java::QtCore::QPropertyBinding::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<QVariant>);
        }else{
            typeId = &typeid(QUntypedPropertyBinding);
        }
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QUntypedPropertyBinding_1, sizeof(QUntypedPropertyBinding), *typeId, false, &arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

void __qt_construct_QUntypedBindable_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);

// QUntypedBindable::QUntypedBindable(QUntypedPropertyData * d, const QtPrivate::QBindableInterface * i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedBindable__1_1qt_1QUntypedBindable_1new_1copy)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject d0,
 jobject i1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedBindable::QUntypedBindable(QUntypedPropertyData * d, const QtPrivate::QBindableInterface * i)")
    try{
        jvalue arguments[2];
        arguments[0].l = d0;
        arguments[1].l = i1;
        const std::type_info* typeId;
        if(Java::QtCore::QBooleanBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBooleanBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<bool>);
        }else if(Java::QtCore::QByteBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QByteBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<qint8>);
        }else if(Java::QtCore::QShortBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QShortBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<qint16>);
        }else if(Java::QtCore::QIntBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QIntBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<qint32>);
        }else if(Java::QtCore::QLongBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QLongBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<qint64>);
        }else if(Java::QtCore::QFloatBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QFloatBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<float>);
        }else if(Java::QtCore::QDoubleBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QDoubleBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<double>);
        }else if(Java::QtCore::QCharBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QCharBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<QChar>);
        }else if(Java::QtCore::QBindable::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBindable::getClass(__jni_env);
            typeId = &typeid(QBindable<QVariant>);
        }else{
            typeId = &typeid(QUntypedBindable);
        }
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QUntypedBindable_1, sizeof(QUntypedBindable), *typeId, true, arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedBindable_overrideBinding)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 QtJambiNativeID binding0)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        QUntypedPropertyBinding* __qt_binding0 = qtjambi_object_from_nativeId<QUntypedPropertyBinding>(binding0);
        if(iface && iface->setBinding && __qt_binding0) {
            return qtjambi_cast<jobject>(__jni_env, iface->setBinding(data, *__qt_binding0));
        }
        return qtjambi_cast<jobject>(__jni_env, QUntypedPropertyBinding());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIntBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            jint result = 0;
            iface->getter(data, &result);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jshort JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QShortBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            jshort result = 0;
            iface->getter(data, &result);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jbyte JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            jbyte result = 0;
            iface->getter(data, &result);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLongBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            jlong result = 0;
            iface->getter(data, &result);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT double JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDoubleBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            double result = 0;
            iface->getter(data, &result);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT float JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFloatBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            float result = 0;
            iface->getter(data, &result);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBooleanBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            bool result = 0;
            iface->getter(data, &result);
            return result;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jchar JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCharBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            QChar result;
            iface->getter(data, &result);
            return jchar(result.unicode());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIntBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jint value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            iface->setter(data, &value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jbyte value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            iface->setter(data, &value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QShortBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jshort value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            iface->setter(data, &value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLongBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jlong value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            iface->setter(data, &value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDoubleBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 double value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            iface->setter(data, &value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFloatBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jfloat value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            iface->setter(data, &value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCharBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jchar value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            iface->setter(data, &value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBooleanBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jboolean value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            bool b = value;
            iface->setter(data, &b);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 QtJambiNativeID metaTypeId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            QMetaType metaType;
            if(!!metaTypeId)
                metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
            else if(iface->metaType)
                metaType = iface->metaType();
            if(metaType.isValid()){
                QVariant result(metaType);
                iface->getter(data, result.data());
                return qtjambi_cast<jobject>(__jni_env, result);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 QtJambiNativeID metaTypeId,
 jobject value
 )
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            QMetaType metaType;
            if(!!metaTypeId)
                metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
            else if(iface->metaType)
                metaType = iface->metaType();
            if(metaType.isValid()){
                QVariant variant = value ? qtjambi_to_qvariant(__jni_env, value) : QVariant(metaType);
                if(variant.metaType()!=metaType && !variant.convert(metaType))
                    return false;
                iface->setter(data, variant.data());
                return true;
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

template<class T>
QMetaType qtjambi_metatype() { return QMetaType::fromType<T>(); }

template<class Property, class T>
void qtjambi_get_property_value(const QUntypedPropertyData *d, void *value){
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                *static_cast<T*>(value) = qtjambi_cast<T>(env, Property::value(env, property));
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
}

template<class Property, class T>
void qtjambi_set_property_value(QUntypedPropertyData *d, const void *value){
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject _value = qtjambi_cast<jobject>(env, *static_cast<const T*>(value));
                Property::setValue(env, property, _value);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
}

template<class Property>
void qtjambi_get_qproperty_value(const QUntypedPropertyData *d, void *value){
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                QMetaType metaType = qtjambi_cast<QMetaType>(env, Java::QtCore::QPropertyData::valueMetaType(env, property));
                QVariant _value = qtjambi_cast<QVariant>(env, Property::value(env, property));
                if(_value.metaType()!=metaType && !_value.convert(metaType))
                    return;
                metaType.destruct(value);
                metaType.construct(value, _value.data());
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
}

template<class Property>
void qtjambi_set_qproperty_value(QUntypedPropertyData *d, const void *value){
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                QMetaType metaType = qtjambi_cast<QMetaType>(env, Java::QtCore::QPropertyData::valueMetaType(env, property));
                jobject _value = qtjambi_cast<jobject>(env, QVariant(metaType, value));
                Property::setValue(env, property, _value);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
}

template<class Property>
QUntypedPropertyBinding qtjambi_get_binding(const QUntypedPropertyData *d) {
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject binding = Property::binding(env, property);
                return qtjambi_cast<QUntypedPropertyBinding>(env, binding);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
    return QUntypedPropertyBinding();
}

template<class Property>
QUntypedPropertyBinding qtjambi_set_binding(QUntypedPropertyData *d, const QUntypedPropertyBinding &binding) {
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject _binding = qtjambi_cast<jobject>(env, binding);
                jobject oldBinding = Property::setBinding(env, property, Java::QtCore::QPropertyBinding::newInstance(env, _binding));
                return qtjambi_cast<QUntypedPropertyBinding>(env, oldBinding);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
    return QUntypedPropertyBinding();
}

template<class Property>
QUntypedPropertyBinding qtjambi_make_binding(const QUntypedPropertyData *d, const QPropertyBindingSourceLocation &) {
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject binding = Property::makeBinding(env, property);
                return qtjambi_cast<QUntypedPropertyBinding>(env, binding);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
    return QUntypedPropertyBinding();
}

template<class Property>
void qtjambi_set_observer(const QUntypedPropertyData *d, QPropertyObserver *observer){
    try{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject bindingData = Property::bindingData(env, property);
                if(QtPrivate::QPropertyBindingData* data = qtjambi_cast<QtPrivate::QPropertyBindingData*>(env, bindingData))
                    observer->setSource(*data);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){}
}

template<int propertyType, class T>
struct BuiltInBindableInterface : QtPrivate::QBindableInterface{
    BuiltInBindableInterface() : QtPrivate::QBindableInterface{
                                     nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
                             }{}
};

template<class T>
struct BuiltInBindableInterface<0,T> : QtPrivate::QBindableInterface{
    BuiltInBindableInterface() : QtPrivate::QBindableInterface{
                                     &qtjambi_get_property_value<Java::QtCore::QProperty,T>,
                                     &qtjambi_set_property_value<Java::QtCore::QProperty,T>,
                                     &qtjambi_get_binding<Java::QtCore::QProperty>,
                                     &qtjambi_set_binding<Java::QtCore::QProperty>,
                                     &qtjambi_make_binding<Java::QtCore::QProperty>,
                                     &qtjambi_set_observer<Java::QtCore::QProperty>,
                                     &qtjambi_metatype<T>
                             }{}
};

template<class T>
struct BuiltInBindableInterface<1,T> : QtPrivate::QBindableInterface{
    BuiltInBindableInterface() : QtPrivate::QBindableInterface{
                                     &qtjambi_get_property_value<Java::QtCore::QObject$QProperty,T>,
                                     &qtjambi_set_property_value<Java::QtCore::QObject$QProperty,T>,
                                     &qtjambi_get_binding<Java::QtCore::QObject$QProperty>,
                                     &qtjambi_set_binding<Java::QtCore::QObject$QProperty>,
                                     &qtjambi_make_binding<Java::QtCore::QObject$QProperty>,
                                     &qtjambi_set_observer<Java::QtCore::QObject$QProperty>,
                                     &qtjambi_metatype<T>
                             }{}
};

template<class T>
struct BuiltInBindableInterface<2,T> : QtPrivate::QBindableInterface{
    BuiltInBindableInterface() : QtPrivate::QBindableInterface{
                                     &qtjambi_get_property_value<Java::QtCore::QObject$QComputedProperty,T>,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     &qtjambi_set_observer<Java::QtCore::QObject$QAbstractComputedProperty>,
                                     &qtjambi_metatype<T>
                             }{}
};

template<int propertyType, class T>
struct PrimitiveBindableHelper{
};

template<>
struct PrimitiveBindableHelper<0,bool>{
    using Property = Java::QtCore::QBooleanProperty;
    using PropertyBinding = Java::QtCore::QBooleanPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<0,qint8>{
    using Property = Java::QtCore::QByteProperty;
    using PropertyBinding = Java::QtCore::QBytePropertyBinding;
};

template<>
struct PrimitiveBindableHelper<0,qint16>{
    using Property = Java::QtCore::QShortProperty;
    using PropertyBinding = Java::QtCore::QShortPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<0,qint32>{
    using Property = Java::QtCore::QIntProperty;
    using PropertyBinding = Java::QtCore::QIntPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<0,qint64>{
    using Property = Java::QtCore::QLongProperty;
    using PropertyBinding = Java::QtCore::QLongPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<0,char16_t>{
    using Property = Java::QtCore::QCharProperty;
    using PropertyBinding = Java::QtCore::QCharPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<0,double>{
    using Property = Java::QtCore::QDoubleProperty;
    using PropertyBinding = Java::QtCore::QDoublePropertyBinding;
};

template<>
struct PrimitiveBindableHelper<0,float>{
    using Property = Java::QtCore::QFloatProperty;
    using PropertyBinding = Java::QtCore::QFloatPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,bool>{
    using Property = Java::QtCore::QObject$QBooleanProperty;
    using PropertyBinding = Java::QtCore::QBooleanPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,qint8>{
    using Property = Java::QtCore::QObject$QByteProperty;
    using PropertyBinding = Java::QtCore::QBytePropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,qint16>{
    using Property = Java::QtCore::QObject$QShortProperty;
    using PropertyBinding = Java::QtCore::QShortPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,qint32>{
    using Property = Java::QtCore::QObject$QIntProperty;
    using PropertyBinding = Java::QtCore::QIntPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,qint64>{
    using Property = Java::QtCore::QObject$QLongProperty;
    using PropertyBinding = Java::QtCore::QLongPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,char16_t>{
    using Property = Java::QtCore::QObject$QCharProperty;
    using PropertyBinding = Java::QtCore::QCharPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,double>{
    using Property = Java::QtCore::QObject$QDoubleProperty;
    using PropertyBinding = Java::QtCore::QDoublePropertyBinding;
};

template<>
struct PrimitiveBindableHelper<1,float>{
    using Property = Java::QtCore::QObject$QFloatProperty;
    using PropertyBinding = Java::QtCore::QFloatPropertyBinding;
};

template<>
struct PrimitiveBindableHelper<2,bool>{
    using Property = Java::QtCore::QObject$QComputedBooleanProperty;
};

template<>
struct PrimitiveBindableHelper<2,qint8>{
    using Property = Java::QtCore::QObject$QComputedByteProperty;
};

template<>
struct PrimitiveBindableHelper<2,qint16>{
    using Property = Java::QtCore::QObject$QComputedShortProperty;
};

template<>
struct PrimitiveBindableHelper<2,qint32>{
    using Property = Java::QtCore::QObject$QComputedIntProperty;
};

template<>
struct PrimitiveBindableHelper<2,qint64>{
    using Property = Java::QtCore::QObject$QComputedLongProperty;
};

template<>
struct PrimitiveBindableHelper<2,char16_t>{
    using Property = Java::QtCore::QObject$QComputedCharProperty;
};

template<>
struct PrimitiveBindableHelper<2,double>{
    using Property = Java::QtCore::QObject$QComputedDoubleProperty;
};

template<>
struct PrimitiveBindableHelper<2,float>{
    using Property = Java::QtCore::QObject$QComputedFloatProperty;
};

template<int propertyType, class T>
struct PrimitiveBindableInterfaceHelper : QtPrivate::QBindableInterface{
    using Property = typename PrimitiveBindableHelper<propertyType,T>::Property;
    using PropertyBinding = typename PrimitiveBindableHelper<propertyType,T>::PropertyBinding;
    static QtPrivate::QBindableInterface create() {
        return {
                                       /*getter=*/ [](const QUntypedPropertyData *d, void *value){
                                                    try{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            try{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property))
                                                                    *static_cast<T*>(value) = Property::value(env, property);
                                                            }catch(const JavaException& exn){
                                                                exn.report(env);
                                                            }
                                                        }
                                                    }catch(...){}
                                                   },
                                       /*setter=*/ [](QUntypedPropertyData *d, const void *value){
                                                    try{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            try{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property))
                                                                    Property::setValue(env, property, *static_cast<const T*>(value));
                                                            }catch(const JavaException& exn){
                                                                exn.report(env);
                                                            }
                                                        }
                                                    }catch(...){}
                                           },
                                       /*getBinding=*/ [](const QUntypedPropertyData *d) -> QUntypedPropertyBinding {
                                                    try{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            try{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property)){
                                                                    jobject binding = Property::binding(env, property);
                                                                    return qtjambi_cast<QUntypedPropertyBinding>(env, binding);
                                                                }
                                                            }catch(const JavaException& exn){
                                                                exn.report(env);
                                                            }
                                                        }
                                                    }catch(...){}
                                               return QUntypedPropertyBinding();
                                           },
                                       /*setBinding=*/ [](QUntypedPropertyData *d, const QUntypedPropertyBinding &binding) -> QUntypedPropertyBinding {
                                                    try{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            try{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property)){
                                                                   jobject _binding = qtjambi_cast<jobject>(env, binding);
                                                                   jobject oldBinding = Property::setBinding(env, property, PropertyBinding::newInstance(env, _binding));
                                                                   return qtjambi_cast<QUntypedPropertyBinding>(env, oldBinding);
                                                                }
                                                            }catch(const JavaException& exn){
                                                                exn.report(env);
                                                            }
                                                        }
                                                    }catch(...){}
                                               return QUntypedPropertyBinding();
                                           },
                                       /*makeBinding=*/ [](const QUntypedPropertyData *d, const QPropertyBindingSourceLocation &) -> QUntypedPropertyBinding{
                                                    try{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            try{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property)){
                                                                    jobject binding = Property::makeBinding(env, property);
                                                                    return qtjambi_cast<QUntypedPropertyBinding>(env, binding);
                                                                }
                                                            }catch(const JavaException& exn){
                                                                exn.report(env);
                                                            }
                                                        }
                                                    }catch(...){}
                                               return QUntypedPropertyBinding();
                                           },
                                       /*setObserver=*/ [](const QUntypedPropertyData *d, QPropertyObserver *observer){
                                                try{
                                                    if(JNIEnv* env = qtjambi_current_environment()){
                                                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                        try{
                                                           jobject property = qtjambi_find_object(env, d);
                                                           if(Property::isInstanceOf(env, property)){
                                                                jobject bindingData = Property::bindingData(env, property);
                                                                if(QtPrivate::QPropertyBindingData* data = qtjambi_cast<QtPrivate::QPropertyBindingData*>(env, bindingData))
                                                                    observer->setSource(*data);
                                                           }
                                                        }catch(const JavaException& exn){
                                                            exn.report(env);
                                                        }
                                                    }
                                                }catch(...){}
                                           },
                                        &qtjambi_metatype<T>
                             };
        }
};

template<class T>
struct PrimitiveBindableInterfaceHelper<2, T> : QtPrivate::QBindableInterface{
    using Property = typename PrimitiveBindableHelper<2,T>::Property;
    static QtPrivate::QBindableInterface create () {
        return {
                                       /*getter=*/ [](const QUntypedPropertyData *d, void *value){
                                                        try{
                                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                                try{
                                                                   jobject property = qtjambi_cast<jobject>(env, d);
                                                                   *static_cast<T*>(value) = Property::value(env, property);
                                                                }catch(const JavaException& exn){
                                                                    exn.report(env);
                                                                }
                                                            }
                                                        }catch(...){}
                                                   },
                                       /*setter=*/ nullptr,
                                       /*getBinding=*/ nullptr,
                                       /*setBinding=*/ nullptr,
                                       /*makeBinding=*/ nullptr,
                                       /*setObserver=*/ [](const QUntypedPropertyData *d, QPropertyObserver *observer){
                                                    try{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            try{
                                                                jobject property = qtjambi_cast<jobject>(env, d);
                                                                jobject bindingData = Java::QtCore::QObject$QAbstractComputedProperty::bindingData(env, property);
                                                                if(QtPrivate::QPropertyBindingData* data = qtjambi_cast<QtPrivate::QPropertyBindingData*>(env, bindingData))
                                                                    observer->setSource(*data);
                                                            }catch(const JavaException& exn){
                                                                exn.report(env);
                                                            }
                                                        }
                                                    }catch(...){}
                                            },
                                        &qtjambi_metatype<T>
            };
    }
};

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gLock, (QReadWriteLock::Recursive))
typedef QMap<size_t, const QtPrivate::QBindableInterface *> BindableInterfacesHash;
Q_GLOBAL_STATIC(BindableInterfacesHash, gBindableInterfacesHash)

#define BUILTIN_BINDING_INTERFACE(TYPE)\
    switch(propertyType){\
    case 0:{\
        static QtPrivate::QBindableInterface _iface = BuiltInBindableInterface<0,TYPE>();\
        iface = &_iface;\
    }\
    break;\
    case 1:{\
        static QtPrivate::QBindableInterface _iface = BuiltInBindableInterface<1,TYPE>();\
        iface = &_iface;\
    }\
    break;\
    case 2:{\
        static QtPrivate::QBindableInterface _iface = BuiltInBindableInterface<2,TYPE>();\
        iface = &_iface;\
    }\
    break;\
    }

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBindable_bindableInterface)
(JNIEnv *__jni_env,
 jclass,
 jint metaType,
 jint propertyType
 )
{
    try{
        const QtPrivate::QBindableInterface *iface(nullptr);
        switch(metaType){
        case QMetaType::UnknownType:
        case QMetaType::Nullptr:        break;
        case QMetaType::Long:
        case QMetaType::ULong:          switch(QMetaType(QMetaType::ULong).sizeOf()){
                                        case 16: BUILTIN_BINDING_INTERFACE(qint16) break;
                                        case 64: BUILTIN_BINDING_INTERFACE(qint64) break;
                                        }
                                        Q_FALLTHROUGH();
        case QMetaType::UInt:
        case QMetaType::Char32:
        case QMetaType::Int:            BUILTIN_BINDING_INTERFACE(qint32) break;
        case QMetaType::UShort:
        case QMetaType::Short:          BUILTIN_BINDING_INTERFACE(qint16) break;
        case QMetaType::UChar:
        case QMetaType::Char:
        case QMetaType::SChar:          BUILTIN_BINDING_INTERFACE(qint8) break;
        case QMetaType::ULongLong:
        case QMetaType::LongLong:       BUILTIN_BINDING_INTERFACE(qint64) break;
        case QMetaType::Char16:
        case QMetaType::QChar:          BUILTIN_BINDING_INTERFACE(QChar) break;
        case QMetaType::Bool:           BUILTIN_BINDING_INTERFACE(bool) break;
        case QMetaType::Double:         BUILTIN_BINDING_INTERFACE(double) break;
        case QMetaType::Float:          BUILTIN_BINDING_INTERFACE(float) break;
        case QMetaType::QString:        BUILTIN_BINDING_INTERFACE(QString) break;
        case QMetaType::QStringList:    BUILTIN_BINDING_INTERFACE(QStringList) break;
        case QMetaType::QByteArray:     BUILTIN_BINDING_INTERFACE(QByteArray) break;
        case QMetaType::QByteArrayList: BUILTIN_BINDING_INTERFACE(QByteArrayList) break;
        case QMetaType::QVariant:       BUILTIN_BINDING_INTERFACE(QVariant) break;
        case QMetaType::QVariantList:   BUILTIN_BINDING_INTERFACE(QVariantList) break;
        case QMetaType::QVariantMap:    BUILTIN_BINDING_INTERFACE(QVariantMap) break;
        case QMetaType::QVariantHash:   BUILTIN_BINDING_INTERFACE(QVariantHash) break;
        case QMetaType::QVariantPair:   BUILTIN_BINDING_INTERFACE(QVariantPair) break;
        case QMetaType::QObjectStar:    BUILTIN_BINDING_INTERFACE(QObject*) break;
        case QMetaType::VoidStar:       BUILTIN_BINDING_INTERFACE(void*) break;
        default: {
                hash_type _hash = qHash(metaType);
                _hash = _hash * 31 + qHash(propertyType);
                {
                    QReadLocker locker(gLock());
                    Q_UNUSED(locker)
                    iface = gBindableInterfacesHash->value(_hash);
                }
                if (!iface) {
                    QtPrivate::QBindableInterface::GetMetaType getMetaType = /*metaType=*/ qtjambi_function_pointer<64, QMetaType()>([metaType]()->QMetaType { return QMetaType(metaType); }, hash_type(metaType));
                    switch(propertyType){
                    case 0:
                        iface = new QtPrivate::QBindableInterface{
                                &qtjambi_get_qproperty_value<Java::QtCore::QProperty>,
                                &qtjambi_set_qproperty_value<Java::QtCore::QProperty>,
                                &qtjambi_get_binding<Java::QtCore::QProperty>,
                                &qtjambi_set_binding<Java::QtCore::QProperty>,
                                &qtjambi_make_binding<Java::QtCore::QProperty>,
                                &qtjambi_set_observer<Java::QtCore::QProperty>,
                                getMetaType
                        };
                        break;
                    case 1:
                        iface = new QtPrivate::QBindableInterface{
                                &qtjambi_get_qproperty_value<Java::QtCore::QObject$QProperty>,
                                &qtjambi_set_qproperty_value<Java::QtCore::QObject$QProperty>,
                                &qtjambi_get_binding<Java::QtCore::QObject$QProperty>,
                                &qtjambi_set_binding<Java::QtCore::QObject$QProperty>,
                                &qtjambi_make_binding<Java::QtCore::QObject$QProperty>,
                                &qtjambi_set_observer<Java::QtCore::QObject$QProperty>,
                                getMetaType
                        };
                        break;
                    case 2:
                        iface = new QtPrivate::QBindableInterface{
                                &qtjambi_get_qproperty_value<Java::QtCore::QObject$QComputedProperty>,
                                nullptr,
                                nullptr,
                                nullptr,
                                nullptr,
                                nullptr,
                                getMetaType
                        };
                        break;
                    }

                    QWriteLocker locker(gLock());
                    Q_UNUSED(locker)
                    if(const QtPrivate::QBindableInterface *_iface = gBindableInterfacesHash->value(_hash)){
                        delete iface;
                        iface = _iface;
                    }else{
                        gBindableInterfacesHash->insert(_hash, iface);
                    }
                }
            }
            break;
        }
        return qtjambi_cast<jobject>(__jni_env, iface);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

#define PRIMITIVE_BINDING_INTERFACE_METHOD(UTYPE,TYPE)\
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##UTYPE##Bindable_bindableInterface)\
(JNIEnv *__jni_env, jclass, jint propertyType)\
{\
    try{\
    switch(propertyType){\
    case 0:{\
        static QtPrivate::QBindableInterface _iface = PrimitiveBindableInterfaceHelper<0,TYPE>::create();\
        return qtjambi_cast<jobject>(__jni_env, &_iface);\
    }\
    break;\
    case 1:{\
        static QtPrivate::QBindableInterface _iface = PrimitiveBindableInterfaceHelper<1,TYPE>::create();\
        return qtjambi_cast<jobject>(__jni_env, &_iface);\
    }\
    break;\
    case 2:{\
        static QtPrivate::QBindableInterface _iface = PrimitiveBindableInterfaceHelper<2,TYPE>::create();\
        return qtjambi_cast<jobject>(__jni_env, &_iface);\
    }\
    break;\
    }\
    }catch(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }\
    return nullptr;\
}

PRIMITIVE_BINDING_INTERFACE_METHOD(Boolean,bool)
PRIMITIVE_BINDING_INTERFACE_METHOD(Byte,qint8)
PRIMITIVE_BINDING_INTERFACE_METHOD(Short,qint16)
PRIMITIVE_BINDING_INTERFACE_METHOD(Int,qint32)
PRIMITIVE_BINDING_INTERFACE_METHOD(Long,qint64)
PRIMITIVE_BINDING_INTERFACE_METHOD(Double,double)
PRIMITIVE_BINDING_INTERFACE_METHOD(Float,float)
PRIMITIVE_BINDING_INTERFACE_METHOD(Char,char16_t)

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QAbstractPropertyAlias_setObserver)
(JNIEnv*,
 jclass,
 QtJambiNativeID thisId,
 QtJambiNativeID aliasedPropertyId,
 QtJambiNativeID ifaceId)
{
    QPropertyObserver *__qt_this = qtjambi_object_from_nativeId<QPropertyObserver>(thisId);
    const QUntypedPropertyData *aliasedProperty = qtjambi_object_from_nativeId<QUntypedPropertyData>(aliasedPropertyId);
    const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<QtPrivate::QBindableInterface>(ifaceId);
    if (iface && iface->setObserver)
        iface->setObserver(aliasedProperty, __qt_this);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBindableInterface_metaType)
(JNIEnv* __jni_env,
 jclass,
 QtJambiNativeID ifaceId)
{
    try{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<QtPrivate::QBindableInterface>(ifaceId);
        if (iface && iface->metaType)
            return qtjambi_cast<jobject>(__jni_env, iface->metaType());
        else return qtjambi_cast<jobject>(__jni_env, QMetaType());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

template<class Property>
void signal_callback(QUntypedPropertyData *d){
    if(d){
        try{
            if(JNIEnv *env = qtjambi_current_environment()) {
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                try{
                    jobject property = qtjambi_find_object(env, d);
                    if(Property::isInstanceOf(env, property))
                        Property::emitSignal(env, property);
                }catch(const JavaException& exn){
                    exn.report(env);
                }
            }
        }catch(...){}
    }
}

#define IF_IS_RETURN(PROPERTY)\
    if(Java::QtCore::QObject$##PROPERTY::isInstanceOf(env, property) && Java::QtCore::QObject$##PROPERTY::hasSignal(env, property))\
        return &signal_callback<Java::QtCore::QObject$##PROPERTY>;

QtPrivate::QPropertyObserverCallback qtjambi_get_signal_callback(JNIEnv *env, QUntypedPropertyData *d){
    jobject property = qtjambi_cast<jobject>(env, d);
    IF_IS_RETURN(QProperty)
    else IF_IS_RETURN(QBooleanProperty)
    else IF_IS_RETURN(QByteProperty)
    else IF_IS_RETURN(QShortProperty)
    else IF_IS_RETURN(QIntProperty)
    else IF_IS_RETURN(QLongProperty)
    else IF_IS_RETURN(QFloatProperty)
    else IF_IS_RETURN(QDoubleProperty)
    else IF_IS_RETURN(QCharProperty)
    else return nullptr;
}

void __qt_construct_QPropertyObserver_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);
void __qt_construct_QPropertyChangeHandler(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*);
void deleter_QPropertyObserver(void *ptr);

// QPropertyObserver::QPropertyObserver(QUntypedPropertyData * aliasedPropertyPtr)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPropertyObserver__1_1qt_1QPropertyObserver_1newAlias)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject aliasedPropertyPtr0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPropertyObserver::QPropertyObserver(QUntypedPropertyData *)")
    try{
        jvalue arguments;
        arguments.l = aliasedPropertyPtr0;
        const std::type_info* typeId;
        if(Java::QtCore::QBooleanPropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBooleanPropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<bool>);
        }else if(Java::QtCore::QBytePropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QBytePropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<qint8>);
        }else if(Java::QtCore::QShortPropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QShortPropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<qint16>);
        }else if(Java::QtCore::QIntPropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QIntPropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<qint32>);
        }else if(Java::QtCore::QLongPropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QLongPropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<qint64>);
        }else if(Java::QtCore::QFloatPropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QFloatPropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<float>);
        }else if(Java::QtCore::QDoublePropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QDoublePropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<double>);
        }else if(Java::QtCore::QCharPropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QCharPropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<QChar>);
        }else if(Java::QtCore::QPropertyAlias::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QPropertyAlias::getClass(__jni_env);
            typeId = &typeid(QPropertyAlias<QVariant>);
        }else{
            typeId = &typeid(QPropertyObserver);
        }
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_construct_QPropertyObserver_1, sizeof(QPropertyObserver), *typeId, true, &deleter_QPropertyObserver, &arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QPropertyChangeHandler::QPropertyChangeHandler()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPropertyObserver__1_1qt_1QPropertyObserver_1newPropertyChangeHandler)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPropertyChangeHandler::QPropertyChangeHandler()")
    try{
        const std::type_info* typeId;
        if(Java::QtCore::QPropertyChangeHandler::isInstanceOf(__jni_env, __jni_object)){
            __jni_class = Java::QtCore::QPropertyChangeHandler::getClass(__jni_env);
            typeId = &typeid(QPropertyChangeHandler<void(*)()>);
        }else{
            typeId = &typeid(QPropertyObserver);
        }
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_construct_QPropertyChangeHandler, sizeof(QPropertyObserver), *typeId, true, &deleter_QPropertyObserver, nullptr);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

#define PRIMITIVE_PROPERTY_DATA(Boxed,jprimitive,primitive,jvalMember)\
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##Boxed##PropertyData__1_1qt_1new)\
(JNIEnv *__jni_env,\
 jclass __jni_class,\
 jobject __jni_object,\
 jprimitive val)\
{\
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Q" #Boxed "PropertyData::Q" #Boxed "PropertyData()")\
    jvalue args;\
    args.jvalMember = val;\
    try{\
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object,\
                                         [](void* __qtjambi_ptr, JNIEnv*, jobject, jvalue* __java_arguments){\
                                            Q_ASSERT(__java_arguments);\
                                            new(__qtjambi_ptr) QPropertyData<primitive>(__java_arguments->jvalMember);\
                                         },\
                                        qMax(sizeof(QUntypedPropertyData), sizeof(QPropertyData<primitive>)), typeid(QUntypedPropertyData), false,\
                                        [](void *ptr){ delete reinterpret_cast<QPropertyData<primitive> *>(ptr); }, &args);\
    }catch(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }\
}\
extern "C" Q_DECL_EXPORT jprimitive JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##Boxed##PropertyData_getValueBypassingBindings)\
(JNIEnv *__jni_env,\
 jclass,\
 QtJambiNativeID thisId)\
{\
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Q" #Boxed "PropertyData::getValueBypassingBindings()")\
    try{\
        QPropertyData<primitive> *__qt_this = qtjambi_object_from_nativeId<QPropertyData<primitive>>(thisId);\
        qtjambi_check_resource(__jni_env, __qt_this);\
        return __qt_this->valueBypassingBindings();\
    }catch(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }\
    return jprimitive{};\
}\
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##Boxed##PropertyData_setValueBypassingBindings)\
(JNIEnv *__jni_env,\
 jclass,\
 QtJambiNativeID thisId,\
 jprimitive val\
 )\
{\
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Q" #Boxed "PropertyData::setValueBypassingBindings(value)")\
    try{\
        QPropertyData<primitive> *__qt_this = qtjambi_object_from_nativeId<QPropertyData<primitive>>(thisId);\
        qtjambi_check_resource(__jni_env, __qt_this);\
        if(primitive(val)==__qt_this->valueBypassingBindings())\
            return false;\
        __qt_this->setValueBypassingBindings(val);\
        return true;\
    }catch(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }\
    return false;\
}

PRIMITIVE_PROPERTY_DATA(Boolean,jboolean,bool,z)
PRIMITIVE_PROPERTY_DATA(Byte,jbyte,qint8,b)
PRIMITIVE_PROPERTY_DATA(Short,jshort,qint16,s)
PRIMITIVE_PROPERTY_DATA(Int,jint,qint32,i)
PRIMITIVE_PROPERTY_DATA(Long,jlong,qint64,j)
PRIMITIVE_PROPERTY_DATA(Float,jfloat,float,f)
PRIMITIVE_PROPERTY_DATA(Double,jdouble,double,d)
PRIMITIVE_PROPERTY_DATA(Char,jchar,char16_t,c)

typedef QMap<void*, QMetaType> MetaTypesByPointerHash;
Q_GLOBAL_STATIC(MetaTypesByPointerHash, gMetaTypesByPointer)

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QProperty__1_1qt_1new)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject __metaType,
 jobject val)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QProperty::QProperty()")
    jvalue args[2];
    args[0].l = __metaType;
    args[1].l = val;
    try{
        QMetaType& metaType = qtjambi_cast<QMetaType&>(__jni_env, __metaType);
        if(metaType.flags() & QMetaType::IsPointer){
            qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object,
                                             [](void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* __java_arguments){
                                                Q_ASSERT(__java_arguments);
                                                void* _result;
                                                if(__java_arguments[1].l){
                                                    QMetaType& metaType = qtjambi_cast<QMetaType&>(env, __java_arguments[0].l);
                                                    QVariant variant = qtjambi_to_qvariant(env, __java_arguments[1].l);
                                                    if(variant.metaType()!=metaType && !variant.convert(metaType)){
                                                        _result = nullptr;
                                                    } else {
                                                        _result = *reinterpret_cast<void*const*>(variant.constData());
                                                    }
                                                }else{
                                                    _result = nullptr;
                                                }
                                                new (__qtjambi_ptr) QPropertyData<void*>(_result);
                                             },
                                            sizeof(QPropertyData<void*>), typeid(QUntypedPropertyData), false,
                                            [](void *ptr){
                                                delete reinterpret_cast<QPropertyData<void*>*>(ptr);
                                            }, args);
        }else{
            qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object,
                                             [](void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* __java_arguments){
                                                Q_ASSERT(__java_arguments);
                                                QMetaType& metaType = qtjambi_cast<QMetaType&>(env, __java_arguments[0].l);
                                                if(__java_arguments[1].l){
                                                    QVariant variant = qtjambi_cast<QVariant>(env, __java_arguments[1].l);
                                                    if(variant.metaType()!=metaType && !variant.convert(metaType)){
                                                        metaType.construct(__qtjambi_ptr);
                                                    }else{
                                                        metaType.construct(__qtjambi_ptr, variant.constData());
                                                    }
                                                }else{
                                                    metaType.construct(__qtjambi_ptr);
                                                }
                                                QWriteLocker locker(gLock());
                                                gMetaTypesByPointer->insert(__qtjambi_ptr, metaType);
                                             },
                                            qMax(sizeof(QUntypedPropertyData), size_t(metaType.sizeOf())), typeid(QUntypedPropertyData), false,
                                            [](void *ptr){
                                                QMetaType metaType;
                                                {
                                                    QWriteLocker locker(gLock());
                                                    metaType = gMetaTypesByPointer->take(ptr);
                                                }
                                                metaType.destruct(ptr);
                                                operator delete(ptr);
                                            }, args);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QProperty_getValueBypassingBindings)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID thisId,
 QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QProperty::getValueBypassingBindings()")
    try{
        void *__qt_this = qtjambi_from_nativeId(thisId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QMetaType metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
        if(metaType.flags() & QMetaType::IsPointer){
            void* value = static_cast<QPropertyData<void*> *>(__qt_this)->valueBypassingBindings();
            return qtjambi_cast<jobject>(__jni_env, QVariant(metaType, &value));
        }else{
            switch(metaType.id()){
            case QMetaType::QByteArray:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QByteArray> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QByteArrayList:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QByteArrayList> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QString:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QString> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QStringList:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QStringList> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QVariantList:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantList> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QVariantMap:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantMap> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QVariantHash:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantHash> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QVariantPair:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantPair> *>(__qt_this)->valueBypassingBindings());
            case QMetaType::QVariant:
                return qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariant> *>(__qt_this)->valueBypassingBindings());
            default:
                return qtjambi_cast<jobject>(__jni_env, QVariant(metaType, __qt_this));
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QProperty_setValueBypassingBindings)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID thisId,
 QtJambiNativeID metaTypeId,
 jobject val
 )
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QProperty::setValueBypassingBindings(value)")
    try{
        void *__qt_this = qtjambi_from_nativeId(thisId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QMetaType metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
        if(metaType.flags() & QMetaType::IsPointer){
            QPropertyData<void*> *propertyPtr = static_cast<QPropertyData<void*> *>(__qt_this);
            void* _val;
            if(val){
                QVariant variant = qtjambi_to_qvariant(__jni_env, val);
                if(variant.metaType()!=metaType && !variant.convert(metaType))
                    return false;
                _val = *reinterpret_cast<void*const*>(variant.constData());
            }else{
                _val = nullptr;
            }
            if (_val == propertyPtr->valueBypassingBindings())
                return false;
            propertyPtr->setValueBypassingBindings(_val);
            return true;
        }else{
            switch(metaType.id()){
            case QMetaType::QByteArray: {
                    QPropertyData<QByteArray> *propertyPtr = static_cast<QPropertyData<QByteArray> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArray>(__jni_env, val));
                    return true;
                }
            case QMetaType::QByteArrayList: {
                    QPropertyData<QByteArrayList> *propertyPtr = static_cast<QPropertyData<QByteArrayList> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArrayList>(__jni_env, val));
                    return true;
                }
            case QMetaType::QString: {
                    QPropertyData<QString> *propertyPtr = static_cast<QPropertyData<QString> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QString>(__jni_env, val));
                    return true;
                }
            case QMetaType::QStringList: {
                    QPropertyData<QStringList> *propertyPtr = static_cast<QPropertyData<QStringList> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QStringList>(__jni_env, val));
                    return true;
                }
            case QMetaType::QVariantList: {
                    QPropertyData<QVariantList> *propertyPtr = static_cast<QPropertyData<QVariantList> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantList>(__jni_env, val));
                    return true;
                }
            case QMetaType::QVariantMap: {
                    QPropertyData<QVariantMap> *propertyPtr = static_cast<QPropertyData<QVariantMap> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantMap>(__jni_env, val));
                    return true;
                }
            case QMetaType::QVariantHash: {
                    QPropertyData<QVariantHash> *propertyPtr = static_cast<QPropertyData<QVariantHash> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantHash>(__jni_env, val));
                    return true;
                }
            case QMetaType::QVariantPair: {
                    QPropertyData<QVariantPair> *propertyPtr = static_cast<QPropertyData<QVariantPair> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantPair>(__jni_env, val));
                    return true;
                }
            case QMetaType::QVariant: {
                   QPropertyData<QVariant> *propertyPtr = static_cast<QPropertyData<QVariant> *>(__qt_this);
                   propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariant>(__jni_env, val));
                   return true;
               }
            default: {
                   QVariant variant = val ? qtjambi_to_qvariant(__jni_env, val) : QVariant(metaType);
                   if(variant.metaType()!=metaType && !variant.convert(metaType))
                        return false;
                   if(metaType.isEqualityComparable() && metaType.equals(__qt_this, variant.constData()))
                        return false;
                   metaType.destruct(__qt_this);
                   metaType.construct(__qt_this, variant.constData());
               }
               return true;
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPromiseVoid__1_1qt_1future__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPromise::future() const")
    try{
        QPromise<void> *__qt_this = qtjambi_object_from_nativeId<QPromise<void>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->future());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPromise__1_1qt_1future__J)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPromise::future() const")
    try{
        QPromise<JObjectWrapper> *__qt_this = qtjambi_object_from_nativeId<QPromise<JObjectWrapper>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->future());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPromiseVoid_setException)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jthrowable throwable)
{
    try{
        QTJAMBI_DEBUG_METHOD_PRINT("native", "QPromise::setException(QException)")
        QPromise<void> *__qt_this = qtjambi_object_from_nativeId<QPromise<void>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->setException(JavaException(__jni_env, throwable));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPromise_setException)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jthrowable throwable)
{
    try{
        QTJAMBI_DEBUG_METHOD_PRINT("native", "QPromise::setException(QException)")
        QPromise<JObjectWrapper> *__qt_this = qtjambi_object_from_nativeId<QPromise<JObjectWrapper>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->setException(JavaException(__jni_env, throwable));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPromise_addResult)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject result, jint index)
{
    try{
        QTJAMBI_DEBUG_METHOD_PRINT("native", "QPromise::addResult(U,int)")
        QPromise<JObjectWrapper> *__qt_this = qtjambi_object_from_nativeId<QPromise<JObjectWrapper>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->addResult(JObjectWrapper(__jni_env, result), index);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

inline auto future_createFunction(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const QFuture<JObjectWrapper>& parentFuture)->JObjectWrapper{
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject _parentFuture = qtjambi_cast<jobject>(env, parentFuture);
            jobject result = Java::Runtime::Function::apply(env, _function, _parentFuture);
            return JObjectWrapper(env, result);
        }
        return JObjectWrapper();
    };
}

inline auto future_createConsumer(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const QFuture<JObjectWrapper>& parentFuture){
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject _parentFuture = qtjambi_cast<jobject>(env, parentFuture);
            Java::Runtime::Consumer::accept(env, _function, _parentFuture);
        }
    };
}

inline auto future_createExceptionHandler(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const std::exception& exception)->JObjectWrapper{
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jthrowable _exception = nullptr;
            if(const JavaException* javaException = dynamic_cast<const JavaException*>(&exception)){
                _exception = javaException->object();
            }else{
                try{
                    JavaException::raiseRuntimeException(env, exception.what() QTJAMBI_STACKTRACEINFO );
                }catch(const JavaException& exn){
                    _exception = exn.object();
                }
            }
            jobject result = Java::Runtime::Function::apply(env, _function, _exception);
            return JObjectWrapper(env, result);
        }
        return JObjectWrapper();
    };
}

inline auto future_createCancelHandler(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function]()->JObjectWrapper{
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject result = Java::Runtime::Supplier::get(env, _function);
            return JObjectWrapper(env, result);
        }
        return JObjectWrapper();
    };
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_then)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<JObjectWrapper> result = __qt_this->then(future_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenLaunch)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jint policy, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<JObjectWrapper> result = __qt_this->then(QtFuture::Launch(policy), future_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenPool)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID pool, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QThreadPool *_pool = qtjambi_object_from_nativeId<QThreadPool>(pool);
        QFuture<JObjectWrapper> result = __qt_this->then(_pool, future_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<void> result = __qt_this->then(future_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenLaunchVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jint policy, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<void> result = __qt_this->then(QtFuture::Launch(policy), future_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenPoolVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID pool, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QThreadPool *_pool = qtjambi_object_from_nativeId<QThreadPool>(pool);
        QFuture<void> result = __qt_this->then(_pool, future_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onFailed)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<JObjectWrapper> result = __qt_this->onFailed(future_createExceptionHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onCanceled)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<JObjectWrapper> result = __qt_this->onCanceled(future_createCancelHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<JObjectWrapper> result = __qt_this->then(context, future_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenContextVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<void> result = __qt_this->then(context, future_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onFailedContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<JObjectWrapper> result = __qt_this->onFailed(context, future_createExceptionHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onCanceledContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QtJambiFuture *__qt_this = qtjambi_object_from_nativeId<QtJambiFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<JObjectWrapper> result = __qt_this->onCanceled(context, future_createCancelHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}
#endif

inline auto futurevoid_createFunction(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const QFuture<void>& future)->JObjectWrapper{
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject _future = qtjambi_cast<jobject>(env, future);
            jobject result = Java::Runtime::Function::apply(env, _function, _future);
            return JObjectWrapper(env, result);
        }
        return JObjectWrapper();
    };
}

inline auto futurevoid_createConsumer(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const QFuture<void>& future){
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject _future = qtjambi_cast<jobject>(env, future);
            Java::Runtime::Consumer::accept(env, _function, _future);
        }
    };
}

inline auto futurevoid_createCancelHandler(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](){
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            Java::Runtime::Runnable::run(env, _function);
        }
    };
}

inline auto futurevoid_createExceptionHandler(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const std::exception& exception){
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jthrowable _exception = nullptr;
            if(const JavaException* javaException = dynamic_cast<const JavaException*>(&exception)){
                _exception = javaException->object();
            }else{
                try{
                    JavaException::raiseRuntimeException(env, exception.what() QTJAMBI_STACKTRACEINFO );
                }catch(const JavaException& exn){
                    _exception = exn.object();
                }
            }
            Java::Runtime::Consumer::accept(env, _function, _exception);
        }
    };
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_then)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<JObjectWrapper> result = __qt_this->then(futurevoid_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_thenLaunch)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jint policy, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<JObjectWrapper> result = __qt_this->then(QtFuture::Launch(policy), futurevoid_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_thenPool)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID pool, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QThreadPool *_pool = qtjambi_object_from_nativeId<QThreadPool>(pool);
        QFuture<JObjectWrapper> result = __qt_this->then(_pool, futurevoid_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_thenVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<void> result = __qt_this->then(futurevoid_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_thenLaunchVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jint policy, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<void> result = __qt_this->then(QtFuture::Launch(policy), futurevoid_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_thenPoolVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID pool, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QThreadPool *_pool = qtjambi_object_from_nativeId<QThreadPool>(pool);
        QFuture<void> result = __qt_this->then(_pool, futurevoid_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_onFailed)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<void> result = __qt_this->onFailed(futurevoid_createExceptionHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_onCanceled)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QFuture<void> result = __qt_this->onCanceled(futurevoid_createCancelHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_thenContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<JObjectWrapper> result = __qt_this->then(context, futurevoid_createFunction(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_thenContextVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<void> result = __qt_this->then(context, futurevoid_createConsumer(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_onFailedContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<void> result = __qt_this->onFailed(context, futurevoid_createExceptionHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVoidFuture_onCanceledContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    try{
        QVoidFuture *__qt_this = qtjambi_object_from_nativeId<QVoidFuture>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        QFuture<void> result = __qt_this->onCanceled(context, futurevoid_createCancelHandler(__jni_env, function));
        return qtjambi_cast<jobject>(__jni_env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

#endif

// QModelRoleData::setData(T)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QModelRoleData__1_1qt_1QModelRoleData_1setData__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId, jobject data)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QModelRoleData::setData(T)")
    try{
#if QT_CONFIG(itemmodel)
        QModelRoleData *__qt_this = qtjambi_object_from_nativeId<QModelRoleData>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->data() = qtjambi_cast<QVariant>(__jni_env, data);
#else
        Q_UNUSED(__this_nativeId)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(itemmodel)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// new QByteArrayView(QByteArray)
void __qt_construct_QByteArrayView_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(QByteArray)")
    const QByteArray& data = qtjambi_cast<const QByteArray&>(__jni_env, __java_arguments->l);
    QByteArrayView *__qt_this = new(__qtjambi_ptr) QByteArrayView(data);
    Q_UNUSED(__jni_object)
    Q_UNUSED(__jni_env)
    Q_UNUSED(__qt_this)
}

// QByteArrayView::QByteArrayView(QByteArray)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1newByteArray__Ljava_lang_Object_2Lio_qt_core_QByteArray_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject data)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(QByteArray)")
    try{
        jvalue argument;
        argument.l = data;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_1, sizeof(QByteArrayView), typeid(QByteArrayView), false, &argument);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// new QByteArrayView(Buffer)
void __qt_construct_QByteArrayView_2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(Buffer)")
    void* address = __jni_env->GetDirectBufferAddress(__java_arguments->l);
    jlong capacity = __jni_env->GetDirectBufferCapacity(__java_arguments->l);
    if(capacity!=-1){
        new(__qtjambi_ptr) QByteArrayView(reinterpret_cast<char*>(address), capacity);
    }else{
        new(__qtjambi_ptr) QByteArrayView();
    }
    Q_UNUSED(__jni_object)
}

// QByteArrayView::QByteArrayView(Buffer)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1newDirectBuffer__Ljava_lang_Object_2Ljava_nio_ByteBuffer_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject data)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(Buffer)")
    try{
        jvalue argument;
        argument.l = data;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_2, sizeof(QByteArrayView), typeid(QByteArrayView), false, &argument);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// new QByteArrayView(Buffer)
void __qt_construct_QByteArrayView_3(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(Buffer)")
    JBufferConstData* bufferData = new JBufferConstData(__jni_env, __java_arguments[0].l);
    if(bufferData->size()>0){
        jobject address = qtjambi_from_long(__jni_env, qintptr(bufferData));
        __jni_env->SetObjectArrayElement(jobjectArray(__java_arguments[1].l), 0, address);
        qtjambi_throw_java_exception(__jni_env)
        new(__qtjambi_ptr) QByteArrayView(reinterpret_cast<const char*>(bufferData->data()), bufferData->size());
    }else{
        delete bufferData;
        new(__qtjambi_ptr) QByteArrayView();
    }
    Q_UNUSED(__jni_object)
}

// QByteArrayView::QByteArrayView(Buffer)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1newBuffer__Ljava_lang_Object_2Ljava_nio_Buffer_2_3Ljava_lang_Long_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject data, jobjectArray pointerOut)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(Buffer)")
    try{
        jvalue argument[2];
        argument[0].l = data;
        argument[1].l = pointerOut;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_3, sizeof(QByteArrayView), typeid(QByteArrayView), false, argument);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1purgeBuffer__J)
(JNIEnv * __jni_env, jclass, jlong pointer)
{
    try{
        JBufferConstData* bufferData = reinterpret_cast<JBufferConstData*>(pointer);
        delete bufferData;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// new QByteArrayView(String)
void __qt_construct_QByteArrayView_4(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(String)")
    J2CStringBuffer* bufferData = new J2CStringBuffer(__jni_env, jstring(__java_arguments[0].l));
    if(bufferData->length()>0){
        jobject address = qtjambi_from_long(__jni_env, qintptr(bufferData));
        __jni_env->SetObjectArrayElement(jobjectArray(__java_arguments[1].l), 0, address);
        qtjambi_throw_java_exception(__jni_env)
        new(__qtjambi_ptr) QByteArrayView(reinterpret_cast<const char*>(bufferData->data()), bufferData->length());
    }else{
        delete bufferData;
        new(__qtjambi_ptr) QByteArrayView();
    }
    Q_UNUSED(__jni_object)
}

// QByteArrayView::QByteArrayView(String)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1newString__Ljava_lang_Object_2Ljava_lang_String_2_3Ljava_lang_Long_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring data, jobjectArray pointerOut)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(String)")
    try{
        jvalue argument[2];
        argument[0].l = data;
        argument[1].l = pointerOut;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_4, sizeof(QByteArrayView), typeid(QByteArrayView), false, argument);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1purgeString__J)
(JNIEnv * __jni_env, jclass, jlong pointer)
{
    try{
        J2CStringBuffer* bufferData = reinterpret_cast<J2CStringBuffer*>(pointer);
        delete bufferData;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// new QByteArrayView(byte[])
void __qt_construct_QByteArrayView_5(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(byte[])")
    JByteArrayPointer* bufferData = new JByteArrayPointer(__jni_env, jbyteArray(__java_arguments[0].l));
    if(bufferData->size()>0){
        jobject address = qtjambi_from_long(__jni_env, qintptr(bufferData));
        __jni_env->SetObjectArrayElement(jobjectArray(__java_arguments[1].l), 0, address);
        qtjambi_throw_java_exception(__jni_env)
        new(__qtjambi_ptr) QByteArrayView(reinterpret_cast<const char*>(bufferData), bufferData->size());
    }else{
        delete bufferData;
        new(__qtjambi_ptr) QByteArrayView();
    }
    Q_UNUSED(__jni_object)
}

// QByteArrayView::QByteArrayView(byte[])
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1newBytes__Ljava_lang_Object_2_3B_3Ljava_lang_Long_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring data, jobjectArray pointerOut)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(byte[])")
    try{
        jvalue argument[2];
        argument[0].l = data;
        argument[1].l = pointerOut;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_5, sizeof(QByteArrayView), typeid(QByteArrayView), false, argument);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView__1_1qt_1QByteArrayView_1purgeBytes__J)
(JNIEnv * __jni_env, jclass, jlong pointer)
{
    try{
        JByteArrayPointer* bufferData = reinterpret_cast<JByteArrayPointer*>(pointer);
        delete bufferData;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterfaceBase_reportException)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jthrowable throwable)
{
    try{
        QFutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->reportException(JavaException(__jni_env, throwable));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterfaceBase_setContinuation)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject continuation)
{
    try{
        struct FutureInterfaceBase : QFutureInterfaceBase{
            void setContinuation(std::function<void(const QFutureInterfaceBase &)> func){
                QFutureInterfaceBase::setContinuation(func);
            }
        };

        FutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<FutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        JObjectWrapper cont(__jni_env, continuation);
        __qt_this->setContinuation([cont](const QFutureInterfaceBase &base){
            try{
                if(JNIEnv * env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                    try{
                        Java::Runtime::Consumer::accept(env, cont, qtjambi_cast<jobject>(env, base));
                    }catch(const JavaException& exn){
                        exn.report(env);
                    }
                }
            }catch(...){}
        });
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

#endif
