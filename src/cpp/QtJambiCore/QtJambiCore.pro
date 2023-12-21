include(../QtJambi/configure.pri)

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES +=\
    QHash_shell.cpp \
    QIterator_shell.cpp \
    QSet_shell.cpp \
    QMap_shell.cpp \
    QList_shell.cpp \
    future.cpp \
    plugins.cpp \
    impl.cpp \
    utils.cpp

lessThan(QT_MAJOR_VERSION, 6):{
    SOURCES += \
    QVector_shell.cpp \
    QLinkedList_shell.cpp
}
QT = core core-private
CONFIG+=no_private_qt_headers_warning

!ios:{
    PRECOMPILED_HEADER = \
    pch_p.h
    CONFIG += precompile_header
}

win32-arm64-msvc* | win32-msvc*: {
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}
