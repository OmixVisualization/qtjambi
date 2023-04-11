#include "utils_p.h"

class QAbstractGrpcClient{
public:
    static void deleteGrpcChannel(QAbstractGrpcChannel* pointer){
        delete pointer;
    }
};

namespace QtJambiPrivate{

void deleteGrpcChannel(QAbstractGrpcChannel* pointer){
    QAbstractGrpcClient::deleteGrpcChannel(pointer);
}

}
