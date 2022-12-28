include(../QtJambi/configure.pri)

QT = core gui multimedia

lessThan(QT_MAJOR_VERSION, 6):{
    macx:{
        INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtMultimediaWidgets.framework/Headers/
    }
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtMultimediaWidgets
}else{
    QMAKE_LIBS_WMF = \

}

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    impl.cpp
