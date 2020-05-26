#ifndef QTJAMBI_LOCATION_REPOSITORY_H
#define QTJAMBI_LOCATION_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtLocation
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtLocation,QGeoServiceProviderFactory$Result,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(engine)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(error)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(errorString))
private:
    QtLocation() = delete;
    ~QtLocation() = delete;
};
}

#endif // QTJAMBI_LOCATION_REPOSITORY_H
