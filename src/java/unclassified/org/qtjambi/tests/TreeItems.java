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

package org.qtjambi.tests;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


class Item extends QStandardItem {
    public Item() {
        disableGarbageCollection();
    }

    @Override
    public Object data(int role) {
        if (role == Qt.ItemDataRole.DisplayRole)
            return "Item " + id;
        if (role == Qt.ItemDataRole.DecorationRole)
            return pixmap;
        return null;
    }

    public void setIcon(QPixmap p) {
        pixmap = p;
    }

    private static int idCounter = 0;
    private int id = ++idCounter;
    private QPixmap pixmap;
}


public class TreeItems {

    public static QStandardItemModel setupModel() {
        QStandardItemModel model = new QStandardItemModel();

        Item r1 = new Item();
        Item r2 = new Item();
        Item r3 = new Item();

        Item s11 = new Item();
        Item s12 = new Item();
        Item s21 = new Item();

        r1.appendRow(s11);
        r1.appendRow(s12);
        r2.appendRow(s21);

        model.appendRow(r1);
        model.appendRow(r2);
        model.appendRow(r3);

        return model;
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        QTreeView view = new QTreeView();
        view.show();

        view.setModel(setupModel());

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
