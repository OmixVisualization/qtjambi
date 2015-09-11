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

package org.qtjambi.tools.designer;

import java.lang.reflect.Constructor;

import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.*;

public class CustomWidget {

    public CustomWidget(Class<? extends QWidget> pluginClass) throws NoSuchMethodException {
        this.pluginClass = pluginClass;
        constructor = (Constructor<? extends QWidget>) pluginClass.getConstructor(QWidget.class);
    }

    public QWidget createWidget(QWidget parent) {
        QWidget widget = null;

        // Try the parent constructor first...
        try {
            widget = constructor.newInstance(parent);
        } catch (Exception e) {
            e.printStackTrace();
        }

        // Avoid Designer seg-fault when the constructor throws an exception
        if (widget == null)
            widget = new QWidget(parent);

        return widget;
    }

    public boolean isContainer() {
        return container;
    }

    public void setContainer(boolean container) {
        this.container = container;
    }

    public String group() {
        return group;
    }

    public void setGroup(String group) {
        this.group = group;
    }

    public QIcon icon() {
        return icon;
    }

    public void setIcon(QIcon icon) {
        this.icon = icon;
    }

    public String includeFile() {
        if (includeFile.length() == 0)
            return pluginClass.getPackage().getName();
        else
            return includeFile;
    }

    public void setIncludeFile(String includeFile) {
        this.includeFile = includeFile;
    }

    public String name() {
        if (name.length() == 0)
            return pluginClass.getSimpleName();
        else
            return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Class<? extends QWidget> pluginClass() {
        return pluginClass;
    }

    public String tooltip() {
        return tooltip;
    }

    public void setTooltip(String tooltip) {
        this.tooltip = tooltip;
    }

    public String whatsThis() {
        return whatsThis;
    }

    public void setWhatsThis(String whatsThis) {
        this.whatsThis = whatsThis;
    }

    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append("CustomWidget(").append(pluginClass.toString());

        if (name != null && name.length() != 0) s.append(",name=").append(name);
        if (group != null && group.length() != 0) s.append(",group=").append(group);
        if (tooltip != null && tooltip.length() != 0) s.append(",toolTip=").append(tooltip);
        if (whatsThis != null && whatsThis.length() != 0) s.append(",whatsThis=").append(whatsThis);
        if (includeFile != null && includeFile.length() != 0) s.append(",include=").append(includeFile);
        if (icon != null && !icon.isNull()) s.append(",icon=").append(icon);

        s.append(")");

        return s.toString();
    }

    private Class<? extends QWidget> pluginClass;
    private String name;
    private boolean container;
    private String group;
    private String tooltip;
    private String whatsThis;
    private String includeFile;
    private QIcon icon;
    private Constructor<? extends QWidget> constructor;
}

