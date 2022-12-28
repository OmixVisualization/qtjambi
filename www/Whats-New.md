# What's new in QtJambi 6.4.2 / 6.2.10 / 5.15.12

## Bugfixes

Solved issues:
* [Issue 103](../../../issues/103): QList can‘t be sorted
* [Issue 118](../../../issues/118): Support for calling special squish functions from java
* [Issue 121](../../../issues/121): Class Qt3DWindow can‘t be new
* Issue with `QMap` or `QHash` of java types fixed

## New Features

* Completely refactored library kernel
* 99% of third-party legacy code removed
* `qHash(T)` now available in `QtGlobal` namespace
* `qobject_cast<T>(QObject)` now available as static method in `QObject`
* `QModelIndex` is now a Java wrapper for the native type. Thus, it no longer converts to null but to invalid `QModelIndex` instance
* Making QtJambi Generator available
* Making QtJambi header files available

## Compatibility

QtJambi 6.4.2, 6.2.10 and 5.15.12 are source-compatible but not binary-compatible to its predecessor releases. 

