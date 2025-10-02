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

#ifndef RANGE_P_H
#define RANGE_P_H

#if QT_VERSION >= QT_VERSION_CHECK(6,10,0)
#include <type_traits>
#include "utils_p.h"
#include <QtJambi/JObjectWrapper>
#include <QtJambi/CoreAPI>
#include <QtJambi/JavaAPI>

#include <QtCore/qrangemodel.h>

namespace MultiRole{

enum Type{
    None = 0,
    ItemDataRole,
    Integer,
    String
};

Type isMultiRole(const QMetaType& metaType, AbstractContainerAccess* containerAccess);

}

namespace MetaTypeUtils{

enum DataType{
    Value = 0,
    Pointer,
    QPointer,
    QSharedPointer,
    QWeakPointer,
    QSharedDataPointer,
    QExplicitlySharedDataPointer,
    QScopedPointer,
    shared_ptr,
    weak_ptr,
    unique_ptr
};

DataType dataType(const QMetaType& metaType);

}

enum class PointerType{
    None = 0,
    Pointer,
    SmartPointer
};

enum class RowType{
    Data = 0,
    MetaObject,
    Range
};

enum class TreeType{
    None = 0,
    ConstTree,
    MutableTree
};

struct ClassInfo{
    jclass javaClass = nullptr;
    jmethodID defaultConstructor = nullptr;
};

struct GenericTable{
    TreeType treeType;
    bool is_mutable_range;
    bool is_mutable_row;
    bool is_list_range;
    bool is_list_row;
    RowType rowType;
    PointerType row_pointertype;
    PointerType subrow_pointertype;
    JNIEnv* env;
    void* container;
    QMetaType elementMetaType;
    QSharedPointer<AbstractSequentialAccess> sequentialAccess;
    ClassInfo classInfo;
    std::shared_ptr<int> treeColumnCount;
};

namespace QRangeModelDetails{
template <>
struct range_traits<GenericTable> : std::true_type {};
}

template <>
class QGenericTableItemModelImpl<GenericTable> : public QRangeModelImplBase
{
public:
    explicit QGenericTableItemModelImpl(GenericTable &&model, QRangeModel *itemModel);
private:
    template <typename C = QGenericTableItemModelImpl<GenericTable>> using Methods = typename C::template MethodTemplates<C>;
    static void callImpl(size_t index, QtPrivate::QQuasiVirtualInterface<QRangeModelImplBase> &intf, void *ret, void *args);
    void initializeTable(QRangeModel *itemModel, GenericTable&& table);
    void initializeTree(QRangeModel *itemModel, GenericTable&& table);
    template<bool is_mutable_tree,
             bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             RowType rowType,
             typename... Args>
    void initializeTree(QRangeModel *itemModel, Args&&... args);
    template<bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             RowType rowType,
             typename... Args>
    void initializeTable(QRangeModel *itemModel, Args&&... args);
    QtPrivate::QQuasiVirtualInterface<QRangeModelImplBase>* impl = nullptr;
};

void containerDisposer(AbstractContainerAccess* _access);

template<typename T>
void initializeModel(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, T&& range, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options, jobject range0, QObject* parent);

inline bool initializeModel(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options, jobject range0,
                     void* container,
                     AbstractSequentialAccess* containerAccess, QRangeModel::RowCategory rowCategory, QObject* parent, bool isConst = false){
    std::shared_ptr<int> treeColumnCount{new int{1}};
    TreeType treeType = TreeType::None;
    ClassInfo classInfo;
    const QMetaType& elementMetaType = containerAccess->elementMetaType();
    if(elementMetaType==QMetaType::fromType<JObjectWrapper>()){
        auto iter = containerAccess->constElementIterator(container);
        while(iter->hasNext()){
            jobject entry = iter->next(__jni_env);
            if(entry){
                classInfo.javaClass = __jni_env->GetObjectClass(entry);
                break;
            }
        }
    }else{
        classInfo.javaClass = CoreAPI::getClassForMetaType(__jni_env, elementMetaType);
    }
    if(classInfo.javaClass){
        if(Java::QtCore::QRangeModel$ConstTreeRowInterface::isAssignableFrom(__jni_env, classInfo.javaClass)){
            treeType = TreeType::ConstTree;
            if(Java::QtCore::QRangeModel$TreeRowInterface::isAssignableFrom(__jni_env, classInfo.javaClass)){
                classInfo.defaultConstructor = __jni_env->GetMethodID(classInfo.javaClass, "<init>", "()V");
                if(__jni_env->ExceptionCheck())
                    __jni_env->ExceptionClear();
                if(classInfo.defaultConstructor){
                    treeType = TreeType::MutableTree;
                }
            }
        }
    }

    bool is_mutable_range = !isConst;
    if(AbstractSpanAccess* spanAccess = dynamic_cast<AbstractSpanAccess*>(containerAccess)){
        is_mutable_range &= !spanAccess->isConst();
    }
    bool is_mutable_row = is_mutable_range;
    bool is_list_range = dynamic_cast<AbstractListAccess*>(containerAccess);
    bool is_list_row = false;
    RowType rowType = elementMetaType.metaObject() ? RowType::MetaObject : RowType::Data;
    PointerType row_pointertype = PointerType::None;
    PointerType subrow_pointertype = PointerType::None;
    if(elementMetaType.flags() & QMetaType::IsPointer){
        row_pointertype = PointerType::Pointer;
    }else{
        QByteArrayView metaTypeName(elementMetaType.name());
        if(metaTypeName.startsWith("QSharedPointer<")
            || metaTypeName.startsWith("QScopedPointer<")
            || metaTypeName.startsWith("std::shared_ptr<")
            || metaTypeName.startsWith("std::unique_ptr<")){
            row_pointertype = PointerType::SmartPointer;
        }
    }
    if(rowCategory==QRangeModel::RowCategory::MultiRoleItem)
        rowType = RowType::Data;
    else if(auto nestedContainerAccess = containerAccess->elementNestedContainerAccess()){
        rowType = rowCategory==QRangeModel::RowCategory::MultiRoleItem ? RowType::Data : RowType::Range;
        MultiRole::Type multiRoleType = MultiRole::isMultiRole(elementMetaType, nestedContainerAccess);
        is_list_row = dynamic_cast<AbstractListAccess*>(nestedContainerAccess);
        if(AbstractSpanAccess* spanElementAccess = dynamic_cast<AbstractSpanAccess*>(nestedContainerAccess)){
            is_mutable_row = !spanElementAccess->isConst();
        }
        if(AbstractSequentialAccess* sequentialElementAccess = dynamic_cast<AbstractSequentialAccess*>(nestedContainerAccess)){
            if(sequentialElementAccess->elementMetaType().flags() & QMetaType::IsPointer){
                subrow_pointertype = PointerType::Pointer;
            }else{
                QByteArrayView metaTypeName(sequentialElementAccess->elementMetaType().name());
                if(metaTypeName.startsWith("QSharedPointer<")
                    || metaTypeName.startsWith("QScopedPointer<")
                    || metaTypeName.startsWith("std::shared_ptr<")
                    || metaTypeName.startsWith("std::unique_ptr<")){
                    subrow_pointertype = PointerType::SmartPointer;
                }
            }
        }
        if(AbstractAssociativeAccess* associativeElementAccess = dynamic_cast<AbstractAssociativeAccess*>(nestedContainerAccess)){
            if(multiRoleType!=MultiRole::None){
                rowType = RowType::Data;
            }else if(associativeElementAccess->valueMetaType().flags() & QMetaType::IsPointer){
                subrow_pointertype = PointerType::Pointer;
            }else{
                QByteArrayView metaTypeName(associativeElementAccess->valueMetaType().name());
                if(metaTypeName.startsWith("QSharedPointer<")
                    || metaTypeName.startsWith("QScopedPointer<")
                    || metaTypeName.startsWith("std::shared_ptr<")
                    || metaTypeName.startsWith("std::unique_ptr<")){
                    subrow_pointertype = PointerType::SmartPointer;
                }
            }
        }
        nestedContainerAccess->dispose();
    }else if(rowType==RowType::MetaObject){
        auto mo = elementMetaType.metaObject();
        if(treeType != TreeType::None){
            *treeColumnCount = mo->propertyCount() - mo->propertyOffset();
        }
        for(int i=mo->propertyOffset(), l=mo->propertyCount(); i<l; ++i){
            QMetaProperty prop = mo->property(i);
            if(prop.isWritable()){
                is_mutable_row = true;
                break;
            }
        }
        if(treeType != TreeType::None){
            if(AbstractSpanAccess* spanAccess = dynamic_cast<AbstractSpanAccess*>(containerAccess)){
                is_mutable_row &= !spanAccess->isConst();
            }
        }
    }
    QSharedPointer<AbstractSequentialAccess> sequentialAccess(containerAccess->clone(), &containerDisposer);

    initializeModel(__qtjambi_ptr, __jni_env, __jni_object, GenericTable{treeType, is_mutable_range, is_mutable_row,
                                                                         is_list_range, is_list_row, rowType, row_pointertype,
                                                                         subrow_pointertype, __jni_env, container,
                                                                         std::move(elementMetaType), std::move(sequentialAccess),
                                                                         std::move(classInfo), std::move(treeColumnCount)}, __qtjambi_constructor_options, range0, parent);
    return true;
}

template<typename T>
void initializeModelBySpanPointer(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, void* container, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options, jobject range0, QObject* parent){
    initializeModel(__qtjambi_ptr, __jni_env, __jni_object, reinterpret_cast<QSpan<T>*>(container), __qtjambi_constructor_options, range0, parent);
}

inline bool initializeModelBySpanPointer(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options, jobject range0, bool isConst, QRangeModel::RowCategory rowCategory, QObject* parent){
    QPair<void*,AbstractContainerAccess*> containerInfo = ContainerAPI::fromJavaOwner(__jni_env, range0);
    Q_ASSERT(containerInfo.first);
    AbstractSpanAccess* containerAccess = dynamic_cast<AbstractSpanAccess*>(containerInfo.second);
    Q_ASSERT(containerAccess);
    const QMetaType& elementMetaType = containerAccess->elementMetaType();
    if(containerAccess->isConst() || isConst){
        switch(elementMetaType.id()){
        case QMetaType::SChar:
            initializeModelBySpanPointer<const signed char>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Char:
            initializeModelBySpanPointer<const uchar>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::UChar:
            initializeModelBySpanPointer<const char>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Char16:
            initializeModelBySpanPointer<const char16_t>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::QChar:
            initializeModelBySpanPointer<const QChar>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::UShort:
            initializeModelBySpanPointer<const ushort>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Short:
            initializeModelBySpanPointer<const short>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::UInt:
            initializeModelBySpanPointer<const uint>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Int:
            initializeModelBySpanPointer<const int>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Char32:
            initializeModelBySpanPointer<const char32_t>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::ULong:
            initializeModelBySpanPointer<const ulong>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Long:
            initializeModelBySpanPointer<const long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::ULongLong:
            initializeModelBySpanPointer<const unsigned long long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::LongLong:
            initializeModelBySpanPointer<const long long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Bool:
            initializeModelBySpanPointer<const bool>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Float:
            initializeModelBySpanPointer<const float>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Double:
            initializeModelBySpanPointer<const double>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::QString:
            initializeModelBySpanPointer<const QString>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        default:
            break;
        }
    }else{
        switch(elementMetaType.id()){
        case QMetaType::SChar:
            initializeModelBySpanPointer<signed char>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Char:
            initializeModelBySpanPointer<uchar>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::UChar:
            initializeModelBySpanPointer<char>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Char16:
            initializeModelBySpanPointer<char16_t>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::QChar:
            initializeModelBySpanPointer<QChar>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::UShort:
            initializeModelBySpanPointer<ushort>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Short:
            initializeModelBySpanPointer<short>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::UInt:
            initializeModelBySpanPointer<uint>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Int:
            initializeModelBySpanPointer<int>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Char32:
            initializeModelBySpanPointer<char32_t>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::ULong:
            initializeModelBySpanPointer<ulong>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Long:
            initializeModelBySpanPointer<long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::ULongLong:
            initializeModelBySpanPointer<unsigned long long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::LongLong:
            initializeModelBySpanPointer<long long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Bool:
            initializeModelBySpanPointer<bool>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Float:
            initializeModelBySpanPointer<float>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::Double:
            initializeModelBySpanPointer<double>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        case QMetaType::QString:
            initializeModelBySpanPointer<QString>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
            return true;
        default:
            break;
        }
    }
    return initializeModel(__qtjambi_ptr,
                           __jni_env,
                           __jni_object,
                           __qtjambi_constructor_options,
                           range0,
                           containerInfo.first,
                           containerAccess,
                           rowCategory,
                           parent,
                           isConst);
}

template<template<typename> class Container, typename T>
void initializeModelBySequentialPointer(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, void* list, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options, jobject range0, QObject* parent){
    initializeModel(__qtjambi_ptr, __jni_env, __jni_object, reinterpret_cast<Container<T>*>(list), __qtjambi_constructor_options, range0, parent);
}

inline bool initializeModelByListPointer(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options, jobject range0, QRangeModel::RowCategory rowCategory, QObject* parent){
    QPair<void*,AbstractContainerAccess*> containerInfo = ContainerAPI::fromJavaOwner(__jni_env, range0);
    Q_ASSERT(containerInfo.first);
    AbstractSequentialAccess* containerAccess = dynamic_cast<AbstractSequentialAccess*>(containerInfo.second);
    Q_ASSERT(containerAccess);
    switch(containerAccess->elementMetaType().id()){
    case QMetaType::SChar:
        initializeModelBySequentialPointer<QList, signed char>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Char:
        initializeModelBySequentialPointer<QList, uchar>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::UChar:
        initializeModelBySequentialPointer<QList, char>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Char16:
        initializeModelBySequentialPointer<QList, char16_t>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::QChar:
        initializeModelBySequentialPointer<QList, QChar>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::UShort:
        initializeModelBySequentialPointer<QList, ushort>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Short:
        initializeModelBySequentialPointer<QList, short>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::UInt:
        initializeModelBySequentialPointer<QList, uint>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Int:
        initializeModelBySequentialPointer<QList, int>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Char32:
        initializeModelBySequentialPointer<QList, char32_t>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::ULong:
        initializeModelBySequentialPointer<QList, ulong>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Long:
        initializeModelBySequentialPointer<QList, long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::ULongLong:
        initializeModelBySequentialPointer<QList, unsigned long long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::LongLong:
        initializeModelBySequentialPointer<QList, long long>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Bool:
        initializeModelBySequentialPointer<QList, bool>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Float:
        initializeModelBySequentialPointer<QList, float>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::Double:
        initializeModelBySequentialPointer<QList, double>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    case QMetaType::QString:
        initializeModelBySequentialPointer<QList, QString>(__qtjambi_ptr, __jni_env, __jni_object, containerInfo.first, __qtjambi_constructor_options, range0, parent);
        return true;
    default:
        break;
    }
    return initializeModel(__qtjambi_ptr,
                           __jni_env,
                           __jni_object,
                           __qtjambi_constructor_options,
                           range0,
                           containerInfo.first,
                           containerAccess,
                           rowCategory,
                           parent);
}


#endif

#endif // RANGE_P_H
