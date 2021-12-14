#ifndef QTJAMBI_WIDGETS_REPOSITORY_H
#define QTJAMBI_WIDGETS_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtWidgets{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGraphicsItem$BlockedByModalPanelInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFileDialog$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFormLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGridLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSplitter$Range,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QStyle,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findSubControl))
}
}

#endif // QTJAMBI_WIDGETS_REPOSITORY_H
