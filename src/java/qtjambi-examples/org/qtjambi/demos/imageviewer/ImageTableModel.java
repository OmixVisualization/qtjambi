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

package org.qtjambi.demos.imageviewer;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

import org.qtjambi.qt.QThread;
import org.qtjambi.qt.core.QAbstractItemModel;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QSignalMapper;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QImageReader;

public class ImageTableModel extends QAbstractItemModel {

    public static final int NAME_COL = 0;
    public static final int PIXMAP_COL = 1;
    public static final int DIMENSION_COL = 2;
    public static final int SIZE_COL = 3;

    private Thread thread;
    public ImageTableModel(QObject parent) {
        super(parent);

        mapper.mappedInteger.connect(this, "updateRow(int)");

        thread = new QThread(new Runnable() {
                public void run() {
                    while (true) {
                        try {
                            loadQueue.take().loadThumbNail();
                        } catch (InterruptedException e) { }
                    }
                }
            }
        );
        thread.setDaemon(true);
        thread.start();
    }

    public QDir getDirectory() { return directory; }

    public void setDirectory(QDir directory) {
        this.directory = directory;
        update();
    }

    public QImage imageAt(int row) {
        return pixmaps[row].image();
    }

    public int columnCount(QModelIndex parent) {
        return 4;
    }

    public QModelIndex parent(QModelIndex child) {
        return null;
    }

    public QModelIndex index(int row, int column, QModelIndex parent) {
        if (parent == null)
            return createIndex(row, column);
        return null;
    }

    public synchronized Object data(QModelIndex index, int role) {
        int row = index.row();
        int col = index.column();
        if(pixmaps.length > row){
            if (role == Qt.ItemDataRole.DisplayRole) {
                QFileInfo info = infos.get(row);
                if (col == NAME_COL) {
                    return info.fileName();
                } else if (col == SIZE_COL) {
                    return info.size();
                } else if (col == DIMENSION_COL) {
                    if (pixmaps[row].isValid()) {
                        QSize size = pixmaps[row].size();
                        return "[" + size.width() + ", " + size.height() + "]";
                    } else {
                        return "loading";
                    }
                } else if (col == PIXMAP_COL) {
                    return pixmaps[row].isValid() ? null : tr("loading");
                }
            } else if (role == Qt.ItemDataRole.DecorationRole) {
                if (col == PIXMAP_COL)
                    return pixmaps[row].isValid() ? pixmaps[row].thumbNail() : null;
            } else if (role == Qt.ItemDataRole.SizeHintRole) {
                return LazyPixmap.SMALL_SIZE;
            }
        }
        return null;
    }

    public int rowCount(QModelIndex parent__0) {
        int count = infos != null ? infos.size() : 0;
        return count;
    }

    public Object headerData(int section, Qt.Orientation orientation, int role) {
        if (orientation == Qt.Orientation.Horizontal
            && (role == Qt.ItemDataRole.DisplayRole || role == Qt.ItemDataRole.EditRole)) {
            switch (section) {
            case NAME_COL: return "Name";
            case SIZE_COL: return "Size";
            case DIMENSION_COL: return "Dimension";
            case PIXMAP_COL: return "Preview";
            }
        }
        return super.headerData(section, orientation, role);
    }

    public void updateRow(int i) {
        dataChanged.emit(index(i, 2), index(i, 3));
    }

    private void update() {
        List<QByteArray> formats = QImageReader.supportedImageFormats();

        // Svg's require threaded graphics, which is not supported so remove them from the list
        // we use for querying...
        for (int i=0; i<formats.size(); ++i) {
            if (formats.get(i).toString().equals("svg")) {
                formats.remove(i);
                break;
            }
        }

        List<String> filters = new ArrayList<String>(formats.size());
        for (QByteArray ba : formats) {
            filters.add("*." + ba.toString());
        }

        infos = directory.entryInfoList(filters,
                new QDir.Filters(QDir.Filter.Files),
                new QDir.SortFlags(QDir.SortFlag.NoSort));

        synchronized(this){
            loadQueue.clear();
            if(pixmaps!=null){
                for (LazyPixmap pixmap : pixmaps) {
                    synchronized (pixmap) {
                        pixmap.dispose();
                    }
                }
            }
            pixmaps = new LazyPixmap[infos.size()];
            for (int i=0; i<pixmaps.length; ++i) {
                pixmaps[i] = new LazyPixmap(infos.get(i).absoluteFilePath());
                pixmaps[i].loaded.connect(mapper, "map()");
                pixmaps[i].moveToThread(thread);
                loadQueue.offer(pixmaps[i]);
                mapper.setMapping(pixmaps[i], i);
            }
        }
//        reset();
        super.resetInternalData();
    }

    private QDir directory;
    private List<QFileInfo> infos;
    private LazyPixmap pixmaps[];
    private LinkedBlockingQueue<LazyPixmap> loadQueue = new LinkedBlockingQueue<LazyPixmap>();
    private QSignalMapper mapper = new QSignalMapper();
}
