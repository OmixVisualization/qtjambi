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

#if !defined(QTJAMBI_JAVAAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAAPI_H

#include "javautils.h"

#define QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(type_name, members) QTJAMBI_REPOSITORY_DECLARE_CLASS_IMPL(QTJAMBI_EXPORT,type_name, members)
#define QTJAMBI_REPOSITORY_DECLARE_EXPORTED_EMPTY_CLASS(type_name) QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(type_name,)

namespace JavaAPI{

QTJAMBI_EXPORT jmethodID resolveMethod(JNIEnv *env, const char *methodName, const char *signature, jclass clazz, bool isStatic = false, jthrowable* exceptionOccurred = nullptr);
QTJAMBI_EXPORT jclass resolveClass(JNIEnv *env, const char *className, jobject classLoader = nullptr);
QTJAMBI_EXPORT jclass toGlobalReference(JNIEnv * env, jclass clazz);
QTJAMBI_EXPORT jclass resolveClosestQtSuperclass(JNIEnv *env, jclass clazz, jobject classLoader = nullptr);

}

namespace QtJambiPrivate{
QTJAMBI_EXPORT jstring convertString(JNIEnv *env, QAnyStringView s);
}

#define QTJAMBI_REPOSITORY_DECLARE_THROWABLE_BASE\
    private: jmethodID __constructor;\
    public: static inline jthrowable newInstance(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(_this.class_ref,_this.__constructor,args);\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
        return jthrowable(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()\
    QTJAMBI_REPOSITORY_DECLARE_THROWABLE_BASE\
    public: static inline jthrowable newInstanceWithMessage(JNIEnv* env, ::QAnyStringView message){\
        return newInstance(env, ::QtJambiPrivate::convertString(env, message));\
    }

QT_WARNING_DISABLE_CLANG("-Wdollar-in-identifier-extension")

namespace Java{
namespace Runtime
{
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Object,
              QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
              QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(equals)
              QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(hashCode)
              QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(toString)
              QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(clone)
              QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(clone)
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Class,
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_STRING_METHOD(getName)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECT_METHOD(cast)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECT_METHOD(getDeclaredMethods)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECT_METHOD(getDeclaredMethod)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECT_METHOD(getDeclaredField)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECT_METHOD(getMethod)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECTARRAY_METHOD(getEnumConstants)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECT_METHOD(getClassLoader)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_BOOLEAN_METHOD(isAnnotationPresent)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECT_METHOD(getAnnotation)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECTARRAY_METHOD(getInterfaces)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_INT_METHOD(getModifiers)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_CLASS_METHOD(getComponentType)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_CLASS_METHOD(getSuperclass)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_OBJECTARRAY_METHOD(getTypeParameters)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_BOOLEAN_METHOD(isArray)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_BOOLEAN_METHOD(isPrimitive)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_BOOLEAN_METHOD(isSynthetic)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_BOOLEAN_METHOD(isInterface)
                 public: static inline jstring tryGetName(JNIEnv* env,jclass object){
                     auto _this = __qt_get_this(env);
                     if(!_this.class_ref) return nullptr;
                     jobject result = env->CallObjectMethod(object,_this.__getName);
                     return jstring(result);
                 }
                 public: static inline jobject tryGetDeclaredMethod(JNIEnv* env,jclass object,jstring name,jobjectArray parameters){
                     auto _this = __qt_get_this(env);
                     if(!_this.class_ref) return nullptr;
                     jobject result = env->CallObjectMethod(object,_this.__getDeclaredMethod, name, parameters);
                     return jstring(result);
                 }
                 public: static inline jobject tryGetDeclaredField(JNIEnv* env,jclass object,jstring name){
                     auto _this = __qt_get_this(env);
                     if(!_this.class_ref) return nullptr;
                     jobject result = env->CallObjectMethod(object,_this.__getDeclaredField, name);
                     return jstring(result);
                 }
            )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Method,
        QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
        QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
        QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getReturnType))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Constructor,
        QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
        QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Executable,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getParameterTypes))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Field,
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                  QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getType))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Modifier,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isNative)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isStatic)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isFinal)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isAbstract))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ClassCastException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IllegalAccessException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(MissingResourceException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(NumberFormatException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IllegalArgumentException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IllegalStateException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IllegalAccessError,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(UnsupportedOperationException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(NoSuchElementException,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(RuntimeException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Error,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ArithmeticException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Supplier,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(getAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getAsInt))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getAsLong))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(BooleanSupplier,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(getAsBoolean))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Consumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ObjDoubleConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ObjIntConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ObjLongConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(BiConsumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Runnable,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Function,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(BiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ToIntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleToIntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongToIntFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ToIntBiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ToLongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleToLongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntToLongFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ToLongBiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ToDoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntToDoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongToDoubleFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ToDoubleBiFunction,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(BiPredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Predicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoublePredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntPredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongPredicate,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntUnaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleUnaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongUnaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntBinaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(applyAsInt))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleBinaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(applyAsDouble))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongBinaryOperator,
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(applyAsLong))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Optional,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(ofNullable))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(OptionalInt,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getAsInt)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(OptionalLong,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getAsLong)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(OptionalDouble,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(getAsDouble)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Number,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(intValue)
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(doubleValue)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(longValue)
                  QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(floatValue)
                  QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(shortValue)
                  QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(byteValue))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Integer,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Double,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Boolean,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(booleanValue)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(getBoolean)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Long,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(hashCode)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Float,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Short,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Character,
                  QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(charValue)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Byte,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Void,
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_EMPTY_CLASS(String)
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_EMPTY_CLASS(CharSequence)

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ByteBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asCharBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asDoubleBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asFloatBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asIntBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asLongBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asShortBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                  QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IntBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(LongBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(ShortBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(FloatBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(DoubleBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(CharBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Buffer,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDirect)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isReadOnly))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(AtomicBoolean,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(AtomicReference,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(AtomicInteger,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(AtomicLong,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(NullPointerException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(IndexOutOfBoundsException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(List,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(set)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(remove)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(removeByIndex))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Set,)

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Collection,
        QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(add)
        QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(addAll)
        QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(remove)
        QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(contains)
        QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(size)
        QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(toArray)
        QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(clear)
    )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Iterable,
        QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(iterator)
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(Iterator,
        QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasNext)
        QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(remove)
        QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(next)
    )
}

namespace QtCore{
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QString,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QChar,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QCoreApplication,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_FIELD(__qt_isInitializing))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_EMPTY_CLASS(QObject)

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QFutureWatcher,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(future)
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_WRITABLE_FIELD(futureSetter)
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_WRITABLE_FIELD(futureResult)
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_WRITABLE_FIELD(futureGetter)
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_WRITABLE_FIELD(futureInterfaceGetter)
                                     )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QThread,
        QTJAMBI_REPOSITORY_DECLARE_OBJECT_WRITABLE_FIELD(javaThread))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method10,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method9,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method8,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method7,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method6,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method5,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method4,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method3,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method2,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method1,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Method0,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot10,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot9,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot8,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot7,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot6,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot5,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot4,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot3,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot2,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot1,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaObject$Slot0,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMetaType$GenericValue, QTJAMBI_REPOSITORY_DECLARE_INT_WRITABLE_FIELD(type))

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QBindable,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createBinding)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QProperty,
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

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QBooleanProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QCharProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QByteProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QIntProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QShortProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QLongProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QDoubleProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QFloatProperty,
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
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QAbstractComputedProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedBooleanProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedCharProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedByteProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedIntProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedShortProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedLongProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedDoubleProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QObject$QComputedFloatProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(value)
                                     )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QSequentialIterator,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QAssociativeIterator,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QSequentialConstIterator,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QAssociativeConstIterator,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QStringList,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
    )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QList,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )
    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QQueue,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QStack,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMap,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QHash,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMultiMap,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QMultiHash,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
    )

    QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QSet,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
    )
}

namespace QtGui{
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(GLsync,)
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QValidator$QValidationData,
              QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
              QTJAMBI_REPOSITORY_DECLARE_STRING_WRITABLE_FIELD(string)
              QTJAMBI_REPOSITORY_DECLARE_INT_WRITABLE_FIELD(position))
}

namespace QtJambi {
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_EMPTY_CLASS(QtObject)

QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtObjectInterface,
              QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(dispose)
              QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDisposed))
}
}

#undef DECLARE_CLASS_REF
#undef QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS
#undef QTJAMBI_REPOSITORY_DECLARE_EXPORTED_EMPTY_CLASS

#endif // QTJAMBI_JAVAAPI_H

