include(../QtJambi/configure.pri)

QT = core gui location
CONFIG+=no_private_qt_headers_warning

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    impl.cpp
