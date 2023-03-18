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
    packageName: "io.qt.qt3d.core.quick"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DQuick"
    module: "qtjambi.qt3dquick"
    description: "Qt3DQuick Contains classes used for implementing QML functionality into Qt3D applications."

    NamespacePrefix{
        prefix: "Qt3DCore::Quick"
        namespace: "Qt3DCore::Quick"
        namingPolicy: NamespacePrefix.Cut
    }

    NamespaceType{
        name: "Qt3DCore::Quick"
        javaName: "Qt3DQuick"
        generate: false
    }
    
    ObjectType{
        name: "Qt3DCore::Quick::QQmlAspectEngine"

        EnumType{
            name: "Status"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setSource(const QUrl &)"
            blockExceptions: true
        }
    }
    
    ObjectType{
        name: "Qt3DCore::Quick::Quick3DConfiguration"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setControlledCamera(Qt3DRender::QCamera*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcControlledCamera"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::Quick::Quick3DEntity"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::Quick::Quick3DEntityLoader"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::Quick::Quick3DNode"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::Quick::Quick3DTransform"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
}
