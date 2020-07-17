/****************************************************************************
 **
 ** (C) 2007-2009 Nokia. All rights reserved.
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

package io.qt.sql.jdbc;

import io.qt.sql.QSqlDatabase;
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

        QJdbc.initialize();

        try {
            Class.forName("com.mysql.jdbc.Driver").newInstance();
        } catch (Exception ex) { System.err.println(ex); return; }

        QSqlDatabase db = QSqlDatabase.addDatabase(QJdbc.ID);
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
public final class QJdbc extends QSqlDriverPlugin{
	
	private QJdbc(){initialize();}

    /** The id string that should be used in calls to
     * QSqlDatabase.addDatabase() when setting up a new database
     * connection.
     */
    public static final String ID = "QJDBC";

    /** Sets up the QJdbc Database driver plugin. This function must
     * be called before the QJdbc driver plugin can be used.
     */
    public static void initialize() {
        if (creator == null) {
            creator = new QSqlDriverCreator<>(QJdbcSqlDriver::new);
            QSqlDatabase.registerSqlDriver(ID, creator);
        }
    }
    
    private static QSqlDriverCreator<QJdbcSqlDriver> creator;
    
    @Override
	public QSqlDriver create(String key) {
		if(QJdbc.ID.equals(key)) {
			return creator.createObject();
		}
		return null;
	}
}