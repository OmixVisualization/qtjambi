#include <QtCore/QMutex>
#include "qtjambi_gui_repository.h"

Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

namespace Java{
namespace QtGui{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QClipboard$Text,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QQuaternion$Axes,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/gui/QVector3D;Lio/qt/gui/QVector3D;Lio/qt/gui/QVector3D;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QQuaternion$AxisAndAngle,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/gui/QVector3D;F)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QQuaternion$EulerAngles,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(FFF)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QTextCursor$SelectedTableCells,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIII)
)
}
}
