TEMPLATE = subdirs
CONFIG += ordered

contains(QT_CONFIG, qtjambi-qt3dcore):              SUBDIRS += qtjambi_3dcore
contains(QT_CONFIG, qtjambi-qt3drender):            SUBDIRS += qtjambi_3drender
contains(QT_CONFIG, qtjambi-qt3dinput):             SUBDIRS += qtjambi_3dinput
contains(QT_CONFIG, qtjambi-qt3dquick):             SUBDIRS += qtjambi_3dquick
contains(QT_CONFIG, qtjambi-qt3dextras):            SUBDIRS += qtjambi_3dextras
contains(QT_CONFIG, qtjambi-qt3dquickextras):       SUBDIRS += qtjambi_3dquickextras
contains(QT_CONFIG, qtjambi-qt3danimation):         SUBDIRS += qtjambi_3danimation
contains(QT_CONFIG, qtjambi-qt3dquickscene2d):      SUBDIRS += qtjambi_3dquickscene2d
contains(QT_CONFIG, qtjambi-qt3dlogic):             SUBDIRS += qtjambi_3dlogic
contains(QT_CONFIG, qtjambi-datavisualization):   SUBDIRS += qtjambi_datavis
contains(QT_CONFIG, qtjambi-quick3d):             SUBDIRS += qtjambi_quick3d

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
