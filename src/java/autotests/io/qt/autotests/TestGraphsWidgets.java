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

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QTimer;
import io.qt.graphs.*;
import io.qt.graphs.widgets.*;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QIcon;
import io.qt.quick.widgets.QQuickWidget;
import io.qt.widgets.QApplication;
import io.qt.widgets.QWidget;

public class TestGraphsWidgets extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void initialize() {
    	Assert.assertTrue(io.qt.QtUtilities.initializePackage("io.qt.graphs.widgets"));
    	QGuiApplication.setWindowIcon(new QIcon(":io/qt/autotests/icon.png"));
    	{
	    	QQuickWidget window = new QQuickWidget();
	    	Q3DSurfaceWidgetItem item = new Q3DSurfaceWidgetItem();
	    	item.setWidget(window);
	    	window.setMinimumSize(256, 256);
	    	
	    	QSurfaceDataArray data = new QSurfaceDataArray();
	        QSurfaceDataRow dataRow1 = new QSurfaceDataRow();
	        QSurfaceDataRow dataRow2 = new QSurfaceDataRow();

	        dataRow1.add(new QSurfaceDataItem(0.0f, 0.1f, 0.5f));
	        dataRow1.add(new QSurfaceDataItem(1.0f, 0.5f, 0.5f));
	        dataRow2.add(new QSurfaceDataItem(0.0f, 1.8f, 1.0f));
	        dataRow2.add(new QSurfaceDataItem(1.0f, 1.2f, 1.0f));
	        data.add(dataRow1);
	        data.add(dataRow2);

	        QSurface3DSeries series = new QSurface3DSeries();
	        series.dataProxy().resetArray(data);
	        item.addSeries(series);
	        
	    	window.show();
	    	QTimer.singleShot(1500, window, QWidget::close);
	    	QApplication.exec();
    	}
    }
}
