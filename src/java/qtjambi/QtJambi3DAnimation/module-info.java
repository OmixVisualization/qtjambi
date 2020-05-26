module qtjambi.qt3danimation{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qt3dcore;
    requires transitive qtjambi.qt3drender;
    exports io.qt.qt3d.animation;
}
