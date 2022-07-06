TARGET = QtJambi

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(qtjambi_base.pri)

SOURCES += \
    qnativepointer.cpp \
    qtjambi_containeraccess.cpp \
    qtjambi_containeraccess_iterator.cpp \
    qtjambi_containeraccess_list.cpp \
    qtjambi_containeraccess_hash.cpp \
    qtjambi_containeraccess_multihash.cpp \
    qtjambi_containeraccess_pair.cpp \
    qtjambi_containeraccess_set.cpp \
    qtjambi_functionpointer.cpp \
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
    qtjambi_containeraccess_list.h \
    qtjambi_containeraccess_map.h \
    qtjambi_containeraccess_multihash.h \
    qtjambi_containeraccess_multimap.h \
    qtjambi_containeraccess_p.h \
    qtjambi_containeraccess_pair.h \
    qtjambi_containeraccess_set.h \
    qtjambi_core.h \
    qtjambi_global.h \
    qtjambi_interfaces_p.h \
    qtjambi_metainfo.h \
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

lessThan(QT_MAJOR_VERSION, 6):{
    SOURCES += qtjambi_containeraccess_linkedlist.cpp \
               qtjambi_containeraccess_vector.cpp \
               qtjambi_containeraccess_map.cpp \
               qtjambi_containeraccess_multimap.cpp

    HEADERS += qtjambi_containeraccess_linkedlist.h \
               qtjambi_containeraccess_vector.h
}

QTJAMBI_BUILDDIR = $$PWD/../../../$$VERSION/build/
#exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/QtJambi/generated.pri)

CONFIG(debug, debug|release) {
    QTJAMBICONF = debug
}else{
    QTJAMBICONF = release
}

greaterThan(QT_MAJOR_VERSION, 5):{
    RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/qtjambi/$$QTJAMBICONF/functionpointers.qrc
}else{
    android:{
        armeabi-v7a:RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/qtjambi/$$QTJAMBICONF-armeabi-v7a/functionpointers.qrc
        arm64-v8a: RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/qtjambi/$$QTJAMBICONF-arm64-v8a/functionpointers.qrc
        x86: RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/qtjambi/$$QTJAMBICONF-x86/functionpointers.qrc
        x86_64: RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/qtjambi/$$QTJAMBICONF-x86_64/functionpointers.qrc
    }else{
        RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/qtjambi/$$QTJAMBICONF/functionpointers.qrc
    }
}

win32-msvc*: {
    PRECOMPILED_HEADER = qtjambi_pch.h
    CONFIG += precompile_header
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

android:LIBS += -landroid

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
