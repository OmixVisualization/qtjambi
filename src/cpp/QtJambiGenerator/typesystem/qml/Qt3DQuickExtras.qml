/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.qt3d.extras.quick"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DQuickExtras"
    module: "qtjambi.qt3dquickextras"
    description: ""

    NamespacePrefix{
        prefix: "Qt3DExtras::Quick"
        namespace: "Qt3DExtras::Quick"
        namingPolicy: NamespacePrefix.Cut
    }

    NamespaceType{
        name: "Qt3DExtras::Quick"
        generate: false
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DExtras::Quick::Qt3DQuickWindow"
        EnumType{
            name: "CameraAspectRatioMode"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "registerAspect(Qt3DCore::QAbstractAspect *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "showEvent(QShowEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        since: [5, 7]
    }
}
