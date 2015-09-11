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

import java.util.LinkedList;
import java.util.List;

import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QHeaderView;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QProgressDialog;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QTableWidget;
import org.qtjambi.qt.widgets.QTableWidgetItem;

@QtJambiExample (name = "Find Files")
public class FindFiles extends QDialog
{
    private QComboBox fileComboBox;
    private QComboBox textComboBox;
    private QComboBox directoryComboBox;
    private QLabel fileLabel;
    private QLabel textLabel;
    private QLabel directoryLabel;
    private QLabel filesFoundLabel;
    private QPushButton browseButton;
    private QPushButton findButton;
    private QTableWidget filesTable;

//! [0]
    public FindFiles()
    {
        browseButton = createButton(tr("&Browse..."), "browse()");
        findButton = createButton(tr("&Find"), "find()");

        fileComboBox = createComboBox(tr("*"));
        textComboBox = createComboBox("");
        directoryComboBox = createComboBox(QDir.currentPath());

        fileLabel = new QLabel(tr("Named:"));
        textLabel = new QLabel(tr("Containing text:"));
        directoryLabel = new QLabel(tr("In directory:"));
        filesFoundLabel = new QLabel();

        createFilesTable();
//! [0]

//! [1]
        QHBoxLayout buttonsLayout = new QHBoxLayout();
        buttonsLayout.addStretch();
        buttonsLayout.addWidget(findButton);

        QGridLayout mainLayout = new QGridLayout();
        mainLayout.addWidget(fileLabel, 0, 0);
        mainLayout.addWidget(fileComboBox, 0, 1, 1, 2);
        mainLayout.addWidget(textLabel, 1, 0);
        mainLayout.addWidget(textComboBox, 1, 1, 1, 2);
        mainLayout.addWidget(directoryLabel, 2, 0);
        mainLayout.addWidget(directoryComboBox, 2, 1);
        mainLayout.addWidget(browseButton, 2, 2);
        mainLayout.addWidget(filesTable, 3, 0, 1, 3);
        mainLayout.addWidget(filesFoundLabel, 4, 0);
        mainLayout.addLayout(buttonsLayout, 5, 0, 1, 3);
        setLayout(mainLayout);

        setWindowTitle(tr("Find Files"));
        resize(700, 300);
    }
//! [1]

//! [2]
    private void browse()
    {
        String directory = QFileDialog.getExistingDirectory(this,
                                   tr("Find Files"), QDir.currentPath());
        if (!directory.equals("")) {
            directoryComboBox.addItem(directory);
            directoryComboBox.setCurrentIndex(directoryComboBox.currentIndex() + 1);
        }
    }
//! [2]

//! [3]
    private void find()
    {
        filesTable.setRowCount(0);

        String fileName = fileComboBox.currentText();
        String text = textComboBox.currentText();
        String path = directoryComboBox.currentText();
//! [3]

//! [4]
        QDir directory = new QDir(path);
        List<String> files = new LinkedList<String>();

        if (fileName.equals(""))
            fileName = "*";

        List<String> entries = new LinkedList<String>();
        entries.add(fileName);

        files = directory.entryList(entries,
            new QDir.Filters(QDir.Filter.Files, QDir.Filter.NoSymLinks));

        if (!text.equals(""))
            files = findFiles(directory, files, text);
        showFiles(directory, files);
    }
//! [4]

//! [5]
    private List<String> findFiles(QDir directory, List<String> files,
                                   String text)
    {
        QProgressDialog progressDialog = new QProgressDialog(this);
        progressDialog.setCancelButtonText(tr("&Cancel"));
        progressDialog.setRange(0, files.size());
        progressDialog.setWindowTitle(tr("Find Files"));

//! [5] //! [6]
        List<String> foundFiles = new LinkedList<String>();

        for (int i = 0; i < files.size(); ++i) {
            progressDialog.setValue(i);
            progressDialog.setLabelText(tr("Searching file number "+i+
                                           " of "+files.size()+"..."));
            QApplication.processEvents();

            if (progressDialog.wasCanceled())
                break;
//! [6]

//! [7]
            QFile file = new QFile(directory.absoluteFilePath(files.get(i)));

            if (file.open(QIODevice.OpenModeFlag.ReadOnly)) {
                String line = "";
                QTextStream in = new QTextStream(file);
                while (!in.atEnd()) {
                    if (progressDialog.wasCanceled())
                        break;
                    line = in.readLine();
                    if (line.contains(text)) {
                        foundFiles.add(files.get(i));
                        break;
                    }
                }
            }
        }
        return foundFiles;
    }
//! [7]

//! [8]
    private void showFiles(QDir directory, List<String> files)
    {
        for (int i = 0; i < files.size(); ++i) {
            QFile file = new QFile(directory.absoluteFilePath(files.get(i)));
            long size = new QFileInfo(file).size();

            QTableWidgetItem fileNameItem = new QTableWidgetItem(files.get(i));
            fileNameItem.setFlags(Qt.ItemFlag.ItemIsEnabled);
            QTableWidgetItem sizeItem =
                new QTableWidgetItem("" + ((size + 1023) / 1024) + tr("KB"));
            sizeItem.setTextAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignRight,
                                                       Qt.AlignmentFlag.AlignVCenter).value());
            sizeItem.setFlags(Qt.ItemFlag.ItemIsEnabled);

            int row = filesTable.rowCount();
            filesTable.insertRow(row);
            filesTable.setItem(row, 0, fileNameItem);
            filesTable.setItem(row, 1, sizeItem);
        }
        filesFoundLabel.setText("" + files.size() + tr("file(s) found."));
    }
//! [8]

//! [9]
    private QPushButton createButton(String text, String goldMember)
    {
        QPushButton button = new QPushButton(text);
        button.clicked.connect(this, goldMember);

        return button;
    }
//! [9]

//! [10]
    private QComboBox createComboBox(String text)
    {
        QComboBox comboBox = new QComboBox();
        comboBox.setEditable(true);
        comboBox.addItem(text);
        comboBox.setSizePolicy(QSizePolicy.Policy.Expanding,
                               QSizePolicy.Policy.Preferred);

        return comboBox;
    }
//! [10]

//! [11]
    private void createFilesTable()
    {
        filesTable = new QTableWidget(0, 2);

        List<String> labels = new LinkedList<String>();
        labels.add(tr("File Name"));
        labels.add(tr("Size"));

        filesTable.setHorizontalHeaderLabels(labels);
        filesTable.horizontalHeader().setSectionResizeMode(0, QHeaderView.ResizeMode.Stretch);
        filesTable.verticalHeader().hide();
        filesTable.setShowGrid(false);
    }
//! [11]

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new FindFiles().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
