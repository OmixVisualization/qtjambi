TARGET = org_qtjambi_examples_generator

INCLUDEPATH += $$PWD ../src

DESTDIR = ../target

include($(JAMBIDIR)/qtjambi/qtjambi_include.pri)
include(../cpp/org_qtjambi_examples_generator/org_qtjambi_examples_generator.pri)

HEADERS += ../src/gameaction.h \
           ../src/gameanimation.h \
           ../src/gamegrammar.h \
           ../src/gamenamespace.h \
           ../src/gameobject.h \
           ../src/gamescene.h \
           ../src/lookaction.h \
           ../src/pickupaction.h \
           ../src/useaction.h \
           ../src/point3d.h \
           ../src/abstractgameobject.h

SOURCES += ../src/gameaction.cpp \
           ../src/gameanimation.cpp \
           ../src/gamegrammar.cpp \
           ../src/gameobject.cpp \
           ../src/gamescene.cpp \
           ../src/lookaction.cpp \
           ../src/pickupaction.cpp \
           ../src/useaction.cpp
