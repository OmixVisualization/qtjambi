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

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.lang.reflect.Field;

import org.junit.Test;

import org.qtjambi.qt.core.QAbstractItemModel;
import org.qtjambi.qt.core.QModelIndex;

public class TestPrivateSignals extends QApplicationTest {
    private static class DummyModel extends QAbstractItemModel {
        public int columnCount(QModelIndex parent) {
            return 1;
        }
        public int rowCount(QModelIndex parent) {
            return 1;
        }
        public Object data(QModelIndex index, int role) {
            return null;
        }
        public QModelIndex index(int row, int column, QModelIndex parent) {
            return createIndex(row, column);
        }
        public QModelIndex parent(QModelIndex child) {
            return null;
        }
        public void doReset() {
        	beginResetModel();
        	endResetModel();
        }
    }

    private boolean triggered_modelReset;
    private boolean triggered_modelAboutToBeReset;

    public void handle_modelReset() {
        triggered_modelReset = true & triggered_modelAboutToBeReset;
    }
    public void handle_modelAboutToBeReset() {
        triggered_modelAboutToBeReset = true;
    }

    @Test
    public void test_emit_PrivateSignal0() {
        DummyModel model = new DummyModel();

        triggered_modelReset = false;
        triggered_modelAboutToBeReset = false;

        model.doReset();

        assertFalse(triggered_modelReset);
        assertFalse(triggered_modelAboutToBeReset);

        model.modelReset.connect(this, "handle_modelReset()");
        model.modelAboutToBeReset.connect(this, "handle_modelAboutToBeReset()");

        model.doReset();

        assertTrue(triggered_modelAboutToBeReset);
        assertTrue(triggered_modelReset);
    }

    @Test
    public void test_privateSignal_is_PrivateSignal() {
        Field signal = null;
        try {
            signal = QAbstractItemModel.class.getField("modelReset");
        } catch (Exception e) { }

        assertTrue(signal != null);
        assertEquals(signal.getType().getName(), "org.qtjambi.qt.QSignalEmitter$PrivateSignal0");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPrivateSignals.class.getName());
    }
}
