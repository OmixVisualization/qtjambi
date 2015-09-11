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

import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QImage;


public class LazyPixmap extends QObject {

    public static final QSize SMALL_SIZE = new QSize(32, 32);

    public Signal0 loaded = new Signal0();

    public LazyPixmap(String fileName) {
        this.fileName = fileName;
    }

    public void loadThumbNail() {
        QImage img = new QImage(fileName);

        assert !img.isNull();

        QImage small = (img.width() > SMALL_SIZE.width()
                        || img.height() > SMALL_SIZE.height())
                        ? img.scaled(SMALL_SIZE, Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.SmoothTransformation)
                        : img.copy();

        synchronized(this){
            this.thumbNail = small;
            this.size = img.size();
            if(nativeId()!=0)
                loaded.emit();
        }
        img.dispose();
    }

    public synchronized QImage image() { return new QImage(fileName); }
    public synchronized QSize size() { return size; }
    public synchronized QImage thumbNail() { return thumbNail; }
    public synchronized boolean isValid() { return thumbNail != null; }

    private String fileName;
    private QImage thumbNail;
    private QSize size;
}
