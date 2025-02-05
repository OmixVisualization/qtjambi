/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.uic.java;

import java.util.Arrays;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import io.qt.core.QCoreApplication;
import io.qt.core.QFileInfo;
import io.qt.core.QTextStream;
import io.qt.uic.ClassInfoEntries;
import io.qt.uic.ClassInfoEntry;
import io.qt.uic.Option;
import io.qt.uic.Uic;
import io.qt.uic.WriteIncludesBase;
import io.qt.uic.ui4.DomCustomWidget;
import io.qt.uic.ui4.DomInclude;
import io.qt.uic.ui4.DomUI;

/**
 * @hidden
 */
public class WriteImports extends WriteIncludesBase {
	
	public WriteImports(Uic uic) {
		super(uic);
		m_output = uic.output();
		m_option = uic.option();
	}
	
	@Override public void acceptUI(DomUI node)
	{
	    m_imports.clear();
	    m_includeBaseNames.clear();
	    
	    if (m_option.copyrightHeader) {
	        m_output.writeString("/********************************************************************************\n");
	        if (m_option.inputFile==null || m_option.inputFile.isEmpty())
	        	m_output.writeString(" * Form generated from reading UI file\n");
	        else
	        	m_output.writeString(" * Form generated from reading UI file '" + new QFileInfo(m_option.inputFile).fileName() + "'\n");
	        m_output.writeString(" *\n");
	        m_output.writeString(" * Created by: QtJambi User Interface Compiler version " + QCoreApplication.applicationVersion() + "\n");
	        m_output.writeString(" *\n");
	        m_output.writeString(" * WARNING! All changes made in this file will be lost when recompiling UI file!\n");
	        m_output.writeString(" *******************************************************************************/\n\n");
	        String comment = node.elementComment();
	        if (comment !=null && !comment.isEmpty()) {
	            m_output.writeString("/**\n");
	            m_output.writeString(" *"+comment);
	            m_output.writeString("\n */\n\n");
	        }
	    }

	    m_output.append("package ").append(m_option.targetPackage).append(';').endl().endl();

	    super.acceptUI(node);
	    
	    boolean connectSlotsByName = true;
	    if (node.hasAttributeConnectslotsbyname())
	        connectSlotsByName = node.attributeConnectslotsbyname();

	    if (m_option.autoConnection && connectSlotsByName)
	    	m_imports.add("io.qt.core.*");

	    String includeFile = uic().option().imports;
	    for(String incl : includeFile.split(","))
	    	m_imports.add(incl);

	    TreeSet<String> packages = new TreeSet<>();
	    packages.add(m_option.targetPackage);
	    for (String imprt : m_imports) {
	    	imprt = imprt.trim();
	        if (!imprt.isEmpty()) {
	        	String[] inamespaceList = imprt.split("\\.");
	    	    if (inamespaceList.length>0) {
	    	        String last = inamespaceList[inamespaceList.length-1];
	    	        String currentPackage = String.join(".", Arrays.copyOf(inamespaceList, inamespaceList.length-1));
	    	        if("*".equals(last)) {
	    	        	packages.add(currentPackage);
	    	        }else {
	    	        	if(packages.contains(currentPackage))
	    	        		continue;
	    	        }
	    	    }
	        	m_output.append("import ").append(imprt).append(';').endl();
	        }
	    }
	    m_output.endl();
	}

	private void insertIncludeForClass(String className, String header)
	{
	    do {
	        if (!header.isEmpty())
	            break;

	        // Known class
	        if (m_classToPackage.containsKey(className)) {
	            header = m_classToPackage.get(className);
	            break;
	        }

	        // Quick check by class name to detect includehints provided for custom widgets.
	        // Remove namespaces
	        String lowerClassName = className.toLowerCase();
	        String namespaceSeparator = "::";
	        int namespaceIndex = lowerClassName.lastIndexOf(namespaceSeparator);
	        if (namespaceIndex != -1)
	        	lowerClassName = lowerClassName.substring(0, namespaceIndex + namespaceSeparator.length());
	        if (m_includeBaseNames.contains(lowerClassName)) {
	            header = "";
	            break;
	        }

            break;
	    } while (false);

	    if (!header.isEmpty())
	        insertInclude(header);
	}

	@Override public void doAdd(String className, DomCustomWidget dcw)
	{
	    if (dcw != null) {
	    	if(className.contains("::"))
	    		insertInclude(className.replace("::", "."));
	    	else if(dcw.hasElementHeader()) {
	    		if(dcw.elementHeader().text().toLowerCase().endsWith(".h"))
	    			insertIncludeForClass(className, "");
	    		else
	    			insertInclude(dcw.elementHeader().text().replace("::", "."));
	    	}
	    }else {
	        insertIncludeForClass(className, "");
	    }
	}

	@Override public void acceptInclude(DomInclude node)
	{
	    if(m_classToPackage.containsKey(node.text())) {
	    	insertInclude(m_classToPackage.get(node.text()));
	    }else {
	    	insertInclude(node.text());
	    }
	}

	private void insertInclude(String incl)
	{
	    // Insert (if not already done).
//	    boolean isNewHeader = !m_imports.contains(header);
		String[] namespaceList = incl.split("\\.");
	    if (namespaceList.length>0) {
	        namespaceList[namespaceList.length-1] = "*";
	        incl = String.join(".", namespaceList);
	    }
	    m_imports.add(incl);
//	    if (!isNewHeader)
//	        return;
	    // Also remember base name for quick check of suspicious custom plugins
//	    String lowerBaseName = new QFileInfo(header).completeBaseName().toLowerCase();
//	    m_includeBaseNames.insert(lowerBaseName);
	}

	protected QTextStream output() { return m_output; }

	private final QTextStream m_output;
    private final Option m_option;

	private final TreeSet<String> m_imports = new TreeSet<>();
	private final Set<String> m_includeBaseNames = new TreeSet<>();
    public static final Map<String, String> m_classToPackage = new TreeMap<>();
    
    static {
    	for (ClassInfoEntry e : ClassInfoEntries.classInfoEntries()) {
	    	m_classToPackage.put(e.klass, e.packge + "." + e.klass);
	    }
	    m_classToPackage.put("QtCore", "io.qt.core.*");
	    m_classToPackage.put("QtGui", "io.qt.gui.*");
	    m_classToPackage.put("QtWidgets", "io.qt.widgets.*");
	    m_classToPackage.put("QtSvgWidgets", "io.qt.svg.widgets.*");
	    m_classToPackage.put("QtSvg", "io.qt.svg.*");
	    m_classToPackage.put("QtSql", "io.qt.sql.*");
	    m_classToPackage.put("QtNetwork", "io.qt.network.*");
	    m_classToPackage.put("ActiveQt", "io.qt.activex.*");
	    m_classToPackage.put("QtQuickWidgets", "io.qt.quick.widgets.*");
	    m_classToPackage.put("QtQuick", "io.qt.quick.*");
	    m_classToPackage.put("QtQml", "io.qt.qml.*");
    }
}
