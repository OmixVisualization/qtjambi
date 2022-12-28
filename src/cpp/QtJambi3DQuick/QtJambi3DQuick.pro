include(../QtJambi/configure.pri)

greaterThan(QT_MAJOR_VERSION, 5): {
    CONFIG+=no_private_qt_headers_warning
    QT.3dcore_private.name = Qt3DCore
}

QT = core 3dquick
