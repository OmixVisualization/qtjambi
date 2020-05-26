#ifndef QTJAMBI_GUI_REPOSITORY_H
#define QTJAMBI_GUI_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace Private{
class QtGui
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGui,QClipboard$Text,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGui,QQuaternion$Axes,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGui,QQuaternion$AxisAndAngle,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGui,QQuaternion$EulerAngles,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtGui,QTextCursor$SelectedTableCells,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
private:
    QtGui() = delete;
};
}
}

#endif // QTJAMBI_GUI_REPOSITORY_H
