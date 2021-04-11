QTJAMBILIB = QtJambiCore
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

HEADERS += qtjambiqfuture.h \
    qtjambi_biginteger.h \
    qtjambi_core_qhashes.h \
    qtjambi_core_repository.h

SOURCES +=\
    QHash_shell.cpp \
    QIterator_shell.cpp \
    QVector_shell.cpp \
    QSet_shell.cpp \
    QMap_shell.cpp \
    QLinkedList_shell.cpp \
    QList_shell.cpp \
    containers.cpp \
    qtjambi_biginteger.cpp \
    qtjambi_core_repository.cpp \
    qtjambi_core_util.cpp \
    qtjambi_core_variant.cpp \
    qtjambi_containers_hash2.cpp \
    qtjambi_containers_hash3.cpp \
    qtjambi_containers_hash4.cpp \
    qtjambi_containers_hash5.cpp \
    qtjambi_containers_hash6.cpp \
    qtjambi_containers_map2.cpp \
    qtjambi_containers_map3.cpp \
    qtjambi_containers_map4.cpp \
    qtjambi_containers_map5.cpp \
    qtjambi_containers_map6.cpp \
    qtjambi_containers_multihash2.cpp \
    qtjambi_containers_multihash3.cpp \
    qtjambi_containers_multihash4.cpp \
    qtjambi_containers_multihash5.cpp \
    qtjambi_containers_multihash6.cpp \
    qtjambi_containers_multimap2.cpp \
    qtjambi_containers_multimap3.cpp \
    qtjambi_containers_multimap4.cpp \
    qtjambi_containers_multimap5.cpp \
    qtjambi_containers_multimap6.cpp

macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtCore.framework/Headers/$$QT_VERSION/QtCore/private
}
INCLUDEPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION/QtCore/private

QT = core core-private

msvc:QMAKE_CXXFLAGS += /bigobj
win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}
