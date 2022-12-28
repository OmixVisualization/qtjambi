include(../QtJambi/configure.pri)

QT += core qml quick webenginecore webenginequick core-private webenginequick-private
CONFIG+=no_private_qt_headers_warning

SOURCES += \
    impl.cpp

