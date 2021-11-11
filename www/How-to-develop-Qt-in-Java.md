# First Example Application

Add
**[qtjambi-5.15.jar](https://github.com/OmixVisualization/qtjambi/releases/download/v6.0/qtjambi-5.15.jar)**
/
**[qtjambi-6.x.jar](https://github.com/OmixVisualization/qtjambi/releases/download/v6.1/qtjambi-6.1.jar)**
to the classpath of your Java project containing the most essential Qt
Core, Gui and Widgets modules.

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
javac -cp qtjambi-5.15.jar Test.java
```

For execution you need the platform dependent binaries of *QtJambi*. For
instance, if you are working on Windows download and unpack the win64
binaries. The required DLLs are to be found in the `bin` folder (`lib`
folder on Linux and macOS). Additionally, you need *Qt*. Use the [Qt
installer](https://www.qt.io/download-qt-installer) to install Qt on
your system. Make sure you are using the same Qt version and QtJambi
version (e.g. 5.15 or 6.x).

When running a QtJambi application you have to make the locations of Qt
and QtJambi libraries known to Java. Therefore, use the **PATH**
environment (**LD\_LIBRARY\_PATH** on Linux, **DYLD\_LIBRARY\_PATH** on
macOS) or the Java runtime property **java.library.path**. The example
program can be executed this way:

``` powershell
java -cp qtjambi-6.1.jar;. -Djava.library.path=C:\Qt\6.1.0\msvc2019_64\bin;bin Test
```

On *macOS* you need to use the start parameter `-XstartOnFirstThread`:

``` bash
java -XstartOnFirstThread -cp qtjambi-6.1.jar:. -Djava.library.path=/Libraries/Qt/6.1.0/msvc2019_64/lib:lib Test
```

In general, you can start learning how to use Qt in Java [as it is
introduced for
C++](https://doc.qt.io/qt-6/gettingstarted.html#create-your-first-applications).
There are a couple of specifics for QtJambi that are [introduced
here](Characteristics-of-QtJambi "wikilink"). Instead of starting your
program with a java command as shown above you can deploy your
application as executable [as described
here](How-to-deploy-QtJambi-applications "wikilink").

See [QtJambi 5.15 API Reference
Documentation](https://www.omix-visualization.com/qtjambidocs/5.15.2)
and [QtJambi 6.x API Reference
Documentation](https://www.omix-visualization.com/qtjambidocs/latest)
