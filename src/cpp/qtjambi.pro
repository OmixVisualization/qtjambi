TEMPLATE = subdirs

SUBDIRS =  qtjambi qtjambi_modules

qtjambi_modules.file = qtjambi_modules.pri
qtjambi_modules.depends = qtjambi

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}
