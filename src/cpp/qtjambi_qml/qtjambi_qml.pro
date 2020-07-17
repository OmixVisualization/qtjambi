QTJAMBILIB = QtJambiQml
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

HEADERS += qqmllistproperty.h \
    qmlcreateparentfunction.h \
    qmlattachedpropertiesfunction.h \
    qmlcreatorfunction.h \
    qtjambi_qml_repository.h
SOURCES += qmlregistry.cpp qqmllistproperty.cpp \
    qmlcreatorfunction.cpp \
    qmlcreateparentfunction.cpp \
    qmlattachedpropertiesfunction.cpp \
    qtjambi_qml_repository.cpp

QT += core gui qml core-private

msvc:QMAKE_CXXFLAGS += /bigobj

