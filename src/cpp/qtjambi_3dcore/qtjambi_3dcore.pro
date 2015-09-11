TARGET = org_qtjambi_qt3d_core

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt3d_core/org_qtjambi_qt3d_core.pri)

# We manually add these include paths, instead of using "QT += module" which is bad for us
#  as it created unnecessary hardwired linkage to libraries we may never reference any symbols from.
# To be able to remove this we need to make generator better/smarter about #include directives
#  and it might not include these files at all.
INCLUDEPATH += $$(QTDIR)/include/Qt3DCore

QT += core gui 3dcore

DEPENDPATH += $$(QTDIR)/include/Qt3DCore/$$QT_VERSION
DEPENDPATH += $$(QTDIR)/include/Qt3DCore/$$QT_VERSION/Qt3DCore
DEPENDPATH += $$(QTDIR)/include/Qt3DCore/$$QT_VERSION/Qt3DCore/private
DEPENDPATH += $$PWD

INCLUDEPATH += $$(QTDIR)/include/Qt3DCore/$$QT_VERSION
INCLUDEPATH += $$(QTDIR)/include/Qt3DCore/$$QT_VERSION/Qt3DCore
INCLUDEPATH += $$(QTDIR)/include/Qt3DCore/$$QT_VERSION/Qt3DCore/private
INCLUDEPATH += $$PWD
