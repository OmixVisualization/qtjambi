/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "settingstest.h"

SettingsTest::SettingsTest()
{

}

void SettingsTest::writeEnum(QSettings* settings){
    settings->setValue("cpp-enum", QVariant::fromValue<Qt::WindowType>(Qt::WindowFullscreenButtonHint));
}
void SettingsTest::writeFlag(QSettings* settings){
    settings->setValue("cpp-flag", QVariant::fromValue<Qt::WindowFlags>(Qt::WindowFlags(Qt::WindowFullscreenButtonHint | Qt::WindowTransparentForInput)));
}
void SettingsTest::writeInt(QSettings* settings){
    settings->setValue("cpp-int", QVariant::fromValue<int>(int(-5)));
}
void SettingsTest::writeUInt(QSettings* settings){
    settings->setValue("cpp-uint", QVariant::fromValue<uint>(uint(39)));
}
void SettingsTest::writeInt64(QSettings* settings){
    settings->setValue("cpp-int64", QVariant::fromValue<qint64>(qint64(898776658768)));
}
void SettingsTest::writeBool(QSettings* settings){
    settings->setValue("cpp-bool", QVariant::fromValue<bool>(bool(true)));
}
void SettingsTest::writeChar(QSettings* settings){
    settings->setValue("cpp-char", QVariant::fromValue<char>(char(578)));
}
void SettingsTest::writeString(QSettings* settings){
    settings->setValue("cpp-string", QVariant::fromValue<QString>(QString("\"test1, test2\"")));
}
void SettingsTest::writeStringList(QSettings* settings){
    settings->setValue("cpp-stringlist", QVariant::fromValue<QStringList>(QStringList()<<"test1"<<"test2"));
}
void SettingsTest::writeIntList(QSettings* settings){
    settings->setValue("cpp-intlist", QVariant::fromValue<QList<QVariant> >(QList<QVariant>()<<QVariant(1)<<QVariant(2)<<QVariant(3)<<QVariant(4)<<QVariant(5)));
}
void SettingsTest::writeByteArray(QSettings* settings){
    settings->setValue("cpp-bytearray", QVariant::fromValue<QByteArray>(QByteArray("test")));
}
void SettingsTest::write(QSettings* settings, const QString& key, const QVariant& value){
    settings->setValue(key, value);
}
bool SettingsTest::readEnum(QSettings* settings){
    return settings->value("java-enum")==QVariant::fromValue<Qt::WindowType>(Qt::WindowFullscreenButtonHint);
}
bool SettingsTest::readFlag(QSettings* settings){
    return settings->value("java-flag")==QVariant::fromValue<Qt::WindowFlags>(Qt::WindowFlags(Qt::WindowFullscreenButtonHint | Qt::WindowTransparentForInput));
}
bool SettingsTest::readInt(QSettings* settings){
    return settings->value("java-int")==QVariant::fromValue<int>(int(-5));
}
bool SettingsTest::readLong(QSettings* settings){
    return settings->value("java-long")==QVariant::fromValue<qint64>(qint64(898776658768));
}
bool SettingsTest::readBoolean(QSettings* settings){
    return settings->value("java-boolean")==QVariant::fromValue<bool>(bool(true));
}
bool SettingsTest::readByte(QSettings* settings){
    return settings->value("java-byte")==QVariant::fromValue<char>(char(578));
}
bool SettingsTest::readString(QSettings* settings){
    return settings->value("java-string")==QVariant::fromValue<QString>(QString("\"test1, test2\""));
}
bool SettingsTest::readStringList(QSettings* settings){
    return settings->value("java-stringlist").value<QStringList>()==(QStringList()<<"test1"<<"test2");
}
bool SettingsTest::readIntList(QSettings* settings){
    return settings->value("java-intlist").value<QList<QVariant> >()==(QList<QVariant>()<<QVariant(1)<<QVariant(2)<<QVariant(3)<<QVariant(4)<<QVariant(5));
}
bool SettingsTest::readByteArray(QSettings* settings){
    return settings->value("java-bytearray")==QVariant::fromValue<QByteArray>(QByteArray("test"));
}
QVariant SettingsTest::read(QSettings* settings, const QString& key){
    return settings->value(key);
}
