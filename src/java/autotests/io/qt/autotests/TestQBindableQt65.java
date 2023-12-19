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

import org.junit.Test;

import io.qt.core.QBindable;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.Qt;
import io.qt.gui.QColor;

public class TestQBindableQt65 extends ApplicationInitializer {
	
	static class Sender extends QObject{
		private QColor legacyProperty = new QColor(Qt.GlobalColor.black);

		public QColor getLegacyProperty() {
			return legacyProperty;
		}

		public void setLegacyProperty(QColor legacyProperty) {
			this.legacyProperty = legacyProperty;
			legacyPropertyChanged.emit();
		}
		
		public final Signal0 legacyPropertyChanged = new Signal0();
	}
	
	static class Receiver extends QObject{
		public final QProperty<QColor> receivedValue = new QProperty<>();
	}
	
    @Test
    public void test() {
    	Sender sender = new Sender();
    	Receiver receiver = new Receiver();
    	@SuppressWarnings("unchecked")
		QBindable<QColor> receivedValueBindable = (QBindable<QColor>)receiver.metaObject().property("receivedValue").bindable(receiver);
    	QBindable<QColor> legacyPropertyBindable;
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
    		legacyPropertyBindable = new QBindable<>(sender, "legacyProperty", QColor.class);
    	}else {
    		legacyPropertyBindable = QBindable.fromProperty(sender::getLegacyProperty);
    	}
    	assertTrue(receivedValueBindable!=null);
    	assertTrue(legacyPropertyBindable!=null);
    	assertEquals(new QColor(), receivedValueBindable.value());
    	receivedValueBindable.setBinding(legacyPropertyBindable.makeBinding());
    	assertEquals(new QColor(Qt.GlobalColor.black), legacyPropertyBindable.value());
    	assertEquals(new QColor(Qt.GlobalColor.black), receivedValueBindable.value());
    	sender.setLegacyProperty(new QColor(Qt.GlobalColor.red));
    	assertEquals(new QColor(Qt.GlobalColor.red), legacyPropertyBindable.value());
    	assertEquals(new QColor(Qt.GlobalColor.red), receivedValueBindable.value());
    	legacyPropertyBindable.setValue(new QColor(Qt.GlobalColor.yellow));
    	assertEquals(new QColor(Qt.GlobalColor.yellow), sender.getLegacyProperty());
    	assertEquals(new QColor(Qt.GlobalColor.yellow), receivedValueBindable.value());
    }
}
