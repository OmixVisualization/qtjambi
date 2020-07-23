module qtjambi.qml{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.network;
    exports io.qt.qml;
    exports io.qt.qml.util;
}
