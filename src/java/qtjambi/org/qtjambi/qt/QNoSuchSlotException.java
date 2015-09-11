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

import java.lang.reflect.Method;
import java.util.Iterator;
import java.util.Vector;

import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

public class QNoSuchSlotException extends ConnectionException {
    private static final long serialVersionUID = 1L;
    private Object reciver;
    private String slotSignature;
    private String message;

    public QNoSuchSlotException(String message) {
        if (message != null)
            this.message = message;
        else
            this.message = "";
    }

    public QNoSuchSlotException(Object reciver, String slotSignature) {
        this.reciver = reciver;
        this.slotSignature = slotSignature;
    }

    @Override
    public String getMessage() {
        if (message != null)
            return message;
        if (slotSignature.equals("")) {
            return "Slot can't be an empty string";
        } else {
            String res = "\n\nCould not find slot with signature: " + slotSignature + "\n";

            Vector<Method> possibleMethods = findPossibleFunctionRecursive(reciver, slotSignature);
            if(!possibleMethods.isEmpty()) {
                res += "Possible matching methods:\n";

                for (Iterator<Method> iter = possibleMethods.iterator(); iter.hasNext();) {
                    Method method = iter.next();

                    res += "   " + methodToString(method) + "\n";
                }
            }
            return res;
        }
    }

    private String methodToString(Method method) {
        Class<?> a[] = method.getParameterTypes();

        String args = "";
        for (int i = 0; i < a.length; ++i) {
            Class<?> t = a[i];

            String brackets = "";
            if (t.isArray()) {
                do {
                    t = t.getComponentType();
                    brackets += "[]";
                } while (t.isArray());
            }
            args += (i > 0 ? ", " : "") + t.getName() + brackets;
        }
        return method.getDeclaringClass().getName() + "." + method.getName() + "(" + args + ")";
    }


    private static Vector<Method> findPossibleFunctionRecursive(Object reciver, String signature) {
        Class<?> cls = reciver.getClass();

        int pos = signature.indexOf('(');
        String name = signature.substring(0, pos).trim();
        int pos2 = signature.indexOf(')', pos);
        String strTypes = signature.substring(pos + 1, pos2).trim();

        String argumentTypes[];

        if (strTypes.length() == 0)
            argumentTypes = new String[0];
        else
            argumentTypes = RetroTranslatorHelper.split(strTypes, ",");

        for (int i = 0; i < argumentTypes.length; ++i)
            argumentTypes[i] = argumentTypes[i].replace(" ", "");

        return findPossibleFunctionRecursiveHelper(cls, name, argumentTypes, new Vector<Method>());
    }

    private static Vector<Method> findPossibleFunctionRecursiveHelper(Class<?> cls,
            String functionName, String argumentTypes[], Vector<Method> res) {
        Method methods[] = cls.getDeclaredMethods();

        for (Method m : methods) {
            if (!m.getName().equalsIgnoreCase(functionName))
                continue;

            Class<?> a[] = m.getParameterTypes();
            if (a.length != argumentTypes.length)
                continue;

            res.add(m);
        }

        cls = cls.getSuperclass();
        if (cls == null)
            return res;
        else
            return findPossibleFunctionRecursiveHelper(cls, functionName, argumentTypes, res);
    }
}
