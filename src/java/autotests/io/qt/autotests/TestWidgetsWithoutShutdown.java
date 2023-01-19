/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static io.qt.core.QObject.tr;

import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QResource;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.widgets.QApplication;
import io.qt.widgets.QCheckBox;
import io.qt.widgets.QHBoxLayout;
import io.qt.widgets.QSlider;
import io.qt.widgets.QSpinBox;
import io.qt.widgets.QWidget;

public class TestWidgetsWithoutShutdown {
	
    @Test
    public void test() {
    	{
		    QResource.addClassPath(".");
		    QCoreApplication.setApplicationName("QtJambiUnitTest");
		    QApplication.initialize(new String[0]);
		    QWidget window = new QWidget();
			window.setWindowTitle(tr("Enter your age"));
			QCheckBox checkbox = new QCheckBox(tr("Check &Box"));
			QSpinBox spinbox = new QSpinBox();
			spinbox.setRange( 0, 130 );
			spinbox.setValue( 35 );
			QSlider slider = new QSlider( Qt.Orientation.Horizontal );
			slider.setRange( 0, 130 );
			slider.setValue( 35 );
			slider.valueChanged.connect(spinbox::setValue);
			spinbox.valueChanged.connect(slider::setValue);
			QHBoxLayout layout = new QHBoxLayout();
			layout.addWidget( spinbox );
			layout.addWidget( slider );
			layout.addWidget(checkbox);
			window.setLayout( layout );
			window.show();
	    	QTimer.singleShot(500, window::close);
	    	QCoreApplication.exec();
    	}
    	ApplicationInitializer.runGC();
    }
}
