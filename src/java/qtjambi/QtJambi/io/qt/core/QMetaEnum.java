/****************************************************************************
**
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
package io.qt.core;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

import io.qt.QFlags;
import io.qt.QNoSuchEnumValueException;
import io.qt.QtAbstractEnumerator;
import io.qt.QtByteEnumerator;
import io.qt.QtFlagEnumerator;
import io.qt.QtLongEnumerator;
import io.qt.QtShortEnumerator;
import io.qt.QtUninvokable;
import io.qt.internal.NativeAccess;

/**
 * Information about a property in a QObject subclass.
 */
public final class QMetaEnum {
	private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger("io.qt.internal");
    final int enumeratorIndex;
	private final QMetaObject enclosingMetaObject;

    @NativeAccess
    private QMetaEnum(QMetaObject metaObject, int enumeratorIndex) {
        this.enumeratorIndex = enumeratorIndex;
        this.enclosingMetaObject = metaObject;
        type(); // as initialization
    }
    
    @QtUninvokable
    public Class<?> type() {
		return enclosingMetaObject.enumType(this);
	}

    @QtUninvokable
	public String name() {
    	return name(enclosingMetaObject.metaObjectPointer, enumeratorIndex);
    }
    
    @QtUninvokable
    private static native String name(long metaObjectPointer, int enumeratorIndex);
    
    @QtUninvokable
    public boolean isFlag(){
    	return isFlag(enclosingMetaObject.metaObjectPointer, enumeratorIndex);
    }
    
    @QtUninvokable
    private static native boolean isFlag(long metaObjectPointer, int enumeratorIndex);
    
    @QtUninvokable
    public boolean isScoped(){
    	return isScoped(enclosingMetaObject.metaObjectPointer, enumeratorIndex);
    }
    
    @QtUninvokable
    private static native boolean isScoped(long metaObjectPointer, int enumeratorIndex);
    
    @QtUninvokable
    public String scope(){
    	return scope(enclosingMetaObject.metaObjectPointer, enumeratorIndex);
    }
    
    @QtUninvokable
    private static native String scope(long metaObjectPointer, int enumeratorIndex);

    @QtUninvokable
    public int keyCount() {
    	return keyCount(enclosingMetaObject.metaObjectPointer, enumeratorIndex);
    }
    
    @QtUninvokable
    private static native int keyCount(long metaObjectPointer, int enumeratorIndex);
    
    @QtUninvokable
    public String key(int index) {
    	return key(enclosingMetaObject.metaObjectPointer, enumeratorIndex, index);
    }
    
    @QtUninvokable
    private static native String key(long metaObjectPointer, int enumeratorIndex, int index);
    
    @QtUninvokable
    public List<String> keys() {
    	int keyCount = keyCount();
    	List<String> keys = new ArrayList<>(keyCount);
    	for (int i = 0; i < keyCount; ++i) {
    		keys.add(key(i));
		}
    	return Collections.unmodifiableList(keys);
    }

    @QtUninvokable
    public int value(int index) {
    	return value(enclosingMetaObject.metaObjectPointer, enumeratorIndex, index);
    }
    
    @QtUninvokable
    private static native int value(long metaObjectPointer, int enumeratorIndex, int index);

    @QtUninvokable
    public Integer keyToValue(String key) {
    	return keyToValue(enclosingMetaObject.metaObjectPointer, enumeratorIndex, key);
    }
    
    @QtUninvokable
    private static native Integer keyToValue(long metaObjectPointer, int enumeratorIndex, String key);
    
    @QtUninvokable
    public String valueToKey(int value) {
    	return valueToKey(enclosingMetaObject.metaObjectPointer, enumeratorIndex, value);
    }
    
    @QtUninvokable
    private static native String valueToKey(long metaObjectPointer, int enumeratorIndex, int value);
    
    @QtUninvokable
    public Integer keysToValue(String... keys) {
    	return keysToValue(Arrays.asList(keys));
    }
    
    @QtUninvokable
    public Integer keysToValue(List<String> keys) {
    	return keysToValue(enclosingMetaObject.metaObjectPointer, enumeratorIndex, Objects.requireNonNull(keys));
    }
    
    @QtUninvokable
    private static native Integer keysToValue(long metaObjectPointer, int enumeratorIndex, List<String> keys);
    
    @QtUninvokable
    public List<String> valueToKeys(int value) {
    	return valueToKeys(enclosingMetaObject.metaObjectPointer, enumeratorIndex, value);
    }
    
    @QtUninvokable
    public QtAbstractEnumerator[] entries() {
    	return enclosingMetaObject.enumEntries(this);
    }
    
    @QtUninvokable
    public QtAbstractEnumerator entry(int index) {
    	return enclosingMetaObject.enumEntry(this, index);
    }
    
    @QtUninvokable
    public QtAbstractEnumerator entry(String name) {
    	return enclosingMetaObject.enumEntry(this, name);
    }
    
    @QtUninvokable
    public QFlags<?> flags(int value) {
    	Class<?> enumType = type();
    	if(QtFlagEnumerator.class.isAssignableFrom(enumType)) {
    		@SuppressWarnings("unchecked")
			Class<? extends QtFlagEnumerator> _enumType = (Class<? extends QtFlagEnumerator>)enumType;
    		return flags(_enumType, value);
    	}else{
    		return null;
    	}
    }
    
    @QtUninvokable
    public QFlags<?> flags(String... names) {
    	@SuppressWarnings("unchecked")
		QFlags<QtFlagEnumerator> flags = (QFlags<QtFlagEnumerator>)flags(0);
    	for (String name : names) {
    		QtAbstractEnumerator entry = entry(name);
    		if(entry instanceof QtFlagEnumerator) {
    			flags.set((QtFlagEnumerator)entry);
    		}else {
    			throw new QNoSuchEnumValueException(name);
    		}
		}
    	return flags;
    }
    
    private static native QFlags<? extends QtFlagEnumerator> flags(Class<? extends QtFlagEnumerator> cls, int value);
    
    @QtUninvokable
    public QtAbstractEnumerator resolve(int value) {
    	Class<?> type = type();
		if(type.isEnum()) {
			byte bitSize = 4;
			if(QtByteEnumerator.class.isAssignableFrom(type)) {
				bitSize = 1;
			}else if(QtShortEnumerator.class.isAssignableFrom(type)) {
				bitSize = 2;
			}else if(QtLongEnumerator.class.isAssignableFrom(type)) {
				bitSize = 8;
			}
			return resolveEntry(type, value, bitSize);
		}else {
			return enclosingMetaObject.getEnumEntry(enumeratorIndex, value);
		}
    }
    
	private static native QtAbstractEnumerator resolveEntry(Class<?> cls, int value, byte bitSize);
	
    @QtUninvokable
    private static native List<String> valueToKeys(long metaObjectPointer, int enumeratorIndex, int value);

    @QtUninvokable
    public QMetaObject enclosingMetaObject() { 
    	return enclosingMetaObject; 
	}

    @Override
    @QtUninvokable
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + enclosingMetaObject.hashCode();
		result = prime * result + enumeratorIndex;
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
		QMetaEnum other = (QMetaEnum) obj;
		if (!enclosingMetaObject.equals(other.enclosingMetaObject))
			return false;
		if (enumeratorIndex!=other.enumeratorIndex)
			return false;
		return true;
	}
	
	@Override
    @QtUninvokable
	public String toString() {
		return name();
	}
}
