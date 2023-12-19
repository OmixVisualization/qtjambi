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
package io.qt.uic.java;

import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.lang.reflect.Parameter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.core.QTextStream;
import io.qt.uic.CustomWidgetsInfo;
import io.qt.uic.Option;
import io.qt.uic.TreeWalker;
import io.qt.uic.Uic;
import io.qt.uic.ui4.DomCustomWidget;
import io.qt.uic.ui4.DomSlots;
import io.qt.uic.ui4.DomUI;
import io.qt.uic.ui4.DomWidget;

/**
 * @hidden
 */
public class WriteClass extends TreeWalker {
	
	public WriteClass(Uic uic, String className) {
		super();
		m_output = uic.output();
		m_option = uic.option();
		cWidgetsInfo = uic.customWidgetsInfo();
		m_className = className;
	}
	
	@Override public void acceptUI(DomUI node)
	{
		m_elementSlots = node.elementSlots();
	    m_output.append("package ").append(m_option.targetPackage).append(';').endl().endl();
	    super.acceptUI(node);
	}
	
	private String fromUnknownType(String argument) {
		String rawArgument = argument;
//		boolean isPointer = rawArgument.endsWith("*");
		if(rawArgument.startsWith("const ")) {
			rawArgument = rawArgument.substring(6);
		}
		if(rawArgument.endsWith(" const*") || rawArgument.endsWith(" const&")) {
			rawArgument = rawArgument.substring(0, rawArgument.length()-7);
		}
		if(rawArgument.endsWith("*") || rawArgument.endsWith("&")) {
			rawArgument = rawArgument.substring(0, rawArgument.length()-1);
		}
		DomCustomWidget customWidget = cWidgetsInfo.customWidget(rawArgument);
		if(customWidget!=null) {
			if(rawArgument.contains("::"))
				rawArgument = rawArgument.replace("::", ".");
	    	else if(customWidget.hasElementHeader()) {
	    		if(!customWidget.elementHeader().text().toLowerCase().endsWith(".h"))
	    			rawArgument = customWidget.elementHeader().text().replace("::", ".");
	    	}
			return rawArgument;
		}else {
			return argument;
		}
	}
	
	private String typeArgs(String argument) {
		int parenPos = argument.indexOf('<');
		if(parenPos<0) {
			String _argument = getExternalTypeName(argument).replace('$', '.').replace('/', '.');
			if(_argument.isEmpty())
				_argument = fromUnknownType(argument);
			return _argument;
		}
		String container = argument.substring(0, parenPos);
		String arguments = argument.substring(parenPos);
		List<String> args = new ArrayList<>();
		if(arguments.startsWith("<") && arguments.endsWith(">")) {
			arguments = arguments.substring(1, arguments.length()-1);
			int nestedLevels = 0;
			int beginIndex = 0;
			for (int i = 0, length = arguments.length(); i < length; ++i) {
				char c = arguments.charAt(i);
				switch(c) {
				case ',':
					if(nestedLevels==0) {
						String arg = arguments.substring(beginIndex, i);
						args.add(typeArgs(arg));
						beginIndex = i+1;
					}
					break;
				case '(':
				case '<':
				case '[':
					++nestedLevels;
					break;
				case ')':
				case '>':
				case ']':
					--nestedLevels;
					break;
				}
			}
			String arg = arguments.substring(beginIndex);
			if(!arg.trim().isEmpty()) {
				args.add(typeArgs(arg));
			}
			String _container = getExternalTypeName(container).replace('$', '.').replace('/', '.');
			if(_container.isEmpty())
				_container = fromUnknownType(container);
			return String.format("%1$s<%2$s>", _container, String.join(", ", args));
		}else {
			String _argument = getExternalTypeName(argument).replace('$', '.').replace('/', '.');
			if(_argument.isEmpty())
				_argument = fromUnknownType(argument);
			return _argument;
		}
	}
	
	@Override
	public void acceptWidget(DomWidget widget) {
		String superClassName;
		if(WriteImports.m_classToPackage.containsKey(widget.attributeClass())) {
			superClassName = WriteImports.m_classToPackage.get(widget.attributeClass());
			if(superClassName==null || superClassName.isEmpty())
				superClassName = widget.attributeClass();
			else if(superClassName.endsWith("*"))
				superClassName = superClassName.replace("*", widget.attributeClass());
		}else {
			superClassName = widget.attributeClass();
			DomCustomWidget customWidget = cWidgetsInfo.customWidget(superClassName);
			if(customWidget!=null) {
				if(superClassName.contains("::"))
					superClassName = superClassName.replace("::", ".");
		    	else if(customWidget.hasElementHeader()) {
		    		if(!customWidget.elementHeader().text().toLowerCase().endsWith(".h"))
		    			superClassName = customWidget.elementHeader().text().replace("::", ".");
		    	}
			}
		}
		m_output.append("public class ").append(m_className).append(" extends ").append(superClassName).append(" {").endl().endl()
				.append(m_option.indent).append("private final ").append(m_option.prefix).append(m_className).append(m_option.postfix).append(" ui = new ").append(m_option.prefix).append(m_className).append(m_option.postfix).append("();").endl();
		boolean hasConstructor = false;
		try {
			Class<?> superClass = Class.forName(superClassName);
			Map<Integer,Map<String,Constructor<?>>> constructors = new TreeMap<>();
			for(Constructor<?> constructor : superClass.getConstructors()) {
				Map<String,Constructor<?>> map = constructors.computeIfAbsent(constructor.getParameterCount(), i -> new TreeMap<>());
				map.put(constructor.toString(), constructor);
			}
			for(Map<String,Constructor<?>> map : constructors.values()) {
				for(Constructor<?> constructor : map.values()) {
					if(Modifier.isPublic(constructor.getModifiers())) {
						hasConstructor = true;
						m_output.endl().append(m_option.indent).append("public ").append(m_className).append("(");
						Parameter[] parameters = constructor.getParameters();
						for (int i = 0; i < parameters.length; i++) {
							if(i>0)
								m_output.append(", ");
							String typeName = parameters[i].getType().getTypeName().replace('$', '.');
							if(i==parameters.length-1 && parameters[i].isVarArgs()) {
								typeName = typeName.replaceFirst("\\[\\]$", "...");
							}
							m_output.append(typeName);
							m_output.append(' ');
							m_output.append(parameters[i].getName());
						}
						m_output.append(") {").endl()
								.append(m_option.indent).append(m_option.indent).append("super(");
						for (int i = 0; i < parameters.length; i++) {
							if(i>0)
								m_output.append(", ");
							m_output.append(parameters[i].getName());
						}
						m_output.append(");").endl()
								.append(m_option.indent).append(m_option.indent).append("ui.setupUi(this);").endl()
								.append(m_option.indent).append("}").endl();
					}
				}
			}
		}catch(ClassNotFoundException t) {
			Logger.getLogger("qtjambi.uic").log(Level.WARNING, "Unable to load class "+superClassName+". Please make sure to add it to UIC's classpath.", t);
		}catch(Throwable t) {}
		if(!hasConstructor) {
			m_output.endl().append(m_option.indent).append("public ").append(m_className).append("() {").endl()
					.append(m_option.indent).append(m_option.indent).append("ui.setupUi(this);").endl()
					.append(m_option.indent).append("}").endl();
		}
		
		m_output.endl()
				.append(m_option.indent).append("@Override").endl()
				.append(m_option.indent).append("protected void changeEvent(io.qt.core.QEvent e) {").endl()
				.append(m_option.indent).append(m_option.indent).append("super.changeEvent(e);").endl()
				.append(m_option.indent).append(m_option.indent).append("switch (e.type()) {").endl()
				.append(m_option.indent).append(m_option.indent).append("case LanguageChange: ui.retranslateUi(this); break;").endl()
				.append(m_option.indent).append(m_option.indent).append("default: break;").endl()
				.append(m_option.indent).append(m_option.indent).append("}").endl()
				.append(m_option.indent).append("}").endl();
		
		if(m_elementSlots!=null) {
			for(String signal : m_elementSlots.elementSignal()) {
				int parenPos = signal.indexOf('(');
				String functionName = signal.substring(0, parenPos);
				String arguments = signal.substring(parenPos);
				List<String> args = new ArrayList<>();
				if(arguments.startsWith("(") && arguments.endsWith(")")) {
					arguments = arguments.substring(1, arguments.length()-1);
					int nestedLevels = 0;
					int beginIndex = 0;
					for (int i = 0, length = arguments.length(); i < length; ++i) {
						char c = arguments.charAt(i);
						switch(c) {
						case ',':
							if(nestedLevels==0) {
								String arg = arguments.substring(beginIndex, i);
								args.add(typeArgs(arg));
								beginIndex = i+1;
							}
							break;
						case '(':
						case '<':
						case '[':
							++nestedLevels;
							break;
						case ')':
						case '>':
						case ']':
							--nestedLevels;
							break;
						}
					}
					String arg = arguments.substring(beginIndex);
					if(!arg.trim().isEmpty()) {
						args.add(typeArgs(arg));
					}
				}
				m_output.endl().append(m_option.indent).append("public final Signal").append(args.size());
				if(!args.isEmpty()) {
					m_output.append("<").append(String.join(", ", args)).append(">");
				}
				m_output.append(" ").append(functionName).append(" = new Signal").append(args.size());
				if(!args.isEmpty()) {
					m_output.append("<>");
				}
				m_output.append("();").endl();
			}
			
			for(String slot : m_elementSlots.elementSlot()) {
				int parenPos = slot.indexOf('(');
				String functionName = slot.substring(0, parenPos);
				String arguments = slot.substring(parenPos);
				List<String> args = new ArrayList<>();
				if(arguments.startsWith("(") && arguments.endsWith(")")) {
					arguments = arguments.substring(1, arguments.length()-1);
					int nestedLevels = 0;
					int beginIndex = 0;
					for (int i = 0, length = arguments.length(); i < length; ++i) {
						char c = arguments.charAt(i);
						switch(c) {
						case ',':
							if(nestedLevels==0) {
								String arg = arguments.substring(beginIndex, i);
								args.add(typeArgs(arg));
								beginIndex = i+1;
							}
							break;
						case '(':
						case '<':
						case '[':
							++nestedLevels;
							break;
						case ')':
						case '>':
						case ']':
							--nestedLevels;
							break;
						}
					}
					String arg = arguments.substring(beginIndex);
					if(!arg.trim().isEmpty()) {
						args.add(typeArgs(arg));
					}
				}
				m_output.endl().append(m_option.indent).append("public void ").append(functionName).append('(');
				for (int i = 0; i < args.size(); i++) {
					if(i>0)
						m_output.append(", ");
					m_output.append(args.get(i)).append(" arg").append(i+1);
				}
				m_output.append("){").endl()
						.append(m_option.indent).append("}").endl();
			}
		}
		
		m_output.append("}").endl();
	}
	
	public static native String getExternalTypeName(String internalTypeName);

	protected QTextStream output() { return m_output; }

	private final String m_className;
	private final QTextStream m_output;
    private final Option m_option;
    private final CustomWidgetsInfo cWidgetsInfo;
	private DomSlots m_elementSlots;
}
