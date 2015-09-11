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

import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLayout;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QToolButton;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Calculator")
public class Calculator extends QWidget
{
    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    String pendingAdditiveOperator = "";
    String pendingMultiplicativeOperator = "";
    boolean waitingForOperand;

    QLineEdit display;

    public static int NumDigitButtons = 10;
    Button digitButtons[] = new Button[NumDigitButtons];

    public Calculator()
    {
        sumInMemory = 0.0;
        sumSoFar = 0.0;
        factorSoFar = 0.0;
        waitingForOperand = true;

        display = new QLineEdit("0");
        display.setReadOnly(true);
        display.setAlignment(Qt.AlignmentFlag.AlignRight);
        display.setMaxLength(15);

        QFont font = display.font();
        font.setPointSize(font.pointSize() + 8);
        display.setFont(font);

        QColor digitColor = new QColor(150, 205, 205);
        QColor backspaceColor = new QColor(225, 185, 135);
        QColor memoryColor = new QColor(100, 155, 155);
        QColor operatorColor = new QColor(155, 175, 195);

        for (int i = 0; i < NumDigitButtons; ++i) {
            digitButtons[i] = createButton(String.valueOf(i), digitColor,
                                           "digitClicked()");
        }

        Button pointButton = createButton(tr("."), digitColor, "pointClicked()");
        Button changeSignButton = createButton(tr("\261"), digitColor, "changeSignClicked()");

        Button backspaceButton = createButton(tr("Backspace"), backspaceColor,
                                       "backspaceClicked()");
        Button clearButton = createButton(tr("Clear"), backspaceColor, "clear()");
        Button clearAllButton = createButton(tr("Clear All"), backspaceColor.lighter(120),
                                      "clearAll()");

        Button clearMemoryButton = createButton(tr("MC"), memoryColor,
                                         "clearMemory()");
        Button readMemoryButton = createButton(tr("MR"), memoryColor, "readMemory()");
        Button setMemoryButton = createButton(tr("MS"), memoryColor, "setMemory()");
        Button addToMemoryButton = createButton(tr("M+"), memoryColor, "addToMemory()");

        Button divisionButton = createButton(tr("\367"), operatorColor,
                                      "multiplicativeOperatorClicked()");
        Button timesButton = createButton(tr("\327"), operatorColor,
                                   "multiplicativeOperatorClicked()");
        Button minusButton = createButton(tr("-"), operatorColor,
                                   "additiveOperatorClicked()");
        Button plusButton = createButton(tr("+"), operatorColor,
                                  "additiveOperatorClicked()");

        Button squareRootButton = createButton(tr("Sqrt"), operatorColor,
                                        "unaryOperatorClicked()");
        Button powerButton = createButton(tr("x\262"), operatorColor,
                                   "unaryOperatorClicked()");
        Button reciprocalButton = createButton(tr("1/x"), operatorColor,
                                        "unaryOperatorClicked()");
        Button equalButton = createButton(tr("="), operatorColor.lighter(120),
                                   "equalClicked()");

        QGridLayout mainLayout = new QGridLayout();
        mainLayout.setSizeConstraint(QLayout.SizeConstraint.SetFixedSize);

        mainLayout.addWidget(display, 0, 0, 1, 6);
        mainLayout.addWidget(backspaceButton, 1, 0, 1, 2);
        mainLayout.addWidget(clearButton, 1, 2, 1, 2);
        mainLayout.addWidget(clearAllButton, 1, 4, 1, 2);

        mainLayout.addWidget(clearMemoryButton, 2, 0);
        mainLayout.addWidget(readMemoryButton, 3, 0);
        mainLayout.addWidget(setMemoryButton, 4, 0);
        mainLayout.addWidget(addToMemoryButton, 5, 0);

        for (int i = 1; i < NumDigitButtons; ++i) {
            int row = ((9 - i) / 3) + 2;
            int column = ((i - 1) % 3) + 1;
            mainLayout.addWidget(digitButtons[i], row, column);
        }

        mainLayout.addWidget(digitButtons[0], 5, 1);
        mainLayout.addWidget(pointButton, 5, 2);
        mainLayout.addWidget(changeSignButton, 5, 3);

        mainLayout.addWidget(divisionButton, 2, 4);
        mainLayout.addWidget(timesButton, 3, 4);
        mainLayout.addWidget(minusButton, 4, 4);
        mainLayout.addWidget(plusButton, 5, 4);

        mainLayout.addWidget(squareRootButton, 2, 5);
        mainLayout.addWidget(powerButton, 3, 5);
        mainLayout.addWidget(reciprocalButton, 4, 5);
        mainLayout.addWidget(equalButton, 5, 5);
        setLayout(mainLayout);

        setWindowTitle(tr("Calculator"));
    }

    public void digitClicked()
    {
        Button clickedButton = (Button) QSignalEmitter.signalSender();
        int digitValue = Integer.parseInt(clickedButton.text());
        if (display.text().equals("0") && digitValue == 0.0)
            return;

        if (waitingForOperand) {
            display.clear();
            waitingForOperand = false;
        }
        display.setText(display.text() + String.valueOf(digitValue));
    }

    public void unaryOperatorClicked()
    {
        Button clickedButton = (Button) QSignalEmitter.signalSender();
        String clickedOperator = clickedButton.text();
        double operand = parseDouble(display.text());
        double result = 0.0;

        if (clickedOperator.equals(tr("Sqrt"))) {
            if (operand < 0.0) {
                abortOperation();
                return;
            }
            result = Math.sqrt(operand);
        } else if (clickedOperator.equals(tr("x\262"))) {
            result = Math.pow(operand, 2.0);
        } else if (clickedOperator.equals(tr("1/x"))) {
            if (operand == 0.0) {
                abortOperation();
                return;
            }
            result = 1.0 / operand;
        }
        display.setText(String.valueOf(result));
        waitingForOperand = true;
    }

    public void additiveOperatorClicked()
    {
        Button clickedButton = (Button) QSignalEmitter.signalSender();
        String clickedOperator = clickedButton.text();
        double operand = parseDouble(display.text());

        if (pendingMultiplicativeOperator.length() != 0) {
            if (!calculate(operand, pendingMultiplicativeOperator)) {
                abortOperation();
                return;
            }
            display.setText(String.valueOf(factorSoFar));
            operand = factorSoFar;
            factorSoFar = 0.0;
            pendingMultiplicativeOperator = "";
        }

        if (pendingAdditiveOperator.length() != 0) {
            if (!calculate(operand, pendingAdditiveOperator)) {
                abortOperation();
                return;
            }
            display.setText(String.valueOf(sumSoFar));
        } else {
            sumSoFar = operand;
        }

        pendingAdditiveOperator = clickedOperator;
        waitingForOperand = true;
    }

    public void multiplicativeOperatorClicked()
    {
        Button clickedButton = (Button) QSignalEmitter.signalSender();
        String clickedOperator = clickedButton.text();
        double operand = parseDouble(display.text());

        if (pendingMultiplicativeOperator.length() != 0) {
            if (!calculate(operand, pendingMultiplicativeOperator)) {
                abortOperation();
                return;
            }
            display.setText(String.valueOf(factorSoFar));
        } else {
            factorSoFar = operand;
        }

        pendingMultiplicativeOperator = clickedOperator;
        waitingForOperand = true;
    }

    private double parseDouble(String str)
    {
        double ret;
        try {
            ret = Double.parseDouble(str);
        } catch (NumberFormatException e) {
            ret = 0.0;
        }
        return ret;
    }

    public void equalClicked()
    {
        double operand = parseDouble(display.text());

        if (pendingMultiplicativeOperator.length() != 0) {
            if (!calculate(operand, pendingMultiplicativeOperator)) {
                abortOperation();
                return;
            }
            operand = factorSoFar;
            factorSoFar = 0.0;
            pendingMultiplicativeOperator = "";
        }
        if (pendingAdditiveOperator.length() != 0) {
            if (!calculate(operand, pendingAdditiveOperator)) {
                abortOperation();
                return;
            }
            pendingAdditiveOperator = "";
        } else {
            sumSoFar = operand;
        }

        display.setText(String.valueOf(sumSoFar));
        sumSoFar = 0.0;
        waitingForOperand = true;
    }

    public void pointClicked()
    {
        if (waitingForOperand)
            display.setText("0");
        if (!display.text().contains("."))
            display.setText(display.text() + tr("."));
        waitingForOperand = false;
    }

    public void changeSignClicked()
    {
        String text = display.text();
        double value = parseDouble(text);

        if (value > 0.0) {
            text = tr("-") + text;
        } else if (value < 0.0) {
            text.substring(1);
        }
        display.setText(text);
    }

    public void backspaceClicked()
    {
        if (waitingForOperand)
            return;

        String text = display.text();
        text = text.substring(0, text.length() - 1);
        if (text.length() == 0) {
            text = "0";
            waitingForOperand = true;
        }
        display.setText(text);
    }

    public void clear()
    {
        if (waitingForOperand)
            return;

        display.setText("0");
        waitingForOperand = true;
    }

    public void clearAll()
    {
        sumSoFar = 0.0;
        factorSoFar = 0.0;
        pendingAdditiveOperator = "";
        pendingMultiplicativeOperator = "";
        display.setText("0");
        waitingForOperand = true;
    }

    public void clearMemory()
    {
        sumInMemory = 0.0;
    }

    public void readMemory()
    {
        display.setText(String.valueOf(sumInMemory));
        waitingForOperand = true;
    }

    public void setMemory()
    {
        equalClicked();
        sumInMemory = parseDouble(display.text());
    }

    public void addToMemory()
    {
        equalClicked();
        sumInMemory += parseDouble(display.text());
    }

    private Button createButton(String text,  QColor color,
                                String slot)
    {
        Button button = new Button(text, color);
        button.clicked.connect(this, slot);
        return button;
    }

    public void abortOperation()
    {
        clearAll();
        display.setText(tr("####"));
    }

    public boolean calculate(double rightOperand, String pendingOperator)
    {
        if (pendingOperator.equals(tr("+"))) {
            sumSoFar += rightOperand;
        } else if (pendingOperator.equals(tr("-"))) {
            sumSoFar -= rightOperand;
        } else if (pendingOperator.equals(tr("\327"))) {
            factorSoFar *= rightOperand;
        } else if (pendingOperator.equals(tr("\367"))) {
            if (rightOperand == 0.0)
                return false;
            factorSoFar /= rightOperand;
        }
        return true;
    }

    class Button extends QToolButton
    {
        public Button(String text, QColor color)
        {
            setSizePolicy(QSizePolicy.Policy.Expanding,
                          QSizePolicy.Policy.Preferred);
            setText(text);

            QPalette newPalette = palette();
            newPalette.setColor(QPalette.ColorRole.Button, color);
            setPalette(newPalette);
        }

        @Override
        public QSize sizeHint()
        {
            QSize size = super.sizeHint();
            size.setHeight(size.height() + 20);
            size.setWidth(Math.max(size.width(), size.height()));
            return size;
        }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new Calculator().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
