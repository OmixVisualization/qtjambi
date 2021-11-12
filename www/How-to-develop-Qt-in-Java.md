# How to develop with QtJambi

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
Find the [list of all available QtJambi modules](www/Modules.md).

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
javac -cp qtjambi-6.2.jar Test.java
```

For execution you need the platform dependent binaries of *QtJambi*. For
instance, if you are working on Windows download and unpack the win64
binaries. The required DLLs are to be found in the `bin` folder (`lib`
folder on Linux and macOS). Additionally, you need *Qt*. Use the [Qt
installer](https://www.qt.io/download-qt-installer) to install Qt on
your system. Make sure you are using the same Qt version and QtJambi
version (e.g. 5.15 or 6.2).

When running a QtJambi application you have to make the locations of Qt
and QtJambi libraries known to Java. Therefore, use the **PATH**
environment (**LD\_LIBRARY\_PATH** on Linux, **DYLD\_LIBRARY\_PATH** on
macOS) or the Java runtime property **java.library.path**. The example
program can be executed this way:

``` powershell
java -cp qtjambi-6.2.jar;qtjambi-native-windows-x64-6.2.jar;. -Djava.library.path=C:\Qt\6.2.0\msvc2019_64\bin Test
```

On *macOS* you need to use the start parameter `-XstartOnFirstThread`:

``` bash
java -XstartOnFirstThread -cp qtjambi-6.2.jar;qtjambi-native-windows-x64-6.2.jar;. -Djava.library.path=C:\Qt\6.2.0\msvc2019_64\bin Test
```

In general, you can start learning how to use Qt in Java [as it is
introduced for
C++](https://doc.qt.io/qt-6/gettingstarted.html#create-your-first-applications).
There are a couple of specifics for QtJambi that are [introduced
here](Characteristics-of-QtJambi.md). Instead of starting your
program with a java command as shown above you can deploy your
application as executable [as described
here](How-to-deploy-QtJambi-applications.md).

See [QtJambi 5.15 API Reference
Documentation](https://www.omix-visualization.com/qtjambidocs/5.15.2)
and [QtJambi 6.2 API Reference
Documentation](https://www.omix-visualization.com/qtjambidocs/latest)
