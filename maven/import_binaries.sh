#!/bin/bash

source env_releases
source functions

for i in resources-linux32 resources-linux64 resources-win32 resources-win64; do
	mkdir -p ${i}/src/main/resources
done

for i in bin/juic bin/lupdate bin/lrelease lib/libQtCore.so.4 lib/libQtXml.so.4; do
	cp ${QTJAMBI_LINUX32_RELEASE}/${i} resources-linux32/src/main/resources || die
done

for i in bin/juic bin/lupdate bin/lrelease lib/libQtCore.so.4 lib/libQtXml.so.4; do
	cp ${QTJAMBI_LINUX64_RELEASE}/${i} resources-linux64/src/main/resources || die
done

for i in bin/juic.exe bin/lupdate.exe bin/lrelease.exe bin/QtCore4.dll bin/QtXml4.dll; do
	cp ${QTJAMBI_WIN32_RELEASE}/${i} resources-win32/src/main/resources || die
done

for i in bin/juic.exe bin/lupdate.exe bin/lrelease.exe bin/QtCore4.dll bin/QtXml4.dll; do
	cp ${QTJAMBI_WIN64_RELEASE}/${i} resources-win64/src/main/resources || die
done

for i in bin/juic bin/lupdate bin/lrelease lib/libQtCore.4.dylib lib/libQtXml.4.dylib; do
	echo
#	cp ${QTJAMBI_MAC32_RELEASE}/${i} resources-mac32/src/main/resources || die
done



