#include "qtjambi_network_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtNetwork,io/qt/network,QTimeoutException,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtNetwork,io/qt/network,QHostAddress$HostInfo,
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(address,Lio/qt/network/QHostAddress;)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(port,S)
)
}
