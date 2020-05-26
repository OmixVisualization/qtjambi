TEMPLATE = subdirs
CONFIG += ordered


SUBDIRS =   qtjambi \
            qtjambi_core \
            qtjambi_gui \
            qtjambi_widgets \
            qtjambi_network \
            qtjambi_qml \
            qtjambi_quick \
            qtjambi_quick_widgets

contains(QT_CONFIG, sensors):             SUBDIRS += qtjambi_sensors
contains(QT_CONFIG, remoteobjects):       SUBDIRS += qtjambi_remoteobjects
contains(QT_CONFIG, texttospeech):        SUBDIRS += qtjambi_texttospeech
contains(QT_CONFIG, nfc):                 SUBDIRS += qtjambi_nfc
contains(QT_CONFIG, purchasing):          SUBDIRS += qtjambi_purchasing
contains(QT_CONFIG, bluetooth):           SUBDIRS += qtjambi_bluetooth
contains(QT_CONFIG, xml):                 SUBDIRS += qtjambi_xml
contains(QT_CONFIG, scxml):               SUBDIRS += qtjambi_scxml
contains(QT_CONFIG, quickcontrols2):      SUBDIRS += qtjambi_quick_controls2
contains(QT_CONFIG, networkauth):         SUBDIRS += qtjambi_network_auth
contains(QT_CONFIG, printsupport):        SUBDIRS += qtjambi_printsupport

contains(QT_CONFIG, sql):                 SUBDIRS += qtjambi_sql
contains(QT_CONFIG, svg):                 SUBDIRS += qtjambi_svg
contains(QT_CONFIG, concurrent):          SUBDIRS += qtjambi_concurrent
contains(QT_CONFIG, xmlpatterns):         SUBDIRS += qtjambi_xmlpatterns
#contains(QT_CONFIG, opengl):             SUBDIRS += qtjambi_opengl
contains(QT_CONFIG, script):              SUBDIRS += qtjambi_script
contains(QT_CONFIG, scripttools):         SUBDIRS += qtjambi_scripttools

contains(QT_CONFIG, multimediawidgets):   SUBDIRS += qtjambi_multimedia_widgets
contains(QT_CONFIG, multimedia):          SUBDIRS += qtjambi_multimedia
contains(QT_CONFIG, webengine):           SUBDIRS += qtjambi_webengine
contains(QT_CONFIG, webenginecore):       SUBDIRS += qtjambi_webenginecore
contains(QT_CONFIG, webenginewidgets):    SUBDIRS += qtjambi_webenginewidgets

contains(QT_CONFIG, location):            SUBDIRS += qtjambi_location
contains(QT_CONFIG, positioning):         SUBDIRS += qtjambi_positioning

contains(QT_CONFIG, dbus):                SUBDIRS += qtjambi_dbus
contains(QT_CONFIG, testlib):             SUBDIRS += qtjambi_test
contains(QT_CONFIG, help):                SUBDIRS += qtjambi_help
contains(QT_CONFIG, websockets):          SUBDIRS += qtjambi_websockets
contains(QT_CONFIG, webchannel):          SUBDIRS += qtjambi_webchannel
contains(QT_CONFIG, webview):             SUBDIRS += qtjambi_webview
#contains(QT_CONFIG, designer):           SUBDIRS += qtjambi_designer
#contains(QT_CONFIG, designer):           SUBDIRS += designer-integration
contains(QT_CONFIG, serialport):          SUBDIRS += qtjambi_serialport
contains(QT_CONFIG, serialbus):           SUBDIRS += qtjambi_serialbus
contains(QT_CONFIG, winextras):           SUBDIRS += qtjambi_winextras
contains(QT_CONFIG, macextras):           SUBDIRS += qtjambi_macextras
contains(QT_CONFIG, x11extras):           SUBDIRS += qtjambi_x11extras
contains(QT_CONFIG, gamepad):             SUBDIRS += qtjambi_gamepad

SUBDIRS += qtjambi_qt3d

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
