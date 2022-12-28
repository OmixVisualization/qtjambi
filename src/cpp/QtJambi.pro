TEMPLATE = subdirs

SUBDIRS =  functionpointers QtJambi modules

QtJambi.depends = functionpointers

modules.file = modules.pro
modules.depends = QtJambi

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}
