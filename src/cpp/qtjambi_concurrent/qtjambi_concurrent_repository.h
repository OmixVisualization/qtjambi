#ifndef QTJAMBI_CONCURRENT_REPOSITORY_H
#define QTJAMBI_CONCURRENT_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{

class QtConcurrent
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent,QtConcurrent$MapFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(map))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent,QtConcurrent$MappedFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(map))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent,QtConcurrent$ReducedFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(reduce)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(defaultResult))

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtConcurrent,QtConcurrent$FilteredFunctor,
                  QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(filter))
private:
    QtConcurrent() = delete;
};

namespace Private{

class Runtime
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Callable,
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
private:
    Runtime() = delete;
};

}
}

#endif // QTJAMBI_CONCURRENT_REPOSITORY_H
