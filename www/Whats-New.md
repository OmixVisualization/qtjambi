# What's new in QtJambi 6.7.0

## Bugfixes

Solved issue:
* Bugfix in detecting unreadable meta properties
* Bugfix in handling lightweight Java types in Qt containers
* Bugfix in deployer
* `QObject.findChild` now having the correct return type
* Meta object auto-detection now skips methods if property of same name is also available. This led to access errors in QML/JavaScript.
* Improved the API of several `QDBusArgument` extraction methods.

## Further Changes

* Improved error message when trying to use QtJambi with mismatching Qt build
* Improvement of `QMetaObject.invokeMethod` implementation.
* Providing `QMetaObject.writeProperty` and readProperty doing thread-affine access to `QProperty`.
* Providing `QMetaProperty.fromProperty` giving the `QMetaProperty` of a `QProperty`.
* Generator is now able to map c++ array types and `std::array`.
* Generator is now able to treat int-based template parameters.
* Added `QIconEngine.virtual_hook`

This will be the last release with Maven-published binaries for Qt 5.15 and Qt 6.6.