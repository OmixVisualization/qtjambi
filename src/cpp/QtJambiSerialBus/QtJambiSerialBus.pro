include(../QtJambi/configure.pri)

QT = core serialbus

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    impl.cpp

msvc:QMAKE_CXXFLAGS += /bigobj
win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

