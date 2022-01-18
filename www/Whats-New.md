# What's new in QtJambi 6.2.2 / 5.15.4

## Bugfixes

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


## Compatibility

QtJambi 6.2.2 and 5.15.4 are source- and binary-compatible to its predecessor releases. 
