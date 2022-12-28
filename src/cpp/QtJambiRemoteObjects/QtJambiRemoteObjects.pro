include(../QtJambi/configure.pri)

QT = core remoteobjects
lessThan(QT_MAJOR_VERSION, 6):{
    contains(QTJAMBI_MODULE, QtJambiRemoteObjects-private):{
        QT += remoteobjects-private
        CONFIG+=no_private_qt_headers_warning
    }else{
        DEFINES += QTJAMBI_NO_RO_PRIVATE
    }
}

msvc:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
    impl.cpp

HEADERS += \
    hashes.h \
    utils_p.h
