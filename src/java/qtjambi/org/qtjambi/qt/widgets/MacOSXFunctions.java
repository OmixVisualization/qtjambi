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
public class MacOSXFunctions {
    static {
        QtJambi_LibraryInitializer.init();
    }

    /**
     * Turns the Mac OS X secure keyboard feature on or off. QLineEdit uses this when the echo mode is QLineEdit::Password or QLineEdit::NoEcho to guard the editor against keyboard sniffing. If you implement your own password editor, you might want to turn on this feature in your editor's focusInEvent() and turn it off in focusOutEvent().
     * @param enable
     */
    public static void secureKeyboard(boolean enable){
		__qt_mac_secure_keyboard(enable);
	}
    
    /**
     * Sets the menu to display in the Mac OS X Dock for the application. This menu is shown when the user attempts a press-and-hold operation on the application's dock icon or Ctrl-clicks on it while the application is running.
     * The menu will be turned into a Mac menu and the items added to the default Dock menu. There is no merging of the Qt menu items with the items that are in the Dock menu (i.e., it is not recommended to include actions that duplicate functionality of items already in the Dock menu).
     * @param menu
     */
    public static void setDockMenu(QMenu menu){
		__qt_mac_set_dock_menu(menu==null ? 0 : menu.nativeId());
	}
    
    /**
     * Specifies whether icons associated to menu items for the application's menu bar should be shown on Mac OS X. By default, icons are shown on Mac OS X just like on the other platforms.
     * In Qt 4.4, this is equivalent to QApplication::instance()->setAttribute(Qt::AA_DontShowIconsInMenus);.
     * @param enable
     */
    public static void setMenubarIcons(boolean enable){
		__qt_mac_set_menubar_icons(enable);
	}
    
    /**
     * Specifies whether Qt should attempt to relocate standard menu items (such as Quit, Preferences, and About) to the application menu on Mac OS X. This feature is on by default. See Qt for Mac OS X - Specific Issues for the list of menu items for which this applies.
     * @param enable
     */
    public static void setMenubarMerge(boolean enable){
		__qt_mac_set_menubar_merge(enable);
	}
    
    /**
     * Specifies whether the application should use the native menu bar on Mac OS X or be part of the main window. This feature is on by default.
     * In Qt 4.6, this is equivalent to QApplication::instance()->setAttribute(Qt::AA_DontUseNativeMenuBar);.
     * @param enable
     */
    public static void setNativeMenubar(boolean enable){
		__qt_mac_set_native_menubar(enable);
	}
    
    /**
     * Turns emulation of the right mouse button by clicking and holding the left mouse button on or off. This feature is off by default.
     * @param enable
     */
    public static void setPressAndHoldContext(boolean enable){
    	__qt_mac_set_press_and_hold_context(enable);
	}
    
    
    private static native void __qt_mac_secure_keyboard(boolean enable);
    private static native void __qt_mac_set_dock_menu(long menu__nativeId);
    private static native void __qt_mac_set_menubar_icons(boolean enable);
    private static native void __qt_mac_set_menubar_merge(boolean enable);
    private static native void __qt_mac_set_native_menubar(boolean enable);
    private static native void __qt_mac_set_press_and_hold_context(boolean enable);
}
