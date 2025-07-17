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

template<RowType _rowType = RowType::Data, bool is_extensible_row = false, bool _is_mutable_row = false>
class ConstIterator;
template<RowType _rowType = RowType::Data, bool is_extensible_row = false, bool _is_mutable_row = false>
class Iterator;
template<typename Super, bool is_mutable>
struct MetaObjectRow;
template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType, typename... Args>
struct TreeRangeWrapper;

template<typename RowType>
struct ConstRangeRow;

template<typename RowType>
struct MutableRangeRow;

template<typename RowType>
struct ExtensibleRangeRow;

template<typename RowType, bool is_extensible, bool is_mutable>
using RangeRow = std::conditional_t<is_mutable, std::conditional_t<is_extensible, ExtensibleRangeRow<RowType>, MutableRangeRow<RowType>>, ConstRangeRow<RowType>>;

template<bool is_mutable_row,
         bool is_list_row,
         RowType rowType>
struct MutableRangeWrapper;

template<bool is_mutable_row,
         bool is_list_row,
         RowType rowType>
struct ConstRangeWrapper;

template<typename Super>
struct ListRangeWrapper;

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_row,
         RowType rowType>
using RangeWrapper = std::conditional_t<is_mutable_range, MutableRangeWrapper<is_mutable_row,is_list_row,rowType>, ConstRangeWrapper<is_mutable_row,is_list_row,rowType>>;

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType>
using MutableTreeRangeWrapper = TreeRangeWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType,ClassInfo>;

template <TreeType treeType,
         bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType>
using RangeWrapperType = std::conditional_t<treeType==TreeType::MutableTree, MutableTreeRangeWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>,
                                            std::conditional_t<treeType==TreeType::ConstTree, TreeRangeWrapper<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>,
                                                               std::conditional_t<is_list_range, ListRangeWrapper<RangeWrapper<is_mutable_range,is_mutable_row,is_list_row,rowType>>, RangeWrapper<is_mutable_range,is_mutable_row,is_list_row,rowType>>>>;

struct MutableRow{
    static constexpr bool mutableRow = true;
    void* data;
    QMetaType metaType;
    MetaTypeUtils::DataType dataType;
    QSharedPointer<AbstractContainerAccess> containerAccess;
protected:
    bool needsDeletion;
    template<typename>
    friend struct ListRangeWrapper;
    friend struct ConstRow;

    bool initialize(void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess);
public:
    template<typename Value>
    auto initialize(const Value& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(metaType)>,bool> {
        return initialize(value.data, value.metaType, value.dataType, value.containerAccess);
    }
    MutableRow(void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess);
    MutableRow(const MutableRow& other);
    MutableRow(MutableRow&& other);
    MutableRow();
    ~MutableRow();
    MutableRow& operator=(const MutableRow& other);
    MutableRow& operator=(MutableRow&& other);
    void swap(MutableRow& other) noexcept;
    friend void swap(MutableRow&& _this, MutableRow&& other) noexcept;
    friend void swap(MutableRow& _this, MutableRow& other) noexcept;
    friend void qSwap(MutableRow& _this, MutableRow& other) noexcept;
    void* pointer()const;
};

struct ConstRow{
    static constexpr bool mutableRow = false;
    const void* data;
    QMetaType metaType;
    MetaTypeUtils::DataType dataType;
    QSharedPointer<AbstractContainerAccess> containerAccess;
protected:
    bool needsDeletion;
    template<typename>
    friend struct ListRangeWrapper;

    bool initialize(const void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess);
public:
    template<typename Value>
    auto initialize(const Value& value) -> std::enable_if_t<std::is_same_v<decltype(value.metaType),decltype(metaType)>,bool> {
        return ConstRow::initialize(value.data, value.metaType, value.dataType, value.containerAccess);
    }
    ConstRow(const void* _data, const QMetaType& _metaType, MetaTypeUtils::DataType _dataType, const QSharedPointer<AbstractContainerAccess>& _containerAccess);
    ConstRow(const ConstRow& other);
    ConstRow(const MutableRow& other);
    ConstRow(ConstRow&& other);
    ConstRow();
    ~ConstRow();
    ConstRow& operator=(const ConstRow& other);
    ConstRow& operator=(ConstRow&& other);
    const void* pointer()const;
};

class AbstractConstIterator;

class AbstractIterator{
public:
    typedef std::forward_iterator_tag iterator_category;
    typedef qptrdiff difference_type;
protected:
    void* current_data;
    QMetaType current_metaType;
    MetaTypeUtils::DataType current_dataType;
    QSharedPointer<AbstractContainerAccess> current_containerAccess;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> iter;
    friend AbstractConstIterator;
public:
    AbstractIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter);
    AbstractIterator(const AbstractIterator& other);
    AbstractIterator(AbstractIterator&& other);
    AbstractIterator& operator=(const AbstractIterator& other);
    void swap(AbstractIterator& other) noexcept;
    friend void swap(AbstractIterator& _this, AbstractIterator& other) noexcept;
    friend void qSwap(AbstractIterator& _this, AbstractIterator& other) noexcept;
    AbstractIterator& operator=(AbstractIterator&& other);
    bool operator==(const AbstractIterator& other) const;
    bool operator!=(const AbstractIterator& other) const;
    AbstractIterator& operator++();
    AbstractIterator operator++(int);
    bool isConst() const;
    bool operator==(const AbstractConstIterator& other) const;
    bool operator!=(const AbstractConstIterator& other) const;
};

class AbstractConstIterator{
public:
    typedef std::forward_iterator_tag iterator_category;
    typedef qptrdiff difference_type;
protected:
    const void* current_data;
    QMetaType current_metaType;
    MetaTypeUtils::DataType current_dataType;
    QSharedPointer<AbstractContainerAccess> current_containerAccess;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> iter;
    friend class AbstractIterator;
public:
    AbstractConstIterator(std::unique_ptr<AbstractSequentialAccess::ElementIterator>&& _iter);
    AbstractConstIterator(const AbstractConstIterator& other);
    AbstractConstIterator(const AbstractIterator& other);
    AbstractConstIterator(AbstractConstIterator&& other);
    AbstractConstIterator(AbstractIterator&& other);
    AbstractConstIterator& operator=(const AbstractConstIterator& other);
    AbstractConstIterator& operator=(const AbstractIterator& other);
    void swap(AbstractConstIterator& other) noexcept;
    friend void swap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept;
    friend void qSwap(AbstractConstIterator& _this, AbstractConstIterator& other) noexcept;
    AbstractConstIterator& operator=(AbstractConstIterator&& other);
    AbstractConstIterator& operator=(AbstractIterator&& other);
    bool operator==(const AbstractConstIterator& other) const;
    bool operator==(const AbstractIterator& other) const;
    bool operator!=(const AbstractConstIterator& other) const;
    bool isConst() const;
    AbstractConstIterator& operator++();
    AbstractConstIterator operator++(int);
};

template<RowType _rowType, bool is_extensible_row, bool _is_mutable_row>
class Iterator : public AbstractIterator{
public:
    using iterator_category = AbstractIterator::iterator_category;
    using difference_type = AbstractIterator::difference_type;
    typedef MutableRow RowBase;
    typedef std::conditional_t<_rowType==RowType::Range, RangeRow<RowBase, is_extensible_row, _is_mutable_row>,
                               std::conditional_t<_rowType==RowType::MetaObject,
                                                  MetaObjectRow<RowBase, _is_mutable_row>,
                                                  RowBase>> value_type;
    typedef value_type*pointer;
    typedef value_type reference;
    using element_type = const value_type;
    using AbstractIterator::AbstractIterator;
    value_type operator*()const{
        return {current_data, current_metaType, current_dataType, current_containerAccess};
    }
    Iterator& operator++(){
        AbstractIterator::operator++();
        return *this;
    }
    Iterator operator++(int){
        Iterator copy(*this);
        operator++();
        return copy;
    }
    friend class ConstIterator<_rowType,is_extensible_row,_is_mutable_row>;
};

template<RowType _rowType, bool is_extensible_row, bool _is_mutable_row>
class ConstIterator : public AbstractConstIterator{
public:
    using iterator_category = AbstractIterator::iterator_category;
    using difference_type = AbstractIterator::difference_type;
    typedef ConstRow RowBase;
    typedef std::conditional_t<_rowType==RowType::Range, RangeRow<RowBase, false, _is_mutable_row>,
                               std::conditional_t<_rowType==RowType::MetaObject,
                                                  MetaObjectRow<RowBase, _is_mutable_row>,RowBase>
                               > value_type;
    typedef value_type*pointer;
    typedef value_type reference;
    using element_type = const value_type;
    using AbstractConstIterator::AbstractConstIterator;
    value_type operator*() const{
        return {current_data, current_metaType, current_dataType, current_containerAccess};
    }
    ConstIterator& operator++(){
        AbstractConstIterator::operator++();
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
    using const_iterator = ConstIterator<>;
    using value_type = typename const_iterator::value_type;
    using size_type = qsizetype;
public:
    ConstRangeRow(const ConstRangeRow& other) = default;
    ConstRangeRow(ConstRangeRow&& other) = default;
    ConstRangeRow& operator=(const ConstRangeRow& other) = default;
    ConstRangeRow& operator=(ConstRangeRow&& other) = default;
    const_iterator begin() const;
    const_iterator end() const;
};

template<typename RowType>
struct MutableRangeRow : ConstRangeRow<RowType>{
    using Super = ConstRangeRow<RowType>;
    using Super::Super;
    using Super::data;
    using Super::pointer;
    using Super::metaType;
    using Super::containerAccess;
    using Super::mutableRow;
    using Super::begin;
    using Super::end;
    using const_iterator = ConstIterator<>;
    using iterator = Iterator<>;
    using value_type = typename iterator::value_type;
    using size_type = qsizetype;
    MutableRangeRow(const MutableRangeRow& other) = default;
    MutableRangeRow(MutableRangeRow&& other) = default;
    MutableRangeRow& operator=(const MutableRangeRow& other) = default;
    MutableRangeRow& operator=(MutableRangeRow&& other) = default;
    iterator begin();
    iterator end();
};

template<typename RowType>
struct ExtensibleRangeRow : MutableRangeRow<RowType>{
    using Super = MutableRangeRow<RowType>;
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
                if(AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(&*containerAccess)){
                    listAccess->resize(pointer(), pendingResize);
                }
                pendingResize = 0;
            }
            return true;
        }
        return false;
    }

    iterator insert(const const_iterator& iter, size_type n, const value_type& value){
        if(AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(&*containerAccess)){
            size_type i = std::distance(QRangeModelDetails::cbegin(*this), iter);
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
        if(AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(&*containerAccess)){
            const_iterator begin = QRangeModelDetails::cbegin(*this);
            qsizetype i = std::distance(begin, iter);
            listAccess->remove(pointer(), i, 1);
        }
    }

    void erase(const const_iterator& iter, const const_iterator& iter2){
        if(AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(&*containerAccess)){
            const_iterator begin = QRangeModelDetails::cbegin(*this);
            qsizetype i = std::distance(begin, iter);
            qsizetype i2 = std::distance(begin, iter2);
            listAccess->remove(pointer(), i, i2-i);
        }
    }

    void resize(qsizetype newSize){
        if(!containerAccess){
            pendingResize = newSize;
        }else if(AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(&*containerAccess)){
            listAccess->resize(pointer(), newSize);
        }
    }
    void resize(size_type newSize, const value_type& value){
        if(AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(&*containerAccess)){
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
    iterator begin();
    iterator end();
};

template<typename RowType, bool is_mutable>
struct MetaObjectRow : std::conditional_t<is_mutable && RowType::mutableRow, MutableMetaObjectRow<RowType>, RowType> {
    using Super = std::conditional_t<is_mutable && RowType::mutableRow, MutableMetaObjectRow<RowType>, RowType>;
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
    const_iterator begin() const;
    const_iterator end() const;
    int size() const;
};

template<bool, bool, bool, bool, RowType>
struct ConstTreeRow;

template<bool, bool, bool, bool, RowType>
struct MutableTreeRow;

template<typename... Args>
struct TreeRangeData;

template<>
struct TreeRangeData<>{
    void* m_container;
    std::shared_ptr<int> m_treeColumnCount;
    QSharedPointer<AbstractSequentialAccess> m_containerAccess;
    MetaTypeUtils::DataType m_elementDataType;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    TreeRangeData(void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& _treeColumnCount);
    TreeRangeData(JNIEnv*, jobject, const TreeRangeData<>& other);
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
    void initialize(JNIEnv*, const TreeRangeData<>& other);
    int treeColumnCount() const;
};

template<>
struct TreeRangeData<ClassInfo> : TreeRangeData<>{
    ClassInfo m_classInfo;
    JObjectWrapper m_rowObject;
    TreeRangeData(void* container, QSharedPointer<AbstractSequentialAccess>&& containerAccess, std::shared_ptr<int>&& _treeColumnCount, ClassInfo&& _classInfo);
    TreeRangeData(JNIEnv* env, jobject row, const TreeRangeData<ClassInfo>& other);
    TreeRangeData();
    TreeRangeData(TreeRangeData&& other);
    TreeRangeData& operator=(TreeRangeData&& other) = delete;
    TreeRangeData(const TreeRangeData& other) = delete;
    TreeRangeData& operator=(const TreeRangeData& other) = delete;
    void initialize(JNIEnv* env, const TreeRangeData<ClassInfo>& other);
    jobject rowObject(JNIEnv* env) const;
    jobject rowObject(JNIEnv* env);
};

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType,
         typename... Args>
struct TreeRangeWrapper : std::vector<std::conditional_t<sizeof...(Args)==0, ConstTreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>, MutableTreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>>*>{
    using TreeRow = std::conditional_t<sizeof...(Args)==0, ConstTreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>, MutableTreeRow<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>>;
    using Super = std::vector<TreeRow*>;
    using Super::begin;
    using Super::end;
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
    TreeRangeData<Args...> m_data;
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

    template<int S = sizeof...(Args)>
    std::enable_if_t<S!=0, jobject> rowObject(JNIEnv* env) const{
        return m_data.rowObject(env);
    }

    template<int S = sizeof...(Args)>
    std::enable_if_t<S!=0, jobject> rowObject(JNIEnv* env){
        return m_data.rowObject(env);
    }

    template<int S = sizeof...(Args)>
    std::enable_if_t<S!=0, void> erase(const const_iterator& iter){
        const_iterator begin = std::cbegin(*this);
        size_type i = std::distance(begin, iter);
        if(void* container = m_data.container()){
            if(auto sequentialAccess = dynamic_cast<AbstractListAccess*>(&*m_data.containerAccess())){
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
        const_iterator begin = std::cbegin(*this);
        size_type i = std::distance(begin, iter);
        size_type i2 = std::distance(begin, iter2);
        if(void* container = m_data.container()){
            if(auto sequentialAccess = dynamic_cast<AbstractListAccess*>(&*m_data.containerAccess())){
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
            if(auto sequentialAccess = dynamic_cast<AbstractListAccess*>(&*m_data.containerAccess())){
                size_type index = std::distance(std::cbegin(*this), iter);
                if(index>size_type(sequentialAccess->size(container)))
                    index = size_type(sequentialAccess->size(container));
                if(JniEnvironment env{100}){
                    if(!value->metaType.isValid()){
                        value->dataType = m_data.elementDataType();
                        value->metaType = m_data.elementMetaType();
                        value->containerAccess = m_data.elementNestedContainerAccess();
                        if(!value->m_children.m_data.isInitialized()){
                            value->m_children.m_data.initialize(env, m_data);
                        }
                        jobject inserted = value->rowObject(env);
                        if(value->metaType.flags() & QMetaType::IsPointer){
                            void* ptr = QtJambiAPI::convertJavaObjectToNative(env, inserted);
                            sequentialAccess->insert(container, index, 1, &ptr);
                        }else if(CoreAPI::isJObjectWrappedMetaType(value->metaType)){
                            JObjectWrapper wr(env, inserted);
                            sequentialAccess->insert(container, index, 1, &wr);
                        }else{
                            QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, inserted);
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
                        int index = std::distance(std::cbegin(*this), iter);
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
                            auto _iter = std::cbegin(*this);
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
                if(!value->m_children.m_data.isInitialized()){
                    value->m_children.m_data.initialize(env, m_data);
                }
                jobject inserted = value->rowObject(env);
                if(value->metaType.flags() & QMetaType::IsPointer){
                    void* ptr = QtJambiAPI::convertJavaObjectToNative(env, inserted);
                    value->data = new void*(ptr);
                }else if(CoreAPI::isJObjectWrappedMetaType(value->metaType)){
                    value->data = new JObjectWrapper(env, inserted);
                }else{
                    QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, inserted);
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
                auto _iter = std::cbegin(*this);
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
            if(AbstractSequentialAccess* sequentialElementAccess = dynamic_cast<AbstractSequentialAccess*>(&*m_data.elementNestedContainerAccess())){
                auto containerIter = m_data.containerAccess()->constElementIterator(m_data.container());
                while(containerIter->hasNext()){
                    const void* data = containerIter->next();
                    *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, sequentialElementAccess->size(data));
                }
            }else if(AbstractAssociativeAccess* associativeElementAccess = dynamic_cast<AbstractAssociativeAccess*>(&*m_data.elementNestedContainerAccess())){
                auto containerIter = m_data.containerAccess()->constElementIterator(m_data.container());
                while(containerIter->hasNext()){
                    const void* data = containerIter->next();
                    *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, associativeElementAccess->size(data));
                }
            }else if(dynamic_cast<AbstractPairAccess*>(&*m_data.elementNestedContainerAccess())){
                *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, 2);
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
                        QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, row);
                        variant.convert(m_data.elementMetaType());
                        newPtr = container = m_data.elementMetaType().create(variant.constData());
                    }
                    if constexpr(rowType==RowType::Range){
                        if(container){
                            if(AbstractSequentialAccess* sequentialElementAccess = dynamic_cast<AbstractSequentialAccess*>(&*m_data.elementNestedContainerAccess())){
                                *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, sequentialElementAccess->size(container));
                            }else if(AbstractAssociativeAccess* associativeElementAccess = dynamic_cast<AbstractAssociativeAccess*>(&*m_data.elementNestedContainerAccess())){
                                *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, associativeElementAccess->size(container));
                            }else if(dynamic_cast<AbstractPairAccess*>(&*m_data.elementNestedContainerAccess())){
                                *m_data.m_treeColumnCount = qMax(*m_data.m_treeColumnCount, 2);
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
         RowType rowType>
struct ConstTreeRow : std::conditional_t<rowType==RowType::Range, RangeRow<ConstRow, false, false>, ConstRow> {
    using Super = std::conditional_t<rowType==RowType::Range, RangeRow<ConstRow, false, false>, ConstRow>;
    using Super::data;
    using Super::metaType;
    using Super::containerAccess;
    using Super::pointer;
    using Super::needsDeletion;
    using Super::mutableRow;
    using Tree = RangeWrapperType<TreeType::ConstTree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
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
};

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType>
struct MutableTreeRow : std::conditional_t<rowType==RowType::Range, RangeRow<MutableRow, is_mutable_range, true>, MutableRow>{
    using Super = std::conditional_t<rowType==RowType::Range, RangeRow<MutableRow, is_mutable_range, true>, MutableRow>;
    using Super::data;
    using Super::metaType;
    using Super::dataType;
    using Super::containerAccess;
    using Super::pointer;
    using Super::needsDeletion;
    using Super::swap;
    using Super::mutableRow;
    using Tree = RangeWrapperType<TreeType::MutableTree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
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
                        QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, jthis);
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
                if(AbstractListAccess* access = dynamic_cast<AbstractListAccess*>(&*containerAccess)){
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
        size_type i = std::distance(QRangeModelDetails::cbegin(*this), iter);
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
        const_iterator begin = QRangeModelDetails::cbegin(*this);
        size_type i = std::distance(begin, iter);
        static_cast<AbstractListAccess*>(&*containerAccess())->remove(data, i, 1);
    }

    void erase(const const_iterator& iter, const const_iterator& iter2){
        const_iterator begin = QRangeModelDetails::cbegin(*this);
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

    using const_iterator = ConstIterator<rowType,is_list_row,is_mutable_row>;
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
         RowType rowType>
struct MutableRangeWrapper : ConstRangeWrapper<is_mutable_row,is_list_row,rowType>{
    using Super = ConstRangeWrapper<is_mutable_row,is_list_row,rowType>;
    using Super::Super;
    using Super::data;
    using Super::m_containerAccess;
    using Super::begin;
    using Super::end;
    using iterator = Iterator<rowType,is_list_row,is_mutable_row>;
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
          RowType rowType, typename Protocol = QRangeModelDetails::table_protocol_t<RangeWrapperType<treeType,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>>>
class QtJambiRangeModelImpl : public QRangeModelImplBase
{
public:
    using Range = RangeWrapperType<treeType,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    using range_type = QRangeModelDetails::wrapped_t<Range>;
    using row_reference = decltype(*QRangeModelDetails::begin(std::declval<range_type&>()));
    using const_row_reference = decltype(*QRangeModelDetails::cbegin(std::declval<range_type&>()));
    using row_type = std::remove_reference_t<row_reference>;
    using wrapped_row_type = QRangeModelDetails::wrapped_t<row_type>;
    using row_ptr = wrapped_row_type *;
    using const_row_ptr = const wrapped_row_type *;
    using protocol_type = QRangeModelDetails::wrapped_t<Protocol>;
protected:
    using Self = QtJambiRangeModelImpl<treeType, Structure, is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType, Protocol>;
    Structure& that() { return static_cast<Structure &>(*this); }
    const Structure& that() const { return static_cast<const Structure &>(*this); }

    using ModelData = QRangeModelDetails::ModelData<std::conditional_t<
                                                    std::is_pointer_v<Range>,
                                                    Range, std::remove_reference_t<Range>>>;

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
#if defined(__cpp_lib_ranges)
            return int(std::ranges::distance(QRangeModelDetails::begin(c),
                                             QRangeModelDetails::end(c)));
#else
            return int(std::distance(QRangeModelDetails::begin(c),
                                     QRangeModelDetails::end(c)));
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
            reader(*QRangeModelDetails::cpos(row, index.column()));
        }else if constexpr (one_dimensional_range) {
            return reader(row);
        } else if (QRangeModelDetails::isValid(row)) {
            if constexpr (dynamicColumns()){
                reader(*QRangeModelDetails::cpos(row, index.column()));
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
    explicit QtJambiRangeModelImpl(Range &&model, Protocol&& protocol, QRangeModel *itemModel)
        : QRangeModelImplBase(itemModel, static_cast<const Self*>(nullptr))
        , m_data{std::forward<Range>(model)}
        , m_protocol(std::forward<Protocol>(protocol))
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

    static void callConst(ConstOp op, const QRangeModelImplBase *that, void *r, const void *args)
    {
        switch (op) {
        case Index: makeCall(that, &Self::index, r, args);
            break;
        case Parent: makeCall(that, &Structure::parent, r, args);
            break;
        case Sibling: makeCall(that, &Self::sibling, r, args);
            break;
        case RowCount: makeCall(that, &Structure::rowCount, r, args);
            break;
        case ColumnCount: makeCall(that, &Structure::columnCount, r, args);
            break;
        case Flags: makeCall(that, &Self::flags, r, args);
            break;
        case HeaderData: makeCall(that, &Self::headerData, r, args);
            break;
        case Data: makeCall(that, &Self::data, r, args);
            break;
        case ItemData: makeCall(that, &Self::itemData, r, args);
            break;
        }
    }

    static void call(Op op, QRangeModelImplBase *that, void *r, const void *args)
    {
        switch (op) {
        case Destroy: delete static_cast<Structure *>(that);
            break;
        case SetData: makeCall(that, &Self::setData, r, args);
            break;
        case SetItemData: makeCall(that, &Self::setItemData, r, args);
            break;
        case ClearItemData: makeCall(that, &Self::clearItemData, r, args);
            break;
        case InsertColumns: makeCall(that, &Self::insertColumns, r, args);
            break;
        case RemoveColumns: makeCall(that, &Self::removeColumns, r, args);
            break;
        case MoveColumns: makeCall(that, &Self::moveColumns, r, args);
            break;
        case InsertRows: makeCall(that, &Self::insertRows, r, args);
            break;
        case RemoveRows: makeCall(that, &Self::removeRows, r, args);
            break;
        case MoveRows: makeCall(that, &Self::moveRows, r, args);
            break;
        }
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

        if (column < 0 || column >= itemModel().columnCount())
            return {};

        if (row == index.row()){
            return createIndex(row, column, index.constInternalPointer());
        }

        const_row_ptr parentRow = static_cast<const_row_ptr>(index.constInternalPointer());
        const auto siblingCount = size(that().childrenOf(parentRow));
        if (row < 0 || row >= int(siblingCount))
            return {};
        return createIndex(row, column, parentRow);
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
            return itemModel().QAbstractItemModel::headerData(section, orientation, role);
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
            result = itemModel().QAbstractItemModel::headerData(section, orientation, role);
        return result;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        QVariant result;
        const auto readData = [this, column = index.column(), &result, role](const auto &value) {
            Q_UNUSED(this)
            Q_UNUSED(column)
            auto valuePtr = QRangeModelDetails::pointerTo(value);
            if(!valuePtr->data)
                return;
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
                        result = readRole(role, valuePtr);
                        return;
                    }else{
                        int columnCount = mo->propertyCount() - mo->propertyOffset();
                        if (column <= columnCount
                                   && (role == Qt::DisplayRole || role == Qt::EditRole)) {
                            result = readProperty(column, valuePtr);
                            return;
                        }
                    }
                }
            }
            if (role == Qt::DisplayRole || role == Qt::EditRole) {
                result = read(value);
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
        const auto readItemData = [this, &result, &tried](const auto &value){
            Q_UNUSED(this);
            auto valuePtr = QRangeModelDetails::pointerTo(value);
            if(!valuePtr->data)
                return;
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
                        QVariant data = readProperty(role, roleName, valuePtr);
                        if (data.isValid())
                            result[role] = std::move(data);
                    }
                }else{
                    int columnCount = mo->propertyCount() - mo->propertyOffset();
                    if (columnCount <= 1) {
                        tried = true;
                        for (auto &&[role, roleName] : roleNames().asKeyValueRange()) {
                            QVariant data = readProperty(role, roleName, valuePtr);
                            if (data.isValid())
                                result[role] = std::move(data);
                        }
                    }
                }
            }
        };

        if (index.isValid()) {
            readAt(index, readItemData);

            if (!tried) // no multi-role item found
                result = itemModel().QAbstractItemModel::itemData(index);
        }
        return result;
    }

    bool setData(const QModelIndex &index, const QVariant &data, int role)
    {
        if (!index.isValid())
            return false;

        bool success = false;
        if constexpr (isMutable()/* || rowType==RowType::MetaObject*/) {
            auto emitDataChanged = qScopeGuard([&success, this, &index, &role]{
                if (success) {
                    Q_EMIT dataChanged(index, index, role == Qt::EditRole
                                                         ? QList<int>{} : QList{role});
                }
            });

            const auto writeData = [this, column = index.column(), &data, role](auto &&target) -> bool {
                auto targetPtr = QRangeModelDetails::pointerTo(target);
                if(!targetPtr->data)
                    return false;
                if constexpr (!std::is_const_v<std::remove_pointer_t<decltype(targetPtr->data)>>){
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
                                       && (role == Qt::DisplayRole || role == Qt::EditRole)) {
                                return writeProperty(column, QRangeModelDetails::pointerTo(target), data);
                            }
                        }
                    }
                    if (role == Qt::DisplayRole || role == Qt::EditRole) {
                        return write(target, data);
                    }
                }else{
                    Q_UNUSED(this)
                    Q_UNUSED(column)
                    Q_UNUSED(target)
                }
                return false;
            };

            success = writeAt(index, writeData);
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
                    Q_EMIT dataChanged(index, index, data.keys());
            });

            bool tried = false;
            auto writeItemData = [this, &tried, &data](auto &&target) -> bool {
                auto targetPtr = QRangeModelDetails::pointerTo(target);
                if(!targetPtr->data)
                    return false;
                if constexpr (!std::is_const_v<std::remove_pointer_t<decltype(targetPtr->data)>>){
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
                success = itemModel().QAbstractItemModel::setItemData(index, data);
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
                    Q_EMIT dataChanged(index, index, {});
            });

            auto clearData = [this, column = index.column()](auto &&target) {
                auto targetPtr = QRangeModelDetails::pointerTo(target);
                if(!targetPtr->data)
                    return false;
                if constexpr (!std::is_const_v<std::remove_pointer_t<decltype(targetPtr->data)>>){
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

            beginInsertColumns(parent, column, column + count - 1);
            for (auto child : *children) {
                auto it = QRangeModelDetails::pos(child, column);
                QRangeModelDetails::refTo(child).insert(it, count, {});
            }
            endInsertColumns();
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

            beginRemoveColumns(parent, column, column + count - 1);
            for (auto child : *children) {
                const auto start = QRangeModelDetails::pos(child, column);
                QRangeModelDetails::refTo(child).erase(start, std::next(start, count));
            }
            endRemoveColumns();
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
                if constexpr(!std::is_same_v<DataType, ConstRow>){
                    // we only support ranges as columns, as other types might
                    // not have the same data type across all columns
                    range_type * const children = childRange(sourceParent);
                    if (!children)
                        return false;

                    if (!beginMoveColumns(sourceParent, sourceColumn, sourceColumn + count - 1,
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

                    endMoveColumns();
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
                    if(dynamic_cast<AbstractSpanAccess*>(&*children->containerAccess())){
                        return false;
                    }
                }else{
                    if(AbstractSpanAccess* spanAccess = dynamic_cast<AbstractSpanAccess*>(&*children->containerAccess())){
                        if(spanAccess->isConst())
                            return false;
                    }
                }
            }
            EmptyRowGenerator generator{0, &that(), &parent};

            beginInsertRows(parent, row, row + count - 1);

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

            endInsertRows();
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
            if constexpr (treeType!=TreeType::None) {
                if(!parent.isValid()){
                    if(dynamic_cast<AbstractSpanAccess*>(&*children->containerAccess())){
                        return false;
                    }
                }else{
                    if(AbstractSpanAccess* spanAccess = dynamic_cast<AbstractSpanAccess*>(&*children->containerAccess())){
                        if(spanAccess->isConst())
                            return false;
                    }
                }
            }

            beginRemoveRows(parent, row, row + count - 1);
            [[maybe_unused]] bool callEndRemoveColumns = false;
            if constexpr (dynamicColumns()) {
                // if we remove the last row in a dynamic model, then we no longer
                // know how many columns we should have, so they will be reported as 0.
                if (prevRowCount == count) {
                    if (const int columns = that().columnCount(parent)) {
                        callEndRemoveColumns = true;
                        beginRemoveColumns(parent, 0, columns - 1);
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
                    endRemoveColumns();
                }
            }
            endRemoveRows();
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
                || sourceRow < 0 || sourceRow + count - 1 >= itemModel().rowCount(sourceParent)
                || destRow < 0 || destRow > itemModel().rowCount(destParent)) {
                return false;
            }

            range_type *source = childRange(sourceParent);
            // moving within the same range
            if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destParent, destRow))
                return false;

            const auto first = QRangeModelDetails::pos(source, sourceRow);
            const auto middle = std::next(first, count);
            const auto last = QRangeModelDetails::pos(source, destRow);

            if (sourceRow < destRow) // moving down
                rotate_by_swap(first, middle, last);
            else // moving up
                rotate_by_swap(last, first, middle);

            that().resetParentInChildren(source);

            endMoveRows();
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
        const QByteArray roleName = roleNames().value(role);
        if (const int index = mo->indexOfProperty(roleName.data()); index >= 0)
            return mo->property(index);
        return {};
    }

    template <typename ItemType>
    QVariant readRole(int role, const ItemType *gadget) const
    {
        const QMetaObject* mo = gadget->metaType.metaObject();
        QVariant result;
        QMetaProperty prop = roleProperty(role, mo);
        if (!prop.isValid() && role == Qt::EditRole)
            prop = roleProperty(Qt::DisplayRole, mo);

        if (prop.isValid()){
            result = readProperty(prop, gadget);
        }
        return result;
    }

    template <typename ItemType, std::enable_if_t<!std::is_pointer_v<ItemType>,bool> = true>
    QVariant readRole(int role, const ItemType &gadget) const
    {
        return readRole(role, &gadget);
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
        if(gadget->metaType.flags() & QMetaType::PointerToQObject){
            return readProperty(prop, reinterpret_cast<const QObject*>(gadget->pointer()));
        }else if(gadget->metaType.flags() & QMetaType::PointerToGadget){
            return readProperty(prop, gadget->pointer());
        }else{
            return readProperty(prop, gadget->pointer());
        }
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
        if (gadget->metaType.flags() & QMetaType::PointerToQObject) {
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
        if (gadget->metaType.flags() & QMetaType::PointerToQObject) {
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
            if(object->metaType.flags() & QMetaType::PointerToQObject){
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
            if (object->metaType.flags() & QMetaType::PointerToQObject) {
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
    ModelData m_data;
    Protocol m_protocol;
private:
};

#endif //QT_VERSION >= QT_VERSION_CHECK(6,10,0)

#endif // RANGE_P_P_H
