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

import java.util.ConcurrentModificationException;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QList;
import io.qt.core.QStringList;

public class TestContainersModification extends ApplicationInitializer{
	@BeforeClass
    public static void testInitialize() throws Exception {
		System.setProperty("io.qt.enable-concurrent-container-modification-check", "true");
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
	public void testConcurrentModification() {
		// due to performance reasons concurrent modification tests are skipped
		try {
			QList<String> list = new QList<>(String.class);
			list.add("A");
			list.add("A");
			for (@SuppressWarnings("unused")
			String string : list) {
				list.clear();
			}
			Assert.fail("ConcurrentModificationException expected to be thrown");
		} catch (ConcurrentModificationException e) {
		}
	}

//  @Test
	public void testPerformance() {
		QStringList list = new QStringList();
//  	list.fill("", 0xffffff);//not in Qt5
		long t1 = System.currentTimeMillis();
		for (@SuppressWarnings("unused")
		String string : list) {
		}
		long t2 = System.currentTimeMillis();
		System.out.println("time: " + (t2 - t1));
	}
}
