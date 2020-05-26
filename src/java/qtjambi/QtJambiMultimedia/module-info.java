module qtjambi.multimedia{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.network;
    exports io.qt.multimedia;
    exports io.qt.multimedia.widgets;
}
