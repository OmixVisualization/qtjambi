# What's new in QtJambi 6.6.2

## Bugfixes

Solved issue:
* [Issue 185](../../../issues/185): QtJambi build from scratch on windows has missing opengl classes
* Java-written `QObject`s wrapped as native `QVariant` can be now used in Qml/JavaScript.

## Further Changes

* QtJambi's build system is now able to run on Windows arm64 and FreeBSD.
* Java-based QML plugins now survive a reset by `qmlClearTypeRegistrations()` without crash
* QVariant-wrapped QObject pointers can now be used in QML/JavaScript even when they are Java-owned
* binary compatibility to Qt 6.6.2
* ready for Qt 6.7