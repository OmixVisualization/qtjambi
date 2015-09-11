TARGET = org_qtjambi_qt_concurrent

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_concurrent/org_qtjambi_qt_concurrent.pri)

HEADERS += qtjambiconcurrent_p.h
SOURCES += qtjambiconcurrent.cpp

QT = core concurrent
