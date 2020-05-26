#ifndef QTJAMBI_SERIALBUS_H
#define QTJAMBI_SERIALBUS_H

#include <qtjambi/qtjambi_repository.h>

class QTcpSocket;

namespace Java{
    namespace Private{
        class Runtime{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,QCanBusException,
                QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()
                QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getMessage))
        };
    }
}

#endif // QTJAMBI_SERIALBUS_H
