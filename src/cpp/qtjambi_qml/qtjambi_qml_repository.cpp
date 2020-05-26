#include "qtjambi_qml_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtQml,io/qt/qml,QQmlExpression$ValueIsUndefined,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtQml,io/qt/qml,QQmlIncubationController$WhileFlag,
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(flag,J)
)

namespace Private {

    QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/lang/reflect,Method,
        QTJAMBI_REPOSITORY_DEFINE_METHOD(getReturnType,()Ljava/lang/Class;)
    )

}

}
