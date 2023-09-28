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

equals(QT_MAJOR_VERSION, 6):greaterThan(QT_MINOR_VERSION, 5):{
    QT += gui-private
    CONFIG+=no_private_qt_headers_warning
}
greaterThan(QT_MAJOR_VERSION, 6):{
    QT += gui-private
    CONFIG+=no_private_qt_headers_warning
}

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    impl.cpp
