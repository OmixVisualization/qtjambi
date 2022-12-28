include(../QtJambi/configure.pri)

SOURCES += \
    impl.cpp

HEADERS += \
    hashes.h \
    utils_p.h

!ios:{
    PRECOMPILED_HEADER = \
    pch_p.h
    CONFIG += precompile_header
}

QT = core core-private gui widgets widgets-private
CONFIG+=no_private_qt_headers_warning
