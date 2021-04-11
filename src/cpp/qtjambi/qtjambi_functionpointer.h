/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_FUNCTIONPOINTER_H
#define QTJAMBI_FUNCTIONPOINTER_H

#include <QtCore/QVector>
#include <QtCore/QQueue>
#include <QtCore/QHash>
#include <QtCore/QReadWriteLock>
#include <functional>
#include <limits>
#include <utility>
#include <type_traits>
#include "qtjambi_global.h"

template<typename Callable>
union storage
{
    storage() {}
    storage(Callable&& clb) : callable(std::forward<Callable>(clb)) {}
    ~storage() {}
    typename std::decay<Callable>::type callable;
};

template<typename Callable>
class CallableHash{
public:
    CallableHash(int max) : hashes(max, (std::numeric_limits<hash_type>::max)()), freeIndexes(), s()
    {
        for(int i=0; i<max; ++i){
            freeIndexes.enqueue(i);
        }
    }

    bool contains(hash_type hash){
        return hashes.contains(hash);
    }
    int indexOf(hash_type hash){
        return hashes.indexOf(hash);
    }
    void insert(int index, hash_type hash, Callable&& clb){
        hashes[index] = hash;
        s[hash] = new storage<Callable>(std::forward<Callable>(clb));
    }
    void remove(hash_type hash){
        if(s.contains(hash)){
            storage<Callable>* stor = s.take(hash);
            delete stor;
            int idx = hashes.indexOf(hash);
            if(idx<=0){
                hashes[idx] = (std::numeric_limits<hash_type>::max)();
                freeIndexes.enqueue(idx);
            }
        }
    }
    int size(){
        return hashes.size();
    }
    int next(){
        return freeIndexes.isEmpty() ? -1 : freeIndexes.dequeue();
    }
    storage<Callable>* value(int index){
        return s.value(hashes.at(index));
    }
private:
    QVector<hash_type> hashes;
    QQueue<int> freeIndexes;
    QHash<hash_type,storage<Callable>*> s;
};

#ifdef INSERT_FUNCTION
#undef INSERT_FUNCTION
#endif

#ifndef QTJAMBI_THROW_EXCEPTION
#define QTJAMBI_THROW_EXCEPTION(Exception,env,Message) Q_UNUSED(env)
#endif

#define INSERT_FUNCTION(N)\
    functions->replace(N, [](Args... args) -> Ret {\
        storage<Callable>* stor = callables->value(N);\
        if(!stor){\
            if(JNIEnv *env = qtjambi_current_environment()){\
                QTJAMBI_JNI_LOCAL_FRAME(env, 100)\
                QTJAMBI_THROW_EXCEPTION(NullPointerException, env, "Function pointer is null.")\
            }\
        }\
        return stor ? Ret(stor->callable(std::forward<Args>(args)...)) : Ret();\
    })

template<int n>
struct FunctionCreator{
    template<typename Callable, typename Ret, typename... Args>
    static void createFunction(QVector<Ret (*)(Args...)>* functions, CallableHash<Callable>* _callables){
        static CallableHash<Callable>* callables = _callables;
        //QVector<Fn*>* functions = static_cast<QVector<Fn*>*>(_functions);
        INSERT_FUNCTION(n-1);
        INSERT_FUNCTION(n-2);
        INSERT_FUNCTION(n-3);
        INSERT_FUNCTION(n-4);
        INSERT_FUNCTION(n-5);
        INSERT_FUNCTION(n-6);
        INSERT_FUNCTION(n-7);
        INSERT_FUNCTION(n-8);
        INSERT_FUNCTION(n-9);
        INSERT_FUNCTION(n-10);
        INSERT_FUNCTION(n-11);
        INSERT_FUNCTION(n-12);
        INSERT_FUNCTION(n-13);
        INSERT_FUNCTION(n-14);
        INSERT_FUNCTION(n-15);
        INSERT_FUNCTION(n-16);
        INSERT_FUNCTION(n-17);
        INSERT_FUNCTION(n-18);
        INSERT_FUNCTION(n-19);
        INSERT_FUNCTION(n-20);
        INSERT_FUNCTION(n-21);
        INSERT_FUNCTION(n-22);
        INSERT_FUNCTION(n-23);
        INSERT_FUNCTION(n-24);
        INSERT_FUNCTION(n-25);
        INSERT_FUNCTION(n-26);
        INSERT_FUNCTION(n-27);
        INSERT_FUNCTION(n-28);
        INSERT_FUNCTION(n-29);
        INSERT_FUNCTION(n-30);
        INSERT_FUNCTION(n-31);
        INSERT_FUNCTION(n-32);
        INSERT_FUNCTION(n-33);
        INSERT_FUNCTION(n-34);
        INSERT_FUNCTION(n-35);
        INSERT_FUNCTION(n-36);
        INSERT_FUNCTION(n-37);
        INSERT_FUNCTION(n-38);
        INSERT_FUNCTION(n-39);
        INSERT_FUNCTION(n-40);
        INSERT_FUNCTION(n-41);
        INSERT_FUNCTION(n-42);
        INSERT_FUNCTION(n-43);
        INSERT_FUNCTION(n-44);
        INSERT_FUNCTION(n-45);
        INSERT_FUNCTION(n-46);
        INSERT_FUNCTION(n-47);
        INSERT_FUNCTION(n-48);
        INSERT_FUNCTION(n-49);
        INSERT_FUNCTION(n-50);
        INSERT_FUNCTION(n-51);
        INSERT_FUNCTION(n-52);
        INSERT_FUNCTION(n-53);
        INSERT_FUNCTION(n-54);
        INSERT_FUNCTION(n-55);
        INSERT_FUNCTION(n-56);
        INSERT_FUNCTION(n-57);
        INSERT_FUNCTION(n-58);
        INSERT_FUNCTION(n-59);
        INSERT_FUNCTION(n-60);
        INSERT_FUNCTION(n-61);
        INSERT_FUNCTION(n-62);
        INSERT_FUNCTION(n-63);
        INSERT_FUNCTION(n-64);
        FunctionCreator<n-64>::createFunction(functions, _callables);
    }
};

#undef INSERT_FUNCTION

template<>
struct FunctionCreator<0>{
    template<typename Callable, typename Ret, typename... Args>
    static void createFunction(QVector<Ret (*)(Args...)>*, CallableHash<Callable>*){}
};

template<int max, typename Fn, typename Callable>
Fn* qtjambi_function_pointer(Callable&& c, hash_type hash = 0, std::function<void()>* deleterFunction = nullptr, std::function<const Callable*(Fn*)>* reverseFunctionGetter = nullptr)
{
    static CallableHash<Callable> callables(max*64);
    static QVector<Fn*> functions([](CallableHash<Callable>* chash) -> QVector<Fn*> {
                                      QVector<Fn*> result(max*64, nullptr);
                                      FunctionCreator<max*64>::createFunction(&result, chash);
                                      return result;
                                  }(&callables));
    static QReadWriteLock lock;
    QWriteLocker locker(&lock);
    if(reverseFunctionGetter){
        QVector<Fn*>* _functions = &functions;
        CallableHash<Callable>* chash = &callables;
        *reverseFunctionGetter = [_functions, chash](Fn* ptr) -> const Callable* {
            if(ptr){
                int idx = _functions->indexOf(ptr);
                if(idx>=0){
                    storage<Callable>* stor = chash->value(idx);
                    if(stor){
                        return &stor->callable;
                    }
                }
            }
            return nullptr;
        };
    }
    int idx;
    if(!callables.contains(hash)){
        idx = callables.next();
        if(idx<0 || idx>=functions.size())
            return nullptr;
        callables.insert(idx, hash, std::forward<Callable>(c));
        if(deleterFunction){
            CallableHash<Callable>* chash = &callables;
            *deleterFunction = [chash,hash](){chash->remove(hash);};
        }
    }else{
        idx = callables.indexOf(hash);
    }
    return functions[idx];
}

#endif // QTJAMBI_FUNCTIONPOINTER_H
