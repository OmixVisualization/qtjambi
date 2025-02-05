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

#include "utils_p.h"
#include <QtCore/qendian.h>

namespace Java{
namespace QtCore{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QUnhandledException, QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QException, QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;))
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFile$TrashResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLjava/lang/String;)
)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getValueBypassingBindings,()Ljava/lang/Object;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(Ljava/lang/Object;)Z)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(valueMetaType,()Lio/qt/core/QMetaType;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QProperty,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Object;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(Ljava/lang/Object;)V)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QPropertyBinding;)Lio/qt/core/QPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanProperty,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Z)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(Z)V)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QBooleanPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QBooleanPropertyBinding;)Lio/qt/core/QBooleanPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QBooleanPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QByteProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()B)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(B)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QBytePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QBytePropertyBinding;)Lio/qt/core/QBytePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QBytePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortProperty,
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()S)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(S)V)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QShortPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QShortPropertyBinding;)Lio/qt/core/QShortPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QShortPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()C)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(C)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QCharPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QCharPropertyBinding;)Lio/qt/core/QCharPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QCharPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntProperty,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(I)V)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QIntPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QIntPropertyBinding;)Lio/qt/core/QIntPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QIntPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongProperty,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()J)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(J)V)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QLongPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QLongPropertyBinding;)Lio/qt/core/QLongPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QLongPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoubleProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()D)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(D)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QDoublePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QDoublePropertyBinding;)Lio/qt/core/QDoublePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QDoublePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatProperty,
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()F)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(F)V)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QFloatPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QFloatPropertyBinding;)Lio/qt/core/QFloatPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QFloatPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(analyzeMetaType,(Ljava/io/Serializable;)Lio/qt/core/QMetaType;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBytePropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoublePropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QByteBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoubleBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBytePropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoublePropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(I)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(S)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBytePropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(B)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(J)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoublePropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(D)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(F)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(Z)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(C)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyChangeHandler,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,()V))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyNotifier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,()V))
#endif
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QString$Predicate,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(test,(C)Z)
                                )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QByteArray$Predicate,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(test,(B)Z)
                                )

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCborStreamReader$StringResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Lio/qt/core/QCborStreamReader$StringResultCode;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCborValue$FromCborResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QCborValue;Lio/qt/core/QCborParserError;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDate$Week,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(II)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLockFile$LockInfo,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JLjava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QJsonDocument$FromJsonResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QJsonDocument;Lio/qt/core/QJsonParseError;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QSharedMemory,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(__qt_accessMode,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDebug,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(__rcDevice,Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(disabled,Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QRunnable,)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFuture$Supplier,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Ljava/lang/Object;)
                                )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFuture$Runnable,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(run,()V)
                                )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFuture$Consumer,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;)V)
                                )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFuture$Function,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(Ljava/lang/Object;)Ljava/lang/Object;)
                                )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFuture$FutureConsumer,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Lio/qt/core/QFuture;)V)
                                )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFuture$FutureFunction,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(Lio/qt/core/QFuture;)Ljava/lang/Object;)
                                )
namespace Internal
{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core/internal,QAbstractFileEngineHandler,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(registerFactory,(JLio/qt/core/internal/QAbstractFileEngineHandler$FileEngineFactory;)V)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(unregisterFactory,(J)V)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createByFactory,(JLjava/lang/String;)Lio/qt/core/internal/QAbstractFileEngine;))
}
}

namespace QtWidgets
{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QWidget,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setParent,(Lio/qt/widgets/QWidget;)V))
}

namespace QtGui
{
#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P) QTJAMBI_REPOSITORY_DEFINE_METHOD(M,P)
#else
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P)
#endif


QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QGuiApplication,
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(exec,()I)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(resolveInterface,(Ljava/lang/Class;)Lio/qt/QtObjectInterface;)
                                )
}

namespace QtJambi
{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QUnsuccessfulInvocationException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,ReflectionUtility,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(invocationInfoProvider,()Ljava/util/function/IntFunction;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,InternalAccess$CallerContext,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(declaringClass, Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(methodName, Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(lineNumber, I)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$ShortSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsShort,()S))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$ByteSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsByte,()B))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$FloatSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsFloat,()F))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$CharSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsChar,()C))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QFlags,)
}

namespace Runtime{
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/math,BigInteger,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I[B)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(abs,()Ljava/math/BigInteger;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(signum,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(bitLength,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(longValue,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toByteArray,()[B)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,DirectByteBuffer,
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(att,Ljava/lang/Object;)
                                )
}

}

BigIntegerValue fromBigInteger(JNIEnv* env, jobject object)
{
    Q_ASSERT(object);
    BigIntegerValue value{0, false, false};
    int signum = Java::Runtime::BigInteger::signum(env, object);
    if(signum<0){
        object = Java::Runtime::BigInteger::abs(env, object);
        value.isNegative = true;
    }
    int bitLength = Java::Runtime::BigInteger::bitLength(env, object);
    if(bitLength<=64){
        jbyteArray data = jbyteArray(Java::Runtime::BigInteger::toByteArray(env, object));
        JConstByteArrayPointer pointer(env, data);
        if(pointer.size()>=8){
            value.value = qFromBigEndian<quint64>(pointer.pointer()+(pointer.size()-8));
        }else{
            char data[8]{0,0,0,0,0,0,0,0};
            memcpy(data+(8-pointer.size()), pointer.pointer(), size_t(pointer.size()));
            value.value = qFromBigEndian<quint64>(data);
        }
    }else{
        value.outOfRange = true;
    }
    return value;
}

jobject toBigInteger(JNIEnv* env, quint64 value, bool isNegative)
{
    jobject object = nullptr;
    jbyteArray data = env->NewByteArray(8);
    jbyte* array = env->GetByteArrayElements(data, nullptr);
    qToBigEndian<quint64>(value, array);
    env->ReleaseByteArrayElements(data, array, 0);
    object = Java::Runtime::BigInteger::newInstance(env, value==0 ? 0 : (isNegative ? -1 : 1), data);
    return object;
}
