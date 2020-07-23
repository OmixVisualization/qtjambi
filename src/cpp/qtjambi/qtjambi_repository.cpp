/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include <QtCore/QMutex>

typedef QList<void**> GlobalClassPointers;
Q_GLOBAL_STATIC(GlobalClassPointers, gGlobalClassPointers)
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

void clear_repository_at_shutdown(){
    GlobalClassPointers globalClassPointers;
    {
        QMutexLocker locker(gMutex());
        Q_UNUSED(locker)
        globalClassPointers = *gGlobalClassPointers;
        gGlobalClassPointers->clear();
    }
    for(void** ptr : globalClassPointers){
        *ptr = nullptr;
    }
}

namespace Java{

#ifdef POST_CLASS_REF_ACTION
#undef POST_CLASS_REF_ACTION
#endif
#define POST_CLASS_REF_ACTION(class_ref) gGlobalClassPointers->append(reinterpret_cast<void**>(&class_ref));

#define DEFINE_CLASS_REF(cls)\
    _this->__##cls = getGlobalClassRef(env, jclass(env->GetStaticObjectField(_this->class_ref, env->GetStaticFieldID(_this->class_ref, #cls, "Ljava/lang/Class;") )), nullptr);\
    POST_CLASS_REF_ACTION(_this->__##cls)\
    qtjambi_throw_java_exception(env)

#ifndef QT_QTJAMBI_PORT
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QThread,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(javaThread,Ljava/lang/Thread;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(interruptible,Ljava/lang/Object;)
)
#else
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtCore,io/qt/core,QThread)
#endif

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(Runtime,java/lang,String)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util/concurrent/atomic,AtomicBoolean,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(Z)V)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util/concurrent/atomic,AtomicInteger,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(I)V)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util/concurrent/atomic,AtomicLong,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(J)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util/concurrent/atomic,AtomicReference,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(Ljava/lang/Object;)V)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Number,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(intValue,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(doubleValue,()D)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(longValue,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(floatValue,()F)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(shortValue,()S)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(byteValue,()B)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Boolean,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(Z)Ljava/lang/Boolean;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(booleanValue,()Z)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Integer,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(I)Ljava/lang/Integer;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Void,
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Double,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(D)Ljava/lang/Double;)
    DEFINE_CLASS_REF(TYPE)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Long,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(J)Ljava/lang/Long;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Float,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(F)Ljava/lang/Float;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Short,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(S)Ljava/lang/Short;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Byte,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(B)Ljava/lang/Byte;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Character,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(charValue,()C)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(valueOf,(C)Ljava/lang/Character;)
    DEFINE_CLASS_REF(TYPE)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,ByteBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asCharBuffer,()Ljava/nio/CharBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asDoubleBuffer,()Ljava/nio/DoubleBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asFloatBuffer,()Ljava/nio/FloatBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asIntBuffer,()Ljava/nio/IntBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asLongBuffer,()Ljava/nio/LongBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asShortBuffer,()Ljava/nio/ShortBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/ByteBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)B)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IB)Ljava/nio/ByteBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,IntBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/IntBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(II)Ljava/nio/IntBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,LongBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/LongBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IJ)Ljava/nio/LongBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,ShortBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/ShortBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)S)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IS)Ljava/nio/ShortBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,FloatBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/FloatBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)F)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IF)Ljava/nio/FloatBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,DoubleBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/DoubleBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)D)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(ID)Ljava/nio/DoubleBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,CharBuffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(asReadOnlyBuffer,()Ljava/nio/CharBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)C)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(IC)Ljava/nio/CharBuffer;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/nio,Buffer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isDirect,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isReadOnly,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(capacity,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(limit,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(position,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(mark,()Ljava/nio/Buffer;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(array,()Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasArray,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(arrayOffset,()I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Runnable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(run,()V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtGui,io/qt/gui,QValidator$QValidationData,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(string,Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(position,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QObject$QDeclarativeConstructor,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;J)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(placement,J)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(cls,Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QCoreApplication,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(__qt_isInitializing,Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaObject$Slot1,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,(Ljava/lang/Object;)V)
)

namespace Private{

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QPair,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(first,Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(second,Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QModelIndex,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIJLio/qt/core/QAbstractItemModel;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(row,I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(column,I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(internalId,J)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(model,Lio/qt/core/QAbstractItemModel;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaProperty,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QMetaObject;ILjava/lang/String;Ljava/lang/Class;I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyIndex,I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(metaObject,Lio/qt/core/QMetaObject;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaMethod,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QMetaObject;Ljava/lang/String;Ljava/lang/String;Ljava/util/List;I[ILjava/lang/String;III)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Lio/qt/core/QMetaObject;Ljava/lang/String;Ljava/lang/Class;Ljava/util/List;I[ILjava/lang/String;III)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(methodIndex,I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(enclosingMetaObject,Lio/qt/core/QMetaObject;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(UNSUPPORTED,Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaObject,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(J)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(metaObjectPointer,J)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaEnum,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QMetaObject;I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(enumeratorIndex,I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(enclosingMetaObject,Lio/qt/core/QMetaObject;)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtCore,io/qt/core,QMetaObject$Connection)

QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(Runtime,java/util,HashSet)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(Runtime,java/util,ArrayList)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(Runtime,java/util,ArrayDeque)
QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(Runtime,java/util,LinkedList)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/invoke,MethodHandles$Lookup,)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,List,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(I)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(set,(ILjava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(removeByIndex,remove,(I)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,Set,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,Map,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(size,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(clear,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(entrySet,()Ljava/util/Set;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(Ljava/lang/Object;)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,Map$Entry,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getKey,()Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getValue,()Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Comparable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(compareTo,(Ljava/lang/Object;)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,HashMap,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,NavigableMap,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,Deque,
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,TreeMap,
    QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,TreeSet,
    QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,NullPointerException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,IllegalAccessError,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Error,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,RuntimeException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,IndexOutOfBoundsException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,Collection,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(add,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(addAll,(Ljava/util/Collection;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(size,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toArray,()[Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(clear,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(remove,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(contains,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(iterator,()Ljava/util/Iterator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,Iterator,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(remove,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasNext,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(next,()Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util,Arrays,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(copyOf,([Ljava/lang/Object;I)[Ljava/lang/Object;)
)

    QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/reflect,Constructor,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaringClass,()Ljava/lang/Class;)
        QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
    )

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/reflect,Method,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaringClass,()Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/reflect,Executable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getModifiers,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getParameterTypes,()[Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/reflect,Field,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaringClass,()Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getModifiers,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getType,()Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/reflect,Modifier,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isNative,(I)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isStatic,(I)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isAbstract,(I)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Object,
    QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()
    QTJAMBI_REPOSITORY_DEFINE_METHOD(equals,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toString,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hashCode,()I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiInternal$NativeLink,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(native__id,J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(detach,(J)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(reset,()V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Class,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaredMethods,()[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getDeclaredField,(Ljava/lang/String;)Ljava/lang/reflect/Field;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getEnumConstants,()[Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getClassLoader,()Ljava/lang/ClassLoader;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isAnnotationPresent,(Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAnnotation,(Ljava/lang/Class;)Ljava/lang/annotation/Annotation;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getInterfaces,()[Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isInterface,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getModifiers,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getComponentType,()Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isArray,()Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,System,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(gc,()V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getProperty,(Ljava/lang/String;)Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(identityHashCode,(Ljava/lang/Object;)I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util/function,Supplier,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/net,URL,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/net,URLClassLoader,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(newInstance,([Ljava/net/URL;Ljava/lang/ClassLoader;)Ljava/net/URLClassLoader;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(addURL,(Ljava/net/URL;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,ClassLoader,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(loadClass,(Ljava/lang/String;)Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QNoNativeResourcesException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QNonVirtualOverridingException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QNoImplementationException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,IllegalAccessException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QMisfittingSignatureException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,NumberFormatException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,IllegalArgumentException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,IllegalStateException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiInternal,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(lookupSignal,(Lio/qt/QtSignalEmitterInterface;Ljava/lang/String;[Ljava/lang/Class;)Lio/qt/internal/QtJambiSignals$AbstractSignal;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(lookupSlot,(Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findQmlAttachedProperties,(Ljava/lang/Class;)Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getImplementedInterfaces,(Ljava/lang/Class;)Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getAllImplementedInterfaces,(Ljava/lang/Class;)Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(checkInterfaceImplementation,(Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findEmitMethod,(Ljava/lang/Class;)Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isImplementedInJava,(ZLjava/lang/reflect/Method;Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findGeneratedSuperclass,(Ljava/lang/Class;)Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(writeSerializableJavaObject,(Lio/qt/core/QDataStream;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(readSerializableJavaObject,(Lio/qt/core/QDataStream;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(isGeneratedClass,(Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(signalMatchesSlot,(Ljava/lang/String;Ljava/lang/String;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(putMultiMap,(Ljava/util/Map;Ljava/lang/Object;Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createComparator,(J)Ljava/util/Comparator;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(registerCleaner,(Ljava/lang/Object;Ljava/lang/Runnable;)Lio/qt/internal/QtJambiInternal$Cleanable;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(disconnectAll,(Lio/qt/QtSignalEmitterInterface;Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findDefaultImplementation,(Ljava/lang/Class;)Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(resolveEnumI,resolveEnum,(Ljava/lang/Class;I)Ljava/lang/Enum;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(resolveEnumS,resolveEnum,(Ljava/lang/Class;S)Ljava/lang/Enum;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(resolveEnumB,resolveEnum,(Ljava/lang/Class;B)Ljava/lang/Enum;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(resolveEnumJ,resolveEnum,(Ljava/lang/Class;J)Ljava/lang/Enum;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getLibraryPaths,()Ljava/util/List;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(reportException,(Ljava/lang/String;Ljava/lang/Throwable;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(extendStackTrace,(Ljava/lang/Throwable;Ljava/lang/String;Ljava/lang/String;I)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findInterfaceLink,(Lio/qt/QtObjectInterface;Z)Lio/qt/internal/QtJambiInternal$NativeLink;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(createNativeLinkInterface,createNativeLink,(Lio/qt/QtObjectInterface;)Lio/qt/internal/QtJambiInternal$NativeLink;)
    QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(createNativeLinkObject,createNativeLink,(Lio/qt/internal/QtJambiObject;)Lio/qt/internal/QtJambiInternal$NativeLink;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(extendEnum,(Ljava/lang/Class;[Ljava/lang/Enum;Ljava/lang/Enum;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(setThreadInterruptible,(Ljava/lang/Thread;Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(createAssociation,(Ljava/lang/Object;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(deleteAssociation,(Ljava/lang/Object;)Z)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findAssociation,(Ljava/lang/Object;)Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(shutdown,()V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,MetaObjectTools,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(buildMetaData,(Ljava/lang/Class;)Lio/qt/internal/MetaObjectTools$MetaData;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(methodTypes,(Ljava/lang/reflect/AccessibleObject;)[[Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(signalTypes,(Ljava/lang/reflect/Field;)[Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(getEnumForQFlags,(Ljava/lang/Class;)Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,MetaObjectTools$MetaData,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(metaData,[I)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(stringData,[[B)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(signalsArray,[Ljava/lang/reflect/Field;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(slotsArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(constructorsArray,[Ljava/lang/reflect/Constructor;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyReadersArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyWritersArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyResettersArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyNotifiesArray,[Ljava/lang/reflect/Field;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyDesignableResolverArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyScriptableResolverArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyEditableResolverArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyStoredResolverArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(propertyUserResolverArray,[Ljava/lang/reflect/Method;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(extraDataArray,[Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(originalSignatures,[Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(hasStaticMembers,Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,StackTraceElement,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getClassName,()Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getMethodName,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Thread,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(currentThread,()Ljava/lang/Thread;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getUncaughtExceptionHandler,()Ljava/lang/Thread$UncaughtExceptionHandler;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getContextClassLoader,()Ljava/lang/ClassLoader;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(setUncaughtExceptionHandler,(Ljava/lang/Thread$UncaughtExceptionHandler;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(setContextClassLoader,(Ljava/lang/ClassLoader;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getThreadGroup,()Ljava/lang/ThreadGroup;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getStackTrace,()[Ljava/lang/StackTraceElement;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(interrupt,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isAlive,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isDaemon,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;)
)

#ifdef QT_QTJAMBI_PORT
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QThreadAffinityException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Lio/qt/core/QObject;Ljava/lang/Thread;Ljava/lang/Thread;)
)
#else
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QThreadAffinityException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Lio/qt/core/QObject;Lio/qt/core/QThread;Lio/qt/core/QThread;)
)
#endif

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QInterfaceCannotBeSubclassedException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;)
)

#ifdef QT_QTJAMBI_PORT
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QThread$Wrapper)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Thread;Lio/qt/QtObject$QPrivateConstructor;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(thread,Ljava/lang/Thread;)
)
#endif

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,ThreadGroup,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(add,(Ljava/lang/Thread;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QFlags,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(I)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QFlags$ConcreteWrapper,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtByteEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()B)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtShortEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()S)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtLongEnumerator,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()J)
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(comparator,()Ljava/util/Comparator;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtGadget,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(staticMetaObject,Lio/qt/core/QMetaObject;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/internal/fileengine,QClassPathEngine,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Throwable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(addSuppressed,(Ljava/lang/Throwable;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getMessage,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,Enum,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(ordinal,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(name,()Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/reflect,AccessibleObject,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isAnnotationPresent,(Ljava/lang/Class;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getAnnotation,(Ljava/lang/Class;)Ljava/lang/annotation/Annotation;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtGui,io/qt/gui,QPainter,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(beginPaint,(Lio/qt/gui/QPaintDevice;)V)
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(endPaint,(Lio/qt/gui/QPaintDevice;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericTypeInterface,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(metaType,()I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericValue,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(type,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericGadget,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(type,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIILjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericByteEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIBLjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericShortEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IISLjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericLongEnumerator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIJLjava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMetaType$GenericFlags,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(II)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QList,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;JJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QQueue,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;JJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QVector,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;JJJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QStack,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;JJJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMap,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QHash,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMultiMap,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMultiHash,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;Ljava/lang/Class;JJJJJJJJJJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QSet,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;JJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;JJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QLinkedList,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;JJJJJJJJJJJJJJJJJJJ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/lang/Class;JJJJJJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QIterator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;JJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMapIterator,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;JJJJJJ)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMessageHandler,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(shutdown,()V)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtCore,io/qt/core,QByteArray)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtCore,io/qt/core,QObject)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiSignals$AbstractSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(setInCppEmission,(Z)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(inJavaEmission,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(addConnectionFromCpp,(Ljava/lang/Object;Ljava/lang/reflect/Method;I)Lio/qt/core/QMetaObject$Connection;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(removeConnectionFromCpp,(Ljava/lang/Object;Ljava/lang/reflect/Method;)Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(initializeSignal,(Ljava/lang/reflect/Field;Ljava/lang/reflect/Method;I)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(initializeExtraSignal,(Ljava/lang/Class;Ljava/util/List;I)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QMissingVirtualOverridingException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtRejectedEntries,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()[Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt,QtExtensibleEnum)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiAbstractMapObject,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(keyType,Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(valueType,Ljava/lang/Class;)
)


QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiAbstractMultiMapObject,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(keyType,Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(valueType,Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiCollectionObject,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(elementType,Ljava/lang/Class;)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiListObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiLinkedListObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiVectorObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiStackObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiQueueObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiSetObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiMapObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiMultiMapObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiMultiHashObject)
QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt/internal,QtJambiHashObject)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt,QtPrivateOverride)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiSignals$AbstractConnection,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,([Ljava/lang/Object;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiSignals$NativeConnection,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtObject$QPrivateConstructor;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiSignals,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(NativeConnectionPolicy,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiSignals$MultiSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(initializeSignals,(Ljava/lang/reflect/Field;Ljava/util/List;[I)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(signal,(I)Lio/qt/core/QMetaObject$AbstractSignal;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/internal,QtJambiObject,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(nativeLink,Lio/qt/internal/QtJambiInternal$NativeLink;)
)

QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(QtJambi,io/qt,QtObject)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtObjectInterface,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(dispose,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isDisposed,()Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QtArgument$Stream$Arg,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(type,Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(value,Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt,QNativePointer,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(fromNative,(JIJIZ)Lio/qt/QNativePointer;)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IJIZ)
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(Ljava/nio/Buffer;J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(indirections,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(byteSize,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(isReadOnly,()Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(pointer,()J)
)

#ifdef QT_QTJAMBI_PORT
    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal0,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,()V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal1,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal2,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal3,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal4,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal5,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal6,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal7,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal8,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal9,
        QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(emitMethod,emit,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )
#else
    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal0,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal1,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal2,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal3,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal4,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal5,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal6,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal7,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal8,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(QtJambi,io/qt/core,QInstanceMemberSignals$PrivateSignal9,
        QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;)
    )
#endif
}
}
