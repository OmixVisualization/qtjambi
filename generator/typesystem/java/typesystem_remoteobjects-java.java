/****************************************************************************
**
** Copyright (C) 2019 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

class QRemoteObjectNode___ {
    
    @io.qt.QtUninvokable
    public final java.util.List<java.lang.String> instances(Class<? extends io.qt.core.QObject> type){
        for(io.qt.core.QPair<String, String> info : io.qt.core.QMetaObject.forType(type).classInfos()) {
            if("RemoteObject Type".equals(info.first)) {
                return instances(info.second);
            }
        }
        return new java.util.ArrayList<>();
    }
    
    @io.qt.QtUninvokable
    public final <T extends QRemoteObjectReplica> T acquire(Class<T> type, java.lang.String name) throws NoSuchMethodException{
        java.lang.reflect.Constructor<T> constructor = type.getConstructor(QRemoteObjectNode.class, String.class);
        try {
            return constructor.newInstance(this, name);
        } catch (InstantiationException | IllegalAccessException | IllegalArgumentException e) {
            return acquire(type, constructor, name);
        } catch (java.lang.reflect.InvocationTargetException e) {
            if(e.getTargetException() instanceof RuntimeException) {
                throw (RuntimeException)e.getTargetException();
            }else if(e.getTargetException() instanceof Error) {
                throw (Error)e.getTargetException();
            }
            throw new RuntimeException(e.getTargetException());
        }
    }
    private native final <T extends QRemoteObjectReplica> T acquire(Class<T> type, java.lang.reflect.Constructor<T> constructor, java.lang.String name);
    
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

    @io.qt.QtUninvokable
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
    
    @io.qt.QtUninvokable
    public static void qRegisterRemoteObjectsClient(java.lang.String id, Class<? extends ClientIoDevice> type){
        try {
            java.lang.reflect.Constructor<?> constructor = type.getConstructor(io.qt.core.QObject.class);
            qRegisterRemoteObjectsClient(id, type, constructor);
        } catch (NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
    }
    @io.qt.QtUninvokable
    private static native void qRegisterRemoteObjectsClient(java.lang.String id, Class<? extends ClientIoDevice> type, java.lang.reflect.Constructor<?> constructor);
    
    @io.qt.QtUninvokable
    public static void qRegisterRemoteObjectsServer(java.lang.String id, Class<? extends QConnectionAbstractServer> type){
        try {
            java.lang.reflect.Constructor<?> constructor = type.getConstructor(io.qt.core.QObject.class);
            qRegisterRemoteObjectsServer(id, type, constructor);
        } catch (NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
    }
    @io.qt.QtUninvokable
    private static native void qRegisterRemoteObjectsServer(java.lang.String id, Class<? extends QConnectionAbstractServer> type, java.lang.reflect.Constructor<?> constructor);
    
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
