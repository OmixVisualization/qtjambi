# What's new in QtJambi 6.2.1 / 5.15.3

#Versioning

Since there are multiple releases of QtJambi for the same Qt version and since QtJambi is now available on Maven, 
it is necessary to use more specific version numbers for different QtJambi releases.

From now on, QtJambi builds have the major and minor version of their assigned Qt versions (e.g. 6.2) 
and an increasing patch version counting the number of releases.
Since QtJambi for Qt 5.15 was published three times up to now, the current forth QtJambi release is 5.15.3. QtJambi for Qt6.2 is release number 6.2.1. 
The first QtJambi for Qt 6.3 will be versioned 6.3.0.

QtJambi 5.15.3 works with any patch version of Qt 5.15 and, accordingly, QtJambi 6.2.1 works with any patch version of Qt 6.2.

# Support for Platform Integration Native Interfaces on Qt6

`QCoreApplication`, `QWindow` and other components now give access to their platform integration on macOS, Windows and X11. Example:

``` java
QWindow window = ...
QWindowsWindow windowsWindow = window.nativeInterface(QWindowsWindow.class);
QCocoaWindow macWindow = window.nativeInterface(QCocoaWindow.class);
QXcbWindow x11Window = window.nativeInterface(QXcbWindow.class);
```

# Overloaded Signals

Implementing overloaded signals is simplyfied and can now be done even outside of QObjects. Therefore, subclass `QObject.MultiSignal`, 
`QInstanceMemberSignals.MultiSignal` or `QStaticMemberSignals.MultiSignal` and add custom overloaded emit methods. 
The underlying signal structure is generated automatically.

Beside this, you can connect to overloaded signals without specifying its parameters by `overload(Class)`.
The intended parameterized signal is determined in accordance with the slot's parameters. Example:

``` java
QCompleter completer = ...
completer.activated.connect(receiver, "onActivatedIndex(QModelIndex)");
completer.activated.connect(receiver, "onActivatedText(String)");
completer.activated.connect(receiver::onActivatedIndex);
completer.activated.connect(receiver::onActivatedText);
```

# Performance Improvements

Signal initialization is improved bringing better performance for object initialization.

# Bugfixes

A lot!

# Compatibility

QtJambi 6.2.1 and 5.15.3 are source-compatible but maybe not be binary-compatible to its predecessor releases. 
You might need to compile your code with the new QtJambi version again.
