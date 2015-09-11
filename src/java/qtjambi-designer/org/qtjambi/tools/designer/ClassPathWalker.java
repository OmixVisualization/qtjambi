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
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QSettings;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.core.QTimerEvent;
import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.QPair;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QtEnumerator;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.Stack;

/**
 */
public class ClassPathWalker extends QObject {

    static {
        List<QByteArray> formats = QImageReader.supportedImageFormats();
        List<String> stringFormats = new ArrayList<String>();
        for (QByteArray f : formats) {
            if (f.toString().equals("svg"))
                continue;
            stringFormats.add("*." + f.toString());
        }
        imageFormats = stringFormats;
    }
    private static List<String> imageFormats;

    /**
     * The resourceFound signal is emitted with the resource name for each resource
     * that is found. The signal is emitted from the traversing thread.
     */
    public Signal2<String, QImage> resourceFound = new Signal2<String, QImage>();

    /**
     * The doneSearching signal is emitted when the classpath walker is done processing..
     * It is used internally to close call stop...
     */
    public Signal0 doneSearching = new Signal0();

    public Signal0 beginSearching = new Signal0();

    public ClassPathWalker() {
        if (roots == null) {
            String classpath = System.getProperty("java.class.path");
            List<String> r = new ArrayList<String>();
            Collections.addAll(r, RetroTranslatorHelper.split(classpath, java.io.File.pathSeparator));
            setRoots(r);

            addRootsFromSettings();
        }
    }

    /**
     * Performs the traversal of the directory structure...
     */
    @Override
    protected void timerEvent(QTimerEvent e) {
        if(stopped)
            return;

        if (stack.isEmpty()) {
            kill();
            doneSearching.emit();
            return;
        }

        QPair<Object, String> data = stack.pop();
        if (data.first instanceof QDir) {
            QDir dir = (QDir) data.first;
            String dirPath = QDir.toNativeSeparators(dir.absolutePath());
            if (processedDirs.contains(dirPath))
                return;
            processedDirs.add(dirPath);

            QDir.Filters filters = new QDir.Filters();
            filters.set(QDir.Filter.Readable);
            filters.set(QDir.Filter.Files);
            List<String> imgs = dir.entryList(fileExtensions, filters);

            for (String file : imgs) {
                stack.push(new QPair<Object, String>(new QFileInfo(dir.absoluteFilePath(file)), data.second));
            }

            filters.clear(QDir.Filter.Files);
            filters.set(QDir.Filter.NoDotAndDotDot);
            filters.set(QDir.Filter.Dirs);
            List<String> dirs = dir.entryList(filters);
            for (String dirName: dirs) {
                stack.push(new QPair<Object, String>(new QDir(dir.absoluteFilePath(dirName)), data.second));
            }
        } else if (data.first instanceof QFileInfo) {
            String name = ((QFileInfo) data.first).absoluteFilePath();
            int pos = name.lastIndexOf('#') + 1;

            name = name.substring(pos);
            if (name.startsWith("/"))
                name = name.substring(1);
            name = "classpath:" + name;

            QImage image = new QImage(name);
            if (!image.isNull()) {
                QImage smallImage = image.scaled(size,
                        Qt.AspectRatioMode.KeepAspectRatio,
                        Qt.TransformationMode.SmoothTransformation);

                // aspect ration makes one dimension < 1, thus, problems...
                if (smallImage.isNull()) {
                    smallImage = image.scaled(size,
                            Qt.AspectRatioMode.IgnoreAspectRatio,
                            Qt.TransformationMode.SmoothTransformation);
                }

                image.dispose();

                if (!smallImage.isNull()) {
                    resourceFound.emit(name, smallImage);
                }
            }
        }
    }

    /**
     * Starts the traversal of the directory structure... This is done in a
     * separate thread and feedback can be received through the resourceFound
     * signal.
     */
    private int timerId = 0;
    public void start() {
        stopped = false;

        stack = new Stack<QPair<Object, String>>();
        for (String s : roots) {
            if (!s.equals("")) {
                s = "classpath:" + s + "#/";
                QDir d = new QDir(s);
                stack.push(new QPair<Object, String>(d, d.absolutePath()));
            }
        }
        processedDirs = new HashSet<String>();

        beginSearching.emit();
        timerId = startTimer(50, Qt.TimerType.CoarseTimer);
    }

    public synchronized void kill() {
        if (timerId != 0)
            killTimer(timerId);
        timerId = 0;
        stopped = true;
    }

    public void setPixmapSize(QSize size) {
        this.size = size;
    }

    public synchronized static void addRootsFromSettings() {
        QSettings settings = new QSettings("Trolltech", "Qt Jambi Resource Browser");
        Object path = settings.value("Extra paths");
        if (roots != null && path != null && path instanceof String) {
            String paths[] = RetroTranslatorHelper.split((String) path, java.io.File.pathSeparator);
            for (String p : paths) {
                if (!p.equals("")) {
                    QAbstractFileEngine.addSearchPathForResourceEngine(p);
                    roots.add(p);
                }
            }
        }
    }

    public synchronized static void setRoots(List<String> r) {
        roots = r;
    }

    public synchronized static List<String> roots() {
        return roots;
    }


    /**
     * Traverses the directory and emits a signal for all the files that match the fileExtensions.
     * @param dir The directory to find files in...
     */
    private void traverse(QDir dir, String rootDir) {
    }

    private static List<String> roots;
    //private Thread thread;
    private List<String> fileExtensions = imageFormats;
    private boolean stopped;
    private Stack<QPair<Object, String>> stack;
    private Set<String> processedDirs;


    private QSize size = new QSize(16, 16);
}
