#include <QtCore/QMutex>
#include <QtSerialBus/QModbusPdu>
#include "qtjambi_serialbus.h"

namespace Java{
    namespace QtSerialBus{
        QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/serialbus,QCanBusException,
            QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
            QTJAMBI_REPOSITORY_DEFINE_METHOD(getMessage,()Ljava/lang/String;)
        )
    }
}

class QModbusSerialAdu{
public:
    static void* create_QModbusPdu(void* placement, const void * copy){
        if(copy)
            return new(placement) QModbusPdu(*reinterpret_cast<const QModbusPdu*>(copy));
        else
            return new(placement) QModbusPdu();
    }
};

void* create_QModbusPdu(void* placement, const void * copy){
    return QModbusSerialAdu::create_QModbusPdu(placement, copy);
}
