/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtQml
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtQml,QQmlExpression$ValueIsUndefined,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtQml,QQmlIncubationController$WhileFlag,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(flag))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtQml,QmlTypes,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(registerModule))
private:
    QtQml() = delete;
};

namespace Private {
class Runtime{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,Method,
        QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getReturnType))
private:
    Runtime() = delete;
};

}
}

#endif // QTJAMBI_QML_REPOSITORY_H
