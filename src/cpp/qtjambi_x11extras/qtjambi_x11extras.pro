TARGET = org_qtjambi_qt_x11extras

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt_x11extras/org_qtjambi_qt_x11extras.pri)

# We manually add these include paths, instead of using "QT += module" which is bad for us
#  as it created unnecessary hardwired linkage to libraries we may never reference any symbols from.
# To be able to remove this we need to make generator better/smarter about #include directives
#  and it might not include these files at all.
INCLUDEPATH += $$(QTDIR)/include/QtX11Extras

QT += core x11extras
