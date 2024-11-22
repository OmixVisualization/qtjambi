/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_TYPEENTRY_P_H
#define QTJAMBI_TYPEENTRY_P_H

#include <QtCore/QObject>
#include <typeindex>
#include <typeinfo>
#include "qtjambiapi.h"
#include "registryapi.h"
#include "utils_p.h"

struct PolymorphicIdHandler{
    PolymorphicIdHandler(const std::type_info& targetTypeId, PolymorphyHandler polymorphyHandler)
        : m_targetTypeId(targetTypeId),
          m_polymorphyHandler(polymorphyHandler)
    {
    }
    const std::type_info& m_targetTypeId;
    PolymorphyHandler m_polymorphyHandler;
};

struct InterfaceOffsetInfo{
    QMap<size_t,uint> offsets;
    QSet<size_t> interfaces;
    QMap<size_t, QSet<const std::type_info*>> inheritedInterfaces;
};

enum class EntryTypes
{
    Unspecific = 0,
    ObjectTypeInfo,
    EnumTypeInfo,
    FlagsTypeInfo,
    FunctionalTypeInfo,
    InterfaceValueTypeInfo,
    InterfaceTypeInfo,
    QObjectTypeInfo,
    ValueTypeInfo,
    SpecialTypeInfo,
    StringTypeInfo,
    PrimitiveTypeInfo
};

typedef QExplicitlySharedDataPointer<const class QtJambiTypeEntry> QtJambiTypeEntryPtr;

class QtJambiLink;

class QtJambiTypeEntry : public QSharedData{
public:
    class NativeToJavaResult{
    public:
        NativeToJavaResult(bool success);
        NativeToJavaResult(const QSharedPointer<QtJambiLink>& link);
        inline operator bool() const {return m_success;}
        inline bool operator!() const {return !m_success;}
        //inline const QSharedPointer<QtJambiLink>& link() const {return m_link;}
    private:
        //QSharedPointer<QtJambiLink> m_link;
        bool m_success;
    };
    virtual ~QtJambiTypeEntry();
    virtual bool isEnum() const;
    virtual bool isFlags() const;
    virtual bool isInterface() const;
    virtual bool isFunctional() const;
    virtual bool isQObject() const;
    virtual bool isObject() const;
    virtual bool isValue() const;
    const std::type_info& type() const;
    virtual const std::type_info* superType() const;
    const char *qtName() const;
    const char *javaName() const;
    jclass javaClass() const;
    virtual jclass creatableClass() const;
    jmethodID creatorMethod() const;
    size_t valueSize() const;
    virtual uint offset(const std::type_info& toType) const;

    virtual QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const;
    virtual NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const = 0;
    virtual bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const = 0;
    virtual bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const = 0;
    virtual bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const = 0;

    static QtJambiTypeEntryPtr getTypeEntry(JNIEnv* env, const std::type_info& typeId);
    static QtJambiTypeEntryPtr getTypeEntry(JNIEnv* env, const std::type_info& typeId, const char* qtName);
    static QtJambiTypeEntryPtr getTypeEntryByJavaName(JNIEnv* env, const char* java_name);
    static QtJambiTypeEntryPtr getTypeEntryByJavaName(JNIEnv* env, const QString& java_name);
    static QtJambiTypeEntryPtr getTypeEntryByQtName(JNIEnv* env, const char* qt_name);
    static QtJambiTypeEntryPtr getTypeEntryByIID(JNIEnv* env, const char* iid);

protected:
    QtJambiTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size);
private:
    const std::type_info& m_typeId;
    const char *m_qt_name;
    const char *m_java_name;
    const jclass m_java_class;
    const jmethodID m_creator_method;
    size_t m_value_size;
};

class FlagsTypeEntry;

class EnumTypeEntry : public QtJambiTypeEntry{
public:
    bool isEnum() const final;
    const FlagsTypeEntry* flagType() const;
    EnumTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    FlagsTypeEntry* m_flagType;
    friend FlagsTypeEntry;
};

class FlagsTypeEntry : public QtJambiTypeEntry{
public:
    bool isFlags() const final;
    const EnumTypeEntry* enumType() const;
    FlagsTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size, const EnumTypeEntry* enumType);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    const EnumTypeEntry* m_enumType;
};

class AbstractSimpleTypeEntry : public QtJambiTypeEntry{
public:
    AbstractSimpleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
};

#if QT_VERSION >= 0x050C00
class QCborValueRefTypeEntry : public AbstractSimpleTypeEntry{
public:
    QCborValueRefTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};
#endif

class StringTypeEntry : public AbstractSimpleTypeEntry{
public:
    StringTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
};

class StringUtilTypeEntry : public AbstractSimpleTypeEntry{
public:
    StringUtilTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class MetaUtilTypeEntry : public AbstractSimpleTypeEntry{
public:
    MetaUtilTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class QVariantTypeEntry : public AbstractSimpleTypeEntry{
public:
    QVariantTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
};

class JLongTypeEntry : public AbstractSimpleTypeEntry{
public:
    JLongTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class JIntTypeEntry : public AbstractSimpleTypeEntry{
public:
    JIntTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class JShortTypeEntry : public AbstractSimpleTypeEntry{
public:
    JShortTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class JByteTypeEntry : public AbstractSimpleTypeEntry{
public:
    JByteTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class JCharTypeEntry : public AbstractSimpleTypeEntry{
public:
    JCharTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class JBooleanTypeEntry : public AbstractSimpleTypeEntry{
public:
    JBooleanTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class JDoubleTypeEntry : public AbstractSimpleTypeEntry{
public:
    JDoubleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class JFloatTypeEntry : public AbstractSimpleTypeEntry{
public:
    JFloatTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class NullptrTypeEntry : public AbstractSimpleTypeEntry{
public:
    NullptrTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};

class PolymorphicTypeEntryInterface{
public:
    virtual ~PolymorphicTypeEntryInterface();
    virtual const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers() const = 0;
};

class InterfaceTypeEntry : public QtJambiTypeEntry, public PolymorphicTypeEntryInterface{
public:
    bool isInterface() const final;
    const std::type_info* superType() const override;
    jclass creatableClass() const override;
    jclass implementationClass() const;
    size_t shellSize() const;
    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions() const;
    RegistryAPI::DestructorFn destructor() const;
    ~InterfaceTypeEntry() override;
    InterfaceTypeEntry(JNIEnv* env, const std::type_info& typeId,
                       const char *qt_name,
                       const char *java_name,
                       jclass java_class,
                       jmethodID creator_method,
                       size_t value_size,
                       jclass java_impl_class,
                       jclass java_wrapper_class,
                       const std::type_info* super_type,
                       size_t shell_size,
                       PtrDeleterFunction deleter,
                       const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                       RegistryAPI::DestructorFn destructor,
                       const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                       TypeInfoSupplier typeInfoSupplier
                    );
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
    const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers() const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    jclass m_java_impl_class;
    jclass m_java_wrapper_class;
    const std::type_info* m_super_type;
    size_t m_shell_size;
    PtrDeleterFunction m_deleter;
    const QVector<const RegistryAPI::FunctionInfo>* m_virtualFunctions;
    RegistryAPI::DestructorFn m_destructor;
private:
    QList<const PolymorphicIdHandler*> m_polymorphicIdHandlers;
    TypeInfoSupplier m_typeInfoSupplier;
    friend class InterfaceValueTypeEntry;
};

class OwnedInterfaceTypeEntry : public InterfaceTypeEntry{
public:
    OwnedInterfaceTypeEntry(JNIEnv* env, const std::type_info& typeId,
                       const char *qt_name,
                       const char *java_name,
                       jclass java_class,
                       jmethodID creator_method,
                       size_t value_size,
                       jclass java_impl_class,
                       jclass java_wrapper_class,
                       const std::type_info* super_type,
                       size_t shell_size,
                       PtrDeleterFunction deleter,
                       PtrOwnerFunction owner_function,
                       const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                       RegistryAPI::DestructorFn destructor,
                       const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                       TypeInfoSupplier typeInfoSupplier
                       );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    PtrOwnerFunction m_owner_function;
};

class InterfaceIFTypeEntry : public InterfaceTypeEntry{
public:
    InterfaceIFTypeEntry(JNIEnv* env, const std::type_info& typeId,
                         const char *qt_name,
                         const char *java_name,
                         jclass java_class,
                         jmethodID creator_method,
                         size_t value_size,
                         jclass java_impl_class,
                         jclass java_wrapper_class,
                         const std::type_info* super_type,
                         size_t shell_size,
                         PtrDeleterFunction deleter,
                         const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                         RegistryAPI::DestructorFn destructor,
                         const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                         TypeInfoSupplier typeInfoSupplier,
                         const InterfaceOffsetInfo& interfaceOffsetInfo);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    uint offset(const std::type_info& toType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    const InterfaceOffsetInfo m_interfaceOffsetInfo;
};

class OwnedInterfaceIFTypeEntry : public InterfaceIFTypeEntry{
public:
    OwnedInterfaceIFTypeEntry(JNIEnv* env, const std::type_info& typeId,
                         const char *qt_name,
                         const char *java_name,
                         jclass java_class,
                         jmethodID creator_method,
                         size_t value_size,
                         jclass java_impl_class,
                         jclass java_wrapper_class,
                         const std::type_info* super_type,
                         size_t shell_size,
                         PtrDeleterFunction deleter,
                         PtrOwnerFunction owner_function,
                         const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                         RegistryAPI::DestructorFn destructor,
                         const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                         TypeInfoSupplier typeInfoSupplier,
                         const InterfaceOffsetInfo& interfaceOffsetInfo);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    PtrOwnerFunction m_owner_function;
};

class InterfaceValueTypeEntry : public InterfaceTypeEntry{
public:
    bool isValue() const final;
    InterfaceValueTypeEntry(JNIEnv* env, const std::type_info& typeId,
                       const char *qt_name,
                       const char *java_name,
                       jclass java_class,
                       jmethodID creator_method,
                       size_t value_size,
                       jclass java_impl_class,
                       jclass java_wrapper_class,
                       const std::type_info* super_type,
                       size_t shell_size,
                       PtrDeleterFunction deleter,
                       const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                       RegistryAPI::DestructorFn destructor,
                       const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                            TypeInfoSupplier typeInfoSupplier,
                       const QMetaType& qt_meta_type
                    );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    QMetaType m_qt_meta_type;
};

class OwnedInterfaceValueTypeEntry : public InterfaceValueTypeEntry{
public:
    OwnedInterfaceValueTypeEntry(JNIEnv* env, const std::type_info& typeId,
                            const char *qt_name,
                            const char *java_name,
                            jclass java_class,
                            jmethodID creator_method,
                            size_t value_size,
                            jclass java_impl_class,
                            jclass java_wrapper_class,
                            const std::type_info* super_type,
                            size_t shell_size,
                            PtrDeleterFunction deleter,
                            PtrOwnerFunction owner_function,
                            const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                            RegistryAPI::DestructorFn destructor,
                            const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                            TypeInfoSupplier typeInfoSupplier,
                                 const QMetaType& qt_meta_type
                            );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    PtrOwnerFunction m_owner_function;
};

class InterfaceValueIFTypeEntry : public InterfaceIFTypeEntry{
public:
    InterfaceValueIFTypeEntry(JNIEnv* env, const std::type_info& typeId,
                              const char *qt_name,
                              const char *java_name,
                              jclass java_class,
                              jmethodID creator_method,
                              size_t value_size,
                              jclass java_impl_class,
                              jclass java_wrapper_class,
                              const std::type_info* super_type,
                              size_t shell_size,
                              PtrDeleterFunction deleter,
                              const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                              RegistryAPI::DestructorFn destructor,
                              const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                              TypeInfoSupplier typeInfoSupplier,
                              const QMetaType& qt_meta_type,
                              const InterfaceOffsetInfo& interfaceOffsetInfo);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
private:
    friend class OwnedInterfaceValueIFTypeEntry;
    QMetaType m_qt_meta_type;
};

class OwnedInterfaceValueIFTypeEntry : public InterfaceValueIFTypeEntry{
public:
    OwnedInterfaceValueIFTypeEntry(JNIEnv* env, const std::type_info& typeId,
                              const char *qt_name,
                              const char *java_name,
                              jclass java_class,
                              jmethodID creator_method,
                              size_t value_size,
                              jclass java_impl_class,
                              jclass java_wrapper_class,
                              const std::type_info* super_type,
                              size_t shell_size,
                              PtrDeleterFunction deleter,
                              PtrOwnerFunction owner_function,
                              const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                              RegistryAPI::DestructorFn destructor,
                              const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                              TypeInfoSupplier typeInfoSupplier,
                              const QMetaType& qt_meta_type,
                              const InterfaceOffsetInfo& interfaceOffsetInfo);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
private:
    PtrOwnerFunction m_owner_function;
};

class FunctionalTypeEntry : public QtJambiTypeEntry{
public:
    bool isFunctional() const final;
    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions() const;
    jclass creatableClass() const override;
    RegistryAPI::DestructorFn destructor() const;
    FunctionalResolver registeredFunctionalResolver() const;
    size_t shellSize() const;
    ~FunctionalTypeEntry() override;
    FunctionalTypeEntry(JNIEnv* env, const std::type_info& typeId,
                        const char *qt_name,
                        const char *java_name,
                        jclass java_class,
                        jmethodID creator_method,
                        size_t value_size,
                        size_t shell_size,
                        jclass java_impl_class,
                        jclass java_wrapper_class,
                        const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                        RegistryAPI::DestructorFn destructor,
                        FunctionalResolver registered_functional_resolver,
                        const QMetaType& qt_meta_type,
                        bool is_std_function
                    );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    size_t m_shell_size;
    jclass m_java_impl_class;
    jclass m_java_wrapper_class;
    const QVector<const RegistryAPI::FunctionInfo>* m_virtualFunctions;
    RegistryAPI::DestructorFn m_destructor;
    FunctionalResolver m_registered_functional_resolver;
    QMetaType m_qt_meta_type;
    bool m_is_std_function;
};

class ObjectTypeEntry : public QtJambiTypeEntry, public PolymorphicTypeEntryInterface{
public:
    bool isObject() const final;
    const std::type_info* superType() const final;
    size_t shellSize() const;
    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions() const;
    RegistryAPI::DestructorFn destructor() const;
    ObjectTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier
                );
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
    const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers() const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    const std::type_info* m_super_type;
    size_t m_shell_size;
    PtrDeleterFunction m_deleter;
    const QVector<const RegistryAPI::FunctionInfo>* m_virtualFunctions;
    RegistryAPI::DestructorFn m_destructor;
private:
    QList<const PolymorphicIdHandler*> m_polymorphicIdHandlers;
    TypeInfoSupplier m_typeInfoSupplier;
    friend class ObjectValueTypeEntry;
    friend class ObjectContainerTypeEntry;
    friend class ObjectAbstractContainerTypeEntry;
    friend class QMessageLogContextTypeEntry;
};

class OwnedObjectTypeEntry : public ObjectTypeEntry{
public:
    OwnedObjectTypeEntry(JNIEnv* env,
                              const std::type_info& typeId,
                              const char *qt_name,
                              const char *java_name,
                              jclass java_class,
                              jmethodID creator_method,
                              size_t value_size,
                              const std::type_info* super_type,
                              size_t shell_size,
                              PtrDeleterFunction deleter,
                              PtrOwnerFunction owner_function,
                              const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                              RegistryAPI::DestructorFn destructor,
                              const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                              TypeInfoSupplier typeInfoSupplier
                              );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
private:
    PtrOwnerFunction m_owner_function;
};

class ObjectIFTypeEntry : public ObjectTypeEntry{
public:
    ObjectIFTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    uint offset(const std::type_info& toType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    const InterfaceOffsetInfo m_interfaceOffsetInfo;
};

class OwnedObjectIFTypeEntry : public ObjectIFTypeEntry{
public:
    OwnedObjectIFTypeEntry(JNIEnv* env,
                      const std::type_info& typeId,
                      const char *qt_name,
                      const char *java_name,
                      jclass java_class,
                      jmethodID creator_method,
                      size_t value_size,
                      const std::type_info* super_type,
                      size_t shell_size,
                      PtrDeleterFunction deleter,
                      PtrOwnerFunction owner_function,
                      const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                      RegistryAPI::DestructorFn destructor,
                      const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                      TypeInfoSupplier typeInfoSupplier,
                      const InterfaceOffsetInfo& interfaceOffsetInfo
                      );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
private:
    PtrOwnerFunction m_owner_function;
};

class ObjectAbstractTypeEntry : public ObjectTypeEntry{
public:
    jclass creatableClass() const final;
    ObjectAbstractTypeEntry(JNIEnv* env,
                            const std::type_info& typeId,
                            const char *qt_name,
                            const char *java_name,
                            jclass java_class,
                            jmethodID creator_method,
                            size_t value_size,
                            const std::type_info* super_type,
                            size_t shell_size,
                            PtrDeleterFunction deleter,
                            const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                            RegistryAPI::DestructorFn destructor,
                            const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                            TypeInfoSupplier typeInfoSupplier,
                            jclass java_wrapper_class
                            );
private:
    jclass m_java_wrapper_class;
};

class OwnedObjectAbstractTypeEntry : public OwnedObjectTypeEntry{
public:
    jclass creatableClass() const final;
    OwnedObjectAbstractTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    PtrOwnerFunction owner_function,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    jclass java_wrapper_class
                );
private:
    jclass m_java_wrapper_class;
};

class ObjectAbstractIFTypeEntry : public ObjectIFTypeEntry{
public:
    jclass creatableClass() const final;
    ObjectAbstractIFTypeEntry(JNIEnv* env,
                              const std::type_info& typeId,
                              const char *qt_name,
                              const char *java_name,
                              jclass java_class,
                              jmethodID creator_method,
                              size_t value_size,
                              const std::type_info* super_type,
                              size_t shell_size,
                              PtrDeleterFunction deleter,
                              const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                              RegistryAPI::DestructorFn destructor,
                              const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                              TypeInfoSupplier typeInfoSupplier,
                              jclass java_wrapper_class,
                              const InterfaceOffsetInfo& interfaceOffsetInfo
                              );
private:
    jclass m_java_wrapper_class;
};

class OwnedObjectAbstractIFTypeEntry : public OwnedObjectIFTypeEntry{
public:
    jclass creatableClass() const final;
    OwnedObjectAbstractIFTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    PtrOwnerFunction owner_function,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    jclass java_wrapper_class,
                    const InterfaceOffsetInfo& interfaceOffsetInfo
                );
private:
    jclass m_java_wrapper_class;
};

class ObjectValueTypeEntry : public ObjectTypeEntry{
public:
    bool isValue() const final;
    ObjectValueTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type
                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    QMetaType m_qt_meta_type;
    friend class ObjectContainerTypeEntry;
    friend class ObjectAbstractContainerTypeEntry;
    friend class QModelIndexTypeEntry;
};

class OwnedObjectValueTypeEntry : public ObjectValueTypeEntry{
public:
    OwnedObjectValueTypeEntry(JNIEnv* env,
                         const std::type_info& typeId,
                         const char *qt_name,
                         const char *java_name,
                         jclass java_class,
                         jmethodID creator_method,
                         size_t value_size,
                         const std::type_info* super_type,
                         size_t shell_size,
                         PtrDeleterFunction deleter,
                         PtrOwnerFunction owner_function,
                         const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                         RegistryAPI::DestructorFn destructor,
                         const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                         TypeInfoSupplier typeInfoSupplier,
                         const QMetaType& qt_meta_type
                         );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
private:
    PtrOwnerFunction m_owner_function;
};

class ObjectValueIFTypeEntry : public ObjectIFTypeEntry{
public:
    ObjectValueIFTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type,
                    const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
private:
    QMetaType m_qt_meta_type;
    friend class ObjectOwnedValueIFTypeEntry;
    friend class ObjectContainerIFTypeEntry;
    friend class ObjectAbstractContainerIFTypeEntry;
    friend class ObjectContainerTypeEntry;
    friend class ObjectAbstractContainerTypeEntry;
};

class ObjectOwnedValueIFTypeEntry : public ObjectValueIFTypeEntry{
public:
    ObjectOwnedValueIFTypeEntry(JNIEnv* env,
                           const std::type_info& typeId,
                           const char *qt_name,
                           const char *java_name,
                           jclass java_class,
                           jmethodID creator_method,
                           size_t value_size,
                           const std::type_info* super_type,
                           size_t shell_size,
                           PtrDeleterFunction deleter,
                           PtrOwnerFunction owner_function,
                           const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                           RegistryAPI::DestructorFn destructor,
                           const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                           TypeInfoSupplier typeInfoSupplier,
                           const QMetaType& qt_meta_type,
                           const InterfaceOffsetInfo& interfaceOffsetInfo
                           );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
private:
    PtrOwnerFunction m_owner_function;
};

class QModelIndexTypeEntry : public ObjectValueTypeEntry{
public:
    QModelIndexTypeEntry(JNIEnv* env,
                         const std::type_info& typeId,
                         const char *qt_name,
                         const char *java_name,
                         jclass java_class,
                         PtrDeleterFunction deleter,
                         RegistryAPI::DestructorFn destructor,
                         TypeInfoSupplier typeInfoSupplier);
    static NativeToJavaResult convertModelIndexToJava(JNIEnv *env, const QModelIndex *qt_object, NativeToJavaConversionMode mode, jobject& output, QtJambiScope* scope = nullptr);
    static bool convertJavaToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
    static void deleter(void *ptr, bool isShell);
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
};

class QMessageLogContextTypeEntry : public ObjectTypeEntry{
public:
    QMessageLogContextTypeEntry(JNIEnv* env,
                                const std::type_info& typeId,
                                const char *qt_name,
                                const char *java_name,
                                jclass java_class,
                                jmethodID creator_method,
                                size_t value_size,
                                const std::type_info* super_type,
                                size_t shell_size,
                                PtrDeleterFunction deleter,
                                const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                                RegistryAPI::DestructorFn destructor,
                                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                                TypeInfoSupplier typeInfoSupplier
                                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
};

class ObjectAbstractValueTypeEntry : public ObjectValueTypeEntry{
public:
    jclass creatableClass() const final;
    ObjectAbstractValueTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type,
                    jclass java_wrapper_class
                );
private:
    jclass m_java_wrapper_class;
};

class ObjectAbstractOwnedValueTypeEntry : public OwnedObjectValueTypeEntry{
public:
    jclass creatableClass() const final;
    ObjectAbstractOwnedValueTypeEntry(JNIEnv* env,
                                 const std::type_info& typeId,
                                 const char *qt_name,
                                 const char *java_name,
                                 jclass java_class,
                                 jmethodID creator_method,
                                 size_t value_size,
                                 const std::type_info* super_type,
                                 size_t shell_size,
                                 PtrDeleterFunction deleter,
                                 PtrOwnerFunction owner_function,
                                 const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                                 RegistryAPI::DestructorFn destructor,
                                 const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                                 TypeInfoSupplier typeInfoSupplier,
                                 const QMetaType& qt_meta_type,
                                 jclass java_wrapper_class
                                 );
private:
    jclass m_java_wrapper_class;
};

class ObjectAbstractValueIFTypeEntry : public ObjectValueIFTypeEntry{
public:
    jclass creatableClass() const final;
    ObjectAbstractValueIFTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type,
                    jclass java_wrapper_class,
                    const InterfaceOffsetInfo& interfaceOffsetInfo
                );
private:
    jclass m_java_wrapper_class;
};

class ObjectAbstractOwnedValueIFTypeEntry : public ObjectOwnedValueIFTypeEntry{
public:
    jclass creatableClass() const final;
    ObjectAbstractOwnedValueIFTypeEntry(JNIEnv* env,
                                   const std::type_info& typeId,
                                   const char *qt_name,
                                   const char *java_name,
                                   jclass java_class,
                                   jmethodID creator_method,
                                   size_t value_size,
                                   const std::type_info* super_type,
                                   size_t shell_size,
                                   PtrDeleterFunction deleter,
                                   PtrOwnerFunction owner_function,
                                   const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                                   RegistryAPI::DestructorFn destructor,
                                   const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                                   TypeInfoSupplier typeInfoSupplier,
                                   const QMetaType& qt_meta_type,
                                   jclass java_wrapper_class,
                                   const InterfaceOffsetInfo& interfaceOffsetInfo
                                   );
private:
    jclass m_java_wrapper_class;
};

class ObjectContainerTypeEntry : public ObjectValueTypeEntry{
public:
    ObjectContainerTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    NewContainerAccessFunction containerAccessFactory,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type
                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
    NewContainerAccessFunction m_containerAccessFactory;
};

class ObjectContainerIFTypeEntry : public ObjectValueIFTypeEntry{
public:
    ObjectContainerIFTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    NewContainerAccessFunction containerAccessFactory,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type,
                    const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
    NewContainerAccessFunction m_containerAccessFactory;
};

class ObjectAbstractContainerTypeEntry : public ObjectAbstractValueTypeEntry{
public:
    ObjectAbstractContainerTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    NewContainerAccessFunction containerAccessFactory,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type,
                    jclass java_wrapper_class
                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
    NewContainerAccessFunction m_containerAccessFactory;
};

class ObjectAbstractContainerIFTypeEntry : public ObjectAbstractValueIFTypeEntry{
public:
    ObjectAbstractContainerIFTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    size_t shell_size,
                    NewContainerAccessFunction containerAccessFactory,
                    PtrDeleterFunction deleter,
                    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    TypeInfoSupplier typeInfoSupplier,
                    const QMetaType& qt_meta_type,
                    jclass java_wrapper_class,
                    const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
    NewContainerAccessFunction m_containerAccessFactory;
};

class QObjectTypeEntry : public QtJambiTypeEntry{
public:
    bool isQObject() const final;
    const std::type_info* superType() const override;
    size_t shellSize() const;
    const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions() const;
    const QMetaObject* originalMetaObject() const;
    QObjectTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object
                );
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<QObject>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    virtual const QSharedPointer<QtJambiLink>& createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const;
    const std::type_info* m_super_type;
    size_t m_shell_size;
    const QVector<const RegistryAPI::FunctionInfo>* m_virtualFunctions;
    const QMetaObject* m_original_meta_object;
    const QMetaObject* m_superTypeForCustomMetaObject;
    friend class QObjectPolymorphicTypeEntry;
};

class QObjectIFTypeEntry : public QObjectTypeEntry{
public:
    QObjectIFTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    uint offset(const std::type_info& toType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<QObject>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    const QSharedPointer<QtJambiLink>& createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const override;
    const InterfaceOffsetInfo m_interfaceOffsetInfo;
};

class QObjectPolymorphicTypeEntry : public QObjectTypeEntry, public PolymorphicTypeEntryInterface{
public:
    QObjectPolymorphicTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers
                );
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
    const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers() const override;
protected:
    const QSharedPointer<QtJambiLink>& createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const override;
private:
    QList<const PolymorphicIdHandler*> m_polymorphicIdHandlers;
};

class QObjectPolymorphicIFTypeEntry : public QObjectIFTypeEntry, public PolymorphicTypeEntryInterface{
public:
    QObjectPolymorphicIFTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                     const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
    const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers() const override;
protected:
    const QSharedPointer<QtJambiLink>& createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const override;
private:
    QList<const PolymorphicIdHandler*> m_polymorphicIdHandlers;
};

class QThreadTypeEntry : public QObjectTypeEntry{
public:
    QThreadTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object);
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const;
};

class QObjectAbstractTypeEntry : public QObjectTypeEntry{
public:
    QObjectAbstractTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     jclass java_wrapper_class
                );
    jclass creatableClass() const final;
private:
    jclass m_java_wrapper_class;
};

class QObjectAbstractIFTypeEntry : public QObjectIFTypeEntry{
public:
    QObjectAbstractIFTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     jclass java_wrapper_class,
                     const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    jclass creatableClass() const final;
private:
    jclass m_java_wrapper_class;
};

class QObjectAbstractPolymorphicTypeEntry : public QObjectPolymorphicTypeEntry{
public:
    QObjectAbstractPolymorphicTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                     jclass java_wrapper_class
                );
    jclass creatableClass() const final;
private:
    jclass m_java_wrapper_class;
};

class QObjectAbstractPolymorphicIFTypeEntry : public QObjectPolymorphicIFTypeEntry{
public:
    QObjectAbstractPolymorphicIFTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     size_t shell_size,
                     const QVector<const RegistryAPI::FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                     jclass java_wrapper_class,
                     const InterfaceOffsetInfo& interfaceOffsetInfo
                );
    jclass creatableClass() const final;
private:
    jclass m_java_wrapper_class;
};

#endif // QTJAMBI_TYPEENTRY_P_H
