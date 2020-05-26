/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambi_core.h"

#include "qtjambi_repodefines.h"

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_REPOSITORY_DECLARE_THROW_NEW\
    public: inline void throwNew(JNIEnv* env, const char* message, const char *methodName, const char *fileName, int lineNumber) const {\
        JavaException(env, newInstanceWithMessage(env, message)).raise(env, methodName, fileName, lineNumber);\
    }\
    public: inline void throwNew(JNIEnv* env, const QString& message, const char *methodName, const char *fileName, int lineNumber) const {\
        JavaException(env, newInstanceWithMessage(env, message)).raise(env, methodName, fileName, lineNumber);\
    }
#else
#define QTJAMBI_REPOSITORY_DECLARE_THROW_NEW\
    public: inline void throwNew(JNIEnv* env, const char* message) const {\
        JavaException(env, newInstanceWithMessage(env, message)).raise();\
    }\
    public: inline void throwNew(JNIEnv* env, const QString& message) const {\
        JavaException(env, newInstanceWithMessage(env, message)).raise();\
    }
#endif

#define QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()\
    private: jmethodID __constructor;\
    public: inline jthrowable newInstance(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(class_ref,__constructor,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jthrowable(result);\
    }\
    public: inline jthrowable newInstanceWithMessage(JNIEnv* env,const char* message) const {\
        return newInstance(env, env->NewStringUTF(message));\
    }\
    public: inline jthrowable newInstanceWithMessage(JNIEnv* env,const QString& message) const {\
        return newInstance(env, env->NewStringUTF(qPrintable(message)));\
    }\
    QTJAMBI_REPOSITORY_DECLARE_THROW_NEW

QT_WARNING_DISABLE_CLANG("-Wdollar-in-identifier-extension")

namespace Java{
class QTJAMBI_EXPORT Runtime
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Runnable,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Number,
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(intValue)
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(doubleValue)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(longValue)
                  QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(floatValue)
                  QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(shortValue)
                  QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(byteValue))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Integer,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Double,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Boolean,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(booleanValue)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Long,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Float,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Short,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Character,
                  QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(charValue)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Byte,
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(valueOf)
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Void,
                  DECLARE_CLASS_REF(TYPE))

    QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(Runtime,String)

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,ByteBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asCharBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asDoubleBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asFloatBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asIntBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asLongBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asShortBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,IntBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,LongBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,ShortBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,FloatBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,DoubleBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,CharBuffer,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(asReadOnlyBuffer)
                  QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Buffer,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDirect)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isReadOnly)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(capacity)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(limit)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(position)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(mark)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(array)
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasArray)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(arrayOffset))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,AtomicBoolean,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,AtomicReference,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,AtomicInteger,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,AtomicLong,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(get)
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(set))
private:
    Runtime() = delete;
    ~Runtime() = delete;
    Q_DISABLE_COPY_MOVE(Runtime)
};

class QTJAMBI_EXPORT QtCore{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QCoreApplication,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_FIELD(__qt_isInitializing))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QObject$QDeclarativeConstructor,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(placement)
                  QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(cls))

#ifndef QT_QTJAMBI_PORT
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QThread,
        QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(javaThread)
        QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(interruptible))
#else
    QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtCore,QThread)
#endif

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaObject$Slot1,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))

private:
    QtCore() = delete;
    ~QtCore() = delete;
    Q_DISABLE_COPY_MOVE(QtCore)
};

class QTJAMBI_EXPORT QtGui{
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGui,QValidator$QValidationData,
              QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
              QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(string)
              QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(position))
private:
    QtGui() = delete;
    ~QtGui() = delete;
    Q_DISABLE_COPY_MOVE(QtGui)
};
}

#undef DECLARE_CLASS_REF
#undef REPOSITORY_EXPORT
#define REPOSITORY_EXPORT
#endif // QTJAMBI_REPOSITORY_H

