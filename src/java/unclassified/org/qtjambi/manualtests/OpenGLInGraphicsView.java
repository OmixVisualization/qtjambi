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

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.opengl.*;
import org.qtjambi.qt.gui.*;

public class OpenGLInGraphicsView extends ZoomableGraphicsView {

    public OpenGLInGraphicsView(boolean gl) {
        QGLFormat format = new QGLFormat();
        format.setSampleBuffers(true);

        if (gl) {
            setViewport(new QGLWidget(format));
            setWindowTitle("GL window");
        } else {
            setWindowTitle("Non GL Window");
        }

        QGraphicsScene scene = new QGraphicsScene();

        QGraphicsPixmapItem pixmapItem = new QGraphicsPixmapItem(new QPixmap("classpath:org/qtjambi/images/chip-demo.png")) {

            @Override
            public void paint(QPainter painter,
                    QStyleOptionGraphicsItem option, QWidget widget) {
                //painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, true);
                //super.paint(painter, option, widget);
                painter.drawPixmap(boundingRect().toRect(), this.pixmap());
            }

        };
        pixmapItem.setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsMovable, true);
        pixmapItem.setTransformationMode(Qt.TransformationMode.SmoothTransformation);
        scene.addItem(pixmapItem);

        QGraphicsPixmapItem otherPixmapItem = scene.addPixmap(new QPixmap("classpath:org/qtjambi/images/chip-demo.png"));
        otherPixmapItem.setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsMovable, true);
        otherPixmapItem.setTransformationMode(Qt.TransformationMode.SmoothTransformation);

        scene.addText("Hello World").setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsMovable, true);

        setScene(scene);
        rotate(45);

        setRenderHint(QPainter.RenderHint.SmoothPixmapTransform);

        //setRenderHint(QPainter.RenderHint.Antialiasing, true);
        //setRenderHint(QPainter.RenderHint.HighQualityAntialiasing, true);
        //setRenderHint(QPainter.RenderHint.TextAntialiasing, true);
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        OpenGLInGraphicsView widget1 = new OpenGLInGraphicsView(true);
        widget1.show();

        OpenGLInGraphicsView widget2 = new OpenGLInGraphicsView(false);
        widget2.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
