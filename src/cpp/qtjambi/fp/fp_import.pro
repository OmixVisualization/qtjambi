DEFINES += QT_NO_VERSION_TAGGING
TEMPLATE = lib
CONFIG += unversioned_libname skip_target_version_ext

QT = core

CONFIG(debug, debug|release) {
    QTJAMBICONF = debug
}else{
    QTJAMBICONF = release
}

macx:CONFIG -= precompile_header
macx:QMAKE_CXXFLAGS += -Wc++14-extensions
macx:QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
android:CONFIG += rtti
android:CONFIG += exceptions
android:QMAKE_CXXFLAGS_EXCEPTIONS_ON += -fexceptions
android:QMAKE_CXXFLAGS += -fexceptions -frtti
android:QMAKE_LFLAGS += -Wl,--export-dynamic -Wl,--exclude-libs,libgcc_real.a -Wl,--exclude-libs,libunwind.a -Wl,--exclude-libs,libgcc.a -lunwind
android:QMAKE_CXXFLAGS += -funwind-tables

greaterThan(QT_MAJOR_VERSION, 5):{
    DESTDIR = ../../$$QTJAMBICONF
    macx {
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
        }
    }
}else{
    android:{
        armeabi-v7a: DESTDIR = ../../$$QTJAMBICONF-armeabi-v7a
        arm64-v8a: DESTDIR = ../../$$QTJAMBICONF-arm64-v8a
        x86: DESTDIR = ../../$$QTJAMBICONF-x86
        x86_64: DESTDIR = ../../$$QTJAMBICONF-x86_64
    }else{
        DESTDIR = ../../$$QTJAMBICONF
    }
}

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}
