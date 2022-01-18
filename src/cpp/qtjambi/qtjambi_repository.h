/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_REPOSITORY_H
#define QTJAMBI_REPOSITORY_H

#include "qtjambi_global.h"

#include "qtjambi_repodefines.h"

QTJAMBI_EXPORT jclass resolveClass(JNIEnv *env, const char *className, jobject classLoader = nullptr);

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_REPOSITORY_DECLARE_THROW_NEW\
    public: static inline void throwNew(JNIEnv* env, const char* message, const char *methodName, const char *fileName, int lineNumber){\
        JavaException(env, newInstanceWithMessage(env, message)).raise(env, methodName, fileName, lineNumber);\
    }\
    public: static inline void throwNew(JNIEnv* env, const QString& message, const char *methodName, const char *fileName, int lineNumber){\
        JavaException(env, newInstanceWithMessage(env, message)).raise(env, methodName, fileName, lineNumber);\
    }
#else
#define QTJAMBI_REPOSITORY_DECLARE_THROW_NEW\
    public: static inline void throwNew(JNIEnv* env, const char* message){\
        JavaException(env, newInstanceWithMessage(env, message)).raise();\
    }\
    public: static inline void throwNew(JNIEnv* env, const QString& message){\
        JavaException(env, newInstanceWithMessage(env, message)).raise();\
    }
#endif

#define QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()\
    private: jmethodID __constructor;\
    public: static inline jthrowable newInstance(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(_this.class_ref,_this.__constructor,args);\
        va_end(args);\
        JavaException::check(env);\
        return jthrowable(result);\
    }\
    public: static inline jthrowable newInstanceWithMessage(JNIEnv* env,const char* message){\
        return newInstance(env, env->NewStringUTF(message));\
    }\
    public: static inline jthrowable newInstanceWithMessage(JNIEnv* env,const QString& message){\
        return newInstance(env, env->NewStringUTF(qPrintable(message)));\
    }\
    QTJAMBI_REPOSITORY_DECLARE_THROW_NEW

QT_WARNING_DISABLE_CLANG("-Wdollar-in-identifier-extension")

namespace Java{
namespace Runtime
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Object,
              QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
              QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(equals)
              QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(hashCode)
              QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(toString))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Supplier,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(getAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getAsInt))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getAsLong))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(BooleanSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(getAsBoolean))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Consumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ObjDoubleConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ObjIntConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ObjLongConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(BiConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runnable,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Function,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(BiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ToIntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleToIntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongToIntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ToIntBiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ToLongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleToLongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntToLongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ToLongBiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ToDoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntToDoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongToDoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ToDoubleBiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(BiPredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Predicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoublePredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntPredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongPredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntUnaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleUnaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongUnaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntBinaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleBinaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongBinaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Number,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(intValue)
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(doubleValue)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(longValue)
                  QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(floatValue)
                  QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(shortValue)
                  QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(byteValue))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Integer,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Double,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Boolean,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(booleanValue)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(getBoolean)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Long,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Float,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Short,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Character,
                  QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(charValue)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Byte,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Void,
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(String)

    QTJAMBI_REPOSITORY_DECLARE_CLASS(ByteBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asCharBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asDoubleBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asFloatBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asIntBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asLongBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asShortBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(IntBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(LongBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(ShortBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(FloatBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(DoubleBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(CharBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Buffer,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDirect)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isReadOnly)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(capacity)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(limit)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(position)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(mark)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(array)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasArray)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(arrayOffset))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(AtomicBoolean,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(AtomicReference,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(AtomicInteger,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(AtomicLong,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))
}

namespace QtCore{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCoreApplication,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_FIELD(__qt_isInitializing))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QDeclarativeConstructor,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(placement)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(cls))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QThread,
        QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(javaThread)
        QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(interruptible))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaObject$Slot1,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaObject$Slot0,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericValue, QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(type))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBindable,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createBinding)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QBooleanProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QCharProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QByteProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QIntProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QShortProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QLongProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QDoubleProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QFloatProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasSignal)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(emitSignal)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QAbstractComputedProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedBooleanProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedCharProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedByteProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedIntProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedShortProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedLongProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedDoubleProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QObject$QComputedFloatProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
}

namespace QtGui{
QTJAMBI_REPOSITORY_DECLARE_CLASS(QValidator$QValidationData,
              QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
              QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(string)
              QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(position))
}
}

#undef DECLARE_CLASS_REF
#undef REPOSITORY_EXPORT
#define REPOSITORY_EXPORT
#endif // QTJAMBI_REPOSITORY_H

