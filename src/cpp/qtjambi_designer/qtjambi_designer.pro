QTJAMBILIB = QtJambiDesigner
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT += widgets uiplugin designer

HEADERS +=  \
            designer.h \
            qtjambiresourcebrowser.h

SOURCES += \
    qtjambi_designer_util.cpp
