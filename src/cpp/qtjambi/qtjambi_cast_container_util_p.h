#ifndef QTJAMBI_CAST_CONTAINER_UTIL_P_H
#define QTJAMBI_CAST_CONTAINER_UTIL_P_H

#include "qtjambi_containers.h"
#include "qtjambi_typetests.h"

namespace QtJambiPrivate {

template<typename Iterator, bool supported = std::is_pointer<Iterator>::value || supports_increment<Iterator>::value>
struct IteratorIncrement{
    static void function(JNIEnv * env, void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::increment" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename Iterator>
struct IteratorIncrement<Iterator,true>{
    static void function(JNIEnv *, void* ptr) {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        ++(*iterator);
    }
};

template<typename Iterator, bool supported = std::is_pointer<Iterator>::value || supports_decrement<Iterator>::value>
struct IteratorDecrement{
    static void function(JNIEnv * env, void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::decrement" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename Iterator>
struct IteratorDecrement<Iterator,true>{
    static void function(JNIEnv *, void* ptr) {
         Iterator* iterator = static_cast<Iterator*>(ptr);
         --(*iterator);
    }
};

template<typename Iterator, bool supports_less_than = std::is_pointer<Iterator>::value || supports_less_than<Iterator>::value>
struct IteratorLessThan{
    static jboolean function(JNIEnv *env, void*, void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::lessThan" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<typename Iterator>
struct IteratorLessThan<Iterator, true>{
    static jboolean function(JNIEnv *, void* ptr, void* ptr2) {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        Iterator* iterator2 = static_cast<Iterator*>(ptr2);
        return (*iterator)<(*iterator2);
    }
};

template<typename Iterator, typename SuperType = AbstractIteratorAccess>
struct QAbstractIteratorAccess : SuperType{
    void increment(JNIEnv *env, void* iterator) override {
        IteratorIncrement<Iterator>::function(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        IteratorDecrement<Iterator>::function(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, void* iterator, void* other) override {
        return IteratorLessThan<Iterator>::function(env, iterator, other);
    }
    bool canLess() override {
        return std::is_pointer<Iterator>::value || supports_less_than<Iterator>::value;
    }
    jboolean equals(JNIEnv *, void* ptr, void* ptr2) override {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        Iterator* iterator2 = static_cast<Iterator*>(ptr2);
        return (*iterator)==(*iterator2);
    }
};

}

#endif // QTJAMBI_CAST_CONTAINER_UTIL_P_H
