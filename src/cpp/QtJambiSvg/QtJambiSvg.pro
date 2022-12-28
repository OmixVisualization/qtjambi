include(../QtJambi/configure.pri)

QT = core svg

lessThan(QT_MAJOR_VERSION, 6):{
    QT += widgets
}
