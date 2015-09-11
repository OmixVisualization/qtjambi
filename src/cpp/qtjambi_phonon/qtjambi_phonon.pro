TARGET = org_qtjambi_qt_phonon

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

exists($$(PHONON_INCLUDEPATH)) {
    # This is for kdephonon. No-one can have both in
    # the machine, so this shouldn’t break anything...
    # This is before the qtjambi_include.pri include so try
    # to be before the other things that end up in the list
    INCLUDEPATH += $$(PHONON_INCLUDEPATH)
}

# Generator uses phonon/<includefile> including scheme for phonon,
# this is to allow building as generator uses only single level includes
exists($$(PHONON_INCLUDEPATH)/phonon) {
    INCLUDEPATH += $$(PHONON_INCLUDEPATH)/phonon
}

exists($$(PHONON_LIBS)) {
    LIBS += -L$$(PHONON_LIBS)
}

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_phonon/org_qtjambi_qt_phonon.pri)

# libphonon.so.4 is dependant on many, using modifiers
#  libQtCore.so.4 libQtGui.so.4 libQtXml.so.4 libQtDBus.so.4(known optional)
QT += phonon
