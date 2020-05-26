#ifndef QTJAMBI_WEBENGINEWIDGETS_REPOSITORY_H
#define QTJAMBI_WEBENGINEWIDGETS_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace Private{
class Runtime
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Consumer,
                  QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
private:
    Runtime() = delete;
};
}
}
#endif // QTJAMBI_WEBENGINEWIDGETS_REPOSITORY_H
