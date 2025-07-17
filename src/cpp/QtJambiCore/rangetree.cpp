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

#include "range_p_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,10,0)

template <bool is_mutable_tree,
          bool is_mutable_range,
          bool is_mutable_row,
          bool is_list_range,
          bool is_list_row,
          RowType rowType, typename Protocol>
class QtJambiGenericTreeItemModelImpl : public QtJambiRangeModelImpl<is_mutable_tree ? TreeType::MutableTree : TreeType::ConstTree, QtJambiGenericTreeItemModelImpl<is_mutable_tree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType, Protocol>, is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType, Protocol>
{
    static constexpr TreeType treeType = is_mutable_tree ? TreeType::MutableTree : TreeType::ConstTree;
    using Range = RangeWrapperType<is_mutable_tree ? TreeType::MutableTree : TreeType::ConstTree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    using Base = QtJambiRangeModelImpl<treeType, QtJambiGenericTreeItemModelImpl<is_mutable_tree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType, Protocol>, is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType, Protocol>;
    friend class QtJambiRangeModelImpl<treeType, QtJambiGenericTreeItemModelImpl<is_mutable_tree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType, Protocol>, is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType, Protocol>;

    using range_type = typename Base::range_type;
    using range_features = typename Base::range_features;
    using row_type = typename Base::row_type;
    using row_ptr = typename Base::row_ptr;
    using const_row_ptr = typename Base::const_row_ptr;

    using tree_traits = typename Base::protocol_traits;
    static constexpr bool is_mutable_impl = tree_traits::has_mutable_childRows;
    static_assert(!Base::dynamicColumns(), "A tree must have a static number of columns!");

public:
    QtJambiGenericTreeItemModelImpl(Range &&model, Protocol &&p, QRangeModel *itemModel)
        : Base(std::forward<Range>(model), std::forward<Protocol>(p), itemModel)
    {};

protected:
    QModelIndex indexImpl(int row, int column, const QModelIndex &parent) const
    {
        if (!parent.isValid())
            return this->createIndex(row, column);
        // only items at column 0 can have children
        if (parent.column())
            return QModelIndex();

        const_row_ptr grandParent = static_cast<const_row_ptr>(parent.constInternalPointer());
        const auto &parentSiblings = childrenOf(grandParent);
        const auto it = QRangeModelDetails::cpos(parentSiblings, parent.row());
        return this->createIndex(row, column, QRangeModelDetails::pointerTo(*it));
    }

    QModelIndex parent(const QModelIndex &child) const
    {
        if (!child.isValid())
            return child;

        // no pointer to parent row - no parent
        const_row_ptr parentRow = static_cast<const_row_ptr>(child.constInternalPointer());
        if (!parentRow)
            return {};

        // get the siblings of the parent via the grand parent
        decltype(auto) grandParent = this->protocol().parentRow(QRangeModelDetails::refTo(parentRow));
        const range_type &parentSiblings = childrenOf(QRangeModelDetails::pointerTo(grandParent));
        // find the index of parentRow
        const auto begin = QRangeModelDetails::cbegin(parentSiblings);
        const auto end = QRangeModelDetails::cend(parentSiblings);
        const auto it = std::find_if(begin, end, [parentRow](auto &&s){
            return QRangeModelDetails::pointerTo(s) == parentRow;
        });
        if (it != end)
            return this->createIndex(std::distance(begin, it), 0,
                                     QRangeModelDetails::pointerTo(grandParent));
        return {};
    }

    int rowCount(const QModelIndex &parent) const
    {
        return Base::size(this->childRange(parent));
    }

    int columnCount(const QModelIndex &parent) const
    {
        // // all levels of a tree have to have the same, static, column count
        // if constexpr (Base::one_dimensional_range)
        //     return 1;
        // else
        //     return Base::static_column_count; // if static_column_count is -1, static assert fires
        // Q_UNUSED(parent)
       const range_type *range = this->childRange(parent);
       if(Base::size(range)>0)
           return range->treeColumnCount();
       return 0;
    }

    static constexpr Qt::ItemFlags defaultFlags()
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    static constexpr bool canInsertRows()
    {
        // We must not insert rows if we cannot adjust the parents of the
        // children of the following rows. We don't have to do that if the
        // range operates on pointers.
        return tree_traits::has_setParentRow
             && Base::dynamicRows() && range_features::has_insert;
    }

    static constexpr bool canRemoveRows()
    {
        // We must not remove rows if we cannot adjust the parents of the
        // children of the following rows. We don't have to do that if the
        // range operates on pointers.
        return tree_traits::has_setParentRow
             && Base::dynamicRows() && range_features::has_erase;
    }

    static constexpr bool canMoveColumns(const QModelIndex &, const QModelIndex &)
    {
        return true;
    }

    static constexpr bool canMoveRows(const QModelIndex &, const QModelIndex &)
    {
        return true;
    }

    bool moveRowsAcross(const QModelIndex &sourceParent, int sourceRow, int count,
                        const QModelIndex &destParent, int destRow)
    {
        // If rows are pointers, then reference to the parent row don't
        // change, so we can move them around freely. Otherwise we need to
        // be able to explicitly update the parent pointer.
        if constexpr (!tree_traits::has_setParentRow) {
            return false;
        } else if constexpr (!(range_features::has_insert && range_features::has_erase)) {
            return false;
        } else if (!this->beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1,
                                        destParent, destRow)) {
            return false;
        }

        range_type *source = this->childRange(sourceParent);
        range_type *destination = this->childRange(destParent);

        // If we can insert data from another range into, then
        // use that to move the old data over.
        const auto destStart = QRangeModelDetails::pos(destination, destRow);
        if constexpr (range_features::has_insert_range) {
            const auto sourceStart = QRangeModelDetails::pos(*source, sourceRow);
            const auto sourceEnd = std::next(sourceStart, count);

            destination->insert(destStart, std::move_iterator(sourceStart),
                                           std::move_iterator(sourceEnd));
        } else if constexpr (std::is_copy_constructible_v<row_type>) {
            // otherwise we have to make space first, and copy later.
            destination->insert(destStart, count, row_type{});
        }

        row_ptr parentRow = destParent.isValid()
                          ? QRangeModelDetails::pointerTo(this->rowData(destParent))
                          : nullptr;

        // if the source's parent was already inside the new parent row,
        // then the source row might have become invalid, so reset it.
        if (parentRow == static_cast<row_ptr>(sourceParent.internalPointer())) {
            if (sourceParent.row() < destRow) {
                source = this->childRange(sourceParent);
            } else {
                // the source parent moved down within destination
                source = this->childRange(this->createIndex(sourceParent.row() + count, 0,
                                                            sourceParent.internalPointer()));
            }
        }

        // move the data over and update the parent pointer
        {
            const auto writeStart = QRangeModelDetails::pos(destination, destRow);
            const auto writeEnd = std::next(writeStart, count);
            const auto sourceStart = QRangeModelDetails::pos(source, sourceRow);
            const auto sourceEnd = std::next(sourceStart, count);

            for (auto write = writeStart, read = sourceStart; write != writeEnd; ++write, ++read) {
                // move data over if not already done, otherwise
                // only fix the parent pointer
                if constexpr (!range_features::has_insert_range)
                    *write = std::move(*read);
                this->protocol().setParentRow(QRangeModelDetails::refTo(*write), parentRow);
            }
            // remove the old rows from the source parent
            source->erase(sourceStart, sourceEnd);
        }

        // Fix the parent pointers in children of both source and destination
        // ranges, as the references to the entries might have become invalid.
        // We don't have to do that if the rows are pointers, as in that case
        // the references to the entries are stable.
        resetParentInChildren(destination);
        resetParentInChildren(source);

        this->endMoveRows();
        return true;
    }

    auto makeEmptyRow(const QModelIndex &parent)
    {
        // tree traversal protocol: if we are here, then it must be possible
        // to change the parent of a row.
        static_assert(tree_traits::has_setParentRow);
        row_type empty_row = this->protocol().newRow();
        if (QRangeModelDetails::isValid(empty_row) && parent.isValid()) {
            this->protocol().setParentRow(QRangeModelDetails::refTo(empty_row),
                                        QRangeModelDetails::pointerTo(this->rowData(parent)));
        }
        return empty_row;
    }

    template <typename It, typename Sentinel>
    void deleteRemovedRows(It &&begin, Sentinel &&end, bool atEnd = false)
    {
        if constexpr (tree_traits::has_deleteRow) {
            for (auto it = begin; it != end; ++it){
                if constexpr(treeType==TreeType::MutableTree){
                    if(atEnd){
                        (*it)->prepareDeletion();
                    }
                }
                this->protocol().deleteRow(*it);
            }
        }
    }

    void resetParentInChildren(range_type *children)
    {
        if constexpr (tree_traits::has_setParentRow) {
            const auto begin = QRangeModelDetails::begin(*children);
            const auto end = QRangeModelDetails::end(*children);
            for (auto it = begin; it != end; ++it) {
                if (auto &maybeChildren = this->protocol().childRows(*(*it))) {
                    QModelIndexList fromIndexes;
                    QModelIndexList toIndexes;
                    fromIndexes.reserve(Base::size(*maybeChildren));
                    toIndexes.reserve(Base::size(*maybeChildren));
                    auto *parentRow = QRangeModelDetails::pointerTo(*it);

                    int row = 0;
                    for (auto &child : *maybeChildren) {
                        const_row_ptr oldParent = this->protocol().parentRow(*child);
                        if (oldParent != parentRow) {
                            fromIndexes.append(this->createIndex(row, 0, oldParent));
                            toIndexes.append(this->createIndex(row, 0, parentRow));
                            this->protocol().setParentRow(*child, parentRow);
                        }
                        ++row;
                    }
                    this->changePersistentIndexList(fromIndexes, toIndexes);
                    resetParentInChildren(QRangeModelDetails::pointerTo(*maybeChildren));
                }
            }
        }

        if constexpr (treeType==TreeType::MutableTree) {
            if(JniEnvironment env{100 + 16 * int(children->size())}){
                if(jobject parentObject = children->rowObject(env)){
                    for(auto* row : std::as_const(*children)){
                        if(jobject entry = row->rowObject(env)){
                            Java::QtCore::QRangeModel$TreeRowInterface::setParentRow(env, entry, nullptr);
                            Java::QtCore::QRangeModel$TreeRowInterface::setParentRow(env, entry, parentObject);
                        }
                    }
                }
            }
        }
    }

    decltype(auto) rowDataImpl(const QModelIndex &index) const
    {
        const_row_ptr parentRow = static_cast<const_row_ptr>(index.constInternalPointer());
        const range_type &siblings = childrenOf(parentRow);
        Q_ASSERT(index.row() < int(Base::size(siblings)));
        return *QRangeModelDetails::cpos(siblings, index.row());
    }

    decltype(auto) rowDataImpl(const QModelIndex &index)
    {
        row_ptr parentRow = static_cast<row_ptr>(index.internalPointer());
        range_type &siblings = childrenOf(parentRow);
        Q_ASSERT(index.row() < int(Base::size(siblings)));
        return *QRangeModelDetails::pos(siblings, index.row());
    }

    const range_type *childRangeImpl(const QModelIndex &index) const
    {
        const auto &row = this->rowData(index);
        if (!QRangeModelDetails::isValid(row))
            return static_cast<const range_type *>(nullptr);

        decltype(auto) children = this->protocol().childRows(QRangeModelDetails::refTo(row));
        return QRangeModelDetails::pointerTo(std::forward<decltype(children)>(children));
    }

    range_type *childRangeImpl(const QModelIndex &index)
    {
        auto &row = this->rowData(index);
        if (!QRangeModelDetails::isValid(row))
            return static_cast<range_type *>(nullptr);

        decltype(auto) children = this->protocol().childRows(QRangeModelDetails::refTo(row));
        using Children = std::remove_reference_t<decltype(children)>;

        if constexpr (QRangeModelDetails::is_any_of<Children, std::optional>()
                   && std::is_default_constructible<typename Children::value_type>()) {
            if (!children)
                children.emplace(range_type{});
        }

        return QRangeModelDetails::pointerTo(std::forward<decltype(children)>(children));
    }

    const range_type &childrenOf(const_row_ptr row) const
    {
        return row ? QRangeModelDetails::refTo(this->protocol().childRows(*row))
                   : *this->m_data.model();
    }

private:
    range_type &childrenOf(row_ptr row)
    {
        return row ? QRangeModelDetails::refTo(this->protocol().childRows(*row))
                   : *this->m_data.model();
    }
};

template<bool is_mutable_tree,
         bool is_mutable_range,
         bool is_mutable_row,
         bool is_list_range,
         bool is_list_row,
         RowType rowType,
         typename... Args>
void QGenericTableItemModelImpl<GenericTable>::initializeTree(QRangeModel *itemModel, Args&&... args){
    using Range = RangeWrapperType<is_mutable_tree ? TreeType::MutableTree : TreeType::ConstTree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType>;
    using TreeType = QtJambiGenericTreeItemModelImpl<is_mutable_tree,is_mutable_range,is_mutable_row,is_list_range,is_list_row,rowType,QRangeModelDetails::DefaultTreeProtocol<Range>>;
    impl = new TreeType(Range{std::move(args)...}, {}, itemModel);
    callConst_fn = &TreeType::callConst;
    call_fn = &TreeType::call;
}

void QGenericTableItemModelImpl<GenericTable>::initializeTree(QRangeModel *itemModel, GenericTable&& model){
    switch(model.treeType){
    case TreeType::MutableTree:
        if(model.is_mutable_range){
            if(model.is_list_range){
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<true,true,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::MetaObject:
                    initializeTree<true,true,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::Range:
                    initializeTree<true,true,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                default:
                    break;
                }
            }else{
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<true,true,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::MetaObject:
                    initializeTree<true,true,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::Range:
                    initializeTree<true,true,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                default:
                    break;
                }
            }
        }else{
            if(model.is_list_range){
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<true,false,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::MetaObject:
                    initializeTree<true,false,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::Range:
                    initializeTree<true,false,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                default:
                    break;
                }
            }else{
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<true,false,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::MetaObject:
                    initializeTree<true,false,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                case RowType::Range:
                    initializeTree<true,false,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                    break;
                default:
                    break;
                }
            }
        }
        break;
    case TreeType::ConstTree:
        if(model.is_mutable_range){
            if(model.is_list_range){
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<false,true,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::MetaObject:
                    initializeTree<false,true,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::Range:
                    initializeTree<false,true,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                default:
                    break;
                }
            }else{
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<false,true,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::MetaObject:
                    initializeTree<false,true,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::Range:
                    initializeTree<false,true,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                default:
                    break;
                }
            }
        }else{
            if(model.is_list_range){
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<false,false,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::MetaObject:
                    initializeTree<false,false,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::Range:
                    initializeTree<false,false,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                default:
                    break;
                }
            }else{
                switch(model.rowType){
                case RowType::Data:
                    initializeTree<false,false,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::MetaObject:
                    initializeTree<false,false,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                case RowType::Range:
                    initializeTree<false,false,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                    break;
                default:
                    break;
                }
            }
        }
        break;
    default: break;
    }
}

#endif
