#ifndef QTJAMBI_REMOTE_OJECTS_REPOSITORY_H
#define QTJAMBI_REMOTE_OJECTS_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtRemoteObjects
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtRemoteObjects,IoDeviceBase$ReadResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
private:
    QtRemoteObjects() = delete;
};
}

#endif // QTJAMBI_REMOTE_OJECTS_REPOSITORY_H
