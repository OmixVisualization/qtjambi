# What's new in QtJambi 6.6.1

## Bugfixes

Solved issue:
* [Issue 168](../../../issues/168): QtJambi does not build from sources using JDK 21 - further improvements

Blindfix for issue:
* [Issue 184](../../../issues/184): QtJambi 6.6.0 seems to not update cache of Java resources accessible via Qt?

## Further Changes

* `std::chrono` types now map to `java.time` classes.
* New class `QScope` to manage resources in a try-with-resources block.
* QtJambi API changes its type mapping of data pointers from arrays to NIO buffers. This is especially relevant for virtual functions like `QIODevice::readData`, `QIODevice::writeData` and similar methods. They are now buffer-based. The former array-based API is available as final delegate functions. Subclasses of  `QIODevice`, `QAbstractFileEngine` and `QPicture` implementing read/write need to be changed to using buffers instead.
* The native pointer corresponds to a NIO buffer's position.
* Java arrays can be specified with offset and length parameter to be translated to native pointer and length.
* QtJambi API now provides function overloads for all possibles calls according to implicit constructors of argument types. For legacy code, this may lead to ambiguous situations when using `null` as argument.
* Automatic download of Android SDK/NDK during the build process
* Made some array arguments of `QPaintEngine` and `QPainter`'s draw functions vararg.
