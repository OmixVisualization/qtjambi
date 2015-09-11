TARGET = JambiLanguage

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

TEMPLATE = lib
QT += designer
CONFIG  += qt warn_on plugin

# Have to include after TARGET...
include($$PWD/../../qtjambi/qtjambi_include.pri)

HEADERS += \
    jambilanguageplugin.h \
        qtjambiintrospection_p.h \


SOURCES += \
    jambilanguageplugin.cpp \
    qtjambiintrospection.cpp \

# These option changes are recommended since at least: win32-msvc.net
win32-msvc* {
    QMAKE_CXXFLAGS += -Zm1000
    QMAKE_CXXFLAGS -= -Zm200
    QMAKE_CFLAGS -= -Zm200
}

RESOURCES += resources.qrc

INCLUDEPATH += $$PWD
exists($$QTDIR) {
    INCLUDEPATH += $$QTDIR/QtDesigner
}
INCLUDEPATH += $$PWD/../include

freebsd-g++* {
    QT_SOURCE_TREE=$$fromfile($$(QTDIR)/.qmake.cache,QT_SOURCE_TREE)
    INCLUDEPATH += $$QT_SOURCE_TREE/include/Qt
}

# patch up some bad things set in qtjambi_include...
DESTDIR = ../../plugins/designer
macx:QMAKE_EXTENSION_SHLIB = dylib
