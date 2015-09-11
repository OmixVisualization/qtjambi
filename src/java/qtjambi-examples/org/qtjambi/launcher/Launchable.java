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

package org.qtjambi.launcher;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.examples.QtJambiExample;

import java.lang.reflect.*;
import org.qtjambi.qt.widgets.*;

public class Launchable {

    private static class SourceFormatter {
    private static String KEYWORDS[] = new String[] {
        "for ",
        "if ",
        "switch ",
        " int ",
        "const",
        "void ",
        "case ",
        "double ",
        "static",
        "new",
        "this",
        "package",
        "true",
        "false",
        "public",
        "protected",
        "private",
        "final",
        "native",
        "import",
        "extends",
        "implements",
        "else",
        "class",
        "super",
    };

    static public String format(String source) {
        int start = -1;
        int end = -1;
        boolean stop = false;
        
        QRegExp regExp = new QRegExp("//!\\s*\\[.*\\][^\\n]*\\n");
        regExp.setMinimal(true);
        while (regExp.indexIn(source) != -1) {
            source = source.replace(regExp.cap(), "");
        }

        do {
            start = source.indexOf("// REMOVE-START");
            end = source.indexOf("// REMOVE-END");
            if (start != -1 && end > start) {
                source = source.substring(0, start) + source.substring(end + 13);
            } else {
                stop = true;
            }
        } while (!stop);

        source = source.replace("&", "&amp;");
        source = source.replace("<", "&lt;");
        source = source.replace(">", "&gt;");

        source = source.replace("\t", "   ");

        for (int i=0; i<KEYWORDS.length; ++i) {
            String keyword = KEYWORDS[i];
            source = source.replace(keyword, "<font color=olive>" + keyword + "</font>");
        }
        source = source.replace("(int ", "(<font color=olive><b>int </b></font>");
        source = source.replaceAll("(\\d\\d?)", "<font color=navy>$1</font>");

        String commentRe = "(//+[.[^\n]]*\n)";
        source = source.replaceAll(commentRe, "<font color=darkgreen><i>$1</i></font>");

        String stringLiteralRe = "(\".+\")";
        source = source.replaceAll(stringLiteralRe, "<font color=green>$1</font>");

        source = "<html style=\"white-space:pre-wrap;font-family:courier new\">" + source + "</html>";
        return source;
    }
} // end of SourceFormatter


    private QWidget m_widget;
    private String m_description;
    private String m_name;
    private String m_className;
    private String m_source;

    private Launchable(String name, String className) {
        m_name = name;
        m_className = className;
    }

    public QWidget widget() {
    if (m_widget == null)
        createWidget();
    return m_widget;
    }

    public String name() {
        return m_name;
    }

    public String description() {
    if (m_description == null)
        loadDescription();
    return m_description;

    }

    public String source() {
    if (m_source == null)
        loadSource();
    return m_source;
    }

    public void killWidget() {
        m_widget.close();
        m_widget.disposeLater();
        m_widget = null;
    }

    private final String resourceFile(String fileType) {
    QFile f = new QFile("classpath:" + m_className.replace(".", "/") + "." + fileType);
    if (f.exists() && f.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly, QFile.OpenModeFlag.Text)))
        return f.readAll().toString();
    return null;
    }

    /**
     * Searches for the description and loads it if possible
     */
    private void loadDescription() {
    m_description = resourceFile("html");
    if (m_description == null)
        m_description = "<h1 align=\"center\">" + m_name + "</h1>" +
        "<p>The '<i>" + m_name + "</i>' demo does not have a description.</p>" +
        "<p>Investigate this demo further:<ul><li>Run it, by clicking the '<i>Launch</i>' button.</li>" +
        "<li>Look at the source code, by clicking the '<i>View Source</i>' button.</li></ul></p>";
    }

    /**
       Searches for the source file and loads and HTMLifies it if possible.
    */
    private void loadSource() {
    m_source = resourceFile("java");
        if (m_source == null) {
        m_source = "<i>No source</i>";
    } else {
        m_source = SourceFormatter.format(m_source);
    }
    }

    private void createWidget() {
    if (m_widget != null)
        throw new RuntimeException("widget shouldn't exist at this point");

    try {
        Class<?> cl = Class.forName(m_className);

        try {
        cl.getConstructor();
        m_widget = (QWidget) cl.newInstance();
        } catch(Exception e) {
        Constructor<?> constructor = cl.getConstructor(QWidget.class);
        m_widget = (QWidget) constructor.newInstance((QWidget) null);
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
    }

    public static Launchable create(String className) {
    try {
            Class<?> cl = Class.forName(className);

            if (Modifier.isPublic(cl.getModifiers()) && QWidget.class.isAssignableFrom(cl)) {
                QtJambiExample info = cl.getAnnotation(QtJambiExample.class);
                if (info != null) {

                    Constructor<?> constructor = null;
                    try {
                        constructor = cl.getConstructor();
                    } catch (Exception e) {
                        constructor = cl.getConstructor(QWidget.class);
                    }

                    String visibleName = className;

                    boolean canIndeed = false;
                    String can = info.canInstantiate();
                    if (can.equals("false") || can.equals("no")) {
                        canIndeed = false;
                    } else if (can.startsWith("call-static-method:")) {
                        try {
                            Method checkMethod = cl.getMethod(can.substring(19));
                            canIndeed = (Boolean) checkMethod.invoke(cl);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    } else { // Nothing specified or something else...
                        canIndeed = true;
                    }

                    if (!canIndeed)
                        return null;

                    if (info.name().length() > 0)
                        visibleName = info.name();

                    if (constructor != null
                            && Modifier.isPublic(constructor.getModifiers())) {
                        Launchable l = new Launchable(visibleName, className);
                        return l;
                    }

                }

            }
        } catch (Throwable e) {
            System.out.println("failed: " + className + ": "
                    + e.getClass().getName() + "; " + e.getMessage());
        }

        return null;
    }
}
