/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.uic;

import java.util.Iterator;
import java.util.Objects;

import io.qt.core.QFile;
import io.qt.core.QHash;
import io.qt.core.QIODevice;
import io.qt.core.QPair;
import io.qt.uic.ui4.DomAction;
import io.qt.uic.ui4.DomActionGroup;
import io.qt.uic.ui4.DomAttributeInterface;
import io.qt.uic.ui4.DomButtonGroup;
import io.qt.uic.ui4.DomLayout;
import io.qt.uic.ui4.DomLayoutItem;
import io.qt.uic.ui4.DomSpacer;
import io.qt.uic.ui4.DomWidget;

public class Driver {
	
	public Driver() {}
	
	private boolean isFirst = true;
	
	private <DomClass extends DomAttributeInterface> String findOrInsert(QHash<DomClass,String> domHash, DomClass dom, String className)
	{
		Iterator<QPair<DomClass,String>> it = domHash.find(dom).iterator();
		if (!it.hasNext()) {
			String name = unique(dom.attributeName(), className);
			String value = name;
			if(!isFirst)
				value = "this." + name;
			else
				isFirst = false;
			domHash.insert(dom, value);
			return value;
		}
		return it.next().second;
	}
	
	private <DomClass extends DomAttributeInterface> DomClass findByAttributeName(QHash<DomClass,String> domHash, String name) {
		QPair<DomClass,String> pair = findByAttributeNameIt(domHash, name);
		return pair != null ? pair.first : null;
	}
	
	private <DomClass extends DomAttributeInterface> QPair<DomClass,String> findByAttributeNameIt(QHash<DomClass,String> domHash, String name) {
	    for (QPair<DomClass,String> pair : domHash) {
	        if (Objects.equals(name, pair.first.attributeName()))
	            return pair;
	    }
	    return null;
	}

    // tools
	public boolean printDependencies(String fileName) {
		assert m_option.dependencies == true;
	    m_option.inputFile = fileName;
	    Uic tool = new Uic(this);
	    return tool.printDependencies();
	}
    
    public boolean uic(String fileName) {
    	return uic(fileName, (String)null);
    }
    
    public boolean uic(String fileName, String outputDir) {
    	QIODevice input;
        if (fileName==null || fileName.isEmpty()) {
        	input = QIODevice.fromInputStream(System.in);
        }else {
        	input = new QFile(fileName);
        }
    	if (!input.isOpen() && !input.open(QIODevice.OpenModeFlag.ReadOnly)) {
    		System.err.println("QtJambi UIC: Unable to read input: "+input.errorString());
    		return false;
    	}

        m_option.inputFile = fileName;

        Uic tool = new Uic(this);
        try {
        	return tool.write(input, outputDir);
        }finally {
        	input.close();
        	input.dispose();
        }
    }
    
    // configuration
    public Option option() { return m_option; }
    
    public static String normalizedName(String name)
    {
    	String[] elements = name.split("::");
    	name = elements[elements.length - 1];
        char[] result = name.toCharArray();
        for (int i = 0; i < result.length; i++) {
			if(!Character.isJavaIdentifierPart(result[i])) {
				result[i] = '_';
			}
		}
        if(result.length>0 && !Character.isJavaIdentifierStart(result[0]))
        	return "_"+new String(result);
        return new String(result);
    }
    
    public static String qtify(String name) {
        if (name.charAt(0) == 'Q' || name.charAt(0) == 'K')
        	name = name.substring(1);

        char[] chars = name.toCharArray();
        for (int i = 0, size = chars.length; i < size && Character.isUpperCase(chars[i]); ++i)
        	chars[i] = Character.toLowerCase(chars[i]);

        return new String(chars);
    }
    public String unique() {
    	return unique("");
    }
    public String unique(String instanceName){
    	return unique(instanceName, "");
    }
    public String unique(String instanceName, String className) {
    	String name;
        boolean alreadyUsed = false;

        if (instanceName!=null && !instanceName.isEmpty()) {
            name = qtify(normalizedName(instanceName));
            String base = name;

            for (int id = 1; m_nameRepository.contains(name); ++id) {
                alreadyUsed = true;
                name = base + id;
            }
        } else if (!className.isEmpty()) {
            name = unique(qtify(className));
        } else {
            name = unique("var");
        }

        if (alreadyUsed && !className.isEmpty()) {
        	System.err.println(String.format("%1$s: Warning: The name '%2$s' (%3$s) is already in use, defaulting to '%4$s'.", m_option.messagePrefix(), instanceName, className, name));
        }

        m_nameRepository.insert(name, true);
        return name;
    }

    // symbol table
    public String findOrInsertWidget(DomWidget ui_widget) {
        return findOrInsert(m_widgets, ui_widget, ui_widget.attributeClass());
    }
    
    public String findOrInsertSpacer(DomSpacer ui_spacer) {
    	return findOrInsert(m_spacers, ui_spacer, "QSpacerItem");
    }
    
    public String findOrInsertLayout(DomLayout ui_layout) {
    	return findOrInsert(m_layouts, ui_layout, ui_layout.attributeClass());
    }
    
    public String findOrInsertLayoutItem(DomLayoutItem ui_layoutItem) {
    	switch (ui_layoutItem.kind()) {
	        case Widget:
	            return findOrInsertWidget(ui_layoutItem.elementWidget());
	        case Spacer:
	            return findOrInsertSpacer(ui_layoutItem.elementSpacer());
	        case Layout:
	            return findOrInsertLayout(ui_layoutItem.elementLayout());
	        case Unknown:
	            break;
	    }
	    return "";
    }
    
    public String findOrInsertName(String name) {
    	return unique(name);
    }
    
    public String findOrInsertActionGroup(DomActionGroup ui_group) {
    	return findOrInsert(m_actionGroups, ui_group, "QActionGroup");
    }
    
    public String findOrInsertAction(DomAction ui_action) {
    	return findOrInsert(m_actions, ui_action, "QAction");
    }
    
    public String findOrInsertButtonGroup(DomButtonGroup ui_group) {
    	return findOrInsert(m_buttonGroups, ui_group, "QButtonGroup");
    }
    
    // Find a group by its non-uniqified name
    public DomButtonGroup findButtonGroup(String attributeName) {
    	return findByAttributeName(m_buttonGroups, attributeName);
    }
    
    public DomWidget widgetByName(String attributeName) {
    	return findByAttributeName(m_widgets, attributeName);
    }
    
    public String widgetVariableName(String attributeName) {
    	QPair<DomWidget,String> pair = findByAttributeNameIt(m_widgets, attributeName);
        return pair != null ? pair.second : "";
    }
    
    public DomActionGroup actionGroupByName(String attributeName) {
    	return findByAttributeName(m_actionGroups, attributeName);
    }
    
    public DomAction actionByName(String attributeName) {
    	return findByAttributeName(m_actions, attributeName);
    }

    public boolean useIdBasedTranslations() { return m_idBasedTranslations; }
    public void setUseIdBasedTranslations(boolean u) { m_idBasedTranslations = u; }

    final Option m_option = new Option();

    // symbol tables
    final QHash<DomWidget,String> m_widgets = new QHash<>(DomWidget.class,String.class);
    final QHash<DomSpacer,String> m_spacers = new QHash<>(DomSpacer.class,String.class);
    final QHash<DomLayout,String> m_layouts = new QHash<>(DomLayout.class,String.class);
    final QHash<DomActionGroup,String> m_actionGroups = new QHash<>(DomActionGroup.class,String.class);
    final QHash<DomButtonGroup,String> m_buttonGroups = new QHash<>(DomButtonGroup.class,String.class);
    final QHash<DomAction,String> m_actions = new QHash<>(DomAction.class,String.class);
    final QHash<String, Boolean> m_nameRepository = new QHash<>(String.class, boolean.class);
    boolean m_idBasedTranslations = false;
}
