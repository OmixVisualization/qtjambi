#ifndef QTJAMBI_PLUGIN_H
#define QTJAMBI_PLUGIN_H

#include "qtjambi_global.h"

class QObject;

typedef const char* (*QueryMetadata)();

QTJAMBI_EXPORT QObject* qtjambi_plugin_instance(QueryMetadata qt_plugin_query_metadata);

QTJAMBI_EXPORT QString qtjambi_function_library_path(const void* qt_plugin_query_metadata);

#endif // QTJAMBI_PLUGIN_H
