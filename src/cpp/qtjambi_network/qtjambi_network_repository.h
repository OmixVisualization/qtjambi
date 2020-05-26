#ifndef QTJAMBI_NETWORK_REPOSITORY_H
#define QTJAMBI_NETWORK_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtNetwork
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtNetwork,QTimeoutException,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtNetwork,QHostAddress$HostInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(address)
                                     QTJAMBI_REPOSITORY_DECLARE_SHORT_FIELD(port))

private:
    QtNetwork() = delete;
};
}

#endif // QTJAMBI_NETWORK_REPOSITORY_H
