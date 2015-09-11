#/bin/bash


OS=macosx
VERSION=`cat version.properties | cut -d= -f2 | tr -d ' '`
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

cp dist/mac/qtjambi.sh $RELEASE
cp dist/mac/designer.sh $RELEASE
cp dist/set_qtjambi_env.sh $RELEASE

cp dist/changes-$VERSION $RELEASE
cp dist/install.html $RELEASE
cp dist/LGPL_EXCEPTION.TXT $RELEASE
cp dist/LICENSE.GPL3 $RELEASE
cp dist/LICENSE.LGPL $RELEASE
cp dist/readme.html $RELEASE
  
cp -R java/src/qtjambi-examples/com $RELEASE
  
cp bin/juic $RELEASE/bin
cp -R $QTDIR/bin/Designer.app $RELEASE/bin
cp -R $QTDIR/bin/Linguist.app $RELEASE/bin
cp $QTDIR/bin/lrelease $RELEASE/bin
cp $QTDIR/bin/lupdate $RELEASE/bin

cp lib/libqtjambi.jnilib $RELEASE/lib

cp $QTDIR/lib/libphonon.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtCore.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtGui.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtHelp.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtMultimedia.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtNetwork.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtOpenGL.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtSql.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtScript.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtScriptTools.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtSvg.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtWebKit.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtXml.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtXmlPatterns.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtDesignerComponents.4.dylib $RELEASE/lib
cp $QTDIR/lib/libQtDesigner.4.dylib $RELEASE/lib

cp lib/liborg_qtjambi_qt_core.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_gui.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_help.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_multimedia.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_network.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_opengl.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_phonon.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_sql.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_script.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_scripttools.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_svg.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_webkit.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_xml.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_qt_xmlpatterns.jnilib $RELEASE/lib
cp lib/liborg_qtjambi_tools_designer.jnilib $RELEASE/lib

cp -R $QTDIR/plugins $RELEASE
cp -R plugins/designer/ $RELEASE/plugins
cp -R plugins/qtjambi $RELEASE/plugins


tar czf $RELEASE.tar.gz $RELEASE

rm -Rf $RELEASE