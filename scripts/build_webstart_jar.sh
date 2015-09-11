#!/bin/sh


BASE_DIR=$PWD
WEBSITE=http://anarki.troll.no/~gunnar/packages
PACKAGE=1.0.0-beta2


echo Preparing directory
cd /tmp
rm -rf webstart_builder
mkdir webstart_builder
cd webstart_builder


echo Getting Package
wget $WEBSITE/qtjambi-preview-linux-$PACKAGE.tar.gz


echo Extracting
tar -xzf qtjambi-preview-linux-$PACKAGE.tar.gz


echo Creating linux jar file
echo libstdc++.so.5>>qt_system_libs
cp qtjambi-preview-linux-$PACKAGE/lib/lib*.so* .
cp -R qtjambi-preview-linux-$PACKAGE/plugins plugins
rm libQtDesigner*
jar -cf qtjambi-linux.jar lib*.so* qt_system_libs

cp qtjambi-linux.jar $BASE_DIR

cd $BASE_DIR
