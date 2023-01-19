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
package io.qt.autotests;

import java.io.File;
import java.io.IOException;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QFileInfo;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QStandardPaths;
import io.qt.core.internal.QFactoryLoader;
import io.qt.sql.QSqlDatabase;
import io.qt.sql.QSqlDriver;
import io.qt.sql.QSqlDriverPlugin;
import io.qt.sql.QSqlError;
import io.qt.sql.QSqlQuery;

public class TestSql extends ApplicationInitializer {
	
    @Test
    public void testDefaultConnection() {
    	String defaultConnection = QSqlDatabase.defaultConnection();
    	Assert.assertEquals("qt_sql_default_connection", defaultConnection);
    }
	
    @Test
    public void testSqlDriverPlugin() {
		QFactoryLoader loader = new QFactoryLoader(QSqlDriverPlugin.class, "/sqldrivers");
		System.out.println(loader.keyMap());
		QSqlDriver plugin = loader.loadPlugin(QSqlDriverPlugin::create, "QSQLITE");
		Assert.assertTrue("unable to load plugin QSQLITE.", plugin!=null);
		Assert.assertEquals("QSQLiteDriver", plugin.metaObject().className());
		Assert.assertTrue("not java ownership", General.internalAccess.isJavaOwnership(plugin));
		plugin.dispose();
	}
    
    @Test
    public void testSqlDriverPlugin_Jdbc() {
		QFactoryLoader loader = new QFactoryLoader(QSqlDriverPlugin.class, "/sqldrivers");
		System.out.println(loader.keyMap());
		QSqlDriver plugin = loader.loadPlugin(QSqlDriverPlugin::create, "QJDBC");
		Assert.assertTrue("unable to load plugin QJDBC.", plugin!=null);
		Assert.assertEquals("io::qt::sql::jdbc::QJdbcSqlDriver", plugin.metaObject().className());
		Assert.assertTrue("not java ownership", General.internalAccess.isJavaOwnership(plugin));
		plugin.dispose();
	}
    
    @Test
    public void testJdbc() throws IOException {
    	QSqlDatabase db = QSqlDatabase.addDatabase("QJDBC");
    	QFileInfo file = new QFileInfo(QStandardPaths.writableLocation(QStandardPaths.StandardLocation.TempLocation)+"/test.sqlite");
    	if(file.exists())
    		new File(file.absoluteFilePath()).delete();
    	try {
	    	Assert.assertFalse(file.exists());
	    	db.setDatabaseName("jdbc:sqlite:"+file.absoluteFilePath());
	    	boolean isOpen = db.open();
	    	QSqlError lastError = db.lastError();
	    	Assert.assertTrue(lastError.driverText() + " " + lastError.databaseText(), isOpen);
	    	try {
		    	Assert.assertTrue(file.exists());
		    	QSqlQuery query = new QSqlQuery(db);
		    	query.prepare("CREATE TABLE qtjambitest (\n"
		                + "	id integer PRIMARY KEY,\n"
		                + "	test text NOT NULL\n"
		                + ");");
		    	boolean sucs = query.exec();
		    	Assert.assertTrue(query.lastError().toString(), sucs);
	    	}finally {
	    		db.close();
	    	}
    	}catch(UnsatisfiedLinkError e) {
    		if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
    			throw e;
    		}
    	}finally {
//    		System.out.println(QDir.toNativeSeparators(file.absoluteFilePath()));
    		new File(file.absoluteFilePath()).delete();
    	}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSql.class.getName());
    }
}
