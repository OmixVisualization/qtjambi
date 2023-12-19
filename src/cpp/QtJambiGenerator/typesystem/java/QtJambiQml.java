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
    @QtUninvokable
    public <T extends io.qt.core.@Nullable QObject> T create(@NonNull Class<T> type){
        return create(type, (io.qt.qml.QQmlContext)null);
    }

    @QtUninvokable
    public <T extends io.qt.core.@Nullable QObject> T create(@NonNull Class<T> type, io.qt.qml.@Nullable QQmlContext context){
        String packageName = type.getName();
        int idx = packageName.lastIndexOf('.');
        if(idx>0){
            packageName = packageName.substring(0, idx);
        }
        QtUtilities.initializePackage(packageName);
        return type.cast(create(context));
    }
}// class

class __QJSValue extends QJSValue{

}// class

class __QtQml__ extends QtQml{

    private static final int QQmlModuleImportLatest = -1;

    /**
     * @deprecated Use {@link #qmlRegisterUncreatableType(Class, int, String, int, int, String, String)} instead.
     */
    @Deprecated
    public static int qmlRegisterUncreatableClass(java.lang.@NonNull Class<? extends io.qt.core.@Nullable QObject> type, int metaObjectRevision, java.lang.@NonNull String uri, int versionMajor, int versionMinor, java.lang.@NonNull String qmlName, java.lang.@NonNull String reason) {
        return qmlRegisterUncreatableType(type, metaObjectRevision, uri, versionMajor, versionMinor, qmlName, reason);
    }
    /**
     * @deprecated Use {@link #qmlRegisterUncreatableType(Class, String, int, int, String, String)} instead.
     */
    @Deprecated
    public static int qmlRegisterUncreatableClass(java.lang.@NonNull Class<? extends io.qt.core.@Nullable QObject> type, java.lang.@NonNull String uri, int versionMajor, int versionMinor, java.lang.@NonNull String qmlName, java.lang.@NonNull String reason) {
        return qmlRegisterUncreatableType(type, uri, versionMajor, versionMinor, qmlName, reason);
    }

    /**
     * @deprecated Use {@link #qmlRegisterSingletonInstance(String, int, int, String, QObject)} instead.
     */
    @Deprecated
    public static int qmlRegisterSingletonInstance(io.qt.core.@NonNull QObject instance, @NonNull String uri, int versionMajor, int versionMinor, @NonNull String qmlName){
        return qmlRegisterSingletonInstance(uri, versionMajor, versionMinor, qmlName, instance);
    }

    /**
     * <p>See <code><a href="@docRoot/qqmlengine.html#qmlRegisterSingletonInstance">qmlRegisterSingletonInstance&lt;T>(const char*,<wbr/>int,<wbr/>int,<wbr/>const char*,<wbr/>T*)</a></code></p>
     */
    public static <T extends io.qt.core.QObject> int qmlRegisterSingletonInstance(java.lang.@NonNull String uri, int versionMajor, int versionMinor, java.lang.@NonNull String typeName, @StrictNonNull T singleton){
        java.util.Objects.requireNonNull(singleton, "Argument 'singleton': null not expected.");
        return qmlRegisterSingletonType(QtJambi_LibraryUtilities.internal.getClass(singleton), uri, versionMajor, versionMinor, typeName, (e,j)->singleton);
    }
}// class

class QQmlProperty__{
    
    @QtUninvokable
    public final boolean connectNotifySignal(io.qt.core.QMetaObject.@StrictNonNull Slot0 slot){
        return connectNotifySignal(slot, io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot)));
    }
    
    @QtUninvokable
    public final boolean connectNotifySignal(io.qt.core.QMetaObject.@StrictNonNull Slot1<?> slot){
        return connectNotifySignal(slot, io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot)));
    }
    
    @QtUninvokable
    private final boolean connectNotifySignal(io.qt.core.QMetaObject.@StrictNonNull AbstractSlot slot, io.qt.core.@NonNull QMetaMethod metaMethod){
        if(metaMethod!=null && metaMethod.isValid()) {
            io.qt.core.QObject qobject = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
            if(qobject!=null)
                return connectNotifySignal(qobject, metaMethod.methodIndex());
        }
        return false;
    }
    
}// class

class QJSEngine__{

    @QtUninvokable
    public final @NonNull QJSValue newQMetaObject(@NonNull Class<?> type) {
        return newQMetaObject(io.qt.core.QMetaObject.forType(type));
    }
}// class

class QJSEngine_6_{
    @QtUninvokable
    public final <T> T fromScriptValue(@NonNull QJSValue value, @NonNull Class<T> type, io.qt.core.@NonNull QMetaType @NonNull...instantiations) {
        return fromScriptValue(value, io.qt.core.QMetaType.fromType(type, instantiations));
    }
}// class

class QJSEngine_61_{
    @QtUninvokable
    public final <T> T fromManagedValue(@NonNull QJSManagedValue value, @NonNull Class<T> type, io.qt.core.@NonNull QMetaType @NonNull...instantiations) {
        return fromManagedValue(value, io.qt.core.QMetaType.fromType(type, instantiations));
    }
}// class

class QJSEngine_63_{
    @QtUninvokable
    public final <T> T fromVariant(Object variant, @NonNull Class<T> type, io.qt.core.@NonNull QMetaType @NonNull...instantiations) {
        return fromVariant(variant, io.qt.core.QMetaType.fromType(type, instantiations));
    }
}// class

class QQmlEngine__{
    
    /**
     * <p>See <a href="@docRoot/qqmlengine.html#singletonInstance"><code>QQmlEngine::singletonInstance(int)</code></a></p>
     */
    @QtUninvokable
    public final <T extends QtObjectInterface> T singletonInstance(@NonNull Class<T> type, int qmlTypeId) {
        io.qt.core.QObject object = singletonInstance(qmlTypeId).toQObject();
        if(object!=null)
            return object.qt_metacast(type);
        return null;
    }

}// class

class QQmlEngine_65_{

    /**
     * <p>See <a href="@docRoot/qqmlengine.html#singletonInstance-1"><code>QQmlEngine::singletonInstance(QAnyStringView,QAnyStringView)</code></a></p>
     */
    @QtUninvokable
    public final <T extends QtObjectInterface> T singletonInstance(@NonNull Class<T> type, java.lang.@NonNull String moduleName, java.lang.@NonNull String typeName) {
        io.qt.core.QObject object = singletonInstance(moduleName, typeName).toQObject();
        if(object!=null)
            return object.qt_metacast(type);
        return null;
    }
    
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
