include(../QtJambi/configure.pri)

QT += core gui gui-private
CONFIG+=no_private_qt_headers_warning

macx:{
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtGui.framework/Headers/qpa
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]
}
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtGui/qpa

SOURCES += \
    impl.cpp
