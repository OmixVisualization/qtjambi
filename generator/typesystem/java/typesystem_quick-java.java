/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    public AttributeSet(java.util.Collection<Attribute> attributes, int stride){
        this(attributes.toArray(new Attribute[attributes.size()]), stride);
    }
}// class

class QSGGeometry__{
    public VertexData<?> vertexData(){
        return __qt_QSGGeometry_vertexData(io.qt.internal.QtJambiInternal.checkedNativeId(this));        
    }

    private native VertexData<?> __qt_QSGGeometry_vertexData(long __this__nativeId);

    private static native ColoredPoint2D getColoredPoint2D(long pointer, int index);

    private static native void setColoredPoint2D(long pointer, int index, ColoredPoint2D value);

    private static native TexturedPoint2D getTexturedPoint2D(long pointer, int index);

    private static native void setTexturedPoint2D(long pointer, int index, TexturedPoint2D value);

    private static native Point2D getPoint2D(long pointer, int index);

    private static native void setPoint2D(long pointer, int index, Point2D value);

    public static class VertexData<T extends io.qt.QtObjectInterface>{
        public VertexData(long pointer, int size) {
            super();
            this.pointer = pointer;
            this.size = size;
        }
        
        final long pointer;
        final int size;

        @io.qt.QtUninvokable
        public T get(int index) {
            throw new UnsupportedOperationException();
        }
        
        @io.qt.QtUninvokable
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

class NATIVE_QSGGeometry__{

// QSGGeometry::vertexData() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry__1_1qt_1QSGGeometry_1vertexData__J)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSGGeometry::vertexData() const")
    try{
        QSGGeometry_shell *__qt_this = qtjambi_object_from_nativeId<QSGGeometry_shell>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        jlong __qt_return_value = jlong(__qt_this->vertexData());

        jclass cls;
        jmethodID constructor;
        if(__qt_this->attributeCount() == 1
                && __qt_this->sizeOfVertex() == 2 * sizeof(float)
                && __qt_this->attributes()[0].tupleSize == 2
                && __qt_this->attributes()[0].type == QSGGeometry::FloatType
                && __qt_this->attributes()[0].attributeType == QSGGeometry::PositionAttribute
                && __qt_this->attributes()[0].position == 0)
        {
            //Point2D*
            static jclass Point2DVertexData_class = nullptr;
            static jmethodID Point2DVertexData_constructor = nullptr;
            if(Point2DVertexData_class==nullptr){
                Point2DVertexData_class = __jni_env->FindClass("io/qt/quick/QSGGeometry$Point2DVertexData");
                if(Point2DVertexData_class!=nullptr){
                    Point2DVertexData_class = jclass(__jni_env->NewGlobalRef(Point2DVertexData_class));
                    Point2DVertexData_constructor = __jni_env->GetMethodID(Point2DVertexData_class, "<init>", "(JI)V");
                }
            }
            cls = Point2DVertexData_class;
            constructor = Point2DVertexData_constructor;
        }
        else if(__qt_this->attributeCount() == 2
                && __qt_this->sizeOfVertex() == 4 * sizeof(float)
                && __qt_this->attributes()[0].tupleSize == 2
                && __qt_this->attributes()[0].type == QSGGeometry::FloatType
                && __qt_this->attributes()[0].attributeType == QSGGeometry::PositionAttribute
                && __qt_this->attributes()[0].position == 0
                && __qt_this->attributes()[1].tupleSize == 2
                && __qt_this->attributes()[1].type == QSGGeometry::FloatType
                && __qt_this->attributes()[1].attributeType == QSGGeometry::TexCoordAttribute
                && __qt_this->attributes()[1].position == 1)
        {
            // TexturedPoint2D*
            static jclass TexturedPoint2DVertexData_class = nullptr;
            static jmethodID TexturedPoint2DVertexData_constructor = nullptr;
            if(TexturedPoint2DVertexData_class==nullptr){
                TexturedPoint2DVertexData_class = __jni_env->FindClass("io/qt/quick/QSGGeometry$TexturedPoint2DVertexData");
                if(TexturedPoint2DVertexData_class!=nullptr){
                    TexturedPoint2DVertexData_class = jclass(__jni_env->NewGlobalRef(TexturedPoint2DVertexData_class));
                    TexturedPoint2DVertexData_constructor = __jni_env->GetMethodID(TexturedPoint2DVertexData_class, "<init>", "(JI)V");
                }
            }
            cls = TexturedPoint2DVertexData_class;
            constructor = TexturedPoint2DVertexData_constructor;
        }
        else if(__qt_this->attributeCount() == 2
                && __qt_this->sizeOfVertex() == 2 * sizeof(float) + 4 * sizeof(char)
                && __qt_this->attributes()[0].tupleSize == 2
                && __qt_this->attributes()[0].type == QSGGeometry::FloatType
                && __qt_this->attributes()[0].attributeType == QSGGeometry::PositionAttribute
                && __qt_this->attributes()[0].position == 0
                && __qt_this->attributes()[1].tupleSize == 4
                && __qt_this->attributes()[1].type == QSGGeometry::UnsignedByteType
                && __qt_this->attributes()[1].attributeType == QSGGeometry::ColorAttribute
                && __qt_this->attributes()[1].position == 1)
        {
            // ColoredPoint2D*
            static jclass ColoredPoint2DVertexData_class = nullptr;
            static jmethodID ColoredPoint2DVertexData_constructor = nullptr;
            if(ColoredPoint2DVertexData_class==nullptr){
                ColoredPoint2DVertexData_class = __jni_env->FindClass("io/qt/quick/QSGGeometry$ColoredPoint2DVertexData");
                if(ColoredPoint2DVertexData_class!=nullptr){
                    ColoredPoint2DVertexData_class = jclass(__jni_env->NewGlobalRef(ColoredPoint2DVertexData_class));
                    ColoredPoint2DVertexData_constructor = __jni_env->GetMethodID(ColoredPoint2DVertexData_class, "<init>", "(JI)V");
                }
            }
            cls = ColoredPoint2DVertexData_class;
            constructor = ColoredPoint2DVertexData_constructor;
        }
        else
        {
            // unknown
            static jclass VertexData_class = nullptr;
            static jmethodID VertexData_constructor = nullptr;
            if(VertexData_class==nullptr){
                VertexData_class = __jni_env->FindClass("io/qt/quick/QSGGeometry$VertexData");
                if(VertexData_class!=nullptr){
                    VertexData_class = jclass(__jni_env->NewGlobalRef(VertexData_class));
                    VertexData_constructor = __jni_env->GetMethodID(VertexData_class, "<init>", "(JI)V");
                }
            }
            cls = VertexData_class;
            constructor = VertexData_constructor;
        }

        Q_ASSERT(cls);
        Q_ASSERT(constructor);
        jobject __java_return_value = __jni_env->NewObject(cls, constructor, __qt_return_value, jint(__qt_this->vertexCount()));
        qtjambi_throw_java_exception(__jni_env)
        return __java_return_value;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getColoredPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    try{
        QSGGeometry::ColoredPoint2D* vertexData = reinterpret_cast<QSGGeometry::ColoredPoint2D*>(pointer);
        return qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getTexturedPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    try{
        QSGGeometry::TexturedPoint2D* vertexData = reinterpret_cast<QSGGeometry::TexturedPoint2D*>(pointer);
        return qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    try{
        QSGGeometry::Point2D* vertexData = reinterpret_cast<QSGGeometry::Point2D*>(pointer);
        return qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setColoredPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    try{
        QSGGeometry::ColoredPoint2D* vertexData = reinterpret_cast<QSGGeometry::ColoredPoint2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::ColoredPoint2D>(__jni_env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setTexturedPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    try{
        QSGGeometry::TexturedPoint2D* vertexData = reinterpret_cast<QSGGeometry::TexturedPoint2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::TexturedPoint2D>(__jni_env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    try{
        QSGGeometry::Point2D* vertexData = reinterpret_cast<QSGGeometry::Point2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::Point2D>(__jni_env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

}// class

class QQuickItem__
}// class

