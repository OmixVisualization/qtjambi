# What's new in QtJambi 6.4.1 / 6.2.9 / 5.15.11

## Bugfixes

Solved issues:
* [Issue 94](../../../issues/94): A lot of new logs in the code that seems to have no obvious errors
* [Issue 93](../../../issues/93): Final class fields are not marked as constant when imported as qobject properties
* [Issue 92](../../../issues/92): Meta Object loading system doesn't use custom classloaders - can't find classes from downloaded jars
* [Issue 98](../../../issues/98): QObect meta type parsing broken?
* removing memory leak in QQmlListProperty. QQmlListProperty now requires QList instead of java list. Alternatively, you can specify access functions similar to native Qt.
* Resolved: Stack overflow caused by cascades of registering meta types and generating meta objects.
* Resolved: NullPointerException in combination with classpath resources.

## New Features

* You can now use annotations from `io.qt.qml.util` to define Qml types. Then, use `QtQml.qmlRegisterTypesAndRevisions()` to register these types to Qml.
* Ability to define value types for Qml in Qt 6.4.
* Ability to register metatype converters.
* Ability to use the same signal as notifier for different properties.
* Non-public methods with non-void return type are now considered not to be invokable.

## Compatibility

QtJambi 6.4.1, 6.2.9 and 5.15.11 are source-compatible and binary-compatible to its predecessor releases (except `QQmlListProperty` constructor). 

## Remark

This is the last regular release for Qt 5.15.