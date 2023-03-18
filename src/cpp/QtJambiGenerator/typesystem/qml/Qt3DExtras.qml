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
    packageName: "io.qt.qt3d.extras"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DExtras"
    module: "qtjambi.qt3dextras"
    description: "The Qt 3D Extras module provides a set of prebuilt elements to help you get started with Qt 3D."

    NamespacePrefix{
        prefix: "Qt3DExtras"
        namespace: "Qt3DExtras"
        namingPolicy: NamespacePrefix.Cut
    }

    NamespaceType{
        name: "Qt3DExtras"
        generate: false
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QAbstractCameraController"

        ValueType{
            name: "InputState"
        }
        ModifyFunction{
            signature: "setCamera(Qt3DRender::QCamera*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QAbstractSpriteSheet"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setTexture(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTexture"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QConeGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QConeMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QDiffuseSpecularMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QDiffuseMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QDiffuseSpecularMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSpecular(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSpecular"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QNormalDiffuseMapAlphaMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QNormalDiffuseMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setNormal(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNormal"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QNormalDiffuseSpecularMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSpecular(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSpecular"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setNormal(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNormal"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPhongAlphaMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPhongMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QTexturedMetalRoughMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QExtrudedTextGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QExtrudedTextMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QFirstPersonCameraController"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QForwardRenderer"
        ModifyFunction{
            signature: "setCamera(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setSurface(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QGoochMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QMetalRoughMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QMorphPhongMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QOrbitCameraController"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPerVertexColorMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSkyboxEntity"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSpriteGrid"
        since: [5, 10]
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DExtras::QSpriteSheet"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "addSprite(Qt3DExtras::QSpriteSheetItem*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeSprite(Qt3DExtras::QSpriteSheetItem*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSpriteSheetItem"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QText2DEntity"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QTextureMaterial"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setTexture(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTexture"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::Qt3DWindow"
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
            signature: "setActiveFrameGraph(Qt3DRender::QFrameGraphNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setRootEntity(Qt3DCore::QEntity*)"
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
        ModifyFunction{
            signature: "resizeEvent(QResizeEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QConeGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusGeometryView"
        since: 6
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractSpriteSheet' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DExtras::QAbstractCameraController::InputState."}
}
