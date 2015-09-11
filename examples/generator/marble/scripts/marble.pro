
TARGET = org_qtjambi_marble

DESTDIR = ../target

INCLUDEPATH += $$PWD $(MARBLEDIR)/lib $(MARBLEDIR)/src/lib $(MARBLEDIR)/src/lib/geodata/data $(MARBLEDIR)/src/lib/geodata $(MARBLEDIR)/src/lib/geodata/parser $(JAVA_HOME)/include

include($(JAMBIDIR)/qtjambi/qtjambi_include.pri)
include(../cpp/org_qtjambi_marble/org_qtjambi_marble.pri)


LIBS += -L/usr/local/lib -lmarblewidget

INCLUDES += $(MARBLEDIR)/src/lib/MarbleMap.h
INCLUDES += $(MARBLEDIR)/src/lib/MarbleWidget.h
INCLUDES += $(MARBLEDIR)/src/lib/MarbleModel.h
