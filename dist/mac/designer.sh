#!/bin/sh

if [ -e binpatch ];
then
    ./binpatch
fi

me=$(dirname $0)

if ! java -version 2>&1 | grep -q "1\.[5-9]"
then
    echo "Qt Jambi requires Java version 1.5.0 or higher to be preinstalled"
    echo "to work. If Java is installed then make sure that the 'java' executable"
    echo "is available in the PATH environment."
else
    VERSION=$(ls qtjambi-4*.jar)
    VERSION=${VERSION:8:8}
    CP=qtjambi-$VERSION.jar:qtjambi-examples-$VERSION.jar:qtjambi-designer-$VERSION.jar
    DYLD_LIBRARY_PATH=$me/lib QT_PLUGIN_PATH=$me/plugins PATH=$me/bin:$PATH CLASSPATH=$CP $me/bin/designer.app/Contents/MacOS/Designer
fi
