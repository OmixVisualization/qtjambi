#include <QtCore/QMutex>
#include "qtjambi_positioning_repository.h"

namespace Java{
namespace QtPositioning {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/positioning,QNmeaPositionInfoSource$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/positioning/QGeoPositionInfo;Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(info,()Lio/qt/positioning/QGeoPositionInfo;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasFix,()Z)
)
}
namespace QtJambi {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$AbstractSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(containingObject,()Lio/qt/QtSignalEmitterInterface;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(methodIndex,()I)
)
}
namespace QtCore {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaMethod,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toSignal,(Lio/qt/core/QObject;)Lio/qt/core/QMetaObject$AbstractSignal;)
)
}
}
