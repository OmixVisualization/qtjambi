/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt;

import java.util.AbstractMap;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

/**
 * <p>QtArgument is used to initialize an interface type or a set of inherited interface types with their 
 * non-standard constructors.</p>
 * 
 * <p>Example:</p>
 * 
 * <code>
 * private static class SurfaceObject extends QObject implements QSurface{<br>
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;// parameter parent is for QObject constructor<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;// parameter type is for QSurface constructor<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;public SurfaceObject(QObject parent, SurfaceClass type){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;super((QPrivateConstructor)null);<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// we need to use the private constructor and initialize the native portion of the object otherwise:<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;QtUtilities.initializeNativeObject(this,<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;QtArgument.begin(QObject.class).add(parent)<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;.begin(QSurface.class).add(type)<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;);<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
 * }
 * </code>
 * 
 * @see QtUtilities#initializeNativeObject(QtObjectInterface, Stream)
 */
public final class QtArgument {
	private QtArgument(){}
	public static Stream begin(Class<?> type) {
		return new Stream(type);
	}
	
	/**
	 * Argument stream.
	 */
	public final static class Stream{
		private final Map<Class<?>,List<Map.Entry<Object,Object>>> arguments = new HashMap<>();
		private List<Map.Entry<Object,Object>> currentList;
		
		Stream(Class<?> type) {
			beginImpl(type);
		}
		
		public Stream add(Object value) {
			currentList.add(newArg(QtJambi_LibraryUtilities.internal.getClass(value), value, true));
			return this;
		}
		public Stream add(int value) {
			currentList.add(newArg(int.class, value, false));
			return this;
		}
		public Stream add(long value) {
			currentList.add(newArg(long.class, value, false));
			return this;
		}
		public Stream add(short value) {
			currentList.add(newArg(short.class, value, false));
			return this;
		}
		public Stream add(byte value) {
			currentList.add(newArg(byte.class, value, false));
			return this;
		}
		public Stream add(double value) {
			currentList.add(newArg(double.class, value, false));
			return this;
		}
		public Stream add(float value) {
			currentList.add(newArg(float.class, value, false));
			return this;
		}
		public Stream add(char value) {
			currentList.add(newArg(char.class, value, false));
			return this;
		}
		public Stream add(boolean value) {
			currentList.add(newArg(boolean.class, value, false));
			return this;
		}
		public <T> Stream add(@StrictNonNull Class<? super T> type, T value) {
			currentList.add(newArg(type, value, true));
			return this;
		}
		void addIndexes(int index, int... indexes) {
			currentList.add(new AbstractMap.SimpleImmutableEntry<>(null, index));
			if(indexes!=null && indexes.length>0)
				currentList.add(new AbstractMap.SimpleImmutableEntry<>(null, indexes));
		}
		public Stream begin(Class<?> type) {
			if(arguments.containsKey(type))
				throw new IllegalArgumentException("Type "+type+" has already been used.");
			beginImpl(type);
			return this;
		}
		private void beginImpl(Class<?> type) {
			currentList = arguments.computeIfAbsent(type, Utility.arrayListFactory());
		}
		Map<Class<?>, List<Map.Entry<Object,Object>>> arguments() {
			return Collections.unmodifiableMap(arguments);
		}
		private static Map.Entry<Object,Object> newArg(Class<?> type, Object value, boolean check) {
			if(java.util.Collection.class.isAssignableFrom(type)){
                type = java.util.Collection.class;
            }else if(java.util.Map.class.isAssignableFrom(type)){
                type = java.util.Map.class;
            }else {
            	Class<?> generatedSuperclass = QtJambi_LibraryUtilities.internal.findGeneratedSuperclass(type);
            	if(generatedSuperclass!=null)
            		type = generatedSuperclass;
            }
			if(check) {
				value = type.cast(value);
			}else {
				Objects.requireNonNull(type);
			}
			return new AbstractMap.SimpleImmutableEntry<>(type, value);
		}
	}
}
