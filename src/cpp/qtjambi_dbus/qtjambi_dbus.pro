TARGET = org_qtjambi_qt_dbus

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt_dbus/org_qtjambi_qt_dbus.pri)

# libQtDBus.so.4.7.4 is only dependant on libQtCore.so.4 libQtXml.so.4 (ensures removal of 'Qt -= gui')
QT = core xml dbus

HEADERS += \
    QtJambiDBusReply.h

