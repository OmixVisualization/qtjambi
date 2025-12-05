include(../QtJambi/configure.pri)

QT = core network

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    impl.cpp

!ios:{
    PRECOMPILED_HEADER = pch_p.h
    CONFIG += precompile_header
}else{
    HEADERS += pch_p.h
}
