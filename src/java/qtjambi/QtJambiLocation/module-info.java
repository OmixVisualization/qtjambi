module qtjambi.location{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qml;
    requires transitive qtjambi.positioning;
    exports io.qt.location;
}
