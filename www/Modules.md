# QtJambi Modules

QtJambi is available for Qt5 and Qt6. All modules are published as Maven Artifact. Add them to your software development project by specifying as follows:

```xml
<dependency>
  <groupId>io.qtjambi</groupId>
  <artifactId>$MODULE</artifactId>
  <version>$VERSION</version>
</dependency>
```
Here, exchange `$VERSION` either by `5.15` or by `6.2`, exchange `$MODULE` by *QtJambi* module name as listed below.

Alternatively, you can download corresponding QtJambi modules as JAR file from [Maven Central Repository](https://search.maven.org/search?q=io.qtjambi).

All *QtJambi* modules consist of a Java part optionally for JDK8 or any higher JDK and a native component. 
Native components are avaiable for Windows (x64), Linux (x64) and macOS (x64 and arm64). 

## QtJambi for Qt 6.2

Following Qt 6.2 modules are provided by QtJambi:

### Qt6 Essentials

* [QtCore](https://doc.qt.io/qt-6/qtcore-index.html), [QtGui](https://doc.qt.io/qt-6/qtgui-index.html) and [QtWidgets](https://doc.qt.io/qt-6/qtwidgets-index.html) in 
    * [**qtjambi-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi)
    * [**qtjambi-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-jdk8)
    * [**qtjambi-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-windows-x64)
    * [**qtjambi-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-linux-x64)
    * [**qtjambi-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-macos)
* [QtDBus](https://doc.qt.io/qt-6/qtdbus-index.html) in 
    * [**qtjambi-dbus-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus)
    * [**qtjambi-dbus-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-jdk8)
    * [**qtjambi-dbus-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-windows-x64)
    * [**qtjambi-dbus-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-linux-x64)
    * [**qtjambi-dbus-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-dbus-native-macos)
* [QtNetwork](https://doc.qt.io/qt-6/qtnetwork-index.html) in 
    * [**qtjambi-network-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network)
    * [**qtjambi-network-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-jdk8)
    * [**qtjambi-network-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-windows-x64)
    * [**qtjambi-network-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-linux-x64)
    * [**qtjambi-network-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-network-native-macos)
* [QtQml](https://doc.qt.io/qt-6/qtqml-index.html) in 
    * [**qtjambi-qml-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml)
    * [**qtjambi-qml-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-jdk8)
    * [**qtjambi-qml-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-windows-x64)
    * [**qtjambi-qml-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-linux-x64)
    * [**qtjambi-qml-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qml-native-macos)
* [QtQuick](https://doc.qt.io/qt-6/qtquick-index.html) in 
    * [**qtjambi-quick-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick)
    * [**qtjambi-quick-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-jdk8)
    * [**qtjambi-quick-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-windows-x64)
    * [**qtjambi-quick-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-linux-x64)
    * [**qtjambi-quick-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick-native-macos)
* [QtQuickControls2](https://doc.qt.io/qt-6/qtquickcontrols-index.html) in 
    * [**qtjambi-quickcontrols-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols)
    * [**qtjambi-quickcontrols-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-jdk8)
    * [**qtjambi-quickcontrols-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-windows-x64)
    * [**qtjambi-quickcontrols-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-linux-x64)
    * [**qtjambi-quickcontrols-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickcontrols-native-macos)
* [QtTest](https://doc.qt.io/qt-6/qttest-index.html) in 
    * [**qtjambi-test-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test)
    * [**qtjambi-test-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-jdk8)
    * [**qtjambi-test-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-windows-x64)
    * [**qtjambi-test-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-linux-x64)
    * [**qtjambi-test-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-test-native-macos)

### Qt6 Add-Ons

* [QtBluetooth](https://doc.qt.io/qt-6/qtbluetooth-index.html) in 
    * [**qtjambi-bluetooth-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth)
    * [**qtjambi-bluetooth-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-jdk8)
    * [**qtjambi-bluetooth-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-windows-x64)
    * [**qtjambi-bluetooth-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-linux-x64)
    * [**qtjambi-bluetooth-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-bluetooth-native-macos)
* [QtCharts](https://doc.qt.io/qt-6/qtcharts-index.html) in 
    * [**qtjambi-charts-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts)
    * [**qtjambi-charts-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-jdk8)
    * [**qtjambi-charts-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-windows-x64)
    * [**qtjambi-charts-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-linux-x64)
    * [**qtjambi-charts-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-charts-native-macos)
* [QtDataVisualization](https://doc.qt.io/qt-6/qtdatavisualization-index.html) in 
    * [**qtjambi-datavisualization-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization)
    * [**qtjambi-datavisualization-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-jdk8)
    * [**qtjambi-datavisualization-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-windows-x64)
    * [**qtjambi-datavisualization-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-linux-x64)
    * [**qtjambi-datavisualization-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-datavisualization-native-macos)
* [QtConcurrent](https://doc.qt.io/qt-6/qtconcurrent-index.html) in 
    * [**qtjambi-concurrent-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent)
    * [**qtjambi-concurrent-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-jdk8)
    * [**qtjambi-concurrent-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-windows-x64)
    * [**qtjambi-concurrent-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-linux-x64)
    * [**qtjambi-concurrent-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-concurrent-native-macos)
* [QtHelp](https://doc.qt.io/qt-6/qthelp-index.html) in 
    * [**qtjambi-help-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help)
    * [**qtjambi-help-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-jdk8)
    * [**qtjambi-help-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-windows-x64)
    * [**qtjambi-help-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-linux-x64)
    * [**qtjambi-help-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-help-native-macos)
* [QtOpenGL](https://doc.qt.io/qt-6/qtopengl-index.html) in 
    * [**qtjambi-opengl-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl)
    * [**qtjambi-opengl-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-jdk8)
    * [**qtjambi-opengl-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-windows-x64)
    * [**qtjambi-opengl-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-linux-x64)
    * [**qtjambi-opengl-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-opengl-native-macos)
* [QtOpenGLWidgets](https://doc.qt.io/qt-6/qtopenglwidgets-module.html) in 
    * [**qtjambi-openglwidgets-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets)
    * [**qtjambi-openglwidgets-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-jdk8)
    * [**qtjambi-openglwidgets-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-windows-x64)
    * [**qtjambi-openglwidgets-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-linux-x64)
    * [**qtjambi-openglwidgets-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-openglwidgets-native-macos)
* [QtMultimedia](https://doc.qt.io/qt-6/qtmultimedia-index.html) in 
    * [**qtjambi-multimedia-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia)
    * [**qtjambi-multimedia-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-jdk8)
    * [**qtjambi-multimedia-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-windows-x64)
    * [**qtjambi-multimedia-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-linux-x64)
    * [**qtjambi-multimedia-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimedia-native-macos)
* [QtMultimediaWidgets](https://doc.qt.io/qt-6/qtmultimedia-index.html) in 
    * [**qtjambi-multimediawidgets-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets)
    * [**qtjambi-multimediawidgets-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-jdk8)
    * [**qtjambi-multimediawidgets-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-windows-x64)
    * [**qtjambi-multimediawidgets-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-linux-x64)
    * [**qtjambi-multimediawidgets-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-multimediawidgets-native-macos)
* [QtNfc](https://doc.qt.io/qt-6/qtnfc-index.html) in 
    * [**qtjambi-nfc-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc)
    * [**qtjambi-nfc-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-jdk8)
    * [**qtjambi-nfc-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-windows-x64)
    * [**qtjambi-nfc-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-linux-x64)
    * [**qtjambi-nfc-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-nfc-native-macos)
* [QtNetworkAuth](https://doc.qt.io/qt-6/qtnetworkauth-index.html) in 
    * [**qtjambi-networkauth-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth)
    * [**qtjambi-networkauth-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-jdk8)
    * [**qtjambi-networkauth-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-windows-x64)
    * [**qtjambi-networkauth-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-linux-x64)
    * [**qtjambi-networkauth-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-networkauth-native-macos)
* [QtPositioning](https://doc.qt.io/qt-6/qtpositioning-index.html) in 
    * [**qtjambi-positioning-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning)
    * [**qtjambi-positioning-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-jdk8)
    * [**qtjambi-positioning-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-windows-x64)
    * [**qtjambi-positioning-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-linux-x64)
    * [**qtjambi-positioning-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-positioning-native-macos)
* [QtPrintSupport](https://doc.qt.io/qt-6/qtprintsupport-index.html) in 
    * [**qtjambi-printsupport-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport)
    * [**qtjambi-printsupport-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-jdk8)
    * [**qtjambi-printsupport-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-windows-x64)
    * [**qtjambi-printsupport-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-linux-x64)
    * [**qtjambi-printsupport-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-printsupport-native-macos)
* [QtQuick3D](https://doc.qt.io/qt-6/qtquick3d-index.html) in 
    * [**qtjambi-quick3d-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d)
    * [**qtjambi-quick3d-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-jdk8)
    * [**qtjambi-quick3d-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-windows-x64)
    * [**qtjambi-quick3d-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-linux-x64)
    * [**qtjambi-quick3d-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quick3d-native-macos)
* [QtQuickWidgets](https://doc.qt.io/qt-6/qtquickwidgets-module.html) in 
    * [**qtjambi-quickwidgets-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets)
    * [**qtjambi-quickwidgets-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-jdk8)
    * [**qtjambi-quickwidgets-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-windows-x64)
    * [**qtjambi-quickwidgets-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-linux-x64)
    * [**qtjambi-quickwidgets-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-quickwidgets-native-macos)
* [QtRemoteObjects](https://doc.qt.io/qt-6/qtremoteobjects-module.html) in 
    * [**qtjambi-remoteobjects-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects)
    * [**qtjambi-remoteobjects-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-jdk8)
    * [**qtjambi-remoteobjects-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-windows-x64)
    * [**qtjambi-remoteobjects-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-linux-x64)
    * [**qtjambi-remoteobjects-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-remoteobjects-native-macos)
* [QtSCXML](https://doc.qt.io/qt-6/qtscxml-index.html) in 
    * [**qtjambi-scxml-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml)
    * [**qtjambi-scxml-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-jdk8)
    * [**qtjambi-scxml-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-windows-x64)
    * [**qtjambi-scxml-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-linux-x64)
    * [**qtjambi-scxml-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-scxml-native-macos)
* [QtSensors](https://doc.qt.io/qt-6/qtsensors-index.html) in 
    * [**qtjambi-sensors-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors)
    * [**qtjambi-sensors-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-jdk8)
    * [**qtjambi-sensors-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-windows-x64)
    * [**qtjambi-sensors-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-linux-x64)
    * [**qtjambi-sensors-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sensors-native-macos)
* [QtSerialBus](https://doc.qt.io/qt-6/qtserialbus-index.html) in 
    * [**qtjambi-serialbus-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus)
    * [**qtjambi-serialbus-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-jdk8)
    * [**qtjambi-serialbus-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-windows-x64)
    * [**qtjambi-serialbus-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-linux-x64)
    * [**qtjambi-serialbus-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialbus-native-macos)
* [QtSerialPort](https://doc.qt.io/qt-6/qtserialport-index.html) in 
    * [**qtjambi-serialport-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport)
    * [**qtjambi-serialport-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-jdk8)
    * [**qtjambi-serialport-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-windows-x64)
    * [**qtjambi-serialport-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-linux-x64)
    * [**qtjambi-serialport-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-serialport-native-macos)
* [QtSql](https://doc.qt.io/qt-6/qtsql-index.html) in 
    * [**qtjambi-sql-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql)
    * [**qtjambi-sql-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-jdk8)
    * [**qtjambi-sql-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-windows-x64)
    * [**qtjambi-sql-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-linux-x64)
    * [**qtjambi-sql-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-sql-native-macos)
* [QtSvg](https://doc.qt.io/qt-6/qtsvg-index.html) in 
    * [**qtjambi-svg-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg)
    * [**qtjambi-svg-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-jdk8)
    * [**qtjambi-svg-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-windows-x64)
    * [**qtjambi-svg-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-linux-x64)
    * [**qtjambi-svg-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-svg-native-macos)
* [QtStateMachine](https://doc.qt.io/qt-6/qtstatemachine-index.html) in 
    * [**qtjambi-statemachine-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine)
    * [**qtjambi-statemachine-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-jdk8)
    * [**qtjambi-statemachine-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-windows-x64)
    * [**qtjambi-statemachine-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-linux-x64)
    * [**qtjambi-statemachine-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-statemachine-native-macos)
* [QtUITools](https://doc.qt.io/qt-6/qtuitools-index.html) in 
    * [**qtjambi-uitools-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools)
    * [**qtjambi-uitools-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-jdk8)
    * [**qtjambi-uitools-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-windows-x64)
    * [**qtjambi-uitools-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-linux-x64)
    * [**qtjambi-uitools-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-uitools-native-macos)
* [QtWebChannel](https://doc.qt.io/qt-6/qtwebchannel-index.html) in 
    * [**qtjambi-webchannel-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel)
    * [**qtjambi-webchannel-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-jdk8)
    * [**qtjambi-webchannel-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-windows-x64)
    * [**qtjambi-webchannel-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-linux-x64)
    * [**qtjambi-webchannel-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webchannel-native-macos)
* [QtWebEngineCore](https://doc.qt.io/qt-6/qtwebengine-index.html) in 
    * [**qtjambi-webenginecore-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore)
    * [**qtjambi-webenginecore-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-jdk8)
    * [**qtjambi-webenginecore-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-windows-x64)
    * [**qtjambi-webenginecore-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-linux-x64)
    * [**qtjambi-webenginecore-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginecore-native-macos)
* [QtWebEngineWidgets](https://doc.qt.io/qt-6/qtwebengine-index.html) in 
    * [**qtjambi-webenginewidgets-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets)
    * [**qtjambi-webenginewidgets-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-jdk8)
    * [**qtjambi-webenginewidgets-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-windows-x64)
    * [**qtjambi-webenginewidgets-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-linux-x64)
    * [**qtjambi-webenginewidgets-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginewidgets-native-macos)
* [QtWebEngineQuick](https://doc.qt.io/qt-6/qtwebengine-index.html) in 
    * [**qtjambi-webenginequick-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick)
    * [**qtjambi-webenginequick-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-jdk8)
    * [**qtjambi-webenginequick-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-windows-x64)
    * [**qtjambi-webenginequick-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-linux-x64)
    * [**qtjambi-webenginequick-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webenginequick-native-macos)
* [QtWebSockets](https://doc.qt.io/qt-6/qtwebsockets-index.html) in 
    * [**qtjambi-websockets-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets)
    * [**qtjambi-websockets-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-jdk8)
    * [**qtjambi-websockets-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-windows-x64)
    * [**qtjambi-websockets-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-linux-x64)
    * [**qtjambi-websockets-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-websockets-native-macos)
* [QtWebView](https://doc.qt.io/qt-6/qtwebview-index.html) in 
    * [**qtjambi-webview-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview)
    * [**qtjambi-webview-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-jdk8)
    * [**qtjambi-webview-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-windows-x64)
    * [**qtjambi-webview-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-linux-x64)
    * [**qtjambi-webview-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-webview-native-macos)
* [QtVirtualKeyboard](https://doc.qt.io/qt-6/qtvirtualkeyboard-index.html) in 
    * [**qtjambi-virtualkeyboard-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard)
    * [**qtjambi-virtualkeyboard-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-jdk8)
    * [**qtjambi-virtualkeyboard-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-windows-x64)
    * [**qtjambi-virtualkeyboard-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-linux-x64)
    * [**qtjambi-virtualkeyboard-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-virtualkeyboard-native-macos)
* [QtXml](https://doc.qt.io/qt-6/qtxml-index.html) in 
    * [**qtjambi-xml-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml)
    * [**qtjambi-xml-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-jdk8)
    * [**qtjambi-xml-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-windows-x64)
    * [**qtjambi-xml-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-linux-x64)
    * [**qtjambi-xml-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-xml-native-macos)
* [QtDesigner](https://doc.qt.io/qt-6/qtdesigner-manual.html) in 
    * [**qtjambi-designer-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer)
    * [**qtjambi-designer-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-jdk8)
    * [**qtjambi-designer-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-windows-x64)
    * [**qtjambi-designer-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-linux-x64)
    * [**qtjambi-designer-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-designer-native-macos)

### Qt3D

* [Qt3DCore](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3dcore-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore)
    * [**qtjambi-qt3dcore-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-jdk8)
    * [**qtjambi-qt3dcore-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-windows-x64)
    * [**qtjambi-qt3dcore-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-linux-x64)
    * [**qtjambi-qt3dcore-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dcore-native-macos)
* [Qt3DRender](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3drender-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender)
    * [**qtjambi-qt3drender-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-jdk8)
    * [**qtjambi-qt3drender-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-windows-x64)
    * [**qtjambi-qt3drender-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-linux-x64)
    * [**qtjambi-qt3drender-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3drender-native-macos)
* [Qt3DAnimation](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3danimation-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation)
    * [**qtjambi-qt3danimation-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-jdk8)
    * [**qtjambi-qt3danimation-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-windows-x64)
    * [**qtjambi-qt3danimation-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-linux-x64)
    * [**qtjambi-qt3danimation-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3danimation-native-macos)
* [Qt3DExtras](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3dextras-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras)
    * [**qtjambi-qt3dextras-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-jdk8)
    * [**qtjambi-qt3dextras-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-windows-x64)
    * [**qtjambi-qt3dextras-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-linux-x64)
    * [**qtjambi-qt3dextras-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dextras-native-macos)
* [Qt3DInput](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3dinput-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput)
    * [**qtjambi-qt3dinput-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-jdk8)
    * [**qtjambi-qt3dinput-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-windows-x64)
    * [**qtjambi-qt3dinput-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-linux-x64)
    * [**qtjambi-qt3dinput-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dinput-native-macos)
* [Qt3DLogic](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3dlogic-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic)
    * [**qtjambi-qt3dlogic-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-jdk8)
    * [**qtjambi-qt3dlogic-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-windows-x64)
    * [**qtjambi-qt3dlogic-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-linux-x64)
    * [**qtjambi-qt3dlogic-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dlogic-native-macos)
* [Qt3DQuick](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3dquick-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick)
    * [**qtjambi-qt3dquick-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-jdk8)
    * [**qtjambi-qt3dquick-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-windows-x64)
    * [**qtjambi-qt3dquick-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-linux-x64)
    * [**qtjambi-qt3dquick-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquick-native-macos)
* [Qt3DQuickExtras](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3dquickextras-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras)
    * [**qtjambi-qt3dquickextras-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-jdk8)
    * [**qtjambi-qt3dquickextras-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-windows-x64)
    * [**qtjambi-qt3dquickextras-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-linux-x64)
    * [**qtjambi-qt3dquickextras-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickextras-native-macos)
* [Qt3DQuickScene3D](https://doc.qt.io/qt-6/qt3d-index.html) in 
    * [**qtjambi-qt3dquickscene2d-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d)
    * [**qtjambi-qt3dquickscene2d-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-jdk8)
    * [**qtjambi-qt3dquickscene2d-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-windows-x64)
    * [**qtjambi-qt3dquickscene2d-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-linux-x64)
    * [**qtjambi-qt3dquickscene2d-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-qt3dquickscene2d-native-macos)

### QtJambi Deployment Tools

* QtJambi Deployer in 
    * [**qtjambi-deployer-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer)
    * [**qtjambi-deployer-jdk8-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-jdk8)
    * [**qtjambi-deployer-native-windows-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-windows-x64)
    * [**qtjambi-deployer-native-linux-x64-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-linux-x64)
    * [**qtjambi-deployer-native-macos-6.2.jar**](https://search.maven.org/artifact/io.qtjambi/qtjambi-deployer-native-macos)

