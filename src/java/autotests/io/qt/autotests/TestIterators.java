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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QNoNativeResourcesException;
import io.qt.autotests.generated.General;
import io.qt.core.QDir;
import io.qt.core.QDirIterator;
import io.qt.core.QRegularExpression;
import io.qt.core.QRegularExpressionMatch;
import io.qt.internal.TestUtility;
import io.qt.widgets.QTreeWidget;
import io.qt.widgets.QTreeWidgetItem;
import io.qt.widgets.QTreeWidgetItemIterator;

public class TestIterators extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test(expected=NullPointerException.class)
    public void test_QTreeWidgetItemIterator_construction_no_widget() {
		new QTreeWidgetItemIterator((QTreeWidget)null);
	}
	
	@Test(expected=NullPointerException.class)
    public void test_QTreeWidgetItemIterator_construction_no_item() {
		new QTreeWidgetItemIterator((QTreeWidgetItem)null);
	}
	
	@Test(expected=IllegalArgumentException.class)
    public void test_QTreeWidgetItemIterator_construction_item_no_widget() {
		QTreeWidgetItem item = new QTreeWidgetItem();
		new QTreeWidgetItemIterator(item);
	}
	
	@Test
    public void test_QTreeWidgetItemIterator_owner() {
    	QTreeWidget widget = new QTreeWidget();
    	try {
	    	QTreeWidgetItemIterator iterator = new QTreeWidgetItem(widget).iterator();
	    	Assert.assertTrue(General.internalAccess.hasOwnerFunction(iterator));
	    	Assert.assertEquals(widget.model(), General.internalAccess.owner(iterator));
    	}finally {
    		widget.dispose();
    	}
	}
	
	@Test
    public void test_QTreeWidgetItemIterator_owner_deletion() {
    	QTreeWidget widget = new QTreeWidget();
    	try {
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("A")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("B")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("C")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("D")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("E")));
	    	QTreeWidgetItemIterator iterator = widget.iterator();
	    	widget.dispose();
	    	try {
				iterator.hasNext();
				Assert.assertFalse("QNoNativeResourcesException expected to be thrown.", true);
			} catch (QNoNativeResourcesException e) {
			}
    	}finally {
    		widget.dispose();
    	}
	}
	
	@Test
    public void test_QTreeWidgetItemIterator_correct_iteration() {
    	QTreeWidget widget = new QTreeWidget();
    	try {
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("A")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("B")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("C")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("D")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("E")));
	    	List<String> texts = new ArrayList<>();
	    	for(QTreeWidgetItem item : widget) {
	    		Assert.assertTrue(item!=null);
	    		texts.add(item.text(0));
	    	}
	    	Assert.assertEquals(Arrays.asList("A", "B", "C", "D", "E"), texts);
    	}finally {
    		widget.dispose();
    	}
    }
    
    @Test
    public void test_QDirIterator_correct_iteration() {
    	String uniqueDirectory = "QtJambi_QDirTest_"+TestUtility.processName();
    	try {
	    	QDir temp = QDir.temp();
	    	Assert.assertTrue(temp.mkdir(uniqueDirectory));
	    	Assert.assertTrue(temp.cd(uniqueDirectory));
	    	Assert.assertTrue(temp.mkdir("A"));
	    	Assert.assertTrue(temp.mkdir("B"));
	    	Assert.assertTrue(temp.mkdir("C"));
	    	Assert.assertTrue(temp.mkdir("D"));
	    	Assert.assertTrue(temp.mkdir("E"));
	    	Set<String> subDirs = new HashSet<>();
	    	for(String subdir : new QDirIterator(temp)) {
    			int idx = subdir.lastIndexOf('/');
    			subDirs.add(subdir.substring(idx+1));
	    	}
	    	Assert.assertEquals(new HashSet<>(Arrays.asList(".", "..", "A", "B", "C", "D", "E")), subDirs);
    	}finally {
    		QDir temp = QDir.temp();
    		if(temp.exists(uniqueDirectory)) {
        		temp.cd("QtJambi_QDirTest");
        		for(String s : temp.entryList()) {
        			temp.rmdir(s);
        		}
        		temp.cdUp();
        		temp.rmdir(uniqueDirectory);
        	}
		}
    }
    
    @Test
    public void test_QRegularExpressionMatchIterator_correct_iteration() {
    	QRegularExpression expression = new QRegularExpression("^(aa)");
    	List<QRegularExpressionMatch> matches = new ArrayList<>();
    	for(QRegularExpressionMatch match : expression.globalMatch("aa aa aa bb cc aa", 0, QRegularExpression.MatchType.PartialPreferCompleteMatch)) {
    		matches.add(match);
    	}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestIterators.class.getName());
    }
}
