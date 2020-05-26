module qtjambi.serialbus{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.network;
    exports io.qt.serialbus;
}
