#!/bin/sh

chmod -R u+rx .

# temp path must be long enough to reserve space for RPATH later on...
rm -rf /tmp/qtjambi-package-builder/qt-lgpl
rm -rf /tmp/qtjambi-package-builder/qt-commercial

mkdir /tmp/qtjambi-package-builder

mv lgpl /tmp/qtjambi-package-builder/qt-lgpl
mv commercial /tmp/qtjambi-package-builder/qt-commercial

cd /tmp/qtjambi-package-builder

cd qt-lgpl
rm -f src/gui/kernel/qapplication_mac.cpp
rm -f src/gui/kernel/qapplication_qws.cpp
touch LICENSE.LGPL
QTDIR=$PWD perl bin/syncqt -check-includes
./configure -opensource --confirm-license=yes -no-qt3support -release -shared -prefix $PWD -no-sql-sqlite2 -no-mmx -no-3dnow -no-sse -no-sse2 -D QT_JAMBI_BUILD
cd src && make && make clean && cd ..
cd tools && make && make clean && cd ..
cd ..


cd qt-commercial
rm -f src/gui/kernel/qapplication_mac.cpp
rm -f src/gui/kernel/qapplication_qws.cpp
touch LICENSE
QTDIR=$PWD perl bin/syncqt -check-includes
./configure -commercial --confirm-license=yes -fast -no-qt3support -release -shared -prefix $PWD -no-sql-sqlite2 -no-mmx -no-3dnow -no-sse -no-sse2 -D QT_JAMBI_BUILD
cd src && make && make clean && cd ..
cd tools && make && make clean && cd ..
cd ..


if [ ! -e /tmp/qtjambi-package-builder/qt-lgpl ]; then exit 1; fi
if [ ! -e /tmp/qtjambi-package-builder/qt-commercial ]; then exit 1; fi



