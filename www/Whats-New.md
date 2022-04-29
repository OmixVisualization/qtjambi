# What's new in QtJambi 6.3.1 / 6.2.4 / 5.15.6

## Bugfixes

Solved issues:
* [Issue 51](../../../issues/51): UIC: tabwidget conversion problem
* [Issue 48](../../../issues/48): iconfont does not work as expected
* [Issue 27](../../../issues/27): Building QtJambi on windows using mingw
* [Issue 52](../../../issues/52): The qtjambi-deployer module does not work with jre8
* [Issue 53](../../../issues/53): Child thread executes custom signal and crashes when updating interface content

## New Features

* Comining qtjambi* and qtjambi*-jre8 modules.
* Subsuming classpath resources under Qt resources identified with colon path `":path/to/resource"`. You can continue to use `"classpath:path/to/resource"` but it is subject to be removed in the future. Classpath resources as `QUrl` is specified by `qrc` scheme: `new QUrl("qrc:path/to/resource")`.
* Allowing to combine QtJambi 6.3.1 with QtJambi modules of different version 6.3.x
* Adding operator functions for QPoint, QSize, QCborArray, QJsonArray, QDeadlineTimer, QPainterPath and QJSPrimitiveValue to be used in Kotlin.
* Removing outdated source code of examples and utilities
* Improved exception handling in QThreads

## Compatibility

QtJambi 6.3.1, 6.2.4 and 5.15.6 are source- and binary-compatible to its predecessor releases. 
