###################################################################################################
##
## Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
##
## This file is part of Qt Jambi.
##
## $BEGIN_LICENSE$
##
## GNU Lesser General Public License Usage
## This file may be used under the terms of the GNU Lesser
## General Public License version 2.1 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU Lesser General Public License version 2.1 requirements
## will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3.0 as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU General Public License version 3.0 requirements will be
## met: http://www.gnu.org/copyleft/gpl.html.
##
## $END_LICENSE$
##
## This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
## WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###################################################################################################

include(configure.pri)

SOURCES += \
    applicationdata.cpp \
    cast.cpp \
    containeraccess.cpp \
    containeraccess_hash.cpp \
    containeraccess_iterator.cpp \
    containeraccess_list.cpp \
    containeraccess_multihash.cpp \
    containeraccess_optional.cpp \
    containeraccess_pair.cpp \
    containeraccess_set.cpp \
    containeraccess_span.cpp \
    containeraccess_map.cpp \
    containeraccess_multimap.cpp \
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
    modelapi.cpp \
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
    QtJambiArrayAPI \
    androidapi.h \
    containeraccess.h \
    containeraccess_associative.h \
    containeraccess_p.h \
    containeraccess_sequential.h \
    containerapi.h \
    containerutils.h \
    coreapi.h \
    debugapi.h \
    exception.h \
    functionalbase.h \
    functionpointer.h \
    futureapi.h \
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
    modelapi.h \
    objectdata.h \
    paint.h \
    qmlapi.h \
    qtjambi_cast_arithmetic.h \
    qtjambi_cast_array.h \
    qtjambi_cast_buffer.h \
    qtjambi_cast_container.h \
    qtjambi_cast_enum.h \
    qtjambi_cast_future.h \
    qtjambi_cast_iterator.h \
    qtjambi_cast_model.h \
    qtjambi_cast_object.h \
    qtjambi_cast_smartpointer.h \
    qtjambi_cast_template1.h \
    qtjambi_cast_template2.h \
    qtjambi_cast_template3.h \
    qtjambi_cast_template4.h \
    qtjambi_cast_template5.h \
    qtjambi_cast_time.h \
    qtjambi_cast_util.h \
    qtjambiapi.h \
    qtjambiapi_array.h \
    qtjambiapi_boxed.h \
    qtjambiapi_construct.h \
    qtjambiapi_container.h \
    qtjambiapi_convert.h \
    qtjambiapi_iterator.h \
    qtjambiapi_model.h \
    qtjambiapi_name.h \
    qtjambiapi_nativeid.h \
    qtjambiapi_optional.h \
    qtjambiapi_ownership.h \
    qtjambiapi_smartpointer.h \
    qtjambiapi_string.h \
    qtjambiapi_thread.h \
    qtjambiapi_time.h \
    qtjambiapi_variant.h \
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

defined(QTJAMBI_GENERIC_ACCESS):{
HEADERS += \
    containeraccess_hash.h \
    containeraccess_list.h \
    containeraccess_map.h \
    containeraccess_multihash.h \
    containeraccess_multimap.h \
    containeraccess_pair.h \
    containeraccess_set.h
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
QMAKE_RESOURCE_FLAGS += -no-compress

win32-arm64-msvc* | win32-msvc*: {
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

!ios:{
    PRECOMPILED_HEADER = pch_p.h
    CONFIG += precompile_header
}else{
    HEADERS += pch_p.h
}

android:LIBS += -landroid

linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc*:{
    LIBS += -ldl
    QMAKE_RPATHDIR = $ORIGIN/.
}

linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc* | macx | ios | android | win32-g++* {
    QMAKE_CXXFLAGS += -ftemplate-depth=20000
}

linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc* | macx | ios | win32-g++* {
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
