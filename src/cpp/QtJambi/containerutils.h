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

#if !defined(QTJAMBI_CONTAINERUTILS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_CONTAINERUTILS_H

#include "global.h"

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
    InternalToExternalConverter() noexcept;
    ~InternalToExternalConverter() noexcept;
    InternalToExternalConverter(const InternalToExternalConverter& other) noexcept;
    InternalToExternalConverter(InternalToExternalConverter&& other) noexcept;
    InternalToExternalConverter(FunctionPointer functor) noexcept;
    inline InternalToExternalConverter(std::nullptr_t) noexcept : InternalToExternalConverter(FunctionPointer(nullptr)) {}

    InternalToExternalConverter& operator=(const InternalToExternalConverter& other) noexcept;
    InternalToExternalConverter& operator=(InternalToExternalConverter&& other) noexcept;

    template<typename Functor, std::enable_if_t<!std::is_pointer_v<Functor>, bool> = true
             , std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Functor>>, InternalToExternalConverter>, bool> = true
             , std::enable_if_t<!std::is_null_pointer_v<std::remove_cv_t<std::remove_reference_t<Functor>>>, bool> = true
             , std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Functor>>, FunctionPointer>, bool> = true
             , std::enable_if_t<std::is_invocable_r_v<bool, Functor, JNIEnv*, QtJambiScope*, const void*, jvalue&, bool>, bool> = true
             >
    InternalToExternalConverter(Functor&& functor) noexcept
        : InternalToExternalConverter(
              new std::remove_cv_t<std::remove_reference_t<Functor>>(std::move(functor)),
              [](void* data, JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType){
                  std::remove_cv_t<std::remove_reference_t<Functor>>* fct = reinterpret_cast<std::remove_cv_t<std::remove_reference_t<Functor>>*>(data);
                  return (*fct)(env, scope, in, out, forceBoxedType);
              },
              [](void* data){
                  delete reinterpret_cast<std::remove_cv_t<std::remove_reference_t<Functor>>*>(data);
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

class ExternalToInternalConverter{
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
    inline ExternalToInternalConverter(std::nullptr_t) noexcept : ExternalToInternalConverter(FunctionPointer(nullptr)) {}

    ExternalToInternalConverter& operator=(const ExternalToInternalConverter& other) noexcept;
    ExternalToInternalConverter& operator=(ExternalToInternalConverter&& other) noexcept;

    template<typename Functor, std::enable_if_t<!std::is_pointer_v<Functor>, bool> = true
             , std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Functor>>, ExternalToInternalConverter>, bool> = true
             , std::enable_if_t<!std::is_null_pointer_v<std::remove_cv_t<std::remove_reference_t<Functor>>>, bool> = true
             , std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Functor>>, FunctionPointer>, bool> = true
             , std::enable_if_t<std::is_invocable_r_v<bool, Functor, JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType>, bool> = true
             >
    ExternalToInternalConverter(Functor&& functor) noexcept
        : ExternalToInternalConverter(
              new std::remove_cv_t<std::remove_reference_t<Functor>>(std::move(functor)),
              [](void* data, JNIEnv* env, QtJambiScope* scope, jvalue in, void*& out, jValueType type){
                  std::remove_cv_t<std::remove_reference_t<Functor>>* fct = reinterpret_cast<std::remove_cv_t<std::remove_reference_t<Functor>>*>(data);
                  return (*fct)(env, scope, in, out, type);
              },
              [](void* data){
                  delete reinterpret_cast<std::remove_cv_t<std::remove_reference_t<Functor>>*>(data);
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

class QHashFunction{
    typedef void(*Deleter)(void*);
    typedef size_t(*Invoker)(void*, const void*,size_t);
public:
    typedef size_t(*FunctionPointer)(const void*,size_t);

private:
    explicit QHashFunction(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    QHashFunction() noexcept;
    ~QHashFunction() noexcept;
    QHashFunction(const QHashFunction& other) noexcept;
    QHashFunction(QHashFunction&& other) noexcept;
    QHashFunction(FunctionPointer functor) noexcept;
    inline QHashFunction(std::nullptr_t) noexcept : QHashFunction(FunctionPointer(nullptr)) {}

    QHashFunction& operator=(const QHashFunction& other) noexcept;
    QHashFunction& operator=(QHashFunction&& other) noexcept;

    template<typename Functor, std::enable_if_t<!std::is_pointer_v<Functor>, bool> = true
             , std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Functor>>, QHashFunction>, bool> = true
             , std::enable_if_t<!std::is_null_pointer_v<std::remove_cv_t<std::remove_reference_t<Functor>>>, bool> = true
             , std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Functor>>, FunctionPointer>, bool> = true
             , std::enable_if_t<std::is_invocable_r_v<size_t, Functor, const void*, size_t>, bool> = true
             >
    QHashFunction(Functor&& functor) noexcept
        : QHashFunction(
              new std::remove_cv_t<std::remove_reference_t<Functor>>(std::move(functor)),
              [](void* data, const void* ptr, size_t seed) -> size_t{
                  std::remove_cv_t<std::remove_reference_t<Functor>>* fct = reinterpret_cast<std::remove_cv_t<std::remove_reference_t<Functor>>*>(data);
                  return (*fct)(ptr, seed);
              },
              [](void* data){
                  delete reinterpret_cast<std::remove_cv_t<std::remove_reference_t<Functor>>*>(data);
              }
              ){}
    bool operator==(const QHashFunction& other) const noexcept;
    size_t operator()(const void*, size_t) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    friend QHashFunctionPrivate;
    QExplicitlySharedDataPointer<QHashFunctionPrivate> d;
};
}// namespace QtJambiUtils

#endif // QTJAMBI_CONTAINERUTILS_H
