/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    defaultSuperClass: "io.qt.QtObject"
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
        since: [5, 15]
    }
    
    TypeAliasType{
        name: "Qt3DRender::QAbstractRayCaster::Hits"
    }
    
    Rejection{
        className: "QCameraPrivate"
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
        className: "Qt3DRender"
        functionName: "functorTypeId"
        since: [5, 15]
    }
    
    Rejection{
        className: "Qt3DRender"
        functionName: "functor_cast"
        since: [5, 15]
    }
    
    Rejection{
        className: "Qt3DRender::QAbstractFunctor"
        functionName: "functor_cast"
        since: [5, 15]
    }
    
    Rejection{
        className: "Qt3DRender"
        functionName: "operator=="
        since: [5, 15]
    }
    
    Rejection{
        className: "Qt3DRender"
        functionName: "operator!="
        since: [5, 15]
    }
    
    Rejection{
        className: "Qt3DRender"
        functionName: "swap"
        since: [5, 15]
    }
    
    EnumType{
        name: "Qt3DRender::API"
        since: [5, 15]
    }
    
    Rejection{
        className: "Qt3DRender::FunctorType"
    }
    
    Rejection{
        className: "Qt3DRender::QFrameGraphNodeCreatedChange"
    }
    
    Rejection{
        className: "Qt3DRender::QStencilTest"
        functionName: "front"
        until: [5, 6]
    }
    
    Rejection{
        className: "Qt3DRender::QStencilTest"
        functionName: "back"
        until: [5, 6]
    }
    
    Rejection{
        className: "Qt3DRender::QStencilTestSeparate"
    }
    
    Rejection{
        className: "Qt3DRender::QTechnique"
        functionName: "graphicsApiFilter"
    }
    
    Rejection{
        className: "Qt3DRender::QGeometry"
        functionName: "boundingVolumeSpecifier"
    }
    
    EnumType{
        name: "Qt3DRender::QRenderAspect::RenderType"
    }
    
    EnumType{
        name: "Qt3DRender::QRenderAspect::SubmissionType"
        since: 6
    }
    
    EnumType{
        name: "Qt3DRender::QShaderProgram::Format"
        since: [5, 15]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderAspect"
        javaName: "QRenderAspect"
    }
    
    EnumType{
        name: "Qt3DRender::QMemoryBarrier::Operation"
        flags: "Qt3DRender::QMemoryBarrier::Operations"
    }
    
    ObjectType{
        name: "Qt3DRender::QMemoryBarrier"
        javaName: "QMemoryBarrier"
    }
    
    ObjectType{
        name: "Qt3DRender::QScreenRayCaster"
        javaName: "QScreenRayCaster"
        ModifyFunction{
            signature: "pick(QPoint)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.QList<io.qt.qt3d.render.QRayCasterHit>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<Qt3DRender::QAbstractRayCaster::Hits*>(%env, %in);"}
                }
            }
            since: 6
        }
    }
    
    EnumType{
        name: "Qt3DRender::QSetFence::HandleType"
    }
    
    ObjectType{
        name: "Qt3DRender::QSetFence"
        javaName: "QSetFence"
    }
    
    EnumType{
        name: "Qt3DRender::QWaitFence::HandleType"
    }
    
    ObjectType{
        name: "Qt3DRender::QWaitFence"
        javaName: "QWaitFence"
    }
    
    EnumType{
        name: "Qt3DRender::QLevelOfDetail::ThresholdType"
    }
    
    ObjectType{
        name: "Qt3DRender::QLevelOfDetail"
        javaName: "QLevelOfDetail"
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
        javaName: "QLevelOfDetailSwitch"
    }
    
    EnumType{
        name: "Qt3DRender::QBlitFramebuffer::InterpolationMethod"
    }
    
    ObjectType{
        name: "Qt3DRender::QBlitFramebuffer"
        javaName: "QBlitFramebuffer"
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
        javaName: "QBufferCapture"
    }
    
    ObjectType{
        name: "Qt3DRender::QEnvironmentLight"
        javaName: "QEnvironmentLight"
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
        javaName: "QLevelOfDetailBoundingSphere"
        ModifyFunction{
            signature: "operator=(const Qt3DRender::QLevelOfDetailBoundingSphere &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QFrameGraphNodeCreatedChangeBase"
        javaName: "QFrameGraphNodeCreatedChange"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QLineWidth"
        javaName: "QLineWidth"
    }
    
    ObjectType{
        name: "Qt3DRender::QDiffuseMapMaterial"
        javaName: "QDiffuseMapMaterial"
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
        javaName: "QDiffuseSpecularMapMaterial"
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
    }
    
    ObjectType{
        name: "Qt3DRender::QNormalDiffuseMapAlphaMaterial"
        javaName: "QNormalDiffuseMapAlphaMaterial"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QNormalDiffuseMapMaterial"
        javaName: "QNormalDiffuseMapMaterial"
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
        javaName: "QNormalDiffuseSpecularMapMaterial"
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
        javaName: "QPhongMaterial"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QMaterial"
        javaName: "QMaterial"
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
        javaName: "QSkyboxEntity"
        until: [5, 6]
    }
    
    InterfaceType{
        name: "Qt3DRender::QAbstractMeshFunctor"
        javaName: "QAbstractMeshFunctor"
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
            rename: "get"
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
        javaName: "QAbstractMesh"
    }
    
    NamespaceType{
        name: "Qt3DRender::Render"
        javaName: "Qt3DRenderer"
        generate: false
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractSceneLoader::Status"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QGeometryRenderer::PrimitiveType"
    }
    
    ObjectType{
        name: "Qt3DRender::QGeometryRenderer"
        javaName: "QGeometryRenderer"
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
        javaName: "QGeometry"
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
        javaName: "QAbstractSceneLoader"
        until: [5, 6]
    }
    
    InterfaceType{
        name: "Qt3DRender::QTextureDataFunctor"
        javaName: "QTextureDataFunctor"
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
            rename: "get"
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
        javaName: "QGeometryFunctor"
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
            rename: "get"
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
        javaName: "QAbstractFunctor"
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
        javaName: "QBufferFunctor"
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
            rename: "get"
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
        javaName: "QAbstractTextureImage"
    }
    
    ObjectType{
        name: "Qt3DRender::QPaintedTextureImage"
        javaName: "QPaintedTextureImage"
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
        javaName: "QRenderCapture"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderCaptureReply"
        javaName: "QRenderCaptureReply"
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTextureProvider::Status"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTextureProvider::Target"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTextureProvider::TextureFormat"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTextureProvider::Filter"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTextureProvider::CubeMapFace"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTextureProvider::ComparisonFunction"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTextureProvider::ComparisonMode"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractTextureProvider"
        javaName: "QAbstractTextureProvider"
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
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::Status"
        since: []
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::Target"
        since: []
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::TextureFormat"
        since: []
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::Filter"
        since: []
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::CubeMapFace"
        since: []
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::ComparisonFunction"
        since: []
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::ComparisonMode"
        since: []
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractTexture::HandleType"
        since: [5, 12]
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractTexture"
        javaName: "QAbstractTexture"
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
        since: []
    }
    
    ObjectType{
        name: "Qt3DRender::QSharedGLTexture"
        javaName: "QSharedGLTexture"
        since: [5, 13]
    }
    
    ObjectType{
        name: "Qt3DRender::QAlphaCoverage"
        javaName: "QAlphaCoverage"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    EnumType{
        name: "Qt3DRender::QAlphaTest::AlphaFunc"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QAlphaTest::AlphaFunction"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QAlphaTest"
        javaName: "QAlphaTest"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAnnotation"
        javaName: "QAnnotation"
    }
    
    EnumType{
        name: "Qt3DRender::QBlendEquation::BlendMode"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QBlendEquation::BlendFunction"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QBlendEquation"
        javaName: "QBlendEquation"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    EnumType{
        name: "Qt3DRender::QBlendState::Blending"
        RejectEnumValue{
            name: "OneMinusSrcAlpha"
        }
        RejectEnumValue{
            name: "DstAlpha"
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QBlendState"
        javaName: "QBlendState"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QColorMask"
        javaName: "QColorMask"
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
        javaName: "QCuboidMesh"
    }
    
    EnumType{
        name: "Qt3DRender::QCullFace::CullingMode"
    }
    
    ObjectType{
        name: "Qt3DRender::QCullFace"
        javaName: "QCullFace"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCylinderMesh"
        javaName: "QCylinderMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthMask"
        javaName: "QDepthMask"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    EnumType{
        name: "Qt3DRender::QDepthTest::DepthFunc"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QDepthTest::DepthFunction"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthTest"
        javaName: "QDepthTest"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDirectionalLight"
        javaName: "QDirectionalLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QDithering"
        javaName: "QDithering"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QEffect"
        javaName: "QEffect"
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
        javaName: "QAttribute"
        ModifyFunction{
            signature: "setBuffer(Qt3DRender::QBuffer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        until: 5
    }
    
    EnumType{
        name: "Qt3DRender::QAttribute::AttributeType"
        until: 5
    }
    
    EnumType{
        name: "Qt3DRender::QAttribute::VertexBaseType"
        until: 5
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractAttribute"
        javaName: "QAbstractAttribute"
        ModifyFunction{
            signature: "buffer() const"
            access: Modification.NonFinal
        }
    }
    
    EnumType{
        name: "Qt3DRender::QFrontFace::FaceDir"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QFrontFace::WindingDirection"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QFrontFace"
        javaName: "QFrontFace"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractBuffer"
        javaName: "QAbstractBuffer"
    }
    
    ObjectType{
        name: "Qt3DRender::QBuffer"
        javaName: "QBuffer"
        ModifyFunction{
            signature: "QBuffer(Qt3DRender::QBuffer::BufferType, Qt3DCore::QNode *)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "BufferType.VertexBuffer"
                }
            }
        }
        until: 5
    }
    
    EnumType{
        name: "Qt3DRender::QBuffer::BufferType"
        until: 5
    }
    
    EnumType{
        name: "Qt3DRender::QBuffer::UsageType"
        until: 5
    }
    
    EnumType{
        name: "Qt3DRender::QBuffer::AccessType"
        since: [5, 9]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DRender::QItemModelBuffer"
        javaName: "QItemModelBuffer"
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
        javaName: "QLayer"
    }
    
    EnumType{
        name: "Qt3DRender::QMesh::Status"
        since: [5, 11]
    }
    
    ObjectType{
        name: "Qt3DRender::QMesh"
        javaName: "QMesh"
    }
    
    
    ObjectType{
        name: "Qt3DRender::QParameter"
        javaName: "QParameter"
    }
    
    EnumType{
        name: "Qt3DRender::QParameterMapping::Binding"
        until: [5, 9]
    }
    
    ObjectType{
        name: "Qt3DRender::QParameterMapping"
        javaName: "QParameterMapping"
        until: [5, 9]
    }
    
    ObjectType{
        name: "Qt3DRender::QPlaneMesh"
        javaName: "QPlaneMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QPointLight"
        javaName: "QPointLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QPolygonOffset"
        javaName: "QPolygonOffset"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    EnumType{
        name: "Qt3DRender::QRenderAttachment::RenderAttachmentType"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QRenderAttachment::CubeMapFace"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderAttachment"
        javaName: "QRenderAttachment"
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
        javaName: "QRenderPass"
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
    
    EnumType{
        name: "Qt3DRender::QRenderState::Type"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderState"
        javaName: "QRenderState"
        forceAbstract: true
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderTarget"
        javaName: "QRenderTarget"
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
    
    EnumType{
        name: "Qt3DRender::QSceneLoader::Status"
    }
    
    EnumType{
        name: "Qt3DRender::QSceneLoader::ComponentType"
        since: [5, 9]
    }
    
    ObjectType{
        name: "Qt3DRender::QSceneLoader"
        javaName: "QSceneLoader"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QScissorTest"
        javaName: "QScissorTest"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    EnumType{
        name: "Qt3DRender::QShaderData::TransformType"
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderData"
        javaName: "QShaderData"
        ModifyFunction{
            signature: "event(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractLight::Type"
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractLight"
        javaName: "QAbstractLight"
    }
    
    ObjectType{
        name: "Qt3DRender::QLight"
        javaName: "QLight"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QLight::Type"
        until: [5, 6]
    }
    
    InterfaceType{
        name: "Qt3DRender::PropertyReaderInterface"
        javaName: "PropertyReaderInterface"
    }
    
    EnumType{
        name: "Qt3DRender::QShaderProgram::Status"
        since: [5, 9]
    }
    
    EnumType{
        name: "Qt3DRender::QShaderProgram::ShaderType"
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderProgram"
        javaName: "QShaderProgram"
    }
    
    ObjectType{
        name: "Qt3DRender::QSphereMesh"
        javaName: "QSphereMesh"
    }
    
    ObjectType{
        name: "Qt3DRender::QSpotLight"
        javaName: "QSpotLight"
    }
    
    EnumType{
        name: "Qt3DRender::QStencilTest::StencilFaceMode"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DRender::QStencilTest::StencilFunc"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilTest"
        javaName: "QStencilTest"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QTechnique"
        javaName: "QTechnique"
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
        javaName: "QTextureImage"
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
            until: [5, 6]
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture1D"
        javaName: "QTexture1D"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture1DArray"
        javaName: "QTexture1DArray"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2D"
        javaName: "QTexture2D"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2DArray"
        javaName: "QTexture2DArray"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture3D"
        javaName: "QTexture3D"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureCubeMap"
        javaName: "QTextureCubeMap"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureCubeMapArray"
        javaName: "QTextureCubeMapArray"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2DMultisample"
        javaName: "QTexture2DMultisample"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexture2DMultisampleArray"
        javaName: "QTexture2DMultisampleArray"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureRectangle"
        javaName: "QTextureRectangle"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureBuffer"
        javaName: "QTextureBuffer"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureLoader"
        javaName: "QTextureLoader"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureData"
        javaName: "QTextureData"
        ModifyFunction{
            signature: "addImageData(const QSharedPointer<Qt3DRender::QTextureImageData> &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.qt3d.render.QTextureImageData"
                }
            }
        }
        ModifyFunction{
            signature: "imageData() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "java.util.List<QTextureImageData>"
                }
            }
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QTorusMesh"
        javaName: "QTorusMesh"
    }
    
    EnumType{
        name: "Qt3DRender::QTextureWrapMode::WrapMode"
    }
    
    ObjectType{
        name: "Qt3DRender::QTextureWrapMode"
        javaName: "QTextureWrapMode"
    }
    
    
    Rejection{
        className: "Qt3DRender::Sphere"
    }
    
    ObjectType{
        name: "Qt3DRender::Attribute"
        javaName: "Attribute"
    }
    
    ObjectType{
        name: "Qt3DRender::Buffer"
        javaName: "Buffer"
    }
    
    
    ObjectType{
        name: "Qt3DRender::TexImageData"
        javaName: "TexImageData"
        ModifyFunction{
            signature: "setCompressedData(QByteArray, QOpenGLTexture::PixelFormat)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QCameraSelector"
        javaName: "QCameraSelector"
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
    
    EnumType{
        name: "Qt3DRender::QClearBuffer::BufferType"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QClearBuffer"
        javaName: "QClearBuffer"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QFrameGraph"
        javaName: "QFrameGraph"
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
        javaName: "QFrameGraphNode"
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
        javaName: "QFrameGraphSelectorFunctor"
        ModifyFunction{
            signature: "operator()()"
            rename: "call"
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QFrameGraphSelector"
        javaName: "QFrameGraphSelector"
    }
    
    EnumType{
        name: "Qt3DRender::QLayerFilter::FilterMode"
        since: [5, 5]
    }
    
    ObjectType{
        name: "Qt3DRender::QLayerFilter"
        javaName: "QLayerFilter"
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
        javaName: "QNoDraw"
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderPassFilter"
        javaName: "QRenderPassFilter"
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
        javaName: "QRenderTargetSelector"
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
    
    EnumType{
        name: "Qt3DRender::QSortCriterion::SortType"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QSortCriterion"
        javaName: "QSortCriterion"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DRender::QSortMethod"
        javaName: "QSortMethod"
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
        javaName: "QStateSet"
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
        javaName: "QTechniqueFilter"
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
        javaName: "QViewport"
    }
    
    ObjectType{
        name: "Qt3DRender::QWindow"
        javaName: "QWindow"
    }
    
    ObjectType{
        name: "Qt3DRender::QTexImageData"
        javaName: "QTexImageData"
        ModifyFunction{
            signature: "operator=(const Qt3DRender::QTexImageData &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDispatchCompute"
        javaName: "QDispatchCompute"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QFrustumCulling"
        javaName: "QFrustumCulling"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QClearBuffers"
        javaName: "QClearBuffers"
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
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QClearBuffers::BufferType"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderStateSet"
        javaName: "QRenderStateSet"
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
        javaName: "QRenderSurfaceSelector"
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
        javaName: "QSortPolicy"
        ModifyFunction{
            signature: "sortTypesChanged(const QVector<int> &)"
            rename: "sortTypesIntChanged"
        }
        ModifyFunction{
            signature: "setSortTypes(const QVector<int> &)"
            rename: "setSortTypesInt"
        }
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QSortPolicy::SortType"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QCamera::CameraTranslationOption"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QCamera"
        javaName: "QCamera"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QCameraLens"
        javaName: "QCameraLens"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QCameraLens::ProjectionType"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QComputeCommand::RunType"
        since: [5, 12]
    }
    
    ObjectType{
        name: "Qt3DRender::QComputeCommand"
        javaName: "QComputeCommand"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickingSettings"
        javaName: "QPickingSettings"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QPickingSettings::PickMethod"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QPickingSettings::PickResultMode"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QPickingSettings::FaceOrientationPickingMode"
        since: [5, 8]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderSettings"
        javaName: "QRenderSettings"
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
    
    EnumType{
        name: "Qt3DRender::QRenderSettings::RenderPolicy"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderTargetOutput"
        javaName: "QRenderTargetOutput"
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
    
    EnumType{
        name: "Qt3DRender::QRenderTargetOutput::AttachmentPoint"
        since: [5, 7]
    }
    
    InterfaceType{
        name: "Qt3DRender::QBufferDataGenerator"
        javaName: "QBufferDataGenerator"
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
            rename: "get"
        }
        since: [5, 7]
    }
    
    InterfaceType{
        name: "Qt3DRender::QGeometryFactory"
        javaName: "QGeometryFactory"
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
            rename: "get"
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
        javaName: "QTextureGenerator"
        ExtraIncludes{
            Include{
                fileName: "Qt3DRender/qabstractfunctor.h"
                location: Include.Global
            }
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
            rename: "get"
        }
        since: [5, 7]
    }
    
    InterfaceType{
        name: "Qt3DRender::QTextureImageDataGenerator"
        javaName: "QTextureImageDataGenerator"
        ExtraIncludes{
            Include{
                fileName: "Qt3DRender/qabstractfunctor.h"
                location: Include.Global
                until: 5
            }
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
            rename: "get"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.qt3d.render.QTextureImageData"
                }
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
        javaName: "QFilterKey"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QGraphicsApiFilter"
        javaName: "QGraphicsApiFilter"
        ModifyFunction{
            signature: "operator==(const Qt3DRender::QGraphicsApiFilter &,const Qt3DRender::QGraphicsApiFilter &)"
            remove: RemoveFlag.All
        }
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QGraphicsApiFilter::OpenGLProfile"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QGraphicsApiFilter::Api"
        RejectEnumValue{
            name: "Vulkan"
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QObjectPicker"
        javaName: "QObjectPicker"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickEvent"
        javaName: "QPickEvent"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickLineEvent"
        javaName: "QPickLineEvent"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickPointEvent"
        javaName: "QPickPointEvent"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DRender::QProximityFilter"
        javaName: "QProximityFilter"
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
        javaName: "QShaderProgramBuilder"
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
    
    EnumType{
        name: "Qt3DRender::QPickEvent::Buttons"
        since: [5, 8]
    }
    
    EnumType{
        name: "Qt3DRender::QPickEvent::Modifiers"
        since: [5, 8]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickTriangleEvent"
        javaName: "QPickTriangleEvent"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QBlendEquationArguments"
        javaName: "QBlendEquationArguments"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QClipPlane"
        javaName: "QClipPlane"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QMultiSampleAntiAliasing"
        javaName: "QMultiSampleAntiAliasing"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QNoDepthMask"
        javaName: "QNoDepthMask"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QPointSize"
        javaName: "QPointSize"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QSeamlessCubemap"
        javaName: "QSeamlessCubemap"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilMask"
        javaName: "QStencilMask"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilOperation"
        javaName: "QStencilOperation"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilOperationArguments"
        javaName: "QStencilOperationArguments"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DRender::QStencilTestArguments"
        javaName: "QStencilTestArguments"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QBlendEquationArguments::Blending"
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
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QPointSize::SizeMode"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QStencilOperationArguments::FaceMode"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QStencilOperationArguments::Operation"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QStencilTestArguments::StencilFaceMode"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QStencilTestArguments::StencilFunction"
        since: [5, 7]
    }
    
    EnumType{
        name: "Qt3DRender::QTextureImage::Status"
        since: [5, 7]
    }
    
    ValueType{
        name: "Qt3DRender::QTextureImageData"
        javaName: "QTextureImageData"
        ModifyFunction{
            signature: "operator=(const Qt3DRender::QTextureImageData &)"
            remove: RemoveFlag.All
        }
        since: [5, 7]
    }
    
    FunctionalType{
        name: "Qt3DRender::QTextureImageData::DataConverter"
        using: "std::function<QByteArray(QByteArray,int,int,int)>"
        since: 6
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractRayCaster::RunMode"
    }
    
    EnumType{
        name: "Qt3DRender::QAbstractRayCaster::FilterMode"
    }
    
    ObjectType{
        name: "Qt3DRender::QAbstractRayCaster"
        javaName: "QAbstractRayCaster"
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
        ModifyFunction{
            signature: "hitsChanged(const Qt3DRender::QAbstractRayCaster::Hits &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.core.QList<io.qt.qt3d.render.QRayCasterHit>"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const Qt3DRender::QAbstractRayCaster::Hits& %out = qtjambi_cast<const Qt3DRender::QAbstractRayCaster::Hits&>(%env, %scope, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "hits() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.QList<io.qt.qt3d.render.QRayCasterHit>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QRayCaster"
        javaName: "QRayCaster"
        ModifyFunction{
            signature: "pick(QVector3D,QVector3D,float)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.QList<io.qt.qt3d.render.QRayCasterHit>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<Qt3DRender::QAbstractRayCaster::Hits*>(%env, %in);"}
                }
            }
            since: 6
        }
    }
    
    EnumType{
        name: "Qt3DRender::QRayCasterHit::HitType"
    }
    
    ValueType{
        name: "Qt3DRender::QRayCasterHit"
        javaName: "QRayCasterHit"
        ModifyFunction{
            signature: "operator=(const Qt3DRender::QRayCasterHit &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DRender::QDepthRange"
        javaName: "QDepthRange"
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QNoPicking"
        javaName: "QNoPicking"
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QRasterMode"
        javaName: "QRasterMode"
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QShaderImage"
        javaName: "QShaderImage"
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
        javaName: "QSubtreeEnabler"
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QRenderCapabilities"
        javaName: "QRenderCapabilities"
        since: [5, 15]
    }
    
    ObjectType{
        name: "Qt3DRender::QDebugOverlay"
        javaName: "QDebugOverlay"
        since: [5, 15]
    }
    
    EnumType{
        name: "Qt3DRender::QRenderCapabilities::API"
        since: [5, 15]
    }
    
    EnumType{
        name: "Qt3DRender::QRenderCapabilities::Profile"
        since: [5, 15]
    }
    
    ValueType{
        name: "Qt3DRender::QTextureDataUpdate"
        javaName: "QTextureDataUpdate"
        ModifyFunction{
            signature: "operator=(const Qt3DRender::QTextureDataUpdate &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setData(const QSharedPointer<Qt3DRender::QTextureImageData> &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.qt3d.render.QTextureImageData"
                }
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
                ReplaceType{
                    modifiedType: "io.qt.qt3d.render.QTextureImageData"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        since: [5, 14]
    }
    
    EnumType{
        name: "Qt3DRender::QRasterMode::FaceMode"
        since: [5, 14]
    }
    
    EnumType{
        name: "Qt3DRender::QRasterMode::RasterMode"
        since: [5, 14]
    }
    
    EnumType{
        name: "Qt3DRender::QShaderImage::Access"
        since: [5, 14]
    }
    
    EnumType{
        name: "Qt3DRender::QShaderImage::ImageFormat"
        since: [5, 14]
    }
    
    EnumType{
        name: "Qt3DRender::QSubtreeEnabler::Enablement"
        since: [5, 14]
    }
    
    ObjectType{
        name: "Qt3DRender::QPickingProxy"
        javaName: "QPickingProxy"
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
