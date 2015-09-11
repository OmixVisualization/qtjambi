#!/bin/bash

## TODO ##

# There should be a handler that sanitizes the arguments.
# (or just use build_javadoc.rb)

## ##

export OUTPUT_DIR="javadoc"

if [ "$1" = "" ]; then
    export CP_ARGUMENT=""
else
    export CP_ARGUMENT="-cp $1"
fi

if [ "$2" = "" ]; then
    export QTINCLUDE_ARGUMENT=""
else
    export QTINCLUDE_ARGUMENT="--qt-include-directory=$2"
fi

# TODO: this should be parsed from version.properties...
if  [ "$QTJAMBI_VERSION" = "" ]; then
     export QTJAMBI_VERSION=4.7.0-r1
fi

if [ "$QDOC" = "" ]; then
    if [ "$QTDIR" = "" ]; then
        export LOCAL_QDOC=`which qdoc3`
    else
        export LOCAL_QDOC=$QTDIR/tools/qdoc3
    fi
else
    export LOCAL_QDOC=$QDOC
fi

echo "qdoc3 found in: $LOCAL_QDOC"

export JAMBI=`echo $PWD | sed s,/scripts,,g`

echo "using jambi dir: $JAMBI"

mkdir -p "$JAMBI/$OUTPUT_DIR"

# build the jambidoc Doclet...
cd $JAMBI/tools/jambidoc
command="javac $CP_ARGUMENT -d "$JAMBI/$OUTPUT_DIR" *.java"
echo "running: $command"
eval $command
if [ $? -ne 0 ]; then
    echo "Failed to compile jambidoclet..."
    exit 1
fi


# Clean the directory
echo Cleaning up the old directory...
if [ -d $JAMBI/doc/html ]; then
    rm -Rf $JAMBI/doc/html
fi

mkdir $JAMBI/doc/html


# Generating the QDoc JAPI file
cd $JAMBI/generator
./generator --build-qdoc-japi $QTINCLUDE_ARGUMENT


# Running QDoc
cd $LOCAL_QDOC/test
../qdoc3 qt-for-jambi.qdocconf jambi.qdocconf

cd $JAMBI/doc/html/org/qtjambi/qt
jar -cf $JAMBI/scripts/qtjambi-jdoc-$QTJAMBI_VERSION.jar *.jdoc

# Generating the sourcecode
echo $PWD
cd $JAMBI/generator
./generator --jdoc-enabled --jdoc-dir ../doc/html/org/qtjambi/qt

# Creating header for HtmlDoclet

if [ "$DOCHOME" = "" ] ; then
    DOCHOME="http://doc.trolltech.com/qtjambi-$QTJAMBI_VERSION"
fi

HEADER="<table align='right'><tr><td nowrap><a target='_top' href='$DOCHOME/org/qtjambi/qt/qtjambi-index.html'>Qt Jambi Home</a></td>"
HEADER="$HEADER<td><img src='$DOCHOME/org/qtjambi/qt/images/qt-logo.png' width='32' height='32'></td></tr></table>"

# Generating the Javadoc
cd $JAMBI/doc/html

CLASSPATH=$CLASSPATH:$JAMBI/tools/jambidoc javadoc -doclet jambidoc.JambiDoclet -header "$HEADER" -J-Xmx500m -sourcepath $JAMBI org.qtjambi.qt org.qtjambi.qt.core org.qtjambi.qt.gui org.qtjambi.qt.opengl org.qtjambi.qt.sql org.qtjambi.qt.opengl org.qtjambi.qt.svg org.qtjambi.qt.network org.qtjambi.qt.xml org.qtjambi.qt.designer org.qtjambi.qt.webkit org.qtjambi.qt.phonon

find . -name "qt jambi.dcf" -exec rm {} \;
find . -name "qt jambi.index" -exec rm {} \;
find . -name "*.jdoc" -exec rm {} \;

jar -cf qtjambi-javadoc-$QTJAMBI_VERSION.jar *

cd $JAMBI/scripts
cp $JAMBI/doc/html/qtjambi-javadoc-$QTJAMBI_VERSION.jar .

export LOCAL_QDOC=
