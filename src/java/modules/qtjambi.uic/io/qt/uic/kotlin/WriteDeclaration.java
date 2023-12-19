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
package io.qt.uic.kotlin;

import io.qt.core.*;
import io.qt.uic.*;
import io.qt.uic.ui4.*;

/**
 * @hidden
 */
public class WriteDeclaration extends TreeWalker {
	
	public WriteDeclaration(Uic uic){
	    m_uic = uic;
	    m_driver = uic.driver();
	    m_output = uic.output();
	    m_option = uic.option();
	}
	
	@Override public void acceptUI(DomUI node)
	{
	    String qualifiedClassName = node.elementClass() + m_option.postfix;
	    String className = qualifiedClassName;

	    m_driver.findOrInsertWidget(node.elementWidget());

	    String[] namespaceList = qualifiedClassName.split("::");
	    if (namespaceList.length!=0) {
	        className = namespaceList[namespaceList.length-1];
	    }

	    if (m_option.targetPackage!=null && !m_option.targetPackage.isEmpty())
	        m_output.endl();

	    m_output.append("class ").append(m_option.prefix).append(className).append(" {").endl().endl();

	    for (String connection : m_uic.databaseInfo().connections()) {
	        if (!connection.equals("(default)"))
	            m_output.append(m_option.indent).append("var ").append(Driver.normalizedName(connection)).append("Connection : QSqlDatabase()").endl();
	    }

	    super.acceptWidget(node.elementWidget());
	    DomButtonGroups domButtonGroups = node.elementButtonGroups();
	    if (domButtonGroups!=null)
	        acceptButtonGroups(domButtonGroups);

	    m_output.endl();

	    new WriteInitialization(m_uic).acceptUI(node);

	    m_output.append("}").endl().endl();
	}

	@Override public void acceptWidget(DomWidget node)
	{
	    String className = "QWidget";
	    if (node.hasAttributeClass())
	        className = node.attributeClass();

	    m_output.append(m_option.indent).append("var ").append(unself(m_driver.findOrInsertWidget(node))).append(": ").append(m_uic.customWidgetsInfo().realClassName(className)).append("? = null").endl();
	    super.acceptWidget(node);
	}

	@Override public void acceptSpacer(DomSpacer node)
	{
	     m_output.append(m_option.indent).append("var ").append(unself(m_driver.findOrInsertSpacer(node))).append(": QSpacerItem? = null").endl();
	     super.acceptSpacer(node);
	}

	@Override public void acceptLayout(DomLayout node)
	{
	    String className = "QLayout";
	    if (node.hasAttributeClass())
	        className = node.attributeClass();

	    m_output.append(m_option.indent).append("var ").append(unself(m_driver.findOrInsertLayout(node))).append(": ").append(className).append("? = null").endl();
	    super.acceptLayout(node);
	}

	@Override public void acceptActionGroup(DomActionGroup node)
	{
		m_output.append(m_option.indent).append("var ").append(unself(m_driver.findOrInsertActionGroup(node))).append(": QActionGroup? = null").endl();
	    super.acceptActionGroup(node);
	}

	@Override public void acceptAction(DomAction node)
	{
		m_output.append(m_option.indent).append("var ").append(unself(m_driver.findOrInsertAction(node))).append(": QAction? = null").endl();
	    super.acceptAction(node);
	}

	@Override public void acceptButtonGroup(DomButtonGroup buttonGroup)
	{
		m_output.append(m_option.indent).append("var ").append(unself(m_driver.findOrInsertButtonGroup(buttonGroup))).append(": QButtonGroup? = null").endl();
	    super.acceptButtonGroup(buttonGroup);
	}
	
	private String unself(String referenceName) {
		if(referenceName.startsWith("this."))
			referenceName = referenceName.substring(5);
		return referenceName;
	}
	
    private final Uic m_uic;
    private final Driver m_driver;
    private final QTextStream m_output;
    private final Option m_option;
}
