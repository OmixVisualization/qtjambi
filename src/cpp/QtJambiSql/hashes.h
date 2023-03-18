/****************************************************************************
**
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

#ifndef QTJAMBISQL_HASHES_H
#define QTJAMBISQL_HASHES_H

#include <QtSql/QtSql>
#include <QtJambi/Global>

inline hash_type qHash(const QSqlError &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.type()));
    seed = hash(seed, value.driverText());
    seed = hash(seed, value.databaseText());
    seed = hash(seed, value.nativeErrorCode());
    seed = hash(seed, value.text());
    seed = hash(seed, value.isValid());
    return seed;
}

inline hash_type qHash(const QSqlField &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.type()));
    seed = hash(seed, int(value.requiredStatus()));
    seed = hash(seed, value.name());
    seed = hash(seed, value.tableName());
    seed = hash(seed, value.isReadOnly());
    seed = hash(seed, value.isAutoValue());
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.length());
    seed = hash(seed, value.precision());
    seed = hash(seed, value.typeID());
    seed = hash(seed, value.isGenerated());
    seed = hash(seed, value.isValid());
    return seed;
}

inline hash_type qHash(const QSqlRecord &value, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.count());
    for(int i=0; i<value.count(); ++i){
        seed = hash(seed, value.field(i));
    }
    return seed;
}

#endif // QTJAMBISQL_HASHES_H
