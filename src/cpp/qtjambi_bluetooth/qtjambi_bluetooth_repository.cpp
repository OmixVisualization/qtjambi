#include "qtjambi_bluetooth_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtBluetooth,io/qt/bluetooth,QBluetoothDeviceInfo$ServiceUuids,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/util/List;Lio/qt/bluetooth/QBluetoothDeviceInfo$DataCompleteness;)
)
}
