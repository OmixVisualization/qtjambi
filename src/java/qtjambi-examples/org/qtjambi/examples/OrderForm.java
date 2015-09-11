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

import java.util.List;
import java.util.Vector;

import org.qtjambi.qt.QPair;
import org.qtjambi.qt.core.QDate;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QTextCharFormat;
import org.qtjambi.qt.gui.QTextCursor;
import org.qtjambi.qt.gui.QTextFrame;
import org.qtjambi.qt.gui.QTextFrameFormat;
import org.qtjambi.qt.gui.QTextLength;
import org.qtjambi.qt.gui.QTextTable;
import org.qtjambi.qt.gui.QTextTableFormat;
import org.qtjambi.qt.printsupport.QAbstractPrintDialog;
import org.qtjambi.qt.printsupport.QPrintDialog;
import org.qtjambi.qt.printsupport.QPrinter;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QCheckBox;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QDialogButtonBox;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QTabWidget;
import org.qtjambi.qt.widgets.QTableWidget;
import org.qtjambi.qt.widgets.QTableWidgetItem;
import org.qtjambi.qt.widgets.QTextEdit;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Order Form")
public class OrderForm extends QMainWindow
{
//! [0]
    private static class DetailsDialog extends QDialog
    {
        private QLabel nameLabel;
        private QLabel addressLabel;
        private QCheckBox offersCheckBox;
        private QLineEdit nameEdit;
        private List<String> items;
        private QTableWidget itemsTable;
        private QTextEdit addressEdit;
        private QDialogButtonBox buttonBox;
//! [0]

//! [1]
        public DetailsDialog(String title, QWidget parent)
        {
            nameLabel = new QLabel(tr("Name:"));
            addressLabel = new QLabel(tr("Address:"));
            addressLabel.setAlignment(Qt.AlignmentFlag.createQFlags
                        (Qt.AlignmentFlag.AlignLeft, Qt.AlignmentFlag.AlignTop));

            nameEdit = new QLineEdit();
            addressEdit = new QTextEdit();

            offersCheckBox = new QCheckBox(tr("Send information about products and "
                                              + "special offers"));

            setupItemsTable();

            buttonBox = new QDialogButtonBox(new QDialogButtonBox.StandardButtons(
                                             QDialogButtonBox.StandardButton.Ok,
                                             QDialogButtonBox.StandardButton.Cancel));

            buttonBox.accepted.connect(this, "verify()");
            buttonBox.rejected.connect(this, "reject()");
//! [1]

//! [2]
            QGridLayout mainLayout = new QGridLayout();
            mainLayout.addWidget(nameLabel, 0, 0);
            mainLayout.addWidget(nameEdit, 0, 1);
            mainLayout.addWidget(addressLabel, 1, 0);
            mainLayout.addWidget(addressEdit, 1, 1);
            mainLayout.addWidget(itemsTable, 0, 2, 2, 1);
            mainLayout.addWidget(offersCheckBox, 2, 1, 1, 2);
            mainLayout.addWidget(buttonBox, 3, 0, 1, 3);
            setLayout(mainLayout);

            setWindowTitle(title);
        }

//! [3]
        public void verify()
        {
            if (!nameEdit.text().equals("") &&
                !addressEdit.toPlainText().equals("")) {
                accept();
                return;
            }

            QMessageBox.StandardButton answer;
            answer = QMessageBox.warning(this, tr("Incomplete Form"),
                tr("The form does not contain all the necessary information.\n"
                   + "Do you want to discard it?"),
                new QMessageBox.StandardButtons(QMessageBox.StandardButton.Yes,
                                                QMessageBox.StandardButton.No));

            if (answer.equals(QMessageBox.StandardButton.Yes))
                reject();
        }

//! [4]
        public List<QPair<String, Integer>> orderItems()
        {
            List<QPair<String, Integer>> orderList = new Vector<QPair<String, Integer>>();

            for (int row = 0; row < items.size(); ++row) {
                int quantity = Integer.parseInt(
                    (String) itemsTable.item(row, 1).data(Qt.ItemDataRole.DisplayRole));

                orderList.add(new QPair<String, Integer>(itemsTable.item(row, 0).text(),
                              Math.max(0, quantity)));
            }

            return orderList;
        }

//! [5]
        public String senderName()
        {
            return nameEdit.text();
        }

//! [6]
        public String senderAddress()
        {
            return addressEdit.toPlainText();
        }

//! [7]
        public boolean sendOffers()
        {
            return offersCheckBox.isChecked();
        }

//! [8]
        private void setupItemsTable()
        {
            items = new Vector<String>();
            items.add(tr("T-shirt"));
            items.add(tr("Badge"));
            items.add(tr("Reference book"));
            items.add(tr("Coffee cup"));

            itemsTable = new QTableWidget(items.size(), 2);

            for (int row = 0; row < items.size(); ++row) {
                QTableWidgetItem name = new QTableWidgetItem(items.get(row));
                name.setFlags(new Qt.ItemFlags(Qt.ItemFlag.ItemIsEnabled, Qt.ItemFlag.ItemIsSelectable));
                itemsTable.setItem(row, 0, name);
                QTableWidgetItem quantity = new QTableWidgetItem("1");
                itemsTable.setItem(row, 1, quantity);
            }
        }
    }
//! [2] //! [3] //! [4] //! [5] //! [6] //! [7] //! [8]

//! [9]
    public OrderForm() {
        QMenu fileMenu = new QMenu(tr("&File"), this);
        QAction newAction = fileMenu.addAction(tr("&New..."));
        newAction.setShortcut(tr("Ctrl+N"));
        printAction = new QAction(tr("&Print..."), this);
        fileMenu.addAction(printAction);
        printAction.setShortcut(tr("Ctrl+P"));
        printAction.setEnabled(false);
        QAction quitAction = fileMenu.addAction(tr("E&xit"));
        quitAction.setShortcut(tr("Ctrl+Q"));
        menuBar().addMenu(fileMenu);

        letters = new QTabWidget();

        newAction.triggered.connect(this, "openDialog()");
        printAction.triggered.connect(this, "printFile()");
        quitAction.triggered.connect(this, "close()");

        setCentralWidget(letters);
        setWindowTitle(tr("Order Form"));

        createSample();
    }
//! [9]

//! [10]
    public void createSample()
    {
        DetailsDialog dialog =
            new DetailsDialog("Dialog with default values", this);
        createLetter("Mr. Smith", "12 High Street\nSmall Town\nThis country",
                     dialog.orderItems(), true);
    }
//! [10]

//! [11]
    public void openDialog() {
        DetailsDialog dialog =
                new DetailsDialog(tr("Enter Customer Details"), this);

        if (dialog.exec() == QDialog.DialogCode.Accepted.value())
            createLetter(dialog.senderName(), dialog.senderAddress(),
                    dialog.orderItems(), dialog.sendOffers());
    }
//! [11]

//! [12]
    public void printFile() {
        QTextEdit editor = (QTextEdit) letters.currentWidget();
        QPrinter printer = new QPrinter();

        QPrintDialog dialog = new QPrintDialog(printer, this);
        dialog.setWindowTitle(tr("Print Document"));
        if (editor.textCursor().hasSelection())
            dialog.addEnabledOption(
                    QAbstractPrintDialog.PrintDialogOption.PrintSelection);

        if (dialog.exec() != QDialog.DialogCode.Accepted.value())
            return;

        editor.print(printer);
    }
//! [12]

//! [13]
    private void createLetter(String name, String address,
                              List<QPair<String, Integer>> orderItems,
                              boolean sendOffers) {
        QTextEdit editor = new QTextEdit();
        int tabIndex = letters.addTab(editor, name);
        letters.setCurrentIndex(tabIndex);
//! [13]

//! [14]
        QTextCursor cursor = new QTextCursor(editor.textCursor());
        cursor.movePosition(QTextCursor.MoveOperation.Start);
//! [14] //! [15]
        QTextFrame topFrame = cursor.currentFrame();
        QTextFrameFormat topFrameFormat = topFrame.frameFormat();
        topFrameFormat.setPadding(16);
        topFrame.setFrameFormat(topFrameFormat);

        QTextCharFormat textFormat = new QTextCharFormat();
        QTextCharFormat boldFormat = new QTextCharFormat();
        boldFormat.setFontWeight(QFont.Weight.Bold.value());

        QTextFrameFormat referenceFrameFormat = new QTextFrameFormat();
        referenceFrameFormat.setBorder(1);
        referenceFrameFormat.setPadding(8);
        referenceFrameFormat.setPosition(QTextFrameFormat.Position.FloatRight);
        referenceFrameFormat.setWidth(new QTextLength(QTextLength.Type.PercentageLength, 40));
        cursor.insertFrame(referenceFrameFormat);

        cursor.insertText("A company", boldFormat);
        cursor.insertBlock();
        cursor.insertText("321 City Street");
        cursor.insertBlock();
        cursor.insertText("Industry Park");
        cursor.insertBlock();
        cursor.insertText("Another country");
//! [15]

//! [16]
        cursor.setPosition(topFrame.lastPosition());

        cursor.insertText(name, textFormat);

        String[] lines = RetroTranslatorHelper.split(address, "\n");
        for (String line : lines) {
            cursor.insertBlock();
            cursor.insertText(line);
        }
//! [16]

//! [17]
        cursor.insertBlock();
        cursor.insertBlock();

        QDate date = QDate.currentDate();
        cursor.insertText(tr("Date: ") + date.toString("d MMMM yyyy"),
                textFormat);
        cursor.insertBlock();

        QTextFrameFormat bodyFrameFormat = new QTextFrameFormat();
        bodyFrameFormat.setWidth(new QTextLength(QTextLength.Type.PercentageLength, 100));
        cursor.insertFrame(bodyFrameFormat);
//! [17]

//! [18]
        cursor.insertText(tr("I would like to place an order for the following "
                + "items:"), textFormat);
        cursor.insertBlock();
//! [18] //! [19]
        cursor.insertBlock();
//! [19]

//! [20]
        QTextTableFormat orderTableFormat = new QTextTableFormat();
        orderTableFormat.setAlignment(Qt.AlignmentFlag.AlignHCenter);
        QTextTable orderTable = cursor.insertTable(1, 2, orderTableFormat);

        QTextFrameFormat orderFrameFormat = cursor.currentFrame().frameFormat();
        orderFrameFormat.setBorder(1);
        cursor.currentFrame().setFrameFormat(orderFrameFormat);
//! [20]

//! [21]
        cursor = orderTable.cellAt(0, 0).firstCursorPosition();
        cursor.insertText(tr("Product"), boldFormat);
//! [21]
        cursor = orderTable.cellAt(0, 1).firstCursorPosition();
        cursor.insertText(tr("Quantity"), boldFormat);

//! [22]
        for (int i = 0; i < orderItems.size(); ++i) {
            QPair<String, Integer> item = orderItems.get(i);
            int row = orderTable.rows();

            orderTable.insertRows(row, 1);
            cursor = orderTable.cellAt(row, 0).firstCursorPosition();
            cursor.insertText(item.first, textFormat);
            cursor = orderTable.cellAt(row, 1).firstCursorPosition();
            cursor.insertText("" + item.second, textFormat);
        }
//! [22]

//! [23]
        cursor.setPosition(topFrame.lastPosition());

        cursor.insertBlock();
//! [23] //! [24]
        cursor.insertText(tr("Please update my records to take account of the "
                + "following privacy information:"));
        cursor.insertBlock();
//! [24]

//! [25]
        QTextTable offersTable = cursor.insertTable(2, 2);

        cursor = offersTable.cellAt(0, 1).firstCursorPosition();
        cursor.insertText(tr("I want to receive more information about your "
                + "company's products and special offers."), textFormat);
        cursor = offersTable.cellAt(1, 1).firstCursorPosition();
        cursor.insertText(tr("I do not want to receive any promotional information "
                + "from your company."), textFormat);

        if (sendOffers)
            cursor = offersTable.cellAt(0, 0).firstCursorPosition();
        else
            cursor = offersTable.cellAt(1, 0).firstCursorPosition();

        cursor.insertText("X", boldFormat);
//! [25]

//! [26]
        cursor.setPosition(topFrame.lastPosition());
        cursor.insertBlock();
        cursor.insertText(tr("Sincerely,"), textFormat);
        cursor.insertBlock();
        cursor.insertBlock();
        cursor.insertBlock();
        cursor.insertText(name);

        printAction.setEnabled(true);
    }
//! [26]

    @Override
    public QSize sizeHint() {
        return new QSize(500, 550);
    }

    private QAction printAction;
    private QTabWidget letters;

//! [27]
    public static void main(String args[])
    {
        QApplication.initialize(args);

        OrderForm window = new OrderForm();
        window.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [27]
}

