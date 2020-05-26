#include "qtjambi_location_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtLocation,io/qt/location,QGeoServiceProviderFactory$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QObject;Lio/qt/location/QGeoServiceProvider$Error;Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(engine,()Lio/qt/core/QObject;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(error,()Lio/qt/location/QGeoServiceProvider$Error;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(errorString,()Ljava/lang/String;)
)
}
