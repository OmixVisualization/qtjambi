/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#if !defined(QTJAMBI_OBJECTDATA_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_OBJECTDATA_H

#include <QtCore/QObject>
#include <typeinfo>
#include "global.h"

struct QtJambiObjectData;
struct QtJambiObjectDataReadLockPrivate;
struct QtJambiObjectDataWriteLockPrivate;

struct QtJambiObjectDataReadLock{
    QTJAMBI_EXPORT ~QtJambiObjectDataReadLock();
    QTJAMBI_EXPORT void unlock();
    QTJAMBI_EXPORT void relock();
    QtJambiObjectDataReadLock(QtJambiObjectDataReadLockPrivate&);
private:
    friend QtJambiObjectData;
    struct QtJambiObjectDataReadLockPrivate* p = nullptr;
};

struct QtJambiObjectDataWriteLock{
    QTJAMBI_EXPORT ~QtJambiObjectDataWriteLock();
    QTJAMBI_EXPORT void unlock();
    QTJAMBI_EXPORT void relock();
    QtJambiObjectDataWriteLock(QtJambiObjectDataWriteLockPrivate&);
private:
    friend QtJambiObjectData;
    QtJambiObjectDataWriteLockPrivate* p = nullptr;
};

struct QTJAMBI_EXPORT QtJambiObjectData{
protected:
    QtJambiObjectData();
public:
    virtual ~QtJambiObjectData();
    static QtJambiObjectData* userData(const QObject* object, const std::type_info& id);
    static QtJambiObjectData* setUserData(QObject* object, const std::type_info& id, QtJambiObjectData* data);

    static QtJambiObjectDataWriteLock writeLock();
    static QtJambiObjectDataReadLock readLock();

    template<typename T>
    static T* userData(const QObject* object){
        return static_cast<T*>(userData(object, typeid(T)));
    }

    template<typename T>
    static T* setUserData(QObject* object, T* data){
        return static_cast<T*>(setUserData(object, typeid(T), data));
    }

    static bool isRejectedUserProperty(const QObject* object, const char * propertyName);
    Q_DISABLE_COPY_MOVE(QtJambiObjectData)
};

#endif // QTJAMBI_OBJECTDATA_H
