/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.reflect.RecordComponent;
import java.util.Arrays;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.*;
import io.qt.core.*;

public class TestJDK17FeatureRecord extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }
	
	public static record Book(@QtPropertyReader @QtInvokable String name, @QtPropertyReader @QtInvokable double price) {}
    
	@SuppressWarnings("unused")
	@Test
    public void test() {
		QMetaObject bookObjectType = QMetaObject.forType(Book.class);
		System.out.println(Arrays.toString(Book.class.getDeclaredConstructors()));
		System.out.println(Arrays.toString(Book.class.getDeclaredMethods()));
		bookObjectType.methods().forEach(m->{
			System.out.println(m.typeName()+" "+m.cppMethodSignature());
		});
		bookObjectType.properties().forEach(p->{
			System.out.println(p.typeName()+" "+p.name());
		});
		Book book = new Book("1", 1);
		QMetaType bookType  = QMetaType.fromType(Book.class);
		QVariant recordVariant = QVariant.fromValue(book);
		System.out.println(recordVariant.metaType());
		Object value = recordVariant.value();
		Book bookValue = recordVariant.value(Book.class);
		System.out.println(book);
		for(RecordComponent rc : Book.class.getRecordComponents()) {
			System.out.println(rc.getGenericSignature());
			System.out.println(rc.getName());
			System.out.println(rc.getAccessor());
		}
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestJDK17FeatureRecord.class.getName());
    }
}
