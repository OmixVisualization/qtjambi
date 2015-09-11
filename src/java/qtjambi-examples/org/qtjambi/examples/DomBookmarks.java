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

import java.util.Hashtable;
import java.util.Vector;

import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.core.Qt.ItemFlags;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QHeaderView;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QStyle;
import org.qtjambi.qt.widgets.QTreeWidget;
import org.qtjambi.qt.widgets.QTreeWidgetItem;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.xml.QDomDocument;
import org.qtjambi.qt.xml.QDomElement;
import org.qtjambi.qt.xml.QDomText;

@QtJambiExample(name = "Dom Bookmarks")
public class DomBookmarks extends QMainWindow {

    public static void main(String args[]) {
        QApplication.initialize(args);

        DomBookmarks domBookmarks = new DomBookmarks();
        domBookmarks.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }

    private QMenu fileMenu;
    private QMenu helpMenu;
    private QAction openAct;
    private QAction saveAsAct;
    private QAction exitAct;
    private QAction aboutAct;
    private QAction aboutQtAct;
    private QAction aboutQtJambiAct;

    private XbelTree xbelTree;

    public DomBookmarks() {
        xbelTree = new XbelTree(this);
        setCentralWidget(xbelTree);

        createActions();
        createMenus();

        loadFile("classpath:org/qtjambi/examples/frank.xbel");
        statusBar().showMessage(tr("Ready"));

        setWindowTitle(tr("DOM Bookmarks"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        resize(480, 320);
    }

    private void loadFile(String fileName){
        QFile file = new QFile(fileName);
        if (!file.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly, QFile.OpenModeFlag.Text))) {
            QMessageBox.warning(this, tr("SAX Bookmarks"), String.format(tr("Cannot read file: %s"), fileName)
                    + " :\n" + file.errorString() + ".");
            return;
        }

        if (xbelTree.read(file))
            statusBar().showMessage(tr("File loaded"), 2000);

        file.dispose();
    }

    private void open() {
        String fileName = QFileDialog.getOpenFileName(this, tr("Open Bookmark File"), QDir
                .currentPath(), new QFileDialog.Filter(tr("XBEL Files (*.xbel *.xml)")));
        if (fileName.equals(""))
            return;

        loadFile(fileName);
    }

    private void saveAs() {
        String fileName = QFileDialog.getSaveFileName(this, tr("Save Bookmark File"), QDir
                .currentPath(), new QFileDialog.Filter(tr("XBEL Files (*.xbel *.xml)")));
        if (fileName.equals(""))
            return;

        QFile file = new QFile(fileName);
        if (!file.open(new QFile.OpenMode(QFile.OpenModeFlag.WriteOnly, QFile.OpenModeFlag.Text))) {
            QMessageBox.warning(this, tr("SAX Bookmarks"), String.format(tr("Cannot write file: %s"), fileName)
                    + " :\n" + file.errorString() + ".");
            return;
        }

        if (xbelTree.write(file))
            statusBar().showMessage(tr("File saved"), 2000);

        file.dispose();
    }

    private void about() {
        QMessageBox.about(this, tr("About DOM Bookmarks"),
                tr("The <b>DOM Bookmarks</b> example demonstrates how to "
                        + "use Qt's DOM classes to read and write XML " + "documents."));
    }

    private void createActions() {
        openAct = new QAction(tr("&Open..."), this);
        openAct.setShortcut(tr("Ctrl+O"));
        openAct.triggered.connect(this, "open()");

        saveAsAct = new QAction(tr("&Save As..."), this);
        saveAsAct.setShortcut(tr("Ctrl+S"));
        saveAsAct.triggered.connect(this, "saveAs()");

        exitAct = new QAction(tr("E&xit"), this);
        exitAct.setShortcut(tr("Ctrl+Q"));
        exitAct.triggered.connect(this, "close()");

        aboutAct = new QAction(tr("&About"), this);
        aboutAct.triggered.connect(this, "about()");

        aboutQtJambiAct = new QAction(tr("About &Qt Jambi"), this);
        aboutQtJambiAct.triggered.connect(QApplication.instance(), "aboutQtJambi()");

        aboutQtAct = new QAction(tr("About Q&t"), this);
        aboutQtAct.triggered.connect(QApplication.instance(), "aboutQt()");
    }

    private void createMenus() {
        fileMenu = menuBar().addMenu(tr("&File"));
        fileMenu.addAction(openAct);
        fileMenu.addAction(saveAsAct);
        fileMenu.addAction(exitAct);

        menuBar().addSeparator();

        helpMenu = menuBar().addMenu(tr("&Help"));
        helpMenu.addAction(aboutAct);
        helpMenu.addSeparator();
        helpMenu.addAction(aboutQtJambiAct);
        helpMenu.addAction(aboutQtAct);
    }

    private class XbelTree extends QTreeWidget {

        private QDomDocument domDocument = new QDomDocument();
        private Hashtable<QTreeWidgetItem, QDomElement> domElementForItem = new Hashtable<QTreeWidgetItem, QDomElement>();
        private QIcon folderIcon = new QIcon();
        private QIcon bookmarkIcon;

        private XbelTree(QWidget parent) {
            super(parent);
            Vector<String> labels = new Vector<String>();
            labels.add("Title");
            labels.add("Location");

            header().setSectionResizeMode(QHeaderView.ResizeMode.Stretch);
            setHeaderLabels(labels);

            folderIcon.addPixmap(style().standardIcon(QStyle.StandardPixmap.SP_DirClosedIcon).pixmap(new QSize(24,24),
                    QIcon.Mode.Normal, QIcon.State.Off));

            folderIcon.addPixmap(style().standardIcon(QStyle.StandardPixmap.SP_DirOpenIcon).pixmap(new QSize(24,24),
                    QIcon.Mode.Normal, QIcon.State.On));

            bookmarkIcon = style().standardIcon(QStyle.StandardPixmap.SP_FileIcon);
        }

        private boolean read(QIODevice device) {
            QDomDocument.Result res = domDocument.setContent(device, true);
            if(!res.success){
                QMessageBox.information(window(), tr("DOM Bookmarks"),
                        String.format(tr("Parse error at line %s, column %s :"),  res.errorLine, res.errorColumn)
                        + "\n" + res.errorMessage);
                return false;
            }

            QDomElement root = domDocument.documentElement();

            if (!root.tagName().equals("xbel")) {
                QMessageBox.information(window(), tr("DOM Bookmarks"),
                        tr("The file is not an XBEL file."));
                return false;
            } else if (root.hasAttribute("version") && !root.attribute("version").equals("1.0")) {
                QMessageBox.information(window(), tr("DOM Bookmarks"),
                        tr("The file is not an XBEL version 1.0 " + "file."));
                return false;
            }

            clear();

            itemChanged.disconnect(this, "updateDomElement(QTreeWidgetItem, int)");

            QDomElement child = root.firstChildElement("folder");
            while (!child.isNull()) {
                parseFolderElement(child, null);
                child = child.nextSiblingElement("folder");
            }

            itemChanged.connect(this, "updateDomElement(QTreeWidgetItem, int)");

            return true;
        }

        private boolean write(QIODevice device) {
            final int IndentSize = 4;

            QTextStream out = new QTextStream(device);
            domDocument.save(out, IndentSize);
            return true;
        }

        private void updateDomElement(QTreeWidgetItem item, int column) {
            QDomElement element = domElementForItem.get(item);// .value(item);
            if (!element.isNull()) {
                if (column == 0) {
                    QDomElement oldTitleElement = element.firstChildElement("title");
                    QDomElement newTitleElement = domDocument.createElement("title");

                    QDomText newTitleText = domDocument.createTextNode(item.text(0));
                    newTitleElement.appendChild(newTitleText);

                    element.replaceChild(newTitleElement, oldTitleElement);
                } else {
                    if (element.tagName().equals("bookmark"))
                        element.setAttribute("href", item.text(1));
                }
            }
        }

        private void parseFolderElement(final QDomElement element, QTreeWidgetItem parentItem) {
            QTreeWidgetItem item = createItem(element, parentItem);

            String title = element.firstChildElement("title").text();
            if (title.equals(""))
                title = tr("Folder");

            item.setFlags(new ItemFlags(item.flags().value() | Qt.ItemFlag.ItemIsEditable.value()));

            item.setIcon(0, folderIcon);
            item.setText(0, title);

            boolean folded = (element.attribute("folded") != "no");
            item.setExpanded(!folded);

            QDomElement child = element.firstChildElement();
            while (!child.isNull()) {
                if (child.tagName().equals("folder")) {
                    parseFolderElement(child, item);
                } else if (child.tagName().equals("bookmark")) {
                    QTreeWidgetItem childItem = createItem(child, item);

                    title = child.firstChildElement("title").text();
                    if (title.equals(""))
                        title = tr("Folder");

                    childItem.setFlags(new ItemFlags(item.flags().value()
                            | Qt.ItemFlag.ItemIsEditable.value()));

                    childItem.setIcon(0, bookmarkIcon);
                    childItem.setText(0, title);
                    childItem.setText(1, child.attribute("href"));
                } else if (child.tagName().equals("separator")) {
                    QTreeWidgetItem childItem = createItem(child, item);
                    childItem.setFlags(new ItemFlags(item.flags().value()
                            & ~(Qt.ItemFlag.ItemIsSelectable.value() | Qt.ItemFlag.ItemIsEditable
                                    .value())));
                    childItem.setText(0, "------------------------------");
                }
                child = child.nextSiblingElement();
            }
        }

        private QTreeWidgetItem createItem(final QDomElement element, QTreeWidgetItem parentItem) {
            QTreeWidgetItem item;
            if (parentItem != null) {
                item = new QTreeWidgetItem(parentItem);
            } else {
                item = new QTreeWidgetItem(this);
            }
            domElementForItem.put(item, element);
            return item;
        }
    }
}
