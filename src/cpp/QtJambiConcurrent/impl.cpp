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
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#define QFUTURE_TEST
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/RegistryAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/Template1Cast>
#include <QtJambi/FutureCast>

#include "utils_p.h"

#include <QtJambi/Cast>

Q_LOGGING_CATEGORY(CATEGORY, "io.qt.concurrent", QtWarningMsg)

#ifndef QT_NO_CONCURRENT

template <typename T>
struct ThreadEngineStarter : public QtConcurrent::ThreadEngineStarterBase<T>
{
    typedef QtConcurrent::ThreadEngineStarterBase<T> Base;
    typedef QtConcurrent::ThreadEngine<T> TypedThreadEngine;
    ThreadEngineStarter(const QtConcurrent::ThreadEngineStarter<T> &other)
        : Base(other) { }
    ~ThreadEngineStarter(){
        if(this->Base::threadEngine && !futureInterface)
            delete this->Base::threadEngine;
    }
    QFuture<T> startAsynchronously(){
        if(!futureInterface){
            QFuture<T> future = this->Base::threadEngine->startAsynchronously();
            futureInterface = this->Base::threadEngine->futureInterfaceTyped();
            return future;
        }else{
            return {};
        }
    }
    QtConcurrent::ThreadEngine<T> *threadEngine(){
        return this->Base::threadEngine;
    }
    mutable QFutureInterface<T>* futureInterface = nullptr;
};

struct ThreadEngineStarterWrapperPrivate : QSharedData{
    enum StateFlag{
        None,
        IsVoidFuture,
        IsQVariantFuture,
        IsJObjectWrapperFuture,
    };
    void* m_sequence = nullptr;
    void(*m_deleter)(void*) = nullptr;
    StateFlag m_state = None;
    union Starter{
        Starter() : v(nullptr) {}
        ThreadEngineStarter<void>* v;
        ThreadEngineStarter<QVariant>* q;
        ThreadEngineStarter<JObjectWrapper>* j;
    } starter;
    ThreadEngineStarterWrapperPrivate(){}
    ~ThreadEngineStarterWrapperPrivate(){
        if(starter.v){
            switch(m_state){
            case IsVoidFuture:
                delete starter.v;
                break;
            case IsQVariantFuture:
                delete starter.q;
                break;
            case IsJObjectWrapperFuture:
                delete starter.j;
                break;
            default:
                break;
            }
        }
        if(m_sequence && m_deleter)
            m_deleter(m_sequence);
    }
    void setSequence(void* sequence, void(*deleter)(void*)){
        m_sequence = sequence;
        m_deleter = deleter;
    }
    void setStarter(ThreadEngineStarter<void>* _starter){
        Q_ASSERT(m_state==None || m_state==IsVoidFuture);
        m_state = IsVoidFuture;
        starter.v = _starter;
    }

    void setStarter(ThreadEngineStarter<QVariant>* _starter){
        Q_ASSERT(m_state==None || m_state==IsQVariantFuture);
        m_state = IsQVariantFuture;
        starter.q = _starter;
    }

    void setStarter(ThreadEngineStarter<JObjectWrapper>* _starter){
        Q_ASSERT(m_state==None || m_state==IsJObjectWrapperFuture);
        m_state = IsJObjectWrapperFuture;
        starter.j = _starter;
    }
    void destroyUnstarted(){
        if(this->ref.loadAcquire()==2){
            switch(m_state){
            case IsVoidFuture:
                if(starter.v && !starter.v->futureInterface){
                    delete starter.v;
                    starter.v = nullptr;
                }
                break;
            case IsQVariantFuture:
                if(starter.q && !starter.q->futureInterface){
                    delete starter.q;
                    starter.q = nullptr;
                }
                break;
            case IsJObjectWrapperFuture:
                if(starter.j && !starter.j->futureInterface){
                    delete starter.j;
                    starter.j = nullptr;
                }
                break;
            default:
                break;
            }
        }
    }
    jobject startAsynchronously(JNIEnv* env){
        switch(m_state){
        case IsVoidFuture:
            if(starter.v){
                QFuture<void> future = starter.v->startAsynchronously();
                return ::qtjambi_cast<jobject>(env, std::move(future));
            }else{
                return ::qtjambi_cast<jobject>(env, QFuture<void>());
            }
            break;
        case IsQVariantFuture:
            if(starter.q){
                QFuture<QVariant> future = starter.q->startAsynchronously();
                return ::qtjambi_cast<jobject>(env, std::move(future));
            }else{
                return ::qtjambi_cast<jobject>(env, QFuture<QVariant>());
            }
            break;
        case IsJObjectWrapperFuture:
            if(starter.j){
                QFuture<JObjectWrapper> future = starter.j->startAsynchronously();
                return ::qtjambi_cast<jobject>(env, std::move(future));
            }else{
                return ::qtjambi_cast<jobject>(env, QFuture<QVariant>());
            }
            break;
        default:
            break;
        }
        return nullptr;
    }

    void reportException(const QException &e)const{
        switch(m_state){
        case IsVoidFuture:
            if(starter.v && starter.v->futureInterface)
                starter.v->futureInterface->reportException(e);
            break;
        case IsQVariantFuture:
            if(starter.q && starter.q->futureInterface)
                starter.q->futureInterface->reportException(e);
            break;
        case IsJObjectWrapperFuture:
            if(starter.j && starter.j->futureInterface)
                starter.j->futureInterface->reportException(e);
            break;
        default:
            break;
        }
    }
    void reportException(std::exception_ptr e)const{
        switch(m_state){
        case IsVoidFuture:
            if(starter.v && starter.v->futureInterface)
                starter.v->futureInterface->reportException(e);
            break;
        case IsQVariantFuture:
            if(starter.q && starter.q->futureInterface)
                starter.q->futureInterface->reportException(e);
            break;
        case IsJObjectWrapperFuture:
            if(starter.j && starter.j->futureInterface)
                starter.j->futureInterface->reportException(e);
            break;
        default:
            break;
        }
    }
};

QtConcurrent::ThreadEngineStarterWrapper::ThreadEngineStarterWrapper()
    : d(new ::ThreadEngineStarterWrapperPrivate)
{
}

QtConcurrent::ThreadEngineStarterWrapper::ThreadEngineStarterWrapper(const ThreadEngineStarterWrapper&) = default;
QtConcurrent::ThreadEngineStarterWrapper& QtConcurrent::ThreadEngineStarterWrapper::operator=(const ThreadEngineStarterWrapper&) = default;
QtConcurrent::ThreadEngineStarterWrapper& QtConcurrent::ThreadEngineStarterWrapper::operator=(ThreadEngineStarterWrapper&&) = default;

QtConcurrent::ThreadEngineStarterWrapper::~ThreadEngineStarterWrapper(){
    if(d)
        d->destroyUnstarted();
}

void QtConcurrent::ThreadEngineStarterWrapper::setStarter(const QtConcurrent::ThreadEngineStarter<void>& _starter){
    d->setStarter(new ::ThreadEngineStarter<void>(_starter));
}

void QtConcurrent::ThreadEngineStarterWrapper::setStarter(const QtConcurrent::ThreadEngineStarter<QVariant>& _starter){
    d->setStarter(new ::ThreadEngineStarter<QVariant>(_starter));
}

void QtConcurrent::ThreadEngineStarterWrapper::setStarter(const QtConcurrent::ThreadEngineStarter<JObjectWrapper>& _starter){
    d->setStarter(new ::ThreadEngineStarter<JObjectWrapper>(_starter));
}

void QtConcurrent::ThreadEngineStarterWrapper::setSequence(void* sequence, void(*deleter)(void*)){
    d->setSequence(sequence, deleter);
}

jobject QtConcurrent::ThreadEngineStarterWrapper::startAsynchronously(JNIEnv* env){
    return d ? d->startAsynchronously(env) : nullptr;
}

void QtConcurrent::ThreadEngineStarterWrapper::reportException(const QException &e) const{
    if(d)
        d->reportException(e);
}
void QtConcurrent::ThreadEngineStarterWrapper::reportException(std::exception_ptr e) const{
    if(d)
        d->reportException(e);
}

void QtConcurrent::ThreadEngineStarterWrapper::destroy(void *ptr, bool){
    delete reinterpret_cast<ThreadEngineStarterWrapper *>(ptr);
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024ThreadEngineStarter_startAsynchronously(JNIEnv *env, jclass, QtJambiNativeID thisId){
    try{
        QtConcurrent::ThreadEngineStarterWrapper &starter = qtjambi_cast<QtConcurrent::ThreadEngineStarterWrapper&>(env, thisId);
        return starter.startAsynchronously(env);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

#endif // QT_NO_CONCURRENT


