#ifndef QTJAMBI_MULTIMEDIA_REPOSITORY_H
#define QTJAMBI_MULTIMEDIA_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtMultimedia
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtMultimedia,QMediaService$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(result)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(continuous))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtMultimedia,QMediaService$ListResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtMultimedia,QAbstractVideoBuffer$MapResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(data)
                                     QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(bytesPerLine))
private:
    QtMultimedia() = delete;
};
}

#endif // QTJAMBI_MULTIMEDIA_REPOSITORY_H
