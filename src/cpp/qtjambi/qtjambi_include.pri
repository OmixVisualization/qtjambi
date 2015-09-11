include(qtjambi_base.pri)

greaterThan(QT_MAJOR_VERSION, 4): win32*:{
	CONFIG(debug, debug|release) {
		QTJAMBI_LIB_NAME = qtjambi_debuglib$$QT_MAJOR_VERSION
	}else{
		QTJAMBI_LIB_NAME = qtjambi$$QT_MAJOR_VERSION
	}
}else{
	CONFIG(debug, debug|release) {
		QTJAMBI_LIB_NAME = qtjambi_debuglib
	}else{
		QTJAMBI_LIB_NAME = qtjambi
	}
}

macx:{
    LIBS += $$PWD/../../../build/qmake-qtjambi/lib/lib$$member(QTJAMBI_LIB_NAME, 0).jnilib
} else {
    LIBS += -L$$PWD/../../../build/qmake-qtjambi/lib -l$$QTJAMBI_LIB_NAME
}

QTJAMBI_CPP = ../../../build/generator/out/cpp/

# These option changes are recommended since at least: win32-msvc2005
win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}
