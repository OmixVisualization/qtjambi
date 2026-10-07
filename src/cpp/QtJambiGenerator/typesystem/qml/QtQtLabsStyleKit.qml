/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
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

import QtJambiGenerator 1.0

TypeSystem{
    packageName: "io.qt.labs.stylekit"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtLabsStyleKit"
    module: "qtjambi.labsstylekit"
    description: "Provides classes for integrating Qt Labs StyleKit with Qt Widgets."
    LoadTypeSystem{name: "QtWidgets"}
    ObjectType{
        name: "QStyleKitStyle"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
namespace Java{
namespace QtWidgets{
QTJAMBI_REPOSITORY_DECLARE_CLASS(QStyle,
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findSubControl))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QStyle,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findSubControl,(II)Lio/qt/widgets/QStyle$SubControl;)
)
}
}
                `}
        }
    }
}
