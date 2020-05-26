module qtjambi.networkauth{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.network;
    exports io.qt.network.auth;
}
