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

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import io.qt.core.QProperty;
import io.qt.core.QPropertyChangeHandler;
import io.qt.core.QThread;

public class TestQPropertyChangeHandlerQt6 extends QApplicationTest {
    @Test
    public void testQPropertyChangeHandlerDelete() {
    	QProperty<String> prop = new QProperty<>("");
    	List<QPropertyChangeHandler> handlers = new ArrayList<>();
		for(int i=0; i<10000; ++i) {
			handlers.add(prop.onValueChanged(()->{}));
		}
    	QThread thread = QThread.create(()->{
    		while(!QThread.interrupted()){
    			prop.setValue(""+System.nanoTime());
    		}
    	});
    	thread.start();
    	QThread.yieldCurrentThread();
		QThread.msleep(1);
    	for (QPropertyChangeHandler handler : handlers) {
			QThread.msleep(1);
			handler.dispose();			
		}
    	thread.interrupt();
    }
}
