#include <QtCore/QMutex>
#include "qtjambi_gui_repository.h"

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
namespace QtWidgets{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QMenu,
)
#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P) QTJAMBI_REPOSITORY_DEFINE_METHOD(M,P)
#else
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P)
#endif

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QApplication,
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(exec,()I)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(resolveInterface,(Ljava/lang/Class;)Lio/qt/QtObjectInterface;)
                                )
}
}
