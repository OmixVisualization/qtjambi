# What's new in QtJambi 6.5.3

## Bugfixes

Solved issues:
* [Issue 166](../../../issues/166): QByteArrayView(byte[]) not working correctly

## Further Changes

* QtJambi now includes `QStringDecoder` and `QStringEncoder`
* `QCollator` now derives `Comparator<String>` and provides comparison of char arrays.
* New QtGraphs API available as QtJambi module
* QtEndian now available as QtJambi class
* Debug info is now available for Windows, Linux and macOS
* QNetworkAccessManager is now thread-affine

This version of QtJambi is source compatible with its predecessor versions.