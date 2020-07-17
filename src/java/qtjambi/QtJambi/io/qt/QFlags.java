/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

package io.qt;

import java.util.ArrayList;
import java.util.Arrays;

import io.qt.internal.NativeAccess;
import io.qt.internal.QtJambiInternal;

/**
 * This class manages a set of QtEnumerator values. Each enum value
 * is treated as a flag that is either set or unset. You can set and
 * clear flags, and query which flags are set.
 */
@SuppressWarnings("serial")
public abstract class QFlags<T extends QtFlagEnumerator>
    implements java.io.Serializable,
               Cloneable
{
     /**
      * Creates a new QFlags where the flags in <tt>args</tt> are set.
      */
     @SafeVarargs
	 protected QFlags(T ... args) {
         set(args);
     }
     
     protected QFlags(int value) {
         this.value = value;
     }
     
     @SuppressWarnings("unchecked")
	 public QFlags<T> clone(){
    	 try {
			return (QFlags<T>)super.clone();
		} catch (CloneNotSupportedException e) {
			return null;
		}
     }
     
     public QFlags<T> combined(T flag){
    	 QFlags<T> result = clone();
    	 result.set(flag);
    	 return result;
     }

     /**
      * Sets the flag <tt>other</tt>
      */
     public final void set(QFlags<T> other) {
         value |= other.value();
     }
     
     /**
      * Sets the flag <tt>other</tt>
      */
     public QFlags<T> setFlag(T flag) {
    	 value |= flag.value();
    	 return this;
     }

     public QFlags<T> setFlag(T flag, boolean on) {
    	 if(on)
    		 value |= flag.value();
    	 else 
    		 value &= ~flag.value();
    	 return this;
     }

     /**
      * Sets the flags in <tt>ts</tt>.
      */
     @SafeVarargs
	 public final void set(T ... ts) {
         for (T t : ts)
             value |= t.value();
     }

    /**
     * Returns true if flag <tt>other</tt> is set; otherwise, returns
     * false.
     */
     public final boolean isSet(QFlags<T> other) {
         return (value & other.value()) == other.value();
     }

     /**
      * Returns true if all <tt>ts</tt> flags are set; otherwise,
      * returns false.
      */
     @SafeVarargs
	 public final boolean isSet(T ... ts) {
         for (T t : ts) {
             if ((t.value() & value) != t.value())
                  return false;
         }
         return true;
     }

     /**
      * Returns true if flag <tt>other</tt> is set; otherwise, returns
      * false.
      */
      public final boolean testFlag(T flag) {
          return (value & flag.value()) == flag.value();
      }

     /**
      * Clears the flag <tt>other</tt>.
      */
     public final void clear(QFlags<T> other) {
         value &= ~other.value();
     }

     /**
      * Clears all flags in <tt>ts</tt>.
      *
      */
     @SafeVarargs
	 public final void clear(T ... ts) {
         for (T t : ts)
             value &= ~t.value();
     }

     /**
      * Clears all flags.
      */
     public final void clearAll() {
         value = 0;
     }

     /**
      * Sets the value of this QFlags.
      */
     @NativeAccess
     public final void setValue(int value) {
         this.value = value;
     }

     /**
      * Returns the value of this QFlags.
      */
     @NativeAccess
     public final int value() {
         return value;
     }
     
     /**
      * Returns an unmodifiable set of flag objects represented by this QFlags.
      */
	 public T[] flags() {
		 return flags(QtJambiInternal.flagConstants(this));
     }
     
     /**
      * Returns an unmodifiable set of flag objects represented by this QFlags.
      */
     protected final T[] flags(T[] possibleValues){
    	 if(possibleValues==null || possibleValues.length==0)
    		 return possibleValues;
    	 java.util.List<T> result = new ArrayList<>();
    	 T zeroValue = null;
    	 for (T possibleValue : possibleValues) {
    		 if(possibleValue.value()==0) {
    			 zeroValue = possibleValue;
    		 }else {
	 			 if(this.isSet(possibleValue)) {
	 				 result.add(possibleValue);
	 			 }
    		 }
		 }
    	 if(zeroValue!=null && result.isEmpty() && value==0) {
    		 result.add(zeroValue);
    	 }
    	 return result.toArray(Arrays.copyOf(possibleValues, result.size()));
     }

     /**
      * {@inheritDoc}
      */
     @Override
     public final boolean equals(Object object) {
         return object != null
             && object.getClass() == getClass()
             && ((QFlags<?>) object).value() == value();
     }
     
     /**
      * {@inheritDoc}
      */
     @Override
     public final int hashCode() {
 		final int prime = 31;
 		int result = 1;
 		result = prime * result + getClass().hashCode();
 		result = prime * result + value();
 		return result;
     }

     /**
      * {@inheritDoc}
      */
     @Override
     public final String toString() {
    	 QtFlagEnumerator[] flags = flags();
    	 String className = this.getClass().getName();
    	 int idx = className.lastIndexOf('.');
    	 if(idx>0) {
    		 className = className.substring(idx+1);
    	 }
    	 StringBuilder result = new StringBuilder();
    	 for(String part : className.split("\\$")) {
    		 if(part.isEmpty()) {
    			 result.append('$');
    		 }else {
    			 if(part.charAt(0)>='0' && part.charAt(0)<='9') {
    				 result.append('$');
    			 }else {
    				 if(result.length()>0)
    					 result.append('.');
    			 }
    			 result.append(part);
    		 }
    	 }
    	 result.append('(');
    	 if(flags.length==0 && value()!=0) {
    		 String hexString = Integer.toHexString(value);
    		 result.append("0x");
    		 for (int i = hexString.length(); i < 8; ++i) {
    			 result.append('0');
			 }
    		 result.append(hexString);    		 
    	 }else {
	    	 int initialLength = result.length();
	    	 for(QtFlagEnumerator flag : flags) {
	    		 if(result.length()>initialLength) {
	    			 result.append('|');
	    		 }
	    		 result.append(flag.name());
	    	 }
    	 }
    	 result.append(')');
    	 return result.toString();
     }

     private int value;
     
     @io.qt.internal.NativeAccess
     static final class ConcreteWrapper extends QFlags<QtFlagEnumerator>{
         @io.qt.internal.NativeAccess
    	 ConcreteWrapper(int value) {
			super(value);
		 }
         
         public QtFlagEnumerator[] flags(){
         	return new QtFlagEnumerator[]{
        			new QtFlagEnumerator(){
        				@Override public int value() { return ConcreteWrapper.this.value(); }
						@Override public int ordinal() { return -1; }
						@Override public String name() { return "unknown"; }
						@Override public Class<? extends QtEnumerator> getDeclaringClass() { return this.getClass(); }
					}
        	};
         }

		@Override
		public ConcreteWrapper clone() {
			return new ConcreteWrapper(value());
		}

		@Override
		public ConcreteWrapper combined(QtFlagEnumerator flag) {
			return new ConcreteWrapper(value() | flag.value());
		}
     }
}

