#include "qtjambi_webenginewidgets_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
namespace Private{
QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,java/util/function,Consumer,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;)V)
)
}
}
