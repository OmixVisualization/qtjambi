@echo off
setlocal EnableDelayedExpansion

set BASE_DIR=%~dp0
for /f "tokens=*" %%G in ('dir /b %BASE_DIR%\qtjambi-4*.jar') do (set QTJAMBI_JAR=%%G)
set VERSION=%QTJAMBI_JAR:~8,5%
set QT_PLUGIN_PATH=%BASE_DIR%\plugins
set BIN_PATH=%BASE_DIR%\bin
set PATH=%BIN_PATH%;%PATH%
set CLASSPATH=%BASE_DIR%\qtjambi-%VERSION%.jar;^
%BASE_DIR%\qtjambi-win32-mingw-%VERSION%.jar;^
%BASE_DIR%\qtjambi-designer-%VERSION%.jar;

REM Launch designer
%BIN_PATH%\designer

endlocal