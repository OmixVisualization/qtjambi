QTJAMBILIB = QtJambiCore
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

HEADERS += \
    qtjambi_biginteger.h \
    qtjambi_core_future.h \
    qtjambi_core_qhashes.h \
    qtjambi_core_repository.h \
    qtjambi_stringutil.h

SOURCES +=\
    QHash_shell.cpp \
    QIterator_shell.cpp \
    QVector_shell.cpp \
    QSet_shell.cpp \
    QMap_shell.cpp \
    QLinkedList_shell.cpp \
    QList_shell.cpp \
    qtjambi_biginteger.cpp \
    qtjambi_core_future.cpp \
    qtjambi_core_repository.cpp \
    qtjambi_core_util.cpp \
    qtjambi_core_variant.cpp

QT = core core-private

msvc:QMAKE_CXXFLAGS += /bigobj
win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}
