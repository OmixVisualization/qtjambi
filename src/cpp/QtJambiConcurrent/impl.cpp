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

#include <QtConcurrent/QtConcurrent>
#include <QtJambi/qtjambi_cast.h>

Q_LOGGING_CATEGORY(CATEGORY, "io.qt.concurrent", QtWarningMsg)

namespace Java{
namespace QtConcurrent{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent,
              QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(computeDefaultResult))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$MapFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(map))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$MappedFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(map))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$ReduceFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(reduce))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$FilteredFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(filter))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable1,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable2,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable3,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable4,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable5,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable6,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable7,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable8,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Runnable9,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable1,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable2,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable3,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable4,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable5,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable6,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable7,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable8,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$Callable9,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise1,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise2,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise3,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise4,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise5,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise6,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise7,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise8,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithPromise9,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise1,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise2,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise3,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise4,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise5,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise6,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise7,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise8,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent$RunnableWithVoidPromise9,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(computeDefaultResult,(Lio/qt/concurrent/QtConcurrent$ReduceFunctor;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$MapFunctor,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(map,(Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$MappedFunctor,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(map,(Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$ReduceFunctor,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(reduce,(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$FilteredFunctor,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(filter,(Ljava/lang/Object;)Z)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable1,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable2,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable3,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable4,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable5,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable6,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable7,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable8,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Runnable9,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise1,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise2,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise3,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise4,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise5,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise6,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise7,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise8,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithPromise9,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise1,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise2,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise3,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise4,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise5,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise6,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise7,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise8,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$RunnableWithVoidPromise9,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(run,(Lio/qt/core/QPromise;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable1,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable2,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable3,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable4,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable5,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable6,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable7,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable8,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )

    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$Callable9,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
    )
}
namespace Runtime{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Callable,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/concurrent,Callable,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(call,()Ljava/lang/Object;)
    )
}
}

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
        if(JniEnvironment env{200}){
            if(jobject functor = m_functor.object(env)){
                if (jobject javaObject = qtjambi_cast<jobject>(env, wrapper)){
                    try {
                        Java::QtConcurrent::QtConcurrent$MapFunctor::map(env, functor, javaObject);
                    } catch (const JavaException& exn) {
                        exn.report(env);
                    }
                }
            } else {
                qCWarning(CATEGORY) << "Map functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
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
        if(JniEnvironment env{200}){
            if(jobject functor = m_functor.object(env)){
                jobject javaObject = qtjambi_cast<jobject>(env, wrapper);
                jobject javaResult = nullptr;
                try {
                    javaResult = Java::QtConcurrent::QtConcurrent$MappedFunctor::map(env, functor, javaObject);
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                return QVariant::fromValue(JObjectWrapper(env, javaResult));
            } else {
                qCWarning(CATEGORY) << "Mapped functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
            }
        }
        return QVariant();
    }
};

class ReduceFunctor: public Functor {
public:
    ReduceFunctor(JNIEnv *env, jobject javaReduceFunctor) : Functor(env, javaReduceFunctor) {}
    ReduceFunctor(const ReduceFunctor &other) : Functor(other) {}
    ReduceFunctor(ReduceFunctor &&other) : Functor(std::move(other)) {}
    void operator()(QVariant &result, const QVariant &wrapper)
    {
        if(JniEnvironment env{200}){
            if(jobject functor = m_functor.object(env)){
                // reduce() is synchronous, so while this data is static in terms
                // of the map/reduce operation, it does not need to be protected
                jobject javaResult = qtjambi_cast<jobject>(env, std::move(result));
                try {
                    javaResult = Java::QtConcurrent::QtConcurrent$ReduceFunctor::reduce(env, functor, javaResult, qtjambi_cast<jobject>(env, wrapper));
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                result = QVariant::fromValue(JObjectWrapper(env, javaResult));
            } else {
                qCWarning(CATEGORY) << "Reduce functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
            }
        }
    }
private:
    QSharedPointer<JObjectWrapper> m_initialValue;
};

class FilteredFunctor: public Functor {
public:
    FilteredFunctor(JNIEnv *env, jobject javaFilteredFunctor) : Functor(env, javaFilteredFunctor) {}
    FilteredFunctor(const FilteredFunctor &other) : Functor(other) {}
    FilteredFunctor(FilteredFunctor &&other) : Functor(std::move(other)) {}

    bool operator()(const QVariant &wrapper) {
        if(JniEnvironment env{200}){
            if(jobject functor = m_functor.object(env)){
                bool result = false;
                try {
                    result = Java::QtConcurrent::QtConcurrent$FilteredFunctor::filter(env, functor, qtjambi_cast<jobject>(env, wrapper));
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                return result;
            } else {
                qCWarning(CATEGORY) << "Filtered functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
            }
        }
        return false;
    }
};

enum class PromisePolicy{
    NoPromise,
    VoidPromise,
    TypedPromise
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

#if (defined(Q_OS_ANDROID) /*|| defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_OPENBSD) || defined(Q_OS_SOLARIS)*/)
#define HANDLE_EXCEPTION
#endif

template<typename ...Args>
struct RunFunctorInvocationDecider : RunFunctorInvoker<sizeof...(Args),PromisePolicy::NoPromise>{
#if defined(HANDLE_EXCEPTION)
    static void handleException(QSharedPointer<QFutureInterfaceBase>& future, std::exception_ptr exception, Args...){
        while(!future->isValid())
            QThread::msleep(50);
        future->reportException(exception);
    }
#endif
};

template<typename ...Args>
struct RunFunctorInvocationDecider<QPromise<void>&, Args...> : RunFunctorInvoker<sizeof...(Args),PromisePolicy::VoidPromise>{
#if defined(HANDLE_EXCEPTION)
    static void handleException(QSharedPointer<QFutureInterfaceBase>&, std::exception_ptr exception, QPromise<void>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
#endif
};

template<typename T, typename ...Args>
struct RunFunctorInvocationDecider<QPromise<T>&, Args...> : RunFunctorInvoker<sizeof...(Args),PromisePolicy::TypedPromise>{
#if defined(HANDLE_EXCEPTION)
    static void handleException(QSharedPointer<QFutureInterfaceBase>&, std::exception_ptr exception, QPromise<T>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
#endif
};

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
        if(JniEnvironment env{200}){
            if(jobject functor = m_functor.object(env)){
                QtJambiScope scope;
#if defined(HANDLE_EXCEPTION)
                try {
#endif
                    RunFunctorInvocationDecider<Args...>::run(env, functor, qtjambi_cast<jobject>(env, scope, args)...);
#if defined(HANDLE_EXCEPTION)
                } catch (const JavaException& exn) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))), args...);
                } catch (...) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::current_exception(), args...);
                }
#endif
            } else {
                qCWarning(CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
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
        if(JniEnvironment env{200}){
            if(jobject functor = m_functor.object(env)){
                jobject javaResult = nullptr;
                QtJambiScope scope;
#if defined(HANDLE_EXCEPTION)
                try {
#endif
                    javaResult = CallableFunctorInvoker<sizeof...(Args)>::call(env, functor, qtjambi_cast<jobject>(env, scope, args)...);
#if defined(HANDLE_EXCEPTION)
                } catch (const JavaException& exn) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))), args...);
                } catch (...) {
                    RunFunctorInvocationDecider<Args...>::handleException(future, std::current_exception(), args...);
                }
#endif
                return javaResult ? QVariant::fromValue(JObjectWrapper(env, javaResult)) : QVariant();
            } else {
                qCWarning(CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
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
        typedef std::bidirectional_iterator_tag iterator_category;
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
        const_iterator(JNIEnv* env, const JObjectWrapper& collection, int cursor = 0);
        JObjectWrapper m_collection;
        int m_size;
        int m_cursor;
        mutable QVariant m_current;
        bool m_isList;
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

    jobject object(JNIEnv* env) const;
private:
    bool m_canOverwrite;
    mutable JObjectWrapper m_collection;
};

JavaSequence::JavaSequence() : m_collection() {}

JavaSequence::JavaSequence(const JavaSequence& other) : m_canOverwrite(other.m_canOverwrite), m_collection(other.m_collection){}

JavaSequence::JavaSequence(JavaSequence&& other) : m_canOverwrite(other.m_canOverwrite), m_collection(std::move(other.m_collection)) {}

JavaSequence::JavaSequence(JNIEnv* env, jobject collection, bool canOverwrite) : m_canOverwrite(canOverwrite), m_collection(env, collection) {}

jobject JavaSequence::object(JNIEnv* env) const{
    jobject object = m_collection.object(env);
    if(!Java::Runtime::Collection::isInstanceOf(env, object)){
        m_collection.assign(env, object = QtJambiAPI::newJavaArrayList(env, 0));
    }
    return object;
}

JavaSequence& JavaSequence::operator=(const JavaSequence& other){
    if(&other!=this){
        if(m_canOverwrite){
            if(JniEnvironment env{200}){
                jobject _object = object(env);
                jobject otherObject = other.m_collection.object(env);
                if(!env->IsSameObject(_object, otherObject)){
                    QtJambiAPI::clearJavaCollection(env, _object);
                    if(otherObject){
                        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, otherObject);
                        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                            QtJambiAPI::addToJavaCollection(env, _object, QtJambiAPI::nextOfJavaIterator(env, iterator));
                        }
                    }
                }
            }
        }else{
            m_collection = other.m_collection;
        }
    }
    return *this;
}

JavaSequence& JavaSequence::operator=(std::initializer_list<QVariant> args){
    if(JniEnvironment env{200}){
        jobject _object = object(env);
        QtJambiAPI::clearJavaCollection(env, _object);
        for(const QVariant& arg : args){
            QtJambiAPI::addToJavaCollection(env, _object, qtjambi_cast<jobject>(env, arg));
        }
    }
    return *this;
}

void JavaSequence::push_back(const QVariant& value){
    if(JniEnvironment env{200}){
        jobject _object = object(env);
        QtJambiAPI::addToJavaCollection(env, _object, qtjambi_cast<jobject>(env, value));
    }
}

JavaSequence::const_iterator JavaSequence::begin() const {
    if(JniEnvironment env{128}){
        (void)object(env);
        return JavaSequence::const_iterator(env, m_collection, 0);
    }else return {};
}

JavaSequence::const_iterator JavaSequence::end() const {
    if(JniEnvironment env{128}){
        (void)object(env);
        return JavaSequence::const_iterator(env, m_collection, -1);
    }else return {};
}

JavaSequence::const_iterator JavaSequence::const_iterator::operator ++(int){
    JavaSequence::const_iterator result(*this);
    ++m_cursor;
    if(m_cursor>=m_size){
        m_cursor = -1;
    }
    return result;
}

JavaSequence::const_iterator& JavaSequence::const_iterator::operator ++(){
    ++m_cursor;
    if(m_cursor>=m_size){
        m_cursor = -1;
    }
    return *this;
}

JavaSequence::const_iterator JavaSequence::const_iterator::operator --(int){
    JavaSequence::const_iterator result(*this);
    if(m_cursor>0){
        --m_cursor;
        if(m_cursor>=m_size){
            m_cursor = -1;
        }
    }
    return result;
}

JavaSequence::const_iterator& JavaSequence::const_iterator::operator --(){
    if(m_cursor>0){
        --m_cursor;
        if(m_cursor>=m_size){
            m_cursor = -1;
        }
    }
    return *this;
}

const QVariant &JavaSequence::const_iterator::operator*() const{
    if(m_cursor>=0 && m_cursor<m_size){
        if(JniEnvironment env{200}){
            jobject _object = m_collection.object(env);
            if(_object){
                jobject result = nullptr;
                if(m_isList){
                    result = Java::Runtime::List::get(env, _object, m_cursor);
                }else{
                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, _object);
                    for(int i=0; i<m_cursor; ++i){
                        QtJambiAPI::nextOfJavaIterator(env, iterator);
                    }
                    result = QtJambiAPI::nextOfJavaIterator(env, iterator);
                }
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
    if(JniEnvironment env{300}){
        if(env->ExceptionCheck()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor==o.m_cursor;
    }else return false;
}

bool JavaSequence::const_iterator::operator<(const JavaSequence::const_iterator& o) const {
    if(JniEnvironment env{300}){
        if(env->ExceptionCheck()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor<o.m_cursor;
    }else return false;
}

bool JavaSequence::const_iterator::operator>(const JavaSequence::const_iterator& o) const {
    if(JniEnvironment env{300}){
        if(env->ExceptionCheck()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor>o.m_cursor;
    }else return false;
}

bool JavaSequence::const_iterator::operator<=(const JavaSequence::const_iterator& o) const {
    if(JniEnvironment env{300}){
        if(env->ExceptionCheck()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor<=o.m_cursor;
    }else return false;
}

bool JavaSequence::const_iterator::operator>=(const JavaSequence::const_iterator& o) const {
    if(JniEnvironment env{300}){
        if(env->ExceptionCheck()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor>=o.m_cursor;
    }else return false;
}

JavaSequence::const_iterator::const_iterator()
    : m_collection(), m_cursor(-1), m_current(), m_isList(false) {}

JavaSequence::const_iterator::const_iterator(const JavaSequence::const_iterator& other)
    : m_collection(other.m_collection), m_size(other.m_size), m_cursor(other.m_cursor), m_current(other.m_current), m_isList(other.m_isList) {}

JavaSequence::const_iterator::const_iterator(JNIEnv* env, const JObjectWrapper& collection, int cursor)
    : m_collection(collection),
      m_size(Java::Runtime::Collection::size(env, collection.object(env))),
      m_cursor(cursor),
      m_current(), m_isList(Java::Runtime::List::isInstanceOf(env, collection.object(env))) {}

struct ThreadEngineStarterInterface{
    virtual ~ThreadEngineStarterInterface();
    virtual jobject startAsynchronously(JNIEnv* env) = 0;
};

void initialize_meta_info_util(){
    using namespace RegistryAPI;
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
    if(future.isValid()){
        QObject::connect(this, &QFutureWatcherBase::finished, this, &FutureWatcher<T>::onTermination, Qt::DirectConnection);
        QObject::connect(this, &QFutureWatcherBase::canceled, this, &FutureWatcher<T>::onTermination, Qt::DirectConnection);
    }
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
    void setStarter(const QtConcurrent::ThreadEngineStarter<T>& _starter);
    JavaSequence& sequence();
private:
    FutureWatcher<T>* watcher;
    QScopedPointer<QtConcurrent::ThreadEngineStarter<T>> starter;
};

template<typename T>
JavaSequence& FutureWatcherStarter<T>::sequence(){
    Q_ASSERT(watcher);
    return watcher->sequence;
}

template<typename T>
void FutureWatcherStarter<T>::setStarter(const QtConcurrent::ThreadEngineStarter<T>& _starter){
    starter.reset(new QtConcurrent::ThreadEngineStarter<T>(_starter));
}

template<typename T>
jobject FutureWatcherStarter<T>::startAsynchronously(JNIEnv* env){
    if(starter){
        QFuture<T> future = starter->startAsynchronously();
        if(watcher)
            watcher->setFuture(future);
        return qtjambi_cast<jobject>(env, std::move(future));
    }else{
        return qtjambi_cast<jobject>(env, QFuture<T>());
    }
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024ThreadEngineStarter_startAsynchronously(JNIEnv *env, jclass, QtJambiNativeID thisId){
    try{
        ThreadEngineStarterInterface *starter = QtJambiAPI::objectFromNativeId<ThreadEngineStarterInterface>(thisId);
        return starter->startAsynchronously(env);
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_startMap
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::objectFromNativeId<QThreadPool>(_threadPool);
        FutureWatcherStarter<void>* watcher = new FutureWatcherStarter<void>(env, javaSequence, true);
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startMap(threadPool, watcher->sequence().begin(), watcher->sequence().end(), MapFunctor(env, javaMapFunctor)));
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_startMapped
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::objectFromNativeId<QThreadPool>(_threadPool);
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startMapped<QVariant>(threadPool, JavaSequence(env, javaSequence), MappedFunctor(env, javaMappedFunctor)));
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_startMappedReduced
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
        QThreadPool *threadPool = QtJambiAPI::objectFromNativeId<QThreadPool>(_threadPool);
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startMappedReduced<QVariant, QVariant>(
                                                            threadPool,
                                                            JavaSequence(env, javaSequence),
                                                            MappedFunctor(env, javaMappedFunctor),
                                                            ReduceFunctor(env, javaReduceFunctor),
                                                            QVariant::fromValue(JObjectWrapper(env, initialValue)),
                                                            QtConcurrent::ReduceOptions(options)));
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_startFiltered
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::objectFromNativeId<QThreadPool>(_threadPool);
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startFiltered(threadPool, JavaSequence(env, javaSequence), FilteredFunctor(env, javaFilteredFunctor)));
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_startFilteredReduced
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
        QThreadPool *threadPool = QtJambiAPI::objectFromNativeId<QThreadPool>(_threadPool);
        FutureWatcherStarter<QVariant>* watcher = new FutureWatcherStarter<QVariant>();
        if(!threadPool)
            threadPool = QThreadPool::globalInstance();
        watcher->setStarter(QtConcurrent::startFilteredReduced<QVariant>(
                                                            threadPool,
                                                            JavaSequence(env, javaSequence),
                                                            FilteredFunctor(env, javaFilteredFunctor),
                                                            ReduceFunctor(env, javaReduceFunctor),
                                                            QVariant::fromValue(JObjectWrapper(env, initialValue)),
                                                            QtConcurrent::ReduceOptions(options)));
        return qtjambi_cast<jobject>(env, static_cast<ThreadEngineStarterInterface *>(watcher));
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_filter
(JNIEnv *env,
 jclass,
 QtJambiNativeID _threadPool,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::objectFromNativeId<QThreadPool>(_threadPool);
        FutureWatcher<void>* watcher = new FutureWatcher<void>(env, javaSequence);
        if(threadPool)
            watcher->setFuture(QtConcurrent::filter(threadPool, watcher->sequence, FilteredFunctor(env, javaFilteredFunctor)));
        else
            watcher->setFuture(QtConcurrent::filter(watcher->sequence, FilteredFunctor(env, javaFilteredFunctor)));
        return qtjambi_cast<jobject>(env, watcher->future());
    } catch (const JavaException& exn) {
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid0
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid1
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid2
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid3
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid4
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid5
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid6
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid7
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid8
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runVoid9
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run0
(JNIEnv *env, jclass, jobject _threadPool, jobject javaCallable)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run1
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run2
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run3
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run4
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run5
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run6
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run7
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run8
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_run9
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        QThreadPool *threadPool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid0
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable)
{
    try{
        using Fun = RunFunctor<QPromise<void>&>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid1
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid2
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid3
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid4
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid5
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        using Fun = RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant,QVariant,QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid6
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid7
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid8
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromiseVoid9
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        using Fun = RunFunctor<QPromise<void>&, QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant,
                                                QVariant, QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise0
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise1
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise2
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise3
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise4
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise5
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise6
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise7
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise8
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_runWithPromise9
(JNIEnv *env, jclass, jobject _threadPool, jobject javaRunnable, jobject a, jobject b, jobject c, jobject d, jobject e, jobject f, jobject g, jobject h, jobject i)
{
    try{
        using Fun = RunFunctor<QPromise<QVariant>&, QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant,
                                                          QVariant, QVariant, QVariant>;
        QThreadPool *pool = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool);
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid0Arg0_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid1Arg1_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid2Arg2_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid3Arg3_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid4Arg4_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid5Arg5_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid6Arg6_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid7Arg7_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid8Arg8_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTaskBuilderVoid9Arg9_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder0Arg0_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder1Arg1_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder2Arg2_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder3Arg3_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(CallableFunctor<QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder4Arg4_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder5Arg5_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder6Arg6_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder7Arg7_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder8Arg8_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedTaskBuilder9Arg9_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid0Arg0_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid1Arg1_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid2Arg2_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid3Arg3_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<void>&,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid4Arg4_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid5Arg5_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid6Arg6_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid7Arg7_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid8Arg8_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QPromiseTaskBuilderVoid9Arg9_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder0Arg0_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&>(env, javaRunnable, futurePointer))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder1Arg1_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder2Arg2_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder3Arg3_spawn
(JNIEnv *env, jclass, jobject _threadPool, jint priority, jobject javaRunnable, jobject a, jobject b, jobject c)
{
    try{
        QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);
        auto task = QtConcurrent::task(RunFunctor<QPromise<QVariant>&,QVariant,QVariant,QVariant>(env, javaRunnable, futurePointer))
                .withArguments(QVariant::fromValue(JObjectWrapper(env, a)),
                           QVariant::fromValue(JObjectWrapper(env, b)),
                           QVariant::fromValue(JObjectWrapper(env, c)))
                .withPriority(priority);
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder4Arg4_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder5Arg5_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder6Arg6_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder7Arg7_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder8Arg8_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_concurrent_QtConcurrent_00024QTypedPromiseTaskBuilder9Arg9_spawn
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
        if(QThreadPool* t = QtJambiAPI::convertJavaObjectToQObject<QThreadPool>(env, _threadPool)){
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

#endif // QT_NO_CONCURRENT


