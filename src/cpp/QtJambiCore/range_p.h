/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "pch_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,10,0)
#include <type_traits>
#include "utils_p.h"
#include <QtJambi/JObjectWrapper>
#include <QtJambi/CoreAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/ContainerAPI>

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
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    JObjectWrapper itemAccess;
#endif
    TreeType treeType;
    bool is_mutable_range;
    bool is_mutable_row;
    bool is_list_range;
    bool is_list_row;
    bool itemsAreQObjects;
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
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    void initializeTableItemAccess(QRangeModel *itemModel, GenericTable&& table);
    void initializeTreeItemAccess(QRangeModel *itemModel, GenericTable&& table);
#endif
    template<bool is_mutable_tree,
             bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             bool itemsAreQObjects,
             bool has_itemAccess,
             std::enable_if_t<!has_itemAccess,RowType> rowType,
             typename... Args>
    void initializeTree(QRangeModel *itemModel, Args&&... args);
    template<bool is_mutable_tree,
             bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             bool itemsAreQObjects,
             bool has_itemAccess,
             std::enable_if_t<has_itemAccess,RowType> rowType,
             typename... Args>
    void initializeTree(QRangeModel *itemModel, JObjectWrapper&& itemAccess, Args&&... args);
    template<bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             bool itemsAreQObjects,
             bool has_itemAccess,
             RowType rowType,
             typename... Args>
    void initializeTable(QRangeModel *itemModel, Args&&... args);
    QtPrivate::QQuasiVirtualInterface<QRangeModelImplBase>* impl = nullptr;
};

void containerDisposer(AbstractContainerAccess* _access);

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
namespace QSpanPrivate{
template <typename T, size_t E> auto adl_begin(QSpan<T,E> &c) { return QRangeModelDetails::adl_begin(c); }
}
#endif

template<typename Factory, typename Access>
void initializeGenericModel(JNIEnv* __jni_env,
                            jobject __jni_object,
                            void* container,
                            Access* containerAccess,
                            jobject rowCategoryJ,
                            Factory&& factory,
                            bool is_mutable_range = true){
    std::shared_ptr<int> treeColumnCount{new int{1}};
    TreeType treeType = TreeType::None;
    ClassInfo classInfo;
    const QMetaType& elementMetaType = containerAccess->elementMetaType();
    classInfo.javaClass = CoreAPI::getClassForMetaType(__jni_env, elementMetaType);
    if(!classInfo.javaClass){
        auto iter = containerAccess->constElementIterator(container);
        while(iter->hasNext()){
            jobject entry = iter->next(__jni_env);
            if(entry){
                jclass type2 = __jni_env->GetObjectClass(entry);
                if(!classInfo.javaClass)
                    classInfo.javaClass = type2;
                else if(!__jni_env->IsAssignableFrom(classInfo.javaClass, type2)){
                    do{
                        classInfo.javaClass = Java::Runtime::Class::getSuperclass(__jni_env, classInfo.javaClass);
                    }while(!__jni_env->IsAssignableFrom(classInfo.javaClass, type2));
                }
            }
        }
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
        if(!rowCategoryJ)
            rowCategoryJ = Java::QtCore::QRangeModel::rowCategory(__jni_env, __jni_object, classInfo.javaClass);
    }
    bool is_mutable_row = is_mutable_range;
    bool is_list_range;
    if constexpr(!std::is_same_v<Access,AbstractSpanAccess>){
        is_list_range = containerAccess->isList();
    }else{
        is_list_range = false;
    }
    bool is_list_row = false;
    bool itemsAreQObjects = Java::QtCore::QObject::isAssignableFrom(__jni_env, classInfo.javaClass);
    RowType rowType = ( elementMetaType.metaObject() && elementMetaType.metaObject()->propertyCount() - elementMetaType.metaObject()->propertyOffset() > 0)
                              || itemsAreQObjects ? RowType::MetaObject : RowType::Data;
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
    QRangeModel::RowCategory rowCategory = rowCategoryJ ? qtjambi_cast<QRangeModel::RowCategory>(__jni_env, rowCategoryJ) : QRangeModel::RowCategory::Default;
    jclass itemClass = classInfo.javaClass;
    if(rowCategory==QRangeModel::RowCategory::MultiRoleItem){
        rowType = RowType::Data;
    }else if(auto nestedContainerAccess = containerAccess->elementNestedContainerAccess()){
        rowType = rowCategory==QRangeModel::RowCategory::MultiRoleItem ? RowType::Data : RowType::Range;
        MultiRole::Type multiRoleType = MultiRole::isMultiRole(elementMetaType, nestedContainerAccess);
        is_list_row = nestedContainerAccess->isList();
        if(nestedContainerAccess->isSpan()){
            is_mutable_row = !static_cast<AbstractSpanAccess*>(nestedContainerAccess)->isConst();
        }
        QMetaType nestedElementMetaType;
        if(nestedContainerAccess->isSequential()){
            AbstractSequentialAccess* sequentialElementAccess = static_cast<AbstractSequentialAccess*>(nestedContainerAccess);
            nestedElementMetaType = sequentialElementAccess->elementMetaType();
        }
        else if(nestedContainerAccess->isAssociative()){
            AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(nestedContainerAccess);
            nestedElementMetaType = associativeElementAccess->valueMetaType();
        }
        else if(nestedContainerAccess->isPair()){
            AbstractPairAccess* pairElementAccess = static_cast<AbstractPairAccess*>(nestedContainerAccess);
            nestedElementMetaType = pairElementAccess->secondMetaType();
        }
        if(multiRoleType!=MultiRole::None){
            rowType = RowType::Data;
        }else if(nestedElementMetaType.flags() & QMetaType::IsPointer){
            subrow_pointertype = PointerType::Pointer;
        }else{
            QByteArrayView metaTypeName(nestedElementMetaType.name());
            if(metaTypeName.startsWith("QSharedPointer<")
                || metaTypeName.startsWith("QScopedPointer<")
                || metaTypeName.startsWith("std::shared_ptr<")
                || metaTypeName.startsWith("std::unique_ptr<")){
                subrow_pointertype = PointerType::SmartPointer;
            }
        }
        itemClass = CoreAPI::getClassForMetaType(__jni_env, nestedElementMetaType);

        if(!itemClass){
            auto iter = containerAccess->constElementIterator(container);
            if(nestedContainerAccess->isSequential()){
                AbstractSequentialAccess* sequentialElementAccess = static_cast<AbstractSequentialAccess*>(nestedContainerAccess);
                while(iter->hasNext()){
                    const void* nestedContainer = iter->constNext();
                    auto nestedIter = sequentialElementAccess->elementIterator(nestedContainer);
                    while(nestedIter->hasNext()){
                        jobject entry = nestedIter->next(__jni_env);
                        if(entry){
                            jclass type2 = __jni_env->GetObjectClass(entry);
                            if(!itemClass)
                                itemClass = type2;
                            else if(!__jni_env->IsAssignableFrom(itemClass, type2)){
                                do{
                                    itemClass = Java::Runtime::Class::getSuperclass(__jni_env, itemClass);
                                }while(!__jni_env->IsAssignableFrom(itemClass, type2));
                            }
                        }
                    }
                }
            }else if(nestedContainerAccess->isAssociative()){
                AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(nestedContainerAccess);
                while(iter->hasNext()){
                    const void* nestedContainer = iter->constNext();
                    auto nestedIter = associativeElementAccess->keyValueIterator(nestedContainer);
                    while(nestedIter->hasNext()){
                        QPair<jobject,jobject> entry = nestedIter->next(__jni_env);
                        if(entry.second){
                            jclass type2 = __jni_env->GetObjectClass(entry.second);
                            if(!itemClass)
                                itemClass = type2;
                            else if(!__jni_env->IsAssignableFrom(itemClass, type2)){
                                do{
                                    itemClass = Java::Runtime::Class::getSuperclass(__jni_env, itemClass);
                                }while(!__jni_env->IsAssignableFrom(itemClass, type2));
                            }
                        }
                    }
                }
            }
            else if(nestedContainerAccess->isPair()){
                AbstractPairAccess* pairElementAccess = static_cast<AbstractPairAccess*>(nestedContainerAccess);
                while(iter->hasNext()){
                    const void* nestedContainer = iter->constNext();
                    auto nestedIter = pairElementAccess->keyValueIterator(nestedContainer);
                    while(nestedIter->hasNext()){
                        QPair<jobject,jobject> entry = nestedIter->next(__jni_env);
                        if(entry.second){
                            jclass type2 = __jni_env->GetObjectClass(entry.second);
                            if(!itemClass)
                                itemClass = type2;
                            else if(!__jni_env->IsAssignableFrom(itemClass, type2)){
                                do{
                                    itemClass = Java::Runtime::Class::getSuperclass(__jni_env, itemClass);
                                }while(!__jni_env->IsAssignableFrom(itemClass, type2));
                            }
                        }
                    }
                }
            }
        }
        itemsAreQObjects = nestedElementMetaType.flags() & (QMetaType::PointerToQObject
                                                            | QMetaType::SharedPointerToQObject
                                                            | QMetaType::WeakPointerToQObject
                                                            | QMetaType::TrackingPointerToQObject)
                           || (itemClass && Java::QtCore::QObject::isAssignableFrom(__jni_env, itemClass));
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
        if constexpr(std::is_same_v<Access,AbstractSpanAccess>){
            if(treeType != TreeType::None){
                is_mutable_row &= !containerAccess->isConst();
            }
        }
    }
    QSharedPointer<AbstractSequentialAccess> sequentialAccess(containerAccess->clone(), &containerDisposer);
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    JObjectWrapper itemAccess{__jni_env, Java::QtCore::QRangeModel::itemAccess(__jni_env, __jni_object, itemClass ? itemClass : Java::Runtime::Object::getClass(__jni_env))};
    if(!itemAccess.isNull() && rowType==RowType::MetaObject){
        rowType = RowType::Data;
        *treeColumnCount = 1;
    }
#endif
    factory(GenericTable{
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
                         std::move(itemAccess),
#endif
                         treeType, is_mutable_range, is_mutable_row,
                         is_list_range, is_list_row, itemsAreQObjects, rowType, row_pointertype,
                         subrow_pointertype, __jni_env, container,
                         std::move(elementMetaType), std::move(sequentialAccess),
                         std::move(classInfo), std::move(treeColumnCount) });
}

template<typename Factory>
void initializeModelBySpanPointer(JNIEnv* __jni_env, jobject __jni_object, jobject range0, bool isConst, jobject rowCategory, Factory&& factory){
    QPair<void*,AbstractContainerAccess*> containerInfo = ContainerAPI::fromJavaOwner(__jni_env, range0);
    Q_ASSERT(containerInfo.first);
    QTJAMBI_CONTAINER_CAST(Span, containerAccess, containerInfo.second);
    isConst |= containerAccess->isConst();
    const QMetaType& elementMetaType = containerAccess->elementMetaType();
    switch(elementMetaType.id()){
    case QMetaType::SChar:
        if(isConst)
            factory(reinterpret_cast<QSpan<const signed char>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<signed char>*>(containerInfo.first));
        break;
    case QMetaType::Char:
        if(isConst)
            factory(reinterpret_cast<QSpan<const uchar>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<uchar>*>(containerInfo.first));
        break;
    case QMetaType::UChar:
        if(isConst)
            factory(reinterpret_cast<QSpan<const char>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<char>*>(containerInfo.first));
        break;
    case QMetaType::Char16:
        if(isConst)
            factory(reinterpret_cast<QSpan<const char16_t>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<char16_t>*>(containerInfo.first));
        break;
    case QMetaType::QChar:
        if(isConst)
            factory(reinterpret_cast<QSpan<const QChar>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<QChar>*>(containerInfo.first));
        break;
    case QMetaType::UShort:
        if(isConst)
            factory(reinterpret_cast<QSpan<const ushort>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<ushort>*>(containerInfo.first));
        break;
    case QMetaType::Short:
        if(isConst)
            factory(reinterpret_cast<QSpan<const short>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<short>*>(containerInfo.first));
        break;
    case QMetaType::UInt:
        if(isConst)
            factory(reinterpret_cast<QSpan<const uint>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<uint>*>(containerInfo.first));
        break;
    case QMetaType::Int:
        if(isConst)
            factory(reinterpret_cast<QSpan<const int>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<int>*>(containerInfo.first));
        break;
    case QMetaType::Char32:
        if(isConst)
            factory(reinterpret_cast<QSpan<const char32_t>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<char32_t>*>(containerInfo.first));
        break;
    case QMetaType::ULong:
        if(isConst)
            factory(reinterpret_cast<QSpan<const ulong>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<ulong>*>(containerInfo.first));
        break;
    case QMetaType::Long:
        if(isConst)
            factory(reinterpret_cast<QSpan<const long>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<long>*>(containerInfo.first));
        break;
    case QMetaType::ULongLong:
        if(isConst)
            factory(reinterpret_cast<QSpan<const unsigned long long>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<unsigned long long>*>(containerInfo.first));
        break;
    case QMetaType::LongLong:
        if(isConst)
            factory(reinterpret_cast<QSpan<const long long>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<long long>*>(containerInfo.first));
        break;
    case QMetaType::Bool:
        if(isConst)
            factory(reinterpret_cast<QSpan<const bool>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<bool>*>(containerInfo.first));
        break;
    case QMetaType::Float:
        if(isConst)
            factory(reinterpret_cast<QSpan<const float>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<float>*>(containerInfo.first));
        break;
    case QMetaType::Double:
        if(isConst)
            factory(reinterpret_cast<QSpan<const double>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<double>*>(containerInfo.first));
        break;
    case QMetaType::QString:
        if(isConst)
            factory(reinterpret_cast<QSpan<const QString>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<QString>*>(containerInfo.first));
        break;
    case QMetaType::QVariant:
        if(isConst)
            factory(reinterpret_cast<QSpan<const QVariant>*>(containerInfo.first));
        else
            factory(reinterpret_cast<QSpan<QVariant>*>(containerInfo.first));
        break;
    default:
        initializeGenericModel<Factory,AbstractSpanAccess>(__jni_env,
                                                           __jni_object,
                                                            containerInfo.first,
                                                           containerAccess,
                                                           rowCategory,
                                                           std::move(factory),
                                                           !isConst);
        break;
    }
}

template<typename Factory>
void initializeModelByListPointer(JNIEnv* __jni_env, jobject __jni_object, jobject range0, jobject rowCategory, Factory&& factory){
    QPair<void*,AbstractContainerAccess*> containerInfo = ContainerAPI::fromJavaOwner(__jni_env, range0);
    Q_ASSERT(containerInfo.first);
    QTJAMBI_CONTAINER_CAST(Sequential, containerAccess, containerInfo.second);
    switch(containerAccess->elementMetaType().id()){
    case QMetaType::SChar:
        factory(reinterpret_cast<QList<signed char>*>(containerInfo.first));
        break;
    case QMetaType::Char:
        factory(reinterpret_cast<QList<uchar>*>(containerInfo.first));
        break;
    case QMetaType::UChar:
        factory(reinterpret_cast<QList<char>*>(containerInfo.first));
        break;
    case QMetaType::Char16:
        factory(reinterpret_cast<QList<char16_t>*>(containerInfo.first));
        break;
    case QMetaType::QChar:
        factory(reinterpret_cast<QList<QChar>*>(containerInfo.first));
        break;
    case QMetaType::UShort:
        factory(reinterpret_cast<QList<ushort>*>(containerInfo.first));
        break;
    case QMetaType::Short:
        factory(reinterpret_cast<QList<short>*>(containerInfo.first));
        break;
    case QMetaType::UInt:
        factory(reinterpret_cast<QList<uint>*>(containerInfo.first));
        break;
    case QMetaType::Int:
        factory(reinterpret_cast<QList<int>*>(containerInfo.first));
        break;
    case QMetaType::Char32:
        factory(reinterpret_cast<QList<char32_t>*>(containerInfo.first));
        break;
    case QMetaType::ULong:
        factory(reinterpret_cast<QList<ulong>*>(containerInfo.first));
        break;
    case QMetaType::Long:
        factory(reinterpret_cast<QList<long>*>(containerInfo.first));
        break;
    case QMetaType::ULongLong:
        factory(reinterpret_cast<QList<unsigned long long>*>(containerInfo.first));
        break;
    case QMetaType::LongLong:
        factory(reinterpret_cast<QList<long long>*>(containerInfo.first));
        break;
    case QMetaType::Bool:
        factory(reinterpret_cast<QList<bool>*>(containerInfo.first));
        break;
    case QMetaType::Float:
        factory(reinterpret_cast<QList<float>*>(containerInfo.first));
        break;
    case QMetaType::Double:
        factory(reinterpret_cast<QList<double>*>(containerInfo.first));
        break;
    case QMetaType::QString:
        factory(reinterpret_cast<QList<QString>*>(containerInfo.first));
        break;
    case QMetaType::QVariant:
        factory(reinterpret_cast<QList<QVariant>*>(containerInfo.first));
        break;
    default:
        initializeGenericModel<Factory,AbstractSequentialAccess>(__jni_env,
                                                                 __jni_object,
                                                                 containerInfo.first,
                                                                 containerAccess,
                                                                 rowCategory,
                                                                 std::move(factory));
        break;
    }
}

#endif

#endif // RANGE_P_H
