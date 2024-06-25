/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.autotests;

import java.io.IOException;
import java.util.Set;
import java.util.TreeSet;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.QNoNativeResourcesException;
import io.qt.QtPrimitiveType;
import io.qt.core.QAbstractListModel;
import io.qt.core.QAbstractTableModel;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.core.Qt.Orientation;
import io.qt.gui.QStandardItemModel;
import io.qt.widgets.QApplication;
import io.qt.widgets.QTableView;

public class TestItemModel extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void testHeaderDataCalledWithRoles() throws InterruptedException, IOException {
    	Set<Integer> roles = new TreeSet<>();
    	QAbstractTableModel model = new QAbstractTableModel() {
			
			@Override
			public int rowCount(QModelIndex parent) {
				return 3;
			}
			
			@Override
			public int columnCount(QModelIndex parent) {
				return 3;
			}
			
			@Override
			public Object data(QModelIndex index, int role) {
			    //qWarning("TableModel::data({%1$s,%2$s},%3$s(%4$s))", index.row(), index.column(), roleNames().get(role).toString(), role);
				return null;
			}

			@Override
			public Object headerData(int section, Orientation orientation, int role) {
				Object result = super.headerData(section, orientation, role);
//				qWarning("TableModel::headerData(%1$s,%2$s,%3$s(%4$s)) = %5$s", section, orientation, roleNames().get(role).toString(), role, result);
				roles.add(role);
				return result;
			}
			
			public final Signal3<io.qt.core.@NonNull QModelIndex, java.lang.@QtPrimitiveType@NonNull Integer, java.lang.@QtPrimitiveType@NonNull Integer> publicRowsInserted = new Signal3<>();
			{
				System.out.println("checkConnectArgs: "+QMetaObject.checkConnectArgs(QMetaMethod.fromSignal(publicRowsInserted), QMetaMethod.fromSignal(rowsInserted)));
				QMetaObject.Connection conn = QObject.connect(this, QMetaMethod.fromSignal(publicRowsInserted), this, QMetaMethod.fromSignal(rowsInserted), Qt.ConnectionType.DirectConnection);
				System.out.println(conn+" "+conn.isConnected());
				conn = this.rowsInserted.connect((a,b,c)->System.out.println(a+" "+b+" "+c));
				System.out.println(conn+" "+conn.isConnected());
				publicRowsInserted.emit(createIndex(0, 0), 0, 0);
			}
		};
		QTableView view = new QTableView();
		view.setModel(model);
		view.show();
		QTimer.singleShot(500, QApplication::quit);
		QApplication.exec();
//		QTimer.singleShot(15000, QApplication::quit);
//		QApplication.exec();
//		QApplication.processEvents();
		view.close();
		view.dispose();
		Assert.assertTrue("headerData called with role: "+roles, roles.size()>1);
    }
    
    @Test
    public void testSignalDefaultArgs() {
    	QAbstractListModel listModel = new QAbstractListModel() {
			@Override
			public int rowCount(@NonNull QModelIndex parent) {
				return 0;
			}
			
			@Override
			public Object data(@NonNull QModelIndex index, int role) {
				return null;
			}
		};
		listModel.dataChanged.connect((a,b,c)->{});
		listModel.dataChanged.emit(listModel.index(0, 0), listModel.index(0, 0));
    }
    
    @Test
    public void testIndexDangledModel() {
    	class Model extends QStandardItemModel {
			public QModelIndex createIndex() {
				return super.createIndex(0, 0);
			}
    	}
    	Model model = new Model();
    	QModelIndex idx = model.createIndex();
		Assert.assertEquals(model, idx.model());
		model.dispose();
		try {
			idx.model();
		} catch (QNoNativeResourcesException e) {
			Assert.assertEquals("Dependent object has been deleted.", e.getMessage());
		}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestItemModel.class.getName());
    }
}
