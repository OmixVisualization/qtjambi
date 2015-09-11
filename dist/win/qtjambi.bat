@echo off

set OLD_PATH=%PATH%
set OLD_PLUGIN_PATH=%QT_PLUGIN_PATH%
set OLD_CLASSPATH=%CLASSPATH%

set QT_PLUGIN_PATH=%cd%\plugins
set PATH=%cd%\bin;%PATH%

dir /b qtjambi-4*.jar > .tmp
set /p QTJAMBI_JAR=< .tmp
del .tmp
set VERSION=%QTJAMBI_JAR:~8,5%

set CLASSPATH=%cd%\qtjambi-%VERSION%.jar;%cd%\qtjambi-designer-%VERSION%.jar;%cd%\qtjambi-examples-%VERSION%.jar;%cd%

java org.qtjambi.launcher.Launcher

set PATH=%OLD_PATH%
set QT_PLUGIN_PATH=%OLD_PLUGIN_PATH%
set CLASSPATH=%OLD_CLASSPATH%

set OLD_PATH=
set OLD_PLUGIN_PATH=
set OLD_CLASSPATH=
set QTJAMBI_JAR=
