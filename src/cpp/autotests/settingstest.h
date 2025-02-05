/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef SETTINGSTEST_H
#define SETTINGSTEST_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include<QtCore/QSettings>

class SettingsTest
{
private:
    SettingsTest();
public:
    static void writeEnum(QSettings* settings);
    static void writeFlag(QSettings* settings);
    static void writeInt(QSettings* settings);
    static void writeUInt(QSettings* settings);
    static void writeInt64(QSettings* settings);
    static void writeBool(QSettings* settings);
    static void writeChar(QSettings* settings);
    static void writeString(QSettings* settings);
    static void writeStringList(QSettings* settings);
    static void writeIntList(QSettings* settings);
    static void writeByteArray(QSettings* settings);
    static void write(QSettings* settings, const QString& key, const QVariant& value);
    static bool readEnum(QSettings* settings);
    static bool readFlag(QSettings* settings);
    static bool readInt(QSettings* settings);
    static bool readLong(QSettings* settings);
    static bool readBoolean(QSettings* settings);
    static bool readByte(QSettings* settings);
    static bool readString(QSettings* settings);
    static bool readStringList(QSettings* settings);
    static bool readIntList(QSettings* settings);
    static bool readByteArray(QSettings* settings);
    static QVariant read(QSettings* settings, const QString& key);
};

#endif // SETTINGSTEST_H
