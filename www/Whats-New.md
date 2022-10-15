# What's new in QtJambi 6.4.0 / 6.3.5 / 6.2.8 / 5.15.10

## Bugfixes

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

## New Features

* New modules QtHttpServer and QtSpatialAudio
* New classes QtMath, QtAlgorithms and QtGlobal making available many Qt functions in global scope
* Providing QVariant as value class.
* Removing `$` character from class names translated to native.
* All Java classes lead to distinct values in `QMetaType.fromType()`.
* All Java classes can be gadgets ready for meta-programming and qml.
* Improved "reduce" features in QtConcurrent

## Compatibility

QtJambi 6.3.5, 6.2.8 and 5.15.10 are source-compatible and in most cases binary-compatible to its predecessor releases. 
