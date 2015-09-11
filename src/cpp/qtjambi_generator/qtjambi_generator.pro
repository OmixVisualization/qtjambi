TARGET = org_qtjambi_tools_generator

include(../qtjambi/qtjambi_include.pri)
include(../cpp/org_qtjambi_tools_generator/org_qtjambi_tools_generator.pri)

INCLUDEPATH += ../generator
INCLUDEPATH += ../common

include(../generator/parser/rxx.pri)
include(../generator/parser/rpp/rpp.pri)

GENERATOR_DIR = ../generator

HEADERS += \
    $$GENERATOR_DIR/reporthandler.h \
    $$GENERATOR_DIR/typesystem.h \
        $$GENERATOR_DIR/main.h \
        $$GENERATOR_DIR/typeparser.h \
        $$GENERATOR_DIR/metajavabuilder.h \
        $$GENERATOR_DIR/metajava.h \
        $$GENERATOR_DIR/generator.h \
        $$GENERATOR_DIR/javagenerator.h \
        $$GENERATOR_DIR/cppgenerator.h \
        $$GENERATOR_DIR/cppimplgenerator.h \
        $$GENERATOR_DIR/cppheadergenerator.h \
        $$GENERATOR_DIR/metainfogenerator.h \
        $$GENERATOR_DIR/juicdatagenerator.h \
        $$GENERATOR_DIR/customtypes.h \

SOURCES += \
    $$GENERATOR_DIR/reporthandler.cpp \
    $$GENERATOR_DIR/typesystem.cpp \
        $$GENERATOR_DIR/typeparser.cpp \
        $$GENERATOR_DIR/metajavabuilder.cpp \
        $$GENERATOR_DIR/metajava.cpp \
        $$GENERATOR_DIR/generator.cpp \
        $$GENERATOR_DIR/javagenerator.cpp \
        $$GENERATOR_DIR/cppgenerator.cpp \
        $$GENERATOR_DIR/cppimplgenerator.cpp \
        $$GENERATOR_DIR/cppheadergenerator.cpp \
        $$GENERATOR_DIR/metainfogenerator.cpp \
        $$GENERATOR_DIR/juicdatagenerator.cpp \
        $$GENERATOR_DIR/customtypes.cpp \

QT += xml
