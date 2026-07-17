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

#ifndef RANGE_P_P_H
#define RANGE_P_P_H

#include "range_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,10,0)

bool logRangeModel();

template<typename ForwardIt>
void rotate_by_swap(ForwardIt first, ForwardIt middle, ForwardIt last) {
    if (first == middle || middle == last) return;

    ForwardIt read = middle;
    ForwardIt write = first;
    ForwardIt next = middle;

    while (read != last) {
        swap(*write++, *read++);
        if (write == middle) {
            middle = next;
            next = read;
        }
    }

    // rotate the remaining range if needed
    while (next != last) {
        read = next;
        while (read != last && write != middle) {
            swap(*write++, *read++);
        }
        if (write == middle) {
            middle = next;
            next = read;
        }
    }
}

struct PropertyRef{
    void* m_gadget;
    const bool m_isGadget;
    QMetaProperty m_property;
    operator QVariant() const;
    PropertyRef& operator=(const QVariant&);
};

class ConstMetaPropertyIterator{
    const void* m_gadget;
    const QMetaObject* m_metaObject;
    const bool m_isGadget;
    int m_index = 0;
    QMetaProperty m_current;
    QMetaType m_currentMetaType;
public:
    typedef std::forward_iterator_tag iterator_category;
    typedef qptrdiff difference_type;
    typedef QVariant value_type;
    typedef value_type*pointer;
    typedef value_type&reference;
    ConstMetaPropertyIterator(const void* gadget, const QMetaType& metaType, int index = 0);
    ConstMetaPropertyIterator(const ConstMetaPropertyIterator& other);
    ~ConstMetaPropertyIterator();
public:
    QVariant operator*();
    bool operator==(const ConstMetaPropertyIterator& other) const;
    inline bool operator!=(const ConstMetaPropertyIterator& other) const{
        return !operator==(other);
    }
    ConstMetaPropertyIterator& operator++();
    ConstMetaPropertyIterator operator++(int);
};

class MetaPropertyIterator{
    void* m_gadget;
    const QMetaObject* m_metaObject;
    const bool m_isGadget;
    int m_index = 0;
    QMetaProperty m_current;
    QMetaType m_currentMetaType;
public:
    typedef std::forward_iterator_tag iterator_category;
    typedef qptrdiff difference_type;
    typedef QVariant value_type;
    typedef value_type*pointer;
    typedef value_type&reference;
    MetaPropertyIterator(void* gadget, const QMetaType& metaType, int index = 0);
    MetaPropertyIterator(const MetaPropertyIterator& other);
    ~MetaPropertyIterator();
public:
    PropertyRef operator*();
    MetaPropertyIterator& operator=(MetaPropertyIterator&& other);
    bool operator==(const MetaPropertyIterator& other) const;
    inline bool operator!=(const MetaPropertyIterator& other) const{
        return !operator==(other);
    }
    MetaPropertyIterator& operator++();
    MetaPropertyIterator operator++(int);
};

template<bool has_itemAccess, RowType _rowType = RowType::Data, bool is_extensible_row = false, bool _is_mutable_row = false>
class ConstIterator;
template<bool has_itemAccess, RowType _rowType = RowType::Data, bool is_extensible_row = false, bool _is_mutable_row = false>
class Iterator;
template<typename Super, bool is_mutable>
struct MetaObjectRow;
template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType, typename... Args>
struct TreeRangeWrapper;

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         RowType rowType,
         typename... Args>
struct TreeRangeItemAccessWrapper;

template<typename RangeRowType>
struct ConstRangeRow;

template<typename RangeRowType>
struct MutableRangeRow;

template<typename RangeRowType>
struct ExtensibleRangeRow;

template<typename RangeRowType, bool is_extensible, bool is_mutable>
using RangeRow = std::conditional_t<is_mutable, std::conditional_t<is_extensible, ExtensibleRangeRow<RangeRowType>, MutableRangeRow<RangeRowType>>, ConstRangeRow<RangeRowType>>;

template<bool is_mutable_row,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType>
struct MutableRangeWrapper;

template<bool is_mutable_row,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType>
struct ConstRangeWrapper;

template<typename Super>
struct ListRangeWrapper;

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType>
using RangeWrapper = std::conditional_t<is_mutable_range, MutableRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,has_itemAccess,rowType>, ConstRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,has_itemAccess,rowType>>;

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType,
         bool root>
using UnmutableTreeRangeWrapper = std::conditional_t<root && has_itemAccess,
                                                     TreeRangeItemAccessWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,rowType>,
                                                     TreeRangeWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType>>;

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType,
         bool root>
using MutableTreeRangeWrapper = std::conditional_t<root && has_itemAccess,
                                                   TreeRangeItemAccessWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,rowType,ClassInfo>,
                                                   TreeRangeWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType,ClassInfo>>;

template <TreeType treeType,
         bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType,
         bool root = false>
using RangeWrapperType = std::conditional_t<treeType==TreeType::MutableTree, MutableTreeRangeWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType,root>,
                                            std::conditional_t<treeType==TreeType::ConstTree, UnmutableTreeRangeWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType,root>,
                                                               std::conditional_t<is_list_range, ListRangeWrapper<RangeWrapper<is_mutable_range,is_mutable_row,is_list_row,itemsAreQObjects,has_itemAccess,rowType>>, RangeWrapper<is_mutable_range,is_mutable_row,is_list_row,itemsAreQObjects,has_itemAccess,rowType>>>>;

template<bool mutable_row>
struct AbstractRow{
    static constexpr bool mutableRow = mutable_row;
    typedef std::conditional_t<mutable_row, void*, const void*> PointerType;
    PointerType data;
    QMetaType metaType;
    MetaTypeUtils::DataType dataType;
    QSharedPointer<AbstractContainerAccess> containerAccess;
    friend struct AbstractRow<false>;
protected:
    bool needsDeletion;
    bool initialize(PointerType _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess){
        data = _data;
        metaType = _metaType;
        dataType = _dataType;
        containerAccess = _containerAccess;
        return true;
    }
    void swap(AbstractRow<mutable_row>& other) noexcept{
        if(metaType.flags() & QMetaType::IsPointer){
            void* tmp = *reinterpret_cast<void*const*>(other.data);
            const_cast<void*&>(*reinterpret_cast<void*const*>(other.data)) = *reinterpret_cast<void*const*>(data);
            const_cast<void*&>(*reinterpret_cast<void*const*>(data)) = tmp;
        }else{
            void* tmp = metaType.create(other.data);
            metaType.destruct(const_cast<void*>(other.data));
            metaType.construct(const_cast<void*>(other.data), data);
            metaType.construct(const_cast<void*>(data), tmp);
            metaType.destroy(tmp);
        }
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractRow<mutable_row>& operator=(const AbstractRow<m>& other){
        if(needsDeletion && data){
            if(metaType.flags() & QMetaType::IsPointer){
                delete reinterpret_cast<void*const*>(data);
            }else{
                metaType.destroy(const_cast<void*>(data));
            }
        }
        data = other.data;
        metaType = other.metaType;
        dataType = other.dataType;
        containerAccess = other.containerAccess;
        needsDeletion = other.needsDeletion;
        if(needsDeletion){
            data = metaType.create(const_cast<void*>(data));
        }
        return *this;
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractRow<mutable_row>& operator=(AbstractRow<m>&& other){
        data = std::move(other.data);
        metaType = std::move(other.metaType);
        dataType = std::move(other.dataType);
        containerAccess = std::move(other.containerAccess);
        needsDeletion = std::move(other.needsDeletion);
        return *this;
    }
    AbstractRow(PointerType _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess)
        : data(_data), metaType(_metaType), dataType(_dataType), containerAccess(_containerAccess), needsDeletion(false)
    {}
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractRow(const AbstractRow<m>& other)
        : data(other.data), metaType(other.metaType), dataType(other.dataType), containerAccess(other.containerAccess), needsDeletion(other.needsDeletion)
    {
        if(needsDeletion){
            data = metaType.create(const_cast<void*>(data));
        }
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractRow(AbstractRow<m>&& other)
        : data(std::move(other.data)), metaType(std::move(other.metaType)), dataType(std::move(other.dataType)), containerAccess(std::move(other.containerAccess)), needsDeletion(std::move(other.needsDeletion))
    {
    }
    AbstractRow()
        : data(nullptr), metaType(), dataType(MetaTypeUtils::Value), containerAccess(nullptr), needsDeletion(false)
    {}
public:
    ~AbstractRow(){
        if(needsDeletion && data){
            if(metaType.flags() & QMetaType::IsPointer){
                delete reinterpret_cast<void*const*>(data);
            }else{
                metaType.destroy(const_cast<void*>(data));
            }
        }
    }
    PointerType pointer()const{
        if(data){
            switch(dataType){
            case MetaTypeUtils::Pointer:
                return *reinterpret_cast<void*const*>(data);
            case MetaTypeUtils::QPointer:
                return reinterpret_cast<const QPointer<QObject>*>(data)->get();
            case MetaTypeUtils::QSharedPointer:
                return reinterpret_cast<const QSharedPointer<char>*>(data)->get();
            case MetaTypeUtils::QWeakPointer:
                return QSharedPointer<char>(*reinterpret_cast<const QWeakPointer<char>*>(data)).get();
            case MetaTypeUtils::QSharedDataPointer:
                return const_cast<QSharedData*>(reinterpret_cast<const QSharedDataPointer<QSharedData>*>(data)->get());
            case MetaTypeUtils::QExplicitlySharedDataPointer:
                return reinterpret_cast<const QExplicitlySharedDataPointer<QSharedData>*>(data)->get();
            case MetaTypeUtils::QScopedPointer:
                return reinterpret_cast<const QScopedPointer<char>*>(data)->get();
            case MetaTypeUtils::shared_ptr:
                return reinterpret_cast<const std::shared_ptr<char>*>(data)->get();
            case MetaTypeUtils::weak_ptr:
                return std::shared_ptr<char>(*reinterpret_cast<const std::weak_ptr<char>*>(data)).get();
            case MetaTypeUtils::unique_ptr:
                return reinterpret_cast<const std::unique_ptr<char>*>(data)->get();
            default:
                break;
            }
        }
        return data;
    }
    bool isConst() const {return !mutable_row;}
};

template<bool has_itemAccess,bool mutable_row>
struct AbstractAccessRow : AbstractRow<mutable_row>{
    template<typename>
    friend struct ListRangeWrapper;
    template<bool,bool>
    friend struct AbstractAccessRow;
    using PointerType = typename AbstractRow<mutable_row>::PointerType;
    using AbstractRow<mutable_row>::data;
    using AbstractRow<mutable_row>::metaType;
    using AbstractRow<mutable_row>::dataType;
    using AbstractRow<mutable_row>::containerAccess;
    static constexpr bool hasItemAccess = has_itemAccess;
public:
    AbstractAccessRow() = default;
    AbstractAccessRow(PointerType _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess)
        : AbstractRow<mutable_row>(_data, _metaType, _dataType, _containerAccess){}
    AbstractAccessRow(PointerType _data, QMetaType&& _metaType, MetaTypeUtils::DataType _dataType, QSharedPointer<AbstractContainerAccess>&& _containerAccess)
        : AbstractRow<mutable_row>(_data, std::move(_metaType), _dataType, std::move(_containerAccess)){}
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow(const AbstractAccessRow<has_itemAccess,m>& other)
        : AbstractRow<mutable_row>(other){}
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow(AbstractAccessRow<has_itemAccess,m>&& other)
        : AbstractRow<mutable_row>(std::move(other)){}

    template<typename Value>
    auto initialize(const Value& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(AbstractRow<mutable_row>::metaType)>,bool> {
        return AbstractRow<mutable_row>::initialize(value.data, value.metaType, value.dataType, value.containerAccess);
    }
    template<typename Value>
    auto initialize(Value&& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(AbstractRow<mutable_row>::metaType)>,bool> {
        return AbstractRow<mutable_row>::initialize(value.data, std::move(value.metaType), value.dataType, std::move(value.containerAccess));
    }
    void swap(AbstractAccessRow<has_itemAccess,mutable_row>& other) noexcept{
        AbstractRow<mutable_row>::swap(other);
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow<has_itemAccess,mutable_row>& operator=(const AbstractAccessRow<has_itemAccess,m>& other) noexcept{
        AbstractRow<mutable_row>::operator=(other);
        return *this;
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow<has_itemAccess,mutable_row>& operator=(AbstractAccessRow<has_itemAccess,m>&& other) noexcept{
        AbstractRow<mutable_row>::operator=(std::move(other));
        return *this;
    }
    friend void swap(AbstractAccessRow<has_itemAccess,mutable_row>&& _this, AbstractAccessRow<has_itemAccess,mutable_row>&& other) noexcept{
        _this.swap(other);
    }
    friend void swap(AbstractAccessRow<has_itemAccess,mutable_row>& _this, AbstractAccessRow<has_itemAccess,mutable_row>& other) noexcept{
        _this.swap(other);
    }
    friend void qSwap(AbstractAccessRow<has_itemAccess,mutable_row>& _this, AbstractAccessRow<has_itemAccess,mutable_row>& other) noexcept{
        _this.swap(other);
    }
    jobject asJObject(JNIEnv*) const { return nullptr; }
};

template<bool mutable_row>
struct AbstractAccessRow<true,mutable_row> : AbstractRow<mutable_row>{
    template<typename>
    friend struct ListRangeWrapper;
    template<bool,bool>
    friend struct AbstractAccessRow;
    using PointerType = typename AbstractRow<mutable_row>::PointerType;
    using AbstractRow<mutable_row>::data;
    using AbstractRow<mutable_row>::metaType;
    using AbstractRow<mutable_row>::dataType;
    using AbstractRow<mutable_row>::containerAccess;
    std::function<jobject(JNIEnv*,const void*)> converter;
    static constexpr bool hasItemAccess = true;
public:
    AbstractAccessRow<true,mutable_row>() = default;
    AbstractAccessRow<true,mutable_row>(PointerType _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess, const std::function<jobject(JNIEnv*,const void*)>& _converter)
        : AbstractRow<mutable_row>(_data, _metaType, _dataType, _containerAccess), converter(_converter) {
        Q_ASSERT(!data || converter);
    }
    AbstractAccessRow<true,mutable_row>(PointerType _data, QMetaType&& _metaType, MetaTypeUtils::DataType _dataType, QSharedPointer<AbstractContainerAccess>&& _containerAccess, std::function<jobject(JNIEnv*,const void*)>&& _converter)
        : AbstractRow<mutable_row>(_data, std::move(_metaType), _dataType, std::move(_containerAccess)), converter(std::move(_converter)) {
        Q_ASSERT(!data || converter);
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow<true,mutable_row>(const AbstractAccessRow<true,m>& other)
        : AbstractRow<mutable_row>(other), converter(other.converter){
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow<true,mutable_row>(AbstractAccessRow<true,m>&& other)
        : AbstractRow<mutable_row>(std::move(other)), converter(std::move(other.converter)){
    }

    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow<true,mutable_row>& operator=(const AbstractAccessRow<true,m>& other) noexcept{
        AbstractRow<mutable_row>::operator=(other);
        converter = other.converter;
        return *this;
    }
    template<bool m = mutable_row, std::enable_if_t<!mutable_row || !m, bool> = true>
    AbstractAccessRow<true,mutable_row>& operator=(AbstractAccessRow<true,m>&& other) noexcept{
        AbstractRow<mutable_row>::operator=(std::move(other));
        converter = std::move(other.converter);
        return *this;
    }
    void swap(AbstractAccessRow<true,mutable_row>& other) noexcept{
        AbstractRow<mutable_row>::swap(other);
        converter.swap(other.converter);
    }
    friend void swap(AbstractAccessRow<true,mutable_row>&& _this, AbstractAccessRow<true,mutable_row>&& other) noexcept{
        _this.swap(other);
    }
    friend void swap(AbstractAccessRow<true,mutable_row>& _this, AbstractAccessRow<true,mutable_row>& other) noexcept{
        _this.swap(other);
    }
    friend void qSwap(AbstractAccessRow<true,mutable_row>& _this, AbstractAccessRow<true,mutable_row>& other) noexcept{
        _this.swap(other);
    }
    jobject asJObject(JNIEnv* env) const { return converter(env, data); }
    template<typename Value>
    auto initialize(const Value& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(metaType)>,bool> {
        return initialize(value.data, value.metaType, value.dataType, value.containerAccess, value.converter);
    }
    template<typename Value>
    auto initialize(Value&& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(metaType)>,bool> {
        return initialize(value.data, std::move(value.metaType), value.dataType, std::move(value.containerAccess), std::move(value.converter));
    }
protected:
    bool initialize(PointerType _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess, const std::function<jobject(JNIEnv*,const void*)>& _converter){
        AbstractRow<mutable_row>::initialize(_data, _metaType, _dataType, _containerAccess);
        converter = _converter;
        Q_ASSERT(!data || converter);
        return true;
    }
    bool initialize(PointerType _data, QMetaType&& _metaType, MetaTypeUtils::DataType _dataType, QSharedPointer<AbstractContainerAccess>&& _containerAccess, std::function<jobject(JNIEnv*,const void*)>&& _converter){
        AbstractRow<mutable_row>::initialize(_data, std::move(_metaType), std::move(_dataType), std::move(_containerAccess));
        converter = std::move(_converter);
        Q_ASSERT(!data || converter);
        return true;
    }
};

template<bool has_itemAccess>
using MutableRow = AbstractAccessRow<has_itemAccess,true>;
template<bool has_itemAccess>
using ConstRow = AbstractAccessRow<has_itemAccess,false>;

template<bool>
class AbstractConstIterator;
class AbstractConstIteratorBase;

class AbstractIteratorBase{
protected:
    void* current_data;
    QMetaType current_metaType;
    MetaTypeUtils::DataType current_dataType;
    QSharedPointer<AbstractContainerAccess> current_containerAccess;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> iter;
    AbstractIteratorBase(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter);
    AbstractIteratorBase(const AbstractIteratorBase& other);
    AbstractIteratorBase(AbstractIteratorBase&& other);
    AbstractIteratorBase& operator=(const AbstractIteratorBase& other);
    void swap(AbstractIteratorBase& other) noexcept;
    AbstractIteratorBase& operator=(AbstractIteratorBase&& other);
public:
    bool operator==(const AbstractIteratorBase& other) const;
    bool operator!=(const AbstractIteratorBase& other) const;
    bool operator==(const AbstractConstIteratorBase& other) const;
    bool operator!=(const AbstractConstIteratorBase& other) const;
    bool isConst() const;
    friend AbstractConstIteratorBase;
};

class AbstractConstIteratorBase{
protected:
    const void* current_data;
    QMetaType current_metaType;
    MetaTypeUtils::DataType current_dataType;
    QSharedPointer<AbstractContainerAccess> current_containerAccess;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> iter;
    AbstractConstIteratorBase(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter);
    AbstractConstIteratorBase(const AbstractConstIteratorBase& other);
    AbstractConstIteratorBase(AbstractConstIteratorBase&& other);
    AbstractConstIteratorBase(const AbstractIteratorBase& other);
    AbstractConstIteratorBase(AbstractIteratorBase&& other);
    AbstractConstIteratorBase& operator=(const AbstractConstIteratorBase& other);
    AbstractConstIteratorBase& operator=(const AbstractIteratorBase& other);
    void swap(AbstractConstIteratorBase& other) noexcept;
    AbstractConstIteratorBase& operator=(AbstractConstIteratorBase&& other);
    AbstractConstIteratorBase& operator=(AbstractIteratorBase&& other);
public:
    bool operator==(const AbstractConstIteratorBase& other) const;
    bool operator!=(const AbstractConstIteratorBase& other) const;
    bool operator==(const AbstractIteratorBase& other) const;
    bool operator!=(const AbstractIteratorBase& other) const;
    bool isConst() const;
    friend AbstractIteratorBase;
};

template<bool has_itemAccess>
class AbstractIterator : public AbstractIteratorBase{
public:
    typedef std::forward_iterator_tag iterator_category;
    typedef qptrdiff difference_type;
    friend AbstractConstIterator<has_itemAccess>;
public:
    using AbstractIteratorBase::AbstractIteratorBase;
    AbstractIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter, bool)
        : AbstractIteratorBase(std::move(_iter)) {}
    AbstractIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
        : AbstractIteratorBase(std::move(_iter)) {
        if(iter){
            if(!iter->isConst()){
                operator++();
            }else{
                iter.reset();
            }
        }
    }
    friend void swap(AbstractIterator& _this, AbstractIterator& other) noexcept{
        _this.swap(other);
    }
    friend void qSwap(AbstractIterator& _this, AbstractIterator& other) noexcept{
        _this.swap(other);
    }
    AbstractIterator& operator++(){
        if(iter){
            if(iter->hasNext()){
                QMetaType nextType = iter->elementMetaType();
                if(nextType!=current_metaType){
                    current_dataType = MetaTypeUtils::dataType(nextType);
                    current_metaType = std::move(nextType);
                }
                current_containerAccess = QSharedPointer<AbstractContainerAccess>(iter->elementNestedContainerAccess(), &containerDisposer);
                current_data = iter->mutableNext();
            }else{
                current_data = nullptr;
                iter.reset();
            }
        }
        return *this;
    }
    AbstractIterator operator++(int){
        AbstractIterator copy(*this);
        operator++();
        return copy;
    }
    bool operator==(const AbstractIterator& other) const { return AbstractIteratorBase::operator==(other); }
    bool operator!=(const AbstractIterator& other) const { return AbstractIteratorBase::operator!=(other); }
};

template<>
class AbstractIterator<true> : public AbstractIterator<false>{
protected:
    std::function<jobject(JNIEnv*,const void*)> current_converter;
    friend AbstractConstIterator<true>;
public:
    using AbstractIterator<false>::AbstractIterator;
    AbstractIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
        : AbstractIterator<false>(std::move(_iter),true) {
        if(iter){
            if(!iter->isConst()){
                operator++();
            }else{
                iter.reset();
            }
        }
    }
    friend void swap(AbstractIterator& _this, AbstractIterator& other) noexcept{
        _this.swap(other);
    }
    friend void qSwap(AbstractIterator& _this, AbstractIterator& other) noexcept{
        _this.swap(other);
    }
    AbstractIterator& operator++(){
        if(iter){
            if(iter->hasNext()){
                QMetaType nextType = iter->elementMetaType();
                if(nextType!=current_metaType){
                    current_dataType = MetaTypeUtils::dataType(nextType);
                    current_converter = iter->elementConverter();
                    current_metaType = std::move(nextType);
                }
                current_containerAccess = QSharedPointer<AbstractContainerAccess>(iter->elementNestedContainerAccess(), &containerDisposer);
                current_data = iter->mutableNext();
            }else{
                current_data = nullptr;
                iter.reset();
            }
        }
        return *this;
    }
    AbstractIterator operator++(int){
        AbstractIterator copy(*this);
        operator++();
        return copy;
    }
    bool operator==(const AbstractIterator& other) const { return AbstractIteratorBase::operator==(other); }
    bool operator!=(const AbstractIterator& other) const { return AbstractIteratorBase::operator!=(other); }
};

template<bool has_itemAccess>
class AbstractConstIterator : public AbstractConstIteratorBase{
public:
    typedef std::forward_iterator_tag iterator_category;
    typedef qptrdiff difference_type;
public:
    using AbstractConstIteratorBase::AbstractConstIteratorBase;
    AbstractConstIterator(const AbstractIterator<has_itemAccess>& other)
     : AbstractConstIteratorBase(other) {}
    AbstractConstIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
        : AbstractConstIteratorBase(std::move(_iter)) {
        operator++();
    }
    AbstractConstIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter, bool)
        : AbstractConstIteratorBase(std::move(_iter)) {}
    friend void swap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept{
        _this.swap(other);
    }
    friend void qSwap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept{
        _this.swap(other);
    }
    AbstractConstIterator& operator++(){
        if(iter){
            if(iter->hasNext()){
                QMetaType nextType = iter->elementMetaType();
                if(nextType!=current_metaType){
                    current_dataType = MetaTypeUtils::dataType(nextType);
                    current_metaType = std::move(nextType);
                }
                current_containerAccess = QSharedPointer<AbstractContainerAccess>(iter->elementNestedContainerAccess(), &containerDisposer);
                current_data = iter->constNext();
            }else{
                current_data = nullptr;
                iter.reset();
            }
        }
        return *this;
    }
    AbstractConstIterator operator++(int){
        AbstractConstIterator copy(*this);
        operator++();
        return copy;
    }
    bool operator==(const AbstractConstIterator& other) const { return AbstractConstIteratorBase::operator==(other); }
    bool operator!=(const AbstractConstIterator& other) const { return AbstractConstIteratorBase::operator!=(other); }
};

template<>
class AbstractConstIterator<true> : public AbstractConstIterator<false>{
protected:
    std::function<jobject(JNIEnv*,const void*)> current_converter;
public:
    using AbstractConstIterator<false>::AbstractConstIterator;
    AbstractConstIterator(const AbstractIterator<true>& other)
        : AbstractConstIterator<false>(other), current_converter(other.current_converter) {}
    AbstractConstIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter)
        : AbstractConstIterator<false>(std::move(_iter),false) {
        operator++();
    }
    friend void swap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept{
        _this.swap(other);
    }
    friend void qSwap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept{
        _this.swap(other);
    }
    AbstractConstIterator& operator++(){
        if(iter){
            if(iter->hasNext()){
                QMetaType nextType = iter->elementMetaType();
                if(nextType!=current_metaType){
                    current_dataType = MetaTypeUtils::dataType(nextType);
                    current_metaType = std::move(nextType);
                    current_converter = iter->elementConverter();
                }
                current_containerAccess = QSharedPointer<AbstractContainerAccess>(iter->elementNestedContainerAccess(), &containerDisposer);
                current_data = iter->constNext();
            }else{
                current_data = nullptr;
                iter.reset();
            }
        }
        return *this;
    }
    AbstractConstIterator operator++(int){
        AbstractConstIterator copy(*this);
        operator++();
        return copy;
    }
    bool operator==(const AbstractConstIterator& other) const { return AbstractConstIteratorBase::operator==(other); }
    bool operator!=(const AbstractConstIterator& other) const { return AbstractConstIteratorBase::operator!=(other); }
};

template<bool has_itemAccess, RowType _rowType, bool is_extensible_row, bool _is_mutable_row>
class Iterator : public AbstractIterator<has_itemAccess>{
public:
    using iterator_category = typename AbstractIterator<has_itemAccess>::iterator_category;
    using difference_type = typename AbstractIterator<has_itemAccess>::difference_type;
    typedef MutableRow<has_itemAccess> RowBase;
    typedef std::conditional_t<_rowType==RowType::Range, RangeRow<RowBase, is_extensible_row, _is_mutable_row>,
                               std::conditional_t<_rowType==RowType::MetaObject,
                                                  MetaObjectRow<RowBase, _is_mutable_row>,
                                                  RowBase>> value_type;
    typedef value_type*pointer;
    typedef value_type reference;
    using element_type = const value_type;
    using AbstractIterator<has_itemAccess>::AbstractIterator;
    value_type operator*()const{
        if constexpr(has_itemAccess)
            return {AbstractIterator<has_itemAccess>::current_data,
                    AbstractIterator<has_itemAccess>::current_metaType,
                    AbstractIterator<has_itemAccess>::current_dataType,
                    AbstractIterator<has_itemAccess>::current_containerAccess,
                    AbstractIterator<has_itemAccess>::current_converter};
        else
            return {AbstractIterator<has_itemAccess>::current_data,
                    AbstractIterator<has_itemAccess>::current_metaType,
                    AbstractIterator<has_itemAccess>::current_dataType,
                    AbstractIterator<has_itemAccess>::current_containerAccess};
    }
    Iterator& operator++(){
        AbstractIterator<has_itemAccess>::operator++();
        return *this;
    }
    Iterator operator++(int){
        Iterator copy(*this);
        operator++();
        return copy;
    }
    friend class ConstIterator<has_itemAccess, _rowType,is_extensible_row,_is_mutable_row>;
};

template<bool has_itemAccess, RowType _rowType, bool is_extensible_row, bool _is_mutable_row>
class ConstIterator : public AbstractConstIterator<has_itemAccess>{
public:
    using iterator_category = typename AbstractIterator<has_itemAccess>::iterator_category;
    using difference_type = typename AbstractIterator<has_itemAccess>::difference_type;
    typedef ConstRow<has_itemAccess> RowBase;
    typedef std::conditional_t<_rowType==RowType::Range, RangeRow<RowBase, false, _is_mutable_row>,
                               std::conditional_t<_rowType==RowType::MetaObject,
                                                  MetaObjectRow<RowBase, _is_mutable_row>,RowBase>
                               > value_type;
    typedef value_type*pointer;
    typedef value_type reference;
    using element_type = const value_type;
    using AbstractConstIterator<has_itemAccess>::AbstractConstIterator;
    ConstIterator(const Iterator<has_itemAccess, _rowType, is_extensible_row, _is_mutable_row>& iter)
        : AbstractConstIterator<has_itemAccess>(iter) {}
    value_type operator*() const{
        if constexpr(has_itemAccess)
            return {AbstractConstIterator<has_itemAccess>::current_data,
                    AbstractConstIterator<has_itemAccess>::current_metaType,
                    AbstractConstIterator<has_itemAccess>::current_dataType,
                    AbstractConstIterator<has_itemAccess>::current_containerAccess,
                    AbstractConstIterator<has_itemAccess>::current_converter};
        else
            return {AbstractConstIterator<has_itemAccess>::current_data,
                    AbstractConstIterator<has_itemAccess>::current_metaType,
                    AbstractConstIterator<has_itemAccess>::current_dataType,
                    AbstractConstIterator<has_itemAccess>::current_containerAccess};
    }
    ConstIterator& operator++(){
        AbstractConstIterator<has_itemAccess>::operator++();
        return *this;
    }
    ConstIterator operator++(int){
        ConstIterator copy(*this);
        operator++();
        return copy;
    }
};

template<typename Super>
struct ConstRangeRow : Super{
    using Super::Super;
    using Super::data;
    using Super::pointer;
    using Super::metaType;
    using Super::containerAccess;
    using Super::mutableRow;
    using const_iterator = ConstIterator<Super::hasItemAccess>;
    using value_type = typename const_iterator::value_type;
    using size_type = qsizetype;
public:
    ConstRangeRow(const ConstRangeRow& other) = default;
    ConstRangeRow(ConstRangeRow&& other) = default;
    ConstRangeRow& operator=(const ConstRangeRow& other) = default;
    ConstRangeRow& operator=(ConstRangeRow&& other) = default;
    const_iterator begin() const{
        if(containerAccess && containerAccess->isSequential()){
            return const_iterator{static_cast<AbstractSequentialAccess*>(&*containerAccess)->constElementIterator(pointer())};
        }else if(containerAccess && containerAccess->isAssociative()){
            return const_iterator{AbstractAssociativeAccess::asValueIterator(static_cast<AbstractAssociativeAccess*>(&*containerAccess)->constKeyValueIterator(pointer()))};
        }else if(containerAccess && containerAccess->isPair()){
            return const_iterator{static_cast<AbstractPairAccess*>(&*containerAccess)->constElementIterator(pointer())};
        }else{
            return const_iterator{nullptr};
        }
    }
    const_iterator end() const{
        return const_iterator{nullptr};
    }
};

template<typename RangeRowType>
struct MutableRangeRow : ConstRangeRow<RangeRowType>{
    using Super = ConstRangeRow<RangeRowType>;
    using Super::Super;
    using Super::data;
    using Super::pointer;
    using Super::metaType;
    using Super::containerAccess;
    using Super::mutableRow;
    using Super::begin;
    using Super::end;
    using const_iterator = ConstIterator<Super::hasItemAccess>;
    using iterator = std::conditional_t<std::is_same_v<void*,decltype(std::declval<Super>().pointer())>, Iterator<Super::hasItemAccess>, ConstIterator<Super::hasItemAccess>>;
    using value_type = typename iterator::value_type;
    using size_type = qsizetype;
    MutableRangeRow(const MutableRangeRow& other) = default;
    MutableRangeRow(MutableRangeRow&& other) = default;
    MutableRangeRow& operator=(const MutableRangeRow& other) = default;
    MutableRangeRow& operator=(MutableRangeRow&& other) = default;
    iterator begin(){
        if(containerAccess && containerAccess->isSequential()){
            return iterator{static_cast<AbstractSequentialAccess*>(&*containerAccess)->elementIterator(pointer())};
        }else if(containerAccess && containerAccess->isAssociative()){
            return iterator{AbstractAssociativeAccess::asValueIterator(static_cast<AbstractAssociativeAccess*>(&*containerAccess)->keyValueIterator(pointer()))};
        }else if(containerAccess && containerAccess->isPair()){
            return iterator{static_cast<AbstractPairAccess*>(&*containerAccess)->elementIterator(pointer())};
        }else{
            return iterator{nullptr};
        }
    }
    iterator end(){
        return iterator{nullptr};
    }
};

template<typename RangeRowType>
struct ExtensibleRangeRow : MutableRangeRow<RangeRowType>{
    using Super = MutableRangeRow<RangeRowType>;
    using Super::Super;
    using Super::data;
    using Super::pointer;
    using Super::metaType;
    using Super::containerAccess;
    using Super::mutableRow;
    using Super::begin;
    using Super::end;
    using const_iterator = typename Super::const_iterator;
    using iterator = typename Super::iterator;
    using value_type = typename iterator::value_type;
    using size_type = typename Super::size_type;
private:
    template<typename>
    friend struct ExtensibleRangeRow;
    size_type pendingResize = 0;
public:
    ExtensibleRangeRow(const ExtensibleRangeRow& other)
        : Super(other), pendingResize(other.pendingResize)
    {
    }
    ExtensibleRangeRow(ExtensibleRangeRow&& other)
        : Super(std::move(other)), pendingResize(std::move(other.pendingResize))
    {
    }
    template<typename Other>
    ExtensibleRangeRow(const ExtensibleRangeRow<Other>& other)
        : Super(other), pendingResize(other.pendingResize)
    {
    }
    ExtensibleRangeRow& operator=(const ExtensibleRangeRow& other){
        Super::operator=(other);
        pendingResize = other.pendingResize;
        return *this;
    }
    ExtensibleRangeRow& operator=(ExtensibleRangeRow&& other){
        Super::operator=(std::move(other));
        pendingResize = std::move(other.pendingResize);
        return *this;
    }

    template<typename Value>
    auto initialize(const Value& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(metaType)>,bool> {
        if(Super::initialize(value)){
            if(pendingResize>0 && data){
                if(containerAccess && containerAccess->isList()){
                    static_cast<AbstractListAccess*>(containerAccess.get())->resize(pointer(), pendingResize);
                }
                pendingResize = 0;
            }
            return true;
        }
        return false;
    }
    template<typename Value>
    auto initialize(Value&& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(metaType)>,bool> {
        if(Super::initialize(std::move(value))){
            if(pendingResize>0 && data){
                if(containerAccess && containerAccess->isList()){
                    static_cast<AbstractListAccess*>(containerAccess.get())->resize(pointer(), pendingResize);
                }
                pendingResize = 0;
            }
            return true;
        }
        return false;
    }

    iterator insert(const const_iterator& iter, size_type n, const value_type& value){
        if(containerAccess && containerAccess->isList()){
            auto listAccess = static_cast<AbstractListAccess*>(containerAccess.get());
            const ExtensibleRangeRow& _this = *this;
            size_type i = std::distance(QRangeModelDetails::begin(_this), iter);
            if(!value.data){
                void* ptr = listAccess->elementMetaType().create();
                listAccess->insert(pointer(), i, n, ptr);
                listAccess->elementMetaType().destroy(ptr);
            }else{
                listAccess->insert(pointer(), i, n, value.pointer());
            }
            iterator itr = QRangeModelDetails::begin(*this);
            std::advance(itr, i+n-1);
            if(!value.data){
                const_cast<value_type&>(value).initialize(*itr);
            }
            return itr;
        }else{
            return QRangeModelDetails::end(*this);
        }
    }

    void erase(const const_iterator& iter){
        if(containerAccess && containerAccess->isList()){
            auto listAccess = static_cast<AbstractListAccess*>(containerAccess.get());
            const ExtensibleRangeRow& _this = *this;
            const_iterator begin = QRangeModelDetails::begin(_this);
            qsizetype i = std::distance(begin, iter);
            listAccess->remove(pointer(), i, 1);
        }
    }

    void erase(const const_iterator& iter, const const_iterator& iter2){
        if(containerAccess && containerAccess->isList()){
            auto listAccess = static_cast<AbstractListAccess*>(containerAccess.get());
            const ExtensibleRangeRow& _this = *this;
            const_iterator begin = QRangeModelDetails::begin(_this);
            qsizetype i = std::distance(begin, iter);
            qsizetype i2 = std::distance(begin, iter2);
            listAccess->remove(pointer(), i, i2-i);
        }
    }

    void resize(qsizetype newSize){
        if(!containerAccess){
            pendingResize = newSize;
        }else if(containerAccess->isList()){
            auto listAccess = static_cast<AbstractListAccess*>(containerAccess.get());
            listAccess->resize(pointer(), newSize);
        }
    }
    void resize(size_type newSize, const value_type& value){
        if(containerAccess && containerAccess->isList()){
            auto listAccess = static_cast<AbstractListAccess*>(containerAccess.get());
            size_type oldSize = listAccess->size(pointer());
            if(!value.data && newSize>oldSize){
                void* ptr = listAccess->elementMetaType().create();
                for(size_type i = oldSize; i < newSize; ++i){
                    listAccess->append(pointer(), ptr);
                }
                listAccess->elementMetaType().destroy(ptr);
            }else{
                void* ptr = value.pointer();
                for(size_type i = oldSize; i < newSize; ++i){
                    listAccess->append(pointer(), ptr);
                }
            }
            if(!value.data && newSize>oldSize){
                iterator itr = QRangeModelDetails::begin(*this);
                std::advance(itr, oldSize);
                const_cast<value_type&>(value).initialize(*itr);
            }
        }
    }
};

template<typename Super>
struct MutableMetaObjectRow : Super{
    using Super::Super;
    using Super::operator=;
    using Super::pointer;
    using Super::metaType;
    using Super::containerAccess;
    using Super::mutableRow;
    using iterator = MetaPropertyIterator;
    MutableMetaObjectRow(const MutableMetaObjectRow& other) = default;
    MutableMetaObjectRow(MutableMetaObjectRow&& other) = default;
    MutableMetaObjectRow& operator=(const MutableMetaObjectRow& other) = default;
    MutableMetaObjectRow& operator=(MutableMetaObjectRow&& other) = default;
    iterator begin(){
        return MetaPropertyIterator{this->pointer(), metaType};
    }
    iterator end(){
        return MetaPropertyIterator{this->pointer(), metaType, metaType.metaObject()->propertyCount()};
    }
};

template<typename RangeRowType, bool is_mutable>
struct MetaObjectRow : std::conditional_t<is_mutable && RangeRowType::mutableRow, MutableMetaObjectRow<RangeRowType>, RangeRowType> {
    using Super = std::conditional_t<is_mutable && RangeRowType::mutableRow, MutableMetaObjectRow<RangeRowType>, RangeRowType>;
    using Super::Super;
    using Super::operator=;
    using Super::pointer;
    using Super::metaType;
    using Super::containerAccess;
    using Super::mutableRow;
    using const_iterator = ConstMetaPropertyIterator;
    MetaObjectRow(const MetaObjectRow& other) = default;
    MetaObjectRow(MetaObjectRow&& other) = default;
    MetaObjectRow& operator=(const MetaObjectRow& other) = default;
    MetaObjectRow& operator=(MetaObjectRow&& other) = default;
    const_iterator begin() const{
        return ConstMetaPropertyIterator{this->pointer(), metaType};
    }
    const_iterator end() const{
        return ConstMetaPropertyIterator{this->pointer(), metaType, metaType.metaObject()->propertyCount()};
    }
    int size() const{
        return metaType.metaObject()->propertyCount();
    }
};

template<bool, bool, bool, bool, bool, bool, RowType>
struct ConstTreeRow;

template<bool, bool, bool, bool, bool, bool, RowType>
struct MutableTreeRow;

template<bool has_itemAccess, typename... Args>
struct TreeRangeData;

template<>
struct TreeRangeData<false>{
    template<bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             bool itemsAreQObjects,
             RowType rowType>
    using TreeRow = ConstTreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,false,rowType>;
    void* m_container;
    std::shared_ptr<int> m_treeColumnCount;
    QSharedPointer<AbstractSequentialAccess> m_containerAccess;
    MetaTypeUtils::DataType m_elementDataType;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    TreeRangeData(void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& _treeColumnCount);
    TreeRangeData(JNIEnv*, jobject, const TreeRangeData<false>& other);
    TreeRangeData();
    TreeRangeData(TreeRangeData&& other);
    TreeRangeData& operator=(TreeRangeData&& other) = delete;
    TreeRangeData(const TreeRangeData& other) = delete;
    TreeRangeData& operator=(const TreeRangeData& other) = delete;
    ~TreeRangeData();
    void* container() const;
    const QMetaType& elementMetaType() const;
    MetaTypeUtils::DataType elementDataType() const;
    const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess() const;
    const QSharedPointer<AbstractSequentialAccess>& containerAccess() const;
    bool isInitialized() const;
    void initialize(JNIEnv*, const TreeRangeData<false>& other);
    int treeColumnCount() const;
    jobject rowObject(JNIEnv*) const{
        return nullptr;
    }
};

template<>
struct TreeRangeData<true> : TreeRangeData<false>{
    template<bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             bool itemsAreQObjects,
             RowType rowType>
    using TreeRow = ConstTreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,true,rowType>;
    using TreeRangeData<false>::initialize;
    using TreeRangeData<false>::TreeRangeData;
    JObjectWrapper m_rowObject;
    TreeRangeData(JNIEnv* env, jobject row, const TreeRangeData<true>& other)
        : TreeRangeData<false>(env, row, other),
        m_rowObject(env, row) {
    }
    TreeRangeData(TreeRangeData&& other)
        : TreeRangeData<false>(std::move(other)){
    }
    TreeRangeData()
        : TreeRangeData<false>() {
    }
    TreeRangeData& operator=(TreeRangeData&& other) = delete;
    TreeRangeData(const TreeRangeData& other) = delete;
    TreeRangeData& operator=(const TreeRangeData& other) = delete;
    jobject rowObject(JNIEnv* env) const{
        return m_rowObject.object(env);
    }
};

template<bool has_itemAccess>
struct TreeRangeData<has_itemAccess,ClassInfo> : TreeRangeData<false>{
    template<bool is_mutable_range,
             bool is_mutable_row,
             bool is_list_range,
             bool is_list_row,
             bool itemsAreQObjects,
             RowType rowType>
    using TreeRow = MutableTreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType>;
    ClassInfo m_classInfo;
    JObjectWrapper m_rowObject;
    TreeRangeData(void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& _treeColumnCount, ClassInfo&& _classInfo)
        : TreeRangeData<false>(container, std::move(containerAccess), std::move(_treeColumnCount)),
        m_classInfo(std::move(_classInfo)), m_rowObject() {
    }
    TreeRangeData(JNIEnv* env, jobject row, const TreeRangeData<has_itemAccess,ClassInfo>& other)
        : TreeRangeData<false>(env, row, other),
        m_classInfo(other.m_classInfo), m_rowObject(env, row) {
    }
    TreeRangeData()
        : TreeRangeData<false>(),
        m_classInfo() {
    }
    TreeRangeData(TreeRangeData&& other)
        : TreeRangeData<false>(std::move(other)),
        m_classInfo(std::move(other.m_classInfo)) {
    }
    TreeRangeData& operator=(TreeRangeData&& other) = delete;
    TreeRangeData(const TreeRangeData& other) = delete;
    TreeRangeData& operator=(const TreeRangeData& other) = delete;
    void initialize(JNIEnv* env, const TreeRangeData<has_itemAccess,ClassInfo>& other){
        TreeRangeData<false>::initialize(env, other);
        m_classInfo = other.m_classInfo;
        m_rowObject = {env, env->NewObject(m_classInfo.javaClass, m_classInfo.defaultConstructor)};
        JavaException::check(env QTJAMBI_STACKTRACEINFO);
    }
    jobject rowObject(JNIEnv* env) const{
        return m_rowObject.object(env);
    }
    jobject rowObject(JNIEnv* env){
        jobject result = m_rowObject.object(env);
        if(!result){
            result = env->NewObject(m_classInfo.javaClass, m_classInfo.defaultConstructor);
            JavaException::check(env QTJAMBI_STACKTRACEINFO);
            m_rowObject = {env, result};
        }
        return result;
    }
};

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType,
         typename... Args>
struct TreeRangeWrapper : std::vector<typename TreeRangeData<has_itemAccess,Args...>::template TreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,rowType>*>
{
    using TreeRow = typename TreeRangeData<has_itemAccess,Args...>::template TreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,rowType>;
    using Super = std::vector<TreeRow*>;
    using Super::begin;
    using Super::end;
    using Super::cbegin;
    using Super::cend;
    using Super::push_back;
    using value_type = typename Super::value_type;
    using iterator = typename Super::iterator;
    using const_iterator = typename Super::const_iterator;
    using pointer = typename Super::pointer;
    using const_pointer = typename Super::const_pointer;
    using reference = typename Super::reference;
    using const_reference = const typename Super::const_reference;
    using size_type = typename Super::size_type;
    using difference_type = typename Super::difference_type;
    TreeRangeData<has_itemAccess,Args...> m_data;
    const QMetaType& elementMetaType() const{
        return m_data.elementMetaType();
    }
    MetaTypeUtils::DataType elementDataType() const{
        return m_data.elementDataType();
    }
    const QMetaObject* elementMetaObject() const{
        return m_data.elementMetaType().metaObject();
    }
    const QSharedPointer<AbstractSequentialAccess>& containerAccess() const {
        return m_data.containerAccess();
    }
    inline const QSharedPointer<AbstractSequentialAccess>& elementNestedContainerAccess() const{
        return m_data.elementNestedContainerAccess();
    }
    int treeColumnCount() const{
        return m_data.treeColumnCount();
    }
    TreeRangeWrapper(JNIEnv* env, void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& treeColumnCount, Args&&... args)
        : m_data(container, std::move(containerAccess), std::move(treeColumnCount), std::move(args)...) {
        initialize(env);
    }
    TreeRangeWrapper(TreeRow* owner, JNIEnv* env, jobject object, const TreeRangeWrapper& other)
        : m_data(env, object, other.m_data) {
        initialize(owner, env, object);
    }
    TreeRangeWrapper()
        : m_data() {
    }
    TreeRangeWrapper(TreeRangeWrapper&& other)
        : Super(std::move(other)), m_data(std::move(other.m_data)) {
    }
    TreeRangeWrapper& operator=(TreeRangeWrapper&& other) = delete;
    TreeRangeWrapper(const TreeRangeWrapper& other) = delete;
    TreeRangeWrapper& operator=(const TreeRangeWrapper& other) = delete;
    ~TreeRangeWrapper(){
    }
    jobject itemAccess(JNIEnv*) const {
        return nullptr;
    }

    template<bool b = sizeof...(Args)!=0, std::enable_if_t<b, bool> = true>
    TreeRangeWrapper& operator *(){
        return *this;
    }
    const TreeRangeWrapper& operator *() const{
        return *this;
    }
    template<bool b = sizeof...(Args)!=0, std::enable_if_t<b, bool> = true>
    TreeRangeWrapper* operator ->(){
        return this;
    }
    template<bool b = sizeof...(Args)!=0, std::enable_if_t<b, bool> = true>
    TreeRangeWrapper& operator=(TreeRangeWrapper*){
        return *this;
    }
    const TreeRangeWrapper* operator ->() const{
        return this;
    }
    operator bool() const{
        return true;
    }
    template<bool b = sizeof...(Args)!=0, std::enable_if_t<b, bool> = true>
    void emplace(...){}

    jobject rowObject(JNIEnv* env) const{
        return m_data.rowObject(env);
    }

    jobject rowObject(JNIEnv* env){
        return m_data.rowObject(env);
    }

    template<int S = sizeof...(Args)>
    std::enable_if_t<S!=0, void> erase(const const_iterator& iter){
        const TreeRangeWrapper& _this = *this;
        const_iterator begin = QRangeModelDetails::begin(_this);
        size_type i = std::distance(begin, iter);
        if(void* container = m_data.container()){
            auto containerAccess = m_data.containerAccess();
            if(containerAccess->isList()){
                auto sequentialAccess = static_cast<AbstractListAccess*>(containerAccess.get());
                sequentialAccess->remove(container, i, 1);
                Super::erase(iter);
                auto iter = sequentialAccess->elementIterator(container);
                i = 0;
                while(iter->hasNext()){
                    TreeRow* row = (*this)[i++];
                    void* data = iter->mutableNext();
                    if(row->data!=data){
                        if(row->needsDeletion){
                            row->metaType.destroy(row->data);
                        }
                        row->data = data;
                    }
                }
            }else{
                Super::erase(iter);
            }
        }else{
            Super::erase(iter);
        }
    }

    template<int S = sizeof...(Args)>
    std::enable_if_t<S!=0, void> erase(const const_iterator& iter, const const_iterator& iter2){
        const TreeRangeWrapper& _this = *this;
        const_iterator begin = QRangeModelDetails::begin(_this);
        size_type i = std::distance(begin, iter);
        size_type i2 = std::distance(begin, iter2);
        if(void* container = m_data.container()){
            if(m_data.containerAccess() && m_data.containerAccess()->isList()){
                auto sequentialAccess = static_cast<AbstractListAccess*>(m_data.containerAccess().get());
                sequentialAccess->remove(container, i, i2-i);
                Super::erase(iter, iter2);
                auto iter = sequentialAccess->elementIterator(container);
                i = 0;
                while(iter->hasNext()){
                    TreeRow* row = (*this)[i++];
                    void* data = iter->mutableNext();
                    if(row->data!=data){
                        if(row->needsDeletion){
                            row->metaType.destroy(row->data);
                        }
                        row->data = data;
                    }
                }
            }else{
                Super::erase(iter, iter2);
            }
        }else{
            Super::erase(iter, iter2);
        }
    }

    template<int S = sizeof...(Args)>
    std::enable_if_t<S!=0, iterator> insert(const_iterator iter, size_type n, TreeRow* value){
        if(!value)
            return Super::insert(iter, n, value);
        if(void* container = m_data.container()){
            if(m_data.containerAccess() && m_data.containerAccess()->isList()){
                auto sequentialAccess = static_cast<AbstractListAccess*>(m_data.containerAccess().get());
                const TreeRangeWrapper& _this = *this;
                size_type index = std::distance(QRangeModelDetails::begin(_this), iter);
                if(index>size_type(sequentialAccess->size(container)))
                    index = size_type(sequentialAccess->size(container));
                if(JniEnvironment env{100}){
                    if(!value->metaType.isValid()){
                        value->dataType = m_data.elementDataType();
                        value->metaType = m_data.elementMetaType();
                        value->containerAccess = m_data.elementNestedContainerAccess();
                        if(!value->childRows().m_data.isInitialized()){
                            value->childRows().m_data.initialize(env, m_data);
                        }
                        jobject inserted = value->rowObject(env);
                        if(value->metaType.flags() & QMetaType::IsPointer){
                            void* ptr = QtJambiAPI::convertJavaObjectToNative(env, inserted);
                            sequentialAccess->insert(container, index, 1, &ptr);
                        }else if(CoreAPI::isJObjectWrappedMetaType(value->metaType)){
                            JObjectWrapper wr(env, inserted);
                            sequentialAccess->insert(container, index, 1, &wr);
                        }else{
                            QVariant variant = qtjambi_cast<QVariant>(env, inserted);
                            variant.convert(value->metaType);
                            sequentialAccess->insert(container, index, 1, variant.constData());
                        }
                        auto result = Super::insert(iter, 1, value);
                        auto iter = sequentialAccess->elementIterator(container);
                        size_type i = 0;
                        while(iter->hasNext()){
                            TreeRow* row = (*this)[i++];
                            void* data = iter->mutableNext();
                            if(row->data!=data){
                                if(row->needsDeletion){
                                    row->metaType.destroy(row->data);
                                }
                                row->data = data;
                            }
                        }
                        if constexpr(rowType==RowType::Range){
                            value->resizeIfNecessary();
                        }
                        if(n>1){
                            TreeRow* row = new TreeRow();
                            row->setParentRow(value->parentRow());
                            insert(result, n-1, row);
                        }
                        return result;
                    }else if(value->data){
                        int index = std::distance(QRangeModelDetails::begin(_this), iter);
                        sequentialAccess->insert(container, index, 1, value->data);
                        auto result = Super::insert(iter, 1, value);
                        auto eiter = sequentialAccess->elementIterator(container);
                        size_type i = 0;
                        while(eiter->hasNext()){
                            TreeRow* row = (*this)[i++];
                            void* data = eiter->mutableNext();
                            if(row->data!=data){
                                if(row->needsDeletion){
                                    row->metaType.destroy(row->data);
                                }
                                row->data = data;
                            }
                        }
                        if(n>1){
                            index = std::distance(std::begin(*this), result);
                            TreeRow* row = new TreeRow();
                            row->setParentRow(value->parentRow());
                            auto _iter = QRangeModelDetails::begin(_this);
                            std::advance(_iter, index+1);
                            insert(_iter, n-1, row);
                        }
                        return result;
                    }
                }
            }
        }else if(JniEnvironment env{100}){
            if(!value->metaType.isValid()){
                value->metaType = m_data.elementMetaType();
                value->dataType = m_data.elementDataType();
                value->containerAccess = m_data.elementNestedContainerAccess();
                if(!value->childRows().m_data.isInitialized()){
                    value->childRows().m_data.initialize(env, m_data);
                }
                jobject inserted = value->rowObject(env);
                if(value->metaType.flags() & QMetaType::IsPointer){
                    void* ptr = QtJambiAPI::convertJavaObjectToNative(env, inserted);
                    value->data = new void*(ptr);
                }else if(CoreAPI::isJObjectWrappedMetaType(value->metaType)){
                    value->data = new JObjectWrapper(env, inserted);
                }else{
                    QVariant variant = qtjambi_cast<QVariant>(env, inserted);
                    variant.convert(value->metaType);
                    value->data = value->metaType.create(variant.constData());
                }
                value->needsDeletion = true;
                if constexpr(rowType==RowType::Range){
                    value->resizeIfNecessary();
                }
            }
            auto result = Super::insert(iter, 1, value);
            int index = std::distance(std::begin(*this), result);
            if(n>1){
                TreeRow* row = new TreeRow();
                row->setParentRow(value->parentRow());
                const TreeRangeWrapper& _this = *this;
                auto _iter = QRangeModelDetails::begin(_this);
                std::advance(_iter, index+1);
                insert(_iter, n-1, row);
            }
            return result;
        }
        return end();
    }
private:
    void initialize(JNIEnv* env){
        if constexpr(rowType==RowType::Range){
            if(auto elementNestedContainerAccess = m_data.elementNestedContainerAccess()){
                if(elementNestedContainerAccess->isSequential()){
                    AbstractSequentialAccess* sequentialElementAccess = static_cast<AbstractSequentialAccess*>(elementNestedContainerAccess.get());
                    auto containerIter = m_data.containerAccess()->constElementIterator(m_data.container());
                    while(containerIter->hasNext()){
                        const void* data = containerIter->next();
                        *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, sequentialElementAccess->size(data));
                    }
                }else if(elementNestedContainerAccess->isAssociative()){
                    AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(elementNestedContainerAccess.get());
                    auto containerIter = m_data.containerAccess()->constElementIterator(m_data.container());
                    while(containerIter->hasNext()){
                        const void* data = containerIter->next();
                        *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, associativeElementAccess->size(data));
                    }
                }else if(elementNestedContainerAccess->isPair()){
                    *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, 2);
                }
            }
        }
        auto javaIter = m_data.containerAccess()->constElementIterator(m_data.container());
        if constexpr(sizeof...(Args)==0){
            auto nativeIter = m_data.containerAccess()->constElementIterator(m_data.container());
            while(javaIter->hasNext()){
                push_back(new TreeRow{nullptr, env, javaIter->next(env), nativeIter->constNext(), m_data.elementMetaType(), m_data.elementDataType(), m_data.elementNestedContainerAccess(), *this});
            }
        }else{
            auto nativeIter = m_data.containerAccess()->elementIterator(m_data.container());
            if(nativeIter->isConst()){
                while(javaIter->hasNext()){
                    TreeRow* t = new TreeRow{nullptr, env, javaIter->next(env), m_data.elementMetaType().create(nativeIter->constNext()), m_data.elementMetaType(), m_data.elementDataType(), m_data.elementNestedContainerAccess(), *this};
                    t->needsDeletion = true;
                    push_back(t);
                }
            }else{
                while(javaIter->hasNext()){
                    push_back(new TreeRow{nullptr, env, javaIter->next(env), nativeIter->mutableNext(), m_data.elementMetaType(), m_data.elementDataType(), m_data.elementNestedContainerAccess(), *this});
                }
            }
        }
    }
    void initialize(TreeRow* parent, JNIEnv* env, jobject object){
        Q_ASSERT(parent);
        if(object){
            jobject list = Java::QtCore::QRangeModel$ConstTreeRowInterface::childRows(env, object);
            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, list);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                if(jobject row = QtJambiAPI::nextOfJavaIterator(env, iter)){
                    void* container = nullptr;
                    void* newPtr;
                    if(m_data.elementMetaType().flags() & QMetaType::IsPointer){
                        void* ptr = container = QtJambiAPI::convertJavaObjectToNative(env, row);
                        newPtr = new void*(ptr);
                    }else if(CoreAPI::isJObjectWrappedMetaType(m_data.elementMetaType())){
                        newPtr = new JObjectWrapper(env, row);
                    }else{
                        QVariant variant = qtjambi_cast<QVariant>(env, row);
                        variant.convert(m_data.elementMetaType());
                        newPtr = container = m_data.elementMetaType().create(variant.constData());
                    }
                    if constexpr(rowType==RowType::Range){
                        if(container){
                            if(auto elementNestedContainerAccess = m_data.elementNestedContainerAccess()){
                                if(elementNestedContainerAccess->isSequential()){
                                    *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, static_cast<AbstractSequentialAccess*>(elementNestedContainerAccess.get())->size(container));
                                }else if(elementNestedContainerAccess->isAssociative()){
                                    *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, static_cast<AbstractAssociativeAccess*>(elementNestedContainerAccess.get())->size(container));
                                }else if(elementNestedContainerAccess->isPair()){
                                    *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, 2);
                                }
                            }
                        }
                    }
                    TreeRow* newRow = new TreeRow{parent, env, row, newPtr, parent->metaType, parent->dataType, parent->containerAccess, *this};
                    newRow->needsDeletion = true;
                    push_back(newRow);
                }
            }
        }
    }
};

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         RowType rowType,
         typename... Args>
struct TreeRangeItemAccessWrapper
    : TreeRangeWrapper<is_mutable_range, is_mutable_row, is_list_range, is_list_row, itemsAreQObjects, true, rowType, Args...>{
    JObjectWrapper m_itemAccess;
    TreeRangeItemAccessWrapper(JObjectWrapper&& itemAccess, JNIEnv* env, void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& treeColumnCount, Args&&... args)
        : TreeRangeWrapper<is_mutable_range, is_mutable_row, is_list_range, is_list_row, itemsAreQObjects, true, rowType, Args...>(env, container, std::move(containerAccess), std::move(treeColumnCount), std::move(args)...),
          m_itemAccess(std::move(itemAccess))
    {
    }
    TreeRangeItemAccessWrapper(TreeRangeItemAccessWrapper&& other) = default;
    jobject itemAccess(JNIEnv* env) const {
        return m_itemAccess.object(env);
    }
};

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType>
struct ConstTreeRow : std::conditional_t<rowType==RowType::Range, RangeRow<ConstRow<false>, false, false>, ConstRow<false>> {
    using Super = std::conditional_t<rowType==RowType::Range, RangeRow<ConstRow<false>, false, false>, ConstRow<false>>;
    using Super::data;
    using Super::metaType;
    using Super::containerAccess;
    using Super::pointer;
    using Super::needsDeletion;
    using Super::mutableRow;
    using Tree = RangeWrapperType<TreeType::ConstTree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType>;
    friend Tree;
private:
    Tree m_children;
    ConstTreeRow* m_parentRow;
protected:
public:
    ConstTreeRow()
        : Super(), m_children(), m_parentRow(nullptr){
    }
    ConstTreeRow(ConstTreeRow* parent, JNIEnv* env, jobject row, const void* data, const QMetaType& metaType, MetaTypeUtils::DataType dataType, const QSharedPointer<AbstractContainerAccess>& containerAccess, const Tree& tree)
        : Super(data, metaType, dataType, containerAccess), m_children(this, env, row, tree), m_parentRow(parent)
    {
    }
    template<bool b = mutableRow, std::enable_if_t<b, bool> = true>
    ConstTreeRow(ConstTreeRow* parent, JNIEnv* env, jobject row, void* data, const QMetaType& metaType, MetaTypeUtils::DataType dataType, const QSharedPointer<AbstractContainerAccess>& containerAccess, const Tree& tree)
        : Super(data, metaType, dataType, containerAccess), m_children(this, env, row, tree), m_parentRow(parent)
    {
    }
    ~ConstTreeRow(){
        qDeleteAll(m_children);
    }
    ConstTreeRow* parentRow() const {
        return m_parentRow;
    }
    const Tree& childRows() const{
        return m_children;
    }
    int treeColumnCount() const{
        return m_children.treeColumnCount();
    }
    template<bool b = has_itemAccess, std::enable_if_t<b, bool> = true>
    jobject asJObject(JNIEnv* env) const { return m_children.rowObject(env); }
};

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType>
struct MutableTreeRow : std::conditional_t<rowType==RowType::Range, RangeRow<MutableRow<false>, is_mutable_range, true>, MutableRow<false>>{
    using Super = std::conditional_t<rowType==RowType::Range, RangeRow<MutableRow<false>, is_mutable_range, true>, MutableRow<false>>;
    using Super::data;
    using Super::metaType;
    using Super::dataType;
    using Super::containerAccess;
    using Super::pointer;
    using Super::needsDeletion;
    using Super::swap;
    using Super::mutableRow;
    using Tree = RangeWrapperType<TreeType::MutableTree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType>;
    friend Tree;
private:
    Tree m_children;
    MutableTreeRow* m_parentRow;
public:
    MutableTreeRow()
        : Super(), m_children(), m_parentRow(nullptr) {
    }
    MutableTreeRow(MutableTreeRow* parent, JNIEnv* env, jobject row, void* data, const QMetaType& metaType, MetaTypeUtils::DataType dataType, const QSharedPointer<AbstractContainerAccess>& containerAccess, const Tree& tree)
        : Super(data, metaType, dataType, containerAccess), m_children(this, env, row, tree), m_parentRow(parent)
    {
    }
    ~MutableTreeRow(){
        setParentRow(nullptr);
        qDeleteAll(m_children);
    }

    jobject rowObject(JNIEnv* env) const{
        return m_children.rowObject(env);
    }

    jobject rowObject(JNIEnv* env){
        return m_children.rowObject(env);
    }
    template<bool b = has_itemAccess, std::enable_if_t<b, bool> = true>
    jobject asJObject(JNIEnv* env) const { return m_children.rowObject(env); }

    void prepareDeletion(){
        m_parentRow = nullptr;
        for(MutableTreeRow* row : std::as_const(m_children)){
            row->prepareDeletion();
        }
    }
    friend void swap(MutableTreeRow*& a, MutableTreeRow*&b){
        if(JniEnvironment env{100}){
            if(!a->m_parentRow){
                Super& rowA = *a;
                rowA.swap(*b);
                std::swap(a->data, b->data);
            }
        }
        MutableTreeRow* tmp = a;
        a = b;
        b = tmp;
    }
    MutableTreeRow* parentRow() const {
        return m_parentRow;
    }
    const Tree& childRows() const{
        return m_children;
    }
    Tree& childRows(){
        return m_children;
    }
    void setParentRow(MutableTreeRow* parent){
        if(m_parentRow != parent){
            if(JniEnvironment env{100}){
                if(!m_children.m_data.isInitialized() && parent){
                    m_children.m_data.initialize(env, parent->m_children.m_data);
                }
                jobject jthis;
                if(!metaType.isValid() && parent){
                    metaType = parent->metaType;
                    dataType = parent->dataType;
                    containerAccess = parent->containerAccess;
                    jthis = m_children.rowObject(env);
                    if(metaType.flags() & QMetaType::IsPointer){
                        void* ptr = QtJambiAPI::convertJavaObjectToNative(env, jthis);
                        data = new void*(ptr);
                    }else if(CoreAPI::isJObjectWrappedMetaType(metaType)){
                        data = new JObjectWrapper(env, jthis);
                    }else{
                        QVariant variant = qtjambi_cast<QVariant>(env, jthis);
                        variant.convert(metaType);
                        data = metaType.create(variant.constData());
                    }
                    needsDeletion = true;
                    if constexpr(rowType==RowType::Range){
                        resizeIfNecessary();
                    }
                }else{
                    jthis = m_children.rowObject(env);
                }
                if(jthis){
                    m_parentRow = parent;
                    if(m_parentRow){
                        jobject parentObject = m_parentRow->rowObject(env);
                        Java::QtCore::QRangeModel$TreeRowInterface::setParentRow(env, jthis, parentObject);
                    }else{
                        Java::QtCore::QRangeModel$TreeRowInterface::setParentRow(env, jthis, nullptr);
                    }
                }
            }
        }
    }
    int treeColumnCount() const{
        return m_children.treeColumnCount();
    }
    bool resizeIfNecessary(){
        if constexpr(rowType==RowType::Range){
            if(data){
                if(containerAccess && containerAccess->isList()){
                    AbstractListAccess* access = static_cast<AbstractListAccess*>(containerAccess.get());
                    void* container = pointer();
                    if(container && access->size(container)<treeColumnCount()){
                        access->resize(container, treeColumnCount());
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

template<typename Super>
struct ListRangeWrapper : Super{
    using const_iterator = typename Super::const_iterator;
    using iterator = typename Super::iterator;
    using value_type = typename Super::value_type;
    using size_type = qsizetype;
    using Super::data;
    using Super::containerAccess;
    using Super::elementNestedContainerAccess;
    using Super::Super;

    template<typename Value>
    std::enable_if_t<std::is_same_v<Value,typename iterator::value_type> || std::is_same_v<Value,typename const_iterator::value_type>,iterator>
    insert(const const_iterator& iter, size_type n, const Value& value){
        const ListRangeWrapper& _this = *this;
        size_type i = std::distance(QRangeModelDetails::begin(_this), iter);
        if(!value.data){
            void* ptr = containerAccess()->elementMetaType().create();
            static_cast<AbstractListAccess*>(&*containerAccess())->insert(data, i, n, ptr);
            containerAccess()->elementMetaType().destroy(ptr);
        }else{
            static_cast<AbstractListAccess*>(&*containerAccess())->insert(data, i, n, value.pointer());
        }
        iterator itr = QRangeModelDetails::begin(*this);
        std::advance(itr, i+n-1);
        if(!value.data){
            const_cast<Value&>(value).initialize(*itr);
        }
        return itr;
    }

    void erase(const const_iterator& iter){
        const ListRangeWrapper& _this = *this;
        const_iterator begin = QRangeModelDetails::begin(_this);
        size_type i = std::distance(begin, iter);
        static_cast<AbstractListAccess*>(&*containerAccess())->remove(data, i, 1);
    }

    void erase(const const_iterator& iter, const const_iterator& iter2){
        const ListRangeWrapper& _this = *this;
        const_iterator begin = QRangeModelDetails::begin(_this);
        size_type i = std::distance(begin, iter);
        size_type i2 = std::distance(begin, iter2);
        static_cast<AbstractListAccess*>(&*containerAccess())->remove(data, i, i2-i);
    }

    template<typename Value>
    std::enable_if_t<std::is_same_v<Value,typename iterator::value_type> || std::is_same_v<Value,typename const_iterator::value_type>,void>
    resize(size_type newSize, const Value& value){
        AbstractListAccess* listAccess = static_cast<AbstractListAccess*>(&*containerAccess());
        size_type oldSize = listAccess->size(data);
        if(!value.data && newSize>oldSize){
            void* ptr = containerAccess()->elementMetaType().create();
            for(size_type i = oldSize; i < newSize; ++i){
                listAccess->append(data, ptr);
            }
            containerAccess()->elementMetaType().destroy(ptr);
        }else{
            void* ptr = value.pointer();
            for(size_type i = oldSize; i < newSize; ++i){
                listAccess->append(data, ptr);
            }
        }
        if(!value.data && newSize>oldSize){
            iterator itr = QRangeModelDetails::begin(*this);
            std::advance(itr, oldSize);
            const_cast<Value&>(value).initialize(*itr);
        }
    }
};

template<bool is_mutable_row,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType>
struct ConstRangeWrapper{
    void* data;
    QMetaType metaType;
    MetaTypeUtils::DataType dataType;
    QSharedPointer<AbstractSequentialAccess> m_containerAccess;
    ConstRangeWrapper(void* _data, const QMetaType& _metaType, QSharedPointer<AbstractSequentialAccess>&& _containerAccess)
        : data(_data),
        metaType(_metaType),
        dataType(MetaTypeUtils::dataType(metaType)),
        m_containerAccess(std::move(_containerAccess)){
    }
    inline const QMetaType& elementMetaType() const{
        return metaType;
    }
    inline MetaTypeUtils::DataType elementDataType() const{
        return dataType;
    }
    inline const QMetaObject* elementMetaObject() const{
        return metaType.metaObject();
    }
    inline const QSharedPointer<AbstractSequentialAccess>& containerAccess() const{
        return m_containerAccess;
    }
    inline QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess() const{
        return QSharedPointer<AbstractContainerAccess>{m_containerAccess->elementNestedContainerAccess(), &containerDisposer};
    }

    using const_iterator = ConstIterator<has_itemAccess, rowType,is_list_row,is_mutable_row>;
    using iterator = const_iterator;
    using value_type = typename const_iterator::value_type;

    const_iterator begin() const{
        return const_iterator(m_containerAccess->constElementIterator(data));
    }
    const_iterator end() const{
        return const_iterator(nullptr);
    }
    int size() const {
        return m_containerAccess ? m_containerAccess->size(data) : 0;
    }
};

template<bool is_mutable_row,
         bool is_list_row,
         bool itemsAreQObjects,
         RowType rowType>
struct ConstRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,true,rowType> : ConstRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,false,rowType>{
    using Super = ConstRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,false,rowType>;
    using Super::data;
    using Super::m_containerAccess;
    JObjectWrapper m_itemAccess;
    ConstRangeWrapper(void* _data, const QMetaType& _metaType, QSharedPointer<AbstractSequentialAccess>&& _containerAccess, JObjectWrapper&& itemAccess)
        : ConstRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,false,rowType>(_data, _metaType, std::move(_containerAccess)),
        m_itemAccess(std::move(itemAccess))
    {
    }
    jobject itemAccess(JNIEnv* env) const {
        return m_itemAccess.object(env);
    }
    using const_iterator = ConstIterator<true, rowType,is_list_row,is_mutable_row>;
    using iterator = const_iterator;
    using value_type = typename const_iterator::value_type;

    const_iterator begin() const{
        return const_iterator(m_containerAccess->constElementIterator(data));
    }
    const_iterator end() const{
        return const_iterator(nullptr);
    }
};

template<bool is_mutable_row,
         bool is_list_row,
         bool itemsAreQObjects,
         bool has_itemAccess,
         RowType rowType>
struct MutableRangeWrapper : ConstRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,has_itemAccess,rowType>{
    using Super = ConstRangeWrapper<is_mutable_row,is_list_row,itemsAreQObjects,has_itemAccess,rowType>;
    using Super::Super;
    using Super::data;
    using Super::m_containerAccess;
    using Super::begin;
    using Super::end;
    using iterator = Iterator<has_itemAccess,rowType,is_list_row,is_mutable_row>;
    iterator begin() {
        return iterator(m_containerAccess->elementIterator(data));
    }
    iterator end() {
        return iterator(nullptr);
    }
};

template <TreeType treeType,
          typename Structure,
          bool is_mutable_range,
          bool is_mutable_row,
          bool is_list_range,
          bool is_list_row,
          bool itemsAreQObjects,
          bool has_itemAccess,
          RowType rowType, typename Protocol = QRangeModelDetails::table_protocol_t<RangeWrapperType<treeType,is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType>>>
class QtJambiRangeModelImpl : public QtPrivate::QQuasiVirtualSubclass<QtJambiRangeModelImpl<treeType, Structure, is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType, Protocol>,
                                                                      QRangeModelImplBase>
{
public:
    using Range = RangeWrapperType<treeType,is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType>;
    using RootRange = RangeWrapperType<treeType,is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType,true>;
    using range_type = QRangeModelDetails::wrapped_t<Range>;
    using row_reference = decltype(*QRangeModelDetails::begin(std::declval<range_type&>()));
    using const_row_reference = std::add_const_t<decltype(*QRangeModelDetails::begin(std::declval<const range_type&>()))>;
    using row_type = std::remove_reference_t<row_reference>;
    using const_row_type = std::remove_reference_t<const_row_reference>;
    using wrapped_row_type = QRangeModelDetails::wrapped_t<row_type>;
    using row_ptr = wrapped_row_type *;
    using const_row_ptr = const wrapped_row_type *;
    using protocol_type = QRangeModelDetails::wrapped_t<Protocol>;
protected:
    using Ancestor = QtPrivate::QQuasiVirtualSubclass<QtJambiRangeModelImpl<treeType, Structure, is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType, Protocol>,
                                                      QRangeModelImplBase>;
    using Self = QtJambiRangeModelImpl<treeType, Structure, is_mutable_range,is_mutable_row,is_list_range,is_list_row,itemsAreQObjects,has_itemAccess,rowType, Protocol>;
    Structure& that() { return static_cast<Structure &>(*this); }
    const Structure& that() const { return static_cast<const Structure &>(*this); }

    using range_features = QRangeModelDetails::range_traits<range_type>;
    using row_features = QRangeModelDetails::range_traits<wrapped_row_type>;
    struct row_traits{
        static constexpr int static_size = rowType==RowType::MetaObject ? 0 : (rowType==RowType::Range && treeType==TreeType::None ? -1 : 0);
        static int fixed_size() {
            return 1;
        }
    };
    using protocol_traits = QRangeModelDetails::protocol_traits<Range, protocol_type>;

    static constexpr bool isMutable()
    {
        return range_features::is_mutable && row_features::is_mutable
               && Structure::is_mutable_impl;
    }

    static constexpr int static_row_count = QRangeModelDetails::static_size_v<range_type>;
    static constexpr int static_column_count = row_traits::static_size;
    static constexpr bool one_dimensional_range = static_column_count == 0;
    static constexpr bool rowsAreQObjects = rowType==RowType::MetaObject;

    using ModelData = QRangeModelDetails::ModelData<std::conditional_t<
                                                        std::is_pointer_v<RootRange>,
                                                        RootRange, std::remove_reference_t<RootRange>>,
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
                                                    QRangeModelDetails::PropertyData<rowType==RowType::MetaObject,itemsAreQObjects || rowsAreQObjects>>;
#else
                                                    std::conditional_t<itemsAreQObjects,QObject, std::conditional_t<rowType==RowType::MetaObject,QEvent,QVariant>>>;
#endif

    static constexpr bool dynamicRows() { return isMutable() && static_row_count < 0; }
    static constexpr bool dynamicColumns() { return static_column_count < 0; }

    struct EmptyRowGenerator
    {
        using value_type = row_type;
        using reference = value_type;
        using pointer = value_type *;
        using iterator_category = std::input_iterator_tag;
        using difference_type = int;

        value_type operator*() { return impl->makeEmptyRow(*parent); }
        EmptyRowGenerator &operator++() { ++n; return *this; }
        friend bool operator==(const EmptyRowGenerator &lhs, const EmptyRowGenerator &rhs) noexcept
        { return lhs.n == rhs.n; }
        friend bool operator!=(const EmptyRowGenerator &lhs, const EmptyRowGenerator &rhs) noexcept
        { return !(lhs == rhs); }

        difference_type n = 0;
        Structure *impl = nullptr;
        const QModelIndex* parent = nullptr;
    };
    template <typename C>
    static constexpr int size(const C &c)
    {
        if (!QRangeModelDetails::isValid(c))
            return 0;

        if constexpr (QRangeModelDetails::test_size<C>()) {
            return int(std::size(c));
        } else {
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
#if defined(__cpp_lib_ranges)
            using std::ranges::distance;
#else
            using std::distance;
#endif
            using container_type = std::conditional_t<QRangeModelDetails::range_traits<C>::has_cbegin,
                                                      const QRangeModelDetails::wrapped_t<C>,
                                                      QRangeModelDetails::wrapped_t<C>>;
            container_type& container = const_cast<container_type &>(QRangeModelDetails::refTo(c));
            return int(distance(QRangeModelDetails::adl_begin(container),
                                QRangeModelDetails::adl_end(container)));
#else
#if defined(__cpp_lib_ranges)
            return int(std::ranges::distance(QRangeModelDetails::begin(c),
                                             QRangeModelDetails::end(c)));
#else
            return int(std::distance(QRangeModelDetails::begin(c),
                                     QRangeModelDetails::end(c)));
#endif
#endif
        }
    }

    ~QtJambiRangeModelImpl()
    {
        // We delete row objects if we are not operating on a reference or pointer
        // to a range, as in that case, the owner of the referenced/pointed to
        // range also owns the row entries.
        // ### Problem: if we get a copy of a range (no matter if shared or not),
        // then adding rows will create row objects in the model's copy, and the
        // client can never delete those. But copied rows will be the same pointer,
        // which we must not delete (as we didn't create them).
        if constexpr (protocol_traits::has_deleteRow && !std::is_pointer_v<Range>
                      && !QRangeModelDetails::is_any_of<Range, std::reference_wrapper>()) {
            const auto begin = QRangeModelDetails::begin(*m_data.model());
            const auto end = QRangeModelDetails::end(*m_data.model());
            that().deleteRemovedRows(begin, end, true);
        }
    }

    static constexpr bool canInsertRows()
    {
        if constexpr (dynamicColumns() && !row_features::has_resize) {
            // If we operate on dynamic columns and cannot resize a newly
            // constructed row, then we cannot insert.
            return false;
        } else if constexpr (!protocol_traits::has_newRow) {
            // We also cannot insert if we cannot create a new row element
            return false;
        } else {
            return Structure::canInsertRows();
        }
    }

    const_row_reference rowData(const QModelIndex &index) const
    {
        Q_ASSERT(index.isValid());
        return that().rowDataImpl(index);
    }

    row_reference rowData(const QModelIndex &index)
    {
        Q_ASSERT(index.isValid());
        return that().rowDataImpl(index);
    }

    template <typename F>
    void readAt(const QModelIndex &index, F&& reader) const {
        const_row_reference row = rowData(index);
        if constexpr(treeType!=TreeType::None && rowType==RowType::Range){
            reader(*QRangeModelDetails::pos(row, index.column()));
        }else if constexpr (one_dimensional_range) {
            return reader(row);
        } else if (QRangeModelDetails::isValid(row)) {
            if constexpr (dynamicColumns()){
                reader(*QRangeModelDetails::pos(row, index.column()));
            }
        }
    }
    template <typename F>
    bool writeAt(const QModelIndex &index, F&& writer)
    {
        bool result = false;
        row_reference row = rowData(index);

        if constexpr(treeType!=TreeType::None && rowType==RowType::Range){
            if (QRangeModelDetails::isValid(row)){
                if(row->resizeIfNecessary()){
                }
                result = writer(*QRangeModelDetails::pos(row, index.column()));
            }
        }else if constexpr (one_dimensional_range) {
            result = writer(row);
        } else if (QRangeModelDetails::isValid(row)) {
            if constexpr (dynamicColumns()) {
                result = writer(*QRangeModelDetails::pos(row, index.column()));
            }
        }

        return result;
    }
public:
    explicit QtJambiRangeModelImpl(RootRange &&model, Protocol&& protocol, QRangeModel *itemModel, QGenericTableItemModelImpl<GenericTable>* owner)
        : Ancestor(itemModel)
        , m_data{std::forward<RootRange>(model)}
        , m_protocol(std::forward<Protocol>(protocol))
        , m_owner(owner)
    {
#if defined(Q_OS_WINDOWS)
        if(logRangeModel()){
            using Range_T = QRangeModelDetails::wrapped_t<Range>;
            //using Range_Void = std::void_t<std::tuple_element_t<0, Range_T>>;
            using RangeTraits = typename QRangeModelDetails::range_traits<Range_T>;
            using RangeTraitsValue = typename RangeTraits::value_type;
            using RangeTraitsValueReduced = std::remove_cv_t<QRangeModelDetails::wrapped_t<RangeTraitsValue>>;
            using RowTraits = QRangeModelDetails::row_traits<RangeTraitsValueReduced>;
            using RowTraits_ReducedT = q20::remove_cvref_t<RangeTraitsValueReduced>;
            using RowTraits_ReducedT_RangeTraits = QRangeModelDetails::range_traits<RowTraits_ReducedT>;
            printf("Range %s {\n", QtJambiAPI::typeName(typeid(Range)).constData());
            printf("    is_mutable_range: %s\n", is_mutable_range ? "true" : "false");
            printf("    is_mutable_row: %s\n", is_mutable_row ? "true" : "false");
            printf("    is_list_range: %s\n", is_list_range ? "true" : "false");
            printf("    is_list_row: %s\n", is_list_row ? "true" : "false");
            switch(rowType){
            case RowType::Data:
                printf("    rowType: Data\n");
                break;
            case RowType::MetaObject:
                printf("    rowType: MetaObject\n");
                break;
            case RowType::Range:
                printf("    rowType: Range\n");
                break;
            }
            //printf("    Super: %s\n", QtJambiAPI::typeName(typeid(typename Range::Super)).constData());
            printf("    is_tree_range: %s\n", QRangeModelDetails::is_tree_range<Range>::value ? "true" : "false");
            printf("    range_traits<Range>::value_type: %s\n", QtJambiAPI::typeName(typeid(typename QRangeModelDetails::range_traits<Range>::value_type)).constData());
            printf("    protocol_parentRow: %s\n", QRangeModelDetails::protocol_parentRow<QRangeModelDetails::DefaultTreeProtocol<Range>, typename QRangeModelDetails::range_traits<Range>::value_type>::value ? "true" : "false");
            printf("    protocol_childRows: %s\n", QRangeModelDetails::protocol_childRows<QRangeModelDetails::DefaultTreeProtocol<Range>, typename QRangeModelDetails::range_traits<Range>::value_type>::value ? "true" : "false");
            printf("    Impl: %s\n", QtJambiAPI::typeName(typeid(Self)).constData());
            printf("    RowTraits:{\n");
            printf("        <T>: %s\n", QtJambiAPI::typeName(typeid(RangeTraitsValueReduced)).constData());
            printf("        decltype: %s\n", QtJambiAPI::typeName(typeid(RowTraits)).constData());
            printf("        remove_cvref_t<T>: %s\n", QtJambiAPI::typeName(typeid(RowTraits_ReducedT)).constData());
            printf("        range_traits<T>{\n");
            printf("            decltype: %s\n", QtJambiAPI::typeName(typeid(RowTraits_ReducedT_RangeTraits)).constData());
            printf("            value: %s\n", RowTraits_ReducedT_RangeTraits::value ? "true" : "false");
            printf("        }\n");
            printf("        static_size: %d\n", RowTraits::static_size);
            printf("        void_t<tuple_element>: %s\n", QtJambiAPI::typeName(typeid(std::void_t<RowTraits>)).constData());
            printf("    }\n");
            printf("    RowTraits: %s\n", QtJambiAPI::typeName(typeid(RowTraits)).constData());
            printf("    range_traits: %s\n", QtJambiAPI::typeName(typeid(RangeTraits)).constData());
            printf("    range_traits::value_type: %s\n", QtJambiAPI::typeName(typeid(RangeTraitsValue)).constData());
            printf("    has_metaobject_v<range_traits::value_type>: %d\n", QRangeModelDetails::has_metaobject_v<RangeTraitsValue>);
            printf("    Protocol: %s\n", QtJambiAPI::typeName(typeid(Protocol)).constData());
            printf("    range_type: %s\n", QtJambiAPI::typeName(typeid(range_type)).constData());
            printf("    row_reference: %s\n", QtJambiAPI::typeName(typeid(row_reference)).constData());
            printf("    const_row_reference: %s\n", QtJambiAPI::typeName(typeid(const_row_reference)).constData());
            printf("    row_type: %s\n", QtJambiAPI::typeName(typeid(row_type)).constData());
            printf("    protocol_type: %s\n", QtJambiAPI::typeName(typeid(protocol_type)).constData());
            printf("    protocol_type.row_type: %s\n", QtJambiAPI::typeName(typeid(typename protocol_type::row_type)).constData());
            printf("    range_features:{\n");
            printf("        decltype: %s\n", QtJambiAPI::typeName(typeid(range_features)).constData());
            printf("        value: %s\n", range_features::value ? "true" : "false");
            printf("        is_mutable: %s\n", range_features::is_mutable ? "true" : "false");
            printf("        has_insert: %s\n", range_features::has_insert ? "true" : "false");
            printf("        has_insert_range: %s\n", range_features::has_insert_range ? "true" : "false");
            printf("        has_erase: %s\n", range_features::has_erase ? "true" : "false");
            printf("        has_resize: %s\n", range_features::has_resize ? "true" : "false");
            printf("    }\n");
            printf("    wrapped_row_type: %s\n", QtJambiAPI::typeName(typeid(wrapped_row_type)).constData());
            printf("    row_features:{\n");
            printf("        decltype: %s\n", QtJambiAPI::typeName(typeid(row_features)).constData());
            printf("        value: %s\n", row_features::value ? "true" : "false");
            printf("        value_type: %s\n", QtJambiAPI::typeName(typeid(typename row_features::value_type)).constData());
            printf("        is_mutable: %s\n", row_features::is_mutable ? "true" : "false");
            printf("        has_insert: %s\n", row_features::has_insert ? "true" : "false");
            //printf("        has_insert_range: %s\n", row_features::has_insert_range ? "true" : "false");
            printf("        has_erase: %s\n", row_features::has_erase ? "true" : "false");
            printf("        has_resize: %s\n", row_features::has_resize ? "true" : "false");
            printf("    }\n");
            printf("    row_traits:{\n");
            printf("        decltype: %s\n", QtJambiAPI::typeName(typeid(row_traits)).constData());
            printf("        static_size: %d\n", row_traits::static_size);
            printf("        fixed_size(): %d\n", row_traits::fixed_size());
            printf("    }\n");
            printf("    row_ptr: %s\n", QtJambiAPI::typeName(typeid(row_ptr)).constData());
            printf("    const_row_ptr: %s\n", QtJambiAPI::typeName(typeid(const_row_ptr)).constData());
            printf("    protocol_traits:{\n");
            printf("        decltype: %s\n", QtJambiAPI::typeName(typeid(protocol_traits)).constData());
            printf("        has_newRow: %s\n", protocol_traits::has_newRow ? "true" : "false");
            printf("        has_deleteRow: %s\n", protocol_traits::has_deleteRow ? "true" : "false");
            printf("        has_setParentRow: %s\n", protocol_traits::has_setParentRow ? "true" : "false");
            printf("        has_mutable_childRows: %s\n", protocol_traits::has_mutable_childRows ? "true" : "false");
            printf("        is_default: %s\n", protocol_traits::is_default ? "true" : "false");
            printf("    }\n");
            printf("}\n");
            _flushall();
            //return 0;
        }
#endif
    }

    QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        if (row < 0 || column < 0 || column >= that().columnCount(parent)
            || row >= that().rowCount(parent)) {
            return {};
        }

        return that().indexImpl(row, column, parent);
    }

    QModelIndex sibling(int row, int column, const QModelIndex &index) const
    {
        if (row == index.row() && column == index.column())
            return index;

        if (column < 0 || column >= this->itemModel().columnCount())
            return {};

        if (row == index.row()){
            return this->createIndex(row, column, index.constInternalPointer());
        }

        const_row_ptr parentRow = static_cast<const_row_ptr>(index.constInternalPointer());
        const auto siblingCount = size(that().childrenOf(parentRow));
        if (row < 0 || row >= int(siblingCount))
            return {};
        return this->createIndex(row, column, parentRow);
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        Qt::ItemFlags f = Structure::defaultFlags();
        if constexpr (!std::is_const_v<std::remove_pointer_t<decltype(std::declval<std::remove_pointer_t<row_type>>().data)>>){
            if constexpr(rowType!=RowType::Range || treeType==TreeType::None){
                if(const QMetaObject* mo = m_data.model()->elementMetaObject()){
                    if constexpr (rowType==RowType::MetaObject) {
                        if (index.column() < mo->propertyCount()-mo->propertyOffset()) {
                            const QMetaProperty prop = mo->property(index.column() + mo->propertyOffset());
                            if (prop.isWritable())
                                f |= Qt::ItemIsEditable;
                        }
                    }else{
                        QMetaProperty prop = mo->property(mo->indexOfProperty(roleNames()[Qt::DisplayRole]));
                        if(!prop.isValid()){
                            prop = mo->property(mo->indexOfProperty(roleNames()[Qt::EditRole]));
                        }
                        if(!prop.isValid()){
                            prop = mo->property(mo->propertyOffset());
                        }
                        if (prop.isWritable())
                            f |= Qt::ItemIsEditable;
                    }
                }
            }
            if constexpr (static_column_count <= 0) {
                if constexpr (isMutable()){
                    f |= Qt::ItemIsEditable;
                }
            }
        }
        return f;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        QVariant result;
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal
            || section < 0 || section >= that().columnCount({})) {
            return this->itemModel().QAbstractItemModel::headerData(section, orientation, role);
        }

        if constexpr (rowType==RowType::MetaObject) {
            const QMetaObject* mo = m_data.model()->elementMetaObject();
            if (mo->propertyCount()-mo->propertyOffset() == 1) {
                result = QString::fromUtf8(m_data.model()->elementMetaType().name());
            } else if (section <= mo->propertyCount()-mo->propertyOffset()) {
                const QMetaProperty prop = mo->property(section + mo->propertyOffset());
                result = QString::fromUtf8(prop.name());
            }
        } else if constexpr (static_column_count >= 1) {
            result = QString::fromUtf8(m_data.model()->elementMetaType().name());
        }
        if (!result.isValid())
            result = this->itemModel().QAbstractItemModel::headerData(section, orientation, role);
        return result;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        QVariant result;
        const auto readData = [this, index, column = index.column(), &result, role](const auto &value) {
            Q_UNUSED(this)
            Q_UNUSED(column)
            Q_UNUSED(index)
            auto valuePtr = QRangeModelDetails::pointerTo(value);
            if(!valuePtr->data)
                return;
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            if constexpr (has_itemAccess){
                if(JniEnvironment env{512}){
                    jobject itemAccess = m_data.model()->itemAccess(env);
                    Q_ASSERT(itemAccess);
                    jobject v;
                    if constexpr (itemsAreQObjects){
                        v = qtjambi_cast<jobject>(env, reinterpret_cast<const QObject*>(valuePtr->pointer()));
                    }else{
                        v = valuePtr->asJObject(env);
                    }
                    if(v)
                        result = qtjambi_cast<QVariant>(env, Java::QtCore::QRangeModel$ItemAccess::readRole(env, itemAccess, v, role));
                }
            }else
#endif
            {
                if constexpr (std::is_same_v<decltype(&*valuePtr->containerAccess), AbstractContainerAccess*>){
                    if(MultiRole::Type multiRoleType = MultiRole::isMultiRole(valuePtr->metaType, &*valuePtr->containerAccess)){
                        AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(&*valuePtr->containerAccess);
                        if(multiRoleType==MultiRole::String){
                            const QString& name = roleNames().value(role);
                            if(const QVariant* val = reinterpret_cast<const QVariant*>(associativeElementAccess->value(valuePtr->pointer(), &name)))
                                result = *val;
                        }else{
                            if(const QVariant* val = reinterpret_cast<const QVariant*>(associativeElementAccess->value(valuePtr->pointer(), &role)))
                                result = *val;
                        }
                        return;
                    }
                }
                //if constexpr (rowType!=RowType::Range || treeType==TreeType::None)
                {
                    if(auto mo = valuePtr->metaType.metaObject()) {
                        if constexpr(rowType!=RowType::MetaObject){
                            result = readRole(index, role, valuePtr);
                        }else{
                            if (isPrimaryRole(role)) {
                                result = readProperty(index, valuePtr);
                            }
                        }
                        return;
                    }
                }
                if ((isPrimaryRole(role) || isRangeModelRole(role))) {
                    result = read(value);
                }
            }
        };

        if (index.isValid())
            readAt(index, readData);

        return result;
    }

    QMap<int, QVariant> itemData(const QModelIndex &index) const
    {
        QMap<int, QVariant> result;
        bool tried = false;
        const auto readItemData = [this, index, &result, &tried](const auto &value){
            Q_UNUSED(this);
            Q_UNUSED(index);
            Q_UNUSED(tried);
            auto valuePtr = QRangeModelDetails::pointerTo(value);
            if(!valuePtr->data)
                return;
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            if constexpr (has_itemAccess){
                if(JniEnvironment env{512}){
                    jobject itemAccess = m_data.model()->itemAccess(env);
                    Q_ASSERT(itemAccess);
                    jobject v;
                    if constexpr (itemsAreQObjects){
                        v = qtjambi_cast<jobject>(env, reinterpret_cast<const QObject*>(valuePtr->pointer()));
                    }else{
                        v = valuePtr->asJObject(env);
                    }
                    if(v){
                        for (auto role : roleNames().keys()) {
                            QVariant data = qtjambi_cast<QVariant>(env, Java::QtCore::QRangeModel$ItemAccess::readRole(env, itemAccess, v, role));
                            if (data.isValid())
                                result[role] = std::move(data);
                        }
                    }
                }
            }else
#endif
            {
                if constexpr (std::is_same_v<decltype(&*valuePtr->containerAccess), AbstractContainerAccess*>){
                    if(MultiRole::Type multiRoleType = MultiRole::isMultiRole(valuePtr->metaType, &*valuePtr->containerAccess)){
                        tried = true;
                        if(valuePtr->metaType==QMetaType::fromType<QMap<int, QVariant>>()
                            || QMetaType::canConvert(valuePtr->metaType, QMetaType::fromType<QMap<int, QVariant>>())){
                            result = read(value).template value<QMap<int, QVariant>>();
                        } else {
                            AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(&*valuePtr->containerAccess);
                            auto keyValueIterator = associativeElementAccess->constKeyValueIterator(valuePtr->pointer());
                            while(keyValueIterator->hasNext()) {
                                auto it = keyValueIterator->constNext();
                                int role;
                                if(multiRoleType==MultiRole::String){
                                    role = roleNames().key(reinterpret_cast<const QString*>(it.first)->toUtf8(), -1);
                                }else if(multiRoleType==MultiRole::ItemDataRole){
                                    role = *reinterpret_cast<const Qt::ItemDataRole*>(it.first);
                                }else{
                                    role = *reinterpret_cast<const int*>(it.first);
                                }
                                if (role != -1)
                                    result.insert(role, *reinterpret_cast<const QVariant*>(it.second));
                            }
                        }
                        return;
                    }
                }
                if (auto mo = valuePtr->metaType.metaObject()) {
                    if constexpr(rowType!=RowType::MetaObject){
                        tried = true;
                        for (auto &&[role, roleName] : roleNames().asKeyValueRange()) {
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
                            if constexpr (itemsAreQObjects){
                                if (const QObject* obj = reinterpret_cast<const QObject*>(valuePtr->pointer())){
                                    const int pi = mo->indexOfProperty(roleName);
                                    if (pi >= 0) {
                                        connectPropertyOnRead(index, role, obj, mo->property(pi));
                                    }
                                }
                            }
#endif
                            QVariant data = readProperty(roleName, valuePtr);
                            if (data.isValid())
                                result[role] = std::move(data);
                        }
                    }else{
                        int columnCount = mo->propertyCount() - mo->propertyOffset();
                        if (columnCount <= 1) {
                            tried = true;
                            for (auto &&[role, roleName] : roleNames().asKeyValueRange()) {
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
                                if constexpr (itemsAreQObjects){
                                    if (const QObject* obj = reinterpret_cast<const QObject*>(valuePtr->pointer())){
                                        const int pi = mo->indexOfProperty(roleName);
                                        if (pi >= 0) {
                                            connectPropertyOnRead(index, role, obj, mo->property(pi));
                                        }
                                    }
                                }
#endif
                                QVariant data = readProperty(roleName, valuePtr);
                                if (data.isValid())
                                    result[role] = std::move(data);
                            }
                        }
                    }
                }
            }
        };

        if (index.isValid()) {
            readAt(index, readItemData);

            if (!tried) // no multi-role item found
                result = this->itemModel().QAbstractItemModel::itemData(index);
        }
        return result;
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    void multiData(const QModelIndex &index, QModelRoleDataSpan roleDataSpan) const    {
        for (auto &roleData : roleDataSpan) {
            roleData.setData(data(index, roleData.role()));
        }
    }

    auto maybeBlockDataChangedDispatch()
    {
        if constexpr (itemsAreQObjects || rowsAreQObjects)
            return this->blockDataChangedDispatch();
        else
            return false;
    }

    void setAutoConnectPolicy()    {
        // will be 'void' if columns don't all have the same type
        if constexpr (itemsAreQObjects || rowsAreQObjects) {
            if(const QMetaObject* mo = m_data.model()->elementMetaObject()){
                delete m_data.context;
                m_data.connections = {};
                switch (this->autoConnectPolicy()) {
                case QRangeModelImplBase::AutoConnectPolicy::None:
                    m_data.context = nullptr;
                    break;
                case QRangeModelImplBase::AutoConnectPolicy::Full:
                    m_data.context = new QRangeModelDetails::AutoConnectContext(&this->itemModel());
                    if constexpr (itemsAreQObjects) {
                        m_data.context->mapping = QRangeModelDetails::AutoConnectContext::AutoConnectMapping::Roles;
                        m_data.properties = QRangeModelImplBase::roleProperties(this->itemModel(), *mo);
                    } else {
                        m_data.properties = QRangeModelImplBase::columnProperties(*mo);
                        m_data.context->mapping = QRangeModelDetails::AutoConnectContext::AutoConnectMapping::Columns;
                    }
                    if (!m_data.properties.isEmpty())
                        that().autoConnectPropertiesImpl();
                    break;
                case QRangeModelImplBase::AutoConnectPolicy::OnRead:
                    m_data.context = new QRangeModelDetails::AutoConnectContext(&this->itemModel());
                    if constexpr (itemsAreQObjects) {
                        m_data.context->mapping = QRangeModelDetails::AutoConnectContext::AutoConnectMapping::Roles;
                    } else {
                        m_data.properties = QRangeModelImplBase::columnProperties(*mo);
                        m_data.context->mapping = QRangeModelDetails::AutoConnectContext::AutoConnectMapping::Columns;
                    }
                    break;
                }
            }
        } else {
#ifndef QT_NO_DEBUG
            qWarning("All items in the range must be QObject subclasses");
#endif
        }
    }
#endif //QT_VERSION < QT_VERSION_CHECK(6,11,0)

    static constexpr bool isRangeModelRole(int role)
    {
        return role == Qt::RangeModelDataRole
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
               || role == Qt::RangeModelAdapterRole
#endif
            ;
    }

    static constexpr bool isPrimaryRole(int role)
    {
        return role == Qt::DisplayRole || role == Qt::EditRole;
    }

    bool setData(const QModelIndex &index, const QVariant &data, int role)
    {
        if (!index.isValid())
            return false;

        bool success = false;
        if constexpr (isMutable()/* || rowType==RowType::MetaObject*/) {
            auto emitDataChanged = qScopeGuard([&success, this, &index, &role]{
                if (success) {
                    Q_EMIT this->dataChanged(index, index, role == Qt::EditRole
                                                         ? QList<int>{} : QList{role});
                }
            });
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            // we emit dataChanged at the end, block dispatches from auto-connected properties
            [[maybe_unused]] auto dataChangedBlocker = maybeBlockDataChangedDispatch();
#endif //QT_VERSION < QT_VERSION_CHECK(6,11,0)

            const auto writeData = [this, column = index.column(), &data, role](auto &&target) -> bool {
                auto targetPtr = QRangeModelDetails::pointerTo(target);
                if(!targetPtr->data)
                    return false;
                if constexpr (!std::is_const_v<std::remove_pointer_t<decltype(targetPtr->data)>>){
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
                    if constexpr (has_itemAccess){
                        if(JniEnvironment env{512}){
                            jobject itemAccess = m_data.model()->itemAccess(env);
                            Q_ASSERT(itemAccess);
                            jobject v;
                            if constexpr (itemsAreQObjects){
                                v = qtjambi_cast<jobject>(env, reinterpret_cast<QObject*>(targetPtr->pointer()));
                            }else{
                                v = targetPtr->asJObject(env);
                            }
                            if(v)
                                return Java::QtCore::QRangeModel$ItemAccess::writeRole(env, itemAccess, v, role, qtjambi_cast<jobject>(env, data));
                        }
                    }else
#endif
                    {
                        if constexpr (std::is_same_v<decltype(&*targetPtr->containerAccess), AbstractContainerAccess*>){
                            if(MultiRole::Type multiRoleType = MultiRole::isMultiRole(targetPtr->metaType, &*targetPtr->containerAccess)){
                                AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(&*targetPtr->containerAccess);
                                Qt::ItemDataRole roleToSet = Qt::ItemDataRole(role);
                                // If there is an entry for EditRole, overwrite that; otherwise,
                                // set the entry for DisplayRole.
                                void* container = targetPtr->pointer();
                                if (role == Qt::EditRole) {
                                    if (multiRoleType==MultiRole::String) {
                                        QString name = roleNames().value(roleToSet);
                                        if (!associativeElementAccess->contains(container, &name))
                                            roleToSet = Qt::DisplayRole;
                                    } else {
                                        if (!associativeElementAccess->contains(container, &role))
                                            roleToSet = Qt::DisplayRole;
                                    }
                                }
                                if (multiRoleType==MultiRole::String){
                                    QString name = roleNames().value(roleToSet);
                                    associativeElementAccess->insert(container, &name, &data);
                                }else{
                                    associativeElementAccess->insert(container, &roleToSet, &data);
                                }
                                return true;
                            }
                        }
                        if (auto mo = targetPtr->metaType.metaObject()) {
                            if constexpr(rowType!=RowType::MetaObject){
                                return writeRole(role, QRangeModelDetails::pointerTo(target), data);
                            }else{
                                int columnCount = mo->propertyCount() - mo->propertyOffset();
                                if (targetPtr->metaType == data.metaType()) {
                                    return write(target, data);
                                } else if (columnCount <= 1) {
                                    return writeRole(role, QRangeModelDetails::pointerTo(target), data);
                                } else if (column <= columnCount
                                           && ((isPrimaryRole(role) || isRangeModelRole(role)))) {
                                    return writeProperty(column, QRangeModelDetails::pointerTo(target), data);
                                }
                            }
                        }
                        if ((isPrimaryRole(role) || isRangeModelRole(role))) {
                            return write(target, data);
                        }
                    }
                }else{
                    Q_UNUSED(this)
                    Q_UNUSED(column)
                    Q_UNUSED(target)
                }
                return false;
            };

            success = writeAt(index, writeData);

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            if constexpr (itemsAreQObjects || rowsAreQObjects) {
                if (success && isRangeModelRole(role) && this->autoConnectPolicy() == QRangeModelImplBase::AutoConnectPolicy::Full) {
                    if (QObject *item = data.value<QObject *>())
                        Self::connectProperties(index, item, m_data.context, m_data.properties);
                }
            }
#endif
        }
        return success;
    }

    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &data)
    {
        if (!index.isValid() || data.isEmpty())
            return false;

        bool success = false;
        if constexpr (isMutable()) {
            auto emitDataChanged = qScopeGuard([&success, this, &index, &data]{
                if (success)
                    Q_EMIT this->dataChanged(index, index, data.keys());
            });
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            // we emit dataChanged at the end, block dispatches from auto-connected properties
            [[maybe_unused]] auto dataChangedBlocker = maybeBlockDataChangedDispatch();
#endif //QT_VERSION < QT_VERSION_CHECK(6,11,0)

            bool tried = false;
            auto writeItemData = [this, &tried, &data](auto &&target) -> bool {
                Q_UNUSED(tried)
                auto targetPtr = QRangeModelDetails::pointerTo(target);
                if(!targetPtr->data)
                    return false;
                if constexpr (!std::is_const_v<std::remove_pointer_t<decltype(targetPtr->data)>>){
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
                    if constexpr (has_itemAccess){
                        if(JniEnvironment env{512}){
                            jobject itemAccess = m_data.model()->itemAccess(env);
                            Q_ASSERT(itemAccess);
                            jobject v;
                            if constexpr (itemsAreQObjects){
                                v = qtjambi_cast<jobject>(env, reinterpret_cast<QObject*>(targetPtr->pointer()));
                            }else{
                                v = targetPtr->asJObject(env);
                            }
                            if(v){
                                for (auto &&[role, value] : data.asKeyValueRange()) {
                                    if (!Java::QtCore::QRangeModel$ItemAccess::writeRole(env, itemAccess, v, role, qtjambi_cast<jobject>(env, value))) {
                                        qWarning("Failed to write value for %s", roleNames().value(role).data());
                                        return false;
                                    }
                                }
                                return true;
                            }
                        }
                    }else
#endif
                    {
                        if constexpr (std::is_same_v<decltype(&*targetPtr->containerAccess), AbstractContainerAccess*>){
                            if(MultiRole::Type multiRoleType = MultiRole::isMultiRole(targetPtr->metaType, &*targetPtr->containerAccess)){
                                AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(&*targetPtr->containerAccess);
                                tried = true;
                                const auto roleName = [map = roleNames()](int role) { return map.value(role); };

                                // transactional: only update target if all values from data
                                // can be stored. Storing never fails with int-keys.
                                if(multiRoleType==MultiRole::String)
                                {
                                    auto invalid = std::find_if(data.keyBegin(), data.keyEnd(),
                                        [&roleName](int role) { return roleName(role).isEmpty(); }
                                    );

                                    if (invalid != data.keyEnd()) {
                                        qWarning("No role name set for %d", *invalid);
                                        return false;
                                    }
                                }

                                void* container = targetPtr->pointer();
                                for (auto &&[role, value] : data.asKeyValueRange()) {
                                    if(multiRoleType==MultiRole::String){
                                        QString key = QString::fromUtf8(roleName(role));
                                        associativeElementAccess->insert(container, &key, &value);
                                    }else
                                        associativeElementAccess->insert(container, &role, &value);
                                }
                                return true;
                            }
                        }
                        if (auto mo = targetPtr->metaType.metaObject()) {
                            if constexpr(rowType!=RowType::MetaObject){
                                tried = true;
                                for (auto &&[role, value] : data.asKeyValueRange()) {
                                    const QByteArray roleName = roleNames().value(role);
                                    if (!writeProperty(roleName, QRangeModelDetails::pointerTo(target), value)) {
                                        qWarning("Failed to write value for %s", roleName.data());
                                        return false;
                                    }
                                }
                                return true;
                            }else{
                                int columnCount = mo->propertyCount() - mo->propertyOffset();
                                if (columnCount <= 1) {
                                    tried = true;
                                    for (auto &&[role, value] : data.asKeyValueRange()) {
                                        const QByteArray roleName = roleNames().value(role);
                                        if (!writeProperty(roleName, QRangeModelDetails::pointerTo(target), value)) {
                                            qWarning("Failed to write value for %s", roleName.data());
                                            return false;
                                        }
                                    }
                                    return true;
                                }
                            }
                        }
                    }
                }else{
                    Q_UNUSED(this)
                    Q_UNUSED(target)
                }
                return false;
            };

            success = writeAt(index, writeItemData);

            if (!tried) {
                // setItemData will emit the dataChanged signal
                Q_ASSERT(!success);
                emitDataChanged.dismiss();
                success = this->itemModel().QAbstractItemModel::setItemData(index, data);
            }
        }
        return success;
    }

    bool clearItemData(const QModelIndex &index)
    {
        if (!index.isValid())
            return false;

        bool success = false;
        if constexpr (isMutable()) {
            auto emitDataChanged = qScopeGuard([&success, this, &index]{
                if (success)
                    Q_EMIT this->dataChanged(index, index, {});
            });

            auto clearData = [this, column = index.column()](auto &&target) {
                auto targetPtr = QRangeModelDetails::pointerTo(target);
                if(!targetPtr->data)
                    return false;
                if constexpr (!std::is_const_v<std::remove_pointer_t<decltype(targetPtr->data)>>){
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
                    if constexpr (has_itemAccess){
                        /*if(JniEnvironment env{512}){
                            jobject itemAccess = m_data.model()->itemAccess(env);
                            Q_ASSERT(itemAccess);
                            jobject v;
                            if constexpr (itemsAreQObjects){
                                v = qtjambi_cast<jobject>(env, reinterpret_cast<QObject*>(targetPtr->pointer()));
                            }else{
                                v = targetPtr->asJObject(env);
                            }
                            if(v)
                                return Java::QtCore::QRangeModel$ItemAccess::writeRole(env, itemAccess, v, role, qtjambi_cast<jobject>(env, QVariant(targetPtr->metaType)));
                        }*/
                    }else
#endif
                    {
                        if constexpr (std::is_same_v<decltype(&*targetPtr->containerAccess), AbstractContainerAccess*>){
                            if(MultiRole::isMultiRole(targetPtr->metaType, &*targetPtr->containerAccess)){
                                AbstractAssociativeAccess* associativeElementAccess = static_cast<AbstractAssociativeAccess*>(&*targetPtr->containerAccess);
                                void* container = targetPtr->pointer();
                                void* empty = associativeElementAccess->createContainer();
                                associativeElementAccess->assign(container, empty);
                                associativeElementAccess->deleteContainer(empty);
                                return true;
                            }
                        }
                        if (auto mo = targetPtr->metaType.metaObject()) {
                            int columnCount = mo->propertyCount() - mo->propertyOffset();
                            if (columnCount <= 1) {
                                // multi-role object/gadget: reset all properties
                                return resetProperty(-1, QRangeModelDetails::pointerTo(target));
                            } else if (column <= columnCount) {
                                return resetProperty(column, QRangeModelDetails::pointerTo(target));
                            }
                        } else { // normal structs, values, associative containers
                            return true;
                        }
                    }
                }else{
                    Q_UNUSED(this)
                    Q_UNUSED(column)
                    Q_UNUSED(target)
                }
                return false;
            };

            success = writeAt(index, clearData);
        }
        return success;
    }

    QHash<int, QByteArray> roleNames() const
    {
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
        if constexpr (has_itemAccess){
            return this->itemModel().QAbstractItemModel::roleNames();
        }else
#endif
        if(MultiRole::isMultiRole(m_data.model()->elementMetaType(), &*m_data.model()->containerAccess())){
            return this->itemModel().QAbstractItemModel::roleNames();
        }else if (const QMetaObject* mo = m_data.model()->elementMetaObject()) {
            if constexpr(rowType!=RowType::MetaObject){
                return this->roleNamesForMetaObject(this->itemModel(), *mo);
            }else{
                return this->itemModel().QAbstractItemModel::roleNames();
            }
        }else if(AbstractSequentialAccess* listAccess = m_data.model()->containerAccess().get()){
            if(QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess{listAccess->elementNestedContainerAccess(), &containerDisposer}){
                if(MultiRole::isMultiRole(listAccess->elementMetaType(), elementNestedContainerAccess.get())){
                    return this->itemModel().QAbstractItemModel::roleNames();
                }else if (const QMetaObject* mo = listAccess->elementMetaType().metaObject()){
                    return this->roleNamesForMetaObject(this->itemModel(), *mo);
                }else if(elementNestedContainerAccess->isSequential()){
                    if (const QMetaObject* mo = static_cast<AbstractSequentialAccess*>(elementNestedContainerAccess.get())->elementMetaType().metaObject()){
                        return this->roleNamesForMetaObject(this->itemModel(), *mo);
                    }
                }
            }
        }
        return this->roleNamesForSimpleType();
    }

    bool insertColumns(int column, int count, const QModelIndex &parent)
    {
        Q_UNUSED(column)
        Q_UNUSED(count)
        Q_UNUSED(parent)
        if constexpr (dynamicColumns() && isMutable() && row_features::has_insert) {
            if (count == 0)
                return false;
            range_type * const children = childRange(parent);
            if (!children)
                return false;

            this->beginInsertColumns(parent, column, column + count - 1);
            for (auto child : *children) {
                auto it = QRangeModelDetails::pos(child, column);
                QRangeModelDetails::refTo(child).insert(it, count, {});
            }
            this->endInsertColumns();
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            // endInsertColumns emits columnsInserted, at which point clients might
            // have populated the new columns with objects (if the columns aren't objects
            // themselves).
            if constexpr (itemsAreQObjects) {
                if (m_data.context && this->autoConnectPolicy() == QRangeModelImplBase::AutoConnectPolicy::Full) {
                    for (int r = 0; r < that().rowCount(parent); ++r) {
                        for (int c = column; c < column + count; ++c) {
                            const QModelIndex index = that().index(r, c, parent);
                            writeAt(index, [this, &index](auto&& item) -> bool {
                                return Self::connectProperties(index, const_cast<QObject *>(reinterpret_cast<const QObject *>(item.data)),
                                                               m_data.context, m_data.properties);
                            });
                        }
                    }
                }
            }
#endif
            return true;
        }
        return false;
    }

    bool removeColumns(int column, int count, const QModelIndex &parent)
    {
        Q_UNUSED(column)
        Q_UNUSED(count)
        Q_UNUSED(parent)
        if constexpr (dynamicColumns() && isMutable() && row_features::has_erase) {
            if (column < 0 || column + count > that().columnCount(parent))
                return false;

            range_type * const children = childRange(parent);
            if (!children)
                return false;

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            if constexpr (itemsAreQObjects) {
                if (m_data.context && this->autoConnectPolicy() == QRangeModelImplBase::AutoConnectPolicy::OnRead) {
                    for (int r = 0; r < that().rowCount(parent); ++r) {
                        for (int c = column; c < column + count; ++c) {
                            const QModelIndex index = that().index(r, c, parent);
                            writeAt(index, [this](auto&& item) -> bool {
                                m_data.connections.removeIf([item](const auto &connection) {
                                    return connection.sender == reinterpret_cast<const QObject *>(item.data);
                                });
                                return true;
                            });
                        }
                    }
                }
            }
#endif
            this->beginRemoveColumns(parent, column, column + count - 1);
            for (auto child : *children) {
                const auto start = QRangeModelDetails::pos(child, column);
                QRangeModelDetails::refTo(child).erase(start, std::next(start, count));
            }
            this->endRemoveColumns();
            return true;
        }
        return false;
    }

    bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count,
                     const QModelIndex &destParent, int destColumn)
    {
        Q_UNUSED(sourceParent)
        Q_UNUSED(sourceColumn)
        Q_UNUSED(count)
        Q_UNUSED(destParent)
        Q_UNUSED(destColumn)
        // we only support moving columns within the same parent
        if (sourceParent != destParent)
            return false;
        if constexpr (isMutable()) {
            if (!Structure::canMoveColumns(sourceParent, destParent))
                return false;
            if constexpr (dynamicColumns()) {
                using DataType = decltype(*std::declval<std::remove_pointer_t<row_type>>().begin());
                if constexpr(!std::is_same_v<DataType, ConstRow<has_itemAccess>>){
                    // we only support ranges as columns, as other types might
                    // not have the same data type across all columns
                    range_type * const children = childRange(sourceParent);
                    if (!children)
                        return false;

                    if (!this->beginMoveColumns(sourceParent, sourceColumn, sourceColumn + count - 1,
                                          destParent, destColumn)) {
                        return false;
                    }

                    for (auto child : *children) {
                        const auto first = QRangeModelDetails::pos(child, sourceColumn);
                        const auto middle = std::next(first, count);
                        const auto last = QRangeModelDetails::pos(child, destColumn);

                        if (sourceColumn < destColumn) // moving right
                            rotate_by_swap(first, middle, last);
                        else // moving left
                            rotate_by_swap(last, first, middle);
                    }

                    this->endMoveColumns();
                    return true;
                }
            }
        }
        return false;
    }

    bool insertRows(int row, int count, const QModelIndex &parent)
    {
        Q_UNUSED(row)
        Q_UNUSED(count)
        Q_UNUSED(parent)
        if constexpr (canInsertRows()) {
            range_type *children = childRange(parent);
            if (!children)
                return false;
            if constexpr (treeType!=TreeType::None) {
                if(!parent.isValid()){
                    if(children->containerAccess() && children->containerAccess()->isSpan()){
                        return false;
                    }
                }else{
                    if(children->containerAccess() && children->containerAccess()->isSpan()){
                        if(static_cast<AbstractSpanAccess*>(children->containerAccess().get())->isConst())
                            return false;
                    }
                }
            }
            EmptyRowGenerator generator{0, &that(), &parent};

            this->beginInsertRows(parent, row, row + count - 1);

            const auto pos = QRangeModelDetails::pos(children, row);
            if constexpr (range_features::has_insert_range) {
                children->insert(pos, generator, EmptyRowGenerator{count});
            } else if constexpr (treeType==TreeType::None) {
                switch(children->elementDataType()){
                case MetaTypeUtils::Value:
                    children->insert(pos, count, *generator);
                    break;
                default:
                    auto start = children->insert(pos, count, row_type{});
                    std::copy(generator, EmptyRowGenerator{count}, start);
                    break;
                }
            } else {
                children->insert(pos, count, *generator);
            }

            // fix the parent in all children of the modified row, as the
            // references back to the parent might have become invalid.
            that().resetParentInChildren(children);

            this->endInsertRows();

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            if constexpr(itemsAreQObjects || rowsAreQObjects){
                if (m_data.context && this->autoConnectPolicy() == QRangeModelImplBase::AutoConnectPolicy::Full) {
                    const auto begin = QRangeModelDetails::pos(children, row);
                    const auto end = std::next(begin, count);
                    int rowIndex = row;
                    for (auto it = begin; it != end; ++it, ++rowIndex)
                        autoConnectPropertiesInRow(*it, rowIndex, parent);
                }
            }
#endif
            return true;
        } else
        {
            return false;
        }
    }

    bool removeRows(int row, int count, const QModelIndex &parent = {})
    {
        Q_UNUSED(row)
        Q_UNUSED(count)
        Q_UNUSED(parent)
        if constexpr (Structure::canRemoveRows()) {
            const int prevRowCount = that().rowCount(parent);
            if (row < 0 || row + count > prevRowCount)
                return false;

            range_type *children = childRange(parent);
            if (!children)
                return false;

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            if constexpr(itemsAreQObjects || rowsAreQObjects){
                if (m_data.context && this->autoConnectPolicy() == QRangeModelImplBase::AutoConnectPolicy::OnRead) {
                    const auto begin = QRangeModelDetails::pos(children, row);
                    const auto end = std::next(begin, count);
                    int rowIndex = row;
                    for (auto it = begin; it != end; ++it, ++rowIndex)
                        clearConnectionInRow(*it, rowIndex, parent);
                }
            }
#endif

            if constexpr (treeType!=TreeType::None) {
                if(!parent.isValid()){
                    if(children->containerAccess() && children->containerAccess()->isSpan()){
                        return false;
                    }
                }else{
                    if(children->containerAccess() && children->containerAccess()->isSpan()){
                        if(static_cast<AbstractSpanAccess*>(children->containerAccess().get())->isConst())
                            return false;
                    }
                }
            }

            this->beginRemoveRows(parent, row, row + count - 1);
            [[maybe_unused]] bool callEndRemoveColumns = false;
            if constexpr (dynamicColumns()) {
                // if we remove the last row in a dynamic model, then we no longer
                // know how many columns we should have, so they will be reported as 0.
                if (prevRowCount == count) {
                    if (const int columns = that().columnCount(parent)) {
                        callEndRemoveColumns = true;
                        this->beginRemoveColumns(parent, 0, columns - 1);
                    }
                }
            }
            { // erase invalidates iterators
                const auto begin = QRangeModelDetails::pos(children, row);
                const auto end = std::next(begin, count);
                that().deleteRemovedRows(begin, end);
                children->erase(begin, end);
            }
            // fix the parent in all children of the modified row, as the
            // references back to the parent might have become invalid.
            that().resetParentInChildren(children);

            if constexpr (dynamicColumns()) {
                if (callEndRemoveColumns) {
                    Q_ASSERT(that().columnCount(parent) == 0);
                    this->endRemoveColumns();
                }
            }
            this->endRemoveRows();
            return true;
        } else
        {
            return false;
        }
    }

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                  const QModelIndex &destParent, int destRow)
    {
        Q_UNUSED(sourceParent)
        Q_UNUSED(sourceRow)
        Q_UNUSED(count)
        Q_UNUSED(destParent)
        Q_UNUSED(destRow)
        if constexpr (isMutable() && std::remove_pointer_t<row_type>::mutableRow) {
            if (!Structure::canMoveRows(sourceParent, destParent))
                return false;

            if (sourceParent != destParent) {
                return that().moveRowsAcross(sourceParent, sourceRow, count,
                                             destParent, destRow);
            }

            if (sourceRow == destRow || sourceRow == destRow - 1 || count <= 0
                || sourceRow < 0 || sourceRow + count - 1 >= this->itemModel().rowCount(sourceParent)
                || destRow < 0 || destRow > this->itemModel().rowCount(destParent)) {
                return false;
            }

            range_type *source = childRange(sourceParent);
            // moving within the same range
            if (!this->beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destParent, destRow))
                return false;

            const auto first = QRangeModelDetails::pos(source, sourceRow);
            const auto middle = std::next(first, count);
            const auto last = QRangeModelDetails::pos(source, destRow);

            if (sourceRow < destRow) // moving down
                rotate_by_swap(first, middle, last);
            else // moving up
                rotate_by_swap(last, first, middle);

            that().resetParentInChildren(source);

            this->endMoveRows();
            return true;
        } else
        {
            return false;
        }
    }

private:
    static QVariant readProperty(const QMetaProperty &prop, const void *gadget)
    {
        return gadget ? prop.readOnGadget(gadget) : QVariant{};
    }

    static QVariant readProperty(const QMetaProperty &prop, const QObject *object)
    {
        return object ? prop.read(object) : QVariant{};
    }

    static bool writeProperty(const QMetaProperty &prop, void *gadget, const QVariant &v)
    {
        return gadget && prop.writeOnGadget(gadget, v);
    }
    static bool writeProperty(const QMetaProperty &prop, QObject *object, const QVariant &v)
    {
        return object && prop.write(object, v);
    }

    QMetaProperty roleProperty(int role, const QMetaObject* mo) const
    {
        struct {
            operator QMetaProperty() const {
                const QByteArray roleName = that.itemModel().roleNames().value(role);
                if (const int index = mo->indexOfProperty(roleName.data());
                    index >= 0) {
                    return mo->property(index);
                }
                return {};
            }
            const QtJambiRangeModelImpl &that;
            const int role;
            const QMetaObject* mo;
        } findProperty{*this, role, mo};

        if constexpr (ModelData::cachesProperties)
            return *m_data.properties.tryEmplace(role, findProperty).iterator;
        else
            return findProperty;
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    void connectPropertyOnRead(const QModelIndex &index, int role,
                               const QObject *gadget, const QMetaProperty &prop) const
    {
        const typename ModelData::Connection connection = {gadget, role};
        if (prop.hasNotifySignal() && this->autoConnectPolicy() == QRangeModelImplBase::AutoConnectPolicy::OnRead
            && !m_data.connections.contains(connection)) {
            if constexpr (isMutable())
                Self::connectProperty(index, gadget, m_data.context, role, prop);
            else
                Self::connectPropertyConst(index, gadget, m_data.context, role, prop);
            m_data.connections.insert(connection);
        }
    }
#endif //QT_VERSION >= QT_VERSION_CHECK(6,11,0)

    template <typename ItemType>
    QVariant readRole(const QModelIndex &index, int role, const ItemType *gadget) const
    {
        const QMetaObject* mo = gadget->metaType.metaObject();
        QVariant result;
        QMetaProperty prop = roleProperty(role, mo);
        if (!prop.isValid() && role == Qt::EditRole)
            prop = roleProperty(Qt::DisplayRole, mo);

        if (prop.isValid()){
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            if constexpr (itemsAreQObjects){
                QObject* object = const_cast<QObject*>(reinterpret_cast<const QObject*>(gadget->data));
                connectPropertyOnRead(index, role, object, prop);
            }
#else
            Q_UNUSED(index)
#endif
            result = readProperty(prop, gadget);
        }
        return result;
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    QVariant readRole(const QModelIndex &index, int role, const ItemType &gadget) const
    {
        return readRole(index, role, &gadget);
    }

    template <typename ItemType>
    bool writeRole(int role, ItemType *gadget, const QVariant &data) const
    {
        const QMetaObject* mo = gadget->metaType.metaObject();
        auto prop = roleProperty(role, mo);
        if (!prop.isValid() && role == Qt::EditRole)
            prop = roleProperty(Qt::DisplayRole, mo);

        return writeProperty(prop, gadget, data);
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    bool writeRole(int role, ItemType &&gadget, const QVariant &data) const
    {
        return writeRole(role, &gadget, data);
    }

    template <typename ItemType>
    static QVariant readProperty(const QMetaProperty &prop, const ItemType *gadget)
    {
        if constexpr (itemsAreQObjects){
            return readProperty(prop, reinterpret_cast<const QObject*>(gadget->pointer()));
        }else if(gadget->metaType.flags() & QMetaType::PointerToQObject){
            return readProperty(prop, reinterpret_cast<const QObject*>(gadget->pointer()));
        }else if(gadget->metaType.flags() & QMetaType::PointerToGadget){
            return readProperty(prop, gadget->pointer());
        }else{
            return readProperty(prop, gadget->pointer());
        }
    }

    template <typename ItemType>
    QVariant readProperty(const QModelIndex &index, const ItemType *gadget) const
    {
        const QMetaObject* mo = gadget->metaType.metaObject();
        const QMetaProperty prop = mo->property(index.column() + mo->propertyOffset());
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
        if constexpr (rowsAreQObjects){
            if(mo->inherits(&QObject::staticMetaObject)){
                QObject* object = const_cast<QObject*>(reinterpret_cast<const QObject*>(gadget->pointer()));
                connectPropertyOnRead(index, Qt::DisplayRole, object, prop);
            }
        }
#endif
        return readProperty(prop, gadget);
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    QVariant readProperty(const QModelIndex &index, const ItemType &gadget) const
    {
        return readProperty(index, &gadget);
    }

    template <typename ItemType>
    static QVariant readProperty(int property, const ItemType *gadget)
    {
        const QMetaObject* mo = gadget->metaType.metaObject();
        const QMetaProperty prop = mo->property(property + mo->propertyOffset());
        return readProperty(prop, gadget);
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    static QVariant readProperty(int property, const ItemType &gadget)
    {
        return readProperty(property, &gadget);
    }

    template<typename ItemType>
    static QVariant readProperty(const char* propertyName, const ItemType* gadget)
    {
        if constexpr (itemsAreQObjects){
            if (const QObject* obj = reinterpret_cast<const QObject*>(gadget->pointer()))
                return obj->property(propertyName);
        }else if(gadget->metaType.flags() & QMetaType::PointerToQObject){
            if (const QObject* obj = reinterpret_cast<const QObject*>(gadget->pointer()))
                return obj->property(propertyName);
        } else if(const QMetaObject* mo = gadget->metaType.metaObject()){
            const int pi = mo->indexOfProperty(propertyName);
            if (pi >= 0) {
                return readProperty(pi, gadget);
            }
        }
        return {};
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    static QVariant readProperty(const char* propertyName, const ItemType &gadget)
    {
        return readProperty(propertyName, &gadget);
    }

    template<typename ItemType>
    static QVariant readProperty(int property, const char* propertyName, const ItemType* gadget)
    {
        if constexpr (itemsAreQObjects){
            if (const QObject* obj = reinterpret_cast<const QObject*>(gadget->pointer()))
                return obj->property(propertyName);
        }else if (gadget->metaType.flags() & QMetaType::PointerToQObject) {
            if (const QObject* obj = reinterpret_cast<const QObject*>(gadget->pointer()))
                return obj->property(propertyName);
        } else {
            return readProperty(property, gadget);
        }
        return {};
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    static QVariant readProperty(int property, const char* propertyName, const ItemType &gadget)
    {
        return readProperty(property, propertyName, &gadget);
    }

    template<typename Value>
    static QVariant read(const Value* value)
    {
        if(value->metaType.id()==QMetaType::QVariant){
            return *reinterpret_cast<const QVariant*>(value->data);
        }
        return QVariant{value->metaType, value->data};
    }

    template<typename Value, std::enable_if_t<!std::is_pointer_v<Value>,bool> = true>
    static QVariant read(const Value& value)
    {
        return read(&value);
    }

    template <typename ItemType>
    static bool writeProperty(const QMetaProperty &prop, ItemType* object, const QVariant &v){
        if constexpr (ItemType::mutableRow){
            if constexpr (itemsAreQObjects){
                return writeProperty(prop, reinterpret_cast<QObject*>(object->pointer()), v);
            }else if(object->metaType.flags() & QMetaType::PointerToQObject){
                return writeProperty(prop, reinterpret_cast<QObject*>(object->pointer()), v);
            }else if(object->metaType.flags() & QMetaType::PointerToGadget){
                return writeProperty(prop, object->pointer(), v);
            }else{
                void* ptr = object->pointer();
                if(CoreAPI::isJObjectWrappedMetaType(object->metaType)){
                    if(reinterpret_cast<JObjectWrapper*>(ptr)->isNull()){
                        return false;
                    }
                }
                return writeProperty(prop, ptr, v);
            }
        }else{
            Q_UNUSED(prop)
            Q_UNUSED(object)
            Q_UNUSED(v)
            return false;
        }
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    static bool writeProperty(const QMetaProperty &prop, ItemType &&gadget, const QVariant &data)
    {
        return writeProperty(prop, &gadget, data);
    }

    template <typename ItemType>
    static bool writeProperty(int property, ItemType* object, const QVariant &v)
    {
        const QMetaObject* mo = object->metaType.metaObject();
        return writeProperty(mo->property(property + mo->propertyOffset()), object, v);
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    static bool writeProperty(int property, ItemType &&gadget, const QVariant &data)
    {
        return writeProperty(property, &gadget, data);
    }

    template <typename ItemType>
    static bool writeProperty(const char* propertyName, ItemType* object, const QVariant &v)
    {
        if constexpr (ItemType::mutableRow){
            if constexpr (itemsAreQObjects){
                if (QObject* obj = reinterpret_cast<QObject*>(object->pointer()))
                    return obj->setProperty(propertyName, v);
            } else if (object->metaType.flags() & QMetaType::PointerToQObject) {
                if (QObject* obj = reinterpret_cast<QObject*>(object->pointer()))
                    return obj->setProperty(propertyName, v);
            } else if(const QMetaObject* mo = object->metaType.metaObject()){
                const int pi = mo->indexOfProperty(propertyName);
                if (pi >= 0) {
                    return writeProperty(pi, object, v);
                }
            }
        }else{
            Q_UNUSED(propertyName)
            Q_UNUSED(object)
            Q_UNUSED(v)
        }
        return false;
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    static bool writeProperty(const char* propertyName, ItemType &&gadget, const QVariant &data)
    {
        return writeProperty(propertyName, &gadget, data);
    }

    template<typename Target, std::enable_if_t<!std::is_pointer_v<Target>,bool> = true>
    static bool write(Target& target, const void* value)
    {
        if constexpr (Target::mutableRow){
            if(target.metaType.flags() & QMetaType::IsPointer){
                *reinterpret_cast<void**>(target.data) = *reinterpret_cast<void*const*>(value);
            }else{
                target.metaType.destruct(target.data);
                target.metaType.construct(target.data, value);
            }
            return true;
        }else{
            Q_UNUSED(target)
            Q_UNUSED(value)
            return false;
        }
    }

    template <typename Target>
    static bool write(Target *target, const void* value)
    {
        if (target)
            return write(*target, value);
        return false;
    }

    template<typename Target>
    static bool write(Target& target, const QVariant &value)
    {
        if constexpr (Target::mutableRow){
            if(target.metaType.id()==QMetaType::QVariant){
                *reinterpret_cast<QVariant*>(target.data) = value;
            }else if(value.metaType()!=target.metaType){
                QVariant _value;
                if(!_value.convert(target.metaType))
                    return false;
                if(target.metaType.flags() & QMetaType::IsPointer){
                    *reinterpret_cast<void**>(target.data) = *reinterpret_cast<void*const*>(value.constData());
                }else{
                    target.metaType.destruct(target.data);
                    target.metaType.construct(target.data, _value.constData());
                }
            }else{
                if(target.metaType.flags() & QMetaType::IsPointer){
                    *reinterpret_cast<void**>(target.data) = *reinterpret_cast<void*const*>(value.constData());
                }else{
                    target.metaType.destruct(target.data);
                    target.metaType.construct(target.data, value.constData());
                }
            }
            return true;
        }else{
            Q_UNUSED(target)
            Q_UNUSED(value)
            return false;
        }
    }

    template <typename Target>
    static bool write(Target *target, const QVariant &value)
    {
        if (target)
            return write(*target, value);
        return false;
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    static bool resetProperty(int property, ItemType &&object)
    {
        return resetProperty(property, &object);
    }

    template<typename ItemType>
    static bool resetProperty(int property, ItemType* object)
    {
        if constexpr (ItemType::mutableRow){
            const QMetaObject *mo = object->metaType.metaObject();
            bool success = true;
            if (property == -1) {
                // reset all properties
                if(object->metaType.flags() & QMetaType::PointerToQObject){
                    for (int p = mo->propertyOffset(); p < mo->propertyCount(); ++p)
                        success = writeProperty(mo->property(p), object, {}) && success;
                } else { // reset a gadget by assigning a default-constructed
                    object->metaType.destruct(object->data);
                    object->metaType.construct(object->data);
                }
            } else {
                success = writeProperty(mo->property(property + mo->propertyOffset()), object, {});
            }
            return success;
        }else{
            Q_UNUSED(property)
            Q_UNUSED(object)
            return false;
        }
    }
protected:
    const protocol_type& protocol() const { return QRangeModelDetails::refTo(m_protocol); }
    protocol_type& protocol() { return QRangeModelDetails::refTo(m_protocol); }
    const range_type *childRange(const QModelIndex &index) const
    {
        if (!index.isValid())
            return m_data.model();
        if (index.column()) // only items at column 0 can have children
            return nullptr;
        return that().childRangeImpl(index);
    }

    range_type *childRange(const QModelIndex &index)
    {
        if (!index.isValid())
            return m_data.model();
        if (index.column()) // only items at column 0 can have children
            return nullptr;
        return that().childRangeImpl(index);
    }
public:
    QModelIndex parent(const QModelIndex &child) const { return that().parent(child); }

    int rowCount(const QModelIndex &parent) const { return that().rowCount(parent); }

    int columnCount(const QModelIndex &parent) const { return that().columnCount(parent); }

    void invalidateCaches() { m_data.invalidateCaches(); }

    bool setHeaderData(int , Qt::Orientation , const QVariant &, int ) { return false; }

    void destroy() { delete std::addressof(that()); }

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    template <typename Fn, std::size_t ...Is>
    static bool forEachTupleElement(const row_type &row, Fn &&fn, std::index_sequence<Is...>)
    {
        using std::get;
        return (std::forward<Fn>(fn)(QRangeModelDetails::pointerTo(get<Is>(row))) && ...);
    }

    template <typename Fn>
    bool forEachColumn(const const_row_type &row, int rowIndex, const QModelIndex &parent, Fn &&fn) const
    {
        const auto &model = this->itemModel();
        if constexpr (one_dimensional_range) {
            return fn(model.index(rowIndex, 0, parent), QRangeModelDetails::pointerTo(row));
        } else if constexpr (dynamicColumns() || QRangeModelDetails::array_like_v<row_type>) {
            int columnIndex = -1;
            return std::all_of(QRangeModelDetails::adl_begin(row),
                               QRangeModelDetails::adl_end(row), [&](const auto &item) {
                                   return fn(model.index(rowIndex, ++columnIndex, parent),
                                             QRangeModelDetails::pointerTo(item));
                               });
        } else { // tuple-like (but not necessarily std::tuple, so can't use std::apply)
            int column = -1;
            return forEachTupleElement(row, [&column, &fn, &model, &rowIndex, &parent](QObject *item){
                return std::forward<Fn>(fn)(model.index(rowIndex, ++column, parent), item);
            }, std::make_index_sequence<static_column_count>());
        }
    }

    template <typename RowType>
    bool autoConnectPropertiesInRow(const RowType &row, int rowIndex, const QModelIndex &parent) const
    {
        if (!QRangeModelDetails::isValid(row))
            return true; // nothing to do
        if constexpr(std::is_base_of_v<QObject,std::remove_pointer_t<decltype(QRangeModelDetails::pointerTo(row))>>){
            return forEachColumn(row, rowIndex, parent, [this](const QModelIndex &index, QObject *item) {
                if constexpr (isMutable())
                    return Self::connectProperties(index, item, m_data.context, m_data.properties);
                else
                    return Self::connectPropertiesConst(index, item, m_data.context, m_data.properties);
            });
        }
        return false;
    }

    void clearConnectionInRow(const row_type &row, int rowIndex, const QModelIndex &parent) const
    {
        if (!QRangeModelDetails::isValid(row))
            return;
        if constexpr(std::is_base_of_v<QObject,std::remove_pointer_t<decltype(QRangeModelDetails::pointerTo(row))>>){
            forEachColumn(row, rowIndex, parent, [this](const QModelIndex &, QObject *item) {
                m_data.connections.removeIf([item](const auto &connection) {
                    return connection.sender == item;
                });
                return true;
            });
        }
    }
#endif //QT_VERSION >= QT_VERSION_CHECK(6,11,0)

#if QT_VERSION >= QT_VERSION_CHECK(6,12,0)
    void interfaceVersion(int &versionNumber) const {
        versionNumber = QT_VERSION;
    }

    void sort(int column, Qt::SortOrder order)    {
        Q_UNUSED(column)
        Q_UNUSED(order)
        /*if constexpr (isMutable() && std::is_swappable_v<row_type>) {
            if (rowCount({}) < 2 || column >= columnCount({}))
                return;
            Compare compare(this, column, order);
            if (!compare.checkComparable())
                return;

            this->beginLayoutChange();
            QScopeGuard endLayoutChange([this]{ this->endLayoutChange(); });
            that().sortImpl([&compare](const auto &leftRow, const auto &rightRow) {
                if (auto anyInvalid = Compare::compareInvalid(leftRow, rightRow))
                    return *anyInvalid;
                return row_traits::for_element_at(leftRow, compare.m_index.column(),
                                                  [&rightRow, &compare](const auto &leftItem){
                                                      return row_traits::for_element_at(rightRow, compare.m_index.column(),
                                                                                        [&leftItem, &compare](const auto &rightItem){
                                                                                            // Called by std::stable_sort. Since "column" is a runtime value, we
                                                                                            // can't statically assert that lhs and rhs are of the same type.
                                                                                            if constexpr (std::is_same_v<decltype(leftItem), decltype(rightItem)>) {
                                                                                                if (auto anyInvalid = Compare::compareInvalid(leftItem, rightItem))
                                                                                                    return *anyInvalid;
                                                                                                return compare(QRangeModelDetails::refTo(leftItem),
                                                                                                               QRangeModelDetails::refTo(rightItem));
                                                                                            } else {
                                                                                                Q_UNREACHABLE();
                                                                                            }
                                                                                            return false;
                                                                                        });
                                                  });
            });
        }*/
    }

    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value,
                          int hits, Qt::MatchFlags flags) const    {
        Q_UNUSED(start)
        Q_UNUSED(role)
        Q_UNUSED(value)
        Q_UNUSED(hits)
        Q_UNUSED(flags)
        // return that().matchImpl(start, role,
        //                         QRangeModelImplBase::convertMatchValue(value, flags), hits, flags);
        return {};
    }

    Qt::DropActions adjustSupportedDragActions(Qt::DropActions dragActions){
        if constexpr (!isMutable())
            dragActions &= ~Qt::MoveAction;
        return dragActions;
    }

    Qt::DropActions adjustSupportedDropActions(Qt::DropActions dropActions)    {
        if constexpr (!isMutable())
            dropActions = Qt::IgnoreAction;

        return dropActions;
    }

    QStringList mimeTypes() const    {
        // using ItemType = QRangeModelDetails::wrapped_t<typename row_traits::item_type>;
        // if constexpr (QRangeModelDetails::item_access<ItemType>::hasMimeTypes)
        //     return QRangeModelDetails::QRangeModelItemAccess<ItemType>::mimeTypes();
        // else if constexpr (QRangeModelDetails::hasMimeTypes<wrapped_row_type>)
        //     return QRangeModelDetails::QRangeModelRowOptions<wrapped_row_type>::mimeTypes();
        // else
            return this->itemModel().QAbstractItemModel::mimeTypes();
    }
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                         const QModelIndex &parent) const{
        Q_UNUSED(data)
        Q_UNUSED(action)
        Q_UNUSED(row)
        Q_UNUSED(column)
        Q_UNUSED(parent)
        return false;
    }
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                      const QModelIndex &parent){
        Q_UNUSED(data)
        Q_UNUSED(action)
        Q_UNUSED(row)
        Q_UNUSED(column)
        Q_UNUSED(parent)
        return false;
    }
    QMimeData *mimeData(const QModelIndexList &indexes) const{
        Q_UNUSED(indexes)
        return nullptr;
    }

#endif //QT_VERSION >= QT_VERSION_CHECK(6,12,0)

    ModelData m_data;
    Protocol m_protocol;
    std::unique_ptr<QGenericTableItemModelImpl<GenericTable>> m_owner;

    template <typename BaseMethod, typename BaseMethod::template Overridden<Self> overridden>
    using Override = typename Ancestor::template Override<BaseMethod, overridden>;

    using Destroy = Override<QRangeModelImplBase::Destroy, &Self::destroy>;
    using Index = Override<QRangeModelImplBase::Index, &Self::index>;
    using Parent = Override<QRangeModelImplBase::Parent, &Self::parent>;
    using Sibling = Override<QRangeModelImplBase::Sibling, &Self::sibling>;
    using RowCount = Override<QRangeModelImplBase::RowCount, &Self::rowCount>;
    using ColumnCount = Override<QRangeModelImplBase::ColumnCount, &Self::columnCount>;
    using Flags = Override<QRangeModelImplBase::Flags, &Self::flags>;
    using HeaderData = Override<QRangeModelImplBase::HeaderData, &Self::headerData>;

    using Data = Override<QRangeModelImplBase::Data, &Self::data>;
    using ItemData = Override<QRangeModelImplBase::ItemData, &Self::itemData>;
    using RoleNames = Override<QRangeModelImplBase::RoleNames, &Self::roleNames>;
    using InvalidateCaches = Override<QRangeModelImplBase::InvalidateCaches, &Self::invalidateCaches>;
    using SetHeaderData = Override<QRangeModelImplBase::SetHeaderData, &Self::setHeaderData>;
    using SetData = Override<QRangeModelImplBase::SetData, &Self::setData>;
    using SetItemData = Override<QRangeModelImplBase::SetItemData, &Self::setItemData>;
    using ClearItemData = Override<QRangeModelImplBase::ClearItemData, &Self::clearItemData>;
    using InsertColumns = Override<QRangeModelImplBase::InsertColumns, &Self::insertColumns>;
    using RemoveColumns = Override<QRangeModelImplBase::RemoveColumns, &Self::removeColumns>;
    using MoveColumns = Override<QRangeModelImplBase::MoveColumns, &Self::moveColumns>;
    using InsertRows = Override<QRangeModelImplBase::InsertRows, &Self::insertRows>;
    using RemoveRows = Override<QRangeModelImplBase::RemoveRows, &Self::removeRows>;
    using MoveRows = Override<QRangeModelImplBase::MoveRows, &Self::moveRows>;
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    using MultiData = Override<QRangeModelImplBase::MultiData, &Self::multiData>;
    using SetAutoConnectPolicy = Override<QRangeModelImplBase::SetAutoConnectPolicy, &Self::setAutoConnectPolicy>;
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,12,0)
    using InterfaceVersion = Override<QRangeModelImplBase::InterfaceVersion, &Self::interfaceVersion>;
    using Sort = Override<QRangeModelImplBase::Sort, &Self::sort>;
    using Match = Override<QRangeModelImplBase::Match, &Self::match>;
    using AdjustSupportedDragActions = Override<QRangeModelImplBase::AdjustSupportedDragActions, &Self::adjustSupportedDragActions>;
    using AdjustSupportedDropActions = Override<QRangeModelImplBase::AdjustSupportedDropActions, &Self::adjustSupportedDropActions>;

    using MimeTypes = Override<QRangeModelImplBase::MimeTypes, &Self::mimeTypes>;
    using CanDropMimeData = Override<QRangeModelImplBase::CanDropMimeData, &Self::canDropMimeData>;
    using DropMimeData = Override<QRangeModelImplBase::DropMimeData, &Self::dropMimeData>;
    using MimeData = Override<QRangeModelImplBase::MimeData, &Self::mimeData>;
#endif
private:
};

#endif //QT_VERSION >= QT_VERSION_CHECK(6,10,0)

#endif // RANGE_P_P_H
