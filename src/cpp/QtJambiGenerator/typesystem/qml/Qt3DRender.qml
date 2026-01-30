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
    packageName: "io.qt.qt3d.render"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DRender"
    module: "qtjambi.qt3drender"
    description: "Contains classes that enable 2D and 3D rendering"
    RequiredLibrary{
        name: "QtConcurrent"
    }
    RequiredLibrary{
        name: "QtShaderTools"
        mode: RequiredLibrary.ProvideOnly
    }

    NamespacePrefix{
        prefix: "Qt3DRender"
        namespace: "Qt3DRender"
        namingPolicy: NamespacePrefix.Cut
    }
    
    NamespaceType{
        name: "Qt3DRender"
        ExtraIncludes{
            Include{
                fileName: "Qt3DRender/qrenderapi.h"
                location: Include.Global
            }
        }

        Rejection{
            functionName: "functorTypeId"
        }

        Rejection{
            functionName: "functor_cast"
        }

        Rejection{
            functionName: "operator=="
        }

        Rejection{
            functionName: "operator!="
        }

        Rejection{
            functionName: "swap"
        }

        EnumType{
            name: "API"
        }
    }
    
    Rejection{
        className: "QCameraPrivate"
    }

    Rejection{
        className: "Qt3DRender::Sphere"
    }

    Rejection{
        className: "Qt3DRender::QLevelOfDetailPrivate"
    }

    Rejection{
        className: "Qt3DRender::QAbstractRayCasterPrivate"
    }

    Rejection{
        className: "Qt3DRender::QAbstractRayCasterPrivate"
    }

    Rejection{
        className: "Qt3DRender::FunctorType"
    }

    Rejection{
        className: "Qt3DRender::QFrameGraphNodeCreatedChange"
    }

    Rejection{
        className: "Qt3DRender::QStencilTestSeparate"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderAspect"

        EnumType{
            name: "RenderType"
        }

        EnumType{
            name: "SubmissionType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QMemoryBarrier"

        EnumType{
            name: "Operation"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QScreenRayCaster"
        ModifyFunction{
            signature: "pick(QPoint)"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSetFence"
        EnumType{
            name: "HandleType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QWaitFence"
        EnumType{
            name: "HandleType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QLevelOfDetail"
        EnumType{
            name: "ThresholdType"
        }
        ModifyFunction{
            signature: "setCamera(Qt3DRender::QCamera*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcCamera"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QLevelOfDetailSwitch"
    }
    
    ObjectType{
        name: "Qt3DRender::QBlitFramebuffer"

        EnumType{
            name: "InterpolationMethod"
        }
        ModifyFunction{
            signature: "setDestination(Qt3DRender::QRenderTarget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setSource(Qt3DRender::QRenderTarget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QBufferCapture"
    }
    
    ObjectType{
        name: "Qt3DRender::QEnvironmentLight"
        ModifyFunction{
            signature: "setIrradiance(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setSpecular(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ValueType{
        name: "Qt3DRender::QLevelOfDetailBoundingSphere"
    }
    
    ObjectType{
        name: "Qt3DRender::QFrameGraphNodeCreatedChangeBase"
    }
    
    ObjectType{
        name: "Qt3DRender::QLineWidth"
    }
    
    ObjectType{
        name: "Qt3DRender::QForwardRenderer"
        ModifyFunction{
            signature: "setCamera(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QPhongMaterial"
    }
    
    ObjectType{
        name: "Qt3DRender::QMaterial"
        ModifyFunction{
            signature: "setEffect(Qt3DRender::QEffect*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::QAbstractMeshFunctor"
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QAbstractMeshFunctor &) const"
            rename: "equals"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.JavaInterface
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "return this==%1;"}
            }
        }
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "get"}
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractMesh"
    }
    
    NamespaceType{
        name: "Qt3DRender::Render"
        generate: false
    }
    
    ObjectType{
        name: "Qt3DRender::QGeometryRenderer"

        EnumType{
            name: "PrimitiveType"
        }
        ModifyFunction{
            signature: "setGeometry(Qt3DCore::QGeometry *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::QTextureDataFunctor"
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QTextureDataFunctor &) const"
            rename: "equals"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.JavaInterface
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "return this==%1;"}
            }
        }
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "get"}
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::QBufferFunctor"
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QBufferFunctor &) const"
            rename: "equals"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.JavaInterface
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "return this==%1;"}
            }
        }
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "get"}
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractTextureImage"
    }
    
    ObjectType{
        name: "Qt3DRender::QPaintedTextureImage"
        ModifyFunction{
            signature: "paint(QPainter*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderCapture"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderCaptureReply"
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractTexture"
        EnumType{
            name: "Status"
        }

        EnumType{
            name: "Target"
        }

        EnumType{
            name: "TextureFormat"
        }

        EnumType{
            name: "Filter"
        }

        EnumType{
            name: "CubeMapFace"
        }

        EnumType{
            name: "ComparisonFunction"
        }

        EnumType{
            name: "ComparisonMode"
        }

        EnumType{
            name: "HandleType"
        }
        ModifyFunction{
            signature: "addTextureImage(Qt3DRender::QAbstractTextureImage*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeTextureImage(Qt3DRender::QAbstractTextureImage*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWrapMode(Qt3DRender::QTextureWrapMode)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSharedGLTexture"
    }
    
    ObjectType{
        name: "Qt3DRender::QAlphaCoverage"
    }
    
    ObjectType{
        name: "Qt3DRender::QAlphaTest"
        EnumType{
            name: "AlphaFunc"
        }

        EnumType{
            name: "AlphaFunction"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAnnotation"
    }
    
    ObjectType{
        name: "Qt3DRender::QBlendEquation"
        EnumType{
            name: "BlendFunction"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QColorMask"
    }
    
    ObjectType{
        name: "Qt3DRender::QCuboidMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QCullFace"

        EnumType{
            name: "CullingMode"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCylinderMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthMask"
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthTest"

        EnumType{
            name: "DepthFunc"
        }

        EnumType{
            name: "DepthFunction"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDirectionalLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QDithering"
    }
    
    ObjectType{
        name: "Qt3DRender::QEffect"
        ModifyFunction{
            signature: "addParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addTechnique(Qt3DRender::QTechnique*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeTechnique(Qt3DRender::QTechnique*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractAttribute"
        ModifyFunction{
            signature: "buffer() const"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QFrontFace"

        EnumType{
            name: "WindingDirection"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractBuffer"
    }
    
    ObjectType{
        name: "Qt3DRender::QItemModelBuffer"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcModel"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QLayer"
    }
    
    ObjectType{
        name: "Qt3DRender::QMesh"
        EnumType{
            name: "Status"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QParameter"
    }
    
    ObjectType{
        name: "Qt3DRender::QPlaneMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QPointLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QPolygonOffset"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderPass"
        ModifyFunction{
            signature: "addFilterKey(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeFilterKey(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addRenderState(Qt3DRender::QRenderState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeRenderState(Qt3DRender::QRenderState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setShaderProgram(Qt3DRender::QShaderProgram*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderState"

        EnumType{
            name: "Type"
        }
        forceAbstract: true
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderTarget"
        ModifyFunction{
            signature: "addOutput(Qt3DRender::QRenderTargetOutput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeOutput(Qt3DRender::QRenderTargetOutput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSceneLoader"

        EnumType{
            name: "Status"
        }

        EnumType{
            name: "ComponentType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QScissorTest"
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderData"
        EnumType{
            name: "TransformType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractLight"
        EnumType{
            name: "Type"
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::PropertyReaderInterface"
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderProgram"

        EnumType{
            name: "Status"
        }
        EnumType{
            name: "ShaderType"
        }
        EnumType{
            name: "Format"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSphereMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QSpotLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilTest"
    }
    
    ObjectType{
        name: "Qt3DRender::QTechnique"

        Rejection{
            functionName: "graphicsApiFilter"
        }
        ModifyFunction{
            signature: "addRenderPass(Qt3DRender::QRenderPass*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeRenderPass(Qt3DRender::QRenderPass*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addFilterKey(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeFilterKey(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureImage"

        EnumType{
            name: "Status"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture1D"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture1DArray"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2D"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2DArray"
    }

    ObjectType{
        name: "Qt3DRender::QTexture3D"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureCubeMap"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureCubeMapArray"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2DMultisample"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2DMultisampleArray"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureRectangle"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureBuffer"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureLoader"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureData"
    }
    
    ObjectType{
        name: "Qt3DRender::QTorusMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureWrapMode"

        EnumType{
            name: "WrapMode"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Attribute"
    }
    
    ObjectType{
        name: "Qt3DRender::Buffer"
    }
    
    
    ObjectType{
        name: "Qt3DRender::TexImageData"
        ModifyFunction{
            signature: "setCompressedData(QByteArray, QOpenGLTexture::PixelFormat)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCameraSelector"
        ModifyFunction{
            signature: "setCamera(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QFrameGraph"
        ModifyFunction{
            signature: "setActiveFrameGraph(Qt3DRender::QFrameGraphNode*)"
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
        name: "Qt3DRender::QFrameGraphNode"
    }
    
    InterfaceType{
        name: "Qt3DRender::QFrameGraphSelectorFunctor"
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "call"}
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QFrameGraphSelector"
    }
    
    ObjectType{
        name: "Qt3DRender::QLayerFilter"

        EnumType{
            name: "FilterMode"
        }
        ModifyFunction{
            signature: "addLayer(Qt3DRender::QLayer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeLayer(Qt3DRender::QLayer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QNoDraw"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderPassFilter"
        ModifyFunction{
            signature: "addParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addMatch(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeMatch(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderTargetSelector"
        ModifyFunction{
            signature: "setTarget(Qt3DRender::QRenderTarget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSortMethod"
        ModifyFunction{
            signature: "addCriterion(Qt3DRender::QSortCriterion*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeCriterion(Qt3DRender::QSortCriterion*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QStateSet"
        ModifyFunction{
            signature: "addRenderState(Qt3DRender::QRenderState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeRenderState(Qt3DRender::QRenderState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QTechniqueFilter"
        ModifyFunction{
            signature: "addParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeParameter(Qt3DRender::QParameter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addMatch(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeMatch(Qt3DRender::QFilterKey*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QViewport"
    }
    
    ObjectType{
        name: "Qt3DRender::QWindow"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexImageData"
        ModifyFunction{
            signature: "operator=(const Qt3DRender::QTexImageData &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDispatchCompute"
    }
    
    ObjectType{
        name: "Qt3DRender::QFrustumCulling"
    }
    
    ObjectType{
        name: "Qt3DRender::QClearBuffers"
        ModifyFunction{
            signature: "setColorBuffer(Qt3DRender::QRenderTargetOutput *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcColorBuffer"
                    action: ReferenceCount.Set
                }
            }
        }

        EnumType{
            name: "BufferType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderStateSet"
        ModifyFunction{
            signature: "addRenderState(Qt3DRender::QRenderState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeRenderState(Qt3DRender::QRenderState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderSurfaceSelector"
        ModifyFunction{
            signature: "setSurface(QObject *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSurfaceObject"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSortPolicy"
        ModifyFunction{
            signature: "sortTypesChanged(const QVector<int> &)"
            rename: "sortTypesIntChanged"
        }
        ModifyFunction{
            signature: "setSortTypes(const QVector<int> &)"
            rename: "setSortTypesInt"
        }

        EnumType{
            name: "SortType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCamera"

        EnumType{
            name: "CameraTranslationOption"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCameraLens"

        EnumType{
            name: "ProjectionType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QComputeCommand"

        EnumType{
            name: "RunType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QPickingSettings"
        EnumType{
            name: "PickMethod"
        }
        EnumType{
            name: "PickResultMode"
        }
        EnumType{
            name: "FaceOrientationPickingMode"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderSettings"

        EnumType{
            name: "PickMethod"
        }

        EnumType{
            name: "PickResultMode"
        }

        EnumType{
            name: "FaceOrientationPickingMode"
        }

        EnumType{
            name: "RenderPolicy"
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
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderTargetOutput"

        EnumType{
            name: "AttachmentPoint"
        }
        ModifyFunction{
            signature: "setTexture(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::QBufferDataGenerator"
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QBufferDataGenerator &) const"
            rename: "equals"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.JavaInterface
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "return this==%1;"}
            }
        }
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "get"}
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::QGeometryFactory"
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QGeometryFactory &) const"
            rename: "equals"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.JavaInterface
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "return this==%1;"}
            }
        }
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "get"}
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::QTextureGenerator"
        ExtraIncludes{
            Include{
                fileName: "Qt3DRender/qabstractfunctor.h"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.Native
            Text{content: "QT_WARNING_DISABLE_DEPRECATED\nQT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")"}
        }
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QTextureGenerator &) const"
            rename: "equals"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.JavaInterface
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "return this==%1;"}
            }
        }
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "get"}
        }
    }
    
    InterfaceType{
        name: "Qt3DRender::QTextureImageDataGenerator"

        InjectCode{
            target: CodeClass.Native
            Text{content: "QT_WARNING_DISABLE_DEPRECATED\nQT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")"}
        }
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QTextureImageDataGenerator &) const"
            rename: "equals"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.JavaInterface
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "return this==%1;"}
            }
        }
        ModifyFunction{
            signature: "operator()()"
            Delegate{name: "get"}
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QFilterKey"
    }
    
    ObjectType{
        name: "Qt3DRender::QGraphicsApiFilter"

        EnumType{
            name: "OpenGLProfile"
        }

        EnumType{
            name: "Api"
            RejectEnumValue{
                name: "Vulkan"
            }
        }
    }
    GlobalFunction{
        signature: "operator==(const Qt3DRender::QGraphicsApiFilter &,const Qt3DRender::QGraphicsApiFilter &)"
        remove: RemoveFlag.All
    }
    
    ObjectType{
        name: "Qt3DRender::QObjectPicker"
    }
    
    ObjectType{
        name: "Qt3DRender::QPickEvent"

        EnumType{
            name: "Buttons"
        }

        EnumType{
            name: "Modifiers"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QPickLineEvent"
    }
    
    ObjectType{
        name: "Qt3DRender::QPickPointEvent"
    }
    
    ObjectType{
        name: "Qt3DRender::QProximityFilter"
        ModifyFunction{
            signature: "setEntity(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderProgramBuilder"
        ModifyFunction{
            signature: "setShaderProgram(Qt3DRender::QShaderProgram*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QPickTriangleEvent"
    }
    
    ObjectType{
        name: "Qt3DRender::QBlendEquationArguments"

        EnumType{
            name: "Blending"
            RejectEnumValue{
                name: "Source1Alpha"
            }
            RejectEnumValue{
                name: "Source1Color"
            }
            RejectEnumValue{
                name: "OneMinusSource1Color0"
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QClipPlane"
    }
    
    ObjectType{
        name: "Qt3DRender::QMultiSampleAntiAliasing"
    }
    
    ObjectType{
        name: "Qt3DRender::QNoDepthMask"
    }
    
    ObjectType{
        name: "Qt3DRender::QPointSize"

        EnumType{
            name: "SizeMode"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSeamlessCubemap"
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilMask"
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilOperation"
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilOperationArguments"

        EnumType{
            name: "FaceMode"
        }

        EnumType{
            name: "Operation"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilTestArguments"
        EnumType{
            name: "StencilFaceMode"
        }
        EnumType{
            name: "StencilFunction"
        }
    }
    
    ValueType{
        name: "Qt3DRender::QTextureImageData"

        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }

        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "QT_WARNING_DISABLE_CLANG(\"-Wdeprecated-copy\")\n"+
                          "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-copy\")"}
        }

        FunctionalType{
            name: "DataConverter"
            generate: false
            using: "std::function<QByteArray(QByteArray,int,int,int)>"
        }

        ModifyFunction{
            signature: "setData(const QByteArray &, std::function<QByteArray(QByteArray, int, int, int)>, bool)"
            ModifyArgument{
                index: 2
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
std::function<QByteArray(const QByteArray &, int, int, int)> %out;
if(%in){
    %out = [wrapper = JObjectWrapper(%env, %in)](QByteArray rawData, int layer, int face, int mipmapLevel) -> QByteArray {
        if(JniEnvironmentExceptionHandler env{200}) {
            QTJAMBI_TRY {
                jobject _rawData = qtjambi_cast<jobject>(env, std::move(rawData));
                jobject result = Java::Qt3DRender::QTextureImageData$DataConverter::apply(env, wrapper.object(env), _rawData, layer, face, mipmapLevel);
                return qtjambi_cast<QByteArray>(env, result);
            } QTJAMBI_CATCH(const JavaException& exn){
                env.handleException(exn, "std::function<QByteArray(QByteArray,int,int,int)>");
            } QTJAMBI_TRY_END
        }
        return {};
    };
}`
                    }
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            Text{content: String.raw`
/**
 * <p>Java wrapper for Qt callable <code>std::function&lt;QByteArray(QByteArray,int,int,int)&gt;</code></p>
 */
@FunctionalInterface
public interface DataConverter{
    public io.qt.core.@NonNull QByteArray apply(io.qt.core.@NonNull QByteArray rawData, int layer, int face, int mipmapLevel);
}
                `}
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
namespace Java{
namespace Qt3DRender{
QTJAMBI_REPOSITORY_DECLARE_CLASS(QTextureImageData$DataConverter,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qt3d/render,QTextureImageData$DataConverter,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(Lio/qt/core/QByteArray;III)Lio/qt/core/QByteArray;)
                                )
}
}
                `}
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractRayCaster"

        EnumType{
            name: "RunMode"
        }

        EnumType{
            name: "FilterMode"
        }
        ModifyFunction{
            signature: "addLayer(Qt3DRender::QLayer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeLayer(Qt3DRender::QLayer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRayCaster"
    }
    
    ValueType{
        name: "Qt3DRender::QRayCasterHit"

        EnumType{
            name: "HitType"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthRange"
    }
    
    ObjectType{
        name: "Qt3DRender::QNoPicking"
    }
    
    ObjectType{
        name: "Qt3DRender::QRasterMode"

        EnumType{
            name: "FaceMode"
        }

        EnumType{
            name: "RasterMode"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderImage"

        EnumType{
            name: "Access"
        }

        EnumType{
            name: "ImageFormat"
        }
        ModifyFunction{
            signature: "setTexture(Qt3DRender::QAbstractTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSubtreeEnabler"

        EnumType{
            name: "Enablement"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderCapabilities"

        EnumType{
            name: "API"
        }

        EnumType{
            name: "Profile"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDebugOverlay"
    }
    
    ValueType{
        name: "Qt3DRender::QTextureDataUpdate"
        ModifyFunction{
            signature: "setData(const QSharedPointer<Qt3DRender::QTextureImageData> &)"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSharedPointer<Qt3DRender::QTextureImageData> %out = qtjambi_cast<QSharedPointer<Qt3DRender::QTextureImageData>>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QPickingProxy"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'Qt3DRender::QGraphicsApiFilter*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'Qt3DRender::QStencilTestSeparate*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'Qt3DRender::QBoundingVolumeSpecifier*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'Qt3DRender::QGraphicsApiFilter*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'QLoggingCategory*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*::doClone', unmatched return type 'QNode*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Unable to decide type of property: 'Qt3DRender::QStencilTestSeparate*' in class 'QStencilTest'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Unable to decide type of property: 'Qt3DRender::QGraphicsApiFilter*' in class 'QTechnique'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Unable to decide type of property: 'Qt3DRender::QBoundingVolumeSpecifier*' in class 'QGeometry'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QRenderState' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unknown operator 'quint64'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'createNodeCreationChange() const' in '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'dataGenerator() const' in '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'dependencies() const' in '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Rejected enum has no alternative...: QGraphicsApiFilter::Vulkan"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractLight' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'sceneChangeEvent(const QSharedPointer<Qt3DCore::QSceneChange > & change)' in 'QCameraLens'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DRender::QRayCasterHit."}
    SuppressedWarning{text: "WARNING(Lexer) :: Invalid input: 239 (in *qrendercapabilities.h, *)"}
    SuppressedWarning{text: "WARNING(Lexer) :: Invalid input: 187 (in *qrendercapabilities.h, *)"}
    SuppressedWarning{text: "WARNING(Lexer) :: Invalid input: 191 (in *qrendercapabilities.h, *)"}
}
