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

import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.charts.*;
import io.qt.core.*;

import static org.junit.Assert.*;

import org.junit.BeforeClass;

public class TestCharts extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void testBarSeries() {
    	QChart chart = new QChart();
    	QObject initialParent = new QObject();
    	QBarSeries series = new QBarSeries();
    	QBarSet set = new QBarSet("A");
    	series.append(set);
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(set));
    	assertEquals(series, set.parent());
    	set = new QBarSet("B", initialParent);
    	series.append(set);
    	series.remove(set);
    	assertTrue(set.isDisposed());
    	QBarSet _set = new QBarSet("C", initialParent);
    	series.append(QList.of(_set));
    	assertEquals(series, _set.parent());
    	set = new QBarSet("D", initialParent);
    	series.insert(0, set);
    	assertEquals(initialParent, set.parent());
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(set));
    	chart.addSeries(series);
    	QBarCategoryAxis axis = new QBarCategoryAxis();
    	chart.addAxis(axis, Qt.AlignmentFlag.AlignBottom);
    	assertTrue(axis.parent()!=null);
    	assertEquals(0, series.attachedAxes().size());
    	assertTrue(series.attachAxis(axis));
    	assertEquals(1, series.attachedAxes().size());
    	axis.dispose();
    	assertEquals(0, series.attachedAxes().size());
    	series.dispose();
    	assertTrue(_set.isDisposed());
    	chart.dispose();
    }
    
    @Test
    public void testPieSeries() {
    	QObject initialParent = new QObject();
    	QPieSeries series = new QPieSeries();
    	QPieSlice set = new QPieSlice("A", 1);
    	series.append(set);
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(set));
    	assertEquals(series, set.parent());
    	set = new QPieSlice("B", 1, initialParent);
    	series.append(set);
    	assertEquals(series, set.parent());
    	series.remove(set);
    	assertTrue(set.isDisposed());
    	set = new QPieSlice("C", 1, initialParent);
    	series.insert(0, set);
    	assertEquals(series, set.parent());
    	set = new QPieSlice("D", 1, initialParent);
    	series.append(QList.of(set));
    	assertEquals(series, set.parent());
    	series.dispose();
    	assertTrue(set.isDisposed());
    	initialParent.dispose();
    	series.dispose();
    }
    
    @Test
    public void testBoxPlotSeries() {
    	QObject initialParent = new QObject();
    	QBoxPlotSeries series = new QBoxPlotSeries();
    	QBoxSet set = new QBoxSet("A");
    	series.append(set);
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(set));
    	assertEquals(series, set.parent());
    	set = new QBoxSet("B", initialParent);
    	series.append(set);
    	assertEquals(series, set.parent());
    	series.remove(set);
    	assertTrue(set.isDisposed());
    	QBoxSet _set = new QBoxSet("C", initialParent);
    	series.insert(0, _set);
    	assertEquals(initialParent, _set.parent());
    	set = new QBoxSet("D", initialParent);
    	series.append(QList.of(set));
    	assertEquals(initialParent, set.parent());
    	series.dispose();
    	assertFalse(_set.isDisposed());
    	assertFalse(set.isDisposed());
    	initialParent.dispose();
    	series.dispose();
    }
    
    @Test
    public void testCandlestickSeries() {
    	QObject initialParent = new QObject();
    	QCandlestickSeries series = new QCandlestickSeries();
    	QCandlestickSet set = new QCandlestickSet(1.0);
    	series.append(set);
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(set));
    	assertEquals(null, set.parent());
    	set = new QCandlestickSet(2.0, initialParent);
    	series.append(set);
    	assertEquals(initialParent, set.parent());
    	series.remove(set);
    	assertTrue(set.isDisposed());
    	set = new QCandlestickSet(3.0, initialParent);
    	series.insert(0, set);
    	assertEquals(initialParent, set.parent());
    	set = new QCandlestickSet(4.0, initialParent);
    	series.append(QList.of(set));
    	assertEquals(initialParent, set.parent());
    	series.dispose();
    	assertTrue(set.isDisposed());
    	initialParent.dispose();
    	series.dispose();
    }
    
    @Test
    public void testAreaSeries() {
    	QLineSeries lower = new QLineSeries();
    	QLineSeries upper = new QLineSeries();
    	QAreaSeries series = new QAreaSeries(upper, lower);
    	assertEquals(null, lower.parent());
    	assertEquals(null, upper.parent());
    	assertTrue("not java ownership", General.internalAccess.isJavaOwnership(lower));
    	assertTrue("not java ownership", General.internalAccess.isJavaOwnership(upper));
    	assertEquals(lower, series.lowerSeries());
    	series.dispose();
    	assertFalse(lower.isDisposed());
    }
    
}
