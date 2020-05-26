QTJAMBILIB = QtJambiDBus
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

# libQtDBus.so.4.7.4 is only dependant on libQtCore.so.4 libQtXml.so.4 (ensures removal of 'Qt -= gui')
QT = core dbus

HEADERS += \
    QDBusVariantReply.h
SOURCES += \
    QDBusVariantReply.cpp
