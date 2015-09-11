/****************************************************************************
**
** Copyright (C) 2010-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** ** $BEGIN_LICENSE$
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
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.widgets;

/**
 * @author Peter Droste
 *
 */
public class X11Functions {
    static {
        QtJambi_LibraryInitializer.init();
    }
	
    /**
     * Blocks until the X11 window manager has shown the widget after a call to QWidget::show().
     * @param widget
     */
    public static void waitForWindowManager(QWidget widget){
		__qt_x11_wait_for_window_manager(widget==null ? 0 : widget.nativeId());
	}
    
    private static native void __qt_x11_wait_for_window_manager(long widget__nativeId);
}
