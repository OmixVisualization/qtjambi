TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS =   qtjambi qtjambi_core qtjambi_plugin

contains(QT_CONFIG, qtjambi-gui):                 SUBDIRS += qtjambi_gui
contains(QT_CONFIG, qtjambi-widgets):             SUBDIRS += qtjambi_widgets
contains(QT_CONFIG, qtjambi-uitools):             SUBDIRS += qtjambi_uitools
contains(QT_CONFIG, qtjambi-opengl):              SUBDIRS += qtjambi_opengl
contains(QT_CONFIG, qtjambi-openglwidgets):       SUBDIRS += qtjambi_openglwidgets
contains(QT_CONFIG, qtjambi-network):             SUBDIRS += qtjambi_network
contains(QT_CONFIG, qtjambi-qml):                 SUBDIRS += qtjambi_qml
contains(QT_CONFIG, qtjambi-qml):                 SUBDIRS += qtjambi_jarimport
contains(QT_CONFIG, qtjambi-quick):               SUBDIRS += qtjambi_quick
contains(QT_CONFIG, qtjambi-quickwidgets):        SUBDIRS += qtjambi_quick_widgets
contains(QT_CONFIG, qtjambi-sensors):             SUBDIRS += qtjambi_sensors
contains(QT_CONFIG, qtjambi-remoteobjects):       SUBDIRS += qtjambi_remoteobjects
contains(QT_CONFIG, qtjambi-texttospeech):        SUBDIRS += qtjambi_texttospeech
contains(QT_CONFIG, qtjambi-nfc):                 SUBDIRS += qtjambi_nfc
contains(QT_CONFIG, qtjambi-purchasing):          SUBDIRS += qtjambi_purchasing
contains(QT_CONFIG, qtjambi-bluetooth):           SUBDIRS += qtjambi_bluetooth
contains(QT_CONFIG, qtjambi-xml):                 SUBDIRS += qtjambi_xml
contains(QT_CONFIG, qtjambi-scxml):               SUBDIRS += qtjambi_scxml
contains(QT_CONFIG, qtjambi-statemachine):        SUBDIRS += qtjambi_statemachine
contains(QT_CONFIG, qtjambi-quickcontrols2):      SUBDIRS += qtjambi_quick_controls2
contains(QT_CONFIG, qtjambi-networkauth):         SUBDIRS += qtjambi_network_auth
contains(QT_CONFIG, qtjambi-printsupport):        SUBDIRS += qtjambi_printsupport
contains(QT_CONFIG, qtjambi-charts):              SUBDIRS += qtjambi_charts
contains(QT_CONFIG, qtjambi-virtualkeyboard):     SUBDIRS += qtjambi_virtkey

contains(QT_CONFIG, qtjambi-sql):                 SUBDIRS += qtjambi_sql
contains(QT_CONFIG, qtjambi-svg):                 SUBDIRS += qtjambi_svg
contains(QT_CONFIG, qtjambi-svgwidgets):          SUBDIRS += qtjambi_svgwidgets
contains(QT_CONFIG, qtjambi-concurrent):          SUBDIRS += qtjambi_concurrent
contains(QT_CONFIG, qtjambi-xmlpatterns):         SUBDIRS += qtjambi_xmlpatterns
contains(QT_CONFIG, qtjambi-script):              SUBDIRS += qtjambi_script
contains(QT_CONFIG, qtjambi-scripttools):         SUBDIRS += qtjambi_scripttools

contains(QT_CONFIG, qtjambi-multimediawidgets):   SUBDIRS += qtjambi_multimedia_widgets
contains(QT_CONFIG, qtjambi-multimedia):          SUBDIRS += qtjambi_multimedia
contains(QT_CONFIG, qtjambi-webengine):           SUBDIRS += qtjambi_webengine
contains(QT_CONFIG, qtjambi-webenginecore):       SUBDIRS += qtjambi_webenginecore
contains(QT_CONFIG, qtjambi-webenginewidgets):    SUBDIRS += qtjambi_webenginewidgets

contains(QT_CONFIG, qtjambi-location):            SUBDIRS += qtjambi_location
contains(QT_CONFIG, qtjambi-positioning):         SUBDIRS += qtjambi_positioning

contains(QT_CONFIG, qtjambi-dbus):                SUBDIRS += qtjambi_dbus
contains(QT_CONFIG, qtjambi-test):                SUBDIRS += qtjambi_test
contains(QT_CONFIG, qtjambi-help):                SUBDIRS += qtjambi_help
contains(QT_CONFIG, qtjambi-websockets):          SUBDIRS += qtjambi_websockets
contains(QT_CONFIG, qtjambi-webchannel):          SUBDIRS += qtjambi_webchannel
contains(QT_CONFIG, qtjambi-webview):             SUBDIRS += qtjambi_webview
contains(QT_CONFIG, qtjambi-designer):            SUBDIRS += qtjambi_designer
contains(QT_CONFIG, qtjambi-serialport):          SUBDIRS += qtjambi_serialport
contains(QT_CONFIG, qtjambi-serialbus):           SUBDIRS += qtjambi_serialbus
contains(QT_CONFIG, qtjambi-winextras):           SUBDIRS += qtjambi_winextras
contains(QT_CONFIG, qtjambi-macextras):           SUBDIRS += qtjambi_macextras
contains(QT_CONFIG, qtjambi-x11extras):           SUBDIRS += qtjambi_x11extras
contains(QT_CONFIG, qtjambi-gamepad):             SUBDIRS += qtjambi_gamepad

SUBDIRS += qtjambi_qt3d

SUBDIRS += qtjambi_launcher

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}

#DEFINES += QTJAMBI_DEBUG_TOOLS

# This gives us a top level debug/release
EXTRA_DEBUG_TARGETS =
EXTRA_RELEASE_TARGETS =
for(sub, SUBDIRS) {
sub_pro = $$sub/$${basename(sub)}.pro
!exists($$sub_pro):next()
isEqual($$list($$fromfile($$sub_pro, TEMPLATE)), lib) {
    #debug
    eval(debug-$${sub}.depends = $${sub}/$(MAKEFILE) $$EXTRA_DEBUG_TARGETS)
    eval(debug-$${sub}.commands = (cd $$sub && $(MAKE) -f $(MAKEFILE) debug))
    EXTRA_DEBUG_TARGETS += debug-$${sub}
    QMAKE_EXTRA_TARGETS += debug-$${sub}
    #release
    eval(release-$${sub}.depends = $${sub}/$(MAKEFILE) $$EXTRA_RELEASE_TARGETS)
    eval(release-$${sub}.commands = (cd $$sub && $(MAKE) -f $(MAKEFILE) release))
    EXTRA_RELEASE_TARGETS += release-$${sub}
    QMAKE_EXTRA_TARGETS += release-$${sub}
} else { #do not have a real debug target/release
    #debug
    eval(debug-$${sub}.depends = $${sub}/$(MAKEFILE) $$EXTRA_DEBUG_TARGETS)
    eval(debug-$${sub}.commands = (cd $$sub && $(MAKE) -f $(MAKEFILE) first))
    EXTRA_DEBUG_TARGETS += debug-$${sub}
    QMAKE_EXTRA_TARGETS += debug-$${sub}
    #release
    eval(release-$${sub}.depends = $${sub}/$(MAKEFILE) $$EXTRA_RELEASE_TARGETS)
    eval(release-$${sub}.commands = (cd $$sub && $(MAKE) -f $(MAKEFILE) first))
    EXTRA_RELEASE_TARGETS += release-$${sub}
    QMAKE_EXTRA_TARGETS += release-$${sub}
}
}
debug.depends = $$EXTRA_DEBUG_TARGETS
release.depends = $$EXTRA_RELEASE_TARGETS
QMAKE_EXTRA_TARGETS += debug release
