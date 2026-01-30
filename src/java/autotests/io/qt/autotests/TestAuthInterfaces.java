/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import java.util.Map;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.core.QEasingCurve;
import io.qt.core.QRect;
import io.qt.core.QSize;
import io.qt.core.Qt;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.internal.TestUtility;
import io.qt.network.auth.QAbstractOAuth.ModifyParametersFunction;
import io.qt.network.auth.QAbstractOAuth.Stage;
import io.qt.qml.QtQml;
import io.qt.widgets.QLayoutItem;

public class TestAuthInterfaces extends ApplicationInitializer {
	
	@Test
	public void test_MultiInterfaceImpl() {
		class MultiImpl extends QtObject implements QLayoutItem, QPaintDevice, ModifyParametersFunction, QEasingCurve.EasingFunction, QtQml.ObjectCallback{

			@Override
			public void accept(Stage arg__1, @SuppressWarnings("rawtypes") Map arg__2) {
			}
			
			@Override
			public double applyAsDouble(double v) {
				return v;
			}
			
			@Override
			public io.qt.core.QObject apply(io.qt.qml.QQmlEngine arg__1, io.qt.qml.QJSEngine arg__2){
				return null;
			}

			@Override
			public Qt.Orientations expandingDirections() {
				return new Qt.Orientations(0);
			}

			@Override
			public QRect geometry() {
				return new QRect();
			}

			@Override
			public boolean isEmpty() {
				return false;
			}

			@Override
			public QSize maximumSize() {
				return new QSize();
			}

			@Override
			public QSize minimumSize() {
				return new QSize();
			}

			@Override
			public void setGeometry(QRect arg__1) {
			}

			@Override
			public QSize sizeHint() {
				return new QSize();
			}

			@Override
			public QPaintEngine paintEngine() {
				return null;
			}
		}
		MultiImpl object = new MultiImpl();
		Assert.assertFalse(object.isDisposed());
		object.alignment();
		object.paintingActive();
		object.dispose();
		Assert.assertTrue("object not destroyed.", TestUtility.tryIsObjectDisposed(object));
	}
}
