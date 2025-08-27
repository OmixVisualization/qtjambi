# How To Debug QtJambi Applications

There are different possibilities to debug a QtJambi application:

## Java Debugger

Naturally, you could use the Java debugger of your IDE to debug your code. This is straightforward, as your QtJambi application is written in Java or Kotlin.
If you want to attach a Java debugger to the [deployed executable](How-to-deploy-QtJambi-applications.md) you need to make sure that JVM arguments are passed through to the JVM by specifying corresponding filters.
Therefore, call Qtjambi Deployer with arguments `--filter=-D --filter=-X --filter=-agentlib: --filter=-javaagent: --filter=-agentpath:`. 
Now, all arguments given to the executable starting with `-D`, `-X`, `-agentlib:`, `-javaagent:` and `-agentpath:` are not used as program arguments but passed to JVM.

## QML Debugger

If your QtJambi application also uses QML code executed in a QML engine you can attach a QML debugger.
The application's capability to be QML-debugged has to be enabled explicitely. Therefore, you need to specify the following systems property: `io.qt.enable-qml-debugging=true`.
It is recommended to do this by `-D` JVM argument. Otherwise, you could hardcode it in your program code by `System.setProperty("io.qt.enable-qml-debugging", "true");`.
However, this has to be done right after entering main method. When launching the app, a warning is given now: `QML debugging is enabled. Only use this in a safe environment.`.

Furthermore, you need to call the application with following program argument: `-qmljsdebugger=...` to specify the port and other options [as described here](https://doc.qt.io/qt/qtquick-debugging.html#starting-applications).
Now, follow the instructions for [QML debugging](https://doc.qt.io/qt/qtquick-debugging.html#connecting-to-applications) to proceed.

## Native Debugger

For native debugging you need the debuginfo bundle for every native bundle your application is using. 
For instance, on Windows you need **qtjambi-debuginfo-windows-x64-6.8.5.jar** placed next to **qtjambi-native-windows-x64-6.8.5.jar**.
The debuginfo bundle contains platform-dependent debug symbol files for the native debugger and corresponding source code.
It is necessary to enable the extraction of these files during program start by using the system property `io.qt.provide-debuginfo=true` (`io.qt.no-native-deployment` must not be `true`).
By default, QtJambi does not extract source files unless you specify a target directory for those files with property `io.qt.sourcesdir`.
Specify `io.qt.sourcesdir=temp` to let QtJambi extract source code files to temporary directory, `user` for user's application data directory and
`common` for common program data directory. Specify a target directory (`io.qt.sourcesdir=<path>`) to let them be extracted at the specified location.

When attaching the native debugger to the QtJambi application all debugging symbols are now loaded. You need to add search paths for source code to the directory specified by `io.qt.sourcesdir`.
The source code target `io.qt.sourcesdir=temp` leads to a process-dependent search path which is resolved at program start. If your debugger needs source code locations prior to program start you should use one of the other options.
