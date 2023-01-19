/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_FUNCTIONPOINTER2_H
#define QTJAMBI_FUNCTIONPOINTER2_H

#include "qtjambi_global.h"
#include <QtCore/QVector>
#include <QtCore/QQueue>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <functional>
#include <limits>
#include <utility>
#include <type_traits>

namespace QtJambiPrivate{

typedef void(*FunctionPointerDisposer)(QFunctionPointer);

template<typename T>
struct TSize{
    constexpr static size_t size = sizeof(T);
};
template<>
struct TSize<void>{
    constexpr static size_t size = 0;
};

struct FunctionParamTypeInfo{
    bool isPointer;
    bool isReference;
    bool isConst;
    bool isEnum;
    bool isArithmetic;
    size_t sizeOfType;
    const std::type_info& plainTypeId;
    FunctionParamTypeInfo(const FunctionParamTypeInfo& copy)
        : isPointer(copy.isPointer),
          isReference(copy.isReference),
          isConst(copy.isConst),
          isEnum(copy.isEnum),
          isArithmetic(copy.isArithmetic),
          sizeOfType(copy.sizeOfType),
          plainTypeId(copy.plainTypeId)
    {}
public:
    template<typename T>
    static FunctionParamTypeInfo create(){
        return  FunctionParamTypeInfo(std::is_pointer<T>::value,
                                      std::is_reference<T>::value,
                                      std::is_const<typename std::remove_reference<T>::type>::value,
                                      std::is_enum<T>::value,
                                      std::is_arithmetic<T>::value,
                                      TSize<T>::size,
                                      typeid(typename std::conditional<std::is_pointer<T>::value, typename std::remove_pointer<T>::type, T>::type));
    }
private:
    FunctionParamTypeInfo(bool _isPointer,
                          bool _isReference,
                          bool _isConst,
                          bool _isEnum,
                          bool _isArithmetic,
                          size_t _sizeOfType,
                          const std::type_info& _plainTypeId)
        : isPointer(_isPointer),
          isReference(_isReference),
          isConst(_isConst),
          isEnum(_isEnum),
          isArithmetic(_isArithmetic),
          sizeOfType(_sizeOfType),
          plainTypeId(_plainTypeId)
    {}
};

QTJAMBI_EXPORT QFunctionPointer qtjambi_extract_function(QVector<FunctionParamTypeInfo> functionParamTypeInfos,
                                                         const std::type_info& functionTypeId,
                                                         FunctionPointerDisposer disposer, QFunctionPointer caller);
QTJAMBI_EXPORT QRecursiveMutex* qtjambi_function_pointer_lock();
QTJAMBI_EXPORT bool qtjambi_dispose_function(QFunctionPointer fn);
QTJAMBI_EXPORT void qtjambi_no_function_available(const std::type_info& functionTypeId);

template<typename Callable>
union storage
{
    storage() {}
    storage(Callable&& clb) : callable(std::forward<Callable>(clb)) {}
    ~storage() {}
    typename std::decay<Callable>::type callable;
};

template<typename Fn, typename Callable>
class CallableHash{
public:
    CallableHash() : hashes(), freeFunctions(), s()
    {
    }

    Fn functionOf(hash_type hash){
        return Fn(hashes.key(hash));
    }
    void insert(Fn fn, hash_type hash, Callable&& clb){
        hashes[qintptr(fn)] = hash;
        s[hash] = new storage<Callable>(std::forward<Callable>(clb));
    }
    void dispose(Fn fn){
        if(hashes.contains(qintptr(fn))){
            hash_type hash = hashes.take(qintptr(fn));
            if(storage<Callable>* stor = s.take(hash))
                delete stor;
        }
        freeFunctions.removeAll(fn);
    }
    void remove(hash_type hash){
        if(s.contains(hash)){
            if(storage<Callable>* stor = s.take(hash))
                delete stor;
            if(Fn fn = Fn(hashes.key(hash))){
                hashes.remove(qintptr(fn));
                if(freeFunctions.size()<10 || !qtjambi_dispose_function(QFunctionPointer(fn))){
                    freeFunctions.enqueue(fn);
                }
            }
        }
    }
    int size(){
        return hashes.size();
    }
    Fn next(){
        return freeFunctions.isEmpty() ? nullptr : freeFunctions.dequeue();
    }
    storage<Callable>* value(Fn fn){
        return s.value(hashes.value(qintptr(fn), std::numeric_limits<hash_type>::max()));
    }
private:
    QHash<qintptr,hash_type> hashes;
    QQueue<Fn> freeFunctions;
    QHash<hash_type,storage<Callable>*> s;
};

template<typename Ret, typename... Args>
struct fn{
    typedef Ret(*type)(Args...);
};

template<typename Callable, typename Ret, typename... Args>
typename fn<Ret,Args...>::type qtjambi_function_pointer_impl(Callable&& c, hash_type hash, std::function<void()>* deleterFunction, std::function<const Callable*(Ret(*)(Args...))>* reverseFunctionGetter)
{
    typedef typename fn<Ret,Args...>::type Fn;
    static CallableHash<Fn,Callable> callables;
    QMutexLocker locker(qtjambi_function_pointer_lock());
    Fn fn = callables.functionOf(hash);
    if(!fn){
        fn = callables.next();
        if(!fn){
            FunctionPointerDisposer disposer = [](QFunctionPointer fn){
                callables.dispose(Fn(fn));
            };
            Ret(*caller)(Fn,Args...) = [](Fn fn, Args...args)->Ret{
                storage<Callable>* stor = callables.value(fn);
                if(!stor)
                    qtjambi_no_function_available(typeid(Ret(*)(Args...)));
                return stor ? Ret(stor->callable(std::forward<Args>(args)...)) : Ret();
            };
            fn = Fn(qtjambi_extract_function(QVector<FunctionParamTypeInfo>{FunctionParamTypeInfo::create<Ret>(), FunctionParamTypeInfo::create<Args>()...},
                                             typeid(Ret(Args...)),
                                             disposer,
                                             QFunctionPointer(caller)));
        }
        if(fn)
            callables.insert(fn, hash, std::forward<Callable>(c));
    }
    if(fn){
        if(reverseFunctionGetter){
            *reverseFunctionGetter = [](Fn fn) -> const Callable* {
                QMutexLocker locker(qtjambi_function_pointer_lock());
                if(storage<Callable>* stor = callables.value(fn)){
                    return &stor->callable;
                }
                return nullptr;
            };
        }
        if(deleterFunction){
            *deleterFunction = [hash](){
                QMutexLocker locker(qtjambi_function_pointer_lock());
                callables.remove(hash);
            };
        }
    }
    return fn;
}

} // namespace QtJambiPrivate

template<typename Fn, typename Callable>
Fn* qtjambi_function_pointer2(Callable&& c, hash_type hash = 0, std::function<void()>* deleterFunction = nullptr, std::function<const Callable*(Fn*)>* reverseFunctionGetter = nullptr)
{
    return QtJambiPrivate::qtjambi_function_pointer_impl(std::move(c), hash, deleterFunction, reverseFunctionGetter);
}

#endif // QTJAMBI_FUNCTIONPOINTER2_H
