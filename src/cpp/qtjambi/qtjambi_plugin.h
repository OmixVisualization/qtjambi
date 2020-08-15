#ifndef QTJAMBI_PLUGIN_H
#define QTJAMBI_PLUGIN_H

#include <QtCore>
#include "qtjambi_global.h"

typedef const char* (*QueryMetadata)();

QTJAMBI_EXPORT QObject* qtjambi_plugin_instance(QueryMetadata qt_plugin_query_metadata);

#endif // QTJAMBI_PLUGIN_H
