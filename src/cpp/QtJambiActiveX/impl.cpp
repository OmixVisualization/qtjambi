#include <QtJambi/QtJambiAPI>

extern "C" JNIEXPORT int JNICALL Java_io_qt_activex_QtJambi_1LibraryUtilities_qtCompileVersion(JNIEnv *, jclass){
    return QT_VERSION;
}
