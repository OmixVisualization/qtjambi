module qtjambi.quickwidgets{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.qml;
    requires transitive qtjambi.quick;
    exports io.qt.quick.widgets;
}
