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

package org.qtjambi.extensions.signalhandler;

import org.qtjambi.qt.QSignalEmitter;

/**
 * Signal handlers are a convenience class that provides compile time type checking of signal / slot
 * connections.
 */
public abstract class QSignalHandler9<A, B, C, D, E, F, G, H, I> {

    /**
     * Creates a new signal handler that sets up a connection from the input signal to this object.
     * @param signal The signal to connect to.
     */
    public QSignalHandler9(QSignalEmitter.Signal9<A, B, C, D, E, F, G, H, I> signal) {
        signal.connect(this, "handle(Object, Object, Object, Object, Object, Object, Object, Object, Object)");
    }

    /**
     * Reimplement this function to handle responses to the signal this handler is connected to..
     * @param arg1 The first signal argument.
     * @param arg2 The second signal argument.
     * @param arg3 The third signal argument.
     * @param arg4 The fourth signal argument.
     * @param arg5 The fifth signal argument.
     * @param arg6 The sixth signal argument.
     * @param arg7 The seventh signal argument.
     * @param arg8 The eighth signal argument.
     * @param arg9 The ninth signal argument.
     */
    public abstract void handle(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9);
}
