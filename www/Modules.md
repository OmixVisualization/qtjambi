# QtJambi Modules

[![Maven Central](https://maven-badges.herokuapp.com/maven-central/io.qtjambi/qtjambi/badge.svg)](https://search.maven.org/artifact/io.qtjambi/qtjambi)

QtJambi is available for Qt5 and Qt6. All modules are published as Maven Artifact. Add them to your software development project by specifying as follows:

```xml
<dependency>
  <groupId>io.qtjambi</groupId>
  <artifactId>$MODULE</artifactId>
  <version>$VERSION</version>
</dependency>
```
Here, exchange `$VERSION` either by `5.15.18`, by `6.5.4` or by `6.6.1`. Exchange `$MODULE` by *QtJambi* module name as listed below.

Alternatively, you can download corresponding QtJambi modules as JAR file from [Maven Central Repository](https://search.maven.org/search?q=io.qtjambi).

All *QtJambi* modules consist of a Java part (minimum java version: JDK8) and a native component. 
Native components are avaiable for Windows (x64), Linux (x64), macOS (x64 and arm64) and Android (x86, x86_64, armeabi-v7a, arm64-v8a). 
Debug info for native debugging is avaiable for Windows, Linux and macOS.

## Requirements and Compatibility

**The native components for Windows available on Maven require Qt binaries for MSVC (msvc2019_64). Mingw Qt is not compatible with QtJambi unless you build it from scratch with Mingw.**
**The native components for Linux could be incompatible with distribution-specific Qt. Maven-published binaries have been compiled with Qt available from Qt installer.**

Search for qtjambi modules by name in Maven repository. Each java module requires the corresponding platform bundle. Therefore search for either 
**<module>-native-windows-x64**, **<module>-native-linux-x64**, **<module>-native-macos** or **<module>-native-android**. 
Info files for native debugging can be found in **<module>-debuginfo-windows-x64**, **<module>-debuginfo-linux-x64** and **<module>-debuginfo-macos**.

QtJambi 6.6.1 requires Qt 6.6.x whereas x can be any patch version greater or equals 0 (except **qtjambi-activex**).
This release has been built with Qt 6.6.1. Qt 6.6.0 is not binary compatible to higher versions.

QtJambi 6.5.4 requires Qt 6.5.x whereas x can be any patch version less or equals 3 (except **qtjambi-activex**).
This release has been built with Qt 6.5.3. Qt 6.5.3 is most likely not binary compatible to higher versions.

QtJambi 5.15.18 requires Qt 5.15.x whereas x can be any patch version (except **qtjambi-activex**).
This release has been built with Qt 5.15.2.

## Qt Essentials

Following Qt modules are provided by QtJambi:

* [QtCore](https://doc.qt.io/qt-6/qtcore-index.html), [QtGui](https://doc.qt.io/qt-6/qtgui-index.html) and [QtWidgets](https://doc.qt.io/qt-6/qtwidgets-index.html) in module **qtjambi**:
    * [**qtjambi-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi/6.5.4/jar)
    * [**qtjambi-native-windows-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-windows-x64/6.5.4/jar)
    * [**qtjambi-debuginfo-windows-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-debuginfo-windows-x64/6.5.4/jar)
    * [**qtjambi-native-linux-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-linux-x64/6.5.4/jar)
    * [**qtjambi-debuginfo-linux-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-debuginfo-linux-x64/6.5.4/jar)
    * [**qtjambi-native-macos-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-macos/6.5.4/jar)
    * [**qtjambi-debuginfo-macos-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-debuginfo-macos/6.5.4/jar)
    * [**qtjambi-native-android-x86-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-android-x86/6.5.4/jar)
    * [**qtjambi-native-android-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-android-x64/6.5.4/jar)
    * [**qtjambi-native-android-arm-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-android-arm/6.5.4/jar)
    * [**qtjambi-native-android-arm64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-android-arm64/6.5.4/jar)
* [QtDBus](https://doc.qt.io/qt-6/qtdbus-index.html) in module **qtjambi-dbus**:
* [QtNetwork](https://doc.qt.io/qt-6/qtnetwork-index.html) in module **qtjambi-network**:
* [QtQml](https://doc.qt.io/qt-6/qtqml-index.html) in module **qtjambi-qml**:
* [QtQuick](https://doc.qt.io/qt-6/qtquick-index.html) in module **qtjambi-quick**:
* [QtQuickControls](https://doc.qt.io/qt-6/qtquickcontrols-index.html) in module **qtjambi-quickcontrols**:
* [QtQuickTest](https://doc.qt.io/qt-6/qtquicktest-index.html) in module **qtjambi-quicktest**:
* [QtTest](https://doc.qt.io/qt-6/qttest-index.html) in module **qtjambi-test**:

## Qt Add-Ons

* [QtBluetooth](https://doc.qt.io/qt-6/qtbluetooth-index.html) in module **qtjambi-bluetooth**:
* [QtCharts](https://doc.qt.io/qt-6/qtcharts-index.html) in module **qtjambi-charts**:
* [QtDataVisualization](https://doc.qt.io/qt-6/qtdatavisualization-index.html) in module **qtjambi-datavisualization**:
* [QtConcurrent](https://doc.qt.io/qt-6/qtconcurrent-index.html) in module **qtjambi-concurrent**:
* [QtHelp](https://doc.qt.io/qt-6/qthelp-index.html) in module **qtjambi-help**:
* [QtOpenGL](https://doc.qt.io/qt-6/qtopengl-index.html) in module **qtjambi-opengl**:
* [QtOpenGLWidgets](https://doc.qt.io/qt-6/qtopenglwidgets-module.html) in module **qtjambi-openglwidgets**:
* [QtMultimedia](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimedia**:
* [QtMultimediaWidgets](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimediawidgets**:
* [QtNfc](https://doc.qt.io/qt-6/qtnfc-index.html) in module **qtjambi-nfc**:
* [QtNetworkAuth](https://doc.qt.io/qt-6/qtnetworkauth-index.html) in module **qtjambi-networkauth**:
* [QtPositioning](https://doc.qt.io/qt-6/qtpositioning-index.html) in module **qtjambi-positioning**:
* [QtPrintSupport](https://doc.qt.io/qt-6/qtprintsupport-index.html) in module **qtjambi-printsupport**:
* [QtQuick3D](https://doc.qt.io/qt-6/qtquick3d-index.html) in module **qtjambi-quick3d**:
* [QtQuickWidgets](https://doc.qt.io/qt-6/qtquickwidgets-module.html) in module **qtjambi-quickwidgets**:
* [QtRemoteObjects](https://doc.qt.io/qt-6/qtremoteobjects-module.html) in module **qtjambi-remoteobjects**:
* [QtSCXML](https://doc.qt.io/qt-6/qtscxml-index.html) in module **qtjambi-scxml**:
* [QtSensors](https://doc.qt.io/qt-6/qtsensors-index.html) in module **qtjambi-sensors**:
* [QtSerialBus](https://doc.qt.io/qt-6/qtserialbus-index.html) in module **qtjambi-serialbus**:
* [QtSerialPort](https://doc.qt.io/qt-6/qtserialport-index.html) in module **qtjambi-serialport**:
* [QtSql](https://doc.qt.io/qt-6/qtsql-index.html) in module **qtjambi-sql**:
* [QtSvg](https://doc.qt.io/qt-6/qtsvg-index.html) in module **qtjambi-svg**:
* [QtSvgWidgets](https://doc.qt.io/qt-6/qtsvg-index.html) in module **qtjambi-svgwidgets**:
* [QtStateMachine](https://doc.qt.io/qt-6/qtstatemachine-index.html) in module **qtjambi-statemachine**:
* [QtUITools](https://doc.qt.io/qt-6/qtuitools-index.html) in module **qtjambi-uitools**:
* [QtWebChannel](https://doc.qt.io/qt-6/qtwebchannel-index.html) in module **qtjambi-webchannel**:
* [QtWebEngineCore](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginecore**:
* [QtWebEngineWidgets](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginewidgets**:
* [QtWebEngineQuick](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginequick**:
* [QtWebSockets](https://doc.qt.io/qt-6/qtwebsockets-index.html) in module **qtjambi-websockets**:
* [QtWebView](https://doc.qt.io/qt-6/qtwebview-index.html) in module **qtjambi-webview**:
* [QtVirtualKeyboard](https://doc.qt.io/qt-6/qtvirtualkeyboard-index.html) in module **qtjambi-virtualkeyboard**:
* [QtXml](https://doc.qt.io/qt-6/qtxml-index.html) in module **qtjambi-xml**:
* [QtDesigner](https://doc.qt.io/qt-6/qtdesigner-manual.html) in module **qtjambi-designer**:
* [QtPDF](https://doc.qt.io/qt-6/qtpdf-index.html) in modules **qtjambi-pdf** and **qtjambi-pdfwidgets**:
* [QtHttpServer](https://doc.qt.io/qt-6/qthttpserver-index.html) in module **qtjambi-httpserver**

## Qt Platform-dependent Add-Ons

* [Active Qt (Windows only)](https://doc.qt.io/qt-6/activeqt-manual.html) in module **qtjambi-activex**:

## Qt3D

* [Qt3DCore](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dcore**:
* [Qt3DRender](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3drender**:
* [Qt3DAnimation](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3danimation**:
* [Qt3DExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dextras**:
* [Qt3DInput](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dinput**:
* [Qt3DLogic](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dlogic**:
* [Qt3DQuick](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquick**:
* [Qt3DQuickExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickextras**:
* [Qt3DQuickScene3D](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickscene2d**:

## QtJambi Plugins

* JDBC Sql Plugin:
    * [**qtjambi-plugin-qsqljdbc-windows-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-windows-x64/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-linux-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-linux-x64/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-macos-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-macos/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-native-android-common-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-native-android-common/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-native-android-x86-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-native-android-x86/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-native-android-x86-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-native-android-x86/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-native-android-x64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-native-android-x64/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-native-android-arm-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-native-android-arm/6.5.4/jar)
    * [**qtjambi-plugin-qsqljdbc-native-android-arm64-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-plugin-qsqljdbc-native-android-arm64/6.5.4/jar)

## QtJambi Tools

* [QtJambi User Interface Compiler (UIC)](https://doc.qt.io/qt-6/designer-using-a-ui-file.html#compile-time-form-processing):
    * [**qtjambi-uic-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic/6.5.4/jar)
    * patform bundles for **windows-x64**, **linux-x64** and **macos**
* QtJambi Deployer:
    * [**qtjambi-deployer-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer/6.5.4/jar)
    * patform bundles for **windows-x64**, **linux-x64**, **macos** and **android**
* QtJambi Generator:
    * [**qtjambi-generator-6.5.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-generator/6.5.4/jar)
    * patform bundles for **windows-x64**, **linux-x64** and **macos**
