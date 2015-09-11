QT             += xml
TEMPLATE        = app
DESTDIR         = ../bin
TARGET          = testGenerator
MOC_DIR         = ./../tmp
OBJECTS_DIR     = ./../tmp
CONFIG         += qt warn_on debug
HEADERS         = ./../src/*.h
SOURCES        += ./../src/*.cpp
FORMS           =
DEFINES        += USE_QT4
OSGROOT         = $$(OSG_ROOT)

INCLUDEPATH    += ./../src

