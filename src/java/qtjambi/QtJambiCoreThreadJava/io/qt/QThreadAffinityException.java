/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt;

import io.qt.core.QObject;
import io.qt.internal.NativeAccess;


/**
 * The QThreadAffinityException class is thrown when a QObject is used outside
 * its own thread.
 *
 * Each QObject has thread affinity, a thread that it belongs to, which is
 * accessible through its thread() method. Accessing an object from outside
 * this thread is forbidden to avoid concurrency problems.
 *
 * Qt Jambi checks if threading affinity is violated in each member of each QObject subclass. It is
 * possible to disable this check by setting the VM runtime parameter
 * <code>io.qt.thread-check</code> to <code>false</code>.
 *
 * @See io.qt.core.QObject#thread()
 * @See <a href="../threads.html">Threading support in Qt</a>
 *
 * @author gunnar
 */
public class QThreadAffinityException extends RuntimeException {


    /**
     * Creates a new QThreadAffinityException with the given message, object and thread.
     * @param message Describes the affinity exception.
     * @param object The object that was accessed.
     * @param expectedThread The expected thread.
     * @param currentThread The thread from which the access was made.
     */
    @NativeAccess
    public QThreadAffinityException(String message, QObject object, Thread expectedThread, Thread currentThread) {
        super(message);
        this.object = object;
        this.expectedThread = expectedThread;
        this.currentThread = currentThread;
    }


    /**
     * Returns a string representation of this exception.
     * @return This exception as a string.
     */
    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(getMessage());
        if(object.isDisposed()) {
        	s.append(", object=").append(object.getClass().getName()).append("@").append(Integer.toHexString(System.identityHashCode(object)));
        }else {
        	s.append(", object=").append(object);
        }
    	s.append(", expectedThread=").append(expectedThread);
        s.append(", currentThread=").append(currentThread);
        return s.toString();
    }

    private final QObject object;
    private final Thread currentThread;
    private final Thread expectedThread;

    private static final long serialVersionUID = 1L;

    public QObject object() {
        return object;
    }


    public Thread currentThread() {
        return currentThread;
    }


    public Thread expectedThread() {
        return expectedThread;
    }
}
