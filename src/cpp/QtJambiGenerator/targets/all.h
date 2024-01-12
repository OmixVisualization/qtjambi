/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#define QTJAMBI_CORE_H
#define QQMLPRIVATE_H
#define QML_GETTYPENAMES
#define QGENERICMATRIX_H
#define QTJAMBI_UTILS_H
#define QTJAMBI_REGISTRY_H
#define QT_KEYPAD_NAVIGATION

#define QT_NO_STL
#define Q_BYTE_ORDER 1

//qtjambi preprocessor does not understand properly
#define GL_APIENTRY
#define Q_COMPILER_CONSTEXPR
#define Q_COMPILER_UNIFORM_INIT

#undef Q_STATIC_ASSERT
#undef Q_STATIC_ASSERT_X

#include <QtCore/qcompilerdetection.h>

#define seed_seq initializer_list<uint>const

#include <QtCore/qstringliteral.h>
#undef QStringLiteral
#define QStringLiteral QString

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
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0) && !defined(QPERMISSIONS_H)
#include <QtCore/qpermissions.h>
#endif
#include <QtCore/qfloat16.h>
#undef QSETTINGS_H
#define Q_OS_WIN
#include <QtCore/QSettings>
#undef Q_OS_WIN
#include <QtJambiCore/hashes.h>

// opengl:
#define GL_TEXTURE_2D                     0x0DE1
#define GL_NEAREST                        0x2600
#define GL_COLOR_BUFFER_BIT               0x00004000

#ifndef QTJAMBI_NO_GUI
#define QACCESSIBLE_H
typedef struct __GLsync *GLsync;
#include <QtGui/QtGui>
typedef void (*GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
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
#include <QtGui/qopenglfunctions_es2.h>
#undef QT_OPENGL_ES_2
#else
#include <QtGui/QOpenGLFunctions_ES2>
#endif

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#undef QACCESSIBLE_H
#define quint64 bool
#include <QtGui/qaccessible.h>
#undef quint64
#include <QtGui/qpa/qplatformintegration.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#include <QtGui/rhi/qrhi.h>
#include <QtGui/rhi/qshader.h>
#include <QtGui/rhi/qshaderdescription.h>
#       ifndef Q_OS_WIN
#       	define Q_OS_WIN
#       	ifndef Q_OS_MACOS
#               define Q_FORWARD_DECLARE_OBJC_CLASS(X)
#       		define Q_OS_MACOS
#       		include <QtGui/rhi/qrhi_platform.h>
#       		undef Q_OS_MACOS
#			else
#       		include <QtGui/rhi/qrhi_platform.h>
#			endif
#       	undef Q_OS_WIN
#       elif !defined(Q_OS_MACOS)
#           define Q_FORWARD_DECLARE_OBJC_CLASS(X)
#       	define Q_OS_MACOS
#       	include <QtGui/rhi/qrhi_platform.h>
#       	undef Q_OS_MACOS
#		else
#       	include <QtGui/rhi/qrhi_platform.h>
#		endif
#include <QtJambiGuiRhi/hashes.h>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#include <QtGui/qutimimeconverter.h>
#include <QtGui/qwindowsmimeconverter.h>
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtPlatformHeaders/QtPlatformHeaders>
#endif
#include <QtJambiGui/hashes.h>
#endif


#ifndef QTJAMBI_NO_NETWORK
#include <QtNetwork/qtnetworkglobal.h>
#undef QT_NO_SSL
#include <QtNetwork/QtNetwork>
#include <QtJambiNetwork/hashes.h>
#endif

#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#include <QtJambiWidgets/hashes.h>
#endif

#ifndef QTJAMBI_NO_UITOOLS
#include <QtUiTools/QtUiTools>
#endif

#ifndef QTJAMBI_NO_ACTIVEX
#ifdef Q_OBJECT_FAKE
#undef Q_OBJECT_FAKE
#endif
#define Q_OBJECT_FAKE
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
#include <ActiveQt/ActiveQt>
#else
#include <ActiveQt/QtActiveQt>
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtAxContainer/QtAxContainer>
#endif
#endif

#ifndef QTJAMBI_NO_DESIGNER
#define DEPRECATED_HEADER_QtDesigner_customwidget_h
#include <QtUiPlugin/QtUiPlugin>
#include <QtDesigner/QtDesigner>
#ifndef QTJAMBI_NO_UI4
#include <QtDesigner/private/ui4_p.h>
#endif
#endif

#ifndef QTJAMBI_NO_QML
#include <QtQml/QtQml>
#include <QtJambiQml/hashes.h>
#endif

#ifndef QTJAMBI_NO_QUICK
#   if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#       include <QtQuick/QtQuick>
#		define Q_QUICK_PRIVATE_EXPORT
#   else
#		include <QtQuick/qtquickglobal.h>
#       ifndef Q_OS_WIN
#       	define Q_OS_WIN
#       	ifndef Q_OS_MACOS
#       		define Q_OS_MACOS
#       		include <QtQuick/QQuickRenderTarget>
#       		include <QtQuick/QQuickGraphicsDevice>
#       		undef Q_OS_MACOS
#			else
#       		include <QtQuick/QQuickRenderTarget>
#       		include <QtQuick/QQuickGraphicsDevice>
#			endif
#       	undef Q_OS_WIN
#       elif !defined(Q_OS_MACOS)
#       	define Q_OS_MACOS
#       		include <QtQuick/QQuickRenderTarget>
#       		include <QtQuick/QQuickGraphicsDevice>
#       	undef Q_OS_MACOS
#		else
#       	include <QtQuick/QQuickRenderTarget>
#       	include <QtQuick/QQuickGraphicsDevice>
#		endif
#       define QSGTEXTURE_PLATFORM_H
#       include <QtQuick/QtQuick>
#       undef QSGTEXTURE_PLATFORM_H
#   endif
#	include <QtQuick/private/qquickevents_p_p.h>
#   include <QtJambiQuick/hashes.h>
#endif

#ifndef QTJAMBI_NO_QUICKWIDGETS
#include <QtQuickWidgets/QtQuickWidgets>
#endif

#ifndef QTJAMBI_NO_QUICKTEST
#include <QtQuickTest/QtQuickTest>
#endif

#ifndef QTJAMBI_NO_QUICKCONTROLS2
#include <QtQuickControls2/QtQuickControls2>
#endif

#ifndef QTJAMBI_NO_XML
#include <QtXml/QtXml>
#include <QtJambiXml/hashes.h>
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
#include <QtJambiQuick3D/hashes.h>
#endif

#ifndef QTJAMBI_NO_DATA_VISUALIZATION
#include <QtDataVisualization/QtDataVisualization>
#include <QtJambiDataVisualization/hashes.h>
#endif

#ifndef QTJAMBI_NO_CHARTS
#include <QtCharts/QtCharts>
#endif

#ifndef QTJAMBI_NO_LOTTIE
#include <QtBodymovin/QtBodymovin>
#endif

#ifndef QTJAMBI_NO_SQL
#ifdef QT_WIDGETS_LIB
#  include <QtSql/QtSql>
#else
#define QT_WIDGETS_LIB
#  include <QtSql/QtSql>
#undef QT_WIDGETS_LIB
#endif
#  include <QtJambiSql/hashes.h>
#endif

#ifndef QTJAMBI_NO_SVG
#  include <QtSvg/QtSvg>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifndef QTJAMBI_NO_SVGWIDGETS
#  include <QtSvgWidgets/QtSvgWidgets>
#endif
#endif

#ifndef QTJAMBI_NO_HELP
#  include <QtHelp/QtHelp>
#  include <QtJambiHelp/hashes.h>
#endif

#ifndef QTJAMBI_NO_MULTIMEDIA
#  include <QtMultimedia/QtMultimedia>
#include <QtJambiMultimedia/hashes.h>
#if QT_VERSION>=0x050000
#ifndef QTJAMBI_NO_MULTIMEDIAWIDGETS
#  include <QtMultimediaWidgets/QtMultimediaWidgets>
#endif
#endif

#endif
#ifndef QTJAMBI_NO_SCRIPT
#  include <QtScript/QtScript>
#  include <QtJambiScript/hashes.h>
#endif

#ifndef QTJAMBI_NO_SCRIPTTOOLS
# if QT_VERSION >= 0x040600
#  include <QtScriptTools/QtScriptTools>
# endif
#endif

#ifndef QTJAMBI_NO_XMLPATTERNS
#  include <QtXmlPatterns/QtXmlPatterns>
#  include <QtJambiXmlPatterns/hashes.h>
#endif

#ifndef QTJAMBI_NO_DBUS
# if QT_VERSION >= 0x040200
#  include <QtDBus/QtDBus>
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


# if QT_VERSION >= 0x050000

#ifndef QTJAMBI_NO_WEBENGINECORE
#include <QtJambiWebEngineCore/hashes.h>
#include <QtWebEngineCore/QtWebEngineCore>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifndef QTJAMBI_NO_WEBENGINEQUICK
#include <QtWebEngineQuick/QtWebEngineQuick>
#endif
#else
#ifndef QTJAMBI_NO_WEBENGINE
#include <QtWebEngine/QtWebEngine>
#endif
#endif

#ifndef QTJAMBI_NO_WEBENGINEWIDGETS
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QtJambiWebEngineWidgets/hashes.h>
#endif
#endif

#ifndef QTJAMBI_NO_WEBSOCKETS
#include <QtWebSockets/QtWebSockets>
#include <QtJambiWebSockets/hashes.h>
#endif

#ifndef QTJAMBI_NO_WEBCHANNEL
#include <QtWebChannel/QtWebChannel>
//#ifndef QTJAMBI_NO_WEBCHANNELQUICK
//#include <QtWebChannelQuick/QtWebChannelQuick>
//#endif
#endif

#ifndef QTJAMBI_NO_WEBVIEW
#include <QtWebView/QtWebView>
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#ifndef QTJAMBI_NO_SERIALPORT
#include <QtSerialPort/QtSerialPort>
#endif

#ifndef QTJAMBI_NO_REMOTEOBJECTS
#define QREMOTEOBJECTS_ABSTRACT_ITEM_MODEL_TYPES_H
#define QREMOTEOBJECTSOURCE_H
#include <QtRemoteObjects/QtRemoteObjects>
#include <QtJambiRemoteObjects/hashes.h>
#endif

#ifndef QTJAMBI_NO_GAMEPAD
#include <QtGamepad/QtGamepad>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifndef QTJAMBI_NO_STATEMACHINE
#include <QtStateMachine/QtStateMachine>
#endif
#endif

#ifndef QTJAMBI_NO_SCXML
#include <QtScxml/QtScxml>
#endif

#ifndef QTJAMBI_NO_NFC
#include <QtNfc/QtNfc>
#include <QtJambiNfc/hashes.h>
#endif

#ifndef QTJAMBI_NO_PURCHASING
#include <QtPurchasing/QtPurchasing>
#endif

#ifndef QTJAMBI_NO_TEXTTOSPEECH
#include <QtTextToSpeech/QtTextToSpeech>
#include <QtJambiTextToSpeech/hashes.h>
#endif

#ifndef QTJAMBI_NO_SERIALBUS
#include <QtSerialBus/QtSerialBus>
#include <QtJambiSerialBus/hashes.h>
#endif

#ifndef QTJAMBI_NO_SENSORS
#include <QtSensors/QtSensors>
#endif

//#define Q_COMPILER_EXPLICIT_OVERRIDES
#ifndef QTJAMBI_NO_QT3DCORE
#include <Qt3DCore/Qt3DCore>
#endif

#ifndef QTJAMBI_NO_QT3DQUICKSCENE2D
#include <Qt3DQuickScene2D/Qt3DQuickScene2D>
#endif

#ifndef QTJAMBI_NO_QT3DINPUT
#include <Qt3DInput/Qt3DInput>
#include <QtJambi3DInput/hashes.h>
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
#include <Qt3DRender/Qt3DRender>
#include <QtJambi3DRender/hashes.h>
#endif

#ifndef QTJAMBI_NO_QT3DQUICKRENDER
#include <Qt3DQuickRender/Qt3DQuickRender>
#endif

#ifndef QTJAMBI_NO_QT3DANIMATION
#include <Qt3DAnimation/Qt3DAnimation>
#include <QtJambi3DAnimation/hashes.h>
#endif

#ifndef QTJAMBI_NO_LOCATION
#include <QtLocation/QtLocation>
#include <QtJambiLocation/hashes.h>
#endif

#ifndef QTJAMBI_NO_POSITIONING
#include <QtPositioning/QtPositioning>
#include <QtJambiPositioning/hashes.h>
#endif

#ifndef QTJAMBI_NO_BLUETOOTH
#include <QtBluetooth/QtBluetooth>
#include <QtJambiBluetooth/hashes.h>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#ifndef QTJAMBI_NO_OPENGLWIDGETS
#include <QtOpenGLWidgets/QtOpenGLWidgets>
#endif

#ifndef QTJAMBI_NO_PDF
#include <QtPdf/QtPdf>
#include <QtJambiPdf/hashes.h>
#endif
#ifndef QTJAMBI_NO_PDFWIDGETS
#include <QtPdfWidgets/QtPdfWidgets>
#endif

#ifndef QTJAMBI_NO_HTTPSERVER
#define QT_WEBSOCKETS_LIB
#include <QtHttpServer/QtHttpServer>
#endif

#ifndef QTJAMBI_NO_SPATIALAUDIO
#include <QtSpatialAudio/QtSpatialAudio>
#endif

#ifndef QTJAMBI_NO_GRPC
#include <QtGrpc/QtGrpc>
#endif

#ifndef QTJAMBI_NO_PROTOBUF
#include <QtProtobuf/QtProtobuf>
#endif

#ifndef QTJAMBI_NO_OPENGL

#ifdef QT_NO_OPENGL
#undef QT_NO_OPENGL
#define QT_NO_OPENGL_BRIDGE
#endif

#include <QtOpenGL/QtOpenGL>
#include <QtJambiOpenGL/hashes.h>
#include <QtOpenGL/qopenglfunctions_es2.h>

#define QTJAMBI_FEATURE_opengles2 -1

#include <QtOpenGL/qopenglfunctions_1_0.h>
#include <QtOpenGL/qopenglfunctions_1_1.h>
#include <QtOpenGL/qopenglfunctions_1_2.h>
#include <QtOpenGL/qopenglfunctions_1_3.h>
#include <QtOpenGL/qopenglfunctions_1_4.h>
#include <QtOpenGL/qopenglfunctions_1_5.h>
#include <QtOpenGL/qopenglfunctions_2_0.h>
#include <QtOpenGL/qopenglfunctions_2_1.h>
#include <QtOpenGL/qopenglfunctions_3_0.h>
#include <QtOpenGL/qopenglfunctions_3_1.h>
#include <QtOpenGL/qopenglfunctions_3_2_compatibility.h>
#include <QtOpenGL/qopenglfunctions_3_2_core.h>
#include <QtOpenGL/qopenglfunctions_3_3_compatibility.h>
#include <QtOpenGL/qopenglfunctions_3_3_core.h>
#include <QtOpenGL/qopenglfunctions_4_0_compatibility.h>
#include <QtOpenGL/qopenglfunctions_4_0_core.h>
#include <QtOpenGL/qopenglfunctions_4_1_compatibility.h>
#include <QtOpenGL/qopenglfunctions_4_1_core.h>
#include <QtOpenGL/qopenglfunctions_4_2_compatibility.h>
#include <QtOpenGL/qopenglfunctions_4_2_core.h>
#include <QtOpenGL/qopenglfunctions_4_3_compatibility.h>
#include <QtOpenGL/qopenglfunctions_4_3_core.h>
#include <QtOpenGL/qopenglfunctions_4_4_compatibility.h>
#include <QtOpenGL/qopenglfunctions_4_4_core.h>
#include <QtOpenGL/qopenglfunctions_4_5_compatibility.h>
#include <QtOpenGL/qopenglfunctions_4_5_core.h>

#undef QT_FEATURE_opengles2
#define QT_FEATURE_opengles2 1

#ifdef QT_NO_OPENGL_BRIDGE
#undef QT_NO_OPENGL_BRIDGE
#define QT_NO_OPENGL
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
#define __OBJC__

#undef QCOREAPPLICATION_PLATFORM_H
#include <QtCore/qcoreapplication_platform.h>

#ifndef QTJAMBI_NO_GUI
#undef QGUIAPPLICATION_P_H
#include <QtGui/private/qguiapplication_p.h>
#undef QGUIAPPLICATION_PLATFORM_H
#include <QtGui/qguiapplication_platform.h>
#include <QtGui/qoffscreensurface_platform.h>

#undef QOPENGLCONTEXT_PLATFORM_H
#include <QtGui/qopenglcontext_platform.h>

#include <QtGui/qpa/qplatformwindow_p.h>
#include <QtGui/qpa/qplatformscreen_p.h>
#endif
#ifndef QTJAMBI_NO_QUICK
#undef QSGTEXTURE_PLATFORM_H
#include <QtQuick/qsgtexture_platform.h>
#endif

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#endif


