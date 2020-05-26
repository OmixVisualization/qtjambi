module qtjambi.quick{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qml;
    exports io.qt.quick;
}
