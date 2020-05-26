QTJAMBILIB = QtJambiWebEngineWidgets
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

# We manually add these include paths, instead of using "QT += module" which is bad for us
#  as it created unnecessary hardwired linkage to libraries we may never reference any symbols from.
# To be able to remove this we need to make generator better/smarter about #include directives
#  and it might not include these files at all.

QT += core qml quick widgets webengine webenginecore webenginewidgets

HEADERS += \
    qtjambi_webenginewidgets_hashes.h \
    qtjambi_webenginewidgets_repository.h

SOURCES += \
    qtjambi_webenginewidgets_repository.cpp

