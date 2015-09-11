TARGET = org_qtjambi_qt_quick

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt_quick/org_qtjambi_qt_quick.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core gui qml quick

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
