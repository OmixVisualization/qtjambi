include(qtjambi_base.pri)

win32*:{
	CONFIG(debug, debug|release) {
                QTJAMBI_LIB_NAME = QtJambid$$QT_MAJOR_VERSION
	}else{
                QTJAMBI_LIB_NAME = QtJambi$$QT_MAJOR_VERSION
	}
}else{
	CONFIG(debug, debug|release) {
                QTJAMBI_LIB_NAME = QtJambi_debug
	}else{
                QTJAMBI_LIB_NAME = QtJambi
	}
}

macx:{
    LIBS += $$PWD/../../../build/qmake-qtjambi/lib/lib$$member(QTJAMBI_LIB_NAME, 0).jnilib
} else {
    LIBS += -L$$PWD/../../../build/qmake-qtjambi/lib
    android:{
        armeabi-v7a: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_armeabi-v7a
        arm64-v8a: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_arm64-v8a
        x86: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_x86
        x86_64: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_x86_64
    }else{
        LIBS += -l$$QTJAMBI_LIB_NAME
    }
}

QTJAMBI_CPP = ../../../build/generator/out/cpp/

# These option changes are recommended since at least: win32-msvc2005
win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}

