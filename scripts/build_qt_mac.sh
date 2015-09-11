#!/bin/sh

chmod -R u+rx .

rm -rf /tmp/qtjambi-package-builder/qt-lgpl
rm -rf /tmp/qtjambi-package-builder/qt-commercial

mkdir /tmp/qtjambi-package-builder

mv lgpl /tmp/qtjambi-package-builder/qt-lgpl
mv commercial /tmp/qtjambi-package-builder/qt-commercial

cd /tmp/qtjambi-package-builder

cd qt-lgpl
touch LICENSE.LGPL
QTDIR=$PWD perl bin/syncqt -check-includes
./configure -opensource --confirm-license=yes -fast -universal -no-framework -no-qt3support -release -no-rpath -shared -no-dbus -prefix $PWD -sdk /Developer/SDKs/MacOSX10.4u.sdk -D QT_JAMBI_BUILD
cd src && make && make clean && cd ..
cd tools && make && make clean && cd ..
cd ..

cd qt-commercial
touch LICENSE
QTDIR=$PWD perl bin/syncqt -check-includes
./configure -commercial --confirm-license=yes -fast -universal -no-framework -no-qt3support -release -no-rpath -shared -no-dbus -prefix $PWD -sdk /Developer/SDKs/MacOSX10.4u.sdk -D QT_JAMBI_BUILD
cd src && make && make clean && cd ..
cd tools && make && make clean && cd ..
cd ..

if [ ! -e /tmp/qtjambi-package-builder/qt-lgpl ]; then return 1; fi
if [ ! -e /tmp/qtjambi-package-builder/qt-commercial ]; then return 1; fi


