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

import java.util.HashMap;

import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.xml.QDomElement;
import org.qtjambi.qt.xml.QDomNode;
import org.qtjambi.qt.xml.QDomNodeList;

class PalettePropertyHandler extends AbstractPropertyHandler {
    public Object create(QDomElement e) throws QUiLoaderException {
        QPalette p = QApplication.palette();
        parse(e.namedItem("active").toElement(), p, QPalette.ColorGroup.Active);
        parse(e.namedItem("inactive").toElement(), p, QPalette.ColorGroup.Inactive);
        parse(e.namedItem("disabled").toElement(), p, QPalette.ColorGroup.Disabled);

        return p;
    }

    private void parse(QDomElement groupElement, QPalette p, QPalette.ColorGroup group) throws QUiLoaderException {
        QDomNodeList list = groupElement.childNodes();
        for (int i=0; i<list.count(); ++i) {
            QDomNode node = list.at(i);
            if (node.nodeName().equals("colorrole")) {
                QDomElement roleElement = node.toElement();
                QPalette.ColorRole role = colorRoles.get(roleElement.attribute("role"));

                QDomElement brushElement = roleElement.namedItem("brush").toElement();

                QBrush brush;
                if (brushElement.attribute("brushstyle").equals("SolidPattern")) {
                    QColor color = (QColor) colorHandler.create(brushElement.namedItem("color").toElement());
                    brush = new QBrush(color);
                } else {
                    throw new QUiLoaderException("Unhandled brush style: " + brushElement.attribute("brushstyle"));
                }

                p.setBrush(group, role, brush);
            }
        }
    }

    private ColorPropertyHandler colorHandler = new ColorPropertyHandler();
    static HashMap<String, QPalette.ColorRole> colorRoles;
    static {
        colorRoles = new HashMap<String, QPalette.ColorRole>();
        for (QPalette.ColorRole role : QPalette.ColorRole.values()) {
            colorRoles.put(role.name(), role);
        }
    }
}
