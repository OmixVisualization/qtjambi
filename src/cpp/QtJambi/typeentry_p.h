/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include <typeinfo>
#include "qtjambiapi.h"
#include "registryapi.h"
#include "utils_p.h"

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
    virtual bool isFunctional() const;
    virtual bool isShellable() const;
    virtual bool isQObject() const;
    virtual bool isObject() const;
    virtual bool isPolymorphic() const;
    const std::type_info& type() const;
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
    static NativeToJavaResult convertModelIndexNativeToJava(JNIEnv *env, const QModelIndex *qt_object, NativeToJavaConversionMode mode, jobject& output, QtJambiScope* scope = nullptr);
    static bool convertModelIndexJavaToNative(JNIEnv *env, jobject java_value, void * output, QtJambiScope* scope);

protected:
    QtJambiTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size);
private:
    static QtJambiTypeEntryPtr getTypeEntry(JNIEnv* env, const std::type_info& typeId, bool recursive, const char* qtName);
    static jobject convertInvalidModelIndexToJava(JNIEnv *env);
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
    using AbstractSimpleTypeEntry::AbstractSimpleTypeEntry;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
};
#endif

class StringTypeEntry : public AbstractSimpleTypeEntry{
public:
    using AbstractSimpleTypeEntry::AbstractSimpleTypeEntry;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
};

class QVariantTypeEntry : public AbstractSimpleTypeEntry{
public:
    using AbstractSimpleTypeEntry::AbstractSimpleTypeEntry;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
};

class ShellableTypeEntry : public QtJambiTypeEntry{
public:
    bool isShellable() const final;
    size_t shellSize() const;
    const QVector<RegistryAPI::FunctionInfo>& virtualFunctions() const;
protected:
    ShellableTypeEntry(JNIEnv* env,
                           const std::type_info& typeId,
                           const char *qt_name,
                           const char *java_name,
                           jclass java_class,
                           jmethodID creator_method,
                           size_t value_size,
                           size_t shell_size,
                           const QVector<RegistryAPI::FunctionInfo>& virtualFunctions
                           );
private:
    size_t m_shell_size;
    const QVector<RegistryAPI::FunctionInfo> m_virtualFunctions;
};

class DestructableTypeEntry : public ShellableTypeEntry{
public:
    RegistryAPI::DestructorFn destructor() const;
protected:
    DestructableTypeEntry(JNIEnv* env,
                           const std::type_info& typeId,
                           const char *qt_name,
                           const char *java_name,
                           jclass java_class,
                           jmethodID creator_method,
                           size_t value_size,
                           size_t shell_size,
                           const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
                           RegistryAPI::DestructorFn destructor
                        );
private:
    RegistryAPI::DestructorFn m_destructor;
};


class FunctionalTypeEntry : public DestructableTypeEntry{
public:
    bool isFunctional() const final;
    jclass creatableClass() const override;
    FunctionalResolver registeredFunctionalResolver() const;
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
                        const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
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
    jclass m_java_impl_class;
    jclass m_java_wrapper_class;
    FunctionalResolver m_registered_functional_resolver;
    QMetaType m_qt_meta_type;
    bool m_is_std_function;
};

class ObjectTypeAbstractEntry : public DestructableTypeEntry{
public:
    bool isObject() const final;
    PtrDeleterFunction deleter() const;
    TypeInfoSupplier typeInfoSupplier() const;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    virtual bool isValue() const;
    virtual bool isInterface() const;
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<char>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    ObjectTypeAbstractEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
                    RegistryAPI::DestructorFn destructor,
                    TypeInfoSupplier typeInfoSupplier
                    );
    struct CopyValueInfo{
        void* value;
        bool isCopy = false;
        AbstractContainerAccess* containerAccess = nullptr;
    };
    virtual CopyValueInfo copyValue(JNIEnv *, const void*, bool makeCopyOfValue) const = 0;
    virtual QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *object, NativeToJavaConversionMode mode, bool isMetaCopy, AbstractContainerAccess* containerAccess = nullptr) const = 0;
    virtual QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const QSharedPointer<char>& smartPointer, PtrOwnerFunction ownerFunction = nullptr) const = 0;
    virtual QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const std::shared_ptr<char>& smartPointer, PtrOwnerFunction ownerFunction = nullptr) const = 0;
private:
    PtrDeleterFunction m_deleter;
    TypeInfoSupplier m_typeInfoSupplier;
};

class QObjectTypeAbstractEntry : public ShellableTypeEntry{
public:
    bool isQObject() const final;
    const QMetaObject* originalMetaObject() const;
    QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const override;
    NativeToJavaResult convertToJava(JNIEnv *env, const void *qt_object, NativeToJavaConversionMode mode, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const QSharedPointer<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertSmartPointerToJava(JNIEnv *env, const std::shared_ptr<char>& smartPointer, qintptr offset, jvalue& output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, jvalue java_value, jValueType javaType, void * output, QtJambiScope* scope) const override;
private:
    template<template<typename> class SmartPointer>
    bool convertSmartPointerToJava(JNIEnv *env, const SmartPointer<QObject>& smartPointer, jvalue& output, jValueType valueType) const;
protected:
    QObjectTypeAbstractEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     size_t shell_size,
                     const QVector<RegistryAPI::FunctionInfo>& virtualFunctions,
                     const QMetaObject* original_meta_object
                     );
    virtual QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& javaObject, QObject *object) const = 0;
    virtual QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const QSharedPointer<QObject>& smartPointer) const = 0;
    virtual QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool createdByJava, bool is_shell, const std::shared_ptr<QObject>& smartPointer) const = 0;
    bool isQThread() const;
private:
    const QMetaObject* m_original_meta_object;
    bool m_isQThread;
};

#endif // QTJAMBI_TYPEENTRY_P_H
