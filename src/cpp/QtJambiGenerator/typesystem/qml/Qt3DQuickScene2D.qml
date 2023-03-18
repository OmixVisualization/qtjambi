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
    packageName: "io.qt.qt3d.render.quick.scene2d"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DQuickScene2D"
    module: "qtjambi.qt3dquickscene2d"
    description: "Qt 3D Scene2D Module"

    NamespacePrefix{
        prefix: "Qt3DRender::Quick"
        namespace: "Qt3DRender::Quick"
        namingPolicy: NamespacePrefix.Cut
    }

    NamespaceType{
        name: "Qt3DRender::Quick"
        generate: false
    }
    
    ObjectType{
        name: "Qt3DRender::Quick::QScene2D"
        EnumType{
            name: "RenderPolicy"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "entities()"
            remove: RemoveFlag.All
            since: [5, 15]
            until: [6, 0, 0]
        }
        ModifyFunction{
            signature: "addEntity(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeEntity(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setItem(QQuickItem *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setOutput(Qt3DRender::QRenderTargetOutput *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 11]
    }
}
