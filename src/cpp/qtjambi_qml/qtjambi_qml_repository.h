/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_QML_REPOSITORY_H
#define QTJAMBI_QML_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtJambi {
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtMetaType,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(type)
                                 QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(name)
                                 QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(id))
}

namespace QtQml {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlExpression$ValueIsUndefined,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlIncubationController$WhileFlag,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(flag))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlListProperty,)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlListProperty$ReplaceFunction,
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlListProperty$AtFunction,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QJSValue,)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlTypeRegistrationException,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())
    namespace Util{
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlSingleton,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlElement,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlNamedElement,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlValueType,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlUncreatable,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlUnavailable,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlExtended,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlAnonymous,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlInterface,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlForeign,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(metaType))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlAttached,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlSequencialContainer,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(valueType))
    }
}
}

#endif // QTJAMBI_QML_REPOSITORY_H
