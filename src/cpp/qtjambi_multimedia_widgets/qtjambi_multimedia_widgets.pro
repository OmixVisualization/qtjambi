QTJAMBILIB = QtJambiMultimediaWidgets
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

# libQtMultimedia.so.4.7.4 is only dependant on libQtCore.so.4 libQtGui.so.4
QT = core gui widgets multimedia multimediawidgets

QMAKE_LIBS_WMF =
