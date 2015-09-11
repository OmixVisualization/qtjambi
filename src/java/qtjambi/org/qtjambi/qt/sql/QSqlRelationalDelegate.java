/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.sql;

import org.qtjambi.qt.widgets.QItemDelegate;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.widgets.QStyleOptionViewItem;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QAbstractItemModel;
import org.qtjambi.qt.core.Qt;

public class QSqlRelationalDelegate extends QItemDelegate {
    public QSqlRelationalDelegate() {
        this(null);
    }

    public QSqlRelationalDelegate(QObject parent) {
        super(parent);
    }


    @Override
    public QWidget createEditor(QWidget parent,
                                QStyleOptionViewItem option,
                                QModelIndex index) {
        QSqlRelationalTableModel sqlModel = (QSqlRelationalTableModel)index.model();
        QSqlTableModel childModel = sqlModel != null ? sqlModel.relationModel(index.column()) : null;
        if (childModel == null)
            return super.createEditor(parent, option, index);

        QComboBox combo = new QComboBox(parent);
        combo.setModel(childModel);
        combo.setModelColumn(childModel.fieldIndex(sqlModel.relation(index.column()).displayColumn()));
        combo.installEventFilter(this);

        return combo;
    }

    @Override
    public void setEditorData(QWidget editor, QModelIndex index) {
        QSqlRelationalTableModel sqlModel = (QSqlRelationalTableModel)index.model();
        QComboBox combo = editor instanceof QComboBox ? (QComboBox) editor : null;
        if (sqlModel == null || combo == null) {
            super.setEditorData(editor, index);
            return;
        }
        combo.setCurrentIndex(combo.findText(sqlModel.data(index).toString()));
    }

    @Override
    public void setModelData(QWidget editor, QAbstractItemModel model, QModelIndex index) {
        if (index == null)
            return;

        QSqlRelationalTableModel sqlModel = (QSqlRelationalTableModel)model;
        QSqlTableModel childModel = sqlModel != null ? sqlModel.relationModel(index.column()) : null;
        QComboBox combo = editor instanceof QComboBox ? (QComboBox) editor : null;
        if (sqlModel == null || childModel == null || combo == null) {
            super.setModelData(editor, model, index);
            return;
        }

        int currentItem = combo.currentIndex();
        int childColIndex = childModel.fieldIndex(sqlModel.relation(index.column()).displayColumn());
        int childEditIndex = childModel.fieldIndex(sqlModel.relation(index.column()).indexColumn());
        sqlModel.setData(index,
                         childModel.data(childModel.index(currentItem, childColIndex), Qt.ItemDataRole.DisplayRole),
                         Qt.ItemDataRole.DisplayRole);
        sqlModel.setData(index,
                childModel.data(childModel.index(currentItem, childEditIndex), Qt.ItemDataRole.EditRole),
                Qt.ItemDataRole.EditRole);
    }
}
