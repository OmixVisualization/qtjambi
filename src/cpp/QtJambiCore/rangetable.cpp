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

#include "pch_p.h"
#include "range_p_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,10,0)

template <bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType>
class QtJambiGenericTableItemModelImpl
    : public QtJambiRangeModelImpl<TreeType::None, QtJambiGenericTableItemModelImpl<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>, is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>
{
    using Range = RangeWrapperType<TreeType::None,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    using Base = QtJambiRangeModelImpl<TreeType::None, QtJambiGenericTableItemModelImpl<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>, is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    friend class QtJambiRangeModelImpl<TreeType::None, QtJambiGenericTableItemModelImpl<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>, is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    using range_type = typename Base::range_type;
    using range_features = typename Base::range_features;
    using row_type = typename Base::row_type;
    using const_row_ptr = typename Base::const_row_ptr;
    using row_traits = typename Base::row_traits;
    using row_features = typename Base::row_features;

    static constexpr bool is_mutable_impl = true;

public:
    explicit QtJambiGenericTableItemModelImpl(Range &&model, QRangeModel *itemModel, QGenericTableItemModelImpl<GenericTable>* owner)
        : Base(std::forward<Range>(model), {}, itemModel, owner)
    {}

protected:
    QModelIndex indexImpl(int row, int column, const QModelIndex &) const
    {
        if constexpr (Base::dynamicColumns()) {
            if (column < int(Base::size(*QRangeModelDetails::pos(*this->m_data.model(), row))))
                return this->createIndex(row, column);
            // if we got here, then column < columnCount(), but this row is to short
            qCritical("QRangeModel: Column-range at row %d is not large enough!", row);
            return {};
        } else {
            return this->createIndex(row, column);
        }
    }

    QModelIndex parent(const QModelIndex &) const
    {
        return {};
    }

    int rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;
        return int(Base::size(*this->m_data.model()));
    }

    int columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;

        // in a table, all rows have the same number of columns (as the first row)
        if constexpr (Base::dynamicColumns()) {
            if(Base::size(*this->m_data.model()) == 0)
                return 0;
            auto iter = QRangeModelDetails::begin(*this->m_data.model());
            int sz = int(Base::size(*iter));
            return sz;
        } else if constexpr (Base::one_dimensional_range) {
            if constexpr (rowType==RowType::MetaObject) {
                const QMetaObject* mo = this->m_data.model()->elementMetaObject();
                return mo->propertyCount()-mo->propertyOffset();
            }else{
                return row_traits::fixed_size();
            }
        } else {
            return Base::static_column_count;
        }
    }

    static constexpr Qt::ItemFlags defaultFlags()
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
    }

    static constexpr bool canInsertRows()
    {
        return Base::dynamicRows() && range_features::has_insert;
    }

    static constexpr bool canRemoveRows()
    {
        return Base::dynamicRows() && range_features::has_erase;
    }

    static constexpr bool canMoveColumns(const QModelIndex &source, const QModelIndex &destination)
    {
        return !source.isValid() && !destination.isValid();
    }

    static constexpr bool canMoveRows(const QModelIndex &source, const QModelIndex &destination)
    {
        return !source.isValid() && !destination.isValid();
    }

    constexpr bool moveRowsAcross(const QModelIndex &, int , int,
                                  const QModelIndex &, int) noexcept
    {
        // table/flat model: can't move rows between different parents
        return false;
    }

    auto makeEmptyRow(const QModelIndex &)
    {
        row_type empty_row = this->protocol().newRow();

        // dynamically sized rows all have to have the same column count
        if constexpr (Base::dynamicColumns() && row_features::has_resize) {
            if (QRangeModelDetails::isValid(empty_row))
                QRangeModelDetails::refTo(empty_row).resize(this->itemModel().columnCount());
        }

        return empty_row;
    }

    template <typename It, typename Sentinel>
    void deleteRemovedRows(It &&begin, Sentinel &&end, bool = false)
    {
        if constexpr (Base::protocol_traits::has_deleteRow) {
            for (auto it = begin; it != end; ++it)
                this->protocol().deleteRow(*it);
        }
    }

    decltype(auto) rowDataImpl(const QModelIndex &index) const
    {
        Q_ASSERT(index.row() < int(Base::size(*this->m_data.model())));
        return *QRangeModelDetails::pos(*this->m_data.model(), index.row());
    }

    decltype(auto) rowDataImpl(const QModelIndex &index)
    {
        Q_ASSERT(index.row() < int(Base::size(*this->m_data.model())));
        return *QRangeModelDetails::pos(*this->m_data.model(), index.row());
    }

    auto childRangeImpl(const QModelIndex &) const
    {
        return nullptr;
    }

    auto childRangeImpl(const QModelIndex &)
    {
        return nullptr;
    }

    const range_type &childrenOf(const_row_ptr row) const
    {
        Q_ASSERT(!row);
        return *this->m_data.model();
    }

    void resetParentInChildren(range_type *)
    {
    }
};

template<bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType,
         typename... Args>
void QGenericTableItemModelImpl<GenericTable>::initializeTable(QRangeModel *itemModel, Args&&... args){
    using Range = RangeWrapperType<TreeType::None,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    using TableType = QtJambiGenericTableItemModelImpl<is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    impl = new TableType(Range{std::move(args)...}, itemModel, this);
}

void QGenericTableItemModelImpl<GenericTable>::initializeTable(QRangeModel *itemModel, GenericTable&& model){
    if(model.is_mutable_range){
        if(model.is_mutable_row){
            if(model.is_list_range){
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,true,true,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,true,true,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,true,true,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }else{
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,true,false,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,true,false,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,true,false,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }
        }else{ // !is_mutable_row
            if(model.is_list_range){
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,false,true,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,false,true,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,false,true,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }else{
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<true,false,false,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<true,false,false,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<true,false,false,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }
        }
    }else{// !is_mutable_range
        if(model.is_mutable_row){
            if(model.is_list_range){
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,true,true,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,true,true,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,true,true,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }else{
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,true,false,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,true,false,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,true,false,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }
        }else{ // !is_mutable_row
            if(model.is_list_range){
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,false,true,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,false,true,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,false,true,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }else{
                if(model.is_list_row){
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }else{
                    switch(model.rowType){
                    case RowType::Data:
                        initializeTable<false,false,false,false,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::MetaObject:
                        initializeTable<false,false,false,false,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    case RowType::Range:
                        initializeTable<false,false,false,false,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess));
                        break;
                    }
                }
            }
        }
    }
}

#endif
