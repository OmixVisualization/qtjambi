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

package org.qtjambi.manualtests;

import org.qtjambi.qt.opengl.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;

public class VideoInGraphicsView extends ZoomableGraphicsView {

    public VideoInGraphicsView(String file) {
        setRenderHint(QPainter.RenderHint.Antialiasing);

        // Video widget
        VideoWidget videoWidget = new VideoWidget();

        // Set up video player
        final MediaObject mediaObject = new MediaObject(this);
        Phonon.createPath(mediaObject, videoWidget);
        mediaObject.setCurrentSource(new MediaSource(file));
        mediaObject.play();

        mediaObject.finished.connect(new Object() {
                public void start() {
                    mediaObject.seek(0);
                    mediaObject.play();
                }
            }, "start()");

        QGraphicsProxyWidget w = new QGraphicsProxyWidget();
        w.setWidget(videoWidget);
        w.setVisible(true);
        setRenderHint(QPainter.RenderHint.SmoothPixmapTransform);

        // Put ellipse on top
        w.setZValue(-1);

        // Set up a graphics scene
        QGraphicsScene scene = new QGraphicsScene();

        scene.addEllipse(10, 10, 100, 100, new QPen(QColor.black), new QBrush(QColor.red)).setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsMovable, true);
        scene.addItem(w);
        QGraphicsTextItem item = new QGraphicsTextItem();


        QFile f = new QFile("test.html");
        f.open(QIODevice.OpenModeFlag.ReadOnly);
        QByteArray ba = f.readAll();
        System.err.println(ba.toString());
        f.close();
        item.setPos(w.boundingRect().bottomLeft());
        item.setHtml(ba.toString());
        item.setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsMovable, true);
        item.setVisible(true);
        item.setTextInteractionFlags(Qt.TextInteractionFlag.TextEditorInteraction);
        scene.addItem(item);

        setScene(scene);

        // Rotate the scene 45 degrees
        rotate(45);

        setViewport(new QGLWidget());

        startTimer(100);

    }

    protected void timerEvent(QTimerEvent e) {
        rotate(0.1);
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

                String file = null;
                if (args.length > 0) {
                    file = args[0];

                    if (!new QFileInfo(file).exists()) {
                        System.err.println("File does not exist: " + file);
                        return;
                    }
                } else {
                    System.err.println("Please specify a movie file...");
                }

        VideoInGraphicsView view = new VideoInGraphicsView(file);
        view.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
