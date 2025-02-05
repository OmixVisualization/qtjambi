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
package io.qt.uic;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.core.QMetaMethod;
import io.qt.core.QMetaMethod.MethodType;
import io.qt.core.QMetaObject;
import io.qt.core.QPair;
import io.qt.uic.java.WriteImports;
import io.qt.uic.ui4.DomCustomWidget;
import io.qt.uic.ui4.DomCustomWidgets;
import io.qt.uic.ui4.DomUI;

/**
 * @hidden
 */
public class CustomWidgetsInfo extends TreeWalker{
	
	public DomCustomWidget customWidget(String name)
    { return m_customWidgets.get(name); }
	
	@Override
	public void acceptUI(DomUI node)
	{
	    m_customWidgets.clear();

	    if (node.elementCustomWidgets()!=null)
	        acceptCustomWidgets(node.elementCustomWidgets());
	}

	@Override
	public void acceptCustomWidgets(DomCustomWidgets node)
	{
	    super.acceptCustomWidgets(node);
	}

	@Override
	public void acceptCustomWidget(DomCustomWidget node)
	{
	    if (node.elementClass().isEmpty())
	        return;

	    m_customWidgets.put(node.elementClass(), node);
	}

	public final boolean _extends(String classNameIn, String baseClassName)
	{
	    if (Objects.equals(classNameIn, baseClassName))
	        return true;

	    String className = classNameIn;
	    DomCustomWidget c = customWidget(className);
	    while (c!=null) {
	        String _extends = c.elementExtends();
	        if (Objects.equals(className, _extends)) // Faulty legacy custom widget entries exist.
	            return false;
	        if (Objects.equals(_extends, baseClassName))
	            return true;
	        className = _extends;
	        c = customWidget(className);
	    }
	    return false;
	}

	public final boolean extendsOneOf(String classNameIn, List<String> baseClassNames)
	{
	    if (baseClassNames.contains(classNameIn))
	        return true;

	    String className = classNameIn;
	    DomCustomWidget c = customWidget(className);
	    while (c!=null) {
	        String _extends = c.elementExtends();
	        if (Objects.equals(className, _extends)) // Faulty legacy custom widget entries exist.
	            return false;
	        if (baseClassNames.contains(_extends))
	            return true;
	        className = _extends;
	        c = customWidget(className);
	    }
	    return false;
	}

	public final boolean isCustomWidgetContainer(String className)
	{
		DomCustomWidget dcw = m_customWidgets.getOrDefault(className, null);
	    if (dcw!=null)
	        if (dcw.hasElementContainer())
	            return dcw.elementContainer() != 0;
	    return false;
	}
	
	/**
	 * @hidden
	 */
	public static class MethodInfo{
		private MethodInfo(QMetaMethod metaMethod, Method method, boolean isAmbiguous) {
			this.metaMethod = metaMethod;
			this.method = method;
			this.isAmbiguous = isAmbiguous;
		}
		public final QMetaMethod metaMethod;
		public final Method method;
		public final boolean isAmbiguous;
	}

	public final MethodInfo methodInfo(String className, String signature, boolean isSignal)
	{
		String javaCassName;
		if(WriteImports.m_classToPackage.containsKey(className)) {
			javaCassName = WriteImports.m_classToPackage.get(className);
			if(javaCassName==null || javaCassName.isEmpty())
				javaCassName = className;
			else if(javaCassName.endsWith("*"))
				javaCassName = javaCassName.replace("*", className);
		}else {
			javaCassName = className;
			DomCustomWidget customWidget = customWidget(javaCassName);
			if(customWidget!=null) {
				if(javaCassName.contains("::"))
					javaCassName = javaCassName.replace("::", ".");
		    	else if(customWidget.hasElementHeader()) {
		    		if(!customWidget.elementHeader().text().toLowerCase().endsWith(".h"))
		    			javaCassName = customWidget.elementHeader().text().replace("::", ".");
		    	}
			}
		}
		try {
			Class<?> javaClass = Class.forName(javaCassName);
			QMetaObject mo = QMetaObject.forType(javaClass);
			QMetaMethod matchingMetaMethod = null;
			Set<Method> reflectedMethods = new HashSet<>();
			List<QMetaMethod> metaMethods = new ArrayList<>();
			int parenPos = signature.indexOf('(');
			String functionName = signature.substring(0, parenPos);
			for(QMetaMethod mtd : mo.methods()) {
				if(isSignal && mtd.methodType()!=MethodType.Signal)
					continue;
				if(mtd.name().toString().equals(functionName)) {
					metaMethods.add(mtd);
					try {
						reflectedMethods.add(mtd.toReflectedMethod());
					}catch(Throwable t) {}
					if(mtd.cppMethodSignature().toString().equals(signature)) {
						matchingMetaMethod = mtd;
					}
				}
			}
			List<Method> methods = new ArrayList<>();
			if(!isSignal) {
				for(Method method : javaClass.getMethods()) {
					if(!reflectedMethods.contains(method) && method.getName().equals(functionName)) {
						methods.add(method);
					}
				}
			}
			if(metaMethods.size() + methods.size()==1) {
				if(metaMethods.size()==1)
					return new MethodInfo(metaMethods.get(0), null, false);
				else
					return new MethodInfo(null, methods.get(0), false);
			}else
				return new MethodInfo(matchingMetaMethod, null, true);
		}catch(ClassNotFoundException t) {
			Logger.getLogger("qtjambi.uic").log(Level.WARNING, "Unable to load class "+javaCassName+". Please make sure to add it to UIC's classpath.", t);
		}catch(Throwable t) {}
	    return new MethodInfo(null, null, false);
	}
	
	public final String realClassName(String className)
	{
	    if ("Line".equals(className))
	        return "QFrame";
	    DomCustomWidget dcw = m_customWidgets.getOrDefault(className, null);
	    if (dcw!=null) {
	    	className = dcw.elementClass();
	    	String[] namespaces = className.split("::");
		    if (namespaces.length>0) {
		    	className = namespaces[namespaces.length-1];
		    }
	    }
	    return className;
	}

	public final String customWidgetAddPageMethod(String name)
	{
		DomCustomWidget dcw = m_customWidgets.getOrDefault(name, null);
	    if (dcw!=null)
	        return dcw.elementAddPageMethod();
	    return "";
	}

	@SuppressWarnings({ "unchecked" })
	private static QPair<String, String> addPageMethods[] = new QPair[]{
	        new QPair<>("QStackedWidget", "addWidget"),
			new QPair<>("QToolBar", "addWidget"),
			new QPair<>("QDockWidget", "setWidget"),
			new QPair<>("QScrollArea", "setWidget"),
			new QPair<>("QSplitter", "addWidget"),
			new QPair<>("QMdiArea", "addSubWindow")
	    };
	// add page methods for simple containers taking only the widget parameter
	public final String simpleContainerAddPageMethod(String name)
	{
	    for (QPair<String, String> m : addPageMethods) {
	        if (_extends(name, m.first))
	            return m.second;
	    }
	    return "";
	}

	private final Map<String, DomCustomWidget> m_customWidgets = new TreeMap<>();
}
