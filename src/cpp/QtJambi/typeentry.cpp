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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/private/qmetatype_p.h>
#include "pch_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

void clear_type_entry(const std::type_info& typeId)
{
    QMap<QByteArray,QtJambiTypeEntryPtr> typeEntries;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker lock(storage->lock());
        if(storage->typeEntries().contains(unique_id(typeId))){
            typeEntries = storage->typeEntries().take(unique_id(typeId));
        }
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
typedef QCborValueRef QCborValueConstRef;
#endif

namespace QtJambiPrivate{
template<typename T, class = decltype(std::declval<T>().metaType() )>
constexpr std::true_type  has_metatype_test(const T&);
constexpr std::false_type has_metatype_test(...);
template<typename T> struct has_metatype : decltype(has_metatype_test(std::declval<T>())){};
template<typename T, class = decltype(std::declval<T>().interfaceOffsetInfo() )>
constexpr std::true_type  has_interface_offset_test(const T&);
constexpr std::false_type has_interface_offset_test(...);
template<typename T> struct has_interface_offset : decltype(has_interface_offset_test(std::declval<T>())){};
template<typename T, class = decltype(std::declval<T>().ownerFunction() )>
constexpr std::true_type  has_owner_function_test(const T&);
constexpr std::false_type has_owner_function_test(...);
template<typename T> struct has_owner_function : decltype(has_owner_function_test(std::declval<T>())){};
template<typename T, class = decltype(std::declval<T>().createContainerAccess() )>
constexpr std::true_type  has_container_access_test(const T&);
constexpr std::false_type has_container_access_test(...);
template<typename T> struct has_container_access : decltype(has_container_access_test(std::declval<T>())){};
template<typename T, class = decltype(std::declval<T>().customMetaObject() )>
constexpr std::true_type  has_superTypeForCustomMetaObject_test(const T&);
constexpr std::false_type has_superTypeForCustomMetaObject_test(...);
template<typename T> struct has_superTypeForCustomMetaObject : decltype(has_superTypeForCustomMetaObject_test(std::declval<T>())){};

template<typename... RuntimeTypes>
struct IsInstanceOfAny{

};

template<typename RuntimeType1, typename... RuntimeTypes>
struct IsInstanceOfAny<RuntimeType1, RuntimeTypes...>{
    static bool fail(JNIEnv *env, jobject obj){
        return obj && failNonNull(env, obj);
    }
    static bool failNonNull(JNIEnv *env, jobject obj){
        return !RuntimeType1::isInstanceOf(env, obj) && !IsInstanceOfAny<RuntimeTypes...>::fail(env, obj);
    }
};

template<typename RuntimeTypeN>
struct IsInstanceOfAny<RuntimeTypeN>{
    static bool fail(JNIEnv *env, jobject obj){
        return obj && failNonNull(env, obj);
    }
    static bool failNonNull(JNIEnv *env, jobject obj){
        return !RuntimeTypeN::isInstanceOf(env, obj);
    }
};

template<>
struct IsInstanceOfAny<>{
    static bool fail(JNIEnv*, jobject){
        return false;
    }
    static bool failNonNull(JNIEnv*, jobject){
        return false;
    }
};

template<typename PrimitiveType,
         typename BoxedType>
struct PrimitiveHelper{
    static PrimitiveType unbox(const BoxedType* bt){
        return *bt;
    }

    static void assign(BoxedType* bt, PrimitiveType pt){
        *bt = pt;
    }
};

template<typename PrimitiveType>
struct PrimitiveHelper<PrimitiveType,QChar>{
    static PrimitiveType unbox(const QChar* bt){
        return bt->unicode();
    }

    static void assign(QChar* bt, PrimitiveType pt){
        *bt = pt;
    }
};

template<typename PrimitiveType>
struct PrimitiveHelper<PrimitiveType,QLatin1Char>{
    static PrimitiveType unbox(const QLatin1Char* bt){
        return bt->unicode();
    }

    static void assign(QLatin1Char* bt, PrimitiveType pt){
        *bt = QLatin1Char(char(pt));
    }
};
}

template<typename PrimitiveType,
         typename BoxedType,
         jobject(*toJavaObject)(JNIEnv *, PrimitiveType),
         PrimitiveType fromJavaObject(JNIEnv *, jobject),
         typename... RuntimeTypes>
class PrimitiveTypeEntry : public AbstractSimpleTypeEntry{
public:
    using AbstractSimpleTypeEntry::AbstractSimpleTypeEntry;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const override{
        if constexpr(std::is_same<PrimitiveType,std::nullptr_t>::value){
            Q_UNUSED(env)
            Q_UNUSED(qt_object)
            output = nullptr;
        }else{
            const BoxedType* value = reinterpret_cast<const BoxedType*>(qt_object);
            if(value)
                output = toJavaObject(env, QtJambiPrivate::PrimitiveHelper<PrimitiveType,BoxedType>::unbox(value));
        }
        return true;
    }
    bool convertToNative(JNIEnv *env, jobject input, void * output) const override{
        if constexpr(std::is_same<PrimitiveType,std::nullptr_t>::value){
            Q_UNUSED(env)
            if(void** value = reinterpret_cast<void**>(output))
                *value = nullptr;
        }else{
            BoxedType* value = reinterpret_cast<BoxedType*>(output);
            if(value){
                if(QtJambiPrivate::IsInstanceOfAny<RuntimeTypes...>::fail(env, input))
                    return false;
                QtJambiPrivate::PrimitiveHelper<PrimitiveType,BoxedType>::assign(value, fromJavaObject(env, input));
            }
        }
        return true;
    }
private:
};

jboolean fromJavaBooleanObject(JNIEnv *env, jobject bool_object)
{
    return QtJambiAPI::fromJavaBooleanObject(env, bool_object);
}

using JLongTypeEntry = PrimitiveTypeEntry<jlong, jlong, QtJambiAPI::toJavaLongObject, QtJambiAPI::fromJavaLongObject, Java::Runtime::Byte, Java::Runtime::Short, Java::Runtime::Integer, Java::Runtime::Long>;
using JIntTypeEntry = PrimitiveTypeEntry<jint, jint, QtJambiAPI::toJavaIntegerObject, QtJambiAPI::fromJavaIntegerObject, Java::Runtime::Byte, Java::Runtime::Short, Java::Runtime::Integer>;
using JShortTypeEntry = PrimitiveTypeEntry<jshort, jshort, QtJambiAPI::toJavaShortObject, QtJambiAPI::fromJavaShortObject, Java::Runtime::Byte, Java::Runtime::Short, Java::Runtime::Integer>;
using JByteTypeEntry = PrimitiveTypeEntry<jbyte, jbyte, QtJambiAPI::toJavaByteObject, QtJambiAPI::fromJavaByteObject, Java::Runtime::Byte, Java::Runtime::Short, Java::Runtime::Integer>;
using JBooleanTypeEntry = PrimitiveTypeEntry<jboolean, jboolean, QtJambiAPI::toJavaBooleanObject, fromJavaBooleanObject, Java::Runtime::Boolean>;
using JDoubleTypeEntry = PrimitiveTypeEntry<jdouble, jdouble, QtJambiAPI::toJavaDoubleObject, QtJambiAPI::fromJavaDoubleObject, Java::Runtime::Number>;
using JFloatTypeEntry = PrimitiveTypeEntry<jfloat, jfloat, QtJambiAPI::toJavaFloatObject, QtJambiAPI::fromJavaFloatObject, Java::Runtime::Byte, Java::Runtime::Short, Java::Runtime::Integer, Java::Runtime::Float>;
using JCharTypeEntry = PrimitiveTypeEntry<jchar, jchar, QtJambiAPI::toJavaCharacterObject, QtJambiAPI::fromJavaCharacterObject, Java::Runtime::Character>;
using JCharQCharTypeEntry = PrimitiveTypeEntry<jchar, QChar, QtJambiAPI::toJavaCharacterObject, QtJambiAPI::fromJavaCharacterObject, Java::Runtime::Character>;
using JCharQLatin1CharTypeEntry = PrimitiveTypeEntry<jchar, QLatin1Char, QtJambiAPI::toJavaCharacterObject, QtJambiAPI::fromJavaCharacterObject, Java::Runtime::Character>;
using NullptrTypeEntry = PrimitiveTypeEntry<std::nullptr_t, std::nullptr_t, nullptr, nullptr>;

template<typename StringType>
class StringUtilTypeEntry : public AbstractSimpleTypeEntry{
public:
    using AbstractSimpleTypeEntry::AbstractSimpleTypeEntry;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jobject& output) const override;
    bool convertToNative(JNIEnv *env, jobject input, void * output) const override;
    bool convertToNative(JNIEnv *env, jobject input, void * output, QtJambiScope& scope) const override;
private:
};

template<typename TargetType>
class MetaUtilTypeEntry : public AbstractSimpleTypeEntry{
public:
    using AbstractSimpleTypeEntry::AbstractSimpleTypeEntry;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jobject& output) const override;
    bool convertToNative(JNIEnv *env, jobject input, void * output) const override;
private:
};

class MetaObjectConnectionTypeEntry : public MetaUtilTypeEntry<QMetaObject::Connection>{
    using MetaUtilTypeEntry<QMetaObject::Connection>::MetaUtilTypeEntry;
public:
    using MetaUtilTypeEntry<QMetaObject::Connection>::convertToJava;
    NativeToJavaResult convertToJava(JNIEnv *env, void *qt_object, jobject& output) const override;
};

template<typename T, class = decltype(std::declval<T>().typeInfoSupplier() )>
std::true_type  supports_typeInfoSupplier_test(const T&);
std::false_type supports_typeInfoSupplier_test(...);
template<typename T> struct supports_typeInfoSupplier : decltype(supports_typeInfoSupplier_test(std::declval<T>())){};

template<typename Super>
class ValueTypeEntry : public Super{
public:
    const QMetaType& metaType() const{ return m_qt_meta_type; }
    QtJambiTypeEntry::NativeToJavaResult convertToJava(JNIEnv *env, void *qt_object, jobject& output) const override{
        if(qt_object){
            if(jobject obj = QtJambiLink::findObjectForPointer(env, javaClass(), qt_object)){
                output = obj;
                return true;
            }
            CopyValueInfo value = moveValue(env, qt_object);
            if(!value.value || !value.isCopy){
                value = copyValue(env, qt_object, true);
            }
            if (!value.value){
                output = nullptr;
                return true;
            }
            output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return createLinkForNativeObject(
                env,
                output,
                value.value,
                NativeToJavaConversionMode::MakeCopyOfValues,
                value.isCopy,
                value.containerAccess);
        }
        return true;
    }
protected:
    using Super::Super;
    using typename Super::CopyValueInfo;
    using Super::convertToJava;
    using Super::javaClass;
    using Super::moveValue;
    using Super::copyValue;
    using Super::creatableClass;
    using Super::creatorMethod;
    using Super::createLinkForNativeObject;
private:
    bool isValue() const final{ return true; }
    QMetaType m_qt_meta_type;
    friend class ObjectTypeEntryFactory;
};

template<typename Super>
class ContainerTypeEntry : public ValueTypeEntry<Super>{
public:
    AbstractContainerAccess* createContainerAccess() const {
        return m_containerAccessFactory();
    }
protected:
    using ValueTypeEntry<Super>::ValueTypeEntry;
private:
    NewContainerAccessFunction m_containerAccessFactory;
    friend class ObjectTypeEntryFactory;
};

template<typename Super>
class InterfaceTypeEntry : public Super{
public:
    bool isInterface() const final override { return true; }
    jclass creatableClass() const override final { return m_java_wrapper_class; }
    jclass implementationClass() const { return m_java_impl_class; }
    bool convertToNative(JNIEnv *env, jobject input
                         , void * output) const override{
        if(input && !env->IsInstanceOf(input, Super::javaClass()))
            return false;
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, input)){
            if(!link->isMultiInheritanceType()){
                void *ptr = link->pointer();
                *reinterpret_cast<void**>(output) = ptr;
                return ptr;
            }else if(link->isInterfaceAvailable(Super::type())){
                void *ptr = link->typedPointer(Super::type());
                *reinterpret_cast<void**>(output) = ptr;
                return ptr;
            }else if(link->isQObject()){
                if(QObject* object = link->qobject()){
                    void* ptr = object->qt_metacast(Super::qtName());
                    if(ptr){
                        *reinterpret_cast<void**>(output) = ptr;
                        return true;
                    }else{
                        QByteArray typeName = Super::qtName();
                        if(typeName.endsWith("*")){
                            typeName = typeName.chopped(1);
                            ptr = object->qt_metacast(typeName);
                            if(ptr){
                                *reinterpret_cast<void**>(output) = ptr;
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, input))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, input).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        return true;
    }
private:
    using Super::Super;
    jclass m_java_impl_class = nullptr;
    jclass m_java_wrapper_class = nullptr;
    friend class ObjectTypeEntryFactory;
};

template<typename Super>
class VirtualTypeEntry : public Super{
public:
    TypeInfoSupplier typeInfoSupplier() const { return m_typeInfoSupplier; }
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
private:
    using Super::Super;
    TypeInfoSupplier m_typeInfoSupplier = nullptr;
    friend class ObjectTypeEntryFactory;
};

template<typename Super>
class OwnedTypeEntry : public Super{
public:
    PtrOwnerFunction ownerFunction() const { return m_owner_function; }
private:
    using Super::Super;
    PtrOwnerFunction m_owner_function = nullptr;
    friend class ObjectTypeEntryFactory;
};

class PolymorphicTypeEntryInterface{
public:
    virtual ~PolymorphicTypeEntryInterface();
    virtual const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicIdHandlers() const = 0;
};

template<typename Super>
class PolymorphicTypeEntry : public Super, public PolymorphicTypeEntryInterface{
public:
    const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicIdHandlers() const final { return m_polymorphicIdHandlers; }
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
    bool isPolymorphic() const final { return true; }
private:
    using Super::Super;
    QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> m_polymorphicIdHandlers;
    friend class QObjectTypeEntryFactory;
    friend class ObjectTypeEntryFactory;
};

template<typename Super>
class AbstractTypeEntry : public Super{
public:
    jclass creatableClass() const override final { return m_java_wrapper_class; }
private:
    using Super::Super;
    jclass m_java_wrapper_class = nullptr;
    friend class QObjectTypeEntryFactory;
    friend class ObjectTypeEntryFactory;
};

template<typename Super>
class ImplementingTypeEntry : public Super{
public:
    uint offset(const std::type_info& toType) const override{
        return m_interfaceOffsetInfo.offsets.value(unique_id(toType), 0);
    }
    const InterfaceOffsetInfo& interfaceOffsetInfo() const {
        return m_interfaceOffsetInfo;
    }
protected:
    using Super::Super;
private:
    InterfaceOffsetInfo m_interfaceOffsetInfo;
    friend class ObjectTypeEntryFactory;
    friend class QObjectTypeEntryFactory;
};

template<typename Super>
class ObjectTypeEntry : public Super{
protected:
    using Super::Super;
    QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *object, NativeToJavaConversionMode mode, bool isMetaCopy, AbstractContainerAccess* containerAccess) const override{
        Q_UNUSED(containerAccess)
        Q_UNUSED(isMetaCopy)
        QtJambiLink::Ownership ownership = QtJambiLink::Ownership::None;
        if constexpr(QtJambiPrivate::has_metatype<Super>::value){
            switch(mode){
            case NativeToJavaConversionMode::CppOwnership:
                ownership = QtJambiLink::Ownership::Cpp;
                break;
            case NativeToJavaConversionMode::None:
                break;
            case NativeToJavaConversionMode::MakeCopyOfValues:
            default:
                ownership = QtJambiLink::Ownership::Java;
                break;
            }
        }else if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
            ownership = mode==NativeToJavaConversionMode::TransferOwnership ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None;
        }
        if constexpr(supports_typeInfoSupplier<Super>::value){
            if(ownership != QtJambiLink::Ownership::Java && !isMetaCopy && enabledDanglingPointerCheck()){
                if constexpr(QtJambiPrivate::has_interface_offset<Super>::value){
                    if constexpr(QtJambiPrivate::has_metatype<Super>::value){
                        if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                            return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        Super::ownerFunction(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::interfaceOffsetInfo(),
                                        Super::typeInfoSupplier()
                                );
                        }else if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                            return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        containerAccess,
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::interfaceOffsetInfo(),
                                        Super::typeInfoSupplier()
                                );
                        }else{
                            return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::interfaceOffsetInfo(),
                                        Super::typeInfoSupplier()
                                );
                        }
                    }else if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        Super::ownerFunction(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::interfaceOffsetInfo(),
                                        Super::typeInfoSupplier()
                                );
                    }else{
                        return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::interfaceOffsetInfo(),
                                        Super::typeInfoSupplier()
                                        );
                    }
                }else{
                    // no interfaceOffsetInfo()
                    if constexpr(QtJambiPrivate::has_metatype<Super>::value){
                        if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                            return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        Super::ownerFunction(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::typeInfoSupplier()
                                );
                        }else if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                            return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        containerAccess,
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::typeInfoSupplier()
                                );
                        }else{
                            return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::typeInfoSupplier()
                                );
                        }
                    }else if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        Super::ownerFunction(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::typeInfoSupplier()
                                );
                    }else{
                        return QtJambiLink::createLinkForNativeObject(
                                        env,
                                        javaObject,
                                        object,
                                        LINK_NAME_ARG(Super::qtName())
                                        Super::deleter(),
                                        ownership==QtJambiLink::Ownership::Cpp,
                                        Super::typeInfoSupplier()
                                );
                    }
                }
            }
        }
        if constexpr(QtJambiPrivate::has_interface_offset<Super>::value){
            if constexpr(QtJambiPrivate::has_metatype<Super>::value){
                if(isMetaCopy){
                    if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    Super::metaType(),
                                    false,
                                    false,
                                    Super::ownerFunction(),
                                    ownership,
                                    Super::interfaceOffsetInfo()
                            );
                    }else if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    containerAccess,
                                    ownership,
                                    Super::interfaceOffsetInfo()
                            );
                    }else{
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    Super::metaType(),
                                    false,
                                    false,
                                    ownership,
                                    Super::interfaceOffsetInfo()
                            );
                    }
                }else{
                    if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    Super::deleter(),
                                    Super::ownerFunction(),
                                    ownership,
                                    Super::interfaceOffsetInfo()
                            );
                    }else if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    Super::deleter(),
                                    containerAccess,
                                    ownership,
                                    Super::interfaceOffsetInfo()
                            );
                    }else{
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    Super::deleter(),
                                    ownership,
                                    Super::interfaceOffsetInfo()
                            );
                    }
                }
            }else if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                return QtJambiLink::createLinkForNativeObject(
                                env,
                                javaObject,
                                object,
                                LINK_NAME_ARG(Super::qtName())
                                false,
                                false,
                                Super::deleter(),
                                Super::ownerFunction(),
                                ownership,
                                Super::interfaceOffsetInfo()
                        );
            }else{
                return QtJambiLink::createLinkForNativeObject(
                                env,
                                javaObject,
                                object,
                                LINK_NAME_ARG(Super::qtName())
                                false,
                                false,
                                Super::deleter(),
                                ownership,
                                Super::interfaceOffsetInfo()
                                );
            }
        }else{
            // no interfaceOffsetInfo()
            if constexpr(QtJambiPrivate::has_metatype<Super>::value){
                if(isMetaCopy){
                    if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    Super::metaType(),
                                    false,
                                    false,
                                    Super::ownerFunction(),
                                    ownership
                            );
                    }else if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    containerAccess,
                                    ownership
                            );
                    }else{
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    Super::metaType(),
                                    false,
                                    false,
                                    ownership
                            );
                    }
                }else{
                    if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    Super::deleter(),
                                    Super::ownerFunction(),
                                    ownership
                            );
                    }else if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    Super::deleter(),
                                    containerAccess,
                                    ownership
                            );
                    }else{
                        return QtJambiLink::createLinkForNativeObject(
                                    env,
                                    javaObject,
                                    object,
                                    LINK_NAME_ARG(Super::qtName())
                                    false,
                                    false,
                                    Super::deleter(),
                                    ownership
                            );
                    }
                }
            }else if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                return QtJambiLink::createLinkForNativeObject(
                                env,
                                javaObject,
                                object,
                                LINK_NAME_ARG(Super::qtName())
                                false,
                                false,
                                Super::deleter(),
                                Super::ownerFunction(),
                                ownership
                        );
            }else{
                return QtJambiLink::createLinkForNativeObject(
                                env,
                                javaObject,
                                object,
                                LINK_NAME_ARG(Super::qtName())
                                false,
                                false,
                                Super::deleter(),
                                ownership
                        );
            }
        }
    }

    template<template<typename> class SmartPointer>
    QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const SmartPointer<char>& smartPointer, PtrOwnerFunction ownerFunction) const{
        Q_UNUSED(createdByJava)
        Q_UNUSED(is_shell)
        if constexpr(QtJambiPrivate::has_interface_offset<Super>::value){
            if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                return QtJambiLink::createLinkForSmartPointerToObject(
                        env,
                        javaObject,
                        LINK_NAME_ARG(Super::qtName())
                        smartPointer,
                        Super::createContainerAccess(),
                        Super::interfaceOffsetInfo()
                    );
            }else{
                if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                    if(!ownerFunction)
                        ownerFunction = Super::ownerFunction();
                    return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                          LINK_NAME_ARG(Super::qtName())
                                                                          createdByJava,
                                                                          is_shell,
                                                                          ownerFunction,
                                                                          smartPointer,
                                                                          Super::interfaceOffsetInfo());
                }else{
                    if(ownerFunction){
                        return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                              LINK_NAME_ARG(Super::qtName())
                                                                              createdByJava,
                                                                              is_shell,
                                                                              ownerFunction,
                                                                              smartPointer,
                                                                              Super::interfaceOffsetInfo());
                    }else{
                        return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                              LINK_NAME_ARG(Super::qtName())
                                                                              createdByJava,
                                                                              is_shell,
                                                                              smartPointer,
                                                                              Super::interfaceOffsetInfo());
                    }
                }
            }
        }else{
            if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                return QtJambiLink::createLinkForSmartPointerToObject(
                        env,
                        javaObject,
                        LINK_NAME_ARG(Super::qtName())
                        smartPointer,
                        Super::createContainerAccess()
                    );
            }else{
                if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                    if(!ownerFunction)
                        ownerFunction = Super::ownerFunction();
                    return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                          LINK_NAME_ARG(Super::qtName())
                                                                          createdByJava,
                                                                          is_shell,
                                                                          ownerFunction,
                                                                          smartPointer);
                }else{
                    if(ownerFunction){
                        return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                              LINK_NAME_ARG(Super::qtName())
                                                                              createdByJava,
                                                                              is_shell,
                                                                              ownerFunction,
                                                                              smartPointer);
                    }else{
                        return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                              LINK_NAME_ARG(Super::qtName())
                                                                              createdByJava,
                                                                              is_shell,
                                                                              smartPointer);
                    }
                }
            }
        }
    }

    QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const QSharedPointer<char>& smartPointer, PtrOwnerFunction ownerFunction) const override{
        return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject, createdByJava, is_shell, smartPointer, ownerFunction);
    }

    QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const std::shared_ptr<char>& smartPointer, PtrOwnerFunction ownerFunction) const override{
        return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject, createdByJava, is_shell, smartPointer, ownerFunction);
    }
    ObjectTypeAbstractEntry::CopyValueInfo moveValue(JNIEnv *env, void* qt_object) const override{
        if constexpr(QtJambiPrivate::has_metatype<Super>::value){
            if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                checkThreadOnQObject(env, Super::ownerFunction(), qt_object, Super::creatableClass());
            }else{
                Q_UNUSED(env)
            }
            if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                AbstractContainerAccess* containerAccess = Super::createContainerAccess();
                return ObjectTypeAbstractEntry::CopyValueInfo{containerAccess->createContainer(qt_object), true, containerAccess};
            }else{
                if(Super::metaType().isMoveConstructible()){
                    auto iface = const_cast<QMetaType&>(Super::metaType()).iface();
                    void* newPointer;
                    if (iface->alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                        newPointer = operator new(iface->size, std::align_val_t(iface->alignment));
                    else
                        newPointer = operator new(iface->size);
                    QtMetaTypePrivate::moveConstruct(iface, newPointer, qt_object);
                    return ObjectTypeAbstractEntry::CopyValueInfo{newPointer, true};
                }else{
                    return ObjectTypeAbstractEntry::CopyValueInfo{nullptr};
                }
            }
        }else{
            if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                AbstractContainerAccess* containerAccess = Super::createContainerAccess();
                return ObjectTypeAbstractEntry::CopyValueInfo{containerAccess->createContainer(qt_object), true, containerAccess};
            }else{
                return ObjectTypeAbstractEntry::CopyValueInfo{nullptr};
            }
        }
    }
    ObjectTypeAbstractEntry::CopyValueInfo copyValue(JNIEnv *env, const void* qt_object, bool makeCopyOfValue) const override{
        if constexpr(QtJambiPrivate::has_metatype<Super>::value){
            if constexpr(QtJambiPrivate::has_owner_function<Super>::value){
                checkThreadOnQObject(env, Super::ownerFunction(), qt_object, Super::creatableClass());
            }else{
                Q_UNUSED(env)
            }
            if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                AbstractContainerAccess* containerAccess = Super::createContainerAccess();
                if(makeCopyOfValue)
                    return ObjectTypeAbstractEntry::CopyValueInfo{containerAccess->createContainer(qt_object), true, containerAccess};
                else
                    return ObjectTypeAbstractEntry::CopyValueInfo{const_cast<void*>(qt_object), false, containerAccess};
            }else{
                if(makeCopyOfValue){
                    if(qt_object ? Super::metaType().isCopyConstructible() : Super::metaType().isDefaultConstructible()){
                        return ObjectTypeAbstractEntry::CopyValueInfo{Super::metaType().create(qt_object), true};
                    }else{
                        return ObjectTypeAbstractEntry::CopyValueInfo{const_cast<void*>(qt_object)};
                    }
                }else{
                    return ObjectTypeAbstractEntry::CopyValueInfo{const_cast<void*>(qt_object)};
                }
            }
        }else{
            if constexpr(QtJambiPrivate::has_container_access<Super>::value){
                AbstractContainerAccess* containerAccess = Super::createContainerAccess();
                if(makeCopyOfValue)
                    return ObjectTypeAbstractEntry::CopyValueInfo{containerAccess->createContainer(qt_object), true, containerAccess};
                else
                    return ObjectTypeAbstractEntry::CopyValueInfo{const_cast<void*>(qt_object), false, containerAccess};
            }else{
                Q_UNUSED(env)
                Q_UNUSED(makeCopyOfValue)
                return ObjectTypeAbstractEntry::CopyValueInfo{const_cast<void*>(qt_object)};
            }
        }
    }
    friend class ObjectTypeEntryFactory;
    friend class QModelIndexTypeEntry;
};

template<typename Super>
class NoDebugMessagingObjectTypeEntry : public ObjectTypeEntry<Super>{
public:
protected:
    using ObjectTypeEntry<Super>::ObjectTypeEntry;
private:
    QtJambiTypeEntry::NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jobject& output) const{
        if (!qt_object){
            output = nullptr;
            return true;
        }
        output = env->NewObject(ObjectTypeEntry<Super>::creatableClass(), ObjectTypeEntry<Super>::creatorMethod(), nullptr);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        auto link = ObjectTypeEntry<Super>::createLinkForNativeObject(
            env,
            output,
            const_cast<void*>(qt_object),
            mode,
            false,
            nullptr);
        link->disableDebugMessaging();
        return link;
    }
};

template<typename Super>
class CustomMetaObjectTypeEntry : public Super{
public:
    const QMetaObject* customMetaObject() const{ return m_superTypeForCustomMetaObject; }
protected:
    using Super::Super;
private:
    const QMetaObject* m_superTypeForCustomMetaObject = nullptr;
    friend class QObjectTypeEntryFactory;
};

template<typename Super>
class QObjectTypeEntry : public Super{
protected:
    using Super::Super;
    QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& javaObject, QObject *object) const override{
        if constexpr(QtJambiPrivate::has_interface_offset<Super>::value){
            if constexpr(QtJambiPrivate::has_superTypeForCustomMetaObject<Super>::value){
                return QtJambiLink::createLinkForNativeQObject(env, javaObject, object, Super::isQThread(), Super::interfaceOffsetInfo(), Super::customMetaObject());
            }else{
                return QtJambiLink::createLinkForNativeQObject(env, javaObject, object, Super::isQThread(), Super::interfaceOffsetInfo());
            }
        }else{
            if constexpr(QtJambiPrivate::has_superTypeForCustomMetaObject<Super>::value){
                return QtJambiLink::createLinkForNativeQObject(env, javaObject, object, Super::isQThread(), Super::customMetaObject());
            }else{
                return QtJambiLink::createLinkForNativeQObject(env, javaObject, object, Super::isQThread());
            }
        }
    }
    template<template<typename> class SmartPointer>
    QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const SmartPointer<QObject>& smartPointer) const{
        if constexpr(QtJambiPrivate::has_interface_offset<Super>::value){
            if constexpr(QtJambiPrivate::has_superTypeForCustomMetaObject<Super>::value){
                return QtJambiLink::createLinkForSmartPointerToQObject(env, javaObject, createdByJava, is_shell, smartPointer, Super::isQThread(), Super::interfaceOffsetInfo(), Super::customMetaObject());
            }else{
                return QtJambiLink::createLinkForSmartPointerToQObject(env, javaObject, createdByJava, is_shell, smartPointer, Super::isQThread(), Super::interfaceOffsetInfo());
            }
        }else{
            if constexpr(QtJambiPrivate::has_superTypeForCustomMetaObject<Super>::value){
                return QtJambiLink::createLinkForSmartPointerToQObject(env, javaObject, createdByJava, is_shell, smartPointer, Super::isQThread(), Super::customMetaObject());
            }else{
                return QtJambiLink::createLinkForSmartPointerToQObject(env, javaObject, createdByJava, is_shell, smartPointer, Super::isQThread());
            }
        }
    }
    QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const QSharedPointer<QObject>& smartPointer) const override{
        return createLinkForSmartPointerToQObject<QSharedPointer>(env, javaObject, createdByJava, is_shell, smartPointer);
    }
    QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const std::shared_ptr<QObject>& smartPointer) const override{
        return createLinkForSmartPointerToQObject<std::shared_ptr>(env, javaObject, createdByJava, is_shell, smartPointer);
    }
    friend class QObjectTypeEntryFactory;
};

class QModelIndexTypeEntry : public ObjectTypeEntry<ObjectTypeAbstractEntry>{
public:
    using ObjectTypeEntry<ObjectTypeAbstractEntry>::ObjectTypeEntry;
    template<typename... Args>
    inline static auto create(Args&&... args){
        return new QModelIndexTypeEntry(std::forward<Args>(args)...);
    }
    const QMetaType& metaType() const{
        static QMetaType t(QMetaType::QModelIndex);
        return t;
    }
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jobject& output) const override{
        if(!qt_object){
            output = nullptr;
            return true;
        }
        return convertModelIndexNativeToJava(env, *reinterpret_cast<const QModelIndex *>(qt_object), mode, output);
    }
    NativeToJavaResult convertToJava(JNIEnv *env, void *qt_object, jobject& output) const override{
        if(!qt_object){
            output = nullptr;
            return true;
        }
        return convertModelIndexNativeToJava(env, std::move(*reinterpret_cast<QModelIndex *>(qt_object)), output);
    }
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    bool convertToNative(JNIEnv *env, jobject input, void * output, QtJambiScope& scope) const override{
        return convertModelIndexJavaToNative(env, input, output, scope);
    }
#endif
    bool convertToNative(JNIEnv *env, jobject input, void * output) const override{
        return convertModelIndexJavaToNative(env, input, output);
    }
    static void deleter(void *ptr, bool){
        QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QModelIndex")
        QModelIndex *_ptr = reinterpret_cast<QModelIndex *>(ptr);
        QtJambiAPI::registerNonShellDeletion(ptr);
        delete _ptr;
    }
private:
    bool isValue() const final{
        return true;
    }
    template<template<typename> class SmartPointer>
    QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const SmartPointer<char>& smartPointer, PtrOwnerFunction ownerFunction) const{
        const QModelIndex* index = reinterpret_cast<const QModelIndex*>(smartPointer.get());
        if(index->model()){
            return QtJambiLink::createExtendedLinkForSmartPointerToObject(
                                                                env,
                                                                javaObject,
                                                                LINK_NAME_ARG(qtName())
                                                                createdByJava,
                                                                is_shell,
                                                                index->model(),
                                                                smartPointer
            );
        }else if(ownerFunction){
            return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                LINK_NAME_ARG(qtName())
                                                                createdByJava,
                                                                is_shell,
                                                                ownerFunction,
                                                                smartPointer);
        }else{
            return QtJambiLink::createLinkForSmartPointerToObject(env, javaObject,
                                                                LINK_NAME_ARG(qtName())
                                                                createdByJava,
                                                                is_shell,
                                                                smartPointer);
        }
    }

    QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const QSharedPointer<char>& smartPointer, PtrOwnerFunction ownerFunction = nullptr) const override{
        return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject, createdByJava, is_shell, smartPointer, ownerFunction);
    }
    QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const std::shared_ptr<char>& smartPointer, PtrOwnerFunction ownerFunction = nullptr) const override{
        return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject, createdByJava, is_shell, smartPointer, ownerFunction);
    }
};

class QObjectTypeEntryFactory{
public:
    template<typename... Args>
    static auto create(JNIEnv *env, Args&&... args){
        return new QObjectTypeEntry<QObjectTypeAbstractEntry>(env, std::forward<Args>(args)...);
    }
    template<typename... Args>
    static auto create(const InterfaceOffsetInfo& interfaceOffsetInfo, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        auto instance = new QObjectTypeEntry<ImplementingTypeEntry<QObjectTypeAbstractEntry>>(std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        Q_ASSERT(superTypeForCustomMetaObject);
        auto instance = new QObjectTypeEntry<CustomMetaObjectTypeEntry<ImplementingTypeEntry<QObjectTypeAbstractEntry>>>(std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        auto instance = new QObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject, JNIEnv* env, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        Q_ASSERT(superTypeForCustomMetaObject);
        auto instance = new QObjectTypeEntry<CustomMetaObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new QObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<QObjectTypeAbstractEntry>>>(std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        Q_ASSERT(superTypeForCustomMetaObject);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new QObjectTypeEntry<CustomMetaObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<QObjectTypeAbstractEntry>>>>(std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new QObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject, JNIEnv* env, Args&&... args){
        Q_ASSERT(!interfaceOffsetInfo.offsets.isEmpty());
        Q_ASSERT(superTypeForCustomMetaObject);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new QObjectTypeEntry<CustomMetaObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaObject* superTypeForCustomMetaObject, Args&&... args){
        Q_ASSERT(superTypeForCustomMetaObject);
        auto instance = new QObjectTypeEntry<CustomMetaObjectTypeEntry<QObjectTypeAbstractEntry>>(std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_wrapper_class, const QMetaObject* superTypeForCustomMetaObject, JNIEnv* env, Args&&... args){
        Q_ASSERT(superTypeForCustomMetaObject);
        auto instance = new QObjectTypeEntry<CustomMetaObjectTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const QMetaObject* superTypeForCustomMetaObject, Args&&... args){
        Q_ASSERT(superTypeForCustomMetaObject);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new QObjectTypeEntry<PolymorphicTypeEntry<CustomMetaObjectTypeEntry<QObjectTypeAbstractEntry>>>(std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const QMetaObject* superTypeForCustomMetaObject, JNIEnv* env, Args&&... args){
        Q_ASSERT(superTypeForCustomMetaObject);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new QObjectTypeEntry<PolymorphicTypeEntry<CustomMetaObjectTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_superTypeForCustomMetaObject = superTypeForCustomMetaObject;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv* env, Args&&... args){
        auto instance = new QObjectTypeEntry<PolymorphicTypeEntry<QObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        auto instance = new QObjectTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new QObjectTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<QObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }

}; // class QObjectTypeEntryFactory

enum LinkFlag{
    NoDebugMessaging
};

class ObjectTypeEntryFactory{
public:
    template<typename... Args>
    static auto create(JNIEnv *env, Args&&... args){
        return new ObjectTypeEntry<ObjectTypeAbstractEntry>(env, std::forward<Args>(args)...);
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_qt_meta_type = metaType;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_qt_meta_type = metaType;
        return instance;
    }

    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }

    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        return instance;
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_qt_meta_type = metaType;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }

    template<typename... Args>
    static auto create(jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<AbstractTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<AbstractTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<AbstractTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<AbstractTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<AbstractTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<AbstractTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        return instance;
    }

    template<typename... Args>
    static auto create(const QMetaType& metaType, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ValueTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        return instance;
    }

    template<typename... Args>
    static auto create(NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ContainerTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        return instance;
    }

    template<typename... Args>
    static auto create(LinkFlag, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<ObjectTypeAbstractEntry>(env, std::forward<Args>(args)...);
        return instance;
    }
    template<typename... Args>
    static auto create(LinkFlag, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<ImplementingTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(LinkFlag, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        return instance;
    }
    template<typename... Args>
    static auto create(LinkFlag, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        Q_ASSERT(owner_function);
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        Q_ASSERT(owner_function);
        instance->m_owner_function = owner_function;
        instance->m_qt_meta_type = metaType;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        Q_ASSERT(owner_function);
        instance->m_owner_function = owner_function;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        Q_ASSERT(owner_function);
        instance->m_owner_function = owner_function;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_qt_meta_type = metaType;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }

    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }

    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        Q_ASSERT(owner_function);
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(owner_function);
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        Q_ASSERT(owner_function);
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_qt_meta_type = metaType;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<OwnedTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<OwnedTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_impl_class, jclass java_wrapper_class, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<InterfaceTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<OwnedTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_owner_function = owner_function;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        instance->m_java_impl_class = java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }

    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<AbstractTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<AbstractTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<AbstractTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<AbstractTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        instance->m_owner_function = owner_function;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<AbstractTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<AbstractTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        instance->m_owner_function = owner_function;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, PtrOwnerFunction owner_function, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, jclass java_wrapper_class, JNIEnv* env, Args&&... args){
        Q_ASSERT(owner_function);
        Q_ASSERT(!polymorphicHandlers.isEmpty());
        auto instance = new ObjectTypeEntry<OwnedTypeEntry<PolymorphicTypeEntry<AbstractTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_owner_function = owner_function;
        instance->m_java_wrapper_class = java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }

    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, const QMetaType& metaType, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ValueTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }

    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, NewContainerAccessFunction containerAccessFactory, const QMetaType& metaType, JNIEnv* env, Args&&... args){
        auto instance = new ObjectTypeEntry<ContainerTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_qt_meta_type = metaType;
        instance->m_containerAccessFactory = containerAccessFactory;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }

    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, LinkFlag, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>(env, std::forward<Args>(args)...);
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, LinkFlag, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<ImplementingTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, LinkFlag, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }
    template<typename... Args>
    static auto create(TypeInfoSupplier typeInfoSupplier, LinkFlag, const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicHandlers, const InterfaceOffsetInfo& interfaceOffsetInfo, JNIEnv *env, Args&&... args){
        auto instance = new NoDebugMessagingObjectTypeEntry<ImplementingTypeEntry<PolymorphicTypeEntry<VirtualTypeEntry<ObjectTypeAbstractEntry>>>>(env, std::forward<Args>(args)...);
        instance->m_polymorphicIdHandlers = polymorphicHandlers;
        instance->m_interfaceOffsetInfo = interfaceOffsetInfo;
        Q_ASSERT(typeInfoSupplier);
        instance->m_typeInfoSupplier = typeInfoSupplier;
        return instance;
    }

}; // class ObjectTypeEntryFactory

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntry(JNIEnv* env, const std::type_info& typeId, const char* qtName)
{
    return getTypeEntry(env, typeId, true, qtName);
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByJavaName(JNIEnv* env, const char* java_name)
{
    const std::type_info* typeId = getTypeByJavaName(java_name);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else return QtJambiTypeEntryPtr();
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByJavaName(JNIEnv* env, const QString& java_name)
{
    const std::type_info* typeId = getTypeByJavaName(java_name);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else return QtJambiTypeEntryPtr();
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByQtName(JNIEnv* env, const char* qt_name)
{
    const std::type_info* typeId = getTypeByQtName(qt_name);
    if(typeId)
        return getTypeEntry(env, *typeId, qt_name);
    else  return QtJambiTypeEntryPtr();
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByIID(JNIEnv* env, const char* iid)
{
    const std::type_info* typeId = getInterfaceTypeForIID(iid);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else  return QtJambiTypeEntryPtr();
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntry(JNIEnv* env, const std::type_info& typeId, bool recursive, const char* qtName)
{
    QtJambiTypeEntryPtr result;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker locker(storage->lock());
            if(storage->typeEntries().contains(unique_id(typeId))){
                const QMap<QByteArray,QtJambiTypeEntryPtr>& entries = storage->typeEntries()[unique_id(typeId)];
                if(qtName){
                    if(QtJambiTypeEntryPtr result = entries.value(qtName)){
                        return result;
                    }
                }else if(!entries.isEmpty()){
                    return entries.first();
                }
            }
        }

        const RegisteredTypeInfo typeInfo = registeredTypeInfo(typeId, qtName);

        if(typeInfo.entryType!=EntryTypes::Unspecific){
            QTJAMBI_JNI_LOCAL_FRAME(env, 512);
            if(!typeInfo.javaName){
                if(qtName)
                    JavaException::raiseError(env, QLatin1String("Java class for native type %1 cannot be found").arg(qtName) QTJAMBI_STACKTRACEINFO );
                else if(typeInfo.qtName)
                    JavaException::raiseError(env, QLatin1String("Java class for native type %1 cannot be found").arg(typeInfo.qtName) QTJAMBI_STACKTRACEINFO );
                else
                    JavaException::raiseError(env, QLatin1String("Java class for native type %1 cannot be found").arg(QtJambiAPI::typeName(typeId)) QTJAMBI_STACKTRACEINFO );
            }else if(jclass java_class = JavaAPI::resolveClass(env, typeInfo.javaName)){
                switch(typeInfo.entryType){
                case EntryTypes::EnumTypeInfo:
                {
                    if(recursive){
                        if(typeInfo.flagForEnum){
                            QtJambiTypeEntryPtr e = QtJambiTypeEntry::getTypeEntry(env, *typeInfo.flagForEnum);
                            if(const FlagsTypeEntry* flagsType = dynamic_cast<const FlagsTypeEntry*>(e.data())){
                                return QtJambiTypeEntryPtr(flagsType->enumType());
                            }
                        }
                    }
                    QString sig;
                    switch ( typeInfo.valueSizeAndAlignment.first ) {
                    case 1:  sig = QLatin1String("(B)L%1;"); break;
                    case 2:  sig = QLatin1String("(S)L%1;"); break;
                    case 8:  sig = QLatin1String("(J)L%1;"); break;
                    default: sig = QLatin1String("(I)L%1;"); break;
                    }
                    jthrowable exceptionOccurred = nullptr;
                    jmethodID creator_method = JavaAPI::resolveMethod(env, "resolve", qPrintable(sig.arg(typeInfo.javaName)), java_class, true, &exceptionOccurred);
                    if(exceptionOccurred)
                        JavaException(env, exceptionOccurred).raise();
                    Q_ASSERT(creator_method);
                    result = new EnumTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    break;
                }
                case EntryTypes::FlagsTypeInfo:
                {
                    Q_ASSERT(typeInfo.enumForFlag);
                    jthrowable exceptionOccurred = nullptr;
                    jmethodID creator_method = JavaAPI::resolveMethod(env, "<init>", typeInfo.valueSizeAndAlignment.first==8 ? "(J)V" : "(I)V", java_class, false, &exceptionOccurred);
                    if(exceptionOccurred)
                        JavaException(env, exceptionOccurred).raise();
                    Q_ASSERT(creator_method);
                    auto e = getTypeEntry(env, *typeInfo.enumForFlag, false, nullptr);
                    result = new FlagsTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, dynamic_cast<const EnumTypeEntry*>(e.data()));
                    break;
                }
                case EntryTypes::StdFunctionTypeInfo:
                {
                    jclass java_impl_class = nullptr;
                    jclass java_wrapper_class = nullptr;
                    jmethodID creator_method = nullptr;
                    if(Java::Runtime::Class::isInterface(env, java_class)){
                        java_impl_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl").arg(typeInfo.javaName)));
                        if(!java_impl_class){
                            JavaException::raiseError(env, QLatin1String("class %1.Impl cannot be found").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                            return QtJambiTypeEntryPtr();
                        }
                        int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(typeInfo.javaName)));
                        }else{
                            java_wrapper_class = java_impl_class;
                        }
                    }else{
                        java_impl_class = java_class;
                        int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(typeInfo.javaName)));
                        }else{
                            java_wrapper_class = java_impl_class;
                        }
                    }
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.Impl").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        return QtJambiTypeEntryPtr();
                    }
                    Q_ASSERT(typeInfo.metaType.isValid());
                    result = new StdFunctionalTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                     java_impl_class, java_wrapper_class, typeInfo.virtualFunctions, typeInfo.destructor, typeInfo.functionalResolver, typeInfo.metaType);
                    break;
                }
                case EntryTypes::FunctionPointerTypeInfo:
                {
                    jclass java_impl_class = nullptr;
                    jclass java_wrapper_class = nullptr;
                    jmethodID creator_method = nullptr;
                    if(Java::Runtime::Class::isInterface(env, java_class)){
                        java_impl_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl").arg(typeInfo.javaName)));
                        if(!java_impl_class){
                            JavaException::raiseError(env, QLatin1String("class %1.Impl cannot be found").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                            return QtJambiTypeEntryPtr();
                        }
                        int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(typeInfo.javaName)));
                        }else{
                            java_wrapper_class = java_impl_class;
                        }
                    }else{
                        java_impl_class = java_class;
                        int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(typeInfo.javaName)));
                        }else{
                            java_wrapper_class = java_impl_class;
                        }
                    }
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.Impl").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        return QtJambiTypeEntryPtr();
                    }
                    Q_ASSERT(typeInfo.metaType.isValid());
                    result = new FunctionPointerTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                     java_impl_class, java_wrapper_class, typeInfo.virtualFunctions, typeInfo.destructor, typeInfo.functionalResolver, typeInfo.metaType);
                    break;
                }
                case EntryTypes::ObjectTypeInfo:
                {
                    int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
                    const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicIdHandlers = typeInfo.polymorphicIdHandlers;
                    if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                        jclass java_wrapper_class = nullptr;
                        try{
                            java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(typeInfo.javaName)));
                        } catch (const JavaException&) {
                        }
                        jmethodID creator_method = nullptr;
                        if(java_wrapper_class){
                            creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                            if(!creator_method){
                                JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.ConcreteWrapper").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                                return QtJambiTypeEntryPtr();
                            }
                        }
                        if(typeInfo.typeInfoSupplier){
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }else{
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers, java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers, java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }else{
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                         java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers, java_wrapper_class,
                                                                         typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }
                        }else{ // !typeInfo.typeInfoSupplier
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }else{
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers, java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }else{
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                         java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class,
                                                                         typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }
                        }
                    }else{
                        jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                        if(!creator_method){
                            JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                            return QtJambiTypeEntryPtr();
                        }
                        if(typeInfo.typeInfoSupplier){
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                         typeInfo.destructor
                                                                                    );
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                     typeInfo.destructor
                                                                 );
                                    }
                                }else{
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }else{
                                    if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }
                        }else{ // !typeInfo.typeInfoSupplier
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeid_equals(typeId, typeid(QMessageLogContext))){
                                        Q_ASSERT(!typeInfo.ownerFunction);
                                        result = ObjectTypeEntryFactory::create(NoDebugMessaging, env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                         typeInfo.destructor);
                                    }else if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                         typeInfo.destructor
                                                                                    );
                                    }else{
                                        result = ObjectTypeEntryFactory::create(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                     typeInfo.destructor
                                                                 );
                                    }
                                }else{
                                    if(typeid_equals(typeId, typeid(QMessageLogContext))){
                                        Q_ASSERT(!typeInfo.ownerFunction);
                                        result = ObjectTypeEntryFactory::create(NoDebugMessaging, typeInfo.interfaceOffsetInfo,
                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                             java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                             typeInfo.destructor);
                                    }else if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    if(typeid_equals(typeId, typeid(QMessageLogContext))){
                                        Q_ASSERT(!typeInfo.ownerFunction);
                                        result = ObjectTypeEntryFactory::create(NoDebugMessaging, polymorphicIdHandlers,
                                                                                           env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                           creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                           typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                           typeInfo.destructor);
                                    }else if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(polymorphicIdHandlers,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }else{
                                    if(typeid_equals(typeId, typeid(QMessageLogContext))){
                                        Q_ASSERT(!typeInfo.ownerFunction);
                                        result = ObjectTypeEntryFactory::create(NoDebugMessaging, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                             java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                             typeInfo.destructor);
                                    }else if(typeInfo.ownerFunction){
                                        result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                         java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }else{
                                        result = ObjectTypeEntryFactory::create(polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                         env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                         creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                         typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                         typeInfo.destructor);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                case EntryTypes::ValueTypeInfo:
                {
                    const QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>& polymorphicIdHandlers = typeInfo.polymorphicIdHandlers;

                    if(typeid_equals(typeId, typeid(QModelIndex))){
                        Q_ASSERT(!typeInfo.ownerFunction);
                        result = QModelIndexTypeEntry::create(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                              nullptr,
                                                              sizeof(QModelIndex),
                                                              alignof(QModelIndex),
                                                              0,0,
                                                              QModelIndexTypeEntry::deleter,
                                                              QVector<RegistryAPI::FunctionInfo>{},
                                                              typeInfo.destructor);
                    }else{
                        int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            jclass java_wrapper_class = nullptr;
                            try{
                                java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(typeInfo.javaName)));
                            } catch (const JavaException&) {
                            }
                            jmethodID creator_method = nullptr;
                            if(java_wrapper_class){
                                creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                                if(!creator_method){
                                    JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.ConcreteWrapper").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                                    return QtJambiTypeEntryPtr();
                                }
                            }
                            if(typeInfo.typeInfoSupplier){
                                if(typeInfo.containerAccessFactory){
                                    Q_ASSERT(typeInfo.metaType.isValid());
                                    Q_ASSERT(!typeInfo.ownerFunction);
                                    if(polymorphicIdHandlers.isEmpty()){
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType, java_wrapper_class,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                             java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                             java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                             typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }
                                    }else{
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                             polymorphicIdHandlers, java_wrapper_class,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                             java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                             polymorphicIdHandlers,
                                                                             java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                             typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }
                                    }
                                }else{
                                    if(typeInfo.metaType.isValid()){
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction, java_wrapper_class,
                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                     typeInfo.virtualFunctions, typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName,
                                                                                     typeInfo.javaName, java_class, creator_method,
                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction,
                                                                                     polymorphicIdHandlers, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, polymorphicIdHandlers, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction,
                                                                                     polymorphicIdHandlers, java_wrapper_class,
                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                     typeInfo.virtualFunctions, typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType,
                                                                                     polymorphicIdHandlers,
                                                                                     java_wrapper_class,
                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName,
                                                                                     typeInfo.javaName, java_class, creator_method,
                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }else{
                                        Q_ASSERT(typeInfo.deleter);
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, java_wrapper_class,
                                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor
                                                                                                         );
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_wrapper_class,
                                                                                                        env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                        creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                        typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                        typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                          creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers, java_wrapper_class,
                                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor
                                                                                                         );
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers, java_wrapper_class,
                                                                                                        env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                        creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                        typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                        typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                          creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }
                                }
                            }else{ // !typeInfo.typeInfoSupplier
                                if(typeInfo.containerAccessFactory){
                                    Q_ASSERT(typeInfo.metaType.isValid());
                                    Q_ASSERT(!typeInfo.ownerFunction);
                                    if(polymorphicIdHandlers.isEmpty()){
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType, java_wrapper_class,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                             java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                             java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                             typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }
                                    }else{
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                             polymorphicIdHandlers, java_wrapper_class,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                             java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                             polymorphicIdHandlers,
                                                                             java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                             typeInfo.deleter, typeInfo.virtualFunctions,
                                                                             typeInfo.destructor);
                                        }
                                    }
                                }else{
                                    if(typeInfo.metaType.isValid()){
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction, java_wrapper_class,
                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                     typeInfo.virtualFunctions, typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName,
                                                                                     typeInfo.javaName, java_class, creator_method,
                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction,
                                                                                     polymorphicIdHandlers, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, polymorphicIdHandlers, java_wrapper_class,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction,
                                                                                     polymorphicIdHandlers, java_wrapper_class,
                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                     typeInfo.virtualFunctions, typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType,
                                                                                     polymorphicIdHandlers,
                                                                                     java_wrapper_class,
                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                     env, typeId, typeInfo.qtName,
                                                                                     typeInfo.javaName, java_class, creator_method,
                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }else{
                                        Q_ASSERT(typeInfo.deleter);
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, java_wrapper_class,
                                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor
                                                                                                         );
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(java_wrapper_class,
                                                                                                        env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                        creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                        typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                        typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                          creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers, java_wrapper_class,
                                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor
                                                                                                         );
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class,
                                                                                                        env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                        creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                        typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                        typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                          creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                            jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                            if(!creator_method){
                                JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                                return QtJambiTypeEntryPtr();
                            }
                            if(typeInfo.typeInfoSupplier){
                                if(typeInfo.containerAccessFactory){
                                    Q_ASSERT(typeInfo.metaType.isValid());
                                    Q_ASSERT(!typeInfo.ownerFunction);
                                    if(polymorphicIdHandlers.isEmpty()){
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType, env, typeId, typeInfo.qtName,
                                                                                                 typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                 typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                 typeInfo.destructor
                                                                                            );
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                                                             typeInfo.interfaceOffsetInfo, env, typeId,
                                                                                                             typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor
                                                                                                        );
                                        }
                                    }else{
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType, polymorphicIdHandlers,
                                                                                                    env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                    creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                    typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                    typeInfo.destructor);
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                                                             polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor);
                                        }
                                    }
                                }else{
                                    if(typeInfo.metaType.isValid()){
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction, env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                          typeInfo.destructor
                                                                                                        );
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType,
                                                                                                     env, typeId, typeInfo.qtName,
                                                                                                     typeInfo.javaName, java_class,
                                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                                     typeInfo.virtualFunctions,
                                                                                                     typeInfo.destructor
                                                                                            );
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                               java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                               typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                               typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                               java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                               typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                               typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }else{
                                        Q_ASSERT(typeInfo.deleter);
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, env, typeId, typeInfo.qtName,
                                                                                     typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }
                                }
                            }else{ // !typeInfo.typeInfoSupplier
                                if(typeInfo.containerAccessFactory){
                                    Q_ASSERT(typeInfo.metaType.isValid());
                                    Q_ASSERT(!typeInfo.ownerFunction);
                                    if(polymorphicIdHandlers.isEmpty()){
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType, env, typeId, typeInfo.qtName,
                                                                                                 typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                 typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                 typeInfo.destructor
                                                                                            );
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                                                             typeInfo.interfaceOffsetInfo, env, typeId,
                                                                                                             typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor
                                                                                                        );
                                        }
                                    }else{
                                        if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType, polymorphicIdHandlers,
                                                                                                    env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                    creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                    typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                    typeInfo.destructor);
                                        }else{
                                            result = ObjectTypeEntryFactory::create(typeInfo.containerAccessFactory, typeInfo.metaType,
                                                                                                             polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                             env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                                             creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                             typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor);
                                        }
                                    }
                                }else{
                                    if(typeInfo.metaType.isValid()){
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction, env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                          typeInfo.destructor
                                                                                                        );
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType,
                                                                                                     env, typeId, typeInfo.qtName,
                                                                                                     typeInfo.javaName, java_class,
                                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                                     typeInfo.virtualFunctions,
                                                                                                     typeInfo.destructor
                                                                                            );
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                               java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                               typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                               typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                               java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                               typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                               typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }else{
                                        Q_ASSERT(typeInfo.deleter);
                                        if(polymorphicIdHandlers.isEmpty()){
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, env, typeId, typeInfo.qtName,
                                                                                     typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }else{
                                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(polymorphicIdHandlers,
                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName, java_class,
                                                                                     creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                     typeInfo.destructor);
                                                }
                                            }else{
                                                if(typeInfo.ownerFunction){
                                                    result = ObjectTypeEntryFactory::create(typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                          env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                          java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                          typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                          typeInfo.destructor);
                                                }else{
                                                    result = ObjectTypeEntryFactory::create(polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                                     env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                                                                     java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                                     typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                                     typeInfo.destructor);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                case EntryTypes::InterfaceTypeInfo:
                {
                    jclass java_impl_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl").arg(typeInfo.javaName)));
                    if(!java_impl_class){
                        JavaException::raiseError(env, QLatin1String("class %1.Impl cannot be found").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        return QtJambiTypeEntryPtr();
                    }
                    jclass java_wrapper_class = nullptr;
                    int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                    if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                        java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(typeInfo.javaName)));
                        if(!java_wrapper_class){
                            JavaException::raiseError(env, QLatin1String("class %1.Impl.ConcreteWrapper cannot be found").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                            return QtJambiTypeEntryPtr();
                        }
                    }else{
                        java_wrapper_class = java_impl_class;
                    }
                    jmethodID creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.Impl.ConcreteWrapper").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }else{
                            JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.Impl").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }
                        return QtJambiTypeEntryPtr();
                    }
                    QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> polymorphicIdHandlers;
                    for(const QExplicitlySharedDataPointer<const PolymorphicIdHandler>& handler : typeInfo.polymorphicIdHandlers){
                        jclass _java_class = JavaAPI::resolveClass(env, getJavaName(handler->m_targetTypeId));
                        if(env->IsAssignableFrom(_java_class, java_class)){
                            polymorphicIdHandlers << handler;
                        }
                    }
                    if(typeInfo.typeInfoSupplier){
                        if(polymorphicIdHandlers.isEmpty()){
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                     typeInfo.ownerFunction, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                     env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                                                     typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                     typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor);
                                }
                            }
                        }else{
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                     typeInfo.ownerFunction, polymorphicIdHandlers, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                     polymorphicIdHandlers, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                                                     typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                     typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, java_impl_class, java_wrapper_class,
                                                                                                     polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor);
                                }
                            }
                        }
                    }else{
                        if(polymorphicIdHandlers.isEmpty()){
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                     typeInfo.ownerFunction, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }else{
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                     env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                                                     typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                     typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                                                     typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor);
                                }
                            }
                        }else{
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                     typeInfo.ownerFunction, polymorphicIdHandlers, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }else{
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                     polymorphicIdHandlers, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor );
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                                                     typeInfo.ownerFunction, polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                     typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(java_impl_class, java_wrapper_class,
                                                                                                     polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                     env, typeId,
                                                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                     typeInfo.deleter, typeInfo.virtualFunctions, typeInfo.destructor);
                                }
                            }
                        }
                    }
                    break;
                }
                case EntryTypes::InterfaceValueTypeInfo:
                {
                    jclass java_impl_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl").arg(typeInfo.javaName)));
                    if(!java_impl_class){
                        JavaException::raiseError(env, QLatin1String("class %1.Impl cannot be found").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        return QtJambiTypeEntryPtr();
                    }
                    jclass java_wrapper_class = nullptr;
                    int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                    if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                        java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(typeInfo.javaName)));
                        if(!java_wrapper_class){
                            JavaException::raiseError(env, QLatin1String("class %1.Impl.ConcreteWrapper cannot be found").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                            return QtJambiTypeEntryPtr();
                        }
                    }else{
                        java_wrapper_class = java_impl_class;
                    }
                    jmethodID creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.Impl.ConcreteWrapper").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }else{
                            JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.Impl").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }
                        return QtJambiTypeEntryPtr();
                    }
                    Q_ASSERT(typeInfo.metaType.isValid());
                    QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> polymorphicIdHandlers;
                    for(const QExplicitlySharedDataPointer<const PolymorphicIdHandler>& handler : typeInfo.polymorphicIdHandlers){
                        jclass _java_class = JavaAPI::resolveClass(env, getJavaName(handler->m_targetTypeId));
                        if(env->IsAssignableFrom(_java_class, java_class)){
                            polymorphicIdHandlers << handler;
                        }
                    }

                    if(typeInfo.typeInfoSupplier){
                        if(polymorphicIdHandlers.isEmpty()){
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                     typeInfo.ownerFunction, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                            typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                            typeInfo.deleter, typeInfo.virtualFunctions,
                                                                     typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                     env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                            typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                            typeInfo.deleter, typeInfo.virtualFunctions,
                                                                     typeInfo.destructor);
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                             env, typeId,
                                                                                                             typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                             typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                                             typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          typeInfo.interfaceOffsetInfo,
                                                                                                             env, typeId,
                                                                                                             typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                             typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor);
                                }
                            }
                        }else{
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                    typeInfo.ownerFunction, polymorphicIdHandlers, env, typeId,
                                                                    typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                    typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                    typeInfo.destructor );
                                }else{
                                result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                    polymorphicIdHandlers, env, typeId,
                                                                    typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                    typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                    typeInfo.destructor);
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          typeInfo.ownerFunction,
                                                                                                          polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                          env, typeId,
                                                                                                          typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                          typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                                          typeInfo.virtualFunctions,
                                                                                                          typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.typeInfoSupplier, typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                          env, typeId,
                                                                                                          typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                          typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                          typeInfo.destructor);
                                }
                            }
                        }
                    }else{
                        if(polymorphicIdHandlers.isEmpty()){
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                     typeInfo.ownerFunction, env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                            typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                            typeInfo.deleter, typeInfo.virtualFunctions,
                                                                     typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                     env, typeId,
                                                                     typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                     typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                            typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                            typeInfo.deleter, typeInfo.virtualFunctions,
                                                                     typeInfo.destructor);
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          typeInfo.ownerFunction, typeInfo.interfaceOffsetInfo,
                                                                                                             env, typeId,
                                                                                                             typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                             typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                                             typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          typeInfo.interfaceOffsetInfo,
                                                                                                             env, typeId,
                                                                                                             typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                             typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                             typeInfo.destructor);
                                }
                            }
                        }else{
                            if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                    typeInfo.ownerFunction, polymorphicIdHandlers, env, typeId,
                                                                    typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                    typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                    typeInfo.destructor );
                                }else{
                                result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                    polymorphicIdHandlers, env, typeId,
                                                                    typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                    typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                    typeInfo.destructor);
                                }
                            }else{
                                if(typeInfo.ownerFunction){
                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          typeInfo.ownerFunction,
                                                                                                          polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                          env, typeId,
                                                                                                          typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                          typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter,
                                                                                                          typeInfo.virtualFunctions,
                                                                                                          typeInfo.destructor);
                                }else{
                                    result = ObjectTypeEntryFactory::create(typeInfo.metaType, java_impl_class, java_wrapper_class,
                                                                                                          polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                                                          env, typeId,
                                                                                                          typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                          typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.deleter, typeInfo.virtualFunctions,
                                                                                                          typeInfo.destructor);
                                }
                            }
                        }
                    }
                    break;
                }
                case EntryTypes::QObjectTypeInfo:
                {
                    int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
                    QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> polymorphicIdHandlers;
                    for(const QExplicitlySharedDataPointer<const PolymorphicIdHandler>& handler : typeInfo.polymorphicIdHandlers){
                        jclass _java_class = JavaAPI::resolveClass(env, getJavaName(handler->m_targetTypeId));
                        if(env->IsAssignableFrom(_java_class, java_class)){
                            polymorphicIdHandlers << handler;
                        }
                    }
                    if(typeInfo.customMetaObject){
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            jclass java_wrapper_class = nullptr;
                            try {
                                java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(typeInfo.javaName)));
                            } catch (const JavaException&) {
                            }
                            jmethodID creator_method = nullptr;
                            if(java_wrapper_class){
                                creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                                if(!creator_method){
                                    JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.ConcreteWrapper").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                                    return QtJambiTypeEntryPtr();
                                }
                            }
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(java_wrapper_class, typeInfo.customMetaObject,
                                                                                                 env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                 typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                 typeInfo.virtualFunctions, typeInfo.originalMetaObject
                                                                                            );
                                }else{
                                    result = QObjectTypeEntryFactory::create(java_wrapper_class, typeInfo.interfaceOffsetInfo, typeInfo.customMetaObject,
                                                                                                       env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                       typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                       typeInfo.virtualFunctions, typeInfo.originalMetaObject
                                                                                                    );
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class, typeInfo.customMetaObject,
                                                                                                                    env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                                    typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                                    typeInfo.virtualFunctions, typeInfo.originalMetaObject
                                                             );
                                }else{
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                                                       typeInfo.customMetaObject,
                                                                                                       env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                       typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                                                       typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }
                            }
                        }else{ // !isAbstract
                            jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                            if(!creator_method){
                                JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                                return QtJambiTypeEntryPtr();
                            }
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(typeInfo.customMetaObject,
                                                                                                 env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                 typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                 typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions, typeInfo.originalMetaObject
                                                             );
                                }else{
                                    result = QObjectTypeEntryFactory::create(typeInfo.interfaceOffsetInfo, typeInfo.customMetaObject,
                                                                                                   env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                   typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                   typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions, typeInfo.originalMetaObject
                                                             );
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers, typeInfo.customMetaObject,
                                                                                                            env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                            typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                            typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }else{
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers, typeInfo.interfaceOffsetInfo, typeInfo.customMetaObject,
                                                                                                              env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                                                              typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                                                              typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }
                            }
                        } // isAbstract
                    }else{ // !typeInfo.customMetaObject
                        if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                            jclass java_wrapper_class = nullptr;
                            try {
                                java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(typeInfo.javaName)));
                            } catch (const JavaException&) {
                            }
                            jmethodID creator_method = nullptr;
                            if(java_wrapper_class){
                                creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                                if(!creator_method){
                                    JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1.ConcreteWrapper").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                                    return QtJambiTypeEntryPtr();
                                }
                            }
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(java_wrapper_class, env, typeId, typeInfo.qtName,
                                                                     typeInfo.javaName, java_class, creator_method,
                                                                      typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                      typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }else{
                                    result = QObjectTypeEntryFactory::create(java_wrapper_class, typeInfo.interfaceOffsetInfo,
                                                                       env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                       typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                       typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class,
                                                                        env, typeId, typeInfo.qtName,
                                                                        typeInfo.javaName, java_class, creator_method,
                                                                        typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                        typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }else{
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers, java_wrapper_class,
                                                                      typeInfo.interfaceOffsetInfo,
                                                                      env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                      typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second,
                                                                      typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }
                            }
                        }else{ // !isAbstract
                            jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                            if(!creator_method){
                                JavaException::raiseError(env, QLatin1String("internal private constructor cannot be found in class %1").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                                return QtJambiTypeEntryPtr();
                            }
                            if(polymorphicIdHandlers.isEmpty()){
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                      typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                      typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions, typeInfo.originalMetaObject
                                                             );
                                }else{
                                    result = QObjectTypeEntryFactory::create(typeInfo.interfaceOffsetInfo,
                                                                       env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method,
                                                                       typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                       typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions, typeInfo.originalMetaObject);
                                }
                            }else{
                                if(typeInfo.interfaceOffsetInfo.offsets.isEmpty()){
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers,
                                                                        env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                        java_class, creator_method,
                                                                        typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                        typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions, typeInfo.originalMetaObject
                                                                    );
                                }else{
                                    result = QObjectTypeEntryFactory::create(polymorphicIdHandlers, typeInfo.interfaceOffsetInfo,
                                                                       env, typeId, typeInfo.qtName, typeInfo.javaName,
                                                                       java_class, creator_method,
                                                                       typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second,
                                                                       typeInfo.shellSizeAndAlignment.first, typeInfo.shellSizeAndAlignment.second, typeInfo.virtualFunctions,
                                                                       typeInfo.originalMetaObject);
                                }
                            }
                        } // isAbstract
                    } // if(typeInfo.customMetaObject)
                    break;
                }
                case EntryTypes::StringTypeInfo:
                {
                    if(typeid_equals(typeId, typeid(QString))){
                        result = new StringTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QStringView))){
                        result = new StringUtilTypeEntry<QStringView>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QAnyStringView))){
                        result = new StringUtilTypeEntry<QAnyStringView>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QUtf8StringView))){
                        result = new StringUtilTypeEntry<QUtf8StringView>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QLatin1String))){
                        result = new StringUtilTypeEntry<QLatin1String>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }
                    break;
                }
                case EntryTypes::PrimitiveTypeInfo:
                {
                    if(typeid_equals(typeId, typeid(double)) || typeid_equals(typeId, typeid(jdouble))){
                        result = new JDoubleTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(float)) || typeid_equals(typeId, typeid(jfloat))){
                        result = new JFloatTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(bool)) || typeid_equals(typeId, typeid(jboolean))){
                        result = new JBooleanTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(wchar_t))
                             || typeid_equals(typeId, typeid(char16_t))
                             || typeid_equals(typeId, typeid(jchar))){
                        result = new JCharTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QChar))){
                        result = new JCharQCharTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QLatin1Char))){
                        result = new JCharQLatin1CharTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(qint32)) || typeid_equals(typeId, typeid(quint32)) || typeid_equals(typeId, typeid(jint))){
                        result = new JIntTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(qint16)) || typeid_equals(typeId, typeid(quint16)) || typeid_equals(typeId, typeid(jshort))){
                        result = new JShortTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(qint64)) || typeid_equals(typeId, typeid(quint64)) || typeid_equals(typeId, typeid(jlong))){
                        result = new JLongTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(qint8)) || typeid_equals(typeId, typeid(quint8)) || typeid_equals(typeId, typeid(jbyte))){
                        result = new JByteTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }
                    break;
                }
                case EntryTypes::SpecialTypeInfo:
                {
                    if(typeid_equals(typeId, typeid(std::nullptr_t))){
                        result = new NullptrTypeEntry(env, typeId, typeInfo.qtName, nullptr, nullptr, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QUrl::FormattingOptions))){
                        jthrowable exceptionOccurred = nullptr;
                        jmethodID creator_method = JavaAPI::resolveMethod(env, "<init>", "(I)V", java_class, false, &exceptionOccurred);
                        if(exceptionOccurred)
                            JavaException(env, exceptionOccurred).raise();
                        Q_ASSERT(creator_method);
                        result = new FlagsTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, creator_method, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second, nullptr);
                    }else if(typeid_equals(typeId, typeid(QMetaObject::Connection))){
                        result = new MetaObjectConnectionTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QMetaObject))){
                        result = new MetaUtilTypeEntry<QMetaObject>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(JCollectionWrapper))
                             || typeid_equals(typeId, typeid(JMapWrapper))
                             || typeid_equals(typeId, typeid(JObjectWrapper))
                             || typeid_equals(typeId, typeid(JIntArrayWrapper))
                             || typeid_equals(typeId, typeid(JLongArrayWrapper))
                             || typeid_equals(typeId, typeid(JShortArrayWrapper))
                             || typeid_equals(typeId, typeid(JByteArrayWrapper))
                             || typeid_equals(typeId, typeid(JBooleanArrayWrapper))
                             || typeid_equals(typeId, typeid(JCharArrayWrapper))
                             || typeid_equals(typeId, typeid(JFloatArrayWrapper))
                             || typeid_equals(typeId, typeid(JDoubleArrayWrapper))){
                        result = new MetaUtilTypeEntry<JObjectWrapper>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(JQObjectWrapper))){
                        result = new MetaUtilTypeEntry<JQObjectWrapper>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(JObjectValueWrapper))){
                        result = new MetaUtilTypeEntry<JObjectValueWrapper>(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QVariant))){
                        result = new QVariantTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }else if(typeid_equals(typeId, typeid(QCborValueRef)) || typeid_equals(typeId, typeid(QCborValueConstRef))){
                        result = new QCborValueRefTypeEntry(env, typeId, typeInfo.qtName, typeInfo.javaName, java_class, typeInfo.valueSizeAndAlignment.first, typeInfo.valueSizeAndAlignment.second);
                    }
                    break;
                }
                default:
                    return QtJambiTypeEntryPtr();
                }
            }else{
                JavaException::raiseError(env, QLatin1String("Java class %1 cannot be found").arg(QString(typeInfo.javaName).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
            }
        }

        {
            QWriteLocker locker(storage->lock());
            if(storage->typeEntries().contains(unique_id(typeId))){
                QMap<QByteArray,QtJambiTypeEntryPtr>& entries = storage->typeEntries()[unique_id(typeId)];
                if(qtName){
                    if(QtJambiTypeEntryPtr altresult = entries.value(qtName)){
                        return altresult;
                    }
                }else if(!entries.isEmpty()){
                    QtJambiTypeEntryPtr e = entries.first();
                    return e;
                }
            }
            storage->typeEntries()[unique_id(typeId)][typeInfo.qtName] = result;
        }
    }
    return result;
}

bool QtJambiTypeEntry::isEnum() const {return false;}
bool QtJambiTypeEntry::isFlags() const {return false;}
bool QtJambiTypeEntry::isFunctional() const {return false;}
bool QtJambiTypeEntry::isShellable() const {return false;}
bool QtJambiTypeEntry::isPolymorphic() const {return false;}
bool QtJambiTypeEntry::isQObject() const {return false;}
bool QtJambiTypeEntry::isObject() const {return false;}

bool EnumTypeEntry::isEnum() const {return true;}
bool FlagsTypeEntry::isFlags() const {return true;}
bool FunctionalTypeEntry::isFunctional() const {return true;}
bool QObjectTypeAbstractEntry::isQObject() const {return true;}
bool ObjectTypeAbstractEntry::isObject() const {return true;}
bool ObjectTypeAbstractEntry::isValue() const {return false;}
bool ObjectTypeAbstractEntry::isInterface() const {return false;}

PtrDeleterFunction ObjectTypeAbstractEntry::deleter() const {return m_deleter;}

QtJambiTypeEntry::NativeToJavaResult::NativeToJavaResult(bool success) : m_success(success) {}
QtJambiTypeEntry::NativeToJavaResult::NativeToJavaResult(const QSharedPointer<QtJambiLink>& link) : /*m_link(link),*/ m_success(link) {}

ShellableTypeEntry::ShellableTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                size_t value_align,
                size_t shell_size,
                size_t shell_align,
                const QVector<RegistryAPI::FunctionInfo>& virtualFunctions
            )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, value_align),
      m_shell_size(shell_size),
      m_shell_align(shell_align),
      m_virtualFunctions(virtualFunctions)
{

}

bool ShellableTypeEntry::isShellable() const { return true; }

size_t ShellableTypeEntry::shellSize() const
{
    return m_shell_size;
}

size_t ShellableTypeEntry::shellAlign() const
{
    return m_shell_align;
}

const QVector<RegistryAPI::FunctionInfo>& ShellableTypeEntry::virtualFunctions() const {
    return m_virtualFunctions;
}

DestructableTypeEntry::DestructableTypeEntry(JNIEnv* env,
                                            const std::type_info& typeId,
                                            const char *qt_name,
                                            const char *java_name,
                                            jclass java_class,
                                            jmethodID creator_method,
                                             size_t value_size,
                                             size_t value_align,
                                             size_t shell_size,
                                             size_t shell_align,
                                            const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
                                            RegistryAPI::DestructorFn destructor
                                        )
    : ShellableTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, value_align, shell_size, shell_align, virtualFunctions),
      m_destructor(destructor)
{
}

RegistryAPI::DestructorFn DestructableTypeEntry::destructor() const
{
    return m_destructor;
}

const FlagsTypeEntry* EnumTypeEntry::flagType() const
{
    return m_flagType;
}

const EnumTypeEntry* FlagsTypeEntry::enumType() const
{
    return m_enumType;
}

const std::type_info& QtJambiTypeEntry::type() const
{
    return m_typeId;
}

size_t QtJambiTypeEntry::valueSize() const
{
    return m_value_size;
}

size_t QtJambiTypeEntry::valueAlign() const
{
    return m_value_align;
}

uint QtJambiTypeEntry::offset(const std::type_info&) const{
    return 0;
}

jclass QtJambiTypeEntry::javaClass() const
{
    return m_java_class;
}

jclass QtJambiTypeEntry::creatableClass() const
{
    return m_java_class;
}

jmethodID QtJambiTypeEntry::creatorMethod() const
{
    return m_creator_method;
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getFittingTypeEntry(JNIEnv *, const void *, qintptr&) const{
    return QtJambiTypeEntryPtr{this};
}

bool QtJambiTypeEntry::convertToNative(JNIEnv *env, jobject input, void * output, QtJambiScope&) const{
    return convertToNative(env, input, output);
}

QtJambiTypeEntry::NativeToJavaResult QtJambiTypeEntry::convertToJava(JNIEnv *env, void *qt_object, jobject& output) const{
    return convertToJava(env, qt_object, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

const char *QtJambiTypeEntry::qtName() const
{
    return m_qt_name;
}

const char *QtJambiTypeEntry::javaName() const
{
    return m_java_name;
}

bool QObjectTypeAbstractEntry::isQThread() const{
    return m_isQThread;
}

const QMetaObject* QObjectTypeAbstractEntry::originalMetaObject() const
{
    return m_original_meta_object;
}

FunctionalResolver FunctionalTypeEntry::registeredFunctionalResolver() const
{
    return m_registered_functional_resolver;
}

jclass FunctionalTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

QtJambiTypeEntry::QtJambiTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size, size_t value_align)
    : m_typeId(typeId),
      m_qt_name(qt_name),
      m_java_name(java_name),
      m_java_class(java_class ? getGlobalClassRef(env, java_class) : nullptr),
      m_creator_method(creator_method),
      m_value_size(value_size),
      m_value_align(value_align)
{
}

QtJambiTypeEntry::~QtJambiTypeEntry()
{
}

jobject QtJambiTypeEntry::convertInvalidModelIndexToJava(JNIEnv *env){
    static JObjectWrapper invalidIndex;
    jobject output = invalidIndex.object(env);
    if(env->IsSameObject(output, nullptr)){
        output = Java::QtCore::QModelIndex::newInstance(env, nullptr);
        invalidIndex = JObjectWrapper(env, output);
    }else{
        output = env->NewLocalRef(output);
    }
    if(Java::QtJambi::QtObjectInterface::isDisposed(env, output)){
        (void)QtJambiLink::createLinkForNativeObject(
                env,
                output,
                new QModelIndex(),
                LINK_NAME_ARG("QModelIndex")
                false,
                false,
                QModelIndexTypeEntry::deleter,
                QtJambiLink::Ownership::Java
                );
    }
    return output;
}

bool enableSingletonInvalid(JNIEnv *env){
    static ResettableBoolFlag value(env, "io.qt.experimental.enable-invalid-modelindex-singleton");
    return value;
}

QtJambiTypeEntry::NativeToJavaResult QtJambiTypeEntry::convertModelIndexNativeToJava(JNIEnv *env, QModelIndex &&index, jobject& output){
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    Q_UNUSED(mode)
    output = Java::QtCore::QModelIndex::newInstance(env, jint(index.row()), jint(index.column()), jlong(index.internalId()), QtJambiAPI::convertQObjectToJavaObject(env, index.model()));
    return true;
#else
    output = Java::QtCore::QModelIndex::newInstance(env, nullptr);
    if(const QAbstractItemModel *model = index.model()){
        return QtJambiLink::createExtendedLinkForObject(
            env,
            output,
            new QModelIndex(std::move(index)),
            LINK_NAME_ARG("QModelIndex")
            false,
            false,
            QModelIndexTypeEntry::deleter,
            model,
            QtJambiLink::Ownership::Java
            );
    }else{
        return QtJambiLink::createLinkForNativeObject(
            env,
            output,
            new QModelIndex(std::move(index)),
            LINK_NAME_ARG("QModelIndex")
            false,
            false,
            QModelIndexTypeEntry::deleter,
            QtJambiLink::Ownership::Java
            );
    }
#endif
}

QtJambiTypeEntry::NativeToJavaResult QtJambiTypeEntry::convertModelIndexNativeToJava(JNIEnv *env, const QModelIndex &index, NativeToJavaConversionMode mode, jobject& output){
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    Q_UNUSED(mode)
    output = Java::QtCore::QModelIndex::newInstance(env, jint(index.row()), jint(index.column()), jlong(index.internalId()), QtJambiAPI::convertQObjectToJavaObject(env, index.model()));
    return true;
#else
    if(enableSingletonInvalid(env) && !index.isValid()){
        output = convertInvalidModelIndexToJava(env);
        return true;
    }else{
        if(mode==NativeToJavaConversionMode::None){
            if(jobject obj = QtJambiLink::findObjectForPointer(env, Java::QtCore::QModelIndex::getClass(env), &index)){
                output = obj;
                return true;
            }
        }
        void *copy = mode==NativeToJavaConversionMode::MakeCopyOfValues ? new QModelIndex(index) : &const_cast<QModelIndex&>(index);
        if (!copy){
            output = nullptr;
            return true;
        }
        output = Java::QtCore::QModelIndex::newInstance(env, nullptr);
        QtJambiLink::Ownership ownership;
        switch(mode){
        case NativeToJavaConversionMode::None: ownership = QtJambiLink::Ownership::None; break;
        case NativeToJavaConversionMode::CppOwnership: ownership = QtJambiLink::Ownership::Cpp; break;
        default: ownership = QtJambiLink::Ownership::Java; break;
        }

        if(const QAbstractItemModel *model = index.model()){
            return QtJambiLink::createExtendedLinkForObject(
                env,
                output,
                copy,
                LINK_NAME_ARG("QModelIndex")
                false,
                false,
                QModelIndexTypeEntry::deleter,
                model,
                ownership
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output,
                copy,
                LINK_NAME_ARG("QModelIndex")
                false,
                false,
                QModelIndexTypeEntry::deleter,
                ownership
                );
        }
    }
#endif
}

QtJambiTypeEntry::NativeToJavaResult QtJambiTypeEntry::convertModelIndexNativeToJava(JNIEnv *env, const QModelIndex &index, NativeToJavaConversionMode mode, jobject& output, QtJambiScope& scope){
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    Q_UNUSED(mode)
    output = Java::QtCore::QModelIndex::newInstance(env, jint(index.row()), jint(index.column()), jlong(index.internalId()), QtJambiAPI::convertQObjectToJavaObject(env, index.model()));
    return true;
#else
    if(enableSingletonInvalid(env) && !index.isValid()){
        output = convertInvalidModelIndexToJava(env);
        return true;
    }else{
        static ResettableBoolFlag enableEphemeralModelIndex(env, "io.qt.experimental.enable-ephemeral-modelindexes");
        if(enableEphemeralModelIndex){
            output = Java::QtCore::QModelIndex::newInstance(env, nullptr);
            if(QtJambiLink::createLinkForNativeObject(
                    env,
                    output,
                    &const_cast<QModelIndex&>(index),
                    LINK_NAME_ARG("QModelIndex")
                    false,
                    false,
                    QtJambiLink::Ownership::Cpp
                    )){
                scope.addObjectInvalidation(env, output, false);
                return true;
            }else{
                return false;
            }
        }
        if(mode==NativeToJavaConversionMode::None){
            if(jobject obj = QtJambiLink::findObjectForPointer(env, Java::QtCore::QModelIndex::getClass(env), &index)){
                output = obj;
                return true;
            }
        }
        void *copy = mode==NativeToJavaConversionMode::MakeCopyOfValues ? new QModelIndex(index) : &const_cast<QModelIndex&>(index);
        if (!copy){
            output = nullptr;
            return true;
        }
        output = Java::QtCore::QModelIndex::newInstance(env, nullptr);
        QtJambiLink::Ownership ownership;
        switch(mode){
        case NativeToJavaConversionMode::None: ownership = QtJambiLink::Ownership::None; break;
        case NativeToJavaConversionMode::CppOwnership: ownership = QtJambiLink::Ownership::Cpp; break;
        default: ownership = QtJambiLink::Ownership::Java; break;
        }

        if(const QAbstractItemModel *model = index.model()){
            return QtJambiLink::createExtendedLinkForObject(
                env,
                output,
                copy,
                LINK_NAME_ARG("QModelIndex")
                false,
                false,
                QModelIndexTypeEntry::deleter,
                model,
                ownership
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output,
                copy,
                LINK_NAME_ARG("QModelIndex")
                false,
                false,
                QModelIndexTypeEntry::deleter,
                ownership
                );
        }
    }
#endif
}

#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
class QAbstractItemViewPrivate{
public:
    static QModelIndex index(const QAbstractItemModel* model,int row, int column, quintptr internalId){
        return model->createIndex(row, column, internalId);
    }
};
#endif

bool QtJambiTypeEntry::convertModelIndexJavaToNative(JNIEnv *env, jobject java_object, void * output){
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    if(Java::QtCore::QModelIndex::isInstanceOf(env, java_object)){
        QAbstractItemModel* model = QtJambiAPI::convertJavaObjectToQObject<QAbstractItemModel>(env, Java::QtCore::QModelIndex::model(env, java_object));
        int row = Java::QtCore::QModelIndex::row(env, java_object);
        int column = Java::QtCore::QModelIndex::column(env, java_object);
        quintptr internalId = Java::QtCore::QModelIndex::internalId(env, java_object);
        QModelIndex index;
        if(model){
            index = QAbstractItemViewPrivate::index(model, row, column, internalId);
        }
        *reinterpret_cast<QModelIndex*>(output) = std::move(index);
        return true;
    }else return env->IsSameObject(java_object, nullptr);
#else
    if(Java::QtCore::QModelIndex::isInstanceOf(env, java_object)){
        *reinterpret_cast<void**>(output) = reinterpret_cast<QModelIndex*>(QtJambiLink::findPointerForJavaObject(env, java_object));
        return true;
    }else if(env->IsSameObject(java_object, nullptr)){
        return true;
    }
    return false;
#endif
}

#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
bool QtJambiTypeEntry::convertModelIndexJavaToNative(JNIEnv *env, jobject java_object, void * output, QtJambiScope& scope){
    if(Java::QtCore::QModelIndex::isInstanceOf(env, java_object)){
        QAbstractItemModel* model = QtJambiAPI::convertJavaObjectToQObject<QAbstractItemModel>(env, Java::QtCore::QModelIndex::model(env, java_object));
        int row = Java::QtCore::QModelIndex::row(env, java_object);
        int column = Java::QtCore::QModelIndex::column(env, java_object);
        quintptr internalId = Java::QtCore::QModelIndex::internalId(env, java_object);
        QModelIndex index;
        if(model){
            index = QAbstractItemViewPrivate::index(model, row, column, internalId);
        }
        QModelIndex* indexPtr = new QModelIndex(std::move(index));
        scope->addDeletion(indexPtr);
        *reinterpret_cast<void**>(output) = indexPtr;
        return true;
    }else return env->IsSameObject(java_object, nullptr);
}
#endif

EnumTypeEntry::EnumTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size, size_t value_align)
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, value_align),
      m_flagType(nullptr)
{

}

FlagsTypeEntry::FlagsTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size, size_t value_align, const EnumTypeEntry* enumType)
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, value_align),
      m_enumType(enumType)
{
    if(enumType)
        const_cast<EnumTypeEntry*>(enumType)->m_flagType = this;
}

PolymorphicTypeEntryInterface::~PolymorphicTypeEntryInterface() = default;

QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const QObject *qt_object, qintptr& offset, const QObjectTypeAbstractEntry* entry, QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> polymorphicHandlers){
    if(qt_object){
        const std::type_info& type = entry->type();
        qintptr _offset = 0;
        const std::type_info* _typeId = tryGetTypeInfo(RegistryAPI::Private::PolymorphicTypeInfoSupplier<QObject>::value, qt_object);
        if(!_typeId){
            QByteArray java_type = QByteArray(getJavaName(type)).replace('/', '.').replace('$', '.');
            if(enabledDanglingPointerCheck()){
                Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Cannot convert dangling pointer %p to object of type %s", qt_object, java_type.constData()) QTJAMBI_STACKTRACEINFO );
            }else{
                qCWarning(DebugAPI::internalCategory, "Trying to convert possible dangling pointer %p to object of type %s", qt_object, java_type.constData());
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                offset = typeEntry->offset(type);
                return typeEntry;
            }
        }
        if(entry->originalMetaObject()){
            const QMetaObject *mo = QtJambiMetaObject::findFirstStaticMetaObject(qt_object->metaObject());
            if(entry->originalMetaObject()!=mo){
                const std::type_info* typeId = &entry->type();
                // Search the hierarchy of classes for a class that has been mapped to Java.
                // Prefer the requested class if no other can be found.
                // Only return objects of subclasses of the requested class
                // If the requested class is not in the object's hierarchy, then we prefer
                // the requested class (this means it's basically a proper subclass of the
                // requested class, and thus probably the concrete wrapper, but atleast a
                // more specific version than anything we can find)
                while (mo) {
                    const std::type_info* _typeId = getTypeByMetaObject(mo);
                    if(_typeId){
                        if(*typeId==*_typeId){
                            // Never go further down the hierarchy than the requested class
                            break;
                        }else{
                            typeId = _typeId;
                            break;
                        }
                    }
                    mo = mo->superClass();
                }
                if(typeid_not_equals(*typeId, entry->type())){
                    _typeId = typeId;
                }
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                if(typeEntry->isPolymorphic()){
                    if(const PolymorphicTypeEntryInterface* pt = dynamic_cast<const PolymorphicTypeEntryInterface*>(typeEntry.data())){
                        polymorphicHandlers = QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>(pt->polymorphicIdHandlers()) << polymorphicHandlers;
                    }
                }
            }
        }
        while(!polymorphicHandlers.isEmpty()){
            QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> _polymorphicHandlers;
            polymorphicHandlers.swap(_polymorphicHandlers);
            void *_object = const_cast<QObject *>(qt_object);
            for(const QExplicitlySharedDataPointer<const PolymorphicIdHandler>& handler : qAsConst(_polymorphicHandlers)){
                Q_ASSERT(handler->m_polymorphyHandler);
                _offset = 0;
                if(handler->m_polymorphyHandler(_object, _offset)) {
                    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, handler->m_targetTypeId)){
                        _typeId = &handler->m_targetTypeId;
                        offset += _offset;
                        if(typeEntry->isPolymorphic()){
                            if(const PolymorphicTypeEntryInterface* pt = dynamic_cast<const PolymorphicTypeEntryInterface*>(typeEntry.data())){
                                polymorphicHandlers = QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>(pt->polymorphicIdHandlers()) << polymorphicHandlers;
                            }
                        }
                        break;
                    }
                }
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                return typeEntry;
            }
        }
    }
    return QtJambiTypeEntryPtr{entry};
}

template<typename ...Args>
QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset, const QtJambiTypeEntry* entry, QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> polymorphicHandlers, Args... typeInfoSupplier){
    if(qt_object){
        const std::type_info& type = entry->type();
        qintptr _offset = 0;
        const std::type_info* _typeId = nullptr;
        if constexpr(sizeof...(typeInfoSupplier)==1){
            _typeId = tryGetTypeInfo(typeInfoSupplier..., qt_object);
            if(!_typeId){
                QByteArray java_type = QByteArray(getJavaName(type)).replace('/', '.').replace('$', '.');
                if(enabledDanglingPointerCheck()){
                    Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Cannot convert dangling pointer %p to object of type %s", qt_object, java_type.constData()) QTJAMBI_STACKTRACEINFO );
                }else{
                    qCWarning(DebugAPI::internalCategory, "Trying to convert possible dangling pointer %p to object of type %s", qt_object, java_type.constData());
                }
            }
            if(_typeId && typeid_not_equals(*_typeId, type)){
                if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                    offset = typeEntry->offset(type);
                    return typeEntry;
                }
            }
        }
        while(!polymorphicHandlers.isEmpty()){
            QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> _polymorphicHandlers;
            polymorphicHandlers.swap(_polymorphicHandlers);
            void *_object = const_cast<void *>(qt_object);
            for(const QExplicitlySharedDataPointer<const PolymorphicIdHandler>& handler : qAsConst(_polymorphicHandlers)){
                Q_ASSERT(handler->m_polymorphyHandler);
                _offset = 0;
                if(handler->m_polymorphyHandler(_object, _offset)) {
                    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, handler->m_targetTypeId)){
                        _typeId = &handler->m_targetTypeId;
                        offset += _offset;
                        if(typeEntry->isPolymorphic()){
                            if(const PolymorphicTypeEntryInterface* pt = dynamic_cast<const PolymorphicTypeEntryInterface*>(typeEntry.data())){
                                polymorphicHandlers = QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>>(pt->polymorphicIdHandlers()) << polymorphicHandlers;
                            }
                        }
                        break;
                    }
                }
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                if(typeEntry->isQObject()){
                    if(const QObjectTypeAbstractEntry* pt = dynamic_cast<const QObjectTypeAbstractEntry*>(typeEntry.data())){
                        Q_UNUSED(pt)
                        const void* ptr = qt_object;
                        if(_offset!=0){
                            ptr = reinterpret_cast<const char*>(ptr)-_offset;
                        }
                        qintptr __offset = 0;
                        QtJambiTypeEntryPtr _typeEntry = typeEntry->getFittingTypeEntry(env, ptr, __offset);
                        if(_typeEntry!=typeEntry){
                            offset += __offset;
                            typeEntry = _typeEntry;
                        }
                    }
                }
                return typeEntry;
            }
        }
    }
    return QtJambiTypeEntryPtr{entry};
}

template<typename Super>
QtJambiTypeEntryPtr PolymorphicTypeEntry<Super>::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    if constexpr(std::is_base_of<QObjectTypeAbstractEntry,Super>::value){
        return ::getFittingTypeEntry(env, reinterpret_cast<const QObject*>(qt_object), offset, this, m_polymorphicIdHandlers);
    }else if constexpr(supports_typeInfoSupplier<Super>::value){
        return ::getFittingTypeEntry(env, qt_object, offset, this, m_polymorphicIdHandlers, Super::typeInfoSupplier());
    }else{
        return ::getFittingTypeEntry(env, qt_object, offset, this, m_polymorphicIdHandlers);
    }
}

template<typename Super>
QtJambiTypeEntryPtr VirtualTypeEntry<Super>::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    return ::getFittingTypeEntry(env, qt_object, offset, this, {}, this->typeInfoSupplier());
}

FunctionalTypeEntry::~FunctionalTypeEntry()
{
}

FunctionalTypeEntry::FunctionalTypeEntry(JNIEnv* env, const std::type_info& typeId,
                                         const char *qt_name,
                                         const char *java_name,
                                         jclass java_class,
                                         jmethodID creator_method,
                                         size_t value_size, size_t value_align,
                                         size_t shell_size, size_t shell_align,
                                         jclass java_impl_class,
                                         jclass java_wrapper_class,
                                         const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
                                         RegistryAPI::DestructorFn destructor,
                                         FunctionalResolver registered_functional_resolver,
                                         const QMetaType& qt_meta_type
                )
    : DestructableTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, value_align, shell_size, shell_align, virtualFunctions, destructor),
      m_java_impl_class(java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr),
      m_registered_functional_resolver(registered_functional_resolver),
      m_qt_meta_type(qt_meta_type)
{

}

ObjectTypeAbstractEntry::ObjectTypeAbstractEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size, size_t value_align,
                size_t shell_size, size_t shell_align,
                PtrDeleterFunction deleter,
                const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
                RegistryAPI::DestructorFn destructor
            )
    : DestructableTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, value_align, shell_size, shell_align, virtualFunctions, destructor),
      m_deleter(deleter)
{

}

template<typename T>
QSharedPointer<T> convertSmartPointer(const QSharedPointer<char>& smartPointer, qintptr offset){
    char* _ptr = smartPointer.get();
    if(offset!=0)
        _ptr -= offset;
    return QtSharedPointer::copyAndSetPointer(reinterpret_cast<T*>(_ptr), smartPointer);
}

template<>
QSharedPointer<char> convertSmartPointer(const QSharedPointer<char>& smartPointer, qintptr offset){
    if(offset!=0){
        char* _ptr = smartPointer.get()-offset;
        return QtSharedPointer::copyAndSetPointer(_ptr, smartPointer);
    }
    return smartPointer;
}

template<typename T>
std::shared_ptr<T> convertSmartPointer(const std::shared_ptr<char>& smartPointer, qintptr offset){
    char* _ptr = smartPointer.get();
    if(offset!=0)
        _ptr -= offset;
    return std::shared_ptr<T>(smartPointer, reinterpret_cast<T*>(_ptr));
}

template<>
std::shared_ptr<char> convertSmartPointer(const std::shared_ptr<char>& smartPointer, qintptr offset){
    if(offset!=0){
        char* _ptr = smartPointer.get()-offset;
        return std::shared_ptr<char>(smartPointer, _ptr);
    }
    return smartPointer;
}

QObjectTypeAbstractEntry::QObjectTypeAbstractEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size, size_t value_align,
                 size_t shell_size, size_t shell_align,
                 const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
                 const QMetaObject* original_meta_object
            )
    : ShellableTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, value_align, shell_size, shell_align, virtualFunctions),
      m_original_meta_object(original_meta_object),
      m_isQThread(Java::QtCore::QThread::isAssignableFrom(env, java_class))
{
}

QtJambiTypeEntry::NativeToJavaResult ObjectTypeAbstractEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jobject& output) const{
    if (!qt_object){
        output = nullptr;
        return true;
    }
    bool makeCopyOfValues = false;
    switch(!isValue() && mode==NativeToJavaConversionMode::MakeCopyOfValues ? NativeToJavaConversionMode::None : mode){
    case NativeToJavaConversionMode::None:
        if(jobject obj = QtJambiLink::findObjectForPointer(env, javaClass(), qt_object)){
            output = obj;
            return true;
        }
        break;
    case NativeToJavaConversionMode::MakeCopyOfValues:
        makeCopyOfValues = true;
        break;
    default: break;
    }
    CopyValueInfo value = copyValue(env, qt_object, makeCopyOfValues);
    if (!value.value){
        output = nullptr;
        return true;
    }
    output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return createLinkForNativeObject(
        env,
        output,
        value.value,
        mode,
        value.isCopy,
        value.containerAccess);
}

template<template<typename> class SmartPointer>
bool ObjectTypeAbstractEntry::convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jobject& output) const{
    void* qt_object = smartPointer.get();
    if (!qt_object){
        output = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output = obj;
                if(!link->isSmartPointer()){
                    bool createdByJava = link->createdByJava();
                    bool is_shell = link->isShell();
                    QtJambiShellImpl* shell = nullptr;
                    if(is_shell){
                        const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output));
                        Q_ASSERT(infos.size());
                        shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                    }
                    link->reset(env);
                    PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                    if(!link->isQObject())
                        registeredThreadAffinityFunction = link->ownerFunction();
                    link->invalidate(env);
                    link.clear();
                    link = createLinkForSmartPointerToObject(env, output, createdByJava, is_shell, smartPointer, registeredThreadAffinityFunction);
                    link->setJavaOwnership(env);
                    if(shell){
                        shell->overrideLink(link);
                    }
                    return true;
                }else{
                    switch(link->smartPointerType()){
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                        return true;
                    default:
                        return false;
                    }
                }
            }else return false;
        }
    }
    output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QSharedPointer<QtJambiLink> link = createLinkForSmartPointerToObject(env, output, false, false, smartPointer);
    link->setJavaOwnership(env);
    return true;
}

bool ObjectTypeAbstractEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<QSharedPointer>(env, offset==0 ? smartPointer : convertSmartPointer<char>(smartPointer, offset), output);
}

bool ObjectTypeAbstractEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<std::shared_ptr>(env, offset==0 ? smartPointer : convertSmartPointer<char>(smartPointer, offset), output);
}

bool ObjectTypeAbstractEntry::convertToNative(JNIEnv *env, jobject input, void * output) const{
    if(env->IsInstanceOf(input, this->javaClass())){
        *reinterpret_cast<void**>(output) = QtJambiLink::findPointerForJavaObject(env, input);
        return true;
    }else return false;
}

QtJambiTypeEntryPtr QObjectTypeAbstractEntry::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    return ::getFittingTypeEntry(env, reinterpret_cast<const QObject*>(qt_object), offset, this, {});
}

QtJambiTypeEntry::NativeToJavaResult QObjectTypeAbstractEntry::convertToJava(JNIEnv *env, const void *ptr, NativeToJavaConversionMode mode, jobject& output) const{
    QObject* qt_object = reinterpret_cast<QObject*>( const_cast<void*>(ptr) );
    if (!qt_object){
        output = nullptr;
        return true;
    }
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object)){
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(!link->createdByJava()){
            QtJambiLinkObjectData *p = nullptr;
            {
                auto locker = QtJambiObjectData::readLock();
                p = QtJambiObjectData::userData<QtJambiLinkObjectData>(qt_object);
            }
            if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                {
                    auto locker = QtJambiObjectData::writeLock();
                    p = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(qt_object, nullptr);
                }
                if(p)
                    delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                link->reset(env);
                link.clear();
            }
        }
        if(link){
            output = link->getJavaObjectLocalRef(env);
            if(!output && (link->ownership()==QtJambiLink::Ownership::Split || link->isCleanedUp())){
                {
                    bool isInvalidated = false;
                    {
                        QtJambiLinkObjectData *p = nullptr;
                        {
                            auto locker = QtJambiObjectData::readLock();
                            p = QtJambiObjectData::userData<QtJambiLinkObjectData>(qt_object);
                        }
                        if(p){
                            {
                                auto locker = QtJambiObjectData::writeLock();
                                p = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(qt_object, nullptr);
                            }
                            if(p)
                                delete p;
                            isInvalidated = true;
                        }
                    }
                    if(!isInvalidated)
                        link->invalidate(env);
                    link.clear();
                }
            }else return true;
        }
    }
    output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QSharedPointer<QtJambiLink> link = createLinkForNativeQObject(env, output, qt_object);
    if(!m_isQThread && mode==NativeToJavaConversionMode::CppOwnership)
        link->setCppOwnership(env);
    return link;
}

template<template<typename> class SmartPointer>
bool QObjectTypeAbstractEntry::convertSmartPointerToJava(JNIEnv *env, const SmartPointer<QObject>& smartPointer, jobject& output) const{
    QObject* qt_object = smartPointer.get();
    if (!qt_object){
        output = nullptr;
        return true;
    }
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object)){
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(!link->createdByJava()){
            QtJambiLinkObjectData *p{nullptr};
            {
                auto locker = QtJambiObjectData::readLock();
                p = QtJambiObjectData::userData<QtJambiLinkObjectData>(qt_object);
            }
            if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                {
                    auto locker = QtJambiObjectData::writeLock();
                    p = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(qt_object, nullptr);
                }
                if(p)
                    delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                link->reset(env);
                link.clear();
            }
        }
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output = obj;
                if(!link->isSmartPointer()){
                    bool createdByJava = link->createdByJava();
                    bool is_shell = link->isShell();
                    QtJambiShellImpl* shell = nullptr;
                    if(is_shell){
                        const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output));
                        Q_ASSERT(infos.size());
                        shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                    }
                    link->reset(env);
                    link->invalidate(env);
                    link.clear();
                    link = createLinkForSmartPointerToQObject(env, output, createdByJava, is_shell, smartPointer);
                    link->setJavaOwnership(env);
                    if(shell){
                        shell->overrideLink(link);
                    }
                    return true;
                }else{
                    switch(link->smartPointerType()){
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                        return true;
                    default:
                        return false;
                    }
                }
            }else return false;
        }
        if(link){
            output = link->getJavaObjectLocalRef(env);
            if(!output && link->ownership()==QtJambiLink::Ownership::Split){
                {
                    bool isInvalidated = false;
                    {
                        QtJambiLinkObjectData *p;
                        {
                            auto locker = QtJambiObjectData::readLock();
                            p = QtJambiObjectData::userData<QtJambiLinkObjectData>(qt_object);
                        }
                        if(p){
                            {
                                auto locker = QtJambiObjectData::writeLock();
                                p = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(qt_object, nullptr);
                            }
                            isInvalidated = true;
                            if(p)
                                delete p;
                        }
                    }
                    if(!isInvalidated)
                        link->invalidate(env);
                    link.clear();
                }
            }else return true;
        }
    }
    output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QSharedPointer<QtJambiLink> link = createLinkForSmartPointerToQObject(env, output, false, false, smartPointer);
    link->setJavaOwnership(env);
    return true;
}

bool QObjectTypeAbstractEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<QSharedPointer>(env, convertSmartPointer<QObject>(smartPointer, offset), output);
}

bool QObjectTypeAbstractEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<std::shared_ptr>(env, convertSmartPointer<QObject>(smartPointer, offset), output);
}

bool QObjectTypeAbstractEntry::convertToNative(JNIEnv *env, jobject input, void * output) const{
    if(env->IsInstanceOf(input, this->javaClass())){
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, input)){
            Q_ASSERT(link->isQObject());
            *reinterpret_cast<QObject**>(output) = link->qobject();
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, input))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, input).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        return true;
    }else return false;
}

QtJambiTypeEntry::NativeToJavaResult StdFunctionalTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    if(m_registered_functional_resolver){
        bool ok{false};
        output = m_registered_functional_resolver(env, qt_object, &ok);
        if(ok || output)
            return true;
    }
    if(!qt_object){
        output = nullptr;
        return true;
    }
    output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return QtJambiLink::createLinkForNativeObject(
            env,
            output,
            m_qt_meta_type.create(qt_object),
            m_qt_meta_type,
            false,
            false,
            QtJambiLink::Ownership::Java
        );
}

QtJambiTypeEntry::NativeToJavaResult StdFunctionalTypeEntry::convertToJava(JNIEnv *env, void *qt_object, jobject& output) const{
    if(!m_qt_meta_type.isMoveConstructible()){
        return convertToJava(env, qt_object, NativeToJavaConversionMode::MakeCopyOfValues, output);
    }
    if(m_registered_functional_resolver){
        bool ok{false};
        output = m_registered_functional_resolver(env, qt_object, &ok);
        if(ok || output)
            return true;
    }
    if(!qt_object){
        output = nullptr;
        return true;
    }
    output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    auto iface = QMetaType(m_qt_meta_type).iface();
    void* newPointer;
    if (iface->alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
        newPointer = operator new(iface->size, std::align_val_t(iface->alignment));
    else
        newPointer = operator new(iface->size);
    iface->moveCtr(iface, newPointer, qt_object);
    return QtJambiLink::createLinkForNativeObject(
            env,
            output,
            newPointer,
            m_qt_meta_type,
            false,
            false,
            QtJambiLink::Ownership::Java
        );
}

bool StdFunctionalTypeEntry::convertToNative(JNIEnv *env, jobject input, void * output) const{
    if(!env->IsInstanceOf(input, javaClass()))
        return false;
    if(input
        && (QtJambiAPI::getObjectClassName(env, input).endsWith("$ConcreteWrapper")
            || env->IsSameObject(env->GetObjectClass(input), creatableClass()))){
        QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, input);
        if(link){
            m_qt_meta_type.destruct(output);
            m_qt_meta_type.construct(output, link->pointer());
        }else if(input){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QString(QLatin1String(this->javaName())).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, input)){
            if(!link->isMultiInheritanceType() && env->IsInstanceOf(input, javaClass())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->pointer())){
                    functionalBase->getFunctional(env, output);
                }
            }else if(link->isInterfaceAvailable(type())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->typedPointer(type()))){
                    functionalBase->getFunctional(env, output);
                }
            }
        }else if(input){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QString(QLatin1String(this->javaName())).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
        }
    }
    return true;
}

QtJambiTypeEntry::NativeToJavaResult FunctionPointerTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    if(m_registered_functional_resolver){
        bool ok{false};
        output = m_registered_functional_resolver(env, &qt_object, &ok);
        if(ok || output)
            return true;
    }
    if(!qt_object){
        output = nullptr;
        return true;
    }
    output = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return QtJambiLink::createLinkForNativeObject(
            env,
            output,
            m_qt_meta_type.create(&qt_object),
            m_qt_meta_type,
            false,
            false,
            QtJambiLink::Ownership::Java
        );
}

bool FunctionPointerTypeEntry::convertToNative(JNIEnv *env, jobject input, void * output) const{
    if(!env->IsInstanceOf(input, javaClass()))
        return false;
    if(input
        && (QtJambiAPI::getObjectClassName(env, input).endsWith("$ConcreteWrapper")
            || env->IsSameObject(env->GetObjectClass(input), creatableClass()))){
        QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, input);
        if(link){
            *reinterpret_cast<void**>(output) = *reinterpret_cast<void**>(link->pointer());
        }else if(input){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QString(QLatin1String(this->javaName())).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, input)){
            if(!link->isMultiInheritanceType() && env->IsInstanceOf(input, javaClass())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->pointer())){
                    functionalBase->getFunctional(env, output);
                }
            }else if(link->isInterfaceAvailable(type())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->typedPointer(type()))){
                    functionalBase->getFunctional(env, output);
                }
            }
        }else if(input){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QString(QLatin1String(this->javaName())).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!*reinterpret_cast<void**>(output) && input){
        QString funTypeName = QtJambiAPI::typeName(type());
        Java::Runtime::ClassCastException::throwNew(env, QStringLiteral("Unable to convert java object of type '%1' to function pointer '%2'.").arg(QString(QLatin1String(this->javaName())).replace('/', '.').replace('$', '.'), funTypeName) QTJAMBI_STACKTRACEINFO );
    }
    return true;
}

bool FunctionalTypeEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

bool FunctionalTypeEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

QtJambiTypeEntry::NativeToJavaResult EnumTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    switch ( this->valueSize() ) {
    case 1:  output = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint8*>(qt_object)); break;
    case 2:  output = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint16*>(qt_object)); break;
    case 4:  output = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint32*>(qt_object)); break;
    default: output = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint64*>(qt_object)); break;
    }
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return true;
}

bool EnumTypeEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

bool EnumTypeEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

bool EnumTypeEntry::convertToNative(JNIEnv *env, jobject input, void * output) const{
    switch ( this->valueSize() ) {
    case 1:
        {
            qint8* value = reinterpret_cast<qint8*>(output);
            try{
                *value = !input ? 0 : Java::QtJambi::QtByteEnumerator::value(env, input);
            }catch(...){
                if(Java::Runtime::Number::isInstanceOf(env,input)){
                    *value = Java::Runtime::Number::byteValue(env, input);
                }else if(env->IsSameObject(nullptr, input)){
                    *value = 0;
                }else {
                    return false;
                }
            }
        }
        break;
    case 2:
        {
            qint16* value = reinterpret_cast<qint16*>(output);
            try{
                *value = !input ? 0 : Java::QtJambi::QtShortEnumerator::value(env, input);
            }catch(...){
                if(Java::Runtime::Number::isInstanceOf(env,input)){
                    *value = Java::Runtime::Number::shortValue(env, input);
                }else if(env->IsSameObject(nullptr, input)){
                    *value = 0;
                }else {
                    return false;
                }
            }
        }
        break;
    case 8:
        {
            qint64* value = reinterpret_cast<qint64*>(output);
            try{
                *value = !input ? 0 : Java::QtJambi::QtLongEnumerator::value(env, input);
            }catch(...){
                if(Java::Runtime::Number::isInstanceOf(env,input)){
                    *value = Java::Runtime::Number::longValue(env, input);
                }else if(env->IsSameObject(nullptr, input)){
                    *value = 0;
                }else {
                    return false;
                }
            }
        }
        break;
    default:
        {
            qint32* value = reinterpret_cast<qint32*>(output);
            try{
                *value = !input ? 0 : Java::QtJambi::QtEnumerator::value(env, input);
            }catch(...){
                if(Java::Runtime::Number::isInstanceOf(env,input)){
                    *value = Java::Runtime::Number::intValue(env, input);
                }else if(env->IsSameObject(nullptr, input)){
                    *value = 0;
                }else {
                    return false;
                }
            }
        }
        break;
    }
    return true;
}

QtJambiTypeEntry::NativeToJavaResult FlagsTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    if(this->valueSize()==8){
        output = env->NewObject(creatableClass(), creatorMethod(), static_cast<jlong>(*reinterpret_cast<const jlong*>(qt_object)));
    }else{
        output = env->NewObject(creatableClass(), creatorMethod(), static_cast<int>(*reinterpret_cast<const int*>(qt_object)));
    }
    return true;
}

bool FlagsTypeEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

bool FlagsTypeEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

bool FlagsTypeEntry::convertToNative(JNIEnv *env, jobject input, void * output) const{
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    if(this->valueSize()==8){
        jlong* value = reinterpret_cast<jlong*>(output);
        if(value){
            try{
                if(Java::QtJambi::QLongFlags::isInstanceOf(env, input))
                    *value = Java::QtJambi::QLongFlags::longValue(env, input);
                else if(Java::QtJambi::QFlags::isInstanceOf(env, input))
                    *value = Java::QtJambi::QFlags::intValue(env, input);
                else if(env->IsSameObject(nullptr, input))
                    *value = 0;
                else
                    return false;
            }catch(...){
                if(Java::Runtime::Number::isInstanceOf(env,input))
                    *value = Java::Runtime::Number::longValue(env, input);
                else if(env->IsSameObject(nullptr, input))
                    *value = 0;
                else
                    return false;
            }
        }
    }else
#endif
    {
        int* value = reinterpret_cast<int*>(output);
        if(value){
            try{
                *value = !input ? 0 : Java::QtJambi::QFlags::intValue(env, input);
            }catch(...){
                if(Java::Runtime::Number::isInstanceOf(env,input))
                    *value = Java::Runtime::Number::intValue(env, input);
                else if(env->IsSameObject(nullptr, input))
                    *value = 0;
                else
                    return false;
            }
        }
    }
    return true;
}

AbstractSimpleTypeEntry::AbstractSimpleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size, size_t value_align)
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, nullptr, value_size, value_align)
{
}

bool AbstractSimpleTypeEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

bool AbstractSimpleTypeEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    void* _ptr = smartPointer.get();
    if (!_ptr){
        output = nullptr;
        return true;
    }
    if(offset!=0)
        _ptr = reinterpret_cast<char*>(_ptr)-offset;
    return convertToJava(env, _ptr, NativeToJavaConversionMode::MakeCopyOfValues, output);
}

QtJambiTypeEntry::NativeToJavaResult StringTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    const QString* strg = reinterpret_cast<const QString*>(qt_object);
    Q_ASSERT(strg->length()>=0);
    output = env->NewString(reinterpret_cast<const jchar *>(strg->constData()), jsize(strg->length()));
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return true;
}

template<template<typename> class SmartPointer>
bool StringTypeEntry::convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jobject& output) const{
    void* qt_object = smartPointer.get();
    if (!qt_object){
        output = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && Java::QtCore::QString::isInstanceOf(env, obj)){
                output = obj;
                if(!link->isSmartPointer()){
                    bool createdByJava = link->createdByJava();
                    bool is_shell = link->isShell();
                    QtJambiShellImpl* shell = nullptr;
                    if(is_shell){
                        const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output));
                        Q_ASSERT(infos.size());
                        shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                    }
                    link->reset(env);
                    PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                    if(!link->isQObject())
                        registeredThreadAffinityFunction = link->ownerFunction();
                    link->invalidate(env);
                    link.clear();
                    if(registeredThreadAffinityFunction){
                        link = QtJambiLink::createLinkForSmartPointerToObject(env, output,
                                                                                   LINK_NAME_ARG(qtName())
                                                                                   createdByJava,
                                                                                   is_shell,
                                                                                   registeredThreadAffinityFunction,
                                                                                   smartPointer);
                    }else{
                        link = QtJambiLink::createLinkForSmartPointerToObject(env, output,
                                                                                   LINK_NAME_ARG(qtName())
                                                                                   createdByJava,
                                                                                   is_shell,
                                                                                   smartPointer);
                    }
                    link->setJavaOwnership(env);
                    if(shell){
                        shell->overrideLink(link);
                    }
                    return true;
                }else{
                    switch(link->smartPointerType()){
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                        return true;
                    default:
                        return false;
                    }
                }
            }else return false;
        }
    }
    output = Java::QtCore::QString::newInstance(env, nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(
        env,
        output,
        LINK_NAME_ARG(qtName())
        false,
        false,
        smartPointer
        );
    link->setJavaOwnership(env);
    return true;
}

bool StringTypeEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<QSharedPointer>(env, offset==0 ? smartPointer : convertSmartPointer<char>(smartPointer, offset), output);
}

bool StringTypeEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<std::shared_ptr>(env, offset==0 ? smartPointer : convertSmartPointer<char>(smartPointer, offset), output);
}

bool StringTypeEntry::convertToNative(JNIEnv *env, jobject input, void * output) const{
    QString* value = reinterpret_cast<QString*>(output);
    if(value){
        if(Java::QtCore::QString::isInstanceOf(env, input)){
            QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, input);
            QtJambiAPI::checkNullPointer(env, strg);
            *value = *strg;
        }else{
            jstring in = QtJambiAPI::toJavaString(env, input);
            int length = in ? env->GetStringLength(in) : 0;
            value->resize(length);
            if(length>0)
                env->GetStringRegion(in, 0, length, reinterpret_cast<ushort*>(value->data()));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    return true;
}

bool StringTypeEntry::convertToNative(JNIEnv *env, jobject input, void * output, QtJambiScope& scope) const{
    QString* value = reinterpret_cast<QString*>(output);
    if(value){
        if(Java::QtCore::QString::isInstanceOf(env, input)){
            QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, input);
            QtJambiAPI::checkNullPointer(env, strg);
            *value = *strg;
        }else{
            PersistentJString2QChars* buffer = new PersistentJString2QChars(env, static_cast<jstring>(input));
            scope.addDeletion(buffer);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            *value = buffer->toString();
        }
    }
    return true;
}

QtJambiTypeEntry::NativeToJavaResult QCborValueRefTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    const QCborValueConstRef* vref = reinterpret_cast<const QCborValueConstRef*>(qt_object);
    QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(QCborValue), "QCborValue");
    Q_ASSERT(typeEntry);
    QCborValue value = *vref;
    return typeEntry->convertToJava(env, &value, output);
}

bool QCborValueRefTypeEntry::convertToNative(JNIEnv *env, jobject, void *) const{
    JavaException::raiseError(env, "Cannot convert to QCborValueRef" QTJAMBI_STACKTRACEINFO );
    return false;
}

template<typename StringType>
QtJambiTypeEntry::NativeToJavaResult StringUtilTypeEntry<StringType>::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    if constexpr(std::is_same<StringType,QStringView>::value){
        const QStringView* sref = reinterpret_cast<const QStringView*>(qt_object);
        if(sref){
            Q_ASSERT(sref->length()>=0);
            output = env->NewString(reinterpret_cast<const jchar *>(sref->data()), jsize(sref->length()));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }else if constexpr(std::is_same<StringType,QAnyStringView>::value){
        const QAnyStringView* sref = reinterpret_cast<const QAnyStringView*>(qt_object);
        if(sref){
            output = QtJambiAPI::convertNativeToJavaObject(env, *sref);
        }
    }else if constexpr(std::is_same<StringType,QUtf8StringView>::value){
        const QUtf8StringView* sref = reinterpret_cast<const QUtf8StringView*>(qt_object);
        if(sref){
            output = qtjambi_cast<jstring>(env, *sref);
        }
    }else if constexpr(std::is_same<StringType,QLatin1String>::value){
        const QLatin1String* sref = reinterpret_cast<const QLatin1String*>(qt_object);
        if(sref){
            output = env->NewStringUTF(sref->data());
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }else{
        Q_UNUSED(env)
        Q_UNUSED(qt_object)
        Q_UNUSED(output)
    }
    return true;
}

template<typename StringType>
bool StringUtilTypeEntry<StringType>::convertToNative(JNIEnv *env, jobject, void *) const{
    if constexpr(std::is_same<StringType,QStringView>::value){
        JavaException::raiseError(env, "Cannot convert to QStringView" QTJAMBI_STACKTRACEINFO );
    }else if constexpr(std::is_same<StringType,QAnyStringView>::value){
        JavaException::raiseError(env, "Cannot convert to QAnyStringView" QTJAMBI_STACKTRACEINFO );
    }else if constexpr(std::is_same<StringType,QUtf8StringView>::value){
        JavaException::raiseError(env, "Cannot convert to QUtf8StringView" QTJAMBI_STACKTRACEINFO );
    }else if constexpr(std::is_same<StringType,QLatin1String>::value){
        JavaException::raiseError(env, "Cannot convert to QLatin1String" QTJAMBI_STACKTRACEINFO );
    }else{
        Q_UNUSED(env)
        return false;
    }
}

template<typename StringType>
bool StringUtilTypeEntry<StringType>::convertToNative(JNIEnv *env, jobject input, void * output, QtJambiScope& scope) const{
    if constexpr(std::is_same<StringType,QStringView>::value){
        QStringView* value = reinterpret_cast<QStringView*>(output);
        if(value){
            if(Java::QtCore::QString::isInstanceOf(env, input)){
                QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, input);
                QtJambiAPI::checkNullPointer(env, strg);
                *value = *strg;
            }else{
                PersistentJString2QChars* buffer = new PersistentJString2QChars(env, static_cast<jstring>(input));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                scope.addDeletion(buffer);
                *value = buffer->toStringView();
            }
        }
        return true;
    }else if constexpr(std::is_same<StringType,QAnyStringView>::value){
        QAnyStringView* value = reinterpret_cast<QAnyStringView*>(output);
        if(value){
            if(Java::QtCore::QString::isInstanceOf(env, input)){
                QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, input);
                QtJambiAPI::checkNullPointer(env, strg);
                *value = *strg;
            }else{
                PersistentJString2QChars* buffer = new PersistentJString2QChars(env, static_cast<jstring>(input));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                scope.addDeletion(buffer);
                *value = buffer->toAnyStringView();
            }
        }
        return true;
    }else if constexpr(std::is_same<StringType,QUtf8StringView>::value){
        QUtf8StringView* value = reinterpret_cast<QUtf8StringView*>(output);
        if(value){
            if(Java::QtCore::QString::isInstanceOf(env, input)){
                QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, input);
                QtJambiAPI::checkNullPointer(env, strg);
                QByteArray* ba = new QByteArray(strg->toUtf8());
                scope.addDeletion(ba);
                *value = *ba;
            }else{
                PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, static_cast<jstring>(input));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                scope.addDeletion(buffer);
                *value = buffer->toUtf8StringView();
            }
        }
        return true;
    }else if constexpr(std::is_same<StringType,QLatin1String>::value){
        QLatin1String* value = reinterpret_cast<QLatin1String*>(output);
        if(value){
            if(Java::QtCore::QString::isInstanceOf(env, input)){
                QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, input);
                QtJambiAPI::checkNullPointer(env, strg);
                QByteArray* ba = new QByteArray(strg->toUtf8());
                scope.addDeletion(ba);
                *value = QLatin1String(*ba);
            }else{
                PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, static_cast<jstring>(input));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                scope.addDeletion(buffer);
                *value = buffer->toLatin1String();
            }
        }
        return true;
    }else{
        Q_UNUSED(env)
        Q_UNUSED(input)
        Q_UNUSED(output)
        Q_UNUSED(scope)
        return false;
    }
}

template<typename TargetType>
QtJambiTypeEntry::NativeToJavaResult MetaUtilTypeEntry<TargetType>::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jobject& output) const{
    if(qt_object){
        if constexpr(std::is_base_of<QMetaObject::Connection,TargetType>::value){
            bool makeCopyOfValues = false;
            switch(mode){
            case NativeToJavaConversionMode::None:
                if(jobject obj = QtJambiLink::findObjectForPointer(env, javaClass(), qt_object)){
                    output = obj;
                    return true;
                }
                break;
            case NativeToJavaConversionMode::MakeCopyOfValues:
                makeCopyOfValues = true;
                break;
            default: break;
            }

            static QMetaType metaTypeId(registeredMetaType(typeid(QMetaObject::Connection)));
            output = Java::QtJambi::SignalUtility$NativeConnection::newInstance(env, nullptr);
            return QtJambiLink::createLinkForNativeObject(
                    env,
                    output,
                    makeCopyOfValues ? new QMetaObject::Connection(*reinterpret_cast<const QMetaObject::Connection*>(qt_object)) : const_cast<void*>(qt_object),
                    metaTypeId,
                    false,
                    false,
                    mode==NativeToJavaConversionMode::None ? QtJambiLink::Ownership::None : QtJambiLink::Ownership::Java
                );
        }else if constexpr(std::is_base_of<QMetaObject,TargetType>::value){
            Q_UNUSED(mode)
            output = QtJambiMetaObject::convertToJavaObject(env, reinterpret_cast<const QMetaObject*>(qt_object));
        }else if constexpr(std::is_base_of<JQObjectWrapper,TargetType>::value){
            Q_UNUSED(mode)
            output = reinterpret_cast<const JQObjectWrapper*>(qt_object)->javaObject(env);
        }else if constexpr(std::is_base_of<JObjectValueWrapper,TargetType>::value){
            Q_UNUSED(mode)
            output = reinterpret_cast<const JObjectValueWrapper*>(qt_object)->value(env);
        }else if constexpr(std::is_base_of<JObjectWrapper,TargetType>::value){
            Q_UNUSED(mode)
            output = reinterpret_cast<const JObjectWrapper*>(qt_object)->object(env);
        }else{
            Q_UNUSED(env)
            Q_UNUSED(mode)
            Q_UNUSED(output)
        }
    }
    return true;
}

QtJambiTypeEntry::NativeToJavaResult MetaObjectConnectionTypeEntry::convertToJava(JNIEnv *env, void *qt_object, jobject& output) const{
    if(qt_object){
        if(jobject obj = QtJambiLink::findObjectForPointer(env, javaClass(), qt_object)){
            output = obj;
            return true;
        }
        static QMetaType metaTypeId(registeredMetaType(typeid(QMetaObject::Connection)));
        output = Java::QtJambi::SignalUtility$NativeConnection::newInstance(env, nullptr);
        return QtJambiLink::createLinkForNativeObject(
                env,
                output,
                new QMetaObject::Connection(std::move(*reinterpret_cast<QMetaObject::Connection*>(qt_object))),
                metaTypeId,
                false,
                false,
                QtJambiLink::Ownership::Java
            );
    }
    return true;
}

template<typename TargetType>
bool MetaUtilTypeEntry<TargetType>::convertToNative(JNIEnv *env, jobject input, void *output) const{
    Q_UNUSED(output)
    if constexpr(std::is_same<TargetType,QMetaObject::Connection>::value){
        if(!env->IsSameObject(input, nullptr)){
            if(!Java::QtCore::QMetaObject$Connection::isInstanceOf(env, input))
                return false;
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, input)){
                Q_ASSERT(!link->isQObject());
                QMetaObject::Connection*& pointer = *reinterpret_cast<QMetaObject::Connection**>(output);
                if(!pointer){
                    pointer = reinterpret_cast<QMetaObject::Connection*>(link->pointer());
                }else{
                    *pointer = *reinterpret_cast<QMetaObject::Connection*>(link->pointer());
                }
            }
            else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, input))
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, input).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }else{
            QMetaObject::Connection*& pointer = *reinterpret_cast<QMetaObject::Connection**>(output);
            if(pointer){
                *pointer = QMetaObject::Connection();
            }
        }
    }else if constexpr(std::is_same<TargetType,QMetaObject>::value){
        if(!env->IsSameObject(input, nullptr)){
            if(!Java::QtCore::QMetaObject::isInstanceOf(env, input))
                return false;
            jlong ptr = Java::QtCore::QMetaObject::__qt_persistentPointer(env, input);
            *reinterpret_cast<const QMetaObject**>(output) = reinterpret_cast<const QMetaObject *>(ptr);
        }else{
            *reinterpret_cast<const QMetaObject**>(output) = nullptr;
        }
    }else if constexpr(std::is_same<TargetType,JQObjectWrapper>::value){
        QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, input);
        if(link && link->isQObject())
            *reinterpret_cast<JQObjectWrapper*>(output) = JQObjectWrapper(env, std::move(link));
        else return false;
    }else if constexpr(std::is_same<TargetType,JObjectValueWrapper>::value){
        reinterpret_cast<JObjectValueWrapper*>(output)->assign(env, input);
    }else if constexpr(std::is_base_of_v<JObjectWrapper, TargetType>){
        reinterpret_cast<JObjectWrapper*>(output)->assign(env, input);
    }
    return true;
}

QtJambiTypeEntry::NativeToJavaResult QVariantTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode, jobject& output) const{
    output = QtJambiAPI::convertQVariantToJavaObject(env, *reinterpret_cast<const QVariant*>(qt_object));
    return true;
}

bool QVariantTypeEntry::convertToNative(JNIEnv *env, jobject input, void *output) const{
    *reinterpret_cast<QVariant*>(output) = QtJambiAPI::convertJavaObjectToQVariant(env, input);
    return true;
}

template<template<typename> class SmartPointer>
bool QVariantTypeEntry::convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jobject& output) const{
    void* qt_object = smartPointer.get();
    if (!qt_object){
        output = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && Java::QtCore::QVariant::isInstanceOf(env, obj)){
                output = obj;
                if(!link->isSmartPointer()){
                    bool createdByJava = link->createdByJava();
                    bool is_shell = link->isShell();
                    QtJambiShellImpl* shell = nullptr;
                    if(is_shell){
                        const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output));
                        Q_ASSERT(infos.size());
                        shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                    }
                    link->reset(env);
                    PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                    if(!link->isQObject())
                        registeredThreadAffinityFunction = link->ownerFunction();
                    link->invalidate(env);
                    link.clear();
                    if(registeredThreadAffinityFunction){
                        link = QtJambiLink::createLinkForSmartPointerToObject(env, output,
                                                                               LINK_NAME_ARG(qtName())
                                                                               createdByJava, is_shell,
                                                                               registeredThreadAffinityFunction,
                                                                               smartPointer);
                    }else{
                        link = QtJambiLink::createLinkForSmartPointerToObject(env, output,
                                                                               LINK_NAME_ARG(qtName())
                                                                               createdByJava, is_shell,
                                                                               smartPointer);
                    }
                    link->setJavaOwnership(env);
                    if(shell){
                        shell->overrideLink(link);
                    }
                    return true;
                }else{
                    switch(link->smartPointerType()){
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                    case QtJambiPrivate::SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                        return true;
                    default:
                        return false;
                    }
                }
            }else return false;
        }
    }
    output = Java::QtCore::QVariant::newInstance(env, nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(
        env,
        output,
        LINK_NAME_ARG(qtName())
        false,
        false,
        smartPointer
        );
    link->setJavaOwnership(env);
    return true;
}

bool QVariantTypeEntry::convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<QSharedPointer>(env, offset==0 ? smartPointer : convertSmartPointer<char>(smartPointer, offset), output);
}

bool QVariantTypeEntry::convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jobject& output) const{
    return convertSmartPointerToJava<std::shared_ptr>(env, offset==0 ? smartPointer : convertSmartPointer<char>(smartPointer, offset), output);
}
