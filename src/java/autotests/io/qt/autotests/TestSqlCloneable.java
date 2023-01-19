/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import static org.junit.Assert.*;

import org.junit.*;

import io.qt.autotests.generated.General;
import io.qt.sql.QSqlDatabase;
import io.qt.sql.QSqlError;
import io.qt.sql.QSqlField;
import io.qt.sql.QSqlIndex;
import io.qt.sql.QSqlQuery;
import io.qt.sql.QSqlRecord;
import io.qt.sql.QSqlRelation;

public class TestSqlCloneable extends ApplicationInitializer {

    @Test
    public void run_clone_QSqlDatabase() {
        QSqlDatabase org = new QSqlDatabase();
        QSqlDatabase clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QSqlDatabase clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.connectionName(), clone2.connectionName());
    }

    @Test
    public void run_clone_QSqlError() {
        QSqlError org = new QSqlError("error");
        QSqlError clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QSqlError clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.driverText(), clone2.driverText());
    }

    @Test
    public void run_clone_QSqlField() {
        QSqlField org = new QSqlField();
        QSqlField clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QSqlField clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QSqlIndex() {
        QSqlIndex org = new QSqlIndex();
        QSqlIndex clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QSqlIndex clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QSqlQuery() {
        QSqlQuery org = new QSqlQuery("some query");
        QSqlQuery clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QSqlQuery clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.lastQuery(), clone2.lastQuery());
    }

    @Test
    public void run_clone_QSqlRecord() {
        QSqlRecord org = new QSqlRecord();
        QSqlRecord clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QSqlRecord clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    // this test does not make sense because QSqlRelation does not have an equals operator
	//@Test
	public void run_clone_QSqlRelation() {
		QSqlRelation org = new QSqlRelation();
		QSqlRelation clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QSqlRelation clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSqlCloneable.class.getName());
    }
}
