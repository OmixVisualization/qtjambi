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

import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QMainWindow;

@QtJambiExample(name = "Qt Designer and JUIC")
//! [0]
public class SimpleUIExample extends QMainWindow {

//! [1]
    public static void main(String[] args) {
        QApplication.initialize(args);
        SimpleUIExample mainw = new SimpleUIExample();
        mainw.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

    Ui_SimpleUIExampleMainWindow mainWindowUi = new Ui_SimpleUIExampleMainWindow();
//! [0]

//! [2]
    public SimpleUIExample() {
        // Place what you made in Designer onto the main window.
        mainWindowUi.setupUi(this);
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        // Connect the OpenDialog button to the showDialog method.
        mainWindowUi.pushButton_OpenDialog.clicked.connect(this, "showDialog()");
    }
//! [2]

//! [3]
    private void showDialog() {
        // Make the dialog.
        Ui_SimpleUIExample dialogUi = new Ui_SimpleUIExample();
        QDialog dialog = new QDialog(this);
        dialogUi.setupUi(dialog);
//! [3]

//! [4]
        String result = "";
        if (dialog.exec() == QDialog.DialogCode.Accepted.value()) {
            result += "Name: " + dialogUi.lineEdit_Name.text() + "\n";
            result += "E-Mail: " + dialogUi.lineEdit_Email.text() + "\n";
            // Get rest of dialog information here

        } else {
            result = "Cancelled by user.";
        }

        mainWindowUi.textBrowser.setText(result);
//! [4] //! [5]
    }
//! [5] //! [6]
}
//! [6]
