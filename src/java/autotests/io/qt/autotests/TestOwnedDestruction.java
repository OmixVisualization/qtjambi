/****************************************************************************
**
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

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QThread;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;

public class TestOwnedDestruction extends ApplicationInitializer {
    @Test
    public void testOwnedValueObjectThreadedDisposeWithoutCrash() throws InterruptedException {
    	long tstart = System.currentTimeMillis();
    	int numberOfTests = 20;
    	int numberOfThreads = 20;
    	int numberOfCursors = 1000;
    	for (int k = 0; k < numberOfTests; k++) {
    		if(System.currentTimeMillis()-tstart>120000)
    			break;
    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "running test "+(k+1));
    		QTextDocument document = new QTextDocument();
        	List<QThread> threads = new ArrayList<>();
        	List<List<QTextCursor>> threadDatas = new ArrayList<>();
        	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "Test"+(k+1)+": create QTextCursors");
        	for (int i = 0; i < numberOfThreads; i++) {
        		List<QTextCursor> cursors = new ArrayList<>();
        		for (int j = 0; j < numberOfCursors; j+=2) {
        			QTextCursor c = new QTextCursor(document);
        			cursors.add(c);
        			cursors.add(c.clone());
    			}
        		for (QTextCursor textCursor : cursors) {
					assertEquals(document, General.internalAccess.owner(textCursor));
				}
        		threadDatas.add(cursors);
    		}
        	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "Test"+(k+1)+": create deleter threads");
        	for (List<QTextCursor> cursors : threadDatas) {
        		threads.add(QThread.create(()->{
        			for(QTextCursor c : cursors) {
        				c.dispose();
        		    	QThread.yieldCurrentThread();
        			}
        			cursors.clear();
        		}));
    		}
        	threadDatas.clear();
        	threadDatas = null;
        	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "Test"+(k+1)+": start deleter threads");
        	for (QThread thread : threads) {
    			thread.start();
    		}
        	QThread.sleep(2);
        	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "Test"+(k+1)+": delete QTextDocument");
        	document.dispose();
        	document = null;
	    	ApplicationInitializer.runGC();
        	QThread.yieldCurrentThread();
        	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "Test"+(k+1)+": send dispose events");
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "Test"+(k+1)+": wait for finish deleting");
        	for (QThread thread : threads) {
    			thread.join(-1);
    		}
        	threads.clear();
        	threads = null;
        	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, "Test"+(k+1)+": all threads terminated. purging...");
	    	ApplicationInitializer.runGC();
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    }
    
}
