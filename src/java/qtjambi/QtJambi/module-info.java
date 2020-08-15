/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

/**
 * <p>QtJambi base module containing QtCore, QtGui and QtWidgets.</p>
 * <ul>
 * <li>QtCore - Core non-graphical classes used by other modules.</li>
 * <li>QtGui - Base classes for graphical user interface (GUI) components. Includes OpenGL.</li>
 * <li>QtWidgets - Classes to extend Qt GUI with C++ widgets.</li>
 * </ul>
 */
module qtjambi{
    requires java.base;
    requires transitive java.logging;
    requires transitive java.xml;
    opens io.qt.internal to qtjambi.autotest;
    exports io.qt;
    exports io.qt.core;
    exports io.qt.core.internal;
    exports io.qt.gui;
    exports io.qt.gui.gl;
    exports io.qt.widgets;
    exports io.qt.internal to qtjambi.network,
                              qtjambi.dbus,
                              qtjambi.sql,
                              qtjambi.scxml,
                              qtjambi.quick,
                              qtjambi.webenginecore,
                              qtjambi.xmlpatterns,
                              qtjambi.serialport,
                              qtjambi.script,
                              qtjambi.websockets,
                              qtjambi.core,
                              qtjambi.multimedia,
                              qtjambi.quickcontrols,
                              qtjambi.test,
                              qtjambi.macextras,
                              qtjambi.webchannel,
                              qtjambi.x11extras,
                              qtjambi.webenginewidgets,
                              qtjambi.scripttools,
                              qtjambi.help,
                              qtjambi.networkauth,
                              qtjambi.gamepad,
                              qtjambi.texttospeech,
                              qtjambi.qt3dinput,
                              qtjambi.location,
                              qtjambi.nfc,
                              qtjambi.winextras,
                              qtjambi.qml,
                              qtjambi.positioning,
                              qtjambi.serialbus,
                              qtjambi.printsupport,
                              qtjambi.concurrent,
                              qtjambi.svg,
                              qtjambi.bluetooth,
                              qtjambi.remoteobjects,
                              qtjambi.purchasing,
                              qtjambi.sensors,
                              qtjambi.quickwidgets,
                              qtjambi.webview,
                              qtjambi.xml,
                              qtjambi.webengine,
                              qtjambi.qt3dcore,
                              qtjambi.qt3drender,
                              qtjambi.qt3dextras,
                              qtjambi.qt3dquickextras,
                              qtjambi.qt3dquick,
                              qtjambi.qt3dlogic,
                              qtjambi.qt3danimation,
                              qtjambi.qt3dquickscene2d,
                              qtjambi.plugindeployer;
}
