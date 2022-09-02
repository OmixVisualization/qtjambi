#include <qglobal.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/QIODevice>
#include <QtCore/QFuture>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QModelRoleData>
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
#include <QtCore/QOperatingSystemVersion>
#endif
#include <QtCore/QVariant>
#include <QtCore/QCalendar>
#include <qtjambi/qtjambi_application.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_thread.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include "qtjambi_core_repository.h"
#include "qtjambi_core_qhashes.h"
#include "qtjambi_stringutil.h"
#include <QtCore/private/qcoreapplication_p.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#include <QtCore/qcoreapplication_platform.h>
#endif

#include <qtjambi/qtjambi_cast.h>

QT_WARNING_DISABLE_GCC("-Wfloat-equal")
QT_WARNING_DISABLE_CLANG("-Wfloat-equal")

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_preinit)
(JNIEnv *__jni_env, jclass)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCoreApplication::preinit()")
    QTJAMBI_TRY{
        qtjambi_initialize_main_thread(__jni_env);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QAbstractItemModel_setData)
(JNIEnv *__jni_env, jobject, QtJambiNativeID _this_nativeId, jint row, jint col, jobject value, jint role)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QAbstractItemModel *__qt_this = qtjambi_object_from_nativeId<QAbstractItemModel>(_this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QVariant qt_value = qtjambi_to_qvariant(__jni_env, value);
        QModelIndex index = __qt_this->index(row, col, QModelIndex());
        _result = __qt_this->setData(index, qt_value, role);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QAbstractItemModel_data)
(JNIEnv *__jni_env, jobject, QtJambiNativeID _this_nativeId, jint row, jint col, jint role)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QAbstractItemModel *__qt_this = qtjambi_object_from_nativeId<QAbstractItemModel>(_this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QModelIndex index = __qt_this->index(row, col, QModelIndex());
        _result = qtjambi_from_qvariant(__jni_env, __qt_this->data(index, role));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCalendar_unspecified__)
(JNIEnv *, jclass)
{
    return jint(QCalendar::Unspecified);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QString_toUtf8)
(JNIEnv *__jni_env, jclass, jobject string)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QString::toUtf8(string)")
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QString __qt_this = qtjambi_to_qstring(__jni_env, string);
        QByteArray __qt_return_value = __qt_this.toUtf8();
        __java_return_value = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QString_toLatin1)
(JNIEnv *__jni_env, jclass, jobject string)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QString::toUtf8(string)")
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QString __qt_this = qtjambi_to_qstring(__jni_env, string);
        QByteArray __qt_return_value = __qt_this.toUtf8();
        __java_return_value = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QString_toLocal8Bit)
(JNIEnv *__jni_env, jclass, jobject string)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QString::toUtf8(string)")
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QString __qt_this = qtjambi_to_qstring(__jni_env, string);
        QByteArray __qt_return_value = __qt_this.toUtf8();
        __java_return_value = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_bin)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::bin;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_oct)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::oct;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_dec)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::dec;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_hex)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::hex;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_showbase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::showbase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_forcesign)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::forcesign;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_forcepoint)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::forcepoint;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_noshowbase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::noshowbase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_noforcesign)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::noforcesign;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_noforcepoint)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::noforcepoint;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_uppercasebase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::uppercasebase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_uppercasedigits)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::uppercasedigits;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_lowercasebase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::lowercasebase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_lowercasedigits)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::lowercasedigits;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_fixed)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::fixed;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_scientific)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::scientific;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_left)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::left;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_right)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::right;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_center)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::center;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_endl)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::endl;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_flush)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::flush;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_reset)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::reset;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_bom)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::bom;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_ws)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << Qt::ws;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_setFieldWidth)
(JNIEnv *env, jobject, QtJambiNativeID debugId, jint width)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << qSetFieldWidth(width);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_setRealNumberPrecision)
(JNIEnv *env, jobject, QtJambiNativeID debugId, jint precision)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << qSetRealNumberPrecision(precision);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_setPadChar)
(JNIEnv *env, jobject, QtJambiNativeID debugId, jchar ch)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QDebug>(env, debugId) << qSetPadChar(ch);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_bin)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::bin;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_oct)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::oct;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_dec)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::dec;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_hex)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::hex;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_showbase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::showbase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_forcesign)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::forcesign;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_forcepoint)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::forcepoint;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_noshowbase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::noshowbase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_noforcesign)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::noforcesign;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_noforcepoint)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::noforcepoint;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_uppercasebase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::uppercasebase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_uppercasedigits)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::uppercasedigits;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_lowercasebase)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::lowercasebase;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_lowercasedigits)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::lowercasedigits;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_fixed)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::fixed;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_scientific)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::scientific;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_left)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::left;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_right)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::right;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_center)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::center;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_endl)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::endl;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_flush)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::flush;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_reset)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::reset;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_bom)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::bom;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_ws)
(JNIEnv *env, jobject, QtJambiNativeID debugId)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << Qt::ws;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_setFieldWidth)
(JNIEnv *env, jobject, QtJambiNativeID debugId, jint width)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << qSetFieldWidth(width);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_setRealNumberPrecision)
(JNIEnv *env, jobject, QtJambiNativeID debugId, jint precision)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << qSetRealNumberPrecision(precision);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QTextStream_setPadChar)
(JNIEnv *env, jobject, QtJambiNativeID debugId, jchar ch)
{
    QTJAMBI_TRY{
        qtjambi_object_from_nativeId_deref<QTextStream>(env, debugId) << qSetPadChar(ch);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDebug_debugStream)
(JNIEnv *env, jobject, QtJambiNativeID debugId, QtJambiNativeID metaTypeId, jobject value)
{
    QDebug& debug = qtjambi_object_from_nativeId_deref<QDebug>(env, debugId);
    const QMetaType& metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
    QTJAMBI_TRY{
        bool success = false;
        if(metaType==QMetaType::fromType<JObjectWrapper>()
                || metaType==QMetaType::fromType<JCollectionWrapper>()
                || metaType==QMetaType::fromType<JMapWrapper>()
                || metaType==QMetaType::fromType<JIteratorWrapper>()
                || metaType==QMetaType::fromType<JIntArrayWrapper>()
                || metaType==QMetaType::fromType<JByteArrayWrapper>()
                || metaType==QMetaType::fromType<JShortArrayWrapper>()
                || metaType==QMetaType::fromType<JLongArrayWrapper>()
                || metaType==QMetaType::fromType<JDoubleArrayWrapper>()
                || metaType==QMetaType::fromType<JFloatArrayWrapper>()
                || metaType==QMetaType::fromType<JCharArrayWrapper>()
                || metaType==QMetaType::fromType<JBooleanArrayWrapper>()
                || metaType==QMetaType::fromType<JObjectArrayWrapper>()){
            debug << JObjectWrapper(env, value);
            success = true;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(!success && metaType.hasRegisteredDebugStreamOperator()){
            QVariant variant = qtjambi_cast<QVariant>(env, value);
            if(variant.metaType()==metaType || variant.convert(metaType)){
                success = QMetaType(metaType).debugStream(debug, variant.constData());
            }
        }
#else
        if(!success){
            QVariant variant = qtjambi_cast<QVariant>(env, value);
            if(variant.userType()==metaType.id() || variant.convert(metaType.id())){
                success = QMetaType::debugStream(debug, variant.constData(), metaType.id());
            }
            if(!success){
                if(const QtPrivate::AbstractDebugStreamFunction * fct = qtjambi_registered_debugstream_operator(metaType.id())){
                    QVariant variant = qtjambi_cast<QVariant>(env, value);
                    if(variant.userType()==metaType.id() || variant.convert(metaType.id())){
                        fct->stream(fct, debug, variant.constData());
                        success = true;
                    }
                }else if(metaType.id()<QMetaType::User){
                    QString tmp;
                    {
                        QDebug(&tmp) << variant;
                    }
                    tmp = tmp.trimmed();
                    QString suffix = QLatin1String("QVariant(%1, ");
                    suffix = suffix.arg(QLatin1String(metaType.name()));
                    if(tmp.startsWith(suffix) && tmp.endsWith(")")){
                        tmp = tmp.mid(suffix.length()).chopped(1);
                    }
                    debug.noquote() << tmp;
                    success = true;
                }
            }
        }
#endif
        if(!success){
            if(value)
                debug.noquote() << qtjambi_to_qstring(env, Java::Runtime::Object::toString(env, value));
            else
                debug << nullptr;
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_qInstallMessageHandler)
(JNIEnv *env, jclass, jobject supportedMessageTypes, jobject handler)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        env->PushLocalFrame(500);
        _result = qtjambi_install_message_handler(env, supportedMessageTypes, handler);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLogging_showCMessageFromSupplier)
(JNIEnv *env, jclass, jint messageType, QtJambiNativeID categoryId, jobject messageSupplier)
{
#if !defined(QT_NO_DEBUG_OUTPUT)
    QTJAMBI_TRY{
        QLoggingCategory& category = qtjambi_object_from_nativeId_deref<QLoggingCategory>(env, categoryId);
        if (messageType != QtMsgType::QtFatalMsg && !category.isEnabled(QtMsgType(messageType))) {
            return;
        }
        jstring message = Java::Runtime::Object::toString(env, Java::Runtime::Supplier::get(env, messageSupplier));
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 2);
        jclass declaringClass = Java::QtJambi::InternalAccess$CallerContext::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::InternalAccess$CallerContext::methodName(env, invocationInfo);
        line = Java::QtJambi::InternalAccess$CallerContext::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QtJambiExceptionRaiser __exnRaiser;
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
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        __exnRaiser.raise(env);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
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
    QTJAMBI_TRY{
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 2);
        jclass declaringClass = Java::QtJambi::InternalAccess$CallerContext::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::InternalAccess$CallerContext::methodName(env, invocationInfo);
        line = Java::QtJambi::InternalAccess$CallerContext::lineNumber(env, invocationInfo);
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
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
    QTJAMBI_TRY{
        QLoggingCategory& category = qtjambi_object_from_nativeId_deref<QLoggingCategory>(env, categoryId);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 2);
        jclass declaringClass = Java::QtJambi::InternalAccess$CallerContext::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::InternalAccess$CallerContext::methodName(env, invocationInfo);
        line = Java::QtJambi::InternalAccess$CallerContext::lineNumber(env, invocationInfo);
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
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
    QTJAMBI_TRY{
        QLoggingCategory& category = qtjambi_object_from_nativeId_deref<QLoggingCategory>(env, categoryId);
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 2);
        jclass declaringClass = Java::QtJambi::InternalAccess$CallerContext::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::InternalAccess$CallerContext::methodName(env, invocationInfo);
        line = Java::QtJambi::InternalAccess$CallerContext::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QtJambiExceptionRaiser __exnRaiser;
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
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        __exnRaiser.raise(env);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
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
    QTJAMBI_TRY{
        if (messageType != QtMsgType::QtFatalMsg) {
            if (QLoggingCategory *defaultCategory = QLoggingCategory::defaultCategory()) {
                if (!defaultCategory->isEnabled(QtMsgType(messageType)))
                    return;
            }
        }
        jstring message = Java::Runtime::Object::toString(env, Java::Runtime::Supplier::get(env, messageSupplier));
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 2);
        jclass declaringClass = Java::QtJambi::InternalAccess$CallerContext::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::InternalAccess$CallerContext::methodName(env, invocationInfo);
        line = Java::QtJambi::InternalAccess$CallerContext::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QtJambiExceptionRaiser __exnRaiser;
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
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        __exnRaiser.raise(env);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
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
    QTJAMBI_TRY{
        const char* _message = env->GetStringUTFChars(message, nullptr);
        const char* _method = nullptr;
        const char* _className = nullptr;
        int line = 0;
#if !defined (QT_NO_DEBUG)
        jobject invocationInfoProvider = Java::QtJambi::QtJambiInternal::invocationInfoProvider(env);
        jobject invocationInfo = Java::Runtime::IntFunction::apply(env, invocationInfoProvider, 2);
        jclass declaringClass = Java::QtJambi::InternalAccess$CallerContext::declaringClass(env, invocationInfo);
        jstring method = Java::QtJambi::InternalAccess$CallerContext::methodName(env, invocationInfo);
        line = Java::QtJambi::InternalAccess$CallerContext::lineNumber(env, invocationInfo);
        _method = env->GetStringUTFChars(method, nullptr);
        jstring className = Java::Runtime::Class::getName(env, declaringClass);
        _className = env->GetStringUTFChars(className, nullptr);
#endif
        QtJambiExceptionRaiser __exnRaiser;
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
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        __exnRaiser.raise(env);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
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
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    QTJAMBI_TRY{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        QByteArray __qt_return_value = __qt_list0.join();
        _result = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QByteArrayList::join(char sep)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2B)
(JNIEnv *__jni_env,
 jclass,
 jobject list0,
 jbyte sep1)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    QTJAMBI_TRY{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        QByteArray __qt_return_value = __qt_list0.join(char(sep1));
        _result = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QByteArrayList::join(const QByteArray & sep)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2Lio_qt_core_QByteArray_2)
(JNIEnv *__jni_env,
 jclass,
 jobject list0,
 jobject sep1)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    QTJAMBI_TRY{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        const QByteArray&  __qt_sep1 = qtjambi_cast<QByteArray>(__jni_env, sep1);
        QByteArray __qt_return_value = __qt_list0.join(__qt_sep1);
        _result = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}


extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIODevice_fromDirectBuffer)
(JNIEnv *env,
 jclass,
 jobject buffer)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        bool isDirect = Java::Runtime::Buffer::isDirect(env, buffer);
        if(isDirect){
            jlong capacity = env->GetDirectBufferCapacity(buffer);
            void* address = env->GetDirectBufferAddress(buffer);
            bool readOnly = Java::Runtime::Buffer::isReadOnly(env, buffer);
            if(address && capacity>0){
                QIODevice* device = qtjambi_new_direct_address_device(capacity, reinterpret_cast<char*>(address), readOnly, env, buffer);
                _result = qtjambi_cast<jobject>(env, device);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

// QCborValue::operator[](const QString & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborValue_setValue__Ljava_lang_String_2Lio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jobject _this,
 jstring key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborValue::operator[](const QString & key)")
    QTJAMBI_TRY{
        QCborValue *__qt_this = qtjambi_to_object<QCborValue>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QString&  __qt_key0 = qtjambi_cast<QString >(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QCborValue::operator[](long long key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborValue_setValue__JLio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jobject _this,
 jlong key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborValue::operator[](long long key)")
    QTJAMBI_TRY{
        QCborValue *__qt_this = qtjambi_to_object<QCborValue>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[static_cast<qlonglong>(key0)] = __qt_value1;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QCborArray::operator[](qsizetype i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborArray_setValue)
(JNIEnv *__jni_env,
 jobject _this,
 jlong i0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborArray::operator[](qsizetype i)")
    QTJAMBI_TRY{
        QCborArray *__qt_this = qtjambi_to_object<QCborArray>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[static_cast<qsizetype>(i0)] = __qt_value1;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QCborMap::operator[](const QCborValue & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap_setValue__Lio_qt_core_QCborValue_2Lio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jobject _this,
 jobject key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](const QCborValue & key)")
    QTJAMBI_TRY{
        QCborMap *__qt_this = qtjambi_to_object<QCborMap>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QCborValue&  __qt_key0 = qtjambi_cast<const QCborValue&>(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QCborMap::operator[](const QString & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap_setValue__Ljava_lang_String_2Lio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jobject _this,
 jstring key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](const QString & key)")
    QTJAMBI_TRY{
        QCborMap *__qt_this = qtjambi_to_object<QCborMap>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QString&  __qt_key0 = qtjambi_cast<QString >(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QCborMap::operator[](long long key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap_setValue__JLio_qt_core_QCborValue_2)
(JNIEnv *__jni_env,
 jobject _this,
 jlong key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](long long key)")
    QTJAMBI_TRY{
        QCborMap *__qt_this = qtjambi_to_object<QCborMap>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[key0] = __qt_value1;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QCborStreamReader::readByteArray()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader_readByteArray)
(JNIEnv *__jni_env,
 jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readByteArray()")
    QTJAMBI_TRY{
        QCborStreamReader *__qt_this = qtjambi_to_object<QCborStreamReader>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborStreamReader::StringResult<QByteArray> __qt_return_value = __qt_this->readByteArray();
        _result = Java::QtCore::QCborStreamReader$StringResult::newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QCborStreamReader::readString()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader_readString)
(JNIEnv *__jni_env,
 jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readString()")
    QTJAMBI_TRY{
        QCborStreamReader *__qt_this = qtjambi_to_object<QCborStreamReader>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborStreamReader::StringResult<QString> __qt_return_value = __qt_this->readString();
        _result = Java::QtCore::QCborStreamReader$StringResult::newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QCborStreamReader::readChunk()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader_readChunk)
(JNIEnv *__jni_env,
 jobject _this, jobject buffer)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readChunk()")
    QTJAMBI_TRY{
        QCborStreamReader *__qt_this = qtjambi_to_object<QCborStreamReader>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        JBufferData __qt_buffer(__jni_env, buffer);
        QCborStreamReader::StringResult<qsizetype> __qt_return_value = __qt_this->readStringChunk(__qt_buffer, __qt_buffer.size());
        _result = Java::QtCore::QCborStreamReader$StringResult::newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QObject::metaObject() const
extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_metaObject)
    (JNIEnv * env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QObject *__qt_this = qtjambi_to_QObject<QObject>(env, _this);
        qtjambi_check_resource(env, __qt_this);
        _result = qtjambi_cast<jobject>(env, __qt_this->metaObject());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

// QObject::findChild(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChild__Ljava_lang_Class_2JLjava_lang_String_2I)
(JNIEnv *__jni_env,
 jobject _this,
 jclass cl0,
 jlong metaObjectPointer1,
 jobject name2,
 jint options3)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChild(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    QTJAMBI_TRY{
        QObject *__qt_this = qtjambi_to_QObject<QObject>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        QString __qt_name2(qtjambi_to_qstring(__jni_env, jstring(name2)));
        QObject * __qt_return = qt_qFindChild_helper(__qt_this, __qt_name2, *__qt_metaObjectPointer1, Qt::FindChildOptions(options3));
        _result = qtjambi_from_qobject(__jni_env, __qt_return, cl0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QObject::findChildren(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenString__JLjava_lang_String_2I)
(JNIEnv *__jni_env,
 jobject _this,
 jlong metaObjectPointer1,
 jobject name2,
 jint options3)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    QTJAMBI_TRY{
        QObject *__qt_this = qtjambi_to_QObject<QObject>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        QString __qt_name2(qtjambi_to_qstring(__jni_env, jstring(name2)));
        QList<QObject *> list;
        qt_qFindChildren_helper(__qt_this, __qt_name2, *__qt_metaObjectPointer1, reinterpret_cast<QList<void*>*>(&list), Qt::FindChildOptions(options3));
        _result = qtjambi_cast<jobject>(__jni_env, list);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
// QObject::findChildren(const QRegExp &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenQRegExp__JLio_qt_core_QRegExp_2I)
(JNIEnv *__jni_env,
 jobject _this,
 jlong metaObjectPointer1,
 jobject regExp2,
 jint options3)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QRegExp &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    QTJAMBI_TRY{
        //JavaException::raiseQNoImplementationException(__jni_env, "Deprecated method removed." QTJAMBI_STACKTRACEINFO );
        QObject *__qt_this = qtjambi_to_QObject<QObject>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        const QRegExp& __qt_regExp2 = qtjambi_cast<const QRegExp&>(__jni_env, regExp2);
        QList<QObject *> list;
        qt_qFindChildren_helper(__qt_this, __qt_regExp2, *__qt_metaObjectPointer1, reinterpret_cast<QList<void*>*>(&list), Qt::FindChildOptions(options3));
        _result = qtjambi_cast<jobject>(__jni_env, list);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}
#endif

// QObject::findChildren(const QRegularExpression &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenQRegularExpression__JLio_qt_core_QRegularExpression_2I)
(JNIEnv *__jni_env,
 jobject _this,
 jlong metaObjectPointer1,
 jobject regExp2,
 jint options3)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QRegularExpression &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    QTJAMBI_TRY{
        QObject *__qt_this = qtjambi_to_QObject<QObject>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        const QRegularExpression& __qt_regExp2 = qtjambi_cast<const QRegularExpression&>(__jni_env, regExp2);
        QList<QObject *> list;
        qt_qFindChildren_helper(__qt_this, __qt_regExp2, *__qt_metaObjectPointer1, reinterpret_cast<QList<void*>*>(&list), Qt::FindChildOptions(options3));
        _result = qtjambi_cast<jobject>(__jni_env, list);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1javaThread)
(JNIEnv *__jni_env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QThread* __qt_this = qtjambi_to_QObject<QThread>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_from_thread(__jni_env, __qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_thread)
(JNIEnv *__jni_env, jclass, jobject thread)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_get_qthread_for_jthread(__jni_env, thread);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1getThreadGroup)
(JNIEnv *__jni_env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QThread* __qt_this = qtjambi_to_QObject<QThread>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_thread_get_group(__jni_env, __qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1setUncaughtExceptionHandler)
(JNIEnv *__jni_env, jobject _this, jobject handler)
{
    QTJAMBI_TRY{
        qtjambi_thread_set_UncaughtExceptionHandler(__jni_env, _this, handler);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1getUncaughtExceptionHandler)
(JNIEnv *__jni_env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QThread* __qt_this = qtjambi_to_QObject<QThread>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_thread_get_UncaughtExceptionHandler(__jni_env, __qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1setContextClassLoader)
(JNIEnv *__jni_env, jobject _this, jobject cl)
{
    QTJAMBI_TRY{
        qtjambi_thread_set_ContextClassLoader(__jni_env, _this, cl);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1getContextClassLoader)
(JNIEnv *__jni_env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QThread* __qt_this = qtjambi_to_QObject<QThread>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_thread_get_ContextClassLoader(__jni_env, __qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}


extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1setName)
(JNIEnv *__jni_env, jobject _this, jstring name)
{
    QTJAMBI_TRY{
        qtjambi_thread_set_name(__jni_env, _this, name);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1getName)
(JNIEnv *__jni_env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QThread* __qt_this = qtjambi_to_QObject<QThread>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_thread_get_name(__jni_env, __qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1setDaemon)
(JNIEnv *__jni_env, jobject _this, jboolean daemon)
{
    QTJAMBI_TRY{
        qtjambi_thread_set_daemon(__jni_env, _this, daemon);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1isDaemon)
(JNIEnv *__jni_env, jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QThread* __qt_this = qtjambi_to_QObject<QThread>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_thread_is_daemon(__qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread__1_1qt_1initialize__Ljava_lang_ThreadGroup_2)
(JNIEnv *__jni_env, jobject _this, jobject group)
{
    QTJAMBI_TRY{
        qtjambi_initialize_thread(__jni_env, _this, group);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_initialize__)
(JNIEnv *env, jclass)
{
    QTJAMBI_TRY{
        qtjambi_initialize_current_thread(env);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

// QStaticPlugin::instance()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QStaticPlugin_instance)
(JNIEnv *__jni_env,
 jobject __this)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QStaticPlugin::instance()")
    QTJAMBI_TRY{
        QStaticPlugin *__qt_this = qtjambi_to_QObject<QStaticPlugin>(__jni_env, __this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_cast<jobject>(__jni_env, __qt_this->instance());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaType_00024GenericValue_buffer)
(JNIEnv *__jni_env,
 jobject _this,
 jint type0)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        if(void* ptr = qtjambi_to_object(__jni_env, _this)){
            jlong size = QMetaType::sizeOf(type0);
            if(size>0){
                _result = __jni_env->NewDirectByteBuffer(ptr, size);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaType_00024GenericValue_clone)
(JNIEnv *__jni_env,
 jobject _this,
 jint type0)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        if(void* ptr = qtjambi_to_object(__jni_env, _this)){
            _result = qtjambi_from_object(__jni_env, QMetaType::create(type0, ptr), Java::QtCore::QMetaType$GenericValue::getClass(__jni_env), false);
            if(_result)
                Java::QtCore::QMetaType$GenericValue::set_type(__jni_env, _result, type0);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_toReflected)
    (JNIEnv * env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_metamethod_to_reflected(env, _this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_parameterClassTypes_1native)
    (JNIEnv * env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_metamethod_parameter_class_types(env, _this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_returnClassType_1native)
    (JNIEnv * env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_metamethod_return_class_type(env, _this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_methodFromMethod)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jobjectArray ok)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_method_from_javamethod(env, metaObjectPointer, ok);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_invoke_1native__Lio_qt_core_QObject_2_3Ljava_lang_Class_2I_3Ljava_lang_Object_2)
    (JNIEnv * env, jobject _this, jobject object, jobjectArray argClassTypeArray, jint connection, jobjectArray args)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_metamethod_invoke(env, _this, object, argClassTypeArray, connection, args);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_invoke_1native__Ljava_lang_Object_2_3Ljava_lang_Class_2_3Ljava_lang_Object_2)
    (JNIEnv * env, jobject _this, jobject object, jobjectArray argClassTypeArray, jobjectArray args)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_metamethod_invoke_on_gadget(env, _this, object, argClassTypeArray, args);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_resetOnGadget)
    (JNIEnv *env, jobject _this, jobject gadget){
    jboolean _result{false};
    QTJAMBI_TRY{
        _result = qtjambi_metaproperty_reset_on_gadget(env, _this, gadget);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_readOnGadget)
    (JNIEnv *env, jobject _this, jobject gadget){
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_metaproperty_read_on_gadget(env, _this, gadget);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_writeOnGadget)
    (JNIEnv *env, jobject _this, jobject gadget, jobject value){
    jboolean _result{false};
    QTJAMBI_TRY{
        _result = qtjambi_metaproperty_write_on_gadget(env, _this, gadget, value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jclass JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_classType_1native)
    (JNIEnv *env, jobject _this){
    jclass _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_metaproperty_class(env, _this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_resolveEntry)
    (JNIEnv * env, jclass, jclass enumClass, jint value, jbyte bitSize)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        switch ( bitSize ) {
        case 1:  _result = qtjambi_from_enum(env, qint8(value), enumClass); break;
        case 2:  _result = qtjambi_from_enum(env, qint16(value), enumClass); break;
        case 8:  _result = qtjambi_from_enum(env, qint64(value), enumClass); break;
        default: _result = qtjambi_from_enum(env, qint32(value), enumClass); break;
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_flags)
    (JNIEnv * env, jclass, jclass flagsClass, jint value)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_from_flags(env, value, qPrintable(qtjambi_class_name(env, flagsClass).replace(".", "/")));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumeratorIndex)
    (JNIEnv *env, jclass, jlong metaObjectPointer, QtJambiNativeID enumID)
{
    jint _result{-1};
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            Q_ASSERT(metaObject != nullptr);
            const QMetaEnum *__qt_this = qtjambi_object_from_nativeId<QMetaEnum>(enumID);
            _result = metaObject->indexOfEnumerator(__qt_this->name());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_userProperty)
    (JNIEnv *env, jobject, jlong metaObjectPointer)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            Q_ASSERT(metaObject != nullptr);
            _result = qtjambi_cast<jobject>(env, metaObject->userProperty());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_properties)
    (JNIEnv *env, jobject, jlong metaObjectPointer)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QList<QMetaProperty> propertyList;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int count = metaObject->propertyCount();
            for (int i=0; i<count; ++i) {
                propertyList << metaObject->property(i);
            }
        }
        _result = qtjambi_cast<jobject>(env, propertyList);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_propertyCount)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    jint _result{0};
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            _result = metaObject->propertyCount();
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_propertyByIndex)
    (JNIEnv *env, jobject, jlong metaObjectPointer, jint index)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            _result = qtjambi_cast<jobject>(env, metaObject->property(index));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_property)
    (JNIEnv *env, jobject, jlong metaObjectPointer, jobject name)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int i = metaObject->indexOfProperty(qPrintable(qtjambi_to_qstring(env, jstring(name))));
            _result = qtjambi_cast<jobject>(env, metaObject->property(i));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
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
    (JNIEnv *env, jclass, jobject _sender, jstring signalName, jobject _receiver, jstring slotName, jbyte connectionType)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        QObject *sender = qtjambi_to_qobject(env, _sender);
        QObject *receiver = qtjambi_to_qobject(env, _receiver);
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_connectMethods)
    (JNIEnv *env, jclass, jobject _sender, jint signalIdx, jlong signalEnclosingMetaObjectPointer, jobject _receiver, jint slotIdx, jlong receiverEnclosingMetaObjectPointer, jbyte connectionType)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        QObject *sender = qtjambi_to_qobject(env, _sender);
        QObject *receiver = qtjambi_to_qobject(env, _receiver);
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}



extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_disconnect)
    (JNIEnv *env, jclass, jobject _sender, jstring signalName, jobject _receiver, jstring slotName)
{
    bool result = false;
    QTJAMBI_TRY{
        QObject *sender = qtjambi_to_qobject(env, _sender);
        if(sender){
            QObject *receiver = qtjambi_to_qobject(env, _receiver);
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_disconnectMethods)
    (JNIEnv *env, jclass, jobject _sender, jint signalIdx, jlong signalEnclosingMetaObjectPointer, jobject _receiver, jint slotIdx, jlong receiverEnclosingMetaObjectPointer)
{
    bool result = false;
    QTJAMBI_TRY{
        QObject *sender = qtjambi_to_qobject(env, _sender);
        if(sender){
            QObject *receiver = qtjambi_to_qobject(env, _receiver);
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_findEnumForFlags)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
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
                        result = qtjambi_cast<jobject>(__jni_env, enumerator);
                        break;
                    }
                }
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methodByIndex)
    (JNIEnv * env, jobject, jlong metaObjectPointer, jint index)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->method(index);
            if(method.isValid()){
                result = qtjambi_cast<jobject>(env, method);
            }else{
                JavaException::raiseIndexOutOfBoundsException(env, qPrintable(QString("Index %1 is not a valid method of class %2").arg(index).arg(metaObject->className())) QTJAMBI_STACKTRACEINFO );
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methods)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        env->EnsureLocalCapacity(500);
        QList<QMetaMethod> methods;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            const int methodCount = metaObject->methodCount();
            for(int i=0; i<methodCount; ++i){
                methods << metaObject->method(i);
            }
        }
        result = qtjambi_cast<jobject>(env, methods);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methodCount)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jint result = 0;
    QTJAMBI_TRY{
        if(const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer)){
            result = metaObject->methodCount();
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructor)
    (JNIEnv * env, jobject, jlong metaObjectPointer, jobject normalizedSignature)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->constructor(metaObject->indexOfConstructor(qPrintable(qtjambi_to_qstring(env, jstring(normalizedSignature)))));
            if(method.isValid() && method.methodType()==QMetaMethod::Constructor){
                result = qtjambi_cast<jobject>(env, method);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructorByIndex)
    (JNIEnv * env, jobject, jlong metaObjectPointer, jint index)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->constructor(index);
            if(method.isValid() && method.methodType()==QMetaMethod::Constructor){
                result = qtjambi_cast<jobject>(env, method);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructorCount)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    jint result = 0;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            result = metaObject->constructorCount();
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructors)
    (JNIEnv * env, jobject, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        env->EnsureLocalCapacity(100);
        QList<QMetaMethod> methods;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            for(int i=0; i<metaObject->constructorCount(); ++i){
                methods << metaObject->constructor(i);
            }
        }
        result = qtjambi_cast<jobject>(env, methods);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumerator)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jobject name)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            QString enumName = qtjambi_to_qstring(env, jstring(name));
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int idx = metaObject->indexOfEnumerator(qPrintable(enumName));
            QMetaEnum enumerator = metaObject->enumerator(idx);
            result = qtjambi_cast<jobject>(env, enumerator);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumeratorByIndex)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jint index)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaEnum enumerator = metaObject->enumerator(index);
            result = qtjambi_cast<jobject>(env, enumerator);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_connectSlotsByName)
    (JNIEnv * env, jclass, QtJambiNativeID native_id)
{
    QTJAMBI_TRY{
        QObject *object = qtjambi_object_from_nativeId<QObject>(native_id);
        qtjambi_check_resource(env, object);
        QMetaObject::connectSlotsByName(object);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumeratorCount)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jint result = 0;
    QTJAMBI_TRY{
        if(const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer)){
            result = metaObject->enumeratorCount();
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumerators)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        QList<QMetaEnum> javaEnums;
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int count = metaObject->enumeratorCount();
            for (int i=0; i<count; ++i) {
                javaEnums << metaObject->enumerator(i);
            }
        }
        result = qtjambi_cast<jobject>(env, javaEnums);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_superClass)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject && metaObject->superClass()){
            result = qtjambi_cast<jobject>(env, metaObject->superClass());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_className)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            result = qtjambi_from_qstring(env, QLatin1String(metaObject->className()));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfos)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            QList<QPair<QString,QString>> list;
            for (int var = 0; var < metaObject->classInfoCount(); ++var) {
                list << QPair<QString,QString>{QLatin1String(metaObject->classInfo(var).name()), QLatin1String(metaObject->classInfo(var).value())};
            }
            result = qtjambi_cast<jobject>(env, list);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfoCount)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jint result = 0;
    QTJAMBI_TRY{
        if(const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer)){
            result = metaObject->classInfoCount();
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfo)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jstring name)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            int var = metaObject->indexOfClassInfo(qPrintable(qtjambi_to_qstring(env, name)));
            if(var){
                result = qtjambi_from_qstring(env, QLatin1String(metaObject->classInfo(var).value()));
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_inherits)
    (JNIEnv *env, jclass, jlong metaObjectPointer, jlong other)
{
    jboolean result = false;
    QTJAMBI_TRY{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            result = metaObject->inherits(reinterpret_cast<const QMetaObject *>(other));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_checkConnectArgsString)
    (JNIEnv * env, jclass, jobject signal, jobject method)
{
    jboolean result = false;
    QTJAMBI_TRY{
        result = QMetaObject::checkConnectArgs(qPrintable(qtjambi_to_qstring(env, jstring(signal))), qPrintable(qtjambi_to_qstring(env, jstring(method))));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_checkConnectArgsMethods)
    (JNIEnv *env, jclass, jlong signalMetaObjectPointer, jint signalMethodIndex, jlong methodMetaObjectPointer, jint methodMethodIndex)
{
    jboolean result = false;
    QTJAMBI_TRY{
        const QMetaObject *signalMetaObject = reinterpret_cast<const QMetaObject *>(signalMetaObjectPointer);
        const QMetaObject *methodMetaObject = reinterpret_cast<const QMetaObject *>(methodMetaObjectPointer);
        result = QMetaObject::checkConnectArgs(signalMetaObject->method(signalMethodIndex), methodMetaObject->method(methodMethodIndex));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject__1_1qt_1forType)
    (JNIEnv * env, jclass, jclass type)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        const QMetaObject *metaObject = qtjambi_metaobject_for_class(env, type);
        result = qtjambi_cast<jobject>(env, metaObject);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}



extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_type)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            result = qtjambi_metaobject_javatype(env, metaObject);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_exactType)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            result = qtjambi_metaobject_javatype(env, metaObject, true);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_cast)
    (JNIEnv *env, jclass, jobject object, jclass targetType)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        result = qtjambi_metaobject_cast(env, object, targetType);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_method)
    (JNIEnv * env, jobject, jlong metaObjectPointer, jstring normalizedSignature)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        if(metaObjectPointer){
            QString sig = qtjambi_to_qstring(env, jstring(normalizedSignature));
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);

            QMetaMethod method = qtjambi_metaobject_get_method(metaObject, sig);
            result = qtjambi_cast<jobject>(env, method);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_newInstanceImpl)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jint methodIndex, jobject argTypes, jobjectArray args)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        result = qtjambi_metaobject_new_instance(env, metaObjectPointer, methodIndex, argTypes, args);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_futureInterface)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        QFuture<QVariant> *__qt_this = qtjambi_object_from_nativeId<QFuture<QVariant>>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        result = qtjambi_cast<jobject>(__jni_env, reinterpret_cast<QFutureInterface<QVariant>*>(__qt_this));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterfaceBase_reportException)
(JNIEnv * __jni_env, jobject __this, jthrowable throwable)
{
    QTJAMBI_TRY{
        if(__jni_env->IsSameObject(nullptr, throwable))
            JavaException::raiseNullPointerException(__jni_env, "Reported exception must not be null." QTJAMBI_STACKTRACEINFO );
        QFutureInterfaceBase *__qt_this = qtjambi_to_object<QFutureInterfaceBase>(__jni_env, __this);
        qtjambi_check_resource(__jni_env, __qt_this);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        __qt_this->reportException(std::make_exception_ptr(JavaException(__jni_env, throwable)));
#else
        __qt_this->reportException(JavaException(__jni_env, throwable));
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

template<int functorType>
class BindingFunctionVTableHelper : public QtPrivate::BindingFunctionVTable{
public:
    static QtPrivate::BindingFunctionVTable create(){
        return {
                     [](QMetaType metaType, QUntypedPropertyData *dataPtr, void *f) -> bool {
                         JObjectWrapper* _f = reinterpret_cast<JObjectWrapper*>(f);
                         bool boolOut = false;
                         QTJAMBI_TRY_ANY{
                             if(JNIEnv* env = qtjambi_current_environment()){
                                 QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                 QTJAMBI_TRY{
                                     if constexpr(functorType=='L'){
                                         jobject result = Java::Runtime::Supplier::get(env, env->NewLocalRef(_f->object()));
                                         if(metaType.flags() & QMetaType::IsPointer){
                                             jobject propertyObject = qtjambi_find_object(env, dataPtr);
                                             bool setIt = true;
                                             if(Java::QtCore::QPropertyData::isInstanceOf(env, propertyObject)){
                                                 jobject oldValue = Java::QtCore::QPropertyData::getValueBypassingBindings(env, propertyObject);
                                                 if(!oldValue){
                                                     if(!result)
                                                         setIt = false;
                                                 }else{
                                                     if(Java::Runtime::Object::equals(env, oldValue, result))
                                                         setIt = false;
                                                 }
                                                 if(setIt)
                                                    boolOut = Java::QtCore::QPropertyData::setValueBypassingBindings(env, propertyObject, result);
                                             }else{
                                                 QPropertyData<void*> *propertyPtr = static_cast<QPropertyData<void*> *>(dataPtr);
                                                 void* _result = nullptr;
                                                 if(result){
                                                     QVariant variant = qtjambi_to_qvariant(env, result);
                                                     if(variant.metaType()!=metaType && !variant.convert(metaType))
                                                         setIt = false;
                                                     else
                                                        _result = *reinterpret_cast<void*const*>(variant.constData());
                                                 }else{
                                                     _result = nullptr;
                                                 }
                                                 if (setIt && _result != propertyPtr->valueBypassingBindings()){
                                                     propertyPtr->setValueBypassingBindings(_result);
                                                     boolOut = true;
                                                 }
                                             }
                                         }else{
                                             switch(metaType.id()){
                                             case QMetaType::QByteArray: {
                                                     QPropertyData<QByteArray> *propertyPtr = static_cast<QPropertyData<QByteArray> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArray>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QByteArrayList: {
                                                     QPropertyData<QByteArrayList> *propertyPtr = static_cast<QPropertyData<QByteArrayList> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArrayList>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QString: {
                                                     QPropertyData<QString> *propertyPtr = static_cast<QPropertyData<QString> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QString>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QStringList: {
                                                     QPropertyData<QStringList> *propertyPtr = static_cast<QPropertyData<QStringList> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QStringList>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantList: {
                                                     QPropertyData<QVariantList> *propertyPtr = static_cast<QPropertyData<QVariantList> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantList>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantMap: {
                                                     QPropertyData<QVariantMap> *propertyPtr = static_cast<QPropertyData<QVariantMap> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantMap>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantHash: {
                                                     QPropertyData<QVariantHash> *propertyPtr = static_cast<QPropertyData<QVariantHash> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantHash>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QVariantPair: {
                                                     QPropertyData<QVariantPair> *propertyPtr = static_cast<QPropertyData<QVariantPair> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantPair>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             case QMetaType::QVariant: {
                                                     QPropertyData<QVariant> *propertyPtr = static_cast<QPropertyData<QVariant> *>(dataPtr);
                                                     propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariant>(env, result));
                                                     boolOut = true;
                                                 }
                                                 break;
                                             default: if(!boolOut){
                                                    QVariant variant = result ? qtjambi_to_qvariant(env, result) : QVariant(metaType);
                                                    if(variant.metaType()!=metaType && !variant.convert(metaType)){
                                                         boolOut = false;
                                                    }else if(metaType.isEqualityComparable() && metaType.equals(dataPtr, variant.constData())){
                                                         boolOut = false;
                                                    }else{
                                                        metaType.destruct(dataPtr);
                                                        metaType.construct(dataPtr, variant.constData());
                                                        boolOut = true;
                                                    }
                                                }
                                             }
                                         }
                                     }else if constexpr(functorType=='I'){
                                         jint result = Java::Runtime::IntSupplier::getAsInt(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint32> *propertyPtr = static_cast<QPropertyData<qint32> *>(dataPtr);
                                         if (result != propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }else if constexpr(functorType=='S'){
                                         jshort result = Java::QtJambi::QtUtilities$ShortSupplier::getAsShort(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint16> *propertyPtr = static_cast<QPropertyData<qint16> *>(dataPtr);
                                         if (result != propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }else if constexpr(functorType=='B'){
                                         jbyte result = Java::QtJambi::QtUtilities$ByteSupplier::getAsByte(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint8> *propertyPtr = static_cast<QPropertyData<qint8> *>(dataPtr);
                                         if (result != propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }else if constexpr(functorType=='J'){
                                         jlong result = Java::Runtime::LongSupplier::getAsLong(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<qint64> *propertyPtr = static_cast<QPropertyData<qint64> *>(dataPtr);
                                         if (result != propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }else if constexpr(functorType=='D'){
                                         jdouble result = Java::Runtime::DoubleSupplier::getAsDouble(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<double> *propertyPtr = static_cast<QPropertyData<double> *>(dataPtr);
                                         if (result!=propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }else if constexpr(functorType=='F'){
                                         jfloat result = Java::QtJambi::QtUtilities$FloatSupplier::getAsFloat(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<float> *propertyPtr = static_cast<QPropertyData<float> *>(dataPtr);
                                         if (result!=propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }else if constexpr(functorType=='C'){
                                         char16_t result = char16_t(Java::QtJambi::QtUtilities$CharSupplier::getAsChar(env, env->NewLocalRef(_f->object())));
                                         QPropertyData<char16_t> *propertyPtr = static_cast<QPropertyData<char16_t> *>(dataPtr);
                                         if (result != propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }else if constexpr(functorType=='Z'){
                                         bool result = Java::Runtime::BooleanSupplier::getAsBoolean(env, env->NewLocalRef(_f->object()));
                                         QPropertyData<bool> *propertyPtr = static_cast<QPropertyData<bool> *>(dataPtr);
                                         if (result != propertyPtr->valueBypassingBindings()){
                                             propertyPtr->setValueBypassingBindings(result);
                                             boolOut = true;
                                         }
                                     }
                                 }QTJAMBI_CATCH(const JavaException& exn){
                                     exn.report(env);
                                 }QTJAMBI_TRY_END
                             }
                         }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
                         return boolOut;
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
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedPropertyBinding_initialize_1native__Lio_qt_core_QUntypedPropertyBinding_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject functor)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedPropertyBinding::QUntypedPropertyBinding(QMetaType, const BindingFunctionVTable *, void *, const QPropertyBindingSourceLocation &)")
    QTJAMBI_TRY{
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
            if(!Java::Runtime::Supplier::isInstanceOf(__jni_env, functor))
                functor = nullptr;
            __jni_class = Java::QtCore::QPropertyBinding::getClass(__jni_env);
            typeId = &typeid(QPropertyBinding<QVariant>);
            arguments[1].c = 'L';
        }else{
            typeId = &typeid(QUntypedPropertyBinding);
        }

        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QUntypedPropertyBinding_2, sizeof(QUntypedPropertyBinding), *typeId, false, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QUntypedPropertyBinding::QUntypedPropertyBinding(QPropertyBindingPrivate *)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedPropertyBinding_initialize_1native__Lio_qt_core_QUntypedPropertyBinding_2Lio_qt_core_QPropertyBindingData_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject bindingData)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedPropertyBinding::QUntypedPropertyBinding(QPropertyBindingPrivate *)")
    QTJAMBI_TRY{
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void __qt_construct_QUntypedPropertyBinding_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);

// QUntypedPropertyBinding::QUntypedPropertyBinding(QUntypedPropertyBinding)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedPropertyBinding_initialize_1native__Lio_qt_core_QUntypedPropertyBinding_2Lio_qt_core_QUntypedPropertyBinding_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject other,
 jboolean)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedPropertyBinding::QUntypedPropertyBinding(QUntypedPropertyBinding)")
    QTJAMBI_TRY{
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void __qt_construct_QUntypedBindable_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);

// QUntypedBindable::QUntypedBindable(QUntypedPropertyData * d, const QtPrivate::QBindableInterface * i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedBindable_initialize_1native__Lio_qt_core_QUntypedBindable_2Lio_qt_core_QUntypedPropertyData_2Lio_qt_core_QBindableInterface_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject d0,
 jobject i1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QUntypedBindable::QUntypedBindable(QUntypedPropertyData * d, const QtPrivate::QBindableInterface * i)")
    QTJAMBI_TRY{
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
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QUntypedBindable_overrideBinding)
(JNIEnv *__jni_env,
 jobject,
 jobject _iface,
 jobject _data,
 jobject binding0)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_to_object<QtPrivate::QBindableInterface>(__jni_env, _iface);
        QUntypedPropertyData *data = qtjambi_to_object<QUntypedPropertyData>(__jni_env, _data);
        QUntypedPropertyBinding* __qt_binding0 = qtjambi_to_object<QUntypedPropertyBinding>(__jni_env, binding0);
        if(_iface)
            qtjambi_check_resource(__jni_env, iface);
        qtjambi_check_resource(__jni_env, data);
        if(_iface)
            qtjambi_check_resource(__jni_env, __qt_binding0);
        if(iface && iface->setBinding && __qt_binding0) {
            result = qtjambi_cast<jobject>(__jni_env, iface->setBinding(data, *__qt_binding0));
        }else{
            result = qtjambi_cast<jobject>(__jni_env, QUntypedPropertyBinding());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIntBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    jint result = 0;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<int>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jshort JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QShortBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    jshort result = 0;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<qint16>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jbyte JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    jbyte result = 0;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<char>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLongBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    jlong result = 0;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<qint64>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT double JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDoubleBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    double result = 0;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<double>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT float JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFloatBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    float result = 0;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<float>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBooleanBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    bool result = 0;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<bool>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jchar JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCharBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId)
{
    QChar result;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->getter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant;
                iface->getter(data, &variant);
                result = variant.value<QChar>();
            }else{
                iface->getter(data, &result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return jchar(result.unicode());
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIntBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jint value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(value);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &value);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jbyte value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(value);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &value);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QShortBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jshort value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(value);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &value);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLongBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jlong value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(value);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &value);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QDoubleBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 double value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(value);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &value);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFloatBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jfloat value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(value);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &value);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCharBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jchar value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(value);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &value);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBooleanBindable_setValue)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 jboolean value
 )
{
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_object_from_nativeId<const QtPrivate::QBindableInterface>(ifaceId);
        QUntypedPropertyData *data = qtjambi_object_from_nativeId<QUntypedPropertyData>(dataId);
        if (iface && iface->setter) {
            bool b = value;
            if(iface->metaType && iface->metaType().id()==QMetaType::QVariant){
                QVariant variant = QVariant::fromValue(b);
                iface->setter(data, &variant);
            }else{
                iface->setter(data, &b);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QBindable_value)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID ifaceId,
 QtJambiNativeID dataId,
 QtJambiNativeID metaTypeId)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
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
                _result = qtjambi_cast<jobject>(__jni_env, result);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
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
    jboolean _result = false;
    QTJAMBI_TRY{
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
                    _result = false;
                else{
                    iface->setter(data, variant.data());
                    _result = true;
                }
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

template<class T>
QMetaType qtjambi_metatype() { return QMetaType::fromType<T>(); }

template<class Property, class T>
void qtjambi_get_property_value(const QUntypedPropertyData *d, void *value){
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                *static_cast<T*>(value) = qtjambi_cast<T>(env, Property::value(env, property));
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
}

template<class Property, class T>
void qtjambi_set_property_value(QUntypedPropertyData *d, const void *value){
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject _value = qtjambi_cast<jobject>(env, *static_cast<const T*>(value));
                Property::setValue(env, property, _value);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
}

template<class Property>
void qtjambi_get_qproperty_value(const QUntypedPropertyData *d, void *value){
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                QMetaType metaType = qtjambi_cast<QMetaType>(env, Java::QtCore::QPropertyData::valueMetaType(env, property));
                QVariant _value = qtjambi_cast<QVariant>(env, Property::value(env, property));
                if(_value.metaType()!=metaType && !_value.convert(metaType))
                    return;
                metaType.destruct(value);
                metaType.construct(value, _value.data());
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
}

template<class Property>
void qtjambi_set_qproperty_value(QUntypedPropertyData *d, const void *value){
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                QMetaType metaType = qtjambi_cast<QMetaType>(env, Java::QtCore::QPropertyData::valueMetaType(env, property));
                jobject _value = qtjambi_cast<jobject>(env, QVariant(metaType, value));
                Property::setValue(env, property, _value);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
}

template<class Property>
QUntypedPropertyBinding qtjambi_get_binding(const QUntypedPropertyData *d) {
    QUntypedPropertyBinding result;
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject binding = Property::binding(env, property);
                result = qtjambi_cast<QUntypedPropertyBinding>(env, binding);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    return result;
}

template<class Property>
QUntypedPropertyBinding qtjambi_set_binding(QUntypedPropertyData *d, const QUntypedPropertyBinding &binding) {
    QUntypedPropertyBinding result;
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject _binding = qtjambi_cast<jobject>(env, binding);
                jobject oldBinding = Property::setBinding(env, property, Java::QtCore::QPropertyBinding::newInstance(env, _binding));
                result = qtjambi_cast<QUntypedPropertyBinding>(env, oldBinding);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    return result;
}

template<class Property>
QUntypedPropertyBinding qtjambi_make_binding(const QUntypedPropertyData *d, const QPropertyBindingSourceLocation &) {
    QUntypedPropertyBinding result;
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject binding = Property::makeBinding(env, property);
                result = qtjambi_cast<QUntypedPropertyBinding>(env, binding);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    return result;
}

template<class Property>
void qtjambi_set_observer(const QUntypedPropertyData *d, QPropertyObserver *observer){
    QTJAMBI_TRY_ANY{
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QTJAMBI_TRY{
                jobject property = qtjambi_cast<jobject>(env, d);
                jobject bindingData = Property::bindingData(env, property);
                if(QtPrivate::QPropertyBindingData* data = qtjambi_cast<QtPrivate::QPropertyBindingData*>(env, bindingData))
                    observer->setSource(*data);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
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
                                                    QTJAMBI_TRY_ANY{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            QTJAMBI_TRY{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property))
                                                                    *static_cast<T*>(value) = Property::value(env, property);
                                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                                exn.report(env);
                                                            }QTJAMBI_TRY_END
                                                        }
                                                    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
                                              },
                                       /*setter=*/ [](QUntypedPropertyData *d, const void *value){
                                                    QTJAMBI_TRY_ANY{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            QTJAMBI_TRY{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                QT_WARNING_PUSH
                                                                QT_WARNING_DISABLE_CLANG("-Wdouble-promotion")
                                                                QT_WARNING_DISABLE_GCC("-Wdouble-promotion")
                                                                if(Property::isInstanceOf(env, property))
                                                                    Property::setValue(env, property, *static_cast<const T*>(value));
                                                                QT_WARNING_POP
                                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                                exn.report(env);
                                                            }QTJAMBI_TRY_END
                                                        }
                                                    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
                                              },
                                       /*getBinding=*/ [](const QUntypedPropertyData *d) -> QUntypedPropertyBinding {
                                                    QUntypedPropertyBinding result;
                                                    QTJAMBI_TRY_ANY{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            QTJAMBI_TRY{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property)){
                                                                    jobject binding = Property::binding(env, property);
                                                                    result = qtjambi_cast<QUntypedPropertyBinding>(env, binding);
                                                                }
                                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                                exn.report(env);
                                                            }QTJAMBI_TRY_END
                                                        }
                                                    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
                                                    return result;
                                              },
                                       /*setBinding=*/ [](QUntypedPropertyData *d, const QUntypedPropertyBinding &binding) -> QUntypedPropertyBinding {
                                                    QUntypedPropertyBinding result;
                                                    QTJAMBI_TRY_ANY{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            QTJAMBI_TRY{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property)){
                                                                   jobject _binding = qtjambi_cast<jobject>(env, binding);
                                                                   jobject oldBinding = Property::setBinding(env, property, PropertyBinding::newInstance(env, _binding));
                                                                   result = qtjambi_cast<QUntypedPropertyBinding>(env, oldBinding);
                                                                }
                                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                                exn.report(env);
                                                            }QTJAMBI_TRY_END
                                                        }
                                                    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
                                                    return result;
                                              },
                                       /*makeBinding=*/ [](const QUntypedPropertyData *d, const QPropertyBindingSourceLocation &) -> QUntypedPropertyBinding{
                                                    QUntypedPropertyBinding result;
                                                    QTJAMBI_TRY_ANY{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            QTJAMBI_TRY{
                                                                jobject property = qtjambi_find_object(env, d);
                                                                if(Property::isInstanceOf(env, property)){
                                                                    jobject binding = Property::makeBinding(env, property);
                                                                    result = qtjambi_cast<QUntypedPropertyBinding>(env, binding);
                                                                }
                                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                                exn.report(env);
                                                            }QTJAMBI_TRY_END
                                                        }
                                                    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
                                                    return result;
                                              },
                                       /*setObserver=*/ [](const QUntypedPropertyData *d, QPropertyObserver *observer){
                                                QTJAMBI_TRY_ANY{
                                                    if(JNIEnv* env = qtjambi_current_environment()){
                                                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                        QTJAMBI_TRY{
                                                           jobject property = qtjambi_find_object(env, d);
                                                           if(Property::isInstanceOf(env, property)){
                                                                jobject bindingData = Property::bindingData(env, property);
                                                                if(QtPrivate::QPropertyBindingData* data = qtjambi_cast<QtPrivate::QPropertyBindingData*>(env, bindingData))
                                                                    observer->setSource(*data);
                                                           }
                                                        }QTJAMBI_CATCH(const JavaException& exn){
                                                            exn.report(env);
                                                        }QTJAMBI_TRY_END
                                                    }
                                                }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
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
                                                        QTJAMBI_TRY_ANY{
                                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                                QTJAMBI_TRY{
                                                                   jobject property = qtjambi_cast<jobject>(env, d);
                                                                   *static_cast<T*>(value) = Property::value(env, property);
                                                                }QTJAMBI_CATCH(const JavaException& exn){
                                                                    exn.report(env);
                                                                }QTJAMBI_TRY_END
                                                            }
                                                        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
                                                   },
                                       /*setter=*/ nullptr,
                                       /*getBinding=*/ nullptr,
                                       /*setBinding=*/ nullptr,
                                       /*makeBinding=*/ nullptr,
                                       /*setObserver=*/ [](const QUntypedPropertyData *d, QPropertyObserver *observer){
                                                    QTJAMBI_TRY_ANY{
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                            QTJAMBI_TRY{
                                                                jobject property = qtjambi_cast<jobject>(env, d);
                                                                jobject bindingData = Java::QtCore::QObject$QAbstractComputedProperty::bindingData(env, property);
                                                                if(QtPrivate::QPropertyBindingData* data = qtjambi_cast<QtPrivate::QPropertyBindingData*>(env, bindingData))
                                                                    observer->setSource(*data);
                                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                                exn.report(env);
                                                            }QTJAMBI_TRY_END
                                                        }
                                                    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
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
    jobject _result = nullptr;
    QTJAMBI_TRY{
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
                    QtPrivate::QBindableInterface::GetMetaType getMetaType = /*metaType=*/ qtjambi_function_pointer<32,QMetaType()>([metaType]()->QMetaType { return QMetaType(metaType); }, hash_type(metaType));
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
        _result = qtjambi_cast<jobject>(__jni_env, iface);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

#define PRIMITIVE_BINDING_INTERFACE_METHOD(UTYPE,TYPE)\
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##UTYPE##Bindable_bindableInterface)\
(JNIEnv *__jni_env, jclass, jint propertyType)\
{\
    jobject _result = nullptr;\
    QTJAMBI_TRY{\
    switch(propertyType){\
    case 0:{\
        static QtPrivate::QBindableInterface _iface = PrimitiveBindableInterfaceHelper<0,TYPE>::create();\
        _result = qtjambi_cast<jobject>(__jni_env, &_iface); break;\
    }\
    break;\
    case 1:{\
        static QtPrivate::QBindableInterface _iface = PrimitiveBindableInterfaceHelper<1,TYPE>::create();\
        _result = qtjambi_cast<jobject>(__jni_env, &_iface); break;\
    }\
    break;\
    case 2:{\
        static QtPrivate::QBindableInterface _iface = PrimitiveBindableInterfaceHelper<2,TYPE>::create();\
        _result = qtjambi_cast<jobject>(__jni_env, &_iface); break;\
    }\
    break;\
    }\
    }QTJAMBI_CATCH(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }QTJAMBI_TRY_END\
    return _result;\
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
 jobject _this)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        const QtPrivate::QBindableInterface *iface = qtjambi_to_object<QtPrivate::QBindableInterface>(__jni_env, _this);
        if (iface && iface->metaType)
            result = qtjambi_cast<jobject>(__jni_env, iface->metaType());
        else result = qtjambi_cast<jobject>(__jni_env, QMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

template<class Property>
void signal_callback(QUntypedPropertyData *d){
    if(d){
        QTJAMBI_TRY_ANY{
            if(JNIEnv *env = qtjambi_current_environment()) {
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                QTJAMBI_TRY{
                    jobject property = qtjambi_find_object(env, d);
                    if(Property::isInstanceOf(env, property))
                        Property::emitSignal(env, property);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
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

size_t sizeof_QPropertyObserver_shell();
void __qt_destruct_QPropertyObserver(void* ptr);
size_t sizeof_QPropertyObserver_shell();
void __qt_construct_QPropertyObserver_with_ChangeHandler(void* __qtjambi_ptr, void (*changeHandler)(QPropertyObserver*, QUntypedPropertyData *));
void __qt_construct_QPropertyObserver_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);
void deleter_QPropertyObserver(void *ptr, bool);

void __qt_construct_QPropertyChangeHandler(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QPropertyChangeHandler()")
    __qt_construct_QPropertyObserver_with_ChangeHandler(__qtjambi_ptr, [](QPropertyObserver* self, QUntypedPropertyData *){
            QTJAMBI_TRY_ANY{
                if(JNIEnv *env = qtjambi_current_environment()) {
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    QTJAMBI_TRY{
                        jobject _self = qtjambi_cast<jobject>(env, self);
                        // during GC deletion 'self' is dangling pointer and java object is mapped to QPropertyObserver
                        if(Java::QtCore::QPropertyChangeHandler::isInstanceOf(env, _self)){
                            Java::QtCore::QPropertyChangeHandler::invoke(env, _self);
                        }
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
            }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
        });
}

void __qt_construct_QPropertyNotifier(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QPropertyNotifier()")
    __qt_construct_QPropertyObserver_with_ChangeHandler(__qtjambi_ptr, [](QPropertyObserver* self, QUntypedPropertyData *){
            QTJAMBI_TRY_ANY{
                if(JNIEnv *env = qtjambi_current_environment()) {
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    QTJAMBI_TRY{
                        jobject _self = qtjambi_cast<jobject>(env, self);
                        // during GC deletion 'self' is dangling pointer and java object is mapped to QPropertyObserver
                        if(Java::QtCore::QPropertyNotifier::isInstanceOf(env, _self)){
                            Java::QtCore::QPropertyNotifier::invoke(env, _self);
                        }
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
            }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
        });
}

// QPropertyObserver::QPropertyObserver(QUntypedPropertyData * aliasedPropertyPtr)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QAbstractPropertyAlias_initialize)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject aliasedPropertyPtr0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPropertyObserver::QPropertyObserver(QUntypedPropertyData *)")
    QTJAMBI_TRY{
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
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_construct_QPropertyObserver_1, sizeof_QPropertyObserver_shell(), *typeId, true, &deleter_QPropertyObserver, &arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QPropertyChangeHandler::QPropertyChangeHandler()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPropertyChangeHandler_initialize)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPropertyChangeHandler::QPropertyChangeHandler()")
    QTJAMBI_TRY{
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_construct_QPropertyChangeHandler, sizeof_QPropertyObserver_shell(), typeid(QPropertyChangeHandler<void(*)()>), true, &deleter_QPropertyObserver, nullptr);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPropertyNotifier_initialize)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPropertyNotifier::QPropertyNotifier()")
    QTJAMBI_TRY{
#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_construct_QPropertyNotifier, sizeof_QPropertyObserver_shell(), typeid(QPropertyNotifier), true, &deleter_QPropertyObserver, nullptr);
#else
        JavaException::raiseQNoImplementationException(__jni_env, "QPropertyNotifier is not available prior to Qt 6.2." QTJAMBI_STACKTRACEINFO );
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#define PRIMITIVE_PROPERTY_DATA(Boxed,jprimitive,primitive,jvalMember)\
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##Boxed##PropertyData_initialize_1native)\
(JNIEnv *__jni_env,\
 jclass __jni_class,\
 jobject __jni_object,\
 jprimitive val)\
{\
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Q" #Boxed "PropertyData::Q" #Boxed "PropertyData()")\
    jvalue args;\
    args.jvalMember = val;\
    QTJAMBI_TRY{\
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object,\
                                         [](void* __qtjambi_ptr, JNIEnv*, jobject, jvalue* __java_arguments){\
                                            Q_ASSERT(__java_arguments);\
                                            new(__qtjambi_ptr) QPropertyData<primitive>(__java_arguments->jvalMember);\
                                         },\
                                        qMax(sizeof(QUntypedPropertyData), sizeof(QPropertyData<primitive>)), typeid(QUntypedPropertyData), false,\
                                        [](void *ptr,bool){ delete reinterpret_cast<QPropertyData<primitive> *>(ptr); }, &args);\
    }QTJAMBI_CATCH(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }QTJAMBI_TRY_END\
}\
extern "C" Q_DECL_EXPORT jprimitive JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##Boxed##PropertyData_getValueBypassingBindings)\
(JNIEnv *__jni_env,\
 jclass,\
 QtJambiNativeID thisId)\
{\
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Q" #Boxed "PropertyData::getValueBypassingBindings()")\
    jprimitive _result{0};\
    QTJAMBI_TRY{\
        QPropertyData<primitive> *__qt_this = qtjambi_object_from_nativeId<QPropertyData<primitive>>(thisId);\
        qtjambi_check_resource(__jni_env, __qt_this);\
        _result = __qt_this->valueBypassingBindings();\
    }QTJAMBI_CATCH(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }QTJAMBI_TRY_END\
    return _result;\
}\
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Q##Boxed##PropertyData_setValueBypassingBindings)\
(JNIEnv *__jni_env,\
 jclass,\
 QtJambiNativeID thisId,\
 jprimitive val\
 )\
{\
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Q" #Boxed "PropertyData::setValueBypassingBindings(value)")\
    jboolean result = false;\
    QTJAMBI_TRY{\
        QPropertyData<primitive> *__qt_this = qtjambi_object_from_nativeId<QPropertyData<primitive>>(thisId);\
        qtjambi_check_resource(__jni_env, __qt_this);\
        if(primitive(val)!=__qt_this->valueBypassingBindings()){\
            __qt_this->setValueBypassingBindings(val);\
            result = true;\
        }\
    }QTJAMBI_CATCH(const JavaException& exn){\
        exn.raiseInJava(__jni_env);\
    }QTJAMBI_TRY_END\
    return result;\
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

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QProperty_initialize_1native)
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
    QTJAMBI_TRY{
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
                                            [](void *ptr,bool){
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
                                            [](void *ptr,bool){
                                                QMetaType metaType;
                                                {
                                                    QWriteLocker locker(gLock());
                                                    metaType = gMetaTypesByPointer->take(ptr);
                                                }
                                                metaType.destruct(ptr);
                                                operator delete(ptr);
                                            }, args);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QProperty_getValueBypassingBindings)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID thisId,
 QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QProperty::getValueBypassingBindings()")
    jobject _result = nullptr;
    QTJAMBI_TRY{
        void *__qt_this = qtjambi_from_nativeId(thisId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QMetaType metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
        if(metaType.flags() & QMetaType::IsPointer){
            void* value = static_cast<QPropertyData<void*> *>(__qt_this)->valueBypassingBindings();
            _result = qtjambi_cast<jobject>(__jni_env, QVariant(metaType, &value));
        }else{
            switch(metaType.id()){
            case QMetaType::QByteArray:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QByteArray> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QByteArrayList:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QByteArrayList> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QString:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QString> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QStringList:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QStringList> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QVariantList:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantList> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QVariantMap:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantMap> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QVariantHash:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantHash> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QVariantPair:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariantPair> *>(__qt_this)->valueBypassingBindings()); break;
            case QMetaType::QVariant:
                _result = qtjambi_cast<jobject>(__jni_env, static_cast<QPropertyData<QVariant> *>(__qt_this)->valueBypassingBindings()); break;
            default:
                _result = qtjambi_cast<jobject>(__jni_env, QVariant(metaType, __qt_this)); break;
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
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
    jboolean _result = false;
    QTJAMBI_TRY{
        void *__qt_this = qtjambi_from_nativeId(thisId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QMetaType metaType = qtjambi_value_from_nativeId<QMetaType>(metaTypeId);
        if(metaType.flags() & QMetaType::IsPointer){
            QPropertyData<void*> *propertyPtr = static_cast<QPropertyData<void*> *>(__qt_this);
            void* _val = nullptr;
            bool compare = true;
            if(val){
                QVariant variant = qtjambi_to_qvariant(__jni_env, val);
                if(variant.metaType()!=metaType && !variant.convert(metaType))
                    compare = false;
                else
                    _val = *reinterpret_cast<void*const*>(variant.constData());
            }else{
                _val = nullptr;
            }
            if (compare && _val != propertyPtr->valueBypassingBindings()){
                propertyPtr->setValueBypassingBindings(_val);
                _result = true;
            }
        }else{
            switch(metaType.id()){
            case QMetaType::QByteArray: {
                    QPropertyData<QByteArray> *propertyPtr = static_cast<QPropertyData<QByteArray> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArray>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QByteArrayList: {
                    QPropertyData<QByteArrayList> *propertyPtr = static_cast<QPropertyData<QByteArrayList> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QByteArrayList>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QString: {
                    QPropertyData<QString> *propertyPtr = static_cast<QPropertyData<QString> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QString>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QStringList: {
                    QPropertyData<QStringList> *propertyPtr = static_cast<QPropertyData<QStringList> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QStringList>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QVariantList: {
                    QPropertyData<QVariantList> *propertyPtr = static_cast<QPropertyData<QVariantList> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantList>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QVariantMap: {
                    QPropertyData<QVariantMap> *propertyPtr = static_cast<QPropertyData<QVariantMap> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantMap>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QVariantHash: {
                    QPropertyData<QVariantHash> *propertyPtr = static_cast<QPropertyData<QVariantHash> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantHash>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QVariantPair: {
                    QPropertyData<QVariantPair> *propertyPtr = static_cast<QPropertyData<QVariantPair> *>(__qt_this);
                    propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariantPair>(__jni_env, val));
                    _result = true;
                }break;
            case QMetaType::QVariant: {
                   QPropertyData<QVariant> *propertyPtr = static_cast<QPropertyData<QVariant> *>(__qt_this);
                   propertyPtr->setValueBypassingBindings(qtjambi_cast<QVariant>(__jni_env, val));
                   _result = true;
               }break;
            default: {
                   QVariant variant = val ? qtjambi_to_qvariant(__jni_env, val) : QVariant(metaType);
                   if(variant.metaType()!=metaType && !variant.convert(metaType)){
                        _result = false;
                   }else if(metaType.isEqualityComparable() && metaType.equals(__qt_this, variant.constData())){
                        _result = false;
                   }else{
                       metaType.destruct(__qt_this);
                       metaType.construct(__qt_this, variant.constData());
                       _result = true;
                   }
               }break;
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

template<class T>
auto future_createFunction(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const QFuture<T>& parentFuture)->QVariant{
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject _parentFuture = qtjambi_cast<jobject>(env, parentFuture);
            jobject result = Java::Runtime::Function::apply(env, _function, _parentFuture);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
    };
}

template<class T>
inline auto future_createConsumer(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const QFuture<T>& parentFuture){
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject _parentFuture = qtjambi_cast<jobject>(env, parentFuture);
            Java::Runtime::Consumer::accept(env, _function, _parentFuture);
        }
    };
}

inline auto future_createExceptionHandler(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function](const std::exception& exception)->QVariant{
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jthrowable _exception = nullptr;
            if(const JavaException* javaException = dynamic_cast<const JavaException*>(&exception)){
                _exception = javaException->object();
            }else{
                QTJAMBI_TRY{
                    JavaException::raiseRuntimeException(env, exception.what() QTJAMBI_STACKTRACEINFO );
                }QTJAMBI_CATCH(const JavaException& exn){
                    _exception = exn.object();
                }QTJAMBI_TRY_END
            }
            jobject result = Java::Runtime::Function::apply(env, _function, _exception);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
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
                QTJAMBI_TRY{
                    JavaException::raiseRuntimeException(env, exception.what() QTJAMBI_STACKTRACEINFO );
                }QTJAMBI_CATCH(const JavaException& exn){
                    _exception = exn.object();
                }QTJAMBI_TRY_END
            }
            Java::Runtime::Consumer::accept(env, _function, _exception);
        }
    };
}

inline auto future_createCancelHandler(JNIEnv * env, jobject function){
    JObjectWrapper _function(env, function);
    return [_function]()->QVariant{
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            jobject result = Java::Runtime::Supplier::get(env, _function);
            return qtjambi_cast<QVariant>(env, result);
        }
        return QVariant();
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

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_then)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<QVariant> result = iface->future().then(future_createFunction<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<QVariant> result = future.then(future_createFunction<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenLaunch)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jint policy, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<QVariant> result = iface->future().then(QtFuture::Launch(policy), future_createFunction<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<QVariant> result = future.then(QtFuture::Launch(policy), future_createFunction<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenPool)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID pool, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        QThreadPool *_pool = qtjambi_object_from_nativeId<QThreadPool>(pool);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<QVariant> result = iface->future().then(_pool, future_createFunction<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<QVariant> result = future.then(_pool, future_createFunction<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<QVariant> result = iface->future().then(context, future_createFunction<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<QVariant> result = future.then(context, future_createFunction<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
#else
        Q_UNUSED(__this_nativeId);
        Q_UNUSED(contextId);
        Q_UNUSED(function);
        JavaException::raiseQNoImplementationException(__jni_env, "No implementation available prior to Qt 6.1." QTJAMBI_STACKTRACEINFO );
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<void> result = iface->future().then(future_createConsumer<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<void> result = future.then(future_createConsumer<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenLaunchVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jint policy, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<void> result = iface->future().then(QtFuture::Launch(policy), future_createConsumer<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<void> result = future.then(QtFuture::Launch(policy), future_createConsumer<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenPoolVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID pool, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        QThreadPool *_pool = qtjambi_object_from_nativeId<QThreadPool>(pool);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<void> result = iface->future().then(_pool, future_createConsumer<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<void> result = future.then(_pool, future_createConsumer<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_thenContextVoid)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            QFuture<void> result = iface->future().then(context, future_createConsumer<QVariant>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            QFuture<void> future(base);
            QFuture<void> result = future.then(context, future_createConsumer<void>(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
#else
        Q_UNUSED(__this_nativeId);
        Q_UNUSED(contextId);
        Q_UNUSED(function);
        JavaException::raiseQNoImplementationException(__jni_env, "No implementation available prior to Qt 6.1." QTJAMBI_STACKTRACEINFO );
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onFailed)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            if(!Java::Runtime::Function::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<T>::onFailed() can only be called with Function<Throwable, T>." QTJAMBI_STACKTRACEINFO );
            QFuture<QVariant> result = iface->future().onFailed(future_createExceptionHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            if(!Java::Runtime::Consumer::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<void>::onFailed() can only be called with Consumer<Throwable>." QTJAMBI_STACKTRACEINFO );
            QFuture<void> future(base);
            QFuture<void> result = future.onFailed(futurevoid_createExceptionHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onFailedContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            if(!Java::Runtime::Function::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<T>::onFailed() can only be called with Function<Throwable, T>." QTJAMBI_STACKTRACEINFO );
            QFuture<QVariant> result = iface->future().onFailed(context, future_createExceptionHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            if(!Java::Runtime::Consumer::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<void>::onFailed() can only be called with Consumer<Throwable>." QTJAMBI_STACKTRACEINFO );
            QFuture<void> future(base);
            QFuture<void> result = future.onFailed(context, futurevoid_createExceptionHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
#else
        Q_UNUSED(__this_nativeId);
        Q_UNUSED(contextId);
        Q_UNUSED(function);
        JavaException::raiseQNoImplementationException(__jni_env, "No implementation available prior to Qt 6.1." QTJAMBI_STACKTRACEINFO );
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onCanceled)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            if(!Java::Runtime::Supplier::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<T>::onCanceled() can only be called with Supplier<T>." QTJAMBI_STACKTRACEINFO );
            QFuture<QVariant> result = iface->future().onCanceled(future_createCancelHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            if(!Java::Runtime::Runnable::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<void>::onCanceled() can only be called with Runnable." QTJAMBI_STACKTRACEINFO );
            QFuture<void> future(base);
            QFuture<void> result = future.onCanceled(futurevoid_createCancelHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFuture_onCanceledContext)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID contextId, jobject function)
{
    jobject _result = nullptr;
    QTJAMBI_TRY{
#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)
        QFutureInterfaceBase* base = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, base);
        QObject *context = qtjambi_object_from_nativeId<QObject>(contextId);
        if(QFutureInterface<QVariant>* iface = dynamic_cast<QFutureInterface<QVariant>*>(base)){
            if(!Java::Runtime::Supplier::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<T>::onCanceled() can only be called with Supplier<T>." QTJAMBI_STACKTRACEINFO );
            QFuture<QVariant> result = iface->future().onCanceled(context, future_createCancelHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }else{
            if(!Java::Runtime::Runnable::isInstanceOf(__jni_env, function))
                JavaException::raiseIllegalArgumentException(__jni_env, "QFuture<void>::onCanceled() can only be called with Runnable." QTJAMBI_STACKTRACEINFO );
            QFuture<void> future(base);
            QFuture<void> result = future.onCanceled(context, futurevoid_createCancelHandler(__jni_env, function));
            _result = qtjambi_cast<jobject>(__jni_env, result);
        }
#else
        Q_UNUSED(__this_nativeId);
        Q_UNUSED(contextId);
        Q_UNUSED(function);
        JavaException::raiseQNoImplementationException(__jni_env, "No implementation available prior to Qt 6.1." QTJAMBI_STACKTRACEINFO );
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QModelRoleData::setData(T)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QModelRoleData_setData)
(JNIEnv *__jni_env,
 jobject _this, jobject data)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QModelRoleData::setData(T)")
    QTJAMBI_TRY{
#if QT_CONFIG(itemmodel)
        QModelRoleData *__qt_this = qtjambi_to_object<QModelRoleData>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->data() = qtjambi_cast<QVariant>(__jni_env, data);
#else
        Q_UNUSED(__this_nativeId)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(itemmodel)
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
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
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_initialize_1native__Lio_qt_core_QByteArrayView_2Lio_qt_core_QByteArray_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject data)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(QByteArray)")
    QTJAMBI_TRY{
        jvalue argument;
        argument.l = data;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_1, sizeof(QByteArrayView), typeid(QByteArrayView), false, &argument);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
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
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_initialize_1native__Lio_qt_core_QByteArrayView_2Ljava_nio_ByteBuffer_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject data)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(Buffer)")
    QTJAMBI_TRY{
        jvalue argument;
        argument.l = data;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_2, sizeof(QByteArrayView), typeid(QByteArrayView), false, &argument);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// new QByteArrayView(Buffer)
void __qt_construct_QByteArrayView_3(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(Buffer)")
    JBufferConstData* bufferData = new JBufferConstData(__jni_env, __java_arguments[0].l);
    if(bufferData->size()>0){
        jobject address = qtjambi_from_long(__jni_env, qintptr(bufferData));
        __jni_env->SetObjectArrayElement(jobjectArray(__java_arguments[1].l), 0, address);
        qtjambi_throw_java_exception(__jni_env);
        new(__qtjambi_ptr) QByteArrayView(reinterpret_cast<const char*>(bufferData->data()), bufferData->size());
    }else{
        delete bufferData;
        new(__qtjambi_ptr) QByteArrayView();
    }
    Q_UNUSED(__jni_object)
}

// QByteArrayView::QByteArrayView(Buffer)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_initialize_1native__Lio_qt_core_QByteArrayView_2Ljava_nio_Buffer_2_3Ljava_lang_Long_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject data, jobjectArray pointerOut)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(Buffer)")
    QTJAMBI_TRY{
        jvalue argument[2];
        argument[0].l = data;
        argument[1].l = pointerOut;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_3, sizeof(QByteArrayView), typeid(QByteArrayView), false, argument);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_purgeBuffer__J)
(JNIEnv * __jni_env, jclass, jlong pointer)
{
    QTJAMBI_TRY{
        JBufferConstData* bufferData = reinterpret_cast<JBufferConstData*>(pointer);
        delete bufferData;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// new QByteArrayView(String)
void __qt_construct_QByteArrayView_4(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(String)")
    J2CStringBuffer* bufferData = new J2CStringBuffer(__jni_env, jstring(__java_arguments[0].l));
    if(bufferData->length()>0){
        jobject address = qtjambi_from_long(__jni_env, qintptr(bufferData));
        __jni_env->SetObjectArrayElement(jobjectArray(__java_arguments[1].l), 0, address);
        qtjambi_throw_java_exception(__jni_env);
        new(__qtjambi_ptr) QByteArrayView(reinterpret_cast<const char*>(bufferData->data()), bufferData->length());
    }else{
        delete bufferData;
        new(__qtjambi_ptr) QByteArrayView();
    }
    Q_UNUSED(__jni_object)
}

// QByteArrayView::QByteArrayView(String)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_initialize_1native__Lio_qt_core_QByteArrayView_2Ljava_lang_String_2_3Ljava_lang_Long_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring data, jobjectArray pointerOut)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(String)")
    QTJAMBI_TRY{
        jvalue argument[2];
        argument[0].l = data;
        argument[1].l = pointerOut;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_4, sizeof(QByteArrayView), typeid(QByteArrayView), false, argument);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_purgeString__J)
(JNIEnv * __jni_env, jclass, jlong pointer)
{
    QTJAMBI_TRY{
        J2CStringBuffer* bufferData = reinterpret_cast<J2CStringBuffer*>(pointer);
        delete bufferData;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// new QByteArrayView(byte[])
void __qt_construct_QByteArrayView_5(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QByteArrayView(byte[])")
    JByteArrayPointer* bufferData = new JByteArrayPointer(__jni_env, jbyteArray(__java_arguments[0].l));
    if(bufferData->size()>0){
        jobject address = qtjambi_from_long(__jni_env, qintptr(bufferData));
        __jni_env->SetObjectArrayElement(jobjectArray(__java_arguments[1].l), 0, address);
        qtjambi_throw_java_exception(__jni_env);
        new(__qtjambi_ptr) QByteArrayView(reinterpret_cast<const char*>(bufferData), bufferData->size());
    }else{
        delete bufferData;
        new(__qtjambi_ptr) QByteArrayView();
    }
    Q_UNUSED(__jni_object)
}

// QByteArrayView::QByteArrayView(byte[])
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_initialize_1native__Lio_qt_core_QByteArrayView_2_3B_3Ljava_lang_Long_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring data, jobjectArray pointerOut)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayView::QByteArrayView(byte[])")
    QTJAMBI_TRY{
        jvalue argument[2];
        argument[0].l = data;
        argument[1].l = pointerOut;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QByteArrayView_5, sizeof(QByteArrayView), typeid(QByteArrayView), false, argument);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArrayView_purgeBytes__J)
(JNIEnv * __jni_env, jclass, jlong pointer)
{
    QTJAMBI_TRY{
        JByteArrayPointer* bufferData = reinterpret_cast<JByteArrayPointer*>(pointer);
        delete bufferData;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_nativeinterface_QAndroidApplication_runOnAndroidMainThread__Ljava_util_function_Supplier_2Lio_qt_core_QDeadlineTimer_2)
(JNIEnv * env, jclass, jobject runnable, jobject timer)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
        QDeadlineTimer deadlineTimer = qtjambi_cast<QDeadlineTimer>(env, timer);
        JObjectWrapper _runnable(env, runnable);
        _result = qtjambi_cast<jobject>(env, QNativeInterface::QAndroidApplication::runOnAndroidMainThread([_runnable]()->QVariant{
            if(JNIEnv * _env = qtjambi_current_environment()){
                jobject value = Java::Runtime::Supplier::get(_env, _runnable.object());
                return qtjambi_cast<QVariant>(_env, value);
            }else{
                return QVariant();
            }
        }, deadlineTimer));
#else
        Q_UNUSED(runnable)
        Q_UNUSED(timer)
        JavaException::raiseQNoImplementationException(env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_nativeinterface_QAndroidApplication_runOnAndroidMainThread__Ljava_lang_Runnable_2Lio_qt_core_QDeadlineTimer_2)
(JNIEnv * env, jclass, jobject runnable, jobject timer)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
        QDeadlineTimer deadlineTimer = qtjambi_cast<QDeadlineTimer>(env, timer);
        JObjectWrapper _runnable(env, runnable);
        _result = qtjambi_cast<jobject>(env, QNativeInterface::QAndroidApplication::runOnAndroidMainThread([_runnable](){
            if(JNIEnv * _env = qtjambi_current_environment()){
                Java::Runtime::Runnable::run(_env, _runnable.object());
            }
        }, deadlineTimer));
#else
        Q_UNUSED(runnable)
        Q_UNUSED(timer)
        JavaException::raiseQNoImplementationException(env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)

#endif

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFunctionPointer_invoke_1native__Lio_qt_QtObjectInterface_2Ljava_lang_Object_2_3Ljava_lang_Object_2)
(JNIEnv * __jni_env, jclass, jobject __this,
 jobject returnType,
 jobjectArray arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFunctionPointer::invoke(...)")
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QFunctionPointer* __qt_this = qtjambi_to_interface<QFunctionPointer>(__jni_env, __this);
        qtjambi_check_resource(__jni_env, __qt_this);
        _result = qtjambi_invoke_function_pointer(__jni_env, *__qt_this, returnType, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFunctionPointer_cast)
(JNIEnv * __jni_env, jclass, jobject function,
 jclass functionalInterface)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFunctionPointerUtil::cast(...)")
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_function_pointer_cast(__jni_env, function, functionalInterface);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFunctionPointerUtil_returnType)
(JNIEnv * __jni_env, jclass, jobject returnType)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFunctionPointerUtil::returnType(..)")
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_function_pointer_return_type(__jni_env, returnType);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFunctionPointerUtil_getParameterTypes)
(JNIEnv * __jni_env, jclass, jobjectArray argumentTypes, jobjectArray parameterTypes, jboolean isVarArgs)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFunctionPointerUtil::getParameterTypes(..)")
    QTJAMBI_TRY{
        qtjambi_function_pointer_get_parameter_types(__jni_env, argumentTypes, parameterTypes, isVarArgs);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFunctionPointerUtil_convertParameters)
(JNIEnv * __jni_env, jclass, jobjectArray argumentTypes, jobjectArray args, jobjectArray convertedArgs, jboolean isVarArgs, jboolean forward)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFunctionPointerUtil::getParameterTypes(..)")
    QTJAMBI_TRY{
        qtjambi_function_pointer_convert_parameters(__jni_env, argumentTypes, args, convertedArgs, isVarArgs, forward);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFunctionPointerUtil_convertReturn)
(JNIEnv * __jni_env, jclass, jobject returnType, jobject result)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFunctionPointerUtil::convertReturn(..)")
    jobject _result{nullptr};
    QTJAMBI_TRY{
        _result = qtjambi_function_pointer_convert_return(__jni_env, returnType, result);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFunctionPointerUtil_dispose)
(JNIEnv * __jni_env, jclass, jlong peer)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFunctionPointerUtil::dispose(..)")
    QTJAMBI_TRY{
        qtjambi_function_pointer_dispose(__jni_env, peer);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void initialize_meta_info_QFutureInterface();

void initialize_meta_info_QtCore(){
    initialize_meta_info_QFutureInterface();

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    {
        struct AccessBindable : QUntypedBindable{
            const QtPrivate::QBindableInterface *iface() const{
                return QUntypedBindable::iface;
            }
        };
        const std::type_info& typeId = typeid(QUntypedBindable);
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<bool>>("QBindable<bool>", "io/qt/core/QBooleanBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.id()==QMetaType::Bool;
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<qint8>>("QBindable<qint8>", "io/qt/core/QByteBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==1
                                                    && (metaType.id()==QMetaType::SChar
                                                        || metaType.id()==QMetaType::Char
                                                        || metaType.id()==QMetaType::UChar);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<QChar>>("QBindable<QChar>", "io/qt/core/QCharBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==2
                                                    && (metaType.id()==QMetaType::QChar);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<char16_t>>("QBindable<char16_t>", "io/qt/core/QCharBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==2
                                                    && (metaType.id()==QMetaType::Char16);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<qint16>>("QBindable<qint16>", "io/qt/core/QShortBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==2
                                                    && (metaType.id()==QMetaType::Short
                                                        || metaType.id()==QMetaType::UShort);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<qint32>>("QBindable<qint32>", "io/qt/core/QIntBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==4
                                                    && (metaType.id()==QMetaType::Int
                                                        || metaType.id()==QMetaType::UInt
                                                        || metaType.id()==QMetaType::Long
                                                        || metaType.id()==QMetaType::ULong
                                                        || metaType.id()==QMetaType::Char32);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<qint64>>("QBindable<qint64>", "io/qt/core/QLongBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==8
                                                    && (metaType.id()==QMetaType::Long
                                                        || metaType.id()==QMetaType::LongLong
                                                        || metaType.id()==QMetaType::ULong
                                                        || metaType.id()==QMetaType::ULongLong);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<char32_t>>("QBindable<char32_t>", "io/qt/core/QIntBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType==QMetaType::fromType<char32_t>();
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<float>>("QBindable<float>", "io/qt/core/QFloatBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==4
                                                    && (metaType.id()==QMetaType::Float
                                                        || metaType.id()==QMetaType::Double);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<double>>("QBindable<double>", "io/qt/core/QDoubleBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return metaType.sizeOf()==8
                                                    && (metaType.id()==QMetaType::Float
                                                        || metaType.id()==QMetaType::Double);
                                        }
                                        return false;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QBindable<QVariant>>("QBindable<QVariant>", "io/qt/core/QBindable"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                                        Q_ASSERT(bindable);
                                        offset = 0;
                                        if(bindable->iface()
                                                && bindable->iface()->metaType){
                                            QMetaType metaType = bindable->iface()->metaType();
                                            return !(metaType.id()==QMetaType::SChar
                                                     || metaType.id()==QMetaType::Char
                                                     || metaType.id()==QMetaType::UChar
                                                     || metaType.id()==QMetaType::QChar
                                                     || metaType.id()==QMetaType::Char16
                                                     || metaType.id()==QMetaType::Short
                                                     || metaType.id()==QMetaType::UShort
                                                     || metaType.id()==QMetaType::Int
                                                     || metaType.id()==QMetaType::UInt
                                                     || metaType.id()==QMetaType::Char32
                                                     || metaType.id()==QMetaType::Long
                                                     || metaType.id()==QMetaType::LongLong
                                                     || metaType.id()==QMetaType::ULong
                                                     || metaType.id()==QMetaType::ULongLong
                                                     || metaType.id()==QMetaType::Float
                                                     || metaType.id()==QMetaType::Double
                                                     || metaType.id()==QMetaType::Bool
                                                     || metaType.id()==QMetaType::Void
                                                     || metaType.id()==QMetaType::UnknownType);
                                        }
                                        return false;
                                    });
        registerMetaType<QBindable<bool>>("QBindable<bool>");
        registerMetaType<QBindable<qint8>>("QBindable<qint8>");
        registerMetaType<QBindable<qint16>>("QBindable<qint16>");
        registerMetaType<QBindable<qint32>>("QBindable<qint32>");
        registerMetaType<QBindable<char32_t>>("QBindable<char32_t>");
        registerMetaType<QBindable<qint64>>("QBindable<qint64>");
        registerMetaType<QBindable<float>>("QBindable<float>");
        registerMetaType<QBindable<double>>("QBindable<double>");
        registerMetaType<QBindable<QChar>>("QBindable<QChar>");
        registerMetaType<QBindable<char16_t>>("QBindable<char16_t>");
        registerMetaType<QBindable<QVariant>>("QBindable<QVariant>");
    }

    {
        const std::type_info& typeId = typeid(QUntypedPropertyBinding);
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<bool>>("QPropertyBinding<bool>", "io/qt/core/QBooleanPropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.id()==QMetaType::Bool;
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<qint8>>("QPropertyBinding<qint8>", "io/qt/core/QBytePropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.sizeOf()==1
                                                && (metaType.id()==QMetaType::SChar
                                                    || metaType.id()==QMetaType::Char
                                                    || metaType.id()==QMetaType::UChar);
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<QChar>>("QPropertyBinding<QChar>", "io/qt/core/QCharPropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.sizeOf()==2
                                                && (metaType.id()==QMetaType::QChar
                                                    || metaType.id()==QMetaType::Char16);
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<qint16>>("QPropertyBinding<qint16>", "io/qt/core/QShortPropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.sizeOf()==2
                                                && (metaType.id()==QMetaType::Short
                                                    || metaType.id()==QMetaType::UShort);
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<qint32>>("QPropertyBinding<qint32>", "io/qt/core/QIntPropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.sizeOf()==4
                                                && (metaType.id()==QMetaType::Int
                                                    || metaType.id()==QMetaType::UInt
                                                    || metaType.id()==QMetaType::Long
                                                    || metaType.id()==QMetaType::ULong
                                                    || metaType.id()==QMetaType::Char32);
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<qint64>>("QPropertyBinding<qint64>", "io/qt/core/QLongPropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.sizeOf()==8
                                                && (metaType.id()==QMetaType::Long
                                                    || metaType.id()==QMetaType::LongLong
                                                    || metaType.id()==QMetaType::ULong
                                                    || metaType.id()==QMetaType::ULongLong);
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<float>>("QPropertyBinding<float>", "io/qt/core/QFloatPropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.sizeOf()==4
                                                && (metaType.id()==QMetaType::Float
                                                    || metaType.id()==QMetaType::Double);
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<double>>("QPropertyBinding<double>", "io/qt/core/QDoublePropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return metaType.sizeOf()==8
                                                && (metaType.id()==QMetaType::Float
                                                    || metaType.id()==QMetaType::Double);
                                    });
        registerPolymorphyHandler(typeId, registerValueTypeInfo<QPropertyBinding<QVariant>>("QPropertyBinding<QVariant>", "io/qt/core/QPropertyBinding"),
                                  [](void *ptr, qintptr& offset) -> bool {
                                        QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                                        Q_ASSERT(binding);
                                        offset = 0;
                                        QMetaType metaType = binding->valueMetaType();
                                        return !(metaType.id()==QMetaType::SChar
                                                 || metaType.id()==QMetaType::Char
                                                 || metaType.id()==QMetaType::UChar
                                                 || metaType.id()==QMetaType::QChar
                                                 || metaType.id()==QMetaType::Char16
                                                 || metaType.id()==QMetaType::Short
                                                 || metaType.id()==QMetaType::UShort
                                                 || metaType.id()==QMetaType::Int
                                                 || metaType.id()==QMetaType::UInt
                                                 || metaType.id()==QMetaType::Char32
                                                 || metaType.id()==QMetaType::Long
                                                 || metaType.id()==QMetaType::LongLong
                                                 || metaType.id()==QMetaType::ULong
                                                 || metaType.id()==QMetaType::ULongLong
                                                 || metaType.id()==QMetaType::Float
                                                 || metaType.id()==QMetaType::Double
                                                 || metaType.id()==QMetaType::Void
                                                 || metaType.id()==QMetaType::UnknownType);
                                    });
        registerMetaType<QPropertyBinding<bool>>("QPropertyBinding<bool>");
        registerMetaType<QPropertyBinding<qint8>>("QPropertyBinding<qint8>");
        registerMetaType<QPropertyBinding<QChar>>("QPropertyBinding<QChar>");
        registerMetaType<QPropertyBinding<qint16>>("QPropertyBinding<qint16>");
        registerMetaType<QPropertyBinding<qint32>>("QPropertyBinding<qint32>");
        registerMetaType<QPropertyBinding<qint64>>("QPropertyBinding<qint64>");
        registerMetaType<QPropertyBinding<float>>("QPropertyBinding<float>");
        registerMetaType<QPropertyBinding<double>>("QPropertyBinding<double>");
        registerMetaType<QPropertyBinding<QVariant>>("QPropertyBinding<QVariant>");
    }

    {
        registerValueTypeInfo<QPropertyAlias<bool>>("QPropertyAlias<bool>", "io/qt/core/QBooleanPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint8>>("QPropertyAlias<qint8>", "io/qt/core/QBytePropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint16>>("QPropertyAlias<qint16>", "io/qt/core/QShortPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint32>>("QPropertyAlias<qint32>", "io/qt/core/QIntPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<char32_t>>("QPropertyAlias<char32_t>", "io/qt/core/QIntPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint64>>("QPropertyAlias<qint64>", "io/qt/core/QLongPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<float>>("QPropertyAlias<float>", "io/qt/core/QFloatPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<double>>("QPropertyAlias<double>", "io/qt/core/QDoublePropertyAlias");
        registerValueTypeInfo<QPropertyAlias<QChar>>("QPropertyAlias<QChar>", "io/qt/core/QCharPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<char16_t>>("QPropertyAlias<char16_t>", "io/qt/core/QCharPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<QVariant>>("QPropertyAlias<QVariant>", "io/qt/core/QPropertyAlias");
        registerMetaType<QPropertyAlias<bool>>("QPropertyAlias<bool>");
        registerMetaType<QPropertyAlias<qint8>>("QPropertyAlias<qint8>");
        registerMetaType<QPropertyAlias<qint16>>("QPropertyAlias<qint16>");
        registerMetaType<QPropertyAlias<qint32>>("QPropertyAlias<qint32>");
        registerMetaType<QPropertyAlias<char32_t>>("QPropertyAlias<char32_t>");
        registerMetaType<QPropertyAlias<qint64>>("QPropertyAlias<qint64>");
        registerMetaType<QPropertyAlias<float>>("QPropertyAlias<float>");
        registerMetaType<QPropertyAlias<double>>("QPropertyAlias<double>");
        registerMetaType<QPropertyAlias<QChar>>("QPropertyAlias<QChar>");
        registerMetaType<QPropertyAlias<char16_t>>("QPropertyAlias<char16_t>");
        registerMetaType<QPropertyAlias<QVariant>>("QPropertyAlias<QVariant>");
    }
    {
        const std::type_info& typeId = registerObjectTypeInfo<QPropertyChangeHandler<void(*)()>>("QPropertyChangeHandler", "io/qt/core/QPropertyChangeHandler");
        registerConstructorInfos(typeId, &__qt_destruct_QPropertyObserver, {
            ConstructorInfo(&__qt_construct_QPropertyChangeHandler, nullptr)
        });
        registerDeleter(typeId, &deleter_QPropertyObserver);
        registerSizeOfShell(typeId, sizeof_QPropertyObserver_shell());
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
    {
        const std::type_info& typeId = registerObjectTypeInfo<QPropertyNotifier>("QPropertyNotifier", "io/qt/core/QPropertyNotifier");
        registerConstructorInfos(typeId, &__qt_destruct_QPropertyObserver, {
            ConstructorInfo(&__qt_construct_QPropertyNotifier, nullptr)
        });
        registerDeleter(typeId, &deleter_QPropertyObserver);
        registerSizeOfShell(typeId, sizeof_QPropertyObserver_shell());
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
            registerValueTypeInfo<QOperatingSystemVersionBase>("QOperatingSystemVersion", "io/qt/core/QOperatingSystemVersion");
#endif

#endif

#endif

}
