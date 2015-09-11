/****************************************************************************
 **
 **  (C) 1992-2009 Nokia. All rights reserved.
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

import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.Qt.AlignmentFlag;
import org.qtjambi.qt.core.Qt.AspectRatioMode;
import org.qtjambi.qt.core.Qt.TransformationMode;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QCheckBox;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QSpinBox;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Screenshot")
//! [0]
public class Screenshot extends QWidget {
//! [0]

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);
        Screenshot screenshot = new Screenshot(null);
        screenshot.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    volatile QPixmap originalPixmap;

    QLabel screenshotLabel;
    QGroupBox optionsGroupBox;
    QSpinBox delaySpinBox;
    QLabel delaySpinBoxLabel;
    QCheckBox hideThisWindowCheckBox;
    QPushButton newScreenshotButton;
    QPushButton saveScreenshotButton;
    QPushButton quitScreenshotButton;

    QVBoxLayout mainLayout;
    QGridLayout optionsGroupBoxLayout;
    QHBoxLayout buttonsLayout;

    public Screenshot(QWidget parent) {
        super(parent);
        screenshotLabel = new QLabel();
        screenshotLabel.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding);
        screenshotLabel.setAlignment(AlignmentFlag.AlignCenter);
        screenshotLabel.setMinimumSize(240, 160);

        createOptionsGroupBox();
        createButtonsLayout();

        mainLayout = new QVBoxLayout();
        mainLayout.addWidget(screenshotLabel);
        mainLayout.addWidget(optionsGroupBox);
        mainLayout.addLayout(buttonsLayout);
        setLayout(mainLayout);

        shootScreen();
        delaySpinBox.setValue(5);

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        setWindowTitle(tr("Screenshot"));
        resize(300, 200);
    }
//! [2]

    @Override
//! [3]
    public void resizeEvent(QResizeEvent event) {
        QSize scaledSize = originalPixmap.size();
        scaledSize.scale(screenshotLabel.size(),
                         AspectRatioMode.KeepAspectRatio);
        if (screenshotLabel.pixmap() != null
            || scaledSize != screenshotLabel.pixmap().size())
                updateScreenshotLabel();
    }
//! [3]

//! [4]
    void newScreenshot() {
        if (hideThisWindowCheckBox.isChecked())
            hide();
        newScreenshotButton.setDisabled(true);

        QTimer.singleShot(delaySpinBox.value() * 1000,
                          this, "shootScreen()");
    }
//! [4]

//! [5]
    void saveScreenshot() {
        String format = "png";
        String initialPath = QDir.currentPath() + tr("/untitled.") + format;
        String filter = String.format(tr("%1$s Files (*.%2$s);;All Files (*)"),
                                      format.toUpperCase(), format);
        String fileName;
        fileName = QFileDialog.getSaveFileName(this, tr("Save As"), initialPath,
                                               new QFileDialog.Filter(filter));

        if (!fileName.equals(""))
            originalPixmap.save(fileName, format);
    }
//! [5]

//! [6]
    void shootScreen() {
        if (delaySpinBox.value() != 0)
            QApplication.beep();
//! [6]

//! [7]
        originalPixmap = null;

        originalPixmap = QApplication.desktop().grab();
        updateScreenshotLabel();

        newScreenshotButton.setDisabled(false);
        if (hideThisWindowCheckBox.isChecked())
            show();
    }
//! [7]

//! [8]
    void updateCheckBox() {
        if (delaySpinBox.value() == 0)
            hideThisWindowCheckBox.setDisabled(true);
        else
            hideThisWindowCheckBox.setDisabled(false);
    }
//! [8]

//! [9]
    void createOptionsGroupBox() {
        optionsGroupBox = new QGroupBox(tr("Options"));

        delaySpinBox = new QSpinBox();
        delaySpinBox.setSuffix(tr(" s"));
        delaySpinBox.setMaximum(60);
        delaySpinBox.valueChanged.connect(this, "updateCheckBox()");

        delaySpinBoxLabel = new QLabel(tr("Screenshot Delay:"));

        hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"));

        optionsGroupBoxLayout = new QGridLayout();
        optionsGroupBoxLayout.addWidget(delaySpinBoxLabel, 0, 0);
        optionsGroupBoxLayout.addWidget(delaySpinBox, 0, 1);
        optionsGroupBoxLayout.addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);
        optionsGroupBox.setLayout(optionsGroupBoxLayout);
    }
//! [9]

//! [10]
    void createButtonsLayout() {
        newScreenshotButton = createButton(tr("New Screenshot"), this,
                                           "newScreenshot()");

        saveScreenshotButton = createButton(tr("Save Screenshot"), this,
                                            "saveScreenshot()");

        quitScreenshotButton = createButton(tr("Quit"), this, "close()");

        buttonsLayout = new QHBoxLayout();
        buttonsLayout.addStretch();
        buttonsLayout.addWidget(newScreenshotButton);
        buttonsLayout.addWidget(saveScreenshotButton);
        buttonsLayout.addWidget(quitScreenshotButton);
    }
//! [10]

//! [11]
    QPushButton createButton(final String text, QWidget receiver,
                             String member) {
        QPushButton button = new QPushButton(text);
        button.clicked.connect(receiver, member);
        return button;
    }
//! [11]

//! [12]
    void updateScreenshotLabel() {
        screenshotLabel.setPixmap(originalPixmap.scaled(screenshotLabel.size(),
                                  AspectRatioMode.KeepAspectRatio,
                                  TransformationMode.SmoothTransformation));
    }
//! [12] //! [13]
}
//! [13]
