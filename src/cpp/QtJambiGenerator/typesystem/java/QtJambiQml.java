/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
**
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

class __QQmlComponent extends QQmlComponent{

    @io.qt.QtUninvokable
    public final void setData(String data, io.qt.core.QUrl baseUrl){
        setData(new io.qt.core.QByteArray(data), baseUrl);
    }

    @io.qt.QtUninvokable
    public final void setData(byte[] data, io.qt.core.QUrl baseUrl){
        setData(new io.qt.core.QByteArray(data), baseUrl);
    }
    
    @io.qt.QtUninvokable
    public <T extends io.qt.core.QObject> T create(Class<T> type){
        return create(type, (io.qt.qml.QQmlContext)null);
    }

    @io.qt.QtUninvokable
    public <T extends io.qt.core.QObject> T create(Class<T> type, io.qt.qml.QQmlContext context){
        String packageName = type.getName();
        int idx = packageName.lastIndexOf('.');
        if(idx>0){
            packageName = packageName.substring(0, idx);
        }
        io.qt.QtUtilities.initializePackage(packageName);
        return type.cast(create(context));
    }
}// class

class __QJSValue extends QJSValue{

    @io.qt.QtUninvokable
    public final void setProperty(java.lang.String name, String value){
        setProperty(name, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(java.lang.String name, boolean value){
        setProperty(name, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(java.lang.String name, double value){
        setProperty(name, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(java.lang.String name, int value){
        setProperty(name, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(java.lang.String name, QJSValue.SpecialValue value){
        setProperty(name, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(int arrayIndex, String value){
        setProperty(arrayIndex, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(int arrayIndex, boolean value){
        setProperty(arrayIndex, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(int arrayIndex, double value){
        setProperty(arrayIndex, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(int arrayIndex, int value){
        setProperty(arrayIndex, new QJSValue(value));
    }
    
    @io.qt.QtUninvokable
    public final void setProperty(int arrayIndex, QJSValue.SpecialValue value){
        setProperty(arrayIndex, new QJSValue(value));
    }

}// class

class __QtQml__ extends QtQml{
    
    private static final int QQmlModuleImportLatest = -1;
    
    /**
     * @deprecated Use {@link #qmlRegisterSingletonInstance(String, int, int, String, QObject)} instead.
     */
    @Deprecated
    public static int qmlRegisterSingletonInstance(QObject instance, String uri, int versionMajor, int versionMinor, String qmlName){
        java.util.Objects.requireNonNull(instance);
        return qmlRegisterSingletonType(QtJambi_LibraryUtilities.internal.getClass(instance), uri, versionMajor, versionMinor, qmlName, (e,v)->instance);
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qqmlengine.html#qmlRegisterSingletonInstance">qmlRegisterSingletonInstance(const char*,int,int,const char*,QObject*)</a></p>
     */
    public static int qmlRegisterSingletonInstance(String uri, int versionMajor, int versionMinor, String qmlName, QObject instance){
        java.util.Objects.requireNonNull(instance);
        return qmlRegisterSingletonType(QtJambi_LibraryUtilities.internal.getClass(instance), uri, versionMajor, versionMinor, qmlName, (e,v)->instance);
    }

    /**
     * @deprecated Use {@link #qmlRegisterUncreatableType(Class, int, String, int, int, String, String)} instead.
     */
    @Deprecated
    public static int qmlRegisterUncreatableClass(java.lang.Class<? extends io.qt.core.QObject> type, int metaObjectRevision, java.lang.String uri, int versionMajor, int versionMinor, java.lang.String qmlName, java.lang.String reason) {
        return qmlRegisterUncreatableType(type, metaObjectRevision, uri, versionMajor, versionMinor, qmlName, reason);
    }
    /**
     * @deprecated Use {@link #qmlRegisterUncreatableType(Class, String, int, int, String, String)} instead.
     */
    @Deprecated
    public static int qmlRegisterUncreatableClass(java.lang.Class<? extends io.qt.core.QObject> type, java.lang.String uri, int versionMajor, int versionMinor, java.lang.String qmlName, java.lang.String reason) {
        return qmlRegisterUncreatableType(type, uri, versionMajor, versionMinor, qmlName, reason);
    }
}// class

class QQmlProperty__{
    
    @io.qt.QtUninvokable
    public final boolean connectNotifySignal(io.qt.core.QMetaObject.Slot0 slot){
        return connectNotifySignal(slot, io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot)));
    }
    
    @io.qt.QtUninvokable
    public final boolean connectNotifySignal(io.qt.core.QMetaObject.Slot1<?> slot){
        return connectNotifySignal(slot, io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot)));
    }
    
    @io.qt.QtUninvokable
    private final boolean connectNotifySignal(io.qt.core.QMetaObject.AbstractSlot slot, io.qt.core.QMetaMethod metaMethod){
        if(metaMethod!=null && metaMethod.isValid()) {
            io.qt.core.QObject qobject = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
            if(qobject!=null)
                return connectNotifySignal(qobject, metaMethod.methodIndex());
        }
        return false;
    }
    
}// class

class QJSEngine__{

    @io.qt.QtUninvokable
    public final QJSValue newQMetaObject(Class<?> type) {
        return newQMetaObject(io.qt.core.QMetaObject.forType(type));
    }
}// class

class QJSEngine_6_{
    @io.qt.QtUninvokable
    public final <T> T fromScriptValue(QJSValue value, Class<T> type, io.qt.core.QMetaType...instantiations) {
        return fromScriptValue(value, io.qt.core.QMetaType.fromType(type, instantiations));
    }
    
    @io.qt.QtUninvokable
    public final <T> T fromScriptValue(QJSValue value, io.qt.core.QMetaType type) {
        return fromScriptValue(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.checkedNativeId(value), QtJambi_LibraryUtilities.internal.checkedNativeId(type));
    }
    
    private static native <T> T fromScriptValue(long thisId, long valueId, long typeId);
    
    @io.qt.QtUninvokable
    public final <T> QJSValue toScriptValue(T value) {
        return toScriptValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    }
    
    private static native QJSValue toScriptValue(long thisId, Object value);
}// class

class QJSEngine_61_{
    @io.qt.QtUninvokable
    public final <T> T fromManagedValue(QJSManagedValue value, Class<T> type, io.qt.core.QMetaType...instantiations) {
        return fromManagedValue(value, io.qt.core.QMetaType.fromType(type, instantiations));
    }
    
    @io.qt.QtUninvokable
    public final <T> T fromManagedValue(QJSManagedValue value, io.qt.core.QMetaType type) {
        return fromManagedValue(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.checkedNativeId(value), QtJambi_LibraryUtilities.internal.checkedNativeId(type));
    }
    
    private static native <T> T fromManagedValue(long thisId, long valueId, long typeId);
    
    @io.qt.QtUninvokable
    public final <T> QJSManagedValue toManagedValue(T value) {
        return toManagedValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    }
    
    private static native QJSManagedValue toManagedValue(long thisId, Object value);
    
}// class

class QJSEngine_63_{
    @io.qt.QtUninvokable
    public final <T> T fromVariant(Object variant, Class<T> type, io.qt.core.QMetaType...instantiations) {
        return fromVariant(variant, io.qt.core.QMetaType.fromType(type, instantiations));
    }
    
    @io.qt.QtUninvokable
    public final <T> T fromVariant(Object variant, io.qt.core.QMetaType type) {
        return fromVariant(QtJambi_LibraryUtilities.internal.nativeId(this), variant, QtJambi_LibraryUtilities.internal.checkedNativeId(type));
    }
    
    private static native <T> T fromVariant(long thisId, Object variant, long typeId);
    
}// class

class QQmlEngine__{
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qqmlengine.html#singletonInstance">QQmlEngine::singletonInstance(int qmlTypeId)</a></p>
     */
    @io.qt.QtUninvokable
    public final <T extends io.qt.QtObjectInterface> T singletonInstance(Class<T> type, int qmlTypeId) {
        io.qt.core.QObject object = singletonInstance(QtJambi_LibraryUtilities.internal.nativeId(this), qmlTypeId).toQObject();
        if(object!=null)
            return object.qt_metacast(type);
        return null;
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qqmlengine.html#singletonInstance">QQmlEngine::singletonInstance(int qmlTypeId)</a></p>
     */
    @io.qt.QtUninvokable
    public final QJSValue singletonInstance(int qmlTypeId) {
        return singletonInstance(QtJambi_LibraryUtilities.internal.nativeId(this), qmlTypeId);
    }
    
    @io.qt.QtUninvokable
    private static native QJSValue singletonInstance(long __this__nativeId, int qmlTypeId);
    
}// class

class QQmlExpression__{
    public static class ValueIsUndefined extends RuntimeException{
        private static final long serialVersionUID = 7719401165632826435L;
        private ValueIsUndefined(String message) {
            super(message);
        }
    }

}// class

class QQmlIncubationController__{
	public static class WhileFlag{
	    private final long flag;
        public WhileFlag() {
            this(true);
        }
        
        public WhileFlag(boolean flag) {
            long ptr = create(flag);
            this.flag = ptr;
            QtJambi_LibraryUtilities.internal.registerCleaner(this, ()->destroy(ptr));
        }
        
        public void set(boolean flag) {
            set(this.flag, flag);
        }
        
        private static native long create(boolean flag);
        
        private static native void destroy(long ptr);
        
        private static native void set(long ptr, boolean flag);
    }
}// class

class QQmlIncubationController_native__{
}// class

class QJSPrimitiveValue__{
    @io.qt.QtUninvokable
    @Deprecated
    public final io.qt.qml.QJSPrimitiveValue modulo(io.qt.qml.QJSPrimitiveValue rhs){
        return mod(rhs);
    }
    
    @io.qt.QtUninvokable
    @Deprecated
    public final io.qt.qml.QJSPrimitiveValue multiplied(io.qt.qml.QJSPrimitiveValue rhs){
        return times(rhs);
    }
    
    @io.qt.QtUninvokable
    @Deprecated
    public final io.qt.qml.QJSPrimitiveValue added(io.qt.qml.QJSPrimitiveValue rhs){
        return plus(rhs);
    }
    
    @io.qt.QtUninvokable
    @Deprecated
    public final io.qt.qml.QJSPrimitiveValue divided(io.qt.qml.QJSPrimitiveValue rhs){
        return div(rhs);
    }
    
    @io.qt.QtUninvokable
    @Deprecated
    public final io.qt.qml.QJSPrimitiveValue substracted(io.qt.qml.QJSPrimitiveValue rhs){
        return minus(rhs);
    }
}// class
