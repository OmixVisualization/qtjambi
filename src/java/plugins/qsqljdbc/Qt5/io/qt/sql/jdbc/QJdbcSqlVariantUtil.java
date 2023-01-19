/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

package io.qt.sql.jdbc;

import java.net.MalformedURLException;
import java.net.URL;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Types;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.HashMap;

import io.qt.core.QDate;
import io.qt.core.QDateTime;
import io.qt.core.QTime;
import io.qt.core.QUrl;
import io.qt.core.QVariant;
import io.qt.sql.QSqlError;
import io.qt.sql.QSqlField;

class QJdbcSqlVariantUtil
{
    private static HashMap<String, QVariant.Type> javaToVariant = new HashMap<String, QVariant.Type>();

    static {
                                                        // QVariant::
        javaToVariant.put(null, QVariant.Type.Invalid);                     // Invalid
        javaToVariant.put("java.lang.String", QVariant.Type.String);      // String
        javaToVariant.put("java.lang.Integer", QVariant.Type.Int);      // Int
        javaToVariant.put("java.lang.Short", QVariant.Type.Int);        // Int
        javaToVariant.put("java.lang.Byte", QVariant.Type.Int);         // Int
        javaToVariant.put("java.lang.Boolean", QVariant.Type.Bool);      // Bool
        javaToVariant.put("java.lang.Byte[]", QVariant.Type.ByteArray);      // ByteArray
        javaToVariant.put("java.util.Date", QVariant.Type.Date);        // Date
        javaToVariant.put("java.sql.Date", QVariant.Type.Date);         // Date
        javaToVariant.put("java.lang.Float", QVariant.Type.Double);        // Double
        javaToVariant.put("java.lang.Double", QVariant.Type.Double);       // Double
        javaToVariant.put("java.lang.Long", QVariant.Type.LongLong);         // LongLong
        javaToVariant.put("java.sql.Time", QVariant.Type.Time);         // Time
        javaToVariant.put("java.sql.TimeStamp", QVariant.Type.DateTime);    // DateTime
        javaToVariant.put("java.net.Url", QVariant.Type.Url);          // Url
    }

    static QVariant.Type javaTypeToVariantType(String javaType)
    {
    	QVariant.Type variantType = javaToVariant.get(javaType);
        if (variantType == null)
            return QVariant.Type.UserType;
        return variantType;
    }

    static QVariant.Type javaTypeIdToVariantType(int variantType)
    {
        // can't use QVariant types directly, since we don't want a dependency to QtGui
        switch (variantType) {
        case Types.VARCHAR:
        case Types.CHAR:
            return QVariant.Type.String; // String
        case Types.INTEGER:
        case Types.SMALLINT:
        case Types.TINYINT:
            return QVariant.Type.Int; // Int
        case Types.BOOLEAN:
        case Types.BIT:
            return QVariant.Type.Bool; // Bool
        case Types.BIGINT:
        case Types.DECIMAL:
            return QVariant.Type.LongLong; // LongLong
        case Types.DATE:
            return QVariant.Type.Date; // Date
        case Types.DOUBLE:
        case Types.FLOAT:
        case Types.NUMERIC:
        case Types.REAL:
            return QVariant.Type.Double; // Double
        case Types.TIME:
            return QVariant.Type.Time; // Time
        case Types.TIMESTAMP:
            return QVariant.Type.DateTime; // DateTime
        }

        // System.out.println("Unknown type id: " + variantType);

        return QVariant.Type.UserType; // QVariant.UserType
    }
}
