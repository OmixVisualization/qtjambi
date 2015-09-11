/****************************************************************************
**
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

class QSGGeometry.AttributeSet__{
	public AttributeSet(Attribute[] attributes, int stride){
		setStride(stride);
		setCount(attributes.length);
		QNativePointer np = new QNativePointer(QNativePointer.Type.Pointer, attributes.length);
		for(int i=0; i<attributes.length; i++){
			if(attributes[i]!=null){
				np.setPointerAt(i, attributes[i].nativePointer());
			}
		}
	}
	
	public AttributeSet(java.util.List<Attribute> attributes, int stride){
		setStride(stride);
		setCount(attributes.size());
		QNativePointer np = new QNativePointer(QNativePointer.Type.Pointer, attributes.size());
		for(int i=0; i<attributes.size(); i++){
			if(attributes.get(i)!=null){
				np.setPointerAt(i, attributes.get(i).nativePointer());
			}
		}
	}
	
	@QtBlockedSlot
    public final Attribute[] attributes()    {
    	Attribute[] result = new Attribute[count()];
    	org.qtjambi.qt.QNativePointer np = attributes_private();
    	for (int i = 0; i < result.length; i++) {
			result[i] = Attribute.fromNativePointer(np.pointerValue());
		}
    	return result;
    }
}// class

class QSGGeometry__{
	@QtBlockedSlot
    public final Attribute[] attributes()    {
    	Attribute[] result = new Attribute[attributeCount()];
    	org.qtjambi.qt.QNativePointer np = attributes_private();
    	for (int i = 0; i < result.length; i++) {
			result[i] = Attribute.fromNativePointer(np.pointerValue());
		}
    	return result;
    }
	
	@QtBlockedSlot
    public final ColoredPoint2D[] vertexDataAsColoredPoint2D()    {
    	ColoredPoint2D[] result = new ColoredPoint2D[vertexCount()];
    	org.qtjambi.qt.QNativePointer np = vertexDataAsColoredPoint2D_private();
    	for (int i = 0; i < result.length; i++) {
			result[i] = ColoredPoint2D.fromNativePointer(np.pointerValue());
		}
    	return result;
    }
	
	@QtBlockedSlot
    public final TexturedPoint2D[] vertexDataAsTexturedPoint2D()    {
    	TexturedPoint2D[] result = new TexturedPoint2D[vertexCount()];
    	org.qtjambi.qt.QNativePointer np = vertexDataAsTexturedPoint2D_private();
    	for (int i = 0; i < result.length; i++) {
			result[i] = TexturedPoint2D.fromNativePointer(np.pointerValue());
		}
    	return result;
    }
	
	@QtBlockedSlot
    public final Point2D[] vertexDataAsPoint2D()    {
    	Point2D[] result = new Point2D[vertexCount()];
    	org.qtjambi.qt.QNativePointer np = vertexDataAsPoint2D_private();
    	for (int i = 0; i < result.length; i++) {
			result[i] = Point2D.fromNativePointer(np.pointerValue());
		}
    	return result;
    }
	
	@QtBlockedSlot
    public final Number[] indexData()    {
    	Number[] result = new Number[indexCount()];
    	org.qtjambi.qt.QNativePointer np = indexData_private();
		switch(sizeOfIndex()){
		case 1:
	    	for (int i = 0; i < result.length; i++) {
				result[i] = np.byteAt(i);
	    	}
	    	break;
		case 2:
	    	for (int i = 0; i < result.length; i++) {
				result[i] = np.shortAt(i);
	    	}
	    	break;
		default:
	    	for (int i = 0; i < result.length; i++) {
				result[i] = np.intAt(i);
	    	}
	    	break;
		}
    	return result;
    }
}// class

class QQuickItem__
}// class

