isEmpty(TARGET):{
    PRO_FILE = $$basename(_PRO_FILE_)
    PRO_FILE = $$section(PRO_FILE, .pro, 0, 0)
    TARGET = PRO_FILE;
}
TEMPLATE = lib

QT -= core gui

DEFINES += QT_NO_VERSION_TAGGING
CONFIG -= staticlib static
CONFIG += unversioned_libname skip_target_version_ext shared

INCLUDEPATH += $$[QT_INSTALL_HEADERS]

SOURCES = impl.cpp

CONFIG(debug, debug|release) {
    QTJAMBICONF = debug
}else{
    QTJAMBICONF = release
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

macx:CONFIG -= precompile_header
macx:CONFIG += no_default_rpath no_qt_rpath
macx:QMAKE_CXXFLAGS += -Wc++14-extensions
macx:QMAKE_CXXFLAGS += -F$$[QT_INSTALL_LIBS]
macx:QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
android:CONFIG += rtti
android:CONFIG += exceptions
android:QMAKE_CXXFLAGS_EXCEPTIONS_ON += -fexceptions
android:QMAKE_CXXFLAGS += -fexceptions -frtti
android:QMAKE_LFLAGS += -Wl,--export-dynamic -Wl,--exclude-libs,libgcc_real.a -Wl,--exclude-libs,libunwind.a -Wl,--exclude-libs,libgcc.a -lunwind
android:QMAKE_CXXFLAGS += -funwind-tables

greaterThan(QT_MAJOR_VERSION, 5):{
    DESTDIR = ../../QtJambi/$$QTJAMBICONF
    macx {
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
        }
    }
}else{
    android:{
        armeabi-v7a: DESTDIR = ../../QtJambi/$$QTJAMBICONF-armeabi-v7a
        arm64-v8a: DESTDIR = ../../QtJambi/$$QTJAMBICONF-arm64-v8a
        x86: DESTDIR = ../../QtJambi/$$QTJAMBICONF-x86
        x86_64: DESTDIR = ../../QtJambi/$$QTJAMBICONF-x86_64
    }else{
        DESTDIR = ../../QtJambi/$$QTJAMBICONF
    }
}
DESTDIR = ../../lib
DLLDESTDIR = ../../lib

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}
