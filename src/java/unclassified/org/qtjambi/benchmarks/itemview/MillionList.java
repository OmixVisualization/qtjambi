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

package org.qtjambi.benchmarks.itemview;

import javax.swing.*;

import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

public class MillionList {

    public static final int ITEM_COUNT = 100000;

    public static class QtList extends QListView {
        public long populateTime;
        public long setModelTime;

        private QStandardItemModel model = new QStandardItemModel(ITEM_COUNT, 1);
        // private QStringListModel model = new QStringListModel();

        public QtList() {
            populate();
        }

        private void populate() {
            long t1 = System.currentTimeMillis();

//          List<String> strings = new LinkedList<String>();

            for (int i=0; i<ITEM_COUNT; ++i) {
                // strings.add(String.valueOf(i));
                QModelIndex index = model.index(i, 0);

//                System.out.printf("row=%d, col=%d\n", index.row(), index.column());
//
//
//
//                System.out.println(" - internalId=" + index.internalId());
//                System.out.println(" - model=" + index.model());

                model.setData(index, i);
            }


            populateTime = System.currentTimeMillis() - t1;

            System.out.println("Population took: " + populateTime);

            setModel(model);

            setModelTime = System.currentTimeMillis() - populateTime - t1;
            System.out.println("Setting the model: " + setModelTime);
        }
    }

    private static class FakeModelIndex {
        int row, col;
        Object data;

        public FakeModelIndex(int row, int col) {
            this.row = row;
            this.col = col;
        }
    }

    public static class SwingList extends JList {
        private static final long serialVersionUID = 1L;
        public long populateTime;
        public long setModelTime;

        private DefaultListModel model = new DefaultListModel();

        public SwingList() {
            populate();
        }

        private void populate() {
            long t1 = System.currentTimeMillis();


            for (int i=0; i<ITEM_COUNT; ++i) {
//              FakeModelIndex index = new FakeModelIndex(i, 0);
                model.add(i, i);
            }

            populateTime = System.currentTimeMillis() - t1;

            System.out.println("Population took: " + populateTime);

            setModel(model);

            setModelTime = System.currentTimeMillis() - populateTime - t1;

            System.out.println("Setting model took: " + setModelTime);
        }
    }

    /**
     * @param args
     */
    public static void main(String[] args) {

        QApplication.initialize(args);

        if (args.length >= 1 && (args[0].contains("java") || args[0].contains("swing"))) {
            // run swing test
            SwingList l = new SwingList();
            JFrame f = new JFrame();
            f.getContentPane().add(l);
            f.pack();
            f.setVisible(true);
        } else {
            QtList l = new QtList();
            l.show();
        }

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
