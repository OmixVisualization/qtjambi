include(../QtJambi/configure.pri)

QT = core sensors

contains(QTJAMBI_MODULE, QtJambiSensors-private):{
    QT += sensors-private core-private
    CONFIG+=no_private_qt_headers_warning
}else{
    DEFINES += QTJAMBI_NO_SENSORS_PRIVATE
}

HEADERS += \
    utils_p.h

SOURCES += \
    impl.cpp
