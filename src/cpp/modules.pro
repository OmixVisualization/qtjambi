TEMPLATE = subdirs

SUBDIRS = QtJambiCore

for(MOD, QTJAMBI_MODULE) : {
    exists($$member(MOD,0)/$$member(MOD,0).pro):SUBDIRS += $$MOD
}

contains(QTJAMBI_MODULE, QtJambiQml): SUBDIRS += jarimport

contains(QTJAMBI_MODULE, QtJambiAxBase): SUBDIRS += QtJambiActiveX

!android:!ios:{
    contains(QTJAMBI_MODULE, QtJambiDesigner-private): SUBDIRS += QtJambiUIC
    SUBDIRS += QtJambiLauncher
    QtJambiGenerator
    QtJambiGenerator.file = QtJambiGenerator/library.pro
}

SUBDIRS += QtJambiPlugin
