/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

package io.qt.sql;

import io.qt.core.QAbstractItemModel;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QMetaType;
import io.qt.core.Qt;
import io.qt.widgets.QComboBox;
import io.qt.widgets.QStyleOptionViewItem;
import io.qt.widgets.QItemDelegate;
import io.qt.widgets.QWidget;

/**
 * <p>The QSqlRelationalDelegate class provides a delegate that is used to display and edit data from a QSqlRelationalTableModel.</p>
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qsqlrelationaldelegate.html">QSqlRelationalDelegate</a></p>
 */
public class QSqlRelationalDelegate extends QItemDelegate {
	
	static {
        QtJambi_LibraryUtilities.initialize();
    }
	
    /**
     * <p>See <a href="https://doc.qt.io/qt/qsqlrelationaldelegate.html#QSqlRelationalDelegate">QSqlRelationalDelegate::QSqlRelationalDelegate(QObject *)</a></p>
     */
	public QSqlRelationalDelegate() {
		super((QObject)null);
    }

    /**
     * <p>See <a href="https://doc.qt.io/qt/qsqlrelationaldelegate.html#QSqlRelationalDelegate">QSqlRelationalDelegate::QSqlRelationalDelegate(QObject *)</a></p>
     */
    public QSqlRelationalDelegate(QObject parent) {
        super(parent);
    }

    /**
     * Constructor for internal use only.
     * It is not allowed to call the declarative constructor from inside Java.
     */
    @io.qt.NativeAccess
	protected QSqlRelationalDelegate(QPrivateConstructor p) {
		super(p);
	}

    /**
     * <p>See <a href="https://doc.qt.io/qt/qsqlrelationaldelegate.html#createEditor">QSqlRelationalDelegate::createEditor(QWidget *, const QStyleOptionViewItem &amp;, const QModelIndex &amp;) const</a></p>
     */
	@Override
    public QWidget createEditor(QWidget parent,
                                QStyleOptionViewItem option,
                                QModelIndex index) {
        if (!index.isValid())
            return null;
        QSqlRelationalTableModel sqlModel = qobject_cast(QSqlRelationalTableModel.class, index.model());
        QSqlTableModel childModel = sqlModel!=null ? sqlModel.relationModel(index.column()) : null;
        if (childModel==null)
            return super.createEditor(parent, option, index);
        QSqlDriver driver = childModel.database().driver();
        QComboBox combo = new QComboBox(parent);
        combo.setModel(childModel);
        combo.setModelColumn(fieldIndex(childModel, driver,
                                         sqlModel.relation(index.column()).displayColumn()));
        combo.installEventFilter(this);
        return combo;
    }

    /**
     * <p>See <a href="https://doc.qt.io/qt/qsqlrelationaldelegate.html#setModelData">QSqlRelationalDelegate::setModelData(QWidget *, QAbstractItemModel *, const QModelIndex &amp;) const</a></p>
     */
    @Override
    public void setEditorData(QWidget editor, QModelIndex index) {
        if (index==null || !index.isValid())
            return;
        if (editor instanceof QComboBox) {
            Object value = index.data(Qt.ItemDataRole.DisplayRole);
            String n = editor.metaObject().userProperty().name();
            if (!n.isEmpty()) {
                if (value==null)
                    value = QMetaType.create(editor.metaObject().userProperty().userType());
                editor.setProperty(n, value);
                return;
            }
        }
        super.setEditorData(editor, index);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setModelData(QWidget editor, QAbstractItemModel model, QModelIndex index) {
        if (!index.isValid())
            return;
        QSqlRelationalTableModel sqlModel = qobject_cast(QSqlRelationalTableModel.class, index.model());
        QSqlTableModel childModel = sqlModel!=null ? sqlModel.relationModel(index.column()) : null;
        QComboBox combo = qobject_cast(QComboBox.class, editor);
        if (sqlModel==null || childModel==null || combo==null) {
            super.setModelData(editor, model, index);
            return;
        }
        QSqlDriver driver = childModel.database().driver();

        int currentItem = combo.currentIndex();
        int childColIndex = fieldIndex(childModel, driver,
                                       sqlModel.relation(index.column()).displayColumn());
        int childEditIndex = fieldIndex(childModel, driver,
                                        sqlModel.relation(index.column()).indexColumn());
        sqlModel.setData(index,
                childModel.data(childModel.index(currentItem, childColIndex), Qt.ItemDataRole.DisplayRole),
                Qt.ItemDataRole.DisplayRole);
        sqlModel.setData(index,
                childModel.data(childModel.index(currentItem, childEditIndex), Qt.ItemDataRole.EditRole),
                Qt.ItemDataRole.EditRole);
    }
    
    private static int fieldIndex(QSqlTableModel model, QSqlDriver driver, String fieldName) {
    	String stripped = driver.isIdentifierEscaped(fieldName, QSqlDriver.IdentifierType.FieldName)
    			? driver.stripDelimiters(fieldName, QSqlDriver.IdentifierType.FieldName) : fieldName;
	  return model.fieldIndex(stripped);
	}
}
