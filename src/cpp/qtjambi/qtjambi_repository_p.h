#ifndef QTJAMBI_REPOSITORY_P_H
#define QTJAMBI_REPOSITORY_P_H

#include "qtjambi_repository.h"

namespace Java{
    namespace Private{
        class QtCore{
            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtCore,QObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtCore,QByteArray)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericTypeInterface, QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(metaType))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericValue, QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(type))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericGadget, QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(type))
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericByteEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericShortEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericLongEnumerator,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaType$GenericFlags,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMessageHandler,
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(shutdown))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QIterator,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMapIterator,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QList,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QQueue,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QVector,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QStack,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMap,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QHash,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMultiMap,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMultiHash,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QLinkedList,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QSet,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QClassPathEngine,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaEnum,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(enumeratorIndex)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(enclosingMetaObject))

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtCore,QMetaObject$Connection)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QModelIndex,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(row)
                          QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(column)
                          QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(internalId)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(model))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaProperty,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(propertyIndex)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(metaObject))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaMethod,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
                          QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(methodIndex)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(enclosingMetaObject)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(UNSUPPORTED))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMetaObject,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(metaObjectPointer))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QPair,
                QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(first)
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(second))

        #ifdef QT_QTJAMBI_PORT
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QThread$Wrapper,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(thread))
        #endif

        };

        class QtGui{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGui,QPainter,
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(beginPaint)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(endPaint))
        };

        class Runtime{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Object,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(equals)
                          QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(hashCode)
                          QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(toString))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Class,
                          QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getDeclaredMethods)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getDeclaredField)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getEnumConstants)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getClassLoader)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isAnnotationPresent)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getAnnotation)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(getInterfaces)
                          QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                          QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getComponentType)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isArray)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isInterface))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,ClassLoader,QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(loadClass))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,System,
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(gc)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(getProperty)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(identityHashCode))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,URLClassLoader,
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(newInstance)
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(addURL))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,URL,QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,MethodHandles$Lookup,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Method,
                QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
                QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Constructor,
                QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
                QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Executable,
                          QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getParameterTypes))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Field,
                          QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getDeclaringClass)
                          QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(getModifiers)
                          QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName)
                          QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(getType))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Modifier,
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isNative)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isStatic)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isAbstract))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,HashMap,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,HashSet, QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,TreeMap, QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                   QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,TreeSet,
                           QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                           QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Map$Entry,
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getKey)
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getValue)
            )

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,NavigableMap,)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Map,
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put)
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(size)
                QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(clear)
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(entrySet))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Comparable, QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(compareTo))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,AccessibleObject,
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isAnnotationPresent)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getAnnotation))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,ArrayList,
                                             QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,LinkedList,
                                             QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,List,
                                             QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Deque,)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Set,
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(of))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Collection,
                QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(add)
                QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(remove)
                QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(contains)
                QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(size)
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(iterator)
                QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(toArray)
                QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(clear)
            )

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Iterator,
                QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasNext)
                QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(next)
            )

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,ArrayDeque,
                QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
            )

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,NullPointerException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,IndexOutOfBoundsException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,StackTraceElement,
                QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getClassName)
                QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getMethodName)
            )

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Thread,
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

        #undef BLOCKER_FIELD

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,ThreadGroup,QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(add))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Throwable,
                         QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(addSuppressed)
                         QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getMessage)
                         public: inline jstring tryGetMessage(JNIEnv* env,jobject object) const {
                             __qt_resolve(env, this);
                             jobject result;
                             result = env->CallObjectMethod(object,__getMessage);
                             return jstring(result);
                         }
            )

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,IllegalAccessException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,NumberFormatException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,IllegalArgumentException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,IllegalStateException, QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,IllegalAccessError,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,RuntimeException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Error,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Arrays,
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(copyOf))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Enum,
                          QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(ordinal)
                          QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(name))
        };

        class QtJambi
        {
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiInternal,
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findEmitMethod)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(findGeneratedSuperclass)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isImplementedInJava)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(lookupSignal)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(lookupSlot)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findQmlAttachedProperties)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getImplementedInterfaces)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getAllImplementedInterfaces)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(checkInterfaceImplementation)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(writeSerializableJavaObject)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(readSerializableJavaObject)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(isGeneratedClass)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(signalMatchesSlot)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(putMultiMap)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createComparator)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(registerCleaner)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(disconnectAll)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(findDefaultImplementation)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveEnumI)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveEnumS)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveEnumB)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(resolveEnumJ)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getLibraryPaths)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(reportException)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(extendStackTrace)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findInterfaceLink)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createNativeLinkInterface)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(createNativeLinkObject)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(extendEnum)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(setThreadInterruptible)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(createAssociation)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(deleteAssociation)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findAssociation)
                QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(shutdown)
            )

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiSignals$AbstractSignal,
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setInCppEmission)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(addConnectionFromCpp)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(removeConnectionFromCpp)
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initializeSignal)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(inJavaEmission)
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initializeExtraSignal))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,MetaObjectTools,
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(buildMetaData)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(methodTypes)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(signalTypes)
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getEnumForQFlags))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,MetaObjectTools$MetaData,
                          QTJAMBI_REPOSITORY_DECLARE_INTARRAY_FIELD(metaData)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(stringData)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(signalsArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(slotsArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(constructorsArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyReadersArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyWritersArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyResettersArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyNotifiesArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyDesignableResolverArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyScriptableResolverArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyEditableResolverArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyStoredResolverArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(propertyUserResolverArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(extraDataArray)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(originalSignatures)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(hasStaticMembers))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QThreadAffinityException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInterfaceCannotBeSubclassedException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtEnumerator,
                                             QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtByteEnumerator,
                                             QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(value)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtShortEnumerator,
                                             QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(value)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtLongEnumerator,
                                             QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(value)
                                             QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(comparator))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QFlags,
                                             QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                             QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QFlags$ConcreteWrapper,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QNoNativeResourcesException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QNoImplementationException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QNonVirtualOverridingException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QMisfittingSignatureException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtPrivateOverride)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiSignals,
                                             QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(NativeConnectionPolicy))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiSignals$NativeConnection,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiSignals$AbstractConnection,
                                             QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiSignals$MultiSignal,
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initializeSignals)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(signal))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiObject,
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(nativeLink))

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtObject)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtObjectInterface,
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(dispose)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isDisposed))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtArgument$Stream$Arg,
                          QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(type)
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(value))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiCollectionObject,
                QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(elementType))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiAbstractMapObject,
                QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(keyType)
                QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(valueType))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiAbstractMultiMapObject,
                QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(keyType)
                QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(valueType))

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiHashObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiListObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiLinkedListObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiVectorObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiStackObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiQueueObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiSetObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiMapObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiMultiMapObject)

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtJambiMultiHashObject)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtRejectedEntries,
                                             QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(value))

            QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(QtJambi,QtExtensibleEnum)

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QMissingVirtualOverridingException,
                          QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QNativePointer,
                          QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(fromNative)
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()
                          QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(indirections)
                          QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(byteSize)
                          QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(isReadOnly)
                          QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(pointer))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtJambiInternal$NativeLink,
                          QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(native__id)
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(detach)
                          QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(reset))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QtGadget,
                          QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(staticMetaObject)
            )

        #ifdef QT_QTJAMBI_PORT
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal0,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal1,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal2,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal3,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal4,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal5,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal6,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal7,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal8,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal9,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                          QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(emitMethod))
        #else
            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal0,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal1,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal2,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal3,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal4,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal5,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal6,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal7,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal8,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

            QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambi,QInstanceMemberSignals$PrivateSignal9,
                          QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
        #endif
        };
    }
}
#endif // QTJAMBI_REPOSITORY_P_H
