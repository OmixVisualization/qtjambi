module qtjambi.deployer {
    requires java.base;
    requires java.xml;
    requires transitive qtjambi;
    exports io.qt.qtjambi.deployer to qtjambi.autotest;
}
