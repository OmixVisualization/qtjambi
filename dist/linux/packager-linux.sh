#/bin/bash

echo "This version is currently broken! It shouldn't even be applied to master repository!"
exit 1

if [ "$2" == "" ] ; then
	echo "You need to give LIBDIR and BINDIR!"
	exit 1
fi

LIBDIR="$1"
BINDIR="$2"

if [ "$3" == "--phonon-dir" ] ; then
	phononlib="$4"
else
	phononlib=$LIBDIR
fi

if [ "$5" == "--version" ] ; then
	VERSION="$6"
else
	VERSION=`cat version.properties | cut -d= -f2 | tr -d ' '`
fi


OS=linux32
RELEASE=qtjambi-$OS-community-$VERSION
COMPILER=gcc

mkdir $RELEASE
mkdir $RELEASE/bin
mkdir $RELEASE/lib

cp version.properties $RELEASE
cp qtjambi-$VERSION.jar $RELEASE
cp qtjambi-designer-$VERSION.jar $RELEASE
cp qtjambi-examples-$VERSION.jar $RELEASE
cp qtjambi-util-$VERSION.jar $RELEASE
cp qtjambi-$OS-$COMPILER-$VERSION.jar $RELEASE
    
cp dist/linux/qtjambi.sh $RELEASE
cp dist/linux/designer.sh $RELEASE

cp dist/changes-$VERSION $RELEASE 
cp dist/install.html $RELEASE
cp dist/LICENSE.GPL3 $RELEASE
cp dist/LICENSE.LGPL $RELEASE
cp dist/readme.html $RELEASE
  
cp -R java/src/qtjambi-examples/com $RELEASE
  
cp bin/juic $RELEASE/binbcp $BIND
IR/lrelease $RELEASE/bin
cp $BINDIR/lrelease $RELEASE/bin
cp $BINDIR/lupdate $RELEASE/bin
cp $QTDIR/bin/designer $RELEASE/bin

cp lib/libqtjambi.so $RELEASE/lib
cp lib/libqtjambi.so.1 $RELEASE/lib
cp $phononlib/libphonon.so.4 $RELEASE/lib
cp $LIBDIR/libQtCore.so.4 $RELEASE/lib
cp $LIBDIR/libQtGui.so.4 $RELEASE/lib
cp $QTDIR/lib/libQtHelp.so.4 $RELEASE/lib
cp $QTDIR/lib/libQtMultimedia.so.4 $RELEASE/lib
cp $LIBDIR/libQtSql.so.4 $RELEASE/lib
cp $LIBDIR/libQtScript.so.4 $RELEASE/lib
cp $LIBDIR/libQtSvg.so.4 $RELEASE/lib
cp $LIBDIR/libQtWebKit.so.4 $RELEASE/lib
cp $QTDIR/lib/libQtScriptTools.so.4 $RELEASE/lib
cp $LIBDIR/libQtXmlPatterns.so.4 $RELEASE/lib
cp $LIBDIR/libQtDesignerComponents.so.4 $RELEASE/lib
cp $LIBDIR/libQtDesigner.so.4 $RELEASE/lib
  
cp lib/liborg_qtjambi_qt_core.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_gui.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_help.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_multimedia.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_network.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_opengl.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_phonon.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_sql.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_script.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_scripttools.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_svg.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_webkit.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_xml.so $RELEASE/lib
cp lib/liborg_qtjambi_qt_xmlpatterns.so $RELEASE/lib
cp lib/liborg_qtjambi_tools_designer.so $RELEASE/lib

cp -R $LIBDIR/plugins $RELEASE
cp -R plugins/designer/ $RELEASE/plugins
cp -R plugins/qtjambi $RELEASE/plugins
  

tar czf $RELEASE.tar.gz $RELEASE

rm -Rf $RELEASE
