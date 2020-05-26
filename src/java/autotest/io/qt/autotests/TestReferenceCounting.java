/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.autotests;

import org.junit.Test;

public class TestReferenceCounting extends AbstractTestReferenceCounting {
//	@org.junit.BeforeClass
//	public static void init() {
//		org.junit.Assume.assumeFalse(true);
//	}
	
	@Test 
	public void testIconEngine() {
		super.testIconEngine();
	}
	
	@Test
    public void setItemDelegateForRowToNull() {
		super.setItemDelegateForRowToNull();
	}
	
	@Test
    public void setItemDelegateForColumnToNull() {
		super.setItemDelegateForColumnToNull();
	}
	
	@Test
    public void setWidgetMappingToNull() {
		super.setWidgetMappingToNull();
	}
	
	@Test
    public void setObjectMappingToNull() {
		super.setObjectMappingToNull();
	}
	
	@Test 
	public void testQAbstractProxyModelSetSourceModel() {
		super.testQAbstractProxyModelSetSourceModel();
	}
	
	@Test 
    public void testQAbstractProxyModelSetSourceModelNull() {
		super.testQAbstractProxyModelSetSourceModelNull();
	}
	
	@Test 
    public void testQSortFilterProxyModelSetSourceModel() {
		super.testQSortFilterProxyModelSetSourceModel();
	}
	
	@Test 
	public void testQSortFilterProxyModelSetSourceModelNull() {
		super.testQSortFilterProxyModelSetSourceModelNull();
	}
	
	@Test 
	public void testQComboBoxSetCompleter() {
		super.testQComboBoxSetCompleter();
	}
	
	@Test 
	public void testQComboBoxEditableSetCompleter() {
		super.testQComboBoxEditableSetCompleter();
	}
	
	@Test 
	public void testQComboBoxSetCompleterNull() {
		super.testQComboBoxSetCompleterNull();
	}
	
	@Test 
	public void testQComboBoxSetCompleterNoLineEdit() {
		super.testQComboBoxSetCompleterNoLineEdit();
	}
	
	@Test 
	public void testQLineEditSetCompleter() {
		super.testQLineEditSetCompleter();
	}
	
	@Test 
	public void testQLineEditSetCompleterNull() {
		super.testQLineEditSetCompleterNull();
	}
	
	@Test 
	public void testQComboBoxLineEditSetCompleterNull() {
		super.testQComboBoxLineEditSetCompleterNull();
	}
	
	@Test 
	public void testQLineEditComboBoxSetCompleter() {
		super.testQLineEditComboBoxSetCompleter();
	}
	
	@Test 
	public void testQLineEditComboBoxSetCompleterNull() {
		super.testQLineEditComboBoxSetCompleterNull();
	}
	
	@Test public void testQComboBoxSetItemDelegate() {
		super.testQComboBoxSetItemDelegate();
	}
	
	@Test public void testQComboBoxSetItemDelegateNull() {
		super.testQComboBoxSetItemDelegateNull();
	}
	
	@Test public void testQComboBoxSetModel() {
		super.testQComboBoxSetModel();
	}
	
	@Test public void testQComboBoxSetValidator() {
		super.testQComboBoxSetValidator();
	}
	
	@Test public void testQComboBoxSetValidatorNull() {
		super.testQComboBoxSetValidatorNull();
	}
	
	@Test public void testQComboBoxLineEditSetValidator() {
		super.testQComboBoxLineEditSetValidator();
	}
	
	@Test public void testQButtonGroupAddButton() {
		super.testQButtonGroupAddButton();
	}
	
	@Test public void testQButtonGroupAddButtonId() {
		super.testQButtonGroupAddButtonId();
	}
	
	@Test public void testQButtonGroupRemoveButton() {
		super.testQButtonGroupRemoveButton();
	}
	
	@Test public void testQAbstractItemViewSetItemDelegate() {
		super.testQAbstractItemViewSetItemDelegate();
	}
	
	@Test public void testQAbstractItemViewSetItemDelegateNull() {
		super.testQAbstractItemViewSetItemDelegateNull();
	}
	
	@Test public void testQAbstractItemViewSetModel() {
		super.testQAbstractItemViewSetModel();
	}
	
	@Test public void testQAbstractItemViewSetSelectionModelThenSetModel() {
		super.testQAbstractItemViewSetSelectionModelThenSetModel();
	}
	
	@Test public void testQAbstractItemViewSetSelectionModel() {
		super.testQAbstractItemViewSetSelectionModel();
	}
}
