#include "qtjambi_winextras_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtWinExtras,io/qt/winextras,QtWin$ColorizationColorInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/gui/QColor;Z)
)
}
