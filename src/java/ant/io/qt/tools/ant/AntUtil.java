/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.tools.ant;

import org.apache.tools.ant.PropertyHelper;

/**
 * This class exists to help ANT 1.7.x to 1.8.x migration.
 * We know we maybe using deprecated API as we support both.
 * At this time the QtJambi project doesn't need or make use of the new features in
 *  1.8.x at all but 1.7.x is much less likely to see updates and 1.8.x is being
 *  provided as standard.
 *
 */
abstract class AntUtil {
    //@SuppressWarnings("deprecation")
    public static Object getProperty(PropertyHelper props, String attr) {
        //return props.getProperty((String) null, attr);   // ANT 1.7.x
        return props.getProperty(attr);   // ANT 1.8.x
    }

    public static String getPropertyAsString(PropertyHelper props, String attr) {
        Object objectValue = getProperty(props, attr);
        if(objectValue instanceof String)
            return (String) objectValue;
        return null;
    }

    //@SuppressWarnings("deprecation")
    public static void setNewProperty(PropertyHelper props, String name, String value) {
        //props.setNewProperty((String) null, name, value);   // ANT 1.7.x
        props.setNewProperty(name, value);   // ANT 1.8.x
    }

    //@SuppressWarnings("deprecation")
    public static void setProperty(PropertyHelper props, String name, String value) {
        //props.setProperty((String) null, name, value, false);   // ANT 1.7.x
        props.setProperty(name, value, false);   // ANT 1.8.x
    }

    //@SuppressWarnings("deprecation")
    public static void setProperty(PropertyHelper props, String name, String value, boolean verbose) {
        //props.setProperty((String) null, name, value, verbose);   // ANT 1.7.x
        props.setProperty(name, value, verbose);   // ANT 1.8.x
    }
}
