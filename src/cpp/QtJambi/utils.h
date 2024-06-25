/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "global.h"

enum class jValueType;

class QtJambiScope;

struct InternalToExternalConverterPrivate;
struct QHashFunctionPrivate;
struct ExternalToInternalConverterPrivate;
struct RunnablePrivate;

namespace QtJambiUtils{

class QTJAMBI_EXPORT InternalToExternalConverter{
    typedef void(*Deleter)(void*);
    typedef bool(*Invoker)(void*, JNIEnv*, QtJambiScope*, const void*, jvalue&, bool);
public:
    typedef bool(*FunctionPointer)(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool);

private:
    explicit InternalToExternalConverter(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    InternalToExternalConverter() noexcept;
    ~InternalToExternalConverter() noexcept;
    InternalToExternalConverter(const InternalToExternalConverter& other) noexcept;
    InternalToExternalConverter(InternalToExternalConverter&& other) noexcept;
    InternalToExternalConverter(FunctionPointer functor) noexcept;
    inline InternalToExternalConverter(nullptr_t) noexcept : InternalToExternalConverter(FunctionPointer(nullptr)) {}

    InternalToExternalConverter& operator=(const InternalToExternalConverter& other) noexcept;
    InternalToExternalConverter& operator=(InternalToExternalConverter&& other) noexcept;

    template<typename Functor, typename std::enable_if<!std::is_pointer<Functor>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, InternalToExternalConverter>::value, bool>::type = true
                             , typename std::enable_if<!std::is_null_pointer<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, FunctionPointer>::value, bool>::type = true
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                             , typename std::enable_if<std::is_invocable_r<bool, Functor, JNIEnv*, QtJambiScope*, const void*, jvalue&, bool>::value, bool>::type = true
#endif
    >
    InternalToExternalConverter(Functor&& functor) noexcept
        : InternalToExternalConverter(
            new typename std::remove_reference<typename std::remove_cv<Functor>::type>::type(std::move(functor)),
            [](void* data, JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType){
                typename std::remove_reference<typename std::remove_cv<Functor>::type>::type* fct = reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
                return (*fct)(env, scope, in, out, forceBoxedType);
            },
            [](void* data){
                delete reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
            }
            ){}

    bool operator==(const InternalToExternalConverter& other) const noexcept;
    bool operator()(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    friend InternalToExternalConverterPrivate;
    QExplicitlySharedDataPointer<InternalToExternalConverterPrivate> d;
};

class QTJAMBI_EXPORT ExternalToInternalConverter{
    typedef void(*Deleter)(void*);
    typedef bool(*Invoker)(void*, JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType);
public:
    typedef bool(*FunctionPointer)(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType);

private:
    explicit ExternalToInternalConverter(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    ExternalToInternalConverter() noexcept;
    ~ExternalToInternalConverter() noexcept;
    ExternalToInternalConverter(const ExternalToInternalConverter& other) noexcept;
    ExternalToInternalConverter(ExternalToInternalConverter&& other) noexcept;
    ExternalToInternalConverter(FunctionPointer functor) noexcept;
    inline ExternalToInternalConverter(nullptr_t) noexcept : ExternalToInternalConverter(FunctionPointer(nullptr)) {}

    ExternalToInternalConverter& operator=(const ExternalToInternalConverter& other) noexcept;
    ExternalToInternalConverter& operator=(ExternalToInternalConverter&& other) noexcept;

    template<typename Functor, typename std::enable_if<!std::is_pointer<Functor>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, ExternalToInternalConverter>::value, bool>::type = true
                             , typename std::enable_if<!std::is_null_pointer<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, FunctionPointer>::value, bool>::type = true
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                             , typename std::enable_if<std::is_invocable_r<bool, Functor, JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType>::value, bool>::type = true
#endif
    >
    ExternalToInternalConverter(Functor&& functor) noexcept
        : ExternalToInternalConverter(
            new typename std::remove_reference<typename std::remove_cv<Functor>::type>::type(std::move(functor)),
            [](void* data, JNIEnv* env, QtJambiScope* scope, jvalue in, void*& out, jValueType type){
                typename std::remove_reference<typename std::remove_cv<Functor>::type>::type* fct = reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
                return (*fct)(env, scope, in, out, type);
            },
            [](void* data){
                delete reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
            }
            ){}

    bool operator==(const ExternalToInternalConverter& other) const noexcept;
    bool operator()(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    QExplicitlySharedDataPointer<ExternalToInternalConverterPrivate> d;
    friend ExternalToInternalConverterPrivate;
};

class QTJAMBI_EXPORT QHashFunction{
    typedef void(*Deleter)(void*);
    typedef hash_type(*Invoker)(void*, const void*,hash_type);
public:
    typedef hash_type(*FunctionPointer)(const void*,hash_type);

private:
    explicit QHashFunction(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    QHashFunction() noexcept;
    ~QHashFunction() noexcept;
    QHashFunction(const QHashFunction& other) noexcept;
    QHashFunction(QHashFunction&& other) noexcept;
    QHashFunction(FunctionPointer functor) noexcept;
    inline QHashFunction(nullptr_t) noexcept : QHashFunction(FunctionPointer(nullptr)) {}

    QHashFunction& operator=(const QHashFunction& other) noexcept;
    QHashFunction& operator=(QHashFunction&& other) noexcept;

    template<typename Functor, typename std::enable_if<!std::is_pointer<Functor>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, QHashFunction>::value, bool>::type = true
                             , typename std::enable_if<!std::is_null_pointer<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, FunctionPointer>::value, bool>::type = true
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                             , typename std::enable_if<std::is_invocable_r<hash_type, Functor, const void*, hash_type>::value, bool>::type = true
#endif
    >
    QHashFunction(Functor&& functor) noexcept
        : QHashFunction(
            new typename std::remove_reference<typename std::remove_cv<Functor>::type>::type(std::move(functor)),
            [](void* data, const void* ptr, hash_type seed) -> hash_type{
                typename std::remove_reference<typename std::remove_cv<Functor>::type>::type* fct = reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
                return (*fct)(ptr, seed);
            },
            [](void* data){
                delete reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
            }
            ){}
    bool operator==(const QHashFunction& other) const noexcept;
    hash_type operator()(const void*, hash_type) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    friend QHashFunctionPrivate;
    QExplicitlySharedDataPointer<QHashFunctionPrivate> d;
};

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
    inline Runnable(nullptr_t) noexcept : Runnable(FunctionPointer(nullptr)) {}

    Runnable& operator=(const Runnable& other) noexcept;
    Runnable& operator=(Runnable&& other) noexcept;

    template<typename Functor, typename std::enable_if<!std::is_pointer<Functor>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, Runnable>::value, bool>::type = true
                             , typename std::enable_if<!std::is_null_pointer<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::value, bool>::type = true
                             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, FunctionPointer>::value, bool>::type = true
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                             , typename std::enable_if<std::is_invocable<Functor>::value, bool>::type = true
#endif
    >
    Runnable(Functor&& functor) noexcept
        : Runnable(
            new typename std::remove_reference<typename std::remove_cv<Functor>::type>::type(std::move(functor)),
            [](void* data){
                typename std::remove_reference<typename std::remove_cv<Functor>::type>::type* fct = reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
                (*fct)();
            },
            [](void* data){
                delete reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
            }
            ){}
    bool operator==(const Runnable& other) const noexcept;
    void operator()() const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
    template<typename T>
    static Runnable deleter(T* t){
        return Runnable(
            reinterpret_cast<void*>(t),
            [](void* data){
                T* ptr = reinterpret_cast<T*>(data);
                delete ptr;
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
