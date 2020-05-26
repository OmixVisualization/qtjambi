/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "generator_predefinitions.h"

#define QTJAMBI_CORE_H

#if QT_VERSION>=0x050000
	#include <QtCore/private/qabstractfileengine_p.h>
	#include <QtCore/private/qfsfileengine_p.h>
#endif

#define QSETTINGS_H
#include <QtCore/QtCore>
#undef QSETTINGS_H
#define Q_OS_WIN
#include <QtCore/QSettings>
#undef Q_OS_WIN
#include <qtjambi_core/qtjambiqfuture.h>
#include <qtjambi_core/qtjambi_core_qhashes.h>

#define QACCESSIBLE_H
#include <QtGui/QtGui>
#include <QtGui/QOpenGLFunctions_1_0>
#include <QtGui/QOpenGLFunctions_1_1>
#include <QtGui/QOpenGLFunctions_1_2>
#include <QtGui/QOpenGLFunctions_1_3>
#include <QtGui/QOpenGLFunctions_1_4>
#include <QtGui/QOpenGLFunctions_1_5>
#include <QtGui/QOpenGLFunctions_2_0>
#include <QtGui/QOpenGLFunctions_2_1>
#include <QtGui/QOpenGLFunctions_3_0>
#include <QtGui/QOpenGLFunctions_3_1>
#include <QtGui/QOpenGLFunctions_3_2_Compatibility>
#include <QtGui/QOpenGLFunctions_3_2_Core>
#include <QtGui/QOpenGLFunctions_3_3_Compatibility>
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QOpenGLFunctions_4_0_Compatibility>
#include <QtGui/QOpenGLFunctions_4_0_Core>
#include <QtGui/QOpenGLFunctions_4_1_Compatibility>
#include <QtGui/QOpenGLFunctions_4_1_Core>
#include <QtGui/QOpenGLFunctions_4_2_Compatibility>
#include <QtGui/QOpenGLFunctions_4_2_Core>
#include <QtGui/QOpenGLFunctions_4_3_Compatibility>
#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QOpenGLFunctions_4_4_Compatibility>
#include <QtGui/QOpenGLFunctions_4_4_Core>
#include <QtGui/QOpenGLFunctions_4_5_Compatibility>
#include <QtGui/QOpenGLFunctions_4_5_Core>

#ifndef QT_OPENGL_ES_2
#define QT_OPENGL_ES_2
#include <QtGui/QOpenGLFunctions_ES2>
#undef QT_OPENGL_ES_2
#else
#include <QtGui/QOpenGLFunctions_ES2>
#endif

#undef QACCESSIBLE_H
#define quint64 bool
#include <QtGui/qaccessible.h>
#undef quint64

#include <QtPlatformHeaders/QtPlatformHeaders>
#include <qtjambi_gui/qtreemodel.h>
#include <qtjambi_gui/qtjambi_gui.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <qtjambi_gui/qtmatrixes.h>


#include <QtNetwork/qtnetworkglobal.h>
#undef QT_NO_SSL
#include <QtNetwork/QtNetwork>
#include <qtjambi_network/qtjambi_network.h>

#if QT_VERSION>=0x050000
	#include <QtWidgets/QtWidgets>
    #include <qtjambi_widgets/qtjambi_widgets_core.h>
	#include <QtQuickWidgets/QtQuickWidgets>
	#ifndef QT_NO_PRINTSUPPORT
		#include <QtPrintSupport/QtPrintSupport>
	#endif
	#ifndef QT_NO_CONCURRENT
		#include <QtConcurrent/QtConcurrent>
	#endif
	
	#define QQMLPRIVATE_H
	#include <QtQml/QtQml>
	#include <qtjambi_qml/qqmllistproperty.h>
	#include <QtQuick/QtQuick>
	#include <QtQuickControls2/QtQuickControls2>
#endif

#ifndef QT_NO_NETWORKAUTH
#include <QtNetworkAuth/QtNetworkAuth>
#endif

#ifndef QT_NO_SQL
#  include <QtSql/QtSql>
#  include <qtjambi_sql/qtjambi_sql_hashes.h>
#endif
#ifndef QT_NO_SVG
#  include <QtSvg/QtSvg>
#endif
#include <QtXml/QtXml>
#include <qtjambi_xml/qtjambi_xml_hashes.h>
#ifndef QT_NO_HELP
#  include <QtHelp/QtHelp>
#  include <qtjambi_help/qtjambi_help_hashes.h>
#endif
#ifndef QT_NO_MULTIMEDIA
#include <qtjambi_multimedia/qtjambi_multimedia.h>
#  include <QtMultimedia/QtMultimedia>
#if QT_VERSION>=0x050000
#  include <QtMultimediaWidgets/QtMultimediaWidgets>
#endif
#endif
#ifndef QT_NO_SCRIPT
#  include <QtScript/QtScript>
#  include <qtjambi_script/qtjambi_script_hashes.h>
#endif
#ifndef QT_NO_SCRIPTTOOLS
# if QT_VERSION >= 0x040600
#  include <QtScriptTools/QtScriptTools>
# endif
#endif

#ifndef QT_NO_XMLPATTERNS
#  include <QtXmlPatterns/QtXmlPatterns>
#endif

#ifndef QT_NO_DBUS
# if QT_VERSION >= 0x040200
#  include <QtDBus/QtDBus>
#  include <qtjambi_dbus/QDBusVariantReply.h>
# endif
#endif

#ifndef QT_NO_TEST
#  include <QtTest/QtTest>
#endif

#include <qtjambi_designer/designer.h>

//#ifndef QT_NO_OPENGL
//#	include <QtOpenGL/QtOpenGL>
//#endif // QT_NO_OPENGL

# if QT_VERSION >= 0x050000

#ifndef QT_NO_WEBENGINE
#include <qtjambi_webenginecore/qtjambi_qwebenginecore_hashes.h>
#include <QtWebEngineCore/QtWebEngineCore>
#include <QtWebEngine/QtWebEngine>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <qtjambi_webenginewidgets/qtjambi_webenginewidgets_hashes.h>
#endif

#ifndef QT_NO_WEBSOCKETS
#include <QtWebSockets/QtWebSockets>
#endif

#ifndef QT_NO_WEBCHANNEL
#include <QtWebChannel/QtWebChannel>
#endif

#ifndef QT_NO_WEBVIEW
#include <QtWebView/QtWebView>
#endif

#ifndef QT_NO_WINEXTRAS
#ifndef Q_OS_WIN
#define Q_OS_WIN
#include <QtWinExtras/QtWinExtras>
#undef Q_OS_WIN
#else
#include <QtWinExtras/QtWinExtras>
#endif
#endif

#ifndef QT_NO_MACEXTRAS
#ifndef Q_OS_OSX
#define Q_OS_OSX
#include <QtMacExtras/QtMacExtras>
#undef Q_OS_OSX
#else
#include <QtMacExtras/QtMacExtras>
#endif
#endif

#ifndef QT_NO_X11EXTRAS
#ifndef Q_OS_X11
#define Q_OS_X11
#include <QtX11Extras/QtX11Extras>
#include <QtX11Extras/QX11Info>
#undef Q_OS_X11
#else
#include <QtX11Extras/QtX11Extras>
#include <QtX11Extras/QX11Info>
#endif
#endif

#ifndef QT_NO_SERIALPORT
#include <QtSerialPort/QtSerialPort>
#endif

#ifndef QT_NO_REMOTEOBJECTS
#define QREMOTEOBJECTS_ABSTRACT_ITEM_MODEL_TYPES_H
#define QREMOTEOBJECTSOURCE_H
#include <QtRemoteObjects/QtRemoteObjects>
#include <QtRemoteObjects/private/qconnectionfactories_p.h>
#include <qtjambi_remoteobjects/qtjambi_remoteobjects.h>
#endif

#ifndef QT_NO_GAMEPAD
#include <QtGamepad/QtGamepad>
#endif

#ifndef QT_NO_SCXML
#include <QtScxml/QtScxml>
#endif

#ifndef QT_NO_NFC
#include <QtNfc/QtNfc>
#include <qtjambi_nfc/qtjambi_nfc.h>
#endif

#ifndef QT_NO_PURCHASING
#include <QtPurchasing/QtPurchasing>
#endif

#ifndef QT_NO_TEXTTOSPEECH
#include <QtTextToSpeech/QtTextToSpeech>
#include <qtjambi_texttospeech/qtjambi_texttospeech_qhashes.h>
#endif

#ifndef QT_NO_SERIALBUS
#include <QtSerialBus/QtSerialBus>
#include <qtjambi_serialbus/qtjambi_serialbus_qhashes.h>
#endif

#ifndef QT_NO_SENSORS
#include <QtSensors/QtSensors>
#endif

//#define Q_COMPILER_EXPLICIT_OVERRIDES
#ifndef QT_NO_QT3DCORE

// the generator is not able to find enum QSceneChange::Priority
#include <Qt3DCore/QSceneChange>
#define Priority Qt3DCore::QSceneChange::Priority
#include <Qt3DCore/QBackendScenePropertyChange>
#include <qtjambi_qt3d/qtjambi_3dcore/qtjambi_3dcore.h>
#undef Priority

#include <Qt3DCore/Qt3DCore>
#include <Qt3DCore/private/qbackendnodefactory_p.h>
#endif

#ifndef QT_NO_QT3DQUICKSCENE2D
#include <Qt3DQuickScene2D/Qt3DQuickScene2D>
#endif

#ifndef QT_NO_QT3DINPUT
#include <qtjambi_qt3d/qtjambi_3dinput/qt3dinput.h>
#include <Qt3DInput/Qt3DInput>
#endif

#ifndef QT_NO_QT3DEXTRAS
#include <Qt3DExtras/Qt3DExtras>
#endif

#ifndef QT_NO_QT3DLOGIC
#include <Qt3DLogic/Qt3DLogic>
#endif

#ifndef QT_NO_QT3DQUICK
#include <Qt3DQuick/Qt3DQuick>
#endif

#ifndef QT_NO_QT3DQUICKEXTRAS
#include <Qt3DQuickExtras/Qt3DQuickExtras>
#endif

#ifndef QT_NO_QT3DRENDER
#include <qtjambi_qt3d/qtjambi_3drenderer/qt3drender.h>
#include <Qt3DRender/Qt3DRender>
#endif

#ifndef QT_NO_QT3DQUICKRENDER
#include <Qt3DQuickRender/Qt3DQuickRender>
#endif

#ifndef QT_NO_QT3DANIMATION
#include <Qt3DAnimation/Qt3DAnimation>
#endif

#ifndef QT_NO_LOCATION
#include <qtjambi_location/qtjambi_location_hashes.h>
#include <QtLocation/QtLocation>
#endif

#ifndef QT_NO_POSITIONING
#include <qtjambi_positioning/qtjambi_positioning_hashes.h>
#include <QtPositioning/QtPositioning>
#endif

#ifndef QT_NO_BLUETOOTH
#include <qtjambi_bluetooth/qtjambi_bluetooth_hashes.h>
#include <QtBluetooth/QtBluetooth>
#endif

#endif


