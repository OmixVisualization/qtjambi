/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import static org.junit.Assert.*;

import org.junit.Test;

import io.qt.*;
import io.qt.core.*;
import io.qt.test.*;

public class TestTestlib extends QApplicationTest {
    @Test
    public void test() {
    	class Object extends QObject{
    		final Signal2<@QtPrimitiveType Integer, String> sig = new Signal2<>();
    	}
    	Object object = new Object();
    	QSignalSpy signalSpy = new QSignalSpy(object.sig);
    	for(int i=0; i<5; ++i) {
    		object.sig.emit(i, ""+i);
    	}
    	assertEquals(5, signalSpy.size());
//    	QList<?> list = io.qt.internal.QtJambiInternal.fetchField(signalSpy, QSignalSpy.class, "list", QList.class);
//    	System.out.println(signalSpy.toString());
    	assertFalse(signalSpy.isDisposed());
//    	assertFalse(list.isDisposed());
    	signalSpy.dispose();
    	assertTrue(signalSpy.isDisposed());
//    	assertTrue(list.isDisposed());
    }    
}
