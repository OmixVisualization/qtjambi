TARGET = org_qtjambi_qt_webkit_widgets

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_webkit_widgets/org_qtjambi_qt_webkit_widgets.pri)

# libQtWebKit.so.4.7.4 is dependant on many, using modifiers
#   libQtCore.so.4 libQtGui.so.4 libQtNetwork.so.4 libQtXml.so.4 libQtDBus.so.4(known optional)
QT += network webkit webkitwidgets
