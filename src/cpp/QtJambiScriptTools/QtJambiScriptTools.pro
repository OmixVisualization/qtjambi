include(../QtJambi/configure.pri)

macx:{
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtWidgets.framework/Headers/
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtScript.framework/Headers/
}
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtScript
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets

QT += scripttools
