/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_TYPEINFO_P_H
#define QTJAMBI_TYPEINFO_P_H

#include <QtCore/QObject>
#include <typeindex>
#include <typeinfo>
#include "qtjambi_core.h"
#include "qtjambi_registry.h"

typedef std::function<void*(void *)> PointerGetterFunction;

struct PolymorphicIdHandler{
    PolymorphicIdHandler(PolymorphyHandler polymorphyHandler, const char *class_name, const std::type_info& targetTypeId, bool isQObject)
        : m_polymorphyHandler(polymorphyHandler)
        , m_class_name(class_name)
        , m_targetTypeId(targetTypeId)
        , m_isQObject(isQObject)
    {
    }
    PolymorphyHandler m_polymorphyHandler;
    const char *m_class_name;
    const std::type_info& m_targetTypeId;
    bool m_isQObject;
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

class QtJambiTypeEntry{
public:
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
    virtual const QList<const std::type_info*>& interfaceTypes() const;
    const char *qtName() const;
    const char *javaName() const;
    jclass javaClass() const;
    virtual jclass creatableClass() const;
    jmethodID creatorMethod() const;
    size_t valueSize() const;
    virtual uint offset(const std::type_info& toType) const;

    virtual bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const = 0;
    virtual bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const = 0;
    virtual bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const = 0;

    static const QtJambiTypeEntry* getTypeEntry(JNIEnv* env, const std::type_info& typeId);
    static const QtJambiTypeEntry* getTypeEntry(JNIEnv* env, const std::type_info& typeId, const char* qtName);
    static const QtJambiTypeEntry* getTypeEntryByJavaName(JNIEnv* env, const char* java_name);
    static const QtJambiTypeEntry* getTypeEntryByJavaName(JNIEnv* env, const QString& java_name);
    static const QtJambiTypeEntry* getTypeEntryByQtName(JNIEnv* env, const char* qt_name);
    static const QtJambiTypeEntry* getTypeEntryByIID(JNIEnv* env, const char* iid);

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
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
    FlagsTypeEntry* m_flagType;
    friend FlagsTypeEntry;
};

class FlagsTypeEntry : public QtJambiTypeEntry{
public:
    bool isFlags() const final;
    const EnumTypeEntry* enumType() const;
    FlagsTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size, const EnumTypeEntry* enumType);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
    const EnumTypeEntry* m_enumType;
};

class AbstractSimpleTypeEntry : public QtJambiTypeEntry{
public:
    AbstractSimpleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
private:
};

#if QT_VERSION >= 0x050C00
class QCborValueRefTypeEntry : public AbstractSimpleTypeEntry{
public:
    QCborValueRefTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};
#endif

class StringTypeEntry : public AbstractSimpleTypeEntry{
public:
    StringTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class StringUtilTypeEntry : public AbstractSimpleTypeEntry{
public:
    StringUtilTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class MetaUtilTypeEntry : public AbstractSimpleTypeEntry{
public:
    MetaUtilTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class QModelIndexTypeEntry : public AbstractSimpleTypeEntry{
public:
    QModelIndexTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class QVariantTypeEntry : public AbstractSimpleTypeEntry{
public:
    QVariantTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JLongTypeEntry : public AbstractSimpleTypeEntry{
public:
    JLongTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JIntTypeEntry : public AbstractSimpleTypeEntry{
public:
    JIntTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JShortTypeEntry : public AbstractSimpleTypeEntry{
public:
    JShortTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JByteTypeEntry : public AbstractSimpleTypeEntry{
public:
    JByteTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JCharTypeEntry : public AbstractSimpleTypeEntry{
public:
    JCharTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JBooleanTypeEntry : public AbstractSimpleTypeEntry{
public:
    JBooleanTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JDoubleTypeEntry : public AbstractSimpleTypeEntry{
public:
    JDoubleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class JFloatTypeEntry : public AbstractSimpleTypeEntry{
public:
    JFloatTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class NullptrTypeEntry : public AbstractSimpleTypeEntry{
public:
    NullptrTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
};

class InterfaceTypeEntry : public QtJambiTypeEntry{
public:
    bool isInterface() const final;
    const std::type_info* superType() const override;
    const QList<const std::type_info*>& interfaceTypes() const override;
    jclass creatableClass() const override;
    jclass implementationClass() const;
    size_t shellSize() const;
    const QVector<const FunctionInfo>* virtualFunctions() const;
    const QMap<size_t, uint>& interfaceOffsets() const;
    uint offset(const std::type_info& toType) const override;
    Destructor destructor() const;
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
                       const QList<const std::type_info*>& interface_types,
                       const QMap<size_t, uint>& interface_offsets,
                       size_t shell_size,
                       PtrDeleterFunction deleter,
                       PtrOwnerFunction owner_function,
                       const QVector<const FunctionInfo>* virtualFunctions,
                       Destructor destructor,
                       const QList<const PolymorphicIdHandler*>& polymorphicHandlers
                    );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
    jclass m_java_impl_class;
    jclass m_java_wrapper_class;
    const std::type_info* m_super_type;
    QList<const std::type_info*> m_interface_types;
    QMap<size_t, uint> m_interface_offsets;
    size_t m_shell_size;
    PtrDeleterFunction m_deleter;
    PtrOwnerFunction m_owner_function;
    const QVector<const FunctionInfo>* m_virtualFunctions;
    Destructor m_destructor;
    QList<const PolymorphicIdHandler*> m_polymorphicIdHandlers;
    friend class InterfaceValueTypeEntry;
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
                       const QList<const std::type_info*>& interface_types,
                       const QMap<size_t, uint>& interface_offsets,
                       size_t shell_size,
                       PtrDeleterFunction deleter,
                       PtrOwnerFunction owner_function,
                       const QVector<const FunctionInfo>* virtualFunctions,
                       Destructor destructor,
                       const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                       const QMetaType& qt_meta_type
                    );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
private:
    QMetaType m_qt_meta_type;
};

class FunctionalTypeEntry : public QtJambiTypeEntry{
public:
    bool isFunctional() const final;
    const QVector<const FunctionInfo>* virtualFunctions() const;
    jclass creatableClass() const override;
    Destructor destructor() const;
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
                        const QVector<const FunctionInfo>* virtualFunctions,
                        Destructor destructor,
                        FunctionalResolver registered_functional_resolver,
                        const QMetaType& qt_meta_type
                    );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
    size_t m_shell_size;
    jclass m_java_impl_class;
    jclass m_java_wrapper_class;
    const QVector<const FunctionInfo>* m_virtualFunctions;
    Destructor m_destructor;
    FunctionalResolver m_registered_functional_resolver;
    QMetaType m_qt_meta_type;
};

class ObjectTypeEntry : public QtJambiTypeEntry{
public:
    bool isObject() const final;
    const std::type_info* superType() const final;
    const QList<const std::type_info*>& interfaceTypes() const final;
    size_t shellSize() const;
    const QVector<const FunctionInfo>* virtualFunctions() const;
    const QMap<size_t, uint>& interfaceOffsets() const;
    Destructor destructor() const;
    uint offset(const std::type_info& toType) const override;
    ObjectTypeEntry(JNIEnv* env,
                    const std::type_info& typeId,
                    const char *qt_name,
                    const char *java_name,
                    jclass java_class,
                    jmethodID creator_method,
                    size_t value_size,
                    const std::type_info* super_type,
                    const QList<const std::type_info*>& interface_types,
                    const QMap<size_t, uint>& interface_offsets,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    PtrOwnerFunction owner_function,
                    const QVector<const FunctionInfo>* virtualFunctions,
                    Destructor destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers
                );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
    const std::type_info* m_super_type;
    QList<const std::type_info*> m_interface_types;
    QMap<size_t, uint> m_interface_offsets;
    size_t m_shell_size;
    PtrDeleterFunction m_deleter;
    PtrOwnerFunction m_owner_function;
    const QVector<const FunctionInfo>* m_virtualFunctions;
    Destructor m_destructor;
    QList<const PolymorphicIdHandler*> m_polymorphicIdHandlers;
    friend class ObjectValueTypeEntry;
    friend class ObjectContainerTypeEntry;
    friend class ObjectAbstractContainerTypeEntry;
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
                    const QList<const std::type_info*>& interface_types,
                    const QMap<size_t, uint>& interface_offsets,
                    size_t shell_size,
                    PtrDeleterFunction deleter,
                    PtrOwnerFunction owner_function,
                    const QVector<const FunctionInfo>* virtualFunctions,
                    Destructor destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    jclass java_wrapper_class
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
                    const QList<const std::type_info*>& interface_types,
                    const QMap<size_t, uint>& interface_offsets,
                    size_t shell_size,
                    PtrOwnerFunction owner_function,
                    const QVector<const FunctionInfo>* virtualFunctions,
                    Destructor destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    const QMetaType& qt_meta_type
                );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
private:
    QMetaType m_qt_meta_type;
    friend class ObjectContainerTypeEntry;
    friend class ObjectAbstractContainerTypeEntry;
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
                    const QList<const std::type_info*>& interface_types,
                    const QMap<size_t, uint>& interface_offsets,
                    size_t shell_size,
                    PtrOwnerFunction owner_function,
                    const QVector<const FunctionInfo>* virtualFunctions,
                    Destructor destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    const QMetaType& qt_meta_type,
                    jclass java_wrapper_class
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
                    const QList<const std::type_info*>& interface_types,
                    const QMap<size_t, uint>& interface_offsets,
                    size_t shell_size,
                    NewContainerAccessFunction containerAccessFactory,
                    PtrOwnerFunction owner_function,
                    const QVector<const FunctionInfo>* virtualFunctions,
                    Destructor destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    const QMetaType& qt_meta_type
                );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
private:
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
                    const QList<const std::type_info*>& interface_types,
                    const QMap<size_t, uint>& interface_offsets,
                    size_t shell_size,
                    NewContainerAccessFunction containerAccessFactory,
                    PtrOwnerFunction owner_function,
                    const QVector<const FunctionInfo>* virtualFunctions,
                    Destructor destructor,
                    const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                    const QMetaType& qt_meta_type,
                    jclass java_wrapper_class
                );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
private:
    NewContainerAccessFunction m_containerAccessFactory;
};

class QObjectTypeEntry : public QtJambiTypeEntry{
public:
    bool isQObject() const final;
    const std::type_info* superType() const override;
    const QList<const std::type_info*>& interfaceTypes() const final;
    size_t shellSize() const;
    const QVector<const FunctionInfo>* virtualFunctions() const;
    const QMetaObject* originalMetaObject() const;
    uint offset(const std::type_info& toType) const override;
    QObjectTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     const QList<const std::type_info*>& interface_types,
                     const QMap<size_t, uint>& interface_offsets,
                     size_t shell_size,
                     const QVector<const FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object
                );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
    bool convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const override;
private:
    const std::type_info* m_super_type;
    QList<const std::type_info*> m_interface_types;
    QMap<size_t, uint> m_interface_offsets;
    size_t m_shell_size;
    const QVector<const FunctionInfo>* m_virtualFunctions;
    const QMetaObject* m_original_meta_object;
};

class QObjectPolymorphicTypeEntry : public QObjectTypeEntry{
public:
    QObjectPolymorphicTypeEntry(JNIEnv* env,
                     const std::type_info& typeId,
                     const char *qt_name,
                     const char *java_name,
                     jclass java_class,
                     jmethodID creator_method,
                     size_t value_size,
                     const std::type_info* super_type,
                     const QList<const std::type_info*>& interface_types,
                     const QMap<size_t, uint>& interface_offsets,
                     size_t shell_size,
                     const QVector<const FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers
                );
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
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
                     const QList<const std::type_info*>& interface_types,
                     const QMap<size_t, uint>& interface_offsets,
                     size_t shell_size,
                     const QVector<const FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object);
    bool convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const override;
    bool convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const override;
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
                     const QList<const std::type_info*>& interface_types,
                     const QMap<size_t, uint>& interface_offsets,
                     size_t shell_size,
                     const QVector<const FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     jclass java_wrapper_class
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
                     const QList<const std::type_info*>& interface_types,
                     const QMap<size_t, uint>& interface_offsets,
                     size_t shell_size,
                     const QVector<const FunctionInfo>* virtualFunctions,
                     const QMetaObject* original_meta_object,
                     const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                     jclass java_wrapper_class
                );
    jclass creatableClass() const final;
private:
    jclass m_java_wrapper_class;
};

#endif // QTJAMBI_TYPEINFO_P_H
