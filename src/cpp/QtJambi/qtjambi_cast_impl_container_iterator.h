#ifndef QTJAMBI_CAST_IMPL_CONTAINER_ITERATOR_H
#define QTJAMBI_CAST_IMPL_CONTAINER_ITERATOR_H

#include "containerapi.h"
#include "typetests.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

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

template<typename Iterator, bool supported = std::is_pointer<Iterator>::value || (supports_decrement<Iterator>::value && is_bidirectional_iterator<Iterator>::value)>
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
    static jboolean function(JNIEnv *env, const void*, const void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::lessThan" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<typename Iterator>
struct IteratorLessThan<Iterator, true>{
    static jboolean function(JNIEnv *, const void* ptr, const void* ptr2) {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const Iterator* iterator2 = static_cast<const Iterator*>(ptr2);
        return (*iterator)<(*iterator2);
    }
};

template<typename Iterator, typename SuperType = AbstractSequentialConstIteratorAccess>
struct AbstractConstIteratorAccess : SuperType{
    void increment(JNIEnv *env, void* iterator) override {
        IteratorIncrement<Iterator>::function(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        IteratorDecrement<Iterator>::function(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return IteratorLessThan<Iterator>::function(env, iterator, other);
    }
    bool canLess() override {
        return std::is_pointer<Iterator>::value || supports_less_than<Iterator>::value;
    }
    jboolean equals(JNIEnv *, const void* ptr, const void* ptr2) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const Iterator* iterator2 = static_cast<const Iterator*>(ptr2);
        return (*iterator)==(*iterator2);
    }
};

}

#endif // QTJAMBI_CAST_IMPL_CONTAINER_ITERATOR_H
