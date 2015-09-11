!macx {
    !exists($$(JAVA_HOME_TARGET)) {
        # Ant/Maven should set this up (this can't be a symlink)
        exists($$(JAVA_HOME)) {
            JAVA_HOME_TARGET = $$(JAVA_HOME)
        } else {
            error("Please set your JAVA_HOME_TARGET or JAVA_HOME environment variable to point to the directory of your Java SDK. Current JAVA_HOME_TARGET: $$(JAVA_HOME_TARGET) JAVA_HOME: $$(JAVA_HOME)")
        }
    } else {
        # This has the effect of making $$JAVA_HOME_TARGET work in this file as used below
        JAVA_HOME_TARGET = $$(JAVA_HOME_TARGET)
    }
}

VER_MAJ = $$section(VERSION, ., 0, 0)
VER_MIN = $$section(VERSION, ., 1, 1)
VER_PAT = $$section(VERSION, ., 2, 2)

isEmpty(TARGET) {
    error("Please specify TARGET name before including qtjambi_base.pri");
}

TEMPLATE = lib
DESTDIR = ../lib
DLLDESTDIR = ../bin


#!isEmpty(QTJAMBI_CONFIG) {
#    contains(QTJAMBI_CONFIG, debug) {
#        TARGET = $$member(TARGET, 0)_debuglib
#    }
#} else {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)_debuglib
    }
#}

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

macx:{
    # if(!defined(QMAKE_MAC_SDK)) { }
    #QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk
    #QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
    LIBS += -framework JavaVM
    QMAKE_EXTENSION_SHLIB = jnilib
	# this option is necessary to properly compile on mountain lion because of std::string to char* casts
	QMAKE_CXXFLAGS += -fpermissive 
} else {
    INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include)
    win32 {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/win32)
    }
    solaris-g++ | solaris-cc {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/solaris)
    }
    linux-g++* {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/linux)
    }
    freebsd-g++* {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/freebsd)
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
    contains(QT_CONFIG, x86):CONFIG += x86
    contains(QT_CONFIG, ppc):CONFIG += ppc
    contains(QT_CONFIG, x86_64):CONFIG += x86_64
    contains(QT_CONFIG, ppc64):CONFIG += ppc64
    CONFIG -= precompile_header
}

# gcc reports some functions as unused when they are not.
linux-g++* | freebsd-g++* {
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

# Extra options to be set when using jump tables...
jumptable{
    CONFIG += hide_symbols

    # Tell the linker to strip the binaries..
    macx:QMAKE_LFLAGS += -Wl,-x
}

#NOTE: this is just to test, uncomment if after all RPATHs are wanted.
#linux-g++* | freebsd-g++* {
    #QMAKE_LFLAGS += -Wl,--rpath,\\\$\$ORIGIN/../lib
#}
