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

QTJAMBI_BUILDDIR = $$clean_path($$PWD/../../../$$VERSION/build/)

macx:{
    LIBS += $$DESTDIR/lib$$member(QTJAMBI_LIB_NAME, 0).jnilib
    QMAKE_SONAME_PREFIX = @rpath
    QMAKE_RPATHDIR = @loader_path/.
} else {
    LIBS += -L$$DESTDIR
    linux-g++*: QMAKE_RPATHDIR = $ORIGIN/.

    android:{
        armeabi-v7a: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_armeabi-v7a
        arm64-v8a: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_arm64-v8a
        x86: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_x86
        x86_64: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_x86_64
    }else{
        LIBS += -l$$QTJAMBI_LIB_NAME
    }
}

# These option changes are recommended since at least: win32-msvc2005
win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}

