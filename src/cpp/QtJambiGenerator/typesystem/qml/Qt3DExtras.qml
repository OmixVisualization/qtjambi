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
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "Qt3DExtras"
    module: "qtjambi.qt3dextras"
    description: "The Qt 3D Extras module provides a set of prebuilt elements to help you get started with Qt 3D."
    NamespaceType{
        name: "Qt3DExtras"
        generate: false
        since: [5, 10]
    }
    
    ValueType{
        name: "Qt3DExtras::QAbstractCameraController::InputState"
        javaName: "QAbstractCameraController$InputState"
    }
    
    ObjectType{
        name: "Qt3DExtras::QAbstractCameraController"
        javaName: "QAbstractCameraController"
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
        javaName: "QAbstractSpriteSheet"
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
        javaName: "QConeGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QConeMesh"
        javaName: "QConeMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidMesh"
        javaName: "QCuboidMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidGeometry"
        javaName: "QCuboidGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderGeometry"
        javaName: "QCylinderGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderMesh"
        javaName: "QCylinderMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QDiffuseSpecularMaterial"
        javaName: "QDiffuseSpecularMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QDiffuseMapMaterial"
        javaName: "QDiffuseMapMaterial"
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
        javaName: "QDiffuseSpecularMapMaterial"
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
        javaName: "QNormalDiffuseMapAlphaMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QNormalDiffuseMapMaterial"
        javaName: "QNormalDiffuseMapMaterial"
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
        javaName: "QNormalDiffuseSpecularMapMaterial"
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
        javaName: "QPhongAlphaMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPhongMaterial"
        javaName: "QPhongMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QTexturedMetalRoughMaterial"
        javaName: "QTexturedMetalRoughMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QExtrudedTextGeometry"
        javaName: "QExtrudedTextGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QExtrudedTextMesh"
        javaName: "QExtrudedTextMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QFirstPersonCameraController"
        javaName: "QFirstPersonCameraController"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QForwardRenderer"
        javaName: "QForwardRenderer"
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
        javaName: "QGoochMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QMetalRoughMaterial"
        javaName: "QMetalRoughMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QMorphPhongMaterial"
        javaName: "QMorphPhongMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QOrbitCameraController"
        javaName: "QOrbitCameraController"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPerVertexColorMaterial"
        javaName: "QPerVertexColorMaterial"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneGeometry"
        javaName: "QPlaneGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneMesh"
        javaName: "QPlaneMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSkyboxEntity"
        javaName: "QSkyboxEntity"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereGeometry"
        javaName: "QSphereGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereMesh"
        javaName: "QSphereMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QSpriteGrid"
        javaName: "QSpriteGrid"
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
        javaName: "QSpriteSheet"
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
        javaName: "QSpriteSheetItem"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QText2DEntity"
        javaName: "QText2DEntity"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QTextureMaterial"
        javaName: "QTextureMaterial"
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
        javaName: "QTorusGeometry"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusMesh"
        javaName: "QTorusMesh"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DExtras::Qt3DWindow"
        javaName: "Qt3DWindow"
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
        javaName: "QConeGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QCuboidGeometryView"
        javaName: "QCuboidGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QCylinderGeometryView"
        javaName: "QCylinderGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QPlaneGeometryView"
        javaName: "QPlaneGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QSphereGeometryView"
        javaName: "QSphereGeometryView"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DExtras::QTorusGeometryView"
        javaName: "QTorusGeometryView"
        since: 6
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractSpriteSheet' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DExtras::QAbstractCameraController::InputState."}
}
