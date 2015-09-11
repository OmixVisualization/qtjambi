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

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.sql.QSqlDatabase;
import org.qtjambi.qt.sql.QSqlTableModel;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QTableView;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Cached Table",
                canInstantiate = "call-static-method:checkSqlLite")
//! [0]
public class CachedTable extends QDialog {
    private QPushButton submitButton = null;
    private QPushButton revertButton = null;
    private QPushButton quitButton = null;
    private QSqlTableModel model = null;
//! [0]

    public static void main(String[] args) {
        QApplication.initialize(args);

        if (!checkSqlLite()) {
            System.out.println("Missing support for SQL");
            return;
        }

        CachedTable table = new CachedTable(null);
        table.show();
        table.exec();
    }

//! [1]
    public CachedTable(QWidget parent)
    {
        super(parent);

        if (!SqlCommon.createConnection())
            throw new RuntimeException("Couldn't connect to SQLITE server");


//! [1]
        String tableName = "person";

//! [2]
        model = new QSqlTableModel(this);
        model.setTable(tableName);
        model.setEditStrategy(QSqlTableModel.EditStrategy.OnManualSubmit);
        model.select();

        model.setHeaderData(0, Qt.Orientation.Horizontal, tr("ID"));
        model.setHeaderData(1, Qt.Orientation.Horizontal, tr("First name"));
        model.setHeaderData(2, Qt.Orientation.Horizontal, tr("Last name"));

//! [2] //! [3]
        QTableView view = new QTableView();
        view.setModel(model);
//! [3]

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

//! [4]
        submitButton = new QPushButton(tr("Submit"));
        submitButton.setDefault(true);
        revertButton = new QPushButton(tr("&Revert"));
        quitButton = new QPushButton(tr("Quit"));

        submitButton.clicked.connect(this, "submit()");
        revertButton.clicked.connect(model, "revertAll()");
        quitButton.clicked.connect(this, "close()");

//! [4] //! [5]
        QVBoxLayout buttonLayout = new QVBoxLayout();
        buttonLayout.addWidget(submitButton);
        buttonLayout.addWidget(revertButton);
        buttonLayout.addWidget(quitButton);
        buttonLayout.addStretch(1);

        QHBoxLayout mainLayout = new QHBoxLayout();
        mainLayout.addWidget(view);
        mainLayout.addLayout(buttonLayout);
        setLayout(mainLayout);

        setWindowTitle(tr("Cached Table"));
    }
//! [5]

//! [6]
    protected void submit()
    {
        model.database().transaction();
        if (model.submitAll()) {
            model.database().commit();
        } else {
            model.database().rollback();
            QMessageBox.warning(this, tr("Cached Table"),
//! [6] //! [7]
                                tr("The database reported an error: ") + model.lastError().text());
        }
//! [7] //! [8]
    }
//! [8]

//! [9]
    public static boolean checkSqlLite() {
        return QSqlDatabase.isDriverAvailable("QSQLITE");
    }
//! [9]
}
