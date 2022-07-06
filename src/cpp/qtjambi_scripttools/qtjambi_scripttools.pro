QTJAMBILIB = QtJambiScriptTools
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

macx:{
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtWidgets.framework/Headers/
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtScript.framework/Headers/
}
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtScript
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets

QT += scripttools
