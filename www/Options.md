# Useful Java System Properties for QtJambi

Following system properties are accepted by QtJambi.
You can specify Java system properties as start argument `-Dproperty=value` or in Java code `System.setProperty("property", "value")`.

## Logging

* `io.qt.log-messages` - By specifying any combination of `ALL`, `CRITICAL`, `DEBUG`, `WARNING`, `FATAL`, `INFO` and `SYSTEM` you can install a message handler forwarding messages of given types to Java logging. In combination with this use following properties to define logging levels. All specified values have to be parsable by [`Level.parse(String)`](https://docs.oracle.com/en/java/javase/11/docs/api/java.logging/java/util/logging/Level.html#parse(java.lang.String)):
    * `io.qt.log.debug.level` - Specify level value to define the log level for `QtDebugMsg` (defaults to `FINEST`)
    * `io.qt.log.critical.level` - Specify level value to define the log level for `QtCriticalMsg` (defaults to `SEVERE`)
    * `io.qt.log.fatal.level` - Specify level value to define the log level for `QtFatalMsg` (defaults to `SEVERE`)
    * `io.qt.log.info.level` - Specify level value to define the log level for `QtInfoMsg` (defaults to `INFO`)
    * `io.qt.log.warning.level` - Specify level value to define the log level for `QtWarningMsg` (defaults to `WARNING`)
* `io.qt.exceptions-for-messages` - By specifying any combination of `ALL`, `CRITICAL`, `DEBUG`, `WARNING`, `FATAL`, `INFO` and `SYSTEM` you can install a message handler causing exceptions to be thrown in the event of a message of given type.

## Runtime Diagnostics

* `io.qt.enable-event-logs` - Specify `true` to activate logging of all handled events with their receivers.
* `io.qt.enable-method-logs` - Specify `true` to activate method logging. *Caution! This highly decreases performance especially in combination with `io.qt.log-messages` or custom message handlers.*
* `io.qt.enable-cleanup-logs` - Specify `true` to log native object cleanup after GC.
* `io.qt.enable-metaobject-logs` - Specify `true` to log metaobject analysis results.
* `io.qt.enable-dangling-pointer-check` - Specify `true` to activate dangling pointer checks.
* `io.qt.enable-signal-argument-check` - Specify `true` to activate signal argument checks where applicable.
* `io.qt.enable-signal-emit-thread-check` - Specify `true` to activate thread checks on signal emits. If a signal is emitted from a foreign thread a warning is given. Install custom handler with `QtUtilities.installSignalEmitThreadCheckHandler` to change this behavior.
* `io.qt.enable-concurrent-container-modification-check` - Specify `true` to activate concurrent modification checks during container iteration.
* `io.qt.enable-thread-affinity-check` - Specify `true` to activate thread affinity checks when calling certain thread-affine methods.
* `io.qt.enable-event-thread-affinity-check` - ...the same applying to access checks during event handling.
* ~~`io.qt.disable-thread-affinity-check`~~ - _check is disabled by default since QtJambi 6.5.1._
* ~~`io.qt.disable-event-thread-affinity-check`~~ - _check is disabled by default since QtJambi 6.5.1._

## Library Management

* `io.qt.library-path-override` - Use this property if you want to force Java to load Qt and QtJambi libraries from other paths than given by `java.library.path`.
* `io.qt.debug` - Specify `debug` to force Java using debug libraries of Qt and QtJambi.
* `io.qt.pluginpath` - Specify list of paths added as plugin search path.

If you don't specify QtJambi's native location in library path QtJambi will extract libraries from native bundles to temporary directory each time at program startup.
Typically, it is a process specific directory purged at program termination. This behavior can be adapted with following properties:

* `io.qt.deploymentdir` - Specify `temp` to let QtJambi extract libraries to temporary directory (default), `user` for user's application data directory and `common` for common program data directory. Specify a target directory (`io.qt.deploymentdir=<path>`) to let them be extracted at the specified location.
* `io.qt.keep-temp-deployment` - Specify `true` to avoid library deletion at program termination. The libraries remain in temporary directory instead.
* `io.qt.no-native-deployment` - Specify `true` if you want to inhibit the search for native library bundles at all and load QtJambi from library path instead.

Library extraction only takes place if QtJambi does not detect the required libraries elsewhere (e.g. in `io.qt.library-path-override`, `java.library.path` or system's library path).

## Debugging

When using a native debugger to debug your application you need to provide debuginfo along with the native Qtjambi libraries.
This information is available in specific bundles corresponding to native bundles. QtJambi will extract the debug info files if you enable it with `io.qt.provide-debuginfo=true`.
The extraction behavior can be adapted by following properties: 

* `io.qt.provide-debuginfo` - Specify `true` to let QtJambi extract debuginfo files.

Along with debuginfo files, source code files could be extracted. However, this is not done unless you specify a target sources directory:

* `io.qt.sourcesdir` - Specify `temp` to let QtJambi extract source code files to temporary directory, `user` for user's application data directory and `common` for common program data directory. Specify a target directory (`io.qt.sourcesdir=<path>`) to let them be extracted at the specified location.

## Development

For development purpose it is possible to let QtJambi extract header files from native bundles. 

* `io.qt.headersdir` - Specify `user` to let QtJambi extract header files to user's application data directory. Specify `common` to let them be extracted to common program data directory. Specify a target directory to let them be extracted there.

## QtJambi Runtime

* `io.qt.allow-nonfinal-signals` - Specify `true` to avoid exception to be thrown when detecting non-final signal declarations.
* `io.qt.allow-nonfinal-qproperties` - Specify `true` to avoid exception to be thrown when detecting non-final QProperty declarations.
* `io.qt.no-library-shutdown-hook` - Specify `true` to avoid library shutdown at program termination.
* `io.qt.no-app-deletion` - Specify `true` if you combine native code with Java code and your `QCoreApplication` instance has been created elsewhere than inside Java.
* `io.qt.no-exception-forwarding-from-meta-calls` - Specify `true` to avoid `QMetaObject` calls (like method invocation and property access) forwarding exceptions to the Qt caller in any case (it is avoided by default in case of Qml-related QObjects).
* `io.qt.no-exception-forwarding-from-virtual-calls` - Specify `true` to avoid virtual calls (i.e. Java overrides) forwarding exceptions to the Qt caller in any case.
* `io.qt.avoid-jni-global-references` - Specify `true` to avoid using JNI global references. Instead, QtJambi will use a Java `HashMap` to manage references. (default on Android)
* `io.qt.use-jni-global-references` - Only on Android: Specify `true` to let QtJambi use JNI global references instead of `HashMap` to manage references.
* `io.qt.disable-thread-purger` - Specify `true` to not starting a watcher thread for purging Java-related thread data.
* `io.qt.resourcepath` - (semi)colon-separated list of jar files or directories to be added as Qt resource path at initialization. Skips classpath from being used.
* `io.qt.no-classpath-to-resourcepath` - Specify `true` to not adding Java classpath to Qt resource path. You may neeed to do this manually by using `QResource.addClassPath`.
* `io.qt.no-directories-to-resourcepath` - Specify `true` to not adding Java classpath directories to Qt resource path (jar files only). This has no effect on `QResource.addClassPath`.
* `io.qt.allow-classfiles-as-resource` - Specify `true` to allow `.class` files in jar bundles to be found by Qt resources.
* `io.qt.allow-meta-inf-as-resource` - Specify `true` to add the META-INF of jar files to Qt resource path (legacy option).
* `io.qt.resource-cache-timeout` - Specify the amount of time in milliseconds a resource is kept alive in cache.

## QML

* `io.qt.enable-qml-debugging` - Specify `true` to allow QML debugging for the entire runtime of the application.
* ~~`io.qt.enabled-qml-debugging`~~ - Specify `true` to allow QML debugging for the entire runtime of the application.
* `io.qt.enable-qml-debugging-nowarn` - ...also inhibits a security warning.
* ~~`io.qt.enabled-qml-debugging-nowarn`~~ - ...also inhibits a security warning.

Along with this use program argument `-qmljsdebugger=...` to enable QML debugging for Qt. 

## Experimental Features

* `io.qt.experimental.fast-jni` - Specify `true` to enable improved gathering of JNI environment during native calls
* `io.qt.experimental.fast-jni-for-overrides` - Specify `true` to enable improved gathering of JNI environment during native virtual calls

Following properties deal with the improvement of item model performance:

* `io.qt.experimental.item-model-cached-virtuals` - Specify `true` to cache the return values of item model's `rowCount()` and `columnCount()` to improve performance of item views.
* `io.qt.experimental.enable-invalid-modelindex-singleton` - Specify `true` to use one single Java object to represent invalid `QModelIndex` instead of copies.
* `io.qt.experimental.enable-ephemeral-modelindexes` - Specify `true` to call virtual Java methods with an alias for QModelIndex arguments instead of submitting a copies. These alias objects are invalidated after use.
