/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

package io.qt.sql.jdbc;

import io.qt.sql.QSqlDriver;
import io.qt.sql.QSqlDriverCreator;
import io.qt.sql.QSqlDriverPlugin;

/**
 * The QJdbc class is responsible for implementing a Qt database
 * plugin based on a JDBC database driver. In addition to making use
 * of the QJdbc database driver an application also needs a JDBC
 * driver.
 *
 * Below you find an example use of the QJdbc database driver in
 * combination with a mysql JDBC driver and opens the output in
 * two separate QTableView's.

 <pre>
    public static void main(String args[])
    {
        QApplication.initialize(args);

        try {
            Class.forName("com.mysql.jdbc.Driver").newInstance();
        } catch (Exception ex) { System.err.println(ex); return; }

        QSqlDatabase db = QSqlDatabase.addDatabase("QJDBC");
        db.setDatabaseName("jdbc:mysql://myhostname/mydatabase");
        db.setUserName("myusername");
        db.setPassword("mypassword");
        if (db.open()) {
            System.out.println("Connected!");
        } else {
            System.out.println("Connection Failed!");
            System.out.println(db.lastError().text());
            return;
        }

        QSqlTableModel model = new QSqlTableModel(null, db);
        model.setTable("mytablename");
        if (!model.select()) {
            System.err.println(model.lastError().text());
        }
        QTableView view = new QTableView();
        view.setModel(model);
        view.show();

        QTableView view2 = new QTableView();
        view2.setModel(model);
        view2.show();

        QApplication.execStatic();
        QApplication.shutdown();

        db.close();
    }
 </pre>

 */
final class QJdbc extends QSqlDriverPlugin{
	
	private QJdbc(){
		super();
	}

    private QSqlDriverCreator<QJdbcSqlDriver> creator = new QSqlDriverCreator<>(QJdbcSqlDriver::new);
    
    @Override
	public QSqlDriver create(String key) {
		if("QJDBC".equals(key)) {
			return creator.createObject();
		}
		return null;
	}
}
