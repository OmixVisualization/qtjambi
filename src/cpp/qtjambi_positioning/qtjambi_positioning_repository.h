#ifndef QTJAMBI_POSITIONING_REPOSITORY_H
#define QTJAMBI_POSITIONING_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtPositioning {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QNmeaPositionInfoSource$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(info)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasFix))
}
namespace QtJambi {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiSignals$AbstractSignal,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(containingObject)
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(methodIndex))
}
namespace QtCore {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaMethod,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(toSignal))
}
}

#endif // QTJAMBI_POSITIONING_REPOSITORY_H
