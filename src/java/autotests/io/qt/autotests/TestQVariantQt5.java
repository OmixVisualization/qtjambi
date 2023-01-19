/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.Collection;
import java.util.List;

import org.junit.Test;

import io.qt.QtEnumerator;
import io.qt.autotests.generated.Variants;
import io.qt.core.*;

public class TestQVariantQt5 extends ApplicationInitializer {
	
    @Test
    public void testQVariant_IntVector() {
    	Object variant = Variants.getIntVector();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QVector);
    	assertEquals(QVector.of(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_StringVector() {
    	Object variant = Variants.getStringVector();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QVector);
    	assertEquals(QVector.of("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_UnknownEnumVector() {
    	Object variant = Variants.getUnknownEnumVector();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QVector);
    	assertEquals(4, ((Collection<?>)variant).size());
    	int i = 101;
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		assertEquals(i++, ((QtEnumerator)e).value());
    	}
    }
    
    @Test
    public void testQVariant_DirVector() {
    	Object variant = Variants.getDirVector();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QVector);
    	assertEquals(QVector.of(new QDir("/"), new QDir("/home")), variant);
    }
    
    @Test
    public void testQVariant_TestObjectVector() {
    	Object variant = Variants.getTestObjectVector();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QVector);
    	TestQVariant.variantConsumer.accept((List<?>)variant);
    }
    
    @Test
    public void testQVariant_UnknownValueVector() {
    	Object variant = Variants.getUnknownValueVector();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QVector);
    	assertEquals(1, ((Collection<?>)variant).size());
    	for(Object o : (Collection<?>)variant) {
    		assertTrue(o instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", ((QMetaType.GenericTypeInterface)o).metaType().name().toString());
    	}
    }
	
    @Test
    public void testQVariant_IntLinkedList() {
    	Object variant = Variants.getIntLinkedList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QLinkedList);
    	assertEquals(QLinkedList.of(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_StringLinkedList() {
    	Object variant = Variants.getStringLinkedList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QLinkedList);
    	assertEquals(QLinkedList.of("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_DirLinkedList() {
    	Object variant = Variants.getDirLinkedList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QLinkedList);
    	assertEquals(QLinkedList.of(new QDir("/"), new QDir("/home")), variant);
    }
    
    @Test
    public void testQVariant_TestObjectLinkedList() {
    	Object variant = Variants.getTestObjectLinkedList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QLinkedList);
    	TestQVariant.variantConsumer.accept((QLinkedList<?>)variant);
    }
    
    @Test
    public void testQVariant_UnknownValueLinkedList() {
    	Object variant = Variants.getUnknownValueLinkedList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QLinkedList);
    	assertEquals(1, ((Collection<?>)variant).size());
    	for(Object o : (Collection<?>)variant) {
    		assertTrue(o instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", ((QMetaType.GenericTypeInterface)o).metaType().name().toString());
    	}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQVariantQt5.class.getName());
    }
}
