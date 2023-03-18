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

package io.qt.tools.ant;

public abstract class Constants {

    public static final String DEFAULT_QTJAMBI_SONAME_VERSION_MAJOR = "1";

    /*
     * These properties are set outside of this task
     *
     * TODO: These flags should be documented here and if possibly, outside in
     * build documentation.
     * Or rather these binds shouldn't exist, how much of this could be moved to
     * xml side?
     */
    public static final String QMAKE                    = "qtjambi.qt.qmake";
    public static final String QMAKE_ABSPATH            = "qtjambi.qt.qmake.abspath";
    public static final String BINDIR                   = "qtjambi.qt.bindir";
    public static final String LIBDIR                   = "qtjambi.qt.libdir";
    public static final String LIBEXECDIR               = "qtjambi.qt.libexecdir";
    public static final String INCLUDEDIR               = "qtjambi.qt.includedir";
    public static final String DOCSDIR                  = "qtjambi.qt.docdir";
    public static final String PLUGINSDIR               = "qtjambi.qt.pluginsdir";
    public static final String IMPORTSDIR               = "qtjambi.qt.importsdir";
    public static final String QMLMODULESDIR            = "qtjambi.qt.qmlmodulesdir";
    public static final String TRANSLATIONSDIR          = "qtjambi.qt.translationsdir";
    public static final String RESOURCESDIR             = "qtjambi.qt.resourcesdir";
    public static final String GENERATOR_PREPROC_DEFINES = "qtjambi.generator.preproc.defines";
    public static final String GENERATOR_PREPROC_FILE    = "qtjambi.generator.preproc.file";
    public static final String GENERATOR_STATICLIBS      = "qtjambi.generator.staticlibs";
    public static final String JAVALIBDIR               = "qtjambi.java.library.path";
    public static final String JAMBILIBDIR              = "qtjambi.jambi.libdir";
    public static final String JAMBIPLUGINSDIR          = "qtjambi.jambi.pluginsdir";
    public static final String JAMBIIMPORTSDIR          = "qtjambi.jambi.importsdir";
    public static final String CACHEKEY                 = "qtjambi.version.cachekey";
    public static final String VERSION                  = "qtjambi.version";
    public static final String BUNDLE_VERSION           = "qtjambi.version.bundle";
    public static final String BUNDLE_VERSION_MODE      = "qtjambi.version.bundle.mode";
    public static final String SUFFIX_VERSION           = "qtjambi.version.suffix";
    public static final String JAVA_HOME_TARGET         = "java.home.target"; // build
    public static final String JAVA8_HOME_TARGET        = "java8.home.target"; // build
    public static final String JAVA_ARM64_HOME_TARGET   = "java.arm64.home.target"; // build
    public static final String JAVA_X64_HOME_TARGET     = "java.x64.home.target"; // build
    public static final String JAVA_OSARCH_TARGET       = "java.osarch.target"; // target
    public static final String JAVA_OSCPU               = "java.oscpu"; // build
    public static final String JAVA_OSCPU_TARGET        = "java.oscpu.target"; // target
    public static final String EXEC_STRIP               = "exec.strip";
    public static final String MAC_OS_CONVERT_QT_FRAMEWORK  = "macos.convert.qt.frameworks";
    public static final String MAC_OS_USE_FRAMEWORK     = "macos.frameworks";
    public static final String PRIVATE_MODULES          = "qtjambi.private.modules";

    public static final String QT_VERSION_MAJOR         = "qt.version.major";
    public static final String QT_VERSION_MINOR         = "qt.version.minor";
    public static final String QT_VERSION_PATCHLEVEL    = "qt.version.patchlevel";
    public static final String QT_VERSION               = "qt.version";
    public static final String QT_SPEC                  = "qt.qmakespec";

    public static final String QT_VERSION_MAJOR_NEXT    = "qt.version.major.next";
    public static final String QT_VERSION_MINOR_NEXT    = "qt.version.minor.next";

    public static final String CONFIG_RELEASE           = "release";
    public static final String CONFIG_DEBUG             = "debug";
    public static final String CONFIG_TEST              = "test";
    public static final String CONFIG_DEBUG_AND_RELEASE = "debug_and_release";

    public static final String QMAKE_TARGET_DEFAULT     = "qtjambi.qmake.target.default";
    public static final String MAC_OS_GENERATE_FRAMEWORKS         = "qtjambi.macos.frameworks";
    public static final String GENERATOR_INCLUDEPATHS   = "generator.includepaths";

    // Cross compiling, for example (host = intel x86, target = arm)
    public static final String TOOLS_BINDIR             = "tools.qt.bindir";
    public static final String TOOLS_LIBDIR             = "tools.qt.libdir";
    public static final String TOOLS_QMAKE              = "tools.qt.qmake";
    public static final String TOOLS_QMAKE_ABSPATH      = "tools.qt.qmake.abspath";
    public static final String TOOLS_QMAKESPEC          = "tools.qt.qmakespec";
    public static final String TOOLS_COMPILER           = "tools.compiler";

    /*
     * This is needed for Linux/Unix/MacOSX so that the bundled item filename matches the
     *  one referenced by the dynamic linker.
     * This is not needed on Windows.
     */
    public static final String QTJAMBI_SONAME_VERSION_MAJOR   = "qtjambi.soname.version.major";
    public static final String QTJAMBI_SONAME_VERSION_MINOR   = "qtjambi.soname.version.minor";
    public static final String QTJAMBI_QT_LIBINFIX			  = "qtjambi.qt.libinfix";
    public static final String QTJAMBI_LIBINFIX			  	  = "qtjambi.libinfix";
    public static final String QTJAMBI_ABIS  			  	  = "qtjambi.android.abis";

    /*
     * These properties are set inside this task
     */
    public static final String COMPILER           = "qtjambi.compiler";
    public static final String CONFIG             = "qtjambi.config";
    public static final String CONFIGURATION      = "qtjambi.configuration";
    public static final String CONFIGURATION_DASH = "qtjambi.configuration.dash";
    public static final String CONFIGURATION_OSGI = "qtjambi.configuration.osgi";
    public static final String POSITIONINGQUICK   = "qtjambi.positioningquick";
    public static final String QML_MODELS         = "qtjambi.qmlmodels";
    public static final String QML_WORKERSCRIPT   = "qtjambi.qmlworkerscript";
    public static final String QUICKPARTICLES     = "qtjambi.quickparticles";
    public static final String QUICKTEMPLATES2    = "qtjambi.quicktemplates2";
    public static final String QUICKTEST          = "qtjambi.quicktest";
    public static final String SENSORS            = "qtjambi.sensors";
    public static final String MULTIMEDIAQUICK    = "qtjambi.multimediaquick";
    public static final String MULTIMEDIAQUICK_P  = "qtjambi.multimediaquick_p";
    public static final String MULTIMEDIAWIDGETS  = "qtjambi.multimediawidgets";
    public static final String MULTIMEDIAGSTTOOLS = "qtjambi.multimediagsttools";
    public static final String LIBEGL             = "qtjambi.libegl";
    public static final String LIBGLESV2          = "qtjambi.libglesv2";
    public static final String LIBICUDATA         = "qtjambi.libicudata";
    public static final String LIBICUI18N         = "qtjambi.libicui18n";
    public static final String LIBICUIO           = "qtjambi.libicuio";
    public static final String LIBICULE           = "qtjambi.libicule";
    public static final String LIBICULX           = "qtjambi.libiculx";
    public static final String LIBICUTEST         = "qtjambi.libicutest";
    public static final String LIBICUTO           = "qtjambi.libicutu";
    public static final String LIBICUUC           = "qtjambi.libicuuc";
    public static final String OPENGLEXTENSIONS   = "qtjambi.openglextensions";
    public static final String PLATFORMSUPPORT    = "qtjambi.platformsupport";
    public static final String OSNAME             = "qtjambi.osname";
    public static final String OSPLATFORM         = "qtjambi.osplatform";   // linux windows macosx
    public static final String OSCPU              = "qtjambi.oscpu";        // i386 x86_64 x86 x32
    public static final String LABSTEMPLATES      = "qtjambi.labstemplates";
    public static final String ANGLE              = "qtjambi.angle";
    public static final String EGLDEVICEINTEGRATION = "qtjambi.egldeviceintegration";
    public static final String WAYLANDCLIENT 	  = "qtjambi.waylandclient";
    public static final String QTJAMBI_MODULES    = "qtjambi.modules";
    public static final String QTJAMBI_FORCE_DEBUG_INFO    = "qtjambi.force.debug.info";

    public static final String PLUGINS_ACCESSIBLE_QTACCESSIBLEWIDGETS  = "qtjambi.plugins.accessible.qtaccessiblewidgets";

    public static final String PLUGINS_BEARER_CONNMANBEARER       = "qtjambi.plugins.bearer.connmanbearer";
    public static final String PLUGINS_BEARER_GENERICBEARER       = "qtjambi.plugins.bearer.genericbearer";
    public static final String PLUGINS_BEARER_NATIVEWIFIBEARER    = "qtjambi.plugins.bearer.nativewifibearer";
    public static final String PLUGINS_BEARER_NMBEARER            = "qtjambi.plugins.bearer.nmbearer";

    public static final String PLUGINS_CODECS_CNCODECS      = "qtjambi.plugins.codecs.cncodecs";
    public static final String PLUGINS_CODECS_JPCODECS      = "qtjambi.plugins.codecs.jpcodecs";
    public static final String PLUGINS_CODECS_KRCODECS      = "qtjambi.plugins.codecs.krcodecs";
    public static final String PLUGINS_CODECS_TWCODECS      = "qtjambi.plugins.codecs.twcodecs";

    public static final String PLUGINS_DESIGNER_ARTHURPLUGIN         = "qtjambi.plugins.designer.arthurplugin";
    public static final String PLUGINS_DESIGNER_CONTAINEREXTENSION   = "qtjambi.plugins.designer.containerextension";
    public static final String PLUGINS_DESIGNER_CUSTOMWIDGETPLUGIN   = "qtjambi.plugins.designer.customwidgetplugin";
    public static final String PLUGINS_DESIGNER_QAXWIDGET            = "qtjambi.plugins.designer.qaxwidget";
    public static final String PLUGINS_DESIGNER_QDECLARATIVEVIEW     = "qtjambi.plugins.designer.qdeclarativeview";
    public static final String PLUGINS_DESIGNER_QWEBVIEW             = "qtjambi.plugins.designer.qwebview";
    public static final String PLUGINS_DESIGNER_TASKMENUEXTENSION    = "qtjambi.plugins.designer.taskmenuextension";
    public static final String PLUGINS_DESIGNER_WORLDTIMECLOCKPLUGIN = "qtjambi.plugins.designer.worldtimeclockplugin";

    public static final String PLUGINS_GRAPHICSSYSTEMS_GLGRAPHICSSYSTEM    = "qtjambi.plugins.graphicssystems.glgraphicssystem";
    public static final String PLUGINS_GRAPHICSSYSTEMS_TRACEGRAPHICSSYSTEM = "qtjambi.plugins.graphicssystems.tracegraphicssystem";

    public static final String PLUGINS_ICONENGINES_SVGICON  = "qtjambi.plugins.iconengines.svgicon";

    public static final String PLUGINS_IMAGEFORMATS_GIF     = "qtjambi.plugins.imageformats.gif";
    public static final String PLUGINS_IMAGEFORMATS_ICO     = "qtjambi.plugins.imageformats.ico";
    public static final String PLUGINS_IMAGEFORMATS_JPEG    = "qtjambi.plugins.imageformats.jpeg";
    public static final String PLUGINS_IMAGEFORMATS_MNG     = "qtjambi.plugins.imageformats.mng";
    // PNG not seen in wild due to being statically linked into Qt DSOs
    public static final String PLUGINS_IMAGEFORMATS_PNG     = "qtjambi.plugins.imageformats.png";
    public static final String PLUGINS_IMAGEFORMATS_SVG     = "qtjambi.plugins.imageformats.svg";
    public static final String PLUGINS_IMAGEFORMATS_TIFF    = "qtjambi.plugins.imageformats.tiff";

    public static final String PLUGINS_INPUTMETHODS_IMSW_MULTI    = "qtjambi.plugins.inputmethods.imsw-multi";

    public static final String PLUGINS_QMLTOOLING_QMLDBG_TCP      = "qtjambi.plugins.qmltooling.qmldbg_tcp";
    public static final String PLUGINS_SCENEFORMATS_QSCENEAI      = "qtjambi.plugins.sceneformats.qsceneai";
    public static final String PLUGINS_SCENEFORMATS_QSCENEBEZIER  = "qtjambi.plugins.sceneformats.qscenebezier";
    public static final String PLUGINS_SCENEFORMATS_QSCENEOBJ     = "qtjambi.plugins.sceneformats.qsceneobj";
    

    public static final String PLUGINS_SCRIPT_QTSCRIPTDBUS = "qtjambi.plugins.script.qtscriptdbus";

    public static final String PLUGINS_SQLDRIVERS_SQLITE    = "qtjambi.plugins.sqldrivers.sqlite";
    public static final String PLUGINS_SQLDRIVERS_SQLITE2   = "qtjambi.plugins.sqldrivers.sqlite2";
    public static final String PLUGINS_SQLDRIVERS_SQLMYSQL  = "qtjambi.plugins.sqldrivers.sqlmysql";
    public static final String PLUGINS_SQLDRIVERS_SQLODBC   = "qtjambi.plugins.sqldrivers.sqlodbc";
    public static final String PLUGINS_SQLDRIVERS_SQLPSQL   = "qtjambi.plugins.sqldrivers.sqlpsql";
    public static final String PLUGINS_SQLDRIVERS_SQLTDS    = "qtjambi.plugins.sqldrivers.sqltds";

    public static final String PLUGINS_AUDIO_QTAUDIO_WINDOWS = "qtjambi.plugins.audio.qtaudio_windows";
    public static final String PLUGINS_AUDIO_QTAUDIO_ALSA   = "qtjambi.plugins.audio.qtaudio_alsa";
    public static final String PLUGINS_AUDIO_QTAUDIO_COREAUDIO   = "qtjambi.plugins.audio.qtaudio_coreaudio";
    public static final String PLUGINS_AUDIO_QTMEDIA_PULSE   = "qtjambi.plugins.audio.qtmedia_pulse";
    public static final String PLUGINS_GENERIC_QTUIOTOUCHPLUGIN = "qtjambi.plugins.generic.qtuiotouchplugin";
    public static final String PLUGINS_IMAGEFORMATS_QJP2 = "qtjambi.plugins.imageformats.qjp2";
    public static final String PLUGINS_IMAGEFORMATS_QICNS = "qtjambi.plugins.imageformats.qicns";
    public static final String PLUGINS_IMAGEFORMATS_QDDS = "qtjambi.plugins.imageformats.qdds";
    public static final String PLUGINS_IMAGEFORMATS_QTGA = "qtjambi.plugins.imageformats.qtga";
    public static final String PLUGINS_IMAGEFORMATS_QWBMP = "qtjambi.plugins.imageformats.qwbmp";
    public static final String PLUGINS_IMAGEFORMATS_QWEBP = "qtjambi.plugins.imageformats.qwebp";
    public static final String PLUGINS_MEDIASERVICE_DSENGINE = "qtjambi.plugins.mediaservice.dsengine";
    public static final String PLUGINS_MEDIASERVICE_QTMEDIA_AUDIOENGINE = "qtjambi.plugins.mediaservice.qtmedia_audioengine";
    public static final String PLUGINS_MEDIASERVICE_WMFENGINE = "qtjambi.plugins.mediaservice.wmfengine";
    public static final String PLUGINS_MEDIASERVICE_GSTAUDIODECODER = "qtjambi.plugins.mediaservice.gstaudiodecoder";
    public static final String PLUGINS_MEDIASERVICE_GSTCAMERABIN = "qtjambi.plugins.mediaservice.gstcamerabin";
    public static final String PLUGINS_MEDIASERVICE_GSTMEDIACAPTURE = "qtjambi.plugins.mediaservice.gstmediacapture";
    public static final String PLUGINS_MEDIASERVICE_GSTMEDIAPLAYER = "qtjambi.plugins.mediaservice.gstmediaplayer";
    public static final String PLUGINS_MEDIASERVICE_AVFMEDIAPLAYER = "qtjambi.plugins.mediaservice.qavfmediaplayer";
    public static final String PLUGINS_MEDIASERVICE_AVFCAMERA = "qtjambi.plugins.mediaservice.qavfcamera";
    public static final String PLUGINS_PLATFORMS_QMINIMAL = "qtjambi.plugins.platforms.qminimal";
    public static final String PLUGINS_PLATFORMS_QOFFSCREEN = "qtjambi.plugins.platforms.qoffscreen";
    public static final String PLUGINS_PLATFORMS_QMINIMALEGL = "qtjambi.plugins.platforms.qminimalegl";
    public static final String PLUGINS_PLATFORMS_QWAYLAND_EGL = "qtjambi.plugins.platforms.qwayland-egl";
    public static final String PLUGINS_PLATFORMS_QWAYLAND_GENERIC = "qtjambi.plugins.platforms.qwayland-generic";
    public static final String PLUGINS_PLATFORMS_QWINDOWS = "qtjambi.plugins.platforms.qwindows";
    public static final String PLUGINS_PLATFORMS_QLINUXFB = "qtjambi.plugins.platforms.qlinuxfb";
    public static final String PLUGINS_PLATFORMS_QEGLFS = "qtjambi.plugins.platforms.qeglfs";
    public static final String PLUGINS_PLATFORMS_QWEBGL = "qtjambi.plugins.platforms.qwebgl";
    public static final String PLUGINS_PLATFORMS_QCOCOA = "qtjambi.plugins.platforms.qcocoa";
    public static final String PLUGINS_PLATFORMS_QXCB = "qtjambi.plugins.platforms.qxcb";
    public static final String PLUGINS_PLAYLISTFORMATS_QTMULTIMEDIA_M3U = "qtjambi.plugins.playlistformats.qtmultimedia_m3u";
    public static final String PLUGINS_POSITION_QTPOSITION_POSITIONPOLL = "qtjambi.plugins.position.qtposition_positionpoll";
    public static final String PLUGINS_PRINTSUPPORT_WINDOWSPRINTERSUPPORT = "qtjambi.plugins.printsupport.windowsprintersupport";
    public static final String PLUGINS_PRINTSUPPORT_CUPSPRINTERSUPPORT = "qtjambi.plugins.printsupport.cupsprintersupport";
    public static final String PLUGINS_PRINTSUPPORT_COCOAPRINTERSUPPORT = "qtjambi.plugins.printsupport.cocoaprintersupport";
    public static final String PLUGINS_QML1TOOLING_QMLDBG_INSPECTOR = "qtjambi.plugins.qml1tooling.qmldbg_inspector";
    public static final String PLUGINS_QML1TOOLING_QMLDBG_TCP_QTDECLARATIVE = "qtjambi.plugins.qml1tooling.qmldbg_tcp_qtdeclarative";
    public static final String PLUGINS_QMLTOOLING_QMLDBG_QTQUICK2 = "qtjambi.plugins.qmltooling.qmldbg_qtquick2";
    public static final String PLUGINS_SENSORGESTURES_QTSENSORGESTURES_PLUGIN = "qtjambi.plugins.sensorgestures.qtsensorgestures_plugin";
    public static final String PLUGINS_SENSORGESTURES_QTSENSORGESTURES_SHAKEPLUGIN = "qtjambi.plugins.sensorgestures.qtsensorgestures_shakeplugin";
    public static final String PLUGINS_SENSORS_QTSENSORS_GENERIC = "qtjambi.plugins.sensors.qtsensors_generic";
    public static final String PLUGINS_VIDEO_VIDEONODE_EGLVIDEONODE = "qtjambi.plugins.video.videonode.eglvideonode";
    public static final String PLUGINS_GEOSERVICES_QTGEOSERVICES_MAPBOX = "qtjambi.plugins.geoservices.qtgeoservices_mapbox";
    public static final String PLUGINS_GEOSERVICES_QTGEOSERVICES_NOKIA = "qtjambi.plugins.geoservices.qtgeoservices_nokia";
    public static final String PLUGINS_GEOSERVICES_QTGEOSERVICES_OSM = "qtjambi.plugins.geoservices.qtgeoservices_osm";
    public static final String PLUGINS_STYLES_QWINDOWSVISTASTYLE = "qtjambi.plugins.styles.qwindowsvistastyle";
    public static final String PLUGINS_STYLES_QMACSTYLE = "qtjambi.plugins.styles.qmacstyle";
    public static final String PLUGINS_GAMEPAD_DARWIN_GAMEPAD = "qtjambi.plugins.gamepad.darwingamepad";
    
    public static final String QML_QT_LABS_FOLDERLISTMODEL_QMLFOLDERLISTMODELPLUGIN = "qtjambi.Qt.labs.folderlistmodel.qmlfolderlistmodelplugin";
    public static final String QML_QT_LABS_SETTINGS_QMLSETTINGSPLUGIN = "qtjambi.Qt.labs.settings.qmlsettingsplugin";
    public static final String QML_QT3D_QUICK3DCOREPLUGIN = "qtjambi.Qt3D.quick3dcoreplugin";
    public static final String QML_Qt3D_INPUT_QUICK3DINPUTPLUGIN = "qtjambi.Qt3D.Input.quick3dinputplugin";
    public static final String QML_Qt3D_RENDERER_QUICK3DRENDERERPLUGIN = "qtjambi.Qt3D.Renderer.quick3drendererplugin";
    public static final String QML_QtBLUETOOTH_DECLARATIVE_BLUETOOTH = "qtjambi.QtBluetooth.declarative_bluetooth";
    public static final String QML_QTCANVAS3D_QTCANVAS3D_DESIGNER = "qtjambi.QtCanvas3D.designer.qtcanvas3d";
    public static final String QML_QTLOCATION_DECLARATIVE_LOCATION = "qtjambi.QtLocation.declarative_location";
    public static final String QML_QTMULTIMEDIA_DECLARATIVE_MULTIMEDIA = "qtjambi.QtMultimedia.declarative_multimedia";
    public static final String QML_QTNFC_DECLARATIVE_NFC = "qtjambi.QtNfc.declarative_nfc";
    public static final String QML_QTPOSITIONING_DECLARATIVE_POSITIONING = "qtjambi.QtPositioning.declarative_positioning";
    public static final String QML_QTQML_MODELS_2_MODELSPLUGIN = "qtjambi.QtQml.Models.2.modelsplugin";
    public static final String QML_QTQML_MODELS_MODELSPLUGIN = "qtjambi.QtQml.Models.modelsplugin";
    public static final String QML_QTQML_STATEMACHINE_QTQMLSTATEMACHINE = "qtjambi.QtQml.StateMachine.qtqmlstatemachine";
    public static final String QML_QTQUICK_CONTROLS_STYLES_FLAT_QTQUICKEXTRASFLATPLUGIN = "qtjambi.QtQuick.Controls.Styles.Flat.qtquickextrasflatplugin";
    public static final String QML_QTQUICK_CONTROLS_QTQUICKCONTROLSPLUGIN = "qtjambi.QtQuick.Controls.qtquickcontrolsplugin";
    public static final String QML_QTQUICK_DIALOGS_DIALOGPLUGIN = "qtjambi.QtQuick.Dialogs.dialogplugin";
    public static final String QML_QTQUICK_DIALOGS_IMAGES = "qtjambi.QtQuick.Dialogs.images";
    public static final String QML_QTQUICK_DIALOGS_QML = "qtjambi.QtQuick.Dialogs.qml";
    public static final String QML_QTQUICK_DIALOGS_PRIVATE_DIALOGSPRIVATEPLUGIN = "qtjambi.QtQuick.Dialogs.Private.dialogsprivateplugin";
    public static final String QML_QTQUICK_EXTRAS_QTQUICKEXTRASPLUGIN = "qtjambi.QtQuick.Extras.qtquickextrasplugin";
    public static final String QML_QTQUICK_LAYOUTS_QQUICKLAYOUTSPLUGIN = "qtjambi.QtQuick.Layouts.qquicklayoutsplugin";
    public static final String QML_QTQUICK_LOCALSTORAGE_QMLLOCALSTORAGEPLUGIN = "qtjambi.QtQuick.LocalStorage.qmllocalstorageplugin";
    public static final String QML_QTQUICK_PARTICLES_2_PARTICLESPLUGIN = "qtjambi.QtQuick.Particles.2.particlesplugin";
    public static final String QML_QTQUICK_PARTICLES_PARTICLESPLUGIN = "qtjambi.QtQuick.Particles.particlesplugin";
    public static final String QML_QTQUICK_PRIVATEWIDGETS_WIDGETSPLUGIN = "qtjambi.QtQuick.PrivateWidgets.widgetsplugin";
    public static final String QML_QTQUICK_SCENE3D_QTQUICKSCENE3DPLUGIN = "qtjambi.QtQuick.Scene3D.qtquickscene3dplugin";
    public static final String QML_QTQUICK_WINDOW_2_WINDOWPLUGIN = "qtjambi.QtQuick.Window.2.windowplugin";
    public static final String QML_QTQUICK_WINDOW_WINDOWPLUGIN = "qtjambi.QtQuick.Window.windowplugin";
    public static final String QML_QTQUICK_XMLLISTMODEL_QMLXMLLISTMODELPLUGIN = "qtjambi.QtQuick.XmlListModel.qmlxmllistmodelplugin";
    public static final String QML_QTQUICK_2_QTQUICK2PLUGIN = "qtjambi.QtQuick.2.qtquick2plugin";
    public static final String QML_QTQUICK_QTQUICK2PLUGIN = "qtjambi.QtQuick.qtquick2plugin";
    public static final String QML_QTSENSORS_DECLARATIVE_SENSORS = "qtjambi.QtSensors.declarative_sensors";
    public static final String QML_QTWEBCHANNEL_DECLARATIVE_WEBCHANNEL = "qtjambi.QtWebChannel.declarative_webchannel";
    public static final String QML_QTWEBSOCKETS_DECLARATIVE_QMLWEBSOCKETS = "qtjambi.QtWebSockets.declarative_qmlwebsockets";
    public static final String QML_QTWINEXTRAS_QML_WINEXTRAS = "qtjambi.QtWinExtras.qml_winextras";
    public static final String QML_QTGRAPHICALEFFECTS = "qtjambi.QtGraphicalEffects";
    public static final String QML_QTAUDIOENGINE_DECLARATIVE_AUDIOENGINE = "qtjambi.QtAudioEngine.declarative_audioengine";
    
    public static final String QTJAMBI_CONFIG_ISMACOSX      = "qtjambi.config.ismacosx";
    public static final String QTJAMBI_MACOSX_QTMENUNIB_DIR = "qtjambi.macosx.qtmenunib.dir";
    public static final String QTJAMBI_MACOSX_MAC_SDK       = "qtjambi.macosx.macsdk";

    // Windows specific vars...
    public static final String VSINSTALLDIR        = "qtjambi.vsinstalldir";
    public static final String VSREDISTDIR         = "qtjambi.vsredistdir";
    public static final String VSREDISTDIR_PACKAGE = "qtjambi.vsredistdir.package";

    public static final String QTJAMBI_DEBUG_TOOLS         = "qtjambi.debug-tools";
    public static final String QTJAMBI_QREALTYPE           = "qtjambi.qrealtype";

    // Initialize these to empty string if unset.
    public static final String QTJAMBI_CONFIG_JUMPTABLE    = "qtjambi.config.jumptable";
    public static final String QTJAMBI_GENERATOR_JUMPTABLE = "qtjambi.generator.jumptable";
}
