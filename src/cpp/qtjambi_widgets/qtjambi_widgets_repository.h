#ifndef QTJAMBI_WIDGETS_REPOSITORY_H
#define QTJAMBI_WIDGETS_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtWidgets
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtWidgets,QGraphicsItem$BlockedByModalPanelInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtWidgets,QFileDialog$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtWidgets,QFormLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtWidgets,QGridLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtWidgets,QSplitter$Range,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
private:
    QtWidgets() = delete;
};
}

#endif // QTJAMBI_WIDGETS_REPOSITORY_H
