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
Here, exchange `$VERSION` either by `5.15.4` or by `6.2.2`, exchange `$MODULE` by *QtJambi* module name as listed below.

Alternatively, you can download corresponding QtJambi modules as JAR file from [Maven Central Repository](https://search.maven.org/search?q=io.qtjambi).

All *QtJambi* modules consist of a Java part optionally for JDK8 or any higher JDK and a native component. 
Native components are avaiable for Windows (x64), Linux (x64) and macOS (x64 and arm64). 

## QtJambi for Qt 6.2

QtJambi 6.2.2 requires Qt 6.2.x whereas x can be any patch version.
Following Qt 6.2 modules are provided by QtJambi:

### Qt6 Essentials

* [QtCore](https://doc.qt.io/qt-6/qtcore-index.html), [QtGui](https://doc.qt.io/qt-6/qtgui-index.html) and [QtWidgets](https://doc.qt.io/qt-6/qtwidgets-index.html) in module **qtjambi**:
    * [**qtjambi-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi/6.2.2/jar)
    * [**qtjambi-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-jre8/6.2.2/jar)
    * [**qtjambi-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-platform-macos/6.2.2/jar)
* [QtDBus](https://doc.qt.io/qt-6/qtdbus-index.html) in module **qtjambi-dbus**:
    * [**qtjambi-dbus-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus/6.2.2/jar)
    * [**qtjambi-dbus-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-jre8/6.2.2/jar)
    * [**qtjambi-dbus-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-dbus-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-dbus-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-platform-macos/6.2.2/jar)
* [QtNetwork](https://doc.qt.io/qt-6/qtnetwork-index.html) in module **qtjambi-network**:
    * [**qtjambi-network-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network/6.2.2/jar)
    * [**qtjambi-network-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-jre8/6.2.2/jar)
    * [**qtjambi-network-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-network-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-network-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-platform-macos/6.2.2/jar)
* [QtQml](https://doc.qt.io/qt-6/qtqml-index.html) in module **qtjambi-qml**:
    * [**qtjambi-qml-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml/6.2.2/jar)
    * [**qtjambi-qml-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-jre8/6.2.2/jar)
    * [**qtjambi-qml-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qml-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qml-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-platform-macos/6.2.2/jar)
* [QtQuick](https://doc.qt.io/qt-6/qtquick-index.html) in module **qtjambi-quick**:
    * [**qtjambi-quick-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick/6.2.2/jar)
    * [**qtjambi-quick-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-jre8/6.2.2/jar)
    * [**qtjambi-quick-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-quick-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-quick-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-platform-macos/6.2.2/jar)
* [QtQuickControls](https://doc.qt.io/qt-6/qtquickcontrols-index.html) in module **qtjambi-quickcontrols**:
    * [**qtjambi-quickcontrols-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols/6.2.2/jar)
    * [**qtjambi-quickcontrols-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-jre8/6.2.2/jar)
    * [**qtjambi-quickcontrols-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-quickcontrols-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-quickcontrols-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-platform-macos/6.2.2/jar)
* [QtTest](https://doc.qt.io/qt-6/qttest-index.html) in module **qtjambi-test**:
    * [**qtjambi-test-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test/6.2.2/jar)
    * [**qtjambi-test-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-jre8/6.2.2/jar)
    * [**qtjambi-test-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-test-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-test-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-platform-macos/6.2.2/jar)

### Qt6 Add-Ons

* [QtBluetooth](https://doc.qt.io/qt-6/qtbluetooth-index.html) in module **qtjambi-bluetooth**:
    * [**qtjambi-bluetooth-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth/6.2.2/jar)
    * [**qtjambi-bluetooth-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-jre8/6.2.2/jar)
    * [**qtjambi-bluetooth-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-bluetooth-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-bluetooth-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-platform-macos/6.2.2/jar)
* [QtCharts](https://doc.qt.io/qt-6/qtcharts-index.html) in module **qtjambi-charts**:
    * [**qtjambi-charts-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts/6.2.2/jar)
    * [**qtjambi-charts-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-jre8/6.2.2/jar)
    * [**qtjambi-charts-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-charts-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-charts-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-platform-macos/6.2.2/jar)
* [QtDataVisualization](https://doc.qt.io/qt-6/qtdatavisualization-index.html) in module **qtjambi-datavisualization**:
    * [**qtjambi-datavisualization-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization/6.2.2/jar)
    * [**qtjambi-datavisualization-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-jre8/6.2.2/jar)
    * [**qtjambi-datavisualization-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-datavisualization-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-datavisualization-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-platform-macos/6.2.2/jar)
* [QtConcurrent](https://doc.qt.io/qt-6/qtconcurrent-index.html) in module **qtjambi-concurrent**:
    * [**qtjambi-concurrent-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent/6.2.2/jar)
    * [**qtjambi-concurrent-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-jre8/6.2.2/jar)
    * [**qtjambi-concurrent-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-concurrent-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-concurrent-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-platform-macos/6.2.2/jar)
* [QtHelp](https://doc.qt.io/qt-6/qthelp-index.html) in module **qtjambi-help**:
    * [**qtjambi-help-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help/6.2.2/jar)
    * [**qtjambi-help-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-jre8/6.2.2/jar)
    * [**qtjambi-help-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-help-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-help-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-platform-macos/6.2.2/jar)
* [QtOpenGL](https://doc.qt.io/qt-6/qtopengl-index.html) in module **qtjambi-opengl**:
    * [**qtjambi-opengl-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl/6.2.2/jar)
    * [**qtjambi-opengl-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-jre8/6.2.2/jar)
    * [**qtjambi-opengl-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-opengl-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-opengl-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-platform-macos/6.2.2/jar)
* [QtOpenGLWidgets](https://doc.qt.io/qt-6/qtopenglwidgets-module.html) in module **qtjambi-openglwidgets**:
    * [**qtjambi-openglwidgets-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets/6.2.2/jar)
    * [**qtjambi-openglwidgets-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-jre8/6.2.2/jar)
    * [**qtjambi-openglwidgets-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-openglwidgets-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-openglwidgets-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-platform-macos/6.2.2/jar)
* [QtMultimedia](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimedia**:
    * [**qtjambi-multimedia-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia/6.2.2/jar)
    * [**qtjambi-multimedia-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-jre8/6.2.2/jar)
    * [**qtjambi-multimedia-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-multimedia-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-multimedia-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-platform-macos/6.2.2/jar)
* [QtMultimediaWidgets](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimediawidgets**:
    * [**qtjambi-multimediawidgets-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets/6.2.2/jar)
    * [**qtjambi-multimediawidgets-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-jre8/6.2.2/jar)
    * [**qtjambi-multimediawidgets-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-multimediawidgets-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-multimediawidgets-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-platform-macos/6.2.2/jar)
* [QtNfc](https://doc.qt.io/qt-6/qtnfc-index.html) in module **qtjambi-nfc**:
    * [**qtjambi-nfc-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc/6.2.2/jar)
    * [**qtjambi-nfc-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-jre8/6.2.2/jar)
    * [**qtjambi-nfc-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-nfc-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-nfc-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-platform-macos/6.2.2/jar)
* [QtNetworkAuth](https://doc.qt.io/qt-6/qtnetworkauth-index.html) in module **qtjambi-networkauth**:
    * [**qtjambi-networkauth-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth/6.2.2/jar)
    * [**qtjambi-networkauth-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-jre8/6.2.2/jar)
    * [**qtjambi-networkauth-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-networkauth-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-networkauth-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-platform-macos/6.2.2/jar)
* [QtPositioning](https://doc.qt.io/qt-6/qtpositioning-index.html) in module **qtjambi-positioning**:
    * [**qtjambi-positioning-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning/6.2.2/jar)
    * [**qtjambi-positioning-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-jre8/6.2.2/jar)
    * [**qtjambi-positioning-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-positioning-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-positioning-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-platform-macos/6.2.2/jar)
* [QtPrintSupport](https://doc.qt.io/qt-6/qtprintsupport-index.html) in module **qtjambi-printsupport**:
    * [**qtjambi-printsupport-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport/6.2.2/jar)
    * [**qtjambi-printsupport-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-jre8/6.2.2/jar)
    * [**qtjambi-printsupport-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-printsupport-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-printsupport-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-platform-macos/6.2.2/jar)
* [QtQuick3D](https://doc.qt.io/qt-6/qtquick3d-index.html) in module **qtjambi-quick3d**:
    * [**qtjambi-quick3d-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d/6.2.2/jar)
    * [**qtjambi-quick3d-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-jre8/6.2.2/jar)
    * [**qtjambi-quick3d-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-quick3d-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-quick3d-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-platform-macos/6.2.2/jar)
* [QtQuickWidgets](https://doc.qt.io/qt-6/qtquickwidgets-module.html) in module **qtjambi-quickwidgets**:
    * [**qtjambi-quickwidgets-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets/6.2.2/jar)
    * [**qtjambi-quickwidgets-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-jre8/6.2.2/jar)
    * [**qtjambi-quickwidgets-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-quickwidgets-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-quickwidgets-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-platform-macos/6.2.2/jar)
* [QtRemoteObjects](https://doc.qt.io/qt-6/qtremoteobjects-module.html) in module **qtjambi-remoteobjects**:
    * [**qtjambi-remoteobjects-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects/6.2.2/jar)
    * [**qtjambi-remoteobjects-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-jre8/6.2.2/jar)
    * [**qtjambi-remoteobjects-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-remoteobjects-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-remoteobjects-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-platform-macos/6.2.2/jar)
* [QtSCXML](https://doc.qt.io/qt-6/qtscxml-index.html) in module **qtjambi-scxml**:
    * [**qtjambi-scxml-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml/6.2.2/jar)
    * [**qtjambi-scxml-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-jre8/6.2.2/jar)
    * [**qtjambi-scxml-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-scxml-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-scxml-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-platform-macos/6.2.2/jar)
* [QtSensors](https://doc.qt.io/qt-6/qtsensors-index.html) in module **qtjambi-sensors**:
    * [**qtjambi-sensors-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors/6.2.2/jar)
    * [**qtjambi-sensors-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-jre8/6.2.2/jar)
    * [**qtjambi-sensors-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-sensors-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-sensors-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-platform-macos/6.2.2/jar)
* [QtSerialBus](https://doc.qt.io/qt-6/qtserialbus-index.html) in module **qtjambi-serialbus**:
    * [**qtjambi-serialbus-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus/6.2.2/jar)
    * [**qtjambi-serialbus-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-jre8/6.2.2/jar)
    * [**qtjambi-serialbus-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-serialbus-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-serialbus-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-platform-macos/6.2.2/jar)
* [QtSerialPort](https://doc.qt.io/qt-6/qtserialport-index.html) in module **qtjambi-serialport**:
    * [**qtjambi-serialport-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport/6.2.2/jar)
    * [**qtjambi-serialport-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-jre8/6.2.2/jar)
    * [**qtjambi-serialport-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-serialport-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-serialport-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-platform-macos/6.2.2/jar)
* [QtSql](https://doc.qt.io/qt-6/qtsql-index.html) in module **qtjambi-sql**:
    * [**qtjambi-sql-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql/6.2.2/jar)
    * [**qtjambi-sql-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-jre8/6.2.2/jar)
    * [**qtjambi-sql-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-sql-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-sql-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-platform-macos/6.2.2/jar)
* [QtSvg](https://doc.qt.io/qt-6/qtsvg-index.html) in module **qtjambi-svg**:
    * [**qtjambi-svg-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg/6.2.2/jar)
    * [**qtjambi-svg-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-jre8/6.2.2/jar)
    * [**qtjambi-svg-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-svg-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-svg-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-platform-macos/6.2.2/jar)
* [QtStateMachine](https://doc.qt.io/qt-6/qtstatemachine-index.html) in module **qtjambi-statemachine**:
    * [**qtjambi-statemachine-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine/6.2.2/jar)
    * [**qtjambi-statemachine-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-jre8/6.2.2/jar)
    * [**qtjambi-statemachine-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-statemachine-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-statemachine-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-platform-macos/6.2.2/jar)
* [QtUITools](https://doc.qt.io/qt-6/qtuitools-index.html) in module **qtjambi-uitools**:
    * [**qtjambi-uitools-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools/6.2.2/jar)
    * [**qtjambi-uitools-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-jre8/6.2.2/jar)
    * [**qtjambi-uitools-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-uitools-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-uitools-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-platform-macos/6.2.2/jar)
* [QtWebChannel](https://doc.qt.io/qt-6/qtwebchannel-index.html) in module **qtjambi-webchannel**:
    * [**qtjambi-webchannel-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel/6.2.2/jar)
    * [**qtjambi-webchannel-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-jre8/6.2.2/jar)
    * [**qtjambi-webchannel-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-webchannel-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-webchannel-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-platform-macos/6.2.2/jar)
* [QtWebEngineCore](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginecore**:
    * [**qtjambi-webenginecore-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore/6.2.2/jar)
    * [**qtjambi-webenginecore-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-jre8/6.2.2/jar)
    * [**qtjambi-webenginecore-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-webenginecore-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-webenginecore-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-platform-macos/6.2.2/jar)
* [QtWebEngineWidgets](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginewidgets**:
    * [**qtjambi-webenginewidgets-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets/6.2.2/jar)
    * [**qtjambi-webenginewidgets-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-jre8/6.2.2/jar)
    * [**qtjambi-webenginewidgets-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-webenginewidgets-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-webenginewidgets-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-platform-macos/6.2.2/jar)
* [QtWebEngineQuick](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginequick**:
    * [**qtjambi-webenginequick-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick/6.2.2/jar)
    * [**qtjambi-webenginequick-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-jre8/6.2.2/jar)
    * [**qtjambi-webenginequick-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-webenginequick-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-webenginequick-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-platform-macos/6.2.2/jar)
* [QtWebSockets](https://doc.qt.io/qt-6/qtwebsockets-index.html) in module **qtjambi-websockets**:
    * [**qtjambi-websockets-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets/6.2.2/jar)
    * [**qtjambi-websockets-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-jre8/6.2.2/jar)
    * [**qtjambi-websockets-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-websockets-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-websockets-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-platform-macos/6.2.2/jar)
* [QtWebView](https://doc.qt.io/qt-6/qtwebview-index.html) in module **qtjambi-webview**:
    * [**qtjambi-webview-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview/6.2.2/jar)
    * [**qtjambi-webview-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-jre8/6.2.2/jar)
    * [**qtjambi-webview-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-webview-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-webview-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-platform-macos/6.2.2/jar)
* [QtVirtualKeyboard](https://doc.qt.io/qt-6/qtvirtualkeyboard-index.html) in module **qtjambi-virtualkeyboard**:
    * [**qtjambi-virtualkeyboard-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard/6.2.2/jar)
    * [**qtjambi-virtualkeyboard-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-jre8/6.2.2/jar)
    * [**qtjambi-virtualkeyboard-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-virtualkeyboard-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-virtualkeyboard-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-platform-macos/6.2.2/jar)
* [QtXml](https://doc.qt.io/qt-6/qtxml-index.html) in module **qtjambi-xml**:
    * [**qtjambi-xml-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml/6.2.2/jar)
    * [**qtjambi-xml-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-jre8/6.2.2/jar)
    * [**qtjambi-xml-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-xml-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-xml-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-platform-macos/6.2.2/jar)
* [QtDesigner](https://doc.qt.io/qt-6/qtdesigner-manual.html) in module **qtjambi-designer**:
    * [**qtjambi-designer-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer/6.2.2/jar)
    * [**qtjambi-designer-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-jre8/6.2.2/jar)
    * [**qtjambi-designer-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-designer-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-designer-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-platform-macos/6.2.2/jar)

### Qt3D

* [Qt3DCore](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dcore**:
    * [**qtjambi-qt3dcore-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore/6.2.2/jar)
    * [**qtjambi-qt3dcore-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-jre8/6.2.2/jar)
    * [**qtjambi-qt3dcore-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3dcore-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3dcore-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-platform-macos/6.2.2/jar)
* [Qt3DRender](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3drender**:
    * [**qtjambi-qt3drender-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender/6.2.2/jar)
    * [**qtjambi-qt3drender-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-jre8/6.2.2/jar)
    * [**qtjambi-qt3drender-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3drender-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3drender-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-platform-macos/6.2.2/jar)
* [Qt3DAnimation](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3danimation**:
    * [**qtjambi-qt3danimation-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation/6.2.2/jar)
    * [**qtjambi-qt3danimation-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-jre8/6.2.2/jar)
    * [**qtjambi-qt3danimation-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3danimation-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3danimation-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-platform-macos/6.2.2/jar)
* [Qt3DExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dextras**:
    * [**qtjambi-qt3dextras-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras/6.2.2/jar)
    * [**qtjambi-qt3dextras-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-jre8/6.2.2/jar)
    * [**qtjambi-qt3dextras-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3dextras-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3dextras-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-platform-macos/6.2.2/jar)
* [Qt3DInput](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dinput**:
    * [**qtjambi-qt3dinput-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput/6.2.2/jar)
    * [**qtjambi-qt3dinput-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-jre8/6.2.2/jar)
    * [**qtjambi-qt3dinput-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3dinput-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3dinput-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-platform-macos/6.2.2/jar)
* [Qt3DLogic](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dlogic**:
    * [**qtjambi-qt3dlogic-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic/6.2.2/jar)
    * [**qtjambi-qt3dlogic-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-jre8/6.2.2/jar)
    * [**qtjambi-qt3dlogic-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3dlogic-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3dlogic-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-platform-macos/6.2.2/jar)
* [Qt3DQuick](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquick**:
    * [**qtjambi-qt3dquick-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick/6.2.2/jar)
    * [**qtjambi-qt3dquick-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-jre8/6.2.2/jar)
    * [**qtjambi-qt3dquick-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3dquick-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3dquick-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-platform-macos/6.2.2/jar)
* [Qt3DQuickExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickextras**:
    * [**qtjambi-qt3dquickextras-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras/6.2.2/jar)
    * [**qtjambi-qt3dquickextras-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-jre8/6.2.2/jar)
    * [**qtjambi-qt3dquickextras-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3dquickextras-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3dquickextras-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-platform-macos/6.2.2/jar)
* [Qt3DQuickScene3D](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickscene2d**:
    * [**qtjambi-qt3dquickscene2d-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d/6.2.2/jar)
    * [**qtjambi-qt3dquickscene2d-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-jre8/6.2.2/jar)
    * [**qtjambi-qt3dquickscene2d-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-qt3dquickscene2d-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-qt3dquickscene2d-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-platform-macos/6.2.2/jar)

### QtJambi Tools

* QtJambi Deployer:
    * [**qtjambi-deployer-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer/6.2.2/jar)
    * [**qtjambi-deployer-jre8-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-jre8/6.2.2/jar)
    * [**qtjambi-deployer-platform-windows-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-platform-windows-x64/6.2.2/jar)
    * [**qtjambi-deployer-platform-linux-x64-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-platform-linux-x64/6.2.2/jar)
    * [**qtjambi-deployer-platform-macos-6.2.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-platform-macos/6.2.2/jar)


## QtJambi for Qt 5.15

QtJambi 5.15.4 requires Qt 5.15.x whereas x can be any patch version.
Following Qt 5.15 modules are provided by QtJambi:

### Qt5 Essentials

* [QtCore](https://doc.qt.io/qt-5/qtcore-index.html), [QtGui](https://doc.qt.io/qt-5/qtgui-index.html) and [QtWidgets](https://doc.qt.io/qt-5/qtwidgets-index.html) in module **qtjambi**:
    * [**qtjambi-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi/5.15.4/jar)
    * [**qtjambi-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-jre8/5.15.4/jar)
    * [**qtjambi-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-platform-macos/5.15.4/jar)
* [QtMultimedia](https://doc.qt.io/qt-5/qtmultimedia-index.html) in module **qtjambi-multimedia**:
    * [**qtjambi-multimedia-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia/5.15.4/jar)
    * [**qtjambi-multimedia-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-jre8/5.15.4/jar)
    * [**qtjambi-multimedia-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-multimedia-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-multimedia-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-platform-macos/5.15.4/jar)
* [QtMultimediaWidgets](https://doc.qt.io/qt-5/qtmultimedia-index.html) in module **qtjambi-multimediawidgets**:
    * [**qtjambi-multimediawidgets-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets/5.15.4/jar)
    * [**qtjambi-multimediawidgets-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-jre8/5.15.4/jar)
    * [**qtjambi-multimediawidgets-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-multimediawidgets-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-multimediawidgets-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-platform-macos/5.15.4/jar)
* [QtNetwork](https://doc.qt.io/qt-5/qtnetwork-index.html) in module **qtjambi-network**:
    * [**qtjambi-network-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network/5.15.4/jar)
    * [**qtjambi-network-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-jre8/5.15.4/jar)
    * [**qtjambi-network-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-network-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-network-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-platform-macos/5.15.4/jar)
* [QtQml](https://doc.qt.io/qt-5/qtqml-index.html) in module **qtjambi-qml**:
    * [**qtjambi-qml-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml/5.15.4/jar)
    * [**qtjambi-qml-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-jre8/5.15.4/jar)
    * [**qtjambi-qml-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qml-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qml-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-platform-macos/5.15.4/jar)
* [QtQuick](https://doc.qt.io/qt-5/qtquick-index.html) in module **qtjambi-quick**:
    * [**qtjambi-quick-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick/5.15.4/jar)
    * [**qtjambi-quick-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-jre8/5.15.4/jar)
    * [**qtjambi-quick-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-quick-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-quick-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-platform-macos/5.15.4/jar)
* [QtQuickControls](https://doc.qt.io/qt-5/qtquickcontrols-index.html) in module **qtjambi-quickcontrols**:
    * [**qtjambi-quickcontrols-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols/5.15.4/jar)
    * [**qtjambi-quickcontrols-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-jre8/5.15.4/jar)
    * [**qtjambi-quickcontrols-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-quickcontrols-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-quickcontrols-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-platform-macos/5.15.4/jar)
* [QtSql](https://doc.qt.io/qt-5/qtsql-index.html) in module **qtjambi-sql**:
    * [**qtjambi-sql-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql/5.15.4/jar)
    * [**qtjambi-sql-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-jre8/5.15.4/jar)
    * [**qtjambi-sql-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-sql-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-sql-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-platform-macos/5.15.4/jar)
* [QtTest](https://doc.qt.io/qt-5/qttest-index.html) in module **qtjambi-test**:
    * [**qtjambi-test-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test/5.15.4/jar)
    * [**qtjambi-test-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-jre8/5.15.4/jar)
    * [**qtjambi-test-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-test-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-test-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-platform-macos/5.15.4/jar)

### Qt5 Add-Ons

* [QtBluetooth](https://doc.qt.io/qt-5/qtbluetooth-index.html) in module **qtjambi-bluetooth**:
    * [**qtjambi-bluetooth-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth/5.15.4/jar)
    * [**qtjambi-bluetooth-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-jre8/5.15.4/jar)
    * [**qtjambi-bluetooth-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-bluetooth-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-bluetooth-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-platform-macos/5.15.4/jar)
* [QtConcurrent](https://doc.qt.io/qt-5/qtconcurrent-index.html) in module **qtjambi-concurrent**:
    * [**qtjambi-concurrent-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent/5.15.4/jar)
    * [**qtjambi-concurrent-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-jre8/5.15.4/jar)
    * [**qtjambi-concurrent-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-concurrent-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-concurrent-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-platform-macos/5.15.4/jar)
* [QtDBus](https://doc.qt.io/qt-5/qtdbus-index.html) in module **qtjambi-dbus**:
    * [**qtjambi-dbus-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus/5.15.4/jar)
    * [**qtjambi-dbus-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-jre8/5.15.4/jar)
    * [**qtjambi-dbus-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-dbus-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-dbus-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-platform-macos/5.15.4/jar)
* [QtGamepad](https://doc.qt.io/qt-5/qtgamepad-index.html) in module **qtjambi-gamepad**:
    * [**qtjambi-gamepad-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad/5.15.4/jar)
    * [**qtjambi-gamepad-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-jre8/5.15.4/jar)
    * [**qtjambi-gamepad-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-gamepad-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-gamepad-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-platform-macos/5.15.4/jar)
* [QtHelp](https://doc.qt.io/qt-5/qthelp-index.html) in module **qtjambi-help**:
    * [**qtjambi-help-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help/5.15.4/jar)
    * [**qtjambi-help-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-jre8/5.15.4/jar)
    * [**qtjambi-help-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-help-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-help-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-platform-macos/5.15.4/jar)
* [QtLocation](https://doc.qt.io/qt-5/qtlocation-index.html) in module **qtjambi-location**:
    * [**qtjambi-location-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location/5.15.4/jar)
    * [**qtjambi-location-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-jre8/5.15.4/jar)
    * [**qtjambi-location-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-location-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-location-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-platform-macos/5.15.4/jar)
* [QtNfc](https://doc.qt.io/qt-5/qtnfc-index.html) in module **qtjambi-nfc**:
    * [**qtjambi-nfc-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc/5.15.4/jar)
    * [**qtjambi-nfc-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-jre8/5.15.4/jar)
    * [**qtjambi-nfc-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-nfc-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-nfc-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-platform-macos/5.15.4/jar)
* [QtPositioning](https://doc.qt.io/qt-5/qtpositioning-index.html) in module **qtjambi-positioning**:
    * [**qtjambi-positioning-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning/5.15.4/jar)
    * [**qtjambi-positioning-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-jre8/5.15.4/jar)
    * [**qtjambi-positioning-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-positioning-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-positioning-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-platform-macos/5.15.4/jar)
* [QtPrintSupport](https://doc.qt.io/qt-5/qtprintsupport-index.html) in module **qtjambi-printsupport**:
    * [**qtjambi-printsupport-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport/5.15.4/jar)
    * [**qtjambi-printsupport-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-jre8/5.15.4/jar)
    * [**qtjambi-printsupport-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-printsupport-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-printsupport-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-platform-macos/5.15.4/jar)
* [QtPurchasing](https://doc.qt.io/qt-5/qtpurchasing-index.html) in module **qtjambi-purchasing**:
    * [**qtjambi-purchasing-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing/5.15.4/jar)
    * [**qtjambi-purchasing-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-jre8/5.15.4/jar)
    * [**qtjambi-purchasing-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-purchasing-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-purchasing-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-platform-macos/5.15.4/jar)
* [QtQuickWidgets](https://doc.qt.io/qt-5/qtquickwidgets-module.html) in module **qtjambi-quickwidgets**:
    * [**qtjambi-quickwidgets-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets/5.15.4/jar)
    * [**qtjambi-quickwidgets-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-jre8/5.15.4/jar)
    * [**qtjambi-quickwidgets-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-quickwidgets-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-quickwidgets-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-platform-macos/5.15.4/jar)
* [QtRemoteObjects](https://doc.qt.io/qt-5/qtremoteobjects-module.html) in module **qtjambi-remoteobjects**:
    * [**qtjambi-remoteobjects-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects/5.15.4/jar)
    * [**qtjambi-remoteobjects-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-jre8/5.15.4/jar)
    * [**qtjambi-remoteobjects-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-remoteobjects-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-remoteobjects-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-platform-macos/5.15.4/jar)
* [QtScript](https://doc.qt.io/qt-5/qtscript-module.html) in module **qtjambi-script**:
    * [**qtjambi-script-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script/5.15.4/jar)
    * [**qtjambi-script-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-jre8/5.15.4/jar)
    * [**qtjambi-script-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-script-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-script-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-platform-macos/5.15.4/jar)
* [QtScriptTools](https://doc.qt.io/qt-5/qtscripttools-module.html) in module **qtjambi-scripttools**:
    * [**qtjambi-scripttools-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools/5.15.4/jar)
    * [**qtjambi-scripttools-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-jre8/5.15.4/jar)
    * [**qtjambi-scripttools-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-scripttools-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-scripttools-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-platform-macos/5.15.4/jar)
* [QtSCXML](https://doc.qt.io/qt-5/qtscxml-index.html) in module **qtjambi-scxml**:
    * [**qtjambi-scxml-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml/5.15.4/jar)
    * [**qtjambi-scxml-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-jre8/5.15.4/jar)
    * [**qtjambi-scxml-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-scxml-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-scxml-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-platform-macos/5.15.4/jar)
* [QtSensors](https://doc.qt.io/qt-5/qtsensors-index.html) in module **qtjambi-sensors**:
    * [**qtjambi-sensors-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors/5.15.4/jar)
    * [**qtjambi-sensors-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-jre8/5.15.4/jar)
    * [**qtjambi-sensors-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-sensors-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-sensors-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-platform-macos/5.15.4/jar)
* [QtSerialBus](https://doc.qt.io/qt-5/qtserialbus-index.html) in module **qtjambi-serialbus**:
    * [**qtjambi-serialbus-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus/5.15.4/jar)
    * [**qtjambi-serialbus-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-jre8/5.15.4/jar)
    * [**qtjambi-serialbus-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-serialbus-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-serialbus-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-platform-macos/5.15.4/jar)
* [QtSerialPort](https://doc.qt.io/qt-5/qtserialport-index.html) in module **qtjambi-serialport**:
    * [**qtjambi-serialport-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport/5.15.4/jar)
    * [**qtjambi-serialport-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-jre8/5.15.4/jar)
    * [**qtjambi-serialport-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-serialport-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-serialport-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-platform-macos/5.15.4/jar)
* [QtSvg](https://doc.qt.io/qt-5/qtsvg-index.html) in module **qtjambi-svg**:
    * [**qtjambi-svg-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg/5.15.4/jar)
    * [**qtjambi-svg-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-jre8/5.15.4/jar)
    * [**qtjambi-svg-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-svg-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-svg-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-platform-macos/5.15.4/jar)
* [QtTextToSpeech](https://doc.qt.io/qt-5/qtspeech-index.html) in module **qtjambi-texttospeech**:
    * [**qtjambi-texttospeech-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech/5.15.4/jar)
    * [**qtjambi-texttospeech-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-jre8/5.15.4/jar)
    * [**qtjambi-texttospeech-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-texttospeech-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-texttospeech-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-platform-macos/5.15.4/jar)
* [QtUITools](https://doc.qt.io/qt-5/qtuitools-index.html) in module **qtjambi-uitools**:
    * [**qtjambi-uitools-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools/5.15.4/jar)
    * [**qtjambi-uitools-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-jre8/5.15.4/jar)
    * [**qtjambi-uitools-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-uitools-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-uitools-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-platform-macos/5.15.4/jar)
* [QtWebChannel](https://doc.qt.io/qt-5/qtwebchannel-index.html) in module **qtjambi-webchannel**:
    * [**qtjambi-webchannel-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel/5.15.4/jar)
    * [**qtjambi-webchannel-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-jre8/5.15.4/jar)
    * [**qtjambi-webchannel-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-webchannel-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-webchannel-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-platform-macos/5.15.4/jar)
* [QtWebEngineCore](https://doc.qt.io/qt-5/qtwebengine-index.html) in module **qtjambi-webenginecore**:
    * [**qtjambi-webenginecore-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore/5.15.4/jar)
    * [**qtjambi-webenginecore-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-jre8/5.15.4/jar)
    * [**qtjambi-webenginecore-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-webenginecore-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-webenginecore-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-platform-macos/5.15.4/jar)
* [QtWebEngineWidgets](https://doc.qt.io/qt-5/qtwebengine-index.html) in module **qtjambi-webenginewidgets**:
    * [**qtjambi-webenginewidgets-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets/5.15.4/jar)
    * [**qtjambi-webenginewidgets-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-jre8/5.15.4/jar)
    * [**qtjambi-webenginewidgets-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-webenginewidgets-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-webenginewidgets-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-platform-macos/5.15.4/jar)
* [QtWebEngine](https://doc.qt.io/qt-5/qtwebengine-index.html) in module **qtjambi-webengine**:
    * [**qtjambi-webengine-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine/5.15.4/jar)
    * [**qtjambi-webengine-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-jre8/5.15.4/jar)
    * [**qtjambi-webengine-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-webengine-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-webengine-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-platform-macos/5.15.4/jar)
* [QtWebSockets](https://doc.qt.io/qt-5/qtwebsockets-index.html) in module **qtjambi-websockets**:
    * [**qtjambi-websockets-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets/5.15.4/jar)
    * [**qtjambi-websockets-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-jre8/5.15.4/jar)
    * [**qtjambi-websockets-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-websockets-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-websockets-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-platform-macos/5.15.4/jar)
* [QtWebView](https://doc.qt.io/qt-5/qtwebview-index.html) in module **qtjambi-webview**:
    * [**qtjambi-webview-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview/5.15.4/jar)
    * [**qtjambi-webview-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-jre8/5.15.4/jar)
    * [**qtjambi-webview-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-webview-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-webview-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-platform-macos/5.15.4/jar)
* [QtXml](https://doc.qt.io/qt-5/qtxml-index.html) in module **qtjambi-xml**:
    * [**qtjambi-xml-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml/5.15.4/jar)
    * [**qtjambi-xml-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-jre8/5.15.4/jar)
    * [**qtjambi-xml-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-xml-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-xml-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-platform-macos/5.15.4/jar)
* [QtXmlPatterns](https://doc.qt.io/qt-5/qtxmlpatterns-index.html) in module **qtjambi-xmlpatterns**:
    * [**qtjambi-xmlpatterns-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns/5.15.4/jar)
    * [**qtjambi-xmlpatterns-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-jre8/5.15.4/jar)
    * [**qtjambi-xmlpatterns-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-xmlpatterns-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-xmlpatterns-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-platform-macos/5.15.4/jar)
* [QtCharts](https://doc.qt.io/qt-5/qtcharts-index.html) in module **qtjambi-charts**:
    * [**qtjambi-charts-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts/5.15.4/jar)
    * [**qtjambi-charts-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-jre8/5.15.4/jar)
    * [**qtjambi-charts-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-charts-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-charts-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-platform-macos/5.15.4/jar)
* [QtDataVisualization](https://doc.qt.io/qt-5/qtdatavisualization-index.html) in module **qtjambi-datavisualization**:
    * [**qtjambi-datavisualization-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization/5.15.4/jar)
    * [**qtjambi-datavisualization-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-jre8/5.15.4/jar)
    * [**qtjambi-datavisualization-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-datavisualization-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-datavisualization-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-platform-macos/5.15.4/jar)
* [QtNetworkAuth](https://doc.qt.io/qt-5/qtnetworkauth-index.html) in module **qtjambi-networkauth**:
    * [**qtjambi-networkauth-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth/5.15.4/jar)
    * [**qtjambi-networkauth-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-jre8/5.15.4/jar)
    * [**qtjambi-networkauth-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-networkauth-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-networkauth-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-platform-macos/5.15.4/jar)
* [QtVirtualKeyboard](https://doc.qt.io/qt-5/qtvirtualkeyboard-index.html) in module **qtjambi-virtualkeyboard**:
    * [**qtjambi-virtualkeyboard-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard/5.15.4/jar)
    * [**qtjambi-virtualkeyboard-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-jre8/5.15.4/jar)
    * [**qtjambi-virtualkeyboard-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-virtualkeyboard-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-virtualkeyboard-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-platform-macos/5.15.4/jar)
* [QtQuick3D](https://doc.qt.io/qt-5/qtquick3d-index.html) in module **qtjambi-quick3d**:
    * [**qtjambi-quick3d-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d/5.15.4/jar)
    * [**qtjambi-quick3d-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-jre8/5.15.4/jar)
    * [**qtjambi-quick3d-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-quick3d-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-quick3d-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-platform-macos/5.15.4/jar)
* [QtDesigner](https://doc.qt.io/qt-5/qtdesigner-manual.html) in module **qtjambi-designer**:
    * [**qtjambi-designer-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer/5.15.4/jar)
    * [**qtjambi-designer-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-jre8/5.15.4/jar)
    * [**qtjambi-designer-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-designer-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-designer-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-platform-macos/5.15.4/jar)

### Qt5 Platform-dependent Add-Ons

* [QtWinExtras](https://doc.qt.io/qt-5/qtwinextras-index.html) in module **qtjambi-winextras**:
    * [**qtjambi-winextras-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-winextras/5.15.4/jar)
    * [**qtjambi-winextras-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-winextras-jre8/5.15.4/jar)
    * [**qtjambi-winextras-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-winextras-platform-windows-x64/5.15.4/jar)
* [QtMacExtras](https://doc.qt.io/qt-5/qtmacextras-index.html) in module **qtjambi-macextras**:
    * [**qtjambi-macextras-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-macextras/5.15.4/jar)
    * [**qtjambi-macextras-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-macextras-jre8/5.15.4/jar)
    * [**qtjambi-macextras-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-macextras-platform-macos/5.15.4/jar)
* [QtX11Extras](https://doc.qt.io/qt-5/qtx11extras-index.html) in module **qtjambi-x11extras**:
    * [**qtjambi-x11extras-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-x11extras/5.15.4/jar)
    * [**qtjambi-x11extras-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-x11extras-jre8/5.15.4/jar)
    * [**qtjambi-x11extras-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-x11extras-platform-linux-x64/5.15.4/jar)

### Qt3D

* [Qt3DCore](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dcore**:
    * [**qtjambi-qt3dcore-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore/5.15.4/jar)
    * [**qtjambi-qt3dcore-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-jre8/5.15.4/jar)
    * [**qtjambi-qt3dcore-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3dcore-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3dcore-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-platform-macos/5.15.4/jar)
* [Qt3DRender](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3drender**:
    * [**qtjambi-qt3drender-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender/5.15.4/jar)
    * [**qtjambi-qt3drender-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-jre8/5.15.4/jar)
    * [**qtjambi-qt3drender-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3drender-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3drender-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-platform-macos/5.15.4/jar)
* [Qt3DAnimation](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3danimation**:
    * [**qtjambi-qt3danimation-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation/5.15.4/jar)
    * [**qtjambi-qt3danimation-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-jre8/5.15.4/jar)
    * [**qtjambi-qt3danimation-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3danimation-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3danimation-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-platform-macos/5.15.4/jar)
* [Qt3DExtras](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dextras**:
    * [**qtjambi-qt3dextras-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras/5.15.4/jar)
    * [**qtjambi-qt3dextras-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-jre8/5.15.4/jar)
    * [**qtjambi-qt3dextras-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3dextras-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3dextras-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-platform-macos/5.15.4/jar)
* [Qt3DInput](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dinput**:
    * [**qtjambi-qt3dinput-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput/5.15.4/jar)
    * [**qtjambi-qt3dinput-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-jre8/5.15.4/jar)
    * [**qtjambi-qt3dinput-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3dinput-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3dinput-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-platform-macos/5.15.4/jar)
* [Qt3DLogic](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dlogic**:
    * [**qtjambi-qt3dlogic-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic/5.15.4/jar)
    * [**qtjambi-qt3dlogic-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-jre8/5.15.4/jar)
    * [**qtjambi-qt3dlogic-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3dlogic-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3dlogic-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-platform-macos/5.15.4/jar)
* [Qt3DQuick](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dquick**:
    * [**qtjambi-qt3dquick-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick/5.15.4/jar)
    * [**qtjambi-qt3dquick-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-jre8/5.15.4/jar)
    * [**qtjambi-qt3dquick-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3dquick-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3dquick-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-platform-macos/5.15.4/jar)
* [Qt3DQuickExtras](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dquickextras**:
    * [**qtjambi-qt3dquickextras-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras/5.15.4/jar)
    * [**qtjambi-qt3dquickextras-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-jre8/5.15.4/jar)
    * [**qtjambi-qt3dquickextras-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3dquickextras-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3dquickextras-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-platform-macos/5.15.4/jar)
* [Qt3DQuickScene3D](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dquickscene2d**:
    * [**qtjambi-qt3dquickscene2d-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d/5.15.4/jar)
    * [**qtjambi-qt3dquickscene2d-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-jre8/5.15.4/jar)
    * [**qtjambi-qt3dquickscene2d-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-qt3dquickscene2d-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-qt3dquickscene2d-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-platform-macos/5.15.4/jar)

### QtJambi Tools

* QtJambi Deployer:
    * [**qtjambi-deployer-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer/5.15.4/jar)
    * [**qtjambi-deployer-jre8-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-jre8/5.15.4/jar)
    * [**qtjambi-deployer-platform-windows-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-platform-windows-x64/5.15.4/jar)
    * [**qtjambi-deployer-platform-linux-x64-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-platform-linux-x64/5.15.4/jar)
    * [**qtjambi-deployer-platform-macos-5.15.4.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-platform-macos/5.15.4/jar)

