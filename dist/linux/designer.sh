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
    VERSION=`sed '/^\#/d' version.properties | grep 'qtjambi.version'  | tail -n 1 | sed 's/^.*=//;s/^[[:space:]]*//;s/[[:space:]]*$//'`
    CP=$me/qtjambi-$VERSION.jar:$me/qtjambi-examples-$VERSION.jar:$me/qtjambi-designer-$VERSION.jar
    LD_LIBRARY_PATH=$me/lib QT_PLUGIN_PATH=$me/plugins CLASSPATH=$CP $me/bin/designer
fi
