/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBISQL_HASHES_H
#define QTJAMBISQL_HASHES_H

#include <QtSql/QtSql>
#include <QtJambi/Global>

inline hash_type qHash(const QSqlError &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.type()));
    hashCode = hashCode * 31 + qHash(value.driverText());
    hashCode = hashCode * 31 + qHash(value.databaseText());
    hashCode = hashCode * 31 + qHash(value.nativeErrorCode());
    hashCode = hashCode * 31 + qHash(value.text());
    hashCode = hashCode * 31 + qHash(value.isValid());
    return hashCode;
}

inline hash_type qHash(const QSqlField &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.type()));
    hashCode = hashCode * 31 + qHash(int(value.requiredStatus()));
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.tableName());
    hashCode = hashCode * 31 + qHash(value.isReadOnly());
    hashCode = hashCode * 31 + qHash(value.isAutoValue());
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.length());
    hashCode = hashCode * 31 + qHash(value.precision());
    hashCode = hashCode * 31 + qHash(value.typeID());
    hashCode = hashCode * 31 + qHash(value.isGenerated());
    hashCode = hashCode * 31 + qHash(value.isValid());
    return hashCode;
}

inline hash_type qHash(const QSqlRecord &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.count());
    for(int i=0; i<value.count(); ++i){
        hashCode = hashCode * 31 + qHash(value.field(i));
    }
    return hashCode;
}

#endif // QTJAMBISQL_HASHES_H
