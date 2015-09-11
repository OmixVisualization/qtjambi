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

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QTranslator;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QCloseEvent;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QAbstractButton;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QCheckBox;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QDialogButtonBox;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QListWidget;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QRadioButton;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "I18N",
                canInstantiate = "call-static-method:notWebstart")
public class I18N extends QDialog {

    private class MainWindow extends QMainWindow {
        private QWidget centralWidget;
        private QGroupBox groupBox;
        private QListWidget listWidget;
        private QRadioButton perspectiveRadioButton;
        private QRadioButton isometricRadioButton;
        private QRadioButton obliqueRadioButton;
        private QMenu fileMenu;
        private QAction exitAction;

        public Signal1<Boolean> visible = new Signal1<Boolean>();

        public MainWindow(QWidget parent) {
            super(parent);
            centralWidget = new QWidget();
            setCentralWidget(centralWidget);

            createGroupBox();

            listWidget = new QListWidget();
            listWidget.addItem(tr("First"));
            listWidget.addItem(tr("Second"));
            listWidget.addItem(tr("Third"));

            QVBoxLayout mainLayout = new QVBoxLayout();
            mainLayout.addWidget(groupBox);
            mainLayout.addWidget(listWidget);
            centralWidget.setLayout(mainLayout);

            exitAction = new QAction(tr("E&xit"), this);
            exitAction.triggered.connect(this, "close()");

            fileMenu = menuBar().addMenu(tr("&File"));
            fileMenu.addAction(exitAction);

//![0]
            setWindowTitle(String.format(tr("Language: %1$s"), tr("English")));
//![0]

            setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

            statusBar().showMessage(tr("Internationalization Example"));

            if (tr("LTR").equals("RTL"))
                setLayoutDirection(Qt.LayoutDirection.RightToLeft);
        }

        private void createGroupBox() {
            groupBox = new QGroupBox(tr("View"));
            perspectiveRadioButton = new QRadioButton(tr("Perspective"));
            isometricRadioButton = new QRadioButton(tr("Isometric"));
            obliqueRadioButton = new QRadioButton(tr("Oblique"));
            perspectiveRadioButton.setChecked(true);

            QVBoxLayout groupBoxLayout = new QVBoxLayout();
            groupBoxLayout.addWidget(perspectiveRadioButton);
            groupBoxLayout.addWidget(isometricRadioButton);
            groupBoxLayout.addWidget(obliqueRadioButton);
            groupBox.setLayout(groupBoxLayout);
        }

        @Override
        protected void closeEvent(QCloseEvent event) {
            visible.emit(false);
        }
    }

    public I18N() {
        this(null);
    }

    public I18N(QWidget parent) {
        super(parent, new Qt.WindowFlags(Qt.WindowType.WindowStaysOnTopHint));
        groupBox = new QGroupBox("Languages");

        QGridLayout groupBoxLayout = new QGridLayout();

        List<String> qmFiles = findQmFiles();
        for (int i = 0; i < qmFiles.size(); ++i) {
            QCheckBox checkBox = new QCheckBox(languageName(qmFiles.get(i)));
            qmFileForCheckBoxMap.put(checkBox, qmFiles.get(i));
            checkBox.toggled.connect(this, "checkBoxToggled()");
            groupBoxLayout.addWidget(checkBox, i / 2, i % 2);
        }
        groupBox.setLayout(groupBoxLayout);

        buttonBox = new QDialogButtonBox();

        showAllButton = buttonBox.addButton("Show All", QDialogButtonBox.ButtonRole.ActionRole);
        hideAllButton = buttonBox.addButton("Hide All", QDialogButtonBox.ButtonRole.ActionRole);

        showAllButton.clicked.connect(this, "showAll()");
        hideAllButton.clicked.connect(this, "hideAll()");

        QVBoxLayout mainLayout = new QVBoxLayout();
        mainLayout.addWidget(groupBox);
        mainLayout.addWidget(buttonBox);
        setLayout(mainLayout);
        setWindowTitle("I18N");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    @Override
    public void closeEvent(QCloseEvent event) {
        hideAll();
    }

    protected void checkBoxToggled() {
        QCheckBox checkBox = (QCheckBox) signalSender();

        MainWindow window = mainWindowForCheckBoxMap.get(checkBox);
        if (window == null) {
//[1]
            QTranslator translator = new QTranslator(this);
            translator.load("classpath:org/qtjambi/examples/translation/" + qmFileForCheckBoxMap.get(checkBox));
            QApplication.installTranslator(translator);
//![1]
            window = new MainWindow(this);
            window.visible.connect(checkBox, "setChecked(boolean)");

            window.installEventFilter(this);
            mainWindowForCheckBoxMap.put(checkBox, window);

            QApplication.removeTranslator(translator);
        }
        window.setVisible(checkBox.isChecked());
    }

    protected void showAll() {
        for (QCheckBox checkBox : qmFileForCheckBoxMap.keySet())
            checkBox.setChecked(true);
    }

    protected void hideAll() {
        for (QCheckBox checkBox : qmFileForCheckBoxMap.keySet())
            checkBox.setChecked(false);
    }

    private List<String> findQmFiles() {
        QDir dir = new QDir("classpath:org/qtjambi/examples/translation");
        List<String> filter = new Vector<String>();
        filter.add("*.qm");
        List<String> fileNames = dir.entryList(filter, new QDir.Filters(QDir.Filter.Files), QDir.SortFlag.Name);

        return fileNames;
    }

    private String languageName(final String qmFile) {
        QTranslator translator = new QTranslator(this);
        translator.load("classpath:org/qtjambi/examples/translation/" + qmFile);
        return translator.translate("org.qtjambi.examples.I18N", "English");
    }

    public static boolean notWebstart() {
        return System.getProperty("org.qtjambi.launcher.webstart") == null;
    }

    private QGroupBox groupBox;
    private QDialogButtonBox buttonBox;
    private QAbstractButton showAllButton;
    private QAbstractButton hideAllButton;
    private Map<QCheckBox, String> qmFileForCheckBoxMap = new HashMap<QCheckBox, String>();
    private Map<QCheckBox, MainWindow> mainWindowForCheckBoxMap = new HashMap<QCheckBox, MainWindow>();

    public static void main(String args[]) {
        QApplication.initialize(args);
        I18N i18n = new I18N();
        i18n.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
}

