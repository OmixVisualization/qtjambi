QTJAMBILIB = QtJambiXmlPatterns
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

# libQtXmlPatterns.so.4.7.4 is only dependant on libQtCore.so.4 libQtNetwork.so.4 (ensures removal of 'Qt -= gui')
QT = core network xmlpatterns
