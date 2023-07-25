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

class QScxmlExecutableContent___ {
    public final static int NoContainer = -1;
    public final static int NoString = -1;
    public final static int NoInstruction = -1;
    public final static int NoEvaluator = -1;
}// class

class ForeachLoopBody___ {
    
}// class

class QScxmlStateMachine___ {
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#onEntry"><code>QScxmlStateMachine::onEntry(QObject, const char*)</code></a></p>
     */
    public static io.qt.core.QMetaObject.Slot1<Boolean> onEntry(io.qt.core.QObject object, String slot){
        io.qt.core.QMetaMethod method = object.metaObject().method(slot);
        if(method==null || !method.isValid())
            throw new QNoSuchSlotException(object, slot);
        return (isEnteringState) -> {
            if (isEnteringState && !object.isDisposed())
                method.invoke(object);
        };
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#onExit"><code>QScxmlStateMachine::onExit(QObject, const char*)</code></a></p>
     */
    public static io.qt.core.QMetaObject.Slot1<Boolean> onExit(io.qt.core.QObject object, String slot){
        io.qt.core.QMetaMethod method = object.metaObject().method(slot);
        if(method==null || !method.isValid())
            throw new QNoSuchSlotException(object, slot);
        return (isEnteringState) -> {
            if (!isEnteringState && !object.isDisposed())
                method.invoke(object);
        };
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#onEntry"><code>QScxmlStateMachine::onEntry(Functor)</code></a></p>
     */
    public static io.qt.core.QMetaObject.Slot1<Boolean> onEntry(io.qt.core.QMetaObject.Slot0 slot){
        io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
        return (isEnteringState) -> {
            if (isEnteringState && (object==null || !object.isDisposed()))
                slot.invoke();
        };
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#onExit"><code>QScxmlStateMachine::onExit(Functor)</code></a></p>
     */
    public static io.qt.core.QMetaObject.Slot1<Boolean> onExit(io.qt.core.QMetaObject.Slot0 slot){
        io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
        return (isEnteringState) -> {
            if (!isEnteringState && (object==null || !object.isDisposed()))
                slot.invoke();
        };
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#onEntry"><code>QScxmlStateMachine::onEntry(Functor)</code></a></p>
     */
    public static io.qt.core.QMetaObject.Slot1<Boolean> onEntry(io.qt.core.QMetaObject.Emitable0 signal){
        return (isEnteringState) -> {
            if (isEnteringState && !(signal.containingObject() instanceof io.qt.core.QObject && ((io.qt.core.QObject)signal.containingObject()).isDisposed()))
                signal.emit();
        };
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#onExit"><code>QScxmlStateMachine::onExit(Functor)</code></a></p>
     */
    public static io.qt.core.QMetaObject.Slot1<Boolean> onExit(io.qt.core.QMetaObject.Emitable0 signal){
        return (isEnteringState) -> {
            if (!isEnteringState && !(signal.containingObject() instanceof io.qt.core.QObject && ((io.qt.core.QObject)signal.containingObject()).isDisposed()))
                signal.emit();
        };
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToState"><code>QScxmlStateMachine::connectToState(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot));
        if(metaMethod!=null && metaMethod.isValid() && metaMethod.parameterCount()==0) {
            io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToState(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToState(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        return connectToState(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, slot, _type);
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToState"><code>QScxmlStateMachine::connectToState(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName,
            io.qt.core.QMetaObject.Connectable0 signal, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromSignal(java.util.Objects.requireNonNull(signal));
        if(metaMethod!=null && metaMethod.isValid() && metaMethod.parameterCount()==0) {
            io.qt.core.QObject object = null;
            if(signal.containingObject() instanceof io.qt.core.QObject){
                object = (io.qt.core.QObject)signal.containingObject();
            }
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToState(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToState(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        if(signal instanceof io.qt.core.QMetaObject.Emitable0)
            return connectToState(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, (io.qt.core.QMetaObject.Slot0)((io.qt.core.QMetaObject.Emitable0)signal)::emit, _type);
        else return null;
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToState"><code>QScxmlStateMachine::connectToState(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName,
            io.qt.core.QMetaObject.Slot1<Boolean> slot, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot));
        if(metaMethod!=null && metaMethod.isValid()) {
            if(metaMethod.parameterCount()!=1 || metaMethod.parameterType(0)!=io.qt.core.QMetaType.Type.Bool.value()) {
                throw new IllegalArgumentException("Method does not take a single boolean argument: "+metaMethod.cppMethodSignature());
            }
            io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToState(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToState(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        return connectToState(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, slot, _type);
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToState"><code>QScxmlStateMachine::connectToState(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    @SuppressWarnings("unchecked")
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName,
            io.qt.core.QMetaObject.Connectable1<Boolean> signal, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromSignal(java.util.Objects.requireNonNull(signal));
        if(metaMethod!=null && metaMethod.isValid()) {
            if(metaMethod.parameterCount()!=1 || metaMethod.parameterType(0)!=io.qt.core.QMetaType.Type.Bool.value()) {
                throw new IllegalArgumentException("Method does not take a single boolean argument: "+metaMethod.cppMethodSignature());
            }
            io.qt.core.QObject object = null;
            if(signal.containingObject() instanceof io.qt.core.QObject){
                object = (io.qt.core.QObject)signal.containingObject();
            }
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToState(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToState(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        if(signal instanceof io.qt.core.QMetaObject.Emitable1)
            return connectToState(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, (io.qt.core.QMetaObject.Slot1<Boolean>)((io.qt.core.QMetaObject.Emitable1<Boolean>)signal)::emit, _type);
        else return null;
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToState"><code>QScxmlStateMachine::connectToState(QString, QObject, const char*, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName,
            io.qt.core.QObject object, String member, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        if(member!=null && !member.startsWith("1") && !member.startsWith("2")) {
            io.qt.core.QMetaMethod method = object.metaObject().method(member);
            if(method!=null && method.isValid()) {
                if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)
                    member = "2" + method.cppMethodSignature();
                else
                    member = "1" + method.cppMethodSignature();
            }else {
                member = "1" + member;
            }
        }
        return connectToState(scxmlStateName, object, member, _type);
    }
    
    private native io.qt.core.QMetaObject.Connection connectToState(long nativeId, String scxmlStateName, io.qt.core.QMetaObject.AbstractSlot slot, int type);
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToEvent"><code>QScxmlStateMachine::connectToEvent(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot));
        if(metaMethod!=null && metaMethod.isValid() && metaMethod.parameterCount()==0) {
            io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToEvent(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToEvent(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        return connectToEvent(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, slot, _type);
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToEvent"><code>QScxmlStateMachine::connectToEvent(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName,
            io.qt.core.QMetaObject.Connectable0 signal, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromSignal(java.util.Objects.requireNonNull(signal));
        if(metaMethod!=null && metaMethod.isValid() && metaMethod.parameterCount()==0) {
            io.qt.core.QObject object = null;
            if(signal.containingObject() instanceof io.qt.core.QObject){
                object = (io.qt.core.QObject)signal.containingObject();
            }
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToEvent(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToEvent(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        if(signal instanceof io.qt.core.QMetaObject.Emitable0)
            return connectToEvent(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, (io.qt.core.QMetaObject.Slot0)((io.qt.core.QMetaObject.Emitable0)signal)::emit, _type);
        else
            return null;
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToEvent"><code>QScxmlStateMachine::connectToEvent(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName,
            io.qt.core.QMetaObject.Slot1<QScxmlEvent> slot, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot));
        if(metaMethod!=null && metaMethod.isValid()) {
            if(metaMethod.parameterCount()!=1 || metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QScxmlEvent.class).id()) {
                throw new IllegalArgumentException("Method does not take a single QScxmlEvent argument: "+metaMethod.cppMethodSignature());
            }
            io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToEvent(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToEvent(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        return connectToEvent(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, slot, _type);
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToEvent"><code>QScxmlStateMachine::connectToEvent(QString, Functor, Qt::ConnectionType)</code></a></p>
     */
    @SuppressWarnings("unchecked")
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName,
            io.qt.core.QMetaObject.Connectable1<QScxmlEvent> signal, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromSignal(java.util.Objects.requireNonNull(signal));
        if(metaMethod!=null && metaMethod.isValid()) {
            if(metaMethod.parameterCount()!=1 || metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QScxmlEvent.class).id()) {
                throw new IllegalArgumentException("Method does not take a single QScxmlEvent argument: "+metaMethod.cppMethodSignature());
            }
            io.qt.core.QObject object = null;
            if(signal.containingObject() instanceof io.qt.core.QObject){
                object = (io.qt.core.QObject)signal.containingObject();
            }
            if(object!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return connectToEvent(scxmlStateName, object, "2"+metaMethod.cppMethodSignature(), _type);
                case Method:
                case Slot:
                    return connectToEvent(scxmlStateName, object, "1"+metaMethod.cppMethodSignature(), _type);
                default:
                    break;
                }
            }
        }
        if(signal instanceof io.qt.core.QMetaObject.Emitable1)
            return connectToEvent(QtJambi_LibraryUtilities.internal.nativeId(this), scxmlStateName, (io.qt.core.QMetaObject.Slot1<QScxmlEvent>)((io.qt.core.QMetaObject.Emitable1<QScxmlEvent>)signal)::emit, _type);
        else return null;
    }
    
    /**
     * <p>See <a href="@docRoot/qscxmlstatemachine.html#connectToEvent"><code>QScxmlStateMachine::connectToEvent(QString, QObject, const char*, Qt::ConnectionType)</code></a></p>
     */
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName,
            io.qt.core.QObject object, String member, io.qt.core.Qt.ConnectionType... type){
        int _type = 0;
        for (io.qt.core.Qt.ConnectionType t : type) {
            _type |= t.value();
        }
        if(member!=null && !member.startsWith("1") && !member.startsWith("2")) {
            io.qt.core.QMetaMethod method = object.metaObject().method(member);
            if(method!=null && method.isValid()) {
                if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)
                    member = "2" + method.cppMethodSignature();
                else
                    member = "1" + method.cppMethodSignature();
            }else {
                member = "1" + member;
            }
        }
        return connectToEvent(scxmlStateName, object, member, _type);
    }
    
    private native io.qt.core.QMetaObject.Connection connectToEvent(long nativeId, String scxmlStateName, io.qt.core.QMetaObject.AbstractSlot slot, int type);
}// class

