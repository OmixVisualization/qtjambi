#include <QtCore/QMutex>
#include "qtjambi_serialbus.h"

namespace Java{
    namespace QtSerialBus{
        QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/serialbus,QCanBusException,
            QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
            QTJAMBI_REPOSITORY_DEFINE_METHOD(getMessage,()Ljava/lang/String;)
        )
    }
}
