#include "qtjambi_concurrent_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtConcurrent,io/qt/concurrent,QtConcurrent$MapFunctor,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(map,(Ljava/lang/Object;)V)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtConcurrent,io/qt/concurrent,QtConcurrent$MappedFunctor,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(map,(Ljava/lang/Object;)Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtConcurrent,io/qt/concurrent,QtConcurrent$ReducedFunctor,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(reduce,(Ljava/lang/Object;Ljava/lang/Object;)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(defaultResult,()Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtConcurrent,io/qt/concurrent,QtConcurrent$FilteredFunctor,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(filter,(Ljava/lang/Object;)Z)
)

namespace Private{
QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util/concurrent,Callable,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(call,()Ljava/lang/Object;)
)
}
}
