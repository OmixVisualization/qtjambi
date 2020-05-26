module qtjambi.webenginecore{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.network;
    exports io.qt.webengine.core;
}
