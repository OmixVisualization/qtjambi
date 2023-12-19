include(../QtJambi/configure.pri)

QT = core gui widgets
win32*: {
    QT += axcontainer
    CONFIG  += dll
}
