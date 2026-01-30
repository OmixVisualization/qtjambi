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
    packageName: "io.qt.svg"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtSvg"
    module: "qtjambi.svg"
    description: "Classes for displaying the contents of SVG files. Supports a subset of the SVG 1.2 Tiny standard."
    NamespaceType{
        name: "QtSvg"
        EnumType{
            name: "Option"
        }
        since: 6.7
    }
    
    ObjectType{
        name: "QSvgRenderer"
    }
    
    ObjectType{
        name: "QSvgGenerator"
        EnumType{
            name: "SvgVersion"
            since: [6,5]
        }
        ModifyFunction{
            signature: "setOutputDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcOutputDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
}
