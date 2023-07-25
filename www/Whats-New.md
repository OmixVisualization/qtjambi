# What's new in QtJambi 6.5.2

## Bugfixes

Solved issues:
* [Issue 163](../../../issues/163): QModelIndex implementation of 5.15.15 slower than 5.15.11
* [Issue 160](../../../issues/160): QMetaType.metaObject() returns null although instance expected
* [Issue 159](../../../issues/159): Custom subclasses of QPaintEngine crash when using QPaintEngineState

## Further Changes

* Re-parenting of Qt container types
* Improved java comments
* `deployment` directory now only contains Java modules. Source bundles are located in subdirectory `sources`, docs are located in subdirectories `javadocs` and native bundles are loacted in subdirectory `native`.

This version of QtJambi is source but not binary compatible ti its predecessor versions.