/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusUnixFileDescriptor>
#include <QtDBus/qdbusmetatype.h>
#include <QtDBus/private/qdbusargument_p.h>
#include <QtDBus/QDBusReply>
#include <QtCore/QVariant>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusPendingCall>
#include <QtCore/QIODevice>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/RegistryAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/qtjambi_cast.h>
#include <QtJambi/FunctionPointer>
#include <QtJambi/JObjectWrapper>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#define qtjambiMetaType qMetaTypeId
#else
#define qtjambiMetaType QMetaType::fromType
#endif

// emitting (writeSignalFunction)
// emitting (writeConstructors)
// new QDBusReply(const QDBusError & dbusError)
void __qt_construct_QDBusReply_0(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QDBusReply(const QDBusError & dbusError)")
    QDBusReply<QVariant> *__qt_this = new(__qtjambi_ptr) QDBusReply<QVariant>();
    Q_UNUSED(__qt_this)
}

// new QDBusReply(const QDBusError & dbusError)
void __qt_construct_QDBusReply_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QDBusReply(const QDBusError & dbusError)")
    jobject dbusError0 = __java_arguments[0].l;
    const QDBusError& __qt_dbusError0 = qtjambi_cast<const QDBusError& >(__jni_env, dbusError0);
    QDBusReply<QVariant> *__qt_this = new(__qtjambi_ptr) QDBusReply<QVariant>(__qt_dbusError0);
    Q_UNUSED(__qt_this)
}

struct DBusReply{
    QDBusError m_error;
    QVariant m_data;
    DBusReply(const QDBusMessage& reply, const QMetaType& metaType) : m_error(), m_data(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                                     .id()
#endif
                                                                                              , nullptr) {
        qDBusReplyFill(reply, m_error, m_data);
    }
};

// new QDBusReply(const QDBusMessage & reply)
void __qt_construct_QDBusReply_2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QDBusReply(const QDBusMessage & reply)")
    jobject reply0 = __java_arguments[0].l;
    jobject metaType1 = __java_arguments[1].l;
    const QDBusMessage& reply = qtjambi_cast<const QDBusMessage& >(__jni_env, reply0);
    const QMetaType& metaType = qtjambi_cast<const QMetaType& >(__jni_env, metaType1);
    new(__qtjambi_ptr) DBusReply(reply, metaType);
    Q_UNUSED(__jni_object)
}

// new QDBusReply(const QDBusPendingCall & pcall)
void __qt_construct_QDBusReply_3(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QDBusReply(const QDBusPendingCall & pcall)")
    jobject pcall0 = __java_arguments[0].l;
    jobject metaType1 = __java_arguments[1].l;
    const QDBusPendingCall& pcall = qtjambi_cast<const QDBusPendingCall& >(__jni_env, pcall0);
    const QMetaType& metaType = qtjambi_cast<const QMetaType& >(__jni_env, metaType1);
    QDBusPendingCall other(pcall);
    other.waitForFinished();
    new(__qtjambi_ptr) DBusReply(other.reply(), metaType);
    Q_UNUSED(__jni_object)
}

// destruct QDBusReply
void __qt_destruct_QDBusReply(void* ptr)
{
    QTJAMBI_NATIVE_METHOD_CALL("destruct QDBusReply<QVariant>")
    reinterpret_cast<QDBusReply<QVariant>*>(ptr)->~QDBusReply<QVariant>();
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QDBusReply::QDBusReply(const QDBusError & dbusError)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply_initialize_1native__Lio_qt_dbus_QDBusReply_2Lio_qt_dbus_QDBusError_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject dbusError0)
{
    QTJAMBI_NATIVE_METHOD_CALL("QDBusReply::QDBusReply(const QDBusError & dbusError)")
    QTJAMBI_TRY{
        jvalue arguments;
        arguments.l = dbusError0;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QDBusReply_1, sizeof(QDBusReply<QVariant>), typeid(QDBusReply<QVariant>), false, &arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END

}

// QDBusReply::QDBusReply(const QDBusMessage & reply)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply_initialize_1native__Lio_qt_dbus_QDBusReply_2Lio_qt_dbus_QDBusMessage_2Lio_qt_core_QMetaType_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject reply0,
 jobject metaType)
{
    QTJAMBI_NATIVE_METHOD_CALL("QDBusReply::QDBusReply(const QDBusMessage & reply)")
    QTJAMBI_TRY{
        jvalue arguments[2];
        arguments[0].l = reply0;
        arguments[1].l = metaType;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QDBusReply_2, sizeof(QDBusReply<QVariant>), typeid(QDBusReply<QVariant>), false, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END

}

// QDBusReply::QDBusReply(const QDBusPendingCall & pcall)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply_initialize_1native__Lio_qt_dbus_QDBusReply_2Lio_qt_dbus_QDBusPendingCall_2Lio_qt_core_QMetaType_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject pcall0,
 jobject metaType)
{
    QTJAMBI_NATIVE_METHOD_CALL("QDBusReply::QDBusReply(const QDBusPendingCall & pcall)")
    QTJAMBI_TRY{
        jvalue arguments[2];
        arguments[0].l = pcall0;
        arguments[1].l = metaType;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QDBusReply_3, sizeof(QDBusReply<QVariant>), typeid(QDBusReply<QVariant>), false, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END

}

// QDBusReply::error() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply_error)
(JNIEnv *__jni_env,
 jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        const QDBusReply<QVariant> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(__jni_env, _this);
        Q_ASSERT(__qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QDBusReply::error() const", __qt_this)
        const QDBusError& __qt_return_value = __qt_this->error();
        _result = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QDBusReply::isValid() const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply_isValid)
(JNIEnv *__jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        const QDBusReply<QVariant> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(__jni_env, _this);
        Q_ASSERT(__qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QDBusReply::isValid() const", __qt_this)
        _result = __qt_this->isValid();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QDBusReply::operator=(const QDBusReply<QVariant > & other)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply_set)
(JNIEnv *__jni_env,
 jobject _this,
 jobject other0)
{
    jobject _result{nullptr};
    QtJambiScope __qtjambi_scope(__jni_env, _this);
    QTJAMBI_TRY{
        QDBusReply<QVariant> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(__jni_env, _this);
        Q_ASSERT(__qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QDBusReply::operator=(const QDBusReply<QVariant > & other)", __qt_this)
        const QDBusReply<QVariant>&  __qt_other0 = qtjambi_cast<QDBusReply<QVariant> >(__jni_env, __qtjambi_scope, other0);
        QDBusReply<QVariant>& __qt_return_value = ((*static_cast<QDBusReply<QVariant>*>(__qt_this)) = __qt_other0);
        _result = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QDBusReply::value() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply_value)
(JNIEnv *__jni_env,
 jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        const QDBusReply<QVariant> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(__jni_env, _this);
        Q_ASSERT(__qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QDBusReply::value() const", __qt_this)
        QVariant __qt_return_value = __qt_this->value();
        _result = qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

template<typename T>
void qtjambiDBusRegisterMetaType(int metaType)
{
    auto mf = [](QDBusArgument &arg, const void *t) { arg << *static_cast<const T *>(t); };
    auto df = [](const QDBusArgument &arg, void *t) { arg >> *static_cast<T *>(t); };
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QDBusMetaType::registerMarshallOperators(metaType, mf, df);
#else
    QDBusMetaType::registerMarshallOperators(QMetaType(metaType), mf, df);
#endif
}

template<>
void qtjambiDBusRegisterMetaType<std::nullptr_t>(int metaType)
{
    auto mf = [](QDBusArgument &arg, const void *t) { arg << *static_cast<const qlonglong *>(t); };
    auto df = [](const QDBusArgument &arg, void *t) { arg >> *static_cast<qlonglong *>(t); };
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QDBusMetaType::registerMarshallOperators(metaType, mf, df);
#else
    QDBusMetaType::registerMarshallOperators(QMetaType(metaType), mf, df);
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusMetaType_registerDBusMetaType)
(JNIEnv *__jni_env,
 jclass,
 jint metaTypeId,
 jclass classType,
 jobject marshallFunction,
 jobject demarshallFunction)
{
    QTJAMBI_TRY{
        QMetaType metaType(metaTypeId);
        if(metaTypeId==QMetaType::QString
                || metaTypeId==QMetaType::QStringList
                || metaTypeId==QMetaType::QByteArray
                || metaTypeId==QMetaType::UChar
                || metaTypeId==QMetaType::Bool
                || metaTypeId==QMetaType::Short
                || metaTypeId==QMetaType::UShort
                || metaTypeId==QMetaType::Int
                || metaTypeId==QMetaType::UInt
                || metaTypeId==QMetaType::LongLong
                || metaTypeId==QMetaType::ULongLong
                || metaTypeId==QMetaType::Double
                || metaTypeId==QMetaType::QVariant
                || metaTypeId==qMetaTypeId<QDBusVariant>()
                || metaTypeId==qMetaTypeId<QDBusObjectPath>()
                || metaTypeId==qMetaTypeId<QDBusSignature>()
                || metaTypeId==qMetaTypeId<QDBusUnixFileDescriptor>()
                || metaTypeId==qMetaTypeId<JIntArrayWrapper>()
                || metaTypeId==qMetaTypeId<JByteArrayWrapper>()
                || metaTypeId==qMetaTypeId<JShortArrayWrapper>()
                || metaTypeId==qMetaTypeId<JLongArrayWrapper>()
                || metaTypeId==qMetaTypeId<JBooleanArrayWrapper>()
                || metaTypeId==qMetaTypeId<JCharArrayWrapper>()
                || metaTypeId==qMetaTypeId<JDoubleArrayWrapper>()
                || metaTypeId==qMetaTypeId<JFloatArrayWrapper>()){
            return;
#ifndef QDBUS_NO_SPECIALTYPES
        }else if(metaTypeId==QMetaType::Nullptr){
            qtjambiDBusRegisterMetaType<std::nullptr_t>(metaTypeId);
        }else if(metaTypeId==QMetaType::QDate){
            qtjambiDBusRegisterMetaType<QDate>(metaTypeId);
        }else if(metaTypeId==QMetaType::QTime){
            qtjambiDBusRegisterMetaType<QTime>(metaTypeId);
        }else if(metaTypeId==QMetaType::QDateTime){
            qtjambiDBusRegisterMetaType<QDateTime>(metaTypeId);
        }else if(metaTypeId==QMetaType::QRect){
            qtjambiDBusRegisterMetaType<QRect>(metaTypeId);
        }else if(metaTypeId==QMetaType::QRectF){
            qtjambiDBusRegisterMetaType<QRectF>(metaTypeId);
        }else if(metaTypeId==QMetaType::QSize){
            qtjambiDBusRegisterMetaType<QSize>(metaTypeId);
        }else if(metaTypeId==QMetaType::QSizeF){
            qtjambiDBusRegisterMetaType<QSizeF>(metaTypeId);
        }else if(metaTypeId==QMetaType::QPoint){
            qtjambiDBusRegisterMetaType<QPoint>(metaTypeId);
        }else if(metaTypeId==QMetaType::QPointF){
            qtjambiDBusRegisterMetaType<QPointF>(metaTypeId);
        }else if(metaTypeId==QMetaType::QLine){
            qtjambiDBusRegisterMetaType<QLine>(metaTypeId);
        }else if(metaTypeId==QMetaType::QLineF){
            qtjambiDBusRegisterMetaType<QLineF>(metaTypeId);
#endif
        }else{
            QDBusMetaType::MarshallFunction mf(nullptr);
            QDBusMetaType::DemarshallFunction df(nullptr);
            if(!marshallFunction || !demarshallFunction){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                {
                    void*const* ptr = reinterpret_cast<void*const*>(&metaType);
                    if(!ptr[2] || !ptr[3])
                        Java::Runtime::RuntimeException::throwNew(__jni_env, QStringLiteral("Meta type %1 does not provide data stream operators.").arg(QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
                }
#else
                if(!metaType.hasRegisteredDataStreamOperators())
                    Java::Runtime::RuntimeException::throwNew(__jni_env, QStringLiteral("Meta type %1 does not provide data stream operators.").arg(QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
#endif
                mf = qtjambi_function_pointer<16,void(QDBusArgument &, const void *)>([metaTypeId](QDBusArgument &arg, const void *t) {
                    QByteArray data;
                    {
                        QDataStream stream(&data, QIODevice::WriteOnly);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                        QMetaType::save(stream, metaTypeId, t);
#else
                        QMetaType(metaTypeId).save(stream, t);
#endif
                    }
                    arg.beginStructure();
                    arg << data;
                    arg.endStructure();
                }, qHash(metaTypeId));
                df = qtjambi_function_pointer<16,void(const QDBusArgument &, void *)>([metaTypeId](const QDBusArgument &arg, void *t) {
                    QByteArray data;
                    arg.beginStructure();
                    arg >> data;
                    arg.endStructure();
                    {
                        QDataStream stream(&data, QIODevice::ReadOnly);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                        QMetaType::load(stream, metaTypeId, t);
#else
                        QMetaType(metaTypeId).load(stream, t);
#endif
                    }
                }, qHash(metaTypeId));
            }else if(metaTypeId==qMetaTypeId<JObjectWrapper>()
                     || metaTypeId==qMetaTypeId<JMapWrapper>()
                     || metaTypeId==qMetaTypeId<JCollectionWrapper>()
                     || metaTypeId==qMetaTypeId<JIteratorWrapper>()
                     || metaTypeId==qMetaTypeId<JObjectArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JIntArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JByteArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JLongArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JFloatArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JDoubleArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JBooleanArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JCharArrayWrapper>()
                     || metaTypeId==qMetaTypeId<JShortArrayWrapper>()){
                Java::Runtime::IllegalArgumentException::throwNew(__jni_env, QStringLiteral("Unable to marhall/unmarshall type %1.").arg(QtJambiAPI::getClassName(__jni_env, classType)) QTJAMBI_STACKTRACEINFO );
            }else{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                if(!QMetaType(metaType).iface()->defaultCtr
                        || !QMetaType(metaType).iface()->copyCtr){
                    Java::Runtime::RuntimeException::throwNew(__jni_env, QStringLiteral("Meta type %1 does not provide default and copy constructor.").arg(QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
                }
#endif
                JObjectWrapper _marshallFunction(__jni_env, marshallFunction);
                mf = qtjambi_function_pointer<16,void(QDBusArgument &, const void *)>([metaTypeId,_marshallFunction](QDBusArgument &arg, const void *t) {
                    if(JniEnvironment env{300}){
                        jobject _arg = qtjambi_cast<jobject>(env, &arg);
                        QTJAMBI_INVALIDATE_AFTER_USE(env, _arg);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                        QVariant variant(metaTypeId, t);
#else
                        QVariant variant(QMetaType(metaTypeId), t);
#endif
                        Java::Runtime::BiConsumer::accept(env, _marshallFunction.object(), _arg, qtjambi_cast<jobject>(env, variant));
                    }
                }, qHash(metaTypeId));
                JObjectWrapper _demarshallFunction(__jni_env, demarshallFunction);
                df = qtjambi_function_pointer<16,void(const QDBusArgument &, void *)>([metaTypeId,_demarshallFunction](const QDBusArgument &arg, void *t) {
                    if(JniEnvironment env{300}){
                        jobject _arg = qtjambi_cast<jobject>(env, &arg);
                        QTJAMBI_INVALIDATE_AFTER_USE(env, _arg);
                        jobject result = Java::Runtime::Function::apply(env, _demarshallFunction.object(), _arg);
                        QVariant variant = qtjambi_cast<QVariant>(env, result);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                        if(variant.userType()!=metaTypeId){
                            if(!variant.convert(metaTypeId))
#else
                        QMetaType metaType(metaTypeId);
                        if(variant.metaType()!=metaType){
                            if(!variant.convert(metaType))
#endif
                                JavaException::raiseRuntimeException(env, "Unable to convert variant." QTJAMBI_STACKTRACEINFO );
                        }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                        QMetaType::destruct(metaTypeId, t);
                        QMetaType::construct(metaTypeId, t, variant.constData());
#else
                        metaType.destruct(t);
                        metaType.construct(t, variant.constData());
#endif
                    }
                }, qHash(metaTypeId));
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QDBusMetaType::registerMarshallOperators(metaTypeId, mf, df);
#else
            QDBusMetaType::registerMarshallOperators(metaType, mf, df);
#endif
    }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jstring JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusMetaType_typeToSignature)
(JNIEnv *__jni_env,
 jclass,
 jobject _metaType)
{
    jstring _result{nullptr};
    QTJAMBI_TRY{
        const QMetaType& metaType = qtjambi_cast<const QMetaType&>(__jni_env, _metaType);
        _result = __jni_env->NewStringUTF(QDBusMetaType::typeToSignature(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                              .id()
#endif
                                                                      ));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusMetaType_signatureToMetaType)
(JNIEnv *__jni_env,
 jclass,
 jstring _signature)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        J2CStringBuffer signature(__jni_env, _signature);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QMetaType metaType(QDBusMetaType::signatureToType(signature.constData()));
#else
        QMetaType metaType(QDBusMetaType::signatureToMetaType(signature.constData()));
#endif
        _result = qtjambi_cast<jobject>(__jni_env, metaType);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusMetaType_demarshall)
(JNIEnv *__jni_env,
 jclass,
 jobject _arg,
 jobject _metaType)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        const QDBusArgument* arg = qtjambi_cast<const QDBusArgument*>(__jni_env, _arg);
        QtJambiAPI::checkNullPointer(__jni_env, arg);
        const QMetaType& metaType = qtjambi_cast<const QMetaType&>(__jni_env, _metaType);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QVariant variant(metaType.id(), nullptr);
#else
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
        if(!QMetaType(metaType).iface()->copyCtr || !QMetaType(metaType).iface()->defaultCtr){
            Java::Runtime::UnsupportedOperationException::throwNew(__jni_env, QStringLiteral("Unable to extract value of type %1 from QDBusArgument.").arg(QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
        }
#endif
        QVariant variant(metaType, nullptr);
#endif
        if(QDBusMetaType::demarshall(*arg, metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                  .id()
#endif
                                                    , variant.data())){
            _result = QtJambiAPI::convertQVariantToJavaObject(__jni_env, variant);
        }else{
            Java::Runtime::UnsupportedOperationException::throwNew(__jni_env, QStringLiteral("Unable to extract value of type %1 from QDBusArgument.").arg(QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusMetaType_marshall)
(JNIEnv *__jni_env,
 jclass,
 jobject _arg,
 jobject _metaType,
 jobject _value)
{
    QTJAMBI_TRY{
        QDBusArgument* arg = qtjambi_cast<QDBusArgument*>(__jni_env, _arg);
        QtJambiAPI::checkNullPointer(__jni_env, arg);
        QMetaType* metaType = qtjambi_cast<QMetaType*>(__jni_env, _metaType);
        QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(__jni_env, _value);
        if(metaType){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            if(variant.userType()!=metaType->id()){
                if(!variant.convert(metaType->id()))
#else
            if(variant.metaType()!=*metaType){
                if(!variant.convert(*metaType))
#endif
                    Java::Runtime::IllegalArgumentException::throwNew(__jni_env, QStringLiteral("Unable to convert value to type %1.").arg(QLatin1String(metaType->name())) QTJAMBI_STACKTRACEINFO );
            }
        }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if(!QDBusMetaType::marshall(*arg,
                                   variant.userType(),
                                   variant.constData())){
            Java::Runtime::UnsupportedOperationException::throwNew(__jni_env, QStringLiteral("Unable to append %1 value to QDBusArgument.").arg(QLatin1String(QMetaType::typeName(variant.userType()))) QTJAMBI_STACKTRACEINFO );
        }
#else
        if(!QDBusMetaType::marshall(*arg,
                                   variant.metaType(),
                                   variant.constData())){
            Java::Runtime::UnsupportedOperationException::throwNew(__jni_env, QStringLiteral("Unable to append %1 value to QDBusArgument.").arg(QLatin1String(variant.metaType().name())) QTJAMBI_STACKTRACEINFO );
        }
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void qtjambi_dbus_check_write_argument(JNIEnv * env, const QDBusArgument * arg){
    QDBusArgumentPrivate* d = arg ? QDBusArgumentPrivate::d(*const_cast<QDBusArgument *>(arg)) : nullptr;
    if(d && d->direction!=QDBusArgumentPrivate
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                 ::Direction
#endif
                 ::Marshalling)
        Java::Runtime::IllegalAccessException::throwNew(env, "QDBusArgument: write from a read-only object" QTJAMBI_STACKTRACEINFO );
}

void qtjambi_dbus_check_read_argument(JNIEnv * env, const QDBusArgument * arg){
    QDBusArgumentPrivate* d = arg ? QDBusArgumentPrivate::d(*const_cast<QDBusArgument *>(arg)) : nullptr;
    if(d && d->direction!=QDBusArgumentPrivate
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                 ::Direction
#endif
                 ::Demarshalling)
        Java::Runtime::IllegalAccessException::throwNew(env, "QDBusArgument: read from a write-only object" QTJAMBI_STACKTRACEINFO );
}

bool qtjambi_dbus_is_read_argument(const QDBusArgument *arg){
    QDBusArgumentPrivate* d = arg ? QDBusArgumentPrivate::d(*const_cast<QDBusArgument *>(arg)) : nullptr;
    return d && d->direction==QDBusArgumentPrivate
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                    ::Direction
#endif
                    ::Demarshalling;
}

void initialize_meta_info_QtDBus()
{
    using namespace RegistryAPI;
    const std::type_info& typeId = registerUnspecificTypeInfo<QDBusReply<QVariant>>("QDBusReply", "io/qt/dbus/QDBusReply");
    registerOperators<QDBusReply<QVariant>>();
    int metaTypeID = registerMetaType<QDBusReply<QVariant>>("QDBusReply<QVariant>");
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QMetaType::registerNormalizedTypedef("QDBusReply", metaTypeID);
#else
    QMetaType::registerNormalizedTypedef("QDBusReply", QMetaType(metaTypeID));
#endif
    registerConstructorInfos(typeId, &__qt_destruct_QDBusReply, {
        ConstructorInfo(&__qt_construct_QDBusReply_0, nullptr)
       ,ConstructorInfo(&__qt_construct_QDBusReply_1, "Lio/qt/dbus/QDBusError;")
       ,ConstructorInfo(&__qt_construct_QDBusReply_2, "Lio/qt/dbus/QDBusMessage;")
       ,ConstructorInfo(&__qt_construct_QDBusReply_3, "Lio/qt/dbus/QDBusPendingCall;")
    });
    QDBusMetaType::MarshallFunction mf = [](QDBusArgument &arg, const void *t) {
        if(const JObjectWrapper* objectWrapper = reinterpret_cast<const JObjectWrapper*>(t)){
            QByteArray buffer;
            QDataStream s(&buffer, QIODevice::WriteOnly);
            s << *objectWrapper;
            arg.beginStructure();
            arg << buffer;
            arg.endStructure();
        }
    };
    QDBusMetaType::DemarshallFunction df = [](const QDBusArgument &arg, void *t) {
        if(JObjectWrapper* objectWrapper = reinterpret_cast<JObjectWrapper*>(t)){
            QByteArray buffer;
            arg.beginStructure();
            arg >> buffer;
            arg.endStructure();
            QDataStream s(&buffer, QIODevice::ReadOnly);
            s >> *objectWrapper;
        }
    };

    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JObjectWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JMapWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JCollectionWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JIteratorWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JObjectArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JIntArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JByteArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JShortArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JLongArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JBooleanArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JCharArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JDoubleArrayWrapper>(), mf, df);
    QDBusMetaType::registerMarshallOperators(qtjambiMetaType<JFloatArrayWrapper>(), mf, df);
}
