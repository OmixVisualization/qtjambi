![QtJambi - Java Bindings for Qt](https://github.com/OmixVisualization/qtjambi/raw/master/www/images/QtJambi_eyecatcher.png "QtJambi - Java Bindings for Qt")

# QtJambi - Java Bindings for Qt

## Overview

QtJambi is Qt bindings for the Java programming language originally developed by Trolltech, the predecessor of The Qt Company. 
The successive open source project provides bindings of Qt5 and Qt6 releases for latest Java JDK versions.

The bindings are generated by inspection of the C++ header files of the Qt implementation then emitting code to provide the 
necessary JNI (Java Native Interface) and .java files to provide linkage to Java. An application programmer using QtJambi 
would simply add the coresponding Java libraries (.jar files) to their Java project to be able to develop and deploy their application.

## Availability

QtJambi is available for Java 8 or 11 and higher using Qt5.15 and Qt6.2 in Java on Windows Linux and macOS. 
Most Qt modules are available as QtJambi module as [listed here](www/Modules.md). All modules are published as Maven Artifact.

## How to build

If you just want to use QtJambi for your application development you can skip these instructions 
and simply use the provided binaries from the [list of modules](www/Modules.md).
Make yourself familiar with [developing applications with QtJambi](www/How-to-develop-Qt-in-Java.md).

### Requirements
* [Apache Ant](https://ant.apache.org/) (min. 1.10.x)
* Java Development Kit (e.g. [OpenJDK](https://adoptopenjdk.net/), tested with Java 1.8 and 14)
* Qt 5.15 or 6.x (using the Qt Online Installer)
* C++ compiler (Clang, MSVC2019)
* chrpath (Linux only)
* XCode command line tools (macOS only)

### Building QtJambi
If you do'n need the entire set of Qt modules available in Java edit property `qtjambi.skipped.modules` in `build.properties` and exclude Qt modules you don't need.
This saves compilation time.

#### Pre-Build Steps On Windows
* Open the Visual Studio command prompt for x64 architecture.
> to be found in: --> Start Menu --> Programs --> Visual Studio 2019 --> Visual Studio Tools --> VC

* Change to directory of your QtJambi clone

* Set paths:

`> set JAVA_HOME_TARGET=path to your java jdk`

`> set PATH=...\apache-ant-1.10.3\bin;%PATH%`

`> set QTDIR=C:\Qt\6.2.0\msvc2019_64`

#### Pre-Build Steps On Linux and macOS

* Open a shell and change to directory of your QtJambi clone

* Set paths:

`> export JAVA_HOME_TARGET=path to your java jdk`

`> export QTDIR=/opt/Qt/6.2.0/gcc_64` on Linux

`> export QTDIR=/opt/Qt/6.2.0/macos` on macOS

#### Building Bindings

* build QtJambi:

`> ant all`

(This step may take several hours.)

* optionally, create and run unit tests:

`> ant tests.generate tests.run`

* Find unit test results here: `build/tests/TestResults-release` and `build/tests/TestResults-debug`.

* Finally, find all Java libraries in directory `deployment` and all native libraries in platform- and configuration-specific subdirectory, e.g. `native/windows-x64/release`.

## How To Use QtJambi

### Example

Add **qtjambi-5.15.jar**/**qtjambi-6.2.jar** to the classpath of your Java project containing the most 
essential Qt Core, Gui and Widgets modules. If you use Maven to build your application simply add following dependency
to your project:

```xml
<dependency>
  <groupId>io.qtjambi</groupId>
  <artifactId>qtjambi</artifactId>
  <version>$VERSION</version>
</dependency>
```
(exchange `$VERSION` either by `5.15` or by `6.2`).

Otherwise, download QtJambi JAR file from [Maven Central Repository](https://search.maven.org/artifact/io.qtjambi/qtjambi/).

Create a file **Test.java** containing the following code:

```java
import io.qt.widgets.*;
public class Test {
    public static void main(String[] args) {
        QApplication.initialize(args);
        QMessageBox.information(null, "QtJambi", "Hello World!");
        QApplication.shutdown();
    }
}
```

Compile the file:

``` powershell
javac -cp qtjambi-6.2.jar Test.java
```

### Execute Example

For execution you need the platform dependent binaries of QtJambi either as self-extracting bundle or as library file (e.b. dll). 
For instance, if you are working on Windows download **qtjambi-native-windows-x64-5.15.jar**/**qtjambi-native-windows-x64-6.2.jar**
from [Maven Central Repository](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-windows-x64/) and add it to java class path. 
Additionally, you need *Qt*. Use the [Qt installer](https://www.qt.io/download-qt-installer) to install Qt on your system. Make sure you are using the same Qt version and QtJambi version (e.g. 5.15 or 6.x).

When running a QtJambi application you have to make the locations of Qt libraries known to Java.
Therefore, use the PATH environment (LD_LIBRARY_PATH on Linux, DYLD_LIBRARY_PATH on macOS) 
or the Java runtime property java.library.path. The example program can be executed this way:

```
java -cp qtjambi-6.2.jar;qtjambi-native-windows-x64-6.2.jar;. -Djava.library.path=C:\Qt\6.2.0\msvc2019_64\bin Test
```

(On macOS you additionally need to use the start parameter -XstartOnFirstThread)

In general, you can start learning how to use Qt in Java [as it is introduced for C++](https://doc.qt.io/qt-6/gettingstarted.html#create-your-first-applications). 
There are a couple of specifics for QtJambi that are [introduced here](/www/Characteristics-of-QtJambi.md). 
Instead of starting your program with a java command as shown above you can deploy your application as executable as [described here](/www/How-to-deploy-QtJambi-applications.md).

## Further Reading

* [QtJambi modules](www/Modules.md)
* [How to develop with QtJambi](www/How-to-develop-Qt-in-Java.md)
* [Characteristics of QtJambi](www/Characteristics-of-QtJambi.md)
* [How to deploy QtJambi applications](www/How-to-deploy-QtJambi-applications.md)
* [QtJambi API Reference Documentation](https://doc.qtjambi.io/latest)
