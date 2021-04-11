/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.core;

import io.qt.internal.NativeAccess;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qmodelindex.html">QModelIndex</a></p>
 */
public final class QModelIndex
	implements java.lang.Comparable<io.qt.core.QModelIndex>,
	java.lang.Cloneable
{
	@NativeAccess
    private QModelIndex(int row, int column, long internalId, QAbstractItemModel model) {
        this.row = row;
        this.column = column;
        this.internalId = internalId;
        this.model = model;
    }

    @io.qt.QtUninvokable
    public int row() { return row; }

    @io.qt.QtUninvokable
    public int column() { return column; }

    @io.qt.QtUninvokable
    public long internalId() { return internalId; }

    /**
     * Queries the model for the parent of this item. This is equivalent to calling
     * <pre>
     * item.model().parent(item);
     * </pre>
     * @return The parent index for this model index.
     */
    @io.qt.QtUninvokable
    public QModelIndex parent() {
        return model != null ? model.parent(this) : null;
    }

    /**
     * Queries the model for a sibling of this model index.
     * @param arow The row for the sibling.
     * @param acolumn The column for the sibling.
     * @return The sibling of this item at the given position.
     */
    @io.qt.QtUninvokable
    public QModelIndex sibling(int arow, int acolumn) {
        return model != null ? model.index(arow, acolumn, model.parent(this)) : null;
    }


    /**
     * Queries the model for a child index of this index.
     * @param arow the row of the child.
     * @param acolumn the column of the child.
     * @return The child at the given position.
     */
    @io.qt.QtUninvokable
    public QModelIndex child(int arow, int acolumn) {
        return model != null ? model.index(arow, acolumn, this) : null;
    }

    /**
     * Queries the model for the default data for this index. The default data is
     * specified in <code>Qt.ItemDataRole.DisplayRole</code>.
     * @return The data for this index
     */
    @io.qt.QtUninvokable
    public Object data() { return data(Qt.ItemDataRole.DisplayRole); }

    /**
     * Queries the model for data for this index.
     * @param role The data role to query for. The different data roles are available in
     * <code>io.qt.core.Qt.ItemDataRole</code>
     * @return The data for this index.
     */
    @io.qt.QtUninvokable
    public Object data(int role) {
        return model != null ? model.data(this, role) : null;
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.Qt.ItemFlags flags(){
    	return model != null ? model.flags(this) : null;
    }

    /**
     * @return The model for this index.
     */
    @io.qt.QtUninvokable
    public QAbstractItemModel model() {
        return model;
    }

    /**
     * Compares this model index to another
     * @param other The object to compare to
     * @return True if the objects are equal; otherwise returns false.
     */
    @io.qt.QtUninvokable
    public boolean equals(Object other) {
        if (!(other instanceof QModelIndex))
            return false;
        QModelIndex oi = (QModelIndex) other;
        return oi.row == row
            && oi.column == column
            && oi.internalId == internalId
            && oi.model == model;
    }

    @io.qt.QtUninvokable
    public int hashCode() {
        return row << 4 + column + internalId;
    }

    @io.qt.QtUninvokable
    public String toString() {
        return new StringBuilder()
                .append("QModelIndex(row=")
                .append(row)
                .append(",col=")
                .append(column)
                .append(",internal=")
                .append(internalId)
                .append(")").toString();
    }
    
    @io.qt.QtUninvokable
    public int compareTo(io.qt.core.QModelIndex other) {
        if (equals(other)) return 0;
        else if (operator_less(other)) return -1;
        else return 1;
    }
    
    @io.qt.QtUninvokable
    private boolean operator_less(io.qt.core.QModelIndex other) {
    	return  row <  other.row
                || (row == other.row && (column <  other.column
                                 || (column == other.column && (internalId <  other.internalId
                                                  || (internalId == other.internalId && io.qt.internal.QtJambiInternal.nativeId(model) < io.qt.internal.QtJambiInternal.nativeId(other.model))))));
    }

    private @NativeAccess int row;
    private @NativeAccess int column;
    private @NativeAccess long internalId;
    private @NativeAccess QAbstractItemModel model;
    
    @Override
    @io.qt.QtUninvokable
    public QModelIndex clone() {
        return new QModelIndex(row, column, internalId, model);
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QModelIndex siblingAtColumn(int column){
    	return model!=null ? (this.column == column) ? this : model.sibling(row, column, this) : null;
    }
    @io.qt.QtUninvokable
    private native io.qt.core.QModelIndex __qt_QModelIndex_siblingAtColumn_int(long __this__nativeId, int column);

    @io.qt.QtUninvokable
    public final io.qt.core.QModelIndex siblingAtRow(int row){
    	return model!=null ? (this.row == row) ? this : model.sibling(row, column, this) : null;
    }
}
