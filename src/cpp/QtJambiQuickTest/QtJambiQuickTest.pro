include(../QtJambi/configure.pri)

QT += core core-private qmltest quick
CONFIG+=no_private_qt_headers_warning

SOURCES += \
    impl.cpp
