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

import java.util.Vector;

import org.qtjambi.demos.Interpreter;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QInputDialog;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QTextBrowser;

@QtJambiExample(name = "Advanced Calculator")
//! [0]
public class AdvancedCalculator extends QMainWindow {

    public static void main(String[] args) {
        QApplication.initialize(args);
        AdvancedCalculator calculator = new AdvancedCalculator();
        calculator.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }

    private QLineEdit lineEdit;
    private QTextBrowser textBrowser;

    private Interpreter interpreter = new Interpreter();
//! [0]

//! [1]
    public AdvancedCalculator() {
        Vector<String> uiTypes = new Vector<String>(3);
        uiTypes.add("Simple");
        uiTypes.add("Normal");
        uiTypes.add("Dockable");
//! [1]

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

//! [2]
        String item = QInputDialog.getItem(this, tr("Ui selector"), tr("Ui configurations:"), uiTypes, 0, false);
//! [2]

//! [3]
        if (item == null || item.equals("Simple")) {
            Ui_CalculatorSimple uiSimple = new Ui_CalculatorSimple();
            uiSimple.setupUi(this);
            lineEdit = uiSimple.lineEdit;
            textBrowser = uiSimple.textBrowser;
        } else if (item.equals("Normal")) {
//! [3] //! [4]
            Ui_CalculatorNormal uiNormal = new Ui_CalculatorNormal();
            uiNormal.setupUi(this);
            lineEdit = uiNormal.lineEdit;
            textBrowser = uiNormal.textBrowser;
        } else if (item.equals("Dockable")) {
//! [4] //! [5]
            Ui_CalculatorDockable uiDockable = new Ui_CalculatorDockable();
            uiDockable.setupUi(this);
            lineEdit = uiDockable.lineEdit;
            textBrowser = uiDockable.textBrowser;
        }
//! [5]
    }

//! [6]
    public void on_button_equal_clicked() {
        String expression = lineEdit.text();
        String result = "";
        boolean error = false;
        try {
            result = interpreter.evaluate(interpreter.parse(expression)).toString();
        } catch (Interpreter.ParseException exception) {
            result = "Error: <font color=\"red\">" + exception.getMessage() + "</font>";
            error = true;
        }

        textBrowser.append(expression + "<b> = " + result + "</b><br>");
        if (error)
            result = expression;
        lineEdit.setText(result);
    }
//! [6]

    public void type(String s) {
        lineEdit.setText(lineEdit.text() + s);
    }

    public void typeAround(String s) {
        lineEdit.setText(s + "(" + lineEdit.text() + ")");
    }

    public void on_button_1_clicked() {
        type("1");
    }

    public void on_button_2_clicked() {
        type("2");
    }

    public void on_button_3_clicked() {
        type("3");
    }

    public void on_button_4_clicked() {
        type("4");
    }

    public void on_button_5_clicked() {
        type("5");
    }

    public void on_button_6_clicked() {
        type("6");
    }

    public void on_button_7_clicked() {
        type("7");
    }

    public void on_button_8_clicked() {
        type("8");
    }

    public void on_button_9_clicked() {
        type("9");
    }

    public void on_button_0_clicked() {
        type("0");
    }

    public void on_button_add_clicked() {
        type("+");
    }

    public void on_button_subtract_clicked() {
        type("-");
    }

    public void on_button_multiply_clicked() {
        type("*");
    }

    public void on_button_devide_clicked() {
        type("/");
    }

    public void on_button_comma_clicked() {
        type(".");
    }

    public void on_button_left_clicked() {
        type("(");
    }

    public void on_button_right_clicked() {
        type(")");
    }

    public void on_button_sin_clicked() {
        typeAround("sin");
    }

    public void on_button_cos_clicked() {
        typeAround("cos");
    }

    public void on_button_random_clicked() {
        type("random()");
    }

    public void on_button_functions_clicked() {
        Vector<String> functionKeys = new Vector<String>(interpreter.functions.size());
        functionKeys.addAll(interpreter.functions.keySet());

        String key = QInputDialog.getItem(this, tr("Function selector"), tr("Available functions:"), functionKeys, 0, false);
        if (key != null)
            type(key);
    }
//! [7]
}
//! [7]
