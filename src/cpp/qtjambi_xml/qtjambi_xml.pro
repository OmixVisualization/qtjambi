TARGET = org_qtjambi_qt_xml

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_xml/org_qtjambi_qt_xml.pri)

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = qtjambi_xml_pch.h

# libQtXml.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core xml
