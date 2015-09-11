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

import org.qtjambi.qt.QSysInfo;
import org.qtjambi.qt.gui.QCloseEvent;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QCheckBox;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QSystemTrayIcon;
import org.qtjambi.qt.widgets.QTextEdit;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "System Tray Example")
//! [0]
public class SystemTrayExample extends QWidget {

    private QSystemTrayIcon trayIcon;
    private QMenu trayIconMenu;

    private QLineEdit titleEdit;
    private QTextEdit messageEdit;
    private QComboBox typeCombo;

    private QTextEdit infoDisplay;
    private QComboBox iconCombo;
//! [0]

    private QAction toggleVisibilityAction;

//! [1]
    public static void main(String[] args) {
        QApplication.initialize(args);

        SystemTrayExample editor = new SystemTrayExample();
        editor.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    public SystemTrayExample() {
        this(null);
    }

    public SystemTrayExample(QWidget parent) {
        super(parent);
        if (!QSystemTrayIcon.isSystemTrayAvailable())
            QMessageBox.warning(this, tr("System tray is unavailable"),
//! [2] //! [3]
                                      tr("System tray unavailable"));
//! [3]

        // Create the menu that will be used for the context menu
//! [4]
        trayIconMenu = new QMenu(this);
        trayIconMenu.aboutToShow.connect(this, "updateMenu()");
//! [4]

//! [5]
        toggleVisibilityAction = new QAction("Show/Hide", this);
        toggleVisibilityAction.triggered.connect(this, "toggleVisibility()");
        trayIconMenu.addAction(toggleVisibilityAction);

        QAction restoreAction = new QAction("Restore", this);
        restoreAction.triggered.connect(this, "showNormal()");
        trayIconMenu.addAction(restoreAction);

        QAction minimizeAction = new QAction("Minimize", this);
        minimizeAction.triggered.connect(this, "showMinimized()");
        trayIconMenu.addAction(minimizeAction);

        QAction maximizeAction = new QAction("Maximize", this);
        maximizeAction.triggered.connect(this, "showMaximized()");
        trayIconMenu.addAction(maximizeAction);

        trayIconMenu.addSeparator();

        QAction quitAction = new QAction("&Quit", this);
        quitAction.triggered.connect(this, "close()");
        trayIconMenu.addAction(quitAction);
//! [5]

        // Create the tray icon
//! [6]
        trayIcon = new QSystemTrayIcon(this);
        trayIcon.setToolTip("System trayIcon example");
        trayIcon.setContextMenu(trayIconMenu);

        trayIcon.activated.connect(this, "activated(org.qtjambi.qt.gui.QSystemTrayIcon$ActivationReason)");
        trayIcon.messageClicked.connect(this, "balloonClicked()");

        changeIcon(0);
        trayIcon.show();
//! [6]

//! [7]
        QLabel titleLabel = new QLabel(tr("Message Title"));
        titleEdit = new QLineEdit(tr("Message Title"));

        QLabel messageLabel = new QLabel(tr("Message Contents"));
        messageEdit = new QTextEdit(tr("Man is more ape than many of the apes"));
        messageEdit.setAcceptRichText(false);

        QLabel typeLabel = new QLabel(tr("Message Type"));
        typeCombo = new QComboBox();
        Vector<String> types = new Vector<String>();
        types.add("NoIcon");
        types.add("Information");
        types.add("Warning");
        types.add("Critical");
        typeCombo.addItems(types);
        typeCombo.setCurrentIndex(2);

        QPushButton balloonButton = new QPushButton(tr("Balloon message"));
        balloonButton.setToolTip(tr("Click here to balloon the message"));
        balloonButton.clicked.connect(this, "showMessage()");

        infoDisplay = new QTextEdit(tr("Status messages will be visible here"));
        infoDisplay.setMaximumHeight(100);

        QCheckBox toggleIconCheckBox = new QCheckBox(tr("Show system tray icon"));
        toggleIconCheckBox.setChecked(true);
        toggleIconCheckBox.clicked.connect(trayIcon, "setVisible(boolean)");

        QLabel iconLabel = new QLabel("Select icon");
        iconCombo = new QComboBox();
        Vector<String> icons = new Vector<String>();
        icons.add("16x16 icon");
        icons.add("22x22 icon");
        icons.add("32x32 icon");
        iconCombo.addItems(icons);
        iconCombo.activatedIndex.connect(this, "changeIcon(int)");
//! [7]

//! [8]
        QGridLayout layout = new QGridLayout();
        layout.addWidget(titleLabel, 0, 0);
        layout.addWidget(titleEdit, 0, 1);
        layout.addWidget(messageLabel, 1, 0);
        layout.addWidget(messageEdit, 1, 1);
        layout.addWidget(typeLabel, 2, 0);
        layout.addWidget(typeCombo, 2, 1);
        layout.addWidget(balloonButton, 4, 1);
        layout.addWidget(infoDisplay, 5, 0, 1, 2);
        layout.addWidget(toggleIconCheckBox, 6, 0);
        layout.addWidget(iconLabel, 7, 0);
        layout.addWidget(iconCombo, 7, 1);
        setLayout(layout);

        setWindowTitle(tr("System Tray Example"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }
//! [8]

    @Override
//! [9]
    public void closeEvent(QCloseEvent e) {

    }
//! [9]

//! [10]
    protected void updateMenu() {
        toggleVisibilityAction.setText(isVisible() ? tr("Hide") : tr("Show"));
    }
//! [10]

//! [11]
    protected void toggleVisibility() {
        if (isVisible())
            hide();
        else
            show();
    }
//! [11]

//! [12]
    protected void showMessage() {
//! [12]
        // #ifdef Q_WS_MAC
//! [13]
        if (QSysInfo.macVersion() != 0) {
            QMessageBox.information(this, tr("System tray example"),
                    tr("Balloon tips are not supported on Mac OS X"));
        } else {
//! [13] //! [14]
            QSystemTrayIcon.MessageIcon icon;
            icon = QSystemTrayIcon.MessageIcon.resolve(typeCombo.currentIndex());
            trayIcon.showMessage(titleEdit.text(), messageEdit.toPlainText(),
                                 icon, 10000);
            trayIcon.setToolTip(titleEdit.text());
        }
//! [14] //! [15]
    }
//! [15]

//! [16]
    protected void balloonClicked() {
        infoDisplay.append(tr("Balloon message was clicked"));
    }
//! [16]

//! [17]
    public void activated(QSystemTrayIcon.ActivationReason reason) {
        String name = QSystemTrayIcon.MessageIcon.resolve(reason.value()).name();
        if (name != null)
            infoDisplay.append("Activated - Reason " + name);
    }
//! [17]

//! [18]
    protected void changeIcon(int index) {
        String iconName;
        switch (index) {
        default:
        case 0:
            iconName = "classpath:org/qtjambi/examples/images/icon_16x16.png";
            break;

        case 1:
            iconName = "classpath:org/qtjambi/examples/images/icon_22x22.png";
            break;

        case 2:
            iconName = "classpath:org/qtjambi/examples/images/icon_32x32.png";
            break;
        }
//! [18] //! [19]
        QPixmap pixmap = new QPixmap(iconName);
        trayIcon.setIcon(new QIcon(pixmap));
    }
//! [19]

//! [20]
}
//! [20]
