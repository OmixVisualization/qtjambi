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

package org.qtjambi.qt.core;

import org.qtjambi.qt.QNativePointer;

public class QModelIndex
{
    private QModelIndex(int row, int column, long internalId, QAbstractItemModel model) {
        this.row = row;
        this.column = column;
        this.internalId = internalId;
        this.model = model;
    }

    public int row() { return row; }

    public int column() { return column; }

    public long internalId() { return internalId; }
    
    public QNativePointer internalPointer() { 
    	QNativePointer np = new QNativePointer(QNativePointer.Type.Long); 
    	np.setLongValue(internalId); 
    	return np; 
    }

    /**
     * Queries the model for the parent of this item. This is equivalent to calling
     * <pre>
     * item.model().parent(item);
     * </re>
     * @return The parent index for this model index.
     */
    public QModelIndex parent() {
        return model != null ? model.parent(this) : null;
    }

    /**
     * Queries the model for a sibling of this model index.
     * @param arow The row for the sibling.
     * @param acolumn The column for the sibling.
     * @return The sibling of this item at the given position.
     */
    public QModelIndex sibling(int arow, int acolumn) {
        return model != null ? model.index(arow, acolumn, model.parent(this)) : null;
    }


    /**
     * Queries the model for a child index of this index.
     * @param arow the row of the child.
     * @param acolumn the column of the child.
     * @return The child at the given position.
     */
    public QModelIndex child(int arow, int acolumn) {
        return model != null ? model.index(arow, acolumn, this) : null;
    }

    /**
     * Queries the model for the default data for this index. The default data is
     * specified in <code>Qt.ItemDataRole.DisplayRole</code>.
     * @return The data for this index
     */
    public Object data() { return data(Qt.ItemDataRole.DisplayRole); }

    /**
     * Queries the model for data for this index.
     * @param role The data role to query for. The different data roles are available in
     * <code>org.qtjambi.qt.core.Qt.ItemDataRole</code>
     * @return The data for this index.
     */
    public Object data(int role) {
        return model != null ? model.data(this, role) : null;
    }

    /**
     * @return The model for this index.
     */
    public QAbstractItemModel model() {
        return model;
    }

    /**
     * Compares this model index to another
     * @param other The object to compare to
     * @return True if the objects are equal; otherwise returns false.
     */
    public boolean equals(Object other) {
        if (!(other instanceof QModelIndex))
            return false;
        QModelIndex oi = (QModelIndex) other;
        return oi.row == row
            && oi.column == column
            && oi.internalId == internalId
            && oi.model == model;
    }

    public int hashCode() {
        return row << 4 + column + internalId;
    }

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

    private int row;
    private int column;
    private long internalId;
    private QAbstractItemModel model;
}
