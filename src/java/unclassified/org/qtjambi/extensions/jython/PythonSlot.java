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

package org.qtjambi.extensions.jython;

import org.python.core.*;

class PythonSlot {

    public PythonSlot(PyMethod slot) {
        this.slot = slot;
    }

    private PyJavaInstance wrap(Object x) { return new PyJavaInstance(x); }

    public void execute_0() {
        slot.__call__();
    }

    public void execute_1(Object a) {
        slot.__call__(wrap(a));
    }

    public void execute_2(Object a, Object b) {
        slot.__call__(wrap(a), wrap(b));
    }

    public void execute_3(Object a, Object b, Object c) {
        slot.__call__(wrap(a), wrap(b), wrap(c));
    }

    public void execute_4(Object a, Object b, Object c, Object d) {
        slot.__call__(wrap(a), wrap(b), wrap(c), wrap(d));
    }

    private void genericExecute(Object ... vals) {
        PyObject pys[] = new PyObject[vals.length];
        for (int i=0; i<vals.length; ++i) {
            pys[i] = wrap(vals[i]);
        }
        slot.__call__(pys);
    }

    public void execute_5(Object a, Object b, Object c, Object d, Object e) {
        genericExecute(a, b, c, d, e);
    }

    public void execute_6(Object a, Object b, Object c, Object d, Object e, Object f) {
        genericExecute(a, b, c, d, e, f);
    }

    public void execute_7(Object a, Object b, Object c, Object d, Object e, Object f, Object g) {
        genericExecute(a, b, c, d, e, f, g);
    }

    public void execute_8(Object a, Object b, Object c, Object d, Object e, Object f, Object g, Object h) {
        genericExecute(a, b, c, d, e, f, g, h);
    }

    public void execute_9(Object a, Object b, Object c, Object d, Object e, Object f, Object g, Object h, Object i) {
        genericExecute(a, b, c, d, e, f, g, h, i);
    }

    public static String signature(int count) {
        StringBuilder s = new StringBuilder();
        s.append("execute_").append(count).append('(');
        for (int i=0; i<count; ++i) {
            if (i != 0)
                s.append(',');
            s.append("Object");
        }
        s.append(')');
        return s.toString();
    }


    private PyMethod slot;
}

