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

#include "rangetree_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,10,0)

void QGenericTableItemModelImpl<GenericTable>::initializeTree(QRangeModel *itemModel, GenericTable&& model){
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    if(model.itemAccess.isNull()){
#endif
        if(model.itemsAreQObjects){
            switch(model.treeType){
            case TreeType::MutableTree:
                if(model.is_mutable_range){
                    if(model.is_list_range){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,true,false,true,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,true,false,true,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,true,false,true,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,true,false,false,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,true,false,false,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,true,false,false,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        default:
                            break;
                        }
                    }
                }else{
                    if(model.is_list_range){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,false,false,true,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,false,false,true,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,false,false,true,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,false,false,false,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,false,false,false,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,false,false,false,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
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
                            initializeTree<false,true,false,true,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,true,false,true,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,true,false,true,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<false,true,false,false,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,true,false,false,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,true,false,false,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        default:
                            break;
                        }
                    }
                }else{
                    if(model.is_list_range){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<false,false,false,true,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,false,false,true,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,false,false,true,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<false,false,false,false,false,true,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,false,false,false,false,true,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,false,false,false,false,true,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        default:
                            break;
                        }
                    }
                }
                break;
            default: break;
            }
        }else{// !model.itemsAreQObjects
            switch(model.treeType){
            case TreeType::MutableTree:
                if(model.is_mutable_range){
                    if(model.is_list_range){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,true,false,true,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,true,false,true,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,true,false,true,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,true,false,false,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,true,false,false,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,true,false,false,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        default:
                            break;
                        }
                    }
                }else{
                    if(model.is_list_range){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,false,false,true,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,false,false,true,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,false,false,true,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<true,false,false,false,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::MetaObject:
                            initializeTree<true,false,false,false,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
                            break;
                        case RowType::Range:
                            initializeTree<true,false,false,false,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount), std::move(model.classInfo));
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
                            initializeTree<false,true,false,true,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,true,false,true,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,true,false,true,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<false,true,false,false,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,true,false,false,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,true,false,false,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        default:
                            break;
                        }
                    }
                }else{
                    if(model.is_list_range){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<false,false,false,true,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,false,false,true,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,false,false,true,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        default:
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTree<false,false,false,false,false,false,false,RowType::Data>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::MetaObject:
                            initializeTree<false,false,false,false,false,false,false,RowType::MetaObject>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
                            break;
                        case RowType::Range:
                            initializeTree<false,false,false,false,false,false,false,RowType::Range>(itemModel, model.env, model.container, std::move(model.sequentialAccess), std::move(model.treeColumnCount));
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
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    }else{
        initializeTreeItemAccess(itemModel, std::move(model));
    }
#endif //QT_VERSION >= QT_VERSION_CHECK(6,11,0)
}

#endif
