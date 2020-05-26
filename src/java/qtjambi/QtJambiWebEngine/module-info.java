module qtjambi.webengine{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qml;
    requires transitive qtjambi.webenginecore;
    exports io.qt.webengine;
}
