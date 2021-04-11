#ifndef QTJAMBI_LOCATION_REPOSITORY_H
#define QTJAMBI_LOCATION_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtLocation{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGeoServiceProviderFactory$Result,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(engine)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(error)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(errorString))
}
}

#endif // QTJAMBI_LOCATION_REPOSITORY_H
