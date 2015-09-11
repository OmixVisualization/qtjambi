#!/bin/sh

me=$(dirname $0)

if [ -e binpatch ];
then
    ./binpatch
    export LD_LIBRARY_PATH=$me/lib
    export QT_PLUGIN_PATH=$me/plugins
fi

if ! java -version 2>&1 | grep -q "1\.[5-9]"
then
    echo "Qt Jambi requires Java version 1.5.0 or higher to be preinstalled"
    echo "to work. If Java is installed then make sure that the 'java' executable"
    echo "is available in the PATH environment."
else
    for lib in $(ls $me/qtjambi*.jar); do
        CP=$lib:$CP
    done
    java -cp $CP org.qtjambi.launcher.Launcher
fi
