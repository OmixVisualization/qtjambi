QTJAMBILIB = QtJambiQml
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

HEADERS += qqmllistproperty.h \
    qmlcreateparentfunction.h \
    qmlattachedpropertiesfunction.h \
    qmlcreatorfunction.h \
    qtjambi_jarimport.h \
    jarimport.h \
    qtjambi_qml_hashes.h \
    qtjambi_qml_repository.h
SOURCES += qmlregistry.cpp qqmllistproperty.cpp \
    qmlcreatorfunction.cpp \
    qmlcreateparentfunction.cpp \
    qmlattachedpropertiesfunction.cpp \
    jarimport.cpp \
    qtjambi_qml_repository.cpp

DEFINES += QTJAMBI_QML_EXPORT

QT += core gui qml core-private qml-private

msvc:QMAKE_CXXFLAGS += /bigobj

