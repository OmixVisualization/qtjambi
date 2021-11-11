#include <QtCore/QMutex>
#include "qtjambi_network_repository.h"

namespace Java{
namespace QtNetwork{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/network,QTimeoutException,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/network,QHostAddress$HostInfo,
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(address,Lio/qt/network/QHostAddress;)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(port,S)
)
}
}
