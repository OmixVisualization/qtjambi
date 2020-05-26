#ifndef QTJAMBI_CORE_REPOSITORY_H
#define QTJAMBI_CORE_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace Private{
class QtCore
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QCborStreamReader$StringResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QCborValue$FromCborResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QDate$Week,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QJsonDocument$FromJsonResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QSharedMemory,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(__qt_accessMode))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QLockFile$LockInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtCore,QMessageHandler,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(process))
private:
    QtCore() = delete;
    ~QtCore() = delete;
};

class Runtime{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,ArithmeticException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,BigInteger,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(abs)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(signum)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(bitLength)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(longValue)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(toByteArray))

    private:
    Runtime() = delete;
    ~Runtime() = delete;
};
}
}

#endif // QTJAMBI_CORE_REPOSITORY_H
