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

import org.qtjambi.qt.core.QAbstractFileEngine;
import org.qtjambi.qt.core.QAbstractItemModel;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QSortFilterProxyModel;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.QNativePointer.Type;
import org.qtjambi.qt.QSignalEmitter;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class ResourceBrowserModel extends QAbstractItemModel {

    public static final QSize PIXMAP_SIZE = new QSize(16, 16);
    private static final String DEFAULT_PACKAGE = "<default package>";
    private static final QPixmap FOLDER_PIXMAP;
    static {
        String extra = System.getProperty("org.qtjambi.qtjambi.internal.extra_classpath");
        if (extra != null) {
            QAbstractFileEngine.addSearchPathForResourceEngine(extra);
        }
        QPixmap pm = new QPixmap("classpath:org/qtjambi/tools/designer/folder.png");
        if (pm.isNull()) {
            FOLDER_PIXMAP = pm; // invalid
            System.err.println("Qt Jambi Designer: folder.png not found in org/qtjambi/tools/designer!");
        } else {
            FOLDER_PIXMAP = pm.scaled(PIXMAP_SIZE, Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.SmoothTransformation);
        }
    }

    private static class NamedItem implements Comparable<Object> {
        public NamedItem(String name) {
            this.name = name;
        }
        @Override
        public String toString() { return name; }
        public int compareTo(Object o) { return name.compareTo(o.toString()); }
        String name;
    }

    private static class Path extends NamedItem implements Comparable<Object> {
        List<Resource> images;

        public Path(String name) {
            super(name);
            images = new ArrayList<Resource>();
        }

        public void addResource(String name, String fullName, QImage image) {
            Resource r = new Resource(name);
            r.fullName = fullName;
            r.image = image;
            images.add(r);
        }
    }

    public static class FilterModel extends QSortFilterProxyModel {

        public FilterModel(QObject parent) {
            super(parent);
        }

        private boolean pathContains(Path p, QRegExp re) {
            for (Resource r : p.images)
                if (re.indexIn(r.name) >= 0)
                    return true;
            return false;
        }

        @Override
        protected boolean filterAcceptsRow(int row, QModelIndex parent) {
            ResourceBrowserModel m = (ResourceBrowserModel) sourceModel();

            QRegExp re = filterRegExp();
            if (parent == null) {
                Path p = m.roots.get(row);
                return re.indexIn(p.name) >= 0 || pathContains(p, re);
            } else {
                Path p = m.roots.get(parent.row());
                Resource r = p.images.get(row);
                return re.indexIn(p.name) >= 0 || re.indexIn(r.name) >= 0;
            }
        }
    }

    public static class Resource extends NamedItem implements Comparable<Object> {
        public Resource(String name) {
            super(name);
        }
        String fullName;
        QImage image;
    }

    public Signal1<QModelIndex> rowsAdded = new Signal1<QModelIndex>();

    public ResourceBrowserModel() {
        QPalette p = QApplication.palette();

        QLinearGradient lg = new QLinearGradient(0, 0, 0, 16);
        lg.setColorAt(0, p.color(QPalette.ColorRole.AlternateBase));
        lg.setColorAt(0.1, p.color(QPalette.ColorRole.Base));
        lg.setColorAt(1, p.color(QPalette.ColorRole.AlternateBase));
        gradient = new QBrush(lg);
    }

    public String resource(QModelIndex index) {
        int id = (int) index.internalId();
        if (id > 0) {
            Path p = roots.get(id - 1);
            return p.images.get(index.row()).fullName;
        }
        return null;
    }

    private static String pathName(String name) {
        int pathPos = Math.max(name.indexOf(':'), name.indexOf('#')) + 1;
        if (name.charAt(pathPos) == '/')
            ++pathPos;
        int filePos = name.lastIndexOf('/');
        if (pathPos >= filePos)
            return null;
        return name.substring(pathPos, filePos).replace('/', '.');
    }

    private static String imageName(String name) {
        int filePos = name.lastIndexOf('/');
        return name.substring(filePos+1);
    }

    public QModelIndex indexForPath(String s) {
        String pathName = pathName(s).replace('/', '.');
        String imageName = null;

        for (int i=0; i<roots.size(); ++i) {
            Path p = roots.get(i);
            if (p.name.equals(pathName)) {
                if (imageName == null)
                    imageName = imageName(s);
                for (int j=0; j<p.images.size(); ++j) {
                    Resource r = p.images.get(j);
                    if (imageName.equals(r.name)) {
                    	QNativePointer np = new QNativePointer(Type.Long);
                    	np.setLongValue(i+1);
                        return createIndex(j, 0, np);
                    }
                }
            }

        }

        return null;
    }

    public void addResource(String name, QImage image) {
//        if (!name.startsWith("classpath:"))
//            throw new RuntimeException("Bad resource name: " + name);
        String pathName = pathName(name);
        String imageName = imageName(name);

        if (pathName == null)
            return;

        if (pathName.length() == 0)
            pathName = DEFAULT_PACKAGE;

        int pathIndex = resolvePath(pathName);
        Path path = roots.get(pathIndex);

        int s = path.images.size();

        QModelIndex parent = index(pathIndex, 0, null);
        beginInsertRows(parent, s, s);
        path.addResource(imageName, name, image);
        endInsertRows();

        rowsAdded.emit(parent);
    }


    //@SuppressWarnings("unchecked")  // FIXME: ECJ says this is no needed
    private int resolvePath(String name) {
        name = name.intern();
        for (int i=0; i<roots.size(); ++i) {
            Path p = roots.get(i);
            if (p.name == name) {
                return i;
            }
        }
        int s = roots.size();
        beginInsertRows(null, s, s);

        Path p = new Path(name);
        roots.add(p);

        endInsertRows();

        return s;
    }

    @Override
    public int columnCount(QModelIndex parent) {
        return 1;
    }

    @Override
    public Object data(QModelIndex index, int role) {
        if (index == null)
            return null;

        if (index.internalId() > 0) {
            Path p = roots.get((int) index.internalId()-1);
            Resource r = p.images.get(index.row());
            if (role == Qt.ItemDataRole.DisplayRole)
                return r.name;
            else if (role == Qt.ItemDataRole.DecorationRole)
                return r.image;
        } else {
            if (role == Qt.ItemDataRole.DisplayRole)
                return roots.get(index.row()).name;
            else if (role == Qt.ItemDataRole.BackgroundRole)
                return gradient;
//            else if (role == Qt.ItemDataRole.BackgroundRole)
//                return QApplication.palette().alternateBase().color();
            else if (role == Qt.ItemDataRole.DecorationRole)
                return FOLDER_PIXMAP;
        }

        return null;
    }

    @Override
    public QModelIndex index(int row, int column, QModelIndex parent) {
        if (parent == null) {
            return createIndex(row, column);
        } else {
        	QNativePointer np = new QNativePointer(Type.Long);
        	np.setLongValue(parent.row() + 1);
            return createIndex(row, column, np);
        }
    }

    @Override
    public QModelIndex parent(QModelIndex child) {
        if (child.internalId() == 0)
            return null;
        return createIndex((int) child.internalId() - 1, 0);
    }

    @Override
    public int rowCount(QModelIndex parent) {
        if (parent == null)
            return roots.size();
        else if (parent.internalId() == 0)
            return roots.get(parent.row()).images.size();
        else
            return 0;
    }

    private List<Path> roots = new ArrayList<Path>();

    private QBrush gradient;
}
