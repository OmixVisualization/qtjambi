include(../QtJambi/configure.pri)

QT = core texttospeech

HEADERS += \
    hashes.h \
    utils_p.h

SOURCES += \
    impl.cpp

macx : greaterThan(QT_MAJOR_VERSION, 5) : {
    INCLUDEPATH += $(QTDIR)/include
}
