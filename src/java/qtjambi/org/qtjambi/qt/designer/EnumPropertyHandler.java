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

import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.xml.QDomElement;

class EnumPropertyHandler extends AbstractPropertyHandler {

    public Object create(QDomElement e) throws QUiLoaderException {
        String name = childStringValue(e);
        return enumForValue(name);
    }

    @SuppressWarnings("unchecked")
    static Object enumForValue(String name) throws QUiLoaderException {

        Object hcValue = hardcodedValues.get(name);
        if (hcValue != null)
            return hcValue;

        int valuePos = name.lastIndexOf('.');
        int enumPos = name.lastIndexOf('.', valuePos-1);
        if (enumPos > 0 && valuePos > 0) {
            String className = name.substring(0, enumPos);
            String enumName = name.substring(enumPos+1, valuePos);
            String enumValue = name.substring(valuePos + 1);
            try {
                Class<? extends Enum> cl = (Class<? extends Enum>) Class.forName(className + '$' + enumName);
                return Enum.valueOf(cl, enumValue);
            } catch (Exception ex) {
                throw new QUiLoaderException("Converting enum '" + name + "' failed...", ex);
            }
        } else {
            throw new QUiLoaderException("Converting enum '" + name + "' failed", null);
        }
    }

    private static HashMap<String,Object> hardcodedValues = new HashMap<String,Object>();
    static {
        hardcodedValues.put("TopToolBarArea", Qt.ToolBarArea.TopToolBarArea.value());
        hardcodedValues.put("BottomToolBarArea", Qt.ToolBarArea.BottomToolBarArea.value());
        hardcodedValues.put("RightToolBarArea", Qt.ToolBarArea.RightToolBarArea.value());
        hardcodedValues.put("LeftToolBarArea", Qt.ToolBarArea.LeftToolBarArea.value());
    }

}
