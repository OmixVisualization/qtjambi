/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import io.qt.core.QMetaType;
import io.qt.core.QVariant;
import io.qt.widgets.QWidget;

public class TestQVariantQt6 extends ApplicationInitializer {
	
	@Test
    public void testNull() {
    	QVariant variant = QVariant.fromValue(null);
    	assertTrue(variant.isValid());
    	assertEquals(variant.metaType(), QMetaType.Type.Nullptr);
    	assertEquals(null, variant.value(Object.class));
    	Object nll = variant.value();
    	assertTrue(null==nll);
    	
    	variant = QVariant.fromValue(QVariant.NULL);
    	assertTrue(variant.isValid());
    	assertEquals(variant.metaType(), QMetaType.Type.Nullptr);
    	assertEquals(null, variant.value(Object.class));
    	nll = variant.value();
    	assertTrue(null==nll);
    	variant = QVariant.fromValue(QVariant.typedNullable(null, QWidget.class));
    	assertTrue(variant.isValid());
    	assertEquals(QMetaType.fromType(QWidget.class), variant.metaType());
    	assertEquals(null, variant.value(Object.class));
    	nll = variant.value();
    	assertTrue(null==nll);
    }
	
	public static class VariantConstructor implements TestQVariant.VariantConstructor{
		public QVariant create(io.qt.core.QMetaType type) {
    		return new QVariant(type);
    	}
    	public QVariant create(io.qt.core.QMetaType type, java.lang.Object copy) {
    		return new QVariant(type, copy);
    	}
    	public QVariant create(io.qt.core.QMetaType.Type type) {
    		return new QVariant(type);
    	}
    	public QVariant create(io.qt.core.QMetaType.Type type, java.lang.Object copy) {
    		return new QVariant(type, copy);
    	}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQVariantQt6.class.getName());
    }
}
