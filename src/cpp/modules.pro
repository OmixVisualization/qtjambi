TEMPLATE = subdirs

SUBDIRS = QtJambiCore

for(MOD, QTJAMBI_MODULE) : {
    exists($$member(MOD,0)/$$member(MOD,0).pro):SUBDIRS += $$MOD
}

contains(QTJAMBI_MODULE, QtJambiQml): SUBDIRS += jarimport

contains(QTJAMBI_MODULE, QtJambiAxBase): SUBDIRS += QtJambiActiveX

greaterThan(QT_MAJOR_VERSION, 6):{
    contains(QTJAMBI_MODULE, QtJambiGui-private): SUBDIRS += QtJambiGuiQpa
}

equals(QT_MAJOR_VERSION, 6):greaterThan(QT_MINOR_VERSION, 5):{
    contains(QTJAMBI_MODULE, QtJambiGui-private): SUBDIRS += QtJambiGuiRhi
}
greaterThan(QT_MAJOR_VERSION, 6):{
    contains(QTJAMBI_MODULE, QtJambiGui-private): SUBDIRS += QtJambiGuiRhi
}

!android:!ios:{
    contains(QTJAMBI_MODULE, QtJambiDesigner-private): SUBDIRS += QtJambiUIC
    SUBDIRS += QtJambiLauncher\
    QtJambiGenerator
    QtJambiGenerator.file = QtJambiGenerator/library.pro
}

SUBDIRS += QtJambiPlugin
