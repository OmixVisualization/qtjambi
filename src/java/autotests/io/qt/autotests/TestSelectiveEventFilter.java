/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.HashSet;
import java.util.Set;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.Nullable;
import io.qt.QtUtilities;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QTimer;
import io.qt.widgets.QApplication;
import io.qt.widgets.QDialog;

public class TestSelectiveEventFilter extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void test() {
    	class EventFilter extends QObject{
    		Set<QEvent.Type> events = new HashSet<>();
			@Override
			public boolean eventFilter(@Nullable QObject watched, @Nullable QEvent event) {
				events.add(event.type());
				return false;
			}
    	}
    	EventFilter filter = new EventFilter();
    	QApplication.instance().installEventFilter(filter);
    	QDialog dialog = new QDialog();
    	QTimer.singleShot(1000, dialog::accept);
    	dialog.exec();
    	Assert.assertTrue(filter.events.size()>1);
    	QApplication.instance().removeEventFilter(filter);
    	filter.events.clear();
    	QApplication.instance().installEventFilter(QtUtilities.asSelectiveEventFilter(filter, QEvent.Type.ChildAdded, QEvent.Type.Timer));
    	dialog = new QDialog();
    	QTimer.singleShot(5000, dialog::accept);
    	dialog.exec();
    	Assert.assertTrue(filter.events.size()==2);
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSelectiveEventFilter.class.getName());
    }
}
