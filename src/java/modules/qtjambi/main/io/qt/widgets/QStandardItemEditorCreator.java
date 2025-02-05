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

package io.qt.widgets;

import java.io.Serializable;
import java.lang.invoke.MethodHandleInfo;
import java.lang.invoke.MethodType;
import java.lang.invoke.SerializedLambda;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.Objects;
import java.util.function.Function;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.StrictNonNull;
import io.qt.core.QByteArray;
import io.qt.core.QMetaObject;
import io.qt.core.QOperatingSystemVersion;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qstandarditemeditorcreator.html">QStandardItemEditorCreator</a></p>
 */
public class QStandardItemEditorCreator<T extends QWidget> implements QItemEditorCreatorBase {
	
	/**
	 * Item editor widget factory handle.
	 */
	public interface Factory<@Nullable T> extends Function<@Nullable QWidget, @Nullable T>, Serializable{
	}
	
	private final QByteArray valuePropertyName;
	private final Function<QWidget, T> constructorHandle;
	
    public QStandardItemEditorCreator(@StrictNonNull Factory<@Nullable T> constructor) {
        super();
        SerializedLambda serializedLambda = QtJambi_LibraryUtilities.internal.serializeLambdaExpression(Objects.requireNonNull(constructor, "Argument 'constructor': null not expected."));
        QMetaObject metaObject = null;
        if(serializedLambda==null) {
            Class<?> implClass = null;
			for(Type iface : QtJambi_LibraryUtilities.internal.getClass(constructor).getGenericInterfaces()) {
				if(iface instanceof ParameterizedType) {
					ParameterizedType piface = (ParameterizedType)iface;
					if(piface.getRawType()==Factory.class) {
						Type[] args = piface.getActualTypeArguments();
						for (int i = 0; i < args.length; i++) {
							implClass = QtJambi_LibraryUtilities.internal.toClass(args[i]);
						}
					}
				}
			}
			if(implClass==null) {
				if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
					QWidget widget = constructor.apply(null);
					metaObject = widget.metaObject();
					widget.dispose();
				}else {
					throw new RuntimeException("Constructor handle required as argument, e.g. QWidget::new");
				}
	    	}else {
	    		metaObject = QMetaObject.forType(implClass);
	    	}
        }else{
        	metaObject = LambdaTools.metaObjectFromLambda(serializedLambda, constructor);
        }
		this.valuePropertyName = new QByteArray(metaObject.userProperty().name());
        this.constructorHandle = constructor;
    }
    
    private static class LambdaTools{
    	static <T> QMetaObject metaObjectFromLambda(SerializedLambda serializedLambda, Factory<T> constructor) {
    		if(serializedLambda.getImplMethodKind()==MethodHandleInfo.REF_newInvokeSpecial){
    	        try {
    	        	return QMetaObject.forType(QtJambi_LibraryUtilities.internal.getClass(constructor).getClassLoader().loadClass(serializedLambda.getImplClass().replace('/', '.')));
    			} catch (ClassNotFoundException e) {
    				throw new RuntimeException(e);
    			}
            }else{
            	MethodType methodType = MethodType.fromMethodDescriptorString(serializedLambda.getImplMethodSignature(), QtJambi_LibraryUtilities.internal.getClass(constructor).getClassLoader());
            	return QMetaObject.forType(methodType.returnType());
            }
    	}
    }
    
    public QStandardItemEditorCreator(@StrictNonNull Class<T> widgetType) {
        this(widgetType, QItemEditorCreator.findConstructor("QStandardItemEditorCreator", widgetType));
    }
    
    private QStandardItemEditorCreator(Class<T> widgetType, Function<QWidget, T> constructorHandle) {
    	super();
        this.constructorHandle = Objects.requireNonNull(constructorHandle, "Argument 'constructorHandle': null not expected.");
        this.valuePropertyName = new QByteArray(QMetaObject.forType(widgetType).userProperty().name());
    }

    @Override
    public final @Nullable T createWidget(@Nullable QWidget parent) {
    	return this.constructorHandle.apply(parent);
    }

    @Override
    public final @NonNull QByteArray valuePropertyName() {
        return valuePropertyName.clone();
    }
}
