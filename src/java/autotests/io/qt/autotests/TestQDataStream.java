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

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serializable;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.DoubleBuffer;

import org.junit.Test;

import io.qt.core.QDataStream;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QPointF;
import io.qt.core.QTemporaryFile;
import io.qt.gui.QPolygonF;

public class TestQDataStream extends ApplicationInitializer {
    // For ARM float/double conversion values must be within precision of a float
    private static final double DELTA = 0.000001;

    @Test public void testQPolygonF() {
        QPolygonF p = new QPolygonF();
        p.add(new QPointF(10, 11));
        p.add(new QPointF(12.2, 13.3));
        p.add(new QPointF(14, 15));

        QFile f = new QTemporaryFile();

        {
            f.open(QFile.OpenModeFlag.WriteOnly);
            QDataStream stream = new QDataStream(f);
            p.writeTo(stream);
            f.close();
        }

        {
            f.open(QFile.OpenModeFlag.ReadOnly);
            QDataStream stream = new QDataStream(f);
            QPolygonF p2 = new QPolygonF();
            p2.readFrom(stream);

            assertEquals(10.0, p2.get(0).x(), 0.0);
            assertEquals(11.0, p2.get(0).y(), 0.0);
            assertEquals(12.2, p2.get(1).x(), DELTA);	// ARM expected:<12.2> but was:<12.199999809265137>
            assertEquals(13.3, p2.get(1).y(), DELTA);	// ARM expected:<13.3> but was:<13.300000190734863>
            assertEquals(14.0, p2.get(2).x(), 0.0);
            assertEquals(15.0, p2.get(2).y(), 0.0);
            f.close();
        }
        f.remove();
        f.dispose();
    }
    
    @Test public void testBuffer() {
    	DoubleBuffer doubleBuffer = ByteBuffer.allocateDirect(1024).order(ByteOrder.nativeOrder()).asDoubleBuffer();
    	doubleBuffer.put(0);
    	doubleBuffer.put(0);
    	doubleBuffer.put(0);
    	doubleBuffer.put(1.2);
    	doubleBuffer.put(2.3);
    	doubleBuffer.put(3.4);
    	doubleBuffer.put(4.5);
    	// required to support JRE 8
    	Buffer buffer = doubleBuffer;
    	buffer.rewind().limit(7).position(3);
    	QIODevice device = QIODevice.fromBuffer(doubleBuffer);
    	if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
	    	QDataStream stream = new QDataStream(device);
	    	try {
	    		assertEquals(1.2, stream.readDouble(), DELTA);
	        	assertEquals(2.3, stream.readDouble(), DELTA);
	        	assertEquals(3.4, stream.readDouble(), DELTA);
	        	assertEquals(4.5, stream.readDouble(), DELTA);
	    	}finally {
	    		stream.dispose();
	    		doubleBuffer.rewind();
	    	}
    	}
    }
    
    public static class SObject implements Serializable{
		private static final long serialVersionUID = 1206575809155446407L;
    }
    
    @Test public void testSerialization() throws IOException, ClassNotFoundException {
    	ByteArrayOutputStream bas = new ByteArrayOutputStream();
    	QDataStream stream = new QDataStream(QIODevice.fromOutputStream(bas));
    	stream.writeObject(new SObject());
    	try(ObjectInputStream ois = new ObjectInputStream(new ByteArrayInputStream(bas.toByteArray()))){
    		assertTrue(ois.readObject() instanceof SObject);
    	}
    	stream = new QDataStream(QIODevice.fromInputStream(new ByteArrayInputStream(bas.toByteArray())));
    	assertTrue(stream.readObject(SObject.class) instanceof SObject);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQDataStream.class.getName());
    }
}
