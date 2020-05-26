module qtjambi.qt3dquickscene2d{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qml;
    requires transitive qtjambi.quick;
    requires transitive qtjambi.qt3dcore;
    requires transitive qtjambi.qt3drender;
    exports io.qt.qt3d.render.quick.scene2d;
}
