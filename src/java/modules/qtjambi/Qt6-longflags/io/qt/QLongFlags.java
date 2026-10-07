/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

/**
 * <p>
 * The QFlags class provides a type-safe way of storing OR-combinations of enum values.
 * </p>
 * <p>
 * Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qflags.html">QFlags</a>
 * </p>
 */
public abstract class QLongFlags<T extends QtLongFlagEnumerator> extends QFlags<T> {
    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#QFlags">QFlags::QFlags()</a>
	 */
	protected QLongFlags() {
	}
	
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#QFlags-2">QFlags::QFlags(QFlag)</a>
	 */
	protected QLongFlags(long value) {
		setValue(value);
	}
	
	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#QFlags-3">QFlags::QFlags(std::initializer_list&lt;Enum>)</a>
	 */
	@SafeVarargs
	protected QLongFlags(@Nullable T @NonNull... args) {
		set(args);
	}
	
	/**
	 * Clones the flags
	 */
	public @Nullable QLongFlags<T> clone() {
		return (QLongFlags<T>) super.clone();
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-7c">QFlags::operator|(Enum) const</a>
	 */
	public @StrictNonNull QLongFlags<T> combined(@Nullable T flag) {
		return clone().setFlag(flag, true);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-and-eq-3">QFlags::operator&amp;=(Enum)</a>
	 * and <a href="https://doc.qt.io/qt/qflags.html#operator-7e">QFlags::operator~()</a>
	 */
	public @StrictNonNull QLongFlags<T> cleared(@Nullable T flag) {
		return clone().setFlag(flag, false);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-7c-1">QFlags::operator|=(QFlags&lt;T>)</a>
	 */
	public final void set(@StrictNonNull QLongFlags<T> flag) {
		if(flag.getClass()==getClass()) {
			long value = value();
			value |= flag.value();
			setValue(value);
		}
	}
	
	/**
	 * @hidden
	 */
	void setImpl(@StrictNonNull QFlags<T> flag) {
		long value = this.value();
		if(flag.getClass()==getClass())
			value |= ((QLongFlags<?>)flag).value();
		this.setValue(value);
	}
	
	boolean isNull() {
		return value()==0;
	}
	
	/**
	 * <p>
	 * Overloaded constructor for
	 * {@link #setFlag(QtAbstractFlagEnumerator, boolean)} with
	 * <code>on = true</code>.
	 * </p>
	 */
	public @NonNull QLongFlags<T> setFlag(@Nullable T flag) {
		return setFlag(flag, true);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#setFlag">QFlags::setFlag(Enum, bool)</a>
	 */
	public @NonNull QLongFlags<T> setFlag(@Nullable T flag, boolean on) {
		long value = this.value();
		if (on) {
			if (flag!=null) {
				value |= flag.value();
			}
		} else {
			if (flag!=null) {
				value &= ~flag.value();
			}
		}
		this.setValue(value);
		return this;
	}
	
	/**
	 * @hidden
	 */
	void setFlagImpl(@Nullable T flag, boolean on) {
		long value = this.value();
		if (on) {
			if (flag!=null) {
				value |= flag.value();
			}
		} else {
			if (flag!=null) {
				value &= ~flag.value();
			}
		}
		this.setValue(value);
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#testFlags">QFlags::testFlags(QFlags&lt;T>)const</a>
	 */
	public final boolean testFlags(@StrictNonNull QLongFlags<T> flags) {
		if(flags.getClass()==getClass())
			return (value() & flags.value()) == flags.value();
		return false;
	}

	/**
	 * See <a href="https://doc.qt.io/qt/qflags.html#testAnyFlags">QFlags::testAnyFlags(QFlags&lt;T>)const</a>
	 */
	public final boolean testAnyFlags(@StrictNonNull QLongFlags<T> flags) {
		if(flags.getClass()==getClass())
			return (value() & flags.value()) != 0;
		return false;
	}
	
	boolean testFlagImpl(@Nullable T flag) {
		if (flag instanceof QtLongFlagEnumerator) {
			QtLongFlagEnumerator t = (QtLongFlagEnumerator) flag;
			return (value() & t.value()) == t.value();
		}
		return super.testFlagImpl(flag);
	}

	/**
	 * Clears the flag <code>other</code>.
	 * See <a href="https://doc.qt.io/qt/qflags.html#operator-and-eq-2">QFlags::operator&amp;=(QFlags&lt;T>)</a>
	 * and <a href="https://doc.qt.io/qt/qflags.html#operator-7e">QFlags::operator~()</a>
	 */
	public final void clear(@StrictNonNull QLongFlags<T> other) {
		long value = this.value();
		if(other.getClass()==getClass())
			value &= ~other.value();
		this.setValue(value);
	}

	/**
	 * @hidden
	 */
	void clearAllImpl() {
		this.setValue(0);
	}

    /**
     * See <a href="https://doc.qt.io/qt/qflags.html#toInt"><code>QFlags::toInt() const</code></a>
     */
    public final long toLong(){
        return value();
    }
    
    /**
     * Returns the value of this QFlags.
     */
	@io.qt.NativeAccess
    public final long value() {
        return ((long) high << 32) | (super.toInt() & 0xFFFFFFFFL);
    }

    /**
     * Sets the value of this QFlags.
     * 
     * @param value new value
     */
	@io.qt.NativeAccess
    public final void setValue(long value) {
		super.setValue((int)(value & 0xFFFFFFFFL));
		this.high = (int)(value >> 32);
    }
	
	@Override
	boolean isLong() {
		return true;
	}

	private int high;

	/**
	 * @hidden
	 */
	@io.qt.NativeAccess
	static final class ConcreteWrapper extends QLongFlags<QtLongFlagEnumerator> implements Comparable<QLongFlags<?>>{
		@io.qt.NativeAccess
		ConcreteWrapper(long value) {
			super(value);
		}

		public @NonNull QtLongFlagEnumerator @NonNull[] flags() {
			return new QtLongFlagEnumerator[] { new QtLongFlagEnumerator() {
				@Override
				public long value() {
					return ConcreteWrapper.this.value();
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
				public Class<? extends QtLongEnumerator> getDeclaringClass() {
					return this.getClass();
				}
			} };
		}

		@Override
		public @NonNull ConcreteWrapper clone() {
			return new ConcreteWrapper(value());
		}

		@Override
		public @NonNull ConcreteWrapper combined(@StrictNonNull QtLongFlagEnumerator flag) {
			return new ConcreteWrapper(value() | flag.value());
		}

		@Override
		public int compareTo(@Nullable QLongFlags<?> o) {
			return compareToImpl(o);
		}
	}
	
	/**
     * Compares two QFlags values numerically.
     * The value returned is identical to what would be returned by:
     * <pre>
     *    Long.compare(a.value(), b.value())
     * </pre>
     */
    public static <E extends QtLongFlagEnumerator> int compare(@StrictNonNull QLongFlags<E> a, @Nullable QLongFlags<E> b) {
    	return a.compareToImpl(b);
    }
    
	/**
	 * @hidden
	 */
    boolean testFlagsImpl(@StrictNonNull QFlags<T> flags) {
		if(flags.getClass()==getClass())
			return testFlags((QLongFlags<T>)flags);
		return false;
	}
    
	/**
	 * @hidden
	 */
    boolean testAnyFlagsImpl(@StrictNonNull QFlags<T> flags) {
		if(flags.getClass()==getClass())
			return testAnyFlags((QLongFlags<T>)flags);
		return false;
	}
    
	/**
	 * @hidden
	 */
    void clearImpl(@StrictNonNull QFlags<T> other) {
		if(other.getClass()==getClass())
			clear((QLongFlags<T>)other);
	}
    
	/**
	 * @hidden
	 */
    int hashCodeImpl() {
		return Long.hashCode(value());
	}
    
	/**
	 * @hidden
	 */
    String hexString() {
		return Long.toHexString(value());
	}
    
	/**
	 * @hidden
	 */
    boolean equalsImpl(QFlags<?> other) {
		return other!=null && other.getClass() == getClass() && ((QLongFlags<?>)other).value() == value();
	}
    
	/**
	 * @hidden
	 */
    int compareToImpl(@NonNull QFlags<?> o) {
    	if(o!=null && o.getClass()==getClass())
			return Long.compare(value(), ((QLongFlags<?>)o).value());
		return -1;
	}
}
