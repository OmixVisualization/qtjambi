#include "qtjambi_core_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
namespace Private{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QCborStreamReader$StringResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Lio/qt/core/QCborStreamReader$StringResultCode;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QCborValue$FromCborResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QCborValue;Lio/qt/core/QCborParserError;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QDate$Week,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(II)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QLockFile$LockInfo,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JLjava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QJsonDocument$FromJsonResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QJsonDocument;Lio/qt/core/QJsonParseError;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QSharedMemory,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(__qt_accessMode,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtCore,io/qt/core,QMessageHandler,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(process,(ILjava/lang/String;)Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang,ArithmeticException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/math,BigInteger,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I[B)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(abs,()Ljava/math/BigInteger;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(signum,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(bitLength,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(longValue,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toByteArray,()[B)
)
}
}
