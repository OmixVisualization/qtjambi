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
        generate: false
        until: [5, 14]
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
        since: [5, 15]
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
            since: 6
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
            since: 6
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
        ModifyFunction{
            signature: "operator=(const Qt3DRender::QLevelOfDetailBoundingSphere &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QFrameGraphNodeCreatedChangeBase"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QLineWidth"
    }
    
    ObjectType{
        name: "Qt3DRender::QDiffuseMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QDiffuseSpecularMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSpecular(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSpecular"
                    action: ReferenceCount.Set
                }
            }
        }
        until: [5, 6]
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
        name: "Qt3DRender::QNormalDiffuseMapAlphaMaterial"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QNormalDiffuseMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setNormal(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNormal"
                    action: ReferenceCount.Set
                }
            }
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QNormalDiffuseSpecularMapMaterial"
        ModifyFunction{
            signature: "setDiffuse(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDiffuse"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSpecular(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSpecular"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setNormal(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNormal"
                    action: ReferenceCount.Set
                }
            }
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QPhongMaterial"
        until: [5, 6]
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
        ModifyFunction{
            signature: "setTextureParameter(QString,Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcTextureParameters"
                    keyArgument: 1
                    action: ReferenceCount.Put
                }
            }
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QSkyboxEntity"
        until: [5, 6]
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
            signature: "setGeometry(Qt3DRender::QGeometry *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 5
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
            since: 6
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QGeometry"

        Rejection{
            functionName: "boundingVolumeSpecifier"
        }
        ModifyFunction{
            signature: "addAttribute(Qt3DRender::QAttribute *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "removeAttribute(Qt3DRender::QAttribute *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setBoundingVolumePositionAttribute(Qt3DRender::QAttribute *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcBoundingVolumePositionAttribute"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "addAttribute(Qt3DCore::QAttribute *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "removeAttribute(Qt3DCore::QAttribute *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setBoundingVolumePositionAttribute(Qt3DCore::QAttribute *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcBoundingVolumePositionAttribute"
                    action: ReferenceCount.Set
                }
            }
            since: 6
        }
        until: 5
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractSceneLoader"
        EnumType{
            name: "Status"
        }
        until: [5, 6]
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
        name: "Qt3DRender::QGeometryFunctor"
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QGeometryFunctor &) const"
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
        until: 5
    }
    
    InterfaceType{
        name: "Qt3DRender::QAbstractFunctor"

        Rejection{
            functionName: "functor_cast"
            since: [5, 15]
        }
        ModifyFunction{
            signature: "id()const"
            remove: RemoveFlag.JavaAndNative
            InjectCode{
                target: CodeClass.Shell
                Text{content: "if(JniEnvironment %env{200}) {\n"+
                              "    if(jobject %this = __shell()->getJavaObjectLocalRef(%env)){\n"+
                              "        return qintptr(QtJambiAPI::getJavaObjectHashCode(%env, %env->GetObjectClass(%this)));\n"+
                              "    }else{\n"+
                              "        __shell()->warnForMethod(\"Qt3DRender::QAbstractFunctor::id() const\");\n"+
                              "    }\n"+
                              "}"}
            }
        }
        until: 5
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
        name: "Qt3DRender::QAbstractTextureProvider"
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
            until: [5, 6]
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
        until: [5, 6]
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
            since: [5, 12]
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
        since: [5, 13]
    }
    
    ObjectType{
        name: "Qt3DRender::QAlphaCoverage"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAlphaTest"
        EnumType{
            name: "AlphaFunc"
        }

        EnumType{
            name: "AlphaFunction"
            since: [5, 7]
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAnnotation"
    }
    
    ObjectType{
        name: "Qt3DRender::QBlendEquation"
        EnumType{
            name: "BlendMode"
            until: [5, 6]
        }

        EnumType{
            name: "BlendFunction"
            since: [5, 7]
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QBlendState"

        EnumType{
            name: "Blending"
            RejectEnumValue{
                name: "OneMinusSrcAlpha"
            }
            RejectEnumValue{
                name: "DstAlpha"
            }
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QColorMask"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
        ModifyFunction{
            signature: "copy(const Qt3DCore::QNode *)"
            access: Modification.DeclFinal
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCuboidMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QCullFace"

        EnumType{
            name: "CullingMode"
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCylinderMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthMask"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthTest"

        EnumType{
            name: "DepthFunc"
        }

        EnumType{
            name: "DepthFunction"
            since: [5, 7]
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDirectionalLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QDithering"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
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
        name: "Qt3DRender::QAttribute"
        ModifyFunction{
            signature: "setBuffer(Qt3DRender::QBuffer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }

        EnumType{
            name: "AttributeType"
        }

        EnumType{
            name: "VertexBaseType"
        }
        until: 5
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
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }

        EnumType{
            name: "FaceDir"
            until: [5, 6]
        }

        EnumType{
            name: "WindingDirection"
            since: [5, 7]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractBuffer"
    }
    
    ObjectType{
        name: "Qt3DRender::QBuffer"
        ModifyFunction{
            signature: "QBuffer(Qt3DRender::QBuffer::BufferType, Qt3DCore::QNode *)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "BufferType.VertexBuffer"
                }
            }
        }

        EnumType{
            name: "BufferType"
        }

        EnumType{
            name: "UsageType"
        }

        EnumType{
            name: "AccessType"
            since: [5, 9]
        }
        until: 5
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
            since: [5, 11]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QParameter"
    }
    
    ObjectType{
        name: "Qt3DRender::QParameterMapping"

        EnumType{
            name: "Binding"
        }
        until: [5, 9]
    }
    
    ObjectType{
        name: "Qt3DRender::QPlaneMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QPointLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QPolygonOffset"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderAttachment"

        EnumType{
            name: "RenderAttachmentType"
        }

        EnumType{
            name: "CubeMapFace"
        }
        ModifyFunction{
            signature: "setTexture(Qt3DRender::QAbstractTextureProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderPass"
        ModifyFunction{
            signature: "glslNameForParameter(QString) const"
            remove: RemoveFlag.All
            until: [5, 6]
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
            signature: "addAnnotation(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "removeAnnotation(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "addBinding(Qt3DRender::QParameterMapping*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "removeBinding(Qt3DRender::QParameterMapping*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
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
            signature: "addAttachment(Qt3DRender::QRenderAttachment*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "removeAttachment(Qt3DRender::QRenderAttachment*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
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
            since: [5, 9]
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QScissorTest"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderData"

        EnumType{
            name: "TransformType"
        }
        ModifyFunction{
            signature: "event(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractLight"
        EnumType{
            name: "Type"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QLight"
        EnumType{
            name: "Type"
        }
        until: [5, 6]
    }
    
    InterfaceType{
        name: "Qt3DRender::PropertyReaderInterface"
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderProgram"

        EnumType{
            name: "Status"
            since: [5, 9]
        }
        EnumType{
            name: "ShaderType"
        }
        EnumType{
            name: "Format"
            since: [5, 15]
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

        Rejection{
            functionName: "front"
            until: [5, 6]
        }

        Rejection{
            functionName: "back"
            until: [5, 6]
        }

        EnumType{
            name: "StencilFaceMode"
            until: [5, 6]
        }

        EnumType{
            name: "StencilFunc"
            until: [5, 6]
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QTechnique"

        Rejection{
            functionName: "graphicsApiFilter"
        }
        ModifyFunction{
            signature: "addPass(Qt3DRender::QRenderPass*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "removePass(Qt3DRender::QRenderPass*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "addRenderPass(Qt3DRender::QRenderPass*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        ModifyFunction{
            signature: "removeRenderPass(Qt3DRender::QRenderPass*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
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
            signature: "addAnnotation(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "removeAnnotation(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
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
            since: [5, 7]
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
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
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureData"
        since: [5, 7]
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
        name: "Qt3DRender::QClearBuffer"

        EnumType{
            name: "BufferType"
        }
        until: [5, 6]
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
        ModifyFunction{
            signature: "isEnabled() const"
            rename: "isFrameGraphEnabled"
            until: [5, 6]
        }
        ModifyFunction{
            signature: "setEnabled(bool)"
            rename: "setFrameGraphEnabled"
            until: [5, 6]
        }
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
            since: [5, 5]
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
        since: [5, 5]
    }
    
    ObjectType{
        name: "Qt3DRender::QNoDraw"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderPassFilter"
        ModifyFunction{
            signature: "addInclude(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "removeInclude(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
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
        name: "Qt3DRender::QSortCriterion"

        EnumType{
            name: "SortType"
        }
        until: [5, 6]
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
            signature: "addRequirement(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "removeRequirement(Qt3DRender::QAnnotation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: [5, 6]
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
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QFrustumCulling"
        since: [5, 7]
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
        since: [5, 7]
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
        since: [5, 7]
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
        since: [5, 7]
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
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QCamera"

        EnumType{
            name: "CameraTranslationOption"
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QCameraLens"

        EnumType{
            name: "ProjectionType"
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QComputeCommand"

        EnumType{
            name: "RunType"
            since: [5, 12]
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickingSettings"
        since: [5, 7]
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
            since: [5, 8]
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
        since: [5, 7]
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
        since: [5, 7]
    }
    
    InterfaceType{
        name: "Qt3DRender::QBufferDataGenerator"
        ExtraIncludes{
            Include{
                fileName: "Qt3DRender/qabstractfunctor.h"
                location: Include.Global
                until: 5
            }
        }
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
        since: [5, 7]
    }
    
    InterfaceType{
        name: "Qt3DRender::QGeometryFactory"
        ExtraIncludes{
            Include{
                fileName: "Qt3DRender/qabstractfunctor.h"
                location: Include.Global
                until: 5
            }
        }
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
        since: [5, 7]
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
        since: [5, 7]
    }
    
    InterfaceType{
        name: "Qt3DRender::QTextureImageDataGenerator"
        ExtraIncludes{
            Include{
                fileName: "Qt3DRender/qabstractfunctor.h"
                location: Include.Global
                until: 5
            }
        }
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
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QFilterKey"
        since: [5, 7]
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
        since: [5, 7]
    }
    GlobalFunction{
        signature: "operator==(const Qt3DRender::QGraphicsApiFilter &,const Qt3DRender::QGraphicsApiFilter &)"
        remove: RemoveFlag.All
    }
    
    ObjectType{
        name: "Qt3DRender::QObjectPicker"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickEvent"

        EnumType{
            name: "Buttons"
            since: [5, 8]
        }

        EnumType{
            name: "Modifiers"
            since: [5, 8]
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickLineEvent"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickPointEvent"
        since: [5, 10]
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
        since: [5, 10]
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
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickTriangleEvent"
        since: [5, 7]
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
                since: [5, 8]
            }
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QClipPlane"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QMultiSampleAntiAliasing"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QNoDepthMask"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPointSize"

        EnumType{
            name: "SizeMode"
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QSeamlessCubemap"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilMask"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilOperation"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilOperationArguments"

        EnumType{
            name: "FaceMode"
        }

        EnumType{
            name: "Operation"
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilTestArguments"
        EnumType{
            name: "StencilFaceMode"
        }
        EnumType{
            name: "StencilFunction"
        }
        since: [5, 7]
    }
    
    ValueType{
        name: "Qt3DRender::QTextureImageData"
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "QT_WARNING_DISABLE_CLANG(\"-Wdeprecated-copy\")\n"+
                          "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-copy\")"}
        }

        FunctionalType{
            name: "DataConverter"
            using: "std::function<QByteArray(QByteArray,int,int,int)>"
            since: 6
        }
        since: [5, 7]
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
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QNoPicking"
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QRasterMode"

        EnumType{
            name: "FaceMode"
        }

        EnumType{
            name: "RasterMode"
        }
        since: [5, 14]
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
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QSubtreeEnabler"

        EnumType{
            name: "Enablement"
        }
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderCapabilities"

        EnumType{
            name: "API"
        }

        EnumType{
            name: "Profile"
        }
        since: [5, 15]
    }
    
    ObjectType{
        name: "Qt3DRender::QDebugOverlay"
        since: [5, 15]
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
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickingProxy"
        since: 6
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
