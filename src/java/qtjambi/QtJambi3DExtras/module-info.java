module qtjambi.qt3dextras{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qt3dcore;
    requires transitive qtjambi.qt3drender;
    requires transitive qtjambi.qt3dinput;
    exports io.qt.qt3d.extras;
}
