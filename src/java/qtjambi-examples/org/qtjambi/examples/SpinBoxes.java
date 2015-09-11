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

import org.qtjambi.qt.core.QDate;
import org.qtjambi.qt.core.QDateTime;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QDateEdit;
import org.qtjambi.qt.widgets.QDateTimeEdit;
import org.qtjambi.qt.widgets.QDoubleSpinBox;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QSpinBox;
import org.qtjambi.qt.widgets.QTimeEdit;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Spin Boxes")
//! [0]
public class SpinBoxes extends QWidget {
    private QDateTimeEdit meetingEdit;
    private QDoubleSpinBox doubleSpinBox;
    private QDoubleSpinBox priceSpinBox;
    private QDoubleSpinBox scaleSpinBox;
    private QGroupBox spinBoxesGroup;
    private QGroupBox editsGroup;
    private QGroupBox doubleSpinBoxesGroup;
    private QLabel meetingLabel;
//! [0]


//! [1]
    public SpinBoxes() {
        this(null);
    }
//! [1]

    public SpinBoxes(QWidget parent) {
        super(parent);
//! [2]
        createSpinBoxes();
        createDateTimeEdits();
        createDoubleSpinBoxes();

        QHBoxLayout layout = new QHBoxLayout();
        layout.addWidget(spinBoxesGroup);
        layout.addWidget(editsGroup);
        layout.addWidget(doubleSpinBoxesGroup);
        setLayout(layout);

        setWindowTitle(tr("Spin Boxes"));

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    public void createSpinBoxes() {
        spinBoxesGroup = new QGroupBox(tr("Spinboxes"));

        QLabel integerLabel = new QLabel(String.format(tr("Enter a value between %1$d and %2$d:"), -20, 20));
        QSpinBox integerSpinBox = new QSpinBox();
        integerSpinBox.setRange(-20, 20);
        integerSpinBox.setSingleStep(1);
        integerSpinBox.setValue(0);
//! [2]

//! [3]
        QLabel zoomLabel = new QLabel(String.format(tr("Enter a zoom value between %1$d and %2$d:"), 0, 1000));
        QSpinBox zoomSpinBox = new QSpinBox();
        zoomSpinBox.setRange(0, 1000);
        zoomSpinBox.setSingleStep(10);
        zoomSpinBox.setSuffix("%");
        zoomSpinBox.setSpecialValueText(tr("Automatic"));
        zoomSpinBox.setValue(100);
//! [3]

//! [4]
        QLabel priceLabel = new QLabel(String.format(tr("Enter a price between %1$d and %2$d:"), 0, 999));
        QSpinBox priceSpinBox = new QSpinBox();
        priceSpinBox.setRange(0, 999);
        priceSpinBox.setSingleStep(1);
        priceSpinBox.setPrefix("$");
        priceSpinBox.setValue(99);
//! [4] //! [5]

        QVBoxLayout spinBoxLayout = new QVBoxLayout();
        spinBoxLayout.addWidget(integerLabel);
        spinBoxLayout.addWidget(integerSpinBox);
        spinBoxLayout.addWidget(zoomLabel);
        spinBoxLayout.addWidget(zoomSpinBox);
        spinBoxLayout.addWidget(priceLabel);
        spinBoxLayout.addWidget(priceSpinBox);
        spinBoxesGroup.setLayout(spinBoxLayout);
    }
//! [5]

//! [6]
    public void createDateTimeEdits() {
        editsGroup = new QGroupBox(tr("Date and time spin boxes"));

        QLabel dateLabel = new QLabel();
        QDateEdit dateEdit = new QDateEdit(QDate.currentDate());
        dateEdit.setDateRange(new QDate(2005, 1, 1), new QDate(2010, 12, 31));
        dateLabel.setText(String.format(tr("Appointment date (between %1$s and %2$s):"),
                                        dateEdit.minimumDate().toString(Qt.DateFormat.ISODate),
                                        dateEdit.maximumDate().toString(Qt.DateFormat.ISODate)));
//! [6]

//! [7]
        QLabel timeLabel = new QLabel();
        QTimeEdit timeEdit = new QTimeEdit(QTime.currentTime());
        timeEdit.setTimeRange(new QTime(9, 0, 0, 0), new QTime(16, 30, 0, 0));
        timeLabel.setText(String.format(tr("Appointment time (between %1$s and %2$s):"),
                                        timeEdit.minimumTime().toString(Qt.DateFormat.ISODate),
                                        timeEdit.maximumTime().toString(Qt.DateFormat.ISODate)));
//! [7]

//! [8]
        meetingLabel = new QLabel();
        meetingEdit = new QDateTimeEdit(QDateTime.currentDateTime());
//! [8]

//! [9]
        QLabel formatLabel = new QLabel(tr("Format string for the meeting date and time:"));
        QComboBox formatComboBox = new QComboBox();
        formatComboBox.addItem("yyyy-MM-dd hh:mm:ss (zzz 'ms')");
        formatComboBox.addItem("hh:mm:ss MM/dd/yyyy");
        formatComboBox.addItem("hh:mm:ss dd/MM/yyyy");
        formatComboBox.addItem("hh:mm:ss");
        formatComboBox.addItem("hh:mm ap");
//! [9] //! [10]

        formatComboBox.activated.connect(this, "setFormatString(String)");
//! [10]

        setFormatString(formatComboBox.currentText());

//! [11]
        QVBoxLayout editsLayout = new QVBoxLayout();
        editsLayout.addWidget(dateLabel);
        editsLayout.addWidget(dateEdit);
        editsLayout.addWidget(timeLabel);
        editsLayout.addWidget(timeEdit);
        editsLayout.addWidget(meetingLabel);
        editsLayout.addWidget(meetingEdit);
        editsLayout.addWidget(formatLabel);
        editsLayout.addWidget(formatComboBox);
        editsGroup.setLayout(editsLayout);
    }
//! [11]

//! [12]
    public void setFormatString(String formatString) {
        meetingEdit.setDisplayFormat(formatString);
//! [12] //! [13]
        if (meetingEdit.displayedSections().isSet(QDateTimeEdit.Section.DateSections_Mask)) {
            meetingEdit.setDateRange(new QDate(2004, 11, 1), new QDate(2005, 11, 30));
            meetingLabel.setText(String.format(tr("Meeting date (between %1$s and %2$s):"),
                                               meetingEdit.minimumDate().toString(Qt.DateFormat.ISODate), meetingEdit.maximumDate().toString(Qt.DateFormat.ISODate)));
        } else {
            meetingEdit.setTimeRange(new QTime(0, 7, 20, 0), new QTime(21, 0, 0, 0));
            meetingLabel.setText(String.format(tr("Meeting time (between %1$s and %2$s):"), meetingEdit.minimumTime().toString(Qt.DateFormat.ISODate), meetingEdit.maximumTime().toString(Qt.DateFormat.ISODate)));
//! [13] //! [14]
        }
//! [14] //! [15]
    }
//! [15]

//! [16]
    public void createDoubleSpinBoxes() {
        doubleSpinBoxesGroup = new QGroupBox(tr("Double precision spinboxes"));

        QLabel precisionLabel = new QLabel(tr("Number of decimal places to show:"));
        QSpinBox precisionSpinBox = new QSpinBox();
        precisionSpinBox.setRange(0, 13);
        precisionSpinBox.setValue(2);
//! [16]

//! [17]
        QLabel doubleLabel = new QLabel(String.format(tr("Enter a value between %1$d and %2$d:"), -20, 20));
        doubleSpinBox = new QDoubleSpinBox();
        doubleSpinBox.setRange(-20.0, 20.0);
        doubleSpinBox.setSingleStep(1.0);
        doubleSpinBox.setValue(0.0);
//! [17]

//! [18]
        QLabel scaleLabel = new QLabel(String.format(tr("Enter a scale factor between %1$d and %2$d:"), 0, 1000));
        scaleSpinBox = new QDoubleSpinBox();
        scaleSpinBox.setRange(0.0, 1000.0);
        scaleSpinBox.setSingleStep(10.0);
        scaleSpinBox.setSuffix("%");
        scaleSpinBox.setSpecialValueText(tr("No scaling"));
        scaleSpinBox.setValue(100.0);
//! [18]

//! [19]
        QLabel priceLabel = new QLabel(String.format(tr("Enter a price between %1$d and %2$d:"), 0, 1000));
        priceSpinBox = new QDoubleSpinBox();
        priceSpinBox.setRange(0.0, 1000.0);
        priceSpinBox.setSingleStep(1.0);
        priceSpinBox.setPrefix("$");
        priceSpinBox.setValue(99.99);

        precisionSpinBox.valueChanged.connect(this, "changePrecision(int)");
//! [19]

//! [20]
        QVBoxLayout spinBoxLayout = new QVBoxLayout();
        spinBoxLayout.addWidget(precisionLabel);
        spinBoxLayout.addWidget(precisionSpinBox);
        spinBoxLayout.addWidget(doubleLabel);
        spinBoxLayout.addWidget(doubleSpinBox);
        spinBoxLayout.addWidget(scaleLabel);
        spinBoxLayout.addWidget(scaleSpinBox);
        spinBoxLayout.addWidget(priceLabel);
        spinBoxLayout.addWidget(priceSpinBox);
        doubleSpinBoxesGroup.setLayout(spinBoxLayout);
    }
//! [20]

//! [21]
    public void changePrecision(int decimals) {
        doubleSpinBox.setDecimals(decimals);
        scaleSpinBox.setDecimals(decimals);
        priceSpinBox.setDecimals(decimals);
    }
//! [21]

//! [22]
    public static void main(String args[]) {
        QApplication.initialize(args);

        SpinBoxes spinBoxes = new SpinBoxes();
        spinBoxes.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [22] //! [23]
}
//! [23]
