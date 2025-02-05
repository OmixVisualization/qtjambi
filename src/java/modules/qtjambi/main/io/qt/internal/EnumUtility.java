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
package io.qt.internal;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.NativeAccess;
import io.qt.QFlags;
import io.qt.QNoSuchEnumValueException;
import io.qt.QtAbstractFlagEnumerator;
import io.qt.QtByteEnumerator;
import io.qt.QtByteFlagEnumerator;
import io.qt.QtEnumerator;
import io.qt.QtExtensibleEnum;
import io.qt.QtFlagEnumerator;
import io.qt.QtLongEnumerator;
import io.qt.QtLongFlagEnumerator;
import io.qt.QtShortEnumerator;
import io.qt.QtShortFlagEnumerator;

/**
 * @hidden
 */
final class EnumUtility {
	static {
		QtJambi_LibraryUtilities.initialize();
	}

	private EnumUtility() {throw new RuntimeException();}
	
	private static class FlagsConstructors{
		Supplier<QFlags<?>> defaultConstructor;
		Function<Integer,QFlags<?>> intConstructor;
		Function<Long,QFlags<?>> longConstructor;
		SignalUtility.SlotInvoker setIntValue;
		SignalUtility.SlotInvoker setLongValue;
	}
	private static final boolean hasLongFlags = NativeUtility.majorVersion()>=7 
			|| (NativeUtility.majorVersion()==6 && NativeUtility.minorVersion()>=9);
	private static final Map<Class<? extends QtAbstractFlagEnumerator>, FlagsConstructors> flagsConstructorsByEnumType;
	private static final Map<Class<? extends QtAbstractFlagEnumerator>, QtAbstractFlagEnumerator[]> flagsConstants;
	private static final Class<?> longFlagsClass;
	static {
		Class<?> longFlags = null;
		if(hasLongFlags) {
			try {
				longFlags = Class.forName("io.qt.QLongFlags");
			} catch (Throwable e) {
			}
		}
		longFlagsClass = longFlags;
		flagsConstructorsByEnumType = Collections.synchronizedMap(new HashMap<>());
		flagsConstants = Collections.synchronizedMap(new HashMap<>());
	}
	
	@SuppressWarnings("unchecked")
	static QFlags<?> asFlags(QtAbstractFlagEnumerator flag) {
		@SuppressWarnings("rawtypes")
		FlagsConstructors flagsConstructor = flagsConstructorsByEnumType.computeIfAbsent(AccessUtility.instance.getClass(flag), cls -> {
			FlagsConstructors flagsConstructors = new FlagsConstructors();
			Class<?> declClass = cls.getDeclaringClass();
			Class<?> flagClass = null;
			if (declClass != null) {
				Class<?> flagsSuperClass;
				if(hasLongFlags && QtLongFlagEnumerator.class.isAssignableFrom(cls))
					flagsSuperClass = longFlagsClass;
				else
					flagsSuperClass = QFlags.class;
				for (Class<?> _flagClass : declClass.getDeclaredClasses()) {
					if (_flagClass.getSuperclass() == flagsSuperClass) {
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
					for(Class<?> _flagClass : flagsSuperClass.getDeclaredClasses()) {
						if(flagsSuperClass.isAssignableFrom(_flagClass)) {
							flagClass = _flagClass;
							break;
						}
					}
				}
				if(flagClass!=null) {
					if(hasLongFlags && QtLongFlagEnumerator.class.isAssignableFrom(cls)) {
						try {
							flagsConstructors.longConstructor = (Function)ReflectionUtility.getFactory1(flagClass.getDeclaredConstructor(long.class));
						} catch (Throwable e) {
						}
					}
					if(flagsConstructors.longConstructor==null) try {
						flagsConstructors.intConstructor = (Function)ReflectionUtility.getFactory1(flagClass.getDeclaredConstructor(int.class));
					} catch (Throwable e) {
					}
					if(flagsConstructors.longConstructor==null && flagsConstructors.intConstructor==null){
						try {
							flagsConstructors.defaultConstructor = (Supplier)ReflectionUtility.getFactory0(flagClass.getConstructor());
						} catch (Throwable e) {
						}
						if(flagsConstructors.defaultConstructor==null) {
							try {
								flagsConstructors.defaultConstructor = (Supplier)ReflectionUtility.getFactory0(flagClass.getDeclaredConstructor());
							} catch (Throwable e) {
							}
						}
						if(flagsConstructors.defaultConstructor!=null) {
							if(hasLongFlags && QtLongFlagEnumerator.class.isAssignableFrom(cls)) {
								try {
									flagsConstructors.setLongValue = ReflectionUtility.getSlotInvoker(flagClass.getDeclaredMethod("setValue", long.class), null);
								} catch (Throwable e) {
								}
								if(flagsConstructors.setLongValue==null) {
									try {
										flagsConstructors.setLongValue = ReflectionUtility.getSlotInvoker(flagsSuperClass.getDeclaredMethod("setLongValue", long.class), null);
									} catch (Throwable e) {
									}
								}
							}
							if(flagsConstructors.setLongValue==null) {
								try {
									flagsConstructors.setIntValue = ReflectionUtility.getSlotInvoker(flagClass.getDeclaredMethod("setValue", int.class), null);
								} catch (Throwable e) {
								}
								if(flagsConstructors.setIntValue==null) {
									try {
										flagsConstructors.setIntValue = ReflectionUtility.getSlotInvoker(flagsSuperClass.getDeclaredMethod("setIntValue", int.class), null);
									} catch (Throwable e) {
									}
								}
							}
						}
					}
				}
			}
			return flagsConstructors;
		});
		if(hasLongFlags && flagsConstructor.longConstructor!=null) {
			if (flag instanceof QtLongFlagEnumerator) {
				try {
					return flagsConstructor.longConstructor.apply(((QtLongFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}else if (flag instanceof QtFlagEnumerator) {
				try {
					return flagsConstructor.longConstructor.apply((long)((QtFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}else if (flag instanceof QtByteFlagEnumerator) {
				try {
					return flagsConstructor.longConstructor.apply((long)((QtByteFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}else if (flag instanceof QtShortFlagEnumerator) {
				try {
					return flagsConstructor.longConstructor.apply((long)((QtShortFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}
			return null;
		}else if(flagsConstructor.intConstructor!=null) {
			if (flag instanceof QtFlagEnumerator) {
				try {
					return flagsConstructor.intConstructor.apply(((QtFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}else if (flag instanceof QtLongFlagEnumerator) {
				try {
					return flagsConstructor.intConstructor.apply((int)((QtLongFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}else if (flag instanceof QtByteFlagEnumerator) {
				try {
					return flagsConstructor.intConstructor.apply((int)((QtByteFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}else if (flag instanceof QtShortFlagEnumerator) {
				try {
					return flagsConstructor.intConstructor.apply((int)((QtShortFlagEnumerator) flag).value());
				} catch (Throwable e) {
				}
			}
			return null;
		}else if(flagsConstructor.defaultConstructor!=null) {
			try {
				QFlags<?> flags = flagsConstructor.defaultConstructor.get();
				if(hasLongFlags && flagsConstructor.setLongValue!=null) {
					if (flag instanceof QtLongFlagEnumerator) {
						try {
							flagsConstructor.setLongValue.invokeArguments(flags, ((QtLongFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}else if (flag instanceof QtFlagEnumerator) {
						try {
							flagsConstructor.setLongValue.invokeArguments(flags, (long)((QtFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}else if (flag instanceof QtByteFlagEnumerator) {
						try {
							flagsConstructor.setLongValue.invokeArguments(flags, (long)((QtByteFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}else if (flag instanceof QtShortFlagEnumerator) {
						try {
							flagsConstructor.setLongValue.invokeArguments(flags, (long)((QtShortFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}
				}else if(flagsConstructor.setIntValue!=null) {
					if (flag instanceof QtFlagEnumerator) {
						try {
							flagsConstructor.setIntValue.invokeArguments(flags, ((QtFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}else if (flag instanceof QtLongFlagEnumerator) {
						try {
							flagsConstructor.setIntValue.invokeArguments(flags, (int)((QtLongFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}else if (flag instanceof QtByteFlagEnumerator) {
						try {
							flagsConstructor.setIntValue.invokeArguments(flags, (int)((QtByteFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}else if (flag instanceof QtShortFlagEnumerator) {
						try {
							flagsConstructor.setIntValue.invokeArguments(flags, (int)((QtShortFlagEnumerator) flag).value());
						} catch (Throwable e) {
						}
					}
				}
				return flags;
			} catch (Throwable e) {
			}
		}
		return null;
	}
	
	static boolean isSmallEnum(QtAbstractFlagEnumerator enm) {
		try {
			QtAbstractFlagEnumerator[] values;
			Class<? extends QtAbstractFlagEnumerator> cls = AccessUtility.instance.getClass(enm);
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
	static <T extends QtAbstractFlagEnumerator> T[] flagConstants(QFlags<T> flags) {
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
    static Class<?> getEnumForQFlags(Class<?> flagsType) {
        Type t = flagsType.getGenericSuperclass();
        if (t instanceof ParameterizedType) {
            Type typeArguments[] = ((ParameterizedType)t).getActualTypeArguments();
            return ((Class<?>) typeArguments[0]);
        }
        return null;
    }

	private native static <E extends Enum<E> & QtEnumerator> E resolveIntEnum(int hashCode, Class<E> cl, int value, String name) throws Throwable;
	private native static <E extends Enum<E> & QtByteEnumerator> E resolveByteEnum(int hashCode, Class<E> cl, byte value, String name) throws Throwable;
	private native static <E extends Enum<E> & QtShortEnumerator> E resolveShortEnum(int hashCode, Class<E> cl, short value, String name) throws Throwable;
	private native static <E extends Enum<E> & QtLongEnumerator> E resolveLongEnum(int hashCode, Class<E> cl, long value, String name) throws Throwable;
	
	static <E extends Enum<E> & QtEnumerator> E resolveEnum(Class<E> cl, int value, String name) {
		if (name != null) {
			if (name.isEmpty())
				name = null;
			else {
				E enm = null;
				try {
					enm = Enum.valueOf(cl, name);
				} catch (Exception e) {
				}
				if (enm != null) {
					if (enm.value() == value) {
						return enm;
					} else {
						throw new io.qt.QNoSuchEnumValueException(value, name);
					}
				}
			}
		}
		try {
			E enm = EnumUtility.resolveIntEnum(cl.hashCode(), cl, value, name);
			if (enm == null) {
				if (name == null)
					throw new QNoSuchEnumValueException(value);
				else
					throw new QNoSuchEnumValueException(value, name);
			}
			return enm;
		} catch (QNoSuchEnumValueException e) {
			throw e;
		} catch (Throwable e) {
			throw new QNoSuchEnumValueException(value, e);
		}
	}

	static <E extends Enum<E> & QtByteEnumerator> E resolveEnum(Class<E> cl, byte value, String name) {
		if (name != null) {
			if (name.isEmpty())
				name = null;
			else {
				E enm = null;
				try {
					enm = Enum.valueOf(cl, name);
				} catch (Exception e) {
				}
				if (enm != null) {
					if (enm.value() == value) {
						return enm;
					} else {
						throw new io.qt.QNoSuchEnumValueException(value, name);
					}
				}
			}
		}
		try {
			E enm = EnumUtility.resolveByteEnum(cl.hashCode(), cl, value, name);
			if (enm == null) {
				if (name == null)
					throw new QNoSuchEnumValueException(value);
				else
					throw new QNoSuchEnumValueException(value, name);
			}
			return enm;
		} catch (QNoSuchEnumValueException e) {
			throw e;
		} catch (Throwable e) {
			throw new QNoSuchEnumValueException(value, e);
		}
	}

	static <E extends Enum<E> & QtShortEnumerator> E resolveEnum(Class<E> cl, short value, String name) {
		if (name != null) {
			if (name.isEmpty())
				name = null;
			else {
				E enm = null;
				try {
					enm = Enum.valueOf(cl, name);
				} catch (Exception e) {
				}
				if (enm != null) {
					if (enm.value() == value) {
						return enm;
					} else {
						throw new io.qt.QNoSuchEnumValueException(value, name);
					}
				}
			}
		}
		try {
			E enm = EnumUtility.resolveShortEnum(cl.hashCode(), cl, value, name);
			if (enm == null) {
				if (name == null)
					throw new QNoSuchEnumValueException(value);
				else
					throw new QNoSuchEnumValueException(value, name);
			}
			return enm;
		} catch (QNoSuchEnumValueException e) {
			throw e;
		} catch (Throwable e) {
			throw new QNoSuchEnumValueException(value, e);
		}
	}

	static <E extends Enum<E> & QtLongEnumerator> E resolveEnum(Class<E> cl, long value, String name) {
		if (name != null) {
			if (name.isEmpty())
				name = null;
			else {
				E enm = null;
				try {
					enm = Enum.valueOf(cl, name);
				} catch (Exception e) {
				}
				if (enm != null) {
					if (enm.value() == value) {
						return enm;
					} else {
						throw new io.qt.QNoSuchEnumValueException(value, name);
					}
				}
			}
		}
		try {
			E enm = EnumUtility.resolveLongEnum(cl.hashCode(), cl, value, name);
			if (enm == null) {
				if (name == null)
					throw new QNoSuchEnumValueException(value);
				else
					throw new QNoSuchEnumValueException(value, name);
			}
			return enm;
		} catch (QNoSuchEnumValueException e) {
			throw e;
		} catch (Throwable e) {
			throw new QNoSuchEnumValueException(value, e);
		}
	}
}
