#include <QtCore/QtGlobal>
#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include <QtCore/QtPlugin>
#include <QtCore/QPluginLoader>

#define CBOR_DATA

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaData[] = {
    CBOR_DATA
};
extern "C" Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QPluginMetaData) qt_plugin_query_metadata_v2()
{ static constexpr QT_PLUGIN_METADATAV2_SECTION QPluginMetaDataV2<qt_pluginMetaData> md{}; return md; }

#define METADATA_FUNCTION &qt_plugin_query_metadata_v2

#else
QT_PLUGIN_METADATA_SECTION
static constexpr unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements()
#ifdef QTJAMBI_NO_DEBUG_PLUGINS
    & ~1
#endif
   ,CBOR_DATA
};

extern "C" Q_DECL_EXPORT const char *qt_plugin_query_metadata()
{ return reinterpret_cast<const char *>(qt_pluginMetaData); }

#define METADATA_FUNCTION &qt_plugin_query_metadata

#endif

typedef QObject* (*CreatePluginInstanceFunction)(decltype(METADATA_FUNCTION) qt_plugin_query_metadata);

extern "C" Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject) *qt_plugin_instance(){
//qt_plugin_instance_impl
    return nullptr;
}
