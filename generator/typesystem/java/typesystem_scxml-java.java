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

class QScxmlExecutableContent___ {
public final static int NoContainer = -1;
public final static int NoString = -1;
public final static int NoInstruction = -1;
public final static int NoEvaluator = -1;
}// class

class ForeachLoopBody___ {
    
}// class

class QScxmlStateMachine___ {
    public static io.qt.core.QMetaObject.Slot1<Boolean> onEntry(io.qt.core.QMetaObject.Slot0 slot){
        return (isEnteringState) -> {
            if (isEnteringState)
                slot.invoke();
        };
    }

    public static io.qt.core.QMetaObject.Slot1<Boolean> onExit(io.qt.core.QMetaObject.Slot0 slot){
        return (isEnteringState) -> {
            if (!isEnteringState)
                slot.invoke();
        };
    }
    
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName, io.qt.core.QMetaObject.Slot0 slot){
        return connectToState(scxmlStateName, slot, io.qt.core.Qt.ConnectionType.AutoConnection);
    }
    
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.Qt.ConnectionType type){
        return connectToState(scxmlStateName, b->slot.invoke(), type.value());
    }
    
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName, io.qt.core.QMetaObject.Slot1<Boolean> slot){
        return connectToState(scxmlStateName, slot, io.qt.core.Qt.ConnectionType.AutoConnection);
    }
    
    public io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName,
            io.qt.core.QMetaObject.Slot1<Boolean> slot, io.qt.core.Qt.ConnectionType type){
        return connectToState(scxmlStateName, slot, type.value());
    }
    
    private native io.qt.core.QMetaObject.Connection connectToState(String scxmlStateName, io.qt.core.QMetaObject.Slot1<Boolean> slot, int type);
    
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName, io.qt.core.QMetaObject.Slot0 slot){
        return connectToEvent(scxmlStateName, slot, io.qt.core.Qt.ConnectionType.AutoConnection);
    }
    
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.Qt.ConnectionType type){
        return connectToEvent(scxmlStateName, e->slot.invoke(), type.value());
    }
    
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName, io.qt.core.QMetaObject.Slot1<QScxmlEvent> slot){
        return connectToEvent(scxmlStateName, slot, io.qt.core.Qt.ConnectionType.AutoConnection);
    }
    
    public io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName,
            io.qt.core.QMetaObject.Slot1<QScxmlEvent> slot, io.qt.core.Qt.ConnectionType type){
        return connectToEvent(scxmlStateName, slot, type.value());
    }
    
    private native io.qt.core.QMetaObject.Connection connectToEvent(String scxmlStateName, io.qt.core.QMetaObject.Slot1<QScxmlEvent> slot, int type);
}// class

