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

package org.qtjambi.qt;

import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;

/**
 * The QThread class extends the java.lang.Thread class and should be used when
 * QObjects, i.e., instances of classes that inherit from QObject, are used in
 * threads. Thread may be used for all other classes that inherit from QtJambiObject.
 * <p>
 * QObjects have object affinity, i.e., they belong to a given thread, which is
 * accessed through their thread() method. It is only allowed to access a QObject
 * from the thread to which it belongs.
 * <p>
 * The QThread class was introduced to ensure that native resources are freed when
 * QObjects are garbage collected. The garbage collector thread posts an event to
 * the native QObject, which then deletes itself. Before exiting, the thread will
 * flush all events - causing all native QObjects to be deleted.
 * <p>
 * QThread has two convenience signals: starting and finished. Started is emitted
 * just before the runnable target is invoked. Finished is emitted just before the
 * thread shuts down - after the execution of the runnable target and the flushing
 * of the event loop.
 *
 * @See org.qtjambi.qt.core.QObject#thread()
 * @See org.qtjambi.qt.QThreadAffinityException
 * @See <a href="../threads.html">Threading support in Qt</a>
 *
 * @author gunnar
 */
public final class QThread extends Thread {


    /**
     * The starting signal is emitted when before the QThread invokes
     * its runnable target. The signal is emitted from the running
     * thread.
     */
    public final QSignalEmitter.Signal0 starting;


    /**
     * The finished signal is emitted after the QThread has finished
     * executing its runnable target. The signal is emitted from the
     * running thread.
     */
    public final QSignalEmitter.Signal0 finished;


    /**
     * Creates a new QThread with the specified invokable target
     * @param target The invokable target.
     */
    public QThread(Runnable target) {
        super(target);
        notifier = new Notifier();
        starting = notifier.starting;
        finished = notifier.finished;
    }


    /**
     * Creates a new QThread with the specified invokable target and thread group.
     * @param group The thread group.
     * @param target The target.
     */
    public QThread(ThreadGroup group, Runnable target) {
        super(group, target);
        notifier = new Notifier();
        starting = notifier.starting;
        finished = notifier.finished;
    }


    /**
     * Creates a new QThread with the specified invokable target and the given name.
     * @param target The invokable target.
     * @param name The name.
     */
    public QThread(Runnable target, String name) {
        super(target, name);
        notifier = new Notifier();
        starting = notifier.starting;
        finished = notifier.finished;
    }


    /**
     * Creates a new QThread with the specified invokable target, name and thread group.
     * @param group The Thread group
     * @param target The invokable target
     * @param name The name.
     */
    public QThread(ThreadGroup group, Runnable target, String name) {
        super(group, target, name);
        notifier = new Notifier();
        starting = notifier.starting;
        finished = notifier.finished;
    }


    /**
     * Creates a new QThread with the specified invokable target, name, thread group and stack size.
     * @param group The Thread group
     * @param target The invokable target
     * @param name The name.
     * @param stackSize The stack size.
     */
    public QThread(ThreadGroup group, Runnable target, String name, long stackSize) {
        super(group, target, name, stackSize);
        notifier = new Notifier();
        starting = notifier.starting;
        finished = notifier.finished;
    }


    /**
     * Called by the thread to invoke the runnable target.
     *
     * This method emits starting before the runnable  is called and finished after
     * the runnable is called. After the runnable is called and before finished is emitted the
     * thread will flush any pending events in this thread. This will ensure cleanup of objects
     * that are deleted via disposeLater() or similar.
     *
     * @see org.qtjambi.qt.core.QObject#disposeLater()
     */
    @Override
    public void run() {
        starting.emit();

        try {
            super.run();
        } finally {
            System.gc();
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());

            finished.emit();
        }
    }


    private class Notifier extends QSignalEmitter {
        public final Signal0 starting = new Signal0();
        public final Signal0 finished = new Signal0();
    }


    private Notifier notifier;
}
