#include <QtJambi/QtJambiAPI>

extern "C" Q_DECL_EXPORT int JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_activex_QtJambi_1LibraryUtilities_qtCompileVersion)(JNIEnv *, jclass){
    return QT_VERSION;
}
