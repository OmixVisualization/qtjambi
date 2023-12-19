/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
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

import java.lang.ref.WeakReference;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import java.util.List;

import io.qt.core.QIODevice;

/**
 * Represents a native pointer.
 */
public final class QNativePointer {
    static {
    	io.qt.QtUtilities.initializePackage("io.qt.internal");
    }
    
    /**
     * The type of the pointer.
     */
    public enum Type implements QtEnumerator{
        /** <code>representing bool*</code> */ Boolean,
        /** <code>representing char*</code> */ Byte,
        /** <code>representing char_16*</code> */ Char,
        /** <code>representing short*</code> */ Short,
        /** <code>representing int*</code> */ Int,
        /** <code>representing long long*</code> */ Long,
        /** <code>representing float*</code> */ Float,
        /** <code>representing double*</code> */ Double,
        /** <code>representing void* or any other typeed pointer</code> */ Pointer,
        /** <code>representing char *const*</code> */ String;
    	
        public static Type resolve(int type) {
            switch (type) {
            case 0:
                return Type.Boolean;
            case 1:
                return Type.Byte;
            case 2:
                return Type.Char;
            case 3:
                return Type.Short;
            case 4:
                return Type.Int;
            case 5:
                return Type.Long;
            case 6:
                return Type.Float;
            case 7:
                return Type.Double;
            case 8:
                return Type.Pointer;
            case 9:
                return Type.String;
            default:
            	throw new QNoSuchEnumValueException(type);
            }
        }
    }

    /**
     * The AutoDeleteMode enum describes how garbage collection of the
     * QNativePointer handles the deletion of the native pointer. By default,
     * the mode is set to Delete or DeleteArray.
     */
    public enum AutoDeleteMode implements QtEnumerator{
        /**
         * Use this deletion mode if you do not want to delete the c++ pointer
         * when the QNativePointer object is garbage collected.
         */
        None,
        /**
         * Free must be used if the pointer was allocated using c++ <code>free()</code>.
         * Currently, no pointers in QtJambi is allocated in this manner.
         */
        Free,
        /**
         * The pointer is allocated with c++ <code>new</code>.
         * This should be used for all pointers in QtJambi
         */
        Delete,
        /**
         * This must be the mode of deletion if the pointer is an array.
         */
        DeleteArray;
    	
    	public static AutoDeleteMode resolve(int type) {
            switch (type) {
            case 0:
                return None;
            case 1:
                return Free;
            case 2:
                return Delete;
            case 3:
                return DeleteArray;
            default:
            	throw new QNoSuchEnumValueException(type);
            }
        }
    }

    /**
     * Creates a native pointer of the specified <code>type</code>.
     * The object has an indirection of 1 and the internal pointer
     * will be deleted when the QNativePointer object is deleted.
     *
     * @param type the type of pointer to create.
     */
    public QNativePointer(Type type) {
        this(type, 1);
    }

    /**
     * Creates a native pointer to an array with <code>size</code>
     * length of the specified <code>type</code>. The pointer will
     * have an indirection of 1 and be deleted when the QNativePointer
     * is garbage collected.
     *
     * @param type the type of pointer to create.
     * @param size the size of the array.
     */
    public QNativePointer(Type type, long size) {
        this(type, size, 1);
    }

    /**
     * Creates a native pointer of the specified <code>type</code>.
     * It will be an array if <code>size</code> is larger than one and
     * have an indirection of <code>indirections</code>. For instance,
     * the following Java statement will create a <code>**char</code>
     * pointer with the first array dimension of length 5:
     * <code>QNativePointer ptr = new
     * QNativePointer(QNativePointer.Type.Int, 5, 2);</code>
     *
     * @param type the type of pointer to create.
     * @param size the length of the array.
     * @param indirections the number of indirections for the pointer.
     */
    public QNativePointer(Type type, long size, int indirections) {
    	this(type, size, indirections, false);
    }
    
    private QNativePointer(Type type, long size, int indirections, boolean isReadOnly) {
    	data = new Data();
    	QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::dispose);
        if (indirections < 1)
            throw new IllegalArgumentException("level of indirection must be at least 1");
        if (size == 0)
            throw new IllegalArgumentException("size must be at least 1");
        data.m_ptr = createPointer(type.ordinal(), size, indirections);
        data.m_type = type;
        data.m_knownSize = size;
        data.m_indirections = indirections;
        data.m_autodelete = size == 1 ? AutoDeleteMode.Delete : AutoDeleteMode.DeleteArray;
        data.m_isReadonly = isReadOnly;
    }
    
    @NativeAccess
    private QNativePointer(int type, long size, int indirections, long ptr, boolean isReadOnly) {
    	data = new Data();
    	QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::dispose);
        data.m_ptr = ptr;
        data.m_type = Type.resolve(type);
        data.m_knownSize = size;
        data.m_indirections = indirections;
        data.m_autodelete = AutoDeleteMode.None;
        data.m_isReadonly = isReadOnly;
    }

    @NativeAccess
    private QNativePointer(int type, long size, int indirections, boolean isReadOnly) {
        this(Type.resolve(type), size, indirections, isReadOnly);
    }
    
    @NativeAccess
    private QNativePointer(Buffer buffer, long ptr, long bufferAccess) {
    	if(buffer.isDirect()) {
    		data = new Data();
    	}else {
    		data = new BufferData(bufferAccess, buffer);
    	}
    	QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::dispose);
        data.m_ptr = ptr;
        if(buffer instanceof ByteBuffer)
        	data.m_type = Type.Byte;
        else if(buffer instanceof IntBuffer)
        	data.m_type = Type.Int;
        else if(buffer instanceof ShortBuffer)
        	data.m_type = Type.Short;
        else if(buffer instanceof LongBuffer)
        	data.m_type = Type.Long;
        else if(buffer instanceof CharBuffer)
        	data.m_type = Type.Char;
        else if(buffer instanceof FloatBuffer)
        	data.m_type = Type.Float;
        else if(buffer instanceof DoubleBuffer)
        	data.m_type = Type.Double;
        else
        	data.m_type = Type.Pointer;

        data.m_knownSize = buffer.remaining();
        data.m_indirections = 1;
        data.m_isReadonly = buffer.isReadOnly();
        if(buffer.isDirect()) {
        	QtJambi_LibraryUtilities.internal.registerCleaner(buffer, data::reset);
        }
    }

    private QNativePointer() {
    	data = new Data();
    	QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::dispose);
    	data.m_knownSize = -1;
    	data.m_autodelete = AutoDeleteMode.None;
    };

    /**
     * If the native pointer is of boolean type, this function returns
     * its value. If it is an array of booleans, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public boolean booleanValue() {
        return booleanAt(0);
    }

    /**
     * If the native pointer is of byte type, this function returns
     * its value. If it is an array of bytes, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public byte byteValue() {
        return byteAt(0);
    }

    /**
     * If the native pointer is of char type, this function returns
     * its value. If it is an array of chars, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public char charValue() {
        return charAt(0);
    }

    /**
     * If the native pointer is of short type, this function returns
     * its value. If it is an array of shorts, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public short shortValue() {
        return shortAt(0);
    }

    /**
     * If the native pointer is of int type, this function returns
     * its value. If it is an array of ints, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public int intValue() {
        return intAt(0);
    }

    /**
     * If the native pointer is of long type, this function returns
     * its value. If it is an array of longs, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public long longValue() {
        return longAt(0);
    }

    /**
     * If the native pointer is of float type, this function returns
     * its value. If it is an array of floats, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public float floatValue() {
        return floatAt(0);
    }

    /**
     * If the native pointer is of double type, this function returns
     * its value. If it is an array of doubles, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public double doubleValue() {
        return doubleAt(0);
    }

    /**
     * If the native pointer is of boolean type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public boolean[] booleanArray() {
        verifyAccess(Type.Boolean, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	boolean[] result = new boolean[(int)this.data.m_knownSize];
    	for (int i = 0; i < result.length; i++) {
    		result[i] = booleanAt(i);
		}
        return result;
    }

    /**
     * If the native pointer is of byte type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public byte[] byteArray() {
        verifyAccess(Type.Byte, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	byte[] result = new byte[(int)this.data.m_knownSize];
    	copyBackB(data.m_ptr, result);
        return result;
    }

    /**
     * If the native pointer is of char type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public char[] charArray() {
        verifyAccess(Type.Char, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	char[] result = new char[(int)this.data.m_knownSize];
    	for (int i = 0; i < result.length; i++) {
    		result[i] = charAt(i);
		}
        return result;
    }

    /**
     * If the native pointer is of short type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public short[] shortArray() {
        verifyAccess(Type.Short, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	short[] result = new short[(int)this.data.m_knownSize];
    	copyBackS(data.m_ptr, result);
        return result;
    }

    /**
     * If the native pointer is of int type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public int[] intArray() {
        verifyAccess(Type.Int, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	int[] result = new int[(int)this.data.m_knownSize];
    	copyBackI(data.m_ptr, result);
        return result;
    }

    /**
     * If the native pointer is of long type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public long[] longArray() {
        verifyAccess(Type.Long, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	long[] result = new long[(int)this.data.m_knownSize];
    	copyBackJ(data.m_ptr, result);
        return result;
    }

    /**
     * If the native pointer is of float type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public float[] floatArray() {
        verifyAccess(Type.Float, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	float[] result = new float[(int)this.data.m_knownSize];
    	copyBackF(data.m_ptr, result);
        return result;
    }

    /**
     * If the native pointer is of double type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public double[] doubleArray() {
        verifyAccess(Type.Double, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	double[] result = new double[(int)this.data.m_knownSize];
    	copyBackD(data.m_ptr, result);
        return result;
    }

    /**
     * If the native pointer is of pointer type, this function returns
     * its value matching the given valueType.
     * @param valueType the type of the value
     *
     * @return the value of the pointer.
     */
    @SuppressWarnings("unchecked")
	public <T extends QtObjectInterface> T[] objectArray(Class<T> valueType) {
    	if(data.m_knownSize>1) {
    		verifyAccess(Type.Byte, 0);
    	}else {
    		verifyAccess(Type.Pointer, 0);
    	}
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	Object[] array = (Object[])java.lang.reflect.Array.newInstance(valueType, (int)this.data.m_knownSize);
    	for (int i = 0; i < array.length; i++) {
    		array[i] = objectAt(valueType, i);
		}
        return (T[])array;
    }

    /**
     * If the native pointer is of pointer type, this function returns
     * its value. If it is an array of pointers, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public QNativePointer pointerValue() {
        return pointerAt(0);
    }

    /**
     * If the native pointer is of pointer type, this function returns
     * its value matching the given valueType. If it is an array of pointers, the first element is
     * returned.
     * @param valueType the type of the value
     *
     * @return the value of the pointer.
     */
    public <T extends QtObjectInterface> T object(Class<T> valueType) {
        return objectAt(valueType, 0);
    }

    /**
     * If the native pointer is of string type, this function returns
     * its value. If it is an array of strings, the first element is
     * returned.
     *
     * @return the value of the pointer.
     */
    public String stringValue() {
        return stringAt(0);
    }

    /**
     * If the native pointer is of string type, this function returns
     * its values as array.
     *
     * @return the values of the pointer.
     */
    public String[] stringArray() {
        verifyAccess(Type.String, 0);
    	if(this.data.m_knownSize>Integer.MAX_VALUE) {
    		throw new IndexOutOfBoundsException("The pointer's size exceeds allowed array size.");
    	}
    	String[] result = new String[(int)this.data.m_knownSize];
    	for (int i = 0; i < result.length; i++) {
    		result[i] = stringAt(i);
		}
        return result;
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must be boolean.
     *
     * @param value the value to which the pointer is set.
     */
    public void setBooleanValue(boolean value) {
        setBooleanAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must be byte.
     *
     * @param value the value to which the pointer is set.
     */
    public void setByteValue(byte value) {
        setByteAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must be char.
     *
     * @param value the value to which the pointer is set.
     */
    public void setCharValue(char value) {
        setCharAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must be short.
     *
     * @param value the value to which the pointer is set.
     */
    public void setShortValue(short value) {
        setShortAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must be int.
     *
     * @param value the value to which the pointer is set.
     */
    public void setIntValue(int value) {
        setIntAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must be long.
     *
     * @param value the value to which the pointer is set.
     */
    public void setLongValue(long value) {
        setLongAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must be float.
     *
     * @param value the value to which the pointer is set.
     */
    public void setFloatValue(float value) {
        setFloatAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The type of the pointer must double.
     *
     * @param value the value to which the pointer is set.
     */
    public void setDoubleValue(double value) {
        setDoubleAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The pointer must be of pointer type.
     *
     * @param value the value to which the pointer is set.
     */
    public void setPointerValue(QNativePointer value) {
        setPointerAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The pointer must be of pointer type.
     *
     * @param value the value to which the pointer is set.
     */
    public <T extends QtObjectInterface> void setObjectValue(T value) {
        setObjectAt(0, value);
    }

    /**
     * Sets the value of this pointer to <code>value</code>.
     * The pointer must point to a string.
     *
     * @param value the value to which the pointer is set.
     */
    public void setStringValue(String value) {
        setStringAt(0, value);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public boolean booleanAt(long pos) {
        verifyAccess(Type.Boolean, pos);
        return readBoolean(data.m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public byte byteAt(long pos) {
        verifyAccess(Type.Byte, pos);
        return readByte(data.m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public char charAt(long pos) {
        verifyAccess(Type.Char, pos);
        return readChar(data.m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public short shortAt(long pos) {
        verifyAccess(Type.Short, pos);
        return readShort(data.m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public int intAt(long pos) {
        verifyAccess(Type.Int, pos);
        return readInt(data.m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public long longAt(long pos) {
        verifyAccess(Type.Long, pos);
        return readLong(data.m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public float floatAt(long pos) {
        verifyAccess(Type.Float, pos);
        return readFloat(data.m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public double doubleAt(long pos) {
        verifyAccess(Type.Double, pos);
        return readDouble(data.m_ptr, pos);
    }
    
    public ByteBuffer byteBuffer() {
        verifyAccess(Type.Byte, 0);
        return toByteBuffer(data.m_ptr, data.m_knownSize, data.m_isReadonly);
    }
    
    public CharBuffer charBuffer() {
        verifyAccess(Type.Char, 0);
        return toByteBuffer(data.m_ptr, data.m_knownSize, data.m_isReadonly).asCharBuffer();
    }
    
    public ShortBuffer shortBuffer() {
        verifyAccess(Type.Short, 0);
        return toByteBuffer(data.m_ptr, data.m_knownSize, data.m_isReadonly).asShortBuffer();
    }
    
    public IntBuffer intBuffer() {
        verifyAccess(Type.Int, 0);
        return toByteBuffer(data.m_ptr, data.m_knownSize, data.m_isReadonly).asIntBuffer();
    }
    
    public LongBuffer longBuffer() {
        verifyAccess(Type.Long, 0);
        return toByteBuffer(data.m_ptr, data.m_knownSize, data.m_isReadonly).asLongBuffer();
    }
    
    public FloatBuffer floatBuffer() {
        verifyAccess(Type.Float, 0);
        return toByteBuffer(data.m_ptr, data.m_knownSize, data.m_isReadonly).asFloatBuffer();
    }
    
    public DoubleBuffer doubleBuffer() {
        verifyAccess(Type.Double, 0);
        return toByteBuffer(data.m_ptr, data.m_knownSize, data.m_isReadonly).asDoubleBuffer();
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public QNativePointer pointerAt(long pos) {
        verifyAccess(Type.Pointer, pos);
        long ptr = readPointer(data.m_ptr, pos);
        return fromNative(ptr, data.m_type, -1, data.m_indirections - 1, data.m_isReadonly);
    }

    /**
     * Returns the value of the native pointer matching the given valueType at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     * @param valueType the type of the value
     * @param pos the array index
     * 
     * @return the value of the pointer.
     */
    public <T extends QtObjectInterface> T objectAt(Class<T> valueType, long pos) {
    	if(data.m_knownSize>1 && pos>0) {
    		verifyAccess(Type.Byte, pos);
    	}else {
    		verifyAccess(Type.Pointer, pos);
    	}
        T object = readObject(data.m_ptr, valueType, pos, data.m_isReadonly, data.m_type.value(), data.m_knownSize);
        if(data.m_autodelete!=AutoDeleteMode.None) {
        	if(data.m_dependentObjects==null) {
        		data.m_dependentObjects = new ArrayList<>();
        	}
        	data.m_dependentObjects.add(new WeakReference<QtObjectInterface>(object));
        }
        return object;
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public String stringAt(long pos) {
        verifyAccess(Type.String, pos);
        return readString(data.m_ptr, pos);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setBooleanAt(long pos, boolean value) {
        verifyWriteAccess(Type.Boolean, pos);
        writeBoolean(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setByteAt(long pos, byte value) {
        verifyWriteAccess(Type.Byte, pos);
        writeByte(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setCharAt(long pos, char value) {
        verifyWriteAccess(Type.Char, pos);
        writeChar(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setShortAt(long pos, short value) {
        verifyWriteAccess(Type.Short, pos);
        writeShort(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setIntAt(long pos, int value) {
        verifyWriteAccess(Type.Int, pos);
        writeInt(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setLongAt(long pos, long value) {
        verifyWriteAccess(Type.Long, pos);
        writeLong(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setFloatAt(long pos, float value) {
        verifyWriteAccess(Type.Float, pos);
        writeFloat(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setDoubleAt(long pos, double value) {
        verifyWriteAccess(Type.Double, pos);
        writeDouble(data.m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setPointerAt(long pos, QNativePointer value) {
        verifyWriteAccess(Type.Pointer, pos);
        if(value != null)
            value.setAutoDeleteMode(AutoDeleteMode.None);
        writePointer(data.m_ptr, pos, value == null ? 0 : value.data.m_ptr);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public <T extends QtObjectInterface> void setObjectAt(long pos, T value) {
    	if(data.m_knownSize>1 && pos>0) {
    		verifyWriteAccess(Type.Byte, pos);
    	}else {
    		verifyWriteAccess(Type.Pointer, pos);
    	}
        Class<?> valueType = QtJambi_LibraryUtilities.internal.getClass(value);
        writeObject(data.m_ptr, data.m_knownSize, valueType, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setStringAt(long pos, String value) {
        verifyWriteAccess(Type.String, pos);
        writeString(data.m_ptr, pos, value);
    }

    /**
     * Returns the type of the native pointer.
     *
     * @return the data type of the native pointer
     */
    public Type type() {
        return data.m_type;
    }

    /**
     * Returns the number of indirections of the pointer.
     *
     * @return the number of indirections of the pointer
     */
    public int indirections() {
        return data.m_indirections;
    }
    
    public long knownSize() {
        return data.m_knownSize;
    }
    
    @NativeAccess
    private long byteSize() {
    	if(data.m_knownSize==-1 && data.m_indirections==1 && data.m_type==Type.Pointer) {
    		return -1;
    	}
    	if(data.m_indirections>1 && data.m_type==Type.Pointer) {
    		if(data.m_knownSize>0)
    			return data.m_knownSize * 8;
    		else
    			return -1;
    	}
    	if(data.m_knownSize>0 && data.m_indirections==1) {
	    	switch(data.m_type) {
			case Boolean:
			case Byte:
				return data.m_knownSize;
			case Short:
			case Char:
				return data.m_knownSize * 2;
			case Float:
			case Int:
				return data.m_knownSize * 4;
			case Double:
			case Long:
			case Pointer:
				return data.m_knownSize * 8;
			case String:
				return -1;
			default:
				break;
	    	}
    	}
        return -1;
    }
    
    /**
     *  Returns true if the native pointer is 0; otherwise false.
     *
     * @return true if the native pointer is 0.
     */
    public boolean isNull() {
        return data.isNull();
    }

    /**
     * Returns the auto-delete mode of the pointer.
     *
     * @return the auto-delete mode of this QNativePointer
     */
    public AutoDeleteMode autoDeleteMode() {
        return data.m_autodelete;
    }

    /**
     * This function sets the auto delete mode of the QNativePointer.
     *
     * The internal pointer is deleted by default when the
     * QNativePointer object is garbage collected, so you only need to
     * call this function when you want to keep the pointer valid
     * after the Java object is garbage collected.
     *
     * @param autodelete the new auto delete mode.
     */
    public void setAutoDeleteMode(AutoDeleteMode autodelete) {
    	data.m_autodelete = autodelete;
    }

    /**
     * This function deletes the internal pointer.
     *
     * Currently, all QNativePointers should be deleted using the
     * delete() function.
     * After the pointer has been deleted, you cannot use the same
     * QNativePointer to allocate further data. Note also that the
     * pointer will by default be deleted upon garbage collection.
     */
    public void free() {
        data.free();
    }

    /**
     * This function deletes the internal pointer.
     *
     * After the pointer has been deleted, you cannot allocate it
     * again. The pointer is by default deleted when the
     * QNativePointer object is garbage collected.
     */
    public void delete() {
        data.delete();
    }

    /**
     * This function deletes elements in the array of this
     * QNativePointer.
     *
     * After the pointer has been deleted, you cannot use this
     * QNativePointer object again.
     */
    public void deleteArray() {
        data.deleteArray();
    }

    /**
     * Returns the native pointer.
     *
     * The returned long is the <code>void *</code> value in c++.
     *
     * @return the native pointer.
     */
    public long pointer() {
        return data.m_ptr;
    }

    /**
     * This function creates a QNativePointer from an existing c++
     * pointer of unknown type. The long is the <code>void *</code> (i.e., address)
     * value of the pointer.
     *
     * @param ptr the void * value of the pointer.
     * @return a QNativePointer object with ptr as the native pointer
     */
    public static QNativePointer fromNative(long ptr) {
        QNativePointer nativePointer = new QNativePointer();
        nativePointer.data.m_ptr = ptr;
        nativePointer.data.m_knownSize = -1;
        nativePointer.data.m_type = Type.Pointer;
        nativePointer.data.m_indirections = 1;
        nativePointer.data.m_isReadonly = true;
        return nativePointer;
    }

    /**
     * This function creates a QNativePointer from an existing c++
     * pointer. The long is the <code>void *</code> (i.e., address)
     * value of the pointer. There are several ways of acquiring a native pointer. For instance,
     * QNativePointer internal pointer is returned by pointer(), and
     * QtJambiObject.nativeId() returns the c++ pointer to its Qt object.
     *
     * @param ptr the void * value of the pointer.
     * @param type the Type of the pointer
     * @param indirections the number of pointer indirections
     * @return a QNativePointer object with ptr as the native pointer
     */
    public static QNativePointer fromNative(long ptr, Type type, long size, int indirections, boolean readOnly) {
        QNativePointer nativePointer = new QNativePointer();
        nativePointer.data.m_ptr = ptr;
        nativePointer.data.m_knownSize = size;
        nativePointer.data.m_type = type;
        nativePointer.data.m_indirections = indirections;
        nativePointer.data.m_isReadonly = readOnly;
        return nativePointer;
    }
    
    @SafeVarargs
	public static <T extends QtObjectInterface> QNativePointer fromArray(T... array) {
    	Class<?> arrayClass = QtJambi_LibraryUtilities.internal.getClass(array);
    	Class<?> componentType = arrayClass.getComponentType();
    	return fromArray(componentType, array);
    }
    
	private static native <T extends QtObjectInterface> QNativePointer fromArray(Class<?> valueType, T[] array);
    
	public static native <T extends QtObjectInterface> QNativePointer fromObject(T object);
	
	public static native QNativePointer fromBuffer(Buffer buffer);

    public static QNativePointer fromArray(int data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Int, data.length);
        copyFromI(np.data.m_ptr, data);
        return np;
    }

    public static QNativePointer fromArray(byte data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Byte, data.length);
        copyFromB(np.data.m_ptr, data);
        return np;
    }
    
    public static QNativePointer fromArray(float data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Float, data.length);
        copyFromF(np.data.m_ptr, data);
        return np;
    }

    public static QNativePointer fromArray(double data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Double, data.length);
        copyFromD(np.data.m_ptr, data);
        return np;
    }

    public static QNativePointer fromArray(boolean data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean, data.length);
        for (int i=0; i<data.length; ++i)
            np.setBooleanAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(char data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Char, data.length);
        for (int i=0; i<data.length; ++i)
            np.setCharAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(long data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Long, data.length);
        copyFromJ(np.data.m_ptr, data);
        return np;
    }

    public static QNativePointer fromArray(short data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Short, data.length);
        copyFromS(np.data.m_ptr, data);
        return np;
    }
    
    private static native void copyFromB(long ptr, byte[] array);
    private static native void copyFromS(long ptr, short[] array);
    private static native void copyFromI(long ptr, int[] array);
    private static native void copyFromJ(long ptr, long[] array);
    private static native void copyFromD(long ptr, double[] array);
    private static native void copyFromF(long ptr, float[] array);
    private static native void copyBackB(long ptr, byte[] array);
    private static native void copyBackS(long ptr, short[] array);
    private static native void copyBackI(long ptr, int[] array);
    private static native void copyBackJ(long ptr, long[] array);
    private static native void copyBackD(long ptr, double[] array);
    private static native void copyBackF(long ptr, float[] array);

    public static QNativePointer fromArray(Integer data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Int, data.length);
        for (int i=0; i<data.length; ++i)
        	if(data[i]!=null) np.setIntAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(Byte data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Byte, data.length);
        for (int i=0; i<data.length; ++i)
        	if(data[i]!=null) np.setByteAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(Float data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Float, data.length);
        for (int i=0; i<data.length; ++i)
        	if(data[i]!=null) np.setFloatAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(Double data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Double, data.length);
        for (int i=0; i<data.length; ++i)
        	if(data[i]!=null) np.setDoubleAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(Boolean data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean, data.length);
        for (int i=0; i<data.length; ++i)
        	if(data[i]!=null) np.setBooleanAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(Character data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Char, data.length);
        for (int i=0; i<data.length; ++i)
        	if(data[i]!=null) np.setCharAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(Long data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Long, data.length);
        for (int i=0; i<data.length; ++i)
        	if(data[i]!=null) np.setLongAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(Short data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.Short, data.length);
        for (int i=0; i<data.length; ++i)
            if(data[i]!=null) np.setShortAt(i, data[i]);
        return np;
    }

    public static QNativePointer fromArray(String data[]) {
        QNativePointer np = new QNativePointer(QNativePointer.Type.String, data.length);
        for (int i=0; i<data.length; ++i)
            np.setStringAt(i, data[i]);
        return np;
    }

    /**
     * Returns if verification is enabled or not.
     *
     * @return true if verification is enabled; otherwise false.
     */
    public boolean verificationEnabled() {
        return data.m_verification_enabled;
    }
    
    public boolean isReadOnly() {
        return data.m_isReadonly;
    }

    /**
     * Sets if the any accesses should be type verified or not. By default this
     * value is set to true, meaning that trying to access a char pointer as an
     * int pointer will trigger an exception. Disabling this value allows
     * complete access, but without any safety, so incorrect usage may lead to
     * memory corruption in the C++ implementation.
     *
     * @param a Set to true if verification should be enabled.
     */
    public void setVerificationEnabled(boolean a) {
    	data.m_verification_enabled = a;
    }

    /**
     * This is an overloaded function provided for convenience.
     * @see #fromNative(long ptr, Type type, int indirections)
     */
    @NativeAccess
    private static QNativePointer fromNative(long ptr, int type, long size, int indirections, boolean readOnly) {
        return fromNative(ptr, Type.resolve(type), size, indirections, readOnly);
    }

    /**
     * Creates a char** native pointer from the array of input strings.
     *
     * @param strings the input strings
     * @return a char **
     */
    public static QNativePointer createCharPointerPointer(String strings[]) {
        if (strings == null)
            return null;
        QNativePointer ptrs = new QNativePointer(Type.Byte, strings.length + 1, 2);
        for (int j = 0; j < strings.length; ++j) {
            String string = strings[j];
            ptrs.setPointerAt(j, createCharPointer(string));
        }
        ptrs.setPointerAt(strings.length, null);
        return ptrs;
    }

    /**
     * Creates a char* from the input string
     *
     * @param string The input string
     * @return The char*
     */
    public static QNativePointer createCharPointer(String string) {
        if (string == null)
            return null;
        byte[] data = string.getBytes();
        QNativePointer s = new QNativePointer(QNativePointer.Type.Byte, data.length + 1);
        copyFromB(s.data.m_ptr, data);
        s.setByteAt(data.length, (byte) 0);
        return s;
    }
    
    private void verifyWriteAccess(Type type, long pos) {
    	verifyAccess(type, pos);
    	if(data.m_isReadonly){
    		throw new ReadOnlyNativePointerException();
    	}
    }

    private void verifyAccess(Type type, long pos) {
    	if(data.m_isInvalid)
    		throw new IllegalStateException();
        if (!data.m_verification_enabled)
            return;

        if (isNull())
            throw new NullPointerException("native pointer is null");

        if (pos < 0)
            throw new IndexOutOfBoundsException("negative index: " + pos);

        if (data.m_knownSize >= 0 && pos >= data.m_knownSize)
            throw new IndexOutOfBoundsException("size: " + data.m_knownSize + ", access at: " + pos);

        if (data.m_indirections > 1) {
            if (type != Type.Pointer)
                throw new ClassCastException("accessing pointer with " + data.m_indirections
                        + " levels of indirection as " + type);
        } else if (type != data.m_type) {
            throw new ClassCastException("type: " + data.m_type + ", accessed as: " + type);
        }
    }
    
    public QIODevice openAsDevice(QIODevice.OpenModeFlag... openMode) {
    	return openAsDevice(new QIODevice.OpenMode(openMode));
    }
    
    public QIODevice openAsDevice(QIODevice.OpenMode openMode) {
    	if(data.m_isInvalid)
    		throw new IllegalStateException();
        if (isNull())
            throw new NullPointerException("native pointer is null");
    	if(openMode.testFlag(QIODevice.OpenModeFlag.WriteOnly)) {
        	if(data.m_isReadonly){
        		throw new ReadOnlyNativePointerException();
        	}
    	}
    	QIODevice ioDevice = ioDevice();
    	if(ioDevice.open(openMode)) {
    		return ioDevice;
    	}else {
    		return null;
    	}
    }
    
    private native QIODevice ioDevice();

    private static native boolean readBoolean(long ptr, long pos);
    private static native byte readByte(long ptr, long pos);
    private static native char readChar(long ptr, long pos);
    private static native short readShort(long ptr, long pos);
    private static native int readInt(long ptr, long pos);
    private static native long readLong(long ptr, long pos);
    private static native float readFloat(long ptr, long pos);
    private static native double readDouble(long ptr, long pos);
    private static native ByteBuffer toByteBuffer(long ptr, long capacity, boolean readOnly);
    private static native long readPointer(long ptr, long pos);
    private static native <T extends QtObjectInterface> T readObject(long ptr, Class<T> valueType, long pos, boolean readOnly, int type, long size);
    private static native String readString(long ptr, long pos);
    private static native void writeBoolean(long ptr, long pos, boolean value);
    private static native void writeByte(long ptr, long pos, byte value);
    private static native void writeChar(long ptr, long pos, char value);
    private static native void writeShort(long ptr, long pos, short value);
    private static native void writeInt(long ptr, long pos, int value);
    private static native void writeLong(long ptr, long pos, long value);
    private static native void writeFloat(long ptr, long pos, float value);
    private static native void writeDouble(long ptr, long pos, double value);
    private static native void writePointer(long ptr, long pos, long value);
    private static native void writeObject(long ptr, long knownSize, Class<?> valueType, long pos, QtObjectInterface value);
    private static native void writeString(long ptr, long pos, String value);
    private static native long createPointer(int type, long size, int indirections);
    private static native void deletePointer(long ptr, int type, int deleteMode);
    private static native void deleteBufferAccess(long ptr, boolean readOnly);
    
    private final Data data;

    public final void invalidate() {
		data.dispose();
	}
    
    private static class Data{
        long m_ptr = 0;
        private Type m_type = Type.Pointer;
        private long m_knownSize = 0;
        AutoDeleteMode m_autodelete = AutoDeleteMode.None;
        private int m_indirections = 0;
        private boolean m_isReadonly = true;
        boolean m_isInvalid = false;
        private boolean m_verification_enabled = true;
        private List<WeakReference<? extends QtObjectInterface>> m_dependentObjects;
        
        void invalidateDependentObjects() {
        	if(m_dependentObjects!=null) {
        		for(WeakReference<? extends QtObjectInterface> object : m_dependentObjects) {
        			QtJambi_LibraryUtilities.internal.invalidateObject(object.get());
        		}
        		m_dependentObjects = null;
        	}
        }
        
        void dispose() {
        	invalidateDependentObjects();
        	switch (m_autodelete) {
            case Free:
                free();
                break;
            case Delete:
                delete();
                break;
            case DeleteArray:
                deleteArray();
                break;
    		case None:
    			break;
    		default:
    			break;
            }
            m_ptr = 0;
            m_autodelete = AutoDeleteMode.None;
            m_isInvalid = true;
        }
        
		private void free() {
            if (isNull())
                return;
            deletePointer(m_ptr, m_type.ordinal(), 0);
            m_ptr = 0;
        }
        
        private void delete() {
            if (isNull())
                return;
            deletePointer(m_ptr, m_type.ordinal(), 1);
            m_ptr = 0;
        }
        
        private void deleteArray() {
            if (isNull())
                return;
            deletePointer(m_ptr, m_type.ordinal(), 2);
            m_ptr = 0;
        }
        
        boolean isNull() {
            return m_ptr == 0;
        }
        
        void reset() {
        	m_ptr = 0;
        }
    }
    
    private static class BufferData extends Data{
    	private final long m_bufferAccess;
    	private final Buffer buffer;
    	
    	BufferData(long m_bufferAccess, Buffer buffer) {
			this.m_bufferAccess = m_bufferAccess;
			this.buffer = buffer;
			m_autodelete = AutoDeleteMode.Delete;
		}
		void dispose() {
            if (isNull())
                return;
        	invalidateDependentObjects();
        	deleteBufferAccess(m_bufferAccess, buffer.isReadOnly());
        	m_ptr = 0;
            m_autodelete = AutoDeleteMode.None;
            m_isInvalid = true;
    	}
    }
    
    /**
     * This exception is thrown when trying to write on a read-only native pointer.
     */
    public static class ReadOnlyNativePointerException extends UnsupportedOperationException{
		private static final long serialVersionUID = -8238568644716457250L;
    }
}
