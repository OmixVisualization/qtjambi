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
#include "qtjambiconcurrent_p.h"

#include <QtCore/QtCore>
#include <QtConcurrent/QtConcurrent>

#ifndef QT_NO_CONCURRENT

FutureSequenceCleanUp::FutureSequenceCleanUp(QList<JObjectWrapper> *sequence)
        : m_sequence(sequence)
{
    connect(this, SIGNAL(finished()), this, SLOT(cleanUp()));
}

FutureSequenceCleanUp::~FutureSequenceCleanUp()
{
    delete m_sequence;
}

void FutureSequenceCleanUp::cleanUp()
{
    delete m_sequence; m_sequence = 0;
    deleteLater();
}


class Functor {
public:
    Functor(jobject functor) : m_functor(0)
    {
        init(functor);
    }

    Functor(const Functor &other) : m_functor(0)
    {
        init(other.m_functor);
    }

    virtual ~Functor()
    {
        JNIEnv *env = qtjambi_current_environment();
        if (env != 0)
            env->DeleteGlobalRef(m_functor);
    }

protected:
    jobject m_functor;

private:
    void init(jobject functor) {
        JNIEnv *env = qtjambi_current_environment();
        if (env != 0)
            m_functor = env->NewGlobalRef(functor);
    }


};

class MapFunctor: public Functor {
public:
    MapFunctor(jobject javaMapFunctor) : Functor(javaMapFunctor) {}
    MapFunctor(const MapFunctor &other) : Functor(other) {}

    void operator ()(JObjectWrapper &wrapper)
    {
        JNIEnv *env = qtjambi_current_environment();
        if (env != 0 && m_functor) {
            jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);
            StaticCache *sc = StaticCache::instance();
            sc->resolveQtConcurrent_MapFunctor();

            if (javaObject != 0)
                env->CallVoidMethod(m_functor, sc->QtConcurrent_MapFunctor.map, javaObject);
        } else {
            qWarning("Map functor called with invalid data. JNI Environment == %p, java functor object == %p",
                    env, m_functor);
        }
    }
};

class MappedFunctor: public Functor {
public:
    typedef JObjectWrapper result_type;

    MappedFunctor(jobject javaMappedFunctor) : Functor(javaMappedFunctor) {}
    MappedFunctor(const MapFunctor &other) : Functor(other) {}

    JObjectWrapper operator ()(const JObjectWrapper &wrapper)
    {
        JNIEnv *env = qtjambi_current_environment();
        if (env != 0 && m_functor) {
            jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);

            StaticCache *sc = StaticCache::instance();
            sc->resolveQtConcurrent_MappedFunctor();

            jobject javaResult = env->CallObjectMethod(m_functor, sc->QtConcurrent_MappedFunctor.map, javaObject);
            return qtjambi_to_jobjectwrapper(env, javaResult);
        } else {
            qWarning("Mapped functor called with invalid data. JNI Environment == %p, java functor object == %p",
                    env, m_functor);
            return JObjectWrapper();
        }
    }
};

class ReducedFunctor: public Functor {
public:
    ReducedFunctor(jobject javaReducedFunctor) : Functor(javaReducedFunctor), m_first_call(true) {}
    ReducedFunctor(const ReducedFunctor &other) : Functor(other), m_first_call(other.m_first_call) {}

    void operator()(JObjectWrapper &result, const JObjectWrapper &wrapper)
    {
        JNIEnv *env = qtjambi_current_environment();

        if (env != 0 && m_functor != 0) {
            StaticCache *sc = StaticCache::instance();
            sc->resolveQtConcurrent_ReducedFunctor();

            // reduce() is synchronous, so while this data is static in terms
            // of the map/reduce operation, it does not need to be protected
            if (m_first_call) {
                m_first_call = false;
                result = JObjectWrapper(env, env->CallObjectMethod(m_functor, sc->QtConcurrent_ReducedFunctor.defaultResult));
            }

            jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);
            jobject javaResult = qtjambi_from_jobjectwrapper(env, result);
            env->CallVoidMethod(m_functor, sc->QtConcurrent_ReducedFunctor.reduce, javaResult, javaObject);
        } else {
            qWarning("Reduce functor called with invalid data. JNI Environment == %p, java functor object == %p",
                    env, m_functor);
        }
    }

private:
    uint m_first_call : 1;

};

class FilteredFunctor: public Functor {
public:
    FilteredFunctor(jobject javaFilteredFunctor) : Functor(javaFilteredFunctor) {}
    FilteredFunctor(const FilteredFunctor &other) : Functor(other) {}

    bool operator()(const JObjectWrapper &wrapper) {
        JNIEnv *env = qtjambi_current_environment();
        if (env != 0 && m_functor != 0) {
            jobject javaObject = qtjambi_from_jobjectwrapper(env, wrapper);

            StaticCache *sc = StaticCache::instance();
            sc->resolveQtConcurrent_FilteredFunctor();

            return env->CallBooleanMethod(m_functor, sc->QtConcurrent_FilteredFunctor.filter, javaObject);
        } else {
            qWarning("Filtered functor called with invalid data. JNI Environment == %p, java functor object == %p",
                    env, m_functor);
            return false;
        }
    }
};

class RunFunctorBase: public Functor
{
public:

    RunFunctorBase(jobject javaThis, jclass declaringClass, jmethodID javaMethodId, jobjectArray javaArguments, jintArray typeConversionScheme,
                   jbyte resultType)
        : Functor(javaThis), m_declaring_class(0), m_method_id(javaMethodId), m_result_type(resultType), m_conversion_scheme(0)
    {
        init(declaringClass, javaArguments, QVarLengthArray<jvalue>(), typeConversionScheme);
    }

    RunFunctorBase(const RunFunctorBase &other)
        : Functor(other), m_declaring_class(0), m_method_id(other.m_method_id), m_result_type(other.m_result_type), m_conversion_scheme(0)
    {
        init(other.m_declaring_class, 0, other.m_arguments, other.m_conversion_scheme);
    }

    ~RunFunctorBase() {
        JNIEnv *env = qtjambi_current_environment();
        if (env != 0) {
            if (m_declaring_class != 0)
                env->DeleteGlobalRef(m_declaring_class);

            jint *a = m_conversion_scheme != 0 ? env->GetIntArrayElements(m_conversion_scheme, 0) : 0;
            for (int i=0; i<m_arguments.size(); ++i) {
                if (a != 0 && a[i] == 'L')
                    env->DeleteGlobalRef(m_arguments[i].l);
            }
            env->ReleaseIntArrayElements(m_conversion_scheme, a, JNI_ABORT);
            if (m_conversion_scheme != 0)
                env->DeleteGlobalRef(m_conversion_scheme);
        }
    }


private:
    void init(jclass declaringClass, jobjectArray javaArguments, QVarLengthArray<jvalue> convertedArgs, jintArray conversionScheme) {
        JNIEnv *env = qtjambi_current_environment();
        Q_ASSERT(env != 0);
        if (env != 0) {
            if (declaringClass != 0)
                m_declaring_class = reinterpret_cast<jclass>(env->NewGlobalRef(declaringClass));
            if (conversionScheme != 0)
                m_conversion_scheme = reinterpret_cast<jintArray>(env->NewGlobalRef(conversionScheme));

            if (javaArguments != 0) {
                // Convert all the arguments
                m_arguments = qtjambi_from_jobjectArray(env, javaArguments, m_conversion_scheme, true);
            } else {
                // Copy the converted arguments
                jint *a = m_conversion_scheme != 0 ? env->GetIntArrayElements(m_conversion_scheme, 0) : 0;
                for (int i=0; i<convertedArgs.size(); ++i) {
                    if (a != 0 && a[i] == 'L') {
                        jvalue val;
                        val.l = env->NewGlobalRef(convertedArgs[i].l);
                        m_arguments.append(val);
                    } else {
                        m_arguments.append(convertedArgs[i]);
                    }
                }
                env->ReleaseIntArrayElements(m_conversion_scheme, a, JNI_ABORT);
            }
        }
    }

protected:
    jclass m_declaring_class;
    jmethodID m_method_id;
    QVarLengthArray<jvalue> m_arguments;
    jbyte m_result_type;
    jintArray m_conversion_scheme;
};

class RunFunctor: public RunFunctorBase {
public:
    typedef JObjectWrapper result_type;

    RunFunctor(jobject javaThis, jclass declaringClass, jmethodID javaMethodId, jobjectArray javaArguments, jintArray typeConversionScheme, jbyte resultType)
        : RunFunctorBase(javaThis, declaringClass, javaMethodId, javaArguments, typeConversionScheme, resultType)
    {
    }

    RunFunctor(const RunFunctor &other)
        : RunFunctorBase(other)
    {
    }

    JObjectWrapper operator()() {
        JNIEnv *env = qtjambi_current_environment();
        Q_ASSERT(env != 0);
        if (env != 0 && m_method_id != 0) {
            jobject javaResult = qtjambi_invoke_method(env, m_functor, m_method_id, m_result_type, m_arguments);
            return javaResult != 0 ? qtjambi_to_jobjectwrapper(env, javaResult) : JObjectWrapper();
        } else {
            qWarning("Run functor called with invalid data. JNI Environment == %p, method id == %p",
                     env, m_method_id);
            return JObjectWrapper();
        }
    }
};

class RunVoidFunctor: public RunFunctorBase {
public:
    typedef void result_type;

    RunVoidFunctor(jobject javaThis, jclass declaringClass, jmethodID javaMethodId, jobjectArray javaArguments, jintArray conversionScheme)
        : RunFunctorBase(javaThis, declaringClass, javaMethodId, javaArguments, conversionScheme, 'V')
    {
    }

    RunVoidFunctor(const RunFunctor &other)
        : RunFunctorBase(other)
    {
    }

    void operator()() {
        JNIEnv *env = qtjambi_current_environment();
        Q_ASSERT(env != 0);
        if (env != 0 && m_method_id != 0) {
            qtjambi_invoke_method(env, m_functor, m_method_id, m_result_type, m_arguments);
        } else {
            qWarning("Run functor called with invalid data. JNI Environment == %p, method_id == %p",
                     env, m_method_id);
        }
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
        QTJAMBI_EXCEPTION_CHECK(env);
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

static jobject convertCppFutureToJava(JNIEnv *env, const QFuture<JObjectWrapper> &future)
{
    return qtjambi_from_object(env, &future, "QFuture", "org/qtjambi/qt/core/", true);
}

static jobject convertCppFutureVoidToJava(JNIEnv *env, const QFuture<void> &future)
{
    return qtjambi_from_object(env, &future, "QFutureVoid", "org/qtjambi/qt/core/", true);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_map)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    // Make sure we don't destroy this list while it's in use
    // (map does in place editing and keeps a reference to the list instead of copying)
    QList<JObjectWrapper> *heapSequence = new QList<JObjectWrapper>(sequence);

    // Make sure the list on the heap is cleaned up when the
    // job is over.
    FutureSequenceCleanUp *cleanUp = new FutureSequenceCleanUp(heapSequence);

    MapFunctor mapFunctor(javaMapFunctor);
    QFuture<void> future = QtConcurrent::map(*heapSequence, mapFunctor);
    cleanUp->setFuture(future);

    return convertCppFutureVoidToJava(__jni_env, future);
}

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_blockingMap)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaMapFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    MapFunctor mapFunctor(javaMapFunctor);
    QtConcurrent::blockingMap(sequence, mapFunctor);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_mapped)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    MappedFunctor mappedFunctor(javaMappedFunctor);
    QFuture<JObjectWrapper> result = QtConcurrent::mapped(sequence, mappedFunctor);

    return convertCppFutureToJava(__jni_env, result);
}


extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_blockingMapped)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    MappedFunctor mappedFunctor(javaMappedFunctor);
    QList<JObjectWrapper> result = QtConcurrent::blockingMapped(sequence, mappedFunctor);

    return convertCppSequenceToJava(__jni_env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_blockingMappedReduced)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    MappedFunctor mappedFunctor(javaMappedFunctor);
    ReducedFunctor reduceFunctor(javaReducedFunctor);
    JObjectWrapper result = QtConcurrent::blockingMappedReduced<JObjectWrapper, QList<JObjectWrapper>, MappedFunctor, ReducedFunctor>(sequence, mappedFunctor, reduceFunctor, QtConcurrent::ReduceOptions(options));

    return qtjambi_from_jobjectwrapper(__jni_env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_mappedReduced)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaMappedFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    MappedFunctor mappedFunctor(javaMappedFunctor);
    ReducedFunctor reduceFunctor(javaReducedFunctor);
    QFuture<JObjectWrapper> result = QtConcurrent::mappedReduced<JObjectWrapper, QList<JObjectWrapper>, MappedFunctor, ReducedFunctor>(sequence, mappedFunctor, reduceFunctor, QtConcurrent::ReduceOptions(options));

    return convertCppFutureToJava(__jni_env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_filtered)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    FilteredFunctor filteredFunctor(javaFilteredFunctor);
    QFuture<JObjectWrapper> result = QtConcurrent::filtered(sequence, filteredFunctor);

    return convertCppFutureToJava(__jni_env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_blockingFiltered)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    FilteredFunctor filteredFunctor(javaFilteredFunctor);
    QList<JObjectWrapper> result = QtConcurrent::blockingFiltered(sequence, filteredFunctor);

    return convertCppSequenceToJava(__jni_env, result);
}


extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_filteredReduced)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    FilteredFunctor filteredFunctor(javaFilteredFunctor);
    ReducedFunctor reducedFunctor(javaReducedFunctor);
    QFuture<JObjectWrapper> result = QtConcurrent::filteredReduced<JObjectWrapper, QList<JObjectWrapper>, FilteredFunctor, ReducedFunctor>(sequence, filteredFunctor, reducedFunctor, QtConcurrent::ReduceOptions(options));

    return convertCppFutureToJava(__jni_env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_blockingFilteredReduced)
(JNIEnv *__jni_env,
 jclass,
 jobject javaSequence,
 jobject javaFilteredFunctor,
 jobject javaReducedFunctor,
 jint options)
{
    QList<JObjectWrapper> sequence = convertJavaSequenceToCpp(__jni_env, javaSequence);

    FilteredFunctor filteredFunctor(javaFilteredFunctor);
    ReducedFunctor reducedFunctor(javaReducedFunctor);
    JObjectWrapper result = QtConcurrent::blockingFilteredReduced<JObjectWrapper, QList<JObjectWrapper>, FilteredFunctor, ReducedFunctor>(sequence, filteredFunctor, reducedFunctor, QtConcurrent::ReduceOptions(options));

    return qtjambi_from_jobjectwrapper(__jni_env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_runPrivate)
(JNIEnv *__jni_env,
 jclass,
 jobject javaThis,
 jclass declaringClass,
 jobject javaMethod,
 jobjectArray javaArgs,
 jintArray typeConversionScheme,
 jbyte resultType)
{
    jmethodID methodId = __jni_env->FromReflectedMethod(javaMethod);
    Q_ASSERT(methodId);

    RunFunctor runFunctor(javaThis, declaringClass, methodId, javaArgs, typeConversionScheme, resultType);
    QFuture<JObjectWrapper> result = QtConcurrent::run(runFunctor);

    return convertCppFutureToJava(__jni_env, result);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_concurrent_QtConcurrent_runVoidMethodPrivate)
(JNIEnv *__jni_env,
 jclass,
 jobject javaThis,
 jclass declaringClass,
 jobject javaMethod,
 jobjectArray javaArgs,
 jintArray typeConversionScheme)
{
    jmethodID methodId = __jni_env->FromReflectedMethod(javaMethod);
    Q_ASSERT(methodId);

    RunVoidFunctor runFunctor(javaThis, declaringClass, methodId, javaArgs, typeConversionScheme);
    QFuture<void> result = QtConcurrent::run(runFunctor);

    return convertCppFutureVoidToJava(__jni_env, result);
}


#endif // QT_NO_CONCURRENT
