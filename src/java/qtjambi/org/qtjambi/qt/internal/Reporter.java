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

package org.qtjambi.qt.internal;

public class Reporter {

    public void report(String a) {
        if (!report) return;
        buffer.append(a);
        buffer.append('\n');
    }

    public void report(String a, String b) {
        if (!report) return;
        buffer.append(a);
        buffer.append(b);
        buffer.append('\n');
    }

    public void report(String a, String b, String c) {
        if (!report) return;
        buffer.append(a);
        buffer.append(b);
        buffer.append(c);
        buffer.append('\n');
    }

    public void report(String a, String b, String c, String d) {
        if (!report) return;
        buffer.append(a);
        buffer.append(b);
        buffer.append(c);
        buffer.append(d);
        buffer.append('\n');
    }

    public void report(String a, String b, String c, String d, String e) {
        if (!report) return;
        buffer.append(a);
        buffer.append(b);
        buffer.append(c);
        buffer.append(d);
        buffer.append(e);
        buffer.append('\n');
    }

    public void report(String a, String b, String c, String d, String e, String f) {
        if (!report) return;
        buffer.append(a);
        buffer.append(b);
        buffer.append(c);
        buffer.append(d);
        buffer.append(e);
        buffer.append(f);
        buffer.append('\n');
    }

    public void report(String a, String b, String c, String d, String e, String f, String g) {
        if (!report) return;
        buffer.append(a);
        buffer.append(b);
        buffer.append(c);
        buffer.append(d);
        buffer.append(e);
        buffer.append(f);
        buffer.append(g);
        buffer.append('\n');
    }

    public void report(String a, String b, String c, String d, String e, String f, String g, String h) {
        if (!report) return;
        buffer.append(a);
        buffer.append(b);
        buffer.append(c);
        buffer.append(d);
        buffer.append(e);
        buffer.append(f);
        buffer.append(g);
        buffer.append(h);
        buffer.append('\n');
    }

    public String toString() {
        return (old != null) ? old.toString() + "\n" + buffer.toString() : buffer.toString();
    }

    public void setReportEnabled(boolean e) {
        report = e;
    }

    public boolean isReportEnabled() {
        return report;
    }

    /**
     * Extracts the last batch of reports made since the last time
     * this function was called.
     * @returns The last batch of reprts...
     */
    public String recentReports() {
        String batch = buffer.toString();
        if (old == null)
            old = buffer;
        else
            old.append(buffer);
        buffer = new StringBuilder();
        return batch;
    }

    private boolean report = true;
    private StringBuilder buffer = new StringBuilder();
    private StringBuilder old;
}
