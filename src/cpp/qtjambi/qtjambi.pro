TARGET = QtJambi

VERSION = $$QT_VERSION

include(qtjambi_base.pri)

SOURCES += \
    qnativepointer.cpp \
    qtjambi_core.cpp \
    qtjambi_functions.cpp \
    qtjambi_plugin.cpp \
    qtjambi_registry.cpp \
    qtjambi_repository.cpp \
    qtjambi_typeinfo.cpp \
    qtjambifunctiontable.cpp \
    qtjambilink.cpp \
    qtjambitypemanager.cpp \
    qtjambitypemanager02.cpp \
    qtjambitypemanager03.cpp \
    qtjambitypemanager04.cpp \
    qtjambitypemanager05.cpp \
    qtjambitypemanager06.cpp \
    qtjambitypemanager07.cpp \
    qtjambitypemanager08.cpp \
    qtjambitypemanager09.cpp \
    qtjambitypemanager10.cpp \
    qtjambitypemanager11.cpp \
    qtjambitypemanager12.cpp \
    qtjambitypemanager13.cpp \
    qtjambitypemanager14.cpp \
    qtjambitypemanager15.cpp \
    qtjambitypemanager16.cpp \
    qtjambitypemanager17.cpp \
    qtjambitypemanager18.cpp \
    qtjambitypemanager19.cpp \
    qtjambitypemanager20.cpp \
    qtjambitypemanager21.cpp \
    qtjambitypemanager22.cpp \
    qtjambitypemanager23.cpp \
    qtjambitypemanager24.cpp \
    qtjambitypemanager25.cpp \
    qtjambitypemanager26.cpp \
    qtjambitypemanager27.cpp \
    qtjambitypemanager28.cpp \
    qtjambitypemanager29.cpp \
    qtjambitypemanager30.cpp \
    qtjambitypemanager31.cpp \
    qtobject.cpp \
    qvariant.cpp \
    qtdynamicmetaobject.cpp \
    qtjambivariant.cpp \
    qtjambi_thread.cpp \
    qtjambi_interfaces.cpp \
    staticmetacall1.cpp \
    staticmetacall2.cpp \
    staticmetacall3.cpp \
    qtjambi_containers.cpp \
    qtjambishell.cpp\
    qtjambidebugtools.cpp

HEADERS += \
    qtjambi_core.h \
    qtjambi_global.h \
    qtjambi_plugin.h \
    qtjambi_qml.h \
    qtjambi_registry_p.h \
    qtjambi_repodefines.h \
    qtjambi_repository.h \
    qtjambi_repository_p.h \
    qtjambi_thread_p.h \
    qtjambi_typeinfo_p.h \
    qtjambi_typetests.h \
    qtjambifunctiontable_p.h \
    qtjambitypemanager_p.h \
    qtjambivariant_p.h \
    qtjambi_thread.h \
    qtjambilink_p.h \
    qtjambi_jobjectwrapper.h \
    qtjambi_templates.h \
    qtjambi_functionpointer.h \
    qtjambi_interfaces.h \
    qtdynamicmetaobject_p.h \
    qtjambi_exceptions.h \
    qtjambi_application.h \
    qtjambi_registry.h \
    qtjambi_cast.h \
    qtjambi_containers.h \
    qtjambishell_p.h \
    qtjambi_cast_p.h \
    qtjambi_cast_list_p.h \
    qtjambi_cast_util_p.h \
    qtjambi_cast_map_p.h \
    qtjambi_cast_type_p.h \
    qtjambi_array_cast.h \
    qtjambi_array_cast_p.h \
    qtjambi_cast_arithmetic_p.h \
    qtjambi_cast_jnitype_p.h \
    qtjambi_cast_container1_p.h \
    qtjambi_cast_container2_p.h \
    qtjambi_cast_sharedpointer_p.h \
    qtjambi_cast_enum_p.h \
    qtjambidebugtools_p.h

win32:CONFIG += precompile_header
win32:PRECOMPILED_HEADER = qtjambi_core.h
msvc:QMAKE_CXXFLAGS += /bigobj

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

linux-g++*:{
    LIBS += -ldl
    QMAKE_RPATHDIR = $ORIGIN/.
}

linux-g++* | freebsd-g++* | macx | win32-g++* {
    QMAKE_CXXFLAGS += -ftemplate-depth=20000
}

DEFINES += QTJAMBI_EXPORT=

macx:CONFIG -= precompile_header
macx:{
    QMAKE_RPATHDIR = @loader_path/.
}

QT = core core-private

RESOURCES +=
