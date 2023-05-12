include(configure.pri)

SOURCES += \
    applicationdata.cpp \
    cast.cpp \
    containeraccess.cpp \
    containeraccess_hash.cpp \
    containeraccess_iterator.cpp \
    containeraccess_list.cpp \
    containeraccess_multihash.cpp \
    containeraccess_pair.cpp \
    containeraccess_set.cpp \
    containers.cpp \
    convert.cpp \
    coreapi.cpp \
    debug.cpp \
    enums.cpp \
    events.cpp \
    exception.cpp \
    fileengine.cpp \
    functionalbase.cpp \
    functionpointer.cpp \
    future.cpp \
    java.cpp \
    javaarrays.cpp \
    javabuffers.cpp \
    javainvalidate.cpp \
    javastrings.cpp \
    jna.cpp \
    jni_classanalyser.cpp \
    jni_deployer.cpp \
    jni_enum.cpp \
    jni_metaobjecttools.cpp \
    jni_metatype.cpp \
    jni_native.cpp \
    jni_nativepointer.cpp \
    jni_reflection.cpp \
    jni_signals.cpp \
    jni_test.cpp \
    jni_utilities.cpp \
    jnienvironment.cpp \
    jobjectwrapper.cpp \
    messagehandler.cpp \
    metainfo.cpp \
    objectdata.cpp \
    paint.cpp \
    plugin.cpp \
    qmlapi.cpp \
    qtjambiapi.cpp \
    qtjambilink.cpp \
    qtjambimetaobject.cpp \
    qtjambishell.cpp \
    registry.cpp \
    scope.cpp \
    setup.cpp \
    supertypeinfo.cpp \
    thread.cpp \
    typeentry.cpp \
    typemanager.cpp \
    utils.cpp

HEADERS += \
    containeraccess.h \
    containeraccess_hash.h \
    containeraccess_list.h \
    containeraccess_map.h \
    containeraccess_multihash.h \
    containeraccess_multimap.h \
    containeraccess_p.h \
    containeraccess_pair.h \
    containeraccess_set.h \
    containerapi.h \
    coreapi.h \
    debugapi.h \
    exception.h \
    functionalbase.h \
    functionpointer.h \
    global.h \
    guiapi.h \
    java_p.h \
    javaapi.h \
    javaarrays.h \
    javabuffers.h \
    javainvalidate.h \
    javastrings.h \
    javautils.h \
    jnienvironment.h \
    jobjectwrapper.h \
    metainfo.h \
    paint.h \
    qmlapi.h \
    qtjambi_cast_array.h \
    qtjambi_cast_impl.h \
    qtjambi_cast_impl_arithmetic.h \
    qtjambi_cast_impl_array.h \
    qtjambi_cast_impl_container_associative.h \
    qtjambi_cast_impl_container_iterator.h \
    qtjambi_cast_impl_container_sequential.h \
    qtjambi_cast_impl_enum.h \
    qtjambi_cast_impl_jnitype.h \
    qtjambi_cast_impl_smartpointer.h \
    qtjambi_cast_impl_template1.h \
    qtjambi_cast_impl_template2.h \
    qtjambi_cast_impl_template3.h \
    qtjambi_cast_impl_template4.h \
    qtjambi_cast_impl_template5.h \
    qtjambi_cast_impl_dbus.h \
    qtjambi_cast_impl_util.h \
    qtjambiapi.h \
    qtjambimetaobject_p.h \
    qtjambilink_p.h \
    qtjambi_cast.h \
    qtjambishell.h \
    qtjambishell_p.h \
    registryapi.h \
    registryutil_p.h \
    scope.h \
    supertypeinfo_p.h \
    testapi.h \
    threadapi.h \
    threadutils_p.h \
    typeentry_p.h \
    typemanager_p.h \
    typetests.h \
    utils.h \
    utils_p.h

lessThan(QT_MAJOR_VERSION, 6):{
SOURCES += \
    containeraccess_linkedlist.cpp \
    containeraccess_map.cpp \
    containeraccess_multimap.cpp \
    containeraccess_vector.cpp \
    variant.cpp

HEADERS += \
    containeraccess_linkedlist.h \
    containeraccess_vector.h
}

CONFIG(debug, debug|release) {
    QTJAMBICONF = debug
}else{
    QTJAMBICONF = release
}

greaterThan(QT_MAJOR_VERSION, 5):{
    RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/QtJambi/functionpointers-$$member(QTJAMBICONF,0).qrc
}else{
    android:{
        armeabi-v7a:RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/QtJambi/functionpointers-$$member(QTJAMBICONF,0)-armeabi-v7a.qrc
        arm64-v8a: RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/QtJambi/functionpointers-$$member(QTJAMBICONF,0)-arm64-v8a.qrc
        x86: RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/QtJambi/functionpointers-$$member(QTJAMBICONF,0)-x86.qrc
        x86_64: RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/QtJambi/functionpointers-$$member(QTJAMBICONF,0)-x86_64.qrc
    }else{
        RESOURCES = $$QTJAMBI_PLATFORM_BUILDDIR/QtJambi/functionpointers-$$member(QTJAMBICONF,0).qrc
    }
}

!ios:{
    PRECOMPILED_HEADER = qtjambi_pch.h
    CONFIG += precompile_header
}

win32-msvc*: {
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

linux-g++* | freebsd-g++* | macx | ios | android | win32-g++* {
    QMAKE_CXXFLAGS += -ftemplate-depth=20000
}

linux-g++* | freebsd-g++* | macx | ios | win32-g++* {
QMAKE_CXXFLAGS += -Wall -fexceptions -fnon-call-exceptions
}

DEFINES += QTJAMBI_EXPORT=

INSTALL_HEADERS.path = $$QTJAMBI_PLATFORM_BUILDDIR/include/QtJambi
ALLFILES = $$files(*)
for(file,ALLFILES){
    equals(file, $$section(file, ., 0, 0)):{
        INSTALL_HEADERS.files += $$PWD/$$file
    }
    contains(HEADERS, $$file): !equals(file, $$section(file, _p.h, 0, 0)_p.h):{
        INSTALL_HEADERS.files += $$PWD/$$file
    }
}
GENERATED = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/QtJambi/QtJambiDepends)
exists($$GENERATED): INSTALL_HEADERS.files += $$GENERATED
GENERATED = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/QtJambi/QtJambiVersion)
exists($$GENERATED): INSTALL_HEADERS.files += $$GENERATED
GENERATED = $$clean_path($$PWD/../../../$$VERSION/build/generator/cpp/QtJambi/version.h)
exists($$GENERATED): INSTALL_HEADERS.files += $$GENERATED

macx:{
    contains(CONFIG, lib_bundle):{
        INSTALL_HEADERS.path = Headers/QtJambi
        INSTALL_HEADERS.version = Versions
        QMAKE_BUNDLE_DATA += INSTALL_HEADERS
    }
}else{
    INSTALLS += INSTALL_HEADERS
}

QT = core core-private
CONFIG+=no_private_qt_headers_warning

RESOURCES +=
