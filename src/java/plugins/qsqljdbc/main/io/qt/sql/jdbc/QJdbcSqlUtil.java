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
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.core.QDate;
import io.qt.core.QDateTime;
import io.qt.core.QTime;
import io.qt.core.QUrl;
import io.qt.sql.QSqlError;
import io.qt.sql.QSqlField;

class QJdbcSqlUtil
{
    static QSqlField.RequiredStatus toRequiredStatus(int isNullable)
    {
        switch (isNullable) {
        case ResultSetMetaData.columnNoNulls:
            return QSqlField.RequiredStatus.Required;
        case ResultSetMetaData.columnNullable:
            return QSqlField.RequiredStatus.Optional;
        case ResultSetMetaData.columnNullableUnknown:
        default:
            return QSqlField.RequiredStatus.Unknown;
        }
    }

	static QSqlError getError(SQLException ex, String driverText, QSqlError.ErrorType t)
    {
		Logger.getLogger("io.qt.sql.jdbc").log(Level.SEVERE, driverText, ex);
        StringBuilder dbText = new StringBuilder();
        SQLException e = ex;
        int errorCode = 0;

        while (e != null) {
            if (dbText.length() != 0)
                dbText.append(", ");
            dbText.append(e.getMessage());
            if(e.getCause()!=null 
            		&& e.getCause().getMessage()!=null 
            		&& !e.getCause().getMessage().isEmpty()) {
            	dbText.append(", caused by: ").append(e.getCause().getMessage());
            }
            if(e.getSQLState()!=null)
                dbText.append(" (").append(e.getSQLState()).append(")");
                
            errorCode = e.getErrorCode(); // last one wins
            e = e.getNextException();
        }

        return new QSqlError(driverText, dbText.toString(), t, ""+errorCode);
    }

    static Object javaToQt(Object input) {
        if (input instanceof java.sql.Date) {
            java.sql.Date date = (java.sql.Date) input;
            GregorianCalendar c = new GregorianCalendar();
            c.setTime(date);

            return new QDate(c.get(Calendar.YEAR),
                             c.get(Calendar.MONTH)+1,
                             c.get(Calendar.DAY_OF_MONTH));

        } else if (input instanceof java.sql.Time) {

            java.sql.Time time = (java.sql.Time) input;
            GregorianCalendar c = new GregorianCalendar();
            c.setTime(time);

            return new QTime(c.get(Calendar.HOUR),
                             c.get(Calendar.MINUTE)+1,
                             c.get(Calendar.SECOND));

        } else if (input instanceof java.util.Date) {
            java.util.Date date = (java.util.Date) input;
            GregorianCalendar c = new GregorianCalendar();
            c.setTime(date);

            return new QDateTime(new QDate(c.get(Calendar.YEAR),
                                           c.get(Calendar.MONTH)+1,
                                           c.get(Calendar.DAY_OF_MONTH)),
                                 new QTime(c.get(Calendar.HOUR_OF_DAY),
                                           c.get(Calendar.MINUTE),
                                           c.get(Calendar.SECOND),
                                           c.get(Calendar.MILLISECOND)));

        } else if (input instanceof URL) {

            URL url = (URL) input;
            return new QUrl(url.toString());

        } else {
            return input;
        }

    }

    @SuppressWarnings("deprecation")
    static Object qtToJava(Object input) {
        if (input instanceof QDateTime) {
            QDateTime dt = (QDateTime) input;

            QDate date = dt.date();
            QTime time = dt.time();

            return new java.sql.Timestamp(date.year() - 1900,
                                          date.month() - 1,
                                          date.day(),
                                          time.hour(),
                                          time.minute(),
                                          time.second(),
                                          time.msec());
        } else if (input instanceof QDate) {

            QDate date = (QDate) input;
            return new java.sql.Date(date.year() - 1900,
                                     date.month() - 1,
                                     date.day());

        } else if (input instanceof QTime) {

            QTime time = (QTime) input;
            return new java.sql.Time(time.hour(), time.minute(), time.second());

        } else if (input instanceof QUrl) {

            QUrl url = (QUrl) input;
            try {
            	return new URI(url.scheme(), url.userInfo(), url.host(), url.port(), url.path(), url.query(), url.fragment()).toURL();
            } catch (MalformedURLException | URISyntaxException e) {
                java.util.logging.Logger.getLogger("io.qt.sql").log(java.util.logging.Level.SEVERE, "", e);
                return input;
            }

        } else {
            return input;
        }
    }


}
