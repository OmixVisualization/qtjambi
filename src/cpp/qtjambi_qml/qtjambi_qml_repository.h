#ifndef QTJAMBI_QML_REPOSITORY_H
#define QTJAMBI_QML_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtQml
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtQml,QQmlExpression$ValueIsUndefined,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtQml,QQmlIncubationController$WhileFlag,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(flag))
private:
    QtQml() = delete;
};

namespace Private {
class Runtime{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Method,
        QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getReturnType))
private:
    Runtime() = delete;
};

}
}

#endif // QTJAMBI_QML_REPOSITORY_H
