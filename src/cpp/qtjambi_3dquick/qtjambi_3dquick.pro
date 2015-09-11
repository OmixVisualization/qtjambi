TARGET = org_qtjambi_qt3d_quick

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt3d_quick/org_qtjambi_qt3d_quick.pri)

# We manually add these include paths, instead of using "QT += module" which is bad for us
#  as it created unnecessary hardwired linkage to libraries we may never reference any symbols from.
# To be able to remove this we need to make generator better/smarter about #include directives
#  and it might not include these files at all.
INCLUDEPATH += $$(QTDIR)/include/Qt3DCore

QT += core gui qml 3dcore 3drenderer 3dinput 3dquick

DEPENDPATH += $$(QTDIR)/include/Qt3DQuick/$$QT_VERSION
DEPENDPATH += $$(QTDIR)/include/Qt3DQuick/$$QT_VERSION/Qt3DQuick
DEPENDPATH += $$(QTDIR)/include/Qt3DQuick/$$QT_VERSION/Qt3DQuick/private
DEPENDPATH += $$PWD

INCLUDEPATH += $$(QTDIR)/include/Qt3DQuick/$$QT_VERSION
INCLUDEPATH += $$(QTDIR)/include/Qt3DQuick/$$QT_VERSION/Qt3DQuick
INCLUDEPATH += $$(QTDIR)/include/Qt3DQuick/$$QT_VERSION/Qt3DQuick/private
INCLUDEPATH += $$PWD

greaterThan(QT_MAJOR_VERSION, 4): win32*:{
	CONFIG(debug, debug|release) {
		QML_LIB_NAME = org_qtjambi_qt_qml_debuglib$$VER_MAJ
	}else{
		QML_LIB_NAME = org_qtjambi_qt_qml$$VER_MAJ
	}
}else{
	CONFIG(debug, debug|release) {
		QML_LIB_NAME = org_qtjambi_qt_qml_debuglib
	}else{
		QML_LIB_NAME = org_qtjambi_qt_qml
	}
}

macx:{
    LIBS += $$PWD/../../../build/qmake-qtjambi/lib/lib$$member(QML_LIB_NAME, 0).jnilib
} else {
    LIBS += -L$$PWD/../../../build/qmake-qtjambi/lib -l$$QML_LIB_NAME
}
