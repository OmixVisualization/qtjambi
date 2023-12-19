# How To Develop With QtJambi

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
(exchange `$VERSION` either by `5.15.18`, by `6.5.4` or by `6.6.1`).

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
javac -cp qtjambi-6.5.4.jar Test.java
```

## Executing Example

For execution you need the platform dependent binaries of *QtJambi*. For
instance, if you are working on Windows download the *windows-x64*
binaries. Additionally, you need *Qt*. Use the [Qt
installer](https://www.qt.io/download-qt-installer) to install Qt on
your system. Make sure you are using the same Qt version and QtJambi
version (e.g. 5.15 or 6.5). On Linux, you can alternatively use Qt system libraries (correct version provided).
The required DLLs are to be found in the `bin` folder on Windows and `lib` folder on Linux and macOS, respectively. 

When running a QtJambi application you have to make the locations of Qt
and QtJambi libraries known to Java. Therefore, use the **PATH**
environment (**LD\_LIBRARY\_PATH** on Linux, **DYLD\_FRAMEWORK\_PATH** on
macOS) or the Java runtime property **java.library.path**.

The example program can be executed this way on Windows:

``` powershell
java -cp qtjambi-6.5.4.jar;qtjambi-native-windows-x64-6.5.4.jar;. -Djava.library.path=C:\Qt\6.5.3\msvc2019_64\bin Test
```

On Linux it looks this way:

``` bash
java -cp qtjambi-6.5.4.jar:qtjambi-native-linux-x64-6.5.4.jar:. -Djava.library.path=<path to>/Qt/6.5.3/gcc_64/lib Test
```

On macOS you additionally need to use the start parameter -XstartOnFirstThread:

``` bash
java -cp qtjambi-6.5.4.jar:qtjambi-native-macos-6.5.4.jar:. -Djava.library.path=<path to>/Qt/6.5.3/macos/lib -XstartOnFirstThread Test
```

If the example fails with a `UnsatisfiedLinkError` QtJambi libraries and Qt libraries seem to be incompatible.
[Read here about library requirements and compatibility.](www/Modules.md)

### Native Components

QtJambi automatically detects the required native component jars if they are located next to their Java counterparts or in a subfolder `native`.
You can simply skip `qtjambi-native-OS-VERSION.jar` in your classpath (`-cp`).

If you intend to use automatic module loading (`java -p <dir>`) you strictly need to place native components in `native` subfolder next to `qtjambi-6.5.4.jar`.

Native bundles are extracted every time at program startup. By default, this is a process specific temporal directory purged after program shutdown.
Alternatively, you can use Java system property `io.qt.deploymentdir` to let libraries to be exctacted and persist in user 
application data path or common directory ([see below](#library-management)).

## Further Reading

In general, you can start learning how to use Qt in Java [as it is
introduced for
C++](https://doc.qt.io/qt/gettingstarted.html#create-your-first-applications).
There are a special characteristics of QtJambi that are [introduced
here](Characteristics-of-QtJambi.md). Instead of starting your
program with a java command as shown above you can deploy your
application as executable [as described
here](How-to-deploy-QtJambi-applications.md).
[Read here](www/How-to-bundle-Qt-libraries.md) about creating self-exctracting bundles containing Qt library.
[Read more about how to debug a QtJambi program](www/How-to-debug.md).
[Read more about developing applications for Android](Android.md).

See [QtJambi 5.15 API Reference
Documentation](https://doc.qtjambi.io/5.15.18/),
and [QtJambi 6.5 API Reference
Documentation](https://doc.qtjambi.io/6.5.4/)

## Useful Java System Properties for QtJambi

Following system properties are accepted by QtJambi.
You can specify Java system properties as start argument `-Dproperty=value` or in Java code `System.setProperty("property", "value")`.

### Logging

* `io.qt.log-messages` - By specifying any combination of `ALL`, `CRITICAL`, `DEBUG`, `WARNING`, `FATAL`, `INFO` and `SYSTEM` you can install a message handler forwarding messages of given types to Java logging. In combination with this use following properties to define logging levels. All specified values have to be parsable by [`Level.parse(String)`](https://docs.oracle.com/en/java/javase/11/docs/api/java.logging/java/util/logging/Level.html#parse(java.lang.String)):
    * `io.qt.log.debug.level` - Specify level value to define the log level for `QtDebugMsg` (defaults to `FINEST`)
    * `io.qt.log.critical.level` - Specify level value to define the log level for `QtCriticalMsg` (defaults to `SEVERE`)
    * `io.qt.log.fatal.level` - Specify level value to define the log level for `QtFatalMsg` (defaults to `SEVERE`)
    * `io.qt.log.info.level` - Specify level value to define the log level for `QtInfoMsg` (defaults to `INFO`)
    * `io.qt.log.warning.level` - Specify level value to define the log level for `QtWarningMsg` (defaults to `WARNING`)
* `io.qt.exceptions-for-messages` - By specifying any combination of `ALL`, `CRITICAL`, `DEBUG`, `WARNING`, `FATAL`, `INFO` and `SYSTEM` you can install a message handler causing exceptions to be thrown in the event of a message of given type.

### Runtime Diagnostics

* `io.qt.enable-method-logs` - Specify `true` to activate method logging. *Caution! This highly decreases performance especially in combination with `io.qt.log-messages` or custom message handlers.*
* `io.qt.enable-dangling-pointer-check` - Specify `true` to activate dangling pointer checks.
* `io.qt.enable-concurrent-container-modification-check` - Specify `true` to activate concurrent modification checks during container iteration.
* `io.qt.enable-thread-affinity-check` - Specify `true` to activate thread affinity checks when calling certain thread-affine methods.
* `io.qt.enable-event-thread-affinity-check` - ...the same applying to access checks during event handling.
* ~~`io.qt.disable-thread-affinity-check`~~ - _check is disabled by default since QtJambi 6.5.1._
* ~~`io.qt.disable-event-thread-affinity-check`~~ - _check is disabled by default since QtJambi 6.5.1._

### Library Management

* `io.qt.library-path-override` - Use this property if you want to force Java to load Qt and QtJambi libraries from other paths than given by `java.library.path`.
* `io.qt.debug` - Specify `debug` to force Java using debug libraries of Qt and QtJambi.
* `io.qt.verbose-loading` - Specify `true` to cause QtJambi to print out library loading steps.
* `io.qt.pluginpath` - Specify list of paths added as plugin search path.

If you don't specify QtJambi's native location in library path QtJambi will extract libraries from native bundles to temporary directory each time at program startup.
Typically, it is a process specific directory purged at program termination. This behavior can be adapted with following properties:

* `io.qt.deploymentdir` - Specify `temp` to let QtJambi extract libraries to temporary directory (default), `user` for user's application data directory and `common` for common program data directory. Specify a target directory (`io.qt.sourcesdir=<path>`) to let them be extracted at the specified location.
* `io.qt.keep-temp-deployment` - Specify `true` to avoid library deletion at program termination. The libraries remain in temporary directory instead.
* `io.qt.no-native-deployment` - Specify `true` if you want to inhibit the search for native library bundles at all and load QtJambi from library path instead.

Library extraction only takes place if QtJambi does not detect the required libraries elsewhere (e.g. in `io.qt.library-path-override`, `java.library.path` or system's library path).

### Debugging

When using a native debugger to debug your application you need to provide debuginfo along with the native Qtjambi libraries.
This information is available in specific bundles corresponding to native bundles. QtJambi will extract the debug info files if you enable it with `io.qt.provide-debuginfo=true`.
The extraction behavior can be adapted by following properties: 

* `io.qt.provide-debuginfo` - Specify `true` to let QtJambi extract debuginfo files.

Along with debuginfo files, source code files could be extracted. However, this is not done unless you specify a target sources directory:

* `io.qt.sourcesdir` - Specify `temp` to let QtJambi extract source code files to temporary directory, `user` for user's application data directory and `common` for common program data directory. Specify a target directory (`io.qt.sourcesdir=<path>`) to let them be extracted at the specified location.

### Development

For development purpose it is possible to let QtJambi extract header files from native bundles. 

* `io.qt.headersdir` - Specify `user` to let QtJambi extract header files to user's application data directory. Specify `common` to let them be extracted to common program data directory. Specify a target directory to let them be extracted there.

### QtJambi Runtime

* `io.qt.allow-nonfinal-signals` - Specify `true` to avoid exception to be thrown when detecting non-final signal declarations.
* `io.qt.no-library-shutdown-hook` - Specify `true` to avoid library shutdown at program termination.
* `io.qt.no-app-deletion` - Specify `true` if you combine native code with Java code and your `QCoreApplication` instance has been created elsewhere than inside Java.

### QML

* `io.qt.enable-qml-debugging` - Specify `true` to allow QML debugging for the entire runtime of the application.
* ~~`io.qt.enabled-qml-debugging`~~ - Specify `true` to allow QML debugging for the entire runtime of the application.
* `io.qt.enable-qml-debugging-nowarn` - ...also inhibits a security warning.
* ~~`io.qt.enabled-qml-debugging-nowarn`~~ - ...also inhibits a security warning.

Along with this use program argument `-qmljsdebugger=...` to enable QML debugging for Qt. 
