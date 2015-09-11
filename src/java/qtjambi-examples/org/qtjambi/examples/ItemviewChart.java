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

package org.qtjambi.examples;

import java.util.Vector;

import org.qtjambi.qt.core.QAbstractItemModel;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QItemSelection;
import org.qtjambi.qt.core.QItemSelectionModel;
import org.qtjambi.qt.core.QItemSelectionRange;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeySequence;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QRegion;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.gui.QStandardItemModel;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.widgets.QAbstractItemView;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QRubberBand;
import org.qtjambi.qt.widgets.QSplitter;
import org.qtjambi.qt.widgets.QStyleOptionViewItem;
import org.qtjambi.qt.widgets.QTableView;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Itemview Charts")
//! [0]
public class ItemviewChart extends QMainWindow {

    private QAbstractItemModel model;
//! [0]

    public static void main(String args[]) {
        QApplication.initialize(args);

        ItemviewChart mainW = new ItemviewChart();
        mainW.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

//! [1]
    public ItemviewChart() {
        QMenu fileMenu = new QMenu(tr("&File"), this);

        QAction openAction = new QAction(tr("&Open..."), this);
        openAction.setShortcut(new QKeySequence(tr("Ctrl+O")));
        openAction.triggered.connect(this, "openFile()");
        fileMenu.addAction(openAction);

        QAction saveAction = new QAction(tr("&Save As..."), this);
        saveAction.setShortcut(new QKeySequence(tr("Ctrl+S")));
        saveAction.triggered.connect(this, "saveFile()");
        fileMenu.addAction(saveAction);

        QAction quitAction = new QAction(tr("&Quit"), this);
        quitAction.setShortcut(new QKeySequence(tr("Ctrl+Q")));
        quitAction.triggered.connect(this, "close()");
        fileMenu.addAction(quitAction);

        setupModel();
        setupViews();

        menuBar().addMenu(fileMenu);
        statusBar();

        openFile("classpath:org/qtjambi/examples/qtdata.cht");

        setWindowTitle(tr("Chart"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        resize(750, 500);
    }
//! [1]

//! [2]
    private void setupModel() {
        model = new QStandardItemModel(8, 2, this);
        model.setHeaderData(0, Qt.Orientation.Horizontal, tr("Label"));
        model.setHeaderData(1, Qt.Orientation.Horizontal, tr("Quantity"));
    }
//! [2]

//! [3]
    private void setupViews() {
        QSplitter splitter = new QSplitter();
        QTableView table = new QTableView();
        QAbstractItemView pieChart = new PieView(this);
        splitter.addWidget(table);
        splitter.addWidget(pieChart);
        splitter.setStretchFactor(0, 0);
        splitter.setStretchFactor(1, 1);

        table.setModel(model);
        pieChart.setModel(model);

        QItemSelectionModel selectionModel = new QItemSelectionModel(model);
        table.setSelectionModel(selectionModel);
        pieChart.setSelectionModel(selectionModel);

        setCentralWidget(splitter);
    }
//! [3]

    private void openFile() {
        openFile("");
    }

    private void openFile(String path) {
        String fileName;
        if (path.equals(""))
            fileName = QFileDialog.getOpenFileName(this, tr("Choose a data file"), "", new QFileDialog.Filter("*.cht"));
        else
            fileName = path;

        if (!fileName.equals("")) {
            QFile file = new QFile(fileName);

            if (file.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly, QFile.OpenModeFlag.Text))) {
                QTextStream stream = new QTextStream(file);
                String line;

                model.removeRows(0, model.rowCount(null), null);

                int row = 0;
                do {
                    line = stream.readLine();
                    if (!line.equals("")) {

                        model.insertRows(row, 1, null);

                        String[] pieces = RetroTranslatorHelper.split(line, ",");

                        model.setData(model.index(row, 0, null), pieces[0].trim());
                        model.setData(model.index(row, 1, null), pieces[1].trim());
                        model.setData(model.index(row, 0, null), new QColor(pieces[2].trim()), Qt.ItemDataRole.DecorationRole);
                        row++;

                    }
                } while (!line.equals(""));

                file.close();
                statusBar().showMessage(String.format(tr("Loaded %s"), fileName), 2000);
            }
        }
    }

    private void saveFile() {
        String fileName = QFileDialog.getSaveFileName(this, tr("Save file as"), "", new QFileDialog.Filter("*.cht"));

        if (!fileName.equals("")) {
            QFile file = new QFile(fileName);
            QTextStream stream = new QTextStream(file);

            if (file.open(new QFile.OpenMode(QFile.OpenModeFlag.WriteOnly, QFile.OpenModeFlag.Text))) {
                for (int row = 0; row < model.rowCount(null); ++row) {

                    stream.writeString(model.data(model.index(row, 0, null), Qt.ItemDataRole.DisplayRole).toString() + ",");
                    stream.writeString(model.data(model.index(row, 1, null), Qt.ItemDataRole.DisplayRole).toString() + ",");
                    stream.writeString(((QColor) model.data(model.index(row, 0, null), Qt.ItemDataRole.DecorationRole)).name());
                    stream.writeString("\n");
                }
            }

            file.close();
            statusBar().showMessage(String.format(tr("Saved %s"), fileName), 2000);
        }
    }

//! [4]
    private class PieView extends QAbstractItemView {

        private int margin;
        private int totalSize;
        private int pieSize;
        private int validItems;
        private double totalValue;
        private QPoint origin;
        private QRubberBand rubberBand;
//! [4]

//! [5]
        public PieView(QWidget parent) {
            super(parent);
            horizontalScrollBar().setRange(0, 0);
            verticalScrollBar().setRange(0, 0);

            margin = 8;
            totalSize = 300;
            pieSize = totalSize - 2 * margin;
            validItems = 0;
            totalValue = 0.0;
        }
//! [5]

        @Override
//! [6]
        protected void dataChanged(final QModelIndex topLeft, final QModelIndex bottomRight, java.util.List<java.lang.Integer> roles) {
            super.dataChanged(topLeft, bottomRight, roles);

            validItems = 0;
            totalValue = 0.0;

            for (int row = 0; row < model().rowCount(rootIndex()); ++row) {

                QModelIndex index = model().index(row, 1, rootIndex());
                double value = toDouble(model().data(index));

                if (value > 0.0) {
                    totalValue += value;
                    validItems++;
                }
            }
            viewport().update();
//! [6] //! [7]
        }
//! [7]

        @Override
//! [8]
        protected boolean edit(final QModelIndex index, EditTrigger trigger, QEvent event) {
            return false;
        }
//! [8]

        @Override
//! [9]
        public QModelIndex indexAt(final QPoint point) {
            if (validItems == 0)
                return null;

            int wx = point.x() + horizontalScrollBar().value();
            int wy = point.y() + verticalScrollBar().value();
//! [9]

//! [10]
            if (wx < totalSize) {
                double cx = wx - totalSize / 2;
                double cy = totalSize / 2 - wy;
                double d = Math.pow(Math.pow(cx, 2) + Math.pow(cy, 2), 0.5);

                if (d == 0 || d > pieSize / 2)
                    return null;

                double angle = (180 / Math.PI) * Math.acos(cx / d);
                if (cy < 0)
                    angle = 360 - angle;

                double startAngle = 0.0;

                for (int row = 0; row < model().rowCount(rootIndex()); ++row) {

                    QModelIndex index = model().index(row, 1, rootIndex());
                    double value = toDouble(model().data(index));

                    if (value > 0.0) {
                        double sliceAngle = 360 * value / totalValue;

                        if (angle >= startAngle && angle < (startAngle + sliceAngle))
                            return model().index(row, 1, rootIndex());

                        startAngle += sliceAngle;
//! [10] //! [11]
                    }
                }
            }

            return null;
//! [11] //! [12]
        }
//! [12]

        @Override
        protected boolean isIndexHidden(final QModelIndex index) {
            return false;
        }

//! [13]
        QRect itemRect(final QModelIndex index) {
            if (index == null)
                return new QRect();

            if (index.column() != 1)
                return new QRect();

            if (toDouble(model().data(index)) > 0.0) {
                return new QRect(margin, margin, pieSize, pieSize);
//! [13] //! [14]
            }
            return new QRect();
//! [14] //! [15]
        }
//! [15]

//! [16]
        QRegion itemRegion(final QModelIndex index) {
            if (index == null)
                return null;
//! [16] //! [17]

            if (index.column() != 1)
                return null;
//! [17] //! [18]

            if (toDouble(model().data(index)) <= 0.0)
                return null;
//! [18]

//! [19]
            double startAngle = 0.0;
            for (int row = 0; row < model().rowCount(rootIndex()); ++row) {

                QModelIndex sliceIndex = model().index(row, 1, rootIndex());
                double value = toDouble(model().data(sliceIndex));

                if (value > 0.0) {
                    double angle = 360 * value / totalValue;

                    if (sliceIndex.equals(index)) {
                        QPainterPath slicePath = new QPainterPath();
                        slicePath.moveTo(totalSize / 2, totalSize / 2);
                        slicePath.arcTo(margin, margin, margin + pieSize, margin + pieSize, startAngle, angle);
                        slicePath.closeSubpath();

                        return new QRegion(slicePath.toFillPolygon(new QTransform()).toPolygon());
                    }
                    startAngle += angle;
//! [19] //! [20]
                }
            }

            return null;
//! [20] //! [21]
        }
//! [21]

        @Override
//! [22]
        protected int horizontalOffset() {
            return horizontalScrollBar().value();
        }
//! [22]

        @Override
//! [23]
        protected void mousePressEvent(QMouseEvent event) {
            super.mousePressEvent(event);
            origin = event.pos();
            if (rubberBand == null)
                rubberBand = new QRubberBand(QRubberBand.Shape.Rectangle, this);
            rubberBand.setRubberBandGeometry(new QRect(origin, new QSize()));
            rubberBand.show();
        }
//! [23]

        @Override
//! [24]
        protected void mouseMoveEvent(QMouseEvent event) {
            QRect rect = new QRect(origin, event.pos()).normalized();
            rubberBand.setRubberBandGeometry(rect);
            super.mouseMoveEvent(event);

            QModelIndex underMouseIndex = indexAt(event.pos());
            if (underMouseIndex == null)
                setSelection(rect, selectionCommand(underMouseIndex, event));
            viewport().update();
        }
//! [24]

        @Override
//! [25]
        protected void mouseReleaseEvent(QMouseEvent event) {
            super.mouseReleaseEvent(event);
            rubberBand.hide();
            viewport().update();
        }
//! [25]

        @Override
//! [26]
        protected QModelIndex moveCursor(QAbstractItemView.CursorAction cursorAction, Qt.KeyboardModifiers modifiers) {
            QModelIndex current = currentIndex();

            switch (cursorAction) {
            case MoveLeft:
            case MoveUp:
                if (current.row() > 0)
                    current = model().index(current.row() - 1, current.column(), rootIndex());
                else
                    current = model().index(0, current.column(), rootIndex());
                break;
            case MoveRight:
            case MoveDown:
                if (current.row() < rows(current) - 1)
                    current = model().index(current.row() + 1, current.column(), rootIndex());
                else
                    current = model().index(rows(current) - 1, current.column(), rootIndex());
                break;
            default:
                break;
            }

            viewport().update();
            return current;
//! [26] //! [27]
        }
//! [27]

        @Override
        protected void paintEvent(QPaintEvent event) {
            QItemSelectionModel selections = selectionModel();
            QStyleOptionViewItem option = viewOptions();

            QBrush background = option.palette().base();
            QPen foreground = new QPen(option.palette().color(QPalette.ColorRole.WindowText));

            QPainter painter = new QPainter();
            painter.begin(viewport());
            painter.setRenderHint(QPainter.RenderHint.Antialiasing);

            painter.fillRect(event.rect(), background);
            painter.setPen(foreground);

            QRect pieRect = new QRect(margin, margin, pieSize, pieSize);

            if (validItems > 0) {

                painter.save();
                painter.translate(pieRect.x() - horizontalScrollBar().value(), pieRect.y() - verticalScrollBar().value());
                painter.drawEllipse(0, 0, pieSize, pieSize);
                double startAngle = 0.0;
                int row;

                for (row = 0; row < model().rowCount(rootIndex()); ++row) {

                    QModelIndex index = model().index(row, 1, rootIndex());
                    double value = toDouble(model().data(index));

                    if (value > 0.0) {
                        double angle = 360 * value / totalValue;

                        QModelIndex colorIndex = model().index(row, 0, rootIndex());
                        QColor color = (QColor) model().data(colorIndex, Qt.ItemDataRole.DecorationRole);

                        if (currentIndex() != null && currentIndex().equals(index) && selections.isSelected(index))
                            painter.setBrush(new QBrush(color, Qt.BrushStyle.Dense4Pattern));
                        else if (selections.isSelected(index))
                            painter.setBrush(new QBrush(color, Qt.BrushStyle.Dense3Pattern));
                        else
                            painter.setBrush(new QBrush(color));

                        painter.drawPie(0, 0, pieSize, pieSize, (int) (startAngle * 16), (int) (angle * 16));

                        startAngle += angle;
                    }
                }
                painter.restore();
            }
            painter.end();
        }

        @Override
        protected void resizeEvent(QResizeEvent event) {
            updateGeometries();
        }

        int rows(final QModelIndex index) {
            return model().rowCount(model().parent(index));
        }

        @Override
//! [28]
        protected void rowsInserted(final QModelIndex parent, int start, int end) {
            for (int row = start; row <= end; ++row) {

                QModelIndex index = model().index(row, 1, rootIndex());
                double value = toDouble(model().data(index));

                if (value > 0.0) {
                    totalValue += value;
                    validItems++;
                }
            }

            super.rowsInserted(parent, start, end);
        }

        @Override
        protected void rowsAboutToBeRemoved(final QModelIndex parent, int start, int end) {
//! [28] //! [29]
            for (int row = start; row <= end; ++row) {

                QModelIndex index = model().index(row, 1, rootIndex());
                double value = toDouble(model().data(index));
                if (value > 0.0) {
                    totalValue -= value;
                    validItems--;
                }
            }

            super.rowsAboutToBeRemoved(parent, start, end);
//! [29] //! [30]
        }
//! [30]

        @Override
//! [31]
        public void scrollTo(final QModelIndex index, ScrollHint hint) {
            QRect area = viewport().rect();
            QRect rect = visualRect(index);

            if (rect.left() < area.left())
                horizontalScrollBar().setValue(
                    horizontalScrollBar().value() + rect.left() - area.left());
            else if (rect.right() > area.right())
                horizontalScrollBar().setValue(
                    horizontalScrollBar().value() + Math.min(
                        rect.right() - area.right(), rect.left() - area.left()));

            if (rect.top() < area.top())
                verticalScrollBar().setValue(
                    verticalScrollBar().value() + rect.top() - area.top());
            else if (rect.bottom() > area.bottom())
                verticalScrollBar().setValue(
                    verticalScrollBar().value() + Math.min(
                        rect.bottom() - area.bottom(), rect.top() - area.top()));

            update();
        }
//! [31]

        @Override
//! [32]
        protected void setSelection(final QRect rect, QItemSelectionModel.SelectionFlags command) {
            QRect contentsRect = rect.translated(horizontalScrollBar().value(), verticalScrollBar().value()).normalized();

            int rows = model().rowCount(rootIndex());
            int columns = model().columnCount(rootIndex());
            Vector<QModelIndex> indexes = new Vector<QModelIndex>();

            for (int row = 0; row < rows; ++row) {
                for (int column = 0; column < columns; ++column) {
                    QModelIndex index = model().index(row, column, rootIndex());
                    QRegion region = itemRegion(index);

                    if (region != null && region.intersects(contentsRect))
                        indexes.add(index);
//! [32] //! [33]
                }
//! [33] //! [34]
            }
//! [34] //! [35]

            if (indexes.size() > 0) {
                int firstRow = indexes.elementAt(0).row();
                int lastRow = indexes.elementAt(0).row();
                int firstColumn = indexes.elementAt(0).column();
                int lastColumn = indexes.elementAt(0).column();

                for (int i = 1; i < indexes.size(); ++i) {
                    firstRow = Math.min(firstRow, indexes.elementAt(i).row());
                    lastRow = Math.max(lastRow, indexes.elementAt(i).row());
                    firstColumn = Math.min(firstColumn, indexes.elementAt(i).column());
                    lastColumn = Math.max(lastColumn, indexes.elementAt(i).column());
                }

                QItemSelection selection = new QItemSelection(
                    model().index(firstRow, firstColumn, rootIndex()),
                    model().index(lastRow, lastColumn, rootIndex()));
                selectionModel().select(selection, command);
            } else {
                QModelIndex noIndex = null;
                QItemSelection selection = new QItemSelection(noIndex, noIndex);
                selectionModel().select(selection, command);
            }

            update();
//! [35] //! [36]
        }
//! [36]

        @Override
//! [37]
        protected void updateGeometries() {
            horizontalScrollBar().setPageStep(viewport().width());
            horizontalScrollBar().setRange(0, Math.max(0, totalSize - viewport().width()));
            verticalScrollBar().setPageStep(viewport().height());
            verticalScrollBar().setRange(0, Math.max(0, totalSize - viewport().height()));
        }
//! [37]

        @Override
//! [38]
        protected int verticalOffset() {
            return verticalScrollBar().value();
        }
//! [38]

        @Override
//! [39]
        public QRect visualRect(final QModelIndex index) {
            QRect rect = itemRect(index);
            if (rect.isValid())
                return new QRect(rect.left() - horizontalScrollBar().value(), rect.top() - verticalScrollBar().value(), rect.width(), rect.height());
            else
                return rect;
        }
//! [39]

        @Override
//! [40]
        protected QRegion visualRegionForSelection(final QItemSelection selection) {
            int ranges = selection.size();

            if (ranges == 0)
                return new QRegion(new QRect());

            QRegion region = new QRegion();
            for (int i = 0; i < ranges; ++i) {
                QItemSelectionRange range = selection.at(i);
                for (int row = range.top(); row <= range.bottom(); ++row) {
                    for (int col = range.left(); col <= range.right(); ++col) {
                        QModelIndex index = model().index(row, col, rootIndex());
                        region = region.united(new QRegion(visualRect(index)));
                    }
                }
            }
            return region;
//! [40] //! [41]
        }
//! [41]
    }

    private double toDouble(Object o) {

        if (o instanceof String) {
            try {
                return Double.parseDouble((String) o);
            } catch (NumberFormatException e) {

            }
        }
        return 0;
    }
}
