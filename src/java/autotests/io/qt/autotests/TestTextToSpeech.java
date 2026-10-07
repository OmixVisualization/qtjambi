/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.*;
import java.util.concurrent.atomic.*;

import org.junit.*;

import io.qt.core.*;
import io.qt.texttospeech.*;

public class TestTextToSpeech extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void test() throws Throwable {
    	AtomicBoolean said = new AtomicBoolean(false);
    	List<QThread> threads = new ArrayList<>();
    	List<Throwable> throwables = new ArrayList<>();
    	for(String engine : QTextToSpeech.availableEngines()) {
	    	QThread thread = QThread.create(()->{
	    		QEventLoop loop = new QEventLoop();
	    		QTextToSpeech tts = new QTextToSpeech(engine);
	    		try {
		        	Assert.assertEquals(tts.errorString(), QTextToSpeech.ErrorReason.NoError, tts.errorReason());
		        	tts.setLocale(QLocale.Language.English);
		        	tts.stateChanged.connect(state->{
		        		switch(state) {
		    			case Error:
		    	    		QTimer.singleShot(3000, loop::quit);
		    				break;
		    			case Paused:
		    				break;
		    			case Ready:
		    				break;
		    			case Speaking:
		    	    		said.set(true);
		    	    		QTimer.singleShot(3000, loop::quit);
		    				break;
		    			case Synthesizing:
		    				break;
		    			default:
		    				break;
		        		}
		        	});
		        	tts.say("Text to speech test");
		        	loop.exec();
	    		}catch(Throwable t){
	    			throwables.add(t);
	    		}finally {
		        	loop.dispose();
		        	tts.dispose();	    			
	    		}
	    	});
	    	thread.setDaemon(true);
	    	thread.finished.connect(QCoreApplication::quit);
	    	thread.start();
	    	threads.add(thread);
	    	Thread.sleep(1000);
//	    	if(said.get())
//	    		break;
    	}
    	QTimer.singleShot(15000, QCoreApplication::quit);
    	QCoreApplication.exec();
    	for(QThread thread : threads) {
	    	if(thread.isRunning())
	    		thread.requestInterruption();
    	}
    	if(!throwables.isEmpty()) {
			if(said.get()) {
				for (Throwable throwable : throwables) {
					throwable.printStackTrace();
				}
			}else {
	    		Throwable error = throwables.get(0);
	    		if(!(error instanceof AssertionError)) {
	    			error = new AssertionError();
	    		}
				for (Throwable throwable : throwables) {
					if(error!=throwable) {
						error.addSuppressed(throwable);
					}
					if(!(throwable instanceof AssertionError)) {
						throwable.printStackTrace();
					}
				}
				throw error;
			}
    	}
    	Assert.assertTrue("Text to speech test did not run", said.get());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestTextToSpeech.class.getName());
    }
}
