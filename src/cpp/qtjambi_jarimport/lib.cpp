/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QtPlugin>
#include <QtCore/QPluginLoader>

#define CBOR_DATA \
    0xbf, \
    0x02,  0x78,  0x2e,  'o',  'r',  'g',  '.',  'q', \
    't',  '-',  'p',  'r',  'o',  'j',  'e',  'c', \
    't',  '.',  'Q',  't',  '.',  'Q',  'Q',  'm', \
    'l',  'E',  'n',  'g',  'i',  'n',  'e',  'E', \
    'x',  't',  'e',  'n',  's',  'i',  'o',  'n', \
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', \
    'e', \
    0x03,  0x69,  'J',  'a',  'r',  'i',  'm',  'p', \
    'o',  'r',  't', \
    0xff,


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
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    CBOR_DATA
};
extern "C" Q_DECL_EXPORT const char *qt_plugin_query_metadata()
{ return reinterpret_cast<const char *>(qt_pluginMetaData); }
#define METADATA_FUNCTION &qt_plugin_query_metadata
#endif

typedef QObject* (*CreatePluginInstanceFunction)(decltype(METADATA_FUNCTION) ptr);

extern "C" Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject) *qt_plugin_instance(){
    QJsonValue iid("io.qtjambi.PluginImporter");
    for(QStaticPlugin sp : QPluginLoader::staticPlugins()){
        if(sp.metaData()["IID"]==iid && sp.instance){
            if(QObject* jarimportPlugin = sp.instance()){
                CreatePluginInstanceFunction fun = CreatePluginInstanceFunction(jarimportPlugin->qt_metacast("CreateJarImportFunction"));
                return fun ? fun(METADATA_FUNCTION) : nullptr;
            }
        }
    }
    return nullptr;
}
