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

class QRemoteObjectNode___ {
    
    /**
     * <p>Overloaded function for {@link #acquireModel(java.lang.String, io.qt.remoteobjects.QtRemoteObjects.InitialAction, java.util.Collection)}</p>
     * <p>with: </p><ul>
     * <li><code>action = io.qt.remoteobjects.QtRemoteObjects.InitialAction.FetchRootSize</code></li>
     * </ul>
     */
    @QtUninvokable
    public final io.qt.remoteobjects.QAbstractItemModelReplica acquireModel(java.lang.String name, int...rolesHint) {
        return acquireModel(name, io.qt.remoteobjects.QtRemoteObjects.InitialAction.FetchRootSize, rolesHint);
    }
    
    /**
     * <p>Overloaded function for {@link #acquireModel(java.lang.String, io.qt.remoteobjects.QtRemoteObjects.InitialAction, java.util.Collection)}.
     */
    @QtUninvokable
    public final io.qt.remoteobjects.QAbstractItemModelReplica acquireModel(java.lang.String name, io.qt.remoteobjects.QtRemoteObjects.InitialAction action, int...rolesHint) {
        java.util.List<Integer> _rolesHint = new java.util.ArrayList<>();
        for (int hint : rolesHint) {
            _rolesHint.add(hint);
        }
        return acquireModel(name, action, _rolesHint);
    }
    
    @QtUninvokable
    public final io.qt.core.QStringList instances(Class<? extends io.qt.core.QObject> type){
        for(io.qt.core.QPair<String, String> info : io.qt.core.QMetaObject.forType(type).classInfos()) {
            if("RemoteObject Type".equals(info.first)) {
                return instances(info.second);
            }
        }
        return new io.qt.core.QStringList();
    }
    
    @QtUninvokable
    public final <T extends QRemoteObjectReplica> T acquire(java.util.function.BiFunction<QRemoteObjectNode, String, T> replicaConstructor) {
        return acquire(replicaConstructor, null);
    }
    
    @QtUninvokable
    public final <T extends QRemoteObjectReplica> T acquire(java.util.function.BiFunction<QRemoteObjectNode, String, T> replicaConstructor, String name) {
        return replicaConstructor.apply(this, name);
    }
    
    @QtUninvokable
    public final <T extends QRemoteObjectReplica> T acquire(Class<T> type) throws NoSuchMethodException{
        return acquire(type, null);
    }
    
    @QtUninvokable
    public final <T extends QRemoteObjectReplica> T acquire(Class<T> type, java.lang.String name) throws NoSuchMethodException{
        if(type==QRemoteObjectDynamicReplica.class) {
            return type.cast(acquireDynamic(name));
        }
        java.lang.reflect.Constructor<T> constructor = type.getConstructor(QRemoteObjectNode.class, String.class);
        try {
			return QtJambi_LibraryUtilities.internal.invokeContructor(constructor, this, name);
        } catch (java.lang.reflect.InvocationTargetException e) {
            if(e.getTargetException() instanceof RuntimeException) {
                throw (RuntimeException)e.getTargetException();
            }else if(e.getTargetException() instanceof Error) {
                throw (Error)e.getTargetException();
            }
            throw new RuntimeException(e.getTargetException());
        }catch(RuntimeException | Error e) {
			throw e;
        }catch(Throwable e) {
            throw new RuntimeException(e);
        }
    }    
}// class

class QtRemoteObjects___ {
    
    public static class TypeNameAndMetaobject{
        TypeNameAndMetaobject(String typeName, io.qt.core.QMetaObject metaObject) {
            super();
            this.typeName = typeName;
            this.metaObject = metaObject;
        }
        public final java.lang.String typeName;
        public final io.qt.core.QMetaObject metaObject;
    }

    @QtUninvokable
    public static TypeNameAndMetaobject getTypeNameAndMetaobjectFromClassInfo(io.qt.core.QMetaObject metaObject){
        String typeName = metaObject.classInfo("RemoteObject Type");
        if (typeName!=null) { //We have an object created from repc or at least with QCLASSINFO defined
            while (metaObject.superClass()!=null) {
                //At the point superclass doesn't have the same QCLASSINFO_REMOTEOBJECT_TYPE,
                //we have the metaobject we should work from
                String _typeName = metaObject.superClass().classInfo("RemoteObject Type");
                if(_typeName!=null) {
                    typeName = _typeName;
                    metaObject = metaObject.superClass();
                }
            }
        }
        return new TypeNameAndMetaobject(typeName, metaObject);
    }
    
    @QtUninvokable
    public static <T> void copyStoredProperties(T src, T dst)
    {
        if(src!=null)QtRemoteObjects.<T>copyStoredProperties(QtJambi_LibraryUtilities.internal.getClass(src), src, dst);
    }
    
    @QtUninvokable
    public static <T> void copyStoredProperties(T src, io.qt.core.QDataStream dst)
    {
        if(src!=null)QtRemoteObjects.<T>copyStoredProperties(QtJambi_LibraryUtilities.internal.getClass(src), src, dst);
    }
    
    @QtUninvokable
    public static <T> void copyStoredProperties(io.qt.core.QDataStream src, T dst)
    {
        if(dst!=null)QtRemoteObjects.<T>copyStoredProperties(QtJambi_LibraryUtilities.internal.getClass(dst), src, dst);
    }
}// class

class IoDevice___ {
    
    public class ReadResult{
        public final QtRemoteObjects.QRemoteObjectPacketTypeEnum packetType;
        public final String result;
        
        private ReadResult(QtRemoteObjects.QRemoteObjectPacketTypeEnum packetType, String result) {
            super();
            this.packetType = packetType;
            this.result = result;
        }
    }
    
}// class

class QRemoteObjectPendingCall___{
    
    /**
     * <p>Overloaded function for {@link #waitForFinished(int)}
     *  with <code>timeout = 30000</code>.</p>
     */
    @QtDeclaredFinal
    @QtUninvokable
    public default void waitForFinished() {
        waitForFinished((int)30000);
    }
    
}// class
