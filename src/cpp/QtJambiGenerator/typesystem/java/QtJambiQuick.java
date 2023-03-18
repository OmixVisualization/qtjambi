/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

class QSGGeometry.AttributeSet__{
    public AttributeSet(java.util.Collection<Attribute> attributes, int stride){
        this(attributes.toArray(new Attribute[attributes.size()]), stride);
    }
}// class

class QSGGeometry__{
    public native VertexData<?> vertexData();

    private static native ColoredPoint2D getColoredPoint2D(long pointer, int index);

    private static native void setColoredPoint2D(long pointer, int index, ColoredPoint2D value);

    private static native TexturedPoint2D getTexturedPoint2D(long pointer, int index);

    private static native void setTexturedPoint2D(long pointer, int index, TexturedPoint2D value);

    private static native Point2D getPoint2D(long pointer, int index);

    private static native void setPoint2D(long pointer, int index, Point2D value);

    public static class VertexData<T extends QtObjectInterface>{
        public VertexData(long pointer, int size) {
            super();
            this.pointer = pointer;
            this.size = size;
        }
        
        final long pointer;
        final int size;

        @QtUninvokable
        public T get(int index) {
            throw new UnsupportedOperationException();
        }
        
        @QtUninvokable
        public void set(int index, T value) {
            throw new UnsupportedOperationException();
        }
    }

    public static class ColoredPoint2DVertexData extends VertexData<ColoredPoint2D>{
        public ColoredPoint2DVertexData(long pointer, int size) {
            super(pointer, size);
        }

        @Override
        public ColoredPoint2D get(int index) {
            if(index<0 || index>=size)
                throw new ArrayIndexOutOfBoundsException();
            return getColoredPoint2D(pointer, index);
        }

        @Override
        public void set(int index, ColoredPoint2D value) {
            if(index<0 || index>=size)
                throw new ArrayIndexOutOfBoundsException();
            setColoredPoint2D(pointer, index, value);
        }
    }

    public static class TexturedPoint2DVertexData extends VertexData<TexturedPoint2D>{
        public TexturedPoint2DVertexData(long pointer, int size) {
            super(pointer, size);
        }

        @Override
        public TexturedPoint2D get(int index) {
            if(index<0 || index>=size)
                throw new ArrayIndexOutOfBoundsException();
            return getTexturedPoint2D(pointer, index);
        }

        @Override
        public void set(int index, TexturedPoint2D value) {
            if(index<0 || index>=size)
                throw new ArrayIndexOutOfBoundsException();
            setTexturedPoint2D(pointer, index, value);
        }
    }

    public static class Point2DVertexData extends VertexData<Point2D>{
        public Point2DVertexData(long pointer, int size) {
            super(pointer, size);
        }

        @Override
        public Point2D get(int index) {
            if(index<0 || index>=size)
                throw new ArrayIndexOutOfBoundsException();
            return getPoint2D(pointer, index);
        }

        @Override
        public void set(int index, Point2D value) {
            if(index<0 || index>=size)
                throw new ArrayIndexOutOfBoundsException();
            setPoint2D(pointer, index, value);
        }
    }
}// class

