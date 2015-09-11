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

package org.qtjambi.examples.sql;

import org.qtjambi.qt.sql.QSqlDatabase;
import org.qtjambi.qt.sql.QSqlQuery;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QMessageBox;

//! [0]
class SqlCommon
{
    static boolean createConnection()
    {
        QSqlDatabase db = QSqlDatabase.addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName(":memory:");
        if (!db.open()) {
            QMessageBox.critical(null, QApplication.instance().tr("Cannot open database"),
                QApplication.instance().tr("Unable to establish a database connection.\n" +
                         "This example needs SQLite support. Please read " +
                         "the Qt SQL driver documentation for information how " +
                         "to build it.\n\n" +
                         "Click Cancel to exit."),
                         new QMessageBox.StandardButtons(QMessageBox.StandardButton.Cancel,
                                                         QMessageBox.StandardButton.NoButton));
            return false;
        }

        QSqlQuery query = new QSqlQuery();
        query.exec("create table person (id int primary key, " +
                   "firstname varchar(20), lastname varchar(20))");
        query.exec("insert into person values(101, 'Danny', 'Young')");
        query.exec("insert into person values(102, 'Christine', 'Holand')");
        query.exec("insert into person values(103, 'Lars', 'Gordon')");
        query.exec("insert into person values(104, 'Roberto', 'Robitaille')");
        query.exec("insert into person values(105, 'Maria', 'Papadopoulos')");

        return true;

    }
}
//! [0]
