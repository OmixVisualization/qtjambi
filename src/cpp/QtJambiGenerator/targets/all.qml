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

import QtJambiGenerator 1.0

TypeSystem{
    LoadTypeSystem{name: "QtCore";              generate: true; unless: "QTJAMBI_NO_CORE"}
    LoadTypeSystem{name: "QtGui";               generate: true; unless: "QTJAMBI_NO_GUI"}
    LoadTypeSystem{name: "QtGuiQpa";            generate: true; unless: "QTJAMBI_NO_GUI";              since: 7}
    LoadTypeSystem{name: "QtGuiRhi";            generate: true; unless: "QTJAMBI_NO_GUI";              since: 6.6}
    LoadTypeSystem{name: "QtOpenGL";            generate: true; unless: "QTJAMBI_NO_OPENGL";           since: 6}
    LoadTypeSystem{name: "QtConcurrent";        generate: true; unless: "QTJAMBI_NO_CONCURRENT"}
    LoadTypeSystem{name: "QtWidgets";           generate: true; unless: "QTJAMBI_NO_WIDGETS"}
    LoadTypeSystem{name: "QtOpenGLWidgets";     generate: true; unless: "QTJAMBI_NO_OPENGLWIDGETS";    since: 6}
    LoadTypeSystem{name: "QtSql";               generate: true; unless: "QTJAMBI_NO_SQL"}
    LoadTypeSystem{name: "QtNetwork";           generate: true; unless: "QTJAMBI_NO_NETWORK"}
    LoadTypeSystem{name: "QtNetworkAuth";       generate: true; unless: "QTJAMBI_NO_NETWORKAUTH"}
    LoadTypeSystem{name: "QtXml";               generate: true; unless: "QTJAMBI_NO_XML"}
    LoadTypeSystem{name: "QtXmlPatterns";       generate: true; unless: "QTJAMBI_NO_XMLPATTERNS";      until: 5}
    LoadTypeSystem{name: "QtQml";               generate: true; unless: "QTJAMBI_NO_QML"}
    LoadTypeSystem{name: "QtQuick";             generate: true; unless: "QTJAMBI_NO_QUICK"}
    LoadTypeSystem{name: "QtQuickTest";         generate: true; unless: "QTJAMBI_NO_QUICKTEST"}
    LoadTypeSystem{name: "QtQuickControls2";    generate: true; unless: "QTJAMBI_NO_QUICKCONTROLS2"}
    LoadTypeSystem{name: "QtQuickWidgets";      generate: true; unless: "QTJAMBI_NO_QUICKWIDGETS"}
    LoadTypeSystem{name: "QtMacExtras";         generate: true; unless: "QTJAMBI_NO_MACEXTRAS";        until: 5}
    LoadTypeSystem{name: "QtWinExtras";         generate: true; unless: "QTJAMBI_NO_WINEXTRAS";        until: 5}
    LoadTypeSystem{name: "QtX11Extras";         generate: true; unless: "QTJAMBI_NO_X11EXTRAS";        until: 5}
    LoadTypeSystem{name: "QtSerialPort";        generate: true; unless: "QTJAMBI_NO_SERIALPORT"}
    LoadTypeSystem{name: "QtSerialBus";         generate: true; unless: "QTJAMBI_NO_SERIALBUS"}
    LoadTypeSystem{name: "QtScript";            generate: true; unless: "QTJAMBI_NO_SCRIPT";           until: 5}
    LoadTypeSystem{name: "QtScriptTools";       generate: true; unless: "QTJAMBI_NO_SCRIPTTOOLS";      until: 5}
    LoadTypeSystem{name: "QtMultimedia";        generate: true; unless: "QTJAMBI_NO_MULTIMEDIA"}
    LoadTypeSystem{name: "QtPrintSupport";      generate: true; unless: "QTJAMBI_NO_PRINTSUPPORT"}
    LoadTypeSystem{name: "QtMultimediaWidgets"; generate: true; unless: "QTJAMBI_NO_MULTIMEDIAWIDGETS"}
    LoadTypeSystem{name: "QtSvg";               generate: true; unless: "QTJAMBI_NO_SVG"}
    LoadTypeSystem{name: "QtSvgWidgets";        generate: true; unless: "QTJAMBI_NO_SVGWIDGETS";       since: 6}
    LoadTypeSystem{name: "QtDBus";              generate: true; unless: "QTJAMBI_NO_DBUS"}
    LoadTypeSystem{name: "QtTest";              generate: true; unless: "QTJAMBI_NO_TEST"}
    LoadTypeSystem{name: "QtHelp";              generate: true; unless: "QTJAMBI_NO_HELP"}
    LoadTypeSystem{name: "Qt3DCore";            generate: true; unless: "QTJAMBI_NO_QT3DCORE"}
    LoadTypeSystem{name: "Qt3DExtras";          generate: true; unless: "QTJAMBI_NO_QT3DEXTRAS"}
    LoadTypeSystem{name: "Qt3DRender";          generate: true; unless: "QTJAMBI_NO_QT3DRENDER"}
    LoadTypeSystem{name: "Qt3DInput";           generate: true; unless: "QTJAMBI_NO_QT3DINPUT"}
    LoadTypeSystem{name: "Qt3DQuick";           generate: true; unless: "QTJAMBI_NO_QT3DQUICK"}
    LoadTypeSystem{name: "Qt3DQuickRender";     generate: true; unless: "QTJAMBI_NO_QT3DQUICKRENDER"}
    LoadTypeSystem{name: "Qt3DQuickExtras";     generate: true; unless: "QTJAMBI_NO_QT3DQUICKEXTRAS";  since: [5, 7]}
    LoadTypeSystem{name: "Qt3DLogic";           generate: true; unless: "QTJAMBI_NO_QT3DLOGIC"}
    LoadTypeSystem{name: "Qt3DAnimation";       generate: true; unless: "QTJAMBI_NO_QT3DANIMATION";    since: [5, 11]}
    LoadTypeSystem{name: "Qt3DQuickScene2D";    generate: true; unless: "QTJAMBI_NO_QT3DQUICKSCENE2D"; since: [5, 11]}
    LoadTypeSystem{name: "QtWebSockets";        generate: true; unless: "QTJAMBI_NO_WEBSOCKETS"}
    LoadTypeSystem{name: "QtWebChannel";        generate: true; unless: "QTJAMBI_NO_WEBCHANNEL"}
    LoadTypeSystem{name: "QtWebChannelQuick";   generate: true; unless: "QTJAMBI_NO_WEBCHANNELQUICK";  since: 6.6}
    LoadTypeSystem{name: "QtWebEngine";         generate: true; unless: "QTJAMBI_NO_WEBENGINE";        until: 5}
    LoadTypeSystem{name: "QtWebEngineQuick";    generate: true; unless: "QTJAMBI_NO_WEBENGINEQUICK";   since: 6}
    LoadTypeSystem{name: "QtWebEngineWidgets";  generate: true; unless: "QTJAMBI_NO_WEBENGINEWIDGETS"}
    LoadTypeSystem{name: "QtWebEngineCore";     generate: true; unless: "QTJAMBI_NO_WEBENGINECORE"}
    LoadTypeSystem{name: "QtWebView";           generate: true; unless: "QTJAMBI_NO_WEBVIEW"}
    LoadTypeSystem{name: "QtGamepad";           generate: true; unless: "QTJAMBI_NO_GAMEPAD"}
    LoadTypeSystem{name: "QtRemoteObjects";     generate: true; unless: "QTJAMBI_NO_REMOTEOBJECTS"}
    LoadTypeSystem{name: "QtScxml";             generate: true; unless: "QTJAMBI_NO_SCXML";            since: [5, 11]}
    LoadTypeSystem{name: "QtBluetooth";         generate: true; unless: "QTJAMBI_NO_BLUETOOTH";        since: [5, 11]}
    LoadTypeSystem{name: "QtLocation";          generate: true; unless: "QTJAMBI_NO_LOCATION";         since: [5, 11]}
    LoadTypeSystem{name: "QtPositioning";       generate: true; unless: "QTJAMBI_NO_POSITIONING";      since: [5, 11]}
    LoadTypeSystem{name: "QtNfc";               generate: true; unless: "QTJAMBI_NO_NFC";              since: [5, 11]}
    LoadTypeSystem{name: "QtPurchasing";        generate: true; unless: "QTJAMBI_NO_PURCHASING";       since: [5, 11]}
    LoadTypeSystem{name: "QtTextToSpeech";      generate: true; unless: "QTJAMBI_NO_TEXTTOSPEECH";     since: [5, 11]}
    LoadTypeSystem{name: "QtSensors";           generate: true; unless: "QTJAMBI_NO_SENSORS";          since: [5, 11]}
    LoadTypeSystem{name: "QtUiTools";           generate: true; unless: "QTJAMBI_NO_UITOOLS"}
    LoadTypeSystem{name: "QtDesigner";          generate: true; unless: "QTJAMBI_NO_DESIGNER"}
    LoadTypeSystem{name: "ui4";                 generate: true; unless: "QTJAMBI_NO_DESIGNER"}
    LoadTypeSystem{name: "QtActiveX";           generate: true; unless: "QTJAMBI_NO_ACTIVEX"}
    LoadTypeSystem{name: "QtDataVisualization"; generate: true; unless: "QTJAMBI_NO_DATA_VISUALIZATION"}
    LoadTypeSystem{name: "QtCharts";            generate: true; unless: "QTJAMBI_NO_CHARTS"}
    LoadTypeSystem{name: "QtBodymovin";         generate: true; unless: "QTJAMBI_NO_LOTTIE"}
    LoadTypeSystem{name: "QtVirtualKeyboard";   generate: true; unless: "QTJAMBI_NO_VIRTUAL_KEYBOARD"}
    LoadTypeSystem{name: "QtQuick3D";           generate: true; unless: "QTJAMBI_NO_QUICK3D"}
    LoadTypeSystem{name: "QtStateMachine";      generate: true; unless: "QTJAMBI_NO_STATEMACHINE";     since: [6, 1]}
    LoadTypeSystem{name: "QtPdf";               generate: true; unless: "QTJAMBI_NO_PDF"}
    LoadTypeSystem{name: "QtPdfWidgets";        generate: true; unless: "QTJAMBI_NO_PDFWIDGETS"}
    LoadTypeSystem{name: "QtHttpServer";        generate: true; unless: "QTJAMBI_NO_HTTPSERVER";       since: [6, 4]}
    LoadTypeSystem{name: "QtSpatialAudio";      generate: true; unless: "QTJAMBI_NO_SPATIALAUDIO";     since: [6, 4]}
    LoadTypeSystem{name: "QtGrpc";              generate: true; unless: "QTJAMBI_NO_GRPC";             since: [6, 5]}
    LoadTypeSystem{name: "QtProtobuf";          generate: true; unless: "QTJAMBI_NO_PROTOBUF";         since: [6, 5]}
}
