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

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.multimedia.QAudioDeviceInfo;
import io.qt.multimedia.QAudioFormat;
import io.qt.multimedia.QVideoSurfaceFormat;

public class TestMultimediaCloneableQt5 extends ApplicationInitializer {

    @BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
    }
    
	@Test
	public void run_clone_QAudioDeviceInfo() {
		QAudioDeviceInfo org = QAudioDeviceInfo.defaultOutputDevice();
		QAudioDeviceInfo clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QAudioDeviceInfo clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.deviceName(), clone2.deviceName());
	}

	@Test
	public void run_clone_QAudioFormat() {
		QAudioFormat org = new QAudioFormat();
		org.setSampleRate(40);
		QAudioFormat clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QAudioFormat clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.sampleRate(), 40);
		assertEquals(clone.sampleRate(), clone2.sampleRate());
	}

	@Test
	public void run_clone_QVideoSurfaceFormat() {
		QVideoSurfaceFormat org = new QVideoSurfaceFormat();
		org.setFrameRate(20.0);
		QVideoSurfaceFormat clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QVideoSurfaceFormat clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.frameRate(), 20.);
		assertEquals((Object)clone.frameRate(), clone2.frameRate());
	}
}
