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

import java.util.LinkedList;

import org.qtjambi.qt.core.QAbstractItemModel;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPolygonF;
import org.qtjambi.qt.widgets.QAbstractItemView;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QItemDelegate;
import org.qtjambi.qt.widgets.QStyle;
import org.qtjambi.qt.widgets.QStyleOptionViewItem;
import org.qtjambi.qt.widgets.QTableWidget;
import org.qtjambi.qt.widgets.QTableWidgetItem;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Star Delegate")
public class StarDelegate extends QWidget {
    private QTableWidget table;

    private Object tableContent[][] = {
        { tr("Mass in B-Minor"), tr("Baroque"), tr("JS Bach"),
          new StarRating(5) },
        { tr("Sex Bomb"), tr("Pop"), tr("Tom Jones"), new StarRating(2) },
        { tr("Three More Foxes"), tr("jazz"), tr("Maynard Ferguson"),
          new StarRating(4) },
        { tr("Barbie Girl"), tr("Pop"), tr("Aqua"), new StarRating(5) }
    };

    public StarDelegate() {
        this(null);
    }

    public StarDelegate(QWidget parent)
    {
        super(parent);
        createTable();

        QGridLayout layout = new QGridLayout();
        layout.addWidget(table, 0, 0);
        setLayout(layout);
        setWindowTitle(tr("Star Delegate"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        setMinimumSize(550, 200);
    }

//! [0]
    public void createTable()
    {
        LinkedList<String> headers = new LinkedList<String>();

        table = new QTableWidget(4, 4);

        table.setItemDelegate(new Delegate(table));

        table.setEditTriggers(QAbstractItemView.EditTrigger.DoubleClicked,
                              QAbstractItemView.EditTrigger.SelectedClicked);
        table.setSelectionBehavior(
            QAbstractItemView.SelectionBehavior.SelectRows);
//! [0]

        headers.add(tr("Title"));
        headers.add(tr("Genre"));
        headers.add(tr("Artist"));
        headers.add(tr("Rating"));
        table.setHorizontalHeaderLabels(headers);

        for (int i = 0; i < tableContent.length; i++) {
            table.setItem(i, 0,
                new QTableWidgetItem((String) tableContent[i][0]));
            table.setItem(i, 1,
                new QTableWidgetItem((String) tableContent[i][1]));
            table.setItem(i, 2,
                new QTableWidgetItem((String) tableContent[i][2]));

            QTableWidgetItem rRating = new QTableWidgetItem();
            rRating.setData(Qt.ItemDataRole.DisplayRole, tableContent[i][3]);
            table.setItem(i, 3, rRating);
        }

        table.resizeColumnsToContents();
//! [1]
    }
//! [1]

    class Delegate extends QItemDelegate
    {
        public Delegate(QWidget parent)
        {
            super(parent);
        }

        @Override
//! [2]
        public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index)
        {
            Object data = index.data();

            if (data != null && data instanceof StarRating) {
                if (option.state().isSet(QStyle.StateFlag.State_Selected)) {
                    painter.fillRect(option.rect(), option.palette().highlight());
                }
                ((StarRating) data).paint(painter, option.rect(), option.palette(),
                                          StarRating.ReadOnly);
            } else
                super.paint(painter, option, index);
        }
//! [2]

        @Override
//! [3]
        public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index)
        {
            Object data = index.data();

            if (data instanceof StarRating)
                return ((StarRating) data).sizeHint();
            else
                return super.sizeHint(option, index);
        }
//! [3]

        @Override
//! [4]
        public QWidget createEditor(QWidget parent, QStyleOptionViewItem item,
                                    QModelIndex index)
        {
            Object data = index.data();

            if (data instanceof StarRating)
                return new StarEditor(parent, (StarRating) data);
            else
                return super.createEditor(parent, item, index);
        }
//! [4]

        @Override
//! [5]
        public void setEditorData(QWidget editor, QModelIndex index)
        {
            Object data = index.data();

            if (data instanceof StarRating)
                ((StarEditor) editor).setStarRating((StarRating) data);
            else
                super.setEditorData(editor, index);
        }
//! [5]

        @Override
//! [6]
        public void setModelData(QWidget editor, QAbstractItemModel model,
                                 QModelIndex index)
        {
            if (index.data() instanceof StarRating)
                model.setData(index, ((StarEditor) editor).starRating());
            else
                super.setModelData(editor, model, index);
        }
//! [6]
    }

    class StarEditor extends QWidget
    {
        private StarRating starRating;

//! [7]
        public StarEditor(QWidget parent, StarRating rating)
        {
            super(parent);

            starRating = rating;
            setMouseTracking(true);
            setAutoFillBackground(true);
        }
//! [7]

        @Override
        public QSize sizeHint()
        {
            return starRating.sizeHint();
        }

        @Override
//! [8]
        public void paintEvent(QPaintEvent event)
        {
            QPainter painter = new QPainter(this);
            starRating.paint(painter, rect(), palette(), StarRating.ReadWrite);
        }
//! [8]

        @Override
//! [9]
        public void mouseMoveEvent(QMouseEvent event)
        {
            int star = starAtPosition(event.x());

            if (star != starRating.getRating() && star > 0) {
                starRating.setRating(star);
                update();
            }
        }
//! [9]

//! [10]
        public int starAtPosition(int x)
        {
            int star = (x / (starRating.sizeHint().width()
                            / starRating.getMaxRating())) + 1;

            if (star <= 0 || star > starRating.getMaxRating())
                return -1;

            return star;
        }
//! [10]

        public void setStarRating(StarRating rating)
        {
            starRating = rating;
        }

        public StarRating starRating()
        {
            return starRating;
        }
    }

    class StarRating
    {
        private int starCount, maxCount;
        private QPolygonF starPolygon, diamondPolygon;

        public static final int ReadOnly = 0, ReadWrite = 1, PaintingFactor = 20;

        private void setupPolygons() {
            starPolygon = new QPolygonF();
            starPolygon.append(new QPointF(1.0, 0.5));
            for (int i = 1; i < 5; i++)
                starPolygon.append(
                    new QPointF(0.5 + 0.5 * Math.cos(0.8 * i * Math.PI),
                                0.5 + 0.5 * Math.sin(0.8 * i * Math.PI)));

            diamondPolygon = new QPolygonF();
            diamondPolygon.append(new QPointF(0.4, 0.5));
            diamondPolygon.append(new QPointF(0.5, 0.4));
            diamondPolygon.append(new QPointF(0.6, 0.5));
            diamondPolygon.append(new QPointF(0.5, 0.6));
            diamondPolygon.append(new QPointF(0.4, 0.5));
        }

        public StarRating()
        {
            this(1, 5);
        }

        public StarRating(int rating)
        {
            this(rating, 5);
        }

//! [11]
        public StarRating(int rating, int maxRating)
        {
            setupPolygons();
            maxCount = maxRating;
            setRating(rating);
        }
//! [11]

        public void setRating(int rating)
        {
            if (rating > 0 && rating <= maxCount)
                starCount = rating;
            else
                starCount = maxCount;
        }

        public int getRating()
        {
            return starCount;
        }

        public int getMaxRating()
        {
            return maxCount;
        }

//! [12]
        public void paint(QPainter painter, QRect rect, QPalette palette,
                      int mode)
        {
            painter.save();

            painter.setRenderHint(QPainter.RenderHint.Antialiasing, true);
            painter.setPen(Qt.PenStyle.NoPen);

            if (mode == ReadWrite)
                painter.setBrush(palette.highlight());
            else
                painter.setBrush(palette.text());

            int yOffset = (rect.height() - PaintingFactor) / 2;
            painter.translate(rect.x(), rect.y() + yOffset);
            painter.scale(PaintingFactor, PaintingFactor);

            for (int i = 0; i < maxCount; i++) {
                if (i < starCount)
                    painter.drawPolygon(starPolygon, Qt.FillRule.WindingFill);
                else
                    painter.drawPolygon(diamondPolygon, Qt.FillRule.WindingFill);

                painter.translate(1.0, 0.0);
            }

            painter.restore();
        }
//! [12]

        public QSize sizeHint()
        {
            return new QSize(PaintingFactor * maxCount, PaintingFactor);
        }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QWidget widget = new StarDelegate();
        widget.show();
        widget.resize(470, 200);

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
