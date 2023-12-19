/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.CharBuffer;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QTemporaryFile;
import io.qt.core.QTextStream;

public class TestQTextStream extends ApplicationInitializer {

    @Test public void testStringStream()
    {
        {
        	StringBuilder s = new StringBuilder("TestString\n55");
            QTextStream stream = new QTextStream(s, new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadWrite));
            stream.writeString(" ");
            stream.writeString("Hei");
            stream.seek(0);

            Assert.assertEquals("TestString", stream.readString());
            Assert.assertEquals(55, stream.readInt());
            Assert.assertEquals("Hei", stream.readString());
            Assert.assertEquals("TestString\n55 Hei", s.toString());
            Assert.assertTrue(stream.string()==s);
            CharBuffer buffer = ByteBuffer.allocateDirect(1024).order(ByteOrder.nativeOrder()).asCharBuffer();
            ((Buffer)buffer).limit(12);
            buffer.append("DataAsBuffer");
            ((Buffer)buffer).position(0);
            Assert.assertEquals('D', buffer.get());
            ((Buffer)buffer).position(4);
            stream.dispose();
            stream = new QTextStream(s, new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadWrite));
            stream.writeString(buffer);
            stream.flush();
            Assert.assertEquals("TestString\n55 HeiAsBuffer", s.toString());
            stream.dispose();
        }

        StringBuilder daString = new StringBuilder();
        {
            QTextStream stream = new QTextStream(daString, QIODevice.OpenModeFlag.WriteOnly);

            stream.writeInt(1);
            stream.writeString(" ");
            stream.writeByte((byte) 'u');
            stream.writeString(" ");
            stream.writeShort((short) 24);
            stream.writeString(" ");
            stream.writeInt(25);
            stream.writeString(" ");
            stream.writeLong(26);
            stream.writeString(" ");
            stream.writeFloat(24.5f);
            stream.writeString(" ");
            stream.writeDouble(26.4);
            stream.flush();
        }
 
        {
            QTextStream stream = new QTextStream(daString, QIODevice.OpenModeFlag.ReadOnly);

            assertEquals(1, stream.readInt());
            stream.skipWhiteSpace();

            assertEquals((byte) 'u', stream.readByte());
            stream.skipWhiteSpace();

            assertEquals((short) 24, stream.readShort());
            stream.skipWhiteSpace();

            assertEquals(25, stream.readInt());
            stream.skipWhiteSpace();

            assertEquals(26l, stream.readLong());
            stream.skipWhiteSpace();

            assertTrue((float) 24.5 == stream.readFloat());
            stream.skipWhiteSpace();

            assertTrue(26.4 == stream.readDouble());
        }
    }

    @Test public void testString() {
        QFile f = new QTemporaryFile();
        {
            f.open(QFile.OpenModeFlag.WriteOnly);
            QTextStream stream = new QTextStream(f);
            stream.writeString("Hello World\nHow is life today?");
            stream.flush();
            f.close();
        }

        {
            f.open(QFile.OpenModeFlag.ReadOnly);
            QTextStream stream = new QTextStream(f);

            assertEquals("Hello", stream.readString());
            assertEquals("World", stream.readString());
            assertEquals("How", stream.readString());
            assertEquals("is", stream.readString());
            assertEquals("life", stream.readString());
            assertEquals("today?", stream.readString());
            assertTrue(stream.atEnd());
            f.close();
        }
        f.remove();
        f.dispose();
    }

    @Test public void testNumbers() {
        QFile f = new QTemporaryFile();

        // Write out...
        {
            f.open(QFile.OpenModeFlag.WriteOnly);
            QTextStream stream = new QTextStream(f);

            stream.writeInt(1);
            stream.writeString(" ");
            stream.writeByte((byte) 'u');
            stream.writeString(" ");
            stream.writeShort((short) 24);
            stream.writeString(" ");
            stream.writeInt(25);
            stream.writeString(" ");
            stream.writeLong(26);
            stream.writeString(" ");
            stream.writeFloat(24.5f);
            stream.writeString(" ");
            stream.writeDouble(26.4);
            f.close();
        }

        {
            f.open(QFile.OpenModeFlag.ReadOnly);
            QTextStream stream = new QTextStream(f);

            assertTrue(stream.readInt() == 1);
            stream.skipWhiteSpace();

            assertEquals((byte) 'u', stream.readByte());
            stream.skipWhiteSpace();

            assertEquals((short) 24, stream.readShort());
            stream.skipWhiteSpace();

            assertEquals(25, stream.readInt());
            stream.skipWhiteSpace();

            assertEquals(26l, stream.readLong());
            stream.skipWhiteSpace();

            assertEquals((float) 24.5, stream.readFloat(), 0.0);
            stream.skipWhiteSpace();

            assertEquals(26.4, stream.readDouble(), 0.0);
            stream.skipWhiteSpace();
            f.close();
        }
        f.remove();
        f.dispose();
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQTextStream.class.getName());
    }
}
