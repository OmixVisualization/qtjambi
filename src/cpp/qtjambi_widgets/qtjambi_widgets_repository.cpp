#include "qtjambi_widgets_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtWidgets,io/qt/widgets,QGraphicsItem$BlockedByModalPanelInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLio/qt/widgets/QGraphicsItem;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtWidgets,io/qt/widgets,QFileDialog$Result,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtWidgets,io/qt/widgets,QFormLayout$ItemInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ILio/qt/widgets/QFormLayout$ItemInfo;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtWidgets,io/qt/widgets,QGridLayout$ItemInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIII)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtWidgets,io/qt/widgets,QSplitter$Range,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(II)
)
}
