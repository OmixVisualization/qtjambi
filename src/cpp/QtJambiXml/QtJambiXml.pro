include(../QtJambi/configure.pri)

QT = core xml

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    impl.cpp

!ios:{
    PRECOMPILED_HEADER = \
    pch_p.h
    CONFIG += precompile_header
}
