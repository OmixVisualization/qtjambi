#ifndef QTJAMBI_BLUETOOTH_REPOSITORY_H
#define QTJAMBI_BLUETOOTH_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtBluetooth
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtBluetooth,QBluetoothDeviceInfo$ServiceUuids,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
private:
    QtBluetooth() = delete;
    ~QtBluetooth() = delete;
};
}

#endif // QTJAMBI_BLUETOOTH_REPOSITORY_H
