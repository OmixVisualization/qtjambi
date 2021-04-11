#include <QtCore/QMutex>
#include "qtjambi_bluetooth_repository.h"

Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

namespace Java{
namespace QtBluetooth{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/bluetooth,QBluetoothDeviceInfo$ServiceUuids,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/util/List;Lio/qt/bluetooth/QBluetoothDeviceInfo$DataCompleteness;)
)
}
}
