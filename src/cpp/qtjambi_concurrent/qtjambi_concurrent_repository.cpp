#include <QtCore/QMutex>
#include "qtjambi_concurrent_repository.h"

namespace Java{
namespace QtConcurrent{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$MapFunctor,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(map,(Ljava/lang/Object;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$MappedFunctor,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(map,(Ljava/lang/Object;)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/concurrent,QtConcurrent$ReducedFunctor,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(reduce,(Ljava/lang/Object;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(defaultResult,()Ljava/lang/Object;)
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
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
#endif

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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
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
#endif
}
namespace Runtime{
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util/concurrent,Callable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(call,()Ljava/lang/Object;)
)
}
}
