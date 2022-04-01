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
Here, exchange `$VERSION` either by `5.15.5`, by `6.2.3` or by `6.3.0`. Exchange `$MODULE` by *QtJambi* module name as listed below.

Alternatively, you can download corresponding QtJambi modules as JAR file from [Maven Central Repository](https://search.maven.org/search?q=io.qtjambi).

All *QtJambi* modules consist of a Java part optionally for JDK8 or any higher JDK and a native component. 
Native components are avaiable for Windows (x64), Linux (x64) and macOS (x64 and arm64). 

## QtJambi for Qt 6.3

QtJambi 6.3.0 requires Qt 6.3.x whereas x can be any patch version.
Following Qt 6.3 modules are provided by QtJambi:

### Qt6 Essentials

* [QtCore](https://doc.qt.io/qt-6/qtcore-index.html), [QtGui](https://doc.qt.io/qt-6/qtgui-index.html) and [QtWidgets](https://doc.qt.io/qt-6/qtwidgets-index.html) in module **qtjambi**:
    * [**qtjambi-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi/6.3.0/jar)
    * [**qtjambi-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-jre8/6.3.0/jar)
    * [**qtjambi-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-windows-x64/6.3.0/jar)
    * [**qtjambi-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-linux-x64/6.3.0/jar)
    * [**qtjambi-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-macos/6.3.0/jar)
* [QtDBus](https://doc.qt.io/qt-6/qtdbus-index.html) in module **qtjambi-dbus**:
    * [**qtjambi-dbus-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus/6.3.0/jar)
    * [**qtjambi-dbus-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-jre8/6.3.0/jar)
    * [**qtjambi-dbus-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-windows-x64/6.3.0/jar)
    * [**qtjambi-dbus-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-linux-x64/6.3.0/jar)
    * [**qtjambi-dbus-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-macos/6.3.0/jar)
* [QtNetwork](https://doc.qt.io/qt-6/qtnetwork-index.html) in module **qtjambi-network**:
    * [**qtjambi-network-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network/6.3.0/jar)
    * [**qtjambi-network-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-jre8/6.3.0/jar)
    * [**qtjambi-network-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-windows-x64/6.3.0/jar)
    * [**qtjambi-network-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-linux-x64/6.3.0/jar)
    * [**qtjambi-network-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-macos/6.3.0/jar)
* [QtQml](https://doc.qt.io/qt-6/qtqml-index.html) in module **qtjambi-qml**:
    * [**qtjambi-qml-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml/6.3.0/jar)
    * [**qtjambi-qml-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-jre8/6.3.0/jar)
    * [**qtjambi-qml-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qml-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qml-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-macos/6.3.0/jar)
* [QtQuick](https://doc.qt.io/qt-6/qtquick-index.html) in module **qtjambi-quick**:
    * [**qtjambi-quick-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick/6.3.0/jar)
    * [**qtjambi-quick-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-jre8/6.3.0/jar)
    * [**qtjambi-quick-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-windows-x64/6.3.0/jar)
    * [**qtjambi-quick-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-linux-x64/6.3.0/jar)
    * [**qtjambi-quick-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-macos/6.3.0/jar)
* [QtQuickControls](https://doc.qt.io/qt-6/qtquickcontrols-index.html) in module **qtjambi-quickcontrols**:
    * [**qtjambi-quickcontrols-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols/6.3.0/jar)
    * [**qtjambi-quickcontrols-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-jre8/6.3.0/jar)
    * [**qtjambi-quickcontrols-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-windows-x64/6.3.0/jar)
    * [**qtjambi-quickcontrols-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-linux-x64/6.3.0/jar)
    * [**qtjambi-quickcontrols-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-macos/6.3.0/jar)
* [QtQuickTest](https://doc.qt.io/qt-6/qtquicktest-index.html) in module **qtjambi-quicktest**:
    * [**qtjambi-quicktest-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest/6.3.0/jar)
    * [**qtjambi-quicktest-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-jre8/6.3.0/jar)
    * [**qtjambi-quicktest-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-windows-x64/6.3.0/jar)
    * [**qtjambi-quicktest-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-linux-x64/6.3.0/jar)
    * [**qtjambi-quicktest-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-macos/6.3.0/jar)
* [QtTest](https://doc.qt.io/qt-6/qttest-index.html) in module **qtjambi-test**:
    * [**qtjambi-test-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test/6.3.0/jar)
    * [**qtjambi-test-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-jre8/6.3.0/jar)
    * [**qtjambi-test-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-windows-x64/6.3.0/jar)
    * [**qtjambi-test-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-linux-x64/6.3.0/jar)
    * [**qtjambi-test-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-macos/6.3.0/jar)

### Qt6 Add-Ons

* [QtBluetooth](https://doc.qt.io/qt-6/qtbluetooth-index.html) in module **qtjambi-bluetooth**:
    * [**qtjambi-bluetooth-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth/6.3.0/jar)
    * [**qtjambi-bluetooth-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-jre8/6.3.0/jar)
    * [**qtjambi-bluetooth-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-windows-x64/6.3.0/jar)
    * [**qtjambi-bluetooth-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-linux-x64/6.3.0/jar)
    * [**qtjambi-bluetooth-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-macos/6.3.0/jar)
* [QtCharts](https://doc.qt.io/qt-6/qtcharts-index.html) in module **qtjambi-charts**:
    * [**qtjambi-charts-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts/6.3.0/jar)
    * [**qtjambi-charts-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-jre8/6.3.0/jar)
    * [**qtjambi-charts-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-windows-x64/6.3.0/jar)
    * [**qtjambi-charts-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-linux-x64/6.3.0/jar)
    * [**qtjambi-charts-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-macos/6.3.0/jar)
* [QtDataVisualization](https://doc.qt.io/qt-6/qtdatavisualization-index.html) in module **qtjambi-datavisualization**:
    * [**qtjambi-datavisualization-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization/6.3.0/jar)
    * [**qtjambi-datavisualization-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-jre8/6.3.0/jar)
    * [**qtjambi-datavisualization-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-windows-x64/6.3.0/jar)
    * [**qtjambi-datavisualization-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-linux-x64/6.3.0/jar)
    * [**qtjambi-datavisualization-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-macos/6.3.0/jar)
* [QtConcurrent](https://doc.qt.io/qt-6/qtconcurrent-index.html) in module **qtjambi-concurrent**:
    * [**qtjambi-concurrent-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent/6.3.0/jar)
    * [**qtjambi-concurrent-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-jre8/6.3.0/jar)
    * [**qtjambi-concurrent-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-windows-x64/6.3.0/jar)
    * [**qtjambi-concurrent-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-linux-x64/6.3.0/jar)
    * [**qtjambi-concurrent-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-macos/6.3.0/jar)
* [QtHelp](https://doc.qt.io/qt-6/qthelp-index.html) in module **qtjambi-help**:
    * [**qtjambi-help-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help/6.3.0/jar)
    * [**qtjambi-help-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-jre8/6.3.0/jar)
    * [**qtjambi-help-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-windows-x64/6.3.0/jar)
    * [**qtjambi-help-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-linux-x64/6.3.0/jar)
    * [**qtjambi-help-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-macos/6.3.0/jar)
* [QtOpenGL](https://doc.qt.io/qt-6/qtopengl-index.html) in module **qtjambi-opengl**:
    * [**qtjambi-opengl-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl/6.3.0/jar)
    * [**qtjambi-opengl-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-jre8/6.3.0/jar)
    * [**qtjambi-opengl-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-windows-x64/6.3.0/jar)
    * [**qtjambi-opengl-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-linux-x64/6.3.0/jar)
    * [**qtjambi-opengl-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-macos/6.3.0/jar)
* [QtOpenGLWidgets](https://doc.qt.io/qt-6/qtopenglwidgets-module.html) in module **qtjambi-openglwidgets**:
    * [**qtjambi-openglwidgets-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets/6.3.0/jar)
    * [**qtjambi-openglwidgets-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-jre8/6.3.0/jar)
    * [**qtjambi-openglwidgets-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-windows-x64/6.3.0/jar)
    * [**qtjambi-openglwidgets-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-linux-x64/6.3.0/jar)
    * [**qtjambi-openglwidgets-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-macos/6.3.0/jar)
* [QtMultimedia](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimedia**:
    * [**qtjambi-multimedia-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia/6.3.0/jar)
    * [**qtjambi-multimedia-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-jre8/6.3.0/jar)
    * [**qtjambi-multimedia-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-windows-x64/6.3.0/jar)
    * [**qtjambi-multimedia-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-linux-x64/6.3.0/jar)
    * [**qtjambi-multimedia-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-macos/6.3.0/jar)
* [QtMultimediaWidgets](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimediawidgets**:
    * [**qtjambi-multimediawidgets-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets/6.3.0/jar)
    * [**qtjambi-multimediawidgets-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-jre8/6.3.0/jar)
    * [**qtjambi-multimediawidgets-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-windows-x64/6.3.0/jar)
    * [**qtjambi-multimediawidgets-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-linux-x64/6.3.0/jar)
    * [**qtjambi-multimediawidgets-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-macos/6.3.0/jar)
* [QtNfc](https://doc.qt.io/qt-6/qtnfc-index.html) in module **qtjambi-nfc**:
    * [**qtjambi-nfc-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc/6.3.0/jar)
    * [**qtjambi-nfc-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-jre8/6.3.0/jar)
    * [**qtjambi-nfc-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-windows-x64/6.3.0/jar)
    * [**qtjambi-nfc-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-linux-x64/6.3.0/jar)
    * [**qtjambi-nfc-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-macos/6.3.0/jar)
* [QtNetworkAuth](https://doc.qt.io/qt-6/qtnetworkauth-index.html) in module **qtjambi-networkauth**:
    * [**qtjambi-networkauth-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth/6.3.0/jar)
    * [**qtjambi-networkauth-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-jre8/6.3.0/jar)
    * [**qtjambi-networkauth-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-windows-x64/6.3.0/jar)
    * [**qtjambi-networkauth-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-linux-x64/6.3.0/jar)
    * [**qtjambi-networkauth-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-macos/6.3.0/jar)
* [QtPositioning](https://doc.qt.io/qt-6/qtpositioning-index.html) in module **qtjambi-positioning**:
    * [**qtjambi-positioning-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning/6.3.0/jar)
    * [**qtjambi-positioning-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-jre8/6.3.0/jar)
    * [**qtjambi-positioning-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-windows-x64/6.3.0/jar)
    * [**qtjambi-positioning-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-linux-x64/6.3.0/jar)
    * [**qtjambi-positioning-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-macos/6.3.0/jar)
* [QtPrintSupport](https://doc.qt.io/qt-6/qtprintsupport-index.html) in module **qtjambi-printsupport**:
    * [**qtjambi-printsupport-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport/6.3.0/jar)
    * [**qtjambi-printsupport-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-jre8/6.3.0/jar)
    * [**qtjambi-printsupport-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-windows-x64/6.3.0/jar)
    * [**qtjambi-printsupport-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-linux-x64/6.3.0/jar)
    * [**qtjambi-printsupport-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-macos/6.3.0/jar)
* [QtQuick3D](https://doc.qt.io/qt-6/qtquick3d-index.html) in module **qtjambi-quick3d**:
    * [**qtjambi-quick3d-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d/6.3.0/jar)
    * [**qtjambi-quick3d-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-jre8/6.3.0/jar)
    * [**qtjambi-quick3d-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-windows-x64/6.3.0/jar)
    * [**qtjambi-quick3d-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-linux-x64/6.3.0/jar)
    * [**qtjambi-quick3d-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-macos/6.3.0/jar)
* [QtQuickWidgets](https://doc.qt.io/qt-6/qtquickwidgets-module.html) in module **qtjambi-quickwidgets**:
    * [**qtjambi-quickwidgets-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets/6.3.0/jar)
    * [**qtjambi-quickwidgets-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-jre8/6.3.0/jar)
    * [**qtjambi-quickwidgets-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-windows-x64/6.3.0/jar)
    * [**qtjambi-quickwidgets-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-linux-x64/6.3.0/jar)
    * [**qtjambi-quickwidgets-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-macos/6.3.0/jar)
* [QtRemoteObjects](https://doc.qt.io/qt-6/qtremoteobjects-module.html) in module **qtjambi-remoteobjects**:
    * [**qtjambi-remoteobjects-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects/6.3.0/jar)
    * [**qtjambi-remoteobjects-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-jre8/6.3.0/jar)
    * [**qtjambi-remoteobjects-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-windows-x64/6.3.0/jar)
    * [**qtjambi-remoteobjects-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-linux-x64/6.3.0/jar)
    * [**qtjambi-remoteobjects-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-macos/6.3.0/jar)
* [QtSCXML](https://doc.qt.io/qt-6/qtscxml-index.html) in module **qtjambi-scxml**:
    * [**qtjambi-scxml-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml/6.3.0/jar)
    * [**qtjambi-scxml-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-jre8/6.3.0/jar)
    * [**qtjambi-scxml-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-windows-x64/6.3.0/jar)
    * [**qtjambi-scxml-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-linux-x64/6.3.0/jar)
    * [**qtjambi-scxml-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-macos/6.3.0/jar)
* [QtSensors](https://doc.qt.io/qt-6/qtsensors-index.html) in module **qtjambi-sensors**:
    * [**qtjambi-sensors-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors/6.3.0/jar)
    * [**qtjambi-sensors-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-jre8/6.3.0/jar)
    * [**qtjambi-sensors-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-windows-x64/6.3.0/jar)
    * [**qtjambi-sensors-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-linux-x64/6.3.0/jar)
    * [**qtjambi-sensors-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-macos/6.3.0/jar)
* [QtSerialBus](https://doc.qt.io/qt-6/qtserialbus-index.html) in module **qtjambi-serialbus**:
    * [**qtjambi-serialbus-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus/6.3.0/jar)
    * [**qtjambi-serialbus-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-jre8/6.3.0/jar)
    * [**qtjambi-serialbus-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-windows-x64/6.3.0/jar)
    * [**qtjambi-serialbus-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-linux-x64/6.3.0/jar)
    * [**qtjambi-serialbus-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-macos/6.3.0/jar)
* [QtSerialPort](https://doc.qt.io/qt-6/qtserialport-index.html) in module **qtjambi-serialport**:
    * [**qtjambi-serialport-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport/6.3.0/jar)
    * [**qtjambi-serialport-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-jre8/6.3.0/jar)
    * [**qtjambi-serialport-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-windows-x64/6.3.0/jar)
    * [**qtjambi-serialport-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-linux-x64/6.3.0/jar)
    * [**qtjambi-serialport-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-macos/6.3.0/jar)
* [QtSql](https://doc.qt.io/qt-6/qtsql-index.html) in module **qtjambi-sql**:
    * [**qtjambi-sql-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql/6.3.0/jar)
    * [**qtjambi-sql-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-jre8/6.3.0/jar)
    * [**qtjambi-sql-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-windows-x64/6.3.0/jar)
    * [**qtjambi-sql-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-linux-x64/6.3.0/jar)
    * [**qtjambi-sql-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-macos/6.3.0/jar)
* [QtSvg](https://doc.qt.io/qt-6/qtsvg-index.html) in module **qtjambi-svg**:
    * [**qtjambi-svg-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg/6.3.0/jar)
    * [**qtjambi-svg-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-jre8/6.3.0/jar)
    * [**qtjambi-svg-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-windows-x64/6.3.0/jar)
    * [**qtjambi-svg-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-linux-x64/6.3.0/jar)
    * [**qtjambi-svg-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-macos/6.3.0/jar)
* [QtStateMachine](https://doc.qt.io/qt-6/qtstatemachine-index.html) in module **qtjambi-statemachine**:
    * [**qtjambi-statemachine-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine/6.3.0/jar)
    * [**qtjambi-statemachine-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-jre8/6.3.0/jar)
    * [**qtjambi-statemachine-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-windows-x64/6.3.0/jar)
    * [**qtjambi-statemachine-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-linux-x64/6.3.0/jar)
    * [**qtjambi-statemachine-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-macos/6.3.0/jar)
* [QtUITools](https://doc.qt.io/qt-6/qtuitools-index.html) in module **qtjambi-uitools**:
    * [**qtjambi-uitools-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools/6.3.0/jar)
    * [**qtjambi-uitools-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-jre8/6.3.0/jar)
    * [**qtjambi-uitools-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-windows-x64/6.3.0/jar)
    * [**qtjambi-uitools-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-linux-x64/6.3.0/jar)
    * [**qtjambi-uitools-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-macos/6.3.0/jar)
* [QtWebChannel](https://doc.qt.io/qt-6/qtwebchannel-index.html) in module **qtjambi-webchannel**:
    * [**qtjambi-webchannel-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel/6.3.0/jar)
    * [**qtjambi-webchannel-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-jre8/6.3.0/jar)
    * [**qtjambi-webchannel-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-windows-x64/6.3.0/jar)
    * [**qtjambi-webchannel-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-linux-x64/6.3.0/jar)
    * [**qtjambi-webchannel-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-macos/6.3.0/jar)
* [QtWebEngineCore](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginecore**:
    * [**qtjambi-webenginecore-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore/6.3.0/jar)
    * [**qtjambi-webenginecore-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-jre8/6.3.0/jar)
    * [**qtjambi-webenginecore-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-windows-x64/6.3.0/jar)
    * [**qtjambi-webenginecore-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-linux-x64/6.3.0/jar)
    * [**qtjambi-webenginecore-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-macos/6.3.0/jar)
* [QtWebEngineWidgets](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginewidgets**:
    * [**qtjambi-webenginewidgets-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets/6.3.0/jar)
    * [**qtjambi-webenginewidgets-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-jre8/6.3.0/jar)
    * [**qtjambi-webenginewidgets-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-windows-x64/6.3.0/jar)
    * [**qtjambi-webenginewidgets-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-linux-x64/6.3.0/jar)
    * [**qtjambi-webenginewidgets-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-macos/6.3.0/jar)
* [QtWebEngineQuick](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginequick**:
    * [**qtjambi-webenginequick-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick/6.3.0/jar)
    * [**qtjambi-webenginequick-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-jre8/6.3.0/jar)
    * [**qtjambi-webenginequick-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-windows-x64/6.3.0/jar)
    * [**qtjambi-webenginequick-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-linux-x64/6.3.0/jar)
    * [**qtjambi-webenginequick-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-macos/6.3.0/jar)
* [QtWebSockets](https://doc.qt.io/qt-6/qtwebsockets-index.html) in module **qtjambi-websockets**:
    * [**qtjambi-websockets-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets/6.3.0/jar)
    * [**qtjambi-websockets-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-jre8/6.3.0/jar)
    * [**qtjambi-websockets-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-windows-x64/6.3.0/jar)
    * [**qtjambi-websockets-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-linux-x64/6.3.0/jar)
    * [**qtjambi-websockets-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-macos/6.3.0/jar)
* [QtWebView](https://doc.qt.io/qt-6/qtwebview-index.html) in module **qtjambi-webview**:
    * [**qtjambi-webview-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview/6.3.0/jar)
    * [**qtjambi-webview-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-jre8/6.3.0/jar)
    * [**qtjambi-webview-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-windows-x64/6.3.0/jar)
    * [**qtjambi-webview-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-linux-x64/6.3.0/jar)
    * [**qtjambi-webview-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-macos/6.3.0/jar)
* [QtVirtualKeyboard](https://doc.qt.io/qt-6/qtvirtualkeyboard-index.html) in module **qtjambi-virtualkeyboard**:
    * [**qtjambi-virtualkeyboard-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard/6.3.0/jar)
    * [**qtjambi-virtualkeyboard-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-jre8/6.3.0/jar)
    * [**qtjambi-virtualkeyboard-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-windows-x64/6.3.0/jar)
    * [**qtjambi-virtualkeyboard-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-linux-x64/6.3.0/jar)
    * [**qtjambi-virtualkeyboard-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-macos/6.3.0/jar)
* [QtXml](https://doc.qt.io/qt-6/qtxml-index.html) in module **qtjambi-xml**:
    * [**qtjambi-xml-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml/6.3.0/jar)
    * [**qtjambi-xml-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-jre8/6.3.0/jar)
    * [**qtjambi-xml-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-windows-x64/6.3.0/jar)
    * [**qtjambi-xml-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-linux-x64/6.3.0/jar)
    * [**qtjambi-xml-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-macos/6.3.0/jar)
* [QtDesigner](https://doc.qt.io/qt-6/qtdesigner-manual.html) in module **qtjambi-designer**:
    * [**qtjambi-designer-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer/6.3.0/jar)
    * [**qtjambi-designer-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-jre8/6.3.0/jar)
    * [**qtjambi-designer-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-windows-x64/6.3.0/jar)
    * [**qtjambi-designer-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-linux-x64/6.3.0/jar)
    * [**qtjambi-designer-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-macos/6.3.0/jar)
* [QtPDF](https://doc.qt.io/qt-6/qtpdf-index.html) in modules **qtjambi-pdf** and **qtjambi-pdfwidgets**:
    * [**qtjambi-pdf-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdf/6.3.0/jar)
    * [**qtjambi-pdf-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdf-jre8/6.3.0/jar)
    * [**qtjambi-pdf-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdf-native-windows-x64/6.3.0/jar)
    * [**qtjambi-pdf-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdf-native-linux-x64/6.3.0/jar)
    * [**qtjambi-pdf-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdf-native-macos/6.3.0/jar)
    * [**qtjambi-pdfwidgets-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdfwidgets/6.3.0/jar)
    * [**qtjambi-pdfwidgets-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdfwidgets-jre8/6.3.0/jar)
    * [**qtjambi-pdfwidgets-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdfwidgets-native-windows-x64/6.3.0/jar)
    * [**qtjambi-pdfwidgets-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdfwidgets-native-linux-x64/6.3.0/jar)
    * [**qtjambi-pdfwidgets-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-pdfwidgets-native-macos/6.3.0/jar)

### Qt6 Platform-dependent Add-Ons

* [Active Qt (Windows only)](https://doc.qt.io/qt-6/activeqt-manual.html) in module **qtjambi-activex**:
    * [**qtjambi-activex-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex/6.3.0/jar)
    * [**qtjambi-activex-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex-jre8/6.3.0/jar)
    * [**qtjambi-activex-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex-native-windows-x64/6.3.0/jar)

### Qt3D

* [Qt3DCore](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dcore**:
    * [**qtjambi-qt3dcore-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore/6.3.0/jar)
    * [**qtjambi-qt3dcore-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-jre8/6.3.0/jar)
    * [**qtjambi-qt3dcore-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3dcore-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3dcore-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-macos/6.3.0/jar)
* [Qt3DRender](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3drender**:
    * [**qtjambi-qt3drender-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender/6.3.0/jar)
    * [**qtjambi-qt3drender-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-jre8/6.3.0/jar)
    * [**qtjambi-qt3drender-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3drender-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3drender-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-macos/6.3.0/jar)
* [Qt3DAnimation](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3danimation**:
    * [**qtjambi-qt3danimation-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation/6.3.0/jar)
    * [**qtjambi-qt3danimation-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-jre8/6.3.0/jar)
    * [**qtjambi-qt3danimation-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3danimation-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3danimation-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-macos/6.3.0/jar)
* [Qt3DExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dextras**:
    * [**qtjambi-qt3dextras-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras/6.3.0/jar)
    * [**qtjambi-qt3dextras-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-jre8/6.3.0/jar)
    * [**qtjambi-qt3dextras-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3dextras-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3dextras-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-macos/6.3.0/jar)
* [Qt3DInput](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dinput**:
    * [**qtjambi-qt3dinput-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput/6.3.0/jar)
    * [**qtjambi-qt3dinput-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-jre8/6.3.0/jar)
    * [**qtjambi-qt3dinput-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3dinput-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3dinput-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-macos/6.3.0/jar)
* [Qt3DLogic](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dlogic**:
    * [**qtjambi-qt3dlogic-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic/6.3.0/jar)
    * [**qtjambi-qt3dlogic-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-jre8/6.3.0/jar)
    * [**qtjambi-qt3dlogic-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3dlogic-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3dlogic-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-macos/6.3.0/jar)
* [Qt3DQuick](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquick**:
    * [**qtjambi-qt3dquick-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick/6.3.0/jar)
    * [**qtjambi-qt3dquick-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-jre8/6.3.0/jar)
    * [**qtjambi-qt3dquick-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3dquick-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3dquick-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-macos/6.3.0/jar)
* [Qt3DQuickExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickextras**:
    * [**qtjambi-qt3dquickextras-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras/6.3.0/jar)
    * [**qtjambi-qt3dquickextras-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-jre8/6.3.0/jar)
    * [**qtjambi-qt3dquickextras-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3dquickextras-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3dquickextras-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-macos/6.3.0/jar)
* [Qt3DQuickScene3D](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickscene2d**:
    * [**qtjambi-qt3dquickscene2d-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d/6.3.0/jar)
    * [**qtjambi-qt3dquickscene2d-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-jre8/6.3.0/jar)
    * [**qtjambi-qt3dquickscene2d-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-windows-x64/6.3.0/jar)
    * [**qtjambi-qt3dquickscene2d-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-linux-x64/6.3.0/jar)
    * [**qtjambi-qt3dquickscene2d-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-macos/6.3.0/jar)

### QtJambi Tools

* [QtJambi User Interface Compiler (UIC)](https://doc.qt.io/qt-6/designer-using-a-ui-file.html#compile-time-form-processing):
    * [**qtjambi-uic-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic/6.3.0/jar)
    * [**qtjambi-uic-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-jre8/6.3.0/jar)
    * [**qtjambi-uic-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-windows-x64/6.3.0/jar)
    * [**qtjambi-uic-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-linux-x64/6.3.0/jar)
    * [**qtjambi-uic-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-macos/6.3.0/jar)
* QtJambi Deployer:
    * [**qtjambi-deployer-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer/6.3.0/jar)
    * [**qtjambi-deployer-jre8-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-jre8/6.3.0/jar)
    * [**qtjambi-deployer-native-windows-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-windows-x64/6.3.0/jar)
    * [**qtjambi-deployer-native-linux-x64-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-linux-x64/6.3.0/jar)
    * [**qtjambi-deployer-native-macos-6.3.0.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-macos/6.3.0/jar)


## QtJambi for Qt 6.2

QtJambi 6.2.3 requires Qt 6.2.x whereas x can be any patch version.
Following Qt 6.2 modules are provided by QtJambi:

### Qt6 Essentials

* [QtCore](https://doc.qt.io/qt-6/qtcore-index.html), [QtGui](https://doc.qt.io/qt-6/qtgui-index.html) and [QtWidgets](https://doc.qt.io/qt-6/qtwidgets-index.html) in module **qtjambi**:
    * [**qtjambi-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi/6.2.3/jar)
    * [**qtjambi-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-jre8/6.2.3/jar)
    * [**qtjambi-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-windows-x64/6.2.3/jar)
    * [**qtjambi-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-linux-x64/6.2.3/jar)
    * [**qtjambi-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-macos/6.2.3/jar)
* [QtDBus](https://doc.qt.io/qt-6/qtdbus-index.html) in module **qtjambi-dbus**:
    * [**qtjambi-dbus-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus/6.2.3/jar)
    * [**qtjambi-dbus-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-jre8/6.2.3/jar)
    * [**qtjambi-dbus-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-windows-x64/6.2.3/jar)
    * [**qtjambi-dbus-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-linux-x64/6.2.3/jar)
    * [**qtjambi-dbus-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-macos/6.2.3/jar)
* [QtNetwork](https://doc.qt.io/qt-6/qtnetwork-index.html) in module **qtjambi-network**:
    * [**qtjambi-network-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network/6.2.3/jar)
    * [**qtjambi-network-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-jre8/6.2.3/jar)
    * [**qtjambi-network-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-windows-x64/6.2.3/jar)
    * [**qtjambi-network-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-linux-x64/6.2.3/jar)
    * [**qtjambi-network-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-macos/6.2.3/jar)
* [QtQml](https://doc.qt.io/qt-6/qtqml-index.html) in module **qtjambi-qml**:
    * [**qtjambi-qml-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml/6.2.3/jar)
    * [**qtjambi-qml-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-jre8/6.2.3/jar)
    * [**qtjambi-qml-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qml-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qml-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-macos/6.2.3/jar)
* [QtQuick](https://doc.qt.io/qt-6/qtquick-index.html) in module **qtjambi-quick**:
    * [**qtjambi-quick-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick/6.2.3/jar)
    * [**qtjambi-quick-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-jre8/6.2.3/jar)
    * [**qtjambi-quick-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-windows-x64/6.2.3/jar)
    * [**qtjambi-quick-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-linux-x64/6.2.3/jar)
    * [**qtjambi-quick-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-macos/6.2.3/jar)
* [QtQuickControls](https://doc.qt.io/qt-6/qtquickcontrols-index.html) in module **qtjambi-quickcontrols**:
    * [**qtjambi-quickcontrols-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols/6.2.3/jar)
    * [**qtjambi-quickcontrols-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-jre8/6.2.3/jar)
    * [**qtjambi-quickcontrols-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-windows-x64/6.2.3/jar)
    * [**qtjambi-quickcontrols-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-linux-x64/6.2.3/jar)
    * [**qtjambi-quickcontrols-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-macos/6.2.3/jar)
* [QtQuickTest](https://doc.qt.io/qt-6/qtquicktest-index.html) in module **qtjambi-quicktest**:
    * [**qtjambi-quicktest-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest/6.2.3/jar)
    * [**qtjambi-quicktest-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-jre8/6.2.3/jar)
    * [**qtjambi-quicktest-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-windows-x64/6.2.3/jar)
    * [**qtjambi-quicktest-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-linux-x64/6.2.3/jar)
    * [**qtjambi-quicktest-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-macos/6.2.3/jar)
* [QtTest](https://doc.qt.io/qt-6/qttest-index.html) in module **qtjambi-test**:
    * [**qtjambi-test-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test/6.2.3/jar)
    * [**qtjambi-test-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-jre8/6.2.3/jar)
    * [**qtjambi-test-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-windows-x64/6.2.3/jar)
    * [**qtjambi-test-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-linux-x64/6.2.3/jar)
    * [**qtjambi-test-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-macos/6.2.3/jar)

### Qt6 Add-Ons

* [QtBluetooth](https://doc.qt.io/qt-6/qtbluetooth-index.html) in module **qtjambi-bluetooth**:
    * [**qtjambi-bluetooth-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth/6.2.3/jar)
    * [**qtjambi-bluetooth-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-jre8/6.2.3/jar)
    * [**qtjambi-bluetooth-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-windows-x64/6.2.3/jar)
    * [**qtjambi-bluetooth-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-linux-x64/6.2.3/jar)
    * [**qtjambi-bluetooth-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-macos/6.2.3/jar)
* [QtCharts](https://doc.qt.io/qt-6/qtcharts-index.html) in module **qtjambi-charts**:
    * [**qtjambi-charts-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts/6.2.3/jar)
    * [**qtjambi-charts-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-jre8/6.2.3/jar)
    * [**qtjambi-charts-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-windows-x64/6.2.3/jar)
    * [**qtjambi-charts-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-linux-x64/6.2.3/jar)
    * [**qtjambi-charts-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-macos/6.2.3/jar)
* [QtDataVisualization](https://doc.qt.io/qt-6/qtdatavisualization-index.html) in module **qtjambi-datavisualization**:
    * [**qtjambi-datavisualization-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization/6.2.3/jar)
    * [**qtjambi-datavisualization-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-jre8/6.2.3/jar)
    * [**qtjambi-datavisualization-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-windows-x64/6.2.3/jar)
    * [**qtjambi-datavisualization-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-linux-x64/6.2.3/jar)
    * [**qtjambi-datavisualization-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-macos/6.2.3/jar)
* [QtConcurrent](https://doc.qt.io/qt-6/qtconcurrent-index.html) in module **qtjambi-concurrent**:
    * [**qtjambi-concurrent-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent/6.2.3/jar)
    * [**qtjambi-concurrent-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-jre8/6.2.3/jar)
    * [**qtjambi-concurrent-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-windows-x64/6.2.3/jar)
    * [**qtjambi-concurrent-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-linux-x64/6.2.3/jar)
    * [**qtjambi-concurrent-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-macos/6.2.3/jar)
* [QtHelp](https://doc.qt.io/qt-6/qthelp-index.html) in module **qtjambi-help**:
    * [**qtjambi-help-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help/6.2.3/jar)
    * [**qtjambi-help-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-jre8/6.2.3/jar)
    * [**qtjambi-help-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-windows-x64/6.2.3/jar)
    * [**qtjambi-help-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-linux-x64/6.2.3/jar)
    * [**qtjambi-help-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-macos/6.2.3/jar)
* [QtOpenGL](https://doc.qt.io/qt-6/qtopengl-index.html) in module **qtjambi-opengl**:
    * [**qtjambi-opengl-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl/6.2.3/jar)
    * [**qtjambi-opengl-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-jre8/6.2.3/jar)
    * [**qtjambi-opengl-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-windows-x64/6.2.3/jar)
    * [**qtjambi-opengl-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-linux-x64/6.2.3/jar)
    * [**qtjambi-opengl-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-macos/6.2.3/jar)
* [QtOpenGLWidgets](https://doc.qt.io/qt-6/qtopenglwidgets-module.html) in module **qtjambi-openglwidgets**:
    * [**qtjambi-openglwidgets-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets/6.2.3/jar)
    * [**qtjambi-openglwidgets-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-jre8/6.2.3/jar)
    * [**qtjambi-openglwidgets-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-windows-x64/6.2.3/jar)
    * [**qtjambi-openglwidgets-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-linux-x64/6.2.3/jar)
    * [**qtjambi-openglwidgets-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-macos/6.2.3/jar)
* [QtMultimedia](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimedia**:
    * [**qtjambi-multimedia-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia/6.2.3/jar)
    * [**qtjambi-multimedia-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-jre8/6.2.3/jar)
    * [**qtjambi-multimedia-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-windows-x64/6.2.3/jar)
    * [**qtjambi-multimedia-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-linux-x64/6.2.3/jar)
    * [**qtjambi-multimedia-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-macos/6.2.3/jar)
* [QtMultimediaWidgets](https://doc.qt.io/qt-6/qtmultimedia-index.html) in module **qtjambi-multimediawidgets**:
    * [**qtjambi-multimediawidgets-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets/6.2.3/jar)
    * [**qtjambi-multimediawidgets-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-jre8/6.2.3/jar)
    * [**qtjambi-multimediawidgets-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-windows-x64/6.2.3/jar)
    * [**qtjambi-multimediawidgets-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-linux-x64/6.2.3/jar)
    * [**qtjambi-multimediawidgets-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-macos/6.2.3/jar)
* [QtNfc](https://doc.qt.io/qt-6/qtnfc-index.html) in module **qtjambi-nfc**:
    * [**qtjambi-nfc-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc/6.2.3/jar)
    * [**qtjambi-nfc-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-jre8/6.2.3/jar)
    * [**qtjambi-nfc-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-windows-x64/6.2.3/jar)
    * [**qtjambi-nfc-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-linux-x64/6.2.3/jar)
    * [**qtjambi-nfc-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-macos/6.2.3/jar)
* [QtNetworkAuth](https://doc.qt.io/qt-6/qtnetworkauth-index.html) in module **qtjambi-networkauth**:
    * [**qtjambi-networkauth-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth/6.2.3/jar)
    * [**qtjambi-networkauth-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-jre8/6.2.3/jar)
    * [**qtjambi-networkauth-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-windows-x64/6.2.3/jar)
    * [**qtjambi-networkauth-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-linux-x64/6.2.3/jar)
    * [**qtjambi-networkauth-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-macos/6.2.3/jar)
* [QtPositioning](https://doc.qt.io/qt-6/qtpositioning-index.html) in module **qtjambi-positioning**:
    * [**qtjambi-positioning-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning/6.2.3/jar)
    * [**qtjambi-positioning-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-jre8/6.2.3/jar)
    * [**qtjambi-positioning-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-windows-x64/6.2.3/jar)
    * [**qtjambi-positioning-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-linux-x64/6.2.3/jar)
    * [**qtjambi-positioning-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-macos/6.2.3/jar)
* [QtPrintSupport](https://doc.qt.io/qt-6/qtprintsupport-index.html) in module **qtjambi-printsupport**:
    * [**qtjambi-printsupport-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport/6.2.3/jar)
    * [**qtjambi-printsupport-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-jre8/6.2.3/jar)
    * [**qtjambi-printsupport-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-windows-x64/6.2.3/jar)
    * [**qtjambi-printsupport-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-linux-x64/6.2.3/jar)
    * [**qtjambi-printsupport-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-macos/6.2.3/jar)
* [QtQuick3D](https://doc.qt.io/qt-6/qtquick3d-index.html) in module **qtjambi-quick3d**:
    * [**qtjambi-quick3d-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d/6.2.3/jar)
    * [**qtjambi-quick3d-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-jre8/6.2.3/jar)
    * [**qtjambi-quick3d-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-windows-x64/6.2.3/jar)
    * [**qtjambi-quick3d-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-linux-x64/6.2.3/jar)
    * [**qtjambi-quick3d-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-macos/6.2.3/jar)
* [QtQuickWidgets](https://doc.qt.io/qt-6/qtquickwidgets-module.html) in module **qtjambi-quickwidgets**:
    * [**qtjambi-quickwidgets-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets/6.2.3/jar)
    * [**qtjambi-quickwidgets-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-jre8/6.2.3/jar)
    * [**qtjambi-quickwidgets-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-windows-x64/6.2.3/jar)
    * [**qtjambi-quickwidgets-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-linux-x64/6.2.3/jar)
    * [**qtjambi-quickwidgets-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-macos/6.2.3/jar)
* [QtRemoteObjects](https://doc.qt.io/qt-6/qtremoteobjects-module.html) in module **qtjambi-remoteobjects**:
    * [**qtjambi-remoteobjects-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects/6.2.3/jar)
    * [**qtjambi-remoteobjects-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-jre8/6.2.3/jar)
    * [**qtjambi-remoteobjects-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-windows-x64/6.2.3/jar)
    * [**qtjambi-remoteobjects-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-linux-x64/6.2.3/jar)
    * [**qtjambi-remoteobjects-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-macos/6.2.3/jar)
* [QtSCXML](https://doc.qt.io/qt-6/qtscxml-index.html) in module **qtjambi-scxml**:
    * [**qtjambi-scxml-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml/6.2.3/jar)
    * [**qtjambi-scxml-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-jre8/6.2.3/jar)
    * [**qtjambi-scxml-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-windows-x64/6.2.3/jar)
    * [**qtjambi-scxml-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-linux-x64/6.2.3/jar)
    * [**qtjambi-scxml-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-macos/6.2.3/jar)
* [QtSensors](https://doc.qt.io/qt-6/qtsensors-index.html) in module **qtjambi-sensors**:
    * [**qtjambi-sensors-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors/6.2.3/jar)
    * [**qtjambi-sensors-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-jre8/6.2.3/jar)
    * [**qtjambi-sensors-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-windows-x64/6.2.3/jar)
    * [**qtjambi-sensors-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-linux-x64/6.2.3/jar)
    * [**qtjambi-sensors-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-macos/6.2.3/jar)
* [QtSerialBus](https://doc.qt.io/qt-6/qtserialbus-index.html) in module **qtjambi-serialbus**:
    * [**qtjambi-serialbus-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus/6.2.3/jar)
    * [**qtjambi-serialbus-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-jre8/6.2.3/jar)
    * [**qtjambi-serialbus-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-windows-x64/6.2.3/jar)
    * [**qtjambi-serialbus-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-linux-x64/6.2.3/jar)
    * [**qtjambi-serialbus-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-macos/6.2.3/jar)
* [QtSerialPort](https://doc.qt.io/qt-6/qtserialport-index.html) in module **qtjambi-serialport**:
    * [**qtjambi-serialport-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport/6.2.3/jar)
    * [**qtjambi-serialport-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-jre8/6.2.3/jar)
    * [**qtjambi-serialport-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-windows-x64/6.2.3/jar)
    * [**qtjambi-serialport-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-linux-x64/6.2.3/jar)
    * [**qtjambi-serialport-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-macos/6.2.3/jar)
* [QtSql](https://doc.qt.io/qt-6/qtsql-index.html) in module **qtjambi-sql**:
    * [**qtjambi-sql-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql/6.2.3/jar)
    * [**qtjambi-sql-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-jre8/6.2.3/jar)
    * [**qtjambi-sql-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-windows-x64/6.2.3/jar)
    * [**qtjambi-sql-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-linux-x64/6.2.3/jar)
    * [**qtjambi-sql-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-macos/6.2.3/jar)
* [QtSvg](https://doc.qt.io/qt-6/qtsvg-index.html) in module **qtjambi-svg**:
    * [**qtjambi-svg-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg/6.2.3/jar)
    * [**qtjambi-svg-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-jre8/6.2.3/jar)
    * [**qtjambi-svg-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-windows-x64/6.2.3/jar)
    * [**qtjambi-svg-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-linux-x64/6.2.3/jar)
    * [**qtjambi-svg-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-macos/6.2.3/jar)
* [QtStateMachine](https://doc.qt.io/qt-6/qtstatemachine-index.html) in module **qtjambi-statemachine**:
    * [**qtjambi-statemachine-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine/6.2.3/jar)
    * [**qtjambi-statemachine-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-jre8/6.2.3/jar)
    * [**qtjambi-statemachine-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-windows-x64/6.2.3/jar)
    * [**qtjambi-statemachine-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-linux-x64/6.2.3/jar)
    * [**qtjambi-statemachine-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-macos/6.2.3/jar)
* [QtUITools](https://doc.qt.io/qt-6/qtuitools-index.html) in module **qtjambi-uitools**:
    * [**qtjambi-uitools-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools/6.2.3/jar)
    * [**qtjambi-uitools-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-jre8/6.2.3/jar)
    * [**qtjambi-uitools-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-windows-x64/6.2.3/jar)
    * [**qtjambi-uitools-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-linux-x64/6.2.3/jar)
    * [**qtjambi-uitools-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-macos/6.2.3/jar)
* [QtWebChannel](https://doc.qt.io/qt-6/qtwebchannel-index.html) in module **qtjambi-webchannel**:
    * [**qtjambi-webchannel-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel/6.2.3/jar)
    * [**qtjambi-webchannel-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-jre8/6.2.3/jar)
    * [**qtjambi-webchannel-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-windows-x64/6.2.3/jar)
    * [**qtjambi-webchannel-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-linux-x64/6.2.3/jar)
    * [**qtjambi-webchannel-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-macos/6.2.3/jar)
* [QtWebEngineCore](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginecore**:
    * [**qtjambi-webenginecore-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore/6.2.3/jar)
    * [**qtjambi-webenginecore-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-jre8/6.2.3/jar)
    * [**qtjambi-webenginecore-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-windows-x64/6.2.3/jar)
    * [**qtjambi-webenginecore-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-linux-x64/6.2.3/jar)
    * [**qtjambi-webenginecore-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-macos/6.2.3/jar)
* [QtWebEngineWidgets](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginewidgets**:
    * [**qtjambi-webenginewidgets-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets/6.2.3/jar)
    * [**qtjambi-webenginewidgets-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-jre8/6.2.3/jar)
    * [**qtjambi-webenginewidgets-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-windows-x64/6.2.3/jar)
    * [**qtjambi-webenginewidgets-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-linux-x64/6.2.3/jar)
    * [**qtjambi-webenginewidgets-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-macos/6.2.3/jar)
* [QtWebEngineQuick](https://doc.qt.io/qt-6/qtwebengine-index.html) in module **qtjambi-webenginequick**:
    * [**qtjambi-webenginequick-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick/6.2.3/jar)
    * [**qtjambi-webenginequick-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-jre8/6.2.3/jar)
    * [**qtjambi-webenginequick-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-windows-x64/6.2.3/jar)
    * [**qtjambi-webenginequick-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-linux-x64/6.2.3/jar)
    * [**qtjambi-webenginequick-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-macos/6.2.3/jar)
* [QtWebSockets](https://doc.qt.io/qt-6/qtwebsockets-index.html) in module **qtjambi-websockets**:
    * [**qtjambi-websockets-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets/6.2.3/jar)
    * [**qtjambi-websockets-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-jre8/6.2.3/jar)
    * [**qtjambi-websockets-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-windows-x64/6.2.3/jar)
    * [**qtjambi-websockets-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-linux-x64/6.2.3/jar)
    * [**qtjambi-websockets-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-macos/6.2.3/jar)
* [QtWebView](https://doc.qt.io/qt-6/qtwebview-index.html) in module **qtjambi-webview**:
    * [**qtjambi-webview-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview/6.2.3/jar)
    * [**qtjambi-webview-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-jre8/6.2.3/jar)
    * [**qtjambi-webview-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-windows-x64/6.2.3/jar)
    * [**qtjambi-webview-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-linux-x64/6.2.3/jar)
    * [**qtjambi-webview-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-macos/6.2.3/jar)
* [QtVirtualKeyboard](https://doc.qt.io/qt-6/qtvirtualkeyboard-index.html) in module **qtjambi-virtualkeyboard**:
    * [**qtjambi-virtualkeyboard-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard/6.2.3/jar)
    * [**qtjambi-virtualkeyboard-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-jre8/6.2.3/jar)
    * [**qtjambi-virtualkeyboard-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-windows-x64/6.2.3/jar)
    * [**qtjambi-virtualkeyboard-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-linux-x64/6.2.3/jar)
    * [**qtjambi-virtualkeyboard-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-macos/6.2.3/jar)
* [QtXml](https://doc.qt.io/qt-6/qtxml-index.html) in module **qtjambi-xml**:
    * [**qtjambi-xml-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml/6.2.3/jar)
    * [**qtjambi-xml-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-jre8/6.2.3/jar)
    * [**qtjambi-xml-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-windows-x64/6.2.3/jar)
    * [**qtjambi-xml-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-linux-x64/6.2.3/jar)
    * [**qtjambi-xml-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-macos/6.2.3/jar)
* [QtDesigner](https://doc.qt.io/qt-6/qtdesigner-manual.html) in module **qtjambi-designer**:
    * [**qtjambi-designer-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer/6.2.3/jar)
    * [**qtjambi-designer-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-jre8/6.2.3/jar)
    * [**qtjambi-designer-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-windows-x64/6.2.3/jar)
    * [**qtjambi-designer-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-linux-x64/6.2.3/jar)
    * [**qtjambi-designer-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-macos/6.2.3/jar)

### Qt6 Platform-dependent Add-Ons

* [Active Qt (Windows only)](https://doc.qt.io/qt-6/activeqt-manual.html) in module **qtjambi-activex**:
    * [**qtjambi-activex-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex/6.2.3/jar)
    * [**qtjambi-activex-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex-jre8/6.2.3/jar)
    * [**qtjambi-activex-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex-native-windows-x64/6.2.3/jar)

### Qt3D

* [Qt3DCore](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dcore**:
    * [**qtjambi-qt3dcore-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore/6.2.3/jar)
    * [**qtjambi-qt3dcore-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-jre8/6.2.3/jar)
    * [**qtjambi-qt3dcore-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3dcore-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3dcore-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-macos/6.2.3/jar)
* [Qt3DRender](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3drender**:
    * [**qtjambi-qt3drender-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender/6.2.3/jar)
    * [**qtjambi-qt3drender-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-jre8/6.2.3/jar)
    * [**qtjambi-qt3drender-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3drender-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3drender-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-macos/6.2.3/jar)
* [Qt3DAnimation](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3danimation**:
    * [**qtjambi-qt3danimation-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation/6.2.3/jar)
    * [**qtjambi-qt3danimation-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-jre8/6.2.3/jar)
    * [**qtjambi-qt3danimation-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3danimation-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3danimation-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-macos/6.2.3/jar)
* [Qt3DExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dextras**:
    * [**qtjambi-qt3dextras-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras/6.2.3/jar)
    * [**qtjambi-qt3dextras-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-jre8/6.2.3/jar)
    * [**qtjambi-qt3dextras-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3dextras-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3dextras-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-macos/6.2.3/jar)
* [Qt3DInput](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dinput**:
    * [**qtjambi-qt3dinput-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput/6.2.3/jar)
    * [**qtjambi-qt3dinput-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-jre8/6.2.3/jar)
    * [**qtjambi-qt3dinput-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3dinput-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3dinput-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-macos/6.2.3/jar)
* [Qt3DLogic](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dlogic**:
    * [**qtjambi-qt3dlogic-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic/6.2.3/jar)
    * [**qtjambi-qt3dlogic-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-jre8/6.2.3/jar)
    * [**qtjambi-qt3dlogic-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3dlogic-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3dlogic-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-macos/6.2.3/jar)
* [Qt3DQuick](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquick**:
    * [**qtjambi-qt3dquick-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick/6.2.3/jar)
    * [**qtjambi-qt3dquick-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-jre8/6.2.3/jar)
    * [**qtjambi-qt3dquick-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3dquick-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3dquick-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-macos/6.2.3/jar)
* [Qt3DQuickExtras](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickextras**:
    * [**qtjambi-qt3dquickextras-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras/6.2.3/jar)
    * [**qtjambi-qt3dquickextras-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-jre8/6.2.3/jar)
    * [**qtjambi-qt3dquickextras-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3dquickextras-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3dquickextras-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-macos/6.2.3/jar)
* [Qt3DQuickScene3D](https://doc.qt.io/qt-6/qt3d-index.html) in module **qtjambi-qt3dquickscene2d**:
    * [**qtjambi-qt3dquickscene2d-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d/6.2.3/jar)
    * [**qtjambi-qt3dquickscene2d-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-jre8/6.2.3/jar)
    * [**qtjambi-qt3dquickscene2d-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-windows-x64/6.2.3/jar)
    * [**qtjambi-qt3dquickscene2d-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-linux-x64/6.2.3/jar)
    * [**qtjambi-qt3dquickscene2d-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-macos/6.2.3/jar)

### QtJambi Tools

* [QtJambi User Interface Compiler (UIC)](https://doc.qt.io/qt-6/designer-using-a-ui-file.html#compile-time-form-processing):
    * [**qtjambi-uic-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic/6.2.3/jar)
    * [**qtjambi-uic-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-jre8/6.2.3/jar)
    * [**qtjambi-uic-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-windows-x64/6.2.3/jar)
    * [**qtjambi-uic-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-linux-x64/6.2.3/jar)
    * [**qtjambi-uic-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-macos/6.2.3/jar)
* QtJambi Deployer:
    * [**qtjambi-deployer-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer/6.2.3/jar)
    * [**qtjambi-deployer-jre8-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-jre8/6.2.3/jar)
    * [**qtjambi-deployer-native-windows-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-windows-x64/6.2.3/jar)
    * [**qtjambi-deployer-native-linux-x64-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-linux-x64/6.2.3/jar)
    * [**qtjambi-deployer-native-macos-6.2.3.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-macos/6.2.3/jar)


## QtJambi for Qt 5.15

QtJambi 5.15.5 requires Qt 5.15.x whereas x can be any patch version.
Following Qt 5.15 modules are provided by QtJambi:

### Qt5 Essentials

* [QtCore](https://doc.qt.io/qt-5/qtcore-index.html), [QtGui](https://doc.qt.io/qt-5/qtgui-index.html) and [QtWidgets](https://doc.qt.io/qt-5/qtwidgets-index.html) in module **qtjambi**:
    * [**qtjambi-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi/5.15.5/jar)
    * [**qtjambi-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-jre8/5.15.5/jar)
    * [**qtjambi-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-windows-x64/5.15.5/jar)
    * [**qtjambi-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-linux-x64/5.15.5/jar)
    * [**qtjambi-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-macos/5.15.5/jar)
* [QtMultimedia](https://doc.qt.io/qt-5/qtmultimedia-index.html) in module **qtjambi-multimedia**:
    * [**qtjambi-multimedia-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia/5.15.5/jar)
    * [**qtjambi-multimedia-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-jre8/5.15.5/jar)
    * [**qtjambi-multimedia-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-windows-x64/5.15.5/jar)
    * [**qtjambi-multimedia-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-linux-x64/5.15.5/jar)
    * [**qtjambi-multimedia-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-macos/5.15.5/jar)
* [QtMultimediaWidgets](https://doc.qt.io/qt-5/qtmultimedia-index.html) in module **qtjambi-multimediawidgets**:
    * [**qtjambi-multimediawidgets-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets/5.15.5/jar)
    * [**qtjambi-multimediawidgets-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-jre8/5.15.5/jar)
    * [**qtjambi-multimediawidgets-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-windows-x64/5.15.5/jar)
    * [**qtjambi-multimediawidgets-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-linux-x64/5.15.5/jar)
    * [**qtjambi-multimediawidgets-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-macos/5.15.5/jar)
* [QtNetwork](https://doc.qt.io/qt-5/qtnetwork-index.html) in module **qtjambi-network**:
    * [**qtjambi-network-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network/5.15.5/jar)
    * [**qtjambi-network-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-jre8/5.15.5/jar)
    * [**qtjambi-network-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-windows-x64/5.15.5/jar)
    * [**qtjambi-network-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-linux-x64/5.15.5/jar)
    * [**qtjambi-network-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-macos/5.15.5/jar)
* [QtQml](https://doc.qt.io/qt-5/qtqml-index.html) in module **qtjambi-qml**:
    * [**qtjambi-qml-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml/5.15.5/jar)
    * [**qtjambi-qml-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-jre8/5.15.5/jar)
    * [**qtjambi-qml-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qml-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qml-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-macos/5.15.5/jar)
* [QtQuick](https://doc.qt.io/qt-5/qtquick-index.html) in module **qtjambi-quick**:
    * [**qtjambi-quick-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick/5.15.5/jar)
    * [**qtjambi-quick-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-jre8/5.15.5/jar)
    * [**qtjambi-quick-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-windows-x64/5.15.5/jar)
    * [**qtjambi-quick-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-linux-x64/5.15.5/jar)
    * [**qtjambi-quick-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-macos/5.15.5/jar)
* [QtQuickControls](https://doc.qt.io/qt-5/qtquickcontrols-index.html) in module **qtjambi-quickcontrols**:
    * [**qtjambi-quickcontrols-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols/5.15.5/jar)
    * [**qtjambi-quickcontrols-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-jre8/5.15.5/jar)
    * [**qtjambi-quickcontrols-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-windows-x64/5.15.5/jar)
    * [**qtjambi-quickcontrols-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-linux-x64/5.15.5/jar)
    * [**qtjambi-quickcontrols-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-macos/5.15.5/jar)
* [QtQuickTest](https://doc.qt.io/qt-5/qtquicktest-index.html) in module **qtjambi-quicktest**:
    * [**qtjambi-quicktest-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest/5.15.5/jar)
    * [**qtjambi-quicktest-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-jre8/5.15.5/jar)
    * [**qtjambi-quicktest-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-windows-x64/5.15.5/jar)
    * [**qtjambi-quicktest-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-linux-x64/5.15.5/jar)
    * [**qtjambi-quicktest-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quicktest-native-macos/5.15.5/jar)
* [QtSql](https://doc.qt.io/qt-5/qtsql-index.html) in module **qtjambi-sql**:
    * [**qtjambi-sql-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql/5.15.5/jar)
    * [**qtjambi-sql-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-jre8/5.15.5/jar)
    * [**qtjambi-sql-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-windows-x64/5.15.5/jar)
    * [**qtjambi-sql-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-linux-x64/5.15.5/jar)
    * [**qtjambi-sql-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-macos/5.15.5/jar)
* [QtTest](https://doc.qt.io/qt-5/qttest-index.html) in module **qtjambi-test**:
    * [**qtjambi-test-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test/5.15.5/jar)
    * [**qtjambi-test-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-jre8/5.15.5/jar)
    * [**qtjambi-test-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-windows-x64/5.15.5/jar)
    * [**qtjambi-test-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-linux-x64/5.15.5/jar)
    * [**qtjambi-test-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-macos/5.15.5/jar)

### Qt5 Add-Ons

* [QtBluetooth](https://doc.qt.io/qt-5/qtbluetooth-index.html) in module **qtjambi-bluetooth**:
    * [**qtjambi-bluetooth-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth/5.15.5/jar)
    * [**qtjambi-bluetooth-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-jre8/5.15.5/jar)
    * [**qtjambi-bluetooth-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-windows-x64/5.15.5/jar)
    * [**qtjambi-bluetooth-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-linux-x64/5.15.5/jar)
    * [**qtjambi-bluetooth-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-macos/5.15.5/jar)
* [QtConcurrent](https://doc.qt.io/qt-5/qtconcurrent-index.html) in module **qtjambi-concurrent**:
    * [**qtjambi-concurrent-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent/5.15.5/jar)
    * [**qtjambi-concurrent-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-jre8/5.15.5/jar)
    * [**qtjambi-concurrent-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-windows-x64/5.15.5/jar)
    * [**qtjambi-concurrent-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-linux-x64/5.15.5/jar)
    * [**qtjambi-concurrent-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-macos/5.15.5/jar)
* [QtDBus](https://doc.qt.io/qt-5/qtdbus-index.html) in module **qtjambi-dbus**:
    * [**qtjambi-dbus-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus/5.15.5/jar)
    * [**qtjambi-dbus-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-jre8/5.15.5/jar)
    * [**qtjambi-dbus-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-windows-x64/5.15.5/jar)
    * [**qtjambi-dbus-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-linux-x64/5.15.5/jar)
    * [**qtjambi-dbus-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-macos/5.15.5/jar)
* [QtGamepad](https://doc.qt.io/qt-5/qtgamepad-index.html) in module **qtjambi-gamepad**:
    * [**qtjambi-gamepad-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad/5.15.5/jar)
    * [**qtjambi-gamepad-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-jre8/5.15.5/jar)
    * [**qtjambi-gamepad-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-native-windows-x64/5.15.5/jar)
    * [**qtjambi-gamepad-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-native-linux-x64/5.15.5/jar)
    * [**qtjambi-gamepad-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-gamepad-native-macos/5.15.5/jar)
* [QtHelp](https://doc.qt.io/qt-5/qthelp-index.html) in module **qtjambi-help**:
    * [**qtjambi-help-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help/5.15.5/jar)
    * [**qtjambi-help-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-jre8/5.15.5/jar)
    * [**qtjambi-help-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-windows-x64/5.15.5/jar)
    * [**qtjambi-help-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-linux-x64/5.15.5/jar)
    * [**qtjambi-help-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-macos/5.15.5/jar)
* [QtLocation](https://doc.qt.io/qt-5/qtlocation-index.html) in module **qtjambi-location**:
    * [**qtjambi-location-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location/5.15.5/jar)
    * [**qtjambi-location-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-jre8/5.15.5/jar)
    * [**qtjambi-location-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-native-windows-x64/5.15.5/jar)
    * [**qtjambi-location-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-native-linux-x64/5.15.5/jar)
    * [**qtjambi-location-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-location-native-macos/5.15.5/jar)
* [QtNfc](https://doc.qt.io/qt-5/qtnfc-index.html) in module **qtjambi-nfc**:
    * [**qtjambi-nfc-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc/5.15.5/jar)
    * [**qtjambi-nfc-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-jre8/5.15.5/jar)
    * [**qtjambi-nfc-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-windows-x64/5.15.5/jar)
    * [**qtjambi-nfc-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-linux-x64/5.15.5/jar)
    * [**qtjambi-nfc-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-macos/5.15.5/jar)
* [QtPositioning](https://doc.qt.io/qt-5/qtpositioning-index.html) in module **qtjambi-positioning**:
    * [**qtjambi-positioning-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning/5.15.5/jar)
    * [**qtjambi-positioning-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-jre8/5.15.5/jar)
    * [**qtjambi-positioning-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-windows-x64/5.15.5/jar)
    * [**qtjambi-positioning-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-linux-x64/5.15.5/jar)
    * [**qtjambi-positioning-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-macos/5.15.5/jar)
* [QtPrintSupport](https://doc.qt.io/qt-5/qtprintsupport-index.html) in module **qtjambi-printsupport**:
    * [**qtjambi-printsupport-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport/5.15.5/jar)
    * [**qtjambi-printsupport-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-jre8/5.15.5/jar)
    * [**qtjambi-printsupport-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-windows-x64/5.15.5/jar)
    * [**qtjambi-printsupport-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-linux-x64/5.15.5/jar)
    * [**qtjambi-printsupport-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-macos/5.15.5/jar)
* [QtPurchasing](https://doc.qt.io/qt-5/qtpurchasing-index.html) in module **qtjambi-purchasing**:
    * [**qtjambi-purchasing-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing/5.15.5/jar)
    * [**qtjambi-purchasing-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-jre8/5.15.5/jar)
    * [**qtjambi-purchasing-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-native-windows-x64/5.15.5/jar)
    * [**qtjambi-purchasing-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-native-linux-x64/5.15.5/jar)
    * [**qtjambi-purchasing-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-purchasing-native-macos/5.15.5/jar)
* [QtQuickWidgets](https://doc.qt.io/qt-5/qtquickwidgets-module.html) in module **qtjambi-quickwidgets**:
    * [**qtjambi-quickwidgets-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets/5.15.5/jar)
    * [**qtjambi-quickwidgets-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-jre8/5.15.5/jar)
    * [**qtjambi-quickwidgets-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-windows-x64/5.15.5/jar)
    * [**qtjambi-quickwidgets-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-linux-x64/5.15.5/jar)
    * [**qtjambi-quickwidgets-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-macos/5.15.5/jar)
* [QtRemoteObjects](https://doc.qt.io/qt-5/qtremoteobjects-module.html) in module **qtjambi-remoteobjects**:
    * [**qtjambi-remoteobjects-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects/5.15.5/jar)
    * [**qtjambi-remoteobjects-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-jre8/5.15.5/jar)
    * [**qtjambi-remoteobjects-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-windows-x64/5.15.5/jar)
    * [**qtjambi-remoteobjects-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-linux-x64/5.15.5/jar)
    * [**qtjambi-remoteobjects-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-macos/5.15.5/jar)
* [QtScript](https://doc.qt.io/qt-5/qtscript-module.html) in module **qtjambi-script**:
    * [**qtjambi-script-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script/5.15.5/jar)
    * [**qtjambi-script-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-jre8/5.15.5/jar)
    * [**qtjambi-script-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-native-windows-x64/5.15.5/jar)
    * [**qtjambi-script-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-native-linux-x64/5.15.5/jar)
    * [**qtjambi-script-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-script-native-macos/5.15.5/jar)
* [QtScriptTools](https://doc.qt.io/qt-5/qtscripttools-module.html) in module **qtjambi-scripttools**:
    * [**qtjambi-scripttools-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools/5.15.5/jar)
    * [**qtjambi-scripttools-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-jre8/5.15.5/jar)
    * [**qtjambi-scripttools-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-native-windows-x64/5.15.5/jar)
    * [**qtjambi-scripttools-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-native-linux-x64/5.15.5/jar)
    * [**qtjambi-scripttools-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scripttools-native-macos/5.15.5/jar)
* [QtSCXML](https://doc.qt.io/qt-5/qtscxml-index.html) in module **qtjambi-scxml**:
    * [**qtjambi-scxml-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml/5.15.5/jar)
    * [**qtjambi-scxml-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-jre8/5.15.5/jar)
    * [**qtjambi-scxml-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-windows-x64/5.15.5/jar)
    * [**qtjambi-scxml-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-linux-x64/5.15.5/jar)
    * [**qtjambi-scxml-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-macos/5.15.5/jar)
* [QtSensors](https://doc.qt.io/qt-5/qtsensors-index.html) in module **qtjambi-sensors**:
    * [**qtjambi-sensors-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors/5.15.5/jar)
    * [**qtjambi-sensors-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-jre8/5.15.5/jar)
    * [**qtjambi-sensors-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-windows-x64/5.15.5/jar)
    * [**qtjambi-sensors-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-linux-x64/5.15.5/jar)
    * [**qtjambi-sensors-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-macos/5.15.5/jar)
* [QtSerialBus](https://doc.qt.io/qt-5/qtserialbus-index.html) in module **qtjambi-serialbus**:
    * [**qtjambi-serialbus-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus/5.15.5/jar)
    * [**qtjambi-serialbus-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-jre8/5.15.5/jar)
    * [**qtjambi-serialbus-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-windows-x64/5.15.5/jar)
    * [**qtjambi-serialbus-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-linux-x64/5.15.5/jar)
    * [**qtjambi-serialbus-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-macos/5.15.5/jar)
* [QtSerialPort](https://doc.qt.io/qt-5/qtserialport-index.html) in module **qtjambi-serialport**:
    * [**qtjambi-serialport-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport/5.15.5/jar)
    * [**qtjambi-serialport-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-jre8/5.15.5/jar)
    * [**qtjambi-serialport-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-windows-x64/5.15.5/jar)
    * [**qtjambi-serialport-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-linux-x64/5.15.5/jar)
    * [**qtjambi-serialport-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-macos/5.15.5/jar)
* [QtSvg](https://doc.qt.io/qt-5/qtsvg-index.html) in module **qtjambi-svg**:
    * [**qtjambi-svg-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg/5.15.5/jar)
    * [**qtjambi-svg-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-jre8/5.15.5/jar)
    * [**qtjambi-svg-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-windows-x64/5.15.5/jar)
    * [**qtjambi-svg-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-linux-x64/5.15.5/jar)
    * [**qtjambi-svg-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-macos/5.15.5/jar)
* [QtTextToSpeech](https://doc.qt.io/qt-5/qtspeech-index.html) in module **qtjambi-texttospeech**:
    * [**qtjambi-texttospeech-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech/5.15.5/jar)
    * [**qtjambi-texttospeech-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-jre8/5.15.5/jar)
    * [**qtjambi-texttospeech-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-native-windows-x64/5.15.5/jar)
    * [**qtjambi-texttospeech-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-native-linux-x64/5.15.5/jar)
    * [**qtjambi-texttospeech-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-texttospeech-native-macos/5.15.5/jar)
* [QtUITools](https://doc.qt.io/qt-5/qtuitools-index.html) in module **qtjambi-uitools**:
    * [**qtjambi-uitools-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools/5.15.5/jar)
    * [**qtjambi-uitools-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-jre8/5.15.5/jar)
    * [**qtjambi-uitools-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-windows-x64/5.15.5/jar)
    * [**qtjambi-uitools-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-linux-x64/5.15.5/jar)
    * [**qtjambi-uitools-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-macos/5.15.5/jar)
* [QtWebChannel](https://doc.qt.io/qt-5/qtwebchannel-index.html) in module **qtjambi-webchannel**:
    * [**qtjambi-webchannel-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel/5.15.5/jar)
    * [**qtjambi-webchannel-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-jre8/5.15.5/jar)
    * [**qtjambi-webchannel-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-windows-x64/5.15.5/jar)
    * [**qtjambi-webchannel-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-linux-x64/5.15.5/jar)
    * [**qtjambi-webchannel-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-macos/5.15.5/jar)
* [QtWebEngineCore](https://doc.qt.io/qt-5/qtwebengine-index.html) in module **qtjambi-webenginecore**:
    * [**qtjambi-webenginecore-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore/5.15.5/jar)
    * [**qtjambi-webenginecore-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-jre8/5.15.5/jar)
    * [**qtjambi-webenginecore-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-windows-x64/5.15.5/jar)
    * [**qtjambi-webenginecore-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-linux-x64/5.15.5/jar)
    * [**qtjambi-webenginecore-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-macos/5.15.5/jar)
* [QtWebEngineWidgets](https://doc.qt.io/qt-5/qtwebengine-index.html) in module **qtjambi-webenginewidgets**:
    * [**qtjambi-webenginewidgets-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets/5.15.5/jar)
    * [**qtjambi-webenginewidgets-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-jre8/5.15.5/jar)
    * [**qtjambi-webenginewidgets-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-windows-x64/5.15.5/jar)
    * [**qtjambi-webenginewidgets-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-linux-x64/5.15.5/jar)
    * [**qtjambi-webenginewidgets-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-macos/5.15.5/jar)
* [QtWebEngine](https://doc.qt.io/qt-5/qtwebengine-index.html) in module **qtjambi-webengine**:
    * [**qtjambi-webengine-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine/5.15.5/jar)
    * [**qtjambi-webengine-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-jre8/5.15.5/jar)
    * [**qtjambi-webengine-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-native-windows-x64/5.15.5/jar)
    * [**qtjambi-webengine-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-native-linux-x64/5.15.5/jar)
    * [**qtjambi-webengine-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webengine-native-macos/5.15.5/jar)
* [QtWebSockets](https://doc.qt.io/qt-5/qtwebsockets-index.html) in module **qtjambi-websockets**:
    * [**qtjambi-websockets-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets/5.15.5/jar)
    * [**qtjambi-websockets-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-jre8/5.15.5/jar)
    * [**qtjambi-websockets-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-windows-x64/5.15.5/jar)
    * [**qtjambi-websockets-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-linux-x64/5.15.5/jar)
    * [**qtjambi-websockets-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-macos/5.15.5/jar)
* [QtWebView](https://doc.qt.io/qt-5/qtwebview-index.html) in module **qtjambi-webview**:
    * [**qtjambi-webview-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview/5.15.5/jar)
    * [**qtjambi-webview-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-jre8/5.15.5/jar)
    * [**qtjambi-webview-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-windows-x64/5.15.5/jar)
    * [**qtjambi-webview-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-linux-x64/5.15.5/jar)
    * [**qtjambi-webview-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-macos/5.15.5/jar)
* [QtXml](https://doc.qt.io/qt-5/qtxml-index.html) in module **qtjambi-xml**:
    * [**qtjambi-xml-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml/5.15.5/jar)
    * [**qtjambi-xml-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-jre8/5.15.5/jar)
    * [**qtjambi-xml-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-windows-x64/5.15.5/jar)
    * [**qtjambi-xml-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-linux-x64/5.15.5/jar)
    * [**qtjambi-xml-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-macos/5.15.5/jar)
* [QtXmlPatterns](https://doc.qt.io/qt-5/qtxmlpatterns-index.html) in module **qtjambi-xmlpatterns**:
    * [**qtjambi-xmlpatterns-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns/5.15.5/jar)
    * [**qtjambi-xmlpatterns-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-jre8/5.15.5/jar)
    * [**qtjambi-xmlpatterns-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-native-windows-x64/5.15.5/jar)
    * [**qtjambi-xmlpatterns-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-native-linux-x64/5.15.5/jar)
    * [**qtjambi-xmlpatterns-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xmlpatterns-native-macos/5.15.5/jar)
* [QtCharts](https://doc.qt.io/qt-5/qtcharts-index.html) in module **qtjambi-charts**:
    * [**qtjambi-charts-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts/5.15.5/jar)
    * [**qtjambi-charts-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-jre8/5.15.5/jar)
    * [**qtjambi-charts-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-windows-x64/5.15.5/jar)
    * [**qtjambi-charts-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-linux-x64/5.15.5/jar)
    * [**qtjambi-charts-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-macos/5.15.5/jar)
* [QtDataVisualization](https://doc.qt.io/qt-5/qtdatavisualization-index.html) in module **qtjambi-datavisualization**:
    * [**qtjambi-datavisualization-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization/5.15.5/jar)
    * [**qtjambi-datavisualization-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-jre8/5.15.5/jar)
    * [**qtjambi-datavisualization-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-windows-x64/5.15.5/jar)
    * [**qtjambi-datavisualization-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-linux-x64/5.15.5/jar)
    * [**qtjambi-datavisualization-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-macos/5.15.5/jar)
* [QtNetworkAuth](https://doc.qt.io/qt-5/qtnetworkauth-index.html) in module **qtjambi-networkauth**:
    * [**qtjambi-networkauth-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth/5.15.5/jar)
    * [**qtjambi-networkauth-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-jre8/5.15.5/jar)
    * [**qtjambi-networkauth-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-windows-x64/5.15.5/jar)
    * [**qtjambi-networkauth-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-linux-x64/5.15.5/jar)
    * [**qtjambi-networkauth-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-macos/5.15.5/jar)
* [QtVirtualKeyboard](https://doc.qt.io/qt-5/qtvirtualkeyboard-index.html) in module **qtjambi-virtualkeyboard**:
    * [**qtjambi-virtualkeyboard-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard/5.15.5/jar)
    * [**qtjambi-virtualkeyboard-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-jre8/5.15.5/jar)
    * [**qtjambi-virtualkeyboard-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-windows-x64/5.15.5/jar)
    * [**qtjambi-virtualkeyboard-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-linux-x64/5.15.5/jar)
    * [**qtjambi-virtualkeyboard-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-macos/5.15.5/jar)
* [QtQuick3D](https://doc.qt.io/qt-5/qtquick3d-index.html) in module **qtjambi-quick3d**:
    * [**qtjambi-quick3d-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d/5.15.5/jar)
    * [**qtjambi-quick3d-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-jre8/5.15.5/jar)
    * [**qtjambi-quick3d-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-windows-x64/5.15.5/jar)
    * [**qtjambi-quick3d-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-linux-x64/5.15.5/jar)
    * [**qtjambi-quick3d-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-macos/5.15.5/jar)
* [QtDesigner](https://doc.qt.io/qt-5/qtdesigner-manual.html) in module **qtjambi-designer**:
    * [**qtjambi-designer-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer/5.15.5/jar)
    * [**qtjambi-designer-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-jre8/5.15.5/jar)
    * [**qtjambi-designer-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-windows-x64/5.15.5/jar)
    * [**qtjambi-designer-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-linux-x64/5.15.5/jar)
    * [**qtjambi-designer-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-macos/5.15.5/jar)

### Qt5 Platform-dependent Add-Ons

* [Active Qt (Windows only)](https://doc.qt.io/qt-5/activeqt-manual.html) in module **qtjambi-activex**:
    * [**qtjambi-activex-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex/5.15.5/jar)
    * [**qtjambi-activex-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex-jre8/5.15.5/jar)
    * [**qtjambi-activex-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-activex-native-windows-x64/5.15.5/jar)
* [QtWinExtras (Windows only)](https://doc.qt.io/qt-5/qtwinextras-index.html) in module **qtjambi-winextras**:
    * [**qtjambi-winextras-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-winextras/5.15.5/jar)
    * [**qtjambi-winextras-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-winextras-jre8/5.15.5/jar)
    * [**qtjambi-winextras-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-winextras-native-windows-x64/5.15.5/jar)
* [QtMacExtras (macOS only)](https://doc.qt.io/qt-5/qtmacextras-index.html) in module **qtjambi-macextras**:
    * [**qtjambi-macextras-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-macextras/5.15.5/jar)
    * [**qtjambi-macextras-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-macextras-jre8/5.15.5/jar)
    * [**qtjambi-macextras-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-macextras-native-macos/5.15.5/jar)
* [QtX11Extras (Linux only)](https://doc.qt.io/qt-5/qtx11extras-index.html) in module **qtjambi-x11extras**:
    * [**qtjambi-x11extras-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-x11extras/5.15.5/jar)
    * [**qtjambi-x11extras-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-x11extras-jre8/5.15.5/jar)
    * [**qtjambi-x11extras-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-x11extras-native-linux-x64/5.15.5/jar)

### Qt3D

* [Qt3DCore](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dcore**:
    * [**qtjambi-qt3dcore-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore/5.15.5/jar)
    * [**qtjambi-qt3dcore-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-jre8/5.15.5/jar)
    * [**qtjambi-qt3dcore-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3dcore-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3dcore-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-macos/5.15.5/jar)
* [Qt3DRender](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3drender**:
    * [**qtjambi-qt3drender-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender/5.15.5/jar)
    * [**qtjambi-qt3drender-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-jre8/5.15.5/jar)
    * [**qtjambi-qt3drender-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3drender-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3drender-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-macos/5.15.5/jar)
* [Qt3DAnimation](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3danimation**:
    * [**qtjambi-qt3danimation-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation/5.15.5/jar)
    * [**qtjambi-qt3danimation-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-jre8/5.15.5/jar)
    * [**qtjambi-qt3danimation-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3danimation-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3danimation-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-macos/5.15.5/jar)
* [Qt3DExtras](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dextras**:
    * [**qtjambi-qt3dextras-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras/5.15.5/jar)
    * [**qtjambi-qt3dextras-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-jre8/5.15.5/jar)
    * [**qtjambi-qt3dextras-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3dextras-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3dextras-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-macos/5.15.5/jar)
* [Qt3DInput](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dinput**:
    * [**qtjambi-qt3dinput-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput/5.15.5/jar)
    * [**qtjambi-qt3dinput-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-jre8/5.15.5/jar)
    * [**qtjambi-qt3dinput-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3dinput-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3dinput-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-macos/5.15.5/jar)
* [Qt3DLogic](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dlogic**:
    * [**qtjambi-qt3dlogic-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic/5.15.5/jar)
    * [**qtjambi-qt3dlogic-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-jre8/5.15.5/jar)
    * [**qtjambi-qt3dlogic-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3dlogic-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3dlogic-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-macos/5.15.5/jar)
* [Qt3DQuick](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dquick**:
    * [**qtjambi-qt3dquick-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick/5.15.5/jar)
    * [**qtjambi-qt3dquick-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-jre8/5.15.5/jar)
    * [**qtjambi-qt3dquick-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3dquick-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3dquick-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-macos/5.15.5/jar)
* [Qt3DQuickExtras](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dquickextras**:
    * [**qtjambi-qt3dquickextras-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras/5.15.5/jar)
    * [**qtjambi-qt3dquickextras-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-jre8/5.15.5/jar)
    * [**qtjambi-qt3dquickextras-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3dquickextras-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3dquickextras-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-macos/5.15.5/jar)
* [Qt3DQuickScene3D](https://doc.qt.io/qt-5/qt3d-index.html) in module **qtjambi-qt3dquickscene2d**:
    * [**qtjambi-qt3dquickscene2d-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d/5.15.5/jar)
    * [**qtjambi-qt3dquickscene2d-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-jre8/5.15.5/jar)
    * [**qtjambi-qt3dquickscene2d-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-windows-x64/5.15.5/jar)
    * [**qtjambi-qt3dquickscene2d-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-linux-x64/5.15.5/jar)
    * [**qtjambi-qt3dquickscene2d-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-macos/5.15.5/jar)

### QtJambi Tools

* [QtJambi User Interface Compiler (UIC)](https://doc.qt.io/qt-5/designer-using-a-ui-file.html#compile-time-form-processing):
    * [**qtjambi-uic-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic/5.15.5/jar)
    * [**qtjambi-uic-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-jre8/5.15.5/jar)
    * [**qtjambi-uic-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-windows-x64/5.15.5/jar)
    * [**qtjambi-uic-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-linux-x64/5.15.5/jar)
    * [**qtjambi-uic-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uic-native-macos/5.15.5/jar)
* QtJambi Deployer:
    * [**qtjambi-deployer-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer/5.15.5/jar)
    * [**qtjambi-deployer-jre8-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-jre8/5.15.5/jar)
    * [**qtjambi-deployer-native-windows-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-windows-x64/5.15.5/jar)
    * [**qtjambi-deployer-native-linux-x64-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-linux-x64/5.15.5/jar)
    * [**qtjambi-deployer-native-macos-5.15.5.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-macos/5.15.5/jar)

