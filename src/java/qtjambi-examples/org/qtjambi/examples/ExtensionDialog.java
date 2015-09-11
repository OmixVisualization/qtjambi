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

import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QCheckBox;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QDialogButtonBox;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLayout;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample (name = "Extension")
//! [0]
public class ExtensionDialog extends QDialog
{
    private QLabel label;
    private QLineEdit lineEdit;
    private QCheckBox caseCheckBox;
    private QCheckBox fromStartCheckBox;
    private QCheckBox wholeWordsCheckBox;
    private QCheckBox searchSelectionCheckBox;
    private QCheckBox backwardCheckBox;
    private QDialogButtonBox buttonBox;
    private QPushButton findButton;
    private QPushButton moreButton;
    private QWidget extension;
//! [0]

//! [1]
    public ExtensionDialog()
    {
        label = new QLabel(tr("Find &what:"));
        lineEdit = new QLineEdit();
        label.setBuddy(lineEdit);

        caseCheckBox = new QCheckBox(tr("Match &case"));
        fromStartCheckBox = new QCheckBox(tr("Search from &start"));
        fromStartCheckBox.setChecked(true);

        findButton = new QPushButton(tr("&Find"));
        findButton.setDefault(true);

        moreButton = new QPushButton(tr("&More"));
        moreButton.setCheckable(true);
//! [1]
        moreButton.setAutoDefault(false);

        buttonBox = new QDialogButtonBox(Qt.Orientation.Vertical);
        buttonBox.addButton(findButton, QDialogButtonBox.ButtonRole.ActionRole);
        buttonBox.addButton(moreButton, QDialogButtonBox.ButtonRole.ActionRole);

//! [2]
        extension = new QWidget();

        wholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
        backwardCheckBox = new QCheckBox(tr("Search &backward"));
        searchSelectionCheckBox = new QCheckBox(tr("Search se&lection"));
//! [2]

//! [3]
        moreButton.toggled.connect(extension, "setVisible(boolean)");

        QVBoxLayout extensionLayout = new QVBoxLayout();
        extensionLayout.setMargin(0);
        extensionLayout.addWidget(wholeWordsCheckBox);
        extensionLayout.addWidget(backwardCheckBox);
        extensionLayout.addWidget(searchSelectionCheckBox);
        extension.setLayout(extensionLayout);
//! [3]

//! [4]
        QHBoxLayout topLeftLayout = new QHBoxLayout();
        topLeftLayout.addWidget(label);
        topLeftLayout.addWidget(lineEdit);

        QVBoxLayout leftLayout = new QVBoxLayout();
        leftLayout.addLayout(topLeftLayout);
        leftLayout.addWidget(caseCheckBox);
        leftLayout.addWidget(fromStartCheckBox);
        leftLayout.addStretch(1);

        QGridLayout mainLayout = new QGridLayout();
        mainLayout.setSizeConstraint(QLayout.SizeConstraint.SetFixedSize);
        mainLayout.addLayout(leftLayout, 0, 0);
        mainLayout.addWidget(buttonBox, 0, 1);
        mainLayout.addWidget(extension, 1, 0, 1, 2);
        setLayout(mainLayout);

        setWindowTitle(tr("Extension"));
//! [4] //! [5]
        extension.hide();
    }
//! [5]

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new ExtensionDialog().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
