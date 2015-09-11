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

package org.qtjambi.examples.qtconcurrent;

import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.concurrent.QtConcurrent;
import org.qtjambi.qt.core.QFutureWatcherVoid;
import org.qtjambi.qt.core.QThreadPool;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QProgressDialog;

class MutableInteger {
    public int value;

    public MutableInteger(int i) { value = i; }

    @Override
    public String toString() { return ((Integer)value).toString(); }
}

public class ProgressDialog implements QtConcurrent.MapFunctor<MutableInteger> {
    static final int ITERATIONS = 20;
    static final int WORK = 1000 * 1000 * 40;

    public void map(MutableInteger iteration) {
        int v = 0;
        for (int j = 0; j < WORK; ++j)
            ++v;

        System.out.println("iteration " +iteration + " in thread " + Thread.currentThread());
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        // Prepare the list.
        List<MutableInteger> list = new ArrayList<MutableInteger>();
        for (int i = 0; i < ITERATIONS; ++i)
            list.add(new MutableInteger(i));

        // Create a progress dialog.
        QProgressDialog dialog = new QProgressDialog();
        dialog.setLabelText("Progressing using " + QThreadPool.globalInstance().maxThreadCount() + " thread(s).");

        // Create a QFutureWatcher and conncect signals and slots.
        QFutureWatcherVoid futureWatcher = new QFutureWatcherVoid();
        futureWatcher.finished.connect(dialog, "reset()");
        dialog.canceled.connect(futureWatcher, "cancel()");
        futureWatcher.progressRangeChanged.connect(dialog, "setRange(int, int)");
        futureWatcher.progressValueChanged.connect(dialog, "setValue(int)");

        // Start the computation.
        futureWatcher.setFuture(QtConcurrent.map(list, new ProgressDialog()));

        // Display the dialog and start the event loop.
        dialog.exec();

        futureWatcher.waitForFinished();

        // Query the future to check if was canceled.
        if (futureWatcher.future().isCanceled())
            System.out.println("The job was canceled.");
        else
            System.out.println("The job was not canceled.");
    }
}
