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

class QChart___{
    /**
     * <p>Overloaded function for {@link #axes(io.qt.core.Qt.Orientations, io.qt.charts.QAbstractSeries)}</p>
     * <p>with: </p><ul>
     * <li><code>series = null</code></li>
     * </ul>
     */
    @QtUninvokable
    public final io.qt.core.@NonNull QList<io.qt.charts.@Nullable QAbstractAxis> axes(io.qt.core.Qt.@NonNull Orientation @NonNull... orientation) {
        return axes(orientation==null || orientation.length==0 ? new io.qt.core.Qt.Orientations(3) : new io.qt.core.Qt.Orientations(orientation), (io.qt.charts.QAbstractSeries)null);
    }
}// class

class QPolarChart___{
    /**
     * <p>Overloaded function for {@link #axes(io.qt.charts.QPolarChart.PolarOrientations, io.qt.charts.QAbstractSeries)}</p>
     * <p>with: </p><ul>
     * <li><code>series = null</code></li>
     * </ul>
     */
    @QtUninvokable
    public final io.qt.core.@NonNull QList<io.qt.charts.@Nullable QAbstractAxis> axes(@NonNull PolarOrientation @NonNull... orientation) {
        return axes(orientation==null || orientation.length==0 ? new PolarOrientations(3) : new PolarOrientations(orientation), (io.qt.charts.QAbstractSeries)null);
    }
}// class

