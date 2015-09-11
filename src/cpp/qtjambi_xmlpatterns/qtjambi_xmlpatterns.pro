TARGET = org_qtjambi_qt_xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_xmlpatterns/org_qtjambi_qt_xmlpatterns.pri)

# libQtXmlPatterns.so.4.7.4 is only dependant on libQtCore.so.4 libQtNetwork.so.4 (ensures removal of 'Qt -= gui')
QT = core network xmlpatterns
