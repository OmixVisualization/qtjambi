include(../QtJambi/configure.pri)

QT += core gui qml quick quick-private
CONFIG+=no_private_qt_headers_warning

SOURCES += \
    impl.cpp

HEADERS += \
    hashes.h \
    utils_p.h

greaterThan(QT_MAJOR_VERSION, 5):{
    OBJECTIVE_SOURCES += QSGMetalTexture.mm
}
