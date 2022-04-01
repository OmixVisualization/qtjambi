QTJAMBILIB = QtJambiTextToSpeech
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core texttospeech

HEADERS += \
    qtjambi_texttospeech.h \
    qtjambi_texttospeech_qhashes.h

SOURCES += \
    qtjambi_texttospeech.cpp
