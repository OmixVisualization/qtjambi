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

import java.lang.reflect.Method;

import org.qtjambi.qt.concurrent.QtConcurrent;
import org.qtjambi.qt.core.QFuture;
import org.qtjambi.qt.core.QFutureVoid;

public class RunFunction {
    public static void hello(String name) {
        System.out.println("Hello " + name + " from " + Thread.currentThread());
    }

    public static String helloReturned(String name) {
        return "Hello " + name + " from " + Thread.currentThread();
    }

    public static void main(String args[]) {
        Method hello = null;
        Method helloReturned = null;
        try {
            hello = RunFunction.class.getMethod("hello", String.class);
            helloReturned = RunFunction.class.getMethod("helloReturned", String.class);
        } catch (Exception e) {
            return ;
        }

        QFutureVoid f1 = QtConcurrent.runVoidMethod(null, hello, "James");
        QFutureVoid f2 = QtConcurrent.runVoidMethod(null, hello, "William");
        QFuture<String> f3 = QtConcurrent.run(null, helloReturned, "Virginia");
        f1.waitForFinished();
        f2.waitForFinished();
        f3.waitForFinished();

        System.out.println(f3.results().get(0));
    }

}
