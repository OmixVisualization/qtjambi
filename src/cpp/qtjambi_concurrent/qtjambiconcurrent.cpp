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

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include "qtjambiconcurrent_p.h"
#include "qtjambi_concurrent_repository.h"

#include <QtCore/QtCore>
#include <QtConcurrent/QtConcurrent>

#ifndef QT_NO_CONCURRENT

FutureSequenceCleanUp::FutureSequenceCleanUp(QList<JObjectWrapper> *sequence)
        : m_sequence(sequence)
{
    connect(this, &QFutureWatcher::finished, this, &FutureSequenceCleanUp::cleanUp);
}

FutureSequenceCleanUp::~FutureSequenceCleanUp()
{
    delete m_sequence;
}

void FutureSequenceCleanUp::cleanUp()
{
    delete m_sequence; m_sequence = nullptr;
    deleteLater();
}


class Functor {
public:
    Functor(JNIEnv *env, jobject functor) : m_functor(env, functor)
    {
    }

    Functor(const Functor &other) : m_functor(other.m_functor)
    {
    }

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

    void operator ()(JObjectWrapper &wrapper)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);
                if (javaObject != nullptr){
                    try {
                        Java::QtConcurrent::QtConcurrent$MapFunctor.map(env, functor, javaObject);
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
    typedef JObjectWrapper result_type;

    MappedFunctor(JNIEnv *env, jobject javaMappedFunctor) : Functor(env, javaMappedFunctor) {}
    MappedFunctor(const MapFunctor &other) : Functor(other) {}

    JObjectWrapper operator ()(const JObjectWrapper &wrapper)
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);
                jobject javaResult = nullptr;
                try {
                    javaResult = Java::QtConcurrent::QtConcurrent$MappedFunctor.map(env, functor, javaObject);
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                return qtjambi_to_jobjectwrapper(env, javaResult);
            } else {
                qWarning("Mapped functor called with invalid data. JNI Environment == %p, java functor object == %p",
                        env, m_functor.object());
            }
        }
        return JObjectWrapper();
    }
};

class ReducedFunctor: public Functor {
public:
    ReducedFunctor(JNIEnv *env, jobject javaReducedFunctor) : Functor(env, javaReducedFunctor), m_first_call(true) {}
    ReducedFunctor(const ReducedFunctor &other) : Functor(other), m_first_call(other.m_first_call) {}

    void operator()(JObjectWrapper &result, const JObjectWrapper &wrapper)
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
                        javaResult = Java::QtConcurrent::QtConcurrent$ReducedFunctor.defaultResult(env, functor);
                    } catch (const JavaException& exn) {
                        exn.report(env);
                    }
                    result = JObjectWrapper(env, javaResult);
                }

                jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);
                jobject javaResult = qtjambi_from_jobjectwrapper(env, result);
                try {
                    Java::QtConcurrent::QtConcurrent$ReducedFunctor.reduce(env, functor, javaResult, javaObject);
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

    bool operator()(const JObjectWrapper &wrapper) {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);
                bool result = false;
                try {
                    result = Java::QtConcurrent::QtConcurrent$FilteredFunctor.filter(env, functor, javaObject);
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

class RunFunctor: public Functor {
public:
    typedef void result_type;
    RunFunctor(JNIEnv *env, jobject javaMapFunctor) : Functor(env, javaMapFunctor) {}
    RunFunctor(const MapFunctor &other) : Functor(other) {}

    void operator ()()
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                try {
                    Java::Runtime::Runnable.run(env, functor);
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
            } else {
                qWarning("Run functor called with invalid data. JNI Environment == %p, method id == %p",
                         env, m_functor.object());
            }
        }
    }
};

class CallableFunctor: public Functor {
public:
    typedef JObjectWrapper result_type;
    CallableFunctor(JNIEnv *env, jobject javaMapFunctor) : Functor(env, javaMapFunctor) {}
    CallableFunctor(const MapFunctor &other) : Functor(other) {}

    JObjectWrapper operator ()()
    {
        if (JNIEnv *env = qtjambi_current_environment()) {
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(jobject functor = qtjambi_from_jobjectwrapper(env, m_functor)){
                jobject javaResult = nullptr;
                try{
                    javaResult = Java::Private::Runtime::Callable.call(env,functor);
                }catch(const JavaException& exn){
                    exn.report(env);
                }
                return javaResult ? JObjectWrapper(env, javaResult) : JObjectWrapper();
            } else {
                qWarning("Run functor called with invalid data. JNI Environment == %p, method id == %p",
                         env, m_functor.object());
            }
        }
        return JObjectWrapper();
    }
};

static QList<JObjectWrapper> convertJavaSequenceToCpp(JNIEnv *env, jobject javaSequence)
{
    jobjectArray array = qtjambi_collection_toArray(env, javaSequence);
    jsize arraySize = env->GetArrayLength(array);

    QList<JObjectWrapper> returned;
    for (int i=0; i<arraySize; ++i) {
        jobject javaElement = env->GetObjectArrayElement(array, i);
        JObjectWrapper wrapper = qtjambi_to_jobjectwrapper(env, javaElement);
        returned << wrapper;
    }

    return returned;
}

static jobject convertCppSequenceToJava(JNIEnv *env, const QList<JObjectWrapper> &sequence)
{
    jobject returned = qtjambi_arraylist_new(env, sequence.size());
    for (int i=0; i<sequence.size(); ++i)
        qtjambi_collection_add(env, returned, qtjambi_from_jobjectwrapper(env, sequence.at(i)));

    return returned;
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_map)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);

    // Make sure we don't destroy this list while it's in use
    // (map does in place editing and keeps a reference to the list instead of copying)
    QList<JObjectWrapper> *heapSequence = new QList<JObjectWrapper>(sequence);

    // Make sure the list on the heap is cleaned up when the
    // job is over.
    FutureSequenceCleanUp *cleanUp = new FutureSequenceCleanUp(heapSequence);

    QFuture<void> future = QtConcurrent::map(*heapSequence, MapFunctor(env, javaMapFunctor));
    cleanUp->setFuture(future);

    return qtjambi_cast<jobject>(env, future);
}

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_blockingMap)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    QtJambiScope scope(nullptr);
    QList<JObjectWrapper>& sequence = qtjambi_cast<QList<JObjectWrapper>&>(env, scope, javaSequence);
    QtConcurrent::blockingMap(sequence, MapFunctor(env, javaMapFunctor));
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_mapped)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    QFuture<JObjectWrapper> result = QtConcurrent::mapped(sequence, MappedFunctor(env, javaMappedFunctor));
    return qtjambi_cast<jobject>(env, result);
}


extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_blockingMapped)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    QList<JObjectWrapper> result = QtConcurrent::blockingMapped(sequence, MappedFunctor(env, javaMappedFunctor));
    return convertCppSequenceToJava(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_blockingMappedReduced)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    MappedFunctor mappedFunctor(env, javaMappedFunctor);
    ReducedFunctor reduceFunctor(env, javaReducedFunctor);
    JObjectWrapper result = QtConcurrent::blockingMappedReduced<JObjectWrapper, QList<JObjectWrapper>, MappedFunctor, ReducedFunctor>(sequence, mappedFunctor, reduceFunctor, QtConcurrent::ReduceOptions(options));
    return qtjambi_from_jobjectwrapper(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_mappedReduced)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    MappedFunctor mappedFunctor(env, javaMappedFunctor);
    ReducedFunctor reduceFunctor(env, javaReducedFunctor);
    QFuture<JObjectWrapper> result = QtConcurrent::mappedReduced<JObjectWrapper, QList<JObjectWrapper>, MappedFunctor, ReducedFunctor>(sequence, mappedFunctor, reduceFunctor, QtConcurrent::ReduceOptions(options));
    return qtjambi_cast<jobject>(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_filtered)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    FilteredFunctor filteredFunctor(env, javaFilteredFunctor);
    QFuture<JObjectWrapper> result = QtConcurrent::filtered(sequence, filteredFunctor);
    return qtjambi_cast<jobject>(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_blockingFiltered)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    FilteredFunctor filteredFunctor(env, javaFilteredFunctor);
    QList<JObjectWrapper> result = QtConcurrent::blockingFiltered(sequence, filteredFunctor);
    return convertCppSequenceToJava(env, result);
}

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_blockingFilter)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    QtJambiScope scope(nullptr);
    QList<JObjectWrapper>& sequence = qtjambi_cast<QList<JObjectWrapper>&>(env, scope, javaSequence);
    QtConcurrent::blockingFilter(sequence, FilteredFunctor(env, javaFilteredFunctor));
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_filteredReduced)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    FilteredFunctor filteredFunctor(env, javaFilteredFunctor);
    ReducedFunctor reducedFunctor(env, javaReducedFunctor);
    QFuture<JObjectWrapper> result = QtConcurrent::filteredReduced<JObjectWrapper, QList<JObjectWrapper>, FilteredFunctor, ReducedFunctor>(sequence, filteredFunctor, reducedFunctor, QtConcurrent::ReduceOptions(options));
    return qtjambi_cast<jobject>(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_blockingFilteredReduced)
(JNIEnv *env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(env, javaSequence);
    FilteredFunctor filteredFunctor(env, javaFilteredFunctor);
    ReducedFunctor reducedFunctor(env, javaReducedFunctor);
    JObjectWrapper result = QtConcurrent::blockingFilteredReduced<JObjectWrapper, QList<JObjectWrapper>, FilteredFunctor, ReducedFunctor>(sequence, filteredFunctor, reducedFunctor, QtConcurrent::ReduceOptions(options));
    return qtjambi_from_jobjectwrapper(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run)
(JNIEnv *env, jclass, jobject javaCallable)
{
    QFuture<JObjectWrapper> result = QtConcurrent::run(CallableFunctor(env, javaCallable));
    return qtjambi_cast<jobject>(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_run2)
(JNIEnv *env, jclass, jobject threadPool, jobject javaCallable)
{
    QFuture<JObjectWrapper> result = QtConcurrent::run(qtjambi_cast<QThreadPool*>(env, threadPool), CallableFunctor(env, javaCallable));
    return qtjambi_cast<jobject>(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid)
(JNIEnv *env, jclass, jobject javaRunnable)
{
    QFuture<void> result = QtConcurrent::run(RunFunctor(env, javaRunnable));
    return qtjambi_cast<jobject>(env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_concurrent_QtConcurrent_runVoid2)
(JNIEnv *env, jclass, jobject threadPool, jobject javaRunnable)
{
    QFuture<void> result = QtConcurrent::run(qtjambi_cast<QThreadPool*>(env, threadPool), RunFunctor(env, javaRunnable));
    return qtjambi_cast<jobject>(env, result);
}


#endif // QT_NO_CONCURRENT


