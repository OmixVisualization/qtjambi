#include "qtjambi_positioning_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtPositioning,io/qt/positioning,QNmeaPositionInfoSource$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/positioning/QGeoPositionInfo;Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(info,()Lio/qt/positioning/QGeoPositionInfo;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasFix,()Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtPositioning,io/qt/internal,QtJambiSignals$AbstractSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(containingObject,()Lio/qt/QtSignalEmitterInterface;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(methodIndex,()I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtPositioning,io/qt/core,QMetaMethod,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toSignal,(Lio/qt/core/QObject;)Lio/qt/core/QMetaObject$AbstractSignal;)
)
}
