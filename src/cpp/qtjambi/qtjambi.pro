TARGET = QtJambi

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(qtjambi_base.pri)

SOURCES += \
    qnativepointer.cpp \
    qtjambi_containeraccess.cpp \
    qtjambi_containeraccess_hash1.cpp \
    qtjambi_containeraccess_linkedlist.cpp \
    qtjambi_containeraccess_list.cpp \
    qtjambi_containeraccess_map1.cpp \
    qtjambi_containeraccess_multihash1.cpp \
    qtjambi_containeraccess_multimap1.cpp \
    qtjambi_containeraccess_pair.cpp \
    qtjambi_containeraccess_set.cpp \
    qtjambi_containeraccess_vector.cpp \
    qtjambi_future.cpp \
    qtjambi_util.cpp \
    qtjambi_core.cpp \
    qtjambi_functions.cpp \
    qtjambi_jobjectwrapper.cpp \
    qtjambi_plugin.cpp \
    qtjambi_registry.cpp \
    qtjambi_repository.cpp \
    qtjambi_typeinfo.cpp \
    qtjambifunctiontable.cpp \
    qtjambilink.cpp \
    qtjambimetaobject.cpp \
    qtjambisignals.cpp \
    qtjambitypemanager.cpp \
    qtjambivariant.cpp \
    qtjambi_thread.cpp \
    qtjambi_interfaces.cpp \
    qtjambi_containers.cpp \
    qtjambishell.cpp \
    qtjambidebugtools.cpp


HEADERS += \
    qtjambi_cast_container1_util_p.h \
    qtjambi_cast_container2_util_p.h \
    qtjambi_cast_container3_p.h \
    qtjambi_cast_container4_p.h \
    qtjambi_cast_container5_p.h \
    qtjambi_cast_container_util_p.h \
    qtjambi_containeraccess.h \
    qtjambi_containeraccess_hash.h \
    qtjambi_containeraccess_linkedlist.h \
    qtjambi_containeraccess_list.h \
    qtjambi_containeraccess_map.h \
    qtjambi_containeraccess_multihash.h \
    qtjambi_containeraccess_multimap.h \
    qtjambi_containeraccess_p.h \
    qtjambi_containeraccess_pair.h \
    qtjambi_containeraccess_set.h \
    qtjambi_containeraccess_vector.h \
    qtjambi_core.h \
    qtjambi_global.h \
    qtjambi_interfaces_p.h \
    qtjambi_internal.h \
    qtjambi_nativeinterface.h \
    qtjambi_plugin.h \
    qtjambi_qml.h \
    qtjambi_registry_p.h \
    qtjambi_repodefines.h \
    qtjambi_repository.h \
    qtjambi_repository_p.h \
    qtjambi_thread_p.h \
    qtjambi_typeinfo_p.h \
    qtjambi_typetests.h \
    qtjambi_utils.h \
    qtjambi_utils_p.h \
    qtjambifunctiontable_p.h \
    qtjambimetaobject_p.h \
    qtjambitypemanager_p.h \
    qtjambivariant_p.h \
    qtjambi_thread.h \
    qtjambilink_p.h \
    qtjambi_jobjectwrapper.h \
    qtjambi_templates.h \
    qtjambi_functionpointer.h \
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
