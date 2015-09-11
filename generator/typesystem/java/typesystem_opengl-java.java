/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package generator;

import org.qtjambi.qt.*;
import org.qtjambi.qt.opengl.*;

class QGLColormap___ extends QGLColormap {

        public final void setEntries(int colors[], int base) {
            setEntries(colors.length, org.qtjambi.qt.internal.QtJambiInternal.intArrayToNativePointer(colors), base);
        }

        public final void setEntries(int colors[]) {
            setEntries(colors.length, org.qtjambi.qt.internal.QtJambiInternal.intArrayToNativePointer(colors));
        }

}// class

class QGLShaderProgram___ extends QGLColormap {
		
		@QtBlockedSlot
		public final void setAttributeValue(java.lang.String name, float[] values, int columns, int rows)    {
			this.setAttributeValue_GLfloat(name, floatsToNativePointer(values), columns, rows);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, float[] values, int tupleSize) {
			setAttributeArray(location, values, tupleSize, (int)0);
		}
		@QtBlockedSlot
		public final void setAttributeArray(int location, float[] values, int tupleSize, int stride) {
			setAttributeArray_GLfloat(location, floatsToNativePointer(values), tupleSize, stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, float[] values, int tupleSize) {
			setAttributeArray(name, values, tupleSize, (int)0);
		}
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, float[] values, int tupleSize, int stride) {
			setAttributeArray_GLfloat(name, floatsToNativePointer(values), tupleSize, stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector2D[] values)    {
			this.setAttributeArray(location, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector2D[] values, int stride)    {
			setAttributeArray_QVector2D(location, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector2D[] values)    {
			this.setAttributeArray(name, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector2D[] values, int stride)    {
			setAttributeArray_QVector2D(name, arrayToNativePointer(values), stride);
		}
    
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector3D[] values)    {
			this.setAttributeArray(location, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector3D[] values, int stride)    {
			setAttributeArray_QVector3D(location, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector4D[] values)    {
			this.setAttributeArray(location, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector4D[] values, int stride)    {
			setAttributeArray_QVector4D(location, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector3D[] values)    {
			this.setAttributeArray(name, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector3D[] values, int stride)    {
			setAttributeArray_QVector3D(name, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector4D[] values)    {
			this.setAttributeArray(name, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector4D[] values, int stride)    {
			setAttributeArray_QVector4D(name, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, float[] values, int count, int tupleSize)    {
			this.setUniformValueArray_GLfloat(location, this.floatsToNativePointer(values), count, tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, float[] values, int count, int tupleSize)    {
			this.setUniformValueArray_GLfloat(name, this.floatsToNativePointer(values), count, tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, int[] values, int tupleSize)    {
			this.setUniformValueArray_int(location, org.qtjambi.qt.internal.QtJambiInternal.intArrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, int[] values, int tupleSize)    {
			this.setUniformValueArray_int(name, org.qtjambi.qt.internal.QtJambiInternal.intArrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix2x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x2(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix2x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x2(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix2x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x3(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix2x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x3(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix2x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x4(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix2x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x4(name, arrayToNativePointer(values), tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix3x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x2(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix3x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x2(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix3x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x3(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix3x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x3(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix3x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x4(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix3x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x4(name, arrayToNativePointer(values), tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix4x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x2(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix4x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x2(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix4x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x3(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix4x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x3(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix4x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x4(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix4x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x4(name, arrayToNativePointer(values), tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QVector2D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector2D(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QVector2D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector2D(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QVector3D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector3D(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QVector3D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector3D(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QVector4D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector4D(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QVector4D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector4D(name, arrayToNativePointer(values), tupleSize);
		}
		
		private org.qtjambi.qt.QNativePointer floatsToNativePointer(float[] values){
			org.qtjambi.qt.QNativePointer pointer = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Float, values.length);
			for (int i = 0; i < values.length; i++) {
				pointer.setFloatAt(i, values[i]);
			}
			return pointer;
		}
		
		private <T extends org.qtjambi.qt.internal.QtJambiObject> org.qtjambi.qt.QNativePointer arrayToNativePointer(T[] values){
			org.qtjambi.qt.QNativePointer pointer = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Pointer, values.length);
			for (int i = 0; i < values.length; i++) {
				pointer.setPointerAt(i, values[i].nativePointer());
			}
			return pointer;
		}
		
}// class

