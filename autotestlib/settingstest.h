#ifndef SETTINGSTEST_H
#define SETTINGSTEST_H

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
