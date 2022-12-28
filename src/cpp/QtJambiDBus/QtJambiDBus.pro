include(../QtJambi/configure.pri)

QT = core dbus dbus-private
CONFIG+=no_private_qt_headers_warning

SOURCES += \
    impl.cpp
