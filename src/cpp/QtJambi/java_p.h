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

#ifndef JAVA_P_H
#define JAVA_P_H

#include "javaapi.h"
#include "jnienvironment.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT5(field)
#define QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(name,sig)
#define QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD_QT5(field)
#define QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(name,sig)
#define QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD_QT5(name)
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT6(field) QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(field)
#define QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD_QT6(field) QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(field)
#define QTJAMBI_REPOSITORY_DEFINE_FIELD_QT6(name,sig) QTJAMBI_REPOSITORY_DEFINE_FIELD(name,sig)
#define QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD_QT6(name) QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(name)
#else
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT5(field) QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(field)
#define QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(name,sig) QTJAMBI_REPOSITORY_DEFINE_FIELD(name,sig)
#define QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD_QT5(field) QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(field)
#define QTJAMBI_REPOSITORY_DEFINE_FIELD_QT5(name,sig) QTJAMBI_REPOSITORY_DEFINE_FIELD(name,sig)
#define QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD_QT5(name) QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(name)
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT6(field)
#define QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD_QT6(field)
#define QTJAMBI_REPOSITORY_DEFINE_FIELD_QT6(name,sig)
#define QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD_QT6(name)
#endif

namespace Java{
    namespace QtQml{
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlListProperty,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(elementType))
        namespace Internal{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlTypes$JarImport,
                                             QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        }
    }
    namespace QtCore{
        namespace Internal{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QCoreApplication,
                                            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(execPreRoutines)
                                            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(execPostRoutines)
                                         )
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QThread,
                                             QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setJavaThreadReference)
                                             QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getJavaThreadReference)
                                             QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        }
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QModelIndex,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(model)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(internalId)
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(row)
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(column))
#else
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QModelIndex,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#endif
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QAbstractItemModel,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QPluginLoader,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(loadPluginInstance)
        )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QException,
                                         QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QUnhandledException,
                                         QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFunctionPointer,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFunctionPointerUtil,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createProxy)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createCallbackProxy)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(registerCleanup))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFunctionPointerUtil$CppToJavaInvocationHandler,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(proxy)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(peer))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QGenericArgumentType,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(metaType)
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(classType)
                                         QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(pointerOrReference))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QGenericArgument,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(metaType))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(AbstractSpan,
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(commit))
#if QT_VERSION > QT_VERSION_CHECK(6,7,0)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QConstSpan,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QSpan,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
                                         )
#endif


        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QByteArray)

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QByteArrayView)
#endif

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtMessageHandler,
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QLogging,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(acceptInstalled)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(shutdown))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaEnum, QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(flagsType))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericTypeInterface, QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(metaType))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericObject, QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(type))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericGadget, QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(type))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericByteEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericShortEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericLongEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericFlags,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericLongFlags,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#endif

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QMetaObject$Connection)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaMethod,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(toSignal))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaObject,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(metaObjectPointer))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QPair,
            QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(first)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(second))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFutureWatcher,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(future)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFuture,        QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(d))

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QPromise,        QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(d)
                                                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(nativeInstance))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QPromise$NativeInstance,
                                                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(promise))
#endif

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFutureInterface,
                                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QVariant,
                                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QVariant$Null,
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(metaTypeID)
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(AbstractContainer,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(__rcContainer)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SmartPointer,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                                         )
    }

    namespace QtGui{
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QPainter,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(beginPaint)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(endPaint))
    }

    namespace Runtime{
        namespace Internal{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(ByteBuffer,
                                            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(order)
                                            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(allocate)
                                            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(allocateDirect))
            QTJAMBI_REPOSITORY_DECLARE_CLASS(Buffer,
                                            QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(capacity)
                                            QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(limit)
                                            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setLimit)
                                            QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(position)
                                             QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(clear)
                                            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(mark)
                                            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(array)
                                            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasArray)
                                            QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(arrayOffset))
        }
        QTJAMBI_REPOSITORY_DECLARE_CLASS(ByteOrder,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(nativeOrder))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(ClassLoader,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(loadClass)
                                         public: static inline jclass tryLoadClass(JNIEnv* env, jobject object, jstring className){
                                             auto _this = __qt_get_this(env);
                                             jobject result = env->CallObjectMethod(object,_this.__loadClass, className);
                                             return jclass(result);
                                         }
                                        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Cloneable,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Serializable,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(System,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(gc)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(getProperty)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(setProperty)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(identityHashCode))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(URL,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(openConnection))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(URLConnection,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getInputStream)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getOutputStream)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getContentLengthLong)
                                         QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(getDoInput)
                                         QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(getDoOutput))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(JarURLConnection,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getJarEntry))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(Channels,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(newInChannel))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(ReadableByteChannel,
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(read))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(AutoCloseable,
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(close))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ZipEntry,
                                         QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getSize)
                                         QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDirectory))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(InputStream,
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(read)
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(read1)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(skip))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MethodHandles$Lookup,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(HashMap,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(HashSet, QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(TreeMap, QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(TreeSet,
                       QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Map$Entry,
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getKey)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getValue)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Reference,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(NavigableMap,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Map,
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(putAll)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(remove)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(removePair)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
            QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(size)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(clear)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(entrySet)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(keySet))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Comparable, QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(compareTo))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(AccessibleObject,
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isAnnotationPresent)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getAnnotation))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ArrayList,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(LinkedList,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Deque,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Queue,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ArrayDeque,
            QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(StackTraceElement,
            QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getClassName)
            QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getMethodName)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Thread,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(currentThread)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getUncaughtExceptionHandler)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getContextClassLoader)
            QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getStackTrace)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setContextClassLoader)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setUncaughtExceptionHandler)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setName)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getThreadGroup)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(interrupt)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isAlive)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDaemon)
            QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
            QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getId)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(dumpStack)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ThreadGroup,QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(add))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Throwable,
                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_VOID_METHOD(addSuppressed)
                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_VOID_METHOD(printStackTrace)
                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_STRING_METHOD(getMessage)
                     public: static inline jstring tryGetMessage(JNIEnv* env, jthrowable object){
                         auto _this = __qt_get_this(env);
                         jobject result = env->CallObjectMethod(object,_this.__getMessage);
                         return jstring(result);
                     }
                     static inline void tryPrintStackTrace(JNIEnv* env, jthrowable object){
                          auto _this = __qt_get_this(env);
                          env->CallVoidMethod(object,_this.__printStackTrace);
                     }
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Arrays,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(copyOf)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_INTARRAY_METHOD(copyOfIntArray))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Objects,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(equals))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Enum,
                      QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(ordinal)
                      QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(name))

        namespace Time{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(Duration,
                                             QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getSeconds)
                                             QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getNano)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(from)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(ofMillis)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(ofSeconds))
            QTJAMBI_REPOSITORY_DECLARE_CLASS(Instant,
                                             QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getEpochSecond)
                                             QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getNano)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(from)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(ofEpochMilli)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(ofEpochSecond))
        }
    }
    namespace QtJambi
    {
        QTJAMBI_REPOSITORY_DECLARE_CLASS(NativeUtility,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(terminateCleanupThread)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(createAssociation)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(deleteAssociation)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(deleteAssociationByHashCode)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findAssociation)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(nativeId)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(nativeIdInterface)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(pushGlobalReference)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(releaseGlobalReference)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getGlobalReference)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(pushWeakGlobalReference)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(releaseWeakGlobalReference)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getWeakGlobalReference)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findInterfaceLink)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(findAndAssignInterfaceLink)
                                 )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(NativeUtility$NativeLink,
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initialize)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(detach)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(reset)
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(nativeId)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(takeOwnership)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(releaseOwnership)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getForID)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(resetForID)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(detachForID)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(takeOwnershipForID)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(releaseOwnershipForID))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(NativeUtility$Object,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(nativeLink)
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(assignNativeLink))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaTypeUtility,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(debugObject)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(writeSerializableJavaObject)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(readSerializableJavaObject)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(analyzeExpectedTemplateName)
                                 )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(ClassAnalyzerUtility,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getImplementedInterfaceInfo)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(checkImplementation)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(findGeneratedSuperclass)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isImplementedInJava)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isGeneratedClass)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(objectToString)
                                        )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(ThreadUtility,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(setThreadInterruptible)
                                        )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(initializePackage)
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(AbstractMultiAssociativeContainer,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(putMultiMap)
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(ExceptionUtility,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(reportException)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTEARRAY_METHOD(printException)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(extendStackTrace)
                                         )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(AccessUtility,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(instance)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ResourceUtility,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveUrlFromPath)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveFileToJarResource)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(cleanupOnShutdown)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ResourceUtility$JarResource,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getJarEntry)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getInputStream)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getChannel)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(fileTime)
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(ensureRef)
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(_deref)
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(entryList)
                                         QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDirectory)
                                         QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(checkIsDirectory)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(LibraryUtility,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(clear)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isNativeDeployment)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(pluginPaths)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(extractContainerAccessLib)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(EnumUtility,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(getEnumForQFlags)
        )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(ReferenceUtility$RCList,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ReferenceUtility$RCSet,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ReferenceUtility$RCMap,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ReferenceUtility$RCMultiMap,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility$AbstractSignal,
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initializeSignal))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility$SignalInfo,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility$SignalParameterType,
                      QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(type))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectUtility,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(analyze))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectData$SignalInfo,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(field)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(signalTypes)
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(signalClass)
                                         QTJAMBI_REPOSITORY_DECLARE_INTARRAY_FIELD(signalMetaTypes)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(methodId)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectData$IntArray,
                                         QTJAMBI_REPOSITORY_DECLARE_INTARRAY_FIELD(array)
                                         QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(size)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectData$MetaTypeInfo,
                                         QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(metaTypeId)
                                         QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(typeName)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectData,
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(intData)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(stringData)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(signalInfos)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(methods)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(methodMetaTypes)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(constructors)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(constructorMetaTypes)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(propertyReaders)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(propertyWriters)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(propertyResetters)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(propertyNotifies)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(propertyMemberFields)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT5(propertyDesignableResolvers)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT5(propertyScriptableResolvers)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT5(propertyEditableResolvers)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT5(propertyStoredResolvers)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT5(propertyUserResolvers)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT6(propertyQPropertyFields)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT6(propertyBindables)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(propertyMetaTypes)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(propertyClassTypes)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD_QT6(metaTypes)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(relatedMetaObjects)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(switchTableFields)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(hasStaticMembers))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QThreadAffinityException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInterfaceCannotBeSubclassedException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtEnumerator,
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtFlagEnumerator,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtByteEnumerator,
                                         QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtShortEnumerator,
                                         QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtLongEnumerator,
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFlags,
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(intValue)
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setIntValue))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongFlags,
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(longValue)
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setLongValue))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFlags$ConcreteWrapper,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongFlags$ConcreteWrapper,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QNoNativeResourcesException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QDanglingPointerException,
                                         QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QNoImplementationException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QNonVirtualOverridingException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMisfittingSignatureException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtPrivateOverride)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveSignal)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility$NativeConnection,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility$AbstractConnection,
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke)
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(onDisconnect))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility$AbstractMultiSignal,
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initializeSignals)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(signal))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtRejectedEntries,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(value))

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtExtensibleEnum)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMissingVirtualOverridingException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QUnsuccessfulInvocationException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QNativePointer,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(fromNative)
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
                      QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(indirections)
                      QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(knownSize)
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setVerificationEnabled)
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invalidate)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isReadOnly)
                      QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(pointer))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGadget,
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(staticMetaObject)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal0,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal1,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal1Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal2,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal2Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal2Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal3,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal3Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal3Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal3Default3,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal4,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal4Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal4Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal4Default3,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal4Default4,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal5,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal5Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal5Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal5Default3,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal5Default4,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal5Default5,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6Default3,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6Default4,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6Default5,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6Default6,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7Default3,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7Default4,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7Default5,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7Default6,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7Default7,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default3,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default4,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default5,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default6,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default7,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8Default8,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default1,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default2,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default3,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default4,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default5,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default6,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default7,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default8,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9Default9,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    }

    namespace JNA{
        QTJAMBI_REPOSITORY_DECLARE_CLASS(Native,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(setProtected)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(invokeStructure)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(createNativeCallback)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(freeNativeCallback)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(ffi_prep_cif)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(ffi_call)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(ffi_prep_closure)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(ffi_free_closure)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(free)
                    )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(Function,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getFunction)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(invoke))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(Pointer,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(peer))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(Structure,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(CallbackProxy,
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(callbackMethod)
                      QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(callbackMethod)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getParameterTypes)
                      QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getReturnType)
                    )
    }

#ifdef Q_OS_ANDROID
    namespace Android{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ContextWrapper,
                  QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getPackageName)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getPackageManager)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getAssets)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getResources))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Context,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getApplicationInfo))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ApplicationInfo,
                  QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(sourceDir))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(AssetManager,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(open)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(list))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(PackageManager,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getApplicationInfo)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getActivityInfo)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getServiceInfo)
                  QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_FIELD(GET_META_DATA))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Activity,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getApplication)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getComponentName))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Service,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getApplication)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getComponentName))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(PackageItemInfo,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(metaData))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(BaseBundle,
                  QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getString))
    }
#endif //def Q_OS_ANDROID
}

enum class JniEnvironmentFlag{
    Default = 0,
    NoGetEnv = 0x001,
    NoInitializeJavaThread = 0x010,
    NoAttachCurrentThread = 0x100
};
typedef QFlags<JniEnvironmentFlag> JniEnvironmentFlags;

struct DefaultJniEnvironment : JniEnvironment{
    DefaultJniEnvironment(int capacity = 0);
};

bool noExceptionForwarding(JNIEnv *env);

#endif // JAVA_P_H
