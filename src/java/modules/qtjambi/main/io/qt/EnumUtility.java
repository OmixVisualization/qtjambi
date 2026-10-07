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

import java.lang.reflect.*;
import java.util.*;
import java.util.function.*;

/**
 * @hidden
 */
final class EnumUtility {
	static {
		QtJambi_LibraryUtilities.initialize();
	}

	private EnumUtility() {throw new RuntimeException();}
	
	private static final Map<Class<?>, Supplier<?>> flagsConstructorsByEnumType = Collections.synchronizedMap(new HashMap<>());
	private static final Map<Class<? extends QtAbstractEnumerator>, Object[]> enumConstants = Collections.synchronizedMap(new HashMap<>());
	private native static Class<?> longFlagsClass();
	private static final Class<?> longFlagsClass = longFlagsClass();
	
	static QFlags<?> asFlags(QtAbstractFlagEnumerator flag) {
		Supplier<?> flagsConstructor = flagsConstructorsByEnumType.computeIfAbsent(QtJambi_LibraryUtilities.internal.getClass(flag), cls -> {
			Supplier<?> constructor = null;
			Class<?> declClass = cls.getDeclaringClass();
			Class<?> flagClass = null;
			if (declClass != null) {
				for (Class<?> _flagClass : declClass.getDeclaredClasses()) {
					if(QFlags.class.isAssignableFrom(_flagClass)) {
						if (_flagClass.getGenericSuperclass() instanceof ParameterizedType) {
							ParameterizedType p = (ParameterizedType) _flagClass.getGenericSuperclass();
							Type[] args = p.getActualTypeArguments();
							if (args.length == 1 && args[0] == cls) {
								flagClass = _flagClass;
							}
						}
					}
					if(flagClass!=null)
						break;
				}
				if(flagClass==null) {
					Class<?> flagsSuperClass;
					if(longFlagsClass!=null && QtLongFlagEnumerator.class.isAssignableFrom(cls))
						flagsSuperClass = longFlagsClass;
					else
						flagsSuperClass = QFlags.class;
					if(flagsSuperClass!=null) {
						for(Class<?> _flagClass : flagsSuperClass.getDeclaredClasses()) {
							if(flagsSuperClass.isAssignableFrom(_flagClass)) {
								flagClass = _flagClass;
								break;
							}
						}
					}
				}
				if(flagClass!=null) {
					Constructor<?> defaultConstructor = null;
					Constructor<?> varargsConstructor = null;
					Constructor<?> intConstructor = null;
					Constructor<?> longConstructor = null;
					for(Constructor<?> cstr : flagClass.getConstructors()) {
						switch(cstr.getParameterCount()) {
						case 0: 
							defaultConstructor = cstr;
							break;
						case 1: {
							Parameter parameter = cstr.getParameters()[0];
							if(parameter.getType()==int.class) {
								intConstructor = cstr;
							}else if(parameter.getType()==long.class) {
								if(longFlagsClass!=null && longFlagsClass.isAssignableFrom(flagClass)) {
									longConstructor = cstr;
								}
							}else if(parameter.isVarArgs()){
								varargsConstructor = cstr;
							}
							break;
						}
						default: break;
						}
					}
					if(defaultConstructor==null && varargsConstructor==null && intConstructor==null && longConstructor==null) {
						for(Constructor<?> cstr : flagClass.getDeclaredConstructors()) {
							switch(cstr.getParameterCount()) {
							case 0:
								defaultConstructor = cstr;
								break;
							case 1: {
								Parameter parameter = cstr.getParameters()[0];
								if(parameter.getType()==int.class) {
									intConstructor = cstr;
								}else if(parameter.getType()==long.class) {
									if(longFlagsClass!=null && longFlagsClass.isAssignableFrom(flagClass)) {
										longConstructor = cstr;
									}
								}else if(parameter.isVarArgs()){
									varargsConstructor = cstr;
								}
								break;
							}
							default: break;
							}
						}
					}
					if(defaultConstructor!=null) {
						try {
							constructor = QtJambi_LibraryUtilities.internal.getFactory0(defaultConstructor);
						} catch (Throwable e) {
						}
					}else if(varargsConstructor!=null) {
						Class<?> componentType = varargsConstructor.getParameterTypes()[0].getComponentType();
						if(componentType!=null) {
							try {
								Object emptyArray = Array.newInstance(componentType, 0);
								Function<Object,?> fn = QtJambi_LibraryUtilities.internal.getFactory1(varargsConstructor);
								constructor = () -> fn.apply(emptyArray);
							} catch (Throwable e) {
							}						
						}
					}else if(intConstructor!=null) {
						try {
							Function<Integer,?> fn = QtJambi_LibraryUtilities.internal.getFactory1(intConstructor);
							constructor = () -> fn.apply(0);
						} catch (Throwable e) {
						}						
					}else if(longConstructor!=null) {
						try {
							Function<Long,?> fn = QtJambi_LibraryUtilities.internal.getFactory1(longConstructor);
							constructor = () -> fn.apply(0l);
						} catch (Throwable e) {
						}
					}
				}
			}
			return constructor;
		});
		if(flagsConstructor!=null) {
			try {
				QFlags<?> flags = (QFlags<?>)flagsConstructor.get();
				if(flags.isLong()) {
					if (flag instanceof QtLongFlagEnumerator) {
						flags.setValue(((QtLongFlagEnumerator) flag).value());
					}else if (flag instanceof QtFlagEnumerator) {
						flags.setValue((long)((QtFlagEnumerator) flag).value());
					}else if (flag instanceof QtByteFlagEnumerator) {
						flags.setValue((long)((QtByteFlagEnumerator) flag).value());
					}else if (flag instanceof QtShortFlagEnumerator) {
						flags.setValue((long)((QtShortFlagEnumerator) flag).value());
					}
				}else{
					if (flag instanceof QtFlagEnumerator) {
						flags.setValue(((QtFlagEnumerator) flag).value());
					}else if (flag instanceof QtLongFlagEnumerator) {
						flags.setValue((int)((QtLongFlagEnumerator) flag).value());
					}else if (flag instanceof QtByteFlagEnumerator) {
						flags.setValue((int)((QtByteFlagEnumerator) flag).value());
					}else if (flag instanceof QtShortFlagEnumerator) {
						flags.setValue((int)((QtShortFlagEnumerator) flag).value());
					}
				}
				return flags;
			} catch (Throwable e) {
			}
		}
		return null;
	}
	
	private static Object[] getEnumConstants(Class<? extends QtAbstractEnumerator> cls) {
		Object[] result = cls.getEnumConstants();
		if(result==null)
			result = (Object[])Array.newInstance(cls, 0);
		return result;
	}
	
	static Object[] enumConstants(Class<? extends QtAbstractEnumerator> cls) {
		Object[] constants;
		if (cls.isAnnotationPresent(QtExtensibleEnum.class)) {
			constants = getEnumConstants(cls);
		} else {
			constants = enumConstants.computeIfAbsent(cls, EnumUtility::getEnumConstants);
		}
		return constants;
	}
	
	static boolean isSmallEnum(QtAbstractEnumerator enm) {
		Class<? extends QtAbstractEnumerator> cls = enm.getDeclaringClass();
		return cls!=null && enumConstants(cls).length <= 33;
	}

	native static <E extends Enum<E> & QtEnumerator> E resolveIntEnum(int hashCode, Class<E> cl, int value, String name) throws Throwable;
	native static <E extends Enum<E> & QtByteEnumerator> E resolveByteEnum(int hashCode, Class<E> cl, byte value, String name) throws Throwable;
	native static <E extends Enum<E> & QtShortEnumerator> E resolveShortEnum(int hashCode, Class<E> cl, short value, String name) throws Throwable;
	native static <E extends Enum<E> & QtLongEnumerator> E resolveLongEnum(int hashCode, Class<E> cl, long value, String name) throws Throwable;
}
