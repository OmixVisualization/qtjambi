#
#

TARGET = org_qtjambi_test

# This must set be before includes and must match the build type used with qtjambi itself
#  try the defaults below first before butchering it up to make it work.
contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release
#} else {
#    contains(QT_CONFIG, debug) {
#        # Qt was configued with both debug and release libs
#        CONFIG += debug
#    }
#    contains(QT_CONFIG, release) {
#        # Qt was configued with both debug and release libs
#        CONFIG += release
#    }
}


include(../../../../src/cpp/qtjambi/qtjambi_include.pri)
include(../build/generator/cpp/org_qtjambi_test/org_qtjambi_test.pri)

#CONFIG += dll
# CONFIG += staticlib
#CONFIG += static_and_shared

INCLUDEPATH += $$PWD $$PWD/../src

# File are included from org_qtjambi_test.pri
HEADERS +=
SOURCES +=

# Add the linkage to the implementation
QTJAMBI_IMPL_NAME = testGenImpl
CONFIG(debug, debug|release) {
    QTJAMBI_IMPL_NAME = $$member(QTJAMBI_IMPL_NAME, 0)_debuglib
}
macx: {
    LIBS += $$PWD/../build/qmake/cpp/lib$$member(QTJAMBI_IMPL_NAME, 0).jnilib
} else {
    win32 {
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../build/qmake/cpp/debug -l$$QTJAMBI_IMPL_NAME
        } else {
            LIBS += -L$$PWD/../build/qmake/cpp/release -l$$QTJAMBI_IMPL_NAME
        }
    } else {
        LIBS += -L$$PWD/../build/qmake/cpp -l$$QTJAMBI_IMPL_NAME
    }
}

DEFINES += QTJAMBI_CPP_EXTEN_DEBUG
