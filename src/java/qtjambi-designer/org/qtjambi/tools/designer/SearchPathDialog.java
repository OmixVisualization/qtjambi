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

package org.qtjambi.tools.designer;

import org.qtjambi.qt.core.QAbstractItemModel;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QtEnumerator;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

public class SearchPathDialog extends QDialog {

    private final QPixmap PIXMAP_FOLDER = new QPixmap("classpath:org/qtjambi/tools/designer/classpathdir.png");
    private final QPixmap PIXMAP_ARCHIVE = new QPixmap("classpath:org/qtjambi/tools/designer/classpathjar.png");

    private QLineEdit path = new QLineEdit();
    private QPushButton addButton = new QPushButton(tr("Add"));
    private QPushButton addDirButton = new QPushButton(tr("Add Directory"));
    private QPushButton addJarButton = new QPushButton(tr("Add .jar"));
    private QPushButton removeButton = new QPushButton(tr("Remove"));
    private QPushButton moveUpButton = new QPushButton(tr("Move  up"));
    private QPushButton moveDownButton = new QPushButton(tr("Move down"));
    private QListWidget list = new QListWidget();
    private QDialogButtonBox buttons = new QDialogButtonBox();

    private String previousDir = null;

    public SearchPathDialog(QWidget parent) {
        super(parent);

        // Properties..
        list.setAlternatingRowColors(true);
        QDialogButtonBox.StandardButtons buts = new QDialogButtonBox.StandardButtons();
        buts.set(QDialogButtonBox.StandardButton.Ok);
        buts.set(QDialogButtonBox.StandardButton.Cancel);
        buttons = new QDialogButtonBox(buts, Qt.Orientation.Horizontal);

        // Layout
        QGridLayout grid = new QGridLayout(this);
        grid.addWidget(path, 0, 0, 1, 1);
        grid.addWidget(addButton, 0, 1);
        grid.addWidget(addJarButton, 1, 1);
        grid.addWidget(addDirButton, 2, 1);
        grid.addWidget(removeButton, 3, 1);
        grid.addWidget(moveUpButton, 4, 1);
        grid.addWidget(moveDownButton, 5, 1);
        grid.addWidget(list, 1, 0, 6, 1);
        grid.addWidget(buttons, 7, 0, 1, 2);

        // Connections
        addButton.clicked.connect(this, "addPath()");
        addJarButton.clicked.connect(this, "browseForArchive()");
        addDirButton.clicked.connect(this, "browseForDirectory()");
        removeButton.clicked.connect(this, "removeCurrent()");
        moveUpButton.clicked.connect(this, "moveCurrentUp()");
        moveDownButton.clicked.connect(this, "moveCurrentDown()");
        path.textChanged.connect(this, "updateAddButtonState(String)");
        buttons.button(QDialogButtonBox.StandardButton.Ok).clicked.connect(this, "accept()");
        buttons.button(QDialogButtonBox.StandardButton.Cancel).clicked.connect(this, "reject()");

        List<String> nameFilters = new ArrayList<String>();
        nameFilters.add("*.jar");
        QDirModel dirModel = new QDirModel(this);
        dirModel.setNameFilters(nameFilters);
        dirModel.setFilter(new QDir.Filters(QDir.Filter.AllDirs,
                                            QDir.Filter.Files));

        QCompleter completer = new QCompleter(this);
        completer.setModel(dirModel);
        completer.setCaseSensitivity(Qt.CaseSensitivity.CaseInsensitive);
        path.setCompleter(completer);
    }

    public List<String> paths() {
        int size = list.count();
        List<String> paths = new ArrayList<String>(size);
        for (int i=0; i<size; ++i) {
            QListWidgetItem item = list.item(i);
            paths.add(item.text());
        }
        return paths;
    }


    public void setPaths(List<String> paths) {
        list.reset();

        for (String s : paths) {
            if (new QFileInfo(s).isDir())
                addPathEntry(s, PIXMAP_FOLDER);
            else
                addPathEntry(s, PIXMAP_ARCHIVE);
        }
    }

    public void moveCurrentUp() {
        QListWidgetItem item = list.currentItem();
        if (item != null) {
            int row = list.row(item);
            if (row > 0) {
                list.takeItem(row);
                list.insertItem(row - 1, item);
                list.setCurrentItem(item);
            }
        }
    }

    public void moveCurrentDown() {
        QListWidgetItem item = list.currentItem();
        if (item != null) {
            int row = list.row(item);
            if (row < list.count() - 1) {
                list.takeItem(row);
                list.insertItem(row + 1, item);
                list.setCurrentItem(item);
            }
        }
    }

    public void browseForArchive() {
        QFileDialog.Filter filter = new QFileDialog.Filter("*.jar");
        String s = QFileDialog.getOpenFileName(this, "Add Java Archive", previousDir, filter);
        addPathEntry(s, PIXMAP_ARCHIVE);
    }

    public void browseForDirectory() {
        String s = QFileDialog.getExistingDirectory(this, "Add Java Classpath Directory", previousDir);
        addPathEntry(s, PIXMAP_FOLDER);
    }

    public void removeCurrent() {
        QListWidgetItem item = list.currentItem();
        if (item != null)
            list.takeItem(list.row(item));
    }

    private QPixmap pixmapForPath(String p) {
        QFileInfo f = new QFileInfo(p);
        if (f.exists()) {
            if (f.isDir())
                return PIXMAP_FOLDER;
            else if (f.suffix().equals("jar"))
                return PIXMAP_ARCHIVE;
        }
        return null;
    }

    public void addPath() {
        String file = path.text();
        QPixmap pm = pixmapForPath(file);
        if (pm != null)
            addPathEntry(file, pm);
    }

    private void updateAddButtonState(String s) {
        addButton.setEnabled(pixmapForPath(s) != null);
    }

    private void addPathEntry(String s, QPixmap pixmap) {
        if (s.length() > 0) {
            QListWidgetItem item = new QListWidgetItem(list);
            item.setText(s);
            item.setIcon(new QIcon(pixmap));
            previousDir = new QFileInfo(s).absolutePath();
        }
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        SearchPathDialog spd = new SearchPathDialog(null);

        String classpath = System.getProperty("java.class.path");
        List<String> list = new ArrayList<String>();
        Collections.addAll(list, RetroTranslatorHelper.split(classpath, java.io.File.pathSeparator));
        spd.setPaths(list);

        spd.exec();

        QApplication.quit();
    }
}
