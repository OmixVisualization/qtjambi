module qtjambi{
    requires java.base;
    requires transitive java.logging;
    requires transitive java.xml;
    opens io.qt.internal to qtjambi.autotest;
    exports io.qt;
    exports io.qt.core;
    exports io.qt.core.internal;
    exports io.qt.gui;
    exports io.qt.gui.gl;
    exports io.qt.widgets;
    exports io.qt.internal to qtjambi.network,
                              qtjambi.dbus,
                              qtjambi.sql,
                              qtjambi.scxml,
                              qtjambi.quick,
                              qtjambi.webenginecore,
                              qtjambi.xmlpatterns,
                              qtjambi.serialport,
                              qtjambi.script,
                              qtjambi.websockets,
                              qtjambi.core,
                              qtjambi.multimedia,
                              qtjambi.quickcontrols2,
                              qtjambi.test,
                              qtjambi.macextras,
                              qtjambi.webchannel,
                              qtjambi.x11extras,
                              qtjambi.webenginewidgets,
                              qtjambi.scripttools,
                              qtjambi.help,
                              qtjambi.networkauth,
                              qtjambi.gamepad,
                              qtjambi.texttospeech,
                              qtjambi.qt3dinput,
                              qtjambi.location,
                              qtjambi.nfc,
                              qtjambi.winextras,
                              qtjambi.qml,
                              qtjambi.positioning,
                              qtjambi.serialbus,
                              qtjambi.printsupport,
                              qtjambi.concurrent,
                              qtjambi.svg,
                              qtjambi.bluetooth,
                              qtjambi.remoteobjects,
                              qtjambi.purchasing,
                              qtjambi.sensors,
                              qtjambi.quickwidgets,
                              qtjambi.webview,
                              qtjambi.xml,
                              qtjambi.webengine,
                              qtjambi.qt3dcore,
                              qtjambi.qt3drender,
                              qtjambi.qt3dextras,
                              qtjambi.qt3dquickextras,
                              qtjambi.qt3dquick,
                              qtjambi.qt3dlogic,
                              qtjambi.qt3danimation,
                              qtjambi.qt3dquickscene2d;
}
