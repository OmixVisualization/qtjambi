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

#if !defined(QTJAMBI_UTILS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_UTILS_H

#include <functional>
#include <QtCore/QExplicitlySharedDataPointer>
#include <QtCore/QPointer>
#include "global.h"

namespace QtJambiPrivate {

template<typename T, typename = void>
struct is_complete : std::false_type {};

template<typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

template<typename O>
constexpr bool is_complete_v = is_complete<O>::value;

template<typename O>
struct qtjambi_cast_result{
    using type = std::conditional_t<std::is_array_v<O>, std::decay_t<O>, O>;
};

template<typename... Args>
struct qtjambi_cast_enabled_test;

template<typename... Args>
static constexpr bool test_qtjambi_cast_enabled() {
    if constexpr(is_complete_v<qtjambi_cast_enabled_test<Args...>>){
        return qtjambi_cast_enabled_test<Args...>::value;
    }else{
        return true;
    }
}

template<class O, typename... Args>
struct qtjambi_cast_impl;

template<typename O>
struct jni_type;

}

template<typename O, typename... Args>
using qtjambi_cast_result_t = std::enable_if_t<QtJambiPrivate::test_qtjambi_cast_enabled<Args...>(), typename QtJambiPrivate::qtjambi_cast_result<O>::type>;

template<class O, typename... Args>
static constexpr auto find_qtjambi_cast_impl() {
    constexpr bool hasCastImpl = QtJambiPrivate::is_complete_v< QtJambiPrivate::qtjambi_cast_impl<O,Args...> >;
    Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/Cast>");
    return QtJambiPrivate::qtjambi_cast_impl<O, Args...>{};
}

template<typename O, typename... Args>
using qtjambi_cast_impl = decltype(find_qtjambi_cast_impl<O,Args...>());

template<class O, typename... Args>
constexpr qtjambi_cast_result_t<O,Args...> qtjambi_cast(Args&&... args){
    return qtjambi_cast_impl<O, Args...>::cast(std::forward<Args>(args)...);
}

class QtJambiScope;

template<class O, typename... Args>
constexpr qtjambi_cast_result_t<O,JNIEnv*,Args...> qtjambi_cast(JNIEnv *env, Args&&... args){
    return qtjambi_cast_impl<O,Args...,JNIEnv*>::cast(std::forward<Args>(args)..., env);
}

template<class O, typename... Args>
constexpr qtjambi_cast_result_t<O,QtJambiScope&,Args...> qtjambi_cast(QtJambiScope& scope, Args&&... args){
    return qtjambi_cast_impl<O,Args...,QtJambiScope&>::cast(std::forward<Args>(args)..., scope);
}

template<class O, typename... Args>
constexpr qtjambi_cast_result_t<O,JNIEnv*,QtJambiScope&,Args...> qtjambi_cast(JNIEnv *env, QtJambiScope& scope, Args&&... args){
    return qtjambi_cast_impl<O,Args...,JNIEnv*,QtJambiScope&>::cast(std::forward<Args>(args)..., env, scope);
}

struct RunnablePrivate;

namespace QtJambiUtils{

class QTJAMBI_EXPORT Runnable{
    typedef void(*Deleter)(void*);
    typedef void(*Invoker)(void*);
public:
    typedef void(*FunctionPointer)();

private:
    explicit Runnable(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    Runnable() noexcept;
    ~Runnable() noexcept;
    Runnable(const Runnable& other) noexcept;
    Runnable(Runnable&& other) noexcept;
    Runnable(FunctionPointer functor) noexcept;
    inline Runnable(std::nullptr_t) noexcept : Runnable(FunctionPointer(nullptr)) {}

    Runnable& operator=(const Runnable& other) noexcept;
    Runnable& operator=(Runnable&& other) noexcept;

    template<typename Functor, std::enable_if_t<!std::is_pointer_v<Functor>, bool> = true
                             , std::enable_if_t<!std::is_same_v<std::remove_reference_t<std::remove_cv_t<Functor>>, Runnable>, bool> = true
                             , std::enable_if_t<!std::is_null_pointer_v<std::remove_reference_t<std::remove_cv_t<Functor>>>, bool> = true
                             , std::enable_if_t<!std::is_same_v<std::remove_reference_t<std::remove_cv_t<Functor>>, FunctionPointer>, bool> = true
                             , std::enable_if_t<std::is_invocable_v<Functor>, bool> = true
    >
    Runnable(Functor&& functor) noexcept
        : Runnable(
            new std::remove_reference_t<std::remove_cv_t<Functor>>(std::move(functor)),
            [](void* data){
                std::remove_reference_t<std::remove_cv_t<Functor>>* fct = reinterpret_cast<std::remove_reference_t<std::remove_cv_t<Functor>>*>(data);
                (*fct)();
            },
            [](void* data){
                delete reinterpret_cast<std::remove_reference_t<std::remove_cv_t<Functor>>*>(data);
            }
            ){}
    bool operator==(const Runnable& other) const noexcept;
    void operator()() const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
    template<typename T>
    static Runnable deleter(T* t){
        if constexpr(std::is_base_of_v<QObject, T>){
            return Runnable(
                reinterpret_cast<void*>(new QPointer<T>(t)),
                [](void* data){
                    QPointer<T>* pointer = reinterpret_cast<QPointer<T>*>(data);
                    if(T* ptr = pointer->get())
                        delete ptr;
                    delete pointer;
                }, nullptr);
        }else{
            return Runnable(
                reinterpret_cast<void*>(t),
                [](void* data){
                    T* ptr = reinterpret_cast<T*>(data);
                    delete ptr;
                }, nullptr);
        }
    }
    template<typename T>
    static Runnable deleter(QScopedArrayPointer<T>&& t){
        return Runnable(
            reinterpret_cast<void*>(new QScopedArrayPointer<T>(std::move(t))),
            [](void* data){
                QScopedArrayPointer<T>* pointer = reinterpret_cast<QScopedArrayPointer<T>*>(data);
                delete pointer;
            }, nullptr);
    }
    template<typename T>
    static Runnable deleter(QScopedPointer<T>&& t){
        return Runnable(
            reinterpret_cast<void*>(new QScopedPointer<T>(std::move(t))),
            [](void* data){
                QScopedPointer<T>* pointer = reinterpret_cast<QScopedPointer<T>*>(data);
                delete pointer;
            }, nullptr);
    }
    template<typename T>
    static Runnable deleter(std::unique_ptr<T>&& t){
        return Runnable(
            reinterpret_cast<void*>(new std::unique_ptr<T>(std::move(t))),
            [](void* data){
                std::unique_ptr<T>* pointer = reinterpret_cast<std::unique_ptr<T>*>(data);
                delete pointer;
            }, nullptr);
    }
    template<typename T>
    static Runnable arrayDeleter(T* t){
        return Runnable(
            reinterpret_cast<void*>(t),
            [](void* data){
                T* ptr = reinterpret_cast<T*>(data);
                delete[] ptr;
            }, nullptr);
    }
private:
    friend RunnablePrivate;
    QExplicitlySharedDataPointer<RunnablePrivate> d;
};

} // namespace QtJambiUtils

#endif // QTJAMBI_UTILS_H
