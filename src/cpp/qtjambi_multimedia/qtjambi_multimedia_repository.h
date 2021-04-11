#ifndef QTJAMBI_MULTIMEDIA_REPOSITORY_H
#define QTJAMBI_MULTIMEDIA_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtMultimedia{

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMediaService$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(result)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(continuous))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMediaService$ListResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QAbstractVideoBuffer$MapResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(data)
                                     QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(bytesPerLine))
}
}

#endif // QTJAMBI_MULTIMEDIA_REPOSITORY_H
