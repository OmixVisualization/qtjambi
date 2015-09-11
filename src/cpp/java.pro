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

contains(QT_CONFIG, xml):               SUBDIRS += qtjambi_xml
contains(QT_CONFIG, printsupport):               SUBDIRS += qtjambi_printsupport

contains(QT_CONFIG, sql):               SUBDIRS += qtjambi_sql
contains(QT_CONFIG, svg):               SUBDIRS += qtjambi_svg
contains(QT_CONFIG, concurrent):               SUBDIRS += qtjambi_concurrent
contains(QT_CONFIG, xmlpatterns):       SUBDIRS += qtjambi_xmlpatterns
contains(QT_CONFIG, opengl):            SUBDIRS += qtjambi_opengl
contains(QT_CONFIG, declarative):       SUBDIRS += qtjambi_declarative
contains(QT_CONFIG, script):            SUBDIRS += qtjambi_script
contains(QT_CONFIG, scripttools):       SUBDIRS += qtjambi_scripttools
contains(QT_CONFIG, multimedia):        SUBDIRS += qtjambi_multimedia
contains(QT_CONFIG, multimediawidgets):        SUBDIRS += qtjambi_multimedia_widgets
contains(QT_CONFIG, dbus):              SUBDIRS += qtjambi_dbus
contains(QT_CONFIG, testlib):			SUBDIRS += qtjambi_test
contains(QT_CONFIG, help):				SUBDIRS += qtjambi_help
contains(QT_CONFIG, webkit):            SUBDIRS += qtjambi_webkit
contains(QT_CONFIG, webkitwidgets):            SUBDIRS += qtjambi_webkit_widgets
contains(QT_CONFIG, designer):          SUBDIRS += qtjambi_designer
contains(QT_CONFIG, designer):          SUBDIRS += designer-integration
contains(QT_CONFIG, serialport):            SUBDIRS += qtjambi_serialport
contains(QT_CONFIG, winextras):            SUBDIRS += qtjambi_winextras
contains(QT_CONFIG, macextras):            SUBDIRS += qtjambi_macextras
contains(QT_CONFIG, x11extras):            SUBDIRS += qtjambi_x11extras

contains(QT_CONFIG, 3dcore):            SUBDIRS += qtjambi_3dcore
contains(QT_CONFIG, 3drenderer):            SUBDIRS += qtjambi_3drenderer
contains(QT_CONFIG, 3dcore):            SUBDIRS += qtjambi_3dinput
contains(QT_CONFIG, 3dquick):            SUBDIRS += qtjambi_3dquick
contains(QT_CONFIG, 3dquickrenderer):            SUBDIRS += qtjambi_3dquickrenderer

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
