# What's new in QtJambi 6.7.1

## Bugfixes

Solved issue:
* switch over extensible enum
* using `QObject::eventFilter` on dying objects
* bugfix related to missing RTTI in libraries on macOS
* [Issue 197](../../../issues/197): Meta object does not take public field if getter and setter are available
* [Issue 199](../../../issues/199): `QTextCursor` is doing a dangling pointer check on `QTextDocument` before GCed to avoid crashes
* QML: Initializing lightweight Java types with QML null without crash
* `QObject::destructed` signal now sending proxy instance for GCed objects

## New Feature

* You can now declare a signal to be lightweight by using `@QtUninvokable` annotation. Such a signal is not implemented by Qt's meta object system but purely in Java. 
* `QtUtilities.asSelectiveEventFilter(eventFilter, eventType...)` now allows you to produce an event filter only for selected event types. This saves native-to-java conversion for irrelevant cases.
* support of `std::unique_ptr` in Qt API
* Class `QChar` giving static access to Qt's character API
* JVM argument `io.qt.enable-cleanup-logs=true` allows you to print all cleanup operations after Java objects are GCed ([Issue 199](../../../issues/199))

## Further Changes

* Performance improvements for thread-affine deletions (e.g. GC deleting a `QPersistentModelIndex`)
* Adding `@since` information to Java Docs.
* Extra signals of internal QObject's without API representation now support cloned signals (signals with default arguments).

QtJambi 6.7.1 is not binary compatible to Qt 6.7.0. Please use Qt 6.7.1 or higher.