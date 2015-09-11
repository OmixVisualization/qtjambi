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
import org.qtjambi.qt.gui.QDoubleValidator;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QIntValidator;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Line Edits")
//! [0]
public class LineEdits extends QWidget {

    private QLineEdit echoLineEdit;
    private QLineEdit validatorLineEdit;
    private QLineEdit alignmentLineEdit;
    private QLineEdit inputMaskLineEdit;
    private QLineEdit accessLineEdit;
//! [0]

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);

//! [2]
        LineEdits lineedits = new LineEdits();
        lineedits.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

    public LineEdits() {
        this(null);
    }

    public LineEdits(QWidget parent) {
        super(parent);
        QGroupBox echoGroup = new QGroupBox(tr("Echo"));

        QLabel echoLabel = new QLabel(tr("Mode:"));
        QComboBox echoComboBox = new QComboBox();
        echoComboBox.addItem(tr("Normal"));
        echoComboBox.addItem(tr("Password"));
        echoComboBox.addItem(tr("No Echo"));

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        echoLineEdit = new QLineEdit();
        echoLineEdit.setFocus();
//! [2]

//! [3]
        QGroupBox validatorGroup = new QGroupBox(tr("Validator"));

        QLabel validatorLabel = new QLabel(tr("Type:"));
        QComboBox validatorComboBox = new QComboBox();
        validatorComboBox.addItem(tr("No validator"));
        validatorComboBox.addItem(tr("Integer validator"));
        validatorComboBox.addItem(tr("Double validator"));

        validatorLineEdit = new QLineEdit();
//! [3]

//! [4]
        QGroupBox alignmentGroup = new QGroupBox(tr("Alignment"));

        QLabel alignmentLabel = new QLabel(tr("Type:"));
        QComboBox alignmentComboBox = new QComboBox();
        alignmentComboBox.addItem(tr("Left"));
        alignmentComboBox.addItem(tr("Centered"));
        alignmentComboBox.addItem(tr("Right"));

        alignmentLineEdit = new QLineEdit();
//! [4]

//! [5]
        QGroupBox inputMaskGroup = new QGroupBox(tr("Input mask"));

        QLabel inputMaskLabel = new QLabel(tr("Type:"));
        QComboBox inputMaskComboBox = new QComboBox();
        inputMaskComboBox.addItem(tr("No mask"));
        inputMaskComboBox.addItem(tr("Phone number"));
        inputMaskComboBox.addItem(tr("ISO date"));
        inputMaskComboBox.addItem(tr("License key"));

        inputMaskLineEdit = new QLineEdit();
//! [5]

//! [6]
        QGroupBox accessGroup = new QGroupBox(tr("Access"));

        QLabel accessLabel = new QLabel(tr("Read-only:"));
        QComboBox accessComboBox = new QComboBox();
        accessComboBox.addItem(tr("False"));
        accessComboBox.addItem(tr("True"));

        accessLineEdit = new QLineEdit();
//! [6]

//! [7]
        echoComboBox.activatedIndex.connect(this, "echoChanged(int)");
        validatorComboBox.activatedIndex.connect(this, "validatorChanged(int)");
        alignmentComboBox.activatedIndex.connect(this, "alignmentChanged(int)");
        inputMaskComboBox.activatedIndex.connect(this, "inputMaskChanged(int)");
        accessComboBox.activatedIndex.connect(this, "accessChanged(int)");
//! [7]

//! [8]
        QGridLayout echoLayout = new QGridLayout();
        echoLayout.addWidget(echoLabel, 0, 0);
        echoLayout.addWidget(echoComboBox, 0, 1);
        echoLayout.addWidget(echoLineEdit, 1, 0, 1, 2);
        echoGroup.setLayout(echoLayout);
//! [8]

//! [9]
        QGridLayout validatorLayout = new QGridLayout();
        validatorLayout.addWidget(validatorLabel, 0, 0);
        validatorLayout.addWidget(validatorComboBox, 0, 1);
        validatorLayout.addWidget(validatorLineEdit, 1, 0, 1, 2);
        validatorGroup.setLayout(validatorLayout);

        QGridLayout alignmentLayout = new QGridLayout();
        alignmentLayout.addWidget(alignmentLabel, 0, 0);
        alignmentLayout.addWidget(alignmentComboBox, 0, 1);
        alignmentLayout.addWidget(alignmentLineEdit, 1, 0, 1, 2);
        alignmentGroup.setLayout(alignmentLayout);

        QGridLayout inputMaskLayout = new QGridLayout();
        inputMaskLayout.addWidget(inputMaskLabel, 0, 0);
        inputMaskLayout.addWidget(inputMaskComboBox, 0, 1);
        inputMaskLayout.addWidget(inputMaskLineEdit, 1, 0, 1, 2);
        inputMaskGroup.setLayout(inputMaskLayout);

        QGridLayout accessLayout = new QGridLayout();
        accessLayout.addWidget(accessLabel, 0, 0);
        accessLayout.addWidget(accessComboBox, 0, 1);
        accessLayout.addWidget(accessLineEdit, 1, 0, 1, 2);
        accessGroup.setLayout(accessLayout);
//! [9]

//! [10]
        QGridLayout layout = new QGridLayout();
        layout.addWidget(echoGroup, 0, 0);
        layout.addWidget(validatorGroup, 1, 0);
        layout.addWidget(alignmentGroup, 2, 0);
        layout.addWidget(inputMaskGroup, 0, 1);
        layout.addWidget(accessGroup, 1, 1);
        setLayout(layout);

        setWindowTitle(tr("Line Edits"));
    }
//! [10]

//! [11]
    public void echoChanged(int index) {
        switch (index) {
        case 0:
            echoLineEdit.setEchoMode(QLineEdit.EchoMode.Normal);
            break;
        case 1:
            echoLineEdit.setEchoMode(QLineEdit.EchoMode.Password);
            break;
        case 2:
            echoLineEdit.setEchoMode(QLineEdit.EchoMode.NoEcho);
        }
//! [11] //! [12]
    }
//! [12]

//! [13]
    public void validatorChanged(int index) {
        switch (index) {
        case 0:
            validatorLineEdit.setValidator(null);
            break;
        case 1:
            validatorLineEdit.setValidator(new QIntValidator(validatorLineEdit));
            break;
        case 2:
            validatorLineEdit.setValidator(new QDoubleValidator(-999.0, 999.0, 2, validatorLineEdit));
        }
//! [13] //! [14]

        validatorLineEdit.setText("");
    }
//! [14]

//! [15]
    public void alignmentChanged(int index) {
        switch (index) {
        case 0:
            alignmentLineEdit.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignLeft));
            break;
        case 1:
            alignmentLineEdit.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignCenter));
            break;
        case 2:
            alignmentLineEdit.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignRight));
        }
//! [15] //! [16]
    }
//! [16]

//! [17]
    public void inputMaskChanged(int index) {
        switch (index) {
        case 0:
            inputMaskLineEdit.setInputMask("");
            break;
        case 1:
            inputMaskLineEdit.setInputMask("+99 99 99 99 99;_");
            break;
        case 2:
            inputMaskLineEdit.setInputMask("0000-00-00");
            inputMaskLineEdit.setText("00000000");
            inputMaskLineEdit.setCursorPosition(0);
            break;
        case 3:
            inputMaskLineEdit.setInputMask(">AAAAA-AAAAA-AAAAA-AAAAA-AAAAA;#");
        }
//! [17] //! [18]
    }
//! [18]

//! [19]
    public void accessChanged(int index) {
        switch (index) {
        case 0:
            accessLineEdit.setReadOnly(false);
            break;
        case 1:
            accessLineEdit.setReadOnly(true);
        }
//! [19] //! [20]
    }
//! [20] //! [21]
}
//! [21]
