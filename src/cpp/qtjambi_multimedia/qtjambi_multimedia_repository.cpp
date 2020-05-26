#include "qtjambi_multimedia_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtMultimedia,io/qt/multimedia,QMediaService$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Z)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(result,Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(continuous,Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtMultimedia,io/qt/multimedia,QMediaService$ListResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/util/List;Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtMultimedia,io/qt/multimedia,QAbstractVideoBuffer$MapResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/nio/ByteBuffer;IZ)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(data,Ljava/nio/ByteBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(bytesPerLine,I)
)
}
