#include "qtjambi_gui_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
namespace Private{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtGui,io/qt/gui,QClipboard$Text,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtGui,io/qt/gui,QQuaternion$Axes,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/gui/QVector3D;Lio/qt/gui/QVector3D;Lio/qt/gui/QVector3D;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtGui,io/qt/gui,QQuaternion$AxisAndAngle,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/gui/QVector3D;F)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtGui,io/qt/gui,QQuaternion$EulerAngles,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(FFF)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtGui,io/qt/gui,QTextCursor$SelectedTableCells,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIII)
)
}
}
