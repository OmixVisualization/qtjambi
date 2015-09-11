/****************************************************************************
 **
 ** (C) 2007-2009 Nokia. All rights reserved.
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

package org.qtjambi.qt.sql;

import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;
import java.util.Vector;

class QJdbcSqlDriver extends QSqlDriver
{
    private static QJdbcSqlDriverCreator creator;
    public static void initialize() {
        if (creator == null) {
            creator = new QJdbcSqlDriverCreator();
            QSqlDatabase.registerSqlDriver(QJdbc.ID, creator);
        }
    }

    QJdbcSqlDriver()
    {
        super();

        // we're usually kept by QSqlDatabase on the C++ side,
        // so prevent the garbage collector from nuking us
        // will be disabled in the disposed() slot
        this.disableGarbageCollection();
    }

    @Override
    public void close()
    {
        if (connection != null) {
            try {
                connection.close();
            } catch (SQLException e) {
                setError(e, tr("Unable to close connection"), QSqlError.ErrorType.ConnectionError);
            }
        }
        connection = null;
        setOpen(false);
    }

    @Override
    public Object handle()
    {
        return connection;
    }

    @Override
    public QSqlResult createResult()
    {
        return new QJdbcSqlResult(this, connection);
    }

    @Override
    public List<String> tables(QSql.TableType tableType)
    {
        Vector<String> list = new Vector<String>();
        if (connection == null)
            return list;

        try {
            DatabaseMetaData metaData = connection.getMetaData();

            String[] types = new String[1];
            switch (tableType) {
            case Tables:
                types[0] = "TABLE";
                break;
            case Views:
                types[0] = "VIEWS";
                break;
            case SystemTables:
                types[0] = "SYSTEM TABLE";
                break;
            case AllTables:
            default:
                types = null;
                break;
            }

            ResultSet rs = metaData.getTables(null, null, null, types);

            while (rs.next()) {
                list.add(rs.getString("TABLE_NAME"));
            }

            rs.close();

        } catch (SQLException ex) {
            setError(ex, tr("Unable to list tables"), QSqlError.ErrorType.ConnectionError);
            return new Vector<String>();
        }

        return list;
    }

    @Override
    public boolean hasFeature(QSqlDriver.DriverFeature f)
    {
        switch (f) {
        case PreparedQueries:
        case PositionalPlaceholders:
        case BLOB:
        case Unicode:
        case Transactions:
            return true;

        case LastInsertId:
            try {
                return connection != null && connection.getMetaData().supportsGetGeneratedKeys();
            } catch (SQLException e) {
                setError(e, "Can't determine availability of LastInsertId", QSqlError.ErrorType.UnknownError);
                return false;
            }

        case QuerySize: // we can only retrieve the fetch count - not the actual result set size
        case NamedPlaceholders: // JDBC supports only positional placeholders
        case BatchOperations: // JDBC can batch statements, but not a bunch of values
        case SimpleLocking: // don't know how to find that out - but shouldn't matter for JDBC
        case LowPrecisionNumbers: // Java has BigInts - no more double vs. string hassle
            return false;
        }

        return false;
    }

    @Override
    public boolean open(String db, String user, String password, String host, int port, String connOpts)
    {
        Connection con = null;
        try {
            con = DriverManager.getConnection(db, user, password);
        } catch (SQLException ex) {
            setError(ex, tr("Unable to find JDBC driver."), QSqlError.ErrorType.ConnectionError);
            setOpenError(true);
            return false;
        }
        if (con == null) {
            setOpenError(true);
            return false;
        }

        try {
            // our default behavior is to commit every statement
            con.setAutoCommit(true);
        } catch (SQLException ex) {
            setError(ex, tr("Unable to enable auto-commit"), QSqlError.ErrorType.ConnectionError);
            // this is annoying, but not fatal.
        }

        this.connection = con;

        setOpen(true);
        setOpenError(false);
        return true;
    }

    @Override
    public QSqlRecord record(String tableName)
    {
        if (connection == null)
            return new QSqlRecord();

        QSqlRecord res = new QSqlRecord();

        try {

            DatabaseMetaData metaData = connection.getMetaData();
            if (metaData == null)
                return new QSqlRecord();

            ResultSet resultSet = metaData.getColumns(null, null, tableName, null);

            while (resultSet.next()) {
                QSqlField f = new QSqlField(resultSet.getString(4),
                    QJdbcSqlUtil.javaTypeIdToVariantType(resultSet.getInt(5)));
                f.setLength(resultSet.getInt(7));
                f.setPrecision(resultSet.getInt(8));
                f.setRequiredStatus(QJdbcSqlUtil.toRequiredStatus(resultSet.getInt(11)));
                f.setSqlType(resultSet.getInt(5));
                f.setDefaultValue(resultSet.getObject(13));

                res.append(f);
            }

            resultSet.close();

        } catch (SQLException ex) {
            setError(ex, tr("Unable to retrieve database meta data"), QSqlError.ErrorType.ConnectionError);
            return new QSqlRecord();
        }

        return res;
    }

    @Override
    public QSqlIndex primaryIndex(String tableName) {
        try {
            DatabaseMetaData metaData = connection.getMetaData();
            if (metaData == null)
                return new QSqlIndex(tableName);

            ResultSet primaryKeys = metaData.getPrimaryKeys(null, null, tableName);

            QSqlIndex idx = new QSqlIndex(tableName);
            while (primaryKeys.next()) {

                String columnName = primaryKeys.getString("COLUMN_NAME");
                ResultSet resultSet = metaData.getColumns(null, null, tableName, columnName);

                idx.setName(primaryKeys.getString("PK_NAME"));

                // There should only be a single entry here, since we are asking for
                // a specific column
                if (resultSet.next()) {
                    QSqlField f = new QSqlField(resultSet.getString(4),
                        QJdbcSqlUtil.javaTypeIdToVariantType(resultSet.getInt(5)));
                    f.setLength(resultSet.getInt(7));
                    f.setPrecision(resultSet.getInt(8));
                    f.setRequiredStatus(QJdbcSqlUtil.toRequiredStatus(resultSet.getInt(11)));
                    f.setSqlType(resultSet.getInt(5));
                    f.setDefaultValue(resultSet.getObject(13));

                    idx.append(f);
                }

                resultSet.close();
            }

            primaryKeys.close();
            return idx;

        } catch (SQLException ex) {
            setError(ex, tr("Unable to get primary index"), QSqlError.ErrorType.ConnectionError);
            return new QSqlIndex(tableName);
        }
    }


    @Override
    public boolean beginTransaction()
    {
        if (connection == null)
            return false;

        try {
            connection.setAutoCommit(false);
        } catch (SQLException ex) {
            setError(ex, "Unable to disable autoCommit", QSqlError.ErrorType.ConnectionError);
            return false;
        }
        return true;
    }

    @Override
    public boolean commitTransaction()
    {
        if (connection == null)
            return false;

        try {
            connection.commit();
        } catch (SQLException ex) {
            setError(ex, tr("Unable to commit transaction"), QSqlError.ErrorType.ConnectionError);
            return false;
        }
        return true;
    }

    @Override
    public boolean rollbackTransaction()
    {
        if (connection == null)
            return false;

        try {
            connection.rollback();
        } catch (SQLException ex) {
            setError(ex, tr("Unable to roll back transaction"), QSqlError.ErrorType.ConnectionError);
            return false;
        }
        return true;
    }

    @Override
    protected void disposed()
    {
        // the C++ object is gone - probably because QSqlDatabase::removeDatabase was called.
        // say bye bye, then
        this.reenableGarbageCollection();

        super.disposed();
    }

    private void setError(SQLException e, String driverText, QSqlError.ErrorType t)
    {
        setLastError(QJdbcSqlUtil.getError(e, driverText, t));
    }

    private Connection connection = null;
}

