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

package org.qtjambi.qt.core;

/**
 * The QSynchronousInvokable class is used internally by QCoreApplication.invokeLaterAndWait()
 * to synchronously execute a java.lang.Runnable from the gui-thread.
 *
 * @author eskil
 * @see QCoreApplication
 */
class QSynchronousInvokable extends QObject {
    static QEvent.Type SYNCHRONOUS_INVOKABLE_EVENT = QEvent.Type.resolve(QEvent.Type.User.value() + 1);

    private Runnable runnable;
    public QSynchronousInvokable(Runnable runnable) {
        disableGarbageCollection();
        if(QCoreApplication.instance().nativeId() != 0) {
            moveToThread(QCoreApplication.instance().thread());
            this.runnable = runnable;
        }

        if (runnable == null || Thread.currentThread().equals(QCoreApplication.instance().thread()))
            invoked = true;

    }

    private Boolean invoked = false;
    synchronized void waitForInvoked() {
        while (!invoked) {
            try {
                wait();
            } catch (InterruptedException e) {
                java.util.logging.Logger.getGlobal().logp(java.util.logging.Level.WARNING, this.getClass().getName(), "waitForInvoked()", "InterruptedException", e);
            }
        }

        invoked = false;
    }
	
	synchronized void waitForInvokedOrInterrupt() throws InterruptedException {
        while (!invoked) {
            wait();
        }

        invoked = false;
    }

    @Override
    public final boolean event(org.qtjambi.qt.core.QEvent e) {
        if (e.type() == SYNCHRONOUS_INVOKABLE_EVENT && runnable != null) {
            runnable.run();
            synchronized (this) {
                invoked = true;
                notifyAll();
            }
            return true;
        }
        return super.event(e);
    }
}
