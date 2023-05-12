!android:{
    JAVA_HOME_TARGET = $$(JAVA_HOME_TARGET)
    isEmpty(JAVA_HOME_TARGET):{
        JAVA_HOME_TARGET = $$(JAVA_HOME)
    }
    isEmpty(JAVA_HOME_TARGET):{
        error("Please set your JAVA_HOME_TARGET or JAVA_HOME environment variable to point to the directory of your Java SDK. Current JAVA_HOME_TARGET: $$(JAVA_HOME_TARGET) JAVA_HOME: $$(JAVA_HOME)")
    }else{
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
}

isEmpty(VERSION):{
    isEmpty(QTJAMBI_PATCH_VERSION):{
        error("Please set QTJAMBI_PATCH_VERSION");
    }
    VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION
}

isEmpty(TARGET):{
    QTJAMBI_PROJECT = $$basename(_PRO_FILE_)
    QTJAMBI_PROJECT = $$section(QTJAMBI_PROJECT, .pro, 0, 0)
    TARGET = $$QTJAMBI_PROJECT
}

TEMPLATE = lib
VER_MAJ = $$section(VERSION, ., 0, 0)
VER_MIN = $$section(VERSION, ., 1, 1)
VER_PAT = $$section(VERSION, ., 2, 2)

isEmpty(QTJAMBI_PROJECT):{
    QTJAMBI_PROJECT = $$TARGET
}

isEmpty(DESTDIR) {
    DESTDIR = ../lib
    DLLDESTDIR = ../bin
}

CONFIG(debug, debug|release) {
    win32:{
        TARGET = $$member(TARGET, 0)d
    }else{
        macx:{
            TARGET = $$member(TARGET, 0)_debug
        }else{
            TARGET = $$member(TARGET, 0)_debug
        }
    }
}

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

DEFINES += QTJAMBI_PATCH_VERSION=$$QTJAMBI_PATCH_VERSION

macx:{
    CONFIG+=no_default_rpath no_qt_rpath
    QMAKE_SONAME_PREFIX = @rpath
    if($$QTJAMBI_GENERATE_FRAMEWORKS):{
        QMAKE_TARGET_BUNDLE_PREFIX = io.qtjambi
        CONFIG+=lib_bundle
        QMAKE_RPATHDIR = @loader_path/../../../
    }else{
        equals(QTJAMBI_EXTENSION_SHLIB,dylib) | isEmpty(QTJAMBI_EXTENSION_SHLIB):{
        }else{
            QMAKE_EXTENSION_SHLIB = $$QTJAMBI_EXTENSION_SHLIB
        }
        QMAKE_RPATHDIR = @loader_path/.
    }
}
macx | ios:{
    INCLUDEPATH += $$JAVA_HOME_TARGET/include
    INCLUDEPATH += $$JAVA_HOME_TARGET/include/darwin

    # this option is necessary to properly compile on mountain lion because of std::string to char* casts
    QMAKE_CXXFLAGS += -fpermissive

    contains(QT_CONFIG, x86):CONFIG += x86
    contains(QT_CONFIG, ppc):CONFIG += ppc
    contains(QT_CONFIG, x86_64):CONFIG += x86_64
    contains(QT_CONFIG, ppc64):CONFIG += ppc64
    contains(QT_CONFIG, arm64):CONFIG += arm64
    QMAKE_CXXFLAGS += -Wc++14-extensions
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
        }
    }
} else {
    !android:INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include)
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

ios:{
    CONFIG -= precompile_header
}else{
    CONFIG += precompile_header
}

android:{
    CONFIG += rtti exceptions
    QMAKE_CXXFLAGS_EXCEPTIONS_ON += -fexceptions
    QMAKE_CXXFLAGS += -fexceptions -frtti -funwind-tables
    QMAKE_LFLAGS += -Wl,--export-dynamic -Wl,--exclude-libs,libgcc_real.a -Wl,--exclude-libs,libunwind.a -Wl,--exclude-libs,libgcc.a -lunwind
}

# gcc reports some functions as unused when they are not.
linux-g++* | freebsd-g++* | win32-g++* {
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

linux-g++* : lessThan(QT_MAJOR_VERSION, 6):{
    QMAKE_CXXFLAGS += -fno-sized-deallocation
}

win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}

GENERATOR_PRI = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/$$QTJAMBI_PROJECT/generated.pri)
exists($$GENERATOR_PRI): include($$GENERATOR_PRI)

!equals(QTJAMBI_PROJECT, "QtJambi"):!equals(QTJAMBI_PROJECT, "QtJambiLauncher"){
    isEmpty(QTJAMBI_LIB_PATH){
        QTJAMBI_LIB_PATH = $$DESTDIR
    }
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
                    CONFIG += optimize_full
            }
    }

    GENERATED = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/$$QTJAMBI_PROJECT/$$QTJAMBI_PROJECT)
    exists($$GENERATED): INSTALL_HEADERS.files += $$GENERATED
    GENERATED = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/$$QTJAMBI_PROJECT/$$member(QTJAMBI_PROJECT,0)Depends)
    exists($$GENERATED): INSTALL_HEADERS.files += $$GENERATED
    GENERATED = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/$$QTJAMBI_PROJECT/$$member(QTJAMBI_PROJECT,0)Version)
    exists($$GENERATED): INSTALL_HEADERS.files += $$GENERATED
    GENERATED = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/$$QTJAMBI_PROJECT/version.h)
    exists($$GENERATED): INSTALL_HEADERS.files += $$GENERATED
    exists($$dirname(_PRO_FILE_)/hashes.h):INSTALL_HEADERS.files += $$dirname(_PRO_FILE_)/hashes.h
    INSTALL_HEADERS.path = $$QTJAMBI_PLATFORM_BUILDDIR/include/$$QTJAMBI_PROJECT

    macx:{
        if($$QTJAMBI_GENERATE_FRAMEWORKS):{
            LIBS += -F$$QTJAMBI_LIB_PATH
            LIBS += -framework $$QTJAMBI_LIB_NAME
            INSTALL_HEADERS.version = Versions
            INSTALL_HEADERS.path = Headers/$$QTJAMBI_PROJECT
            QMAKE_BUNDLE_DATA += INSTALL_HEADERS
        }else{
            equals(QTJAMBI_EXTENSION_SHLIB,dylib) | isEmpty(QTJAMBI_EXTENSION_SHLIB):{
                LIBS += -L$$QTJAMBI_LIB_PATH
                LIBS += -l$$QTJAMBI_LIB_NAME
            }else{
                LIBS += $$QTJAMBI_LIB_PATH/lib$$member(QTJAMBI_LIB_NAME, 0).$$QTJAMBI_EXTENSION_SHLIB
            }
        }
    } else {
        INSTALLS += INSTALL_HEADERS
        LIBS += -L$$QTJAMBI_LIB_PATH
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
}

ios: DEFINES += LIBRARY_NAME=$$member(TARGET, 0)
