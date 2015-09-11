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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.qtjambi.qt.QPair;
import org.qtjambi.qt.QVariant;
import org.qtjambi.qt.QtPropertyReader;
import org.qtjambi.qt.QtPropertyUser;
import org.qtjambi.qt.QtPropertyWriter;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QItemEditorCreatorBase;
import org.qtjambi.qt.widgets.QItemEditorFactory;
import org.qtjambi.qt.widgets.QStandardItemEditorCreator;
import org.qtjambi.qt.widgets.QTableWidget;
import org.qtjambi.qt.widgets.QTableWidgetItem;
import org.qtjambi.qt.widgets.QWidget;

class ColorListEditor extends QComboBox {
    public ColorListEditor() {
        this(null);
    }

    public ColorListEditor(QWidget widget) {
        super(widget);

        populateList();
    }

    @QtPropertyUser()
    @QtPropertyReader()
    public final QColor color() {
        return (QColor) itemData(currentIndex(), Qt.ItemDataRole.DecorationRole);
    }

    @QtPropertyWriter()
    public final void setColor(QColor color) {
        setCurrentIndex(findData(color, Qt.ItemDataRole.DecorationRole));
    }

    private void populateList() {
        List<String> colorNames = QColor.colorNames();

        for (int i = 0; i < colorNames.size(); ++i) {
            QColor color = new QColor(colorNames.get(i));

            insertItem(i, colorNames.get(i));
            setItemData(i, color, Qt.ItemDataRole.DecorationRole);
        }

    }
}

@QtJambiExample(name="Color Editor Factory")
public class ColorEditorFactory extends QWidget {

    QItemEditorCreatorBase colorListCreator;
    QItemEditorFactory factory;
    public ColorEditorFactory() {
        this(null);

        factory = new QItemEditorFactory();
        colorListCreator = new QStandardItemEditorCreator(ColorListEditor.class);

        factory.registerEditor(QVariant.Color, colorListCreator);
        QItemEditorFactory.setDefaultFactory(factory);

        createGUI();
    }

    public ColorEditorFactory(QWidget parent) {
        super(parent);
    }

    private void createGUI() {
        List<QPair<String, QColor>> list = new ArrayList<QPair<String, QColor>>();
        list.add(new QPair<String, QColor> (tr("Alice"), new QColor("aliceblue")));
        list.add(new QPair<String, QColor>(tr("Neptun"), new QColor("aquamarine")));
        list.add(new QPair<String, QColor>(tr("Ferdinand"), new QColor("springgreen")));

        QTableWidget table = new QTableWidget(3, 2);
        table.setHorizontalHeaderLabels(Arrays.asList(new String[] { "Name", "Hair color" } ));
        table.verticalHeader().setVisible(false);
        table.resize(150, 50);

        for (int i = 0; i < list.size(); ++i) {
            QPair<String, QColor> pair = list.get(i);

            QTableWidgetItem nameItem = new QTableWidgetItem(pair.first);
            QTableWidgetItem colorItem = new QTableWidgetItem();
            colorItem.setData(Qt.ItemDataRole.DisplayRole, pair.second);

            table.setItem(i, 0, nameItem);
            table.setItem(i, 1, colorItem);
        }
        table.resizeColumnToContents(0);
        table.horizontalHeader().resizeSection(1, 150);

        QGridLayout layout = new QGridLayout();
        layout.addWidget(table, 0, 0);

        setLayout(layout);

        setWindowTitle(tr("Color Editor Factory"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }


    public static void main(String args[]) {
        QApplication.initialize(args);

        ColorEditorFactory window = new ColorEditorFactory();
        window.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
