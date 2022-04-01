#ifndef QTJAMBI_PLUGIN_H
#define QTJAMBI_PLUGIN_H

#include "qtjambi_global.h"
#include <QtCore/QtPlugin>

QTJAMBI_EXPORT QString qtjambi_function_library_path(const void* qt_plugin_query_metadata);

#endif // QTJAMBI_PLUGIN_H
