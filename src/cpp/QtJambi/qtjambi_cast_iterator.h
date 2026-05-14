/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_CAST_ITERATOR_H
#define QTJAMBI_CAST_ITERATOR_H

#include "qtjambi_cast.h"
#include "qtjambiapi_iterator.h"
#include "containerapi.h"
#include "typetests.h"

namespace QtJambiPrivate {

template<typename Iterator, bool supported = std::is_pointer_v<Iterator> || supports_increment<Iterator>::value>
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

template<typename Iterator, bool supported = std::is_pointer_v<Iterator> || (supports_decrement<Iterator>::value && is_bidirectional_iterator<Iterator>::value)>
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

template<typename Iterator, bool supports_less_than = std::is_pointer_v<Iterator> || supports_less_than<Iterator>::value>
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
        return std::is_pointer_v<Iterator> || supports_less_than<Iterator>::value;
    }
    jboolean equals(JNIEnv *, const void* ptr, const void* ptr2) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const Iterator* iterator2 = static_cast<const Iterator*>(ptr2);
        return (*iterator)==(*iterator2);
    }
};

template<typename Iterator, typename SuperType = AbstractSequentialConstIteratorAccess>
class QSequentialConstIteratorAccess : public AbstractConstIteratorAccess<Iterator,SuperType>{
protected:
    QSequentialConstIteratorAccess(){}
public:
    static QSequentialConstIteratorAccess<Iterator,SuperType>* newInstance(){
        static QSequentialConstIteratorAccess<Iterator,SuperType> instance;
        return &instance;
    }

    QSequentialConstIteratorAccess<Iterator,SuperType>* clone() override{
        return this;
    }

    void dispose() override {}

    jobject value(JNIEnv * env, const void* ptr) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const auto& value = *(*iterator);
        return ::qtjambi_cast<jobject>(env, value);
    }

    const QMetaType& valueMetaType() override{
        typedef std::remove_reference_t<decltype(*std::declval<Iterator>())> T;
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }
};

template<typename Iterator>
class QSequentialIteratorAccess : public QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>{
private:
    QSequentialIteratorAccess(){}
public:
    static QSequentialIteratorAccess<Iterator>* newInstance(){
        static QSequentialIteratorAccess<Iterator> instance;
        return &instance;
    }

    QSequentialIteratorAccess<Iterator>* clone() override{
        return this;
    }

    void dispose() override {}

    const QMetaType& valueMetaType() override{
        return QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>::valueMetaType();
    }

    jobject value(JNIEnv * env, const void* ptr) override {
        return QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>::value(env, ptr);
    }

    void increment(JNIEnv *env, void* iterator) override {
        QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>::increment(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>::decrement(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>::lessThan(env, iterator, other);
    }
    bool canLess() override {
        return QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>::canLess();
    }
    jboolean equals(JNIEnv *env, const void* ptr, const void* ptr2) override {
        return QSequentialConstIteratorAccess<Iterator,AbstractSequentialIteratorAccess>::equals(env, ptr, ptr2);
    }

    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        *(*iterator) = ::qtjambi_cast<std::remove_reference_t<decltype(*(*iterator))>>(env, newValue);
    }
};

template<typename Iterator, bool isMutable, typename... Args>
struct qtjambi_mutable_sequential_iterator_cast{
    static jobject cast(QtJambiNativeID __list_nativeId, std::conditional_t<std::is_pointer_v<Iterator>, Iterator, const Iterator&> iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, __list_nativeId,
                                                                  new Iterator(iter),
                                                                  [](void* ptr,bool) {
                                                                      Iterator* iterator = static_cast<Iterator*>(ptr);
                                                                      delete iterator;
                                                                  },
                                                                  QSequentialConstIteratorAccess<Iterator>::newInstance()
                                                                  );
    }
};

template<typename Iterator, typename... Args>
struct qtjambi_mutable_sequential_iterator_cast<Iterator,true,Args...>{
    static jobject cast(QtJambiNativeID __list_nativeId, std::conditional_t<std::is_pointer_v<Iterator>, Iterator, const Iterator&> iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, __list_nativeId,
                                                                  new Iterator(iter),
                                                                  [](void* ptr,bool) {
                                                                      Iterator* iterator = static_cast<Iterator*>(ptr);
                                                                      delete iterator;
                                                                  },
                                                                  QSequentialIteratorAccess<Iterator>::newInstance()
                                                                  );
    }
};

template<typename Iterator, typename... Args>
struct qtjambi_sequential_iterator_cast : qtjambi_mutable_sequential_iterator_cast<Iterator, std::is_reference_v<decltype(*std::declval<Iterator>())> && !std::is_const_v<std::remove_reference_t<decltype(*std::declval<Iterator>())>>, Args...>{
};

template<typename Iterator, typename SuperType = AbstractAssociativeConstIteratorAccess>
class QAssociativeConstIteratorAccess : public AbstractConstIteratorAccess<Iterator,SuperType>{
protected:
    QAssociativeConstIteratorAccess(){}
public:
    static QAssociativeConstIteratorAccess<Iterator,SuperType>* newInstance(){
        static QAssociativeConstIteratorAccess<Iterator,SuperType> instance;
        return &instance;
    }

    QAssociativeConstIteratorAccess<Iterator,SuperType>* clone() override{
        return this;
    }

    void dispose() override {}

    jobject value(JNIEnv * env, const void* ptr) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const auto& value = iterator->value();
        return ::qtjambi_cast<jobject>(env, value);
    }
    jobject key(JNIEnv * env, const void* ptr) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const auto& key = iterator->key();
        return ::qtjambi_cast<jobject>(env, key);
    }

    const QMetaType& keyMetaType() override{
        typedef std::remove_reference_t<decltype(std::declval<Iterator>().key())> K;
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<K>>());
        return type;
    }

    const QMetaType& valueMetaType() override{
        typedef std::remove_reference_t<decltype(std::declval<Iterator>().value())> V;
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<V>>());
        return type;
    }
};

template<typename Iterator>
class QAssociativeIteratorAccess : public QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess> {
private:
    QAssociativeIteratorAccess(){}
public:
    static QAssociativeIteratorAccess<Iterator>* newInstance(){
        static QAssociativeIteratorAccess<Iterator> instance;
        return &instance;
    }

    void dispose() override {}

    QAssociativeIteratorAccess<Iterator>* clone() override{
        return this;
    }

    const QMetaType& keyMetaType() override{
        return QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::keyMetaType();
    }

    const QMetaType& valueMetaType() override{
        return QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::valueMetaType();
    }

    jobject value(JNIEnv * env, const void* ptr) override {
        return QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::value(env, ptr);
    }

    jobject key(JNIEnv * env, const void* ptr) override {
        return QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::key(env, ptr);
    }

    void increment(JNIEnv *env, void* iterator) override {
        QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::increment(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::decrement(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::lessThan(env, iterator, other);
    }
    bool canLess() override {
        return QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::canLess();
    }
    jboolean equals(JNIEnv *env, const void* ptr, const void* ptr2) override {
        return QAssociativeConstIteratorAccess<Iterator,AbstractAssociativeIteratorAccess>::equals(env, ptr, ptr2);
    }

    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        iterator->value() = ::qtjambi_cast<std::remove_reference_t<decltype(iterator->value())>>(env, newValue);
    }
};

template<typename Iterator, bool isMutable, typename... Args>
struct qtjambi_mutable_associative_iterator_cast{
    static jobject cast(QtJambiNativeID nativeId, std::conditional_t<std::is_pointer_v<Iterator>, Iterator, const Iterator&> iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, nativeId,
                                                                   new Iterator(iter),
                                                                   [](void* ptr,bool) {
                                                                       delete reinterpret_cast<Iterator*>(ptr);
                                                                   },
                                                                   QAssociativeConstIteratorAccess<Iterator>::newInstance()
                                                                   );
    }
};

template<typename Iterator, typename... Args>
struct qtjambi_mutable_associative_iterator_cast<Iterator,true, Args...>{
    struct IteratorContainer{
        Iterator i;
    };
    static jobject cast(QtJambiNativeID nativeId, std::conditional_t<std::is_pointer_v<Iterator>, Iterator, const Iterator&> iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, nativeId,
                                                                   new IteratorContainer{iter},
                                                                   [](void* ptr,bool) {
                                                                       delete reinterpret_cast<IteratorContainer*>(ptr);
                                                                   },
                                                                   QAssociativeIteratorAccess<Iterator>::newInstance()
                                                                   );
    }
};

template<typename Iterator, typename... Args>
struct qtjambi_associative_iterator_cast : qtjambi_mutable_associative_iterator_cast<Iterator, std::is_reference_v<decltype(std::declval<Iterator>().value())> && !std::is_const_v<std::remove_reference_t<decltype(std::declval<Iterator>().value())>>, Args...>{
};

}

#endif // QTJAMBI_CAST_ITERATOR_H
