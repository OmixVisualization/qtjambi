/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#include <QtWidgets/QtWidgets>
#include "qwidgetutils.h"

const QObject* __owner_function_QGraphicsLayoutItem(QGraphicsLayoutItem* __qt_this){
    if(__qt_this->graphicsItem()){
        return __owner_function_QGraphicsItem(__qt_this->graphicsItem());
    }else if(__qt_this->parentLayoutItem()){
        return __owner_function_QGraphicsLayoutItem(__qt_this->parentLayoutItem());
    }else{
        return nullptr;
    }
}

const QObject* __owner_function_QGraphicsItem(QGraphicsItem* __qt_this)
{
    if(__qt_this->toGraphicsObject()){
        return __qt_this->toGraphicsObject();
    }else if(__qt_this->scene()){
        return __qt_this->scene();
    }else{
        return __qt_this->parentObject();
    }
}

const QObject* __owner_function_QTreeWidgetItemIterator(QTreeWidgetItemIterator* iter)
{
    struct TreeWidgetItemIteratorPrivate{
        int m_currentIndex;
        QObject *m_model;
    };

    struct TreeWidgetItemIterator{
        QScopedPointer<TreeWidgetItemIteratorPrivate> d_ptr;
    };
    TreeWidgetItemIteratorPrivate* p = reinterpret_cast<TreeWidgetItemIterator*>(iter)->d_ptr.get();
    return p->m_model;
    /*
    if(!iter->operator*()){
        iter->operator--();
    }
    if(iter->operator*() && iter->operator*()->treeWidget()){
        return iter->operator*()->treeWidget()->model();
    }
    return nullptr;*/
}
