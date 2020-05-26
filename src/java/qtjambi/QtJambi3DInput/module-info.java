module qtjambi.qt3dinput{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qt3dcore;
    exports io.qt.qt3d.input;
}
