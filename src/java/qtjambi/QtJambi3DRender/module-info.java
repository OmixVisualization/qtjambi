module qtjambi.qt3drender{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qt3dcore;
    exports io.qt.qt3d.render;
}
