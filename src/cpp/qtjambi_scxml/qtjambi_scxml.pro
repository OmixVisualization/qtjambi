QTJAMBILIB = QtJambiScxml
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core scxml

equals(QT_MAJOR_VERSION, 5):{
    lessThan(QT_MINOR_VERSION, 12): SOURCES += ForeachLoopBody512.cpp
}

SOURCES += \
    qtjambi_scxml_util.cpp
