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
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QShowEvent;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QButtonGroup;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QFileIconProvider;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QRadioButton;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QTreeWidget;
import org.qtjambi.qt.widgets.QTreeWidgetItem;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;


class ResourceItem extends QTreeWidgetItem
{
    private QFileInfo mainInfo = null;
    private static QFileIconProvider iconProvider = new QFileIconProvider();
    private boolean isPopulated = false;
    private QTreeWidgetItem dummyNode = null;

    public ResourceItem(QTreeWidgetItem parent, QFileInfo info, boolean recurse)
    {
        super(parent);
        setInfo(info, recurse);
    }

    public ResourceItem(QTreeWidget parent, QFileInfo info, boolean recurse)
    {
        super(parent);
        setInfo(info, recurse);
    }

    public QFileInfo getInfo()
    {
        return mainInfo;
    }

    boolean shouldInsertFile(QFileInfo i)
    {
        return ((i.isDir() && !i.fileName().equals(".") && !i.fileName().equals(".."))
                || i.suffix().equals("png")
                || i.suffix().equals("jpg")
                || i.suffix().equals("jpeg"));
    }

    private void populate()
    {
        QDir dir = new QDir(mainInfo.absoluteFilePath());

        List<QFileInfo> entryList = dir.entryInfoList();

        for (QFileInfo i : entryList) {
            if (shouldInsertFile(i))
                new ResourceItem(this, i, false);
        }

        isPopulated = true;
    }

    public void expand()
    {
        if (!isPopulated) {
            if (dummyNode != null) {
                takeChildren();
                dummyNode.dispose();
                dummyNode = null;
            }

            populate();
        }
    }

    private void setInfo(QFileInfo info, boolean recurse)
    {
        mainInfo = info;

        setText(0, mainInfo.fileName());
        setIcon(0, iconProvider.icon(info));

        if (mainInfo.isDir() && recurse) {
            populate();
        } else if (mainInfo.isDir()) {
            dummyNode = new QTreeWidgetItem(this);
        }

    }
}

@QtJambiExample(name = "Resource System",
                canInstantiate = "call-static-method:notWebstart")
public class ResourceSystem extends QWidget
{
    private QLabel currentImage = null;
    private QTreeWidget selection = null;
    private QRadioButton browseClassPathButton;
    private boolean browseClassPath = true;
    private boolean shown = false;
    private String jarName = null;

    public Signal1<String> pathChanged = new Signal1<String>();

    public ResourceSystem()
    {
        setupUI();

        pathChanged.connect(this, "setPreviewFile(String)");
    }

    private void setupUI()
    {
        currentImage = new QLabel();
        selection = new QTreeWidget();
        selection.setColumnCount(1);
        selection.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum);
        selection.currentItemChanged.connect(this, "itemChanged(QTreeWidgetItem, QTreeWidgetItem)");
        selection.itemExpanded.connect(this, "expandItem(QTreeWidgetItem)");

        List<String> labels = new LinkedList<String>();
        labels.add("Name");
        selection.setHeaderLabels(labels);

        browseClassPathButton = new QRadioButton("Browse class path");
        browseClassPathButton.setChecked(true);
        QRadioButton bt2 = new QRadioButton("Pick file to browse");

        QButtonGroup group = new QButtonGroup(this);
        group.addButton(browseClassPathButton);
        group.addButton(bt2);
        group.setId(browseClassPathButton, 0);
        group.setId(bt2, 1);
        group.buttonIdClicked.connect(this, "modeChanged(int)");

        QHBoxLayout layout2 = new QHBoxLayout();
        layout2.addWidget(browseClassPathButton);
        layout2.addWidget(bt2);

        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(selection);
        layout.addLayout(layout2);
        layout.addWidget(currentImage);

        setLayout(layout);

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        setWindowTitle("Resource System");
    }

    protected void expandItem(QTreeWidgetItem item)
    {
        if (item instanceof ResourceItem)
            ((ResourceItem) item).expand();
    }

    protected void modeChanged(int id)
    {
        if (id == 0)
            browseClassPath = true;
        else
            browseClassPath = false;

        setupSelection();
    }

    protected void itemChanged(QTreeWidgetItem cur, QTreeWidgetItem prev)
    {
        if (cur == null || !(cur instanceof ResourceItem))
            return ;

        ResourceItem selected_item = (ResourceItem) cur;
        QFileInfo info = selected_item.getInfo();

        if (info.exists() && !info.isDir()) {
            pathChanged.emit(info.absoluteFilePath());
        }
    }

    public void setPreviewFile(String path) {
        QPixmap pm = new QPixmap(path);
        if (pm.width() > 100 || pm.height() > 100)
            pm = pm.scaled(100, 100, Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.SmoothTransformation);
        currentImage.setPixmap(pm);

    }

    private void setupSelection()
    {
        selection.clear();

        String searchPath = null;
        if (!browseClassPath) {
            String fileName = QFileDialog.getOpenFileName(this, "Select a .jar file",
                    null, new QFileDialog.Filter("Jar Files (*.jar)"));

            if (fileName.length() == 0) {
                browseClassPath = true;
                browseClassPathButton.setChecked(true);
            }

            QFileInfo jarInfo = new QFileInfo(fileName);
            searchPath = "classpath:" + jarInfo.canonicalFilePath() + "#/";
        }

        if (browseClassPath) {
            searchPath = "classpath:/";
        }

        QFileInfo info = new QFileInfo(searchPath);
        if (!info.exists() || !info.isDir()) {
            QMessageBox.warning(this, "Couldn't open root dir", "Problem reading from class path");
            return ;
        }
        new ResourceItem(selection, info, true);

    }

    @Override
    protected void showEvent(QShowEvent e) {
        if (!shown) {
            QTimer.singleShot(0, this, "setupSelection()");
            shown = true;
        }
    }


    public static boolean notWebstart() {
        return System.getProperty("org.qtjambi.launcher.webstart") == null;
    }


    public static void main(String[] args)
    {
        QApplication.initialize(args);
        ResourceSystem w = new ResourceSystem();
        w.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }

}
