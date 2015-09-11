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

package org.qtjambi.tests;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.webkit.*;

class HelloWebKit extends QWidget {

    private QWebView browser;
    private QLineEdit field;
    private QLabel state;

    public HelloWebKit() {
        field = new QLineEdit();
        browser = new QWebView();
        state = new QLabel("No page loaded...");
        state.setFixedWidth(200);

        QGridLayout layout = new QGridLayout(this);
        layout.addWidget(field, 0, 0);
        layout.addWidget(state, 0, 1);
        layout.addWidget(browser, 1, 0, 1, 2);

        field.returnPressed.connect(this, "open()");

        browser.loadStarted.connect(this, "loadStarted()");
        browser.loadProgress.connect(this, "loadProgress(int)");
        browser.loadFinished.connect(this, "loadDone()");
    }

    public void loadStarted() {
        state.setText("Starting to load: " + field.text());
    }

    public void loadDone() {
        state.setText("Loading done...");
    }

    public void loadProgress(int x) {
        state.setText("Loading: " + x + " %");
    }

    public void open() {
        String text = field.text();

        if (text.indexOf("://") < 0)
            text = "http://" + text;

        System.out.println(text);

        browser.load(new QUrl(text));
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        HelloWebKit widget = new HelloWebKit();
        widget.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
