
QT = xml core
CONFIG += console no_batch
mac:CONFIG -= app_bundle

include(uic/uic.pri)

INCLUDEPATH += $$PWD
DEFINES += QT_UIC_JAVA_GENERATOR QT_UIC

HEADERS += javawritedeclaration.h \
    javawriteincludes.h \
    javawriteinitialization.h \
    javautils.h \
    uic/uic.h

SOURCES += javawritedeclaration.cpp \
    javawriteincludes.cpp \
    javawriteinitialization.cpp \
    javautils.cpp \
    main.cpp \
    uic/uic.cpp

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}

# make install related...
!isEmpty(INSTALL_PREFIX) {
    target.path = $$INSTALL_PREFIX
    INSTALLS = target
}

mac {
    #QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.9.sdk
    # (commented out) To inherit the Qt SDK default arches
    #contains(QT_CONFIG, x86):CONFIG += x86
    #contains(QT_CONFIG, ppc):CONFIG += ppc
    #contains(QT_CONFIG, x86_64):CONFIG += x86_64
    #contains(QT_CONFIG, ppc64):CONFIG += ppc64
    CONFIG -= precompile_header
}

linux-g++* | freebsd-g++* {
    QMAKE_LFLAGS += -Wl,--rpath,\\\$\$ORIGIN/../lib
}
