/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.internal;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.NativeAccess;
import io.qt.QFlags;
import io.qt.QtAbstractFlagEnumerator;
import io.qt.QtByteFlagEnumerator;
import io.qt.QtExtensibleEnum;
import io.qt.QtFlagEnumerator;
import io.qt.QtLongFlagEnumerator;
import io.qt.QtShortFlagEnumerator;

public final class QtJambiEnums {
	private QtJambiEnums() {throw new RuntimeException();}
	
	private static final Map<Class<? extends QtAbstractFlagEnumerator>, Object> flagsConstructorsByEnumType;
	private static final Map<Class<? extends QtAbstractFlagEnumerator>, QtAbstractFlagEnumerator[]> flagsConstants;
	static {
		flagsConstructorsByEnumType = Collections.synchronizedMap(new HashMap<>());
		flagsConstants = Collections.synchronizedMap(new HashMap<>());
	}
	
	@SuppressWarnings("unchecked")
	public static QFlags<?> asFlags(QtAbstractFlagEnumerator flag, Function<Integer, QFlags<?>> constructor) {
		Object flagsConstructor = flagsConstructorsByEnumType.computeIfAbsent(QtJambiInternal.getClass(flag), cls -> {
			Class<?> declClass = cls.getDeclaringClass();
			if (declClass != null) {
				for (Class<?> flagClass : declClass.getDeclaredClasses()) {
					if (flagClass.getSuperclass() == QFlags.class) {
						if (flagClass.getGenericSuperclass() instanceof ParameterizedType) {
							ParameterizedType p = (ParameterizedType) flagClass.getGenericSuperclass();
							Type[] args = p.getActualTypeArguments();
							if (args.length == 1 && args[0] == cls) {
								Constructor<?> constr = null;
								try {
									constr = flagClass.getDeclaredConstructor(int.class);
								} catch (Throwable e) {
								}
								if(constr==null) try {
									constr = flagClass.getDeclaredConstructor();
								} catch (Throwable e) {
								}
								if(constr==null) try {
									constr = flagClass.getConstructor();
								} catch (Throwable e) {
								}
								if(constr!=null) {
									if(constr.getParameterCount()==0) {
										return QtJambiInternal.getFactory0(constr);
									}else {
										return QtJambiInternal.getFactory1(constr);
									}
								}
							}
						}
					}
				}
			}
			return null;
		});
		int value = 0;
		if (flag instanceof QtFlagEnumerator) {
			value |= ((QtFlagEnumerator) flag).value();
		} else if (flag instanceof QtByteFlagEnumerator) {
			value |= ((QtByteFlagEnumerator) flag).value();
		} else if (flag instanceof QtShortFlagEnumerator) {
			value |= ((QtShortFlagEnumerator) flag).value();
		} else if (flag instanceof QtLongFlagEnumerator) {
			value |= ((QtLongFlagEnumerator) flag).value();
		}
		if (flagsConstructor != null) {
			if (flagsConstructor instanceof Function) {
				try {
					return ((Function<Integer,QFlags<?>>)flagsConstructor).apply(value);
				} catch (Throwable e) {
				}
			} else if (flagsConstructor instanceof Supplier) {
				try {
					QFlags<?> flags = ((Supplier<QFlags<?>>) flagsConstructor).get();
					flags.setValue(flags.value() | value);
					return flags;
				} catch (Throwable e) {
				}
			}
		}
		return constructor.apply(value);
	}
	
	public static boolean isSmallEnum(QtAbstractFlagEnumerator enm) {
		try {
			QtAbstractFlagEnumerator[] values;
			Class<? extends QtAbstractFlagEnumerator> cls = QtJambiInternal.getClass(enm);
			if (cls.isAnnotationPresent(QtExtensibleEnum.class)) {
				values = cls.getEnumConstants();
			} else {
				values = flagsConstants.computeIfAbsent(cls, _cls -> {
					return _cls.getEnumConstants();
				});
			}
			return values != null && values.length <= 33;
		} catch (Exception e) {
		}
		return false;
	}

	@SuppressWarnings("unchecked")
	public static <T extends QtAbstractFlagEnumerator> T[] flagConstants(QFlags<T> flags) {
		if (flags.getClass().getGenericSuperclass() instanceof ParameterizedType) {
			ParameterizedType superType = (ParameterizedType) flags.getClass().getGenericSuperclass();
			if (superType.getRawType() == QFlags.class) {
				Type[] typeArguments = superType.getActualTypeArguments();
				if (typeArguments.length == 1 && typeArguments[0] instanceof Class) {
					Class<T> enumFlagType = (Class<T>) typeArguments[0];
					T[] values;
					if (enumFlagType.isAnnotationPresent(QtExtensibleEnum.class)) {
						values = enumFlagType.getEnumConstants();
					} else {
						values = (T[]) flagsConstants.computeIfAbsent(enumFlagType, cls -> {
							return cls.getEnumConstants();
						});
					}
					return values;
				}
			}
		}
		return (T[]) new QtFlagEnumerator[0];
	}
	
	@NativeAccess
	private static <T extends Enum<T>> boolean extendEnum(Class<T> enumClass, T[] array, T enumEntry) {
		try {
			Field enumConstantsField = Class.class.getDeclaredField("enumConstants");
			Field enumConstantDirectoryField = Class.class.getDeclaredField("enumConstantDirectory");
			QtJambiInternal.setField(enumClass, enumConstantsField, array);
			@SuppressWarnings("unchecked")
			Map<String, T> directory = (Map<String, T>)QtJambiInternal.fetchField(enumClass, enumConstantDirectoryField);
			if (directory != null) {
				directory.put(enumEntry.name(), enumEntry);
			}
			return true;
		} catch (Throwable e) {
			try {
				Field enumVarsField = Class.class.getDeclaredField("enumVars");
				Object enumVars = QtJambiInternal.fetchField(enumClass, enumVarsField);
				if (enumVars == null) {
					Constructor<?> enumVarsConstr = enumVarsField.getType().getDeclaredConstructor();
					enumVars = QtJambiInternal.invokeContructor(enumVarsConstr);
					QtJambiInternal.setField(enumClass, enumVarsField, enumVars);
				}
				Field enumConstantsField = enumVarsField.getType().getDeclaredField("cachedEnumConstants");
				Field enumConstantDirectoryField = enumVarsField.getType().getDeclaredField("cachedEnumConstantDirectory");
				QtJambiInternal.setField(enumVars, enumConstantsField, array);
				@SuppressWarnings("unchecked")
				Map<String, T> directory = (Map<String, T>) QtJambiInternal.fetchField(enumVars, enumConstantDirectoryField);
				if (directory != null) {
					directory.put(enumEntry.name(), enumEntry);
				}
				return true;
			} catch (Throwable e1) {
				if(NativeLibraryManager.operatingSystem!=NativeLibraryManager.OperatingSystem.Android) {
					e.addSuppressed(e1);
					e.printStackTrace();
				}
			}
		}
		return false;
	}
}
