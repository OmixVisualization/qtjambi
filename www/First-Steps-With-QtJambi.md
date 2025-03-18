# First Steps With QtJambi

[![Maven Central](https://maven-badges.herokuapp.com/maven-central/io.qtjambi/qtjambi/badge.svg)](https://search.maven.org/artifact/io.qtjambi/qtjambi)

Add **qtjambi.jar** to the classpath of your Java project containing the most 
essential Qt Core, Gui and Widgets modules. If you use Maven to build your application simply add following dependency
to your project:

```xml
<dependency>
  <groupId>io.qtjambi</groupId>
  <artifactId>qtjambi</artifactId>
  <version>$VERSION</version>
</dependency>
```
(exchange `$VERSION` either by `6.8.3` or any other published version).

Otherwise, download QtJambi JAR file from [Maven Central Repository](https://search.maven.org/artifact/io.qtjambi/qtjambi/).
Find the [list of all available QtJambi modules](www/Modules.md).

## Example Program

Create a file **Test.java** containing the following code

``` java
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
javac -cp qtjambi-6.8.3.jar Test.java
```

## Executing Example

For execution you need the platform dependent binaries of *QtJambi*. 
For instance, if you are working on Windows download the *windows-x64* binaries (**qtjambi-native-windows-x64-VERSION.jar**)
from [Maven Central Repository](https://search.maven.org/artifact/io.qtjambi/qtjambi-native-windows-x64/) and add it to java class path. 

Additionally, you need *Qt*. Use the [Qt installer](https://www.qt.io/download-qt-installer) to install Qt on your system.
Make sure you are using the same Qt version and QtJambi version (the first and second version digit needs to be the same, e.g. 6.5 or 6.8).
When using Maven artifacts for Windows you need to select MSVC 2022 64-Bit (msvc2022_64), as they are not compatible with Mingw Qt.
On Linux, you can alternatively use Qt system libraries (correct version provided).
The required DLLs are to be found in the `bin` folder on Windows and `lib` folder on Linux and macOS, respectively. 

When running a QtJambi application you have to make the locations of Qt
and QtJambi libraries known to Java. Therefore, use the `PATH`
environment (`LD_LIBRARY_PATH` on Linux, `DYLD_FRAMEWORK_PATH` on macOS) or the Java runtime property `java.library.path`.

The example program can be executed this way on Windows:

``` powershell
java -cp qtjambi-6.8.3.jar;qtjambi-native-windows-x64-6.8.3.jar;. -Djava.library.path=C:\Qt\6.8.3\msvc2022_64\bin Test
```

On Linux it looks this way:

``` bash
java -cp qtjambi-6.8.3.jar:qtjambi-native-linux-x64-6.8.3.jar:. -Djava.library.path=<path to>/Qt/6.8.3/gcc_64/lib Test
```

On macOS you additionally need to use the start parameter -XstartOnFirstThread:

``` bash
java -cp qtjambi-6.8.3.jar:qtjambi-native-macos-6.8.3.jar:. -Djava.library.path=<path to>/Qt/6.8.3/macos/lib -XstartOnFirstThread Test
```

On Linux, if your distribution provides Qt (of correct version) as system library you don't need to specify library path at all.
However, since Maven-published binaries for Linux have been built with Qt from installer they might be incompatible to your system-specific Qt.
In this case it is recommended to build QtJambi on your Linux system by using the installed `qmake` or install Qt by using Qt installer.

If the example fails with a `UnsatisfiedLinkError` QtJambi libraries and Qt libraries seem to be incompatible.
[Read here about library requirements and compatibility.](www/Modules.md)

### Native Components

QtJambi automatically detects the required native component jars if they are located next to their Java counterparts or in a subfolder `native`.
You can simply skip adding `qtjambi-native-OS-VERSION.jar` to your classpath (`-cp`).

If you intend to use automatic module loading (`java -p <dir>`) you strictly need to place native components in `native` subfolder next to `qtjambi-6.8.3.jar`.

Native bundles are extracted every time at program startup. By default, this is a process specific temporal directory purged after program shutdown.
Alternatively, you can use Java system property `io.qt.deploymentdir` to let libraries to be exctacted and persist in user 
application data path or common directory ([see Library Management](Options.md#library-management)).

## Further Reading

In general, you can start learning how to use Qt in Java [as it is introduced for C++](https://doc.qt.io/qt/gettingstarted.html#create-your-first-applications).
There are a special characteristics of QtJambi that are [introduced here](Characteristics-of-QtJambi.md).
Instead of starting your program with a java command as shown above you can deploy your
application as executable [as described here](How-to-deploy-QtJambi-applications.md).
[Read about options](Options.md) useful when runnung QtJambi.
[Read here](How-to-bundle-Qt-libraries.md) about creating self-exctracting bundles containing Qt library.
[Read more about how to debug a QtJambi program](How-to-debug.md).
[Read more about developing applications for Android](Android.md).

and [QtJambi 6.8 API Reference
Documentation](https://doc.qtjambi.io/6.8/)
