/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package generator;

import io.qt.*;
import io.qt.serialbus.*;

class QModbusPdu___ {

@QtUninvokable
public void encodeData(Number...data) {
    io.qt.core.QByteArray byteArray = new io.qt.core.QByteArray(data());
    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(byteArray, io.qt.core.QIODevice.OpenModeFlag.WriteOnly);
    for (Number s : data) {
        if(s instanceof Byte) {
            stream.writeByte((Byte)s);
        }else if(s instanceof Short){
            stream.writeShort((Short)s);
        }else {
            throw new IllegalArgumentException("Only byte and short supported.");
        }
    }
    stream.dispose();
    setData(byteArray);
}

@QtUninvokable
public void encodeData(short...data) {
    io.qt.core.QByteArray byteArray = new io.qt.core.QByteArray(data());
    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(byteArray, io.qt.core.QIODevice.OpenModeFlag.WriteOnly);
    for (short s : data) {
        stream.writeShort(s);
    }
    stream.dispose();
    setData(byteArray);
}

@QtUninvokable
public void encodeData(byte...data) {
    io.qt.core.QByteArray byteArray = new io.qt.core.QByteArray(data());
    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(byteArray, io.qt.core.QIODevice.OpenModeFlag.WriteOnly);
    for (byte s : data) {
        stream.writeByte(s);
    }
    stream.dispose();
    setData(byteArray);
}

@QtUninvokable
public void decodeData(byte[] data) {
    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);
    for (int i = 0; i < data.length; ++i) {
        data[i] = stream.readByte();
    }
    stream.dispose();
}

@QtUninvokable
public void decodeData(short[] data) {
    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);
    for (int i = 0; i < data.length; ++i) {
        data[i] = stream.readShort();
    }
    stream.dispose();
}

@QtUninvokable
public void decodeData(Number[][] data) {
    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);
    for (int i = 0; i < data.length; ++i) {
        if(data[i] instanceof Byte[]) {
            ((Byte[])data[i])[0] = stream.readByte();
        }else if(data[i] instanceof Short[]) {
            ((Short[])data[i])[0] = stream.readShort();
        }else {
            stream.dispose();
            throw new IllegalArgumentException("Only byte and short supported.");
        }
    }
    stream.dispose();
}

}// class
