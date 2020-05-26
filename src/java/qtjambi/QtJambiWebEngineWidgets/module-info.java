module qtjambi.webenginewidgets{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.network;
    requires transitive qtjambi.printsupport;
    requires transitive qtjambi.webchannel;
    requires transitive qtjambi.webenginecore;
    requires transitive qtjambi.webengine;
    exports io.qt.webengine.widgets;
}
