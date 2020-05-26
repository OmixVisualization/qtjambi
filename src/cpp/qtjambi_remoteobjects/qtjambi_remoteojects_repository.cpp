#include "qtjambi_remoteojects_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtRemoteObjects,io/qt/remoteobjects,IoDeviceBase$ReadResult,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/remoteobjects/QtRemoteObjects$QRemoteObjectPacketTypeEnum;Ljava/lang/String;)
)
}
