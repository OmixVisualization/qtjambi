# Input
HEADERS += \
        classlistgenerator.h \
        cppgenerator.h \
        cppheadergenerator.h \
        cppimplgenerator.h \
        debuglog.h \
        docparser.h \
        generatorsetjava.h \
        javagenerator.h \
        jumptable.h \
        metainfogenerator.h \
        metajavabuilder.h \
        qdocgenerator.h \
        uiconverter.h \

SOURCES += \
        classlistgenerator.cpp \
        cppgenerator.cpp \
        cppheadergenerator.cpp \
        cppimplgenerator.cpp \
        docparser.cpp \
        generatorsetjava.cpp \
        javagenerator.cpp \
        jumptable.cpp \
        metainfogenerator.cpp \
        metajavabuilder.cpp \
        qdocgenerator.cpp \
        uiconverter.cpp \

include(generator.pri)

# Profiling generator.exe
#
#CONFIG += DEBUG
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg
#
# Each sample counts as 0.01 seconds.
#  %   cumulative   self              self     total           
# time   seconds   seconds    calls   s/call   s/call  name    
# 34.99    218.69   218.69 4638987900     0.00     0.00  QBasicAtomicInt::ref()
# 34.66    435.31   216.62 4796821756     0.00     0.00  QBasicAtomicInt::deref()
#  4.29    462.12    26.81 2773082225     0.00     0.00  QString::QString(QString const&)
#  2.47    477.54    15.43 2943952981     0.00     0.00  QString::~QString()
#

macx {
    #QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.9.sdk
    # (commented out) To inherit the Qt SDK default arches
    #contains(QT_CONFIG, x86):CONFIG += x86
    #contains(QT_CONFIG, x86_64):CONFIG += x86_64
    # ppc and ppc64 could be disable (uncomment lined below) as most people are using/building with
    # Intel this tool "generator" is only used for building QtJambi and not required at runtime.
    #contains(QT_CONFIG, ppc):CONFIG -= ppc
    #contains(QT_CONFIG, ppc64):CONFIG -= ppc64
}
