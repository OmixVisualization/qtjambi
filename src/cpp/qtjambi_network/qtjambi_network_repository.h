#ifndef QTJAMBI_NETWORK_REPOSITORY_H
#define QTJAMBI_NETWORK_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtNetwork{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QTimeoutException,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QHostAddress$HostInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(address)
                                     QTJAMBI_REPOSITORY_DECLARE_SHORT_FIELD(port))
}
}

#endif // QTJAMBI_NETWORK_REPOSITORY_H
