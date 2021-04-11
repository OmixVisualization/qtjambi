

#include <QtCore/QtGlobal>
#include <QtCore/QtEndian>
#include "qtjambi_biginteger.h"
#include "qtjambi_core_repository.h"

BigIntegerValue fromBigInteger(JNIEnv* env, jobject object)
{
    Q_ASSERT(object);
    BigIntegerValue value{0, false, false};
    int signum = Java::Runtime::BigInteger::signum(env, object);
    if(signum<0){
        object = Java::Runtime::BigInteger::abs(env, object);
        value.isNegative = true;
    }
    int bitLength = Java::Runtime::BigInteger::bitLength(env, object);
    if(bitLength<=64){
        jbyteArray data = jbyteArray(Java::Runtime::BigInteger::toByteArray(env, object));
        JByteArrayPointer pointer(env, data, false);
        if(pointer.size()>=8){
            value.value = qFromBigEndian<quint64>(pointer.pointer()+(pointer.size()-8));
        }else{
            char data[8]{0,0,0,0,0,0,0,0};
            memcpy(data+(8-pointer.size()), pointer.pointer(), size_t(pointer.size()));
            value.value = qFromBigEndian<quint64>(data);
        }
    }else{
        value.outOfRange = true;
    }
    return value;
}

jobject toBigInteger(JNIEnv* env, quint64 value, bool isNegative)
{
    jobject object = nullptr;
    jbyteArray data = env->NewByteArray(8);
    jbyte* array = env->GetByteArrayElements(data, nullptr);
    qToBigEndian<quint64>(value, array);
    env->ReleaseByteArrayElements(data, array, 0);
    object = Java::Runtime::BigInteger::newInstance(env, value==0 ? 0 : (isNegative ? -1 : 1), data);
    return object;
}
