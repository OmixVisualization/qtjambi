/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

class QTJAMBI_EXPORT InternalToExternalConverterPrivate : public QSharedData{
protected:
    InternalToExternalConverterPrivate() noexcept;
public:
    virtual ~InternalToExternalConverterPrivate();
    virtual bool invoke(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool) const = 0;
    friend class InternalToExternalConverter;
};

class QTJAMBI_EXPORT InternalToExternalConverter{
public:
    typedef bool(*FunctionPointer)(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool);

private:
    explicit InternalToExternalConverter(InternalToExternalConverterPrivate* _d) noexcept;
    template<typename Functor, bool = std::is_assignable<FunctionPointer&, Functor>::value, bool = std::is_same<Functor, InternalToExternalConverter>::value>
    class Data : public InternalToExternalConverterPrivate{
    public:
        inline static InternalToExternalConverterPrivate* from(Functor&& functor){
            return new Data(std::forward<Functor>(functor));
        }
        inline bool invoke(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType) const override {
            return m_functor(env, scope, in, out, forceBoxedType);
        }
    private:
        inline Data(Functor&& functor) noexcept : m_functor(std::forward<Functor>(functor)){}
        Functor m_functor;
    };
    template<typename Functor>
    struct Data<Functor,false,true>{
        inline static const InternalToExternalConverter& from(const InternalToExternalConverter& function){
            return function;
        }
        inline static InternalToExternalConverter&& from(InternalToExternalConverter&& function){
            return std::move(function);
        }
    };
    template<typename Functor>
    struct Data<Functor,true,false>{
        inline static FunctionPointer from(Functor&& functor){
            return FunctionPointer(functor);
        }
    };
public:
    InternalToExternalConverter() noexcept;
    InternalToExternalConverter(const InternalToExternalConverter& other) noexcept;
    InternalToExternalConverter(InternalToExternalConverter&& other) noexcept;
    InternalToExternalConverter(FunctionPointer functor) noexcept;

    InternalToExternalConverter& operator=(const InternalToExternalConverter& other) noexcept;
    InternalToExternalConverter& operator=(InternalToExternalConverter& other) noexcept;
    InternalToExternalConverter& operator=(InternalToExternalConverter&& other) noexcept;

    template<typename Functor
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             , typename = std::enable_if_t<std::is_invocable_r_v<bool, Functor, JNIEnv*, QtJambiScope*, const void*, jvalue&, bool>>
#endif
             >
    InternalToExternalConverter(Functor&& functor) noexcept
        : InternalToExternalConverter(Data<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::from(std::forward<Functor>(functor))){}

    bool operator==(const InternalToExternalConverter& other) const noexcept;
    bool operator()(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    template<typename, bool, bool> friend class Data;
    QExplicitlySharedDataPointer<InternalToExternalConverterPrivate> d;
};

class QTJAMBI_EXPORT ExternalToInternalConverterPrivate : public QSharedData{
protected:
    ExternalToInternalConverterPrivate() noexcept;
public:
    virtual ~ExternalToInternalConverterPrivate();
    virtual bool invoke(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) const = 0;
    friend class ExternalToInternalConverter;
};

class QTJAMBI_EXPORT ExternalToInternalConverter{
public:
    typedef bool(*FunctionPointer)(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType);

private:
    explicit ExternalToInternalConverter(ExternalToInternalConverterPrivate* _d) noexcept;
    template<typename Functor, bool = std::is_assignable<FunctionPointer&, Functor>::value, bool = std::is_same<Functor, ExternalToInternalConverter>::value>
    class Data : public ExternalToInternalConverterPrivate{
    public:
        ~Data() noexcept {}
        inline static ExternalToInternalConverterPrivate* from(Functor&& functor){
            return new Data(std::forward<Functor>(functor));
        }
        inline bool invoke(JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) const override {
            return m_functor(env, scope, val, out, valueType);
        }
    private:
        inline Data(Functor&& functor) noexcept : m_functor(std::forward<Functor>(functor)){}
        Functor m_functor;
    };
    template<typename Functor>
    struct Data<Functor,false,true>{
        inline static const ExternalToInternalConverter& from(const ExternalToInternalConverter& function){
            return function;
        }
        inline static ExternalToInternalConverter&& from(ExternalToInternalConverter&& function){
            return std::move(function);
        }
    };
    template<typename Functor>
    struct Data<Functor,true,false>{
        inline static FunctionPointer from(Functor&& functor){
            return FunctionPointer(functor);
        }
    };
public:
    ExternalToInternalConverter() noexcept;
    ExternalToInternalConverter(const ExternalToInternalConverter& other) noexcept;
    ExternalToInternalConverter(ExternalToInternalConverter&& other) noexcept;
    ExternalToInternalConverter(FunctionPointer functor) noexcept;

    ExternalToInternalConverter& operator=(const ExternalToInternalConverter& other) noexcept;
    ExternalToInternalConverter& operator=(ExternalToInternalConverter& other) noexcept;
    ExternalToInternalConverter& operator=(ExternalToInternalConverter&& other) noexcept;

    template<typename Functor
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    , typename = std::enable_if_t<std::is_invocable_r_v<bool, Functor, JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType>>
#endif
          >
    ExternalToInternalConverter(Functor&& functor) noexcept
        : ExternalToInternalConverter(Data<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::from(std::forward<Functor>(functor))){}

    bool operator==(const ExternalToInternalConverter& other) const noexcept;
    bool operator()(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    template<typename, bool, bool> friend class Data;
    QExplicitlySharedDataPointer<ExternalToInternalConverterPrivate> d;
};

class QTJAMBI_EXPORT QHashFunctionPrivate : public QSharedData{
protected:
    QHashFunctionPrivate() noexcept;
public:
    virtual ~QHashFunctionPrivate();
    virtual hash_type invoke(const void*, hash_type) const = 0;
    friend class QHashFunction;
};

class QTJAMBI_EXPORT QHashFunction{
public:
    typedef hash_type(*FunctionPointer)(const void*,hash_type);

private:
    explicit QHashFunction(QHashFunctionPrivate* _d) noexcept;
    template<typename Functor, bool = std::is_assignable<FunctionPointer&, Functor>::value, bool = std::is_same<Functor, QHashFunction>::value>
    class Data : public QHashFunctionPrivate{
    public:
        ~Data() noexcept {}
        inline hash_type invoke(const void* ptr, hash_type seed) const override {
            return m_functor(ptr, seed);
        }
        inline static QHashFunctionPrivate* from(Functor&& functor){
            return new Data(std::forward<Functor>(functor));
        }
    private:
        inline Data(Functor&& functor) noexcept : m_functor(std::forward<Functor>(functor)){}
        Functor m_functor;
    };
    template<typename Functor>
    struct Data<Functor,false,true>{
        inline static const QHashFunction& from(const QHashFunction& function){
            return function;
        }
        inline static QHashFunction&& from(QHashFunction&& function){
            return std::move(function);
        }
    };
    template<typename Functor>
    struct Data<Functor,true,false>{
        inline static FunctionPointer from(Functor&& functor){
            return FunctionPointer(functor);
        }
    };
public:
    QHashFunction() noexcept;
    QHashFunction(const QHashFunction& other) noexcept;
    QHashFunction(QHashFunction&& other) noexcept;
    QHashFunction(FunctionPointer functor) noexcept;

    QHashFunction& operator=(const QHashFunction& other) noexcept;
    QHashFunction& operator=(QHashFunction& other) noexcept;
    QHashFunction& operator=(QHashFunction&& other) noexcept;

    template<typename Functor
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    , typename = std::enable_if_t<std::is_invocable_r_v<hash_type, Functor, const void*, hash_type>>
#endif
             >
    QHashFunction(Functor&& functor) noexcept
        : QHashFunction(Data<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::from(std::forward<Functor>(functor))){}
    bool operator==(const QHashFunction& other) const noexcept;
    hash_type operator()(const void*, hash_type) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    template<typename, bool, bool> friend class Data;
    QExplicitlySharedDataPointer<QHashFunctionPrivate> d;
};

namespace QtJambiPrivate{

}

#endif // QTJAMBI_UTILS_H
