QTJAMBILIB = QtJambiScriptTools
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtWidgets.framework/Headers/
    INCLUDEPATH += $$(QTDIR)/lib/QtScript.framework/Headers/
}
INCLUDEPATH += $$(QTDIR)/include/QtScript
INCLUDEPATH += $$(QTDIR)/include/QtWidgets

QT += scripttools
