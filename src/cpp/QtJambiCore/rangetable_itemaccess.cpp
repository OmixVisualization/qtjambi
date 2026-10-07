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

#include "rangetable_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
void QGenericTableItemModelImpl<GenericTable>::initializeTableItemAccess(QRangeModel *itemModel, GenericTable&& model){
    if(model.itemsAreQObjects){
        if(model.is_mutable_range){
            if(model.is_mutable_row){
                if(model.is_list_range){
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,true,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,true,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,true,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,true,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,true,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,true,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,false,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,false,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,false,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,false,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,false,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,false,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }
            }else{ // !is_mutable_row
                if(model.is_list_range){
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,true,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,true,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,true,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,true,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,true,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,true,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,false,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,false,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,false,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,false,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,false,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,false,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
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
                            initializeTable<false,true,true,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,true,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,true,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,true,true,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,true,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,true,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,true,false,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,false,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,false,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,true,false,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,false,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,false,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }
            }else{ // !is_mutable_row
                if(model.is_list_range){
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,true,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,true,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,true,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,true,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,true,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,true,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,false,true,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,false,true,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,false,true,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,false,false,true,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,false,false,true,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,false,false,true,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }
            }
        }
    }else{// !model.itemsAreQObjects
        if(model.is_mutable_range){
            if(model.is_mutable_row){
                if(model.is_list_range){
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,true,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,true,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,true,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,true,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,true,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,true,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,false,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,false,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,false,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,true,false,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,true,false,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,true,false,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }
            }else{ // !is_mutable_row
                if(model.is_list_range){
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,true,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,true,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,true,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,true,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,true,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,true,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,false,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,false,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,false,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<true,false,false,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<true,false,false,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<true,false,false,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
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
                            initializeTable<false,true,true,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,true,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,true,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,true,true,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,true,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,true,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,true,false,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,false,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,false,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,true,false,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,true,false,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,true,false,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }
            }else{ // !is_mutable_row
                if(model.is_list_range){
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,true,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,true,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,true,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,true,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,true,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,true,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }else{
                    if(model.is_list_row){
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,false,true,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,false,true,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,false,true,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }else{
                        switch(model.rowType){
                        case RowType::Data:
                            initializeTable<false,false,false,false,false,true,RowType::Data>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::MetaObject:
                            initializeTable<false,false,false,false,false,true,RowType::MetaObject>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        case RowType::Range:
                            initializeTable<false,false,false,false,false,true,RowType::Range>(itemModel, model.container, std::move(model.elementMetaType), std::move(model.sequentialAccess), std::move(model.itemAccess));
                            break;
                        }
                    }
                }
            }
        }
    }
}
#endif