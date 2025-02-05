/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if !defined(QTJAMBI_TYPEUTILS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_TYPEUTILS_H

#include "global.h"

#if defined(QTJAMBI_GENERIC_ACCESS)
#define TYPEUTILS_EXPORT QTJAMBI_EXPORT
#else
#define TYPEUTILS_EXPORT
#endif

enum class jValueType;
class QtJambiScope;
struct InternalToExternalConverterPrivate;
struct QHashFunctionPrivate;
struct ExternalToInternalConverterPrivate;

namespace QtJambiUtils{
class InternalToExternalConverter{
    typedef void(*Deleter)(void*);
    typedef bool(*Invoker)(void*, JNIEnv*, QtJambiScope*, const void*, jvalue&, bool);
public:
    typedef bool(*FunctionPointer)(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool);

private:
    explicit InternalToExternalConverter(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    TYPEUTILS_EXPORT InternalToExternalConverter() noexcept;
    TYPEUTILS_EXPORT ~InternalToExternalConverter() noexcept;
    TYPEUTILS_EXPORT InternalToExternalConverter(const InternalToExternalConverter& other) noexcept;
    TYPEUTILS_EXPORT InternalToExternalConverter(InternalToExternalConverter&& other) noexcept;
    TYPEUTILS_EXPORT InternalToExternalConverter(FunctionPointer functor) noexcept;
    inline InternalToExternalConverter(nullptr_t) noexcept : InternalToExternalConverter(FunctionPointer(nullptr)) {}

    TYPEUTILS_EXPORT InternalToExternalConverter& operator=(const InternalToExternalConverter& other) noexcept;
    TYPEUTILS_EXPORT InternalToExternalConverter& operator=(InternalToExternalConverter&& other) noexcept;

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

    TYPEUTILS_EXPORT bool operator==(const InternalToExternalConverter& other) const noexcept;
    TYPEUTILS_EXPORT bool operator()(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool) const;
    TYPEUTILS_EXPORT operator bool() const noexcept;
    TYPEUTILS_EXPORT bool operator !() const noexcept;
private:
    friend InternalToExternalConverterPrivate;
    QExplicitlySharedDataPointer<InternalToExternalConverterPrivate> d;
};

class ExternalToInternalConverter{
    typedef void(*Deleter)(void*);
    typedef bool(*Invoker)(void*, JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType);
public:
    typedef bool(*FunctionPointer)(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType);

private:
    explicit ExternalToInternalConverter(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    TYPEUTILS_EXPORT ExternalToInternalConverter() noexcept;
    TYPEUTILS_EXPORT ~ExternalToInternalConverter() noexcept;
    TYPEUTILS_EXPORT ExternalToInternalConverter(const ExternalToInternalConverter& other) noexcept;
    TYPEUTILS_EXPORT ExternalToInternalConverter(ExternalToInternalConverter&& other) noexcept;
    TYPEUTILS_EXPORT ExternalToInternalConverter(FunctionPointer functor) noexcept;
    inline ExternalToInternalConverter(nullptr_t) noexcept : ExternalToInternalConverter(FunctionPointer(nullptr)) {}

    TYPEUTILS_EXPORT ExternalToInternalConverter& operator=(const ExternalToInternalConverter& other) noexcept;
    TYPEUTILS_EXPORT ExternalToInternalConverter& operator=(ExternalToInternalConverter&& other) noexcept;

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

    TYPEUTILS_EXPORT bool operator==(const ExternalToInternalConverter& other) const noexcept;
    TYPEUTILS_EXPORT bool operator()(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) const;
    TYPEUTILS_EXPORT operator bool() const noexcept;
    TYPEUTILS_EXPORT bool operator !() const noexcept;
private:
    QExplicitlySharedDataPointer<ExternalToInternalConverterPrivate> d;
    friend ExternalToInternalConverterPrivate;
};

class QHashFunction{
    typedef void(*Deleter)(void*);
    typedef hash_type(*Invoker)(void*, const void*,hash_type);
public:
    typedef hash_type(*FunctionPointer)(const void*,hash_type);

private:
    explicit QHashFunction(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    TYPEUTILS_EXPORT QHashFunction() noexcept;
    TYPEUTILS_EXPORT ~QHashFunction() noexcept;
    TYPEUTILS_EXPORT QHashFunction(const QHashFunction& other) noexcept;
    TYPEUTILS_EXPORT QHashFunction(QHashFunction&& other) noexcept;
    TYPEUTILS_EXPORT QHashFunction(FunctionPointer functor) noexcept;
    inline QHashFunction(nullptr_t) noexcept : QHashFunction(FunctionPointer(nullptr)) {}

    TYPEUTILS_EXPORT QHashFunction& operator=(const QHashFunction& other) noexcept;
    TYPEUTILS_EXPORT QHashFunction& operator=(QHashFunction&& other) noexcept;

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
    TYPEUTILS_EXPORT bool operator==(const QHashFunction& other) const noexcept;
    TYPEUTILS_EXPORT hash_type operator()(const void*, hash_type) const;
    TYPEUTILS_EXPORT operator bool() const noexcept;
    TYPEUTILS_EXPORT bool operator !() const noexcept;
private:
    friend QHashFunctionPrivate;
    QExplicitlySharedDataPointer<QHashFunctionPrivate> d;
};
}// namespace QtJambiUtils

#endif // QTJAMBI_TYPEUTILS_H
