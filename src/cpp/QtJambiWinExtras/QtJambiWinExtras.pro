include(../QtJambi/configure.pri)

QT = core gui widgets
win32-g++* | win32-msvc*: {
    QT += winextras

    HEADERS += \
        utils_p.h

    SOURCES += \
        impl.cpp
}
