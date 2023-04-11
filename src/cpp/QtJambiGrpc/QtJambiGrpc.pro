include(../QtJambi/configure.pri)

QT = core grpc core-private

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    channel.cpp \
    impl.cpp
