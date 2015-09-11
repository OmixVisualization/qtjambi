TARGET = org_qtjambi_tools_designer

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_tools_designer/org_qtjambi_tools_designer.pri)

# libQtDesigner.so.4.7.4 is dependant on many, using modifiers
#  libQtCore.so.4 libQtGui.so.4 libQtXml.so.4 libQtScript.so.4
QT += designer

INCLUDEPATH += $$PWD

HEADERS += jambiresourcebrowser.h
