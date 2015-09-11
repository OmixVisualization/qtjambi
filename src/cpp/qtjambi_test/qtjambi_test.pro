TARGET = org_qtjambi_qt_test

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt_test/org_qtjambi_qt_test.pri)

# This looks like a bug in Qt to me
INCLUDEPATH += $$QMAKE_INCDIR_QT/QtTest
# We appear to reference qwidget.h
INCLUDEPATH += $$QMAKE_INCDIR_QT/QtGui

# libQtTest.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
# Hmmm no (not anymore?) in 4.7.4 qtestlib references qWaitForWindowShown() which
#  on linux needs qt_x11_wait_for_window_manager(QWidget*) from QtGui
#QT = core qtestlib
QT += core gui widgets testlib
