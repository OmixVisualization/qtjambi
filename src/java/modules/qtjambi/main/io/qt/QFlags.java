/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * <p>
 * The QFlags class provides a type-safe way of storing OR-combinations of enum values.
 * </p>
 * <p>
 * Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qflags.html">QFlags</a>
 * </p>
 * 
 */
@SuppressWarnings("serial")
public abstract class QFlags<T extends QtAbstractFlagEnumerator> implements java.io.Serializable, Cloneable {
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#QFlags">QFlags::QFlags()</a>
	 */
	protected QFlags() {
		this(0);
	}
	
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#QFlags-2">QFlags::QFlags(QFlag)</a>
	 */
	protected QFlags(int value) {
		this.value = value;
	}
	
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#QFlags-3">QFlags::QFlags(std::initializer_list&lt;Enum>)</a>
	 */
	@SafeVarargs
	protected QFlags(@Nullable T @NonNull... args) {
		this(0);
		set(args);
	}
	
	/**
	 * Clones the flags
	 */
	@SuppressWarnings("unchecked")
	public @Nullable QFlags<T> clone() {
		try {
			return (QFlags<T>) super.clone();
		} catch (CloneNotSupportedException e) {
			return null;
		}
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-7c">QFlags::operator|(Enum) const</a>
	 */
	public @StrictNonNull QFlags<T> combined(@Nullable T flag) {
		return clone().setFlag(flag, true);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-and-eq-3">QFlags::operator&amp;=(Enum)</a>
	 * and <a href="https://doc.qt.io/qt/qflags.html#operator-7e">QFlags::operator~()</a>
	 */
	public @StrictNonNull QFlags<T> cleared(@Nullable T flag) {
		return clone().setFlag(flag, false);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-7c-1">QFlags::operator|=(QFlags&lt;T>)</a>
	 */
	public final void set(@StrictNonNull QFlags<T> flag) {
		setImpl(flag);
	}
	
	/**
	 * @hidden
	 */
	void setImpl(@StrictNonNull QFlags<T> flag) {
		int value = this.value;
		if(flag.getClass()==getClass())
			value |= flag.value;
		this.setIntValue(value);
	}
	
	/**
	 * <p>
	 * Overloaded constructor for
	 * {@link #setFlag(QtAbstractFlagEnumerator, boolean)} with
	 * <code>on = true</code>.
	 * </p>
	 */
	public @NonNull QFlags<T> setFlag(@Nullable T flag) {
		return setFlag(flag, true);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#setFlag">QFlags::setFlag(Enum, bool)</a>
	 */
	public @NonNull QFlags<T> setFlag(@Nullable T flag, boolean on) {
		setFlagImpl(flag, on);
		return this;
	}
	
	void setFlagImpl(@Nullable T flag, boolean on) {
		int value = this.value;
		if (on) {
			if (flag instanceof QtFlagEnumerator) {
				value |= ((QtFlagEnumerator) flag).value();
			} else if (flag instanceof QtByteFlagEnumerator) {
				value |= ((QtByteFlagEnumerator) flag).value();
			} else if (flag instanceof QtShortFlagEnumerator) {
				value |= ((QtShortFlagEnumerator) flag).value();
			} else if (flag instanceof QtLongFlagEnumerator) {
				value |= ((QtLongFlagEnumerator) flag).value();
			}
		} else {
			if (flag instanceof QtFlagEnumerator) {
				value &= ~((QtFlagEnumerator) flag).value();
			} else if (flag instanceof QtByteFlagEnumerator) {
				value &= ~((QtByteFlagEnumerator) flag).value();
			} else if (flag instanceof QtShortFlagEnumerator) {
				value &= ~((QtShortFlagEnumerator) flag).value();
			} else if (flag instanceof QtLongFlagEnumerator) {
				value &= ~((QtLongFlagEnumerator) flag).value();
			}
		}
		this.setIntValue(value);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-7c-eq-1">QFlags::operator|=(Enum)</a>
	 */
	@SafeVarargs
	public final void set(@Nullable T @NonNull... ts) {
		for (T flag : ts) {
			setFlag(flag, true);
		}
	}
	
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-and-eq-3">QFlags::operator&amp;=(Enum)</a>
	 * and <a href="https://doc.qt.io/qt/qflags.html#operator-7e">QFlags::operator~()</a>
	 */
	@SafeVarargs
	public final void exclude(@Nullable T @NonNull... ts) {
		for (T flag : ts) {
			setFlag(flag, false);
		}
	}

	/**
	 * @deprecated Use {@link #testFlags(QFlags)} instead.
	 */
	@Deprecated(forRemoval=true)
	public final boolean isSet(@StrictNonNull QFlags<T> other) {
		return testFlags(other);
	}

	/**
	 * @deprecated Use {@link #testFlags(QtAbstractFlagEnumerator...)} instead.
	 */
	@Deprecated(forRemoval=true)
	@SafeVarargs
	public final boolean isSet(@Nullable T @NonNull... flags) {
		return testFlags(flags);
	}

	/**
	 * See
	 * <a href="https://doc.qt.io/qt/qflags.html#testFlag">QFlags::testFlag(Enum)const</a>
	 */
	public final boolean testFlag(@Nullable T flag) {
		return testFlagImpl(flag);
	}
	
	/**
	 * @hidden
	 */
	boolean testFlagImpl(@Nullable T flag) {
		if (flag instanceof QtFlagEnumerator) {
			QtFlagEnumerator t = (QtFlagEnumerator) flag;
			return (value & t.value()) == t.value();
		} else if (flag instanceof QtByteFlagEnumerator) {
			QtByteFlagEnumerator t = (QtByteFlagEnumerator) flag;
			return (value & t.value()) == t.value();
		} else if (flag instanceof QtShortFlagEnumerator) {
			QtShortFlagEnumerator t = (QtShortFlagEnumerator) flag;
			return (value & t.value()) == t.value();
		} else if (flag instanceof QtLongFlagEnumerator) {
			QtLongFlagEnumerator t = (QtLongFlagEnumerator) flag;
			return (value & t.value()) == t.value();
		}
		return false;
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#testFlags">QFlags::testFlags(QFlags&lt;T>)const</a>
	 */
	@SafeVarargs
	public final boolean testFlags(@Nullable T @NonNull... flags) {
		QFlags<T> f = this.clone();
		f.clearAll();
		f.set(flags);
		return testFlags(f);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#testFlags">QFlags::testFlags(QFlags&lt;T>)const</a>
	 */
	public final boolean testFlags(@StrictNonNull QFlags<T> flags) {
		return testFlagsImpl(flags);
	}
	
	/**
	 * @hidden
	 */
	boolean testFlagsImpl(@StrictNonNull QFlags<T> flags) {
		if(flags.getClass()==getClass())
			return (value & flags.value) == flags.value;
		return false;
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#testAnyFlags">QFlags::testAnyFlags(QFlags&lt;T>)const</a>
	 */
	public final boolean testAnyFlags(@StrictNonNull QFlags<T> flags) {
		return testAnyFlagsImpl(flags);
	}
	
	/**
	 * @hidden
	 */
	boolean testAnyFlagsImpl(@StrictNonNull QFlags<T> flags) {
		if(flags.getClass()==getClass())
			return (value & flags.value) != 0;
		return false;
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#testAnyFlags">QFlags::testAnyFlags(QFlags&lt;T>)const</a>
	 */
	@SafeVarargs
	public final boolean testAnyFlags(@Nullable T @NonNull... flags) {
		QFlags<T> f = this.clone();
		f.clearAll();
		f.set(flags);
		return testAnyFlags(f);
	}

	/**
	 * Clears the flag <code>other</code>.
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-and-eq-2">QFlags::operator&amp;=(QFlags&lt;T>)</a>
	 * and <a href="https://doc.qt.io/qt/qflags.html#operator-7e">QFlags::operator~()</a>
	 */
	public final void clear(@StrictNonNull QFlags<T> other) {
		clearImpl(other);
	}
	
	/**
	 * @hidden
	 */
	void clearImpl(@StrictNonNull QFlags<T> other) {
		if(other.getClass()==getClass())
			value &= ~other.value;
	}

	/**
	 * Clears all flags in <code>ts</code>.
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-and-eq-3">QFlags::operator&amp;=(Enum)</a>
	 * and <a href="https://doc.qt.io/qt/qflags.html#operator-7e">QFlags::operator~()</a>
	 */
	@SafeVarargs
	public final void clear(@Nullable T @NonNull... ts) {
		for (T flag : ts) {
			setFlag(flag, false);
		}
	}

	/**
	 * Clears all flags.
	 */
	public final void clearAll() {
		clearAllImpl();
	}
	
	/**
	 * @hidden
	 */
	void clearAllImpl() {
		value = 0;
	}

	/**
	 * This method is only for binary compatibility
	 * @hidden
	 */
	@Deprecated(forRemoval=true)
	@SuppressWarnings("unused")
	private void setValue(int value) {
		setIntValue(value);
	}
	
	/**
	 * @hidden
	 */
	@NativeAccess
	protected void setIntValue(int value) {
		this.value = value;
	}
	
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#toInt">QFlags::toInt() const</a>
	 */
	public final int toInt() {
		return intValue();
	}

	/**
	 * This method is only for binary compatibility
	 * @hidden
	 */
	@SuppressWarnings("unused")
	@Deprecated(forRemoval=true)
	private int value() {
		return intValue();
	}

	/**
	 * @hidden
	 */
	@NativeAccess
	protected int intValue() {
		return value;
	}
	
	boolean isNull() {
		return value==0;
	}
	
	/**
	 * Returns an array of flag objects represented by this QFlags.
	 * @return array of enum entries
	 */
	public @NonNull T@NonNull[] flags() {
		return flags(QtJambi_LibraryUtilities.internal.flagConstants(this));
	}

	/**
	 * Returns an unmodifiable set of flag objects represented by this QFlags.
	 * @return array of enum entries
	 */
	protected final @NonNull T@NonNull[] flags(@Nullable T @StrictNonNull[] possibleValues) {
		if (possibleValues == null || possibleValues.length == 0)
			return possibleValues;
		java.util.List<T> result = new ArrayList<>();
		T zeroValue = null;
		for (T possibleValue : possibleValues) {
			boolean isNull = false;
			if (possibleValue instanceof QtFlagEnumerator) {
				isNull = 0 == ((QtFlagEnumerator) possibleValue).value();
			} else if (possibleValue instanceof QtByteFlagEnumerator) {
				isNull = 0 == ((QtByteFlagEnumerator) possibleValue).value();
			} else if (possibleValue instanceof QtShortFlagEnumerator) {
				isNull = 0 == ((QtShortFlagEnumerator) possibleValue).value();
			} else if (possibleValue instanceof QtLongFlagEnumerator) {
				isNull = 0 == ((QtLongFlagEnumerator) possibleValue).value();
			}
			if (isNull) {
				zeroValue = possibleValue;
			} else {
				if (this.testFlag(possibleValue)) {
					result.add(possibleValue);
				}
			}
		}
		if (zeroValue != null && result.isEmpty() && isNull()) {
			result.add(zeroValue);
		}
		return result.toArray(Arrays.copyOf(possibleValues, result.size()));
	}

	/**
	 * Returns true if the given object is instance of the same class and has the same flag value.
	 */
	@Override
	public final boolean equals(Object object) {
		return object instanceof QFlags<?> && equalsImpl((QFlags<?>) object);
	}
	
	/**
	 * @hidden
	 */
	boolean equalsImpl(QFlags<?> other) {
		return other.getClass() == getClass() && other.value == value;
	}

	/**
	 * Returns a hash code computed from the flag's class and its value.
	 */
	@Override
	public final int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + getClass().hashCode();
		result = prime * result + hashCodeImpl();
		return result;
	}
	
	/**
	 * @hidden
	 */
	int hashCodeImpl() {
		return value;
	}

    /**
     * Returns the string representation of the object given by <code>QVariant(this).toString()</code>.
     */
	@Override
	public final @NonNull String toString() {
		QtAbstractFlagEnumerator[] flags = flags();
		String className = this.getClass().getName();
		int idx = className.lastIndexOf('.');
		if (idx > 0) {
			className = className.substring(idx + 1);
		}
		StringBuilder result = new StringBuilder();
		for (String part : className.split("\\$")) {
			if (part.isEmpty()) {
				result.append('$');
			} else {
				if (part.charAt(0) >= '0' && part.charAt(0) <= '9') {
					result.append('$');
				} else {
					if (result.length() > 0)
						result.append('.');
				}
				result.append(part);
			}
		}
		result.append('(');
		if (flags.length == 0 && !isNull()) {
			String hexString = hexString();
			result.append("0x");
			for (int i = hexString.length(); i < 8; ++i) {
				result.append('0');
			}
			result.append(hexString);
		} else {
			int initialLength = result.length();
			for (QtAbstractFlagEnumerator flag : flags) {
				if (result.length() > initialLength) {
					result.append('|');
				}
				result.append(flag.name());
			}
		}
		result.append(')');
		return result.toString();
	}
	
	/**
	 * @hidden
	 */
	String hexString() {
		return Integer.toHexString(value);
	}
	

	private int value;

	/**
	 * @hidden
	 */
	@io.qt.NativeAccess
	static final class ConcreteWrapper extends QFlags<QtFlagEnumerator> implements Comparable<QFlags<?>>{
		@io.qt.NativeAccess
		ConcreteWrapper(int value) {
			super(value);
		}

		public @NonNull QtFlagEnumerator @NonNull[] flags() {
			return new QtFlagEnumerator[] { new QtFlagEnumerator() {
				@Override
				public int value() {
					return ConcreteWrapper.this.intValue();
				}

				@Override
				public int ordinal() {
					return -1;
				}

				@Override
				public String name() {
					return "unknown";
				}

				@Override
				public Class<? extends QtEnumerator> getDeclaringClass() {
					return this.getClass();
				}
			} };
		}

		@Override
		public @NonNull ConcreteWrapper clone() {
			return new ConcreteWrapper(intValue());
		}

		@Override
		public @NonNull ConcreteWrapper combined(@StrictNonNull QtFlagEnumerator flag) {
			return new ConcreteWrapper(intValue() | flag.value());
		}

		@Override
		public int compareTo(@Nullable QFlags<?> o) {
			return compareToImpl(o);
		}
	}
	
	/**
	 * @hidden
	 */
	int compareToImpl(@Nullable QFlags<?> o) {
		if(o!=null && o.getClass()==getClass())
			return Integer.compare(value, o.value);
		return -1;
	}
	
	/**
     * Compares two QFlags values numerically.
     * The value returned is identical to what would be returned by:
     * <pre>
     *    Integer.compare(a.value(), b.value())
     * </pre>
     */
	public static <E extends QtAbstractFlagEnumerator> int compare(@StrictNonNull QFlags<E> a, @Nullable QFlags<E> b) {
        return a.compareToImpl(b);
    }
}
