include(../QtJambi/configure.pri)

QT = core network httpserver core-private httpserver-private
CONFIG+=no_private_qt_headers_warning

SOURCES += \
    impl.cpp
