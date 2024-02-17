/****************************************************************************
**
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

package io.qt.designer.util;

import java.lang.reflect.Array;
import java.lang.reflect.GenericArrayType;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;

import io.qt.core.QMetaObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.designer.QDesignerCustomWidgetInterface;
import io.qt.gui.QIcon;
import io.qt.widgets.QWidget;

/**
 * Generic implementation for {@link QDesignerCustomWidgetInterface}.
 */
public final class CustomWidgetInterface extends io.qt.QtObject implements QDesignerCustomWidgetInterface{
	
	static {
		QtJambi_LibraryUtilities.initialize();
	}
	
	public CustomWidgetInterface(String group, QMetaObject.Method1<QWidget, ? extends QWidget> constructor) {
		super();
		this.group = group;
		this.constructor = constructor;
		Class<?> resultType = QtJambi_LibraryUtilities.internal.lambdaReturnType(QMetaObject.Method1.class, constructor);
		QMetaObject metaObject = null;
		if(resultType==null) {
			Class<?> implClass = null;
			for(Type iface : constructor.getClass().getGenericInterfaces()) {
				if(iface instanceof ParameterizedType) {
					ParameterizedType piface = (ParameterizedType)iface;
					if(piface.getRawType()==QMetaObject.Method1.class) {
						Type[] args = piface.getActualTypeArguments();
						for (int i = 1; i < args.length; i++) {
							if(args[i] instanceof Class) {
								implClass = (Class<?>)args[i];
							}else if(args[i] instanceof ParameterizedType) {
								implClass = (Class<?>)((ParameterizedType)args[i]).getRawType();
							} else if (args[i] instanceof GenericArrayType) {
								GenericArrayType gtype = (GenericArrayType)args[i];
								if (gtype.getGenericComponentType() instanceof Class) {
									implClass = (Class<?>) gtype.getGenericComponentType();
								} else if (gtype.getGenericComponentType() instanceof ParameterizedType) {
									ParameterizedType ptype = (ParameterizedType) gtype.getGenericComponentType();
									implClass = (Class<?>) ptype.getRawType();
								} else if (gtype.getGenericComponentType() instanceof TypeVariable<?>) {
									TypeVariable<?> tv = (TypeVariable<?>)gtype.getGenericComponentType();
									Type[] bounds = tv.getBounds();
									if(bounds==null || bounds.length==0)
										implClass = Object.class;
									else if(bounds[0] instanceof Class)
										implClass = (Class<?>)bounds[0];
									else if(bounds[0] instanceof ParameterizedType)
										implClass = (Class<?>)((ParameterizedType)bounds[0]).getRawType();
									implClass = Array.newInstance(implClass, 0).getClass();
								}
							}
						}
					}
				}
			}
			if(implClass==null) {
				if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
					try {
						QWidget widget = constructor.invoke(null);
						metaObject = widget.metaObject();
						widget.dispose();
					} catch (Throwable e) {
					}
				}else {
					throw new RuntimeException("Constructor handle required as argument, e.g. QWidget::new");
				}
	    	}else {
	    		metaObject = QMetaObject.forType(implClass);
	    	}
		}else {
			metaObject = QMetaObject.forType(resultType);
		}
		if(metaObject!=null)
			name = metaObject.className();
		else if(resultType!=null)
			name = resultType.getName().replace(".", "::");
		else
			name = QWidget.class.getName().replace(".", "::");
	}

	public CustomWidgetInterface(String group, QMetaObject.Method1<QWidget, ? extends QWidget> constructor, String name) {
		super();
		this.group = group;
		this.constructor = constructor;
		this.name = name;
	}

	public CustomWidgetInterface(String group, QMetaObject.Method1<QWidget, ? extends QWidget> constructor, String whatsThis,
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
	private final QMetaObject.Method1<QWidget,? extends QWidget> constructor;
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
