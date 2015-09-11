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

package org.qtjambi.qt;

/**
 *   QNativePointer encapsulates a native C++ pointer. The class
 *   provides the functionality that you would get if you had direct
 *   access to the pointer through function calls. It is as such a
 *   low-level memory manager that should be used sparsely; its
 *   intended and legitimate use is for JNI bindings not handled by
 *   generator. Examples can be found in the
 *   <a href="qtjambi-generatorexample.html">generator example</a>.
 *   <p>
 *   QNativePointer does type checking of pointers. Also, if the
 *   pointer is pointing to an array, you must also specify the array
 *   length; array bounds checking is enforced by QNativePointer. Any
 *   number of indirections are allowed (i.e., arrays can have any
 *   number of dimensions).
 *   <p>
 *   The QNativePointer will by default delete the internal pointer
 *   when being garbage collected. However, if the ownership of the
 *   pointer is given to a c++ class, you do not want this behavior.
 *   The AutoDeleteMode enum values defines the ways in which
 *   deletion of the pointer can be handled.
 *   <p>
 *   The data types that can be pointed to are defined by the Type
 *   enum. An allocation of an Integer pointer can, for example, be
 *   done like this:
 *   <p>
 *   <code>
 *      QNativePointer ptr =
 *           new QNativePointer(QNativePointer.Type.Int);<br>
 *      ptr.setIntValue(10);<br>
 *   </code>
 *   <p>
 *   An array of length 5 is created in the following way:
 *   <p>
 *   <code>
 *      QNativePointer ptr = new QNativePointer(QNativePointer.Type.Int, 5);<br>
 *      for (int i = 0; i  &lt; 5, ++i)
 *      <pre>    ptr.setIntAt(i, i*i);</pre><br>
 *   </code>
 *   <p>
 *   If you are creating a multi dimensional array, you have two
 *   possibilities. You can make QNativePointers of the Pointer type or
 *   specify the number indirections of a single QNativePointer.
 *   We recommend the second alternative
 *   since it creates type safe pointers. Here is an example using the first alternative:
 *   <p>
 *   <code>
 *      QNativePointer ptr = new QNativePointer(QNativePointer.Type.Pointer, 2);<br>
 *      QNativePointer charArray1 = new QNativePointer(QNativePointer.Type.Char, 5);<br>
 *      ptr.setPointerAt(0, carArray1);
 *   </code>
 *   <p>
 *   And here is the code for the second:
 *   <p>
 *   <code>
 *      QNativePointer ptr = new QNativePointer(Type.Char, 5, 2);<br>
 *      ptr.setPointerAt(0, createCharPointer(myString));
 *   </code>
 */
public class QNativePointer {
    static {
        QtJambi_LibraryInitializer.init();
    }

    // Keep this in sync with the values in common/nativepointer.h
    /**
     * The Type enum describe the Java types that can be used by
     * a QNativePointer.
     *
     */
    public enum Type {
        /** Java Boolean*/ Boolean,
        /** Java Byte*/ Byte,
        /** Java Char*/ Char,
        /** Java Short*/ Short,
        /** Java Int*/ Int,
        /** Java Long*/ Long,
        /** Java Float*/ Float,
        /** Java Double*/ Double,
        /** Another QNativePointer of any type*/ Pointer,
        /** Java String*/ String
    }

    /**
     * The AutoDeleteMode enum describes how garbage collection of the
     * QNativePointer handles the deletion of the native pointer. By default,
     * the mode is set to Delete or DeleteArray.
     */
    public enum AutoDeleteMode {
        /**
         * Free must be used if the pointer was allocated using c++ <code>free()</code>.
         * Currently, no pointers in Qt Jambi is allocated in this manner.
         */
        Free,
        /**
         * The pointer is allocated with c++ <code>new</code>.
         * This should be used for all pointers in Qt Jambi
         */
        Delete,
        /**
         * This must be the mode of deletion if the pointer is an array.
         */
        DeleteArray,
        /**
         * Use this deletion mode if you do not want to delete the c++ pointer
         * when the QNativePointer object is garbage collected.
         */
        None
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
    public QNativePointer(Type type, int size) {
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
    public QNativePointer(Type type, int size, int indirections) {
        if (indirections < 1)
            throw new IllegalArgumentException("level of indirection must be at least 1");
        if (size == 0)
            throw new IllegalArgumentException("size must be at least 1");
        m_ptr = createPointer(type.ordinal(), size, indirections);
        m_type = type;
        m_knownSize = size;
        m_indirections = indirections;
        m_autodelete = size == 1 ? AutoDeleteMode.Delete : AutoDeleteMode.DeleteArray;
    }

    private QNativePointer(int type, int size, int indirections) {
        this(typeOf(type), size, indirections);
    }

    private QNativePointer() {
        m_knownSize = -1;
        m_autodelete = AutoDeleteMode.None;
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
    public boolean booleanAt(int pos) {
        verifyAccess(Type.Boolean, pos);
        return readBoolean(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public byte byteAt(int pos) {
        verifyAccess(Type.Byte, pos);
        return readByte(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public char charAt(int pos) {
        verifyAccess(Type.Char, pos);
        return readChar(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public short shortAt(int pos) {
        verifyAccess(Type.Short, pos);
        return readShort(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public int intAt(int pos) {
        verifyAccess(Type.Int, pos);
        return readInt(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public long longAt(int pos) {
        verifyAccess(Type.Long, pos);
        return readLong(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public float floatAt(int pos) {
        verifyAccess(Type.Float, pos);
        return readFloat(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public double doubleAt(int pos) {
        verifyAccess(Type.Double, pos);
        return readDouble(m_ptr, pos);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public QNativePointer pointerAt(int pos) {
        verifyAccess(Type.Pointer, pos);
        long ptr = readPointer(m_ptr, pos);
        return fromNative(ptr, m_type, m_indirections - 1);
    }

    /**
     * Returns the value of the native pointer at the specified
     * position. If <code>pos</code> is larger than 1, QNativePointer
     * will check that the position is within the array bounds.
     *
     * @param pos the array index
     */
    public String stringAt(int pos) {
        verifyAccess(Type.String, pos);
        return readString(m_ptr, pos);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setBooleanAt(int pos, boolean value) {
        verifyAccess(Type.Boolean, pos);
        writeBoolean(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setByteAt(int pos, byte value) {
        verifyAccess(Type.Byte, pos);
        writeByte(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setCharAt(int pos, char value) {
        verifyAccess(Type.Char, pos);
        writeChar(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setShortAt(int pos, short value) {
        verifyAccess(Type.Short, pos);
        writeShort(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setIntAt(int pos, int value) {
        verifyAccess(Type.Int, pos);
        writeInt(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setLongAt(int pos, long value) {
        verifyAccess(Type.Long, pos);
        writeLong(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setFloatAt(int pos, float value) {
        verifyAccess(Type.Float, pos);
        writeFloat(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setDoubleAt(int pos, double value) {
        verifyAccess(Type.Double, pos);
        writeDouble(m_ptr, pos, value);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setPointerAt(int pos, QNativePointer value) {
        verifyAccess(Type.Pointer, pos);
        if(value != null)
            value.setAutoDeleteMode(AutoDeleteMode.None);
        writePointer(m_ptr, pos, value == null ? 0 : value.m_ptr);
    }

    /**
     *  Sets the value of the array element at <code>pos</code> to
     *  which this native pointer points.
     *
     *  @param pos the array index
     *  @param value the value to set the index to
     */
    public void setStringAt(int pos, String value) {
        verifyAccess(Type.String, pos);
        writeString(m_ptr, pos, value);
    }

    /**
     * Returns the type of the native pointer.
     *
     * @return the data type of the native pointer
     */
    public Type type() {
        return m_type;
    }

    /**
     * Returns the number of indirections of the pointer.
     *
     * @return the number of indirections of the pointer
     */
    public int indirections() {
        return m_indirections;
    }

    /**
     *  Returns true if the native pointer is 0; otherwise false.
     *
     * @return true if the native pointer is 0.
     */
    public boolean isNull() {
        return m_ptr == 0;
    }

    /**
     * Returns the auto-delete mode of the pointer.
     *
     * @return the auto-delete mode of this QNativePointer
     */
    public AutoDeleteMode autoDeleteMode() {
        return m_autodelete;
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
        m_autodelete = autodelete;
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
        if (isNull())
            return;
        deletePointer(m_ptr, m_type.ordinal(), 0);
        m_ptr = 0;
    }

    /**
     * This function deletes the internal pointer.
     *
     * After the pointer has been deleted, you cannot allocate it
     * again. The pointer is by default deleted when the
     * QNativePointer object is garbage collected.
     */
    public void delete() {
        if (isNull())
            return;
        deletePointer(m_ptr, m_type.ordinal(), 1);
        m_ptr = 0;
    }

    /**
     * This function deletes elements in the array of this
     * QNativePointer.
     *
     * After the pointer has been deleted, you cannot use this
     * QNativePointer object again.
     */
    public void deleteArray() {
        if (isNull())
            return;
        deletePointer(m_ptr, m_type.ordinal(), 2);
        m_ptr = 0;
    }

    /**
     * Returns the native pointer.
     *
     * The returned long is the <code>void *</code> value in c++.
     *
     * @return the native pointer.
     */
    public long pointer() {
        return m_ptr;
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
    public static QNativePointer fromNative(long ptr, Type type, int indirections) {
        QNativePointer nativePointer = new QNativePointer();
        nativePointer.m_ptr = ptr;
        nativePointer.m_type = type;
        nativePointer.m_indirections = indirections;
        return nativePointer;
    }

    /**
     * Returns if verification is enabled or not.
     *
     * @return true if verification is enabled; otherwise false.
     */
    public boolean verificationEnabled() {
        return m_verification_enabled;
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
        m_verification_enabled = a;
    }

    private static Type typeOf(int type) {
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
        }
        throw new IllegalArgumentException("Unknown type id: " + type);
    }

    /**
     * This is an overloaded function provided for convenience.
     * @see #fromNative(long ptr, Type type, int indirections)
     */
    public static QNativePointer fromNative(long ptr, int type, int indirections) {
        return fromNative(ptr, typeOf(type), indirections);
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
        QNativePointer s = new QNativePointer(QNativePointer.Type.Byte, string.length() + 1);
        for (int i = 0; i < string.length(); ++i)
            s.setByteAt(i, (byte) string.charAt(i));
        s.setByteAt(string.length(), (byte) 0);
        return s;
    }

    /*
     * This is doc in superclass
     */
    @Override
    protected void finalize() {
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
        }
    }

    private void verifyAccess(Type type, int pos) {
        if (!m_verification_enabled)
            return;

        if (isNull())
            throw new NullPointerException("native pointer is null");

        if (pos < 0)
            throw new IndexOutOfBoundsException("negative index: " + pos);

        if (m_knownSize >= 0 && pos >= m_knownSize)
            throw new IndexOutOfBoundsException("size: " + m_knownSize + ", access at: " + pos);

        if (m_indirections > 1) {
            if (type != Type.Pointer)
                throw new ClassCastException("accessing pointer with " + m_indirections
                        + " levels of indirection as " + type);
        } else if (type != m_type) {
            throw new ClassCastException("type: " + m_type + ", accessed as: " + type);
        }
    }

    private static native boolean readBoolean(long ptr, int pos);
    private static native byte readByte(long ptr, int pos);
    private static native char readChar(long ptr, int pos);
    private static native short readShort(long ptr, int pos);
    private static native int readInt(long ptr, int pos);
    private static native long readLong(long ptr, int pos);
    private static native float readFloat(long ptr, int pos);
    private static native double readDouble(long ptr, int pos);
    private static native long readPointer(long ptr, int pos);
    private static native String readString(long ptr, int pos);
    private static native void writeBoolean(long ptr, int pos, boolean value);
    private static native void writeByte(long ptr, int pos, byte value);
    private static native void writeChar(long ptr, int pos, char value);
    private static native void writeShort(long ptr, int pos, short value);
    private static native void writeInt(long ptr, int pos, int value);
    private static native void writeLong(long ptr, int pos, long value);
    private static native void writeFloat(long ptr, int pos, float value);
    private static native void writeDouble(long ptr, int pos, double value);
    private static native void writePointer(long ptr, int pos, long value);
    private static native void writeString(long ptr, int pos, String value);
    private static native long createPointer(int type, int size, int indirections);
    private static native void deletePointer(long ptr, int type, int deleteMode);

    private long m_ptr;
    private Type m_type;
    private int m_knownSize;
    private AutoDeleteMode m_autodelete;
    private int m_indirections;

    private boolean m_verification_enabled = true;
}
