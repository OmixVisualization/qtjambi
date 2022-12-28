include(../QtJambi/configure.pri)

QT = core sql

macx:{
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtWidgets.framework/Headers
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]
}
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets

HEADERS += \
    hashes.h

SOURCES += \
    impl.cpp
