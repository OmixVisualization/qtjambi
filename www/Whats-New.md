# What's new in QtJambi 6.3.0 / 6.2.3 / 5.15.5

## Bugfixes

Solved issues:
* [Issue 15](../../../issues/15): Crash during shutdown on some linux systems is solved now (verified).
* [Issue 18](../../../issues/18): FileAlreadyExistsException fixed

## New Features

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

## Compatibility

QtJambi 6.2.3 and 5.15.5 are source- and binary-compatible to its predecessor releases. 
