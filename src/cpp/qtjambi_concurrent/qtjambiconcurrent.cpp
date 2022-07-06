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

class ReducedFunctor: public Functor {
public:
    ReducedFunctor(JNIEnv *env, jobject javaReducedFunctor) : Functor(env, javaReducedFunctor), m_first_call(true) {}
    ReducedFunctor(const ReducedFunctor &other) : Functor(other), m_first_call(other.m_first_call) {}
    ReducedFunctor(ReducedFunctor &&other) : Functor(std::move(other)), m_first_call(other.m_first_call) {}

    void operator()(QVariant &result, const QVariant &wrapper)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                // reduce() is synchronous, so while this data is static in terms
                // of the map/reduce operation, it does not need to be protected
                if (m_first_call) {
                    m_first_call = false;
                    jobject javaResult = nullptr;
                    try {
                        javaResult = Java::QtConcurrent::QtConcurrent$ReducedFunctor::defaultResult(env, functor);
                    } catch (const JavaException& exn) {
                        exn.report(env);
                    }
                    result = QVariant::fromValue(JObjectWrapper(env, javaResult));
                }

                try {
                    Java::QtConcurrent::QtConcurrent$ReducedFunctor::reduce(env, functor, qtjambi_cast<jobject>(env, result), qtjambi_cast<jobject>(env, wrapper));
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
            } else {
                qWarning("Reduce functor called with invalid data. JNI Environment == %p, java functor object == %p",
                        env, m_functor.object());
            }
        }
    }

private:
    uint m_first_call : 1;

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

template<typename T>
struct FutureWatcher
        : QFutureWatcher<T>
{
    FutureWatcher(JNIEnv* env, jobject collection, bool canOverwrite = true);
    JavaSequence sequence;
};

template<typename T>
FutureWatcher<T>::FutureWatcher(JNIEnv* env, jobject collection, bool canOverwrite)
  : QFutureWatcher<T>(),
    sequence(env, collection, canOverwrite)
{
    QObject::connect(this, &QFutureWatcherBase::finished, this, &FutureWatcher::deleteLater, Qt::DirectConnection);
    QObject::connect(this, &QFutureWatcherBase::canceled, this, &FutureWatcher::deleteLater, Qt::DirectConnection);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#define THREADPOOL(_threadPool) Q_UNUSED(_threadPool)
#else
#define THREADPOOL(_threadPool) QThreadPool *threadPool = qtjambi_to_QObject<QThreadPool>(env, _threadPool);
#endif

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1map)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcher<void>* watcher = new FutureWatcher<void>(env, javaSequence, true);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(threadPool)
            watcher->setFuture(QtConcurrent::map(threadPool, watcher->sequence, MapFunctor(env, javaMapFunctor)));
        else
#endif
            watcher->setFuture(QtConcurrent::map(watcher->sequence, MapFunctor(env, javaMapFunctor)));
        return qtjambi_cast<jobject>(env, watcher->future());
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1blockingMap)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    try{
        THREADPOOL(_threadPool)
        JavaSequence sequence(env, javaSequence);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(threadPool)
            QtConcurrent::blockingMap(threadPool, sequence, MapFunctor(env, javaMapFunctor));
        else
#endif
            QtConcurrent::blockingMap(sequence, MapFunctor(env, javaMapFunctor));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1mapped)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcher<QVariant>* watcher = new FutureWatcher<QVariant>(env, javaSequence, false);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(threadPool)
            watcher->setFuture(QtConcurrent::mapped(threadPool, watcher->sequence, MappedFunctor(env, javaMappedFunctor)));
        else
#endif
            watcher->setFuture(QtConcurrent::mapped(watcher->sequence, MappedFunctor(env, javaMappedFunctor)));
        return qtjambi_cast<jobject>(env, watcher->future());
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}


extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1blockingMapped)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    try{
        THREADPOOL(_threadPool)
        JavaSequence sequence(env, javaSequence);
        JavaSequence result =
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                threadPool ? QtConcurrent::blockingMapped<JavaSequence>(threadPool, sequence, MappedFunctor(env, javaMappedFunctor)) :
#endif
                             QtConcurrent::blockingMapped<JavaSequence>(sequence, MappedFunctor(env, javaMappedFunctor));
        return env->NewLocalRef(result.object());
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1blockingMappedReduced)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaMappedFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    try{
        THREADPOOL(_threadPool)
        QVariant result =
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                threadPool ? QtConcurrent::blockingMappedReduced<QVariant, JavaSequence, MappedFunctor, ReducedFunctor>(
                                                    threadPool,
                                                    JavaSequence(env, javaSequence),
                                                    MappedFunctor(env, javaMappedFunctor),
                                                    ReducedFunctor(env, javaReducedFunctor),
                                                    QtConcurrent::ReduceOptions(options)) :
#endif
                                             QtConcurrent::blockingMappedReduced<QVariant, JavaSequence, MappedFunctor, ReducedFunctor>(
                                                    JavaSequence(env, javaSequence),
                                                    MappedFunctor(env, javaMappedFunctor),
                                                    ReducedFunctor(env, javaReducedFunctor),
                                                    QtConcurrent::ReduceOptions(options));
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1mappedReduced)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaMappedFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    try{
        THREADPOOL(_threadPool)
        QFuture<QVariant> result =
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                threadPool ? QtConcurrent::mappedReduced<QVariant, JavaSequence, MappedFunctor, ReducedFunctor>(
                                                            threadPool,
                                                            JavaSequence(env, javaSequence),
                                                            MappedFunctor(env, javaMappedFunctor),
                                                            ReducedFunctor(env, javaReducedFunctor),
                                                            QtConcurrent::ReduceOptions(options)) :
#endif
                                                      QtConcurrent::mappedReduced<QVariant, JavaSequence, MappedFunctor, ReducedFunctor>(
                                                          JavaSequence(env, javaSequence),
                                                          MappedFunctor(env, javaMappedFunctor),
                                                          ReducedFunctor(env, javaReducedFunctor),
                                                          QtConcurrent::ReduceOptions(options));
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1filter)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
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

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1filtered)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    try{
        THREADPOOL(_threadPool)
        FutureWatcher<QVariant>* watcher = new FutureWatcher<QVariant>(env, javaSequence, false);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(threadPool)
            watcher->setFuture(QtConcurrent::filtered(threadPool, watcher->sequence, FilteredFunctor(env, javaFilteredFunctor)));
        else
#endif
            watcher->setFuture(QtConcurrent::filtered(watcher->sequence, FilteredFunctor(env, javaFilteredFunctor)));
        return qtjambi_cast<jobject>(env, watcher->future());
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1blockingFiltered)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    try{
        THREADPOOL(_threadPool)
        JavaSequence result =
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                threadPool ? QtConcurrent::blockingFiltered(threadPool, JavaSequence(env, javaSequence), FilteredFunctor(env, javaFilteredFunctor)) :
#endif
                             QtConcurrent::blockingFiltered(JavaSequence(env, javaSequence), FilteredFunctor(env, javaFilteredFunctor));
        return env->NewLocalRef(result.object());
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1blockingFilter)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    try{
        THREADPOOL(_threadPool)
        JavaSequence sequence(env, javaSequence, true);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(threadPool)
            QtConcurrent::blockingFilter(threadPool, sequence, FilteredFunctor(env, javaFilteredFunctor));
        else
#endif
            QtConcurrent::blockingFilter(sequence, FilteredFunctor(env, javaFilteredFunctor));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1filteredReduced)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    try{
        THREADPOOL(_threadPool)
        QFuture<QVariant> result =
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                threadPool ? QtConcurrent::filteredReduced<QVariant, JavaSequence, FilteredFunctor, ReducedFunctor>(
                                                            threadPool,
                                                            JavaSequence(env, javaSequence),
                                                            FilteredFunctor(env, javaFilteredFunctor),
                                                            ReducedFunctor(env, javaReducedFunctor),
                                                            QtConcurrent::ReduceOptions(options)) :
#endif
                QtConcurrent::filteredReduced<QVariant, JavaSequence, FilteredFunctor, ReducedFunctor>(
                                                            JavaSequence(env, javaSequence),
                                                            FilteredFunctor(env, javaFilteredFunctor),
                                                            ReducedFunctor(env, javaReducedFunctor),
                                                            QtConcurrent::ReduceOptions(options));
        return qtjambi_cast<jobject>(env, result);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent__1blockingFilteredReduced)
(JNIEnv *env,
 jclass,
 jobject _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    try{
        THREADPOOL(_threadPool)
        QVariant result =
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                        threadPool ? QtConcurrent::blockingFilteredReduced<QVariant, JavaSequence, FilteredFunctor, ReducedFunctor>(
                                                    threadPool,
                                                    JavaSequence(env, javaSequence), FilteredFunctor(env, javaFilteredFunctor),
                                                    ReducedFunctor(env, javaReducedFunctor),
                                                    QtConcurrent::ReduceOptions(options)) :
#endif
            QtConcurrent::blockingFilteredReduced<QVariant, JavaSequence, FilteredFunctor, ReducedFunctor>(
                                                    JavaSequence(env, javaSequence), FilteredFunctor(env, javaFilteredFunctor),
                                                    ReducedFunctor(env, javaReducedFunctor),
                                                    QtConcurrent::ReduceOptions(options));
        return qtjambi_cast<jobject>(env, result);
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


