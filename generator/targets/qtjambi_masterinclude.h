/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#define QQMLPRIVATE_H
#define QML_GETTYPENAMES
#define QGENERICMATRIX_H
#define QTJAMBI_UTILS_H
#define QTJAMBI_REGISTRY_H

#include <QtCore/private/qabstractfileengine_p.h>
#include <QtCore/private/qfsfileengine_p.h>
#include <QtCore/private/qmetaobject_p.h>
#include <QtCore/private/qfactoryloader_p.h>

#define QCOREAPPLICATION_PLATFORM_H
#define QSGTEXTURE_PLATFORM_H
#define QOPENGLCONTEXT_PLATFORM_H
#define QGUIAPPLICATION_PLATFORM_H
#define QGUIAPPLICATION_P_H

#define QSETTINGS_H
#include <QtCore/QtCore>
#undef QSETTINGS_H
#define Q_OS_WIN
#include <QtCore/QSettings>
#undef Q_OS_WIN
#include <qtjambi_core/qtjambi_stringutil.h>
#include <qtjambi_core/qtjambi_core_qhashes.h>

#ifndef QTJAMBI_NO_GUI
#define QACCESSIBLE_H
#include <QtGui/QtGui>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#undef QACCESSIBLE_H
#define quint64 bool
#include <QtGui/qaccessible.h>
#undef quint64

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtPlatformHeaders/QtPlatformHeaders>
#endif
#include <qtjambi_gui/qtjambi_gui.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <qtjambi_gui/qtmatrixes.h>
#endif


#ifndef QTJAMBI_NO_NETWORK
#include <QtNetwork/qtnetworkglobal.h>
#undef QT_NO_SSL
#include <QtNetwork/QtNetwork>
#include <qtjambi_network/qtjambi_network.h>
#endif

#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#include <qtjambi_widgets/qtjambi_widgets_core.h>
#endif

#ifndef QTJAMBI_NO_UITOOLS
#include <QtUiTools/QtUiTools>
#endif

#ifndef QTJAMBI_NO_DESIGNER
#include <QtUiPlugin/QtUiPlugin>
#include <QtDesigner/QtDesigner>
#include <qtjambi_designer/designer.h>
#endif

#ifndef QTJAMBI_NO_QML
#include <QtQml/QtQml>
#include <qtjambi_qml/qqmllistproperty.h>
#include <qtjambi_qml/qtjambi_qml_hashes.h>
#endif

#ifndef QTJAMBI_NO_QUICK
#   if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#       include <QtQuick/QtQuick>
#   else
#       define QSGTEXTURE_PLATFORM_H
#       include <QtQuick/QtQuick>
#       undef QSGTEXTURE_PLATFORM_H
#       ifndef Q_OS_WIN
#           define Q_OS_WIN
#           include <QtQuick/qsgtexture_platform.h>
#           undef Q_OS_WIN
#       else
#           include <QtQuick/qsgtexture_platform.h>
#       endif
#   endif
#   include <qtjambi_quick/qtjambi_quick_hashes.h>
#endif

#ifndef QTJAMBI_NO_QUICKWIDGETS
#include <QtQuickWidgets/QtQuickWidgets>
#endif

#ifndef QTJAMBI_NO_QUICKCONTROLS2
#include <QtQuickControls2/QtQuickControls2>
#endif

#ifndef QTJAMBI_NO_XML
#include <QtXml/QtXml>
#include <qtjambi_xml/qtjambi_xml_hashes.h>
#endif

#ifndef QTJAMBI_NO_PRINTSUPPORT
    #include <QtPrintSupport/QtPrintSupport>
#endif
#ifndef QTJAMBI_NO_CONCURRENT
    #include <QtConcurrent/QtConcurrent>
#endif

#ifndef QTJAMBI_NO_NETWORKAUTH
#include <QtNetworkAuth/QtNetworkAuth>
#endif

#ifndef QTJAMBI_NO_VIRTUAL_KEYBOARD
#include <QtVirtualKeyboard/QtVirtualKeyboard>
#endif

#ifndef QTJAMBI_NO_QUICK3D
#include <QtQuick3D/QtQuick3D>
#include <qtjambi_qt3d/qtjambi_quick3d/qtjambi_quick3d_hashes.h>
#endif

#ifndef QTJAMBI_NO_DATA_VISUALIZATION
#include <QtDataVisualization/QtDataVisualization>
#include <qtjambi_qt3d/qtjambi_datavis/qtjambi_datavis3d_hashes.h>
#endif

#ifndef QTJAMBI_NO_CHARTS
#include <QtCharts/QtCharts>
#endif

#ifndef QTJAMBI_NO_LOTTIE
#include <QtBodyMovin/QtBodyMovin>
#endif

#ifndef QTJAMBI_NO_SQL
#ifdef QT_WIDGETS_LIB
#  include <QtSql/QtSql>
#else
#define QT_WIDGETS_LIB
#  include <QtSql/QtSql>
#undef QT_WIDGETS_LIB
#endif
#  include <qtjambi_sql/qtjambi_sql_hashes.h>
#endif

#ifndef QTJAMBI_NO_SVG
#  include <QtSvg/QtSvg>
#endif

#ifndef QTJAMBI_NO_SVGWIDGETS
#  include <QtSvgWidgets/QtSvgWidgets>
#endif

#ifndef QTJAMBI_NO_HELP
#  include <QtHelp/QtHelp>
#  include <qtjambi_help/qtjambi_help_hashes.h>
#endif

#ifndef QTJAMBI_NO_MULTIMEDIA
#include <qtjambi_multimedia/qtjambi_multimedia.h>
#  include <QtMultimedia/QtMultimedia>
#if QT_VERSION>=0x050000
#  include <QtMultimediaWidgets/QtMultimediaWidgets>
#endif

#endif
#ifndef QTJAMBI_NO_SCRIPT
#  include <QtScript/QtScript>
#  include <qtjambi_script/qtjambi_script_hashes.h>
#endif

#ifndef QTJAMBI_NO_SCRIPTTOOLS
# if QT_VERSION >= 0x040600
#  include <QtScriptTools/QtScriptTools>
# endif
#endif

#ifndef QTJAMBI_NO_XMLPATTERNS
#  include <QtXmlPatterns/QtXmlPatterns>
#endif

#ifndef QTJAMBI_NO_DBUS
# if QT_VERSION >= 0x040200
#  include <QtDBus/QtDBus>
#  include <qtjambi_dbus/QDBusVariantReply.h>
# endif
#endif

#ifndef QTJAMBI_NO_TEST
#define QT_WIDGETS_LIB
#  include <QtTest/QtTest>
#  include <QtGui/qtestsupport_gui.h>
#  include <QtWidgets/qtestsupport_widgets.h>
#  include <QtCore/qtestsupport_core.h>
#  include <QtTest/qtest_gui.h>
#  include <QtTest/qtest_widgets.h>
#  include <QtTest/qtest_network.h>
#  include <QtTest/qtestsystem.h>
#endif

//#include <qtjambi_designer/designer.h>

//#ifndef QTJAMBI_NO_OPENGL
//#	include <QtOpenGL/QtOpenGL>
//#endif // QTJAMBI_NO_OPENGL

# if QT_VERSION >= 0x050000

#ifndef QTJAMBI_NO_WEBENGINECORE
#include <qtjambi_webenginecore/qtjambi_qwebenginecore_hashes.h>
#include <QtWebEngineCore/QtWebEngineCore>

#ifndef QTJAMBI_NO_WEBENGINE
#include <QtWebEngine/QtWebEngine>
#endif

#ifndef QTJAMBI_NO_WEBENGINEQUICK
#include <QtWebEngineQuick/QtWebEngineQuick>
#endif

#ifndef QTJAMBI_NO_WEBENGINEWIDGETS
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <qtjambi_webenginewidgets/qtjambi_webenginewidgets_hashes.h>
#endif
#endif

#ifndef QTJAMBI_NO_WEBSOCKETS
#include <QtWebSockets/QtWebSockets>
#endif

#ifndef QTJAMBI_NO_WEBCHANNEL
#include <QtWebChannel/QtWebChannel>
#endif

#ifndef QTJAMBI_NO_WEBVIEW
#include <QtWebView/QtWebView>
#endif

#ifndef QTJAMBI_NO_WINEXTRAS
#ifndef Q_OS_WIN
#define Q_OS_WIN
#include <QtWinExtras/QtWinExtras>
#undef Q_OS_WIN
#else
#include <QtWinExtras/QtWinExtras>
#endif
#endif

#ifndef QTJAMBI_NO_MACEXTRAS
#ifndef Q_OS_OSX
#define Q_OS_OSX
#include <QtMacExtras/QtMacExtras>
#undef Q_OS_OSX
#else
#include <QtMacExtras/QtMacExtras>
#endif
#endif

#ifndef QTJAMBI_NO_X11EXTRAS
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

#ifndef QTJAMBI_NO_SERIALPORT
#include <QtSerialPort/QtSerialPort>
#endif

#ifndef QTJAMBI_NO_REMOTEOBJECTS
#define QREMOTEOBJECTS_ABSTRACT_ITEM_MODEL_TYPES_H
#define QREMOTEOBJECTSOURCE_H
#include <QtRemoteObjects/QtRemoteObjects>
#include <QtRemoteObjects/private/qconnectionfactories_p.h>
#include <qtjambi_remoteobjects/qtjambi_remoteobjects.h>
#endif

#ifndef QTJAMBI_NO_GAMEPAD
#include <QtGamepad/QtGamepad>
#endif

#ifndef QTJAMBI_NO_STATEMACHINE
#include <QtStateMachine/QtStateMachine>
#endif

#ifndef QTJAMBI_NO_SCXML
#include <QtScxml/QtScxml>
#endif

#ifndef QTJAMBI_NO_NFC
#include <QtNfc/QtNfc>
#include <qtjambi_nfc/qtjambi_nfc.h>
#endif

#ifndef QTJAMBI_NO_PURCHASING
#include <QtPurchasing/QtPurchasing>
#endif

#ifndef QTJAMBI_NO_TEXTTOSPEECH
#include <QtTextToSpeech/QtTextToSpeech>
#include <qtjambi_texttospeech/qtjambi_texttospeech_qhashes.h>
#endif

#ifndef QTJAMBI_NO_SERIALBUS
#include <QtSerialBus/QtSerialBus>
#include <qtjambi_serialbus/qtjambi_serialbus_qhashes.h>
#endif

#ifndef QTJAMBI_NO_SENSORS
#include <QtSensors/QtSensors>
#endif

//#define Q_COMPILER_EXPLICIT_OVERRIDES
#ifndef QTJAMBI_NO_QT3DCORE
#include <qtjambi_qt3d/qtjambi_3dcore/qtjambi_3dcore.h>
#include <Qt3DCore/Qt3DCore>
#endif

#ifndef QTJAMBI_NO_QT3DQUICKSCENE2D
#include <Qt3DQuickScene2D/Qt3DQuickScene2D>
#endif

#ifndef QTJAMBI_NO_QT3DINPUT
#include <qtjambi_qt3d/qtjambi_3dinput/qt3dinput.h>
#include <Qt3DInput/Qt3DInput>
#endif

#ifndef QTJAMBI_NO_QT3DEXTRAS
#include <Qt3DExtras/Qt3DExtras>
#endif

#ifndef QTJAMBI_NO_QT3DLOGIC
#include <Qt3DLogic/Qt3DLogic>
#endif

#ifndef QTJAMBI_NO_QT3DQUICK
#include <Qt3DQuick/Qt3DQuick>
#endif

#ifndef QTJAMBI_NO_QT3DQUICKEXTRAS
#include <Qt3DQuickExtras/Qt3DQuickExtras>
#endif

#ifndef QTJAMBI_NO_QT3DRENDER
#include <qtjambi_qt3d/qtjambi_3drender/qt3drender.h>
#include <Qt3DRender/Qt3DRender>
#endif

#ifndef QTJAMBI_NO_QT3DQUICKRENDER
#include <Qt3DQuickRender/Qt3DQuickRender>
#endif

#ifndef QTJAMBI_NO_QT3DANIMATION
#include <qtjambi_qt3d/qtjambi_3danimation/qtjambi_3danimation_hashes.h>
#include <Qt3DAnimation/Qt3DAnimation>
#endif

#ifndef QTJAMBI_NO_LOCATION
#include <qtjambi_location/qtjambi_location_hashes.h>
#include <QtLocation/QtLocation>
#endif

#ifndef QTJAMBI_NO_POSITIONING
#include <qtjambi_positioning/qtjambi_positioning_hashes.h>
#include <QtPositioning/QtPositioning>
#endif

#ifndef QTJAMBI_NO_BLUETOOTH
#include <qtjambi_bluetooth/qtjambi_bluetooth_hashes.h>
#include <QtBluetooth/QtBluetooth>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#ifndef QTJAMBI_NO_OPENGLWIDGETS
#include <QtOpenGLWidgets/QtOpenGLWidgets>
#endif

#ifndef QTJAMBI_NO_OPENGL

typedef void (*GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

#include <QtOpenGL/QtOpenGL>

#if QT_CONFIG(opengles2)
#include <QtOpenGL/QOpenGLFunctions_ES2>
#undef QT_CONFIG
#define QT_CONFIG(A) 0
#define QT_OPENGL_ES_2_BRIDGE
#endif

#ifdef QT_NO_OPENGL
#undef QT_NO_OPENGL
#define QT_NO_OPENGL_BRIDGE
#endif

#include <qtjambi_opengl/qtjambi_opengl_qhashes.h>
#include <QtOpenGL/QOpenGLPixelTransferOptions>
#include <QtOpenGL/QOpenGLFunctions_1_0>
#include <QtOpenGL/QOpenGLFunctions_1_1>
#include <QtOpenGL/QOpenGLFunctions_1_2>
#include <QtOpenGL/QOpenGLFunctions_1_3>
#include <QtOpenGL/QOpenGLFunctions_1_4>
#include <QtOpenGL/QOpenGLFunctions_1_5>
#include <QtOpenGL/QOpenGLFunctions_2_0>
#include <QtOpenGL/QOpenGLFunctions_2_1>
#include <QtOpenGL/QOpenGLFunctions_3_0>
#include <QtOpenGL/QOpenGLFunctions_3_1>
#include <QtOpenGL/QOpenGLFunctions_3_2_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_3_2_Core>
#include <QtOpenGL/QOpenGLFunctions_3_3_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>
#include <QtOpenGL/QOpenGLFunctions_4_0_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_4_0_Core>
#include <QtOpenGL/QOpenGLFunctions_4_1_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_4_1_Core>
#include <QtOpenGL/QOpenGLFunctions_4_2_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_4_2_Core>
#include <QtOpenGL/QOpenGLFunctions_4_3_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_4_3_Core>
#include <QtOpenGL/QOpenGLFunctions_4_4_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_4_4_Core>
#include <QtOpenGL/QOpenGLFunctions_4_5_Compatibility>
#include <QtOpenGL/QOpenGLFunctions_4_5_Core>

#ifdef QT_OPENGL_ES_2_BRIDGE
#undef QT_OPENGL_ES_2_BRIDGE
#undef QT_CONFIG
#define QT_CONFIG(feature) (1/QT_FEATURE_##feature == 1)
#endif

#ifdef QT_NO_OPENGL_BRIDGE
#undef QT_NO_OPENGL_BRIDGE
#define QT_NO_OPENGL
#endif


#if !QT_CONFIG(opengles2)
#undef QT_FEATURE_opengles2
#define QT_FEATURE_opengles2 1
#define QT_OPENGL_ES_2_BRIDGE
#endif

#include <QtOpenGL/QOpenGLFunctions_ES2>

#ifdef QT_OPENGL_ES_2_BRIDGE
#undef QT_FEATURE_opengles2
#undef QT_OPENGL_ES_2_BRIDGE
#endif

#endif

#undef Q_OS_ANDROID_EMBEDDED
#define Q_OS_ANDROID
#undef QT_FEATURE_xcb
#undef QT_FEATURE_opengl
#undef QT_FEATURE_vulkan
#undef QT_FEATURE_vsp2
#undef Q_OS_WEBOS
#define QT_FEATURE_xcb 1
#define QT_FEATURE_opengl 1
#define QT_FEATURE_vulkan 1
#define QT_FEATURE_vsp2 1
#define Q_OS_WIN
#define Q_OS_MACOS
#define Q_CLANG_QDOC
#define Q_OS_WEBOS

#undef QCOREAPPLICATION_PLATFORM_H
#include <QtCore/qcoreapplication_platform.h>

#undef QGUIAPPLICATION_P_H
#include <QtGui/private/qguiapplication_p.h>
#undef QGUIAPPLICATION_PLATFORM_H
#include <QtGui/qguiapplication_platform.h>
#include <QtGui/qoffscreensurface_platform.h>

#undef QOPENGLCONTEXT_PLATFORM_H
#include <QtGui/qopenglcontext_platform.h>

#include <QtGui/qpa/qplatformwindow_p.h>
#include <QtGui/qpa/qplatformscreen_p.h>

#undef QSGTEXTURE_PLATFORM_H
#include <QtQuick/qsgtexture_platform.h>

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#endif


