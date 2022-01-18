#ifndef QTJAMBI_REPOSITORY_P_H
#define QTJAMBI_REPOSITORY_P_H

#include "qtjambi_repository.h"

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
    namespace QtCore{
        namespace Internal{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QCoreApplication,
                                            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(execPreRoutines)
                                            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(execPostRoutines)
                                         )
        }
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

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QObject)

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QByteArray)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtMessageHandler,
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaEnum, QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(flagsType))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericTypeInterface, QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(metaType))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericObject, QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(type))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericGadget, QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(type))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericByteEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericShortEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericLongEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaType$GenericFlags,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QIterator,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMapIterator,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QLinkedList,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QVector,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )
#endif

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QStringList,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QList,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
        )
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QQueue,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QStack,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMap,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QHash,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMultiMap,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMultiHash,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QSet,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QClassPathEngine,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QMetaObject$Connection)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QModelIndex,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                      QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(row)
                      QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(column)
                      QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(internalId)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(model))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaMethod,)

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
                                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(userType)
                                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(value)
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
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(allocateDirect))
        }
        QTJAMBI_REPOSITORY_DECLARE_CLASS(Class,
                      QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(cast)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getDeclaredMethods)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getDeclaredField)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getEnumConstants)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getClassLoader)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isAnnotationPresent)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getAnnotation)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getInterfaces)
                      QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                      QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getComponentType)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getTypeParameters)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isArray)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPrimitive)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isInterface))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ClassLoader,QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(loadClass))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Optional,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(ofNullable))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(OptionalInt,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                      QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getAsInt)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(OptionalLong,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                      QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(getAsLong)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(OptionalDouble,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(empty)
                      QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(getAsDouble)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isPresent)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(System,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(gc)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(getProperty)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(identityHashCode))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(URLClassLoader,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(newInstance)
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(addURL))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(URL,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MethodHandles$Lookup,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Method,
            QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
            QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
            QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getReturnType))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Constructor,
            QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
            QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Executable,
                      QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getParameterTypes))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Field,
                      QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
                      QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                      QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
                      QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getType))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Modifier,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isNative)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isStatic)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isFinal)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isAbstract))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(HashMap,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(HashSet, QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(TreeMap, QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                               QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(TreeSet,
                       QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                       QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Map$Entry,
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getKey)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getValue)
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
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(entrySet))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Comparable, QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(compareTo))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(AccessibleObject,
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isAnnotationPresent)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getAnnotation))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ArrayList,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(LinkedList,
                                         QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(List,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(set)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(remove)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(removeByIndex))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Deque,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Queue,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Set,)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Collection,
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(add)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(addAll)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(remove)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(contains)
            QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(size)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(iterator)
            QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(toArray)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(clear)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Iterator,
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasNext)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(remove)
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(next)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ArrayDeque,
            QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(NullPointerException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(IndexOutOfBoundsException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

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
            QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getThreadGroup)
            QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(interrupt)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isAlive)
            QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDaemon)
            QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ThreadGroup,QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(add))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Throwable,
                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(addSuppressed)
                     QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getMessage)
                     public: static inline jstring tryGetMessage(JNIEnv* env,jobject object){
                         auto _this = __qt_get_this(env);
                         jobject result = env->CallObjectMethod(object,_this.__getMessage);
                         return jstring(result);
                     }
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(IllegalAccessException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(NumberFormatException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(IllegalArgumentException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(IllegalStateException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(IllegalAccessError,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(UnsupportedOperationException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(ClassCastException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(RuntimeException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Error,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Arrays,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(copyOf))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Objects,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(equals))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(Enum,
                      QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(ordinal)
                      QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(name))
    }

    namespace QtJambi
    {
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiInternal,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(findGeneratedSuperclass)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isImplementedInJava)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findQmlAttachedProperties)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getImplementedInterfaces)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getAllImplementedInterfaces)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(checkImplementation)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(writeSerializableJavaObject)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(readSerializableJavaObject)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(debugObject)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isGeneratedClass)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(putMultiMap)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createComparator)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getLibraryPaths)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(reportException)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(extendStackTrace)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findInterfaceLink)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(nativeId)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(checkedNativeId)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(nativeIdInterface)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(checkedNativeIdInterface)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createNativeLinkInterface)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createNativeLinkObject)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(setThreadInterruptible)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(createAssociation)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(deleteAssociation)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findAssociation)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(lambdaReturnType)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(objectToString)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(terminateCleanupThread)
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(internalAccess)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(NativeLibraryManager,QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(resetDeploymentSpecs))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiEnums,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(extendEnum)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiPlugins,
            QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(loadPluginInstance)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiInternal$RCList,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiInternal$RCSet,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiInternal$RCMap,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals$AbstractSignal,
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initializeSignal))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals$SignalInfo,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals$SignalParameterType,
                      QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(type))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectTools,
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(buildMetaData)
                      QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(getEnumForQFlags))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectTools$SignalInfo,
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(field)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(signalTypes)
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(signalClass)
                                         QTJAMBI_REPOSITORY_DECLARE_INTARRAY_FIELD(signalMetaTypes)
                                         QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(methodId)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(MetaObjectTools$MetaData,
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(metaData)
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
                                         QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QFlags$ConcreteWrapper,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QNoNativeResourcesException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QNoImplementationException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QNonVirtualOverridingException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QMisfittingSignatureException,
                      QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtPrivateOverride)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals,
                                         QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveSignal)
                                         )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals$NativeConnection,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals$AbstractConnection,
                                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals$AbstractMultiSignal,
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initializeSignals)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(signal))

        QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtObject)

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtObjectInterface,
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(dispose)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDisposed))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiObject,
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(nativeLink))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtArgument$Stream$Arg,
                      QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(type)
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(value))

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
                      QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(byteSize)
                      QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isReadOnly)
                      QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(pointer))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiInternal$NativeLink,
                      QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(native__id)
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(detach)
                      QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(reset))

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGadget,
                      QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(staticMetaObject)
        )

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal0,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal1,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal2,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal3,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal4,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal5,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal6,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal7,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal8,
                      QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

        QTJAMBI_REPOSITORY_DECLARE_CLASS(QInstanceMemberSignals$Signal9,
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
}
#endif // QTJAMBI_REPOSITORY_P_H
