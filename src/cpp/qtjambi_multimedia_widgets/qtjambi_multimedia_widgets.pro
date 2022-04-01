QTJAMBILIB = QtJambiMultimediaWidgets
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

# libQtMultimedia.so.4.7.4 is only dependant on libQtCore.so.4 libQtGui.so.4
QT = core gui widgets multimedia multimediawidgets

QMAKE_LIBS_WMF =
