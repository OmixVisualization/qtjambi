/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_IMPL_CONTAINER_ITERATOR_H
#define QTJAMBI_CAST_IMPL_CONTAINER_ITERATOR_H

#include "containerapi.h"
#include "typetests.h"

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
