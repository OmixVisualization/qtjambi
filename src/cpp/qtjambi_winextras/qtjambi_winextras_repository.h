#ifndef QTJAMBI_WINEXTRAS_REPOSITORY_H
#define QTJAMBI_WINEXTRAS_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtWinExtras
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtWinExtras,QtWin$ColorizationColorInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
private:
    QtWinExtras() = delete;
};
}

#endif // QTJAMBI_WINEXTRAS_REPOSITORY_H
