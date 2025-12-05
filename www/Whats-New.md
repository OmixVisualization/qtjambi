# What's new in QtJambi 6.10.1

* Removing Qt5 backward compatibility
* QFuture and QPromise are now native wrapper types
* The new internal class `io.qt.internal.LibraryExtractor` (main) can be used to extract all required QtJambi libraries.
* `QQmlInfo` not mirrors native API entirely.
* All appearances of quint16 http ports now map to Java's int type.
* Fixed memory leak in using function pointers.

# History

## What's new in QtJambi 6.10.0

QtJambi 6.10.0 is equivalent to QtJambi 6.9.3.

## What's new in QtJambi 6.9.3

* In-place construction
* QObject::destructed signal can now execute instance methods of the destroyed object if they are connected by method handle.
* Full availability of new QRangeModel.
* Bugfix: memory leak when using lambda expressions in signal connections
* Bugfix: sharp character in file name of classpath resources foes not work
* Bugfix: comparing empty QHash containers always false
* Bugfix: QList::takeAt for first of two entries fails
* New option `io.qt.resourcepath` to specify jar files for Qt resource system (instead using classpath).
* New option `io.qt.no-classpath-to-resourcepath` to avoid Java class path top be used as Qt resource.
* New option `io.qt.allow-classfiles-as-resource` to allow Java class files top be found by Qt resource system.
* New option `io.qt.allow-meta-inf-as-resource` to allow a jar file's META-INF folder and content be used as Qt resource.
* New option `io.qt.resource-cache-timeout` to specify how long jar files shall be cached in resource system.

This will be the last release with Maven-published binaries for Qt 6.9.

## What's new in QtJambi 6.9.2

* 6.8.7 linux libraries are based on GLIBC 2.29 again
* Ready for Qt 6.10

## What's new in QtJambi 6.9.1

* End of support for Java 8.
* End of support for Qt 5.
* Compatible with Java 24.

### Bugfixes

Solved issue:
* [Issue 223](../../../issues/223): ant javadocs fails

## What's new in QtJambi 6.9.0

* Compatible to Java 24.

## What's new in QtJambi 6.8.3

### Bugfixes

Solved issue:
* [Issue 221](../../../issues/221): [BUG] 6.8.2 Memory leak on MacOS Sequoia ARM
* [Issue 219](../../../issues/219): [BUG] Ant compilation error
* deadlock during object deletion with enabled cleanup logs
* crash during thread cleanup
* QDanglingPointerException thrown unexpectedly during window deletion
* thread affinity check during QApplication initialization

### New Feature

* On Windows, QtJambi can now be compiled with MinGW and LLVM-MinGW. Native bundles are named `native-windows-mingw-x64` and `native-windows-llvm-mingw-x64`, respectively.
* Added API method: `QSpan::chopped`, `QWaylandApplication`
* ready for Qt 6.9

## What's new in QtJambi 6.8.2

### Bugfixes

Solved issue:
* bugfix: QFutureInterface does not run continuations

### New Feature
* QtJambi resource system got a performance boost.

### Removed Deprecated API

* `QStack` no longer implements Deque due to an incompatibility with JDK 21 and higher.
* `QtResources` - Use `QResource` instead.
* `@QmlImportMajorVersion` - Use `@QmlImport` instead.
* `QRemoteObjectPendingReply(Class)`
* `QRemoteObjectPendingReply(Class,QRemoteObjectPendingCall)`
* `QScopedPointer.cleanup(Object,Consumer)` - Use `cleanup(Consumer,Object)` instead.
* `QtUtilities.jambiTempDir()` - Use `jambiDeploymentDir()` instead.

## What's new in QtJambi 6.8.1

### Bugfixes

Solved issue:
* [Issue 211](../../../issues/211): Building QtJambi 6.8.0 from sources fails on Kubuntu 24.10
* [Issue 214](../../../issues/214): QtJambi 6.8.1 RC1 does not build from sources against Qt 6.8.1
* Fixed: Crashes in QML due to forwarded Java exceptions
* Fixed: Crash on macOS when using application-wide event filter and QtWebEngine

### New Feature

* New JVM option `io.qt.no-exception-forwarding-from-meta-calls` to avoid `QMetaObject` calls (like method invocation and property access) forwarding exceptions to the Qt caller in any case (it is avoided by default in case of Qml-related QObjects).
* `QtUtilities::setNoExceptionForwardingFromMetaCallsEnabled(boolean)` enable/disable avoiding exception forwarding from meta calls.
* New JVM option `io.qt.no-exception-forwarding-from-virtual-calls` to avoid virtual calls (i.e. Java overrides) forwarding exceptions to the Qt caller in any case.
* `QtUtilities::setNoExceptionForwardingFromVirtualCallsEnabled(boolean)` enable/disable avoiding exception forwarding from virtual calls.
* New JVM option `io.qt.enable-signal-emit-thread-check` to activate thread checks on signal emits. If a signal is emitted from a foreign thread a warning is given. Install custom handler with `QtUtilities.installSignalEmitThreadCheckHandler` to change this behavior.
* `QtUtilities::setSignalEmitThreadCheckEnabled(boolean)` enable/disable thread checks at signal emit.
* `QtUtilities::installSignalEmitThreadCheckHandler(BiConsumer<QObject,AbstractSignal>)` Install a handler for signal emit thread checks to be is executed every time an event is emitted from a foreign thread.
* `QObject::connect`, `Signal::connect`, `QTimer::singleShot` as well as `QMetaObject::invokeMethod` now accept class method handles, e.g. `QMetaObject::invokeMethod(widget, QWidget::setVisible, true);`
* Performance improvements on `QMetaObject::invokeMethod`.
* More flexibility for selective event filters: class type filtering, object name filtering (string comparison or regular expression match). Moved from `QtUtilities` to `QCoreApplication`.
* `QAbstractFileEngineHandler` can be created from file name tests (string comparison or regular expression match). This avoids Java calls for every single file operation.
* QtJambi is now shipped with native binaries for Linux arm64 (6.8 and higher).
* Added API method: `QRandomGenerator::bounded`, `QAbstractHttpServer::addWebSocketUpgradeVerifier`
* Added API types: `QSemaphoreReleaser`, `QReadLocker`, `QWriteLocker` and `QMutexLocker` to be used in try-with-resources

### Deprecated

* `QtUtilities.asSelectiveEventFilter(eventFilter, eventType...)` now deprecated. Use `QCoreApplication.asSelectiveEventFilter` instead.

QtJambi 6.8.1 is not binary-compatible to Qt 6.8.0.  Please use Qt 6.8.1 or higher.

## What's new in QtJambi 6.8.0

QtJambi 6.8.0 is equivalent to QtJambi 6.7.3.

## What's new in QtJambi 6.7.3

### Bugfixes

Solved issue:
* [Issue 203](../../../issues/203): qtjambi-native-linux-x64 5.15 crash on ubuntu 24.04
* [Issue 206](../../../issues/206): Nullability of arguments and return values - `QUndoGroup::addStack` does no longer accept `null`
* [Issue 208](../../../issues/208): nothing happened after click executable exe when I follow the guide in How-to-deploy-QtJambi-applications - improved documentation
* Fixed: Deadlock in QML engine when using qml-registered type
* Fixed: Crash during JVM shutdown on macOS
* Fixed: Java-adopted Thread object for native QThread remaining alife after thread end

### New Feature

* New JVM option `io.qt.enable-event-logs` to activate logging of all handled events with their receivers.
* `QtUtilities::setDanglingPointerCheckEnabled(boolean)` enable/disable dangling pointer checks at runtime
* `QtUtilities::setMethodLogsEnabled(boolean)` enable/disable method logging at runtime
* `QtUtilities::setEventLogsEnabled(boolean)` enable/disable event logging at runtime
* Number of JNI global references per object has been reduced
* Java representatives of Qt containers (`QList`, `QMap`, `QHash`, `QSet`) have been improved
* `QSpan` now available as Java type
* Signals now able to treat single shot connections

QtJambi 6.7.3 is not binary-compatible to Qt 6.7.0. Please use Qt 6.7.1 or higher.
QtJambi 6.8.0 is not binary-compatible to Qt 6.8.1.

## What's new in QtJambi 6.7.2

#### Bugfixes

Solved issue:
* Exception when emitting `QAbstractItemModel.dataChanged` with default arguments
* Auto-detected invokable method in subclass shadowing superclass property
* Signal connections to lambda expression fail on queued emit
* Type registration of `QMetaType(QAbstractItemModel\*)` incomplete when implicitly done during subclass registration
* Running on Android again

#### New Feature

* New nullness support in `QVariant`:
	* `QVariant.fromValue()` with argument `null` now converts to `QVariant(Nullptr)` (`QVariant.setValue()` similar)
	* `QVariant.NULL` is an alias object represents `QVariant(Nullptr)`
	* `QVariant.nullable(Object)` returns the given object if not `null` and `QVariant.NULL` otherwise
	* `QVariant.typedNullable<T>(Object,Class,QMetaType...)` returns the given object if not `null` and a type-specific null alias object otherwise
* New experimental features to be enabled by JVM argument `-D<prop>=true` or via `System.setProperty("<prop>", "true")`:
	* `io.qt.experimental.fast-jni` enables improved gathering of JNI environment during native calls
	* `io.qt.experimental.fast-jni-for-overrides` enables improved gathering of JNI environment during native virtual calls
	* Following experimental features deal with the improvement of item model performance:
		* `io.qt.experimental.item-model-cached-virtuals` item model's `rowCount()` and `columnCount()` return values are cached to improve performance of item views.
		* `io.qt.experimental.enable-invalid-modelindex-singleton` invalid `QModelIndex` is represented by one single Java object.
		* `io.qt.experimental.enable-ephemeral-modelindexes` instead of calling virtual Java methods with a copy of a QModelIndex argument it submits an alias which is invalidated after use.

#### Further Changes

* QPropertyAlias is deprecated

QtJambi 6.7.2 is not binary-compatible to Qt 6.7.0. Please use Qt 6.7.1 or higher.

## What's new in QtJambi 6.7.1

#### Bugfixes

Solved issue:
* switch over extensible enum
* using `QObject::eventFilter` on dying objects
* bugfix related to missing RTTI in libraries on macOS
* [Issue 197](../../../issues/197): Meta object does not take public field if getter and setter are available
* [Issue 199](../../../issues/199): `QTextCursor` is doing a dangling pointer check on `QTextDocument` before GCed to avoid crashes
* QML: Initializing lightweight Java types with QML null without crash
* `QObject::destructed` signal now sending proxy instance for GCed objects

#### New Feature

* You can now declare a signal to be lightweight by using `@QtUninvokable` annotation. Such a signal is not implemented by Qt's meta object system but purely in Java. 
* `QtUtilities.asSelectiveEventFilter(eventFilter, eventType...)` now allows you to produce an event filter only for selected event types. This saves native-to-java conversion for irrelevant cases.
* support of `std::unique_ptr` in Qt API
* Class `QChar` giving static access to Qt's character API
* JVM argument `io.qt.enable-cleanup-logs=true` allows you to print all cleanup operations after Java objects are GCed ([Issue 199](../../../issues/199))

#### Further Changes

* Performance improvements for thread-affine deletions (e.g. GC deleting a `QPersistentModelIndex`)
* Adding `@since` information to Java Docs.
* Extra signals of internal QObject's without API representation now support cloned signals (signals with default arguments).

QtJambi 6.7.1 is not binary-compatible to Qt 6.7.0. Please use Qt 6.7.1 or higher.

## What's new in QtJambi 6.7.0

#### Bugfixes

Solved issue:
* Bugfix in detecting unreadable meta properties
* Bugfix in handling lightweight Java types in Qt containers
* Bugfix in deployer
* `QObject.findChild` now having the correct return type
* Meta object auto-detection now skips methods if property of same name is also available. This led to access errors in QML/JavaScript.
* Improved the API of several `QDBusArgument` extraction methods.

#### Further Changes

* Improved error message when trying to use QtJambi with mismatching Qt build
* Improvement of `QMetaObject.invokeMethod` implementation.
* Providing `QMetaObject.writeProperty` and readProperty doing thread-affine access to `QProperty`.
* Providing `QMetaProperty.fromProperty` giving the `QMetaProperty` of a `QProperty`.
* Generator is now able to map c++ array types and `std::array`.
* Generator is now able to treat int-based template parameters.
* Added `QIconEngine.virtual_hook`

This will be the last release with Maven-published binaries for Qt 5.15 and Qt 6.6.

## What's new in QtJambi 6.6.2

#### Bugfixes

Solved issue:
* [Issue 185](../../../issues/185): QtJambi build from scratch on windows has missing opengl classes
* Java-written `QObject`s wrapped as native `QVariant` can be now used in Qml/JavaScript.

#### Further Changes

* QtJambi's build system is now able to run on Windows arm64 and FreeBSD.
* Java-based QML plugins now survive a reset by `qmlClearTypeRegistrations()` without crash
* QVariant-wrapped QObject pointers can now be used in QML/JavaScript even when they are Java-owned
* binary compatibility to Qt 6.6.2
* ready for Qt 6.7

## What's new in QtJambi 6.6.1

#### Bugfixes

Solved issue:
* [Issue 168](../../../issues/168): QtJambi does not build from sources using JDK 21 - further improvements

Blindfix for issue:
* [Issue 184](../../../issues/184): QtJambi 6.6.0 seems to not update cache of Java resources accessible via Qt?

#### Further Changes

* `std::chrono` types now map to `java.time` classes.
* New class `QScope` to manage resources in a try-with-resources block.
* QtJambi API changes its type mapping of data pointers from arrays to NIO buffers. This is especially relevant for virtual functions like `QIODevice::readData`, `QIODevice::writeData` and similar methods. They are now buffer-based. The former array-based API is available as final delegate functions. Subclasses of  `QIODevice`, `QAbstractFileEngine` and `QPicture` implementing read/write need to be changed to using buffers instead.
* The native pointer corresponds to a NIO buffer's position.
* Java arrays can be specified with offset and length parameter to be translated to native pointer and length.
* QtJambi API now provides function overloads for all possibles calls according to implicit constructors of argument types. For legacy code, this may lead to ambiguous situations when using `null` as argument.
* Automatic download of Android SDK/NDK during the build process
* Made some array arguments of `QPaintEngine` and `QPainter`'s draw functions vararg.

## What's new in QtJambi 6.6.0

#### Bugfixes

Solved issues:
* [Issue 168](../../../issues/168): QtJambi does not build from sources using JDK 21

#### Further Changes

* Improved Qt container access

This version of QtJambi is source and binary-compatible with its predecessor versions.

## What's new in QtJambi 6.5.3

#### Bugfixes

Solved issues:
* [Issue 166](../../../issues/166): QByteArrayView(byte[]) not working correctly

#### Further Changes

* QtJambi now includes `QStringDecoder` and `QStringEncoder`
* `QCollator` now derives `Comparator<String>` and provides comparison of char arrays.
* New QtGraphs API available as QtJambi module
* QtEndian now available as QtJambi class
* Debug info is now available for Windows, Linux and macOS
* QNetworkAccessManager is now thread-affine

This version of QtJambi is source compatible with its predecessor versions.

## What's new in QtJambi 6.5.2

#### Bugfixes

Solved issues:
* [Issue 163](../../../issues/163): QModelIndex implementation of 5.15.15 slower than 5.15.11
* [Issue 160](../../../issues/160): QMetaType.metaObject() returns null although instance expected
* [Issue 159](../../../issues/159): Custom subclasses of QPaintEngine crash when using QPaintEngineState

#### Further Changes

* Re-parenting of Qt container types
* Improved java comments
* `deployment` directory now only contains Java modules. Source bundles are located in subdirectory `sources`, docs are located in subdirectories `javadocs` and native bundles are loacted in subdirectory `native`.

This version of QtJambi is source but not binary-compatible ti its predecessor versions.

## What's new in QtJambi 6.5.1

#### Bugfixes

Solved issues:
* [Issue 148](../../../issues/148): QGuiApplication.overrideCursor() is crashing

#### New Features

Solved issues:
* [Issue 147](../../../issues/147): [uic] Add an option to *not* generate the "MyForm" container class
* [Issue 149](../../../issues/149): Improve QTJAMBI_DEBUG_TRACE feature
* [Issue 150](../../../issues/150): Use RTTI to identify dangling pointers
* [Issue 151](../../../issues/151): Make diagnostics features disabled by default
* [Issue 152](../../../issues/152): Configurable log levels
* [Issue 153](../../../issues/153): Make concurrent modification detection on Qt containers optional
* [Issue 158](../../../issues/158): Make QPlatformIntegration available


## What's new in QtJambi 6.5.0

#### Bugfixes

Solved issues:
* [Issue 143](../../../issues/143): QObject.connect methods are non-static with 6.4.4

#### New Features

* `QJSEngine::CppOwnership` is now renamed to `JavaOwnership` to avoid missunderstanding. When working with Qml, QObjects are either managed by the Java programmer or by the Qml engine.
* Further improvements of QtJambi generator.

## What's new in QtJambi 6.4.4 / 6.2.12 / 5.15.14

#### Bugfixes

Solved issues:
* [Issue 129](../../../issues/129): UIC to Kotlin errors and exceptions
* [Issue 135](../../../issues/135): Decreasing UI performance of QListView
* [Issue 125](../../../issues/125): Slow/Unresponsive UI starting Jambi 6.4.2

#### New Features

* QtJambi API now gives information about arguments' nullability.
* Further improvements of QtJambi generator.

#### Compatibility

QtJambi 6.4.4, 6.2.12 and 5.15.14 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.4.3 / 6.2.11 / 5.15.13

#### Bugfixes

Solved issues:
* [Issue 125](../../../issues/125): Slow/Unresponsive UI starting Jambi 6.4.2
* [Issue 118](../../../issues/118): Support for calling special squish functions from java

#### New Features

* First attempts in making QtJambi compatible to GraalVM ([see here](../../../discussions/105)).

#### Compatibility

QtJambi 6.4.3, 6.2.11 and 5.15.13 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.4.2 / 6.2.10 / 5.15.12

#### Bugfixes

Solved issues:
* [Issue 103](../../../issues/103): QList can‘t be sorted
* [Issue 118](../../../issues/118): Support for calling special squish functions from java
* [Issue 121](../../../issues/121): Class Qt3DWindow can‘t be new
* Issue with `QMap` or `QHash` of java types fixed

#### New Features

* Completely refactored library kernel
* 99% of third-party legacy code removed
* `qHash(T)` now available in `QtGlobal` namespace
* `qobject_cast<T>(QObject)` now available as static method in `QObject`
* `QModelIndex` is now a Java wrapper for the native type. Thus, it no longer converts to null but to invalid `QModelIndex` instance
* Making QtJambi Generator available
* Making QtJambi header files available

#### Compatibility

QtJambi 6.4.2, 6.2.10 and 5.15.12 are source-compatible but not binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.4.1 / 6.2.9 / 5.15.11

#### Bugfixes

Solved issues:
* [Issue 94](../../../issues/94): A lot of new logs in the code that seems to have no obvious errors
* [Issue 93](../../../issues/93): Final class fields are not marked as constant when imported as qobject properties
* [Issue 92](../../../issues/92): Meta Object loading system doesn't use custom classloaders - can't find classes from downloaded jars
* [Issue 98](../../../issues/98): QObect meta type parsing broken?
* removing memory leak in QQmlListProperty. QQmlListProperty now requires QList instead of java list. Alternatively, you can specify access functions similar to native Qt.
* Resolved: Stack overflow caused by cascades of registering meta types and generating meta objects.
* Resolved: NullPointerException in combination with classpath resources.

#### New Features

* You can now use annotations from `io.qt.qml.util` to define Qml types. Then, use `QtQml.qmlRegisterTypesAndRevisions()` to register these types to Qml.
* Ability to define value types for Qml in Qt 6.4.
* Ability to register metatype converters.
* Ability to use the same signal as notifier for different properties.
* Non-public methods with non-void return type are now considered not to be invokable.

#### Compatibility

QtJambi 6.4.1, 6.2.9 and 5.15.11 are source-compatible and binary-compatible to its predecessor releases (except `QQmlListProperty` constructor). 

#### Remark

This is the last regular release for Qt 5.15.

## What's new in QtJambi 6.4.0 / 6.3.5 / 6.2.8 / 5.15.10

#### Bugfixes

Solved issues:
* `qrc:` paths not working for directory classpaths
* [Issue 89](../../../issues/89): Functions missing from QQuickRenderTarget
* [Issue 86](../../../issues/86): Add all java object constants as readonly properties when used in qml
* [Issue 85](../../../issues/85): Fix ‘numeric_limits’ is not a member of ‘std’ error
* [Issue 84](../../../issues/84): Could not load the Qt platform plugin "wayland" in "" even though it was found.
* [Issue 83](../../../issues/83): QUrl with "qrc:" not working for resources in directory-based class paths.
* [Issue 79](../../../issues/79): Wrong minial version for QWebEngineFileSystemAccessRequest?
* [Issue 78](../../../issues/78): better description of macos startup in readme
* [Issue 77](../../../issues/77): Using "Object" in a slot
* [Issue 75](../../../issues/75): QProperty and notification signals

#### New Features

* New modules QtHttpServer and QtSpatialAudio
* New classes QtMath, QtAlgorithms and QtGlobal making available many Qt functions in global scope
* Providing QVariant as value class.
* Removing `$` character from class names translated to native.
* All Java classes lead to distinct values in `QMetaType.fromType()`.
* All Java classes can be gadgets ready for meta-programming and qml.
* Improved "reduce" features in QtConcurrent

#### Compatibility

QtJambi 6.3.5, 6.2.8 and 5.15.10 are source-compatible and in most cases binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.3.4 / 6.2.7 / 5.15.9

#### Bugfixes

Solved issues:
* [Issue 73](../../../issues/73): Add support for QQuickWebEngineView
* [Issue 72](../../../issues/72): Web page runJavascript always crashes jvm when you try to use the returned object
* [Issue 71](../../../issues/71): Java DeferredDelete throws thread affinity exception, one of threads is undefined
* Solved bug in converting QVariantMap

#### New Features

* Added QQmlEngine template method: `singletonInstance`
* Added QJSEngine template methods: `fromScriptValue`, `toScriptValue`, `fromManagedValue`, `toManagedValue`
* Added functions to QtQml: `qmlTypeId`, `qjsEngine`
* Added functions to QtWebEngineQuick: `toWebEngineSettings`, `toWebEngineScriptCollection`
* Qt 6.4 ready.

#### Compatibility

QtJambi 6.3.4, 6.2.7 and 5.15.9 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.3.3 / 6.2.6 / 5.15.8

#### Bugfixes

Solved issues:
* [Issue 69](../../../issues/69): Signal connection does not work as expected with generics
* [Issue 66](../../../issues/66): raiseQThreadAffinityException cores java instead of throwing an exception (blindfixed)
* [Issue 64](../../../issues/64): QtJambi build failed
* [Issue 63](../../../issues/63): Program cannot be started
* [Issue 62](../../../issues/62): symbol lookup error
* [Issue 60](../../../issues/60): Deployment Icon not working in windows (feature removed)

#### New Features

* Class `io.qt.core.QString` now makes all data conversion and formatting features available in Java.
* Qt 6.4 ready.

#### Compatibility

QtJambi 6.3.3, 6.2.6 and 5.15.8 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.3.2 / 6.2.5 / 5.15.7

#### Bugfixes

Solved issues:
* [Issue 56](../../../issues/56): QtJambi UIC produces wrong code for Form Layout
* [Issue 55](../../../issues/55): Some methods of the QOpenGLFunctions_3_2_Core class do not work as expected

#### New Features

* Android support!!! [See here how to create Android apps with QtJambi.](Android.md)
* Massive size reduction of `QtJambi` and `QtJambiCore` library
* QtJambi Deployer is now able to create self-extracting [Qt library bundles](How-to-bundle-Qt-libraries.md).

#### Compatibility

QtJambi 6.3.2, 6.2.5 and 5.15.7 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.3.1 / 6.2.4 / 5.15.6

#### Bugfixes

Solved issues:
* [Issue 51](../../../issues/51): UIC: tabwidget conversion problem
* [Issue 48](../../../issues/48): iconfont does not work as expected
* [Issue 27](../../../issues/27): Building QtJambi on windows using mingw
* [Issue 52](../../../issues/52): The qtjambi-deployer module does not work with jre8
* [Issue 53](../../../issues/53): Child thread executes custom signal and crashes when updating interface content

#### New Features

* Comining qtjambi* and qtjambi*-jre8 modules.
* Subsuming classpath resources under Qt resources identified with colon path `":path/to/resource"`. You can continue to use `"classpath:path/to/resource"` but it is subject to be removed in the future. Classpath resources as `QUrl` is specified by `qrc` scheme: `new QUrl("qrc:path/to/resource")`.
* Allowing to combine QtJambi 6.3.1 with QtJambi modules of different version 6.3.x
* Adding operator functions for QPoint, QSize, QCborArray, QJsonArray, QDeadlineTimer, QPainterPath and QJSPrimitiveValue to be used in Kotlin.
* Removing outdated source code of examples and utilities
* Improved exception handling in QThreads

#### Compatibility

QtJambi 6.3.1, 6.2.4 and 5.15.6 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.3.0 / 6.2.3 / 5.15.5

#### Bugfixes

Solved issues:
* [Issue 15](../../../issues/15): Crash during shutdown on some linux systems is solved now (verified).
* [Issue 18](../../../issues/18): FileAlreadyExistsException fixed

#### New Features

* Module **QtQuickTest** now available
* Module **ActiveQt** now available
* Module **QtPDF** now available for Qt 6.3
* Module **QtUiTools** now also available for QtJambi 5
* `io.qt.QtResources` is now deprecated. Its functions are moved to `io.qt.core.QResource`. 
* `QHashSeed` is now available
* `QList.of(T...)` now returns `QStringList` if `T` is `String`.
* Adding missing functions:
    * `QUuid::fromString`
    * `QStringConverter::encodingForHtml`
    * `QStringConverter::encodingForData`
    * `QBluetoothUuid::toUInt16`
    * `QBluetoothUuid::toUInt32`
    * `endl` on `QTextStream`
    * `QStack::top`
* QtJambi now gives information when trying to combine MinGW and MSVC compiled libraries.
* Tool **UIC** for QtJambi ([Issue 17](../../../issues/17))
* Deployer now generates source code for compiling plugin library

#### Compatibility

QtJambi 6.2.3 and 5.15.5 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.2.2 / 5.15.4

#### Bugfixes

Solved issues:
* [Issue 15](../../../issues/15): Crash during shutdown on some linux systems should be solved (blind fix).
* [Issue 16](../../../issues/16): All possible race conditions in `QMetaObject` solved. 
* [Issue 18](../../../issues/18): QtJambi stores library files temporarily or persistently by choice. Therefore, use JVM arguments `io.qt.keep-temp-deployment` or `io.qt.deploymentdir`.
* [Issue 19](../../../issues/19): `QDBusReply<T>` and `QDBusPendingReply<T>` now work as expected.
* [Issue 23](../../../issues/23): QPainter now also works fine in `QOpenGLWidget::paintGL()`.
* [Issue 25](../../../issues/25): invalid `QVariant` is now coded by Java `null`. If you want to specify null pointer variant you need to call `QVariant.fromValue(null)`.
* [Issue 27](../../../issues/27): Reduced size of single compilation units to support MinGW.
* [Issue 30](../../../issues/30): QML debugging is now possible by starting an application with Java runtime flag `-Dio.qt.enabled-qml-debugging=true`.
* Signal connection to synthetic java method no longer throws an exception.


#### Compatibility

QtJambi 6.2.2 and 5.15.4 are source- and binary-compatible to its predecessor releases. 

## What's new in QtJambi 6.2.1 / 5.15.3

#Versioning

Since there are multiple releases of QtJambi for the same Qt version and since QtJambi is now available on Maven, 
it is necessary to use more specific version numbers for different QtJambi releases.

From now on, QtJambi builds have the major and minor version of their assigned Qt versions (e.g. 6.2) 
and an increasing patch version counting the number of releases.
Since QtJambi for Qt 5.15 was published three times up to now, the current forth QtJambi release is 5.15.3. QtJambi for Qt6.2 is release number 6.2.1. 
The first QtJambi for Qt 6.3 will be versioned 6.3.0.

QtJambi 5.15.3 works with any patch version of Qt 5.15 and, accordingly, QtJambi 6.2.1 works with any patch version of Qt 6.2.

## Support for Platform Integration Native Interfaces on Qt6

`QCoreApplication`, `QWindow` and other components now give access to their platform integration on macOS, Windows and X11. Example:

``` java
QWindow window = ...
QWindowsWindow windowsWindow = window.nativeInterface(QWindowsWindow.class);
QCocoaWindow macWindow = window.nativeInterface(QCocoaWindow.class);
QXcbWindow x11Window = window.nativeInterface(QXcbWindow.class);
```

## Overloaded Signals

Implementing overloaded signals is simplyfied and can now be done even outside of QObjects. Therefore, subclass `QObject.MultiSignal`, 
`QInstanceMemberSignals.MultiSignal` or `QStaticMemberSignals.MultiSignal` and add custom overloaded emit methods. 
The underlying signal structure is generated automatically.

Beside this, you can connect to overloaded signals without specifying its parameters by `overload(Class)`.
The intended parameterized signal is determined in accordance with the slot's parameters. Example:

``` java
QCompleter completer = ...
completer.activated.connect(receiver, "onActivatedIndex(QModelIndex)");
completer.activated.connect(receiver, "onActivatedText(String)");
completer.activated.connect(receiver::onActivatedIndex);
completer.activated.connect(receiver::onActivatedText);
```

## Performance Improvements

Signal initialization is improved bringing better performance for object initialization.

## Bugfixes

A lot!

## Compatibility

QtJambi 6.2.1 and 5.15.3 are source-compatible but maybe not be binary-compatible to its predecessor releases. 
You might need to compile your code with the new QtJambi version again.