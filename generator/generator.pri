isEmpty(GENERATORPATH):GENERATORPATH = $$PWD
INCLUDEPATH += $$GENERATORPATH

TEMPLATE = app
TARGET +=
DEPENDPATH += $$GENERATORPATH tests parser
mac:CONFIG -= app_bundle
INCLUDEPATH += $$GENERATORPATH/.

CONFIG += console
RESOURCES += generator.qrc

include($$GENERATORPATH/parser/rxx.pri)

include($$GENERATORPATH/parser/rpp/rpp.pri)

# These option changes are recommended since at least: win32-msvc.net
#          We have checked they are still needed with: win32-msvc2010 
win32-msvc* {   
        QMAKE_CXXFLAGS += -wd4996
        QMAKE_CFLAGS += -wd4996
}

# Input
HEADERS += \
        $$GENERATORPATH/generator.h \
        $$GENERATORPATH/main.h \
        $$GENERATORPATH/wrapper.h \
        $$GENERATORPATH/preprocesshandler.h \
        $$GENERATORPATH/reporthandler.h \
        $$GENERATORPATH/typeparser.h \
        $$GENERATORPATH/typesystem/typesystem.h \
        $$GENERATORPATH/typesystem/codesnip.h \
        $$GENERATORPATH/typesystem/handler.h \
        $$GENERATORPATH/typesystem/modification.h \
        $$GENERATORPATH/typesystem/typedatabase.h \
        $$GENERATORPATH/typesystem/typeentry.h \
        $$GENERATORPATH/asttoxml.h \
        $$GENERATORPATH/fileout.h \
        $$GENERATORPATH/generatorset.h \
        $$GENERATORPATH/metajava.h \
        $$GENERATORPATH/customtypes.h \
        $$GENERATORPATH/abstractmetabuilder.h \
        $$GENERATORPATH/abstractmetalang.h \
        $$GENERATORPATH/prigenerator.h \
        $$GENERATORPATH/util.h \
        $$GENERATORPATH/docindex/docindexreader.h \
        $$GENERATORPATH/docindex/docclass.h \
        $$GENERATORPATH/docindex/docelement.h \
        $$GENERATORPATH/docindex/docenum.h \
        $$GENERATORPATH/docindex/docfunction.h \
        $$GENERATORPATH/docindex/docmodel.h \
        $$GENERATORPATH/docindex/docvariable.h \
        $$GENERATORPATH/docindex/docproperty.h \
        $$PWD/docindex/docnamespace.h \
        $$PWD/docindex/doctypedef.h

SOURCES += \
        $$GENERATORPATH/generator.cpp \
        $$GENERATORPATH/main.cpp \
        $$GENERATORPATH/wrapper.cpp \
        $$GENERATORPATH/preprocesshandler.cpp \
        $$GENERATORPATH/reporthandler.cpp \
        $$GENERATORPATH/typeparser.cpp \
        $$GENERATORPATH/typesystem/codesnip.cpp \
        $$GENERATORPATH/typesystem/handler.cpp \
        $$GENERATORPATH/typesystem/modification.cpp \
        #$$GENERATORPATH/typesystem/stackelement.cpp \
        $$GENERATORPATH/typesystem/typedatabase.cpp \
        $$GENERATORPATH/typesystem/typeentry.cpp \
        $$GENERATORPATH/typesystem/typesystem.cpp \
        #$$GENERATORPATH/typesystem/
        $$GENERATORPATH/asttoxml.cpp \
        $$GENERATORPATH/fileout.cpp \
        $$GENERATORPATH/generatorset.cpp \
        $$GENERATORPATH/metajava.cpp \
        $$GENERATORPATH/customtypes.cpp \
        $$GENERATORPATH/abstractmetabuilder.cpp \
        $$GENERATORPATH/abstractmetalang.cpp \
        $$GENERATORPATH/prigenerator.cpp \
        $$GENERATORPATH/util.cpp \
        $$GENERATORPATH/docindex/docindexreader.cpp \
        $$GENERATORPATH/docindex/docclass.cpp \
        $$GENERATORPATH/docindex/docelement.cpp \
        $$GENERATORPATH/docindex/docenum.cpp \
        $$GENERATORPATH/docindex/docfunction.cpp \
        $$GENERATORPATH/docindex/docmodel.cpp \
        $$GENERATORPATH/docindex/docvariable.cpp \
        $$GENERATORPATH/docindex/docproperty.cpp \
        $$PWD/docindex/docnamespace.cpp \
        $$PWD/docindex/doctypedef.cpp



QT = core xml network concurrent

# These option changes are recommended since at least: win32-msvc.net
win32-msvc* {
        QMAKE_CXXFLAGS += -Zm500
        QMAKE_CXXFLAGS -= -Zm200
        QMAKE_CFLAGS -= -Zm200
}

mac {
    contains(QT_CONFIG, x86):CONFIG += x86
    contains(QT_CONFIG, ppc):CONFIG += ppc
    contains(QT_CONFIG, x86_64):CONFIG += x86_64
    contains(QT_CONFIG, ppc64):CONFIG += ppc64
	# this option is necessary to properly compile on mountain lion because of std::string to char* casts
	QMAKE_CXXFLAGS += -fpermissive
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
}
