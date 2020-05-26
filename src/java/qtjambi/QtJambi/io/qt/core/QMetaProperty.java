/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved
** Copyright (C) 2017-2019 Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.core;

import io.qt.QtUninvokable;
import io.qt.internal.NativeAccess;
import io.qt.internal.QtJambiInternal;

/**
 * Information about a property in a QObject subclass.
 */
public final class QMetaProperty {
	private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger("io.qt.internal");
	
    private final static int Invalid = 0x00000000;
    private final static int Readable = 0x00000001;
    private final static int Writable = 0x00000002;
    private final static int Resettable = 0x00000004;
    private final static int EnumOrFlag = 0x00000008;
    private final static int StdCppSet = 0x00000100;
//	     private final static int Override = 0x00000200;
    private final static int Constant = 0x00000400;
    private final static int Final = 0x00000800;
    private final static int Designable = 0x00001000;
    private final static int ResolveDesignable = 0x00002000;
    private final static int Scriptable = 0x00004000;
    private final static int ResolveScriptable = 0x00008000;
    private final static int Stored = 0x00010000;
    private final static int ResolveStored = 0x00020000;
    private final static int Editable = 0x00040000;
    private final static int ResolveEditable = 0x00080000;
    private final static int User = 0x00100000;
    private final static int ResolveUser = 0x00200000;
    private final static int Notify = 0x00400000;
    private final static int Revisioned = 0x00800000;
	
    private final int flags;
    private final String name;
    private final Class<?> type;
    private final int propertyIndex;
	private final QMetaObject metaObject;

    @NativeAccess
    private QMetaProperty(QMetaObject metaObject, int propertyIndex, String name, Class<?> type, int flags) {
    	this.flags = flags;
        this.propertyIndex = propertyIndex;
        this.name = name;
        this.metaObject = metaObject;
//        Class<?> type = Object.class;
//		switch(typeName) {
//		case "": type = Object.class; break;
//		case "int": type = int.class; break;
//		case "short": type = short.class; break;
//		case "long": type = long.class; break;
//		case "byte": type = byte.class; break;
//		case "boolean": type = boolean.class; break;
//		case "double": type = double.class; break;
//		case "char": type = char.class; break;
//		case "float": type = float.class; break;
//		default:
//			try {
//				type = Class.forName(typeName.replace('/', '.'));
//			} catch (Throwable e) {
//				logger.log(java.util.logging.Level.SEVERE, e, ()->"Unable to determine java type for property "+name);
//			}
//		}
        this.type = type;
    }

	@QtUninvokable
    public final boolean isWritable() { 
		return (this.flags & Writable) == Writable; 
    }
	
	@QtUninvokable
    public final boolean isResettable() { 
		return (this.flags & Resettable) == Resettable; 
    }
	
	@QtUninvokable
    public final boolean isDesignable() { 
		return (this.flags & Designable) == Designable; 
    }
	
	@QtUninvokable
    public final boolean isUser() {
		return (this.flags & User) == User;
    }
	
	@QtUninvokable
    public final String name() { 
		return name; 
	}
	
	@QtUninvokable
    public final boolean isReadable() {
		return (this.flags & Readable) == Readable;
	}

	@QtUninvokable
	public final boolean hasNotifySignal() {
		return (this.flags & Notify) == Notify;
	}

	@QtUninvokable
	public final boolean isConstant() {
		return (this.flags & Constant) == Constant;
	}

	@QtUninvokable
	public final boolean isFinal() {
		return (this.flags & Final) == Final;
	}

	@QtUninvokable
	public final Class<?> type() {
		return type;
	}
	
	@QtUninvokable
	public int propertyIndex() {
		return propertyIndex;
	}

    @Override
    @QtUninvokable
    public final String toString() {
        return name;
    }
    
    @QtUninvokable
	public final boolean isEditable(QObject object) {
    	QtJambiInternal.threadCheck(object);
		return query(object, QMetaObject.Call.QueryPropertyEditable) || (this.flags & Editable) == Editable;
    }
    
    @QtUninvokable
	public final boolean isDesignable(QObject object) {
    	QtJambiInternal.threadCheck(object);
		return query(object, QMetaObject.Call.QueryPropertyDesignable) || isDesignable();
    }
    
    @QtUninvokable
	public final boolean isScriptable(QObject object) {
    	QtJambiInternal.threadCheck(object);
		return query(object, QMetaObject.Call.QueryPropertyScriptable) || (this.flags & Scriptable) == Scriptable;
    }
    
    @QtUninvokable
	public final boolean isStored(QObject object) {
    	QtJambiInternal.threadCheck(object);
		return query(object, QMetaObject.Call.QueryPropertyStored) || (this.flags & Stored) == Stored;
    }
    
    @QtUninvokable
	public final boolean isUser(QObject object) {
    	QtJambiInternal.threadCheck(object);
		return query(object, QMetaObject.Call.QueryPropertyUser) || (this.flags & User) == User;
    }
    
    @QtUninvokable
	public final Object read(QObject object) {
    	if((this.flags & Readable) == Readable) {
        	QtJambiInternal.threadCheck(object);
        	Class<?> type = this.type;
        	if(this.type==int.class) {
        		type = Integer.class;
        	}else if(this.type==short.class) {
        		type = Short.class;
        	}else if(this.type==long.class) {
        		type = Long.class;
        	}else if(this.type==byte.class) {
        		type = Byte.class;
        	}else if(this.type==boolean.class) {
        		type = Boolean.class;
        	}else if(this.type==double.class) {
        		type = Double.class;
        	}else if(this.type==float.class) {
        		type = Float.class;
        	}else if(this.type==char.class) {
        		type = Character.class;
        	}
    		return type.cast(read(io.qt.internal.QtJambiInternal.nativeId(object), metaObject.metaObjectPointer, propertyIndex));
    	}
    	return null;
    }
    
    @QtUninvokable
	public final Object readOnGadget(Object object) {
    	if((this.flags & Readable) == Readable) {
    		QMetaObject objectMO = QMetaObject.forGadget(object);
    		if(objectMO!=metaObject && !objectMO.inherits(metaObject)) {
    			throw new IllegalArgumentException(String.format("Given gadget is not an instance of %1$s", metaObject.className()));
    		}
        	Class<?> type = this.type;
        	if(this.type==int.class) {
        		type = Integer.class;
        	}else if(this.type==short.class) {
        		type = Short.class;
        	}else if(this.type==long.class) {
        		type = Long.class;
        	}else if(this.type==byte.class) {
        		type = Byte.class;
        	}else if(this.type==boolean.class) {
        		type = Boolean.class;
        	}else if(this.type==double.class) {
        		type = Double.class;
        	}else if(this.type==float.class) {
        		type = Float.class;
        	}else if(this.type==char.class) {
        		type = Character.class;
        	}
    		return type.cast(readOnGadget(object, metaObject.metaObjectPointer, propertyIndex));
    	}
    	return null;
    }
    
    @QtUninvokable
	public final boolean reset(QObject object){
    	if(isResettable()) {
        	QtJambiInternal.threadCheck(object);
        	return reset(io.qt.internal.QtJambiInternal.nativeId(object), metaObject.metaObjectPointer, propertyIndex);
    	}
    	return false;
    }
    
    @QtUninvokable
	public final boolean resetOnGadget(Object object){
    	if(isResettable()) {
    		QMetaObject objectMO = QMetaObject.forGadget(object);
    		if(objectMO!=metaObject && !objectMO.inherits(metaObject)) {
    			throw new IllegalArgumentException(String.format("Given gadget is not an instance of %1$s", metaObject.className()));
    		}
        	return resetOnGadget(object, metaObject.metaObjectPointer, propertyIndex);
    	}
    	return false;
    }
    
    @QtUninvokable
	public final boolean write(QObject object, Object value) {
    	if(isWritable()) {
        	QtJambiInternal.threadCheck(object);
        	Class<?> type = this.type;
        	if(this.type==int.class) {
        		type = Integer.class;
        	}else if(this.type==short.class) {
        		type = Short.class;
        	}else if(this.type==long.class) {
        		type = Long.class;
        	}else if(this.type==byte.class) {
        		type = Byte.class;
        	}else if(this.type==boolean.class) {
        		type = Boolean.class;
        	}else if(this.type==double.class) {
        		type = Double.class;
        	}else if(this.type==float.class) {
        		type = Float.class;
        	}else if(this.type==char.class) {
        		type = Character.class;
        	}
        	if(this.type.isPrimitive() && value==null)
        		throw new IllegalArgumentException(String.format("Null not allowed for property %1$s of type %2$s.", this.name, this.type.getName()));
    		return write(io.qt.internal.QtJambiInternal.nativeId(object), metaObject.metaObjectPointer, propertyIndex, type.cast(value));
    	}
    	return false;
    }
    
    @QtUninvokable
	public final boolean writeOnGadget(Object object, Object value) {
    	if(isWritable()) {
    		QMetaObject objectMO = QMetaObject.forGadget(object);
    		if(objectMO!=metaObject && !objectMO.inherits(metaObject)) {
    			throw new IllegalArgumentException(String.format("Given gadget is not an instance of %1$s", metaObject.className()));
    		}
        	Class<?> type = this.type;
        	if(this.type==int.class) {
        		type = Integer.class;
        	}else if(this.type==short.class) {
        		type = Short.class;
        	}else if(this.type==long.class) {
        		type = Long.class;
        	}else if(this.type==byte.class) {
        		type = Byte.class;
        	}else if(this.type==boolean.class) {
        		type = Boolean.class;
        	}else if(this.type==double.class) {
        		type = Double.class;
        	}else if(this.type==float.class) {
        		type = Float.class;
        	}else if(this.type==char.class) {
        		type = Character.class;
        	}
    		return writeOnGadget(object, metaObject.metaObjectPointer, propertyIndex, type.cast(value));
    	}
    	return false;
    }
    
    @QtUninvokable
	public final QMetaMethod notifySignal() {
    	if(hasNotifySignal()) {
    		return notifySignal(metaObject, metaObject.metaObjectPointer, propertyIndex);
    	}
    	return null;
    }
    
    @QtUninvokable
	public final QMetaObject.AbstractSignal notifySignal(QObject object) {
    	QtJambiInternal.threadCheck(object);
    	QMetaMethod notifySignal = notifySignal();
    	return notifySignal==null ? null : QtJambiInternal.findSignal(object, notifySignal.name(), notifySignal.parameterTypes().toArray(new Class[notifySignal.parameterTypes().size()]));
    }
    
    @QtUninvokable
    private boolean query(QObject object, QMetaObject.Call call) {
    	return query(io.qt.internal.QtJambiInternal.checkedNativeId(object), propertyIndex, call.value());
    }
    
    @QtUninvokable
    private static native boolean query(long object__id, int propertyAbsoluteIndex, int call);
    
    @QtUninvokable
    private static native boolean reset(long object__id, long metaObjectPointer, int propertyIndex);
    
    @QtUninvokable
    private static native boolean write(long object__id, long metaObjectPointer, int propertyIndex, Object value);
    
    @QtUninvokable
    private static native Object read(long object__id, long metaObjectPointer, int propertyIndex);
    
    @QtUninvokable
    private static native boolean resetOnGadget(Object object, long metaObjectPointer, int propertyIndex);
    
    @QtUninvokable
    private static native boolean writeOnGadget(Object object, long metaObjectPointer, int propertyIndex, Object value);
    
    @QtUninvokable
    private static native Object readOnGadget(Object object, long metaObjectPointer, int propertyIndex);
    
    @QtUninvokable
    private static native QMetaMethod notifySignal(QMetaObject metaObject, long metaObjectPointer, int propertyIndex);
    
    @Override
    @QtUninvokable
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + metaObject.hashCode();
		result = prime * result + propertyIndex;
		return result;
	}

	@Override
    @QtUninvokable
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		QMetaProperty other = (QMetaProperty) obj;
		if (!metaObject.equals(other.metaObject))
			return false;
		if (propertyIndex != other.propertyIndex)
			return false;
		return true;
	}
}
