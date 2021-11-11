#ifndef QTJAMBI_GUI_REPOSITORY_H
#define QTJAMBI_GUI_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtGui{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QClipboard$Text,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuaternion$Axes,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuaternion$AxisAndAngle,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuaternion$EulerAngles,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QTextCursor$SelectedTableCells,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
}
namespace QtWidgets{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMenu,)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QApplication,
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(exec))
}
}

#endif // QTJAMBI_GUI_REPOSITORY_H
