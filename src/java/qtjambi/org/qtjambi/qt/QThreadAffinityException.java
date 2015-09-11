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

/**
 *
 */
package org.qtjambi.qt;

import org.qtjambi.qt.core.QObject;


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
 * <code>org.qtjambi.qt.thread-check</code> to <code>false</code>.
 *
 * @See org.qtjambi.qt.core.QObject#thread()
 * @See <a href="../threads.html">Threading support in Qt</a>
 *
 * @author gunnar
 */
public class QThreadAffinityException extends RuntimeException {


    /**
     * Creates a new QThreadAffinityException with the given message, object and thread.
     * @param message Describes the affinity exception.
     * @param object The object that was accessed.
     * @param thread The thread from which the access was made.
     */
    public QThreadAffinityException(String message, QObject object, Thread thread) {
        super(message);
        this.object = object;
        this.thread = thread;
    }


    /**
     * Returns a string representation of this exception.
     * @return This exception as a string.
     */
    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(getMessage());
        s.append(", object=").append(object);
        s.append(", objectThread=").append(object.thread());
        s.append(", currentThread=").append(thread);
        return s.toString();
    }

    private QObject object;
    private Thread thread;

    private static final long serialVersionUID = 1L;
}
