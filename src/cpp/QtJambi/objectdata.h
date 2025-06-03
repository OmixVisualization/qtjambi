/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
using QtJambiObjectData = QObjectUserData;
#define QTJAMBI_OBJECTUSERDATA_TYPE_ID(TYPE) TYPE::id()
#define QTJAMBI_OBJECTUSERDATA_ID_TYPE uint
#define QTJAMBI_GET_OBJECTUSERDATA(TYPE, object) static_cast<TYPE*>(object->userData(TYPE::id()))
#define QTJAMBI_SET_OBJECTUSERDATA(TYPE, object, data) object->setUserData(TYPE::id(), data)
#define QTJAMBI_SET_OBJECTUSERDATA_ID(ID, object, data) object->setUserData(ID, data)
#define QTJAMBI_OBJECTUSERDATA_ID_DECL static uint id();
#define QTJAMBI_OBJECTUSERDATA_ID_IMPL(STATIC,SCOPE)STATIC uint SCOPE id(){\
static uint _id = QObject::registerUserData();\
    return _id;\
}
namespace CoreAPI{
QTJAMBI_EXPORT QDebug& appendToDebugStream(QDebug& debug, uint typeId, const void* ptr);
}
#else
struct QTJAMBI_EXPORT QtJambiObjectData{
protected:
    QtJambiObjectData();
public:
    virtual ~QtJambiObjectData();
    static QtJambiObjectData* userData(const QObject* object, const std::type_info& id);
    static void setUserData(QObject* object, const std::type_info& id, QtJambiObjectData* data);

    template<typename T>
    static T* userData(const QObject* object){
        return static_cast<T*>(userData(object, typeid(T)));
    }

    template<typename T>
    static void setUserData(QObject* object, T* data){
        setUserData(object, typeid(T), data);
    }

    static bool isRejectedUserProperty(const QObject* object, const char * propertyName);
    Q_DISABLE_COPY_MOVE(QtJambiObjectData)
};
#define QTJAMBI_OBJECTUSERDATA_TYPE_ID(TYPE) typeid(TYPE)
#define QTJAMBI_OBJECTUSERDATA_ID_TYPE const std::type_info&
#define QTJAMBI_OBJECTUSERDATA_ID_DECL
#define QTJAMBI_OBJECTUSERDATA_ID_IMPL(STATIC,SCOPE)
#define QTJAMBI_GET_OBJECTUSERDATA(TYPE, object) QtJambiObjectData::userData<TYPE>(object)
#define QTJAMBI_SET_OBJECTUSERDATA(TYPE, object, data) QtJambiObjectData::setUserData<TYPE>(object, data)
#define QTJAMBI_SET_OBJECTUSERDATA_ID(ID, object, data) QtJambiObjectData::setUserData(object, ID, data)
#endif

#endif // QTJAMBI_OBJECTDATA_H
