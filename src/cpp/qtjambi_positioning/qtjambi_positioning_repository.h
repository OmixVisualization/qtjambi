#ifndef QTJAMBI_POSITIONING_REPOSITORY_H
#define QTJAMBI_POSITIONING_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtPositioning
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtPositioning,QNmeaPositionInfoSource$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(info)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasFix))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtPositioning,QtJambiSignals$AbstractSignal,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(containingObject)
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(methodIndex))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtPositioning,QMetaMethod,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(toSignal))
private:
    QtPositioning() = delete;
};
}

#endif // QTJAMBI_POSITIONING_REPOSITORY_H
