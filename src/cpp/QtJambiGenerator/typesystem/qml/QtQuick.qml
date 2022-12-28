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
    packageName: "io.qt.quick"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtQuick"
    module: "qtjambi.quick"
    description: "A declarative framework for building highly dynamic applications with custom user interfaces."

    InjectCode{
        position: Position.Position4
        Text{content: "if(io.qt.QtUtilities.isAvailableQtLibrary(\"QmlWorkerScript\"))\n"+
                      "    io.qt.QtUtilities.loadQtLibrary(\"QmlWorkerScript\");"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_registerParserStatusCaster();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_registerParserStatusCaster();"}
    }
    
    TemplateType{
        name: "QuickItem"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "itemChange(QQuickItem::ItemChange, const QQuickItem::ItemChangeData &)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.Object"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//conversion-rule-shell\n"+
                                  "jobject %out = nullptr;\n"+
                                  "switch(%1){\n"+
                                  "case QQuickItem::ItemChildAddedChange:      // value.item\n"+
                                  "case QQuickItem::ItemChildRemovedChange:    // value.item\n"+
                                  "case QQuickItem::ItemParentHasChanged:      // value.item\n"+
                                  "    {\n"+
                                  "        %out = qtjambi_cast<jobject>(%env, %in.item);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuickItem::ItemSceneChange:           // value.window\n"+
                                  "    {\n"+
                                  "        %out = qtjambi_cast<jobject>(%env, %in.window);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuickItem::ItemVisibleHasChanged:     // value.boolValue\n"+
                                  "case QQuickItem::ItemActiveFocusHasChanged: // value.boolValue\n"+
                                  "case QQuickItem::ItemAntialiasingHasChanged: // value.boolValue\n"+
                                  "case QQuickItem::ItemEnabledHasChanged:      // value.boolValue\n"+
                                  "    {\n"+
                                  "        %out = qtjambi_cast<jobject>(%env, %in.boolValue);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuickItem::ItemOpacityHasChanged:     // value.realValue\n"+
                                  "case QQuickItem::ItemRotationHasChanged:    // value.realValue\n"+
                                  "case QQuickItem::ItemDevicePixelRatioHasChanged: // value.realValue\n"+
                                  "    {\n"+
                                  "        %out = qtjambi_cast<jobject>(%env, double(%in.realValue));\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "default:\n"+
                                  "    break;\n"+
                                  "}"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "//conversion-rule-native\n"+
                                  "QQuickItem::ItemChangeData %out(false);\n"+
                                  "switch(__qt_%1){\n"+
                                  "case QQuickItem::ItemChildAddedChange:      // value.item\n"+
                                  "case QQuickItem::ItemChildRemovedChange:    // value.item\n"+
                                  "case QQuickItem::ItemParentHasChanged:      // value.item\n"+
                                  "    {\n"+
                                  "        if(%in && !Java::QtQuick::QQuickItem::isInstanceOf(%env, %in)){\n"+
                                  "            JavaException::raiseIllegalArgumentException(%env, \"Object of type QQuickItem expected.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "        }\n"+
                                  "        %out.item = qtjambi_cast<QQuickItem*>(%env, %in);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuickItem::ItemSceneChange:           // value.window\n"+
                                  "    {\n"+
                                  "        if(%in && !Java::QtQuick::QQuickWindow::isInstanceOf(%env, %in)){\n"+
                                  "            JavaException::raiseIllegalArgumentException(%env, \"Object of type QQuickWindow expected.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "        }\n"+
                                  "        %out.window = qtjambi_cast<QQuickWindow*>(%env, %in);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuickItem::ItemVisibleHasChanged:     // value.boolValue\n"+
                                  "case QQuickItem::ItemActiveFocusHasChanged: // value.boolValue\n"+
                                  "case QQuickItem::ItemAntialiasingHasChanged: // value.boolValue\n"+
                                  "case QQuickItem::ItemEnabledHasChanged:      // value.boolValue\n"+
                                  "    {\n"+
                                  "        if(%in && !Java::Runtime::Boolean::isInstanceOf(%env, %in)){\n"+
                                  "            JavaException::raiseIllegalArgumentException(%env, \"Boolean value expected.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "        }\n"+
                                  "        %out.boolValue = QtJambiAPI::fromJavaBooleanObject(%env, %in);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuickItem::ItemOpacityHasChanged:     // value.realValue\n"+
                                  "case QQuickItem::ItemRotationHasChanged:    // value.realValue\n"+
                                  "case QQuickItem::ItemDevicePixelRatioHasChanged: // value.realValue\n"+
                                  "    {\n"+
                                  "        if(%in && !Java::Runtime::Number::isInstanceOf(%env, %in)){\n"+
                                  "            JavaException::raiseIllegalArgumentException(%env, \"Number value expected.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "        }\n"+
                                  "        %out.realValue = qreal(QtJambiAPI::fromJavaDoubleObject(%env, %in));\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "default:\n"+
                                  "    break;\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "updatePaintNode(QSGNode *, QQuickItem::UpdatePaintNodeData *)"
            noExcept: true
            threadAffinity: false
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Invalidate
                }
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 0
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
    
    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.Optional
        since: 6
    }
    
    Rejection{
        className: "QSGTexturePrivate"
    }
    
    Rejection{
        className: "QSGOpaqueTextureMaterial"
        fieldName: "m_texture"
    }
    
    Rejection{
        className: "QSGOpaqueTextureMaterial"
        fieldName: "m_mipmap_filtering"
    }
    
    Rejection{
        className: "QSGOpaqueTextureMaterial"
        fieldName: "m_filtering"
    }
    
    Rejection{
        className: "QSGOpaqueTextureMaterial"
        fieldName: "m_horizontal_wrap"
    }
    
    Rejection{
        className: "QSGOpaqueTextureMaterial"
        fieldName: "m_vertical_wrap"
    }
    
    Rejection{
        className: "QSGOpaqueTextureMaterial"
        fieldName: "m_anisotropy_level"
    }
    
    Rejection{
        className: "QSGOpaqueTextureMaterial"
        fieldName: "m_reserved"
    }
    
    Rejection{
        className: "QQuickWindow"
        enumName: "NativeObjectType"
    }
    
    Rejection{
        className: "QQuickWindow"
        functionName: "createTextureFromNativeObject"
    }
    
    Rejection{
        className: "QSGMaterialRhiShader::RenderState"
        functionName: "rhi"
    }
    
    Rejection{
        className: "QSGMaterialRhiShader::RenderState"
        functionName: "resourceUpdateBatch"
    }
    
    Rejection{
        className: "QSGMaterialRhiShader"
        functionName: "setShader"
    }
    
    Rejection{
        className: "QSGMaterialShader::RenderState"
        functionName: "rhi"
    }
    
    Rejection{
        className: "QSGMaterialShader::RenderState"
        functionName: "resourceUpdateBatch"
    }
    
    Rejection{
        className: "QSGMaterialShader"
        functionName: "setShader"
    }
    
    Rejection{
        className: "QSGTexture"
        functionName: "updateRhiTexture"
    }
    
    Rejection{
        className: "QSGTexture"
        functionName: "setWorkResourceUpdateBatch"
    }
    
    Rejection{
        className: "QSGTexture"
        functionName: "rhiTexture"
    }
    
    Rejection{
        className: "QSGTexture"
        functionName: "removedFromAtlas"
    }
    
    Rejection{
        className: "QSGTexture"
        functionName: "commitTextureOperations"
    }
    
    Rejection{
        className: "PtrShaderCreateFunc"
    }
    
    Rejection{
        className: "QSGTexture::NativeTexture"
    }
    
    Rejection{
        className: "QSGTexture"
        functionName: "nativeTexture"
    }
    
    Rejection{
        className: "QQuickWheelEvent"
    }
    
    Rejection{
        className: "QQuickMouseEvent"
    }
    
    Rejection{
        className: "QQuickKeyEvent"
    }
    
    Rejection{
        className: "QQuickGraphicsDevice"
        functionName: "fromDeviceObjects"
    }
    
    Rejection{
        className: "QQuickGraphicsDevice"
        functionName: "fromPhysicalDevice"
    }
    
    Rejection{
        className: "QQuickGraphicsDevice"
        functionName: "fromDeviceAndContext"
    }
    
    Rejection{
        className: "QQuickGraphicsDevice"
        functionName: "fromDeviceAndCommandQueue"
    }
    
    EnumType{
        name: "QQuickItem::Flag"
        flags: "QQuickItem::Flags"
    }
    
    EnumType{
        name: "QQuickItem::ItemChange"
    }
    
    EnumType{
        name: "QQuickItem::TransformOrigin"
    }
    
    EnumType{
        name: "QQuickPaintedItem::PerformanceHint"
        flags: "QQuickPaintedItem::PerformanceHints"
    }
    
    EnumType{
        name: "QQuickPaintedItem::RenderTarget"
    }
    
    EnumType{
        name: "QQuickView::ResizeMode"
    }
    
    EnumType{
        name: "QQuickView::Status"
    }
    
    EnumType{
        name: "QQuickWindow::CreateTextureOption"
        flags: "QQuickWindow::CreateTextureOptions"
    }
    
    EnumType{
        name: "QSGNode::DirtyStateBit"
        flags: "QSGNode::DirtyState"
    }
    
    EnumType{
        name: "QSGNode::Flag"
        flags: "QSGNode::Flags"
    }
    
    EnumType{
        name: "QSGNode::NodeType"
        generate: false
    }
    
    EnumType{
        name: "QSGGeometry::DataPattern"
    }
    
    EnumType{
        name: "QSGGeometry::Type"
    }
    
    EnumType{
        name: "QSGGeometry::DrawingMode"
    }
    
    EnumType{
        name: "QSGGeometry::AttributeType"
    }
    
    EnumType{
        name: "QSGMaterial::Flag"
        flags: "QSGMaterial::Flags"
        RejectEnumValue{
            name: "CustomCompileStep"
            since: [6, 3]
        }
    }
    
    EnumType{
        name: "QSGTexture::Filtering"
    }
    
    EnumType{
        name: "QSGTexture::WrapMode"
    }
    
    EnumType{
        name: "QSGTexture::AnisotropyLevel"
        since: [5, 9]
    }
    
    EnumType{
        name: "QSGMaterialShader::RenderState::DirtyState"
        flags: "QSGMaterialShader::RenderState::DirtyStates"
    }
    
    EnumType{
        name: "QSGSimpleTextureNode::TextureCoordinatesTransformFlag"
        flags: "QSGSimpleTextureNode::TextureCoordinatesTransformMode"
    }
    
    EnumType{
        name: "QQuickWindow::SceneGraphError"
    }
    
    EnumType{
        name: "QQuickWindow::RenderStage"
    }
    
    EnumType{
        name: "QQuickWindow::TextRenderType"
        since: [5, 10]
    }
    
    
    
    EnumType{
        name: "QSGRendererInterface::GraphicsApi"
        RejectEnumValue{
            name: "OpenGLRhi"
            since: 6
        }
        RejectEnumValue{
            name: "Direct3D11Rhi"
            since: 6
        }
        RejectEnumValue{
            name: "VulkanRhi"
            since: 6
        }
        RejectEnumValue{
            name: "MetalRhi"
            since: 6
        }
        RejectEnumValue{
            name: "NullRhi"
            since: 6
        }
        since: [5, 8]
    }
    
    EnumType{
        name: "QSGRendererInterface::Resource"
        since: [5, 8]
    }
    
    EnumType{
        name: "QSGRendererInterface::ShaderType"
        since: [5, 8]
    }
    
    EnumType{
        name: "QSGRendererInterface::RenderMode"
        since: 6
    }
    
    EnumType{
        name: "QSGRendererInterface::ShaderCompilationType"
        flags: "QSGRendererInterface::ShaderCompilationTypes"
        since: [5, 8]
    }
    
    EnumType{
        name: "QSGRendererInterface::ShaderSourceType"
        flags: "QSGRendererInterface::ShaderSourceTypes"
        since: [5, 8]
    }
    
    InterfaceType{
        name: "QSGRendererInterface"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QTextCodec"
                location: Include.Global
                until: 5
            }
        }
        ModifyFunction{
            signature: "getResource(QQuickWindow *, QSGRendererInterface::Resource) const"
            remove: RemoveFlag.JavaAndNative
        }
        ModifyFunction{
            signature: "getResource(QQuickWindow *, const char *) const"
            remove: RemoveFlag.JavaAndNative
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jstring %out = qtjambi_cast<jstring>(%env, QLatin1String(%in));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray b1 = qtjambi_cast<QByteArray>(%env, %in);\n"+
                                  "const char* %out = b1.data();"}
                }
            }
        }
        since: [5, 8]
    }
    
    NamespaceType{
        name: "QQuickOpenGLUtils"
        since: 6
    }
    
    ObjectType{
        name: "QQuickRenderControl"
        ModifyFunction{
            signature: "renderWindow(QPoint *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.core.QPoint"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QPoint* %out = qtjambi_cast<QPoint*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "renderWindowFor(QQuickWindow *, QPoint *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.core.QPoint"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QPoint* %out = qtjambi_cast<QPoint*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QSGContextPlugin"
    }
    
    ObjectType{
        name: "QQuickCloseEvent"
        packageName: "io.qt.quick.internal"
        targetType: "final class"
        generate: "no-shell"
        ModifyFunction{
            signature: "QQuickCloseEvent()"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QQuickItem"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "childMouseEventFilter(QQuickItem*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragEnterEvent(QDragEnterEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragLeaveEvent(QDragLeaveEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragMoveEvent(QDragMoveEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dropEvent(QDropEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "focusInEvent(QFocusEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "focusOutEvent(QFocusEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hoverEnterEvent(QHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hoverLeaveEvent(QHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hoverMoveEvent(QHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "inputMethodEvent(QInputMethodEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyPressEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyReleaseEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseDoubleClickEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseMoveEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "touchEvent(QTouchEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QWheelEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        Import{
            template: "QuickItem"
        }
        ModifyFunction{
            signature: "textureProvider()const"
            threadAffinity: false
        }
        ModifyFunction{
            signature: "setContainmentMask(QObject*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcContainmentMask"
                    condition: "hasContainsMethod"
                    action: ReferenceCount.Set
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "boolean hasContainsMethod = true;\n"+
                              "if(%1!=null){\n"+
                              "    hasContainsMethod = %1.metaObject().method(\"contains\", io.qt.core.QPointF.class)!=null;\n"+
                              "}"}
            }
            since: [5, 11]
        }
        ModifyFunction{
            signature: "itemTransform(QQuickItem *, bool *) const"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "bool b = false;\n"+
                              "bool* __qt_arg__2 = &b;"}
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = b ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "setParentItem(QQuickItem *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QQuickPaintedItem"
        ModifyFunction{
            signature: "antialiasing()const"
            rename: "paintAntialiasing"
        }
        ModifyFunction{
            signature: "setAntialiasing(bool)"
            rename: "setPaintAntialiasing"
        }
        ModifyFunction{
            signature: "paint( QPainter *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        Import{
            template: "QuickItem"
        }
    }
    
    ObjectType{
        name: "QQuickTextDocument"
    }
    
    ObjectType{
        name: "QQuickFramebufferObject"
        ModifyFunction{
            signature: "createRenderer()const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
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
        ModifyFunction{
            signature: "updatePaintNode(QSGNode *, QQuickItem::UpdatePaintNodeData *)"
            noExcept: true
            threadAffinity: false
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Invalidate
                }
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 0
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
        ModifyFunction{
            signature: "textureProvider()const"
            threadAffinity: false
        }
    }
    
    ObjectType{
        name: "QQuickFramebufferObject::Renderer"
        ModifyFunction{
            signature: "createFramebufferObject(const QSize &)"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
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
        name: "QQuickTextureFactory"
        ModifyFunction{
            signature: "createTexture(QQuickWindow*)const"
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
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
        name: "QQuickView"
        ModifyFunction{
            signature: "QQuickView(const QUrl &, QWindow *)"
            blockExceptions: true
        }
        ModifyFunction{
            signature: "setSource(const QUrl &)"
            blockExceptions: true
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setContent(QUrl,QQmlComponent*,QObject*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcQmlComponent"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 3
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QQuickWindow"
        ModifyFunction{
            signature: "setRenderTarget(QOpenGLFramebufferObject*)"
            threadAffinity: false
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setRenderTarget(uint, const QSize &)"
            threadAffinity: false
            until: 5
        }
        ModifyFunction{
            signature: "rendererInterface() const"
            threadAffinity: false
        }
        ModifyFunction{
            signature: "createImageNode()const"
            threadAffinity: false
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createRectangleNode()const"
            threadAffinity: false
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createNinePatchNode()const"
            threadAffinity: false
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createTextureFromImage(QImage)const"
            threadAffinity: false
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createTextureFromImage(QImage,QFlags<QQuickWindow::CreateTextureOption>)const"
            threadAffinity: false
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createTextureFromId(uint,QSize,QFlags<QQuickWindow::CreateTextureOption>)const"
            threadAffinity: false
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "scheduleRenderJob(QRunnable *, QQuickWindow::RenderStage)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
        InjectCode{
            Text{content: "@io.qt.QtUninvokable\n"+
                          "public final void scheduleRenderJob(Runnable job, io.qt.quick.QQuickWindow.RenderStage schedule){\n"+
                          "    scheduleRenderJob(io.qt.core.QRunnable.of(job), schedule);\n"+
                          "}"}
        }
    }
    
    ValueType{
        name: "QQuickWindow::GraphicsStateInfo"
        since: [5, 14]
    }
    
    ObjectType{
        name: "QSGNode"
        ModifyFunction{
            signature: "QSGNode(QSGNode::NodeType)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "type()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "removeChildNode ( QSGNode * )"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcChildren"
                    action: ReferenceCount.Take
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                    condition: "flags().testFlag(QSGNode.Flag.OwnedByParent)"
                }
            }
        }
        ModifyFunction{
            signature: "removeAllChildNodes()"
            InjectCode{
                position: Position.Beginning
                Text{content: "int size = childCount();\n"+
                              "java.util.List<QSGNode> nodes = new java.util.ArrayList<>(size);\n"+
                              "for (int i = 0; i < size; i++) {\n"+
                              "    nodes.add(childAtIndex(i));\n"+
                              "}"}
            }
            InjectCode{
                position: Position.End
                Text{content: "for (QSGNode node : nodes) {\n"+
                              "    if (node!=null && node.flags().testFlag(QSGNode.Flag.OwnedByParent)) {\n"+
                              "        QtJambi_LibraryUtilities.internal.setDefaultOwnership(node);\n"+
                              "    }\n"+
                              "}\n"+
                              "if(__rcChildren!=null)__rcChildren.clear();"}
            }
        }
        ModifyFunction{
            signature: "appendChildNode ( QSGNode * )"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcChildren"
                    condition: "!flags().testFlag(QSGNode.Flag.OwnedByParent)"
                    action: ReferenceCount.Add
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                    condition: "flags().testFlag(QSGNode.Flag.OwnedByParent)"
                }
            }
            InjectCode{
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.parent()!=null)\n"+
                              "    throw new IllegalArgumentException(\"QSGNode is already a child!\");"}
            }
        }
        ModifyFunction{
            signature: "prependChildNode ( QSGNode * )"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcChildren"
                    condition: "!flags().testFlag(QSGNode.Flag.OwnedByParent)"
                    action: ReferenceCount.Add
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                    condition: "flags().testFlag(QSGNode.Flag.OwnedByParent)"
                }
            }
            InjectCode{
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.parent()!=null)\n"+
                              "    throw new IllegalArgumentException(\"QSGNode is already a child!\");"}
            }
        }
        ModifyFunction{
            signature: "insertChildNodeBefore ( QSGNode *, QSGNode * )"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcChildren"
                    condition: "!flags().testFlag(QSGNode.Flag.OwnedByParent)"
                    action: ReferenceCount.Add
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                    condition: "flags().testFlag(QSGNode.Flag.OwnedByParent)"
                }
            }
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
            InjectCode{
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.parent()!=null)\n"+
                              "    throw new IllegalArgumentException(\"QSGNode is already a child!\");"}
            }
        }
        ModifyFunction{
            signature: "insertChildNodeAfter ( QSGNode *, QSGNode * )"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcChildren"
                    condition: "!flags().testFlag(QSGNode.Flag.OwnedByParent)"
                    action: ReferenceCount.Add
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                    condition: "flags().testFlag(QSGNode.Flag.OwnedByParent)"
                }
            }
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
            InjectCode{
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.parent()!=null)\n"+
                              "    throw new IllegalArgumentException(\"QSGNode is already a child!\");"}
            }
        }
    }
    
    GlobalFunction{
        signature: "qsgnode_set_description(QSGNode*, QString)"
        rename: "setDescription"
        targetType: "QSGNode"
    }
    
    ObjectType{
        name: "QSGBasicGeometryNode"
        forceAbstract: true
        ModifyFunction{
            signature: "QSGBasicGeometryNode(QSGNode::NodeType)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "geometry() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "matrix() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.gui.QMatrix4x4"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setRendererMatrix(const QMatrix4x4 *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.gui.QMatrix4x4"
                }
                ReferenceCount{
                    variableName: "__rcRendererMatrix"
                    action: ReferenceCount.Set
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QMatrix4x4* %out = qtjambi_cast<QMatrix4x4*>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setRendererClipList(const QSGClipNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRendererClipList"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setGeometry(QSGGeometry *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGeometry"
                    condition: "!flags().testFlag(QSGNode.Flag.OwnsGeometry)"
                    action: ReferenceCount.Set
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                    condition: "flags().testFlag(QSGNode.Flag.OwnsGeometry)"
                }
            }
        }
    }
    
    ObjectType{
        name: "QSGClipNode"
    }
    
    ObjectType{
        name: "QSGGeometryNode"
        ModifyFunction{
            signature: "setMaterial(QSGMaterial *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMaterial"
                    condition: "!flags().testFlag(QSGNode.Flag.OwnsMaterial)"
                    action: ReferenceCount.Set
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                    condition: "flags().testFlag(QSGNode.Flag.OwnsMaterial)"
                }
            }
        }
        ModifyFunction{
            signature: "setOpaqueMaterial(QSGMaterial *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcOpaqueMaterial"
                    condition: "!flags().testFlag(QSGNode.Flag.OwnsOpaqueMaterial)"
                    action: ReferenceCount.Set
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                    condition: "flags().testFlag(QSGNode.Flag.OwnsOpaqueMaterial)"
                }
            }
        }
    }
    
    ObjectType{
        name: "QSGOpacityNode"
    }
    
    ObjectType{
        name: "QSGNinePatchNode"
        ModifyFunction{
            signature: "setTexture(QSGTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 8]
    }
    
    ObjectType{
        name: "QSGImageNode"
        ModifyFunction{
            signature: "setTexture(QSGTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 8]
    }
    
    EnumType{
        name: "QSGImageNode::TextureCoordinatesTransformFlag"
        flags: "QSGImageNode::TextureCoordinatesTransformMode"
        since: [5, 8]
    }
    
    ObjectType{
        name: "QSGRectangleNode"
        since: [5, 8]
    }
    
    ObjectType{
        name: "QSGSimpleRectNode"
    }
    
    ObjectType{
        name: "QSGSimpleTextureNode"
        ModifyFunction{
            signature: "setTexture(QSGTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if (texture != null && ownsTexture()) {\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(texture);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "setOwnsTexture(bool)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if (texture() != null) {\n"+
                              "    if(owns){\n"+
                              "        QtJambi_LibraryUtilities.internal.setCppOwnership(texture());\n"+
                              "    }else{\n"+
                              "        QtJambi_LibraryUtilities.internal.setJavaOwnership(texture());\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QSGTransformNode"
    }
    
    ObjectType{
        name: "QSGRenderNode"
        ModifyFunction{
            signature: "flags() const"
            rename: "renderingFlags"
        }
        ModifyFunction{
            signature: "matrix() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.gui.QMatrix4x4"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QMatrix4x4* %out = qtjambi_cast<QMatrix4x4*>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "render(const QSGRenderNode::RenderState*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QSGRenderNode::RenderState"
        ModifyFunction{
            signature: "projectionMatrix() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.gui.QMatrix4x4"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QMatrix4x4* %out = qtjambi_cast<QMatrix4x4*>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "clipRegion() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.gui.QRegion"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QRegion* %out = qtjambi_cast<QRegion*>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "get(const char *) const"
            remove: RemoveFlag.JavaAndNative
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jstring>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = qtjambi_cast<const char*>(%env, %scope, %in);"}
                }
            }
        }
    }
    
    EnumType{
        name: "QSGRenderNode::StateFlag"
        flags: "QSGRenderNode::StateFlags"
        since: [5, 8]
    }
    
    EnumType{
        name: "QSGRenderNode::RenderingFlag"
        flags: "QSGRenderNode::RenderingFlags"
        since: [5, 8]
    }
    
    ObjectType{
        name: "QSGRootNode"
    }
    
    ObjectType{
        name: "QSGRootNode::RenderState"
    }
    
    ObjectType{
        name: "QSGDynamicTexture"
    }
    
    ObjectType{
        name: "QSGFlatColorMaterial"
        ModifyFunction{
            signature: "createShader() const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "createShader(QSGRendererInterface::RenderMode) const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QSGGeometry"
        ModifyFunction{
            signature: "indexData() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "indexDataAsUInt() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "indexDataAsUShort() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "indexDataAsUInt()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "indexDataAsUShort()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vertexData() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vertexDataAsPoint2D()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vertexDataAsPoint2D() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vertexDataAsColoredPoint2D()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vertexDataAsColoredPoint2D() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vertexDataAsTexturedPoint2D()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vertexDataAsTexturedPoint2D() const"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "indexData()"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.nio.Buffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %env->NewDirectByteBuffer(%in, __qt_this->sizeOfIndex()*__qt_this->indexCount());\n"+
                                  "switch(__qt_this->sizeOfIndex()){\n"+
                                  "case 4:\n"+
                                  "    %out = Java::Runtime::ByteBuffer::asIntBuffer(%env,%out);\n"+
                                  "    break;\n"+
                                  "case 2:\n"+
                                  "    %out = Java::Runtime::ByteBuffer::asShortBuffer(%env,%out);\n"+
                                  "    break;\n"+
                                  "default:\n"+
                                  "    break;\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "vertexData()"
            rename: "vertexData_private"
            access: Modification.Private
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "long"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = jlong(%in);"}
                }
            }
        }
        ModifyFunction{
            signature: "attributes()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGGeometry$Attribute[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::toJObjectArray<QSGGeometry::Attribute>(%env, %in, __qt_this->attributeCount(),\n"+
                                  "                                                        [](JNIEnv * env, const QSGGeometry::Attribute& item)->jobject{\n"+
                                  "                                                            return qtjambi_cast<jobject>(env, item);\n"+
                                  "                                                       });"}
                }
            }
        }
        ModifyFunction{
            signature: "defaultAttributes_Point2D()"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertNativeToJavaObject<QSGGeometry::AttributeSet>(%env, &%in, false);"}
                }
            }
        }
        ModifyFunction{
            signature: "defaultAttributes_ColoredPoint2D()"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertNativeToJavaObject<QSGGeometry::AttributeSet>(%env, &%in, false);"}
                }
            }
        }
        ModifyFunction{
            signature: "defaultAttributes_TexturedPoint2D()"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertNativeToJavaObject<QSGGeometry::AttributeSet>(%env, &%in, false);"}
                }
            }
        }
        ModifyFunction{
            signature: "QSGGeometry(const QSGGeometry::AttributeSet &, int, int, int)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcAttributeSet"
                    action: ReferenceCount.Set
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const QSGGeometry::AttributeSet& %out = QtJambiAPI::checkedAddressOf<QSGGeometry::AttributeSet>(%env, QtJambiAPI::convertJavaObjectToNative<QSGGeometry::AttributeSet>(%env, %in));"}
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQuick.java"
                quoteAfterLine: "class QSGGeometry__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QSGMaterial"
        ModifyFunction{
            signature: "compare(const QSGMaterial*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "createShader() const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "createShader(QSGRendererInterface::RenderMode) const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QSGMaterialShader"
        ModifyFunction{
            signature: "updateState(QSGMaterialShader::RenderState,QSGMaterial*,QSGMaterial*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
            until: 5
        }
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedArrayPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.ShellDeclaration
            position: Position.End
            Text{content: "    mutable QByteArray __qt_vertexShader;\n"+
                          "    mutable QByteArray __qt_fragmentShader;\n"+
                          "    mutable QByteArray __qt_attributeNameByteArrays;\n"+
                          "    mutable QVector<const char *> __qt_attributeNames;"}
        }
        ModifyFunction{
            signature: "attributeNames()const"
            InjectCode{
                target: CodeClass.Shell
                position: Position.Beginning
                Text{content: "if (__qt_attributeNames.size())\n"+
                              "    return __qt_attributeNames.constData();"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize size = 0;\n"+
                                  "while(%in[size]){\n"+
                                  "    ++size;\n"+
                                  "}\n"+
                                  "jobjectArray %out = Java::Runtime::String::newArray(%env, size);\n"+
                                  "for(jsize i = 0; i < size; i++){\n"+
                                  "    jstring element = qtjambi_cast<jstring>(%env, QLatin1String(%in[i]));\n"+
                                  "    %env->SetObjectArrayElement(%out, i, element);\n"+
                                  "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jsize length = %in ? %env->GetArrayLength(%in) : 0;\n"+
                                  "QList<QByteArray> names;\n"+
                                  "for(jsize i=0; i<length; i++){\n"+
                                  "    jstring element = jstring(__jni_env->GetObjectArrayElement(__java_return_value, i));\n"+
                                  "    names << qtjambi_cast<QByteArray>(__jni_env, element);\n"+
                                  "}\n"+
                                  "int total = 0;\n"+
                                  "for (int i=0; i<names.size(); ++i)\n"+
                                  "    total += names.at(i).size() + 1;\n"+
                                  "__qt_attributeNameByteArrays.reserve(total);\n"+
                                  "__qt_attributeNames.reserve(names.size()+1);\n"+
                                  "for (int i=0; i<names.size(); ++i) {\n"+
                                  "    __qt_attributeNames << __qt_attributeNameByteArrays.constData() + __qt_attributeNameByteArrays.size();\n"+
                                  "    __qt_attributeNameByteArrays.append(names.at(i));\n"+
                                  "    __qt_attributeNameByteArrays.append('\\0');\n"+
                                  "}\n"+
                                  "__qt_attributeNames << nullptr;\n"+
                                  "const char*const * %out = __qt_attributeNames.constData();"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "vertexShader()const"
            InjectCode{
                target: CodeClass.Shell
                position: Position.Beginning
                Text{content: "if (__qt_vertexShader.size())\n"+
                              "return __qt_vertexShader.constData();"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jstring %out = qtjambi_cast<jstring>(%env, QLatin1String(%in));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "const char * %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "    __qt_vertexShader = qtjambi_cast<QByteArray>(%env, %in);\n"+
                                  "    %out = __qt_vertexShader.constData();\n"+
                                  "}"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "fragmentShader()const"
            InjectCode{
                target: CodeClass.Shell
                position: Position.Beginning
                Text{content: "if (__qt_fragmentShader.size())\n"+
                              "return __qt_fragmentShader.constData();"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jstring %out = qtjambi_cast<jstring>(%env, QLatin1String(%in));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "const char * %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "    __qt_fragmentShader = qtjambi_cast<QByteArray>(%env, %in);\n"+
                                  "    %out = __qt_fragmentShader.constData();\n"+
                                  "}"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "updateGraphicsPipelineState(QSGMaterialShader::RenderState &, QSGMaterialShader::GraphicsPipelineState *, QSGMaterial *, QSGMaterial *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialShader$RenderState"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialShader::RenderState& %out = qtjambi_cast<QSGMaterialShader::RenderState&>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialShader$GraphicsPipelineState"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialShader::GraphicsPipelineState* %out = qtjambi_cast<QSGMaterialShader::GraphicsPipelineState*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 4
                invalidateAfterUse: true
            }
            since: 6
        }
        ModifyFunction{
            signature: "updateUniformData(QSGMaterialShader::RenderState &, QSGMaterial *, QSGMaterial *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialShader$RenderState"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialShader::RenderState& %out = qtjambi_cast<QSGMaterialShader::RenderState&>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
            since: 6
        }
        ModifyFunction{
            signature: "updateSampledImage(QSGMaterialShader::RenderState &, int, QSGTexture **, QSGMaterial *, QSGMaterial *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialShader$RenderState"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialShader::RenderState& %out = qtjambi_cast<QSGMaterialShader::RenderState&>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
                ArrayType{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 4
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 5
                invalidateAfterUse: true
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QSGOpaqueTextureMaterial"
        ModifyFunction{
            signature: "createShader() const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "createShader(QSGRendererInterface::RenderMode) const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setTexture(QSGTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    
    
    ObjectType{
        name: "QSGTexture"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            rename: "nativeInterface"
            access: Modification.Public
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends io.qt.QtObjectInterface> QNativeInterface"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObject(%env, %in, %1, false, false);\n"+
                                  "CoreAPI::registerDependency(%env, %out, __this_nativeId);"}
                }
            }
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Class<QNativeInterface>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "CoreAPI::NITypeInfo info = CoreAPI::getNativeInterfaceInfo(%env, %in);\n"+
                                  "const char* %out = info.name;"}
                }
            }
            ModifyArgument{
                index: 2
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out = info.revision;"}
                }
                RemoveArgument{
                }
            }
            since: [6, 2]
        }
    }
    
    ObjectType{
        name: "QSGTextureMaterial"
        ModifyFunction{
            signature: "createShader() const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "createShader(QSGRendererInterface::RenderMode) const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QSGVertexColorMaterial"
        ModifyFunction{
            signature: "createShader() const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "createShader(QSGRendererInterface::RenderMode) const"
            noExcept: true
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QQuickTransform"
        ModifyFunction{
            signature: "applyTo(QMatrix4x4 *)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.gui.QMatrix4x4"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QMatrix4x4* %out = qtjambi_cast<QMatrix4x4*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QSGGeometry::Attribute"
        ModifyFunction{
            signature: "Attribute()"
            remove: RemoveFlag.All
        }
        ModifyField{
            name: "position"
            read: true
            write: false
        }
        ModifyField{
            name: "tupleSize"
            read: true
            write: false
        }
        ModifyField{
            name: "type"
            read: true
            write: false
        }
        ModifyField{
            name: "attributeType"
            read: true
            write: false
        }
        ModifyField{
            name: "isVertexCoordinate"
            read: true
            write: false
            ReplaceType{
                modifiedType: "boolean"
            }
        }
        ModifyField{
            name: "reserved"
            read: false
            write: false
        }
    }
    
    ObjectType{
        name: "QSGGeometry::AttributeSet"
        InjectCode{
            target: CodeClass.DestructorFunction
            Text{content: "delete[] %this->attributes;"}
        }
        ModifyFunction{
            signature: "AttributeSet()"
            AddArgument{
                name: "attributes"
                type: "io.qt.quick.QSGGeometry$Attribute[]"
            }
            AddArgument{
                name: "stride"
                type: "int"
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "__qt_this->stride = stride;\n"+
                              "__qt_this->count = %env->GetArrayLength(attributes);\n"+
                              "QSGGeometry::Attribute* _attributes = new QSGGeometry::Attribute[size_t(__qt_this->count)];\n"+
                              "__qt_this->attributes = _attributes;\n"+
                              "for(jsize i=0; i<jsize(__qt_this->count); ++i){\n"+
                              "    jobject element = %env->GetObjectArrayElement(attributes, i);\n"+
                              "    _attributes[i] = qtjambi_cast<const QSGGeometry::Attribute&>(%env, element);\n"+
                              "}"}
            }
        }
        ModifyField{
            name: "attributes"
            write: false
            ReplaceType{
                modifiedType: "io.qt.quick.QSGGeometry$Attribute[]"
            }
            ConversionRule{
                codeClass: CodeClass.NativeGetter
                Text{content: "%out = qtjambi_array_cast<jobjectArray>(%env, %scope, %in, __qt_this->count);"}
            }
        }
        ModifyField{
            name: "count"
            write: false
        }
        ModifyField{
            name: "stride"
            write: false
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQuick.java"
                quoteAfterLine: "class QSGGeometry.AttributeSet__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QSGGeometry::ColoredPoint2D"
    }
    
    ValueType{
        name: "QQuickItem::ItemChangeData"
        generate: false
        CustomConstructor{
            Text{content: "if(copy)\n"+
                          "    return new(placement) QQuickItem::ItemChangeData(*copy);\n"+
                          "else\n"+
                          "    return new(placement) QQuickItem::ItemChangeData(false);"}
        }
        ModifyField{
            name: "window"
            read: true
            write: false
        }
        ModifyField{
            name: "item"
            read: true
            write: false
        }
        ModifyField{
            name: "boolValue"
            read: true
            write: false
            rename: "booleanValue"
        }
        ModifyField{
            name: "realValue"
            read: true
            write: false
            rename: "doubleValue"
        }
    }
    
    ValueType{
        name: "QSGGeometry::Point2D"
    }
    
    ObjectType{
        name: "QQuickImageProvider"
        ModifyFunction{
            signature: "requestImage(const QString &, QSize *, const QSize &)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.core.QSize"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSize* %out = qtjambi_cast<QSize*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "requestPixmap(const QString &, QSize *, const QSize &)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.core.QSize"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSize* %out = qtjambi_cast<QSize*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "requestTexture(const QString &, QSize *, const QSize &)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.core.QSize"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSize* %out = qtjambi_cast<QSize*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QQuickAsyncImageProvider"
    }
    
    ObjectType{
        name: "QQuickImageResponse"
    }
    
    ObjectType{
        name: "QQuickItem::UpdatePaintNodeData"
        ExtraIncludes{
            Include{
                fileName: "QtQuick/QSGTransformNode"
                location: Include.Global
            }
        }
        ModifyField{
            name: "transformNode"
            read: true
            write: true
            ReferenceCount{
                action: ReferenceCount.Ignore
            }
        }
    }
    
    ObjectType{
        name: "QSGMaterialType"
    }
    
    ValueType{
        name: "QSGGeometry::TexturedPoint2D"
    }
    
    ObjectType{
        name: "QQuickItemGrabResult"
        ModifyFunction{
            signature: "saveToFile(const QString &)"
            remove: RemoveFlag.All
            since: [5, 9]
            until: 5
        }
    }
    
    ObjectType{
        name: "QSGAbstractRenderer"
        ModifyFunction{
            signature: "nodeChanged(QSGNode*,QFlags<QSGNode::DirtyStateBit>)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setRootNode(QSGRootNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRootNode"
                    action: ReferenceCount.Set
                }
            }
        }
        until: 5
    }
    
    EnumType{
        name: "QSGAbstractRenderer::ClearModeBit"
        flags: "QSGAbstractRenderer::ClearMode"
        until: 5
    }
    
    EnumType{
        name: "QSGAbstractRenderer::MatrixTransformFlag"
        flags: "QSGAbstractRenderer::MatrixTransformFlags"
        since: [5, 14]
        until: 5
    }
    
    ObjectType{
        name: "QSGEngine"
        ModifyFunction{
            signature: "createImageNode()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createRectangleNode()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createNinePatchNode()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createRenderer()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createTextureFromImage(QImage,QFlags<QSGEngine::CreateTextureOption>)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createTextureFromId(uint,QSize,QFlags<QSGEngine::CreateTextureOption>)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        until: 5
    }
    
    EnumType{
        name: "QSGEngine::CreateTextureOption"
        flags: "QSGEngine::CreateTextureOptions"
        until: 5
    }
    
    Rejection{
        className: "QSGSimpleMaterial"
    }
    
    Rejection{
        className: "QSGSimpleMaterialComparableMaterial"
    }
    
    Rejection{
        className: "QSGSimpleMaterialShader"
    }
    
    ObjectType{
        name: "QSGTextureProvider"
    }
    
    ObjectType{
        name: "QSGNodeVisitor"
        ModifyFunction{
            signature: "enterClipNode(QSGClipNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "enterGeometryNode(QSGGeometryNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "enterOpacityNode(QSGOpacityNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "enterTransformNode(QSGTransformNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "leaveClipNode(QSGClipNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "leaveGeometryNode(QSGGeometryNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "leaveOpacityNode(QSGOpacityNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "leaveTransformNode(QSGTransformNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "visitChildren(QSGNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "visitNode(QSGNode*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ValueType{
        name: "QQuickGraphicsDevice"
        ModifyFunction{
            signature: "operator=(QQuickGraphicsDevice)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "fromAdapter(unsigned int, int, int)"
            ppCondition: "defined(Q_OS_WIN)"
        }
        since: 6
    }
    
    ValueType{
        name: "QQuickRenderTarget"
        ModifyFunction{
            signature: "operator=(QQuickRenderTarget)"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            since: [6, 2]
            Text{content: "QQuickRenderTarget qtjambi_QQuickRenderTarget_fromVulkanImage(JNIEnv *env, jlong image, jint layout, const QSize& pixelSize, int sampleCount);"}
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            since: [6, 4]
            Text{content: "QQuickRenderTarget qtjambi_QQuickRenderTarget_fromVulkanImage(JNIEnv *env, jlong image, jint layout, jint format, const QSize& pixelSize, int sampleCount);"}
        }
        ModifyFunction{
            signature: "fromD3D11Texture(void *, QSize, int)"
            ppCondition: "defined(Q_OS_WIN)"
            since: [6, 2]
        }
        ModifyFunction{
            signature: "fromD3D11Texture(void *, unsigned int, QSize, int)"
            ppCondition: "defined(Q_OS_WIN)"
            since: [6, 4]
        }
        ModifyFunction{
            signature: "fromMetalTexture(MTLTexture *, QSize, int)"
            ppCondition: "defined(Q_OS_MACOS) || defined(Q_OS_IOS)"
            since: [6, 2]
        }
        ModifyFunction{
            signature: "fromMetalTexture(MTLTexture *, unsigned int, QSize, int)"
            ppCondition: "defined(Q_OS_MACOS) || defined(Q_OS_IOS)"
            since: [6, 4]
        }
        ModifyFunction{
            signature: "fromVulkanImage(VkImage, VkImageLayout, QSize, int)"
            ppCondition: "QT_CONFIG(vulkan)"
            proxyCall: "qtjambi_QQuickRenderTarget_fromVulkanImage"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "long"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "#ifdef %out\n"+
                                  "#undef %out\n"+
                                  "#endif\n"+
                                  "#define %out %in"}
                }
            }
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "#ifdef %out\n"+
                                  "#undef %out\n"+
                                  "#endif\n"+
                                  "#define %out %in"}
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "fromVulkanImage(VkImage, VkImageLayout, VkFormat, QSize, int)"
            ppCondition: "QT_CONFIG(vulkan)"
            proxyCall: "qtjambi_QQuickRenderTarget_fromVulkanImage"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "long"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "#ifdef %out\n"+
                                  "#undef %out\n"+
                                  "#endif\n"+
                                  "#define %out %in"}
                }
            }
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "#ifdef %out\n"+
                                  "#undef %out\n"+
                                  "#endif\n"+
                                  "#define %out %in"}
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "#ifdef %out\n"+
                                  "#undef %out\n"+
                                  "#endif\n"+
                                  "#define %out %in"}
                }
            }
            since: [6, 4]
        }
        since: 6
    }
    
    ObjectType{
        name: "QSGMaterialRhiShader"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.ShellDeclaration
            position: Position.End
            Text{content: "    mutable QByteArray __qt_vertexShader;\n"+
                          "    mutable QByteArray __qt_fragmentShader;\n"+
                          "    mutable QByteArray __qt_attributeNameByteArrays;\n"+
                          "    mutable QVector<const char *> __qt_attributeNames;"}
        }
        ModifyFunction{
            signature: "updateGraphicsPipelineState(QSGMaterialRhiShader::RenderState &, QSGMaterialRhiShader::GraphicsPipelineState *, QSGMaterial *, QSGMaterial *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialRhiShader$RenderState"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialRhiShader::RenderState& %out = qtjambi_cast<QSGMaterialRhiShader::RenderState&>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialRhiShader$GraphicsPipelineState"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialRhiShader::GraphicsPipelineState* %out = qtjambi_cast<QSGMaterialRhiShader::GraphicsPipelineState*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 4
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "updateSampledImage(QSGMaterialRhiShader::RenderState &, int, QSGTexture **, QSGMaterial *, QSGMaterial *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialRhiShader$RenderState"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialRhiShader::RenderState& %out = qtjambi_cast<QSGMaterialRhiShader::RenderState&>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 4
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
                NoNullPointer{
                }
                ArrayType{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 5
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "updateUniformData(QSGMaterialRhiShader::RenderState &, QSGMaterial *, QSGMaterial *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGMaterialRhiShader$RenderState"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSGMaterialRhiShader::RenderState& %out = qtjambi_cast<QSGMaterialRhiShader::RenderState&>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
        since: [5, 14]
        until: 5
    }
    
    EnumType{
        name: "QSGMaterialRhiShader::Flag"
        flags: "QSGMaterialRhiShader::Flags"
        since: [5, 14]
        until: 5
    }
    
    EnumType{
        name: "QSGMaterialRhiShader::Stage"
        since: [5, 14]
        until: 5
    }
    
    ValueType{
        name: "QSGMaterialRhiShader::RenderState"
        ModifyFunction{
            signature: "uniformData()"
            remove: RemoveFlag.All
        }
        since: [5, 14]
        until: 5
    }
    
    ValueType{
        name: "QSGMaterialRhiShader::GraphicsPipelineState"
        since: [5, 14]
        until: 5
    }
    
    EnumType{
        name: "QSGMaterialRhiShader::GraphicsPipelineState::BlendFactor"
        since: [5, 14]
        until: 5
    }
    
    EnumType{
        name: "QSGMaterialRhiShader::GraphicsPipelineState::ColorMaskComponent"
        flags: "QSGMaterialRhiShader::GraphicsPipelineState::ColorMask"
        since: [5, 14]
        until: 5
    }
    
    EnumType{
        name: "QSGMaterialRhiShader::GraphicsPipelineState::CullMode"
        since: [5, 14]
        until: 5
    }
    
    EnumType{
        name: "QSGMaterialShader::Flag"
        flags: "QSGMaterialShader::Flags"
        since: 6
    }
    
    EnumType{
        name: "QSGMaterialShader::Stage"
        since: 6
    }
    
    ValueType{
        name: "QSGMaterialShader::RenderState"
        ModifyFunction{
            signature: "uniformData()"
            remove: RemoveFlag.All
            since: 6
        }
    }
    
    ValueType{
        name: "QSGMaterialShader::GraphicsPipelineState"
        since: 6
    }
    
    ValueType{
        name: "QQuickGraphicsConfiguration"
        ModifyFunction{
            signature: "operator=(QQuickGraphicsConfiguration)"
            remove: RemoveFlag.All
        }
        since: 6
    }
    
    EnumType{
        name: "QSGMaterialShader::GraphicsPipelineState::BlendFactor"
        since: 6
    }
    
    EnumType{
        name: "QSGMaterialShader::GraphicsPipelineState::PolygonMode"
        since: [6, 4]
    }
    
    EnumType{
        name: "QSGMaterialShader::GraphicsPipelineState::ColorMaskComponent"
        flags: "QSGMaterialShader::GraphicsPipelineState::ColorMask"
        since: 6
    }
    
    EnumType{
        name: "QSGMaterialShader::GraphicsPipelineState::CullMode"
        since: 6
    }
    
    Rejection{
        className: "QNativeInterface::QSGOpenGLTexture::TypeInfo"
    }
    
    InterfaceType{
        name: "QNativeInterface::QSGOpenGLTexture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGOpenGLTexture"
        ppCondition: "QT_CONFIG(opengl)"
        isNativeInterface: true
        generate: "no-shell"
        ModifyFunction{
            signature: "QSGOpenGLTexture()"
            remove: RemoveFlag.All
        }
        since: [6, 2]
    }
    
    Rejection{
        className: "QNativeInterface::QSGD3D11Texture::TypeInfo"
    }
    
    InterfaceType{
        name: "QNativeInterface::QSGD3D11Texture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGD3D11Texture"
        ppCondition: "defined(Q_OS_WIN)"
        isNativeInterface: true
        generate: "no-shell"
        ModifyFunction{
            signature: "QSGD3D11Texture()"
            remove: RemoveFlag.All
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "VkImage"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "VkImageLayout"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "VkFormat"
        generate: false
        since: [6, 4]
    }
    
    ValueType{
        name: "MTLTexture"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "MTLDevice"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "MTLCommandQueue"
        generate: false
        since: [6, 2]
    }
    
    ObjectType{
        name: "id"
        template: true
        TemplateArguments{
            arguments: ["MTLTexture"]
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "id<MTLTexture>"
        generate: false
        since: [6, 2]
    }
    
    Rejection{
        className: "QNativeInterface::QSGMetalTexture::TypeInfo"
    }
    
    InterfaceType{
        name: "QNativeInterface::QSGMetalTexture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGMetalTexture"
        ppCondition: "defined(Q_OS_MACOS)"
        isNativeInterface: true
        generate: "no-shell"
        ModifyFunction{
            signature: "QSGMetalTexture()"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "#if defined(Q_OS_MACOS)\n"+
                          "namespace QNativeInterface {\n"+
                          "    struct QSGMetalTexture{\n"+
                          "        QT_DECLARE_NATIVE_INTERFACE(QSGMetalTexture, 1, QSGTexture)\n"+
                          "    };\n"+
                          "}\n"+
                          "\n"+
                          "void* qtjambi_QSGMetalTexture_nativeTexture(JNIEnv *env, const void* ptr);\n"+
                          "QSGTexture * qtjambi_QSGMetalTexture_fromNative(JNIEnv *env, void* texture, QQuickWindow* window, const QSize& size, QQuickWindow::CreateTextureOptions options);\n"+
                          "#endif // defined(Q_OS_MACOS)"}
        }
        ModifyFunction{
            signature: "nativeTexture() const"
            proxyCall: "qtjambi_QSGMetalTexture_nativeTexture"
        }
        ModifyFunction{
            signature: "fromNative(id<MTLTexture>, QQuickWindow *, QSize, QFlags<QQuickWindow::CreateTextureOption>)"
            proxyCall: "qtjambi_QSGMetalTexture_fromNative"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "void* %out = QtJambiAPI::convertQNativePointerToNative(%env, %in, 0);"}
                }
            }
        }
        since: [6, 2]
    }
    
    Rejection{
        className: "QNativeInterface::QSGVulkanTexture::TypeInfo"
    }
    
    InterfaceType{
        name: "QNativeInterface::QSGVulkanTexture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGVulkanTexture"
        ppCondition: "QT_CONFIG(vulkan)"
        isNativeInterface: true
        generate: "no-shell"
        ModifyFunction{
            signature: "QSGVulkanTexture()"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "QSGTexture * qtjambi_QSGVulkanTexture_fromNative(JNIEnv *env, jlong image, jint layout, QQuickWindow* window, const QSize& size, QQuickWindow::CreateTextureOptions options);"}
        }
        ModifyFunction{
            signature: "fromNative(VkImage, VkImageLayout, QQuickWindow *, QSize, QFlags<QQuickWindow::CreateTextureOption>)"
            proxyCall: "qtjambi_QSGVulkanTexture_fromNative"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "long"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "#ifdef %out\n"+
                                  "#undef %out\n"+
                                  "#endif\n"+
                                  "#define %out %in"}
                }
            }
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "#ifdef %out\n"+
                                  "#undef %out\n"+
                                  "#endif\n"+
                                  "#define %out %in"}
                }
            }
        }
        ModifyFunction{
            signature: "nativeImage() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "long"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = jlong(%in);"}
                }
            }
        }
        ModifyFunction{
            signature: "nativeImageLayout() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = jint(%in);"}
                }
            }
        }
        since: [6, 2]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type 'Vk*'"}
}
