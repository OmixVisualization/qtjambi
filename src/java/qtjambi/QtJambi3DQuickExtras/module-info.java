module qtjambi.qt3dquickextras{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qt3dcore;
    requires transitive qtjambi.qt3dquick;
    exports io.qt.qt3d.extras.quick;
}
