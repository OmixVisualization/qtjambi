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

import static org.junit.Assert.*;

import org.junit.Test;

import io.qt.gui.rhi.QRhiReadbackResult;
import io.qt.gui.rhi.QRhiScissor;
import io.qt.gui.rhi.QRhiViewport;
import io.qt.gui.rhi.QShaderDescription;

public class TestRhiQt66 extends ApplicationInitializer{
	
	@Test
    public void test() {
		QRhiReadbackResult value = new QRhiReadbackResult();
		assertEquals(null, value.completed());
		QRhiReadbackResult.CompletedFunction cf = ()->{};
		value.setCompleted(cf);
		assertEquals(cf, value.completed());
		
		QShaderDescription d = new QShaderDescription();
		int[] computeShaderLocalSize = d.computeShaderLocalSize();
		assertEquals(3, computeShaderLocalSize.length);
		
		QRhiScissor rscissor = new QRhiScissor(1,2,3,4);
		int[] scissor = rscissor.scissor();
		assertEquals(4, scissor.length);
		assertEquals(1, scissor[0]);
		assertEquals(2, scissor[1]);
		assertEquals(3, scissor[2]);
		assertEquals(4, scissor[3]);
		
		QRhiViewport rviewport = new QRhiViewport();
		float[] viewport = rviewport.viewport();
		assertEquals(4, viewport.length);
	}
}
