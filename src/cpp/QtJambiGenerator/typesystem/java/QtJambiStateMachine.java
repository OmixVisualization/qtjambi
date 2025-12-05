/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


class QSignalTransition___ {

    /**
     * Overloaded constructor for {@link #QSignalTransition(io.qt.core.QMetaObject.AbstractSignal, io.qt.core.QState)}.
     */
    public QSignalTransition(io.qt.core.QMetaObject.AbstractSignal signal) {
        this(signal, null);
    }

    /**
     * Constructs a new signal transition associated with the given signal of the given sender object and with the given sourceState.
     */
    public QSignalTransition(io.qt.core.QMetaObject.AbstractSignal signal, QState sourceState) {
        super((QPrivateConstructor)null);
        if(signal.containingObject() instanceof io.qt.core.QObject) {
            io.qt.core.QObject sender = (io.qt.core.QObject)signal.containingObject();
            String signalSignature = "2" + sender.metaObject().methods().get(signal.methodIndex()).cppMethodSignature();
            initialize_native(this, sender, signalSignature, sourceState);
        }else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }

    /**
     * Sets the signal associated with this signal transition.
     */
    @QtUninvokable
    public final void setSignal(io.qt.core.QMetaObject.AbstractSignal signal) {
        if (signal.containingObject() instanceof io.qt.core.QObject) {
            io.qt.core.QObject sender = (io.qt.core.QObject) signal.containingObject();
            String signalSignature = "2" + sender.metaObject().methods().get(signal.methodIndex()).cppMethodSignature();
            setSenderObject(sender);
            setSignal(new io.qt.core.QByteArray(signalSignature));
        } else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }

}// class

class QState___ {

    /**
     * Adds a transition associated with the given signal of the given sender
     * object, and returns the new QSignalTransition object. The transition has this
     * state as the source, and the given target as the target state.
     */
    @QtUninvokable
    public final QSignalTransition addTransition(io.qt.core.QMetaObject.AbstractSignal signal, QAbstractState target) {
        if (signal.containingObject() instanceof io.qt.core.QObject && signal.methodIndex() != 0) {
            io.qt.core.QObject sender = (io.qt.core.QObject) signal.containingObject();
            String signalSignature = "2" + sender.metaObject().methods().get(signal.methodIndex()).cppMethodSignature();
            return addTransition(sender, signalSignature, target);
        } else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }

}// class
