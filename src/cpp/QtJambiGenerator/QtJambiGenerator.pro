TEMPLATE = subdirs

SUBDIRS = library executable

library.file = library.pro
executable.file = executable.pro
executable.depends = library

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}
