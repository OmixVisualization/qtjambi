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
    packageName: "io.qt.gui.rhi"
    defaultSuperClass: "QtObject"
    targetName: "QtJambiGuiRhi"
    module: "qtjambi"
    LoadTypeSystem{name: "QtGui"}
    RequiredLibrary{
        name: "QtGui"
    }

    Rejection{className: "QRhiImplementation"}

    PrimitiveType{
        name: "MTLCommandBuffer"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
    }

    PrimitiveType{
        name: "MTLRenderCommandEncoder"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
    }

    ObjectType{
        name: "QRhi"
        EnumType{
            name: "BeginFrameFlag"
        }
        EnumType{
            name: "EndFrameFlag"
        }
        EnumType{
            name: "Feature"
        }
        EnumType{
            name: "Flag"
        }
        EnumType{
            name: "FrameOpResult"
        }
        EnumType{
            name: "Implementation"
        }
        EnumType{
            name: "ResourceLimit"
        }
        FunctionalType{
            name: "CleanupCallback"
        }
        ModifyFunction{
            signature: "beginOffscreenFrame(QRhiCommandBuffer **, QRhi::BeginFrameFlags)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
                NoNullPointer{}
            }
        }
        ModifyFunction{
            signature: "create(QRhi::Implementation,QRhiInitParams*,QRhi::Flags,QRhiNativeHandles*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(__qt_return_value!=null){\n"+
                              "    __qt_return_value.__rc_params = params;\n"+
                              "    __qt_return_value.__rc_importDevice = importDevice;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "newBuffer(QRhiBuffer::Type,QRhiBuffer::UsageFlags,quint32)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newComputePipeline()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newGraphicsPipeline()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newRenderBuffer(QRhiRenderBuffer::Type,QSize,int,QRhiRenderBuffer::Flags,QRhiTexture::Format)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newSampler(QRhiSampler::Filter,QRhiSampler::Filter,QRhiSampler::Filter,QRhiSampler::AddressMode,QRhiSampler::AddressMode,QRhiSampler::AddressMode)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newShaderResourceBindings()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newSwapChain()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newTexture(QRhiTexture::Format,QSize,int,QRhiTexture::Flags)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newTexture(QRhiTexture::Format,int,int,int,int,QRhiTexture::Flags)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newTextureArray(QRhiTexture::Format,int,QSize,int,QRhiTexture::Flags)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "newTextureRenderTarget(QRhiTextureRenderTargetDescription,QRhiTextureRenderTarget::Flags)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.Beginning
            Text{content: "private io.qt.gui.rhi.QRhiInitParams __rc_params;\n"+
                          "private io.qt.gui.rhi.QRhiNativeHandles __rc_importDevice;"}
        }
    }
    ObjectType{
        name: "QRhiDepthStencilClearValue"
    }
    ObjectType{
        name: "QRhiViewport"
    }
    ObjectType{
        name: "QRhiScissor"
    }
    ObjectType{
        name: "QRhiVertexInputBinding"
        EnumType{
            name: "Classification"
        }
    }
    ObjectType{
        name: "QRhiVertexInputAttribute"
        EnumType{
            name: "Format"
        }
    }
    ObjectType{
        name: "QRhiVertexInputLayout"
        ModifyFunction{
            signature: "setBindings<InputIterator>(InputIterator,InputIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setAttributes<InputIterator>(InputIterator,InputIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cbeginBindings() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cendBindings() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cbeginAttributes() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cendAttributes() const"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QRhiShaderStage"
        EnumType{
            name: "Type"
        }
    }
    ObjectType{
        name: "QRhiShaderResourceBinding"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        EnumType{
            name: "Type"
        }
        EnumType{
            name: "StageFlag"
        }
        ObjectType{
            name: "TextureAndSampler"
            ModifyField{
                name: "sampler"
                ReferenceCount{
                    variableName: "__rcSampler"
                    action: ReferenceCount.Set
                }
            }
            ModifyField{
                name: "tex"
                ReferenceCount{
                    variableName: "__rcTex"
                    action: ReferenceCount.Set
                }
            }
        }
        Rejection{className: "Data"}
        Rejection{className: "Data::UniformBufferData"}
        Rejection{className: "Data::TextureAndOrSamplerData"}
        Rejection{className: "Data::StorageImageData"}
        Rejection{className: "Data::UniformBufferData"}
        ModifyFunction{
            signature: "textures(int,QRhiShaderResourceBinding::StageFlags,int,QRhiTexture**)"
            ModifyArgument{
                index: 4
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
                NoNullPointer{}
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                ArgumentMap{
                    metaName: "%3"
                    index: 3
                }
                Text{content: "if(%3 == 0 || %3 > QRhiShaderResourceBinding::Data::MAX_TEX_SAMPLER_ARRAY_SIZE){\n"+
                              "    Java::Runtime::IllegalArgumentException::throwNew(%env, \"Array size must not be 0 or greater 16.\" QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "serializeLayoutDescription<Output>(const QRhiShaderResourceBinding *,const QRhiShaderResourceBinding *,Output)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.gui.rhi.@NonNull QRhiShaderResourceBinding @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
JObjectArrayPointer<QRhiShaderResourceBinding> array(%env, %in,
    [](QRhiShaderResourceBinding & pointer,JNIEnv *env, jobject element){
        pointer = qtjambi_cast<QRhiShaderResourceBinding>(env, element);
    }
);
const QRhiShaderResourceBinding * %out = array.pointer();
                        `}
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.util.@NonNull Collection<@NonNull@QtPrimitiveType Integer>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "auto %out = std::back_inserter(qtjambi_cast<QList<quint32>&>(%env, %scope, %in));"}
                }
            }
            Instantiation{
                Argument{
                    type: "QList<quint32>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 2
                    RemoveArgument{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "const QRhiShaderResourceBinding * %out = array.pointer()+array.size();"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "quint32*"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsBuffer{}
                    AddImplicitCall{type: "int @NonNull[]"}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
JBufferData %out_buffer(__jni_env, %in);
quint32* %out = %out_buffer.data<quint32>();
if(%out_buffer.size()<array.size()*4)
    Java::Runtime::IllegalArgumentException::throwNew(%env, \"Buffer size must be greater binding count * 4.\" QTJAMBI_STACKTRACEINFO );\n"+
`}
                    }
                }
            }
        }
        Rejection{fieldName:"*"}
    }
    ObjectType{
        name: "QRhiColorAttachment"
        ModifyFunction{
            signature: "setRenderBuffer(QRhiRenderBuffer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRenderBuffer"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setResolveTexture(QRhiTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcResolveTexture"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setTexture(QRhiTexture*)"
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
        name: "QRhiTextureRenderTargetDescription"
        ModifyFunction{
            signature: "setDepthStencilBuffer(QRhiRenderBuffer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDepthStencilBuffer"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setDepthTexture(QRhiTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDepthTexture"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setColorAttachments<InputIterator>(InputIterator,InputIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cbeginColorAttachments() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cendColorAttachments() const"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QRhiTextureSubresourceUploadDescription"
        ModifyFunction{
            signature: "QRhiTextureSubresourceUploadDescription(const void*,quint32)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter:  2
                }
            }
        }
    }
    ObjectType{
        name: "QRhiTextureUploadEntry"
    }
    ObjectType{
        name: "QRhiTextureUploadDescription"
        ModifyFunction{
            signature: "setEntries<InputIterator>(InputIterator,InputIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cbeginEntries() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cendEntries() const"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QRhiTextureCopyDescription"
    }
    ObjectType{
        name: "QRhiNativeHandles"
    }
    ObjectType{
        name: "QRhiInitParams"
    }
    ObjectType{
        name: "QRhiStats"
    }
    ObjectType{
        name: "QRhiDriverInfo"
        EnumType{
            name: "DeviceType"
        }
    }
    ObjectType{
        name: "QRhiReadbackResult"
    }
    ObjectType{
        name: "QShader"
        ObjectType{
            name: "NativeShaderInfo"
        }
        ObjectType{
            name: "SeparateToCombinedImageSamplerMapping"
        }
        EnumType{
            name: "SerializedFormatVersion"
        }
        EnumType{
            name: "Source"
        }
        EnumType{
            name: "Stage"
        }
        EnumType{
            name: "Variant"
        }
    }
    ObjectType{
        name: "QShaderDescription"
        ObjectType{
            name: "BlockVariable"
        }
        ObjectType{
            name: "BuiltinVariable"
        }
        ObjectType{
            name: "InOutVariable"
        }
        ObjectType{
            name: "PushConstantBlock"
        }
        ObjectType{
            name: "StorageBlock"
        }
        ObjectType{
            name: "UniformBlock"
        }
        EnumType{
            name: "BuiltinType"
        }
        EnumType{
            name: "ImageFlag"
        }
        EnumType{
            name: "ImageFormat"
        }
        EnumType{
            name: "QualifierFlag"
        }
        EnumType{
            name: "TessellationMode"
        }
        EnumType{
            name: "TessellationPartitioning"
        }
        EnumType{
            name: "TessellationWindingOrder"
        }
        EnumType{
            name: "VariableType"
        }
    }
    ObjectType{
        name: "QRhiResource"
        EnumType{
            name: "Type"
        }
        Rejection{fieldName: "m_rhi"}
        Rejection{fieldName: "m_id"}
        Rejection{fieldName: "m_objectName"}
        ModifyFunction{
            signature: "deleteLater()"
            Rename{
                to: "disposeLater"
            }
            /*InjectCode{
                target: CodeClass.Java
                Text{content: ""}
            }*/
        }
    }
    ObjectType{
        name: "QRhiBuffer"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ObjectType{
            name: "NativeBuffer"
        }
        EnumType{
            name: "Type"
        }
        EnumType{
            name: "UsageFlag"
        }
        Rejection{fieldName: "*"}
        ModifyFunction{
            signature: "beginFullDynamicBufferUpdateForCurrentFrame()"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.nio.ByteBuffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = DataJBuffer(%env, %in, this->size()).take();"}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(__qt_this->type()!=QRhiBuffer::Dynamic){\n"+
                              "    Java::Runtime::IllegalStateException::throwNew(%env, \"Buffer type is not Dynamic.\" QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
            }
        }
    }
    ObjectType{
        name: "QRhiCommandBuffer"
        EnumType{
            name: "BeginPassFlag"
        }
        EnumType{
            name: "IndexFormat"
        }
        ModifyFunction{
            signature: "setComputePipeline(QRhiComputePipeline*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcComputePipeline"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setGraphicsPipeline(QRhiGraphicsPipeline*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGraphicsPipeline"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setVertexInput(int,int,const std::pair<QRhiBuffer*,quint32>*,QRhiBuffer*,quint32,QRhiCommandBuffer::IndexFormat)"
            ModifyArgument{
                index: 3
                AsArray{
                    lengthParameter: 2
                    noOffset: true
                }
                NoNullPointer{}
            }
            ModifyArgument{
                index: 3
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setShaderResources(QRhiShaderResourceBindings*,int,const std::pair<int,quint32>*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 3
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
                NoNullPointer{}
                ReplaceDefaultExpression{expression: "noDynamicOffsets"}
            }
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.End
            Text{content: "@SuppressWarnings(\"unchecked\")\nprivate final static io.qt.core.QPair<java.lang.@QtPrimitiveType Integer, java.lang.@QtPrimitiveType Integer>[] noDynamicOffsets = new io.qt.core.QPair[0];"}
        }
    }
    ObjectType{
        name: "QRhiComputePipeline"
        EnumType{
            name: "Flag"
        }
        ModifyFunction{
            signature: "setShaderResourceBindings(QRhiShaderResourceBindings*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcShaderResourceBindings"
                    action: ReferenceCount.Set
                }
            }
        }
        Rejection{fieldName: "*"}
    }
    ObjectType{
        name: "QRhiGraphicsPipeline"
        ObjectType{
            name: "StencilOpState"
        }
        ObjectType{
            name: "TargetBlend"
        }
        EnumType{
            name: "BlendFactor"
        }
        EnumType{
            name: "BlendOp"
        }
        EnumType{
            name: "ColorMaskComponent"
        }
        EnumType{
            name: "CompareOp"
        }
        EnumType{
            name: "CullMode"
        }
        EnumType{
            name: "Flag"
        }
        EnumType{
            name: "FrontFace"
        }
        EnumType{
            name: "PolygonMode"
        }
        EnumType{
            name: "StencilOp"
        }
        EnumType{
            name: "Topology"
        }
        Rejection{fieldName: "*"}
        ModifyFunction{
            signature: "setRenderPassDescriptor(QRhiRenderPassDescriptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRenderPassDescriptor"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setShaderResourceBindings(QRhiShaderResourceBindings*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcShaderResourceBindings"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setTargetBlends<InputIterator>(InputIterator,InputIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cbeginTargetBlends() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cendTargetBlends() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setShaderStages<InputIterator>(InputIterator,InputIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cbeginShaderStages() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cendShaderStages() const"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QRhiRenderBuffer"
        ObjectType{
            name: "NativeRenderBuffer"
        }
        EnumType{
            name: "Flag"
        }
        EnumType{
            name: "Type"
        }
        Rejection{fieldName: "*"}
    }
    ObjectType{
        name: "QRhiRenderPassDescriptor"
        ModifyFunction{
            signature: "newCompatibleRenderPassDescriptor()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    ObjectType{
        name: "QRhiRenderTarget"
        Rejection{fieldName: "*"}
        ModifyFunction{
            signature: "setRenderPassDescriptor(QRhiRenderPassDescriptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRenderPassDescriptor"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    ObjectType{
        name: "QRhiSampler"
        EnumType{
            name: "AddressMode"
        }
        EnumType{
            name: "CompareOp"
        }
        EnumType{
            name: "Filter"
        }
        Rejection{fieldName: "*"}
    }
    ObjectType{
        name: "QRhiShaderResourceBindings"
        EnumType{
            name: "UpdateFlag"
        }
        ModifyFunction{
            signature: "setBindings<InputIterator>(InputIterator,InputIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cbeginBindings() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cendBindings() const"
            remove: RemoveFlag.All
        }
        Rejection{fieldName: "*"}
    }
    ObjectType{
        name: "QRhiSwapChain"
        EnumType{
            name: "Flag"
        }
        EnumType{
            name: "Format"
        }
        EnumType{
            name: "StereoTargetBuffer"
        }
        Rejection{fieldName: "*"}
        ModifyFunction{
            signature: "newCompatibleRenderPassDescriptor()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setDepthStencil(QRhiRenderBuffer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDepthStencil"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setRenderPassDescriptor(QRhiRenderPassDescriptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRenderPassDescriptor"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setWindow(QWindow*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWindow"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    ObjectType{
        name: "QRhiTexture"
        ObjectType{
            name: "NativeTexture"
        }
        EnumType{
            name: "Flag"
        }
        EnumType{
            name: "Format"
        }
        Rejection{fieldName: "*"}
    }
    ObjectType{
        name: "QRhiDriverInfo"
    }
    ObjectType{
        name: "QRhiGles2InitParams"
        ppCondition: "QT_CONFIG(opengl)"
        ExtraIncludes{
            Include{
                fileName: "QOpenGLContext"
                location: Include.Global
            }
            Include{
                fileName: "QWindow"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "newFallbackSurface(QSurfaceFormat)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyField{
            name: "fallbackSurface"
            ReferenceCount{
                variableName: "__rcfallbackSurface"
                action: ReferenceCount.Set
            }
        }
        ModifyField{
            name: "window"
            ReferenceCount{
                variableName: "__rcwindow"
                action: ReferenceCount.Set
            }
        }
        ModifyField{
            name: "shareContext"
            ReferenceCount{
                variableName: "__rcshareContext"
                action: ReferenceCount.Set
            }
        }
    }
    ObjectType{
        name: "QRhiGles2NativeHandles"
        ppCondition: "QT_CONFIG(opengl)"
        ExtraIncludes{
            Include{
                fileName: "QOpenGLContext"
                location: Include.Global
            }
        }
        ModifyField{
            name: "context"
            ReferenceCount{
                variableName: "__rccontext"
                action: ReferenceCount.Set
            }
        }
    }
    ObjectType{
        name: "QRhiInitParams"
    }
    ObjectType{
        name: "QRhiNullInitParams"
    }
    ObjectType{
        name: "QRhiNullNativeHandles"
    }
    ObjectType{
        name: "QRhiD3D11InitParams"
        ppCondition: "defined(Q_OS_WIN)"
    }
    ObjectType{
        name: "QRhiD3D11NativeHandles"
        ppCondition: "defined(Q_OS_WIN)"
    }
    ObjectType{
        name: "QRhiD3D12CommandBufferNativeHandles"
        ppCondition: "defined(Q_OS_WIN)"
    }
    ObjectType{
        name: "QRhiD3D12InitParams"
        ppCondition: "defined(Q_OS_WIN)"
    }
    ObjectType{
        name: "QRhiD3D12NativeHandles"
        ppCondition: "defined(Q_OS_WIN)"
    }
    ObjectType{
        name: "QRhiMetalCommandBufferNativeHandles"
        ppCondition: "defined(Q_OS_MACOS)"
    }
    ObjectType{
        name: "QRhiMetalCommandBufferNativeHandles"
        ppCondition: "defined(Q_OS_MACOS)"
    }
    ObjectType{
        name: "QRhiMetalInitParams"
        ppCondition: "defined(Q_OS_MACOS)"
    }
    ObjectType{
        name: "QRhiMetalNativeHandles"
        ppCondition: "defined(Q_OS_MACOS)"
        Rejection{fieldName: "dev"}
        Rejection{fieldName: "cmdQueue"}
    }
    ObjectType{
        name: "QRhiReadbackDescription"
        ModifyFunction{
            signature: "setTexture(QRhiTexture*)"
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
        name: "QRhiReadbackResult"
    }
    ObjectType{
        name: "QRhiResourceUpdateBatch"
        ModifyFunction{
            signature: "updateDynamicBuffer(QRhiBuffer*,quint32,quint32,const void*)"
            ModifyArgument{
                index: 4
                AsBuffer{
                    lengthParameter: 3
                }
            }
        }
        ModifyFunction{
            signature: "uploadStaticBuffer(QRhiBuffer*,quint32,quint32,const void*)"
            ModifyArgument{
                index: 4
                AsBuffer{
                    lengthParameter: 3
                }
            }
        }
        ModifyFunction{
            signature: "uploadStaticBuffer(QRhiBuffer*,const void*)"
            ModifyArgument{
                index: 2
                AsBuffer{
                }
            }
        }
    }
    ObjectType{
        name: "QRhiStats"
    }
    ObjectType{
        name: "QRhiSwapChainHdrInfo"
        Rejection{fieldName: "*"}
    }
    ObjectType{
        name: "QRhiSwapChainProxyData"
    }
    ObjectType{
        name: "QRhiSwapChainRenderTarget"
        Rejection{fieldName: "*"}
    }
    ObjectType{
        name: "QRhiTextureRenderTarget"
        Rejection{fieldName: "*"}
        ModifyFunction{
            signature: "newCompatibleRenderPassDescriptor()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    ObjectType{
        name: "QShaderCode"
    }
    ObjectType{
        name: "QShaderKey"
    }
    ObjectType{
        name: "QShaderVersion"
    }
}
