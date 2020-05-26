module qtjambi.scripttools{
    requires java.base;
    requires transitive qtjambi;
    requires transitive qtjambi.script;
    exports io.qt.script.tools;
}
