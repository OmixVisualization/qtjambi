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

win32-g++* {
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}
