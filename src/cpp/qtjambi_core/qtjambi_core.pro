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
    qtjambi_core_util.cpp

macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtCore.framework/Headers/$$QT_VERSION/QtCore/private
}
INCLUDEPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION/QtCore/private

QT = core core-private
