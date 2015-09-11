!macx:!exists($$(JAVA_HOME_TARGET)) {
    # Ant/Maven should set this up (this can't be a symlink)
    error("Please set your JAVA_HOME_TARGET environment variable to point to the directory of your Java SDK.  Current JAVA_HOME_TARGET: $$(JAVA_HOME_TARGET)")
}

isEmpty(TARGET) {
    error("Please specify TARGET name before including qtjambi_base.pri");
}

macx:{
    LIBS += -framework JavaVM
} else {
    INCLUDEPATH += $$(JAVA_HOME_TARGET)/include
    win32 {
        INCLUDEPATH += $$(JAVA_HOME_TARGET)/include/win32
    }
    solaris-g++ | solaris-cc {
        INCLUDEPATH += $$(JAVA_HOME_TARGET)/include/solaris
    }
    linux-g++* {
        INCLUDEPATH += $$(JAVA_HOME_TARGET)/include/linux
    }
    freebsd-g++* {
        INCLUDEPATH += $$(JAVA_HOME_TARGET)/include/freebsd
    }
}

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}

# make install related...
!isEmpty(INSTALL_PREFIX) {
    target.path = $$INSTALL_PREFIX
    INSTALLS = target
}

win32:CONFIG += precompile_header

macx {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.4
    contains(QT_CONFIG, x86):CONFIG += x86
    contains(QT_CONFIG, ppc):CONFIG += ppc
    contains(QT_CONFIG, x86_64):CONFIG += x86_64
    contains(QT_CONFIG, ppc64):CONFIG += ppc64
    CONFIG -= precompile_header
}

INCLUDEPATH += $$PWD/../include


LIB_QTJAMBI = qtjambi
CONFIG(debug, debug|release) {
    LIB_QTJAMBI = $$join(LIB_QTJAMBI,,,_debuglib)
}


INCLUDEPATH += $$PWD/../../qtjambi

macx:{
    LIBS += ../../lib/lib$${LIB_QTJAMBI}.jnilib
} else {
    LIBS += -L../../lib -l$${LIB_QTJAMBI}
}

linux-g++* | freebsd-g++* {
    QMAKE_LFLAGS += -Wl,--rpath,\\\$\$ORIGIN/../lib
}
