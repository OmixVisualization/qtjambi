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
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt.AlignmentFlag;
import org.qtjambi.qt.core.Qt.ItemFlag;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QCheckBox;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QDateTimeEdit;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QListView;
import org.qtjambi.qt.widgets.QListWidget;
import org.qtjambi.qt.widgets.QListWidgetItem;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QSpinBox;
import org.qtjambi.qt.widgets.QStackedWidget;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Config Dialog")
public class ConfigDialog extends QDialog {

    private QListWidget contentsWidget;
    private QStackedWidget pagesWidget;

    public static void main(String args[]) {
        QApplication.initialize(args);

        ConfigDialog dialog = new ConfigDialog(null);
        dialog.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    public ConfigDialog(QWidget parent) {
        super(parent);
        contentsWidget = new QListWidget(this);
        contentsWidget.setViewMode(QListView.ViewMode.IconMode);
        contentsWidget.setIconSize(new QSize(96, 84));
        contentsWidget.setMovement(QListView.Movement.Static);
        contentsWidget.setMaximumWidth(128);
        contentsWidget.setSpacing(12);

        pagesWidget = new QStackedWidget(this);
        pagesWidget.addWidget(new ConfigurationPage(this));
        pagesWidget.addWidget(new UpdatePage(this));
        pagesWidget.addWidget(new QueryPage(this));

        QPushButton closeButton = new QPushButton(tr("Close"));

        createIcons();
        contentsWidget.setCurrentRow(0);

        closeButton.clicked.connect(this, "close()");

        QHBoxLayout horizontalLayout = new QHBoxLayout();
        horizontalLayout.addWidget(contentsWidget);
        horizontalLayout.addWidget(pagesWidget, 1);

        QHBoxLayout buttonsLayout = new QHBoxLayout();
        buttonsLayout.addStretch(1);
        buttonsLayout.addWidget(closeButton);

        QVBoxLayout mainLayout = new QVBoxLayout();
        mainLayout.addLayout(horizontalLayout);
        mainLayout.addStretch(1);
        mainLayout.addSpacing(12);
        mainLayout.addLayout(buttonsLayout);
        setLayout(mainLayout);

        setWindowTitle(tr("Config Dialog"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    private void createIcons() {
        QListWidgetItem configButton = new QListWidgetItem(contentsWidget);
        configButton.setIcon(new QIcon("classpath:org/qtjambi/examples/images/config.png"));
        configButton.setText(tr("Configuration"));
        configButton.setTextAlignment(AlignmentFlag.AlignHCenter.value());
        configButton
                .setFlags(ItemFlag.ItemIsSelectable, ItemFlag.ItemIsEnabled);

        QListWidgetItem updateButton = new QListWidgetItem(contentsWidget);
        updateButton.setIcon(new QIcon("classpath:org/qtjambi/examples/images/update.png"));
        updateButton.setText(tr("Update"));
        updateButton.setTextAlignment(AlignmentFlag.AlignHCenter.value());
        updateButton
                .setFlags(ItemFlag.ItemIsSelectable, ItemFlag.ItemIsEnabled);

        QListWidgetItem queryButton = new QListWidgetItem(contentsWidget);
        queryButton.setIcon(new QIcon("classpath:org/qtjambi/examples/images/query.png"));
        queryButton.setText(tr("Query"));
        queryButton.setTextAlignment(AlignmentFlag.AlignHCenter.value());
        queryButton.setFlags(ItemFlag.ItemIsSelectable, ItemFlag.ItemIsEnabled);

        contentsWidget.currentItemChanged.connect(this,
                "changePage(QListWidgetItem , QListWidgetItem)");

    }

    protected void changePage(QListWidgetItem current, QListWidgetItem previous) {
        pagesWidget.setCurrentIndex(contentsWidget.row(current));
    }

    private class ConfigurationPage extends QWidget {

        public ConfigurationPage(QWidget parent) {
            super(parent);
            QGroupBox configGroup = new QGroupBox(tr("Server configuration"));

            QLabel serverLabel = new QLabel(tr("Server:"));
            QComboBox serverCombo = new QComboBox();
            serverCombo.addItem(tr("Qt Software (Australia)"));
            serverCombo.addItem(tr("Qt Software (Germany)"));
            serverCombo.addItem(tr("Qt Software (Norway)"));
            serverCombo.addItem(tr("Qt Software (People's Republic of China)"));
            serverCombo.addItem(tr("Qt Software (USA)"));

            QHBoxLayout serverLayout = new QHBoxLayout();
            serverLayout.addWidget(serverLabel);
            serverLayout.addWidget(serverCombo);

            QVBoxLayout configLayout = new QVBoxLayout();
            configLayout.addLayout(serverLayout);
            configGroup.setLayout(configLayout);

            QVBoxLayout mainLayout = new QVBoxLayout();
            mainLayout.addWidget(configGroup);
            mainLayout.addStretch(1);
            setLayout(mainLayout);
        }
    }

    private class UpdatePage extends QWidget {

        public UpdatePage(QWidget parent) {
            super(parent);
            QGroupBox updateGroup = new QGroupBox(tr("Package selection"));
            QCheckBox systemCheckBox = new QCheckBox(tr("Update system"));
            QCheckBox appsCheckBox = new QCheckBox(tr("Update applications"));
            QCheckBox docsCheckBox = new QCheckBox(tr("Update documentation"));

            QGroupBox packageGroup = new QGroupBox(tr("Existing packages"));

            QListWidget packageList = new QListWidget();
            QListWidgetItem qtItem = new QListWidgetItem(packageList);
            qtItem.setText(tr("Qt"));
            QListWidgetItem qsaItem = new QListWidgetItem(packageList);
            qsaItem.setText(tr("QSA"));
            QListWidgetItem teamBuilderItem = new QListWidgetItem(packageList);
            teamBuilderItem.setText(tr("Teambuilder"));

            QPushButton startUpdateButton = new QPushButton(tr("Start update"));

            QVBoxLayout updateLayout = new QVBoxLayout();
            updateLayout.addWidget(systemCheckBox);
            updateLayout.addWidget(appsCheckBox);
            updateLayout.addWidget(docsCheckBox);
            updateGroup.setLayout(updateLayout);

            QVBoxLayout packageLayout = new QVBoxLayout();
            packageLayout.addWidget(packageList);
            packageGroup.setLayout(packageLayout);

            QVBoxLayout mainLayout = new QVBoxLayout();
            mainLayout.addWidget(updateGroup);
            mainLayout.addWidget(packageGroup);
            mainLayout.addSpacing(12);
            mainLayout.addWidget(startUpdateButton);
            mainLayout.addStretch(1);
            setLayout(mainLayout);
        }
    }

    private class QueryPage extends QWidget {

        public QueryPage(QWidget parent) {
            super(parent);
            QGroupBox packagesGroup = new QGroupBox(tr("Look for packages"));

            QLabel nameLabel = new QLabel(tr("Name:"));
            QLineEdit nameEdit = new QLineEdit();

            QLabel dateLabel = new QLabel(tr("Released after:"));
            QDateTimeEdit dateEdit = new QDateTimeEdit(QDate.currentDate());

            QCheckBox releasesCheckBox = new QCheckBox(tr("Releases"));
            QCheckBox upgradesCheckBox = new QCheckBox(tr("Upgrades"));

            QSpinBox hitsSpinBox = new QSpinBox();
            hitsSpinBox.setPrefix(tr("Return up to") + " ");
            hitsSpinBox.setSuffix(" " + tr("results"));
            hitsSpinBox.setSpecialValueText(tr("Return only the first result"));
            hitsSpinBox.setMinimum(1);
            hitsSpinBox.setMaximum(100);
            hitsSpinBox.setSingleStep(10);

            QPushButton startQueryButton = new QPushButton(tr("Start query"));

            QGridLayout packagesLayout = new QGridLayout();
            packagesLayout.addWidget(nameLabel, 0, 0);
            packagesLayout.addWidget(nameEdit, 0, 1);
            packagesLayout.addWidget(dateLabel, 1, 0);
            packagesLayout.addWidget(dateEdit, 1, 1);
            packagesLayout.addWidget(releasesCheckBox, 2, 0);
            packagesLayout.addWidget(upgradesCheckBox, 3, 0);
            packagesLayout.addWidget(hitsSpinBox, 4, 0, 1, 2);
            packagesGroup.setLayout(packagesLayout);

            QVBoxLayout mainLayout = new QVBoxLayout();
            mainLayout.addWidget(packagesGroup);
            mainLayout.addSpacing(12);
            mainLayout.addWidget(startQueryButton);
            mainLayout.addStretch(1);
            setLayout(mainLayout);
        }
    }
}
