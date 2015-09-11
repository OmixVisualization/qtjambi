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

package org.qtjambi.qt.designer;

import org.qtjambi.qt.xml.QDomElement;
import org.qtjambi.qt.xml.QDomNode;


abstract class AbstractPropertyHandler {
    public abstract Object create(QDomElement e) throws QUiLoaderException;

    public int childIntValue(QDomElement e, String s) {
        assert !e.isNull();
        QDomNode n = namedChildContent(e, s);
        return n.isNull() ? 0 : Integer.parseInt(n.nodeValue());
    }

    public int childIntValue(QDomElement e) {
        assert !e.isNull();
        if (!e.isNull()) {
            QDomNode n = e.firstChild();
            if (!n.isNull())
                return Integer.parseInt(n.nodeValue());
        }
        return 0;
    }

    public String childStringValue(QDomElement e, String s) {
        assert !e.isNull();
        QDomNode n = namedChildContent(e, s);
        return n.isNull() ? "" : n.nodeValue();
    }

    public String childStringValue(QDomElement e) {
        assert !e.isNull();
        if (!e.isNull()) {
            QDomNode n = e.firstChild();
            if (!n.isNull())
                return n.nodeValue();
        }
        return "";
    }

    public boolean childBoolValue(QDomElement e, String s) {
        assert !e.isNull();
        QDomNode n = namedChildContent(e, s);
        return !n.isNull() && n.nodeValue().equals("true");
    }

    public boolean childBoolValue(QDomElement e) {
        assert !e.isNull();
        if (!e.isNull()) {
            QDomNode n = e.firstChild();
            if (!n.isNull())
                return n.nodeValue().equals("true");
        }
        return false;
    }

    private QDomNode namedChildContent(QDomElement e, String s) {
        assert !e.isNull();
        QDomNode n = e;
        if (!e.isNull()) n = e.namedItem(s);
        if (!n.isNull()) n = n.firstChild();
        return n;
    }

}
