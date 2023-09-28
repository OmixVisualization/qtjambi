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

import static org.junit.Assert.*;

import org.junit.Test;

import io.qt.*;
import io.qt.core.*;
import io.qt.gui.*;

public class TestNonVirtualOverride extends ApplicationInitializer {
	
	@Test
    public void testItemModel() {
		class ItemModel extends QStandardItemModel{
			@Override
			protected void beginInsertColumns(io.qt.core.@NonNull QModelIndex parent, int first, int last){
				super.beginInsertColumns(parent, first, last);
			}
		}
		try {
			new ItemModel();
			fail("QNonVirtualOverridingException expected to be thrown");
		} catch (QNonVirtualOverridingException e) {
			assertEquals(String.format("Malformed %1$s subtype: Following method has to be declared final in class %2$s: %3$s", "QStandardItemModel", ItemModel.class.getName(), "protected final void beginInsertColumns(io.qt.core.QModelIndex arg0,int arg1,int arg2)"), e.getMessage());
		}
	}
	
	@Test
    public void testQRunnable() throws Throwable{
		class Runnable extends QtObject implements QRunnable{
			@Override
			public boolean autoDelete() {
				return QRunnable.super.autoDelete();
			}

			@Override
			public void run() {
			}
		}
		try {
			new Runnable();
			fail("QNonVirtualOverridingException expected to be thrown");
		} catch (QNonVirtualOverridingException e) {
			assertEquals(String.format("Malformed %1$s subtype: Class %2$s overrides following final method: %3$s", "QRunnable", Runnable.class.getName(), "public final boolean autoDelete()"), e.getMessage());
		}
	}
	
	@Test
    public void testQRunnable2() throws Throwable{
		class Runnable implements QRunnable{
			@Override
			public void dispose() {
				QRunnable.super.dispose();
			}

			@Override
			public void run() {
			}
			
			{
				QtUtilities.initializeNativeObject(this);
			}
		}
		try {
			new Runnable();
			fail("QNonVirtualOverridingException expected to be thrown");
		} catch (QNonVirtualOverridingException e) {
			assertEquals(String.format("Malformed %1$s subtype: Class %2$s overrides following final method: %3$s", "QRunnable", Runnable.class.getName(), "public final void dispose()"), e.getMessage());
		}
	}
	
	@Test
    public void testQTextCharFormat() throws Throwable{
		class TextCharFormat extends QTextCharFormat{
			@Override
			public boolean isValid() {
				return super.isValid();
			}

			public TextCharFormat() {
				super();
			}
		}
		try {
			new TextCharFormat();
			fail("QNonVirtualOverridingException expected to be thrown");
		} catch (QNonVirtualOverridingException e) {
			assertEquals(String.format("Malformed %1$s subtype: Class %2$s overrides following final method: public final boolean isValid()", "QTextCharFormat", TextCharFormat.class.getName()), e.getMessage());
		}
	}
	
	@Test
    public void testQStandardItemModel() throws Throwable{
		new QStandardItemModel();
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestNonVirtualOverride.class.getName());
    }
}
