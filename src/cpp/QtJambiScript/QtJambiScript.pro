include(../QtJambi/configure.pri)

msvc:QMAKE_CXXFLAGS += /bigobj

QT = core script

HEADERS += \
    hashes.h
