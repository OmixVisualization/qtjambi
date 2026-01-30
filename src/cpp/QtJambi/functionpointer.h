/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

#include "global.h"
#include "qtjambiapi.h"
#include <QtCore/QVector>
#include <QtCore/QQueue>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <functional>
#include <limits>
#include <utility>
#include <type_traits>

#if Q_CC_MSVC && !defined(QT_DEBUG)
// required because msvc merges the templates as code optimization
QTJAMBI_EXPORT QFunctionPointer template_keep_dummy(QFunctionPointer ptr, ushort n);
#endif

namespace FunctionPointerPrivate{

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

QTJAMBI_EXPORT QFunctionPointer extractFunction(QVector<FunctionParamTypeInfo> functionParamTypeInfos,
                                                         const std::type_info& functionTypeId,
                                                         FunctionPointerDisposer disposer, QFunctionPointer caller);
QTJAMBI_EXPORT QRecursiveMutex* functionPointerLock();
QTJAMBI_EXPORT void registerFunctionPointerCleanup(void* ptr, void(*cleanup)(void*));
QTJAMBI_EXPORT void unregisterFunctionPointerCleanup(void* ptr);
QTJAMBI_EXPORT bool disposeFunction(QFunctionPointer fn);
QTJAMBI_EXPORT void noFunctionAvailable(const std::type_info& functionTypeId);

template<typename Callable>
union storage
{
    storage() {}
    storage(Callable&& clb) : callable(std::forward<Callable>(clb)) {}
    ~storage() {}
    typename std::decay<Callable>::type callable;
};

template<ushort count, typename Callable, typename Ret, typename... Args>
void setInitialFunction(QQueue<typename QtJambiAPI::FunctionType<Ret, Args...>::type>& freeFunctions);

template<ushort count, typename Callable, typename Ret, typename... Args>
class CallableHash{
    typedef typename QtJambiAPI::FunctionType<Ret,Args...>::type Fn;

    CallableHash();
    ~CallableHash();
    static void cleanup(void* ptr){
        CallableHash* _this = reinterpret_cast<CallableHash*>(ptr);
        _this->hashes.clear();
        _this->freeFunctions.clear();
        _this->s.clear();
    }
public:
    static CallableHash& instance();

    Fn functionOf(size_t hash) const{
        return Fn(hashes.key(hash));
    }
    void insert(Fn fn, size_t hash, Callable&& clb){
        hashes[qintptr(fn)] = hash;
        s[hash] = new storage<Callable>(std::forward<Callable>(clb));
    }
    void dispose(Fn fn){
        if(hashes.contains(qintptr(fn))){
            size_t hash = hashes.take(qintptr(fn));
            if(storage<Callable>* stor = s.take(hash))
                delete stor;
        }
        freeFunctions.removeAll(fn);
    }
    void remove(size_t hash){
        if(s.contains(hash)){
            if(storage<Callable>* stor = s.take(hash))
                delete stor;
            if(Fn fn = Fn(hashes.key(hash))){
                hashes.remove(qintptr(fn));
                if(!disposeFunction(QFunctionPointer(fn))){
                    freeFunctions.enqueue(fn);
                }
            }
        }
    }
    int size() const{
        return hashes.size();
    }
    Fn next(){
        if(freeFunctions.isEmpty())
            return nullptr;
        Fn fn = freeFunctions.dequeue();
        return fn;
    }
    storage<Callable>* value(Fn fn){
        if(hashes.contains(qintptr(fn))){
            size_t hash = hashes[qintptr(fn)];
            if(s.contains(hash))
                return s[hash];
        }
        return nullptr;
    }

    static Ret caller(Fn fn, Args...args);
private:
    QHash<qintptr,size_t> hashes;
    QQueue<Fn> freeFunctions;
    QHash<size_t,storage<Callable>*> s;
};

template<ushort n, ushort count, typename Callable, typename Ret, typename... Args>
struct CallableHashInitializer{
    typedef typename QtJambiAPI::FunctionType<Ret,Args...>::type Fn;

    static Ret call(Args...args){
#if Q_CC_MSVC && !defined(QT_DEBUG)
        // required because msvc merges the templates as code optimization
        return CallableHash<count, Callable, Ret, Args...>::caller(Fn(template_keep_dummy(QFunctionPointer(&call), n)), std::forward<Args>(args)...);
#endif
        return CallableHash<count, Callable, Ret, Args...>::caller(&call, std::forward<Args>(args)...);
    }

    static void initialize(QQueue<Fn>& freeFunctions){
        freeFunctions.enqueue(&call);
        CallableHashInitializer<n+1, count, Callable, Ret, Args...>::initialize(freeFunctions);
    }
};

template<ushort count, typename Callable, typename Ret, typename... Args>
struct CallableHashInitializer<count, count, Callable, Ret, Args...>{
    typedef typename QtJambiAPI::FunctionType<Ret,Args...>::type Fn;
    static void initialize(QQueue<Fn>&){}
};

template<ushort count, typename Callable, typename Ret, typename... Args>
CallableHash<count, Callable,Ret,Args...>::CallableHash() : hashes(), freeFunctions(), s()
{
    freeFunctions.reserve(count);
    CallableHashInitializer<0, count, Callable, Ret, Args...>::initialize(freeFunctions);
    registerFunctionPointerCleanup(this, &CallableHash::cleanup);
}

template<ushort count, typename Callable, typename Ret, typename... Args>
CallableHash<count, Callable,Ret,Args...>::~CallableHash()
{
    unregisterFunctionPointerCleanup(this);
}

template<ushort count, typename Callable, typename Ret, typename... Args>
Ret CallableHash<count, Callable,Ret,Args...>::caller(Fn fn, Args...args){
    storage<Callable>* stor = instance().value(fn);
    if(!stor){
        noFunctionAvailable(typeid(Ret(*)(Args...)));
        if constexpr(std::is_same<Ret,void>::value){
            return;
        }else if constexpr(std::is_pointer<Ret>::value || std::is_null_pointer<Ret>::value){
            return nullptr;
        }else if constexpr(std::is_integral<Ret>::value || std::is_floating_point<Ret>::value){
            return Ret{0};
        }else if constexpr(std::is_default_constructible<Ret>::value){
            return Ret{};
        }else{
            throw std::bad_function_call();
        }
    }
    return stor->callable(std::forward<Args>(args)...);
};

template<ushort count, typename Callable, typename Ret, typename... Args>
CallableHash<count,Callable,Ret,Args...>& CallableHash<count,Callable,Ret,Args...>::instance(){
    static CallableHash<count,Callable,Ret,Args...> _instance;
    return _instance;
}


template<ushort count, typename Callable, typename Ret, typename... Args>
typename QtJambiAPI::FunctionType<Ret,Args...>::type functionPointer(Callable&& c, size_t hash, std::function<void()>* deleterFunction, std::function<const Callable*(Ret(*)(Args...))>* reverseFunctionGetter)
{
    typedef typename QtJambiAPI::FunctionType<Ret,Args...>::type Fn;
    CallableHash<count,Callable,Ret,Args...>& callables = CallableHash<count,Callable,Ret,Args...>::instance();
    QMutexLocker locker(functionPointerLock());
    Fn fn = callables.functionOf(hash);
    if(!fn){
        fn = callables.next();
        if(!fn){
            FunctionPointerDisposer disposer = [](QFunctionPointer fn){
                QMutexLocker locker(functionPointerLock());
                CallableHash<count,Callable,Ret,Args...>::instance().dispose(Fn(fn));
            };
            fn = Fn(extractFunction(QVector<FunctionParamTypeInfo>{FunctionParamTypeInfo::create<Ret>(), FunctionParamTypeInfo::create<Args>()...},
                                             typeid(Ret(Args...)),
                                             disposer,
                                             QFunctionPointer(CallableHash<count,Callable,Ret,Args...>::caller)));
        }
        if(fn)
            callables.insert(fn, hash, std::forward<Callable>(c));
    }
    if(fn){
        if(reverseFunctionGetter){
            *reverseFunctionGetter = [](Fn fn) -> const Callable* {
                QMutexLocker locker(functionPointerLock());
                if(storage<Callable>* stor = CallableHash<count,Callable,Ret,Args...>::instance().value(fn)){
                    return &stor->callable;
                }
                return nullptr;
            };
        }
        if(deleterFunction){
            *deleterFunction = [hash](){
                QMutexLocker locker(functionPointerLock());
                CallableHash<count,Callable,Ret,Args...>::instance().remove(hash);
            };
        }
    }
    return fn;
}

} // namespace FunctionPointerPrivate

template<ushort count, typename Fn, typename Callable>
Fn* qtjambi_function_pointer(Callable&& c, size_t hash = 0, std::function<void()>* deleterFunction = nullptr, std::function<const Callable*(Fn*)>* reverseFunctionGetter = nullptr)
{
    return FunctionPointerPrivate::functionPointer<count>(std::move(c), hash, deleterFunction, reverseFunctionGetter);
}

#endif // QTJAMBI_FUNCTIONPOINTER_H
