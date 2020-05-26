module qtjambi.sql{
    requires java.base;
    requires java.sql;
    requires transitive qtjambi;
    exports io.qt.sql;
}
