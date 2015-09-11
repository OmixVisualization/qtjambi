TARGET = org_qtjambi_qt_webkit

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_webkit/org_qtjambi_qt_webkit.pri)

# We manually add these include paths, instead of using "QT += module" which is bad for us
#  as it created unnecessary hardwired linkage to libraries we may never reference any symbols from.
# To be able to remove this we need to make generator better/smarter about #include directives
#  and it might not include these files at all.
INCLUDEPATH += $$(QTDIR)/include/QtNetwork
INCLUDEPATH += $$(QTDIR)/include/QtWebKitWidgets

# libQtWebKit.so.4.7.4 is dependant on many, using modifiers
#   libQtCore.so.4 libQtGui.so.4 libQtNetwork.so.4 libQtXml.so.4 libQtDBus.so.4(known optional)
QT += network webkit
