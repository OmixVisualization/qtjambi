module qtjambi.qt3dquick{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qml;
    requires transitive qtjambi.qt3dcore;
    exports io.qt.qt3d.core.quick;
}
