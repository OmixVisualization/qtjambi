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
package io.qt;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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
		private final Map<Class<?>,List<Arg>> arguments;
		private List<Arg> currentList;
		
		private Stream(Class<?> type) {
			arguments = new HashMap<>();
			currentList = arguments.computeIfAbsent(type, key->new ArrayList<>());
		}
		
		public Stream add(Object value) {
			currentList.add(new Arg(QtJambi_LibraryUtilities.internal.getClass(value), value, true));
			return this;
		}
		public Stream add(int value) {
			currentList.add(new Arg(int.class, value, false));
			return this;
		}
		public Stream add(long value) {
			currentList.add(new Arg(long.class, value, false));
			return this;
		}
		public Stream add(short value) {
			currentList.add(new Arg(short.class, value, false));
			return this;
		}
		public Stream add(byte value) {
			currentList.add(new Arg(byte.class, value, false));
			return this;
		}
		public Stream add(double value) {
			currentList.add(new Arg(double.class, value, false));
			return this;
		}
		public Stream add(float value) {
			currentList.add(new Arg(float.class, value, false));
			return this;
		}
		public Stream add(char value) {
			currentList.add(new Arg(char.class, value, false));
			return this;
		}
		public Stream add(boolean value) {
			currentList.add(new Arg(boolean.class, value, false));
			return this;
		}
		public <T> Stream add(Class<? super T> type, T value) {
			currentList.add(new Arg(type, value, true));
			return this;
		}
		public Stream begin(Class<?> type) {
			if(arguments.containsKey(type))
				throw new IllegalArgumentException("Type "+type+" has already been used.");
			currentList = arguments.computeIfAbsent(type, key->new ArrayList<>());
			return this;
		}
		Map<Class<?>, List<?>> arguments() {
			return Collections.unmodifiableMap(arguments);
		}
		
		private static class Arg{
			private Arg(Class<?> type, Object value, boolean check) {
				if(java.util.Collection.class.isAssignableFrom(type)){
	                type = java.util.Collection.class;
	            }else if(java.util.Map.class.isAssignableFrom(type)){
	                type = java.util.Map.class;
	            }else {
	            	Class<?> generatedSuperclass = QtJambi_LibraryUtilities.internal.findGeneratedSuperclass(type);
	            	if(generatedSuperclass!=null)
	            		type = generatedSuperclass;
	            }
				this.type = type;
				if(check) {
					this.value = this.type.cast(value);
				}else {
					this.value = value;
				}
			}
			@NativeAccess
			private final Class<?> type;
			@NativeAccess
			private final Object value;
		}
	}
}
