/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QLocale;
import io.qt.core.QTimer;
import io.qt.texttospeech.QTextToSpeech;
import io.qt.widgets.QApplication;

public class TestTextToSpeech extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void test() {
    	QTextToSpeech tts = new QTextToSpeech();
    	Assert.assertEquals(tts.errorString(), QTextToSpeech.ErrorReason.NoError, tts.errorReason());
    	tts.setLocale(QLocale.Language.English);
    	boolean[] said = {false};
    	tts.stateChanged.connect(state->{
    		switch(state) {
			case Error:
	    		QTimer.singleShot(1000, QCoreApplication::quit);
				break;
			case Paused:
				break;
			case Ready:
				break;
			case Speaking:
	    		said[0] = true;
	    		QTimer.singleShot(1000, QCoreApplication::quit);
				break;
			case Synthesizing:
				break;
			default:
				break;
    		}
    	});
    	tts.say("Text to speech test");
    	QTimer.singleShot(10000, QCoreApplication::quit);
    	QApplication.exec();
    	Assert.assertTrue("Text to speech test did not run", said[0]);
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestTextToSpeech.class.getName());
    }
}
