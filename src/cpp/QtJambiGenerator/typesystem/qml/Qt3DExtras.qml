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
    }
    
    ObjectType{
        name: "Qt3DExtras::QConeGeometry"
    }
    
    ObjectType{
        name: "Qt3DExtras::QConeMesh"
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidMesh"
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidGeometry"
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderGeometry"
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderMesh"
    }
    
    ObjectType{
        name: "Qt3DExtras::QDiffuseSpecularMaterial"
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
    }
    
    ObjectType{
        name: "Qt3DExtras::QNormalDiffuseMapAlphaMaterial"
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
    }
    
    ObjectType{
        name: "Qt3DExtras::QPhongAlphaMaterial"
    }
    
    ObjectType{
        name: "Qt3DExtras::QPhongMaterial"
    }
    
    ObjectType{
        name: "Qt3DExtras::QTexturedMetalRoughMaterial"
    }
    
    ObjectType{
        name: "Qt3DExtras::QExtrudedTextGeometry"
    }
    
    ObjectType{
        name: "Qt3DExtras::QExtrudedTextMesh"
    }
    
    ObjectType{
        name: "Qt3DExtras::QFirstPersonCameraController"
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
    }
    
    ObjectType{
        name: "Qt3DExtras::QGoochMaterial"
    }
    
    ObjectType{
        name: "Qt3DExtras::QMetalRoughMaterial"
    }
    
    ObjectType{
        name: "Qt3DExtras::QMorphPhongMaterial"
    }
    
    ObjectType{
        name: "Qt3DExtras::QOrbitCameraController"
    }
    
    ObjectType{
        name: "Qt3DExtras::QPerVertexColorMaterial"
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneGeometry"
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneMesh"
    }
    
    ObjectType{
        name: "Qt3DExtras::QSkyboxEntity"
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereGeometry"
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereMesh"
    }
    
    ObjectType{
        name: "Qt3DExtras::QSpriteGrid"
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
    }
    
    ObjectType{
        name: "Qt3DExtras::QSpriteSheetItem"
    }
    
    ObjectType{
        name: "Qt3DExtras::QText2DEntity"
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
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusGeometry"
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusMesh"
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
    }
    
    ObjectType{
        name: "Qt3DExtras::QConeGeometryView"
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidGeometryView"
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderGeometryView"
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneGeometryView"
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereGeometryView"
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusGeometryView"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractSpriteSheet' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DExtras::QAbstractCameraController::InputState."}
}
