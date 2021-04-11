#include <QtCore/QMutex>
#include "qtjambi_location_repository.h"

Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

namespace Java{
namespace QtLocation{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/location,QGeoServiceProviderFactory$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QObject;Lio/qt/location/QGeoServiceProvider$Error;Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(engine,()Lio/qt/core/QObject;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(error,()Lio/qt/location/QGeoServiceProvider$Error;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(errorString,()Ljava/lang/String;)
)
}
}
