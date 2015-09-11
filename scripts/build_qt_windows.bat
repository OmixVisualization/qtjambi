set THE_COMPILER=msvc2005
if "%PROCESSOR_ARCHITECTURE%" == "AMD64" set THE_COMPILER=msvc2005_x64
call qt_pkg_setup %THE_COMPILER%

set ORIGINAL_PATH=%PATH%

echo y | rd /s c:\tmp\qtjambi-package-builder\qt-lgpl
echo y | rd /s c:\tmp\qtjambi-package-builder\qt-commercial

md c:\tmp\qtjambi-package-builder
move lgpl c:\tmp\qtjambi-package-builder\qt-lgpl
move commercial c:\tmp\qtjambi-package-builder\qt-commercial

cd c:\tmp\qtjambi-package-builder




cd qt-lgpl
set QTDIR=%cd%
set PATH=%ORIGINAL_PATH%;%QTDIR%\bin
echo blah > LICENSE.LGPL
perl bin/syncqt
copy configure.exe configure_hack.exe
echo yes | configure_hack -opensource -nokia-developer -no-qt3support -release -shared -no-vcproj -no-dsp -D QT_JAMBI_BUILD -plugin-manifests
echo #ifdef APPLY_AWESOME_CRAXX >> src\corelib\global\qglobal.h
echo #ifndef AWESOME_CRAXX >> src\corelib\global\qglobal.h
echo #define AWESOME_CRAXX >> src\corelib\global\qglobal.h
echo #if defined(__cplusplus) >> src\corelib\global\qglobal.h
echo QT_LICENSED_MODULE(ActiveQt) >> src\corelib\global\qglobal.h
echo #endif >> src\corelib\global\qglobal.h
echo #endif >> src\corelib\global\qglobal.h
echo #endif >> src\corelib\global\qglobal.h
echo CONFIG+=force_embed_manifest >> .qmake.cache
REM Not running "cd src && nmake" simply because there is no master makefile in src, only sub makefiles
nmake sub-src sub-tools

REM Force re-qmake and make ActiveQt without QT_EDITION=OpenSource
cd src\activeqt && qmake -r QT_EDITION=Internal DEFINES+=APPLY_AWESOME_CRAXX && nmake && cd ..\..
cd src\tools\idc && qmake DEFINES+=APPLY_AWESOME_CRAXX && nmake && cd ..\..\..

REM deleting explicitly because "nmake clean" would fail when it got to examples...
del *.obj *.ilk *.pdb moc_* *.pch /s
cd ..



cd qt-commercial
set QTDIR=%cd%
set PATH=%ORIGINAL_PATH%;%QTDIR%\bin
echo blah > LICENSE
perl bin/syncqt
copy configure.exe configure_hack.exe
echo yes | configure_hack -commercial -nokia-developer -no-qt3support -release -shared -no-dsp -no-vcproj -D QT_JAMBI_BUILD -plugin-manifests
echo CONFIG+=force_embed_manifest >> .qmake.cache
nmake sub-src sub-tools
del *.obj *.ilk *.pdb moc_* *.pch /s
cd ..


