/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.designer.util;

import io.qt.core.QMetaObject;
import io.qt.designer.QDesignerCustomWidgetInterface;
import io.qt.gui.QIcon;
import io.qt.internal.QtJambiInternal;
import io.qt.widgets.QWidget;

/**
 * Generic implementation for {@link QDesignerCustomWidgetInterface}.
 */
public final class CustomWidgetInterface extends io.qt.QtObject implements QDesignerCustomWidgetInterface{

	public CustomWidgetInterface(String group, QMetaObject.Method1<QWidget, QWidget> constructor) {
		super();
		this.group = group;
		this.constructor = constructor;
		Class<?> resultType = QtJambiInternal.getReturnType(constructor);
		if(resultType==null)
			throw new IllegalArgumentException("constructor argument needs to be a method handle like Type::new.");
		QMetaObject mo = QMetaObject.forType(resultType);
		if(mo!=null)
			name = mo.className();
		else
			name = resultType.getName();
	}

	public CustomWidgetInterface(String group, QMetaObject.Method1<QWidget, QWidget> constructor, String name) {
		super();
		this.group = group;
		this.constructor = constructor;
		this.name = name;
	}

	public CustomWidgetInterface(String group, QMetaObject.Method1<QWidget, QWidget> constructor, String whatsThis,
			String name, String toolTip, boolean isContainer, String includeFile, QIcon icon) {
		super();
		this.group = group;
		this.constructor = constructor;
		this.whatsThis = whatsThis;
		this.name = name;
		this.toolTip = toolTip;
		this.isContainer = isContainer;
		this.includeFile = includeFile;
		this.icon = icon;
	}

	private final String group;
	private final QMetaObject.Method1<QWidget,QWidget> constructor;
	private String whatsThis;
	private final String name;
	private String toolTip;
	private boolean isContainer;
	private String includeFile;
	private QIcon icon;
	private String domXml;
			
	public void setDomXml(String domXml) {
		this.domXml = domXml;
	}

	public void setWhatsThis(String whatsThis) {
		this.whatsThis = whatsThis;
	}

	public void setToolTip(String toolTip) {
		this.toolTip = toolTip;
	}

	public void setContainer(boolean isContainer) {
		this.isContainer = isContainer;
	}

	public void setIncludeFile(String includeFile) {
		this.includeFile = includeFile;
	}

	public void setIcon(QIcon icon) {
		this.icon = icon;
	}

	@Override
	public String whatsThis() {
		return whatsThis;
	}
	
	@Override
	public String toolTip() {
		return toolTip;
	}
	
	@Override
	public String name() {
		return name;
	}
	
	@Override
	public boolean isContainer() {
		return isContainer;
	}
	
	@Override
	public String includeFile() {
		return includeFile;
	}
	
	@Override
	public QIcon icon() {
		return icon;
	}
	
	@Override
	public String group() {
		return group;
	}
	
	@Override
	public QWidget createWidget(QWidget parent) {
		try {
			return constructor.invoke(parent);
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	public String domXml() {
		return domXml==null ? QDesignerCustomWidgetInterface.super.domXml() : domXml;
	}
}
