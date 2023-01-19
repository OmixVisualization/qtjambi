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

import java.sql.Types;
import java.util.HashMap;

import io.qt.core.QMetaType;

class QJdbcSqlVariantUtil
{
    private static HashMap<String, QMetaType.Type> javaToVariant = new HashMap<>();

    static {
                                                        // QVariant::
        javaToVariant.put(null, QMetaType.Type.UnknownType);                     // Invalid
        javaToVariant.put("java.lang.String", QMetaType.Type.QString);      // String
        javaToVariant.put("java.lang.Integer", QMetaType.Type.Int);      // Int
        javaToVariant.put("java.lang.Short", QMetaType.Type.Int);        // Int
        javaToVariant.put("java.lang.Byte", QMetaType.Type.Int);         // Int
        javaToVariant.put("java.lang.Boolean", QMetaType.Type.Bool);      // Bool
        javaToVariant.put("java.lang.Byte[]", QMetaType.Type.QByteArray);      // ByteArray
        javaToVariant.put("java.util.Date", QMetaType.Type.QDate);        // Date
        javaToVariant.put("java.sql.Date", QMetaType.Type.QDate);         // Date
        javaToVariant.put("java.lang.Float", QMetaType.Type.Double);        // Double
        javaToVariant.put("java.lang.Double", QMetaType.Type.Double);       // Double
        javaToVariant.put("java.lang.Long", QMetaType.Type.LongLong);         // LongLong
        javaToVariant.put("java.sql.Time", QMetaType.Type.QTime);         // Time
        javaToVariant.put("java.sql.TimeStamp", QMetaType.Type.QDateTime);    // DateTime
        javaToVariant.put("java.net.Url", QMetaType.Type.QUrl);          // Url
    }

    static QMetaType javaTypeToVariantType(String javaType)
    {
    	QMetaType.Type variantType = javaToVariant.get(javaType);
        if (variantType == null)
            return new QMetaType();
        return new QMetaType(variantType);
    }

    static QMetaType javaTypeIdToVariantType(int variantType)
    {
        // can't use QVariant types directly, since we don't want a dependency to QtGui
        switch (variantType) {
        case Types.VARCHAR:
        case Types.CHAR:
            return new QMetaType(QMetaType.Type.QString); // String
        case Types.INTEGER:
        case Types.SMALLINT:
        case Types.TINYINT:
            return new QMetaType(QMetaType.Type.Int); // Int
        case Types.BOOLEAN:
        case Types.BIT:
            return new QMetaType(QMetaType.Type.Bool); // Bool
        case Types.BIGINT:
        case Types.DECIMAL:
            return new QMetaType(QMetaType.Type.LongLong); // LongLong
        case Types.DATE:
            return new QMetaType(QMetaType.Type.QDate); // Date
        case Types.DOUBLE:
        case Types.FLOAT:
        case Types.NUMERIC:
        case Types.REAL:
            return new QMetaType(QMetaType.Type.Double); // Double
        case Types.TIME:
            return new QMetaType(QMetaType.Type.QTime); // Time
        case Types.TIMESTAMP:
            return new QMetaType(QMetaType.Type.QDateTime); // DateTime
        }

        // System.out.println("Unknown type id: " + variantType);

        return new QMetaType(); // QVariant.UserType
    }
}
