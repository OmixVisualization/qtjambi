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

#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M) QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(M)
#else
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M)
#endif

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QApplication,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(exec)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(resolveInterface)
                                     )
}
}

#endif // QTJAMBI_GUI_REPOSITORY_H
