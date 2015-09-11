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

package org.qtjambi.qt.qtjambi.util;

import java.util.Stack;

// !!NOTE!! This class can have no dependencies on Qt
//          as it is used by the NativeLibraryManager

/**
 * This class is an internal binding layer between JNI and
 * Java 1.5 API to make it possible to retrotranslate Qt Jambi
 * and make it work with JRE 1.4.
 *
 * @exclude
 *
 */
public class RetroTranslatorHelper {

    /**
     * Calls getEnumConstants method on given class
     *
     * @param cls The class on which to call the method
     * @return The result of calling the method
     */
    public static Object[] getEnumConstants(Class<?> cls) {
        return cls.getEnumConstants();
    }

    /**
     * Determines whether the given class is a subclass of java.lang.Enum
     *
     * @param cls The class to check
     * @return true if the class is a subclass of Enum, otherwise false
     */
    public static boolean isEnumType(Class<?> cls) {
        return Enum.class.isAssignableFrom(cls);
    }

    /**
     * If the given object is an enum, returns the result of
     * calling ordinal() on the enum.
     */
    @SuppressWarnings("rawtypes")
    public static int enumOrdinal(Object _enum) {
        return ((Enum) _enum).ordinal();
    }

    /**
     * Same as calling split(string, token, 0)
     */
    public static String[] split(String string, String token) {
        return split(string, token, 0);
    }

    /**
     * Splits a string on a specified token. The token is not
     * a regular expression. Works around missing String.split() in some
     * runtime environments and is also a lot faster than using
     * a regular expression for splitting on regular string tokens.
     */
    public static String[] split(String string, String token, int limit) {
        Stack<String> parts = new Stack<String>();

        int pos = 0, nextPos = 0;
        do {
            // Break if limit is reached
            if (limit > 0 && parts.size() == limit-1) {
                parts.push(string.substring(pos));
                break;
            } else {
                nextPos = string.indexOf(token, pos);

                String part = nextPos >= 0 ? string.substring(pos, nextPos) : string.substring(pos);
                // System.err.println("nextPos: " + nextPos + " part: " + part);
                if (part.length() != 0
                    || parts.isEmpty()
                    || parts.peek().length() != 0) {
                    parts.push(part);
                }
                pos = nextPos + token.length();
                // Always progress at least by one to avoid infinite loops
                if (pos == nextPos)
                    ++pos;
            }

        } while (nextPos >= 0 && pos <= string.length());

        // No trailing empty elements if limit==0
        if (limit == 0 && parts.peek().length() == 0)
            parts.pop();

        return parts.toArray(new String[] {});
    }
}
