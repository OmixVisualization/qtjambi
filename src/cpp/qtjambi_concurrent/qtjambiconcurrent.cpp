/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#define QFUTURE_TEST
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_registry.h>
#include "qtjambiconcurrent_p.h"
#include "qtjambi_concurrent_repository.h"

#include <QtConcurrent/QtConcurrent>
#include <qtjambi/qtjambi_cast.h>

#ifndef QT_NO_CONCURRENT

class Functor {
public:
    Functor(JNIEnv *env, jobject functor) : m_functor(env, functor)
    {
    }

    Functor(const Functor &other) : m_functor(other.m_functor)
    {
    }

    Functor(Functor &&other) : m_functor(std::move(other.m_functor))
    {
    }

    Functor& operator=(const Functor &other) = delete;
    Functor& operator=(Functor &&other) = delete;

    virtual ~Functor()
    {
    }

protected:
    const JObjectWrapper m_functor;

private:
};

class MapFunctor: public Functor {
public:
    MapFunctor(JNIEnv *env, jobject javaMapFunctor) : Functor(env, javaMapFunctor) {}
    MapFunctor(const MapFunctor &other) : Functor(other) {}
    MapFunctor(MapFunctor &&other) : Functor(std::move(other)) {}

    void operator ()(const QVariant &wrapper)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                if (jobject javaObject = qtjambi_cast<jobject>(env, wrapper)){
                    try {
                        Java::QtConcurrent::QtConcurrent$MapFunctor::map(env, functor, javaObject);
                    } catch (const JavaException& exn) {
                        exn.report(env);
                    }
                }
            } else {
                qWarning("Map functor called with invalid data. JNI Environment == %p, java functor object == %p",
                        env, m_functor.object());
            }
        }
    }
};

class MappedFunctor: public Functor {
public:
    typedef QVariant result_type;

    MappedFunctor(JNIEnv *env, jobject javaMappedFunctor) : Functor(env, javaMappedFunctor) {}
    MappedFunctor(const MappedFunctor &other) : Functor(other) {}
    MappedFunctor(MappedFunctor &&other) : Functor(std::move(other)) {}

    QVariant operator ()(const QVariant &wrapper)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                jobject javaObject = qtjambi_cast<jobject>(env, wrapper);
                jobject javaResult = nullptr;
                try {
                    javaResult = Java::QtConcurrent::QtConcurrent$MappedFunctor::map(env, functor, javaObject);
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                return QVariant::fromValue(JObjectWrapper(env, javaResult));
            } else {
                qWarning("Mapped functor called with invalid data. JNI Environment == %p, java functor object == %p",
                        env, m_functor.object());
            }
        }
        return QVariant();
    }
};

class ReduceFunctor: public Functor {
public:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    ReduceFunctor(JNIEnv *env, jobject javaReduceFunctor, JObjectWrapper&& initialValue) : Functor(env, javaReduceFunctor), m_initialValue(new JObjectWrapper(std::move(initialValue))) {}
    ReduceFunctor(const ReduceFunctor &other) : Functor(other), m_initialValue(other.m_initialValue) {}
    ReduceFunctor(ReduceFunctor &&other) : Functor(std::move(other)), m_initialValue(std::move(other.m_initialValue)) {}
#else
    ReduceFunctor(JNIEnv *env, jobject javaReduceFunctor) : Functor(env, javaReduceFunctor) {}
    ReduceFunctor(const ReduceFunctor &other) : Functor(other) {}
    ReduceFunctor(ReduceFunctor &&other) : Functor(std::move(other)) {}
#endif
    void operator()(QVariant &result, const QVariant &wrapper)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                // reduce() is synchronous, so while this data is static in terms
                // of the map/reduce operation, it does not need to be protected
                jobject javaResult = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                if (m_initialValue) {
                    javaResult = env->NewLocalRef(m_initialValue->object());
                    m_initialValue.reset();
                }else
#endif
                {
                    javaResult = qtjambi_cast<jobject>(env, result);
                }

                try {
                    javaResult = Java::QtConcurrent::QtConcurrent$ReduceFunctor::reduce(env, functor, javaResult, qtjambi_cast<jobject>(env, wrapper));
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                result = QVariant::fromValue(JObjectWrapper(env, javaResult));
            } else {
                qWarning("Reduce functor called with invalid data. JNI Environment == %p, java functor object == %p",
                        env, m_functor.object());
            }
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
private:
    QSharedPointer<JObjectWrapper> m_initialValue;
#endif
};

class FilteredFunctor: public Functor {
public:
    FilteredFunctor(JNIEnv *env, jobject javaFilteredFunctor) : Functor(env, javaFilteredFunctor) {}
    FilteredFunctor(const FilteredFunctor &other) : Functor(other) {}
    FilteredFunctor(FilteredFunctor &&other) : Functor(std::move(other)) {}

    bool operator()(const QVariant &wrapper) {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                bool result = false;
                try {
                    result = Java::QtConcurrent::QtConcurrent$FilteredFunctor::filter(env, functor, qtjambi_cast<jobject>(env, wrapper));
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                return result;
            } else {
                qWarning("Filtered functor called with invalid data. JNI Environment == %p, java functor object == %p",
                        env, m_functor.object());
            }
        }
        return false;
    }
};

enum class PromisePolicy{
    NoPromise
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    ,VoidPromise,
    TypedPromise
#endif
};

template<int i, PromisePolicy promise>
struct RunFunctorInvoker{
    static void run(JNIEnv *, jobject, ...){
    }
};

template<>
struct RunFunctorInvoker<0,PromisePolicy::NoPromise> : Java::Runtime::Runnable{
};

template<>
struct RunFunctorInvoker<1,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable1{
};

template<>
struct RunFunctorInvoker<2,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable2{
};

template<>
struct RunFunctorInvoker<3,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable3{
};

template<>
struct RunFunctorInvoker<4,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable4{
};

template<>
struct RunFunctorInvoker<5,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable5{
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<>
struct RunFunctorInvoker<6,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable6{
};

template<>
struct RunFunctorInvoker<7,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable7{
};

template<>
struct RunFunctorInvoker<8,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable8{
};

template<>
struct RunFunctorInvoker<9,PromisePolicy::NoPromise> : Java::QtConcurrent::QtConcurrent$Runnable9{
};

template<>
struct RunFunctorInvoker<0,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise{
};

template<>
struct RunFunctorInvoker<1,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise1{
};

template<>
struct RunFunctorInvoker<2,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise2{
};

template<>
struct RunFunctorInvoker<3,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise3{
};

template<>
struct RunFunctorInvoker<4,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise4{
};

template<>
struct RunFunctorInvoker<5,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise5{
};

template<>
struct RunFunctorInvoker<6,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise6{
};

template<>
struct RunFunctorInvoker<7,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise7{
};

template<>
struct RunFunctorInvoker<8,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise8{
};

template<>
struct RunFunctorInvoker<9,PromisePolicy::VoidPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithVoidPromise9{
};

template<>
struct RunFunctorInvoker<0,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise{
};

template<>
struct RunFunctorInvoker<1,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise1{
};

template<>
struct RunFunctorInvoker<2,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise2{
};

template<>
struct RunFunctorInvoker<3,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise3{
};

template<>
struct RunFunctorInvoker<4,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise4{
};

template<>
struct RunFunctorInvoker<5,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise5{
};

template<>
struct RunFunctorInvoker<6,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise6{
};

template<>
struct RunFunctorInvoker<7,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise7{
};

template<>
struct RunFunctorInvoker<8,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise8{
};

template<>
struct RunFunctorInvoker<9,PromisePolicy::TypedPromise> : Java::QtConcurrent::QtConcurrent$RunnableWithPromise9{
};
#endif

template<typename ...Args>
struct RunFunctorInvocationDecider : RunFunctorInvoker<sizeof...(Args),PromisePolicy::NoPromise>{
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static void handleException(QSharedPointer<QFutureInterfaceBase>& future, std::exception_ptr exception, Args...){
        while(!future->isValid())
            QThread::msleep(50);
        future->reportException(exception);
    }
#endif
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<typename ...Args>
struct RunFunctorInvocationDecider<QPromise<void>&, Args...> : RunFunctorInvoker<sizeof...(Args),PromisePolicy::VoidPromise>{
#ifdef Q_OS_ANDROID
    static void handleException(QSharedPointer<QFutureInterfaceBase>&, std::exception_ptr exception, QPromise<void>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
#endif
};

template<typename T, typename ...Args>
struct RunFunctorInvocationDecider<QPromise<T>&, Args...> : RunFunctorInvoker<sizeof...(Args),PromisePolicy::TypedPromise>{
#ifdef Q_OS_ANDROID
    static void handleException(QSharedPointer<QFutureInterfaceBase>&, std::exception_ptr exception, QPromise<T>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
#endif
};
#endif

template<typename ...Args>
class RunFunctor: public Functor {
public:
    typedef void result_type;
    RunFunctor(JNIEnv *env, jobject javaMapFunctor, const QSharedPointer<QFutureInterfaceBase>& futurePointer)
        : Functor(env, javaMapFunctor),
          future(futurePointer) {}
    RunFunctor(const RunFunctor &other) : Functor(other), future(other.future) {}
    RunFunctor(RunFunctor &&other) : Functor(std::move(other)), future(std::move(other.future)) {}

    void operator ()(Args... args)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                QtJambiScope scope;
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                try {
#endif
                    RunFunctorInvocationDecider<Args...>::run(env, functor, qtjambi_cast<jobject>(env, scope, args)...);
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                } catch (const JavaException& exn) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))), args...);
                } catch (...) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::current_exception(), args...);
                }
#endif
            } else {
                qWarning("Run functor called with invalid data. JNI Environment == %p, method id == %p",
                         env, m_functor.object());
            }
        }
    }
private:
    QSharedPointer<QFutureInterfaceBase> future;
};

template<int i>
struct CallableFunctorInvoker{
    static jobject call(JNIEnv *, jobject, ...){
        return nullptr;
    }
};

template<>
struct CallableFunctorInvoker<0> : Java::Runtime::Callable{
};

template<>
struct CallableFunctorInvoker<1> : Java::QtConcurrent::QtConcurrent$Callable1{
};

template<>
struct CallableFunctorInvoker<2> : Java::QtConcurrent::QtConcurrent$Callable2{
};

template<>
struct CallableFunctorInvoker<3> : Java::QtConcurrent::QtConcurrent$Callable3{
};

template<>
struct CallableFunctorInvoker<4> : Java::QtConcurrent::QtConcurrent$Callable4{
};

template<>
struct CallableFunctorInvoker<5> : Java::QtConcurrent::QtConcurrent$Callable5{
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<>
struct CallableFunctorInvoker<6> : Java::QtConcurrent::QtConcurrent$Callable6{
};

template<>
struct CallableFunctorInvoker<7> : Java::QtConcurrent::QtConcurrent$Callable7{
};

template<>
struct CallableFunctorInvoker<8> : Java::QtConcurrent::QtConcurrent$Callable8{
};

template<>
struct CallableFunctorInvoker<9> : Java::QtConcurrent::QtConcurrent$Callable9{
};
#endif

template<typename ...Args>
class CallableFunctor: public Functor {
public:
    typedef QVariant result_type;
    CallableFunctor(JNIEnv *env, jobject javaMapFunctor, const QSharedPointer<QFutureInterfaceBase>& futurePointer)
        : Functor(env, javaMapFunctor),
          future(futurePointer) {
    }
    CallableFunctor(const CallableFunctor &other) : Functor(other), future(other.future) {}
    CallableFunctor(CallableFunctor &&other) : Functor(std::move(other)), future(std::move(other.future)) {}

    QVariant operator ()(Args... args)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                jobject javaResult = nullptr;
                QtJambiScope scope;
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                try {
#endif
                    javaResult = CallableFunctorInvoker<sizeof...(Args)>::call(env, functor, qtjambi_cast<jobject>(env, scope, args)...);
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                } catch (const JavaException& exn) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))), args...);
                } catch (...) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::current_exception(), args...);
                }
#endif
                return javaResult ? QVariant::fromValue(JObjectWrapper(env, javaResult)) : QVariant();
            } else {
                qWarning("Run functor called with invalid data. JNI Environment == %p, method id == %p",
                         env, m_functor.object());
            }
        }
        return QVariant();
    }
private:
    QSharedPointer<QFutureInterfaceBase> future;
};

struct JavaSequence{
    typedef QVariant value_type;

    class const_iterator{
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef QVariant value_type;
        typedef const QVariant *pointer;
        typedef const QVariant &reference;

        const_iterator();
        const_iterator(const const_iterator& other);

        const_iterator operator ++(int);
        const_iterator operator --(int);
        const_iterator& operator ++();
        const_iterator& operator --();

        const QVariant &operator*() const;
        const QVariant *operator->() const;
        inline operator const QVariant *() const { return operator->(); }
        bool operator<(const const_iterator& o) const;
        bool operator>(const const_iterator& o) const;
        bool operator<=(const const_iterator& o) const;
        bool operator>=(const const_iterator& o) const;
        bool operator==(const const_iterator& o) const;
        inline bool operator!=(const const_iterator& o) const {
            return !(*this == o);
        }
    private:
        const_iterator(const JCollectionWrapper& collection, int cursor = 0);
        JCollectionWrapper m_collection;
        int m_cursor;
        mutable QVariant m_current;
        friend JavaSequence;
    };

    JavaSequence();

    JavaSequence(const JavaSequence& other);

    JavaSequence(JavaSequence&& other);

    JavaSequence(JNIEnv* env, jobject collection, bool canOverwrite = false);

    JavaSequence& operator=(const JavaSequence& other);
    JavaSequence& operator=(std::initializer_list<QVariant> args);

    const_iterator begin() const;

    const_iterator end() const;

    inline const_iterator cbegin() const {return begin();}

    inline const_iterator cend() const {return end();}

    void push_back(const QVariant& value);

    jobject object() const;
private:
    bool m_canOverwrite;
    mutable JCollectionWrapper m_collection;
};

JavaSequence::JavaSequence() : m_collection() {}

JavaSequence::JavaSequence(const JavaSequence& other) : m_canOverwrite(other.m_canOverwrite), m_collection(other.m_collection){}

JavaSequence::JavaSequence(JavaSequence&& other) : m_canOverwrite(other.m_canOverwrite), m_collection(std::move(other.m_collection)){}

JavaSequence::JavaSequence(JNIEnv* env, jobject collection, bool canOverwrite) : m_canOverwrite(canOverwrite), m_collection(env, collection) {}

jobject JavaSequence::object() const{
    jobject object = m_collection.object();
    if(!object){
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            m_collection = JCollectionWrapper(env, qtjambi_arraylist_new(env, 0));
            object = m_collection.object();
        }
    }
    return object;
}

JavaSequence& JavaSequence::operator=(const JavaSequence& other){
    if(m_canOverwrite){
        jobject _object = object();
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            qtjambi_collection_clear(env, _object);
            if(other.m_collection.object()){
                jobject iterator = qtjambi_collection_iterator(env, other.m_collection.object());
                while(qtjambi_iterator_has_next(env, iterator)){
                    qtjambi_collection_add(env, _object, qtjambi_iterator_next(env, iterator));
                }
            }
        }
    }else{
        m_collection = other.m_collection;
    }
    return *this;
}

JavaSequence& JavaSequence::operator=(std::initializer_list<QVariant> args){
    jobject _object = object();
    if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        qtjambi_collection_clear(env, _object);
        for(const QVariant& arg : args){
            qtjambi_collection_add(env, _object, qtjambi_cast<jobject>(env, arg));
        }
    }
    return *this;
}

void JavaSequence::push_back(const QVariant& value){
    if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        jobject _object = object();
        qtjambi_collection_add(env, _object, qtjambi_cast<jobject>(env, value));
    }
}

JavaSequence::const_iterator JavaSequence::begin() const {
    (void)object();
    return JavaSequence::const_iterator(m_collection, 0);
}

JavaSequence::const_iterator JavaSequence::end() const {
    (void)object();
    return JavaSequence::const_iterator(m_collection, -1);
}

JavaSequence::const_iterator JavaSequence::const_iterator::operator ++(int){
    JavaSequence::const_iterator result(*this);
    ++m_cursor;
    if(m_cursor>=m_collection.size()){
        m_cursor = -1;
    }
    return result;
}

JavaSequence::const_iterator& JavaSequence::const_iterator::operator ++(){
    ++m_cursor;
    if(m_cursor>=m_collection.size()){
        m_cursor = -1;
    }
    return *this;
}

JavaSequence::const_iterator JavaSequence::const_iterator::operator --(int){
    JavaSequence::const_iterator result(*this);
    if(m_cursor>0){
        --m_cursor;
        if(m_cursor>=m_collection.size()){
            m_cursor = -1;
        }
    }
    return result;
}

JavaSequence::const_iterator& JavaSequence::const_iterator::operator --(){
    if(m_cursor>0){
        --m_cursor;
        if(m_cursor>=m_collection.size()){
            m_cursor = -1;
        }
    }
    return *this;
}

const QVariant &JavaSequence::const_iterator::operator*() const{
    if(m_cursor>=0 && m_cursor<m_collection.size()){
        jobject _object = m_collection.object();
        if(_object){
            if(JNIEnv *env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject result = nullptr;
                QTJAMBI_TRY{
                    result = qtjambi_list_get(env, _object, m_cursor);
                }QTJAMBI_CATCH(const JavaException&){
                    jobject iterator = qtjambi_collection_iterator(env, _object);
                    for(int i=0; i<m_cursor; ++i){
                        qtjambi_iterator_next(env, iterator);
                    }
                    result = qtjambi_iterator_next(env, iterator);
                }QTJAMBI_TRY_END
                m_current = QVariant::fromValue(JObjectWrapper(env, result));
            }else{
                m_current = QVariant();
            }
        }else{
            m_current = QVariant();
        }
    }else{
        m_current = QVariant();
    }
    return m_current;
}

const QVariant *JavaSequence::const_iterator::operator->() const{
    return &operator*();
}

bool JavaSequence::const_iterator::operator==(const JavaSequence::const_iterator& o) const {
    JNIEnv* env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 300)
    return env->IsSameObject(m_collection.object(), o.m_collection.object()) && m_cursor==o.m_cursor;
}

bool JavaSequence::const_iterator::operator<(const JavaSequence::const_iterator& o) const {
    JNIEnv* env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 300)
    return env->IsSameObject(m_collection.object(), o.m_collection.object()) && m_cursor<o.m_cursor;
}

bool JavaSequence::const_iterator::operator>(const JavaSequence::const_iterator& o) const {
    JNIEnv* env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 300)
    return env->IsSameObject(m_collection.object(), o.m_collection.object()) && m_cursor>o.m_cursor;
}

bool JavaSequence::const_iterator::operator<=(const JavaSequence::const_iterator& o) const {
    JNIEnv* env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 300)
    return env->IsSameObject(m_collection.object(), o.m_collection.object()) && m_cursor<=o.m_cursor;
}

bool JavaSequence::const_iterator::operator>=(const JavaSequence::const_iterator& o) const {
    JNIEnv* env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 300)
    return env->IsSameObject(m_collection.object(), o.m_collection.object()) && m_cursor>=o.m_cursor;
}

JavaSequence::const_iterator::const_iterator()
    : m_collection(), m_cursor(-1), m_current() {}

JavaSequence::const_iterator::const_iterator(const JavaSequence::const_iterator& other)
    : m_collection(other.m_collection), m_cursor(other.m_cursor), m_current() {}

JavaSequence::const_iterator::const_iterator(const JCollectionWrapper& collection, int cursor)
    : m_collection(collection), m_cursor(cursor), m_current() {}

struct ThreadEngineStarterInterface{
    virtual ~ThreadEngineStarterInterface();
    virtual jobject startAsynchronously(JNIEnv* env) = 0;
};

void initialize_meta_info_util(){
    const std::type_info& typeId = registerObjectTypeInfo<ThreadEngineStarterInterface>("ThreadEngineStarterInterface", "io/qt/concurrent/QtConcurrent$ThreadEngineStarter");
    registerDeleter(typeId, [](void *ptr, bool){
        delete reinterpret_cast<ThreadEngineStarterInterface *>(ptr);
    });
}

ThreadEngineStarterInterface::~ThreadEngineStarterInterface(){}

template<typename T>
struct FutureWatcher
        : QFutureWatcher<T>
{
    FutureWatcher(JNIEnv* env, jobject collection, bool canOverwrite = true);
    void setFuture(const QFuture<T>& future);
    void onTermination();
    JavaSequence sequence;
};

template<typename T>
FutureWatcher<T>::FutureWatcher(JNIEnv* env, jobject collection, bool canOverwrite)
  : QFutureWatcher<T>(),
    sequence(env, collection, canOverwrite)
{
}

template<typename T>
void FutureWatcher<T>::setFuture(const QFuture<T>& future){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    if(future.isValid()){
        QObject::connect(this, &QFutureWatcherBase::finished, this, &FutureWatcher<T>::onTermination, Qt::DirectConnection);
        QObject::connect(this, &QFutureWatcherBase::canceled, this, &FutureWatcher<T>::onTermination, Qt::DirectConnection);
    }
#else
    QObject::connect(this, &QFutureWatcherBase::finished, this, &FutureWatcher<T>::onTermination, Qt::DirectConnection);
    QObject::connect(this, &QFutureWatcherBase::canceled, this, &FutureWatcher<T>::onTermination, Qt::DirectConnection);
#endif
    QFutureWatcher<T>::setFuture(future);
}

template<typename T>
void FutureWatcher<T>::onTermination(){
    QObject::deleteLater();
}

template<typename T>
struct FutureWatcherStarter : ThreadEngineStarterInterface{
    FutureWatcherStarter()
        : ThreadEngineStarterInterface(), watcher(nullptr), starter(nullptr) {}
    FutureWatcherStarter(JNIEnv* env, jobject collection, bool canOverwrite = true)
        : ThreadEngineStarterInterface(), watcher(new FutureWatcher<T>(env, collection, canOverwrite)), starter(nullptr) {}
    jobject startAsynchronously(JNIEnv* env) override;
    void setStarter(QtConcurrent::ThreadEngineStarter<T>&& _starter);
    JavaSequence& sequence();
private:
    FutureWatcher<T>* watcher;
    QtConcurrent::ThreadEngineStarter<T> starter;
};

template<typename T>
JavaSequence& FutureWatcherStarter<T>::sequence(){
    Q_ASSERT(watcher);
    return watcher->sequence;
}

template<typename T>
void FutureWatcherStarter<T>::setStarter(QtConcurrent::ThreadEngineStarter<T>&& _starter){
    starter = std::move(_starter);
}

template<typename T>
jobject FutureWatcherStarter<T>::startAsynchronously(JNIEnv* env){
    QFuture<T> future = starter.startAsynchronously();
    if(watcher)
        watcher->setFuture(future);
    return qtjambi_cast<jobject>(env, future);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#define THREADPOOL(_threadPool) Q_UNUSED(_threadPool)
#else
#define THREADPOOL(_threadPool) QThreadPool *threadPool = qtjambi_object_from_nativeId<QThreadPool>(_threadPool);
#endif

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024ThreadEngineStarter_startAsynchronously)
(JNIEnv *env,
 jclass,
 QtJambiNativeID thisId)
{
    try{
        ThreadEngineStarterInterface *starter = qtjambi_object_from_nativeId<ThreadEngineStarterInterface>(thisId);
        return starter->startAsynchronously(env);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_startMap)
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcherStarter<void>* watcher = new FutureWatcherStarter<void>(env, javaSequence, true);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startMap(threadPool, watcher->sequence().begin(), watcher->sequence().end(), MapFunctor(env, javaMapFunctor)));
#else
        watcher->setStarter(QtConcurrent::startMap(watcher->sequence().begin(), watcher->sequence().end(), MapFunctor(env, javaMapFunctor)));
#endif
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_startMapped)
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startMapped<QVariant>(threadPool, JavaSequence(env, javaSequence), MappedFunctor(env, javaMappedFunctor)));
#else
        watcher->setStarter(QtConcurrent::startMapped<QVariant>(JavaSequence(env, javaSequence), MappedFunctor(env, javaMappedFunctor)));
#endif
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_startMappedReduced)
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaMappedFunctor,
 jobject javaReduceFunctor,
 jobject initialValue,
 jint options)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startMappedReduced<QVariant, QVariant>(
                                                            threadPool,
                                                            JavaSequence(env, javaSequence),
                                                            MappedFunctor(env, javaMappedFunctor),
                                                            ReduceFunctor(env, javaReduceFunctor),
                                                            QVariant::fromValue(JObjectWrapper(env, initialValue)),
                                                            QtConcurrent::ReduceOptions(options)));
#else
        watcher->setStarter(QtConcurrent::startMappedReduced<QVariant, QVariant>(
                                JavaSequence(env, javaSequence),
                                MappedFunctor(env, javaMappedFunctor),
                                ReduceFunctor(env, javaReduceFunctor, JObjectWrapper(env, initialValue)),
                                QtConcurrent::ReduceOptions(options)));
#endif
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_startFiltered)
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startFiltered(threadPool, JavaSequence(env, javaSequence), FilteredFunctor(env, javaFilteredFunctor)));
#else
        watcher->setStarter(QtConcurrent::startFiltered(JavaSequence(env, javaSequence), FilteredFunctor(env, javaFilteredFunctor)));
#endif
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_startFilteredReduced)
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor,
 jobject javaReduceFunctor,
 jobject initialValue,
 jint options)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startFilteredReduced<QVariant>(
                                                            threadPool,
                                                            JavaSequence(env, javaSequence),
                                                            FilteredFunctor(env, javaFilteredFunctor),
                                                            ReduceFunctor(env, javaReduceFunctor),
                                                            QVariant::fromValue(JObjectWrapper(env, initialValue)),
                                                            QtConcurrent::ReduceOptions(options)));
#else
        watcher->setStarter(QtConcurrent::startFilteredReduced<QVariant>(
                                JavaSequence(env, javaSequence),
                                FilteredFunctor(env, javaFilteredFunctor),
                                ReduceFunctor(env, javaReduceFunctor, JObjectWrapper(env, initialValue)),
                                QtConcurrent::ReduceOptions(options)));
#endif
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_filter)
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcher<void>* watcher = new FutureWatcher<void>(env, javaSequence);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(threadPool)
            watcher->setFuture(QtConcurrent::filter(threadPool, watcher->sequence, FilteredFunctor(env, javaFilteredFunctor)));
        else
#endif
        watcher->setFuture(QtConcurrent::filter(watcher->sequence, FilteredFunctor(env, javaFilteredFunctor)));
        return qtjambi_cast<jobject>(env, watcher->future());
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid0)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool, RunFunctor<>(env, javaRunnable, futurePointer))
                                          : QtConcurrent::run(RunFunctor<>(env, javaRunnable, futurePointer));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid1)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)))
                                          : QtConcurrent::run(RunFunctor<QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid2)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid3)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid4)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid5)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid6)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid7)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid8)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid9)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = threadPool ? QtConcurrent::run(threadPool,
                                                              RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)),
                                                              QVariant::fromValue(JObjectWrapper(env, i)))
                                          : QtConcurrent::run(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)),
                                                              QVariant::fromValue(JObjectWrapper(env, i)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}
#endif

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run0)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaCallable)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool, CallableFunctor<>(env, javaCallable, futurePointer))
                                                    : QtConcurrent::run(CallableFunctor<>(env, javaCallable, futurePointer));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run1)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)))
                                          : QtConcurrent::run(CallableFunctor<QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run2)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run3)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run4)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run5)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run6)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run7)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run8)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run9)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = threadPool ? QtConcurrent::run(threadPool,
                                                              CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)),
                                                              QVariant::fromValue(JObjectWrapper(env, i)))
                                          : QtConcurrent::run(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)),
                                                              QVariant::fromValue(JObjectWrapper(env, i)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid0)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable)
{
    try{
        using Fun = RunFunctor<QPromise<void>&>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid1)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer), QVariant::fromValue(JObjectWrapper(env, a)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid2)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid3)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid4)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid5)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant,QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid6)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid7)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid8)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid9)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<void> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)),
                                                              QVariant::fromValue(JObjectWrapper(env, i)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise0)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise1)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise2)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise3)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise4)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise5)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise6)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise7)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise8)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runWithPromise9)
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant, QVariant>;
        QThreadPool *pool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
        if(!pool){
            pool = QThreadPool::globalInstance();
        }
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        QFuture<QVariant> result = QtConcurrent::run(pool, Fun(env, javaRunnable, futurePointer),
                                                              QVariant::fromValue(JObjectWrapper(env, a)),
                                                              QVariant::fromValue(JObjectWrapper(env, b)),
                                                              QVariant::fromValue(JObjectWrapper(env, c)),
                                                              QVariant::fromValue(JObjectWrapper(env, d)),
                                                              QVariant::fromValue(JObjectWrapper(env, e)),
                                                              QVariant::fromValue(JObjectWrapper(env, f)),
                                                              QVariant::fromValue(JObjectWrapper(env, g)),
                                                              QVariant::fromValue(JObjectWrapper(env, h)),
                                                              QVariant::fromValue(JObjectWrapper(env, i)));
        *futurePointer = result.d;
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid0Arg0_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid1Arg1_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid2Arg2_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid3Arg3_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid4Arg4_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid5Arg5_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid6Arg6_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid7Arg7_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid8Arg8_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid9Arg9_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)),
                           QVariant::fromValue(JObjectWrapper(env, i)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder0Arg0_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder1Arg1_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder2Arg2_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder3Arg3_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder4Arg4_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder5Arg5_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder6Arg6_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder7Arg7_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder8Arg8_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder9Arg9_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)),
                           QVariant::fromValue(JObjectWrapper(env, i)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid0Arg0_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid1Arg1_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid2Arg2_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid3Arg3_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid4Arg4_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid5Arg5_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid6Arg6_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid7Arg7_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid8Arg8_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid9Arg9_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)),
                           QVariant::fromValue(JObjectWrapper(env, i)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<void> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<void> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder0Arg0_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder1Arg1_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder2Arg2_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder3Arg3_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder4Arg4_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder5Arg5_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder6Arg6_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder7Arg7_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder8Arg8_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder9Arg9_spawn)
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)),
                           QVariant::fromValue(JObjectWrapper(env, d)),
                           QVariant::fromValue(JObjectWrapper(env, e)),
                           QVariant::fromValue(JObjectWrapper(env, f)),
                           QVariant::fromValue(JObjectWrapper(env, g)),
                           QVariant::fromValue(JObjectWrapper(env, h)),
                           QVariant::fromValue(JObjectWrapper(env, i)))
                .withPriority(priority);
        if(QThreadPool* t = qtjambi_to_QObject<QThreadPool>(env, _threadPool)){
            QFuture<QVariant> result = task.onThreadPool(*t).spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }else{
            QFuture<QVariant> result = task.spawn();
            *futurePointer = result.d;
            return qtjambi_cast<jobject>(env, result);
        }
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#endif // QT_NO_CONCURRENT


