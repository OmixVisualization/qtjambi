include(../QtJambi/configure.pri)

# because QActionEvent refers to qaction.h
macx:{
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtGui.framework/Headers/qpa
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtWidgets.framework/Headers
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]
}
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtGui/qpa
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtPlatformHeaders

QT = core core-private gui gui-private
CONFIG+=no_private_qt_headers_warning

SOURCES += \
    impl.cpp

HEADERS += \
    hashes.h \
    utils_p.h

!ios:{
    PRECOMPILED_HEADER = \
    pch_p.h
    CONFIG += precompile_header
}

win32-msvc*: {
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

