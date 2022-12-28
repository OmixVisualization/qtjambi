include(../QtJambi/configure.pri)

HEADERS += \
    hashes.h \
    utils_p.h
SOURCES += \
    impl.cpp

QT = core qml core-private qml-private
CONFIG+=no_private_qt_headers_warning

msvc:QMAKE_CXXFLAGS += /bigobj

